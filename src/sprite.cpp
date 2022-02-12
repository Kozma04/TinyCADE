#include "sprite.h"

#define SHIFT_MASK()       \
    mask >>= 1;            \
    if(mask == 0) {        \
        mask = 0b10000000; \
        p_spr++;           \
        foo++;             \
    }
    
#define SHIFT_MASK_ALPHA() \
    mask >>= 1;            \
    if(mask == 0) {        \
        mask = 0b10000000; \
        p_spr++;           \
        p_spr_a++;         \
        foo++;             \
    }

#define SPR_COORDS_CHECK() \
    if(screenX + sprW < 0 || screenY + sprH < 0 || screenX >= 128 || screenY >= 64) \
        return; \
    if(screenX < 0) { \
        sprX -= screenX; \
        sprW += screenX; \
        screenX = 0; \
    } \
    if(screenX + sprW >= 128) { \
        sprW = 128 - screenX; \
    } \
    if(screenY < 0) { \
        sprY -= screenY; \
        sprH += screenY; \
        screenY = 0; \
    } \
    if(screenY + sprH >= 64) { \
        sprH = 64 - screenY; \
    } \

extern "C" {
    uint8_t inv_shift_lut[8] = {
        0b10000000,
        0b01000000,
        0b00100000,
        0b00010000,
        0b00001000,
        0b00000100,
        0b00000010,
        0b00000001
    };
}

uint8_t get_pixel_4bpp(sprite_t* spr, uint8_t x, uint8_t y) {
    uint8_t p = *(spr->data + ((uint16_t(y) * spr->width + x) >> 1));
    if(x & 1) return p & 0xf;
    return p >> 4;
}

uint8_t get_pixel_4bpp_progmem(sprite_t* spr, uint8_t x, uint8_t y) {
    uint8_t p = pgm_read_byte(spr->data + ((uint16_t(y) * spr->width + x) >> 1));
    if(x & 1) return p & 0xf;
    return p >> 4;
}

void draw_sprite_1bpp_int(sprite_t* spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal) {
    uint8_t *p_fb = oled_framebuffer + (screenY * 128 + screenX) / 2;
    uint8_t *p_spr = spr->data + (sprY * (spr->width_d8 + ((((sprW + sprX) & 7) || (((sprW + sprX) ^ 0xff) & 7)) ? 1 : 0)) + sprX / 8);
    uint8_t mask = inv_shift_lut[sprX & 7];
    uint8_t pal0 = sprPal & 0xf;
    uint8_t pal1 = sprPal >> 4;
    uint8_t out;
    uint8_t x;
    uint8_t foo;

    for(uint8_t y = 0; y < sprH; y++) {
        x = 0;
        foo = 0;
        if(screenX & 1) { // odd
            x++;
            if((*p_spr) & mask) out = pal1;
            else out = pal0;

            *p_fb = (*p_fb & 0xf0) | out;
            *p_fb++;

            SHIFT_MASK();
        }
        for(; x <= sprW - 2; x += 2) {
            if((*p_spr) & mask) out = pal1 << 4;
            else out = pal0 << 4;

            SHIFT_MASK();

            if((*p_spr) & mask) out |= pal1;
            else out |= pal0;

            SHIFT_MASK();

            *(p_fb++) = out;
        }
        if(x < sprW) {
            if((*p_spr) & mask) out = pal1 << 4;
            else out = pal0 << 4;

            *p_fb = (*p_fb & 0x0f) | out;
            if((x & 1) == 0) p_fb++;

            SHIFT_MASK();
        }
        p_fb += (128 - sprW) >> 1;
        p_spr += (spr->width_d8 - foo);
        if((sprW + sprX) & 7) p_spr++;
        else if(((sprW + sprX) ^ 0xff) & 7) p_spr++;
        mask = inv_shift_lut[sprX & 7];
    }
}

