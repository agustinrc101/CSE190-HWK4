#ifndef H_PACKET
#define H_PACKET
#pragma once

#define LOCAL_HOST "127.0.0.1"
#define DEFAULT_IP "127.0.0.1"	//TODO 
#define DEFAULT_PORT 54000
#define MAX_PACKET_SIZE 256	//Change to actual packet size (maybe)

enum PacketType {
	TEST = -1,		
	INIT = 0,			//Player joining 
	EXIT = 1,			//Player leaving
	PLAYER_DATA = 2,	//Sending player transform data

};

struct Packet {
	PacketType type;

	virtual void serialize(char * data) {
		memcpy(data, this, sizeof(Packet));
	}

	virtual void deserialize(char * data) {
		memcpy(this, data, sizeof(Packet));
	}
};

struct PacketPlayerData : Packet {
	PacketType type;
	
	void serialize(char * data) override {
		memcpy(data, this, sizeof(PacketPlayerData));
	}

	void deserialize(char * data) override {
		memcpy(this, data, sizeof(PacketPlayerData));
	}
	
};



#endif