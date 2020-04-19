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

#include "config.h"
#include "assets.h"
#include "level.h"
#include "bullets.h"
#include "math.h"
#include "player.h"
#include "enemies.h"
#include "timer.h"

Engine *engine;
Entity *player;
Level* level;

GameTimer timer;

PlayerState player_state;
Text *text_gun;
Text *text_game_end;
Text *text_kills;
Text *text_time;

Text *text_loading;

#define TEXT_HELP_COUNT 6
Text *text_help[TEXT_HELP_COUNT];

Sprite *cursor;



static void setup_game();
static void setup_loading_screen();
static void render_game(Engine *engine);
static void render_hud(Engine *engine);

void init_play() {
    // Set starting position
    double x = get_rand_int(0.1 * engine->width, 0.9 * engine->width);
    double y = get_rand_int(0.1 * engine->height, 0.9 * engine->height);
    entity_set_position(player, x, y);

    player->rects[0].x = player->position.x;
    player->rects[0].y = player->position.y;
    player->rects[0].w = player->sprite->texture->width;
    player->rects[0].h = player->sprite->texture->height;
    player->rects_sz = 1;

    player_state.game_state = GAME_STATE_PLAY;
    player_state.health = 100;
    player_state.survival_time = 0;
    player_state.kills = 0;
    player_state.weapon = WEAPON_PISTOL;
    player_state.since_last_shot = 0;

    timer.total_time = 0;
    timer.since_last_enemy_launched = 0;

    reset_enemies(engine);
    reset_bullets();

}

void render_engine(Engine *engine) {
    engine_render(engine);

    if (engine->state == ENGINE_LOADING_SCREEN_SETUP) {
        setup_loading_screen();
    }

    if (engine->state == ENGINE_LOADING) {

        if (engine->loader->total_to_load > 0) {
            char text[100];
            sprintf(text, "Alive: the game | Loading: %d%%", (int)ceil(engine->loader->total_loaded / (float)engine->loader->total_to_load * 100));
            text_set(text_loading, engine->renderer, text);
        }

        sprite_render(text_loading->sprite, engine->renderer);
    }

    if (engine->state == ENGINE_GAME_SETUP) {
        setup_game();
    }

    if (engine->state == ENGINE_RUNNING) {
        render_game(engine);
    }
}

