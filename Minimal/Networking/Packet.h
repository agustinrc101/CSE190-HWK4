#ifndef H_PACKET
#define H_PACKET
#pragma once

#include <glm/glm.hpp>


#define LOCAL_HOST "127.0.0.1"
#define DEFAULT_IP "128.54.70.72"	//TODO Change with computer changes 
#define DEFAULT_PORT 54000
#define MAX_PACKET_SIZE 72	//Change to actual packet size (maybe)

enum PacketType {
	PACKET_TEST = -1,		
	PACKET_INIT = 0,			//Player joining
	PACKET_EXIT = 1,			//Player leaving
	PACKET_INIT_REPLY = 2,
	PACKET_PLAYER_DATA = 3,		//Sending player transform data
	PACKET_AUDIO = 4,
};

enum PacketDataType {
	PDATA_HEAD = 0,
	PDATA_HAND_LEFT = 1,
	PDATA_HAND_RIGHT = 2,
	PDATA_BALL = 3,
	PDATA_CONTROLLER_LEFT = 4,
	PDATA_CONTROLLER_RIGHT = 5,
	PDATA_PLAYER = 6,
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