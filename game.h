#ifndef GAME_H_
#define GAME_H_

typedef enum {
    Hotseat,
    Host,
    Guest,
} GameMode;

typedef enum {
    YourTurn = 0,
    NotYourTurn = 1,
} Turn;

void game_init(GameMode game_mode, char *ip, char *port);

void game_loop();

#endif