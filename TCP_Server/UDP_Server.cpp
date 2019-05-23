#include "UDP_Server.h"
#include <iostream>
#include <WS2TCPip.h>
#include <string>
#include "Client.h"

UDP_Server::UDP_Server()
{
	if (InitServer())
	{
		RunServer();
		CloseServer();
	}

	//std::cout << "Server Closed...";
	std::cin;
}

bool UDP_Server::InitServer()
{
	FPS = 30;
	updateTime = MStoS / FPS;
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
	for (int i = 0; i < MaxPlayers; i++)
	{
		Clients[i] = Client();
		Clients[i].Changed = false;
	}
	//ZeroMemory(Clients, sizeof(Clients));
	running = true;
	return true;
}

void UDP_Server::SendState()
{
	std::string StateString = "";
	int IDLen = 2;
	int posLen = 10;
	int statLen = 4;
	std::string PlayerX;
	int iPlayerX;
	std::string PlayerY;
	int iPlayerY;
	std::string PlayerHP;
	int iPlayerHP;
	std::string PlayerMP;
	int iPlayerMP;
	int bufferIterator = 0;

	std::stringstream ss;
	for (int i = 0; i < MaxPlayers; i++)
	{
		//check for non 0 player ID
		if (Clients[i].Changed && Clients[i].GetplayerID().compare("00") != 0)
		{
			Clients[i].Changed = false;
			//Add Identifier For State
			StateString += "2";
			//Add Player ID
			ss << std::setw(IDLen) << std::setfill('0') << Clients[i].GetplayerID();
			StateString += ss.str();
			ss.str("");
			ss.clear();
			//Add X Pos posLen
			ss << std::setw(posLen) << std::setfill('0') << Clients[i].player.GetExPos();
			StateString += ss.str();
			ss.str("");
			ss.clear();
			//Add Y Pos posLen
			ss << std::setw(posLen) << std::setfill('0') << Clients[i].player.GetEyPos();
			StateString += ss.str();
			ss.str("");
			ss.clear();
			//Add HP Pos statLen
			ss << std::setw(statLen) << std::setfill('0') << Clients[i].player.GetHP();
			StateString += ss.str();
			ss.str("");
			ss.clear();
			//Add MP Pos statLen
			ss << std::setw(statLen) << std::setfill('0') << Clients[i].player.GetMP();
			StateString += ss.str();
			ss.str("");
			ss.clear();
		}
	}
	Broadcast(StateString);
}

void UDP_Server::RunServer()
{
	std::cout << "Starting Main Server Loop" << std::endl;
	//Looooop
	fpsTime = std::chrono::system_clock::now();
	priorTime = std::chrono::system_clock::now();
	while (running)
	{
		if (ReceivePacket())
		{
			HandlePacket();
		}
		fpsTime = std::chrono::system_clock::now();
		elapsedfpsTime = fpsTime - priorTime;
		//do state x times per second
		if (elapsedfpsTime.count() > updateTime)
		{
			SendState();
			priorTime = fpsTime;
		}
	}
	std::cout << "Leaving Main Server Loop" << std::endl;
}

void UDP_Server::SendData(std::string strData)
{
	if (sendto(sServer, strData.c_str(), strData.size(), 0, (sockaddr*)&clientData, clientLength) == SOCKET_ERROR)
	{
		std::cout << "Send Failed With : " << WSAGetLastError() << std::endl;
	}
	//std::cout << "Data Sent" << std::endl;
}

void UDP_Server::Broadcast(std::string strData)
{
	for (int i = 0; i < MaxPlayers; i++)
	{
		if (Clients[i].playerID.compare("00") != 0)
		{
			//std::cout << Clients[i].playerID.c_str() << std::endl;
			clientData.sin_addr.S_un.S_addr = Clients[i].Network.address;
			clientData.sin_port = Clients[i].Network.port;
			//std::cout << strData.c_str() << std::endl;
			if (sendto(sServer, strData.c_str(), strData.size(), 0, (sockaddr*)&clientData, clientLength) == SOCKET_ERROR)
			{
				std::cout << "Send Failed With : " << WSAGetLastError() << std::endl;
			}
		}
	}
	//std::cout << "Data Sent to All" << std::endl;
}

bool UDP_Server::ReceivePacket()
{
	ZeroMemory(buffer, bufferLength);
	//get messages
	//std::cout << "Listening For Client" << std::endl;
	bytesIn = recvfrom(sServer, buffer, bufferLength, 0, (sockaddr*)&clientData, &clientLength);
	if (bytesIn == SOCKET_ERROR)
	{
		//check for disconnect and renew the slot if disconnected
		if (WSAGetLastError() == 10054)
		{
			int address = clientData.sin_addr.S_un.S_addr;
			short port = clientData.sin_port;
			//check if player is already connected
			for (int i = 0; i < MaxPlayers; i++)
			{
				if (Clients[i].Network.GetAddress() == address && Clients[i].Network.port == port)
				{
					std::cout << "Client disconnected : " << i << std::endl;
					Clients[i] = Client();
					return false;
				}
			}
		}
		//std::cout << "Could not get client data : " << WSAGetLastError() << std::endl;
		return false;
	}

	if (bytesIn > 0)
	{
		//Show client info
		ZeroMemory(clientIP, clientIPLength);
		inet_ntop(AF_INET, &clientData.sin_addr, clientIP, clientIPLength);

		//std::cout << "Data Received From : " << clientIP << " : " << buffer << std::endl;
		return true;
	}
	return false;
}


