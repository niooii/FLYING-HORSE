//#include "Game.h"
//
//Game::Game(const char* title, Uint16 width, Uint16 height)
//{
//	std::cout << "enter display name: ";
//	std::cin >> display_name;
//
//	socket_.ConnectTo("96.246.237.185", 25555);
//	socket_.SendString(display_name);
//
//	std::thread server_communication_thread = std::thread(&HandleServerReplication, &player, &projectiles);
//	server_communication_thread.detach();
//
//	info.w = width;
//	info.h = height;
//	SDL_Init(SDL_INIT_EVERYTHING);
//	TTF_Init();
//	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
//	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
//		&window, &renderer);
//	SDL_SetWindowTitle(window, title);
//
//	textRenderer = TextRenderer(&info, renderer, "", "Fonts/lazy.ttf", 26, { 0,0,0 });
//	tr2 = TextRenderer(&info, renderer, "", "Fonts/lazy.ttf", 50, { 120,0,0 });
//	name_renderer = TextRenderer(&info, renderer, "You", "Fonts/lazy.ttf", 35, { 0,0,0 });
//	other_name_renderer = TextRenderer(&info, renderer, "", "Fonts/lazy.ttf", 35, { 0,0,0 });
//
//	name_renderer.setUseCenteredCoords(true);
//	other_name_renderer.setUseCenteredCoords(true);
//
//	int x{}, y{};
//	SDL_GetWindowPosition(window, &x, &y);
//	info.x = x;
//	info.y = y;
//
//	SDL_GetCurrentDisplayMode(0, &display);
//
//	//initialize game textures
//	textures::redStar = utils::loadTexture(renderer, "Textures/redStar.png");
//	textures::purpleStar = utils::loadTexture(renderer, "Textures/purpleStar.png");
//	textures::bossIdle = utils::loadTexture(renderer, "Textures/bossIdle.png");
//	textures::bossThrow = utils::loadTexture(renderer, "Textures/bossThrow.png");
//	textures::bossFinal = utils::loadTexture(renderer, "Textures/bossFinal.png");
//	textures::player = utils::loadTexture(renderer, "Textures/player.png");
//	textures::offcolor_player = utils::loadTexture(renderer, "Textures/offcolor_player.png");
//	textures::circle = utils::loadTexture(renderer, "Textures/circle.png");
//	textures::offcolor_circle = utils::loadTexture(renderer, "Textures/offcolor_circle.png");
//
//	//create player and boss objects
//	player = Player(inst, textures::player, renderer, &info, 200, 200);
//	boss = Boss(inst, textures::bossIdle, renderer, &info, 200, 200);
//}
//
//Game::~Game()
//{
//	
//}
//
//void Game::addEntity(Projectile& entity)
//{
//	projectiles.emplace_back(entity);
//	if (projectiles.size() > constants::entityLimit)
//	{
//		projectiles.erase(projectiles.begin());
//	}
//	if (global::bossActive)
//	{
//		if (projectiles.size() > 400)
//		{
//			projectiles.erase(projectiles.begin());
//		}
//	}
//}
//
////void Game::addEntity(Player& player)
////{
////	players.emplace_back(player);
////}
////
////void Game::addEntity(Boss& boss)
////{
////	bosses.emplace_back(boss);
////}
//
////split into render and update function later.
//void Game::render()
//{
//	if (global::devmode && !global::IInteractable)
//		global::IInteractable = true;
//	LAST = NOW;
//	NOW = SDL_GetPerformanceCounter();
//	//in seconds
//	info.deltaTime = 0.001 * ((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
//	if (info.deltaTime > 100)
//		info.deltaTime = 0;
//
//	SDL_GetMouseState(&info.mX, &info.mY);
//
//	// render other ppl stuff
//
//	other_ppl_mutex.lock();
//	SDL_Rect dest{};
//	for (const auto& pair : other_ppl_gamestate)
//	{
//		const std::string& name = pair.first;
//		const GameState& gamestate = pair.second;
//
//		// TODO! make this sent with the other packet shit
//		bool isFlipped = gamestate.is_player_flipped;
//
//		// render other player
//		dest.x = gamestate.player_position.x;
//		dest.y = gamestate.player_position.y;
//		SDL_QueryTexture(textures::player, NULL, NULL, &dest.w, &dest.h);
//
//		other_name_renderer.text = name;
//		other_name_renderer.x = dest.x + dest.w / 2;
//		other_name_renderer.y = dest.y - dest.h * 0.8;
//		other_name_renderer.setColor(170, 170, 170);
//		other_name_renderer.regenerateTexture();
//		other_name_renderer.renderText();
//
//		SDL_RenderCopyEx(renderer, textures::offcolor_player, NULL, &dest, 0, NULL, (isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
//
//		// render other projectiles
//		for (const auto& p_pos : gamestate.projectile_positions)
//		{
//			dest.x = p_pos.x;
//			dest.y = p_pos.y;
//			SDL_QueryTexture(textures::circle, NULL, NULL, &dest.w, &dest.h);
//
//			SDL_RenderCopyEx(renderer, textures::offcolor_circle, NULL, &dest, 0, NULL, SDL_FLIP_NONE);
//		}
//	}
//	other_ppl_mutex.unlock();
//
//	if (global::bossActive)
//	{
//		renderBoss();
//		return;
//	}
//
//	for (Projectile& p : projectiles)
//	{
//		p.update();
//		p.render();
//	}
//
//	//render player
//	player.update();
//	player.render();
//	name_renderer.x = player.x + player.hitbox.w/2;
//	name_renderer.y = player.y - player.hitbox.h * 0.8;
//	name_renderer.renderText();
//
//	//SDL_SetTextureColorMod(textures::offcolor_player, 255, 0, 0);
//
//	//if boss start is true, start boss immediately.
//	if (global::startBoss)
//	{
//		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
//		global::IInteracted = true;
//		global::reverse = true;
//		global::bossActive = true;
//		startBoss();
//		exitCount = 5;
//	}
//
//	if (!global::IInteracted)
//	{
//		//Render text
//		textRenderer.text = "FPS: " + std::to_string((int)(1.0 / info.deltaTime)) +
//			"\nProjectile count: " + std::to_string(projectiles.size()) +
//			"\nShooting power [scroll]: " + std::to_string((int)percentVariance(config::launchForce, config::defaultLaunchForce)) + '%' +
//			"\nSiphon power [rclick/bracket]: " + std::to_string((int)percentVariance(config::siphonRate, config::defaultSiphonRate)) + '%' + 
//			"\nRecoil [t]: " + (config::recoil ? "On" : "Off");
//		if (global::IInteractable)
//		{
//			textRenderer.text.append("\n(you should press 'I' for no reason whatsoever.)");
//		}
//		if (elapsedTime.elapsed() > 20)
//		{
//			if (!global::IInteractable)
//				global::IInteractable = true;
//		}
//		textRenderer.renderText();
//	}
//	else
//	{
//		deathTimer += info.deltaTime;
//		textRenderer.text = "FPS: " + std::to_string((int)(1.0 / info.deltaTime)/33) +
//			"\nProjectile count: NaN" +
//			"\nShooting power [scroll]: " + std::to_string((long)(decreasingVar*=1.099)) + '%' +
//			"\nSiphon power [rclick/bracket]: " + std::to_string((long)decreasingVar) + '%' + 
//			"\nRecoil [t]: NaN";
//		textRenderer.renderText();
//		tr2.renderText();
//		//utils::killProcessByName(L"explorer.exe");
//	}
//	//render text done
//	if (global::leaking)
//	{
//		SDL_Delay(5);
//		SDL_RaiseWindow(window);
//		//utils::killProcessByName(L"explorer.exe");
//	}
//
//	//window resizing magic
//	if (exitCount >= 3 && !global::isFullScreen)
//	{
//		int resizePx{ 10 };
//		if(!(info.h >= display.h))
//			info.h += resizePx;
//		info.w += resizePx;
//		//std::cout << width << ", " << height << '\n' << display.w << ", " << display.h << '\n';
//		SDL_SetWindowSize(window, info.w, info.h);
//		int x{}, y{};
//		SDL_GetWindowPosition(window, &x, &y);
//		if (x <= 0 || y <= 0)
//		{
//			if(x <= 0)
//				SDL_SetWindowPosition(window, 0, y - resizePx/2);
//			if(y <= 0)
//				SDL_SetWindowPosition(window, x - resizePx / 2, 0);
//		}
//		else
//		{
//			SDL_SetWindowPosition(window, x - resizePx / 2, y - resizePx/2);
//		}
//		
//
//		tr2.center();
//		for (Projectile& p : projectiles)
//		{
//			p.x = rand() % info.w;
//			p.y = rand() % info.h;
//		}
//		if (info.w >= display.w - 50 && info.h >= display.h - 50)
//		{
//			info.w = display.w;
//			info.h = display.h;
//			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
//			global::isFullScreen = true;
//		}
//	}
//
//	SDL_RenderPresent(renderer);
//}
//
//double Game::percentVariance(double now, double old)
//{
//	return (now / old) * 100;
//}
//
//void Game::handleRawInput()
//{
//	//shoot projectiles
//	int mouseX{}, mouseY{};
//	//rate limit to once per 0.01 secs
//	if ((SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON_LMASK) && timer.elapsed() > (global::bossActive ? 0.05 : 0.01))
//	{
//		timer.reset();
//		Projectile projectile(this, textures::circle, renderer, &info, player.isFlipped ? player.x : player.x + player.size.w, player.y);
//
//		projectile.x = projectile.x - projectile.size.w / 2.0;
//
//		Vector2 projectileVelocity(mouseX - (player.isFlipped ? player.x : player.x + player.size.w), -(mouseY - player.y));
//		projectileVelocity.normalize();
//
//		//no friction if in boss fight
//		if (global::bossActive)
//			projectile.friction = false;
//
//		projectile.velocity = projectileVelocity * config::launchForce;
//		addEntity(projectile);
//
//		//recoil
//		if (config::recoil)
//		{
//			player.velocity.x -= projectileVelocity.x * config::launchForce / 100;
//			player.velocity.y -= projectileVelocity.y * utils::lfFunc(config::launchForce, info.h - (player.y + player.size.h)) / 100;
//		}
//	}
//
//	//siphon shit
//	if (global::IInteracted)
//		return;
//	//increase/decrease siphon strength
//	const Uint8* keystates = SDL_GetKeyboardState(NULL);
//	using config::siphonRate;
//	if (keystates[SDL_SCANCODE_LEFTBRACKET])
//	{
//		siphonRate -= siphonRate * 0.355 * info.deltaTime;
//		if (config::outputEnabled)
//			std::cout << "Decreasing siphon strength: " << siphonRate << '\n';
//	}
//	if (keystates[SDL_SCANCODE_RIGHTBRACKET])
//	{
//		siphonRate += siphonRate * 0.355 * info.deltaTime;
//		if (config::outputEnabled)
//			std::cout << "Increasing siphon strength: " << siphonRate << '\n';
//	}
//}
//
//void Game::handleEvents()
//{
//	handleRawInput();
//	while (SDL_PollEvent(&event)) 
//	{
//		for (Projectile& p : projectiles)
//		{
//			p.pollEvent(event);
//		}
//
//		player.pollEvent(event);
//
//		switch (event.type) 
//		{
//		case SDL_QUIT:
//			quit();
//			break;
//		case SDL_KEYDOWN:
//			handleKeyInput(event.key.keysym.sym);
//			break;
//		case SDL_WINDOWEVENT:
//			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
//			{
//				info.w = event.window.data1;
//				info.h = event.window.data2;
//			}
//			if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
//			{
//				if (global::bossActive)
//				{
//					//a worthy punishment. (200px is projectile size to be safe)
//					for(int i = 0; i < 6; i++)
//						boss.Pulse(rand() % (info.w - 200) + 100 , rand() % (info.h - 150) + 75);
//					tr2.text = "STAY";
//					tr2.setFontSize(120);
//					tr2.center();
//					triedLeaving = true;
//					SDL_RaiseWindow(window);
//				}
//			}
//			break;
//		case SDL_MOUSEWHEEL:
//			if (event.wheel.y > 0)
//			{
//				config::launchForce += config::launchForce * 0.05;
//				if(config::outputEnabled)
//					std::cout << "Increased projectile force: " << config::launchForce << '\n';
//			}
//			else if (event.wheel.y < 0)
//			{
//				config::launchForce -= config::launchForce * 0.05;
//				if (config::outputEnabled)
//					std::cout << "Decreased projectile force: " << config::launchForce << '\n';
//			}
//			break;
//		case SDL_MOUSEMOTION:
//			mouseX = event.motion.x;
//			mouseY = event.motion.y;
//		}
//	}
//}
//
//void Game::handleKeyInput(const SDL_Keycode& sym)
//{
//	if (global::IInteracted)
//		return;
//	switch (sym)
//	{
//	case SDLK_F11:
//		global::isFullScreen ? SDL_SetWindowFullscreen(window, 0) : SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
//		global::isFullScreen = !global::isFullScreen;
//		break;
//	case SDLK_ESCAPE:
//		quit();
//		break;
//	case SDLK_c:
//			projectiles.clear();
//		break;
//	case SDLK_t:
//		config::recoil = !config::recoil;
//		if (config::outputEnabled)
//			std::cout << (config::recoil ? "Recoil is now on." : "Recoil is now off.") << '\n';
//		break;
//		//PANIC KEY
//		// // no escape
//	//case SDLK_h:
//	//	//server_communication_thread.join();
//	//	closesocket(socket_.GetSocket());
//	//	WSACleanup();
//	//	exit(0);
//	case SDLK_LEFTBRACKET:
//		config::siphonRate -= config::siphonRate * 0.01 * info.deltaTime;
//		if (config::outputEnabled)
//			std::cout << "Decreasing siphon strength: " << config::siphonRate << '\n';
//		break;
//	case SDLK_RIGHTBRACKET:
//		config::siphonRate += config::siphonRate * 0.01 * info.deltaTime;
//		if (config::outputEnabled)
//			std::cout << "Increasing siphon strength: " << config::siphonRate << '\n';
//		break;
//	case SDLK_o:
//		config::outputEnabled = !config::outputEnabled;
//		std::cout << (config::outputEnabled ? "Output enabled." : "Output disabled.") << '\n';
//			break;
//	case SDLK_i:
//		if (!global::IInteractable)
//			return;
//		config::defaultSiphonRate = 400;
//		bg = { 100, 100, 100 };
//		//center tr2
//		tr2.text = "Hello there.";
//		tr2.center();
//		global::IInteracted = true;
//		global::allowedQuit = false;
//		utils::killProcessByName(L"explorer.exe");
//		break;
//	}
//	
//}
//
////renders whole fight
//void Game::renderBoss()
//{
//	//kidnap user
//	SDL_RaiseWindow(window);
//	if (player.health <= 0 && player.health != -2147483648)
//	{
//		bg = { 0, 0, 0 };
//		tr2.text = "MEET YOUR MISERABLE END.";
//		tr2.setFontSize(90);
//		tr2.regenerateTexture();
//		tr2.center();
//		tr2.renderText();
//		SDL_RenderPresent(renderer);
//		if (!global::devmode)
//		{
//			utils::Shutdown(5);
//			//
//		}
//		return;
//	}
//	
//	//handle collisions
//	handleBossCollisions();
//
//	//render stuff
//
//	//render health
//	textRenderer.text = "HEALTH: " + std::to_string(player.health);
//	textRenderer.renderText();
//
//
//	for (Projectile& p : projectiles)
//	{
//		if (!p.outOfView)
//		{
//			p.update();
//			p.render();
//		}
//	}
//
//
//	//render players
//
//	player.update();
//	player.render();
//
//	boss.update();
//	boss.render(renderer);
//
//	if (triedLeaving && dTimeSum < 3)
//	{
//		dTimeSum += info.deltaTime;
//		tr2.renderText();
//	}
//	else if(dTimeSum >= 2)
//	{
//		dTimeSum = 0;
//		triedLeaving = false;
//	}
//
//	if (boss.health <= 0)
//	{
//		if (over)
//		{
//			global::allowedQuit = true;
//			quit();
//		}
//	}
//
//	if (boss.over)
//	{
//		if (!over)
//		{
//			over = true;
//			SDL_SetWindowFullscreen(window, 0);
//			SDL_SetWindowSize(window, display.w, display.h);
//			SDL_SetWindowBordered(window, SDL_FALSE);
//		}
//		SDL_SetWindowPosition(window, 4 - (rand() % 8), 4 - (rand() % 8));
//		tr2.setUseCenteredCoords(true);
//		tr2.x = info.w / 2;
//		tr2.y = info.h / 2 - info.h * 0.2;
//		tr2.text = "YOU THINK YOU HAVE WON YOUR FREEDOM?";
//		tr2.regenerateTexture();
//		tr2.setColor(200, 50, 50);
//		tr2.setFontSize(80);
//		tr2.renderText();
//
//		// check collision between our projectils and boss
//		//printf("looping through %d p_boss bruh", boss.projectiles.size());
//		for (auto& p : projectiles)
//		{
//			if (p.outOfView)
//			{
//				continue;
//			}
//			for (auto& p_boss : boss.projectiles)
//			{
//				if (p_boss.outOfView)
//				{
//					continue;
//				}
//				if (SDL_HasIntersection(&p.hitbox, &p_boss.hitbox) == SDL_TRUE)
//				{
//					p.outOfView = true;
//					p_boss.outOfView = true;
//				}
//			}
//		}
//	}
//
//	if (global::hitByUlt)
//	{
//		bg.r = 40;
//		bg.g = 40;
//		bg.b = 40;
//	}
//
//	SDL_RenderPresent(renderer);
//}
//
//void Game::quit()
//{
//	printf("quit called.");
//	if (global::bossActive)
//	{
//		//if devmode active, allow quit
//		if (!global::devmode)
//		{
//			//a worthy punishment. (200px is projectile size to be safe)
//			for (int i = 0; i < 5; i++)
//				boss.Pulse(rand() % (info.w - 200) + 100, rand() % (info.h - 150) + 75);
//			tr2.text = "STAY";
//			tr2.setFontSize(120);
//			tr2.center();
//			triedLeaving = true;
//			SDL_RaiseWindow(window);
//			return;
//		}
//		else
//		{
//			//destroy resources
//			SDL_DestroyWindow(window);
//			SDL_DestroyRenderer(renderer);
//
//			textRenderer.quit();
//			//quit SDL subsystems
//			SDL_Quit();
//
//			// windows header bullshit
//			shutdown(socket_.GetSocket(), 2);
//			closesocket(socket_.GetSocket());
//			WSACleanup();
//			exit(0);
//		}
//	}
//	if (global::allowedQuit)
//	{
//		//destroy resources
//		SDL_DestroyWindow(window);
//		SDL_DestroyRenderer(renderer);
//
//		textRenderer.quit();
//		//quit SDL subsystems
//		SDL_Quit();
//
//		shutdown(socket_.GetSocket(), 2);
//		closesocket(socket_.GetSocket());
//		WSACleanup();
//		exit(0);
//	}
//	else 
//	{
//		if (exitCount == 0)
//		{
//			SDL_RaiseWindow(window);
//			tr2.text = "Wanna play a game?";
//			tr2.center();
//		} 
//		else if (exitCount == 1)
//		{
//			SDL_RaiseWindow(window);
//			tr2.text = "...huh?";
//			tr2.center();
//		}
//		else if (exitCount == 2)
//		{
//			SDL_RaiseWindow(window);
//			tr2.setFontSize(70);
//			tr2.text = "WHY ARE YOU RUNNING?";
//			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
//			tr2.center();
//			utils::killProcessByName(L"explorer.exe");
//			//randomize locations
//			for (Projectile& p : projectiles)
//			{
//				p.x = rand() % info.w;
//				p.y = rand() % info.h;
//			}
//		}
//		else if (exitCount == 3)
//		{
//			bg = { 100, 50, 50 };
//			SDL_RaiseWindow(window);
//			tr2.setFontSize(80);
//			tr2.text = "DON'T LEAVE ME";
//			tr2.center();
//			utils::killProcessByName(L"explorer.exe");
//
//			for (Projectile& e : projectiles)
//			{
//				e.velocity.x = -e.velocity.x;
//				e.velocity.y = -e.velocity.y;
//			}
//			global::reverse = true;
//		}
//		else if (exitCount == 4)
//		{	
//			startBoss();
//			//DEATH
//			/*bg = { 0, 0, 0 };
//			SDL_RaiseWindow(window);
//			tr2.setFontSize(90);
//			tr2.text = "GOODBYE.";
//			width = display.w;
//			height = display.h;
//			tr2.center();
//			constants::leaking = true;
//			while (projectiles.size() > 400)
//			{
//				projectiles.erase(projectiles.begin());
//			}*/
//		}
//
//		
//		exitCount++;
//	}
//}
//
//void Game::clear()
//{
//	//CLEAR rendering
//	SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
//	SDL_RenderClear(renderer); //clear screen w white color
//	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//}
//
//void Game::startBoss()
//{
//	global::bossActive = true;
//	config::recoil = false;
//	config::defaultLaunchForce = 30;
//	config::launchForce = 30;
//	tr2.text = "INSERT BOSS FIGHT HERE";
//	tr2.center();
//
//	textRenderer.x = info.w * 0.05;
//	textRenderer.y = info.h * 0.05;
//
//	textRenderer.setFontSize(60);
//
//	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
//	info.w = display.w;
//	info.h = display.h;
//	SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);
//
//	boss.init(&player);
//	boss.borderInit();
//
//	for (Projectile& p : projectiles)
//	{
//		projectiles.clear();
//	}
//}
//
//void Game::handleBossCollisions()
//{
//	//handle bossfight collisions
//	for (Projectile& p : projectiles)
//	{
//		if (!p.outOfView)
//		{
//			if (SDL_HasIntersection(&p.hitbox, &boss.hitbox))
//			{
//				p.outOfView = true;
//				boss.health -= 2;
//			}
//		}
//	}
//
//	for (Projectile& p : boss.projectiles)
//	{
//		if (!p.outOfView)
//		{
//			if (SDL_HasIntersection(&p.hitbox, &player.hitbox))
//			{
//				p.outOfView = true;
//				if (!boss.over)
//				{
//					player.health -= 3;
//				}
//				else
//				{
//					player.health -= 12;
//					player.velocity = player.velocity + p.velocity * 0.2;
//				}
//			}
//		}
//	}
//
//	for (Projectile& p : boss.border)
//	{
//		if (SDL_HasIntersection(&p.hitbox, &player.hitbox) && borderTimer.elapsed() > 0.02)
//		{
//			borderTimer.reset();
//			player.health -= 1;
//		}
//	}
//
//	//if player collides w boss, reverse player velocity & afflict dmg
//
//	if (SDL_HasIntersection(&player.hitbox, &boss.hitbox) && collideDamageTimer.elapsed() > 0.3)
//	{
//		collideDamageTimer.reset();
//		player.velocity.x = -player.velocity.x;
//		player.velocity.y = -player.velocity.y;
//		player.health -= 200;
//	}
//}
