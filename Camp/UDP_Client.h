#pragma once
#include <iostream>
#include <WS2TCPip.h>
#include <string>

class UDP_Client
{
public:
	UDP_Client();
	UDP_Client(std::string serverIP);
	~UDP_Client();

	enum class Client_Message: int8_t
	{
		Join,      // tell server we're new here
		Leave,      // tell server we're leaving
		Input       // tell server our user input
	};

	enum class Server_Message : int8_t
	{
		Join_Result,// tell client they're accepted/rejected
		State       // tell client game state
	};

	bool InitClient();
	void RunClient();
	void SendData(std::string strData);
	void ReceiveData();
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

	int serverIPLength;
	char serverIP[256];

	std::string outdata;
	bool running;
};