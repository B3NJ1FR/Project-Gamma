#include "Ressources.h"


Ressources::Ressources()
{
	this->quantityOwned = RESET;
	this->name = "";
}

Ressources::~Ressources()
{

}


void Ressources::SetSprite(const std::string _spriteFileName, int _spriteOriginPosition)
{
	this->sprite = LoadSprite(_spriteFileName, _spriteOriginPosition);
}

void Ressources::SetName(const std::string _name)
{
	this->name = _name;
}

void Ressources::SetInitialQuantityOwned(int _quantityOwned)
{
	this->quantityOwned = _quantityOwned;
}

// Revoir sous forme d'opérateurs ?
void Ressources::AddQuantityOwned(int _quantityToAdd)
{
	this->quantityOwned += _quantityToAdd;
}

void Ressources::SubtractQuantityOwned(int _quantityToSubtract)
{
	this->quantityOwned -= _quantityToSubtract;
}


sf::Sprite Ressources::GetSprite()
{
	return this->sprite;
}

std::string Ressources::GetName()
{
	return this->name;
}

int Ressources::GetQuantityOwned()
{
	return this->quantityOwned;
}