#include "Client.h"



Client::Client(int address, short port)
{
	Network = ClientNetwork(address, port);
}


Client::~Client()
{
}
