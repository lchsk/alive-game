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
    ${GAME_DIR}/assets.c \
    ${GAME_DIR}/level.c \
    ${GAME_DIR}/bullets.c \
    ${GAME_DIR}/math.c \
    ${GAME_DIR}/player.c \
    ${GAME_DIR}/enemies.c \
    ${ENGINE_SRC}

.PHONY: game web

game:
	$(CC) -I ${ENGINE_DIR_MAIN} ${GAME_SRC} ${CFLAGS} -o ${BIN} ${LIBS}

web:
	emcc -v -I ${ENGINE_DIR_MAIN} ${GAME_SRC} ${CFLAGS} ${LIBS} -s TOTAL_MEMORY=67108864 -s ALLOW_MEMORY_GROWTH=1 --use-preload-plugins -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -lm -s SDL2_IMAGE_FORMATS='["png"]' --preload-file assets -o alive.html
