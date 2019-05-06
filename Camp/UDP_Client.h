#pragma once
#include <iostream>
#include <WS2TCPip.h>
#include <string>

class UDP_Client
{
public:
	UDP_Client();
	UDP_Client(std::string IPAddr);
	~UDP_Client();

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

	std::string IPAddr;
	std::string outdata;
	bool running;
};