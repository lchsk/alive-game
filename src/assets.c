#include "assets.h"

char* images[TEXTURES_COUNT] = {
    // Tiles
    "./assets/tile_01.png",
    "./assets/tile_02.png",
    "./assets/tile_03.png",
    "./assets/tile_04.png",
    "./assets/tile_05.png",
    "./assets/tile_06.png",

    "./assets/tile_35.png",
    "./assets/tile_36.png",
    "./assets/tile_55.png",

    // Player
    "./assets/survivor1_gun.png",
    "./assets/survivor1_machine.png",
    "./assets/survivor1_silencer.png",

    // Enemies
    "./assets/hitman1_silencer.png",
    "./assets/manBlue_gun.png",
    "./assets/manBrown_machine.png",
    "./assets/manOld_machine.png",
    "./assets/soldier1_machine.png",
    "./assets/womanGreen_silencer.png",
    "./assets/zombie1_gun.png",

    "./assets/crosshair012.png",

    // Bullets
    "./assets/spaceMissiles_015.png",
};

char* fonts[FONTS_COUNT] = {
    "./assets/gunplay_rg.ttf",
};

int fonts_sizes[FONTS_COUNT] = {
    30,
};

char* preloader_fonts[PRELOADER_FONTS_COUNT] = {
    "./assets/gunplay_rg.ttf",
};

int preloader_fonts_sizes[PRELOADER_FONTS_COUNT] = {
    30,
};

char* music[MUSIC_COUNT] = {
    "./assets/music.ogg",
};

char* sounds[SOUNDS_COUNT] = {
    "./assets/pistol.ogg",
    "./assets/rifle.ogg",
    "./assets/machine.ogg",
};

