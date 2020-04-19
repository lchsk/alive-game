#include <engine212/engine.h>

#include "math.h"

#define M_PI 3.14159265358979323846


Pointd get_pos(Input *input, Entity *entity) {
            double x_diff = (input->mouse_x - entity->position.x);
            double y_diff = (input->mouse_y - entity->position.y);

                  double m = fmax(fabs(x_diff), fabs(y_diff));

                  double xd = x_diff / m;
                  double yd = y_diff / m;

                  double change_x = xd * 1;
                  double change_y = yd * 1;

                  return (Pointd) {change_x, change_y};
}

Pointd get_new_position(Input *input, Entity *entity, double delta,
                        int direction) {
            double x_diff = (input->mouse_x - entity->position.x);
            double y_diff = (input->mouse_y - entity->position.y);

                  double m = fmax(fabs(x_diff), fabs(y_diff));

                  double xd = x_diff / m;
                  double yd = y_diff / m;

                  double player_x = entity->position.x;
                  double player_y = entity->position.y;

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

double rad_to_deg(double rad) {
    return rad * 180 / M_PI;
}
