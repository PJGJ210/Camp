#pragma once
#include <iostream>
#include <WS2TCPip.h>
#include "Client.h"

class UDP_Server
{
public:
	UDP_Server();
	~UDP_Server();

	enum class Client_Message
	{
		Join,      // tell server we're new here
		Leave,      // tell server we're leaving
		Input       // tell server our user input
	};

	enum class Server_Message
	{
		Join_Result,// tell client they're accepted/rejected
		State       // tell client game state
	};

	bool InitServer();
	void RunServer();
	void SendData(std::string strData);
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

	int serverIPLength;
	char serverIP[256];

	Client* Clients;
	int MaxPlayers = 10;

	bool running;
};

