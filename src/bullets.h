#ifndef BULLETS_H
    #define BULLETS_H

#define BULLETS_COUNT 1000

extern Entity bullets[BULLETS_COUNT];


typedef struct BulletProperties {
    int is_flying;
    double speed_x;
    double speed_y;

    double speed_factor;
    int is_enemy;
    int strength;
} BulletProperties;

extern BulletProperties bp[BULLETS_COUNT];

Entity* get_free_bullet();
void reset_bullet_properties(BulletProperties *bp);
void reset_bullets();

void bullets_render(Engine *engine);

    #endif /* BULLETS_H */
