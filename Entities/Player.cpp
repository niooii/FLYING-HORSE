#include "Player.h"

#include "Entity.h"

void Player::handleInput() {

	if (global::IInteracted && config::launchForce != config::defaultLaunchForce)
		config::launchForce = config::defaultLaunchForce;

	double deltaTime = info->deltaTime;

	if (global::IInteracted && !global::bossActive)
		deltaTime *= 0.1;

	//handle input keystrokes
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if (keystates[SDL_SCANCODE_SPACE] || keystates[SDL_SCANCODE_W])
		velocity.y += 20 * deltaTime;
	if (keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_LSHIFT])
		velocity.y -= 20 * deltaTime;
	if (keystates[SDL_SCANCODE_A])
	{
		velocity.x -= 20 * deltaTime;
		isFlipped = true;
	}
	if (keystates[SDL_SCANCODE_D])
	{
		velocity.x += 20 * deltaTime;
		isFlipped = false;
	}
}


void Player::update()
{
	double deltaTime = info->deltaTime;

	if (global::IInteracted && !global::bossActive)
		deltaTime *= 0.01;

	deltaTimeSum += deltaTime;

	Uint16 bottom = info->h;
	Uint16 top = 0;

	Uint16 right = info->w;
	Uint16 left = 0;

	//name misleading, bool states if object is currently colliding and has momentum into the border.
	bool isAtFloor = y + size.h >= bottom && velocity.y < 0;
	bool isAtCeiling = y <= top && velocity.y > 0;

	bool isAtRightBorder = x + size.w >= right && velocity.x > 0;
	bool isAtLeftBorder = x <= left && velocity.x < 0;

	//logic for borders of screen
	if (isAtFloor || isAtCeiling)
	{
		velocity.y = -velocity.y;
		velocity.y -= (0.65 * velocity.y);
	}
	else
	{
		double distanceY = -(velocity.y * deltaTime + 0.5 * acceleration.y * deltaTime * deltaTime);
		velocity.y = velocity.y + acceleration.y * deltaTime;

		y += (distanceY * constants::pxPerMeter);
	}

	if (isAtLeftBorder || isAtRightBorder)
	{
		velocity.x = -velocity.x;
		velocity.x -= (0.65 * velocity.x);
	}
	else
	{
		double distanceX = (velocity.x * deltaTime + 0.5 * acceleration.x * deltaTime * deltaTime);
		velocity.x = velocity.x + acceleration.x * deltaTime;

		x += (distanceX * constants::pxPerMeter);
	}

	Uint16 clipThreshold = 5;
	//fix clipping into borders
	if (y + size.h > bottom + clipThreshold)
		y = bottom - size.h;

	if (y < top - clipThreshold)
		y = top;

	if (x + size.w > right + clipThreshold)
		x = right - size.w;

	if (x < left - clipThreshold)
		x = left;

	//apply friction when not stupid
	velocity.x -= velocity.x * constants::frictionalForce * deltaTime;

	handleInput();

	updateBox();
}

void Player::pollEvent(SDL_Event& event) {
	//kinda reset velocity
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_q)
			velocity.x = isFlipped ? 2 : -2;
		else if (event.key.keysym.sym == SDLK_r)
		{
			//using constants namespace
			config::launchForce = config::defaultLaunchForce;
			config::siphonRate = config::defaultSiphonRate;
			config::recoil = true;
			if (config::outputEnabled)
				std::cout << "Restored default state." << '\n';
		}
	}
}