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

#define WINDOW_W 1280
#define WINDOW_H 768

#define BULLETS_COUNT 100

Entity *player;

Level* level;



Entity bullets[BULLETS_COUNT];

typedef struct BulletProperties {
    int is_flying;
    double speed_x;
    double speed_y;
} BulletProperties;

BulletProperties bp[BULLETS_COUNT];

void reset_bullet_properties(BulletProperties *bp) {
    bp->is_flying = 0;
    bp->speed_x = 0;
    bp->speed_y = 0;
}

Entity* get_bullet() {
    for (int i = 0; i < BULLETS_COUNT; i++) {
        Entity *e = &bullets[i];

        BulletProperties *bp = e->properties;

        if (!bp->is_flying) {
            bp->is_flying = 1;
            return e;
        }
    }

    return NULL;
}


void SETUP();
void SETUP_LOADING_SCREEN();


Engine *engine;
Entity *entity;
Entity *entity2;

Pointd get_pos(Input *input) {
            double x_diff = (input->mouse_x - player->position.x);
            double y_diff = (input->mouse_y - player->position.y);

                  double m = fmax(fabs(x_diff), fabs(y_diff));

                  double xd = x_diff / m;
                  double yd = y_diff / m;

                  double change_x = xd * 1;
                  double change_y = yd * 1;

                  return (Pointd) {change_x, change_y};

}

Pointd get_new_position(Input *input, double delta,
                        int direction
    ) {
            double x_diff = (input->mouse_x - player->position.x);
            double y_diff = (input->mouse_y - player->position.y);

                  double m = fmax(fabs(x_diff), fabs(y_diff));

                  double xd = x_diff / m;
                  double yd = y_diff / m;

                  double player_x = player->position.x;
                  double player_y = player->position.y;

                  double change_x = xd * 1 * delta;
                  double change_y = yd * 1 * delta;

                  int dir_x, dir_y;

                  if (direction == 0) {
                      dir_x = dir_y = 1;
                  } else if (direction == 1) {
                  } else if (direction == 2) {
                      dir_x = dir_y = -1;
                  } else if (direction == 3) {
                  }

                  player_x += change_x * dir_x;
                  player_y += change_y * dir_y;

                  return (Pointd) {player_x, player_y};
}

void render(Engine *engine) {
    engine_render(engine);

    if (engine->state == ENGINE_LOADING_SCREEN_SETUP) {
        SETUP_LOADING_SCREEN();
    }

    if (engine->state == ENGINE_LOADING) {
    }

    if (engine->state == ENGINE_GAME_SETUP) {
        SETUP();
    }

    if (engine->state == ENGINE_RUNNING) {

        double a = atan2(engine->input->mouse_y - player->position.y, engine->input->mouse_x - player->position.x)
            * 180/3.14;

        player->sprite->rotation = a;

        render_level(engine, level);

        if (engine->input->mouse_left_down) {
            Entity *e = get_bullet();

            if (e) {
                entity_set_position(e, player->position.x, player->position.y);
                e->sprite->rotation = a + 90;

                BulletProperties *bp = e->properties;

                Pointd speed = get_pos(engine->input);

                bp->speed_x = speed.x;
                bp->speed_y = speed.y;
            }
        }

        if (engine->input->mouse_left_up) {
        }

        if (engine->input->pressed_keys[SDL_SCANCODE_W]) {
            Pointd pos = get_new_position(engine->input, engine->_delta, 0);
            entity_set_position(player, pos.x, pos.y);
        }

        if (engine->input->pressed_keys[SDL_SCANCODE_S]) {
            Pointd pos = get_new_position(engine->input, engine->_delta, 2);
            entity_set_position(player, pos.x, pos.y);
        }

        if (engine->input->keys_up[SDL_SCANCODE_ESCAPE]) {
            engine->is_running = 0;
        }

        entity_render(player, engine->renderer, engine->_delta);

        for (int i = 0; i < BULLETS_COUNT; i++) {
            Entity *e = &bullets[i];
            BulletProperties *bp = e->properties;

            entity_set_position(e, e->position.x + bp->speed_x * engine->_delta, e->position.y + bp->speed_y * engine->_delta);
            entity_render(e, engine->renderer, engine->_delta);
        }

        for (int i = 0; i < BULLETS_COUNT; i++) {
            Entity *e = &bullets[i];

            Pointd p = e->position;

            if (p.x > 1.1 * WINDOW_W
                || p.x < -100
                || p.y < -100
                || p.y > 1.1 * WINDOW_H) {
                BulletProperties *bp = e->properties;
                bp->is_flying = 0;
            }
        }

    }
}

void engine_loop() {
    engine_clear(engine);

    read_input(engine->input);
    render(engine);

    engine_present(engine);
}

void SETUP_LOADING_SCREEN()
{
    engine->state = ENGINE_LOADING;
}


void SETUP()
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

void setup_resources()
{
}

int main() {
    setup_resources();

    Loader *loader = loader_new(
        TEXTURES_COUNT
        , FONTS_COUNT, 0, 0);

    loader->texture_loader->paths = images;
    loader->font_loader->paths = fonts;
    loader->font_loader->sizes = fonts_sizes;

    engine = engine_new(WINDOW_W, WINDOW_H, "alive game");
    engine->loader = loader;

    run_main_loop();

    texture_loader_free(loader->texture_loader);

    engine_free(engine);
}
