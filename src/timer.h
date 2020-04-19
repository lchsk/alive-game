#ifndef TIMER_H
    #define TIMER_H


typedef struct GameTimer {
    // In ms
    unsigned int total_time;
    int since_last_enemy_launched;
} GameTimer;


    #endif /* TIMER_H */
