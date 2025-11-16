        section .text

        global is_power_of_two
        global decode_table_init
        global decode_table_contains
        global decode_table_append
        global decode_table_get_first_byte
        global decode_table_write_bytes
        global reader_init
        global reader_next
        global reader_has_next

; TODO: maybe remove at all
; bool is_power_of_two(uint32_t value)
is_power_of_two:
        mov eax, [esp+4]
        lea edx, [eax-1]
        and eax, edx
        setz al
        movzx eax, al
        ret

; void decode_table_init(struct decode_table *table)
; struct decode_table layout:
;   offset 0: uint32_t entries[4096] (16384 bytes)
;   offset 16384: int16_t next_code (2 bytes)
decode_table_init:
        mov edx, [esp+4]               ; edx = table

        ; Initialize first 256 entries with ENTRY_PACK(0, 0, 1)
        ; ENTRY_PACK(0, 0, 1) = (0 & 0xFFF) | ((0 & 0xFF) << 12) | ((1 & 0x1) << 20)
        ;                     = 0x00100000
        mov eax, 0x00100000
        mov ecx, 256
        mov edi, edx
.loop1:
        mov [edi], eax
        add edi, 4
        dec ecx
        jnz .loop1

        ; Initialize remaining entries (256 to 4095) with 0
        xor eax, eax
        mov ecx, 4096 - 256
.loop2:
        mov [edi], eax
        add edi, 4
        dec ecx
        jnz .loop2

        ; Set next_code = FIRST_CODE (258)
        mov word [edx+16384], 258

        ret

; bool decode_table_contains(struct decode_table const *table, uint16_t code)
decode_table_contains:
        mov edx, [esp+4]               ; edx = table
        movzx eax, word [esp+8]        ; eax = code
        mov eax, [edx+eax*4]           ; eax = table->entries[code]
        shr eax, 20                    ; eax >>= 20 (extract has_value bit)
        and eax, 1                     ; eax &= 1
        ret

; void decode_table_append(struct decode_table *table, int16_t code, uint8_t byte)
decode_table_append:
        mov edx, [esp+4]               ; edx = table
        movsx eax, word [esp+8]        ; eax = code (sign-extended)
        movzx ecx, byte [esp+12]       ; ecx = byte

        ; ENTRY_PACK(code, byte, 1) = (code & 0xFFF) | ((byte & 0xFF) << 12) | (1 << 20)
        and eax, 0xFFF                 ; eax = code & 0xFFF
        shl ecx, 12                    ; ecx = byte << 12
        or eax, ecx                    ; eax |= ecx
        or eax, 0x00100000             ; eax |= (1 << 20)

        ; table->entries[table->next_code] = packed_entry
        movzx ecx, word [edx+16384]    ; ecx = table->next_code
        mov [edx+ecx*4], eax           ; table->entries[next_code] = eax

        ; ++table->next_code
        inc word [edx+16384]

        ret

; uint8_t decode_table_get_first_byte(struct decode_table const *table, uint16_t code)
decode_table_get_first_byte:
        mov edx, [esp+4]               ; edx = table
        movzx eax, word [esp+8]        ; eax = code

.loop:
        cmp eax, 258                   ; FIRST_CODE = 258
        jb .done

        mov ecx, [edx+eax*4]           ; ecx = table->entries[code]
        and ecx, 0xFFF                 ; ecx = ENTRY_GET_PREVIOUS_CODE(entry)
        mov eax, ecx                   ; code = ecx
        jmp .loop

.done:
        ret

; uint16_t decode_table_write_bytes(uint8_t *w, uint16_t code, struct decode_table const *table)
decode_table_write_bytes:
        push ebx
        push esi
        push edi

        mov esi, [esp+16]              ; esi = w
        movzx edi, word [esp+20]       ; edi = code
        mov ebx, [esp+24]              ; ebx = table

        cmp edi, 258                   ; FIRST_CODE = 258
        jae .recursive

        ; Base case: code < FIRST_CODE
        mov eax, edi                   ; eax = code
        mov byte [esi], al             ; *w = code (low byte)
        mov eax, 1                     ; return 1
        pop edi
        pop esi
        pop ebx
        ret

