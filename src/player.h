#ifndef PLAYER_H
    #define PLAYER_H


typedef enum {
    GAME_STATE_HELP,
    GAME_STATE_PLAY,
    GAME_STATE_END
} GameState;

typedef enum {
    WEAPON_PISTOL,
    WEAPON_RIFLE,
    WEAPON_MACHINE,
} Weapon;

typedef struct PlayerState {
    double health;
    GameState game_state;
    double survival_time;
    int kills;
    Weapon weapon;
    double since_last_shot;
} PlayerState;

void rotate_player(Entity* player, Input *input);

    #endif /* PLAYER_H */
