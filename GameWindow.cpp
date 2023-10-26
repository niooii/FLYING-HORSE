#include "GameWindow.h"
#include <wchar.h>
#include <SDL.h>


GameWindow::GameWindow(const char* title, Uint16 width, Uint16 height)
{
	info.w = width;
	info.h = height;
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
		&window, &renderer);
	SDL_SetWindowTitle(window, title);

	textRenderer = TextRenderer(&info, renderer, "", "Fonts/lazy.ttf", 26, { 0,0,0 });
	tr2 = TextRenderer(&info, renderer, "", "Fonts/lazy.ttf", 50, { 120,0,0 });

	int x{}, y{};
	SDL_GetWindowPosition(window, &x, &y);
	info.x = x;
	info.y = y;

	SDL_GetCurrentDisplayMode(0, &display);

	//initialize game textures
	textures::redStar = utils::loadTexture(renderer, "Textures/redStar.png");
	textures::purpleStar = utils::loadTexture(renderer, "Textures/purpleStar.png");
	textures::bossIdle = utils::loadTexture(renderer, "Textures/bossIdle.png");
	textures::bossThrow = utils::loadTexture(renderer, "Textures/bossThrow.png");
	textures::player = utils::loadTexture(renderer, "Textures/player.png");
	textures::circle = utils::loadTexture(renderer, "Textures/circle.png");

	//create player and boss objects
	player = Player(textures::player, renderer, &info, 200, 200);
	boss = Boss(textures::bossIdle, renderer, &info, 200, 200);
}

void GameWindow::addEntity(Projectile& entity)
{
	projectiles.emplace_back(entity);
	if (projectiles.size() > constants::entityLimit)
	{
		projectiles.erase(projectiles.begin());
	}
	if (global::bossActive)
	{
		if (projectiles.size() > 400)
		{
			projectiles.erase(projectiles.begin());
		}
	}
}

//void GameWindow::addEntity(Player& player)
//{
//	players.emplace_back(player);
//}
//
//void GameWindow::addEntity(Boss& boss)
//{
//	bosses.emplace_back(boss);
//}

