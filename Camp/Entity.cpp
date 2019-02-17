#include "Entity.h"
#include <string>
#include <iostream>


Entity::Entity()
{
	Ewidth = 64;
	Eheight = 64;
	ExPos = 100;
	EyPos = 100;
}

Entity::Entity(SDL_Renderer* renderer)
{
	Erenderer = renderer;
	Ewidth = 64;
	Eheight = 64;
	ExPos = 100;
	EyPos = 100;
}

Entity::Entity(SDL_Renderer* renderer, int width, int height, int xPos, int yPos)
{
	Erenderer = renderer;
	Ewidth = width;
	Eheight = height;
	ExPos = xPos;
	EyPos = yPos;
}

void Entity::SetRenderer(SDL_Renderer* renderer)
{
	Erenderer = renderer;
}

SDL_Rect* Entity::GetSprite()
{
	return ESprite;
}

void Entity::SetSprite(SDL_Texture* Spritesheet)
{
	ESpriteSheet = Spritesheet;
}

SDL_Texture* Entity::GetSpriteSheet()
{
	return ESpriteSheet;
}

int Entity::GetEwidth()
{
	return Ewidth;
}

void Entity::WetEwidth(int width)
{
	Ewidth = width;
}

int Entity::GetEheight()
{
	return Eheight;
}

void Entity::SetEheight(int height)
{
	Eheight = height;
}

int Entity::GetExPos()
{
	return ExPos;
}

void Entity::SetExPos(int xPos)
{
	ExPos = xPos;
}

int Entity::GetEyPos()
{
	return EyPos;
}

void Entity::SetEyPos(int yPos)
{
	EyPos = yPos;
}

void Entity::Draw()
{
	EstretchRect.w = Ewidth;
	EstretchRect.h = Eheight;
	EstretchRect.x = ExPos;
	EstretchRect.y = EyPos;
	SDL_RenderCopy(Erenderer, ESpriteSheet, NULL, &EstretchRect);
}

Entity::~Entity()
{
}
