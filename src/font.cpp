#include "font.h"

font_t *oled_current_font;
fontcfg_t oled_fontcfg;
uint8_t oled_cursor_x = 0;
uint8_t oled_cursor_y = 0;

void oled_set_cursor(uint8_t x, uint8_t y) {
    oled_cursor_x = x;
    oled_cursor_y = y;
}

void oled_draw_char(unsigned char c, uint8_t x, uint8_t y) {
    if(c < oled_current_font->char_start || c > oled_current_font->char_end)
        return;

    uint16_t pos, fx, fy;
    c -= oled_current_font->char_start;
    pos = uint16_t(c) * oled_current_font->char_width;
    fx = (pos % oled_current_font->sprite->width);
    fy = pos / oled_current_font->sprite->width * oled_current_font->char_height;

    if(oled_fontcfg.back_color == 16) {
        if(oled_fontcfg.draw_shadow) {
            draw_sprite_1bpp_alpha_progmem(
                oled_current_font->sprite, oled_current_font->sprite,
                oled_cursor_x + 1, oled_cursor_y + 1,
                fx, fy, oled_current_font->char_width, oled_current_font->char_height,
                oled_fontcfg.shadow_color << 4
            );
        }
        draw_sprite_1bpp_alpha_progmem(
            oled_current_font->sprite, oled_current_font->sprite,
            oled_cursor_x, oled_cursor_y,
            fx, fy, oled_current_font->char_width, oled_current_font->char_height,
            oled_fontcfg.color << 4
        );
    }
    else {
        draw_sprite_1bpp_progmem(
            oled_current_font->sprite,
            oled_cursor_x, oled_cursor_y,
            fx, fy, oled_current_font->char_width, oled_current_font->char_height,
            (oled_fontcfg.color << 4) | oled_fontcfg.back_color
        );
    }
}

void oled_print_char(unsigned char c, FILE *stream) {
    if(oled_cursor_y > 63 - oled_current_font->char_height) return;
    if(c == '\n') oled_cursor_x = 0, oled_cursor_y += oled_current_font->char_height + oled_fontcfg.letter_spacing;
    else if(c == '\r') oled_cursor_x = 0;
    else {
        oled_draw_char(c, oled_cursor_x, oled_cursor_y);

        oled_cursor_x += oled_current_font->char_width + oled_fontcfg.letter_spacing;
        if(oled_cursor_x > 127 - oled_current_font->char_width) {
            oled_cursor_x = 0;
            oled_cursor_y += oled_current_font->char_height + oled_fontcfg.letter_spacing;
        }
    }
}

void oled_print_str(unsigned char *str) {
    while(*str) oled_print_char(*(str++), 0);
}