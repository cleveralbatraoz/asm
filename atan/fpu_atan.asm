; x - x^3/3 + x^5/5 - x^7/7 ...
; float fpu_atan(float x, uint32_t n)

        global fpu_atan

        section .text

fpu_atan:
        push ebp
        mov ebp, esp

        sub esp, 4

        mov ecx, [ebp + 12]

        fld dword [ebp + 8]            ; st0 = x
        fmul st0, st0                  ; x *= x
        fchs                           ; x = -x
        fstp dword [ebp - 4]           ; push -x^2 to CPU stack [epb - 4]

        fldz                           ; push(0)

        fld dword [ebp + 8]            ; push(x)

        fld1                           ; push(1)

        fld dword [ebp - 4]            ; push(-x^2)

; st0 = -x^2
; st1 = 1
; st2 = x
; st3 = 0 (acc)

        fxch st2                       ; swap(st0, st2)

; st0 = x
; st1 = 1
; st2 = -x^2
; st3 = 0 (acc)

; st0 = term
; st1 = denominator
; st2 = const -x^2
; st3 = acc

        test ecx, ecx
        jz done

loop:
        ; result += current_term / denominator
        fld st0                       ; copy term
        fdiv st0, st2                  ; term / denominator (denom is at st2 after fld)
        fadd st4, st0                  ; acc += term / denominator (acc is at st4 after fld)
        fstp st0                       ; pop

        ; current_term *= multiplier
        fmul st0, st2                  ; term *= -x^2

        ; denominator += 2
        fxch st1                      ; term <-> denominator
        fld1                          ; push(1)
        fadd st0, st0                 ; st0 = 2
        fadd st1, st0                 ; denominator += 2
        fstp st0                      ; pop
        fxch st1                      ; term <-> denominator

        dec ecx
        jnz loop

done:
        fxch st3
        fstp dword [ebp - 4]
        fstp st0
        fstp st0
        fstp st0
        fld dword [ebp - 4]

        mov esp, ebp
        pop ebp
        ret

        section .note.GNU-stack
