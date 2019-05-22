#pragma once
#include <string>

class Entity
{
public:
	Entity();
	Entity(float width, float height, float xPos, float yPos);
	//Tile Sheets may not be needed
	int MoveClips[16];
	int AttackClips[16];

	void SetEheight(float height);
	float GetExPos();
	void SetExPos(float xPos);
	float GetEyPos();
	void SetEyPos(float yPos);
	bool CollidesWith(Entity e);
	bool CollidesWithPrecise(Entity e);
	float GetEwidth();
	void SetEwidth(float width);
	float GetEheight();
	~Entity();

private:
	//Entity Info
	float Ewidth;
	float Eheight;
	float ExPos;
	float EyPos;
};

