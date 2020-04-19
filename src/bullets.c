#include <engine212/engine.h>

#include "bullets.h"

Entity bullets[BULLETS_COUNT];
BulletProperties bp[BULLETS_COUNT];

static void refresh_bullet(Entity *e, Engine *engine);

Entity* get_free_bullet() {
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

void reset_bullet_properties(BulletProperties *bp) {
    bp->is_flying = 0;
    bp->speed_x = 0;
    bp->speed_y = 0;
}

void bullets_render(Engine *engine) {
        for (int i = 0; i < BULLETS_COUNT; i++) {
            Entity *e = &bullets[i];
            BulletProperties *bp = e->properties;

            entity_set_position(e, e->position.x + bp->speed_x * engine->_delta, e->position.y + bp->speed_y * engine->_delta);
            entity_render(e, engine->renderer, engine->_delta);

            refresh_bullet(e, engine);
        }
}

static void refresh_bullet(Entity *e, Engine *engine) {
            Pointd p = e->position;

            if (p.x > 1.1 * engine->width
                || p.x < -100
                || p.y < -100
                || p.y > 1.1 * engine->height) {
                BulletProperties *bp = e->properties;
                bp->is_flying = 0;
            }
}
