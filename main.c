#define USING_SDL
#ifdef USING_SDL
    #include <SDL2/SDL_events.h>
    #include <SDL2/SDL_timer.h>
#endif

#include <stdbool.h>
#include <stdio.h>

#include "board.h"
#include "common/common.h"
#include "gui.h"

#define STATUS_BAR_START SCREEN_HEIGHT
#define STATUS_BAR_WIDTH SCREEN_WIDTH - SCREEN_HEIGHT
#define STATUS_BAR_HEIGHT SCREEN_HEIGHT

int column_to_put = 0;

int main(void) {
    gui_init();
    gui_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    gui_draw_rect(0, GRID_SIZE, SCREEN_HEIGHT, SCREEN_HEIGHT - GRID_SIZE, BLUE);
    board_init();
    board_draw();
    gui_update();
    bool quit = false;
    int x;
    int unput_coin;
    int turn = 1;
    GameStatus s;
#ifdef USING_SDL
    SDL_Event event;
    while (!quit) {
        SDL_PollEvent(&event);
        CoinBelonging c = turn % 2 ? Mine : Oppo;
        int color = c == Mine ? RED : YELLOW;
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_MOUSEMOTION) {
            x = event.motion.x;
            if (x >= 0 && x < 7 * GRID_SIZE) {
                column_to_put = x / GRID_SIZE;
            }
            if (x < 0.5 * (int)GRID_SIZE) {
                unput_coin = 0.5 * (int)GRID_SIZE;
            } else if (x > 6.5 * (int)GRID_SIZE) {
                unput_coin = 6.5 * (int)GRID_SIZE;
            } else {
                unput_coin = x;
            }
            gui_draw_rect(0, 0, 7 * GRID_SIZE, GRID_SIZE, BLACK);
            gui_draw_circle(unput_coin, GRID_SIZE / 2, COIN_SIZE, color);
            gui_update();
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int y;
            if (board_put_coin(column_to_put, &y, c) != 0) {
                continue;
            }
            s = board_check_game_status(column_to_put, y);
            board_draw();
            gui_update();
            ++turn;
        }
        if (s != NotYet) {
            if (s == Won) {
                puts("Won!");
            } else {
                puts("Lost!");
            }
            break;
        }
    }
#endif
    return 0;
}