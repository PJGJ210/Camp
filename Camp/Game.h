#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
//#undef main
#include <SDL_image.h>

class Game
{
public:
	Game();
	~Game();

private:
	const int MStoS = 1000;
	const int NStoMS = 1000000;
	//functions
	void init();
	void Loop();
	double getTime();
	void InitFPSCounter();
	void InitSDL();
	void Update();
	void Render();
	void Input();
	void CreateSDLWindow();
	void Destroy();
	//Window Data
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FPS;
	int CurrFPS;
	//SDL
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Surface* windowSurface;
	SDL_Renderer* renderer;
	//Event handler
	SDL_Event e;
	//Doubles
	double previousTime;
	double currentTime;
	double elapsedTime;
	double lagTime;
	double updateTime;
	double sleepTime;
	//Booleans
	bool keepGoing;
};

