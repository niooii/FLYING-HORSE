#pragma once

#include <vector>
#include <Vector2.h>

struct GameState
{
	std::vector<Vector2> projectile_positions;
	Vector2 player_position;
	int is_player_flipped;
};

struct ConnectionInfo
{
	unsigned int sock;
	sockaddr_in clientAddr;
};

struct ClientInfo
{
	ConnectionInfo connection_info;
	std::string name;
	GameState state;
	std::string serialized_gamestate;
};

