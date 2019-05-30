#ifndef H_CLIENT
#define H_CLIENT
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <WS2tcpip.h>
#include <process.h>
#pragma comment(lib, "WS2_32.lib")

#include "Packet.h"

class Client{
public:
	Client();
	~Client();

	void joinServer();
	void exitServer();

	static void sendPacket(Packet packet);
	void sendPacket();

private:
	SOCKET sock;
	bool connected;

	bool connectToServer(std::string ip);
	bool connectionErrorHelper();
	static void clientLoop(void *);

protected:
	static Client * client;
};

#endif