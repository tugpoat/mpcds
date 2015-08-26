#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <nds.h>
#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

//debug-related defines
#ifdef _DEBUG
	#define _DEBUG_GFX
	#define _DEBUG_NET
#endif

//things everything needs (tcp socket and send/recv buffers)
//I know this is bad practice, but this program will never be large enough that it matters (because I will always be in control of its development, likely).
int sockd;
char sendbuf[256];
char recvbuf[4096];

#endif

