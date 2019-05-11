#include "Client.h"



Client::Client()
{
	playerID = "00";
}

Client::Client(int address, short port)
{
	Network = ClientNetwork(address, port);
}

void Client::SetNetwork(int address, short port)
{
	Network = ClientNetwork(address, port);
}

std::string Client::GetplayerID()
{
	return playerID;
}


Client::~Client()
{
}
