#pragma once

#include <vector>
#include <Vector2.h>
#include <Multiplayer/ClientInfo.h>
#include <sstream>
#include <iomanip>

namespace Serde 
{
	/* serialization format :
	* player_x,player_y,is_player_flipped|projectile_1_x,projectile_1_y|projectile_2_x...projectile_n_y|\0
	*
	* IF state is being replicated to a client, it will be sent in one big buffer.
	* delimiter for gamestates will be '~', with the name
	* sent before each one. ex:
	* player1~player_x,player_y,is_player_flipped|projectile_1_x,projectile_1_y|projectile_2_x~player2~player_x...
	* */
	static GameState DeserializeState(const char* buffer)
	{
		//printf("recieved %s", buffer);

		GameState state{};

		std::stringstream ss{ buffer };
		std::string token;
		std::getline(ss, token, '|');
		sscanf_s(token.c_str(), "%lf,%lf,%d", &(state.player_position.x), &(state.player_position.y), &(state.is_player_flipped));

		while (std::getline(ss, token, '|')) {
			Vector2 p_pos;
			sscanf_s(token.c_str(), "%lf,%lf", &p_pos.x, &p_pos.y);
			state.projectile_positions.push_back(p_pos);
		}

		return state;
	}

	// returns pair<name, gamestate>
	static std::vector<std::pair<std::string, GameState>> DeserializeServerReplication(const char* buffer)
	{
		std::stringstream ss{ buffer };
		std::string chunk;

		std::vector<std::pair<std::string, GameState>> ret{};

		while (std::getline(ss, chunk, '~'))
		{
			// parse name
			std::string name = chunk;
			// parse gamestate
			std::getline(ss, chunk, '~');
			GameState gamestate = DeserializeState(chunk.c_str());

			ret.emplace_back(name, gamestate);
		}

		return ret;
	}
	
	static std::string SerializeState(Vector2 player_position, bool is_player_flipped, std::vector<Vector2>& projectile_positions)
	{
		std::stringstream ss;

		ss << std::setprecision(2) << std::fixed << player_position.x << ',' << player_position.y << ',' << is_player_flipped << "|";

		for (const auto& pos : projectile_positions) {
			ss << pos.x << "," << pos.y << "|";
		}

		std::string state = ss.str();
		// remove the last '|'
		// i kinda need this no?
		/*if (!state.empty()) {
			state.pop_back();
		}*/

		state.push_back('\0');

		return state;
	}
}