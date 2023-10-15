#include "Utility.h"
#include <iostream>


namespace utils {
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

	SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path)
	{
		SDL_Texture* texture;

		if ((texture = IMG_LoadTexture(renderer, path)) == NULL)
		{
			std::cout << SDL_GetError() << '\n';
		}

		return texture;
	}

	
	double lfFunc(double launchForce, int d)
	{
		return launchForce * (std::pow(4, -0.01 * (d - 80)) + 1);
	}

}
