#pragma once
#include <iostream>
#include <WS2TCPip.h>

class UDP_Server
{
public:
	UDP_Server();
	~UDP_Server();

	bool InitServer();
	void RunServer();
	void CloseServer();

	//WinSOCK
	WSADATA wsData;
	WORD wsVersion;
	int ws0k;
	SOCKET sServer;
	sockaddr_in serverData;
	int serverLength;

	sockaddr_in clientData;
	int clientLength;
	int bufferLength;
	char buffer[1024];
	int bytesIn;

	int clientIPLength;
	char clientIP[256];

	bool running;
};

