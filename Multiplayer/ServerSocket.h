#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <string>

#define VERBOSE

struct ClientConnectionInfo 
{
	unsigned int sock;
	sockaddr_in clientAddr;
	std::string display_name;
};

class ServerSocket
{
protected:
	unsigned int sock;
	short port;

	static void HandleIncomingConnection(ClientConnectionInfo connection_info) 
	{
		printf("Created new thread for client %s\n", connection_info.display_name.c_str());
		while (connection_info.sock != INVALID_SOCKET) 
		{

		}
		printf("lost connection i guess");
	}

public:
	ServerSocket(short PORT);

	void Listen();
};

