#pragma once
#include "ClientNetwork.h"
#include <iostream>

class Client
{
public:
	Client();
	Client(int address, short port);
	~Client();


	ClientNetwork Network;
};

