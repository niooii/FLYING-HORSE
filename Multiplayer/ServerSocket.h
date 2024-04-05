#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

class ServerSocket
{
protected:
	unsigned int sock;

	void HandleIncomingConnection();

public:
	ServerSocket(short PORT);

	void Listen();
};

