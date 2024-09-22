#include "Boss.h"
#include <string>

#include "Entity.h"
#include "SingleplayerGame.h"

Vector2 beam1_pos{};
Vector2 beam2_pos{};
float beam1_dr;
float beam2_dr;
Timer beam1_pulse_timer{};
Timer beam2_pulse_timer{};
float beam1_rot_speed_mult = 1.f;
float beam2_rot_speed_mult = 1.f;
Vector3 beam1_col_mult{255, 255, 255};
Vector3 beam2_col_mult{255, 255, 255};

void Boss::update()
{
	double deltaTime = info->deltaTime;

	double floatSpeed = 2;
	double floatRange = info->h / 20.0;

	beam1_rot_speed_mult -= deltaTime * 5;
	if (beam1_rot_speed_mult < 1)
		beam1_rot_speed_mult = 1;

	beam2_rot_speed_mult -= deltaTime * 5;
	if (beam2_rot_speed_mult < 1)
		beam2_rot_speed_mult = 1;
	
	Vector3 sub_vec{ deltaTime * 400, deltaTime * 400 , deltaTime * 400 };
	beam1_col_mult = beam1_col_mult + sub_vec;
	if (beam1_col_mult.x > 255)
		beam1_col_mult.x = 255;
	if (beam1_col_mult.y > 255)
		beam1_col_mult.y = 255;
	if (beam1_col_mult.z > 255)
		beam1_col_mult.z = 255;

	beam2_col_mult = beam2_col_mult + sub_vec;
	if (beam2_col_mult.x > 255)
		beam2_col_mult.x = 255;
	if (beam2_col_mult.y > 255)
		beam2_col_mult.y = 255;
	if (beam2_col_mult.z > 255)
		beam2_col_mult.z = 255;

	if (projectiles.size() > constants::entityLimit * 8)
	{
		projectiles.erase(projectiles.begin());
	}
	//std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) {return p.outOfView; });

	if (timer.elapsed() > 5 && health > 3250)
		state = boss::state::Throwing;
	else if (health <= 3000 && health > 2600)
		state = boss::state::ThrowingBeam;
	else if (health <= 2500 && health > 2100)
	{
		/*if(health % 50 == 0)
			Pulse();*/
		state = boss::state::Ultimate;
	}
	else if (health <= 2000 && health > 1100)
	{
		state = boss::state::ThrowingBeam2;
	}
	else if (health <= 800 && health > 0 && !over)
	{
		state = boss::state::Final;
	}
	else if (health <= 0 || over)
	{
		if (!over)
		{
			health = 100;
		}
		over = true;
		state = boss::state::Idle;
		/*info->w = 600;
		info->h = 600;*/

		x = info->w / 2;
		y = info->h / 2;
		
		// 0.12
		if (timer.elapsed() > 0.12)
		{
			timer.reset();
			EndingSpiral();
		}
		texture = textures::bossFinal;
		return;
	}
	else
	{
		//beams.move((isFlipped ? x + size.w / 2.0 : x), y + size.h / 2.0);
		ultActive = false;
		thrown = 0;
		idle();
		state = boss::state::Idle;
	}

	switch (state)
	{
	case boss::state::Idle:
		x = info->w / 1.5;
		y = (floatRange * (std::sin(timer.elapsed() * floatSpeed)) + info->h / 2.0) - size.h / 2.0;
		//HITBOX (very impoirtant yes)
		updateBox();
		return;
	case boss::state::Throwing: case boss::state::ThrowingBeam: case boss::state::ThrowingBeam2:
		Throwing();
		//update hitboxes within throwing
		return;
	case boss::state::Ultimate:
		x = info->w / 1.5;
		y = (floatRange * (std::sin(timer.elapsed() * floatSpeed)) + info->h / 2.0) - size.h / 2.0;
		//HITBOX (very impoirtant yes)
		updateBox();
		//if (!ultActive)
		//	y = -200;
		Ulting();
		return;
	case boss::state::Final:
		Throwing();
		Ulting();
		return;
	}

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
}

