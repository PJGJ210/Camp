#include "Game.h"
#include "Entity.h"
#include "UDP_Client.h"
#include <string>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <chrono>
#include <thread>

Game::Game()
{
	//Set Up Server info
	std::cout << "Enter Server IP" << std::endl;
	std::cin >> serverIP;
	UDP_Client Client = UDP_Client(serverIP);
	ptrClient = &Client;
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
	InitMedia();
	InitEntity();
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
	playerID = "00";
	Players = new Player[10];
	for (int i = 0; i < MaxPlayers; i++)
	{
		Players[i] = Player();
		Players[i].SetRenderer(renderer);
		Players[i].SetTexture("Media/images/Hole.png");
	}
	//testP.SetRenderer(renderer);
	//testE.SetRenderer(renderer);
	//testE.SetExPos(testP.GetExPos() + 500);
	//testE.SetEyPos(testP.GetEyPos());
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
	char* buffer = ptrClient->ReceiveData();
	if (buffer) 
	{
		HandlePacket(buffer);
	}
	/*if (testP.CollidesWith(testE))
	{
		testP.DamageHealth(1);
	}
	if (!testP.Alive)
	{
		std::cout << "Player Destroyed" << std::endl;
		keepGoing = false;
	}*/
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
	//Draw players
	for (int i = 0; i < MaxPlayers; i++)
	{
		//std::cout << Players[i].GetID() << (Players[i].GetID() != 0) << std::endl;
		if (Players[i].GetID() != 0)
		{
			//std::cout << "Drawing " << Players[i].GetID() << std::endl;
			Players[i].Draw();
		}
	}
	//testP.Draw();
	//testE.Draw();
	SDL_RenderPresent(renderer);
	//SDL_UpdateWindowSurface( window );
}

void Game::Input() {
	//Handle events on queue
	while (SDL_PollEvent(&e)) {
		//User presses a key
		if (e.type == SDL_KEYDOWN)
		{
			std::string outputdata;
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

			case SDLK_c:
				outputdata = std::to_string(1) + playerID + "C";
				std::cout << outputdata << std::endl;
				ptrClient->SendData(outputdata);
				break;

			case SDLK_x:
				outputdata = std::to_string(2) + playerID + "X";
				std::cout << outputdata << std::endl;
				ptrClient->SendData(outputdata);
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
				FPS++;
				updateTime = MStoS / FPS;
				showFPS = true;
			}
			else if (e.wheel.y < 0) // scroll down
			{
				FPS--;
				updateTime = MStoS / FPS;
				showFPS = false;
			}
		}
		//User requests quit
		if (e.type == SDL_QUIT) {
			std::cout << "Quiting" << std::endl;
			keepGoing = false;
		}
	}//end while event
	std::string outputdata;
	if (keystates[SDL_SCANCODE_UP])
	{
		outputdata = std::to_string(3) + playerID + "U";
		std::cout << outputdata << std::endl;
		ptrClient->SendData(outputdata);
		for (int i = 0; i < MaxPlayers; i++)
		{
			if (Players[i].GetID() == std::atoi(playerID.c_str()))
				Players[i].SetEyPos(Players[i].GetEyPos() - 4.25f);
		}
		//testP.SetEyPos(testP.GetEyPos() - 4.25f);
		//testP.HealHealth(1);
		std::cout << "Up: " << std::endl;
	}
	if (keystates[SDL_SCANCODE_DOWN])
	{
		outputdata = std::to_string(3) + playerID + "D";
		std::cout << outputdata << std::endl;
		ptrClient->SendData(outputdata);
		for (int i = 0; i < MaxPlayers; i++)
		{
			if (Players[i].GetID() == std::atoi(playerID.c_str()))
				Players[i].SetEyPos(Players[i].GetEyPos() + 4.25f);
		}
		//testP.SetEyPos(testP.GetEyPos() + 4.25f);
		//testP.HealMana(1);
		std::cout << "Down: " << std::endl;
	}
	if (keystates[SDL_SCANCODE_LEFT])
	{
		outputdata = std::to_string(3) + playerID + "L";
		std::cout << outputdata << std::endl;
		ptrClient->SendData(outputdata);
		for (int i = 0; i < MaxPlayers; i++)
		{
			if (Players[i].GetID() == std::atoi(playerID.c_str()))
				Players[i].SetExPos(Players[i].GetExPos() - 4.25f);
		}
		//testP.SetExPos(testP.GetExPos() - 4.25f);
		//testP.DamageMana(1);
		std::cout << "Left: " << std::endl;
	}
	if (keystates[SDL_SCANCODE_RIGHT])
	{
		outputdata = std::to_string(3) + playerID + "R";
		std::cout << outputdata << std::endl;
		ptrClient->SendData(outputdata);
		for (int i = 0; i < MaxPlayers; i++)
		{
			if (Players[i].GetID() == std::atoi(playerID.c_str()))
				Players[i].SetExPos(Players[i].GetExPos() + 4.25f);
		}
		//testP.SetExPos(testP.GetExPos() + 4.25f);
		//testP.DamageHealth(1);
		std::cout << "Right: " << testP.Alive << std::endl;
	}
}//end input


