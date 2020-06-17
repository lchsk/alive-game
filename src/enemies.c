#include <cunctator/engine.h>

#include "assets.h"
#include "bullets.h"
#include "config.h"
#include "enemies.h"
#include "math.h"
#include "player.h"

Entity enemies[ENEMIES_COUNT];
EnemyProperties ep[ENEMIES_COUNT];

int get_enemy() {
    int r = get_rand_int(0, 100);

    if (r >= 0 && r < 10) {
        return HITMAN_SILENCER;
    } else if (r >= 10 && r < 20) {
        return MAN_BLUE_GUN;
    } else if (r >= 20 && r < 30) {
        return MAN_BROWN_MACHINE;
    } else if (r >= 30 && r < 55) {
        return MAN_OLD_MACHINE;
    } else if (r >= 55 && r < 70) {
        return SOLDIER1_MACHINE;
    } else if (r >= 70 && r < 80) {
        return WOMAN_GREEN_SILENCER;
    }

    return ZOMBIE1_GUN;
}

void init_enemies(Engine* engine) {
    for (int i = 0; i < ENEMIES_COUNT; i++) {
        Entity* e = &enemies[i];

        int texture = get_enemy();

        Sprite* sprite =
            sprite_new(engine->loader->texture_loader->textures[texture]);
        e->sprite = sprite;
        e->properties = &ep[i];
    }
}

void reset_enemies(Engine* engine) {
    for (int i = 0; i < ENEMIES_COUNT; i++) {
        Entity* e = &enemies[i];
        entity_set_position(e, -100, -100);
        reset_enemy_properties(e->properties);
        e->rects[0].x = -100;
        e->rects[0].y = -100;
        e->rects[0].w = e->sprite->texture->width;
        e->rects[0].h = e->sprite->texture->height;
        e->rects_sz = 1;
    }
}

void enemies_render(Engine* engine, Entity* player) {
    for (int i = 0; i < ENEMIES_COUNT; i++) {
        Entity* e = &enemies[i];

        EnemyProperties* ep = e->properties;

        if (ep->is_active) {
            Pointd p1 = {player->position.x, player->position.y};
            Pointd p2 = {e->position.x, e->position.y};

            if (get_distance(p1, p2) > PLAYER_ENEMY_MIN_DISTANCE) {
                entity_set_position(
                    e, e->position.x + ep->speed_x * engine->_delta,
                    e->position.y + ep->speed_y * engine->_delta);
            }
            entity_render(e, engine->renderer, engine->_delta);
        }
    }
}

void reset_enemy_properties(EnemyProperties* ep) {
    ep->is_active = 0;
    ep->speed_x = 0;
    ep->speed_y = 0;
    ep->health = 100;
    ep->since_last_shot = 0;
}

Entity* get_free_enemy() {
    for (int i = 0; i < ENEMIES_COUNT; i++) {
        Entity* e = &enemies[i];

        EnemyProperties* ep = e->properties;

        if (!ep->is_active) {
            ep->is_active = 1;
            return e;
        }
    }

    return NULL;
}

void enemies_update(Engine* engine,
                    Entity* player,
                    PlayerState* player_state,
                    GameTimer* timer) {
    if (player_state->game_state != GAME_STATE_PLAY) {
        return;
    }
    timer->since_last_enemy_launched += engine->_delta;

    int launch = get_rand_int(ENEMY_LAUNCH_MIN_TIME, ENEMY_LAUNCH_MAX_TIME);

    if (timer->since_last_enemy_launched > launch) {
        Entity* enemy = get_free_enemy();

        if (enemy) {
            int initial_x = get_rand_int(-100, 1.1 * engine->width);
            int initial_y;

            if (get_rand_int(0, 2) == 0) {
                initial_y = get_rand_int(50, 200) * -1;
            } else {
                initial_y =
                    get_rand_int(1.1 * engine->height, 1.2 * engine->height);
            }

            entity_set_position(enemy, initial_x, initial_y);
            timer->since_last_enemy_launched = 0;
        }
    }

    // Update active enemies directions
    for (int i = 0; i < ENEMIES_COUNT; i++) {
        Entity* e = &enemies[i];

        EnemyProperties* ep = e->properties;

        if (ep->is_active) {
            ep->since_last_shot += engine->_delta;

            // Shoot?
            int shot_interval = get_rand_int(
                ENEMY_SHOT_INTERVAL_MS,
                ENEMY_SHOT_INTERVAL_MS + ENEMY_SHOT_INTERVAL_RANGE);

            if (ep->since_last_shot > shot_interval) {
                Entity* bullet = get_free_bullet();

                if (bullet) {
                    entity_set_position(bullet, e->position.x, e->position.y);
                    bullet->sprite->rotation = e->sprite->rotation + 90;

                    bullet->rects[0].x = bullet->position.x;
                    bullet->rects[0].y = bullet->position.y;
                    bullet->rects[0].w = bullet->sprite->texture->width;
                    bullet->rects[0].h = bullet->sprite->texture->height;
                    bullet->rects_sz = 1;

                    Pointd vec = get_pos_entities(
                        (Pointd){player->position.x, player->position.y}, e);

                    BulletProperties* bp = bullet->properties;
                    bp->speed_x = vec.x * ENEMY_BULLET_SPEED;
                    bp->speed_y = vec.y * ENEMY_BULLET_SPEED;

                    bp->is_enemy = 1;
                    ep->since_last_shot = 0;
                }
            }

            /* double x = player->position.x + (rand() % 200) - 100; */
            double x = player->position.x;
            /* double y = player->position.y + (rand() % 200) - 100; */
            double y = player->position.y;

            double rot = rad_to_deg(atan2(player->position.y - e->position.y,
                                          player->position.x - e->position.x));

            e->sprite->rotation = rot;

            Pointd target = {x, y};

            Pointd speed = get_pos_entities(target, e);

            ep->speed_x = speed.x * ENEMY_SPEED;
            ep->speed_y = speed.y * ENEMY_SPEED;

            if (ep->health < 0) {
                reset_enemy_properties(ep);
                player_state->kills++;
            }
        }
    }
}
