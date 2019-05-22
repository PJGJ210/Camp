#include "Player.h"
#include <algorithm>

Player::Player()
{
	Level = 1;
	Experience = 0;
	Alive = true;
	//stats
	MaxHealth = 100;
	Health = 50;
	MaxMana = 100;
	Mana = 18;
	Strength = 10;
	Intelligence = 10;
	Dexterity = 10;
}

void Player::HealHealth(int amount)
{
	Health += amount;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

void Player::DamageHealth(int amount)
{
	Health -= amount;
	if (Health < 0)
	{
		Health = 0;
		Alive = false;
	}
}

void Player::HealMana(int amount)
{
	Mana += amount;
	if (Mana > MaxMana)
	{
		Mana = MaxMana;
	}
}

void Player::DamageMana(int amount)
{
	Mana -= amount;
	if (Mana < 0)
	{
		Mana = 0;
	}
}

void Player::Update()
{

}

Player::~Player()
{
}