#pragma once

#include <windows.h>
#include <stdio.h>
#include <string>

class ClientSocket
{
protected:
	unsigned int sock;

public:
	ClientSocket();

	void ConnectTo(const char* ip, unsigned short port);
	void SendString(std::string str);
};

