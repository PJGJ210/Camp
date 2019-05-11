#pragma once
#include "ClientNetwork.h"
#include <iostream>

class Client
{
public:
	Client();
	Client(int address, short port);
	void SetNetwork(int address, short port);
	std::string GetplayerID();
	~Client();

	ClientNetwork Network;
	std::string playerID;
};