void handle_input(Engine *engine) {
    if (engine->input->mouse_left_down || (player_state.weapon == WEAPON_MACHINE && engine->input->mouse_left_pressed)) {
            Entity *e = get_free_bullet();

            int time_required = 0;

                if (player_state.weapon == WEAPON_PISTOL) {
                    time_required = PISTOL_RELOAD_TIME;
                } else if (player_state.weapon == WEAPON_RIFLE) {
                    time_required = RIFLE_RELOAD_TIME;
                } else if (player_state.weapon == WEAPON_MACHINE) {
                    time_required = MACHINE_RELOAD_TIME;
                }

                if ((player_state.game_state == GAME_STATE_PLAY || player_state.game_state == GAME_STATE_HELP)  && e && player_state.since_last_shot > time_required) {
                player_state.since_last_shot = 0;

                entity_set_position(e, player->position.x, player->position.y);
                e->sprite->rotation = player->sprite->rotation + 90;

                e->rects[0].x = e->position.x;
                e->rects[0].y = e->position.y;
                e->rects[0].w = e->sprite->texture->width;
                e->rects[0].h = e->sprite->texture->height;
                e->rects_sz = 1;

                Pointd speed = get_pos(engine->input, player);

                BulletProperties *bp = e->properties;
                bp->is_enemy = 0;
                bp->speed_x = speed.x;
                bp->speed_y = speed.y;

                bp->strength = player_state.weapon;

                if (player_state.weapon == WEAPON_PISTOL) {
                    int channel = Mix_PlayChannel(-1, engine->loader->sound_loader->sounds[SOUND_PISTOL], 0);
                    Mix_Volume(channel, MIX_MAX_VOLUME * SOUND_VOLUME);

                    bp->speed_factor = PISTOL_BULLET_SPEED;
                } else if (player_state.weapon == WEAPON_RIFLE) {
                    int channel = Mix_PlayChannel(-1, engine->loader->sound_loader->sounds[SOUND_RIFLE], 0);
                    Mix_Volume(channel, MIX_MAX_VOLUME * SOUND_VOLUME);

                    bp->speed_factor = RIFLE_BULLET_SPEED;
                } else if (player_state.weapon == WEAPON_MACHINE) {
                    int channel = Mix_PlayChannel(-1, engine->loader->sound_loader->sounds[SOUND_MACHINE], 0);
                    Mix_Volume(channel, MIX_MAX_VOLUME * SOUND_VOLUME);

                    bp->speed_factor = MACHINE_BULLET_SPEED;
                }
            }
        }

        if (engine->input->pressed_keys[SDL_SCANCODE_W]) {
            Pointd pos = get_new_position(engine->input, player, engine->_delta, 0);
            entity_set_position(player, pos.x, pos.y);
        }

        if (engine->input->pressed_keys[SDL_SCANCODE_A]) {
            Pointd pos = get_new_position(engine->input, player, engine->_delta, 1);
            entity_set_position(player, pos.x, pos.y);
        }

        if (engine->input->pressed_keys[SDL_SCANCODE_S]) {
            Pointd pos = get_new_position(engine->input, player, engine->_delta, 2);
            entity_set_position(player, pos.x, pos.y);
        }

        if (engine->input->pressed_keys[SDL_SCANCODE_D]) {
            Pointd pos = get_new_position(engine->input, player, engine->_delta, 3);
            entity_set_position(player, pos.x, pos.y);
        }

        if (player->position.x < 0) {
            player->position.x = 0;
        }
        if (player->position.y < 0) {
            player->position.y = 0;
        }
        if (player->position.x > engine->width - player->sprite->texture->width) {
            player->position.x = engine->width - player->sprite->texture->width;
        }
        if (player->position.y > engine->height - player->sprite->texture->height) {
            player->position.y = engine->height - player->sprite->texture->height;
        }

        if (engine->input->keys_up[SDL_SCANCODE_ESCAPE]) {
            engine->is_running = 0;
        }

        // Change weapons
        if (engine->input->keys_up[SDL_SCANCODE_1]) {
            player_state.weapon = WEAPON_PISTOL;
        }
        if (engine->input->keys_up[SDL_SCANCODE_2]) {
            player_state.weapon = WEAPON_RIFLE;
        }
        if (engine->input->keys_up[SDL_SCANCODE_3]) {
            player_state.weapon = WEAPON_MACHINE;
        }

        if (player_state.game_state == GAME_STATE_END) {
            if (engine->input->keys_up[SDL_SCANCODE_Q]) {
                engine->is_running = 0;
            }
            if (engine->input->keys_up[SDL_SCANCODE_SPACE]) {
                init_play();
            }
        }

        if (player_state.game_state == GAME_STATE_HELP) {
            if (engine->input->keys_up[SDL_SCANCODE_SPACE]) {
                player_state.game_state = GAME_STATE_PLAY;
            }
        }
}

void update_bullet_hits() {
    for (int i = 0; i < BULLETS_COUNT; i++) {
        Entity* bullet = &bullets[i];
        BulletProperties *bp = bullet->properties;

        if (!bp->is_flying) {
            continue;
        }

        if (bp->is_enemy) {
            if (entity_has_collision(bullet, &player->rects[0])) {
                reset_bullet_properties(bp);
                bp->is_flying = 0;
                player_state.health -= get_rand_int(PLAYER_DAMAGE_MIN, PLAYER_DAMAGE_MAX);
                entity_set_position(bullet, -100, -100);
            }

            continue;
        }

        for (int j = 0; j < ENEMIES_COUNT; j++) {
            Entity *enemy = &enemies[j];
            EnemyProperties *ep = enemy->properties;

            if (!ep->is_active) {
                continue;
            }

            if (entity_has_collision(enemy, &bullet->rects[0])) {
                bp->is_flying = 0;

                int damage = 0;

                if (bp->strength == WEAPON_PISTOL) {
                    damage = get_rand_int(PISTOL_DAMAGE_MIN, PISTOL_DAMAGE_MAX);
                } else if (bp->strength == WEAPON_RIFLE) {
                    damage = get_rand_int(RIFLE_DAMAGE_MIN, RIFLE_DAMAGE_MAX);
                } else if (bp->strength == WEAPON_MACHINE) {
                    damage = get_rand_int(MACHINE_DAMAGE_MIN, MACHINE_DAMAGE_MAX);
                }

                ep->health -= damage;
            }
        }
    }
}

void handle_game_end() {
    if (player_state.health < 0) {
        player_state.game_state = GAME_STATE_END;
    }
}

