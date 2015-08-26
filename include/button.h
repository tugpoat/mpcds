#ifndef _BUTTON_H
#define _BUTTON_H

#include <nds/ndstypes.h> //We're going to need those types

typedef struct
{
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
	
	char name[8];
	u16 *sprite_gfx_mem;
	u8 *frame_gfx;
	//the reason gfx isnt working is because of the above two and that theyir sizes are not accounted for
	unsigned int state:1;
	void (*callback)(void);
} BUTTON;

enum { BTN_UP = 0, BTN_DOWN  = 1};

#endif