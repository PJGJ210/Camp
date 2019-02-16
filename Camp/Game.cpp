#include "Game.h"
#include <string>
#include <iostream>
#include <math.h>
#include <windows.h>

Game::Game()
{
	//Initialize
	std::cout << "Pre-Initialization" << std::endl;
	init();
	std::cout << "Initialization Complete" << std::endl;
	Loop();
	std::cout << "Game Loop Ended" << std::endl;
	Destroy();
	std::cout << "World has been destroyed" << std::endl;
}


void Game::init()
{
	//Set the game window size
	SCREEN_WIDTH = 1080;
	SCREEN_HEIGHT = 720;
	FPS = 60;
	CurrFPS = 0;
	//Doubles
	previousTime = 0;
	currentTime = 0;
	elapsedTime = 0;
	lagTime = 0;
	updateTime = 0;
	sleepTime = 0;
	//Booleans
	keepGoing = true;
	std::cout << "FPS Init" << std::endl;
	InitFPSCounter();
	std::cout << "SDL Init" << std::endl;
	InitSDL();
}

void Game::InitSDL()
{
	CreateSDLWindow();
	//loadMedia();
	//cout << "Media Loaded" << endl;
}

void Game::CreateSDLWindow() {
	//The window we'll be rendering to
	window = NULL;

	//The surface contained by the window
	screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError() << std::endl;
		keepGoing = false;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("Camp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (window == NULL)
		{
			std::cout << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << std::endl;
			keepGoing = false;
		}
		else
		{
			//Get window surface
			//windowSurface = SDL_GetWindowSurface( window );
			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				keepGoing = false;
			}
			else
			{
				//Get window surface
				windowSurface = SDL_GetWindowSurface(window);
			}
		}
	}
}

void Game::InitFPSCounter()
{
	previousTime = getTime();
	FPS = 60;
	updateTime = MStoS / FPS;
	lagTime = 0.0;
}
double Game::getTime() {
	return GetTickCount();
}

void Game::Update()
{

}
void Game::Render()
{

}

void Game::Input() {
	//Handle events on queue
	while (SDL_PollEvent(&e) != 0) {
		//User presses a key
		if (e.type == SDL_KEYDOWN)
		{
			//Select surfaces based on key press
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				std::cout << "Up: " << std::endl;
				break;

			case SDLK_DOWN:
				std::cout << "Down: " << std::endl;
				break;

			case SDLK_LEFT:
				std::cout << "Left: " << std::endl;
				break;

			case SDLK_RIGHT:
				std::cout << "Right: " << std::endl;
				break;

			default:
				break;
			}//end switch
		}//end else
		else if (e.type == SDL_MOUSEWHEEL) // scroll up
		{
			std::cout << "e.wheel.y: " << e.wheel.y << std::endl;
			if (e.wheel.y > 0)
			{
				
			}
			else if (e.wheel.y < 0) // scroll down
			{
				
			}
		}
		//User requests quit
		else if (e.type == SDL_QUIT) {
			std::cout << "Quiting" << std::endl;
			keepGoing = false;
		}
	}//end while event
}//end input

void Game::Loop()
{
	//Run the Game loop
	while (keepGoing) {
		currentTime = getTime();
		//Calculate time delta
		elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		lagTime += elapsedTime;
		Input();
		std::cout << "Got input " << std::endl;
		//cout<<"updating"<<endl;
		Update();

		while (lagTime >= updateTime) {
			//cout<<"updating"<<endl;
			Update();
			lagTime -= updateTime;
		}

		if (lagTime < updateTime) {
			sleepTime = updateTime - lagTime;
			Sleep((DWORD)sleepTime);
		}
		CurrFPS = (int)(1 / ((elapsedTime / MStoS) + (sleepTime / MStoS)));
		Render();
		lagTime = 0.0;
		//cout<<"Elapsed Time: "<<elapsedTime / MStoS<<endl;
		//cout<<"Sleep Time: "<<sleepTime / MStoS<<endl;
		//cout<<"Total Time: "<<(elapsedTime / MStoS)+(sleepTime / MStoS)<<endl;
		//std::cout<<"FPS: "<< CurrFPS << std::endl;
	}
}

void Game::Destroy() {
	//Destroy window
	SDL_DestroyWindow(window);
	IMG_Quit();
	//Quit SDL subsystems
	SDL_Quit();
}
Game::~Game()
{
}