void draw_sprite_1bpp_alpha_int(sprite_t *spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal) {
    uint16_t ptr_add = (sprY * (spr->width_d8 + ((((sprW + sprX) & 7) || (((sprW + sprX) ^ 0xff) & 7)) ? 1 : 0)) + sprX / 8);
    uint8_t *p_fb = oled_framebuffer + (screenY * 128 + screenX) / 2;
    uint8_t *p_spr = spr->data + ptr_add;
    uint8_t *p_spr_a = alpha->data + ptr_add;
    uint8_t mask = inv_shift_lut[sprX & 7];
    uint8_t pal0 = sprPal & 0xf;
    uint8_t pal1 = sprPal >> 4;
    uint8_t out;
    uint8_t x;
    uint8_t na;
    uint8_t foo;

    for(uint8_t y = 0; y < sprH; y++) {
        x = 0;
        foo = 0;
        if(screenX & 1) { // odd
            x++;
            if(*p_spr_a & mask) {
                if((*p_spr) & mask) out = pal1;
                else out = pal0;

                *(p_fb++) = (*p_fb & 0xf0) | out;
            }
            else p_fb++;
            SHIFT_MASK_ALPHA();
        }
        for(; x <= sprW - 2; x += 2) {
            out = 0;
            na = 0;
            if(*p_spr_a & mask) {
                na = 0b10;
                if((*p_spr) & mask) out = pal1 << 4;
                else out = pal0 << 4;
            }

            SHIFT_MASK_ALPHA();

            if(*p_spr_a & mask) {
                na |= 1;
                if((*p_spr) & mask) out |= pal1;
                else out |= pal0;
            }

            SHIFT_MASK_ALPHA();

            if(na == 0b11) *(p_fb++) = out;
            else if(na == 0b10) *(p_fb++) = (*p_fb & 0xf) | out;
            else if(na == 0b1) *(p_fb++) = (*p_fb & 0xf0) | out;
            else p_fb++;
        }
        if(x < sprW) {
            if(*p_spr_a & mask) {
                if((*p_spr) & mask) out = pal1 << 4;
                else out = pal0 << 4;
                *p_fb = (*p_fb & 0x0f) | out;
            }
            
            if((x & 1) == 0) p_fb++;

            SHIFT_MASK_ALPHA();
        }
        p_fb += (128 - sprW) >> 1;
        p_spr += (spr->width_d8 - foo);
        p_spr_a += (spr->width_d8 - foo);

        if((sprW + sprX) & 7) p_spr++, p_spr_a++;
        else if(((sprW + sprX) ^ 0xff) & 7) p_spr++, p_spr_a++;

        mask = inv_shift_lut[sprX & 7];
    }
}

void draw_sprite_1bpp_progmem_int(sprite_t *spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal) {
    uint8_t *p_fb = oled_framebuffer + (screenY * 128 + screenX) / 2;
    uint8_t *p_spr = spr->data + (sprY * (spr->width_d8 + ((((sprW + sprX) & 7) || (((sprW + sprX) ^ 0xff) & 7)) ? 1 : 0)) + sprX / 8);
    uint8_t mask = inv_shift_lut[sprX & 7];
    uint8_t pal0 = sprPal & 0xf;
    uint8_t pal1 = sprPal >> 4;
    uint8_t out;
    uint8_t x;

    for(uint8_t y = 0; y < sprH; y++) {
        x = 0;
        uint8_t foo = 0;
        if(screenX & 1) { // odd
            x++;
            if(pgm_read_byte(p_spr) & mask) out = pal1;
            else out = pal0;

            *p_fb = (*p_fb & 0xf0) | out;
            p_fb++;

            SHIFT_MASK();
        }
        for(; x <= sprW - 2; x += 2) {
            if(pgm_read_byte(p_spr) & mask) out = pal1 << 4;
            else out = pal0 << 4;

            SHIFT_MASK();

            if(pgm_read_byte(p_spr) & mask) out |= pal1;
            else out |= pal0;

            SHIFT_MASK();

            *(p_fb++) = out;
        }
        if(x < sprW) {
            if(pgm_read_byte(p_spr) & mask) out = pal1 << 4;
            else out = pal0 << 4;

            *p_fb = (*p_fb & 0x0f) | out;
            if((x & 1) == 0) p_fb++;

            SHIFT_MASK();
        }
        p_fb += (128 - sprW) >> 1;
        p_spr += spr->width_d8 - foo;
        mask = inv_shift_lut[sprX & 7];
        if((sprW + sprX) & 7) p_spr++;
        else if(((sprW + sprX) ^ 0xff) & 7) p_spr++;
    }
}

