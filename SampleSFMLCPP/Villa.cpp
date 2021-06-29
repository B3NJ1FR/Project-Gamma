#include "Villa.h"
#include "Map.h"
#include "GameDefinitions.h"


Villa::Villa()
{
	m_workerIsThereSprite = LoadSprite("Data/Assets/Sprites/Entities/worker_working.png", 1);
	m_workerInsideSprite = LoadSprite("Data/Assets/Sprites/Entities/worker_inside_building.png", 1);

	m_mapPosition = sf::Vector2i(-1, -1);
	m_constructionState = VillaBuildingStatus::VILLA_NOT_PLANNED;
}


Villa::~Villa()
{

}

Villa* Villa::GetSingleton()
{
	static Villa uniqueInstance;
	return &uniqueInstance;
}




void Villa::Initialisation(Buildings* _buildingConcerned)
{
	m_building = _buildingConcerned;
}

void Villa::AddNewBuilding(sf::Vector2i _mapPosition)
{
	// Save the position in map
	m_mapPosition = _mapPosition;

	// Init of the building construction status after being placed on map
	m_constructionState = VillaBuildingStatus::VILLA_PLANNED;

	m_numberOfWorkersNeededToWorks = (m_building != nullptr) ? m_building->GetNumberWorkersNeeded() : 0;
	m_currentNumberOfWorkersPresent = 0;

	m_lifeTime = RESET;

	m_isChangingSprite = false;
	m_hasBeenBuilt = false;
	m_isWorkerThere = false;
}


void Villa::UpdateBuildingConstruction()
{
	switch (m_constructionState)
	{
	case VillaBuildingStatus::VILLA_PLANNED:
		// Plannification of the building construction

		// If we've the workers accredited to the building construction, we can pass the state to CONSTRUCTION
		//std::cout << "Planned ! " << std::endl;

		// TEMPORARY
		if (m_isWorkerThere == true)
		{
			//std::cout << "Building launched ! " << ((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime << " " << building->GetConstructionTimeCost() << std::endl;
			m_constructionState = VillaBuildingStatus::VILLA_CONSTRUCTION;
			m_lifeTime = RESET;
		}

		break;
	case VillaBuildingStatus::VILLA_CONSTRUCTION:

		if (m_isWorkerThere == true)
		{
			m_lifeTime += TimeManagement::GetSingleton()->GetFrameTime();
			m_isWorkerThere = false;
		}

		//std::cout << "Constructing ...\n";

		// If the building life is higher than the construction time, we launch it's growthing
		if (m_lifeTime >= m_building->GetConstructionTimeCost())
		{
			//std::cout << "Building built ! " << ((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime << " " << building->GetConstructionTimeCost() << std::endl;
			m_constructionState = VillaBuildingStatus::VILLA_BUILT;
			//((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = true;
		}

		break;
	case VillaBuildingStatus::VILLA_BUILT:
		if (m_hasBeenBuilt == false)
		{
			std::cout << "Building successfully constructed ! " << std::endl;
			m_isChangingSprite = true;

		}

		break;
	case VillaBuildingStatus::VILLA_BUILDING_NOT_MAINTAINED:
		break;

	case VillaBuildingStatus::VILLA_BUILDING_DESTROYED:
		break;

	default:
		// ERROR LOG
		break;
	}
}


void Villa::UpdateBuildingSprite()
{
	if (m_isChangingSprite == true)
	{
		if (m_constructionState == VillaBuildingStatus::VILLA_BUILT
			&& m_hasBeenBuilt == false)
		{
			Map* pMap = Map::GetSingleton();
			unsigned short buildingSpriteID = RESET;

			m_hasBeenBuilt = true;
			m_isChangingSprite = false;

			// Ser the ground
			if (pMap->IsCoordinatesIsInMap(m_mapPosition))
			{
				buildingSpriteID = (unsigned short)m_building->GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_GROUND][m_building->GetSize().y - 1][m_building->GetSize().x - 1];

				pMap->GetMap()[ZERO_FLOOR + SPRITE_ID][m_mapPosition.y][m_mapPosition.x] = buildingSpriteID;

				// Set the collisions and buildings id for the ground
				pMap->GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_mapPosition.y][m_mapPosition.x] = (unsigned short)COLLISION;
				pMap->GetMap()[ZERO_FLOOR + BUILDING_ID][m_mapPosition.y][m_mapPosition.x] = (unsigned short)TypeOfBuilding::BUILDING_VILLA;
			}
			
			std::cout << m_building->GetSize().x << " " << m_building->GetSize().y << std::endl;

			// Set the building
			for (int y = 0; y < m_building->GetSize().y; y++)
			{
				for (int x = 0; x < m_building->GetSize().x; x++)
				{
					if (pMap->IsCoordinatesIsInMap(sf::Vector2i(m_mapPosition.x - x, m_mapPosition.y - y)))
					{
						buildingSpriteID = (unsigned short)m_building->GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_MAIN_FLOOR][m_building->GetSize().y - 1 - y][m_building->GetSize().x - 1 - x];

						if (buildingSpriteID >= 0)
						{
							// Set the correct sprite id for this building
							pMap->GetMap()[FIRST_FLOOR + SPRITE_ID][m_mapPosition.y - y][m_mapPosition.x - x] = buildingSpriteID;

							pMap->GetMap()[FIRST_FLOOR + COLLISIONS_ID][m_mapPosition.y - y][m_mapPosition.x - x] = (unsigned short)COLLISION;
							pMap->GetMap()[FIRST_FLOOR + BUILDING_ID][m_mapPosition.y - y][m_mapPosition.x - x] = (unsigned short)TypeOfBuilding::BUILDING_VILLA;
						}
					}
					else
					{
						std::cout << "\n\n\n\tError during building placement\n\n\n";
					}
				}
			}
		}
	}
}



