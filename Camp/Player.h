#pragma once
#include "Entity.h"
class Player : public Entity
{
public:
	Player();
	~Player();
	void Draw();
	void HealHealth(int amount);
	void DamageHealth(int amount);
	void HealMana(int amount);

	void DamageMana(int amount);

	bool Alive;

private:

	void DrawInfo();
	void Update();

	void Die();

	int Level;
	int Experience;

	//stats
	SDL_Rect InfoRect;
	int MaxHealth;
	int Health;
	SDL_Rect HealthRect;
	int MaxMana;
	int Mana;
	SDL_Rect ManaRect;
	int Strength;
	int Intelligence;
	int Dexterity;

	
};

