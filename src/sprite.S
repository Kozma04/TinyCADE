.extern inv_shift_lut
.extern oled_framebuffer

.global get_pixel_1bpp
.global get_pixel_1bpp_progmem

; r24 = sprite pointer
; r22 = x
; r20 = y
get_pixel_1bpp:
    push r0
    push YL
    push YH

    mov r19, r22
    mov r21, r22
    andi r19, 7
    ;brne get_pixel_1bpp_add8
    ;subi r22, -8
get_pixel_1bpp_add8:

    ldi ZL, lo8(inv_shift_lut)
    ldi ZH, hi8(inv_shift_lut)
    add ZL, r19 ; inv_shift_lut % 8 = 0
    ld r19, Z
    
    mov YL, r24
    mov YH, r25
    ld ZL, Y
    ldd ZH, Y+1
    ldd r23, Y+3 ; width / 8
    mul r23, r20 ; pos = (width / 8) * y
    lsr r22
    lsr r22
    lsr r22
    add r0, r22
    clr r22
    adc r1, r22
    
    add ZL, r0
    adc ZH, r1
    ld r20, Z
    and r20, r19
    breq get_pixel_1bpp_0
get_pixel_1bpp_1:
    ldi r24, 1
    clr r1
    pop YH
    pop YL
    pop r0
    ret
get_pixel_1bpp_0:
    ldi r24, 0
    clr r1
    pop YH
    pop YL
    pop r0
    ret


; r24 = sprite pointer
; r22 = x
; r20 = y
; r18 = rampZ
get_pixel_1bpp_progmem:
    push r0
    push YL
    push YH

    mov r19, r22
    mov r21, r22
    andi r19, 7
    ;brne get_pixel_1bpp_add8
    ;subi r22, -8
get_pixel_1bpp_progmem_add8:

    ldi ZL, lo8(inv_shift_lut)
    ldi ZH, hi8(inv_shift_lut)
    add ZL, r19 ; inv_shift_lut % 8 = 0
    ld r19, Z
    
    mov YL, r24
    mov YH, r25
    ld ZL, Y
    ldd ZH, Y+1
    ldd r23, Y+3 ; width / 8
    mul r23, r20 ; pos = (width / 8) * y
    lsr r22
    lsr r22
    lsr r22
    add r0, r22
    clr r22
    adc r1, r22
    
    add ZL, r0
    adc ZH, r1
    ;lsl ZL
    ;rol ZH
    out 0x3B, r18 ; RAMPZ
    elpm r20, Z
    and r20, r19
    breq get_pixel_1bpp_progmem_0
get_pixel_1bpp_progmem_1:
    ldi r24, 1
    clr r1
    pop YH
    pop YL
    pop r0
    ret
get_pixel_1bpp_progmem_0:
    ldi r24, 0
    clr r1
    pop YH
    pop YL
    pop r0
    ret