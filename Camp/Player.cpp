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

void Player::Draw()
{
	Entity::Draw();

	//Draw InfoRect
	InfoRect.w = (int)GetEwidth();
	InfoRect.h = 2 * ((int)GetEheight() / 10);
	InfoRect.x = (int)GetExPos();
	InfoRect.y = (int)GetEyPos() - (InfoRect.h);
	SDL_SetRenderDrawColor(Erenderer, 0, 0, 0, 0);
	SDL_RenderFillRect(Erenderer, &InfoRect);

	//Draw Health
	HealthRect.w = std::min((int)GetEwidth(), (int)(GetEwidth() * ((double)Health / (double)MaxHealth)));
	HealthRect.h = (int)GetEheight() / 10;
	HealthRect.x = (int)GetExPos();
	HealthRect.y = (int)GetEyPos() - (HealthRect.h * 2);
	SDL_SetRenderDrawColor(Erenderer, 0, 255, 0, 0);
	SDL_RenderFillRect(Erenderer, &HealthRect);

	//Draw Mana
	ManaRect.w = std::min((int)GetEwidth(), (int)(GetEwidth() * ((double)Mana / (double)MaxMana)));
	ManaRect.h = (int)GetEheight() / 10;
	ManaRect.x = (int)GetExPos();
	ManaRect.y = (int)GetEyPos() - ManaRect.h;
	SDL_SetRenderDrawColor(Erenderer, 0, 0, 255, 0);
	SDL_RenderFillRect(Erenderer, &ManaRect);
}

void Player::DrawInfo()
{
	HealthRect.w = (int)GetEwidth();
	HealthRect.h = (int)GetEwidth() / 10;
	HealthRect.x = (int)GetExPos();
	HealthRect.y = (int)GetEyPos() + HealthRect.h;
	SDL_RenderDrawRect(Erenderer, &HealthRect);
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

