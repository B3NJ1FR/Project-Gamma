#include "Purchasers.h"


Purchasers::Purchasers()
{
	m_actualStatus = IDLE;
	m_path = nullptr;
	
	m_minimalMoneyValueForRessource = RESET;
	m_maximalMoneyValueForRessource = RESET;

	m_minimalRessourceQuantity = RESET;
	m_actualRessourceQuantity = RESET;
	m_maximalRessourceQuantity = RESET;

	m_ressourceID = AMPHORA_OF_WINE;

	m_minimalMovementTime = RESET;
	m_maximalMovementTime = RESET;

	m_isPreviousOfferHasBeenRefused = false;
}

Purchasers::~Purchasers()
{

}

void Purchasers::SetActualQuantityStored(const int &_quantity)
{
	m_actualRessourceQuantity = _quantity;
}


sf::Vector2i Purchasers::GetUnitPriceScope()
{
	if (m_isPreviousOfferHasBeenRefused == true)
	{
		m_isPreviousOfferHasBeenRefused = false;

		if (m_minimalMoneyValueForRessource + 1 < m_maximalMoneyValueForRessource)
		{
			m_minimalMoneyValueForRessource += 1;
		}

		return sf::Vector2i(m_minimalMoneyValueForRessource, m_maximalMoneyValueForRessource);
	}
	else
	{
		m_isPreviousOfferHasBeenRefused = false;

		return sf::Vector2i(m_minimalMoneyValueForRessource, m_maximalMoneyValueForRessource);
	}
}


void Purchasers::Initialisation(const int& _actualQuantity)
{
	m_actualRessourceQuantity = _actualQuantity;

	m_isPreviousOfferHasBeenRefused = false;

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
					purchaserFile >> m_provenance;
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
			purchaserFile >> m_minimalMoneyValueForRessource >> m_maximalMoneyValueForRessource;
		}

		// Reading of the quantities which he would sells
		if (temporaryString == "QUANTITY")
		{
			sf::Vector2i temporaryMinimalRessourcesQuantity;
			sf::Vector2i temporaryMaximalRessourcesQuantity;

			m_minimalRessourceQuantity = RESET;
			m_maximalRessourceQuantity = RESET;

			purchaserFile >> temporaryMinimalRessourcesQuantity.x >> temporaryMinimalRessourcesQuantity.y >> temporaryMaximalRessourcesQuantity.x >> temporaryMaximalRessourcesQuantity.y;

			m_minimalRessourceQuantity = RandomiseData(temporaryMinimalRessourcesQuantity);

			unsigned short counter(RESET);

			while (m_minimalRessourceQuantity > m_actualRessourceQuantity)
			{
				if (counter >= 5)
				{
					if (m_actualRessourceQuantity - 10 >= 0)
					{
						m_minimalRessourceQuantity = m_actualRessourceQuantity - (rand() % 10);
					}
					else
					{
						m_minimalRessourceQuantity = m_actualRessourceQuantity;
					}
				}
				else
				{
					m_minimalRessourceQuantity = RandomiseData(temporaryMinimalRessourcesQuantity);
				}

				counter++;
			}


			m_maximalRessourceQuantity = RandomiseData(temporaryMaximalRessourcesQuantity);
		}

		if (temporaryString == "RESSOURCE")
		{
			int temporaryNumber(RESET);

			purchaserFile >> temporaryNumber;
			m_ressourceID = (enum TypesOfRessources)temporaryNumber;
		}

		if (temporaryString == "MOVEMENT")
		{
			purchaserFile >> m_minimalMovementTime >> m_maximalMovementTime;
		}
	}

	purchaserFile.close();

	std::cout << "New merchant created\n\n";
}


int Purchasers::RandomiseData(const sf::Vector2i& _data)
{
	return rand() % (_data.y - _data.x + 1) + _data.x;
}


int Purchasers::TimeToTravel()
{
	return (rand() % (m_maximalMovementTime - m_minimalMovementTime + 1) + m_minimalMovementTime);
}

void Purchasers::SavingPurchasersForFile(std::ofstream *_file)
{
	_file->write((char *)&m_actualStatus, sizeof(enum WorkerStatus));
	
	_file->write((char *)&m_provenance, sizeof(std::string));

	_file->write((char *)&m_minimalMoneyValueForRessource, sizeof(int));
	_file->write((char *)&m_maximalMoneyValueForRessource, sizeof(int));


	_file->write((char *)&m_minimalRessourceQuantity, sizeof(int));
	_file->write((char *)&m_actualRessourceQuantity, sizeof(int));
	_file->write((char *)&m_actualRessourceQuantity, sizeof(int));
	

	_file->write((char *)&m_ressourceID, sizeof(enum TypesOfRessources));


	_file->write((char *)&m_minimalMovementTime, sizeof(int));
	_file->write((char *)&m_maximalMovementTime, sizeof(int));


	_file->write((char *)&m_isPreviousOfferHasBeenRefused, sizeof(bool));
}



void Purchasers::LoadingPurchasersFromFile(std::ifstream *_file)
{
	_file->read((char *)&m_actualStatus, sizeof(enum WorkerStatus));

	_file->read((char *)&m_provenance, sizeof(std::string));

	_file->read((char *)&m_minimalMoneyValueForRessource, sizeof(int));
	_file->read((char *)&m_maximalMoneyValueForRessource, sizeof(int));


	_file->read((char *)&m_minimalRessourceQuantity, sizeof(int));
	_file->read((char *)&m_actualRessourceQuantity, sizeof(int));
	_file->read((char *)&m_actualRessourceQuantity, sizeof(int));


	_file->read((char *)&m_ressourceID, sizeof(enum TypesOfRessources));


	_file->read((char *)&m_minimalMovementTime, sizeof(int));
	_file->read((char *)&m_maximalMovementTime, sizeof(int));


	_file->read((char *)&m_isPreviousOfferHasBeenRefused, sizeof(bool));
}