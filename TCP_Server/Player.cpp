#include "Player.h"
#include <algorithm>

Player::Player()
{
	Entity::Entity();
	PlayerID = 0;
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

void Player::SetX(int x)
{
	this->SetExPos((float)x);
}

void Player::SetY(int y)
{
	this->SetEyPos((float)y);
}

void Player::SetHP(int hp)
{
	this->Health = hp;
}

void Player::SetMP(int mp)
{
	this->Mana = mp;
}

void Player::SetID(int id)
{
	this->PlayerID = id;
}

float Player::GetX()
{
	return (float)this->GetExPos();
}

float Player::GetY()
{
	return (float)this->GetEyPos();
}

int Player::GetHP()
{
	return this->Health;
}

int Player::GetMP()
{
	return this->Mana;
}

void Player::Update()
{

}

Player::~Player()
{
}