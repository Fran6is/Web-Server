#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <string>

#pragma comment (lib, "ws2_32.lib")

class WebServer {
private:
	const char* ipAddress;
	const int PORT;
	int listening;
	const char* html;

	//fd_set master;

	//When a client connects
	void onClientConnect(const int& socket);

	//When a client disconnects
	void onClientDisconnect(const int& socket);

	//When a message is received
	void onMessageReceived(const int& clientSocket, const char* msg, int msgLen);

	//send message to client
	void sendToClient(const int& clientSocket, const char* msg, int length);

	//show info
	void showInfo(struct sockaddr_in& client);


public:
	~WebServer() { std::cout << "Server destructor called! \n"; }
	WebServer(const char* IP, const int& port, const char* reply);
	int init();
	int run();
};

int LoadResponse(std::string& result, const char* dirName);
#endif //TcpServer.h