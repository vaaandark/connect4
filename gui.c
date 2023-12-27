#include "gui.h"

#ifdef USING_SDL
    #include "sdl_wrapper.h"
#else
#endif

void gui_init(void) {
#ifdef USING_SDL
    sdl_init();
#else
#endif
}

void gui_update(void) {
#ifdef USING_SDL
    sdl_sync();
#else
#endif
}

void gui_draw_circle(int x, int y, int r, int color) {
#ifdef USING_SDL
    sdl_draw_circle(x, y, r, color);
#else
#endif
}

void gui_draw_rect(int x, int y, int w, int h, int color) {
#ifdef USING_SDL
    sdl_draw_rectangle(x, y, w, h, color);
#else
#endif
}
