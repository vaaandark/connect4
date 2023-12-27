#include "sdl_wrapper.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>

#include "common/common.h"

#define MAX_TEXT 100

SDL_Window *window;
SDL_Renderer *renderer;
int draw_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void sdl_init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "connect4",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

static void sdl_set_color(int color) {
    SDL_SetRenderDrawColor(
        renderer,
        color >> 16 & 0xff,
        color >> 8 & 0xff,
        color & 0xff,
        0xff);
}

void sdl_draw_rectangle(int x, int y, int w, int h, int color) {
    if (x < 0) {
        w += x;
        x = 0;
    }
    if (x + w > SCREEN_WIDTH) {
        w = SCREEN_WIDTH - x;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (y + h > SCREEN_HEIGHT) {
        h = SCREEN_HEIGHT - y;
    }
    if (w <= 0 || h <= 0)
        return;
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            draw_buffer[y + j][x + i] = color;
        }
    }
    return;
}

void sdl_draw_circle(int x, int y, int r, int color) {
    x = x < 0 ? 0 : x;
    x = x >= SCREEN_WIDTH ? SCREEN_WIDTH - 1 : x;
    y = y < 0 ? 0 : y;
    y = y >= SCREEN_HEIGHT ? SCREEN_HEIGHT - 1 : y;
    int x_lower_bound = x - r < 0 ? 0 : x - r;
    int x_upper_bound = x + r >= SCREEN_WIDTH ? SCREEN_WIDTH - 1 : x + r;
    int y_lower_bound = y - r < 0 ? 0 : y - r;
    int y_upper_bound = y + r >= SCREEN_HEIGHT ? SCREEN_HEIGHT - 1 : y + r;
    for (int i = y_lower_bound; i <= y_upper_bound; ++i) {
        for (int j = x_lower_bound; j <= x_upper_bound; ++j) {
            int delta_x = j - x;
            int delta_y = i - y;
            if (delta_x * delta_x + delta_y * delta_y <= r * r) {
                draw_buffer[i][j] = color;
            }
        }
    }
}

void sdl_sync() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            int color = draw_buffer[i][j];
            sdl_set_color(color);
            SDL_RenderDrawPoint(renderer, j, i);
        }
    }
    SDL_RenderPresent(renderer);
}

void sdl_drop() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