//split into render and update function later.
void GameWindow::render()
{
	if (global::devmode && !global::IInteractable)
		global::IInteractable = true;
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();
	//in seconds
	info.deltaTime = 0.001 * ((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
	if (info.deltaTime > 100)
		info.deltaTime = 0;

	SDL_GetMouseState(&info.mX, &info.mY);

	if (global::bossActive)
	{
		renderBoss();
		return;
	}

	for (Projectile& p : projectiles)
	{
		p.update();
		p.render();
	}

	//render player
	player.update();
	player.render();

	//if boss start is true, start boss immediately.
	if (global::startBoss)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		global::IInteracted = true;
		global::reverse = true;
		global::bossActive = true;
		startBoss();
		exitCount = 5;
	}

	if (!global::IInteracted)
	{
		//Render text
		textRenderer.text = "FPS: " + std::to_string((int)(1.0 / info.deltaTime)) +
			"\nProjectile count: " + std::to_string(projectiles.size()) +
			"\nShooting power: " + std::to_string((int)percentVariance(config::launchForce, config::defaultLaunchForce)) + '%' +
			"\nSiphon power: " + std::to_string((int)percentVariance(config::siphonRate, config::defaultSiphonRate)) + '%' + 
			"\nRecoil: " + (config::recoil ? "On" : "Off");
		if (elapsedTime.elapsed() > 20)
		{
			textRenderer.text.append("\n[Hi! You should press 'I' for no reason whatsoever.]");
			if (!global::IInteractable)
				global::IInteractable = true;
		}
		textRenderer.renderText();
	}
	else
	{
		deathTimer += info.deltaTime;
		textRenderer.text = "FPS: " + std::to_string((int)(1.0 / info.deltaTime)/33) +
			"\nProjectile count: NaN" +
			"\nShooting power: " + std::to_string((long)(decreasingVar*=1.099)) + '%' +
			"\nSiphon power: " + std::to_string((long)decreasingVar) + '%' + 
			"\nRecoil: NaN";
		textRenderer.renderText();
		tr2.renderText();
		//utils::killProcessByName(L"explorer.exe");
	}
	//render text done
	if (global::leaking)
	{
		SDL_Delay(5);
		SDL_RaiseWindow(window);
		//utils::killProcessByName(L"explorer.exe");
	}

	//window resizing magic
	if (exitCount >= 3 && !global::isFullScreen)
	{
		int resizePx{ 10 };
		if(!(info.h >= display.h))
			info.h += resizePx;
		info.w += resizePx;
		//std::cout << width << ", " << height << '\n' << display.w << ", " << display.h << '\n';
		SDL_SetWindowSize(window, info.w, info.h);
		int x{}, y{};
		SDL_GetWindowPosition(window, &x, &y);
		if (x <= 0 || y <= 0)
		{
			if(x <= 0)
				SDL_SetWindowPosition(window, 0, y - resizePx/2);
			if(y <= 0)
				SDL_SetWindowPosition(window, x - resizePx / 2, 0);
		}
		else
		{
			SDL_SetWindowPosition(window, x - resizePx / 2, y - resizePx/2);
		}
		

		tr2.center();
		for (Projectile& p : projectiles)
		{
			p.x = rand() % info.w;
			p.y = rand() % info.h;
		}
		if (info.w >= display.w - 50 && info.h >= display.h - 50)
		{
			info.w = display.w;
			info.h = display.h;
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			global::isFullScreen = true;
		}
	}

	SDL_RenderPresent(renderer);
}

double GameWindow::percentVariance(double now, double old)
{
	return (now / old) * 100;
}

void GameWindow::handleRawInput()
{
	//shoot projectiles
	int mouseX{}, mouseY{};
	//rate limit to once per 0.01 secs
	if ((SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON_LMASK) && timer.elapsed() > (global::bossActive ? 0.05 : 0.01))
	{
		timer.reset();
		Projectile projectile(textures::circle, renderer, &info, player.isFlipped ? player.x : player.x + player.size.w, player.y);

		projectile.x = projectile.x - projectile.size.w / 2.0;

		Vector2 projectileVelocity(mouseX - (player.isFlipped ? player.x : player.x + player.size.w), -(mouseY - player.y));
		projectileVelocity.normalize();

		//no friction if in boss fight
		if (global::bossActive)
			projectile.friction = false;

		projectile.velocity = projectileVelocity * config::launchForce;
		addEntity(projectile);

		//recoil
		if (config::recoil)
		{
			player.velocity.x -= projectileVelocity.x * config::launchForce / 100;
			player.velocity.y -= projectileVelocity.y * utils::lfFunc(config::launchForce, info.h - (player.y + player.size.h)) / 100;
		}
	}

	//siphon shit
	if (global::IInteracted)
		return;
	//increase/decrease siphon strength
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	using config::siphonRate;
	if (keystates[SDL_SCANCODE_LEFTBRACKET])
	{
		siphonRate -= siphonRate * 0.355 * info.deltaTime;
		if (config::outputEnabled)
			std::cout << "Decreasing siphon strength: " << siphonRate << '\n';
	}
	if (keystates[SDL_SCANCODE_RIGHTBRACKET])
	{
		siphonRate += siphonRate * 0.355 * info.deltaTime;
		if (config::outputEnabled)
			std::cout << "Increasing siphon strength: " << siphonRate << '\n';
	}
}

void GameWindow::handleEvents()
{
	handleRawInput();
	while (SDL_PollEvent(&event)) 
	{
		for (Projectile& p : projectiles)
		{
			p.pollEvent(event);
		}

		player.pollEvent(event);

		switch (event.type) 
		{
		case SDL_QUIT:
			quit();
			break;
		case SDL_KEYDOWN:
			handleKeyInput(event.key.keysym.sym);
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				info.w = event.window.data1;
				info.h = event.window.data2;
			}
			if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
			{
				if (global::bossActive)
				{
					//a worthy punishment. (200px is projectile size to be safe)
					for(int i = 0; i < 5; i++)
						boss.Pulse(rand() % (info.w - 200) + 100 , rand() % (info.h - 150) + 75);
					tr2.text = "STAY";
					tr2.setFontSize(120);
					tr2.center();
					triedLeaving = true;
					SDL_RaiseWindow(window);
				}
			}
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0)
			{
				config::launchForce += config::launchForce * 0.05;
				if(config::outputEnabled)
					std::cout << "Increased projectile force: " << config::launchForce << '\n';
			}
			else if (event.wheel.y < 0)
			{
				config::launchForce -= config::launchForce * 0.05;
				if (config::outputEnabled)
					std::cout << "Decreased projectile force: " << config::launchForce << '\n';
			}
			break;
		case SDL_MOUSEMOTION:
			mouseX = event.motion.x;
			mouseY = event.motion.y;
		}
	}
}

