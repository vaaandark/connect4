#ifndef GUI_H_
#define GUI_H_

#ifndef USING_SDL
    #include "common/common.h"
extern fb_image *start;
extern fb_image *win;
extern fb_image *lose;
extern fb_image *red_coin;
extern fb_image *yellow_coin;
#endif

void gui_init(void);
void gui_update(void);

void gui_draw_circle(int x, int y, int r, int color);
void gui_draw_rect(int x, int y, int w, int h, int color);

#ifndef USING_SDL
void gui_draw_image(int x, int y, fb_image *img, int color);
#endif

#endif
