#include "mode7.h"

matrix2_s8p7_t mul_matrix2_s8p7(matrix2_s8p7_t *a, matrix2_s8p7_t *b) {
    matrix2_s8p7_t result = {
        ((int32_t(a->a) * b->a) >> 7) + ((int32_t(a->b) * b->c) >> 7),
        ((int32_t(a->a) * b->b) >> 7) + ((int32_t(a->b) * b->d) >> 7),
        ((int32_t(a->c) * b->a) >> 7) + ((int32_t(a->d) * b->c) >> 7),
        ((int32_t(a->c) * b->b) >> 7) + ((int32_t(a->d) * b->d) >> 7)
    };
    return result;
}

vector2_s8p7_t mul_matrix2_s8p7_vector2_s8p7(matrix2_s8p7_t *a, vector2_s8p7_t *b) {
    vector2_s8p7_t result = {
        (((int32_t(a->a) * b->x) >> 7) + ((int32_t(a->b) * b->y) >> 7)) & 0xFFFF,
        (((int32_t(a->c) * b->x) >> 7) + ((int32_t(a->d) * b->y) >> 7)) & 0xFFFF
    };
    return result;
}

uint8_t tileset_get_tile(mode7_tileset_t *tileset, uint8_t x, uint8_t y) {
    return tileset->tilemap[y * tileset->width + x];
}

void draw_sprite_4bpp_mode7_perspective(sprite_t *spr, vector2_s8p7_t *origin, matrix2_s8p7_t *mat, mode7_camera_t *camera) {
    vector2_s8p7_t tmp;
    matrix2_s8p7_t row_mat;
    int16_t stepX;
    int16_t stepY;
    uint16_t dist;
    uint8_t *framebuffer = oled_framebuffer;
    uint8_t val;
    uint8_t cx;
    uint8_t cy;
    uint8_t sprW = spr->width, sprH = spr->height;

    for(uint8_t y = 0; y < 64; y++) {
        dist = 8192 / (y + camera->dist_add);
        row_mat = {
            (int32_t(mat->a) * dist) >> 7,
            (int32_t(mat->b) * dist) >> 7,
            (int32_t(mat->c) * dist) >> 7,
            (int32_t(mat->d) * dist) >> 7
        };
        tmp = {-origin->x - 8192, int16_t(y) * 128 - origin->y + camera->fov};
        tmp = mul_matrix2_s8p7_vector2_s8p7(&row_mat, &tmp);
        tmp.x += origin->x - camera->pos.x;
        tmp.y += origin->y - camera->pos.y;
        stepX = row_mat.a;
        stepY = row_mat.c;

        for(uint8_t x = 0; x < 128; x += 2) {
            val = 0;
            tmp.x += stepX;
            tmp.y += stepY;
            cx = tmp.x >> 7;
            cy = tmp.y >> 7;
            //if(cx < sprW && cy < sprH)
                val |= get_pixel_4bpp(spr, cx & 7, cy & 7) << 4;
            tmp.x += stepX;
            tmp.y += stepY;
            cx = tmp.x >> 7;
            cy = tmp.y >> 7;
            //if(cx < sprW && cy < sprH)
                val |= get_pixel_4bpp(spr, cx & 7, cy & 7);

            *(framebuffer++) = val;
        }
    }
}

void draw_sprite_4bpp_mode7(sprite_t *spr, vector2_s8p7_t *origin, vector2_s8p7_t *scroll, matrix2_s8p7_t *mat) {
    vector2_s8p7_t row_origin = {-origin->x + scroll->x, -origin->y + scroll->y};
    vector2_s8p7_t tmp;
    uint8_t val;
    int16_t stepX = mat->a;
    int16_t stepY = mat->c;
    uint8_t *framebuffer = oled_framebuffer;
    uint8_t cx;
    uint8_t cy;
    uint8_t sprW = spr->width;
    uint8_t sprH = spr->height;
    row_origin = mul_matrix2_s8p7_vector2_s8p7(mat, &row_origin);
    row_origin.x += origin->x;
    row_origin.y += origin->y;

    for(uint8_t y = 0; y < 64; y++) {
        tmp = row_origin;
        row_origin.x += mat->b;
        row_origin.y += mat->d;
        for(uint8_t x = 0; x < 128; x += 2) {
            val = 0;
            tmp.x += stepX;
            tmp.y += stepY;
            cx = tmp.x >> 7;
            cy = tmp.y >> 7;
            if(cx < sprW && cy < sprH)
                val |= get_pixel_4bpp(spr, cx, cy) << 4;
            tmp.x += stepX;
            tmp.y += stepY;
            cx = tmp.x >> 7;
            cy = tmp.y >> 7;
            if(cx < sprW && cy < sprH)
                val |= get_pixel_4bpp(spr, cx, cy);

            *(framebuffer++) = val;
        }
    }
}

void draw_tilemap_4bpp_mode7(vector2_s8p7_t *origin, vector2_s8p7_t *scroll, matrix2_s8p7_t *mat, mode7_tileset_t *tileset) {
    vector2_s8p7_t row_origin = {-origin->x + scroll->x, -origin->y + scroll->y};
    vector2_s8p7_t tmp;
    uint8_t val;
    int16_t stepX = mat->a;
    int16_t stepY = mat->c;
    uint8_t *framebuffer = oled_framebuffer;
    uint8_t cx;
    uint8_t cy;
    uint8_t sprW = tileset->width;
    uint8_t sprH = tileset->height;
    uint8_t shift = 7 - tileset->tile_size_bits;
    uint8_t tile;
    row_origin = mul_matrix2_s8p7_vector2_s8p7(mat, &row_origin);
    row_origin.x += origin->x;
    row_origin.y += origin->y;

    for(uint8_t y = 0; y < 64; y++) {
        tmp = row_origin;
        row_origin.x += mat->b;
        row_origin.y += mat->d;
        for(uint8_t x = 0; x < 128; x += 2) {
            val = 0;
            tmp.x += stepX;
            tmp.y += stepY;
            cx = tmp.x >> 7;
            cy = tmp.y >> 7;
            if(cx < sprW && cy < sprH) {
                tile = tileset_get_tile(tileset, cx, cy);
                val |= get_pixel_4bpp(tileset->tiles[tile], (tmp.x & 0xff) >> shift, (tmp.y & 0xff) >> shift) << 4;
            }
            tmp.x += stepX;
            tmp.y += stepY;
            cx = tmp.x >> 7;
            cy = tmp.y >> 7;
            if(cx < sprW && cy < sprH) {
                tile = tileset_get_tile(tileset, cx, cy);
                val |= get_pixel_4bpp(tileset->tiles[tile], (tmp.x & 0xff) >> shift, (tmp.y & 0xff) >> shift);
            }

            *(framebuffer++) = val;
        }
    }
}