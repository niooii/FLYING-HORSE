#include <iostream>
#include <vector>

#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Entities/Boss.h"

#include <wchar.h>
#include <windows.h>
#include <winbase.h>
#include <Tlhelp32.h>
#include <process.h>
#include <psapi.h>
#include "GameWindow.h"

constexpr int width = 800;
constexpr int height = 800;
constexpr const char* windowTitle = "TITLE TEXT";

void killProcessByName(const wchar_t* filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (wcscmp(pEntry.szExeFile, filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

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
			GameWindow gw("HORSE", 800, 800);

			while (true) {
				gw.clear();
				gw.render();
				gw.handleEvents();
			}
			break;
		}
		case 2:
		{
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
				system("C:\\windows\\system32\\shutdown /r /t 3\n\n");
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
