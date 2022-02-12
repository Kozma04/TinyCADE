#pragma once

#include <stdio.h>
#include "oled.h"
#include "sprite.h"

struct font_t {
    sprite_t *sprite; // 1bpp
    uint8_t char_width;
    uint8_t char_height;
    uint8_t char_start;
    uint8_t char_end;
};

struct fontcfg_t {
    uint8_t color = 15;
    uint8_t back_color = 16;
    uint8_t shadow_color = 1;
    uint8_t letter_spacing = 1;
    bool draw_shadow = true;
};

extern font_t *oled_current_font;
extern fontcfg_t oled_fontcfg;
extern uint8_t oled_cursor_x;
extern uint8_t oled_cursor_y;

extern void oled_set_cursor(uint8_t x, uint8_t y);
extern void oled_draw_char(unsigned char c, uint8_t x, uint8_t y);
extern void oled_print_char(unsigned char c, FILE *stream);
extern void oled_print_str(unsigned char* str);