//initialize boss target & other objects
void Boss::init(Player* target)
{
	player = target;
	healthCounter = TextRenderer(info, renderer, "a", "Fonts/lazy.ttf", 50, { 0,0,0 });
	idle();
	ult = Dodecahedron(inst, info, textures::circle, renderer);
	ult.construct(0.3);
	ult.scale(1);
	ult.move(info->w * 0.5, info->h * 0.5);
	ult.showTexture = false;

	ultWarning = Dodecahedron(inst, info, textures::circle, renderer);
	ultWarning.construct(0.003);
	ultWarning.scale(1);
	ultWarning.move(info->w * 0.5, info->h * 0.5);

	//ult.rotate(0, 0, 1.2);
	//ultWarning.rotate(0, 0, 1.2);
	ult.setRotation(M_PI / 4.0, 0, 0);
	ultWarning.setRotation(M_PI/4.0, 0, 0);

	ultWarning.pointsOnly = true;

	//initialize beams polyhedron
	static const double SPACING = 0.025;
	beams = Polyhedron(inst, info, textures::purpleStar, renderer);
	beams.connectPoints({0,0,0}, { (double)(-info->w) * 2, 0, 0}, info->h * SPACING);
	beams.connectPoints({ 0,0,0 }, { (double)(info->w) * 2, 0, 0 }, info->h * SPACING);
	beams.connectPoints({ 0,0,0 }, { 0, (double)(-info->h) * 2, 0}, info->h * SPACING);
	beams.connectPoints({ 0,0,0 }, { 0, (double)(info->h) * 2, 0 }, info->h * SPACING);
	beam1_pos = { info->w / 2.0, info->h / 2.0 };
	beams.move(beam1_pos.x, beam1_pos.y);
	beams.scale(2);

	beams2 = Polyhedron(inst, info, textures::greenStar, renderer);
	beams2.connectPoints({ 0,0,0 }, { (double)(-info->w) * 2, 0, 0 }, info->h * SPACING);
	beams2.connectPoints({ 0,0,0 }, { (double)(info->w) * 2, 0, 0 }, info->h * SPACING);
	beams2.connectPoints({ 0,0,0 }, { 0, (double)(-info->h) * 2, 0 }, info->h * SPACING);
	beams2.connectPoints({ 0,0,0 }, { 0, (double)(info->h) * 2, 0 }, info->h * SPACING);
	beams2.connectPoints({ 0,0,0 }, { 0, 0, (double)(info->w) * 2 }, info->h * SPACING);
	beams2.connectPoints({ 0,0,0 }, { 0, 0, (double)(-info->w) * 2 }, info->h * SPACING);
	beam2_pos = { info->w / 2.0, info->h / 2.0 };
	beams2.rotate(22 * 3.14, 22 * 3.14, 22 * 3.14);
	beams2.move(beam2_pos.x, beam2_pos.y);
	beams2.scale(2);


	timer.reset();
	throwTimer.reset();
	ultTimer.reset();
}

//states
void Boss::idle()
{
	texture = textures::bossIdle;
	state = boss::state::Idle;
	SDL_QueryTexture(texture, NULL, NULL, &size.w, &size.h);
}

void Boss::Throwing()
{
	if (throwTimer.elapsed() > 0.025)
		texture = textures::bossIdle;
	if (thrown < toThrow)
	{
		if (throwTimer.elapsed() > 0.05)
		{
			if (thrown == 0)
			{
				//random position & hitbox
				y = rand() % (info->h - size.h * 2) + size.h;
				x = rand() % (info->w - size.w * 2) + size.w;
				updateBox();
				//pulse
				Pulse((isFlipped ? x + size.w / 2 : x), y + size.h / 2, 0, 10, textures::redStar);
			}
			else if (thrown == toThrow / 3 || thrown == 2 * toThrow / 3)
				Spiral((isFlipped ? x + size.w / 2 : x), y + size.h / 2, textures::redStar);

			throwTimer.reset();

			texture = textures::bossThrow;
			Vector2 a((player->x + player->size.w / 2) - (isFlipped ? x + size.w / 2 : x), -(player->y + player->size.h / 2 - (y + size.h / 2.0)));
			a.normalize();

			Projectile star = Projectile(inst, textures::redStar, renderer, info, (isFlipped ? x + size.w / 2 : x), y + size.h / 2);
			star.acceleration = a * 20;
			star.friction = false;
			projectiles.emplace_back(star);

			//size check
			if (projectiles.size() > 1500)
			{
				projectiles.erase(projectiles.begin());
			}
			thrown++;
		}
	}
	else
	{
		//pulse
		//Pulse((isFlipped ? x + size.w / 2 : x), y + size.h / 2);
		thrown = 0;
	}
}

