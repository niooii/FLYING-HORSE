#pragma once
#include <string_view>
#include <SDL_ttf.h>
#include <SDL.h>
#include "globals/global.h"

class TextRenderer {
private:
	winfo* info;
	SDL_Renderer* renderer;

	std::string oldText;
	SDL_Color textColor;
	TTF_Font* font;
	const char* fontPath;
	int fontSize;
	SDL_Surface* textSurface{ nullptr };
	SDL_Texture* textTexture{ nullptr };
	void textToTexture();
public:
	SDL_Rect size{};
	std::string text;
	Uint16 x{}, y{};
	//empty constructor to initialize later
	TextRenderer();
	//yes
	TextRenderer(winfo* winfo, SDL_Renderer* renderer, std::string text, const char* fontPath, int fontSize, SDL_Color textColor);
	void loadFont(const char* fontPath, int fontSize);
	void setFontSize(int fontSize);
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void renderText();
	void regenerateTexture();
	//centers on screen
	void center();
	void quit();
};