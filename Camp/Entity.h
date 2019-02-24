#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>

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
	bool CollidesWith(Entity e);
	SDL_Color GetPixelColor(SDL_Surface * pSurface, const int X, const int Y);
	bool CollidesWithPrecise(Entity e);
	void Draw();
	SDL_Rect* GetSprite();
	SDL_Surface* GetSurface();
	void SetTexture(std::string str);
	SDL_Texture* GetTexture();
	int GetEwidth();
	void WetEwidth(int width);
	int GetEheight();
	//SDL Stuff
	SDL_Renderer* Erenderer;
	~Entity();

private:
	SDL_Rect EstretchRect;
	SDL_Surface* ESurface;
	SDL_Texture* ETexture;
	SDL_Rect* ESprite;
	//Entity Info
	int Ewidth;
	int Eheight;
	int ExPos;
	int EyPos;

};