void Boss::Ulting()
{
	if (!ultActive)
	{
		ultActive = true;
		ultTimer.reset();
	}

	if (ultTimer.elapsed() <= 8)
	{
		if (ultTimer.elapsed() <= 1.5)
		{
			ultWarning.move(player->x + player->size.w / 2.0, player->y + player->size.h / 2.0);
			ult.move(player->x + player->size.w / 2.0, player->y + player->size.h / 2.0);
		}

		//uniform rotations
		ult.rotate(0.08 * info->deltaTime, 0.06 * info->deltaTime, 0.05 * info->deltaTime);
		ultWarning.rotate(0.08 * info->deltaTime, 0.06 * info->deltaTime, 0.05 * info->deltaTime);

		//ult.scale();
		//ultWarning.scale();
	}
	else
	{
		ult.rotate(1.5 * info->deltaTime, 1.8 * info->deltaTime, 1.65 * info->deltaTime);
		ultWarning.rotate(1.5 * info->deltaTime, 1.8 * info->deltaTime, 1.65 * info->deltaTime);
	}

	if (ultTimer.elapsed() > 0 && ultTimer.elapsed() < 8)
	{
		if (ultTimer.elapsed() <= 1)
		{
			ult.scale((info->h * 0.6));
			ultWarning.scale((info->h * 0.6));
		}

		red = std::abs(std::sin(ultTimer.elapsed() * M_PI / (( -ultTimer.elapsed() + 8)/4 + 0.5)) * 255);
		ultWarning.renderPoints({ (Uint8)red,0,0 });
		return;
	}
	if (ultTimer.elapsed() >= 8 && ultTimer.elapsed() < 10 && !playerHit)
	{
		red = 255;
		ultWarning.renderPoints({ (Uint8)red,0,0 });
		ult.render();

		ult.scale((info->h * 0.6) * (-1 / 4.0 * ((ultTimer.elapsed() - 10) * (ultTimer.elapsed() - 10))) + 1);
		ultWarning.scale((info->h * 0.6) * (-1 / 4.0 * ((ultTimer.elapsed() - 10) * (ultTimer.elapsed() - 10))) + 1);
		
		//check for collisions
		for (Projectile& p : ult.projectiles)
		{
			if (SDL_HasIntersection(&p.hitbox, &player->hitbox) && !global::hitByUlt)
			{
				global::hitByUlt = true;
			}
		}

		if (global::hitByUlt)
		{
			player->x = ult.center.x - player->size.w / 2.0;
			player->y = ult.center.y - player->size.h / 2.0;
			player->health = -2147483648;
			if (ultTimer.elapsed() >= 9.6)
				//??//?????? /  ???/ //? ????
				player->health = 0;
				true;
		}

		return;
	}
	if (ultTimer.elapsed() > 11)
	{
		ultTimer.reset();
		ultWarning.move(player->x + player->size.w / 2.0, player->y + player->size.h / 2.0);
		ult.move(player->x + player->size.w / 2.0, player->y + player->size.h / 2.0);

		ult.setRotation(M_PI / 4.0, 0, 0);
		ultWarning.setRotation(M_PI / 4.0, 0, 0);
	}
	
}

