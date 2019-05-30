#include "Server.h"
#include "Packet.h"

void Server::startServer() {
	//Multithread
	_beginthread(serverLoop, 0, (void*)12);
}

void Server::stopServer() {
	serverOn = false;
}

void Server::serverLoop(void *) {
	//Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int ws0k = WSAStartup(ver, &wsData);
	if (ws0k != 0) {
		std::cerr << "<<SERVER>> Can't initialize winsock! Quitting..." << std::endl;
		serverOn = false;
		_endthread();
	}

	//Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "<<SERVER>> Can't create a socket... Quitting..." << std::endl;
		serverOn = false;
		std::terminate();
		_endthread();
	}

	//Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(DEFAULT_PORT);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //Could also use inet_pton ....

	bind(listening, (sockaddr *)&hint, sizeof(hint));

	//Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);

	FD_SET(listening, &master);

	std::cout << "<<SERVER>> Server open" << std::endl;
	serverOn = true;

	while (serverOn) {
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == listening) {
				//Accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);
				//Add the new connection to the list of connected clients
				FD_SET(client, &master);

				//Send a welcome message to the connected clients
				std::string welcomeMsg = "Connected Successfully";
				send(client, welcomeMsg.c_str(), (int)(welcomeMsg.size()) + 1, 0);

				std::cout << "<<SERVER>> Client connected" << std::endl;
			}
			else {
				//Accept a new message
				char buf[MAX_PACKET_SIZE];
				ZeroMemory(buf, MAX_PACKET_SIZE);

				int bytesIn = recv(sock, buf, MAX_PACKET_SIZE, 0);	//MESSAGE RECIEVED
				if (bytesIn <= 0) {
					//Drop the client
					std::cout << "<<SERVER>> Client disconnected" << std::endl;
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else {
					//Send message to other clients, NOT the listening socket
					for (u_int j = 0; j < master.fd_count; j++) {
						SOCKET outSock = master.fd_array[j];
						if (outSock != listening && outSock != sock) {
							send(outSock, buf, MAX_PACKET_SIZE, 0);
						}
					}
				}
			}
		}
	}

	//Clean up winsock
	WSACleanup();
	std::cout << "<<SERVER>> Server closed" << std::endl;
	
	//Kills this thread
	_endthread();
}
