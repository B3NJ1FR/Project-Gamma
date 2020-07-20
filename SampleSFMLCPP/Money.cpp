#include "Money.h"

Money::Money()
{
	this->playerMoney = 4450;

	this->sprite = LoadSprite("Data/Assets/Sprites/Menu/sesterce.png", 1);
}

Money::~Money()
{

}

void Money::AddMoney(int _quantityToAdd)
{
	this->playerMoney += _quantityToAdd;
}

void Money::SubtractMoney(int _quantityToRemove)
{
	this->playerMoney -= _quantityToRemove;
}

sf::Sprite Money::GetSprite()
{
	return this->sprite;
}

int Money::GetMoneyQuantity()
{
	return this->playerMoney;
}