void Boss::Pulse(double x, double y, double vel, double accel, SDL_Texture* tex)
{
	Vector2 v(0, 1);
	Projectile star = Projectile(inst, tex, renderer, info, x, y);
	for (int i = 0; i <= 360; i += 5)
	{
		v.rotate(5);
		star.acceleration = v * accel;
		star.velocity = v * vel;
		star.friction = false;
		projectiles.emplace_back(star);
		
		//size check
		if (projectiles.size() > 2000)
		{
			projectiles.erase(projectiles.begin());
		}
	}
}

void Boss::Pulse()
{
	Pulse(x + size.w / 2.0, y + size.h / 2.0, 0, 10, textures::redStar);
}

void Boss::EndingSpiral()
{
	SDL_SetTextureColorMod(textures::redStar, 255 * abs(sinf(spiral_angle_inc.elapsed() * 3)), 0, 0);
	Spiral(x + size.w / 2.0, y + size.h / 2.0, textures::redStar);
}

void Boss::Spiral(double x, double y, SDL_Texture* startex)
{
	Vector2 v(0, 1);
	Vector2 r;
	Projectile star = Projectile(inst, textures::redStar, renderer, info, x, y);
	for (int i = 0; i <= 360; i += 5)
	{
		v.rotate(5);
		star.acceleration = v * 4;
		Vector2 r = v;
		r.rotate(90 + spiral_angle_inc.elapsed() * 50);
		star.velocity = r * 10;
		star.friction = false;
		projectiles.emplace_back(star);

		//size check
		if (projectiles.size() > 1500)
		{
			projectiles.erase(projectiles.begin());
		}
	}
}

void Boss::borderInit()
{	//top border and bottom border
	for (int i = 0; i <= info->w; i += (info->w * 0.03))
	{
		Projectile star = Projectile(inst, textures::redStar, renderer, info, i, 0);
		star.y = -star.size.h / 2.0;
		star.acceleration = Vector2(0, 0);
		border.emplace_back(star);

		Projectile bstar = Projectile(inst, textures::redStar, renderer, info, i, 0);
		bstar.y = info->h - bstar.size.h / 2.0;
		bstar.acceleration = Vector2(0, 0);
		border.emplace_back(bstar);
	}
	//left and right border
	for (int i = 0; i <= info->h; i += (info->h * 0.05))
	{
		Projectile star = Projectile(inst, textures::redStar, renderer, info, 0, i);
		star.x = -star.size.w / 2.0;
		star.acceleration = Vector2(0, 0);
		border.emplace_back(star);

		Projectile bstar = Projectile(inst, textures::redStar, renderer, info, 0, i);
		bstar.x = info->w - bstar.size.w / 2.0;
		bstar.acceleration = Vector2(0, 0);
		border.emplace_back(bstar);
	}

}

