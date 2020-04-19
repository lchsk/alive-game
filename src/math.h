#ifndef MATH_H
    #define MATH_H

Pointd get_pos(Input *input, Entity *entity);
Pointd get_new_position(Input *input, Entity *entity, double delta,
                        int direction);

double rad_to_deg(double rad);




    #endif /* MATH_H */