void draw_sprite_1bpp_alpha_progmem_int(sprite_t *spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal) {
    uint16_t ptr_add = (sprY * (spr->width_d8 + ((((sprW + sprX) & 7) || (((sprW + sprX) ^ 0xff) & 7)) ? 1 : 0)) + sprX / 8);
    uint8_t *p_fb = oled_framebuffer + (screenY * 128 + screenX) / 2;
    uint8_t *p_spr = spr->data + ptr_add;
    uint8_t *p_spr_a = alpha->data + ptr_add;
    uint8_t mask = inv_shift_lut[sprX & 7];
    uint8_t pal0 = sprPal & 0xf;
    uint8_t pal1 = sprPal >> 4;
    uint8_t out;
    uint8_t x;
    uint8_t na;

    for(uint8_t y = 0; y < sprH; y++) {
        x = 0;
        uint8_t foo = 0;
        if(screenX & 1) { // odd
            x++;
            if(pgm_read_byte(p_spr_a) & mask) {
                if(pgm_read_byte(p_spr) & mask) out = pal1;
                else out = pal0;

                *(p_fb++) = (*p_fb & 0xf0) | out;
            }
            else p_fb++;

            SHIFT_MASK_ALPHA();
        }
        for(; x <= sprW - 2; x += 2) {
            out = 0;
            na = 0;
            if(pgm_read_byte(p_spr_a) & mask) {
                na = 0b10;
                if(pgm_read_byte(p_spr) & mask) out = pal1 << 4;
                else out = pal0 << 4;
            }

            SHIFT_MASK_ALPHA();

            if(pgm_read_byte(p_spr_a) & mask) {
                na |= 1;
                if(pgm_read_byte(p_spr) & mask) out |= pal1;
                else out |= pal0;
            }

            SHIFT_MASK_ALPHA();

            if(na == 0b11) *(p_fb++) = out;
            else if(na == 0b10) *(p_fb++) = (*p_fb & 0xf) | out;
            else if(na == 0b1) *(p_fb++) = (*p_fb & 0xf0) | out;
            else p_fb++;
        }
        if(x < sprW) {
            if(pgm_read_byte(p_spr_a) & mask) {
                if(pgm_read_byte(p_spr) & mask) out = pal1 << 4;
                else out = pal0 << 4;
                *p_fb = (*p_fb & 0x0f) | out;
            }
            
            if((x & 1) == 0) p_fb++;

            SHIFT_MASK_ALPHA();
        }
        p_fb += (128 - sprW) >> 1;
        p_spr += spr->width_d8 - foo;
        p_spr_a += spr->width_d8 - foo;
        mask = inv_shift_lut[sprX & 7];
        if((sprW + sprX) & 7) p_spr++, p_spr_a++;
        else if(((sprW + sprX) ^ 0xff) & 7) p_spr++, p_spr_a++;
    }
}

void draw_sprite_4bpp_int(sprite_t *spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH) {
    uint8_t *p_fb = oled_framebuffer + (screenY * 128 + screenX) / 2;
    uint8_t *p_spr = spr->data + (uint16_t(sprY) * spr->width + sprX) / 2;
    uint8_t x;
    uint8_t out;

    if((screenX & 1) == 0) { // even
        for(uint8_t y = 0; y < sprH; y++) {
            x = 0;
            if((sprX & 1) == 1) { // odd
                for(; x <= sprW - 2; x += 2) {
                    out = (*(p_spr++) & 0xf) << 4;
                    out |= (*p_spr & 0xf0) >> 4;
                    *(p_fb++) = out;
                }
                if(x < sprW) {
                    *(p_fb++) = (*p_fb & 0xf) | ((*(p_spr++) & 0xf) << 4);
                }
            }
            else {
                for(; x <= sprW - 2; x += 2) {
                    *(p_fb++) = *(p_spr++);
                }
                if(x < sprW) {
                    *(p_fb++) = (*p_fb & 0xf) | ((*(p_spr++) & 0xf0));
                }
            }
            p_fb += (128 - sprW) >> 1;
            p_spr += (spr->width - sprW) >> 1;
        }
    }
    else { // odd
        for(uint8_t y = 0; y < sprH; y++) {
            x = 1;
            if((sprX & 1) == 1) { // odd
                *(p_fb++) = (*p_fb & 0xf0) | (*(p_spr++) & 0xf);
                for(; x <= sprW - 2; x += 2) {
                    *(p_fb++) = *(p_spr++);
                }
                if(x < sprW) {
                    *(p_fb++) = (*p_fb & 0xf) | ((*(p_spr) & 0xf0));
                }
                else p_spr++;
            }
            else {
                *(p_fb++) = (*p_fb & 0xf0) | ((*(p_spr) & 0xf0) >> 4);
                for(; x <= sprW - 2; x += 2) {
                    out = (*(p_spr++) & 0xf) << 4;
                    out |= (*p_spr & 0xf0) >> 4;
                    *(p_fb++) = out;
                }
                if(x < sprW) {
                    *(p_fb++) = (*p_fb & 0xf) | ((*(p_spr++) & 0xf) << 4);
                }
                else p_spr++;
            }
            p_fb += (128 - sprW - 1) >> 1;
            p_spr += (spr->width - sprW) >> 1;
        }
    }
}

