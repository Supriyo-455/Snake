#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Color.h"

class Texture
{
public:
	Texture(SDL_Renderer* renderer);
	~Texture();

	bool loadFromFile(std::string path);
	bool loadFromRenderedText(std::string textureText, TTF_Font* font, Color color);
	void free();
	void setColor(Color color);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(uint8_t alpha);
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	int getWidth();
	int getHeight();

private:
	SDL_Texture* m_Texture;
	SDL_Renderer* m_Renderer;
	int m_Width;
	int m_Height;
};