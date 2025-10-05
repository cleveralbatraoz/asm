        global store
        global divmod10
        global print_digits
        global negate
        global parse_format
        global bytes_to_dec
        global print

        section .text

negate:
        push ebp
        mov ebp, esp

        mov eax, [ebp+8]

        not dword [eax+0]
        not dword [eax+4]
        not dword [eax+8]
        not dword [eax+12]

        add dword [eax+0], 1
        adc dword [eax+4], 0
        adc dword [eax+8], 0
        adc dword [eax+12], 0

        pop ebp
        ret

divmod10:
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi

        mov edi, [ebp+8]
        mov esi, [ebp+12]

        xor edx, edx
        mov ebx, 10

        mov eax, [esi+12]
        div ebx
        mov [esi+12], eax

        mov eax, [esi+8]
        div ebx
        mov [esi+8], eax

        mov eax, [esi+4]
        div ebx
        mov [esi+4], eax

        mov eax, [esi+0]
        div ebx
        mov [esi+0], eax

        add dl, '0'
        mov [edi], dl

        mov eax, [esi]
        or eax, [esi+4]
        or eax, [esi+8]
        or eax, [esi+12]

        setz al
        movzx eax, al

        pop edi
        pop esi
        pop ebx
        pop ebp
        ret

print_digits:
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi

        sub esp, 72
        lea ebx, [ebp-64]
        mov dword [ebp-68], 0

divmod_loop:
        mov ecx, [ebp-68]
        lea eax, [ebx + ecx]

        push dword [ebp+12]
        push eax
        call divmod10
        add esp, 8

        inc dword [ebp-68]

        test eax, eax
        jz divmod_loop

        mov ecx, [ebp-68]
        mov edi, [ebp+8]

copy_loop:
        dec ecx
        mov al, [ebx + ecx]
        stosb
        test ecx, ecx
        jnz copy_loop

        mov eax, [ebp-68]

        add esp, 72
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret

parse_format:
        push ebp
        mov ebp, esp
        push esi
        push edi

        mov esi, [ebp+8]
        mov edi, [ebp+12]

        xor eax, eax
        mov [edi], eax
        mov [edi+4], eax

        test esi, esi
        jz ret

        cmp byte [esi], '%'
        jne loop
        inc esi

loop:
        movzx eax, byte [esi]

        cmp al, '-'
        je format_minus

        cmp al, '+'
        je format_plus

        cmp al, ' '
        je format_space

        cmp al, '0'
        je format_zero

        sub al, '0'
        cmp al, 9
        jbe digit
        jmp ret

format_zero:
        test byte [edi], 0x08
        jnz skip_zero_flag
        test byte [edi], 0x04
        jnz skip_zero_flag
        or byte [edi], 0x04
skip_zero_flag:
        inc esi
        jmp loop

format_plus:
        or byte [edi], 0x01
        and byte [edi], ~0x02
        inc esi
        jmp loop

format_minus:
        or byte [edi], 0x08
        and byte [edi], ~0x04
        inc esi
        jmp loop

format_space:
        test byte [edi], 0x01
        jnz next
        or byte [edi], 0x02

next:
        inc esi
        jmp loop

digit_zero:
        sub al, '0'

digit:
        inc esi
        mov ecx, [edi+4]
        lea ecx, [ecx + ecx*4]
        lea ecx, [ecx + ecx]
        movzx eax, al
        add ecx, eax
        mov [edi+4], ecx

width_loop:
        movzx eax, byte [esi]
        sub al, '0'
        cmp al, 9
        ja ret

        mov ecx, [edi+4]
        lea ecx, [ecx + ecx*4]
        lea ecx, [ecx + ecx]
        movzx eax, al
        add ecx, eax
        mov [edi+4], ecx
        inc esi
        jmp width_loop

ret:
        pop edi
        pop esi
        pop ebp
        ret

store:
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        sub esp, 48

        mov eax, [ebp+8]
        mov [ebp-44], eax
        mov esi, [ebp+12]

        mov edi, eax
        mov dword [edi], 0
        mov dword [edi+4], 0
        mov dword [edi+8], 0
        mov dword [edi+12], 0

        test esi, esi
        jz store_return

        mov dword [ebp-40], 0
        xor ebx, ebx

        mov al, [esi]
        cmp al, '-'
        jne store_after_sign
        mov dword [ebp-40], 1
        inc esi

store_after_sign:
        lea edi, [ebp-32]

        xor ecx, ecx
        mov edx, esi

