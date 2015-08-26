#ifndef _MAIN_H
#define _MAIN_H

#include "mpd.h"

bool connectAP(void);
void get_status(void);
bool connectMPD(void);
bool mpd_checkerror(void);
void get_status(void);
void get_currentsong(void);
void print_status(bool clear_con);

#endif