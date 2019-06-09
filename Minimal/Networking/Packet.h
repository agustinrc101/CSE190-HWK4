#ifndef H_PACKET
#define H_PACKET
#pragma once

#include <glm/glm.hpp>


#define LOCAL_HOST "127.0.0.1"
#define DEFAULT_IP "128.54.70.73"	//TODO Change with computer changes 
#define DEFAULT_PORT 54000
#define MAX_PACKET_SIZE 72	//Change to actual packet size (maybe)

enum PacketType {
	TEST = -1,		
	INIT = 0,			//Player joining
	EXIT = 1,			//Player leaving
	INIT_REPLY = 2,	
	PLAYER_DATA = 3,	//Sending player transform data

};

enum PacketDataType {
	HEAD = 0,
	HAND_LEFT = 1,
	HAND_RIGHT = 2,
	BALL = 3,
};

struct Packet {
	PacketType type;			//4 bytes
	PacketDataType dataType;	//4 bytes

	glm::mat4 toWorld;			//64 bytes

	void serialize(char * data) {
		memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Packet));
	}
};



#endif