void render_game(Engine *engine) {
    timer.total_time += engine->_delta;
    player_state.since_last_shot += engine->_delta;

    rotate_player(player, engine->input);

    render_level(engine, level);

    handle_input(engine);

    if (player_state.health > 0) {
        entity_render(player, engine->renderer, engine->_delta);
    }

    bullets_render(engine);
    enemies_render(engine, player);

    update_bullet_hits();

    enemies_update(engine, player, &player_state, &timer);

    render_hud(engine);

    handle_game_end();
}

void render_hud(Engine *engine) {
    cursor->position.x = engine->input->mouse_x - cursor->texture->width / 2;
    cursor->position.y = engine->input->mouse_y - cursor->texture->height / 2;

    // Player's health
    if (player_state.health > 0) {
        SDL_SetRenderDrawColor(engine->renderer, 255, 255, 255, 255);
        SDL_Rect rect_health;

        rect_health.x = 20;
        rect_health.y = 20;
        rect_health.w = player_state.health * 2;
        rect_health.h = 30;

        SDL_RenderDrawRect(engine->renderer, &rect_health);

        if (player_state.health >= 75) {
            SDL_SetRenderDrawColor(engine->renderer, 106, 176, 76, 255);
        } else if (player_state.health >= 30) {
            SDL_SetRenderDrawColor(engine->renderer, 240, 147, 43, 255);
        } else {
            SDL_SetRenderDrawColor(engine->renderer, 235, 77, 75, 255);
        }

        rect_health.x += 1;
        rect_health.y += 1;
        rect_health.w -= 2;
        rect_health.h -= 2;

        SDL_RenderFillRect(engine->renderer, &rect_health);
    }

    // Gun
    sprite_render(text_gun->sprite, engine->renderer);

    if (player_state.game_state == GAME_STATE_PLAY) {
        player_state.survival_time += engine->_delta;
    }

    char str_kills[100];
    sprintf(str_kills, "Kills: %d", player_state.kills);
    text_set(text_kills, engine->renderer, str_kills);

    char str_time[100];
    int time_s = player_state.survival_time / 1000;
    int time_ds = ((int)player_state.survival_time % 1000) / 100;

    sprintf(str_time, "Survival time: %d.%d", time_s, time_ds);
    text_set(text_time, engine->renderer, str_time);

    char str_weapon[100];
    if (player_state.weapon == WEAPON_PISTOL) {
        strcpy(str_weapon, "Pistol");
    } else if (player_state.weapon == WEAPON_RIFLE) {
        strcpy(str_weapon, "Rifle");
    } else if (player_state.weapon == WEAPON_MACHINE) {
        strcpy(str_weapon, "Machine gun");
    }

    text_set(text_gun, engine->renderer, str_weapon);
    text_gun->sprite->position.x = engine->width - text_gun->sprite->texture->width - 20;
    text_gun->sprite->position.y = 10;


    // Stats
    sprite_render(text_kills->sprite, engine->renderer);
    sprite_render(text_time->sprite, engine->renderer);

    if (player_state.game_state == GAME_STATE_END) {
        sprite_render(text_game_end->sprite, engine->renderer);
    } else if (player_state.game_state == GAME_STATE_HELP) {
        for (int i = 0; i < TEXT_HELP_COUNT; i++) {
            if (text_help[i]->sprite) {
                sprite_render(text_help[i]->sprite, engine->renderer);
            }
        }
    }

    sprite_render(cursor, engine->renderer);
}

void engine_loop() {
    engine_clear(engine);

    read_input(engine->input);
    render_engine(engine);

    engine_present(engine);
}

void setup_loading_screen()
{
    text_loading = text_new(engine->preloader->font_loader->fonts[0]);
    text_set(text_loading, engine->renderer, "Alive: the game | Loading: 00%");
    text_loading->sprite->position.x = engine->width / 2 - text_loading->sprite->texture->width / 2;
    text_loading->sprite->position.y = engine->height / 2 - text_loading->sprite->texture->height / 2;

    engine->state = ENGINE_LOADING;
}


