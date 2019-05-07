#include "Entity.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <algorithm>


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

SDL_Surface* Entity::GetSurface()
{
	return ESurface;
}

void Entity::SetTexture(std::string str)
{
	//Load image at specified path
	ESurface = IMG_Load(str.c_str());
	ETexture = IMG_LoadTexture(Erenderer, str.c_str());
}

SDL_Texture* Entity::GetTexture()
{
	return ETexture;
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

SDL_Color Entity::GetPixelColor(SDL_Surface* pSurface, const int X, const int Y)
{
	int      Bpp = pSurface->format->BytesPerPixel;
	Uint8*   pPixel = (Uint8*)pSurface->pixels + Y * pSurface->pitch + X * Bpp;

	Uint32 PixelColor = *(Uint32*)pPixel;

	SDL_Color Color = { 0, 0, 0, 0 };

	SDL_GetRGBA(PixelColor, pSurface->format, &Color.r, &Color.g, &Color.b, &Color.a);

	return Color;
}

bool Entity::CollidesWith(Entity e)
{
	if ((ExPos <= e.ExPos && e.ExPos <= ExPos + Ewidth) || (ExPos <= e.ExPos + e.Ewidth && e.ExPos + e.Ewidth <= ExPos + Ewidth))
	{
		if ((EyPos <= e.EyPos && e.EyPos <= EyPos + Eheight) || (EyPos <= e.EyPos + e.Eheight && e.EyPos + e.Eheight <= EyPos + Eheight))
		{
			return CollidesWithPrecise(e);
		}
	}
	return false;
}

bool Entity::CollidesWithPrecise(Entity e)
{
	int LeftBound;
	int RightBound;
	int TopBound;
	int BottomBound;

	int Xx = ExPos;
	int Xy = ExPos + Ewidth;
	int Xw = e.ExPos;
	int Xz = e.ExPos + e.Ewidth;
	int Yx = EyPos;
	int Yy = EyPos + Eheight;
	int Yw = e.EyPos;
	int Yz = e.EyPos + e.Eheight;

	int XxOffset;
	int XwOffset;
	int YxOffset;
	int YwOffset;
	if (Xx <= Xw)
	{
		LeftBound = Xw;
		XxOffset = 0;
		XwOffset = Xx - Xw;
		RightBound = Xy;
	}
	else
	{
		LeftBound = Xx;
		XxOffset = Xw - Xx;
		XwOffset = 0;
		RightBound = Xz;
	}
	if (Yx <= Yw)
	{
		TopBound = Yw;
		YxOffset = 0;
		YwOffset = Yx - Yw;
		BottomBound = Yy;
	}
	else
	{
		TopBound = Yx;
		YwOffset = Yw - Yx;
		YxOffset = 0;
		BottomBound = Yz;
	}

	for (int i = LeftBound; i < RightBound; i++)
	{
		for (int j = TopBound; j < BottomBound; j++)
		{
			int ColorA = GetPixelColor(ESurface, XxOffset + (RightBound - i), YxOffset + (BottomBound - j)).a;
			int ColorB = e.GetPixelColor(e.GetSurface(), XwOffset + (RightBound - i), YwOffset + (BottomBound - j)).a;
			if (ColorA != 0 && ColorB != 0)
			{
				return true;
			}
		}
	}
	return false;
}

void Entity::Draw()
{
	EstretchRect.w = Ewidth;
	EstretchRect.h = Eheight;
	EstretchRect.x = ExPos;
	EstretchRect.y = EyPos;
	SDL_RenderCopy(Erenderer, ETexture, NULL, &EstretchRect);
}

Entity::~Entity()
{
}
