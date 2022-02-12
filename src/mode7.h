#pragma once

#include "oled.h"
#include "sprite.h"

struct matrix2_s8p7_t {
    int16_t a, b;
    int16_t c, d;
};

struct vector2_s8p7_t {
    int16_t x, y;
};

struct mode7_camera_t {
    int16_t fov;
    uint8_t dist_add;
    vector2_s8p7_t pos;
};

struct mode7_tileset_t {
    uint8_t *tilemap;
    uint8_t width;
    uint8_t height;
    sprite_t **tiles;
    uint8_t tile_size_bits;
};

extern matrix2_s8p7_t mul_matrix2_s8p7(matrix2_s8p7_t *a, matrix2_s8p7_t *b);
extern vector2_s8p7_t mul_matrix2_s8p7_vector2_s8p7(matrix2_s8p7_t *a, vector2_s8p7_t *b);

extern uint8_t __attribute__((optimize("O3"), always_inline)) tileset_get_tile(mode7_tileset_t *tileset, uint8_t x, uint8_t y);

extern void __attribute__((optimize("O3"))) draw_sprite_4bpp_mode7(sprite_t *spr, vector2_s8p7_t *origin, vector2_s8p7_t *scroll, matrix2_s8p7_t *mat);
extern void __attribute__((optimize("O3"))) draw_sprite_4bpp_mode7_perspective(sprite_t *spr, vector2_s8p7_t *origin, matrix2_s8p7_t *mat, mode7_camera_t *camera);
extern void __attribute__((optimize("O3"))) draw_tilemap_4bpp_mode7(vector2_s8p7_t *origin, vector2_s8p7_t *scroll, matrix2_s8p7_t *mat, mode7_tileset_t *tileset);