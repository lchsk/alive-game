#include <cunctator/engine.h>

#include "assets.h"
#include "level.h"

int level1_tiles[LEVEL1_WIDTH][LEVEL1_HEIGHT] = {
    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_6, TEX_TILE_5, TEX_TILE_5, TEX_TILE_3,
     TEX_TILE_4, TEX_TILE_3, TEX_TILE_4, TEX_TILE_3, TEX_TILE_2, TEX_TILE_1},
    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_6, TEX_TILE_5, TEX_TILE_6, TEX_TILE_3,
     TEX_TILE_4, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5},
    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5},
    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5},
    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5},

    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5},
    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_2, TEX_TILE_3, TEX_TILE_2, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5},
    {TEX_TILE_6, TEX_TILE_4, TEX_TILE_1, TEX_TILE_2, TEX_TILE_3, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5},
    {TEX_TILE_6, TEX_TILE_3, TEX_TILE_2, TEX_TILE_1, TEX_TILE_2, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5},
    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_3, TEX_TILE_2, TEX_TILE_1, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5},

    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_4, TEX_TILE_2, TEX_TILE_3, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_3, TEX_TILE_1, TEX_TILE_1, TEX_TILE_5},
    {TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_1, TEX_TILE_4, TEX_TILE_3, TEX_TILE_2, TEX_TILE_5},
    {TEX_TILE_6, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_2, TEX_TILE_2, TEX_TILE_1, TEX_TILE_4, TEX_TILE_4},
    {TEX_TILE_1, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_2, TEX_TILE_3, TEX_TILE_1, TEX_TILE_2, TEX_TILE_3},

    {TEX_TILE_35, TEX_TILE_35, TEX_TILE_35, TEX_TILE_35, TEX_TILE_35,
     TEX_TILE_35, TEX_TILE_35, TEX_TILE_35, TEX_TILE_35, TEX_TILE_35,
     TEX_TILE_35, TEX_TILE_35},
    {TEX_TILE_55, TEX_TILE_55, TEX_TILE_55, TEX_TILE_55, TEX_TILE_55,
     TEX_TILE_55, TEX_TILE_55, TEX_TILE_55, TEX_TILE_55, TEX_TILE_55,
     TEX_TILE_55, TEX_TILE_55},
    {TEX_TILE_36, TEX_TILE_36, TEX_TILE_36, TEX_TILE_36, TEX_TILE_36,
     TEX_TILE_36, TEX_TILE_36, TEX_TILE_36, TEX_TILE_36, TEX_TILE_36,
     TEX_TILE_36, TEX_TILE_36},
    {TEX_TILE_1, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_4, TEX_TILE_1, TEX_TILE_2, TEX_TILE_2},
    {TEX_TILE_1, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_4, TEX_TILE_2, TEX_TILE_1, TEX_TILE_3},
    {TEX_TILE_1, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5, TEX_TILE_5,
     TEX_TILE_5, TEX_TILE_5, TEX_TILE_3, TEX_TILE_3, TEX_TILE_1, TEX_TILE_5},
};

Level* build_level_1(Engine* engine) {
    Level* level = calloc(1, sizeof(Level));

    level->entities = calloc(LEVEL1_WIDTH, sizeof(Entity**));

    for (int i = 0; i < LEVEL1_WIDTH; i++) {
        level->entities[i] = calloc(LEVEL1_HEIGHT, sizeof(Entity*));

        for (int j = 0; j < LEVEL1_HEIGHT; j++) {
            Entity* e = entity_new();

            level->entities[i][j] = e;
            Sprite* sprite = sprite_new(
                engine->loader->texture_loader->textures[level1_tiles[i][j]]);
            e->sprite = sprite;
            e->sprite->zoom = TILE_ZOOM;
            entity_set_position(e, i * TILE_WIDTH * TILE_ZOOM,
                                j * TILE_HEIGHT * TILE_ZOOM);
        }
    }

    return level;
}

void render_level(Engine* engine, Level* level) {
    for (int i = 0; i < LEVEL1_WIDTH; i++) {
        for (int j = 0; j < LEVEL1_HEIGHT; j++) {
            entity_render(level->entities[i][j], engine->renderer,
                          engine->_delta);
        }
    }
}
