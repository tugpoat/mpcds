#ifndef _MENU_MAIN_H
#define _MENU_MAIN_H

#include "global.h"
#include "menu_main_callbacks.h"

#include "button.h"

//button graphics information
#include "background.h"
#include "btn_play.h"
#include "btn_pause.h"
#include "btn_stop.h"
#include "btn_prev.h"
#include "btn_next.h"
#include "btn_voldn.h"
#include "btn_volup.h"
#include "btn_repeat.h"

//menu functions, could be callbacks when implemented in a GUI lib or something
void menu_main_touchdown(touchPosition *touch);
void menu_main_touchup(touchPosition *touch);
void menu_main_init(void);
void menu_main_update(void);
void menu_main_redraw(void);
void menu_main_destroy(void);

//button stuff, possibly events or something if implemented in a GUI lib
void initbutton(BUTTON *butt, u8 *gfx);
void initbutton_new(BUTTON *butt, u8 *gfx, const char *name, const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h, void (*callback)(void), const unsigned short int initstate);
void destroybutton(BUTTON *butt);

//actual button stuff
#define NUM_BUTTONS 8
BUTTON (*menu_buttons)[NUM_BUTTONS];

#endif