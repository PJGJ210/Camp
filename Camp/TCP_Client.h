#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>

class TCP_Client
{
public:
	TCP_Client();
	~TCP_Client();

	bool InitClient();
	void RunClient();
	void CloseClient();

	//WinSOCK
	WSADATA wsData;
	WORD wsVersion;
	int ws0k;
	SOCKET sClient;
	sockaddr_in serverData;
	int serverLength;

	sockaddr_in clientData;
	int clientLength;
	int bufferLength;
	char buffer[1024];
	int bytesIn;

	int clientIPLength;
	char clientIP[256];

	std::string outdata;
	bool running;
};

