#include "board.h"

#include <stdbool.h>

#include "common/common.h"
#include "gui.h"

CoinBelonging board[GRID_VERTICAL_COUNT][GRID_HORIZONTAL_COUNT];

void board_init(void) {
    for (int i = 0; i < GRID_VERTICAL_COUNT; ++i) {
        for (int j = 0; j < GRID_HORIZONTAL_COUNT; ++j) {
            board[i][j] = None;
            gui_draw_circle(
                (j + 0.5) * (int)GRID_SIZE,
                (i + 1.5) * (int)GRID_SIZE,
                COIN_SIZE,
                BLACK);
        }
    }
}

int board_put_coin(int x, int *y, CoinBelonging c) {
    for (int i = GRID_VERTICAL_COUNT - 1; i >= 0; --i) {
        if (board[i][x] == None) {
            *y = i;
            board[i][x] = c;
            return 0;
        }
    }
    return 1;
}

static bool inside_board(int x, int y) {
    return x >= 0 && x < GRID_HORIZONTAL_COUNT && y >= 0
        && y < GRID_VERTICAL_COUNT;
}

static int count_in_direction(int x, int y, int dx, int dy) {
    int count = 0;
    CoinBelonging c = board[y][x];
    for (int i = 1;; ++i) {
        int xx = x + dx * i;
        int yy = y + dy * i;
        if (!inside_board(xx, yy)) {
            break;
        }
        if (board[yy][xx] == c) {
            count += 1;
        } else {
            break;
        }
    }
    return count;
}

GameStatus board_check_game_status(int x, int y) {
    CoinBelonging c = board[y][x];
    GameStatus won_or_lost = c == Mine ? Won : Lost;
    int left_count = count_in_direction(x, y, -1, 0);
    int right_count = count_in_direction(x, y, 1, 0);
    if (left_count + right_count >= 3) {
        return won_or_lost;
    }
    int up_count = count_in_direction(x, y, 0, -1);
    int down_count = count_in_direction(x, y, 0, 1);
    if (up_count + down_count >= 3) {
        return won_or_lost;
    }
    int left_up_count = count_in_direction(x, y, -1, -1);
    int right_down_count = count_in_direction(x, y, 1, 1);
    if (left_up_count + right_down_count >= 3) {
        return won_or_lost;
    }
    int left_down_count = count_in_direction(x, y, -1, 1);
    int right_up_count = count_in_direction(x, y, 1, -1);
    if (left_down_count + right_up_count >= 3) {
        return won_or_lost;
    }
    return NotYet;
}

void board_draw(void) {
    for (int i = 0; i < GRID_VERTICAL_COUNT; ++i) {
        for (int j = 0; j < GRID_HORIZONTAL_COUNT; ++j) {
            int color;
#ifdef USING_SDL
            switch (board[i][j]) {
                case Mine:
                    color = RED;
                case Oppo:
                    color = YELLOW;
                default:
                    color = BLACK;
                    break;
            }
            gui_draw_circle(
                (j + 0.5) * (int)GRID_SIZE,
                (i + 1.5) * (int)GRID_SIZE,
                COIN_SIZE,
                color);
#else
            if (board[i][j] == None) {
                color = BLACK;
                gui_draw_circle(
                    (j + 0.5) * (int)GRID_SIZE,
                    (i + 1.5) * (int)GRID_SIZE,
                    COIN_SIZE,
                    color);
            } else {
                fb_image *img = board[i][j] == Mine ? red_coin : yellow_coin;
                gui_draw_image(
                    j * (int)GRID_SIZE,
                    (i + 1) * (int)GRID_SIZE,
                    img,
                    0);
            }
#endif
        }
    }
}
