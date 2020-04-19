#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <engine212/engine.h>

#include "textures.h"
#include "level.h"
#include "bullets.h"
#include "math.h"
#include "player.h"

Engine *engine;
Entity *player;
Level* level;

static void setup_game();
static void setup_loading_screen();
static void render_game(Engine *engine);

void render_engine(Engine *engine) {
    engine_render(engine);

    if (engine->state == ENGINE_LOADING_SCREEN_SETUP) {
        setup_loading_screen();
    }

    if (engine->state == ENGINE_LOADING) {
    }

    if (engine->state == ENGINE_GAME_SETUP) {
        setup_game();
    }

    if (engine->state == ENGINE_RUNNING) {
        render_game(engine);
    }
}

void handle_input(Engine *engine) {
       if (engine->input->mouse_left_down) {
            Entity *e = get_free_bullet();

            if (e) {
                entity_set_position(e, player->position.x, player->position.y);
                e->sprite->rotation = player->sprite->rotation + 90;


                BulletProperties *bp = e->properties;

                Pointd speed = get_pos(engine->input, player);

                bp->speed_x = speed.x;
                bp->speed_y = speed.y;
            }
        }

        if (engine->input->pressed_keys[SDL_SCANCODE_W]) {
            Pointd pos = get_new_position(engine->input, player, engine->_delta, 0);
            entity_set_position(player, pos.x, pos.y);
        }

        if (engine->input->pressed_keys[SDL_SCANCODE_S]) {
            Pointd pos = get_new_position(engine->input, player, engine->_delta, 2);
            entity_set_position(player, pos.x, pos.y);
        }

        if (engine->input->keys_up[SDL_SCANCODE_ESCAPE]) {
            engine->is_running = 0;
        }
}

void render_game(Engine *engine) {
    rotate_player(player, engine->input);

    render_level(engine, level);

    handle_input(engine);

    entity_render(player, engine->renderer, engine->_delta);
    bullets_render(engine);
}

void engine_loop() {
    engine_clear(engine);

    read_input(engine->input);
    render_engine(engine);

    engine_present(engine);
}

void setup_loading_screen()
{
    engine->state = ENGINE_LOADING;
}


void setup_game()
{
    level = build_level_1(engine);

    player = entity_new();
    Sprite *sprite = sprite_new(engine->loader->texture_loader->textures[SURVIVOR_GUN]);
    player->sprite = sprite;
    sprite->zoom = 2;
    sprite->rotation = 0;

    for (int i = 0; i < BULLETS_COUNT; i++) {
        Sprite *sprite = sprite_new(engine->loader->texture_loader->textures[TEX_BULLET]);

        Entity *e = &bullets[i];

        e->sprite = sprite;
        e->properties = &bp[i];
        reset_bullet_properties(e->properties);
        entity_set_position(e, -100, -100);
    }

    engine->state = ENGINE_RUNNING;
}


void run_main_loop() {
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(engine_loop, engine->requested_fps, 1);
    #else
    while (engine->is_running) {
        engine_loop();

        SDL_Delay(engine->_requested_delay);
    }
    #endif
}

int main() {
    Loader *loader = loader_new(
        TEXTURES_COUNT
        , FONTS_COUNT, 0, 0);

    loader->texture_loader->paths = images;
    loader->font_loader->paths = fonts;
    loader->font_loader->sizes = fonts_sizes;

    engine = engine_new(1280, 768, "alive game");
    engine->loader = loader;

    run_main_loop();

    texture_loader_free(loader->texture_loader);

    engine_free(engine);
}
