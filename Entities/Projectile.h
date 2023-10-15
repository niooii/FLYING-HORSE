#pragma once
#include "globals/BossGlobals.h"
#include <iostream>
#include "Entity.h"

class Projectile : public Entity {
public:
	bool friction{true};
	using Entity::Entity;
	bool outOfView{false};
	void handleInput();
	void update();
	void pollEvent(SDL_Event& event);
};