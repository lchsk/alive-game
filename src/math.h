#ifndef MATH_H
    #define MATH_H

Pointd get_pos(Input *input, Entity *entity);
Pointd get_pos_entities(Pointd target, Entity *source);
Pointd get_new_position(Input *input, Entity *entity, double delta,
                        int direction);

double get_distance(Pointd p1, Pointd p2);

double rad_to_deg(double rad);

Pointd rotate(Pointd v, double angle);

    #endif /* MATH_H */
