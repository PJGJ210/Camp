#pragma once
#include "ClientNetwork.h"
#include "Player.h"
#include <iostream>

class Client
{
public:
	Client();
	Client(int address, short port);
	void SetNetwork(int address, short port);
	void NewPlayer();
	std::string GetplayerID();
	~Client();

	bool Changed;
	ClientNetwork Network;
	Player player;
	std::string playerID;
};