void UDP_Server::HandlePacket()
{
	std::string PlayerID;
	int iPlayerID;
	//deal with type of packet
	//std::cout << buffer[0] << std::endl;
	switch (buffer[0])
	{
	case '1':
		//deal with player ID if exists 1-2
		PlayerID = CopyBuffer(1, 2);
		iPlayerID = std::atoi(PlayerID.c_str());
		std::cout << PlayerID << ":" << iPlayerID << std::endl;
		//deal with action 3
		switch (buffer[3])
		{
			case 'C':
				//if playerID is 0 then make a new player and assign an ID
				std::cout << "Case : C" << std::endl;
				ConnectClient(PlayerID);
				for (int i = 0; i < MaxPlayers; i++)
				{
					//std::cout << Clients[i].playerID << " : " << Clients[i].playerID.compare("00") << std::endl;
					if (!Clients[i].playerID.compare("00") == 0)
					{
						Clients[i].Changed = true;
					}
				}
				break;
		}
		break;
	case '2':
		//deal with player ID if exists 1-2
		PlayerID = CopyBuffer(1, 2);
		iPlayerID = std::atoi(PlayerID.c_str());
		std::cout << PlayerID << ":" << iPlayerID << std::endl;
		//deal with action 3
		switch (buffer[3])
		{
			case 'X':
				std::cout << "Case : X" << std::endl;
				Broadcast("X : " + PlayerID);
				break;
		}
		break;
	case '3':
		//deal with player ID if exists 1-2
		PlayerID = CopyBuffer(1, 2);
		iPlayerID = std::atoi(PlayerID.c_str());
		//std::cout << PlayerID << ":" << iPlayerID << std::endl;
		//deal with action 3
		switch (buffer[3])
		{
			case 'L':
				//std::cout << "Case : L" << std::endl;
				for (int i = 0; i < MaxPlayers; i++)
				{
					if (Clients[i].playerID.compare(PlayerID) == 0)
					{
						Clients[i].player.SetExPos(Clients[i].player.GetExPos() - 4.25f); 
						Clients[i].Changed = true;
					}
				}
				SendData("L");
				break;
			case 'U':
				//std::cout << "Case : U" << std::endl;
				for (int i = 0; i < MaxPlayers; i++)
				{
					if (Clients[i].playerID.compare(PlayerID) == 0)
					{
						Clients[i].player.SetEyPos(Clients[i].player.GetEyPos() - 4.25f);
						Clients[i].Changed = true;
					}
				}
				SendData("U");
				break;
			case 'D':
				//std::cout << "Case : D" << std::endl;
				for (int i = 0; i < MaxPlayers; i++)
				{
					if (Clients[i].playerID.compare(PlayerID) == 0)
					{
						Clients[i].player.SetEyPos(Clients[i].player.GetEyPos() + 4.25f);
						Clients[i].Changed = true;
					}
				}
				SendData("D");
				break;
			case 'R':
				//std::cout << "Case : R" << std::endl;
				for (int i = 0; i < MaxPlayers; i++)
				{
					if (Clients[i].playerID.compare(PlayerID) == 0)
					{
						Clients[i].player.SetExPos(Clients[i].player.GetExPos() + 4.25f);
						Clients[i].Changed = true;
					}
				}
				SendData("R");
				break;
		}
		break;
	}
}

void UDP_Server::ConnectClient(std::string PlayerID)
{
	int address = clientData.sin_addr.S_un.S_addr;
	short port = clientData.sin_port;
	//check if player is already connected
	for (int i = 0; i < MaxPlayers; i++)
	{
		if (Clients[i].Network.GetAddress() == address && Clients[i].Network.port == port)
		{
			std::cout << Clients[i].playerID << "Already Assigned!" << std::endl;
			SendData("1" + Clients[i].playerID + "A");
			return;
		}
	}
	//check for an empty slot
	for (int i = 0; i < MaxPlayers; i++)
	{
		//std::cout << Clients[i].playerID << " : " << Clients[i].playerID.compare("00") << std::endl;
		if (Clients[i].playerID.compare("00") == 0)
		{
			//Clients[i] = Client(clientData.sin_addr.S_un.S_addr, clientData.sin_port);
			Clients[i] = Client(address, port);
			std::cout << "Assigning" << std::endl;
			char pID[2];
			if (PlayerID.compare("00") != 0)
			{
				Clients[i].playerID = PlayerID;
				std::cout << Clients[i].playerID << "Given Assigned!" << std::endl;
				SendData("1" + Clients[i].playerID + "A");
				return;
			}
			else
			{
				//Assign Player ID
				std::stringstream ss;
				ss << std::setw(2) << std::setfill('0') << i+1;
				std::string s = ss.str();
				Clients[i].playerID = s;
				std::cout << Clients[i].playerID << "New Assigned!" << std::endl;
				Clients[i].NewPlayer();
				//Send player acceptance w/ID
				SendData("1"+ Clients[i].playerID + "A");
				return;
			}
		}
	}
	std::cout << "Server Full!" << std::endl;
	SendData("100R");
}

std::string UDP_Server::CopyBuffer(int start, int end)
{
	std::string stringBuild = "";
	for (int i = start; i <= end; i++)
	{
		stringBuild += buffer[i];
	}
	return stringBuild;
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
