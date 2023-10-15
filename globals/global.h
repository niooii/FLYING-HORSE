#pragma once
#include <SDL_stdinc.h>

//all information objects need about the window.
struct winfo
{
	int x{};
	int y{};

	int w{};
	int h{};

	//mouse x coordinates
	int mX{};
	//mouse x coordinates
	int mY{};

	double deltaTime{};
};

namespace global
{
	extern bool isFullScreen;
	extern bool reverse;
	extern bool leaking; //heh
	extern bool IInteractable; //heh
	extern bool IInteracted; // heheheh
	extern bool allowedQuit;
	extern bool bossActive;
	extern bool hitByUlt;

	//developer
	extern bool devmode;
	extern bool startBoss;
	extern bool infhp;
}