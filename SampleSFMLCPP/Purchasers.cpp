#include "Purchasers.h"


Purchasers::Purchasers()
{
	this->actualStatus = IDLE;
	this->path = nullptr;
	
	this->minimalMoneyValueForRessource = RESET;
	this->maximalMoneyValueForRessource = RESET;

	this->minimalRessourceQuantity = RESET;
	this->actualRessourceQuantity = RESET;
	this->maximalRessourceQuantity = RESET;

	this->ressourceID = AMPHORA_OF_WINE;

	this->minimalMovementTime = RESET;
	this->maximalMovementTime = RESET;

	this->isPreviousOfferHasBeenRefused = false;
}

Purchasers::~Purchasers()
{

}

void Purchasers::Initialisation(const int &_actualQuantity)
{
	this->actualRessourceQuantity = _actualQuantity;

	this->isPreviousOfferHasBeenRefused = false;

	std::string temporaryString;
	temporaryString.erase();
		

	std::ifstream purchaserFile("Data/Configurations/Purchasers.data", std::ios::in);

	if (!purchaserFile.is_open())
	{
		std::cout << "Error accessing Purchasers.data file" << std::endl;

		exit(EXIT_FAILURE);		
	}


	while (!purchaserFile.eof())
	{
		purchaserFile >> temporaryString;

		// Reading of his provenance
		if (temporaryString == "PROVENANCE")
		{
			int temporaryCounter(RESET);
			int randomResult(RESET);

			purchaserFile >> temporaryCounter;

			randomResult = rand() % temporaryCounter;

			//std::cout << "Provenance numero : " << randomResult << "/" << temporaryCounter << " ";

			for (int i = 0; i < temporaryCounter; i++)
			{
				if (i == randomResult)
				{
					purchaserFile >> this->provenance;
				}
				else
				{
					purchaserFile >> temporaryString;
				}
			}

		}

		// Reading of the price which he would sells
		if (temporaryString == "MONEY")
		{
			purchaserFile >> this->minimalMoneyValueForRessource >> this->maximalMoneyValueForRessource;
		}

		// Reading of the quantities which he would sells
		if (temporaryString == "QUANTITY")
		{
			sf::Vector2i temporaryMinimalRessourcesQuantity;
			sf::Vector2i temporaryMaximalRessourcesQuantity;

			this->minimalRessourceQuantity = RESET;
			this->maximalRessourceQuantity = RESET;

			purchaserFile >> temporaryMinimalRessourcesQuantity.x >> temporaryMinimalRessourcesQuantity.y >> temporaryMaximalRessourcesQuantity.x >> temporaryMaximalRessourcesQuantity.y;

			this->minimalRessourceQuantity = RandomiseData(temporaryMinimalRessourcesQuantity);
			
			unsigned short counter(RESET);

			while (this->minimalRessourceQuantity > this->actualRessourceQuantity)
			{
				if (counter >= 5)
				{
					if (this->actualRessourceQuantity - 10 >= 0)
					{
						this->minimalRessourceQuantity = this->actualRessourceQuantity - (rand() % 10);
					}
					else
					{
						this->minimalRessourceQuantity = this->actualRessourceQuantity;
					}
				}
				else
				{
					this->minimalRessourceQuantity = RandomiseData(temporaryMinimalRessourcesQuantity);
				}

				counter++;
			}


			this->maximalRessourceQuantity = RandomiseData(temporaryMaximalRessourcesQuantity);		
		}

		if (temporaryString == "RESSOURCE")
		{
			int temporaryNumber(RESET);

			purchaserFile >> temporaryNumber;
			this->ressourceID = (enum TypesOfRessources)temporaryNumber;
		}

		if (temporaryString == "MOVEMENT")
		{
			purchaserFile >> this->minimalMovementTime >> this->maximalMovementTime;
		}
	}

	purchaserFile.close();
	
	std::cout << "New merchant created\n\n";
}


int Purchasers::RandomiseData(const sf::Vector2i &_data)
{
	return rand() % (_data.y - _data.x + 1) + _data.x;
}


int Purchasers::TimeToTravel()
{
	return (rand() % (this->maximalMovementTime - this->minimalMovementTime + 1) + this->minimalMovementTime);
}

void Purchasers::SetActualQuantityStored(const int &_quantity)
{
	this->actualRessourceQuantity = _quantity;
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

std::string Purchasers::GetProvenanceName()
{
	return this->provenance;
}

void Purchasers::IsOfferHasBeenRefused()
{
	this->isPreviousOfferHasBeenRefused = true;
}


void Purchasers::SavingPurchasersForFile(std::ofstream *_file)
{
	_file->write((char *)&this->actualStatus, sizeof(enum WorkerStatus));
	
	_file->write((char *)&this->provenance, sizeof(std::string));

	_file->write((char *)&this->minimalMoneyValueForRessource, sizeof(int));
	_file->write((char *)&this->maximalMoneyValueForRessource, sizeof(int));


	_file->write((char *)&this->minimalRessourceQuantity, sizeof(int));
	_file->write((char *)&this->actualRessourceQuantity, sizeof(int));
	_file->write((char *)&this->actualRessourceQuantity, sizeof(int));
	

	_file->write((char *)&this->ressourceID, sizeof(enum TypesOfRessources));


	_file->write((char *)&this->minimalMovementTime, sizeof(int));
	_file->write((char *)&this->maximalMovementTime, sizeof(int));


	_file->write((char *)&this->isPreviousOfferHasBeenRefused, sizeof(bool));
}



void Purchasers::LoadingPurchasersFromFile(std::ifstream *_file)
{
	_file->read((char *)&this->actualStatus, sizeof(enum WorkerStatus));

	_file->read((char *)&this->provenance, sizeof(std::string));

	_file->read((char *)&this->minimalMoneyValueForRessource, sizeof(int));
	_file->read((char *)&this->maximalMoneyValueForRessource, sizeof(int));


	_file->read((char *)&this->minimalRessourceQuantity, sizeof(int));
	_file->read((char *)&this->actualRessourceQuantity, sizeof(int));
	_file->read((char *)&this->actualRessourceQuantity, sizeof(int));


	_file->read((char *)&this->ressourceID, sizeof(enum TypesOfRessources));


	_file->read((char *)&this->minimalMovementTime, sizeof(int));
	_file->read((char *)&this->maximalMovementTime, sizeof(int));


	_file->read((char *)&this->isPreviousOfferHasBeenRefused, sizeof(bool));
}