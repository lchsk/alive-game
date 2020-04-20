#ifndef ENEMIES_H
#define ENEMIES_H

#include "timer.h"
#include "player.h"

#define ENEMIES_COUNT 100

extern Entity enemies[ENEMIES_COUNT];

typedef struct EnemyProperties {
    int is_active;
    double speed_x;
    double speed_y;
    double health;
    int since_last_shot;
} EnemyProperties;

extern EnemyProperties ep[ENEMIES_COUNT];

void init_enemies(Engine* engine);
void enemies_render(Engine* engine, Entity* player);
void reset_enemy_properties(EnemyProperties* ep);

void reset_enemies(Engine* engine);

void enemies_update(Engine* Engine,
                    Entity* player,
                    PlayerState* player_state,
                    GameTimer* timer);

/* Entity* get_free_bullet(); */
/* void reset_bullet_properties(BulletProperties *bp); */

/* void bullets_render(Engine *engine); */

#endif /* ENEMIES_H */
