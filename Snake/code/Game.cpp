#include "Game.h"

SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

Snake::Snake()
{
	m_XDir = 0;
	m_YDir = 0;
	m_WidhtPerPart = 20;
	m_HeightPerPart = 20;
	m_IsAlive = true;
	m_Color = Color(0, 255, 0, 255);
	m_Head = new SnakePart(0, 0, m_WidhtPerPart, m_HeightPerPart);
	m_Body.push_back(m_Head);
}

Snake::~Snake()
{
	for (SnakePart* sp : m_Body)
	{
		delete sp;
	}
}

void Snake::updateBody(int newPosX, int newPosY)
{
	int newXPos = newPosX;
	int newYPos = newPosY;

	for (int i = 0; i < m_Body.size(); i++)
	{
		int prevXPos = m_Body.at(i)->m_XPos;
		int prevYPos = m_Body.at(i)->m_YPos;

		m_Body.at(i)->m_XPos = newXPos;
		m_Body.at(i)->m_YPos = newYPos;

		newXPos = prevXPos;
		newYPos = prevYPos;
	}
}

void Snake::eat(Food* food)
{
	SnakePart* sp = new SnakePart(food->m_XPos, food->m_YPos, food->m_Width, food->m_Height);
	m_Body.push_back(sp);
	this->updateBody(food->m_XPos, food->m_YPos);
}

void Snake::checkCollision(Food* food)
{
	if (this->length() > 1)
		for (int i = 1; i < this->length(); i++)
		{
			if (m_Head->m_XPos == m_Body.at(i)->m_XPos && m_Head->m_YPos == m_Body.at(i)->m_YPos)
			{
				m_IsAlive = false;
			}
		}

	if (food->m_XPos == m_Head->m_XPos && food->m_YPos == m_Head->m_YPos)
	{
		this->eat(food);
		food->respwan();

		// TODO: This is the worst code for food respawn need to update
		bool perfectRespawn = false;
		while (!perfectRespawn)
		{
			perfectRespawn = true;
			for (SnakePart* sp : m_Body)
			{
				if (sp->m_XPos == food->m_XPos && sp->m_YPos == food->m_YPos)
				{
					perfectRespawn = false;
					food->respwan();
					break;
				}
			}
		}
	}
}

int Snake::length()
{
	return m_Body.size();
}

void Snake::move(int xDir, int yDir)
{
	if (this->length() > 1)
	{
		if (m_XDir == -xDir)
			xDir = m_XDir;
		if (m_YDir == -yDir)
			yDir = m_YDir;
	}

	m_XDir = xDir;
	m_YDir = yDir;

	int newHeadX = m_Head->m_XPos + m_XDir * m_Head->m_Width;
	int newHeadY = m_Head->m_YPos + m_YDir * m_Head->m_Height;

	if (newHeadX < 0)
	{
		newHeadX = SCREEN_WIDTH;
	}
	else if (newHeadX > SCREEN_WIDTH - m_Head->m_Width)
	{
		newHeadX = 0;
	}
	
	if (newHeadY < 0)
	{
		newHeadY = SCREEN_HEIGHT;
	}
	else if (newHeadY > SCREEN_HEIGHT - m_Head->m_Height)
	{
		newHeadY = 0;
	}

	this->updateBody(newHeadX, newHeadY);
}

void Snake::draw(SDL_Renderer* renderer)
{
	for (SnakePart* sp : m_Body)
	{
		sp->draw(renderer, m_Color);
	}
}

void Food::respwan()
{
	int cellsAlongXAxis = SCREEN_WIDTH / m_Width;
	int cellsAlongYAxis = SCREEN_HEIGHT / m_Height;

	m_XPos = (rand() % cellsAlongXAxis) * m_Width;
	m_YPos = (rand() % cellsAlongYAxis) * m_Height;
}

bool Game::init()
{
	m_Window = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// TODO: Logging
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		m_Window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (m_Window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
			if(m_Renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				return false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);

				// Initializing PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					return false;
				}
			}
		}
	}

	return true;
}

void Game::close()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	m_Window = NULL;

	SDL_Quit();
}

void Game::input()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			m_IsRunning = false;

		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				m_SnakeXDir = 0;
				m_SnakeYDir = -1;
				break;

			case SDLK_DOWN:
				m_SnakeXDir = 0;
				m_SnakeYDir = 1;
				break;

			case SDLK_LEFT:
				m_SnakeXDir = -1;
				m_SnakeYDir = 0;
				break;

			case SDLK_RIGHT:
				m_SnakeXDir = 1;
				m_SnakeYDir = 0;
				break;

			default:
				break;
			}
		}
	}
}

void Game::clearFrame()
{
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
	SDL_RenderClear(m_Renderer);
}

void Game::drawGrid(Color color, int rectW, int rectH)
{
	for (int i = 0; i < SCREEN_WIDTH; i += rectW)
	{
		Rect rect = Rect(i, 0, 1, SCREEN_HEIGHT);
		rect.draw(m_Renderer, color);
	}
	for (int j = 0; j < SCREEN_HEIGHT; j += rectH)
	{
		Rect rect = Rect(0, j, SCREEN_HEIGHT, 1);
		rect.draw(m_Renderer, color);
	}
}

void Game::run()
{
	if (!this->init())
	{
		// TODO: Logging
		printf("unable to init window!");
	}

	m_IsRunning = true;
	
	Timer fpsTimer;
	Timer capTimer;

	int countedFrames = 0;

	Color redColor = Color(255, 0, 0, 255);
	Color blueColor = Color(0, 0, 255, 255);

	Snake snake;
	Food food = Food(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, snake.m_WidhtPerPart, snake.m_HeightPerPart);

	while (m_IsRunning)
	{
		fpsTimer.start();
		capTimer.start();

		this->clearFrame();
		this->input();

		this->drawGrid(blueColor, snake.m_WidhtPerPart, snake.m_HeightPerPart);
		
		snake.move(m_SnakeXDir, m_SnakeYDir);
		
		snake.checkCollision(&food);

		food.draw(m_Renderer, food.m_Color);

		snake.draw(m_Renderer);

		// Update the renderer
		SDL_RenderPresent(m_Renderer);

		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}

		double FPS = 1 / (fpsTimer.getTicks() / 1000.0);
		std::cout << "FPS: " << FPS << std::endl;

		if (snake.m_IsAlive == false)
		{
			m_IsRunning = false;
			std::cout << "GAME OVER!!" << std::endl;
			std::cout << "Score: " << snake.length() * 10 << std::endl;
		}
	}

	this->close();
}