bool Villa::GetWorkerIsThere(const sf::Vector2i& _mapPosition) const
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		return m_isWorkerThere;
	}
	else
	{
		return false;
	}
}


int Villa::GetNumberOfWorkersPresents(const sf::Vector2i& _mapPosition) const
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		return m_currentNumberOfWorkersPresent;
	}
	else
	{
		return 0;
	}
}

bool Villa::IsBuildingIsWorking(const sf::Vector2i& _mapPosition) const
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		return (m_currentNumberOfWorkersPresent >= m_numberOfWorkersNeededToWorks) ? true : false;
	}

	return false;
}


bool Villa::ConfirmSpecificBuildingPresenceAtPosition(const sf::Vector2i& _mapPosition, const bool& _isPreciseCoordinates, const bool& _thisIsAWorker)
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		if (_isPreciseCoordinates)
		{
			if (_mapPosition == m_mapPosition)
			{
				if (_thisIsAWorker)
				{
					m_isWorkerThere = true;
				}

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (_thisIsAWorker)
			{
				m_isWorkerThere = true;
			}

			return true;
		}
	}

	return false;
}

void Villa::WorkerEnteringInThisPosition(const sf::Vector2i& _mapPosition)
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		m_currentNumberOfWorkersPresent += 1;
	}
}

void Villa::WorkerLeavingThisPosition(const sf::Vector2i& _mapPosition)
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		m_currentNumberOfWorkersPresent -= 1;

		if (m_currentNumberOfWorkersPresent == 0)
		{
			m_isWorkerThere = false;
		}
		else if (m_currentNumberOfWorkersPresent < 0)
		{
			m_currentNumberOfWorkersPresent = 0;
		}
	}
}


bool Villa::CheckBuildingHasBeenBuilt(const sf::Vector2i& _mapPosition)
{
	if (m_mapPosition == _mapPosition)
	{
		return (m_constructionState == VillaBuildingStatus::VILLA_PLANNED || m_constructionState == VillaBuildingStatus::VILLA_CONSTRUCTION) ? false : true;
	}

	return true;
}


void Villa::ForceBuildingConstructionWithoutWorkers(sf::Vector2i _mapPosition)
{
	AddNewBuilding(_mapPosition);

	m_hasBeenBuilt = false;

	m_constructionState = VillaBuildingStatus::VILLA_BUILT;

	m_lifeTime = (m_building != nullptr) ? m_building->GetConstructionTimeCost() : 0;
}



void Villa::SavingVillaForFile(std::ofstream* _file)
{
	_file->write((char*)&m_constructionState, sizeof(enum VillaBuildingStatus));

	if (m_constructionState != VillaBuildingStatus::VILLA_BUILDING_DESTROYED
		&& m_constructionState != VillaBuildingStatus::VILLA_NOT_PLANNED)
	{
		// Save the map position
		_file->write((char*)&m_mapPosition.x, sizeof(int));
		_file->write((char*)&m_mapPosition.y, sizeof(int));

		_file->write((char*)&m_numberOfWorkersNeededToWorks, sizeof(int));
		_file->write((char*)&m_currentNumberOfWorkersPresent, sizeof(int));

		_file->write((char*)&m_lifeTime, sizeof(float));

		_file->write((char*)&m_isChangingSprite, sizeof(bool));
		_file->write((char*)&m_hasBeenBuilt, sizeof(bool));
		_file->write((char*)&m_isWorkerThere, sizeof(bool));
	}
}


void Villa::LoadingVillaFromFile(std::ifstream* _file)
{
	_file->read((char*)&m_constructionState, sizeof(enum VillaBuildingStatus));

	if (m_constructionState != VillaBuildingStatus::VILLA_BUILDING_DESTROYED
		&& m_constructionState != VillaBuildingStatus::VILLA_NOT_PLANNED)
	{
		// Save the map position
		_file->read((char*)&m_mapPosition.x, sizeof(int));
		_file->read((char*)&m_mapPosition.y, sizeof(int));

		_file->read((char*)&m_numberOfWorkersNeededToWorks, sizeof(int));
		_file->read((char*)&m_currentNumberOfWorkersPresent, sizeof(int));

		_file->read((char*)&m_lifeTime, sizeof(float));

		_file->read((char*)&m_isChangingSprite, sizeof(bool));
		_file->read((char*)&m_hasBeenBuilt, sizeof(bool));
		_file->read((char*)&m_isWorkerThere, sizeof(bool));
	}
}