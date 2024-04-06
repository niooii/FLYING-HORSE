#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "Entities/Entity.h"
#include "Entities/Boss.h"
#include "Entities/Player.h"
#include "Entities/Projectile.h"
#include <iostream>
#include <vector>
#include <string>
#include "globals/Constants.h"
#include "TextRenderer.h"
#include "Utility/Utility.h"
#include "globals/global.h"
#include "globals/Config.h"

#include "globals/Textures.h"
#include "Geometry/Dodecahedron.h"
#include "Utility/Timer.h"
#include <Multiplayer/ClientSocket.h>
#include <Multiplayer/Serde.h>
#include <globals/Config.h>

static ClientSocket socket_{};
// render a bunch of other peopels shit
static std::vector<std::pair<std::string, GameState>> other_ppl_gamestate;
static std::mutex other_ppl_mutex;
static Timer update_server_timer;

class Game {
private:
	std::string display_name;
	SDL_Color bg{ 255,255,255 };

	TextRenderer textRenderer;
	TextRenderer tr2;
	TextRenderer name_renderer;
	TextRenderer other_name_renderer;

	SDL_DisplayMode display;

	Timer timer;
	Timer borderTimer;
	Timer elapsedTime;
	Timer collideDamageTimer;

	Uint64 NOW{}, LAST{};
	SDL_Event event;
	int exitCount{};
	double decreasingVar{-100.006};

	void handleKeyInput(const SDL_Keycode& sym);
	void handleRawInput();
	double percentVariance(double now, double old);
	double deathTimer{}; //even more omnimous declaration
	bool triedLeaving{ false };
	double dTimeSum{};
	void renderBoss();
	void handleBossCollisions();

	Dodecahedron d{nullptr, nullptr};

	static void HandleServerReplication(Player* player, std::vector<Projectile>* projectiles)
	{
		constexpr size_t buffsize = 131072;
		char* buffer = new char[buffsize];
		//printf("Listening for replicated data from server...\n");
		while (true)
		{
			if (update_server_timer.elapsed() > 1 / 144.0)
			{
				update_server_timer.reset();


				memset(buffer, 0, buffsize);
				if (config::outputEnabled)
				{
					printf("waiting on server to dispatch states...\n");
				}
				if (recv(socket_.GetSocket(), buffer, buffsize, 0) == 0)
				{
					printf("server has died unlucky");
					break;
				}
				if (config::outputEnabled)
				{
					printf("recieved server states...\n");
				}
				auto replicated_data = Serde::DeserializeServerReplication(buffer);
				{
					other_ppl_mutex.lock();
					other_ppl_gamestate = replicated_data;
					other_ppl_mutex.unlock();
				}

				std::vector<Vector2> projectile_pos_vec{};

				for (const auto& p : *projectiles)
				{
					projectile_pos_vec.emplace_back(p.x, p.y);
				}
				if (config::outputEnabled)
				{
					printf("dispatched local state...\n");
				}
;				socket_.SendString(Serde::SerializeState(Vector2{ player->x, player->y }, player->isFlipped, projectile_pos_vec));
			}

			
		}

		delete buffer;
	}

public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	winfo info;

	Player player;
	Boss boss;
	std::vector<Projectile> projectiles;

	int mouseX{}, mouseY{};
	
	Game(const char* title, Uint16 width, Uint16 height);
	~Game();
	void addEntity(Projectile& entity);
	//void addEntity(Player& player);
	//void addEntity(Boss& boss);
	void render();
	void handleEvents();
	void quit();
	void clear();

	void startBoss();
};