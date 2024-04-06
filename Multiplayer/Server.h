#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <string>
#include <map>
#include <vector>
#include <Vector2.h>
#include <Multiplayer/Serde.h>
#include <Multiplayer/ClientInfo.h>
#include <mutex>
#include <Utility/Timer.h>

#define VERBOSE

enum Stage
{
	Peace,
	BossPhase
};

static unsigned long UID;
static std::map<unsigned long, ClientInfo> clients;
static std::mutex client_mutex;

class Server
{
protected:
	unsigned int sock;
	short port;
	
	static void HandleIncomingConnection(ConnectionInfo connection_info, std::string display_name) 
	{
		printf("Created new thread for client %s\n", display_name.c_str());
		constexpr size_t buffsize = 32768;
		char* buffer = new char[buffsize];
		unsigned long uid = UID++;
		client_mutex.lock();
		clients[uid] = {};
		clients[uid].name = display_name;
		clients[uid].connection_info = connection_info;
		client_mutex.unlock();
		while (true) 
		{
			memset(buffer, 0, buffsize);
			if (recv(connection_info.sock, buffer, buffsize, 0) == 0)
			{
				{
					std::lock_guard<std::mutex> clients_guard(client_mutex);
					printf("%s's connection has been gracefully closed.\n", clients[uid].name.c_str());
				}
				break;
			}

			GameState state = Serde::DeserializeState(buffer);
			client_mutex.lock();
			clients[uid].state = state;
			clients[uid].serialized_gamestate = Serde::SerializeState(state.player_position, state.is_player_flipped, state.projectile_positions);
			client_mutex.unlock();
		}
		printf("cleaning up..\n");
		delete buffer;
		client_mutex.lock();
		clients.erase(uid);
		client_mutex.unlock();
	}

	/*
	* Right now, all states are replicated onto a client every time
	* their state is sent from the client itself.
	* fix with udp.
	* ideally, clients should be able to send state to the server
	* while the server continuously sends state back to the client.
	* SHOULD ONLY BE CALLED WHEN MUTEX IS UNLOCKED
	* nvm i fixed whatever hte fuck was happening here
	*/
	static void ReplicateStatesToClients()
	{
		if (clients.empty())
		{
			return;
		}
		// replication
		for (auto const& [uid, client] : clients)
		{
			std::stringstream ss;
			for (auto const& [uid_, client_] : clients)
			{
				if (uid == uid_)
					continue;
				ss << client_.name << '~' << client_.serialized_gamestate << '~';
			}
			std::string serialized_states = ss.str();
			send(client.connection_info.sock, serialized_states.c_str() + '\0', serialized_states.size() + 1, 0);
		}
	}

	static void ReplicationLoop()
	{
		Timer timer{};
		while (true)
		{
			if (timer.elapsed() > 1 / 144.0)
			{
				timer.reset();
				ReplicateStatesToClients();
			}
		}
	}


public:
	Server(short PORT);
	~Server();

	void Run();
};

