#pragma once
#include <SDL.h>
#include "globals/BossGlobals.h"
#include "Vector2.h"
#include "globals/Constants.h"
#include "Utility/Utility.h"
#include "Player.h"
#include "Projectile.h"
#include <vector>
#include "TextRenderer.h"
#include "Entity.h"
#include "globals/Textures.h"
#include "Geometry/Dodecahedron.h"

class Boss : public Entity
{
private:
	TextRenderer text;
	boss::state state;

	Dodecahedron ult;
	Dodecahedron ultWarning;

	Polyhedron beams;
	Polyhedron beams2;

	Timer timer;
	Timer throwTimer;
	Timer ultTimer;
	Timer beamDamageTimer;
	Timer beam2DamageTimer;
	Timer spiral_angle_inc;
	//init at init
	Player* player{};
	int toThrow{90};
	int thrown{};

	void updateBox();

	TextRenderer healthCounter;

	bool ultActive{ false };
	
	int red{};
	bool playerHit{false};
public:
	SDL_Rect hitbox{};
	bool over{ false };
	std::vector<Projectile> projectiles;
	std::vector<Projectile> border;
	int health{ 4000 };
	SDL_Rect size{};
	Vector2 velocity{};
	using Entity::Entity;
	void idle();
	void Throwing();
	void Pulse(double x, double y, double vel, double accel, SDL_Texture* tex);
	//pulses centered on boss.
	void Pulse();
	void EndingSpiral();
	void Spiral(double x, double y, SDL_Texture* startex);
	void Ulting();
	void borderInit();
	//moves simulation of entity forward by some amount of time (undecided)
	void update();
	void init(Player* target);
	void render(SDL_Renderer* renderer);
};