#include <string.h>

#include "game.h"

int main(int argc, char *argv[]) {
    GameMode game_mode;
    char *ip = NULL;
    char *port = NULL;
    if (argc == 3 && strcmp(argv[1], "host") == 0) {
        game_mode = Host;
        ip = argv[2];
    } else if (argc == 4 && strcmp(argv[1], "guest") == 0) {
        game_mode = Guest;
        ip = argv[2];
        port = argv[3];
    } else {
        game_mode = Hotseat;
    }
    game_init(game_mode, ip, port);
    game_loop();
    return 0;
}
