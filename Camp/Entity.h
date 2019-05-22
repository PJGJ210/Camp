#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>

class Entity
{
public:
	Entity();
	Entity(SDL_Renderer* renderer);
	Entity(SDL_Renderer* renderer, float width, float height, float xPos, float yPos);
	//Tile Sheets
	SDL_Rect MoveClips[16];
	SDL_Rect AttackClips[16];

	void SetRenderer(SDL_Renderer* renderer);
	void SetEheight(float height);
	float GetExPos();
	void SetExPos(float xPos);
	float GetEyPos();
	void SetEyPos(float yPos);
	bool CollidesWith(Entity e);
	SDL_Color GetPixelColor(SDL_Surface * pSurface, const int X, const int Y);
	bool CollidesWithPrecise(Entity e);
	void Draw();
	SDL_Rect* GetSprite();
	SDL_Surface* GetSurface();
	void SetTexture(std::string str);
	SDL_Texture* GetTexture();
	float GetEwidth();
	void SetEwidth(float width);
	float GetEheight();
	//SDL Stuff
	SDL_Renderer* Erenderer;
	~Entity();

private:
	SDL_Rect EstretchRect;
	SDL_Surface* ESurface;
	SDL_Texture* ETexture;
	SDL_Rect* ESprite;
	//Entity Info
	float Ewidth;
	float Eheight;
	float ExPos;
	float EyPos;

};