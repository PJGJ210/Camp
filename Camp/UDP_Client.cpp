#include "UDP_Client.h"
#include <string>

UDP_Client::UDP_Client()
{
	strcpy_s(serverIP, "127.0.0.1");
	if (InitClient())
	{
		RunClient();
		CloseClient();
	}

	std::cout << "Server Closed...";
	std::cin;
}

UDP_Client::UDP_Client(std::string serverIP)
{
	strcpy_s(this->serverIP, serverIP.c_str());
	if (InitClient())
	{
		//RunClient();
		//CloseClient();
	}

	std::cout << "Server Closed...";
	//std::cin;
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
	serverIPLength = sizeof(serverIP);
	bufferLength = sizeof(buffer);
	inet_pton(AF_INET, serverIP, &serverData.sin_addr);
	//inet_pton(AF_INET, serverIP.c_str(), &serverData.sin_addr);

	//Create socket
	sClient = socket(AF_INET, SOCK_DGRAM, 0);
	u_long nonblocking = 1;
	int iResult = ioctlsocket(sClient, FIONBIO, &nonblocking);
	if (iResult != NO_ERROR)
	{
		printf("ioctlsocket failed with error: %ld\n", iResult);
		running = false;
	}
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

void UDP_Client::SendData(std::string strData)
{
	if (sendto(sClient, strData.c_str(), sizeof(strData), 0, (sockaddr*)&serverData, serverLength) == SOCKET_ERROR)
	{
		std::cout << "Send Failed With : " << WSAGetLastError() << std::endl;
	}
	//std::cout << "Data Sent" << std::endl;
}

void UDP_Client::ReceiveData()
{
	ZeroMemory(buffer, bufferLength);
	//get messages
	if (bytesIn = recvfrom(sClient, buffer, bufferLength, 0, (sockaddr*)&serverData, &serverLength) >= 0)
	{
		if (bytesIn == SOCKET_ERROR)
		{
			std::cout << "Could not get server data : " << WSAGetLastError() << std::endl;
			//running = false;
		}

		//Show client info
		ZeroMemory(serverIP, serverIPLength);
		inet_ntop(AF_INET, &serverData.sin_addr, serverIP, serverIPLength);
		if (buffer > 0)
		{
			std::cout << "Data Received From : " << serverIP << " : " << buffer << std::endl;
		}
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
