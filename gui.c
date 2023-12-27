#include "gui.h"

#ifdef USING_SDL
    #include "sdl_wrapper.h"
#else
    #include "common/common.h"
#endif

void gui_init(void) {
#ifdef USING_SDL
    sdl_init();
#else
    fb_init("/dev/fb0");
#endif
}

void gui_update(void) {
#ifdef USING_SDL
    sdl_sync();
#else
    fb_update();
#endif
}

void gui_draw_circle(int x, int y, int r, int color) {
#ifdef USING_SDL
    sdl_draw_circle(x, y, r, color);
#else
    fb_draw_circle(x, y, r, color);
#endif
}

void gui_draw_rect(int x, int y, int w, int h, int color) {
#ifdef USING_SDL
    sdl_draw_rectangle(x, y, w, h, color);
#else
    fb_draw_rect(x, y, w, h, color);
#endif
}
