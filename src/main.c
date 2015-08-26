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
#include "global.h"
#include "main.h"
#include "menu_main.h"

struct sockaddr_in sain;
struct hostent * host;
struct in_addr ip, gateway, mask, dns1, dns2;

MPD_CONNINFO mpd_conn;
MPD_STATUS mpd_state;
MPD_SONG_INFO current_song;

bool connectAP(void)
{
	iprintf("Connecting via WFC data ...\n");
	if(!Wifi_InitDefault(WFC_CONNECT)) {
		iprintf("Failed to connect to AP!");
		return false;
	} else {

		iprintf("Connected to AP.\n");

		ip = Wifi_GetIPInfo(&gateway, &mask, &dns1, &dns2);
	}
	return true;
}

bool connectMPD(void)
{	
	//connect and stuff
	host = gethostbyname(mpd_conn.host);
	
	sockd = socket(AF_INET, SOCK_STREAM, 0);
	if (!sockd)
	{
		
		iprintf("Failed to get socket.\n");
	//	for (;;) { int i = 0; };
		return false;
	}
	
	sain.sin_family = AF_INET;
	sain.sin_port = htons(mpd_conn.port);
	sain.sin_addr.s_addr= *((unsigned long *)(host->h_addr_list[0]));
	if (connect(sockd,(struct sockaddr *)&sain, sizeof(sain)) != 0)
		return false;
		
	memset(recvbuf, 0x0, sizeof(recvbuf));

	return true;
}

bool mpd_checkerror(void)
{
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
	if (strstr(recvbuf, "OK") == NULL)
	{	
		iprintf("Did not get OK from mpd\n");
		return false;
	}
	
	return true;
}

