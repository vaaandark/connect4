#ifndef SDL_WRAPPER_H_
#define SDL_WRAPPER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "common/common.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern int draw_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void sdl_sync();
void sdl_init();
void sdl_drop();

void sdl_draw_rectangle(int x, int y, int w, int h, int color);
void sdl_draw_circle(int x, int y, int r, int color);

#endif
