#include "UDP_Server.h"
#include <iostream>
#include <WS2TCPip.h>
#include "Client.h"

UDP_Server::UDP_Server()
{
	if (InitServer())
	{
		RunServer();
		CloseServer();
	}

	std::cout << "Server Closed...";
	std::cin;
}

bool UDP_Server::InitServer()
{
	//Winsock
	wsVersion = MAKEWORD(2, 2);

	ws0k = WSAStartup(wsVersion, &wsData);
	if (ws0k != 0)
	{
		std::cout << "Could not start WinSock : " << std::endl;
		return false;
	}
	std::cout << "Winsock Started" << std::endl;

	//Bind Socket
	sServer = socket(AF_INET, SOCK_DGRAM, 0);
	serverData.sin_addr.S_un.S_addr = ADDR_ANY;
	serverData.sin_family = AF_INET;
	serverData.sin_port = htons(21000);
	serverLength = sizeof(serverData);
	serverIPLength = sizeof(serverIP);
	if (bind(sServer, (sockaddr*)&serverData, serverLength) == SOCKET_ERROR)
	{
		std::cout << "Could not bind : " << WSAGetLastError() << std::endl;
		return false;
	}
	std::cout << "Server Bound" << std::endl;

	u_long nonblocking = 1;
	int iResult = ioctlsocket(sServer, FIONBIO, &nonblocking);
	if (iResult != NO_ERROR)
	{
		printf("ioctlsocket failed with error: %ld\n", iResult);
		running = false;
	}

	clientLength = sizeof(clientData);
	ZeroMemory(&clientData, clientLength);

	bufferLength = sizeof(buffer);
	clientIPLength = sizeof(clientIP);

	Clients = new Client[MaxPlayers];

	running = true;
	return true;
}

void UDP_Server::RunServer()
{
	std::cout << "Starting Main Server Loop" << std::endl;
	//Looooop
	while (running)
	{
		ZeroMemory(buffer, bufferLength);
		//get messages
		std::cout << "Listening For Client" << std::endl;
		bytesIn = recvfrom(sServer, buffer, bufferLength, 0, (sockaddr*)&clientData, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			std::cout << "Could not get client data : " << WSAGetLastError() << std::endl;
			running = false;
		}

		//Show client info
		ZeroMemory(clientIP, clientIPLength);
		inet_ntop(AF_INET, &clientData.sin_addr, clientIP, clientIPLength);

		std::cout << "Data Received From : " << clientIP << " : " << buffer << std::endl;
		SendData(buffer);
	}
	std::cout << "Leaving Main Server Loop" << std::endl;
}

void UDP_Server::SendData(std::string strData)
{
	if (sendto(sServer, strData.c_str(), sizeof(strData), 0, (sockaddr*)&clientData, clientLength) == SOCKET_ERROR)
	{
		std::cout << "Send Failed With : " << WSAGetLastError() << std::endl;
	}
	std::cout << "Data Sent" << std::endl;
}

void UDP_Server::CloseServer()
{
	//Close socket
	closesocket(sServer);
	//Shutdown winSock
	WSACleanup();
}

UDP_Server::~UDP_Server()
{
}
