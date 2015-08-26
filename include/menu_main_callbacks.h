#ifndef _MENU_MAIN_CALLBACKS_H
#define _MENU_MAIN_CALLBACKS_H
#include "global.h"
#include "mpd.h"

void menu_main_cb_play(void);
void menu_main_cb_pause(void);
void menu_main_cb_stop(void);
void menu_main_cb_prev(void);
void menu_main_cb_next(void);
void menu_main_cb_voldn(void);
void menu_main_cb_volup(void);
void menu_main_cb_repeat(void);

//These are in main.c
extern MPD_STATUS mpd_state;
extern MPD_SONG_INFO current_song;

#endif