#ifndef BOARD_H_
#define BOARD_H_

#define GRID_VERTICAL_COUNT 6
#define GRID_HORIZONTAL_COUNT 7
#define GRID_SIZE (SCREEN_HEIGHT / 7)
#define COIN_SIZE (GRID_SIZE / 2 - 2)

// 棋子归属
typedef enum {
    Mine,
    Oppo,
    None,
} CoinBelonging;

// 棋盘
extern CoinBelonging board[GRID_VERTICAL_COUNT][GRID_HORIZONTAL_COUNT];

// 初始化棋盘
void board_init(void);

// 落子
// 返回 0 说明落子成功
// - `x`: 释放棋子的列（横坐标）
// - `y`: 棋子最终落到位置的纵坐标
// - `c`: 棋子的归属
int board_put_coin(int x, int *y, CoinBelonging c);

// 游戏状态
typedef enum {
    Won, // 赢了
    Lost, // 输了
    NotYet, // 尚未结束
} GameStatus;

// 检查游戏状态
// 从上一次落子的位置开始检查
GameStatus board_check_game_status(int x, int y);

// 绘制棋盘
void board_draw(void);

#endif