#include "Projectile.h"

#include "Entity.h"

void Projectile::handleInput() {
	double deltaTime = info->deltaTime;
	//handle input keystrokes
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if (global::bossActive && !boss::siphonPhase)
		return;

	if (keystates[SDL_SCANCODE_UP])
		velocity.y += 30 * deltaTime;
	if (keystates[SDL_SCANCODE_DOWN])
		velocity.y -= 30 * deltaTime;
	if (keystates[SDL_SCANCODE_LEFT])
	{
		velocity.x -= 30 * deltaTime;
		isFlipped = true;
	}
	if (keystates[SDL_SCANCODE_RIGHT])
	{
		velocity.x += 30 * deltaTime;
		isFlipped = false;
	}

	//right click ball siphoning

	int mouseX{}, mouseY{};
	//S U C K
	if ((SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON_RMASK))
	{

		Vector2 projectileVelocity(mouseX - (x + size.w/2.0), -(mouseY - (y + size.h/2.0)));
		projectileVelocity.normalize();
		
		//std::cout << projectileVelocity.x << ", " << projectileVelocity.y << '\n';
		velocity = velocity + (projectileVelocity * config::siphonRate *  deltaTime);

	}

	//if reversing, repel
	if (global::reverse)
	{
		Vector2 projectileVelocity(mouseX - (x + size.w / 2.0), -(mouseY - (y + size.h / 2.0)));
		projectileVelocity.normalize();

		projectileVelocity.x = -projectileVelocity.x;
		projectileVelocity.y = -projectileVelocity.y;

		velocity = velocity + (projectileVelocity * config::siphonRate * 0.5 * deltaTime);
	}

}

void Projectile::update()
{
	if (global::leaking)
	{
		//cause a memory leak!!!
		TTF_Font* aaaaa = TTF_OpenFont("Fonts/lazy.ttf", 99);
		SDL_Surface* SUFA = TTF_RenderText_Solid_Wrapped(aaaaa, "BAHAHAHAHHAHAHHAHAHAH", { 0,0,0 }, 0);
		SDL_Texture* DWABOUTIT = SDL_CreateTextureFromSurface(renderer, SUFA);
		//done
		return;
	}

	if (outOfView)
		return;

	double deltaTime = info->deltaTime;

	if (global::IInteracted && config::siphonRate != config::defaultSiphonRate)
		config::siphonRate = config::defaultSiphonRate;

	if (global::IInteracted && !global::bossActive)
		deltaTime *= 0.01;

	Uint16 bottom = info->h;
	Uint16 top = 0;

	Uint16 right = info->w;
	Uint16 left = 0;

	//name misleading, bool states if object is currently colliding and has momentum into the border.
	bool isAtFloor = y + size.h >= bottom && velocity.y < 0;
	bool isAtCeiling = y <= top && velocity.y > 0;

	bool isAtRightBorder = x + size.w >= right && velocity.x > 0;
	bool isAtLeftBorder = x <= left && velocity.x < 0;
	
	if (global::reverse)
	{
		isAtFloor = false;
		isAtCeiling = false;
		isAtRightBorder = false;
		isAtLeftBorder = false;
	}

	//logic for borders of screen
	if (isAtFloor || isAtCeiling)
	{
		//if reverse mode, no collision!
			velocity.y = -velocity.y;
			velocity.y -= (0.65 * velocity.y);
	}
	else
	{
		double distanceY = -(velocity.y * deltaTime + 0.5 * acceleration.y * deltaTime * deltaTime);
		velocity.y = velocity.y + acceleration.y * deltaTime;

		/*if (constants::reverse)
			distanceY = -distanceY;*/

		y += (distanceY * constants::pxPerMeter);
	}

	if (isAtLeftBorder || isAtRightBorder)
	{
		//if reverse mode, no collision!
			velocity.x = -velocity.x;
			velocity.x -= (0.65 * velocity.x);
	}
	else
	{
		double distanceX = (velocity.x * deltaTime + 0.5 * acceleration.x * deltaTime * deltaTime);
		velocity.x = velocity.x + acceleration.x * deltaTime;

		/*if (constants::reverse)
			distanceX = -distanceX;*/

		x += (distanceX * constants::pxPerMeter);
	}

	Uint16 clipThreshold = 5;
	//fix clipping into borders
	if (global::reverse && !global::bossActive) 
	{
		//weird ass behaviour
		if (y + size.h > bottom + clipThreshold || y < top - clipThreshold)
		{
			x = info->mX - size.w/2.0;
			y = info->mY - size.h/2.0;
		}
			

		if (x + size.w > right + clipThreshold || x < left - clipThreshold)
		{
			x = info->mX - size.w / 2.0;
			y = info->mY - size.h / 2.0;
		}
	}
	//special behaviour if in boss fight
	else if (global::bossActive)
	{
		if (y + size.h > bottom + clipThreshold || y < top - clipThreshold
			|| x + size.w > right + clipThreshold || x < left - clipThreshold)
		{
			outOfView = true;
		}
	}
	else
	{
		if (collisions)
		{
			//normal behaviour
			if (y + size.h > bottom + clipThreshold)
				y = bottom - size.h;

			if (y < top - clipThreshold)
				y = top;

			if (x + size.w > right + clipThreshold)
				x = right - size.w;

			if (x < left - clipThreshold)
				x = left;
		}
	}
	

	//apply friction when not stupid
	if(friction)
		velocity.x -= velocity.x * constants::frictionalForce * deltaTime;

	updateBox();

	handleInput();
}

void Projectile::pollEvent(SDL_Event& event) {
	//kinda reset velocity
	/*if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_q)
			velocity.x = isFlipped ? 2 : -2;
		else if (event.key.keysym.sym == SDLK_r)
			launchForce = defaultLaunchForce;
	}*/
}