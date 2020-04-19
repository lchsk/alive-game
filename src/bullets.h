#ifndef BULLETS_H
    #define BULLETS_H

#define BULLETS_COUNT 100

extern Entity bullets[BULLETS_COUNT];


typedef struct BulletProperties {
    int is_flying;
    double speed_x;
    double speed_y;
} BulletProperties;

extern BulletProperties bp[BULLETS_COUNT];

Entity* get_free_bullet();
void reset_bullet_properties(BulletProperties *bp);

void bullets_render(Engine *engine);

    #endif /* BULLETS_H */
