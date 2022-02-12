.extern oled_framebuffer

.global oled_set_pixel_fast

oled_set_pixel_fast:
    ; r24 = x
    ; r22 = y
    ; r20 = val

    mov XL, r24
    mov XH, r22

    lsl XL ; r22:r24 = y * 256 + x * 2
    lsr XH
    ror XL ; r22:r24 = y * 128 + x
    lsr XH
    ror XL ; r22:r24 = y * 64 + x / 2

    subi XL, lo8(-(oled_framebuffer))
    sbci XH, hi8(-(oled_framebuffer))

    ; load value
    ld r25, X
    
    ; determine which nibble to modify
    andi r24, 1
    breq oled_set_pixel_fast_h

oled_set_pixel_fast_l:
    andi r25, 0xf0
    or r25, r20
    st X, r25
    ret

oled_set_pixel_fast_h:
    swap r20
    andi r25, 0x0f
    or r25, r20
    st X, r25
    ret