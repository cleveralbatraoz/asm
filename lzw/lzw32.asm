        section .text

        global is_power_of_two
        global reader_init
        global reader_next
        global reader_has_next

; bool is_power_of_two(uint32_t value)
is_power_of_two:
        mov eax, [esp+4]
        lea edx, [eax-1]
        and eax, edx
        setz al
        movzx eax, al
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
