#pragma once

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "Color.h"
#include "Timer.h"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800; 
const int SCREEN_FPS = 15;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

struct Rect
{
	int m_XPos;
	int m_YPos;
	int m_Width;
	int m_Height;

	Rect(int x = 0, int y = 0, int w = 20, int h = 20)
		: m_XPos(x), m_YPos(y), m_Width(w), m_Height(h) {}

	virtual ~Rect() {}

	virtual void draw(SDL_Renderer* renderer, Color color) {
		SDL_Rect rect = { m_XPos, m_YPos, m_Width, m_Height };
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &rect);
	}
};

struct SnakePart : public Rect
{
	SnakePart(int x, int y, int w, int h) : Rect(x, y, w, h){}
};

struct Food : public Rect
{
	Food(int x, int y, int w, int h, Color color = Color(255, 0, 0, 255)) : Rect(x, y, w, h), m_Color(color) {}
	Color m_Color;

	void respwan();
};

struct Snake
{
	Color m_Color;
	int m_WidhtPerPart;
	int m_HeightPerPart;

	std::vector<SnakePart*> m_Body;
	SnakePart* m_Head;
	int m_XDir;
	int m_YDir;
	bool m_IsAlive;

	Snake();
	~Snake();

	int length();
	void eat(Food* food);
	void checkCollision(Food* food);
	void move(int xDir, int yDir);
	void draw(SDL_Renderer* renderer);
	void updateBody(int newPosX, int newPosY);
};

struct Game
{
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	bool m_IsRunning;

	int m_SnakeXDir;
	int m_SnakeYDir;

	bool init();
	void close();
	void input();
	void clearFrame();
	void drawGrid(Color color, int rectW, int rectH);

	void run();
};
