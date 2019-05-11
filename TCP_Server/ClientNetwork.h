#pragma once
#include <iostream>
#include <WS2TCPip.h>

class ClientNetwork
{
public:
	ClientNetwork();
	ClientNetwork(int address, short port);
	int GetAddress();
	short GetPort();
	~ClientNetwork();

	int address;
	short port;
};

