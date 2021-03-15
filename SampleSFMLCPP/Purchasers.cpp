#include "Purchasers.h"
#include "GameDefinitions.h"


Purchasers::Purchasers()
{
	m_actualStatus = PurchaserStatus::IDLE;
	m_path = nullptr;

	m_storage = new Storage();
	m_storage->SetName("Purchaser");
	m_storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(AMPHORA_OF_WINE), ResourceData::RESOURCE_NEEDED_N_PRODUCED);
	
	m_minimalMoneyValueForRessource = RESET;
	m_maximalMoneyValueForRessource = RESET;

	m_minimalRessourceQuantity = RESET;
	m_actualRessourceQuantity = RESET;
	m_maximalRessourceQuantity = RESET;

	m_ressourceID = AMPHORA_OF_WINE;

	m_counterOfRefusedOffers = 0;
	m_isPreviousOfferHasBeenRefused = false;

	m_isStallExist = false;
	m_isCanLeaveTheMap = false;
	m_isWaitingEndTheMap = false;
}

Purchasers::~Purchasers()
{
	std::cout << "Clear Purchasers\n";
	ClearStorage();
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

void Purchasers::InitialisationFromDataFile()
{
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
	}

	purchaserFile.close();
}

void Purchasers::Initialisation(Stalls* _stall)
{
	// A Faire : 
	// O - spawn l'entité à tel ou tel coordonnees
	// O - regarder s'il y a une stall de construite, si c'est le cas :
	//		- lancer le path finding jusque devant le domaine
	//		(soit devant le plus haut niveau de chemin débouchant sur la route, 
	//		soit devant une zone arbitraire)
	//		- lancer l'animation et l'affichage de la selling window une fois ces actions de fini
	// O - sinon :
	//		- lancer le path finding jusqu'à la fin de la route

	m_isPreviousOfferHasBeenRefused = false;

	InitialisationFromDataFile();

	// Spawn position
	// A MODIFIER AVEC UNE VERSION QUI LE FAIT VENIR
	// DE LA BONNE DIRECTION EN FONCTION DE LA PROVENANCE
	m_mapPosition = sf::Vector2f(3, 6);

	if (_stall->GetConstructionStatus() == BuildingStatus::BUILT)
	{
		m_isStallExist = true;
		m_mapEndPosition = FindWherePurchaserMustStopItself(_stall->GetMapPosition());
		m_isLauchingMovement = true;
		InitPathfinding();
	}
	else
	{
		m_isStallExist = false;
		m_isCanLeaveTheMap = true;
	}

	std::cout << "New merchant created\n\n";
}

void Purchasers::InitPathfinding()
{
	if (m_isLauchingMovement == true)
	{
		if (m_path != nullptr)
		{
			delete m_path;
			m_path = nullptr;
		}

		m_path = new Pathfinding;

		m_path->InitMapCopyPathfinding(FIRST_FLOOR + COLLISIONS_ID); // TEMPORAIRE

		m_path->SetPathStartingPosition((sf::Vector2i)m_mapPosition); // TEMPORAIRE
		m_path->SetPathEndingPosition((sf::Vector2i)m_mapEndPosition); // TEMPORAIRE
	}
}

void Purchasers::UpdateLife(BuildingManagement* _builds)
{
	InitPathfinding();
	UpdatePathAndActivities(_builds);
}

void Purchasers::UpdatePathAndActivities(BuildingManagement* _builds)
{
	float speed(RESET);

	Map* currentMap = nullptr;

	switch (m_actualStatus)
	{
	case PurchaserStatus::IDLE:

		if (m_isCanLeaveTheMap)
		{
			m_mapEndPosition = FindEndRoad();
			m_isLauchingMovement = true;

			InitPathfinding();

			m_isCanLeaveTheMap = false;
			m_isWaitingEndTheMap = true;
		}

		//std::cout << "Idle\n";
		if (m_isLauchingMovement == true)
		{
			std::cout << "Changement de status vers Waiting Movement\n";

			SetStatus(PurchaserStatus::WAITING_MOVEMENT);
			m_isLauchingMovement = false;
		}


		break;

	case PurchaserStatus::WAITING_MOVEMENT:

		//std::cout << "WAITING MOVEMENT\n";
		m_path->MainStatePathfinding();

		// In the case that we find a way, we start to move
		if (m_path->GetActualStatus() == PATHFINDING_FIND_WAY_FIRST
			|| m_path->GetActualStatus() == PATHFINDING_FIND_WAY)
		{
			SetStatus(PurchaserStatus::MOVEMENT);
		}
		// In the case that we don't find any way to join the target, we stay where we're
		else if (m_path->GetActualStatus() == PATHFINDING_FIND_NO_WAY)
		{
			SetStatus(PurchaserStatus::IDLE);
			delete m_path;
			m_path = nullptr;

			// TO OPTIMIZE
			std::cout << "No Path find ...\n\n";
		}

		break;

	case PurchaserStatus::MOVEMENT:

		currentMap = Map::GetSingleton();

		if (currentMap->IsCoordinatesIsInMap((sf::Vector2i)m_mapPosition))
		{
			// Speed modification depending on the type of soil
			if (currentMap->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == PATH)
			{
				speed = TimeManagement::GetSingleton()->GetFrameTime() * 2.25f;
			}
			else if (currentMap->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == STONE_PATH)
			{
				speed = TimeManagement::GetSingleton()->GetFrameTime() * 3.5f;
			}
			else if (currentMap->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == ROAD)
			{
				speed = TimeManagement::GetSingleton()->GetFrameTime() * 5.0f;
			}
			else
			{
				speed = TimeManagement::GetSingleton()->GetFrameTime() * 1.5f;
			}

			//std::cout << "MOVEMENT\n";
			m_path->WalkProcess(&m_mapPosition, speed);
		}

		// If the path ask to be deleted, that mean that the worker has reached his destination
		if (m_path->GetActualStatus() == PATHFINDING_NEED_TO_BE_DELETED)
		{
			if (m_isWaitingEndTheMap)
			{
				SetStatus(PurchaserStatus::END_OF_LIFE);
			}
			else
			{
				SetStatus(PurchaserStatus::WORKING);
			}

			// We delete the path and init his pointer to null
			delete m_path;
			m_path = nullptr;
		}

		break;

	case PurchaserStatus::WORKING:
		_builds->m_stall->SetIsPurchaserIsThere(true);

		break;

	case PurchaserStatus::WAITING_RESOURCES:

		break;

	case PurchaserStatus::PICKUP_RESSOURCES:

		break;

	case PurchaserStatus::END_OF_LIFE:
		std::cout << "Purchaser : End of life\n";
 		PurchasersManager::GetSingleton()->RemoveCurrentPurchaser();
		delete this;
		break;

	default:
		break;
	}
}
sf::Vector2f Purchasers::FindWherePurchaserMustStopItself(sf::Vector2i _coordinatesStall)
{
	// A MODIFIER
	return sf::Vector2f(_coordinatesStall.x, 6);
}

