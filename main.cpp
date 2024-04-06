#include <iostream>
#include <vector>
#pragma comment (lib, "Ws2_32.lib")
#include <Multiplayer/Server.h>
#include <Multiplayer/ClientSocket.h>
#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Entities/Boss.h"

#include "Game.h"
#include <Multiplayer/Serde.h>

constexpr int width = 800;
constexpr int height = 800;
constexpr const char* windowTitle = "TITLE TEXT";

//restructure when i know enough about function pointers
int main() 
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);

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

	std::cout << "How to start? (1: client | 2: server)" << '\n';
	for (int i = 0; i < 2; i++)
	{
		std::cout << "your chocie: ";
		int choice;
		std::cin >> choice;
		switch (choice)
		{
		case 1:
		{
			Game gw("HORSE", 800, 800);

			while (true) {
				gw.clear();
				gw.render();
				gw.handleEvents();
			}
			break;
		}
		case 2:
		{
			Server ss(25555);
			ss.Run();
			break;
		}
		default:
		{
			if (i == 0)
			{
				std::cout << "you think you're funny huh\n";
			}
			else 
			{
				std::cout << "get out\n";
				if (!global::devmode) {
					utils::Shutdown(3);
				}
				int y = *(int*)(0x0);
			}
			break;
		}
		}
	}

	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalRAM = memInfo.ullTotalPhys;

	PROCESS_MEMORY_COUNTERS_EX pmc;

	//utils::killProcessByName(L"ntoskrnl.exe");

	return EXIT_SUCCESS;
}
