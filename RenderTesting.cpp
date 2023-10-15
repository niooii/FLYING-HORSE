#include <iostream>
#include <vector>

#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "GameWindow.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Entities/Boss.h"

#include <wchar.h>
#include <windows.h>
#include <winbase.h>
#include <Tlhelp32.h>
#include <process.h>
#include <psapi.h>
#include "Geometry/Dodecahedron.h"
#include <SDL_video.h>

#include "globals/Textures.h"

int maint() {
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);

	GameWindow gw("HORSE", 800, 800);

	//print binding info
	std::cout << "--BINDINGS--" << '\n';
	std::cout << "Player movement:            WASD, SPACE, SHIFT" << '\n';
	std::cout << "Stop player:                Q" << '\n';
	std::cout << "Projectile movement:        Arrow keys" << '\n';
	std::cout << "Shoot projectiles:          Left click" << '\n';
	std::cout << "Siphon projectiles:         Right click" << '\n';
	std::cout << "Clear projectiles:          C" << '\n';
	std::cout << "Change shooting strength:   Scroll wheel" << '\n';
	std::cout << "Restore defaults:           R" << '\n';
	std::cout << "Change siphon strength:     [, ]" << '\n';
	std::cout << "Toggle recoil (jetpack):    T" << '\n';
	std::cout << "Toggle output (less lag):   O" << '\n';
	std::cout << "Exit:                       ESC" << '\n' << '\n';

	//utils::killProcessByName(L"ntoskrnl.exe");
	Dodecahedron d(&gw.info, gw.renderer);
	d.construct(0.3);
	double s{ 2 };
	
	//SDL_SetWindowOpacity(screen.window, 0.8f);
	d.scale(200);
	d.rotate(0.02, 0.02, 0);
	

	while (true) {
		gw.clear();
		d.move(gw.info.w / 2, gw.info.h / 2);
		d.rotate(0.001, 0.001, 0.0012);
		
		d.render();
		gw.render();
		gw.handleEvents();
	}

	return EXIT_SUCCESS;
}