#pragma once
#include <iostream>
#include <WS2TCPip.h>
#include "Client.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>


class UDP_Server
{
public:
	UDP_Server();
	~UDP_Server();

	enum class Client_Message : int8_t
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

	bool InitServer();
	void SendState();
	void RunServer();
	void SendData(std::string strData);
	void Broadcast(std::string strData);
	bool ReceivePacket();
	void HandlePacket();
	void ConnectClient(std::string PlayerID);
	std::string CopyBuffer(int start, int end);
	void CloseServer();

	//Doubles
	std::chrono::system_clock::time_point previousTime;
	std::chrono::system_clock::time_point currentTime;
	std::chrono::system_clock::time_point fpsTime;
	std::chrono::system_clock::time_point priorTime;
	std::chrono::duration<double, std::milli> elapsedfpsTime;
	std::chrono::duration<double, std::milli> elapsedTime;
	std::chrono::duration<double, std::milli> sleepTime;
	double updateTime;
	int FPS;
	int CurrFPS;
	const double MStoS = 1000.0;
	const double NStoMS = 1000000.0;

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