void GameWindow::handleKeyInput(const SDL_Keycode& sym)
{
	if (global::IInteracted)
		return;
	switch (sym)
	{
	case SDLK_F11:
		global::isFullScreen ? SDL_SetWindowFullscreen(window, 0) : SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		global::isFullScreen = !global::isFullScreen;
		break;
	case SDLK_ESCAPE:
		quit();
		break;
	case SDLK_c:
			projectiles.clear();
		break;
	case SDLK_t:
		config::recoil = !config::recoil;
		if (config::outputEnabled)
			std::cout << (config::recoil ? "Recoil is now on." : "Recoil is now off.") << '\n';
		break;
		//PANIC KEY
	case SDLK_h:
		exit(0);
	case SDLK_LEFTBRACKET:
		config::siphonRate -= config::siphonRate * 0.01 * info.deltaTime;
		if (config::outputEnabled)
			std::cout << "Decreasing siphon strength: " << config::siphonRate << '\n';
		break;
	case SDLK_RIGHTBRACKET:
		config::siphonRate += config::siphonRate * 0.01 * info.deltaTime;
		if (config::outputEnabled)
			std::cout << "Increasing siphon strength: " << config::siphonRate << '\n';
		break;
	case SDLK_o:
		config::outputEnabled = !config::outputEnabled;
		std::cout << (config::outputEnabled ? "Output enabled." : "Output disabled.") << '\n';
			break;
	case SDLK_i:
		if (!global::IInteractable)
			return;
		config::defaultSiphonRate = 400;
		bg = { 100, 100, 100 };
		//center tr2
		tr2.text = "ABABABABBAB";
		tr2.center();
		global::IInteracted = true;
		global::allowedQuit = false;
		//utils::killProcessByName(L"explorer.exe");
		break;
	}
	
}

//renders whole fight
void GameWindow::renderBoss()
{
	//kidnap user
	SDL_RaiseWindow(window);
	if (player.health <= 0)
	{
		bg = { 0, 0, 0 };
		tr2.text = "EHEHEHEHEHE";
		tr2.setFontSize(110);
		tr2.renderText();
		SDL_RenderPresent(renderer);
		if (!global::devmode)
		{
			exit(0);
			//system("C:\\windows\\system32\\shutdown /r /t 3\n\n");
			//
		}
		return;
	}
	
	//handle collisions
	handleBossCollisions();

	//render stuff

	//render health
	textRenderer.text = "HEALTH: " + std::to_string(player.health);
	textRenderer.renderText();


	for (Projectile& p : projectiles)
	{
		if (!p.outOfView)
		{
			p.update();
			p.render();
		}
	}


	//render players

	player.update();
	player.render();

	boss.update();
	boss.render(renderer);

	if (triedLeaving && dTimeSum < 3)
	{
		dTimeSum += info.deltaTime;
		tr2.renderText();
	}
	else if(dTimeSum >= 2)
	{
		dTimeSum = 0;
		triedLeaving = false;
	}

	SDL_RenderPresent(renderer);
}

