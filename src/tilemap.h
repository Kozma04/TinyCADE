#pragma once

#include "sprite.h"

struct tile_t {
    sprite_t *sprite;
    sprite_t *mask;
};

struct tileset_t {
    uint8_t tile_count;
    tile_t **tiles;
    uint8_t tile_size;
};

struct tilemap_t {
    uint8_t **map;
    uint8_t layer_count;
    uint8_t width;
    uint8_t height;
};

struct tilemap_camera_t {
    int16_t scroll_x;
    int16_t scroll_y;
    uint8_t viewport_x;
    uint8_t viewport_y;
    uint8_t viewport_w;
    uint8_t viewport_h;
};

extern void draw_tilemap(tileset_t *tileset, tilemap_t *tilemap, tilemap_camera_t *camera);