store_len_loop:
        mov al, [edx]
        test al, al
        jz store_len_done
        inc edx
        inc ecx
        jmp store_len_loop

store_len_done:
        test ecx, 1
        jz store_process_pairs

        movzx eax, byte [esi]
        mov edx, eax
        and eax, 0xF
        shr edx, 6
        lea edx, [edx + edx*8]
        add eax, edx
        mov [edi + ebx], al
        inc ebx
        inc esi
        dec ecx

store_process_pairs:
        test ecx, ecx
        jz store_build_reversed

store_pair_loop:
        movzx eax, byte [esi]
        mov edx, eax
        and eax, 0xF
        shr edx, 6
        lea edx, [edx + edx*8]
        add eax, edx
        mov [ebp-12], eax

        movzx eax, byte [esi+1]
        mov edx, eax
        and eax, 0xF
        shr edx, 6
        lea edx, [edx + edx*8]
        add eax, edx
        mov [ebp-8], eax

        mov eax, [ebp-12]
        shl eax, 4
        or eax, [ebp-8]
        mov [edi + ebx], al

        inc ebx
        add esi, 2
        sub ecx, 2
        jnz store_pair_loop

store_build_reversed:
        mov ecx, ebx
        test ecx, ecx
        jz store_check_negative

        lea esi, [edi + ecx - 1]
        mov edx, [ebp-44]

store_copy_loop:
        mov al, [esi]
        mov [edx], al
        dec esi
        inc edx
        dec ecx
        jnz store_copy_loop

store_check_negative:
        cmp dword [ebp-40], 0
        jz store_return

        mov eax, [ebp-44]
        push eax
        call negate
        add esp, 4

store_return:
        xor eax, eax
        add esp, 48
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret

bytes_to_dec:
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        sub esp, 84

        mov edi, [ebp+16]
        mov al, [edi+15]
        and al, 0x80
        shr al, 7
        mov byte [ebp-81], al

        test al, al
        jz after_negate

        push edi
        call negate
        add esp, 4

after_negate:
        lea ebx, [ebp-64]
        push dword [ebp+16]
        push ebx
        call print_digits
        add esp, 8
        mov [ebp-68], eax

        mov esi, [ebp+12]
        movzx edx, byte [esi]
        xor ecx, ecx
        cmp byte [ebp-81], 0
        setne cl
        push edx
        and edx, 0x03
        setne dl
        or cl, dl
        add eax, ecx
        pop edx

        mov ecx, [esi+4]
        sub ecx, eax
        jnc no_underflow
        xor ecx, ecx

no_underflow:
        mov dword [ebp-72], 0
        mov dword [ebp-76], 0
        mov dword [ebp-80], 0

        test dl, 0x08
        jz not_left_align
        mov [ebp-80], ecx
        jmp padding_done

not_left_align:
        test dl, 0x04
        jz not_zero_pad
        mov [ebp-76], ecx
        jmp padding_done

not_zero_pad:
        mov [ebp-72], ecx

padding_done:
        mov edi, [ebp+8]
        mov ecx, [ebp-72]
        mov al, ' '
        rep stosb

        cmp byte [ebp-81], 0
        je check_plus
        mov al, '-'
        stosb
        jmp write_padding

check_plus:
        test dl, 0x01
        jz check_space
        mov al, '+'
        stosb
        jmp write_padding

check_space:
        test dl, 0x02
        jz write_padding
        mov al, ' '
        stosb

write_padding:
        mov ecx, [ebp-76]
        mov al, '0'
        rep stosb

        mov ecx, [ebp-68]
        mov esi, ebx

digit_loop:
        test ecx, ecx
        jz write_trailing
        mov al, [esi]
        stosb
        inc esi
        dec ecx
        jmp digit_loop

write_trailing:
        mov ecx, [ebp-80]
        mov al, ' '
        rep stosb
        mov byte [edi], 0

        xor eax, eax
        add esp, 84
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret

print:
        push ebp
        mov ebp, esp
        push ebx
        push edi
        push esi
        sub esp, 36

        lea eax, [ebp-20]
        push eax
        push dword [ebp+12]
        call parse_format
        add esp, 8

        lea eax, [ebp-36]
        push dword [ebp+16]
        push eax
        call store
        add esp, 8

        lea eax, [ebp-36]
        push eax
        lea eax, [ebp-20]
        push eax
        push dword [ebp+8]
        call bytes_to_dec
        add esp, 12
        add esp, 36
        pop esi
        pop edi
        pop ebx
        pop ebp
        ret

        section .note.GNU-stack
