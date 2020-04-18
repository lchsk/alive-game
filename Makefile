BIN = alive

GAME_DIR= src

CFLAGS += -std=c99 -pedantic -Wall -O0

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

ENGINE_DIR = ../engine212/engine212
ENGINE_DIR_MAIN = ../engine212

ENGINE_SRC = \
    ${ENGINE_DIR}/list.c \
    ${ENGINE_DIR}/input.c \
    ${ENGINE_DIR}/engine.c \
    ${ENGINE_DIR}/sprite.c \
    ${ENGINE_DIR}/animation.c \
    ${ENGINE_DIR}/entity.c \
    ${ENGINE_DIR}/loader.c \
    ${ENGINE_DIR}/text.c \
    ${ENGINE_DIR}/music_loader.c \
    ${ENGINE_DIR}/sound_loader.c \
    ${ENGINE_DIR}/font_loader.c \
    ${ENGINE_DIR}/texture_loader.c \
    ${ENGINE_DIR}/texture.c \
    ${ENGINE_DIR}/console.c

GAME_SRC = \
    ${GAME_DIR}/main.c \
    ${GAME_DIR}/textures.c \
    ${GAME_DIR}/level.c \
    ${ENGINE_SRC}

.PHONY: game

game:
	$(CC) -I ${ENGINE_DIR_MAIN} ${GAME_SRC} ${CFLAGS} -o ${BIN} ${LIBS}
