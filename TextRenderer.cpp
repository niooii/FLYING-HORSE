#include "TextRenderer.h"
#include <iostream>

TextRenderer::TextRenderer()
{
	this->info = nullptr;
	this->text = "";
	this->oldText = "";
	this->fontPath = fontPath = "";
	this->fontSize = fontSize = NULL;
	this->font = nullptr;
}

TextRenderer::TextRenderer(winfo* winfo, SDL_Renderer* renderer, std::string text, const char* fontPath, int fontSize, SDL_Color textColor)
{
	this->renderer = renderer;
	this->info = winfo;
	this->text = text;
	this->oldText = text;
	this->fontPath = fontPath;
	this->fontSize = fontSize;
	this->textColor = textColor;
	this->font = TTF_OpenFont(fontPath, fontSize);
	textToTexture();
}

void TextRenderer::loadFont(const char* fontPath, int fontSize)
{
	font = TTF_OpenFont(fontPath, fontSize);
	textToTexture();
}

void TextRenderer::setFontSize(int fontSize)
{
	this->fontSize = fontSize;
	loadFont(fontPath, fontSize);
	textToTexture();
}

void TextRenderer::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	textColor = { r, g, b };
	textToTexture();
}

void TextRenderer::renderText()
{
	if (text != oldText)
		textToTexture();

	if (x != size.x)
		size.x = x;
	if (y != size.y)
		size.y = y;

	SDL_QueryTexture(textTexture, NULL, NULL, &size.w, &size.h);

	SDL_RenderCopyEx(renderer, textTexture, NULL, &size, 0, NULL, SDL_FLIP_NONE);
}

void TextRenderer::textToTexture()
{
	if(textTexture != nullptr)
		SDL_DestroyTexture(textTexture);
	if (textSurface != nullptr)
		SDL_FreeSurface(textSurface);
	textSurface = TTF_RenderText_Solid_Wrapped(font, text.c_str(), textColor, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
}

void TextRenderer::regenerateTexture()
{
	if (textTexture != nullptr)
		SDL_DestroyTexture(textTexture);
	if (textSurface != nullptr)
		SDL_FreeSurface(textSurface);
	textSurface = TTF_RenderText_Solid_Wrapped(font, text.c_str(), textColor, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &size.w, &size.h);
}

void TextRenderer::center()
{
	//just to be safe
	regenerateTexture();
	x = info->w / 2 - size.w / 2;
	y = info->h / 2 - size.h / 2;
}

void TextRenderer::quit()
{
	SDL_DestroyTexture(textTexture);
	TTF_Quit();
}