void get_status(void)
{
	char *temp;
	memset(recvbuf, 0x0, sizeof(recvbuf));
	
	send(sockd, "status\n", strlen("status\n"), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
	
	recvbuf[sizeof(recvbuf) - 1] = '\0';
	
	temp = strtok(recvbuf, "\n");
	
	while (temp != NULL)
	{
		//iprintf("%s\n", temp);
		if (strncmp(temp, "volume: ", 8) == 0)
			mpd_state.volume = atoi(temp + 8);
		else if (strncmp(temp, "repeat: ", 8) == 0)
			mpd_state.repeat = atoi(temp + 8);
		else if (strncmp(temp, "random: ", 8) == 0)
			mpd_state.random = atoi(temp + 8);
		else if (strncmp(temp, "state: ", 7) == 0)
		{
			strncpy(mpd_state.host_state, temp + 7, sizeof(mpd_state.host_state) - 1);
			mpd_state.host_state[sizeof(mpd_state.host_state) - 1] = '\0';
		}
		else if (strncmp(temp, "songid: ", 8) == 0)
			mpd_state.songid = atoi(temp + 8);
		else if (strncmp(temp,"time: ",6) == 0)
		{
			strncpy(mpd_state.time, temp + 6, sizeof(mpd_state.time) - 1);
			mpd_state.time[sizeof(mpd_state.time) - 1] = '\0';
		}
		else if (strncmp(temp, "bitrate: ", 9) == 0)
			mpd_state.bitrate = atoi(temp + 9);
		
		temp = strtok(NULL, "\n");
	}
}

void get_currentsong(void)
{
	char *temp;
	memset(recvbuf, 0x0, sizeof(recvbuf));
	
	send(sockd, "currentsong\n", strlen("currentsong\n"), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
	
	recvbuf[sizeof(recvbuf) - 1] = '\0';
	
	temp = strtok(recvbuf, "\n");
	
	while (temp != NULL)
	{
		iprintf("%s\n", temp);
		if (strncmp(temp,"Title: ", 7) == 0)
		{
			strncpy(current_song.title,temp + 7,sizeof(current_song.title) - 1);
			current_song.title[sizeof(current_song.title) - 1] = '\0';
		}
		else if (strncmp(temp,"Artist: ", 8) == 0)
		{
			strncpy(current_song.artist,temp + 8,sizeof(current_song.artist) - 1);
			current_song.artist[sizeof(current_song.artist) - 1] = '\0';
		}
		else if (strncmp(temp,"Album: ", 7) == 0)
		{
			strncpy(current_song.album,temp + 7,sizeof(current_song.album) - 1);
			current_song.album[sizeof(current_song.album) - 1] = '\0';
		}
		temp = strtok(NULL, "\n");
	}
}

/*************************************************
*************************************************/
void print_status(bool clear_con)
{
	if (clear_con)
		consoleClear();
	
	//Network info
	iprintf("host	: %s\nport:	%i\nip	  : %s\nmask	: %s\ndns1	: %s\ndns2	: %s\n", 
	mpd_conn.host,
	mpd_conn.port,
	inet_ntoa(ip),
	inet_ntoa(mask),
	inet_ntoa(dns1),
	inet_ntoa(dns2)
	);
	iprintf("--------------------------------\n");
	//mpd info

	if (strncmp(mpd_state.host_state, "play", 4) == 0)
		iprintf("playing\n");
	else if (strncmp(mpd_state.host_state, "pause", 5) == 0)
		iprintf("paused\n");
	else
		iprintf("stopped\n");
	
	iprintf("Title: %s\nArtist: %s\nAlbum: %s\n", 
		current_song.title, 
		current_song.artist, 
		current_song.album
	);
}

//FIXME: Make this function more flexible to video modes and LCD swapping
//I don't currently fully understand the relationship with VRAM banks, so it's kinda dirty.
void getKeyboardInput(char *buf, unsigned int buflen)
{
	lcdSwap();
	 // set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_0_2D);
	
	consoleInit(NULL, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	//consoleSelect(NULL);
	
	//show keyboard
	keyboardInit(NULL, 3, BgType_Text4bpp, BgSize_T_256x512, 20, 0, false, true);
	keyboardShow();
	
	int key = 0;
	char line[256];
	memset(line, 0x0, sizeof(line));
	
	//loop until enter is pressed
	while (key != 10)
	{
		
		key = keyboardUpdate();
		if(key > 0 && key != 10) {
			//iprintf("%d ", key);
			iprintf("%c", key);
			//If the user has backspaced, handle it
			if (key == 8)
				line[strlen(line) - 1] = '\0';
			else
				sprintf(line, "%s%c", line, key);
		}
	}
	
	keyboardHide();
	lcdSwap();
	videoSetMode(MODE_5_2D);
	//sprintf(buf, line);
	snprintf(buf, buflen, line);
}

int main(void)
{
	touchPosition tpos;
	memset((void *)&mpd_state, 0x0, sizeof(mpd_state));
	memset((void *)&mpd_conn, 0x0, sizeof(mpd_conn));
	memset((void *)&current_song, 0x0, sizeof(MPD_SONG_INFO));
	memset(recvbuf, 0x0, sizeof(recvbuf));
	memset(sendbuf, 0x0, sizeof(sendbuf));
	
	 // set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);
	//set video ram banks for background
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);
	//this is for buttons
	vramSetBankE(VRAM_E_MAIN_SPRITE);
	
	lcdMainOnBottom();
	
	consoleInit(NULL, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	//consoleSelect(NULL);
	
	getKeyboardInput(mpd_conn.host, sizeof(mpd_conn.host));
	
	//re-init console after keyboard input fucked it all up
	consoleInit(NULL, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	//consoleSelect(NULL);
	
	//Did the user specify a port to connect to?
	if (strchr(mpd_conn.host, ':') != NULL)
	{
		char *port;
		strncpy(port, mpd_conn.host, sizeof(mpd_conn.host));
		strncpy(mpd_conn.host, strtok_r(port, ":", &port), sizeof(mpd_conn.host));
		mpd_conn.port = atoi(port);
	}
	else //Nope.
		mpd_conn.port = 6600;
	
	//background
	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	
	dmaCopy(backgroundBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(backgroundPal, BG_PALETTE, 256*2);
	
	#ifndef EMU_IDEAS
	if (!connectAP())
	{
		print_status(false);
		return 0;
	}
	if (!connectMPD())
	{
		iprintf("Could not connect to mpd.\n");
		print_status(false);
		//for (;;) { int i = 0; }; //For debug
		return 0;
	}
	#endif
	
	//sprites
	oamInit(&oamMain, SpriteMapping_1D_128, false);
	menu_main_init();
	//update sprites for the first time so they display correctly on launch
	menu_main_update();
	menu_main_redraw();
	
	get_status();
	
	//display initial status
	get_currentsong();
	print_status(false);
	
	int vblcounter = 0;
	while (1)
	{
		//FIXME: SOMETHING IS CAUSING PROBLEMS WITH GETTING SONG INFO WHILE ALSO GETTING STATUS. I probably derped a pointer or two somewhere.
		//FIXME: This is kind of a dirty way to execute this every second or so.
		if (vblcounter >= 60 && vblcounter < 120) 
		{
		//	get_status();
		//	if (strncmp(mpd_state.host_state, "play", 4) == 0) //If playing
		//		get_currentsong();
		//	print_status(true);
			//vblcounter = 0;
		}
		else if (vblcounter >= 120) //same except a couple seconds
		{
		//	if (mpd_state.playing)
 	                       get_currentsong();
			print_status(true);
			vblcounter = 0;
		}
		
		scanKeys();
		
		if (keysDown() & KEY_TOUCH) 
		{
			touchRead(&tpos);
			menu_main_touchdown(&tpos);
			menu_main_update();
		}
		if (keysUp() & KEY_TOUCH)
		{
			menu_main_touchup(&tpos);
			menu_main_update();
		}
		
		menu_main_redraw();
		
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		vblcounter++;
	}
	
	return 0;
}
