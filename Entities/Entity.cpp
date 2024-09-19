#include "Entity.h"
#include <iostream>
#include "SingleplayerGame.h"

Entity::Entity()
{

}

Entity::Entity(SingleplayerGame* inst, SDL_Texture* texture, SDL_Renderer* renderer, winfo* info, double x, double y) {
	//be careful if texture gets destroyed
	this->texture = texture;
	this->x = x;
	this->y = y;
	this->info = info;
	this->renderer = renderer;
	this->inst = inst;
	SDL_QueryTexture(texture, NULL, NULL, &size.w, &size.h);
}

void Entity::update() 
{
	double deltaTime = info->deltaTime;

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
		//make other entities bounce more for some reason !
		velocity.y -= (0.45 * velocity.y);
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
}

void Entity::pollEvent(SDL_Event& event) {
	if (event.type == SDL_MOUSEBUTTONDOWN)
		if (event.button.type == SDL_BUTTON_LEFT)
			velocity.y = 6;
	if(event.type == SDL_KEYDOWN)
		if(event.key.keysym.sym == SDLK_q)
			velocity.x = isFlipped ? 2 : -2;
}

void Entity::render()
{
	//updateBox();
	
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopyEx(renderer, texture, NULL, &dest, rotation, NULL, (isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));

	if (constants::renderHitboxes && global::bossActive)
		SDL_RenderDrawRect(renderer, &hitbox);
}

void Entity::updateBox()
{
	hitbox.x = x + size.w * 0.15;
	hitbox.y = y + size.h * 0.15;
	hitbox.w = size.w * 0.7;
	hitbox.h = size.h * 0.7;
}