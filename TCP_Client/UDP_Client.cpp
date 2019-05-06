#include "UDP_Client.h"
#include <string>



UDP_Client::UDP_Client()
{
	IPAddr = "127.0.0.1";
	if (InitClient())
	{
		RunClient();
		CloseClient();
	}

	std::cout << "Server Closed...";
	std::cin;
}

UDP_Client::UDP_Client(std::string IPAddr)
{
	this->IPAddr = IPAddr;
	if (InitClient())
	{
		RunClient();
		CloseClient();
	}

	std::cout << "Server Closed...";
	std::cin;
}

bool UDP_Client::InitClient()
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

void UDP_Client::RunClient()
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

void UDP_Client::CloseClient()
{
	std::cout << "Killing Client" << std::endl;
	//Close socket
	closesocket(sClient);
	//Shutdown winSock
	WSACleanup();
}

UDP_Client::~UDP_Client()
{
}
