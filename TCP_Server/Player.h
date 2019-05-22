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

	int PlayerID;
	int Level;
	int Experience;

	//stats
	int MaxHealth;
	int Health;
	int MaxMana;
	int Mana;
	int Strength;
	int Intelligence;
	int Dexterity;

};

