#include "Purchasers.h"


Purchasers::Purchasers()
{
	this->actualStatus = IDLE;

	this->isPreviousOfferHasBeenRefused = false;

	std::string temporaryString;
	temporaryString.erase();

	int temporaryNumber(RESET);
	

	std::ifstream purchaserFile("Data/Configurations/Purchasers.data", std::ios::in);

	if (!purchaserFile.is_open())
	{
		std::cout << "Error accessing Purchasers.data file" << std::endl;

		exit(EXIT_FAILURE);		
	}


	while (!purchaserFile.eof())
	{
		purchaserFile >> temporaryString;

		if (temporaryString == "PROVENANCE")
		{
			purchaserFile >> this->provenance;
		}

		if (temporaryString == "MONEY")
		{
			purchaserFile >> this->minimalMoneyValueForRessource >> this->maximalMoneyValueForRessource;
		}

		if (temporaryString == "QUANTITY")
		{
			purchaserFile >> this->minimalRessourceQuantity >> this->maximalRessourceQuantity;
		}

		if (temporaryString == "RESSOURCE")
		{
			purchaserFile >> temporaryNumber;
			this->ressourceID = (enum TypesOfRessources)temporaryNumber;
		}

		if (temporaryString == "MOVEMENT")
		{
			purchaserFile >> this->minimalMovementTime >> this->maximalMovementTime;
		}
	}

	purchaserFile.close();
	
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
	if (this->isPreviousOfferHasBeenRefused == true)
	{
		this->isPreviousOfferHasBeenRefused = false;

		if (this->minimalMoneyValueForRessource + 1 < this->maximalMoneyValueForRessource)
		{
			this->minimalMoneyValueForRessource += 1;
		}

		return sf::Vector2i(this->minimalMoneyValueForRessource, this->maximalMoneyValueForRessource);
	}
	else
	{
		this->isPreviousOfferHasBeenRefused = false;

		return sf::Vector2i(this->minimalMoneyValueForRessource, this->maximalMoneyValueForRessource);
	}
}

sf::Vector2i Purchasers::GetUnitQuantityRessourceScope()
{
	return sf::Vector2i(this->minimalRessourceQuantity, this->maximalRessourceQuantity);
}

void Purchasers::IsOfferHasBeenRefused()
{
	this->isPreviousOfferHasBeenRefused = true;
}