void draw_sprite_4bpp_alpha_int(sprite_t *spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH) {
    uint16_t alpha_ptr_add = (sprY * (spr->width_d8 + ((((sprW + sprX) & 7) || (((sprW + sprX) ^ 0xff) & 7)) ? 1 : 0)) + sprX / 8);
    uint8_t *p_fb = oled_framebuffer + (screenY * 128 + screenX) / 2;
    uint8_t *p_spr4 = spr->data + (uint16_t(sprY) * spr->width + sprX) / 2;
    uint8_t *p_spr = alpha->data + alpha_ptr_add;
    uint8_t mask = inv_shift_lut[sprX & 7];
    uint8_t x;
    uint8_t out;
    uint8_t foo, na;

    if((screenX & 1) == 0) { // even
        for(uint8_t y = 0; y < sprH; y++) {
            foo = 0;
            x = 0;
            out = 0;
            if((sprX & 1) == 1) { // odd
                for(; x <= sprW - 2; x += 2) {
                    out = 0, na = 0;
                    if(*p_spr & mask) {
                        na = 0b10;
                        out = (*(p_spr4) & 0xf) << 4;
                    }
                    SHIFT_MASK();
                    p_spr4++;

                    if(*p_spr & mask) {
                        na |= 1;
                        out |= (*p_spr4 & 0xf0) >> 4;
                    }
                    SHIFT_MASK();

                    if(na == 0b11) *(p_fb++) = out;
                    else if(na == 0b10) *(p_fb++) = (*p_fb & 0xf) | out;
                    else if(na == 0b1) *(p_fb++) = (*p_fb & 0xf0) | out;
                    else p_fb++;
                }
                if(x < sprW) {
                    if(*p_spr & mask) {
                        *p_fb = (*p_fb & 0xf) | ((*(p_spr4) & 0xf) << 4);
                    }
                    SHIFT_MASK();
                    p_fb++;
                    p_spr4++;
                }
                else p_spr4++;
            }
            else {
                for(; x <= sprW - 2; x += 2) {
                    out = 0, na = 0;
                    if(*p_spr & mask) {
                        na = 0b10;
                        out = (*(p_spr4) & 0xf0);
                    }
                    SHIFT_MASK();
                    if(*p_spr & mask) {
                        na |= 1;
                        out |= (*(p_spr4) & 0xf);
                    }
                    SHIFT_MASK();
                                        
                    if(na == 0b11) *(p_fb++) = out;
                    else if(na == 0b10) *(p_fb++) = (*p_fb & 0xf) | out;
                    else if(na == 0b1) *(p_fb++) = (*p_fb & 0xf0) | out;
                    else p_fb++;
                    p_spr4++;
                }
                if(x < sprW) {
                    if(*p_spr & mask) {
                        *p_fb = (*p_fb & 0xf) | ((*(p_spr4) & 0xf0));
                    }
                    p_fb++;
                    p_spr4++;
                    SHIFT_MASK();
                }
            }
            p_fb += (128 - sprW) >> 1;
            p_spr4 += (spr->width - sprW) >> 1;
            p_spr += spr->width_d8 - foo;
            mask = inv_shift_lut[sprX & 7];
            if((sprW + sprX) & 7) p_spr++;
            else if(((sprW + sprX) ^ 0xff) & 7) p_spr++;
        }
    }
    // TODO COMPLETE
    else { // odd
        for(uint8_t y = 0; y < sprH; y++) {
            x = 1;
            foo = 0;
            out = 0;
            if((sprX & 1) == 1) { // odd
                if(*p_spr & mask) {
                    *(p_fb) = (*p_fb & 0xf0) | (*(p_spr4) & 0xf);
                }
                p_fb++;
                p_spr4++;
                SHIFT_MASK();
                for(; x <= sprW - 2; x += 2) {
                    out = 0, na = 0;
                    if(*p_spr & mask) {
                        na = 0b10;
                        out = *(p_spr4) & 0xf0;
                    }
                    SHIFT_MASK();
                    if(*p_spr & mask) {
                        na |= 0b1;
                        out |= *(p_spr4) & 0xf;
                    }
                    SHIFT_MASK();
                    if(na == 0b11) *(p_fb++) = out;
                    else if(na == 0b10) *(p_fb++) = (*p_fb & 0xf) | out;
                    else if(na == 0b1) *(p_fb++) = (*p_fb & 0xf0) | out;
                    else p_fb++;
                    p_spr4++;
                }
                if(x < sprW) {
                    //*(p_fb++) = (*p_fb & 0xf) | ((*(p_spr4) & 0xf0));
                    if(*p_spr & mask) {
                        *p_fb = (*p_fb & 0xf) | ((*(p_spr4) & 0xf0));
                    }
                    SHIFT_MASK();
                    p_fb++;
                }
                else p_spr4++;
            }
            else {
                if(*p_spr & mask) {
                    *p_fb = (*p_fb & 0xf0) | ((*(p_spr4) & 0xf0) >> 4);
                }
                p_fb++;
                SHIFT_MASK();
                for(; x <= sprW - 2; x += 2) {
                    out = 0, na = 0;
                    if(*p_spr & mask) {
                        na = 0b10;
                        out = (*p_spr4 & 0xf) << 4;
                    }
                    p_spr4++;
                    SHIFT_MASK();
                    if(*p_spr & mask) {
                        na |= 1;
                        out |= (*p_spr4 & 0xf0) >> 4;
                    }
                    SHIFT_MASK();
                    if(na == 0b11) *(p_fb++) = out;
                    else if(na == 0b10) *(p_fb++) = (*p_fb & 0xf) | out;
                    else if(na == 0b1) *(p_fb++) = (*p_fb & 0xf0) | out;
                    else p_fb++;
                }
                if(x < sprW) {
                    if(*p_spr & mask) {
                        *p_fb = (*p_fb & 0xf) | ((*p_spr4 & 0xf) << 4);
                    }
                    SHIFT_MASK();
                    p_fb++;
                    p_spr4++;
                }
                else p_spr4++;
            }
            p_fb += (128 - sprW - 1) >> 1;
            p_spr4 += (spr->width - sprW) >> 1;
            p_spr += spr->width_d8 - foo;
            mask = inv_shift_lut[sprX & 7];
            if((sprW + sprX) & 7) p_spr++;
            else if(((sprW + sprX) ^ 0xff) & 7) p_spr++;
        }
    }
}

