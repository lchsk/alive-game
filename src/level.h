#ifndef LEVEL_H
    #define LEVEL_H


#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 12

#define TILE_ZOOM 4
#define TILE_WIDTH 16
#define TILE_HEIGHT 16

extern int level1_tiles[LEVEL1_WIDTH][LEVEL1_HEIGHT];


typedef struct Level {
    Entity ***entities;
} Level;


Level* build_level_1();
void render_level(Engine *engine, Level *level);

    #endif /* LEVEL_H */
