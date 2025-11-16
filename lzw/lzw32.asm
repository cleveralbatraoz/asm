        section .text

        global is_power_of_two

; bool is_power_of_two(uint32_t value)
is_power_of_two:
        mov eax, [esp+4]
        lea edx, [eax-1]
        and eax, edx
        setz al
        movzx eax, al                  ; TODO: remove this
        ret
