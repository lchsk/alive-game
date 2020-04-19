#include <engine212/engine.h>

#include "player.h"
#include "math.h"

void rotate_player(Entity* player, Input *input) {
    double y_diff = input->mouse_y - player->position.y;
    double x_diff = input->mouse_x - player->position.x;

    double a = rad_to_deg(atan2(y_diff, x_diff));
    player->sprite->rotation = a;
}
