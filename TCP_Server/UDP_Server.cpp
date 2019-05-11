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

	/*u_long nonblocking = 1;
	int iResult = ioctlsocket(sServer, FIONBIO, &nonblocking);
	if (iResult != NO_ERROR)
	{
		printf("ioctlsocket failed with error: %ld\n", iResult);
		running = false;
	}*/

	clientLength = sizeof(clientData);
	ZeroMemory(&clientData, clientLength);

	bufferLength = sizeof(buffer);
	clientIPLength = sizeof(clientIP);

	Clients = new Client[MaxPlayers];
	for (int i = 0; i < MaxPlayers; i++)
	{
		Clients[i] = Client();
	}
	//ZeroMemory(Clients, sizeof(Clients));

	running = true;
	return true;
}

void UDP_Server::RunServer()
{
	std::cout << "Starting Main Server Loop" << std::endl;
	//Looooop
	while (running)
	{
		if (ReceivePacket())
		{
			HandlePacket();
		}
		//SendData(buffer);
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

void UDP_Server::Broadcast(std::string strData)
{
	for (int i = 0; i < MaxPlayers; i++)
	{
		std::cout << Clients[i].playerID << std::endl;
		if (Clients[i].playerID.compare("00") != 0)
		{
			clientData.sin_addr.S_un.S_addr = Clients[i].Network.address;
			clientData.sin_port = Clients[i].Network.port;
			if (sendto(sServer, strData.c_str(), sizeof(strData), 0, (sockaddr*)&clientData, clientLength) == SOCKET_ERROR)
			{
				std::cout << "Send Failed With : " << WSAGetLastError() << std::endl;
			}
		}
	}
	std::cout << "Data Sent to All" << std::endl;
}

bool UDP_Server::ReceivePacket()
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

	if (bytesIn > 0)
	{
		//Show client info
		ZeroMemory(clientIP, clientIPLength);
		inet_ntop(AF_INET, &clientData.sin_addr, clientIP, clientIPLength);

		std::cout << "Data Received From : " << clientIP << " : " << buffer << std::endl;
		return true;
	}
	return false;
}

void UDP_Server::HandlePacket()
{
	//deal with type of packet
	std::cout << buffer[0] << std::endl;
	switch (buffer[0])
	{
	case '1':
		break;
	case '2':
		break;
	case '3':
		break;
	}
	//deal with player ID if exists 1-2
	std::string PlayerID = CopyBuffer(1, 2);
	int iPlayerID = std::atoi(PlayerID.c_str());
	//deal with action 3
	switch (buffer[3])
	{
	case 'C':
		//if playerID is 0 then make a new player and assign an ID
		std::cout << "Case : C" << std::endl;
		ConnectClient(iPlayerID);
		break;
	case 'X':
		std::cout << "Case : X" << std::endl;
		Broadcast("X");
		break;
	case 'L':
		std::cout << "Case : L" << std::endl;
		SendData("L");
		break;
	case 'U':
		std::cout << "Case : U" << std::endl;
		SendData("U");
		break;
	case 'D':
		std::cout << "Case : D" << std::endl;
		SendData("D");
		break;
	case 'R':
		std::cout << "Case : R" << std::endl;
		SendData("R");
		break;
	}
}

void UDP_Server::ConnectClient(int PlayerID)
{
	for (int i = 0; i < MaxPlayers; i++)
	{
		std::cout << Clients[i].playerID << " : " << Clients[i].playerID.compare("00") << std::endl;
		if (Clients[i].playerID.compare("00") == 0)
		{
			Clients[i] = Client(clientData.sin_addr.S_un.S_addr, clientData.sin_port);
			std::cout << "Assigning" << std::endl;
			char pID[2];
			if (PlayerID)
			{
				Clients[i].playerID = snprintf(pID, 2, "%02d", PlayerID);
				std::cout << Clients[i].playerID << " Assigned!" << std::endl;
				return;
			}
			else
			{
				NextPlayerID++;
				Clients[i].playerID = snprintf(pID, 2, "%02d", NextPlayerID);
				std::cout << Clients[i].playerID << " Assigned!" << std::endl;
				return;
			}
		}
	}
}

std::string UDP_Server::CopyBuffer(int start, int end)
{
	char* CharArray = new char[end - start];
	for (int i = start; i < end; i++)
	{
		CharArray[i - start] = buffer[i];
	}
	return (std::string)CharArray;
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
