#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>

class Entity
{
public:
	Entity();
	Entity(SDL_Renderer* renderer);
	Entity(SDL_Renderer* renderer, int width, int height, int xPos, int yPos);
	void SetRenderer(SDL_Renderer* renderer);
	void SetEheight(int height);
	int GetExPos();
	void SetExPos(int xPos);
	int GetEyPos();
	void SetEyPos(int yPos);
	void Draw();
	SDL_Rect* GetSprite();
	void SetSprite(SDL_Texture* Spritesheet);
	SDL_Texture* GetSpriteSheet();
	int GetEwidth();
	void WetEwidth(int width);
	int GetEheight();
	~Entity();

private:
	//SDL Stuff
	SDL_Renderer* Erenderer;
	SDL_Rect EstretchRect;

	SDL_Texture* ESpriteSheet;
	SDL_Rect* ESprite;
	//Entity Info
	int Ewidth;
	int Eheight;
	int ExPos;
	int EyPos;

};