void Game::HandlePacket(char* buffer)
{
	std::string PlayerID;
	int iPlayerID;
	//deal with type of packet
	//std::cout << ptrClient->buffer[0] << std::endl;
	switch (buffer[0])
	{
	case '1':
		//deal with player ID if exists 1-2
		PlayerID = CopyBuffer(1, 2, buffer);
		iPlayerID = std::atoi(PlayerID.c_str());
		std::cout << PlayerID << ":" << iPlayerID << std::endl;
		//deal with action 3
		switch (buffer[3])
		{
			case 'A':
				//Set the player's ID to the one given by the server
				playerID = PlayerID;
				std::cout << "Case : A" << std::endl;
				break;
			case 'X':
				std::cout << "Case : X" << std::endl;
				break;
		}
		break;
	case '2':
		//iterate each segment
		int PlayerIDLen = 2;
		int posLen = 10;
		int statLen = 4;
		std::string PlayerX;
		int iPlayerX;
		std::string PlayerY;
		int iPlayerY;
		std::string PlayerHP;
		int iPlayerHP;
		std::string PlayerMP;
		int iPlayerMP;
		int bufferIterator = 0;
		while(bufferIterator < (int)strlen(buffer))
		{
			bool PlayerFound = false;
			bufferIterator++;
			//get player ID 2 length
			PlayerID = CopyBuffer(bufferIterator, bufferIterator - 1 + PlayerIDLen, buffer);
			iPlayerID = std::atoi(PlayerID.c_str());
			//std::cout << PlayerID << ":" << iPlayerID << std::endl;
			bufferIterator += PlayerIDLen;
			//get PlayerX
			PlayerX = CopyBuffer(bufferIterator, bufferIterator - 1 + posLen, buffer);
			iPlayerX = std::atoi(PlayerX.c_str());
			//std::cout << PlayerX << ":" << iPlayerX << std::endl;
			bufferIterator += posLen;
			//get PlayerY
			PlayerY = CopyBuffer(bufferIterator, bufferIterator - 1 + posLen, buffer);
			iPlayerY = std::atoi(PlayerY.c_str());
			//std::cout << PlayerY << ":" << iPlayerY << std::endl;
			bufferIterator += posLen;
			//get PlayerHP
			PlayerHP = CopyBuffer(bufferIterator, bufferIterator - 1 + statLen, buffer);
			iPlayerHP = std::atoi(PlayerHP.c_str());
			//std::cout << PlayerHP << ":" << iPlayerHP << std::endl;
			bufferIterator += statLen;
			//get PlayerMP
			PlayerMP = CopyBuffer(bufferIterator, bufferIterator - 1 + statLen, buffer);
			iPlayerMP = std::atoi(PlayerMP.c_str());
			//std::cout << PlayerMP << ":" << iPlayerMP << std::endl;
			bufferIterator += statLen;

			//Update Player
			//If player exists then update the coords, else create the character with given
			int i = 0;
			while(!PlayerFound && i < MaxPlayers) 
			{
				if (Players[i].GetID() == iPlayerID)
				{
					PlayerFound = true;
					Players[i].SetID(iPlayerID);
					Players[i].SetX(iPlayerX);
					Players[i].SetY(iPlayerY);
					Players[i].SetHP(iPlayerHP);
					Players[i].SetMP(iPlayerMP);
				}
				i++;
			}
			i = 0;
			while (!PlayerFound && i < MaxPlayers)
			{
				std::cout << Players[i].GetID() << (Players[i].GetID() == 0) << std::endl;
				if (Players[i].GetID() == 0)
				{
					PlayerFound = true;
					Players[i].SetID(iPlayerID);
					Players[i].SetX(iPlayerX);
					Players[i].SetY(iPlayerY);
					Players[i].SetHP(iPlayerHP);
					Players[i].SetMP(iPlayerMP);
					std::cout << "Player " << iPlayerID << " built" << std::endl;
				}
				i++;
			}
		}
		break;
	}
}

std::string Game::CopyBuffer(int start, int end, char* buffer)
{
	std::string stringBuild = "";
	for (int i = start; i <= end; i++)
	{
		stringBuild += buffer[i];
	}
	return stringBuild;
}

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
		ptrClient->ReceiveData();
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
	//testP.SetTexture("Media/images/Hole.png");
	//testE.SetTexture("Media/images/Hole_R.png");
	std::cout << "Player Media Loaded" << std::endl;
	return success;
}

void Game::Destroy() {
	//Destroy window
	SDL_DestroyWindow(window);
	IMG_Quit();
	//Quit SDL subsystems
	SDL_Quit();
	ptrClient->CloseClient();
}
Game::~Game()
{
}