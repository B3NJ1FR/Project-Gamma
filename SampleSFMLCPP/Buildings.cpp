#include "Buildings.h"


Buildings::Buildings()
{
	// Default initialisation
	size = { 0, 0 };
	enterPosition = { 0, 0 };
	exitPosition = { 0, 0 };

	id = RESET;

	moneyCostToBuild = RESET;
	timeToBuild = RESET;
	timeToProduce = RESET;

	timeToPickUpRessources = RESET;
	timeToDeposeRessources = RESET;

	numberRessourceNeeded = RESET;
	numberRessourceProduced = RESET;

	ressourceIDNeeded = nullptr;
	ressourceQuantityNeeded = nullptr;

	ressourceIDProduced = nullptr;
	ressourceQuantityProduced = nullptr;
}



Buildings::~Buildings()
{

}

void Buildings::SetName(const std::string _name)
{
	// Set the building name
	name = _name;
}

void Buildings::SetDescription(std::string _description)
{
	// Get the building description
	description = _description;
}

void Buildings::SetSizeCaracteristics(const sf::Vector2i _buildingSize)
{
	// Set the building size in term of width and lengh
	size = _buildingSize;
}


void Buildings::SetEntranceCaracteristics(const sf::Vector2i _enterPosition)
{
	// Set the building entry position
	enterPosition = _enterPosition;
}


void Buildings::SetExitCaracteristics(const sf::Vector2i _exitPosition)
{
	// Set the building exit position
	exitPosition = _exitPosition;
}


void Buildings::SetSprite(const std::string _spriteFileName, int _spriteOriginPosition)
{
	// Set the building sprite
	sprite = LoadSprite(_spriteFileName, _spriteOriginPosition);
}


void Buildings::SetIcon(const std::string _iconFileName, int _iconOriginPosition)
{
	// Set the icon sprite
	icon = LoadSprite(_iconFileName, _iconOriginPosition);
}

void Buildings::SetConstructionMoneyCost(const int _moneyCost)
{
	// Set the construction cost in term of money
	moneyCostToBuild = _moneyCost;
}


void Buildings::SetConstructionTimeCost(const float &_timeCost)
{
	// Set the construction cost in term of time necessary
	timeToBuild = _timeCost;
}

void Buildings::SetProductionTimeCost(const float &_timeCost)
{
	// Set the production cost in term of time for each ressource produced
	timeToProduce = _timeCost;
}

void Buildings::SetPickupingTimeCost(const float &_timeCost)
{
	// Set the construction cost in term of time necessary
	timeToPickUpRessources = _timeCost;
}

void Buildings::SetDepositingTimeCost(const float &_timeCost)
{
	// Set the production cost in term of time for each ressource produced
	timeToDeposeRessources = _timeCost;
}

void Buildings::SetRessourceIDNeeded(int *_resssourceID, const unsigned char &_numberRessources)
{
	if (ressourceIDNeeded != nullptr)
	{
		delete ressourceIDNeeded;
		ressourceIDNeeded = nullptr;
	}

	numberRessourceNeeded = _numberRessources;

	ressourceIDNeeded = _resssourceID;
}

void Buildings::SetRessourceQuantityNeeded(int *_resssourceQuantity)
{
	if (ressourceQuantityNeeded != nullptr)
	{
		delete ressourceQuantityNeeded;
		ressourceQuantityNeeded = nullptr;
	}

	ressourceQuantityNeeded = _resssourceQuantity;
}

void Buildings::SetRessourceIDProduced(int *_resssourceID, const unsigned char &_numberRessources)
{
	if (ressourceIDProduced != nullptr)
	{
		delete ressourceIDProduced;
		ressourceIDProduced = nullptr;
	}

	numberRessourceProduced = _numberRessources;

	ressourceIDProduced = _resssourceID;
}

void Buildings::SetRessourceQuantityProduced(int *_resssourceQuantity)
{
	if (ressourceQuantityProduced != nullptr)
	{
		delete ressourceQuantityProduced;
		ressourceQuantityProduced = nullptr;
	}

	ressourceQuantityProduced = _resssourceQuantity;
}



int Buildings::GetRessourceIDNeeded(const unsigned char &_value) const
{
	if (_value - 1 >= 0 && _value - 1 < numberRessourceNeeded)
	{
		return ressourceIDNeeded[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}

int Buildings::GetRessourceQuantityNeeded(const unsigned char &_value) const
{
	if (_value - 1 >= 0 && _value - 1 < numberRessourceNeeded)
	{
		return ressourceQuantityNeeded[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}

int Buildings::GetRessourceIDProduced(const unsigned char &_value) const
{
	if (_value - 1 >= 0 && _value - 1 < numberRessourceProduced)
	{
		return ressourceIDProduced[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}

int Buildings::GetRessourceQuantityProduced(const unsigned char &_value) const
{
	if (_value - 1 >= 0 && _value - 1 < numberRessourceProduced)
	{
		return ressourceQuantityProduced[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}