void setup_game()
{
    Mix_VolumeMusic(MIX_MAX_VOLUME * MUSIC_VOLUME);
    Mix_PlayMusic(engine->loader->music_loader->music[MUSIC_1], -1);

    SDL_ShowCursor(SDL_DISABLE);
    cursor = sprite_new(engine->loader->texture_loader->textures[TEX_CROSSHAIR]);

    level = build_level_1(engine);

    player = entity_new();
    Sprite *sprite = sprite_new(engine->loader->texture_loader->textures[SURVIVOR_GUN]);
    player->sprite = sprite;
    sprite->rotation = 0;

    init_enemies(engine);

    for (int i = 0; i < BULLETS_COUNT; i++) {
        Sprite *sprite = sprite_new(engine->loader->texture_loader->textures[TEX_BULLET]);

        Entity *e = &bullets[i];

        e->sprite = sprite;
        e->properties = &bp[i];
    }

    init_play();

    text_gun = text_new(engine->loader->font_loader->fonts[0]);
    text_set(text_gun, engine->renderer, "pistol");
    text_gun->sprite->position.x = engine->width - text_gun->sprite->texture->width - 20;
    text_gun->sprite->position.y = 10;

    text_game_end = text_new(engine->loader->font_loader->fonts[0]);
    text_set(text_game_end, engine->renderer, "You lost. Play again? (yes - space/no - q)");
    text_game_end->sprite->position.x = engine->width / 2 - text_game_end->sprite->texture->width / 2;
    text_game_end->sprite->position.y = engine->height / 2 - text_game_end->sprite->texture->height / 2;


    text_time = text_new(engine->loader->font_loader->fonts[0]);
    text_set(text_time, engine->renderer, "Survival time:");
    text_time->sprite->position.x = 280;
    text_time->sprite->position.y = 10;

    text_kills = text_new(engine->loader->font_loader->fonts[0]);
    text_set(text_kills, engine->renderer, "Kills:");
    text_kills->sprite->position.x = 600;
    text_kills->sprite->position.y = 10;

    for (int i = 0; i < TEXT_HELP_COUNT; i++) {
        text_help[i] = text_new(engine->loader->font_loader->fonts[0]);
    }

    text_set(text_help[0], engine->renderer, "Alive: the game");
    text_help[0]->sprite->position.x = 300;
    text_help[0]->sprite->position.y = 200;
    text_set(text_help[1], engine->renderer, "WASD - movement");
    text_help[1]->sprite->position.x = 300;
    text_help[1]->sprite->position.y = 250;
    text_set(text_help[2], engine->renderer, "Mouse - change direction/target, Mouse left button - shoot");
    text_help[2]->sprite->position.x = 300;
    text_help[2]->sprite->position.y = 300;
    text_set(text_help[3], engine->renderer, "1, 2, 3 - select weapons");
    text_help[3]->sprite->position.x = 300;
    text_help[3]->sprite->position.y = 350;
    text_set(text_help[4], engine->renderer, "Press space to start, esc to quit");
    text_help[4]->sprite->position.x = 300;
    text_help[4]->sprite->position.y = 400;
    text_set(text_help[5], engine->renderer, "Visit https://lchsk.com and https://github.com/lchsk");
    text_help[5]->sprite->position.x = 300;
    text_help[5]->sprite->position.y = 450;

    player_state.game_state = GAME_STATE_HELP;

    engine->state = ENGINE_RUNNING;
}

void free_game() {
    entity_free(player);

    text_free(text_gun);
    text_free(text_game_end);
    text_free(text_kills);
    text_free(text_time);
    text_free(text_loading);

    sprite_free(cursor);

    for (int i = 0; i < TEXT_HELP_COUNT; i++) {
        text_free(text_help[i]);
    }
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
    srand(time(0));

    Loader *loader = loader_new(
        TEXTURES_COUNT
        , FONTS_COUNT, MUSIC_COUNT, SOUNDS_COUNT);
    Loader *preloader = loader_new(0, PRELOADER_FONTS_COUNT, 0, 0);

    loader->texture_loader->paths = images;
    loader->music_loader->paths = music;
    loader->sound_loader->paths = sounds;
    loader->font_loader->paths = fonts;
    loader->font_loader->sizes = fonts_sizes;

    preloader->texture_loader = NULL;
    preloader->music_loader = NULL;
    preloader->sound_loader = NULL;
    preloader->font_loader->paths = preloader_fonts;
    preloader->font_loader->sizes = preloader_fonts_sizes;

    engine = engine_new(1280, 768, "alive game");
    engine->requested_fps = 25;
    engine->loader = loader;
    engine->preloader = preloader;

    run_main_loop();

    free_game();
    loader_free(loader);
    engine_free(engine);
}
