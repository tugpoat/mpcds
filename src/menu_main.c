/*
	mpcDS, an MPD controller for the Nintendo DS
    Copyright (C) 2015  Patrick R. Murphy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    Contact:
    Pat Murphy
    pat@retractableneckbeard.com
    pat.murphy@oksuf.org

*/
#include "menu_main.h"

//Initializes a button and its related VRAM space.
void initbutton(BUTTON *butt, u8 *gfx, 
			const char *name, 
			const unsigned int x, const unsigned int y, 
			const unsigned int w, const unsigned int h, 
			void (*callback)(void), const unsigned short int initstate)
{
	strncpy(butt->name, name, sizeof(butt->name));
	butt->x = x;
	butt->y = y;
	butt->w = w;
	butt->h = h;
	butt->state = initstate;
	butt->callback = callback;
	butt->sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	butt->frame_gfx = gfx;
}

//Frees a button's related VRAM space.
void destroybutton(BUTTON *butt)
{
	oamFreeGfx(&oamMain, butt->sprite_gfx_mem);
}

//Constructor-type dealy
void menu_main_init(void)
{
	int idx= 0;
	//Allocate memory to hold all our buttons
	menu_buttons = calloc(NUM_BUTTONS, sizeof(BUTTON));
	//memset(menu_buttons, 0x0, sizeof(BUTTON) * NUM_BUTTONS);
	
	//let's make all of the buttons!
	
	//FIXME: I don't remember if this actually worked. -pat, 2015
	//test new function to reduce clutter in this one
	initbutton(menu_buttons[idx], (u8*) &btn_playTiles, "PLAY", 14, 16, 32, 32, &menu_main_cb_play, BTN_UP);
	
	idx = sizeof(BUTTON);
	initbutton(menu_buttons[idx], (u8*) &btn_pauseTiles, "PAUSE", 52, 16, 32, 32, &menu_main_cb_pause, BTN_UP);
	
	idx = sizeof(BUTTON) * 2;
	initbutton(menu_buttons[idx], (u8*) &btn_stopTiles, "STOP", 90, 16, 32, 32, &menu_main_cb_stop, BTN_UP);
	
	idx = sizeof(BUTTON) * 3;
	initbutton(menu_buttons[idx], (u8*) &btn_prevTiles, "PREV", 32, 47, 32, 32, &menu_main_cb_prev, BTN_UP);
	
	idx = sizeof(BUTTON) * 4;
	initbutton(menu_buttons[idx], (u8*) &btn_nextTiles, "NEXT", 71, 47, 32, 32, &menu_main_cb_next, BTN_UP);
	
	idx =  sizeof(BUTTON) * 5;
	initbutton(menu_buttons[idx], (u8*) &btn_voldnTiles, "VOLDN", 42, 146, 32, 32, &menu_main_cb_voldn, BTN_UP);
	
	idx =  sizeof(BUTTON) * 6;
	initbutton(menu_buttons[idx], (u8*) &btn_volupTiles, "VOLUP", 42, 119, 32, 32, &menu_main_cb_volup, BTN_UP);
	
	idx =  sizeof(BUTTON) * 7;
	initbutton(menu_buttons[idx], (u8*) &btn_repeatTiles, "REPEAT", 32, 85, 32, 32, &menu_main_cb_repeat, BTN_UP);
	
	/*
		Copy the sprite pallete into memory to be used.
		Any of the sprite palletes will do, they're all the same (for now). I should consolidate them, as this is currently a waste of memory.
	*/
	dmaCopy(btn_prevPal, SPRITE_PALETTE, 512); 
}

//Gracefully exit this menu, aka destructor-type dealy
void menu_main_destroy(void)
{
	//First, let's kill all the buttons
	int i;
	for (i = 0; i <  sizeof(BUTTON) * NUM_BUTTONS; i += sizeof(BUTTON))
	{
		destroybutton(menu_buttons[i]);
	}
	free(menu_buttons);
	menu_buttons = NULL;
	
	//TODO: maybe other things?
}


//Update all of the objects to be displayed before next redraw
void menu_main_update(void)
{
	//we should probably make sure that we draw everything next refresh
	
	int i;
	for (i = 0; i <  sizeof(BUTTON) * NUM_BUTTONS; i += sizeof(BUTTON))
	{
		
		if (menu_buttons[i]->state == BTN_UP)
		{
			//iprintf("%i\n", i);
			u8 *map_offset = menu_buttons[i]->frame_gfx;
			dmaCopy(map_offset, menu_buttons[i]->sprite_gfx_mem, menu_buttons[i]->w * menu_buttons[i]->h);
			
		}
		else
		{
			//iprintf("%i\n", i);
			u8 *map_offset = menu_buttons[i]->frame_gfx +  menu_buttons[i]->w * menu_buttons[i]->h;
			dmaCopy(map_offset, menu_buttons[i]->sprite_gfx_mem, menu_buttons[i]->w * menu_buttons[i]->h);
		}
	}
	
	//TODO: maybe other things?
}

//Set all of the objects to be displayed during the next redraw
void menu_main_redraw(void)
{
	//Let's redraw things!
	
	//all of the buttons
	int i, j;
	for (i = j = 0; i <  sizeof(BUTTON) * NUM_BUTTONS; i += sizeof(BUTTON), j++)
	{
		//iprintf("%i\n", j);
		oamSet(&oamMain, j, menu_buttons[i]->x, menu_buttons[i]->y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
		menu_buttons[i]->sprite_gfx_mem, -1, false, false, false, false, false);
	}
	
	//TODO: maybe other things?
}

//User has touched something
void menu_main_touchdown(touchPosition *touch)
{
	//Handle touchscreen presses for this menu
	
	int i;
	for (i = 0; i <  sizeof(BUTTON) * NUM_BUTTONS; i += sizeof(BUTTON))
	{
		
		//	If the user has pushed a button, we should handle that. Find out if they've pushed a button, and handle it if they have.
		if ((touch->px > menu_buttons[i]->x) && (touch->py > menu_buttons[i]->y) &&
			(touch->px <= (menu_buttons[i]->x + menu_buttons[i]->w)) && (touch->py <= (menu_buttons[i]->y + menu_buttons[i]->h)))
		{
			menu_buttons[i]->state = BTN_DOWN;
			menu_buttons[i]->callback();
		}
	}
	
	//TODO: maybe other things?
}

//User has stopped touching something
void menu_main_touchup(touchPosition *touch)
{
	//Handle touchscreen releases for this menu
	
	int i;
	for (i = 0; i <  sizeof(BUTTON) * NUM_BUTTONS; i += sizeof(BUTTON))
	{
		
		//	If the user has released a button, we should handle that. Find out if they've released a button, and handle it if they have.
		if ((touch->px > menu_buttons[i]->x) && (touch->py > menu_buttons[i]->y) &&
			(touch->px <= (menu_buttons[i]->x + menu_buttons[i]->w)) && (touch->py <= (menu_buttons[i]->y + menu_buttons[i]->h)))
		{
			menu_buttons[i]->state = BTN_UP;
		}
	}
	
	//TODO: maybe other things?
}
