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

class SingleplayerGame {
private:
	std::string display_name;
	SDL_Color bg{ 255,255,255 };

	TextRenderer textRenderer;
	TextRenderer tr2;

	SDL_DisplayMode display;
	bool over{ false };

	Timer timer;
	Timer borderTimer;
	Timer elapsedTime;
	Timer collideDamageTimer;

	Uint64 NOW{}, LAST{};
	SDL_Event event;
	int exitCount{};
	double decreasingVar{ -100.006 };

	void handleKeyInput(const SDL_Keycode& sym);
	void handleRawInput();
	double percentVariance(double now, double old);
	double deathTimer{}; //even more omnimous declaration
	bool triedLeaving{ false };
	double dTimeSum{};
	void renderBoss();
	void handleBossCollisions();

	Dodecahedron d{ nullptr, nullptr };

public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	winfo info;

	Player player;
	Boss boss;
	std::vector<Projectile> projectiles;

	int mouseX{}, mouseY{};

	SingleplayerGame(const char* title, Uint16 width, Uint16 height);
	~SingleplayerGame();
	void addEntity(Projectile& entity);
	//void addEntity(Player& player);
	//void addEntity(Boss& boss);
	void render();
	void handleEvents();
	void quit();
	void clear();

	void startBoss();
};