void Boss::render(SDL_Renderer* renderer)
{
	updateBox();
	//make textrenderer follow boss around
	healthCounter.x = (x + size.w/2.0) - healthCounter.size.w / 2.0;
	healthCounter.y = y - healthCounter.size.h;
	healthCounter.text = std::to_string(health);
	healthCounter.renderText();

	//turns boss right (if player is to the right of boss)
	if (player->x + player->size.w / 2.0 > x + size.w / 2.0)
		isFlipped = true;
	else
		isFlipped = false;
	static const float beam_pulse_rate = 2.f;
	//beam shit
	if (state == boss::state::ThrowingBeam || state == boss::state::Final || state == boss::state::ThrowingBeam2)
	{
		double tickduration = 0.75;
		int beamDmg = 50;
		if (state == boss::state::ThrowingBeam)
		{
			beams.render();
			if (!global::hitByUlt)
			{
				SDL_SetTextureColorMod(textures::purpleStar, beam1_col_mult.x, beam1_col_mult.y, beam1_col_mult.z);
				beams.move(beam1_pos.x, beam1_pos.y);
				float rot_amount = 0.2 * info->deltaTime * beam1_rot_speed_mult;
				beams.rotate(0, 0, rot_amount);
				if (beam1_pulse_timer.elapsed() > beam_pulse_rate)
				{
					beam1_pulse_timer.reset();
					Pulse(beams.center.x - 25, beams.center.y - 25, 5, 10, textures::purpleStar);
				}
				for (Projectile const& p : beams.projectiles)
				{
					if (SDL_HasIntersection(&player->hitbox, &p.hitbox) && beamDamageTimer.elapsed() > tickduration)
					{
						beam1_col_mult = { 0, 0, 0 };
						beamDamageTimer.reset();
						inst->flashColor({ 128, 43, 226 });
						beam1_rot_speed_mult = 6.f;
						player->health -= beamDmg;
					}
				}
			}
		}
		else
		{
			beams.render();
			beams2.render();
			if (!global::hitByUlt)
			{
				SDL_SetTextureColorMod(textures::purpleStar, beam1_col_mult.x, beam1_col_mult.y, beam1_col_mult.z);
				SDL_SetTextureColorMod(textures::greenStar, beam2_col_mult.x, beam2_col_mult.y, beam2_col_mult.z);
				beams.move(beam1_pos.x, beam1_pos.y);
				float rot1_amount = 0.2 * info->deltaTime * beam1_rot_speed_mult;
				beams.rotate(0, 0, rot1_amount);
				if (beam1_pulse_timer.elapsed() > beam_pulse_rate)
				{
					beam1_pulse_timer.reset();
					Pulse(beams.center.x - 25, beams.center.y - 25, 5, 10, textures::purpleStar);
				}
				for (Projectile const& p : beams.projectiles)
				{
					if (SDL_HasIntersection(&player->hitbox, &p.hitbox) && beamDamageTimer.elapsed() > tickduration)
					{
						beam1_col_mult = { 0, 0, 0 };
						beamDamageTimer.reset();
						inst->flashColor({ 128, 43, 226 });
						beam1_rot_speed_mult = 6.f;
						player->health -= beamDmg;
					}
				}

				beams2.move(beam2_pos.x, beam2_pos.y);
				float rot2_amount = 0.2 * info->deltaTime * beam2_rot_speed_mult;
				beams2.rotate(rot2_amount * 0.8, rot2_amount * 0.64, rot2_amount);
				// this is kinda unfair ngl
				/*if (beam2_pulse_timer.elapsed() > beam_pulse_rate)
				{
					beam2_pulse_timer.reset();
					Pulse(beams2.center.x - 25, beams2.center.y - 25, 10, -15, textures::greenStar);
				}*/
				for (Projectile const& p : beams2.projectiles)
				{
					if (SDL_HasIntersection(&player->hitbox, &p.hitbox) && beam2DamageTimer.elapsed() > tickduration)
					{
						beam2_col_mult = { 0, 0, 0 };
						beam2DamageTimer.reset();
						beam2_pos.x = rand() % (info->w - size.w * 2) + size.w;
						beam2_pos.y = rand() % (info->h - size.h * 2) + size.h;
						inst->flashColor({ 144, 238, 144 });
						beam2_rot_speed_mult = 6.f;
						player->health -= beamDmg * 1.2;
					}
				}
			}
		}
	}

	//render projectiles & border
	for (Projectile& p : projectiles)
	{
		if (!p.outOfView)
		{
			p.render();
			if (!global::hitByUlt)
			{
				p.rotation += 200 * info->deltaTime;
				p.update();
				if (constants::renderHitboxes)
					SDL_RenderDrawRect(renderer, &p.hitbox);
			}
		}
	}

	for (Projectile& p : border)
	{
		p.render();
		if (!global::hitByUlt)
		{
			p.rotation -= 90 * info->deltaTime;
			p.update();
			if (constants::renderHitboxes)
				SDL_RenderDrawRect(renderer, &p.hitbox);
		}
	}
	
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopyEx(renderer, texture, NULL, &dest, 0, NULL, (isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));

	if (constants::renderHitboxes)
		SDL_RenderDrawRect(renderer, &hitbox);
}

void Boss::updateBox()
{
	hitbox.x = x + size.w * 0.2;
	hitbox.w = size.w * 0.8;
	if (isFlipped)
	{
		hitbox.x -= size.w * 0.2;
	}
	hitbox.y = y + size.h * 0.2;
	hitbox.h = size.h * 0.8;
}