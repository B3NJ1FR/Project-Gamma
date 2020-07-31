#include "Purchasers.h"


Purchasers::Purchasers()
{
	this->actualStatus = IDLE;

	this->provenance = "Nemausus";

	this->minimalMoneyValueForRessource = 1;
	this->maximalMoneyValueForRessource = 4;


	this->minimalRessourceQuantity = 20;
	this->maximalRessourceQuantity = 100;

	this->ressourceID = AMPHORA_OF_WINE;
		
	this->minimalMovementTime = 5;
	this->maximalMovementTime = 30;

	this->isPreviousOfferHasBeenRefused = false;
	
	std::cout << "Acheteur initialisé\n\n";
}


Purchasers::~Purchasers()
{

}


int Purchasers::TimeToTravel()
{
	return (rand() % (this->maximalMovementTime - this->minimalMovementTime + 1) + this->minimalMovementTime);
}


sf::Vector2i Purchasers::GetUnitPriceScope()
{
	return sf::Vector2i(this->minimalMoneyValueForRessource, this->maximalMoneyValueForRessource);
}

sf::Vector2i Purchasers::GetUnitQuantityRessourceScope()
{
	return sf::Vector2i(this->minimalRessourceQuantity, this->maximalRessourceQuantity);
}