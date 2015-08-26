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
#include "menu_main_callbacks.h"

void menu_main_cb_play(void)
{
	//iprintf("cb_play called\n");
	memset(recvbuf, 0x0, sizeof(recvbuf));
	send(sockd, "play\n", strlen("play\n"), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
//	mpd_checkerror();
}

void menu_main_cb_pause(void)
{
	//iprintf("cb_pause called\n");
	memset(recvbuf, 0x0, sizeof(recvbuf));
	send(sockd, "pause\n", strlen("pause\n"), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
	//mpd_checkerror();
}

void menu_main_cb_stop(void)
{
	//iprintf("cb_stop called\n");
	memset(recvbuf, 0x0, sizeof(recvbuf));
	send(sockd, "stop\n", strlen("stop\n"), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
//	mpd_checkerror();
}

void menu_main_cb_prev(void)
{
	//iprintf("cb_prev called\n");
	memset(recvbuf, 0x0, sizeof(recvbuf));
	send(sockd, "previous\n", strlen("previous\n"), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
//	mpd_checkerror();
}

void menu_main_cb_next(void)
{
	//iprintf("cb_next called\n");
	memset(recvbuf, 0x0, sizeof(recvbuf));
	send(sockd, "next\n", strlen("next\n"), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
//	mpd_checkerror();
}

void menu_main_cb_voldn(void)
{
	//iprintf("cb_voldn called\n");
	memset(recvbuf, 0x0, sizeof(recvbuf));
	send(sockd, "volume -4\n", strlen("volume -4\n"), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
//	mpd_checkerror();
}

void menu_main_cb_volup(void)
{
	//iprintf("cb_volup called\n");
	memset(recvbuf, 0x0, sizeof(recvbuf));
	send(sockd, "volume +4\n", strlen("volume +4\n"), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
//	mpd_checkerror();
}


void menu_main_cb_repeat(void)
{
	if (mpd_state.repeat)
	{
		strcpy(sendbuf, "repeat 0\n");
	}
	else
	{
		strcpy(sendbuf, "repeat 1\n");
	}
	memset(recvbuf, 0x0, sizeof(recvbuf));
	send(sockd, sendbuf, strlen(sendbuf), 0);
	recv(sockd, recvbuf, sizeof(recvbuf), 0);
//	mpd_checkerror();
}
