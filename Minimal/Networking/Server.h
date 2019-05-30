#ifndef H_SERVER
#define H_SERVER
#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <process.h>
#pragma comment (lib, "ws2_32.lib")

class Server{
public:
	static void startServer();
	static void stopServer();

private:
	static void serverLoop(void *);

protected:
	static bool serverOn;
};

#endif