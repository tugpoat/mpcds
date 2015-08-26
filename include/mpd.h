#ifndef _MPD_H
#define _MPD_H

typedef struct
{
	char host[64]; //Long enough, let's be lazy and not mess with the heap.
	unsigned int port;
	char password[64]; //Long enough, let's be lazy and not mess with the heap.
} MPD_CONNINFO;

typedef struct 
{
	unsigned int repeat:1;
	unsigned int random:1;
	int volume;
	int bitrate;
	int songid;
	char time[16];
	char host_state[32];
	char song[256];
} MPD_STATUS;

typedef struct
{
	char title[256];
	char artist[256];
	char album[256];
} MPD_SONG_INFO;

#endif