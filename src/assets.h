#ifndef ASSETS_H
#define ASSETS_H

enum {
    TEX_TILE_1,
    TEX_TILE_2,
    TEX_TILE_3,
    TEX_TILE_4,
    TEX_TILE_5,
    TEX_TILE_6,

    TEX_TILE_35,
    TEX_TILE_36,
    TEX_TILE_55,

    // Characters
    SURVIVOR_GUN,
    SURVIVOR_MACHINE,
    SURVIVOR_SILENCER,

    HITMAN_SILENCER,
    MAN_BLUE_GUN,
    MAN_BROWN_MACHINE,
    MAN_OLD_MACHINE,
    SOLDIER1_MACHINE,
    WOMAN_GREEN_SILENCER,
    ZOMBIE1_GUN,

    TEX_CROSSHAIR,

    TEX_BULLET,

    TEXTURES_COUNT,
};

enum {
    FONT_FREE_SANS_26,
    FONTS_COUNT,
};

enum {
    PRELOADER_FONT_FREE_SANS_26,
    PRELOADER_FONTS_COUNT,
};

enum {
    MUSIC_1,
    MUSIC_COUNT,
};

enum {
    SOUND_PISTOL,
    SOUND_RIFLE,
    SOUND_MACHINE,
    SOUNDS_COUNT,
};

extern char* images[TEXTURES_COUNT];

extern char* fonts[FONTS_COUNT];
extern int fonts_sizes[FONTS_COUNT];
extern char* preloader_fonts[PRELOADER_FONTS_COUNT];
extern int preloader_fonts_sizes[PRELOADER_FONTS_COUNT];

extern char* music[MUSIC_COUNT];
extern char* sounds[SOUNDS_COUNT];

#endif /* ASSETS_H */
