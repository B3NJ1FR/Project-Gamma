#include "Buildings.h"


Buildings::Buildings()
{
	// Default initialisation
	this->size = { 0, 0 };
	this->enterPosition = { 0, 0 };
	this->exitPosition = { 0, 0 };

	this->moneyCostToBuild = RESET;
	this->timeToBuild = RESET;
	this->timeToProduce = RESET;

	this->numberRessourceNeeded = RESET;
	this->numberRessourceProduced = RESET;
}



Buildings::~Buildings()
{

}

void Buildings::SetName(const std::string _name)
{
	// Set the building name
	this->name = _name;
}

void Buildings::SetDescription(std::string _description)
{
	// Get the building description
	this->description = _description;
}

void Buildings::SetSizeCaracteristics(const sf::Vector2i _buildingSize)
{
	// Set the building size in term of width and lengh
	this->size = _buildingSize;
}


void Buildings::SetEntranceCaracteristics(const sf::Vector2i _enterPosition)
{
	// Set the building entry position
	this->enterPosition = _enterPosition;
}


void Buildings::SetExitCaracteristics(const sf::Vector2i _exitPosition)
{
	// Set the building exit position
	this->exitPosition = _exitPosition;
}


void Buildings::SetSprite(const std::string _spriteFileName, int _spriteOriginPosition)
{
	// Set the building sprite
	this->sprite = LoadSprite(_spriteFileName, _spriteOriginPosition);
}


void Buildings::SetIcon(const std::string _iconFileName, int _iconOriginPosition)
{
	// Set the icon sprite
	this->icon = LoadSprite(_iconFileName, _iconOriginPosition);
}

void Buildings::SetConstructionMoneyCost(const int _moneyCost)
{
	// Set the construction cost in term of money
	this->moneyCostToBuild = _moneyCost;
}


void Buildings::SetConstructionTimeCost(const float &_timeCost)
{
	// Set the construction cost in term of time necessary
	this->timeToBuild = _timeCost;
}

void Buildings::SetProductionTimeCost(const float &_timeCost)
{
	// Set the production cost in term of time for each ressource produced
	this->timeToProduce = _timeCost;
}

void Buildings::SetPickupingTimeCost(const float &_timeCost)
{
	// Set the construction cost in term of time necessary
	this->timeToPickUpRessources = _timeCost;
}

void Buildings::SetDepositingTimeCost(const float &_timeCost)
{
	// Set the production cost in term of time for each ressource produced
	this->timeToDeposeRessources = _timeCost;
}

void Buildings::SetRessourceIDNeeded(int *_resssourceID, const unsigned char _numberRessources)
{
	this->ressourceIDNeeded = _resssourceID;
	this->numberRessourceNeeded = _numberRessources;
}

void Buildings::SetRessourceQuantityNeeded(int *_resssourceQuantity)
{
	this->ressourceQuantityNeeded = _resssourceQuantity;
}

void Buildings::SetRessourceIDProduced(int *_resssourceID, const unsigned char _numberRessources)
{
	this->ressourceIDProduced = _resssourceID;
	this->numberRessourceProduced = _numberRessources;
}

void Buildings::SetRessourceQuantityProduced(int *_resssourceQuantity)
{
	this->ressourceQuantityProduced = _resssourceQuantity;
}

std::string Buildings::GetName()
{
	// Get the building name
	return this->name;
}

std::string Buildings::GetDescription()
{
	// Get the building description
	return this->description;
}

sf::Sprite Buildings::GetSprite()
{
	// Get the building sprite
	return this->sprite;
}

sf::Sprite Buildings::GetIcon()
{
	// Get the building icon
	return this->icon;
}

sf::Vector2i Buildings::GetSize()
{
	// Get the building size
	return this->size;
}

int Buildings::GetConstructionCost()
{
	// Get the building money cost
	return this->moneyCostToBuild;
}


float Buildings::GetConstructionTimeCost()
{
	// Get the building time cost
	return this->timeToBuild;
}

float Buildings::GetProductionTimeCost()
{
	// Get the production time cost
	return this->timeToProduce;
}

float Buildings::GetDepositingTimeCost()
{
	// Get the depositing cost in term of time
	return this->timeToDeposeRessources;
}

float Buildings::GetPickupingTimeCost()
{
	// Get the pickuping cost in term of time necessary
	return this->timeToPickUpRessources;
}

int Buildings::GetRessourceIDNeeded(const unsigned char &_value)
{
	if (_value - 1 >= 0 && _value - 1 < this->numberRessourceNeeded)
	{
		return this->ressourceIDNeeded[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}

int Buildings::GetRessourceQuantityNeeded(const unsigned char &_value)
{
	if (_value - 1 >= 0 && _value - 1 < this->numberRessourceNeeded)
	{
		return this->ressourceQuantityNeeded[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}

int Buildings::GetRessourceIDProduced(const unsigned char &_value)
{
	if (_value - 1 >= 0 && _value - 1 < this->numberRessourceProduced)
	{
		return this->ressourceIDProduced[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}

int Buildings::GetRessourceQuantityProduced(const unsigned char &_value)
{
	if (_value - 1 >= 0 && _value - 1 < this->numberRessourceProduced)
	{
		return this->ressourceQuantityProduced[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}
