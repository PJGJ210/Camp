#include "ClientNetwork.h"
#include <iostream>

ClientNetwork::ClientNetwork()
{
}


ClientNetwork::ClientNetwork(int address, short port)
{
	this->address = address;
	this->port = port;
}

int ClientNetwork::GetAddress()
{
	return address;
}

short ClientNetwork::GetPort()
{
	return port;
}

ClientNetwork::~ClientNetwork()
{
}
