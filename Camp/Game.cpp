#include "Game.h"
#include "Entity.h"
#include <string>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <chrono>
#include <thread>

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
	FPS = 144;
	updateTime = MStoS / FPS;
	CurrFPS = 0;
	//Booleans
	showFPS = true;
	keepGoing = true;

	keystates = SDL_GetKeyboardState(NULL);

	std::cout << "SDL Init" << std::endl;
	InitSDL();
	InitEntity();
	InitMedia();
}

void Game::InitSDL()
{
	CreateSDLWindow();
	std::cout << "TTF Init" << std::endl;
	if (TTF_Init() < 0)
	{
		keepGoing = false;
	}
}

void Game::InitEntity()
{
	testP.SetRenderer(renderer);
	testE.SetRenderer(renderer);
	testE.SetExPos(testP.GetExPos() + 500);
	testE.SetEyPos(testP.GetEyPos());
}

void Game::InitMedia()
{
	if (!loadMedia())
	{
		keepGoing = false;
	}
	std::cout << "Media Loaded" << std::endl;
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
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
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

double Game::getTime() {
	return GetTickCount();
}

void Game::Update()
{
	if (testP.CollidesWith(testE))
	{
		testP.DamageHealth(1);
	}
	if (!testP.Alive)
	{
		std::cout << "Player Destroyed" << std::endl;
		keepGoing = false;
	}
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	if (showFPS)
	{
		DrawFPS(true, Black);
		DrawFPS(false, White);
	}
	testP.Draw();
	testE.Draw();
	SDL_RenderPresent(renderer);
	//SDL_UpdateWindowSurface( window );
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
				break;

			case SDLK_DOWN:
				break;

			case SDLK_LEFT:
				break;

			case SDLK_RIGHT:
				break;

			default:
				break;
			}//end switch
		}//end else
		if (e.type == SDL_MOUSEWHEEL) // scroll up
		{
			std::cout << "e.wheel.y: " << e.wheel.y << std::endl;
			if (e.wheel.y > 0)
			{
				showFPS = true;
			}
			else if (e.wheel.y < 0) // scroll down
			{
				showFPS = false;
			}
		}
		//User requests quit
		if (e.type == SDL_QUIT) {
			std::cout << "Quiting" << std::endl;
			keepGoing = false;
		}
	}//end while event
	if (keystates[SDL_SCANCODE_UP])
	{
		testP.SetEyPos(testP.GetEyPos() - 5);
		//testP.HealHealth(1);
		std::cout << "Up: " << std::endl;
	}
	if (keystates[SDL_SCANCODE_DOWN])
	{
		testP.SetEyPos(testP.GetEyPos() + 5);
		//testP.HealMana(1);
		std::cout << "Down: " << std::endl;
	}
	if (keystates[SDL_SCANCODE_LEFT])
	{
		testP.SetExPos(testP.GetExPos() - 5);
		//testP.DamageMana(1);
		std::cout << "Left: " << std::endl;
	}
	if (keystates[SDL_SCANCODE_RIGHT])
	{
		testP.SetExPos(testP.GetExPos() + 5);
		//testP.DamageHealth(1);
		std::cout << "Right: " << testP.Alive << std::endl;
	}
}//end input

void Game::Loop()
{
	currentTime = std::chrono::system_clock::now();
	fpsTime = std::chrono::system_clock::now();
	priorTime = std::chrono::system_clock::now();
	previousTime = currentTime;
	//Run the Game loop
	while (keepGoing) {
		currentTime = std::chrono::system_clock::now();
		//Calculate time delta
		elapsedTime = currentTime - previousTime;
		Input();
		//cout<<"updating"<<endl;
		Update();

		//Sleep if spending update did not take enough time
		if (elapsedTime.count() < updateTime) {
			std::chrono::duration<double, std::milli> delta_ms(updateTime - elapsedTime.count());
			auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
			std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
		}

		previousTime = std::chrono::system_clock::now();
		sleepTime = previousTime - currentTime;

		//Do an fps count
		fpsTime = std::chrono::system_clock::now();
		elapsedfpsTime = fpsTime - priorTime;
		if (elapsedfpsTime.count() > 1000)
		{
			CurrFPS = (int)(MStoS / (elapsedTime + sleepTime).count());
			std::cout << "FPS: " << CurrFPS << std::endl;
			priorTime = fpsTime;
		}
		Render();
		//cout<<"Elapsed Time: "<< MStoS / elapsedTime <<endl;
		//cout<<"Sleep Time: "<< MStoS / sleepTime <<endl;
	}
}

void Game::DrawFPS(bool outline, SDL_Color color)
{
	if (Sans)
	{
		TTF_SetFontOutline(Sans, outline);
		std::string CharFPS = std::to_string(CurrFPS);
		//cout<<CharFPS.c_str()<<endl;
		surfaceMessage = TTF_RenderText_Solid(Sans, CharFPS.c_str(), color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		if (surfaceMessage == NULL)
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			keepGoing = false;
		}
		else
		{
			Message_rect.x = 0;  //controls the rect's x coordinate
			Message_rect.y = 0; // controls the rect's y coordinte
			Message_rect.w = surfaceMessage->w; // controls the width of the rect
			Message_rect.h = surfaceMessage->h; // controls the height of the rect
			Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
			if (Message == NULL)
			{
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
				keepGoing = false;
			}
			else
			{
				SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
				SDL_DestroyTexture(Message);
			}//end texture
			SDL_FreeSurface(surfaceMessage);
		}//end surface
	}//end font
}//end draw

bool Game::loadMedia()
{
	//Loading success flag
	bool success = true;
	//Load font
	Sans = TTF_OpenFont("Media/fonts/OpenSans-Regular.ttf", 24);
	if (!Sans)
	{
		std::cout << TTF_GetError() << std::endl;
		success = false;
	}
	//Load Textures
	std::cout << "Loading Player Media" << std::endl;
	testP.SetTexture("Media/images/Hole.png");
	testE.SetTexture("Media/images/Hole_R.png");
	std::cout << "Player Media Loaded" << std::endl;
	return success;
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


