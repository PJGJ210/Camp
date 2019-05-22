#include "Entity.h"
#include <string>
#include <iostream>
#include <algorithm>


Entity::Entity()
{
	Ewidth = 64;
	Eheight = 64;
	ExPos = rand() % 500;
	EyPos = rand() % 500;
}

Entity::Entity(float width, float height, float xPos, float yPos)
{
	Ewidth = width;
	Eheight = height;
	ExPos = xPos;
	EyPos = yPos;
}

float Entity::GetEwidth()
{
	return Ewidth;
}

void Entity::SetEwidth(float width)
{
	Ewidth = width;
}

float Entity::GetEheight()
{
	return Eheight;
}

void Entity::SetEheight(float height)
{
	Eheight = height;
}

float Entity::GetExPos()
{
	return ExPos;
}

void Entity::SetExPos(float xPos)
{
	ExPos = xPos;
}

float Entity::GetEyPos()
{
	return EyPos;
}

void Entity::SetEyPos(float yPos)
{
	EyPos = yPos;
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

	float Xx = ExPos;
	float Xy = ExPos + Ewidth;
	float Xw = e.ExPos;
	float Xz = e.ExPos + e.Ewidth;
	float Yx = EyPos;
	float Yy = EyPos + Eheight;
	float Yw = e.EyPos;
	float Yz = e.EyPos + e.Eheight;

	float XxOffset;
	float XwOffset;
	float YxOffset;
	float YwOffset;
	if (Xx <= Xw)
	{
		LeftBound = (int)Xw;
		XxOffset = 0;
		XwOffset = Xx - Xw;
		RightBound = (int)Xy;
	}
	else
	{
		LeftBound = (int)Xx;
		XxOffset = Xw - Xx;
		XwOffset = 0;
		RightBound = (int)Xz;
	}
	if (Yx <= Yw)
	{
		TopBound = (int)Yw;
		YxOffset = 0;
		YwOffset = Yx - Yw;
		BottomBound = (int)Yy;
	}
	else
	{
		TopBound = (int)Yx;
		YwOffset = Yw - Yx;
		YxOffset = 0;
		BottomBound = (int)Yz;
	}

	for (int i = LeftBound; i < RightBound; i++)
	{
		for (int j = TopBound; j < BottomBound; j++)
		{
			/*int ColorA = GetPixelColor(ESurface, (int)XxOffset + (RightBound - i), (int)YxOffset + (BottomBound - j)).a;
			int ColorB = e.GetPixelColor(e.GetSurface(), (int)XwOffset + (RightBound - i), (int)YwOffset + (BottomBound - j)).a;
			if (ColorA != 0 && ColorB != 0)
			{
				return true;
			}*/
		}
	}
	return false;
}

Entity::~Entity()
{
}