sf::Vector2f Purchasers::FindEndRoad()
{
	// A MODIFIER
	return sf::Vector2f(20, 6);
}

int Purchasers::RandomiseData(const sf::Vector2i& _data)
{
	return rand() % (_data.y - _data.x + 1) + _data.x;
}


int Purchasers::TimeToTravel()
{
	return 0.0f;
}

void Purchasers::ClearStorage()
{
	// We remove the storage if it exist
	if (m_storage != nullptr)
	{
		delete m_storage;
		m_storage = nullptr;
	}
}



void Purchasers::SavingForFile(std::ofstream *_file)
{
	_file->write((char *)&m_actualStatus, sizeof(enum WorkerStatus));
	
	// Saving the storage of the purchaser
	Storage* purchaserStorage = m_storage;
	_file->write((char*)&purchaserStorage, sizeof(Storage*));

	if (purchaserStorage != nullptr)
	{
		purchaserStorage->SavingForFile(_file);
	}

	SavingStringIntoBinaryFile(_file, m_provenance);

	_file->write((char *)&m_isLauchingMovement, sizeof(bool));

	_file->write((char *)&m_minimalMoneyValueForRessource, sizeof(int));
	_file->write((char *)&m_maximalMoneyValueForRessource, sizeof(int));

	_file->write((char *)&m_minimalRessourceQuantity, sizeof(int));
	_file->write((char *)&m_actualRessourceQuantity, sizeof(int));
	_file->write((char *)&m_actualRessourceQuantity, sizeof(int));

	_file->write((char *)&m_ressourceID, sizeof(enum TypesOfRessources));

	_file->write((char*)&m_counterOfRefusedOffers, sizeof(int));

	_file->write((char *)&m_isPreviousOfferHasBeenRefused, sizeof(bool));
	_file->write((char*)&m_isStallExist, sizeof(bool));
	_file->write((char*)&m_isCanLeaveTheMap, sizeof(bool));
	_file->write((char*)&m_isWaitingEndTheMap, sizeof(bool));
}

void Purchasers::LoadingFromFile(std::ifstream *_file)
{
	if (m_storage != nullptr)
	{
		ClearStorage();
	}

	_file->read((char *)&m_actualStatus, sizeof(enum WorkerStatus));

	// Loading the storage of the purchaser
	Storage* purchaserStorage = nullptr;
	_file->read((char*)&purchaserStorage, sizeof(Storage*));

	if (purchaserStorage != nullptr)
	{
		// We don't need to allocate Storage because it's done at the Worker's allocation
		m_storage = new Storage();
		m_storage->LoadingFromFile(_file);
	}

	m_provenance = LoadingStringFromBinaryFile(_file);

	_file->read((char*)&m_isLauchingMovement, sizeof(bool));

	_file->read((char*)&m_minimalMoneyValueForRessource, sizeof(int));
	_file->read((char*)&m_maximalMoneyValueForRessource, sizeof(int));

	_file->read((char*)&m_minimalRessourceQuantity, sizeof(int));
	_file->read((char*)&m_actualRessourceQuantity, sizeof(int));
	_file->read((char*)&m_actualRessourceQuantity, sizeof(int));

	_file->read((char*)&m_ressourceID, sizeof(enum TypesOfRessources));

	_file->read((char*)&m_counterOfRefusedOffers, sizeof(int));

	_file->read((char*)&m_isPreviousOfferHasBeenRefused, sizeof(bool));
	_file->read((char*)&m_isStallExist, sizeof(bool));
	_file->read((char*)&m_isCanLeaveTheMap, sizeof(bool));
	_file->read((char*)&m_isWaitingEndTheMap, sizeof(bool));
}