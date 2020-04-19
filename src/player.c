#include <engine212/engine.h>

#include "player.h"
#include "math.h"

void rotate_player(Entity* player, Input *input) {
    double a = rad_to_deg(atan2(input->mouse_y - player->position.y, input->mouse_x - player->position.x));

    player->sprite->rotation = a;

}