.recursive:
        ; uint32_t entry = table->entries[code]
        mov eax, [ebx+edi*4]           ; eax = table->entries[code]

        ; ENTRY_GET_PREVIOUS_CODE(entry)
        and eax, 0xFFF                 ; eax = previous_code

        ; Recursive call: decode_table_write_bytes(w, previous_code, table)
        push ebx                       ; Push table
        push eax                       ; Push previous_code (already in eax)
        push esi                       ; Push w
        call decode_table_write_bytes
        add esp, 12                    ; Clean up stack

        ; eax now contains bytes returned from recursion
        ; w[bytes] = ENTRY_GET_BYTE(entry)
        mov ecx, [ebx+edi*4]           ; ecx = table->entries[code]
        shr ecx, 12                    ; ecx >>= 12
        and ecx, 0xFF                  ; ecx &= 0xFF (extract byte)
        mov [esi+eax], cl              ; w[bytes] = cl

        ; return bytes + 1
        inc eax

        pop edi
        pop esi
        pop ebx
        ret

; void reader_init(struct reader *r, uint8_t const *data, size_t size)
; struct reader layout (packed):
;   offset 0: uint8_t const *data (4 bytes)
;   offset 4: size_t size (4 bytes)
;   offset 8: size_t byte_index (4 bytes)
;   offset 12: uint8_t bit_index (1 byte)
reader_init:
        mov eax, [esp+4]               ; eax = r (pointer to struct reader)
        mov edx, [esp+8]               ; edx = data
        mov ecx, [esp+12]              ; ecx = size

        mov [eax], edx                 ; r->data = data
        mov [eax+4], ecx               ; r->size = size
        mov dword [eax+8], 0           ; r->byte_index = 0
        mov byte [eax+12], 0           ; r->bit_index = 0

        ret

; uint16_t reader_next(struct reader *r, uint8_t bits_count)
reader_next:
        push ebx
        mov edx, [esp+8]               ; edx = r
        mov eax, [edx]                 ; eax = r->data
        add eax, [edx+8]               ; eax += r->byte_index
        mov ebx, [eax]                 ; Load 4 bytes
        bswap ebx                      ; Convert to big-endian
        movzx ecx, byte [edx+12]       ; ecx = r->bit_index
        shl ebx, cl                    ; Shift left by bit_index
        mov cl, [esp+12]               ; cl = bits_count
        mov eax, 32
        sub eax, ecx                   ; eax = 32 - bits_count
        mov cl, al
        shr ebx, cl                    ; Extract bits from top

        mov cl, [esp+12]               ; cl = bits_count
        add cl, [edx+12]               ; cl += r->bit_index
        movzx eax, cl
        shr eax, 3                     ; eax /= 8
        add [edx+8], eax               ; r->byte_index += eax
        and cl, 7                      ; cl %= 8
        mov [edx+12], cl               ; r->bit_index = cl

        movzx eax, bx                  ; Return uint16_t value
        pop ebx
        ret

; bool reader_has_next(struct reader const *r, uint8_t bits_count)
reader_has_next:
        mov edx, [esp+4]               ; edx = r (pointer to struct reader)
        movzx eax, byte [esp+8]        ; eax = bits_count

        add al, [edx+12]               ; al = r->bit_index + bits_count
        add eax, 7                     ; eax = r->bit_index + bits_count + 7
        shr eax, 3                     ; eax = (r->bit_index + bits_count + 7) / 8
        add eax, [edx+8]               ; eax = r->byte_index + result

        cmp eax, [edx+4]               ; Compare with r->size
        setle al
        movzx eax, al
        ret
