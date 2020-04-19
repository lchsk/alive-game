#include <engine212/engine.h>

#include "config.h"
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

Pointd get_pos_entities(Pointd target, Entity *source) {
            double x_diff = (target.x - source->position.x);
            double y_diff = (target.y - source->position.y);

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

                  double change_x = xd * delta;
                  double change_y = yd * delta;

                  double speed = 1;

                  int dir_x = 1, dir_y = 1;

                  if (direction == 0) {
                      dir_x = dir_y = 1;
                      speed = PLAYER_FORWARD_SPEED;
                  } else if (direction == 1) {
                      speed = PLAYER_SIDE_SPEED;
                      Pointd d = rotate((Pointd) {change_x, change_y}, -90);

                      change_x = d.x;
                      change_y = d.y;
                  } else if (direction == 2) {
                      dir_x = dir_y = -1;
                      speed = PLAYER_BACKWARD_SPEED;
                  } else if (direction == 3) {
                      speed = PLAYER_SIDE_SPEED;
                      Pointd d = rotate((Pointd) {change_x, change_y}, 90);

                      change_x = d.x;
                      change_y = d.y;
                  }

                  player_x += change_x * dir_x * speed;
                  player_y += change_y * dir_y * speed;

                  return (Pointd) {player_x, player_y};

}

double rad_to_deg(double rad) {
    return rad * 180 / M_PI;
}

double get_distance(Pointd p1, Pointd p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

Pointd rotate(Pointd v, double angle) {
    double x = v.x;
    double y = v.y;

    return (Pointd) {
        cos(angle) * x - sin(angle) * y,
            sin(angle) * x + cos(angle) * y
            };
}
