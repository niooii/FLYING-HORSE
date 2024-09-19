#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Vector2.h"
#include "globals/Constants.h"
#include "globals/global.h"
#include "globals/config.h"
#include "Utility/Utility.h"
#include "Utility/Timer.h"

// Forward declaration of Player class
class Player;
class Projectile;
class Boss;
class SingleplayerGame;

class Entity {
protected:
	SDL_Rect dest;
	SDL_Renderer* renderer;
	winfo* info;
	SingleplayerGame* inst;

public:
	SDL_Rect hitbox;

	bool collisions{ true };
	double x;
	double y;
	double rotation{};
	bool isFlipped{ false };
	SDL_Rect size;
	Vector2 acceleration{ 0, constants::gravity };
	Vector2 velocity{};
	SDL_Texture* texture = nullptr;
	Entity();
	Entity(SingleplayerGame* inst, SDL_Texture* texture, SDL_Renderer* renderer, winfo* info, double x, double y);
	//moves simulation of entity forward by some amount of time (undecided)
	void update();
	void render();
	void updateBox();
	void pollEvent(SDL_Event& event);
};