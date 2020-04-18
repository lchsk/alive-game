#ifndef TEXTURES_H
    #define TEXTURES_H


enum {
    TEX_TILE_1,
    TEX_TILE_2,
    TEX_TILE_3,
    TEX_TILE_4,
    TEX_TILE_5,

    // Characters
    SURVIVOR_GUN,
    SURVIVOR_MACHINE,
    SURVIVOR_SILENCER,

    TEX_BULLET,

    TEXTURES_COUNT,
};

enum
{
    FONT_FREE_SANS_26,
    FONTS_COUNT,
};

extern char* images[TEXTURES_COUNT];
extern char* fonts[FONTS_COUNT];
extern int fonts_sizes[FONTS_COUNT];

    #endif /* TEXTURES_H */