void draw_sprite_1bpp(sprite_t* spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal) {
    SPR_COORDS_CHECK();
    draw_sprite_1bpp_int(spr, screenX, screenY, sprX, sprY, sprW, sprH, sprPal);
}

void draw_sprite_1bpp_alpha(sprite_t *spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal) {
    SPR_COORDS_CHECK();
    draw_sprite_1bpp_alpha_int(spr, alpha, screenX, screenY, sprX, sprY, sprW, sprH, sprPal);
}

void draw_sprite_1bpp_progmem(sprite_t *spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal) {
    SPR_COORDS_CHECK();
    draw_sprite_1bpp_progmem_int(spr, screenX, screenY, sprX, sprY, sprW, sprH, sprPal);
}

void draw_sprite_1bpp_alpha_progmem(sprite_t *spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH, uint8_t sprPal) {
    SPR_COORDS_CHECK();
    draw_sprite_1bpp_alpha_progmem_int(spr, alpha, screenX, screenY, sprX, sprY, sprW, sprH, sprPal);
}

void draw_sprite_4bpp(sprite_t *spr, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH) {
    SPR_COORDS_CHECK();
    draw_sprite_4bpp_int(spr, screenX, screenY, sprX, sprY, sprW, sprH);
}

void draw_sprite_4bpp_alpha(sprite_t *spr, sprite_t *alpha, int16_t screenX, int16_t screenY, uint8_t sprX, uint8_t sprY, uint8_t sprW, uint8_t sprH) {
    SPR_COORDS_CHECK();
    draw_sprite_4bpp_alpha_int(spr, alpha, screenX, screenY, sprX, sprY, sprW, sprH);
}