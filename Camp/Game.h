#pragma once
#include "Player.h"
#include "UDP_Client.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
//#undef main
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>

class Game
{
public:
	Game();
	~Game();

	enum GameState
	{
		State_Main_Menu,
		State_Play_Game,
		State_Other
	};

private:
	const double MStoS = 1000.0;
	const double NStoMS = 1000000.0;
	const Uint8* keystates;
	std::string serverIP;
	//functions
	void init();
	void Loop();
	double getTime();
	void InitSDL();
	void InitEntity();
	void InitMedia();
	bool loadMedia();
	void DrawFPS(bool outline, SDL_Color color);
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
	TTF_Font* Sans;
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;
	SDL_Rect Message_rect;

	//SDL colors
	const SDL_Color White = { 255, 255, 255 };
	const SDL_Color Black = { 0, 0, 0 };
	//Event handler
	SDL_Event e;
	//Doubles
	std::chrono::system_clock::time_point previousTime;
	std::chrono::system_clock::time_point currentTime;
	std::chrono::system_clock::time_point fpsTime;
	std::chrono::system_clock::time_point priorTime;
	std::chrono::duration<double, std::milli> elapsedfpsTime;
	std::chrono::duration<double, std::milli> elapsedTime;
	std::chrono::duration<double, std::milli> sleepTime;
	double updateTime;
	//Booleans
	bool keepGoing;
	bool showFPS;
	bool singlePlayer;

	//Server Info
	UDP_Client* ptrClient;

	//Entities
	Player testP;
	Player testE;
};

