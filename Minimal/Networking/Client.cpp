#include "Client.h"

Client::Client(){
	connected = false;
	client = this;
}

Client::~Client(){
	
	connected = false;
	//Clean up winsock
	closesocket(sock);
	WSACleanup();
}

void Client::joinServer() {
	if (client == 0) return;

	if (connectToServer(DEFAULT_IP)) {
		connected = true;
		_beginthread(clientLoop, 0, (void*)0);
	}
	else if (connectToServer(LOCAL_HOST)) {
		connected = true;
		_beginthread(clientLoop, 0, (void*)0);
	}
}

void Client::exitServer() {
	//TODO - send exit packet
	connected = false;
}

bool Client::connectToServer(std::string ip) {
	std::cout << "<<CLIENT>> Connecting to server with ip " << ip.c_str() << std::endl;

	std::string ipAddress = ip;	//IP address of the server
	int port = DEFAULT_PORT;

	//Initialize Winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);

	if (wsResult != 0) {
		std::cerr << "<<CLIENT>> Can't start winsock, Err #" << wsResult << std::endl;
		return connectionErrorHelper();
	}

	//Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "<<CLIENT>> Can't create socket, Err#" << WSAGetLastError() << std::endl;
		return connectionErrorHelper();
	}

	//Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

	if (connResult == SOCKET_ERROR) {
		std::cerr << "<<CLIENT>> Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return connectionErrorHelper();
	}
	std::cout << "<<CLIENT>> Connection successful!" << std::endl;

	{
		//Get welcome message
		char buffer[MAX_PACKET_SIZE];
		ZeroMemory(buffer, MAX_PACKET_SIZE);
		int bytesReceived = recv(sock, buffer, MAX_PACKET_SIZE, 0);
		if (bytesReceived > 0)
			std::cout << "<<SERVER>> " << std::string(buffer, 0, bytesReceived).c_str() << std::endl;
	}

	return true;
}

bool Client::connectionErrorHelper() {
	std::cout << "Failed to connect to server ip " << DEFAULT_IP << std::endl;
	return false;
}

void Client::clientLoop(void *) {
	//Receiving Packets
	while (client->connected) {
		char data[MAX_PACKET_SIZE];
		ZeroMemory(data, MAX_PACKET_SIZE);
		Packet packet;

		int bytesReceived = recv(client->sock, data, sizeof(Packet), 0);

		int i = 0;
		if (bytesReceived > 0) {
			while (i < bytesReceived) {
				packet.deserialize(&(data[i]));

				switch (packet.type) {
				case TEST:
					i += sizeof(Packet);
					std::cout << "Received TEST packet" << std::endl;
					break;
				case INIT:			//Handle init
					client->initReceived = true;
					i += sizeof(Packet);
				case EXIT:			//Handle exit
					client->initReceived = false;
					i += sizeof(Packet);
					client->packets.push_back(packet);
					break;
				case PLAYER_DATA:	//Handle player data
					i += sizeof(Packet);
					client->playerDataPackets.push_back(packet);
					break;
				default:			//Ignore unkown packets
					i++;
					std::cout << "<<SERVER>> Received package of unknown type" << std::endl;
					return;
				}
			}
		}
	}

	std::cout << "<<CLIENT>> Left the server" << std::endl;

	//Kills this thread
	_endthread();
}

std::vector<Packet> Client::getPlayerPackets() {
	std::vector<Packet> p = playerDataPackets;

	playerDataPackets.clear();

	return p;
}

//Packet sending
void Client::sendPacket(Packet packet) {
	if (client == 0) { std::cout << "<<CLIENT>> Cannot send packet" << std::endl; return; }

	char buf[sizeof(packet)];
	packet.serialize(buf);

	int sendResult = send(client->sock, buf, sizeof(packet), 0);
	if (sendResult == SOCKET_ERROR)
		std::cerr << "Error sending packet, Err#" << WSAGetLastError() << std::endl;
}

void Client::sendPacket() {
	char buf[sizeof(Packet)];

	Packet packet;
	packet.type = TEST;
	packet.serialize(buf);

	int sendResult = send(sock, buf, sizeof(Packet), 0);
	if (sendResult == SOCKET_ERROR)
		std::cerr << "Error sending packet, Err#" << WSAGetLastError() << std::endl;
}

void Client::sendPacket(PacketType type) {
	char buf[sizeof(Packet)];

	Packet packet;
	packet.type = type;
	packet.serialize(buf);

	int sendResult = send(sock, buf, sizeof(Packet), 0);
	if (sendResult == SOCKET_ERROR)
		std::cerr << "Error sending packet, Err#" << WSAGetLastError() << std::endl;
}

void Client::sendPlayerDataPacket(glm::mat4 t, PacketDataType type) {
	char buf[sizeof(Packet)];

	Packet packet;
	packet.type = PLAYER_DATA;
	packet.dataType = type;
	packet.toWorld = t;
	packet.serialize(buf);

	int sendResult = send(sock, buf, sizeof(Packet), 0);
	if (sendResult == SOCKET_ERROR)
		std::cerr << "Error sending packet, Err#" << WSAGetLastError() << std::endl;
}
