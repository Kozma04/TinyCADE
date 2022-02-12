#include "tilemap.h"

void draw_tilemap(tileset_t *tileset, tilemap_t *tilemap, tilemap_camera_t *camera) {
    int16_t tile_x0 = (camera->scroll_x + camera->viewport_x) / tileset->tile_size;
    int16_t tile_y0 = (camera->scroll_y + camera->viewport_y) / tileset->tile_size;
    int16_t tile_x1 = tile_x0 + camera->viewport_w / tileset->tile_size;
    int16_t tile_y1 = tile_y0 + camera->viewport_h / tileset->tile_size;
    if(camera->viewport_w % tileset->tile_size) tile_x1++;
    if(camera->viewport_h % tileset->tile_size) tile_y1++;
    for(int16_t tile_y = tile_y0; tile_y <= tile_y1; tile_y++) {
        for(int16_t tile_x = tile_x0; tile_x <= tile_x1; tile_x++) {
            if(tile_x < 0 || tile_y < 0 || tile_x >= tilemap->width || tile_y >= tilemap->height)
                continue;
            for(uint8_t layer = 0; layer < tilemap->layer_count; layer++) {
                uint8_t tile_id = tilemap->map[layer][uint16_t(tile_y) * tilemap->width + tile_x];
                tile_t *tile = tileset->tiles[tile_id];
                if(tile->sprite == 0) continue;
                if(tile->mask == 0) {
                    draw_sprite_4bpp(
                        tile->sprite,
                        tile_x * tileset->tile_size - camera->scroll_x, tile_y * tileset->tile_size - camera->scroll_y,
                        0, 0, tileset->tile_size, tileset->tile_size
                    );
                }
                else {
                    draw_sprite_4bpp_alpha(
                        tile->sprite, tile->mask,
                        tile_x * tileset->tile_size - camera->scroll_x, tile_y * tileset->tile_size - camera->scroll_y,
                        0, 0, tileset->tile_size, tileset->tile_size
                    );
                }
            }
        }
    }
}