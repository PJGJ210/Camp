#include "TCP_Client.h"
#include <string>

TCP_Client::TCP_Client()
{
	if (InitClient())
	{
		RunClient();
		CloseClient();
	}

	std::cout << "Server Closed...";
	std::cin;
}

bool TCP_Client::InitClient()
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

	//serverInfo
	serverData.sin_family = AF_INET;
	serverData.sin_port = htons(21000);
	serverLength = sizeof(serverData);
	inet_pton(AF_INET, "127.0.0.1", &serverData.sin_addr);

	//Create socket
	sClient = socket(AF_INET, SOCK_DGRAM, 0);

	running = true;
	return true;
}

void TCP_Client::RunClient()
{
	std::cout << "Running Client" << std::endl;
	while (running)
	{
		std::cout << "Enter data to send to server" << std::endl;
		std::cin >> outdata;
		if (sendto(sClient, outdata.c_str(), sizeof(outdata), 0, (sockaddr*)&serverData, serverLength) == SOCKET_ERROR)
		{
			std::cout << "Send Failed With : " << WSAGetLastError() << std::endl;
		}
		std::cout << "Data Sent" << std::endl;
	}
}

void TCP_Client::CloseClient()
{
	std::cout << "Killing Client" << std::endl;
	//Close socket
	closesocket(sClient);
	//Shutdown winSock
	WSACleanup();
}

TCP_Client::~TCP_Client()
{
}
