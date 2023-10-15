#pragma once
#include <iostream>
#include "Entity.h"

class Player : public Entity {
private:
	SDL_Texture* projectileTexture{};
	double launchForceFinal(double launchForce, int d);
protected:
	double deltaTimeSum{};
public:
	int health{(global::infhp ? 1000000 : 6000)};

	using Entity::Entity;
	void handleInput();
	void update();
	void pollEvent(SDL_Event& event);
};