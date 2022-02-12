#pragma once

#include "oled.h"

#include <stdint.h>

struct sprite_t {
    uint8_t *data;
    uint8_t width;
    uint8_t width_d8;
    uint8_t height;
};

extern "C" {
    extern uint8_t __attribute__((aligned(8))) inv_shift_lut[8];

    extern uint8_t get_pixel_1bpp(sprite_t* spr, uint8_t x, uint8_t y);
    extern uint8_t get_pixel_1bpp_progmem(sprite_t* spr, uint8_t x, uint8_t y, bool rampZ = false);

    extern void draw_sprite_4bpp_internal(sprite_t *spr, int16_t screenX, int16_t screenY, uint16_t sprXY, uint16_t sprWH);
    extern void draw_sprite_4bpp_alpha_internal(sprite_t *spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint16_t sprXY, uint16_t sprWH);
}


extern uint8_t __attribute__((optimize("O3"), always_inline)) get_pixel_4bpp(sprite_t* spr, uint8_t x, uint8_t y);
extern uint8_t __attribute__((optimize("O3"), always_inline)) get_pixel_4bpp_progmem(sprite_t* spr, uint8_t x, uint8_t y);

extern void __attribute__((optimize("O3"))) draw_sprite_1bpp_int(sprite_t* spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal);
extern void __attribute__((optimize("O3"))) draw_sprite_1bpp_alpha_int(sprite_t* spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal);
extern void __attribute__((optimize("O3"))) draw_sprite_1bpp_progmem_int(sprite_t* spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal);
extern void __attribute__((optimize("O3"))) draw_sprite_1bpp_alpha_progmem_int(sprite_t* spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal);

extern void __attribute__((optimize("O3"))) draw_sprite_4bpp_int(sprite_t* spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH);
extern void __attribute__((optimize("O3"))) draw_sprite_4bpp_alpha_int(sprite_t *spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH);


extern void __attribute__((optimize("O3"))) draw_sprite_1bpp(sprite_t* spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal);
extern void __attribute__((optimize("O3"))) draw_sprite_1bpp_alpha(sprite_t* spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal);
extern void __attribute__((optimize("O3"))) draw_sprite_1bpp_progmem(sprite_t* spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal);
extern void __attribute__((optimize("O3"))) draw_sprite_1bpp_alpha_progmem(sprite_t* spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal);

extern void __attribute__((optimize("O3"))) draw_sprite_4bpp(sprite_t* spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH);
extern void __attribute__((optimize("O3"))) draw_sprite_4bpp_alpha(sprite_t *spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH);
