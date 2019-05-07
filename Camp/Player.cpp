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
	InfoRect.w = GetEwidth();
	InfoRect.h = 2 * (GetEheight() / 10);
	InfoRect.x = GetExPos();
	InfoRect.y = GetEyPos() - (InfoRect.h);
	SDL_SetRenderDrawColor(Erenderer, 0, 0, 0, 0);
	SDL_RenderFillRect(Erenderer, &InfoRect);

	//Draw Health
	HealthRect.w = std::min(GetEwidth(), (int)(GetEwidth() * ((double)Health / (double)MaxHealth)));
	HealthRect.h = GetEheight() / 10;
	HealthRect.x = GetExPos();
	HealthRect.y = GetEyPos() - (HealthRect.h * 2);
	SDL_SetRenderDrawColor(Erenderer, 0, 255, 0, 0);
	SDL_RenderFillRect(Erenderer, &HealthRect);

	//Draw Mana
	ManaRect.w = std::min(GetEwidth(), (int)(GetEwidth() * ((double)Mana / (double)MaxMana)));
	ManaRect.h = GetEheight() / 10;
	ManaRect.x = GetExPos();
	ManaRect.y = GetEyPos() - ManaRect.h;
	SDL_SetRenderDrawColor(Erenderer, 0, 0, 255, 0);
	SDL_RenderFillRect(Erenderer, &ManaRect);
}

void Player::DrawInfo()
{
	HealthRect.w = GetEwidth();
	HealthRect.h = GetEwidth() / 10;
	HealthRect.x = GetExPos();
	HealthRect.y = GetEyPos() + HealthRect.h;
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

