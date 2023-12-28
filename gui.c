#include "gui.h"

#ifdef USING_SDL
    #include "sdl_wrapper.h"
#else
    #include "common/common.h"
#endif

#ifndef USING_SDL
fb_image *start;
fb_image *win;
fb_image *lose;
fb_image *red_coin;
fb_image *yellow_coin;
fb_image *sidebar;
fb_image *red_arrow;
fb_image *yellow_arrow;
#endif

void gui_init(void) {
#ifdef USING_SDL
    sdl_init();
#else
    fb_init("/dev/fb0");
    start = fb_read_png_image("images/start.png");
    win = fb_read_png_image("images/win.png");
    lose = fb_read_png_image("images/lose.png");
    red_coin = fb_read_png_image("images/redcoin85px.png");
    yellow_coin = fb_read_png_image("images/yellowcoin85px.png");
    sidebar = fb_read_png_image("images/sidebar.png");
    red_arrow = fb_read_png_image("images/redarrow.png");
    yellow_arrow = fb_read_png_image("images/yellowarrow.png");
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

#ifndef USING_SDL
void gui_draw_image(int x, int y, fb_image *img, int color) {
    fb_draw_image(x, y, img, color);
}
#endif