void GameWindow::quit()
{
	if (global::bossActive)
	{
		//if devmode active, allow quit
		if (!global::devmode)
		{
			//a worthy punishment. (200px is projectile size to be safe)
			for (int i = 0; i < 5; i++)
				boss.Pulse(rand() % (info.w - 200) + 100, rand() % (info.h - 150) + 75);
			tr2.text = "STAY";
			tr2.setFontSize(120);
			tr2.center();
			triedLeaving = true;
			SDL_RaiseWindow(window);
			return;
		}
		else
		{
			//destroy resources
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);

			textRenderer.quit();
			//quit SDL subsystems
			SDL_Quit();
			exit(0);
		}
	}
	if (global::allowedQuit)
	{
		//destroy resources
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);

		textRenderer.quit();
		//quit SDL subsystems
		SDL_Quit();
		exit(0);
	}
	else 
	{
		if (exitCount < 2)
		{
			SDL_RaiseWindow(window);
			tr2.text = "WHY ARE YOU RUNNING?";
			tr2.center();
		} 
		else if (exitCount == 2)
		{
			SDL_RaiseWindow(window);
			tr2.setFontSize(70);
			tr2.text = "DONT LEAVE ME";
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			tr2.center();
			//utils::killProcessByName(L"explorer.exe");
			//randomize locations
			for (Projectile& p : projectiles)
			{
				p.x = rand() % info.w;
				p.y = rand() % info.h;
			}
		}
		else if (exitCount == 3)
		{
			bg = { 150, 100, 100 };
			SDL_RaiseWindow(window);
			tr2.setFontSize(90);
			tr2.text = "...";
			tr2.center();
			//utils::killProcessByName(L"explorer.exe");

			for (Projectile& e : projectiles)
			{
				e.velocity.x = -e.velocity.x;
				e.velocity.y = -e.velocity.y;
			}
			global::reverse = true;
		}
		else if (exitCount == 4)
		{	
			startBoss();
			//DEATH
			/*bg = { 0, 0, 0 };
			SDL_RaiseWindow(window);
			tr2.setFontSize(90);
			tr2.text = "GOODBYE.";
			width = display.w;
			height = display.h;
			tr2.center();
			constants::leaking = true;
			while (projectiles.size() > 400)
			{
				projectiles.erase(projectiles.begin());
			}*/
		}

		
		exitCount++;
	}
}

void GameWindow::clear()
{
	//CLEAR rendering
	SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
	SDL_RenderClear(renderer); //clear screen w white color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void GameWindow::startBoss()
{
	global::bossActive = true;
	config::recoil = false;
	config::defaultLaunchForce = 30;
	config::launchForce = 30;
	tr2.text = "INSERT BOSS FIGHT HERE";
	tr2.center();

	textRenderer.x = info.w * 0.05;
	textRenderer.y = info.h * 0.05;

	textRenderer.setFontSize(60);

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	info.w = display.w;
	info.h = display.h;
	SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);

	boss.init(&player);
	boss.borderInit();

	for (Projectile& p : projectiles)
	{
		projectiles.clear();
	}
}

void GameWindow::handleBossCollisions()
{
	//handle bossfight collisions
	for (Projectile& p : projectiles)
	{
		if (!p.outOfView)
		{
			if (SDL_HasIntersection(&p.hitbox, &boss.hitbox))
			{
				p.outOfView = true;
				boss.health -= 1;
				if (boss.health < 0)
				{
					exit(0);
				}
			}
		}
	}

	for (Projectile& p : boss.projectiles)
	{
		if (!p.outOfView)
		{
			if (SDL_HasIntersection(&p.hitbox, &player.hitbox))
			{
				p.outOfView = true;
				player.health -= 3;
			}
		}
	}

	for (Projectile& p : boss.border)
	{
		if (SDL_HasIntersection(&p.hitbox, &player.hitbox) && borderTimer.elapsed() > 0.02)
		{
			borderTimer.reset();
			player.health -= 1;
		}
	}

	//if player collides w boss, reverse player velocity & afflict dmg

	if (SDL_HasIntersection(&player.hitbox, &boss.hitbox) && collideDamageTimer.elapsed() > 0.3)
	{
		collideDamageTimer.reset();
		player.velocity.x = -player.velocity.x;
		player.velocity.y = -player.velocity.y;
		player.health -= 200;
	}
}
