#include "WebServer.h"

WebServer::WebServer(const char* IP, const int& port, const char* reply)
	:ipAddress{ IP }, PORT(port), listening{}, html{ reply } {}

int WebServer::init()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return -1;
	}

	// Create a socket
	listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return -1;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	inet_pton(AF_INET, ipAddress, &hint.sin_addr); // Could also use INADDR_ANY

	if (bind(listening, (sockaddr*)&hint, sizeof(hint)) != 0)
	{
		int err = WSAGetLastError(); WSACleanup(); closesocket(listening);
		return err;
	}

	// Tell Winsock the socket is for listening 
	if (listen(listening, SOMAXCONN) != 0)
	{
		int err = WSAGetLastError(); WSACleanup(); closesocket(listening);
		return err;
	}

	return 0;

}

int WebServer::run()
{
	bool running = true;
	while (running)
	{
		struct sockaddr_in clientInfo {};
		int len = sizeof clientInfo;

		int client = accept(listening, (sockaddr*)&clientInfo, &len);
		if (client == INVALID_SOCKET) continue;

		//showInfo(clientInfo);

		onClientConnect(client);
	}

	closesocket(listening);

	// Cleanup winsock
	WSACleanup();

	return 0;
}


void WebServer::onClientConnect(const int& clientSocket)
{
	
	char buf[4096];
	ZeroMemory(buf, 4096);

	// Receive message
	int bytesIn = recv(clientSocket, buf, 4096, 0);
	if (bytesIn <= 0)
	{
		//(2)when a client disconnects
		onClientDisconnect(clientSocket);
	}
	else
	{
		//(3) what to do when we actually receive a message
		onMessageReceived(clientSocket, buf, bytesIn);
		closesocket(clientSocket);
		
	}

}

void WebServer::onMessageReceived(const int& clientSocket, const char* msg, int msgLen)
{
	int codes[]{ 200, 400, 404 };

	std::ostringstream oss;
	oss << "HTTP/1.1 " << codes[0] << " OK\r\n\r\n";
	oss << html;
	
	std::string response{ oss.str() };
	sendToClient(clientSocket, response.c_str(), response.size() + 1);

}


void WebServer::onClientDisconnect(const int& socket)
{
	closesocket(socket);
	std::cout << "#" << socket << " left! \n";
}

void WebServer::sendToClient(const int& clientSocket, const char* msg, int length)
{
	send(clientSocket, msg, length, 0);
}

void WebServer::showInfo(struct sockaddr_in& client)
{
	//Option 1: getnameinfo()

	//Option 2: inet_ntop(), ntohs()
	char buf[NI_MAXHOST]{};
	inet_ntop(AF_INET, &client.sin_addr.S_un.S_addr, buf, NI_MAXHOST);
	std::cout << buf << " connected on port: " << ntohs(client.sin_port) << std::endl;
}