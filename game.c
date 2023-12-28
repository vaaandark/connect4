#ifdef USING_SDL
    #include <SDL2/SDL_events.h>
    #include <SDL2/SDL_timer.h>
#endif

#include <stdbool.h>
#include <stdio.h>

#include "board.h"
#include "comm.h"
#include "common/common.h"
#include "game.h"
#include "gui.h"

GameMode mode;
Turn turn = YourTurn;

bool to_restart = false;

int sock_fd;

int win_cnt = 0;
int lose_cnt = 0;

bool quit = false;
int unput_coin;
int column_to_put = 0;
GameStatus game_status = NotYet;
#ifndef USING_SDL
int touch_fd;
#endif

#ifndef USING_SDL
static void show_turn(void) {
    gui_draw_rect(SCREEN_HEIGHT, 130, 50, 150, BLACK);
    if (turn == YourTurn) {
        gui_draw_image(SCREEN_HEIGHT, 130, red_arrow, 0);
    } else {
        gui_draw_image(SCREEN_HEIGHT, 200, yellow_arrow, 0);
    }
}

static void show_score(void) {
    gui_draw_rect(SCREEN_HEIGHT + 200, 130, 50, 125, BLACK);
    char buff[5];
    sprintf(buff, "%d", win_cnt);
    fb_draw_text(SCREEN_HEIGHT + 200, 180, buff, 48, RED);
    sprintf(buff, "%d", lose_cnt);
    fb_draw_text(SCREEN_HEIGHT + 200, 248, buff, 48, YELLOW);
}
#endif

#ifndef USING_SDL
static void sock_event(int fd) {
    if (to_restart) {
        return;
    }
    int y;
    recv_data(sock_fd, &column_to_put, sizeof(int));
    if (turn == YourTurn) {
        return;
    }
    board_put_coin(column_to_put, &y, Oppo);
    game_status = board_check_game_status(column_to_put, y);
    board_draw();
    turn = !turn;
    show_turn();
    gui_update();
    fb_image *img;
    if (game_status != NotYet) {
        if (game_status == Won) {
            img = win;
            win_cnt += 1;
            puts("Won!");
        } else {
            img = lose;
            lose_cnt += 1;
            puts("Lost!");
        }
        gui_draw_image(0, 0, img, 0);
        gui_update();
        to_restart = true;
        game_status = NotYet;
    }
}

static void touch_event(int fd) {
    int type, x, _y, finger;
    CoinBelonging c = turn == YourTurn ? Mine : Oppo;
    fb_image *img = c == Mine ? red_coin : yellow_coin;
    int y;
    type = touch_read(fd, &x, &_y, &finger);
    if (to_restart) {
        if (type == TOUCH_RELEASE) {
            gui_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            gui_draw_rect(
                0,
                GRID_SIZE,
                SCREEN_HEIGHT,
                SCREEN_HEIGHT - GRID_SIZE,
                BLUE);
            board_init();
            gui_draw_image(SCREEN_HEIGHT, 0, sidebar, 0);
            show_turn();
            show_score();
            gui_update();
            to_restart = false;
        }
        return;
    }
    if (mode != Hotseat && turn == NotYourTurn) {
        return;
    }
    if (type == TOUCH_ERROR) {
        printf("close touch fd\n");
        close(fd);
        task_delete_file(fd);
    }
    switch (type) {
        case TOUCH_PRESS:
        case TOUCH_MOVE:
            if (x >= 0 && x < 7 * GRID_SIZE) {
                column_to_put = x / GRID_SIZE;
            }
            unput_coin = x - 0.5 * (int)GRID_SIZE;
            if (unput_coin < 0) {
                unput_coin = 0;
            } else if (unput_coin > 6 * (int)GRID_SIZE) {
                unput_coin = 6 * (int)GRID_SIZE;
            }
            gui_draw_rect(0, 0, 7 * GRID_SIZE, GRID_SIZE, BLACK);
            gui_draw_image(unput_coin, 0, img, 0);
            break;
        case TOUCH_RELEASE:
            if (board_put_coin(column_to_put, &y, c) != 0) {
                break;
            }
            send_data(sock_fd, &column_to_put, sizeof(int));
            game_status = board_check_game_status(column_to_put, y);
            board_draw();
            gui_draw_rect(0, 0, 7 * GRID_SIZE, GRID_SIZE, BLACK);
            turn = !turn;
            show_turn();
            break;
        default:
            return;
    }
    gui_update();
    if (game_status != NotYet) {
        fb_image *img;
        if (game_status == Won) {
            img = win;
            win_cnt += 1;
            puts("Won!");
        } else {
            img = lose;
            lose_cnt += 1;
            puts("Lost!");
        }
        gui_draw_image(0, 0, img, 0);
        gui_update();
        to_restart = true;
        game_status = NotYet;
    }
    return;
}
#endif

void game_init(GameMode game_mode, char *ip, char *port) {
    switch (game_mode) {
        case Host:
            sock_fd = host_get_sock_fd(ip);
            turn = YourTurn;
            break;
        case Guest:
            sock_fd = guest_get_sock_fd(ip, port);
            turn = NotYourTurn;
            break;
        default:
            break;
    }
    mode = game_mode;
    gui_init();
    gui_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    gui_draw_rect(0, GRID_SIZE, SCREEN_HEIGHT, SCREEN_HEIGHT - GRID_SIZE, BLUE);
    board_init();
    board_draw();
#ifndef USING_SDL
    gui_draw_image(SCREEN_HEIGHT, 0, sidebar, 0);
    show_turn();
    show_score();
#endif
    gui_update();
}

void game_loop() {
#ifdef USING_SDL
    SDL_Event event;
    int x, y;
    while (!quit) {
        CoinBelonging c = turn == YourTurn ? Mine : Oppo;
        if (mode != Hotseat && turn == NotYourTurn) {
            recv_data(sock_fd, &column_to_put, sizeof(int));
            board_put_coin(column_to_put, &y, c);
            game_status = board_check_game_status(column_to_put, y);
            board_draw();
            turn = !turn;
            continue;
        } else {
            SDL_PollEvent(&event);
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
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (board_put_coin(column_to_put, &y, c) != 0) {
                    continue;
                }
                send_data(sock_fd, &column_to_put, sizeof(int));
                game_status = board_check_game_status(column_to_put, y);
                board_draw();
                gui_draw_rect(0, 0, 7 * GRID_SIZE, GRID_SIZE, BLACK);
                turn = !turn;
            }
        }
        gui_update();
        if (game_status != NotYet) {
            if (game_status == Won) {
                puts("Won!");
            } else {
                puts("Lost!");
            }
            break;
        }
    }
#else
    touch_fd = touch_init("/dev/input/event0");
    task_add_file(touch_fd, touch_event);
    touch_fd = touch_init("/dev/input/event1");
    task_add_file(touch_fd, touch_event);
    touch_fd = touch_init("/dev/input/event2");
    task_add_file(touch_fd, touch_event);
    if (mode != Hotseat) {
        task_add_file(sock_fd, sock_event);
    }
    task_loop();
#endif
}
