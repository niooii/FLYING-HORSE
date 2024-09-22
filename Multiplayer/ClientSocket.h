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
	~ClientSocket();

	void ConnectTo(const char* ip, unsigned short port);
	void SendString(std::string str);
	
	inline SOCKET GetSocket() const
	{
		return sock;
	};
	
	// for CERTAIN situations...
	void ForceClose();
};

