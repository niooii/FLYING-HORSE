#pragma once
#include <wchar.h>
#include <windows.h>
#include <winbase.h>
#include <Tlhelp32.h>
#include <psapi.h>
#include <SDL.h>
#include <SDL_image.h>
#include <process.h>

namespace utils
{
	void killProcessByName(const wchar_t* filename);
	SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);

	//launch force func (will modify later.) d is distance to ground
	double lfFunc(double launchForce, int d);
}