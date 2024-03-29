#include "BuildingDestruction.h"
#include "ListOfAnnualProductions.h"


BuildingDestruction::BuildingDestruction()
{
	m_isSomeBuildingNeedToBeDestroyed = false;
	m_listOfBuildingsToDestroy = nullptr;
}

BuildingDestruction* BuildingDestruction::GetSingleton()
{
	static BuildingDestruction uniqueInstance;
	return &uniqueInstance;
}

BuildingDestruction::~BuildingDestruction()
{

}


void BuildingDestruction::InitialisationBuildingsToDestroy()
{
	std::cout << "List before : " << m_listOfBuildingsToDestroy << std::endl;

	m_listOfBuildingsToDestroy = LinkedListInitialisation();

	std::cout << "List " << m_listOfBuildingsToDestroy << " Size : " << m_listOfBuildingsToDestroy->size << " Real First : " << m_listOfBuildingsToDestroy->first << " & Last : " << m_listOfBuildingsToDestroy->last << std::endl << std::endl;
}

void BuildingDestruction::AddNewBuildingToDestroy(const sf::Vector2f& _coordinates, int _buildingID)
{
	if (!BuildingDestructionExistence(_coordinates, _buildingID))
	{
		LinkedListClass::sElement* newBuildingToDestroy = new LinkedListClass::sElement;
		newBuildingToDestroy->data = new BuildingToDestroyData;

		((BuildingToDestroyData*)newBuildingToDestroy->data)->m_coordinates = _coordinates;
		((BuildingToDestroyData*)newBuildingToDestroy->data)->m_ID = _buildingID;
		((BuildingToDestroyData*)newBuildingToDestroy->data)->m_destructionTimer = 2.0f;

		newBuildingToDestroy->status = ELEMENT_ACTIVE;

		// Add this new building at the end of the list
		AddElementToLinkedList(m_listOfBuildingsToDestroy, newBuildingToDestroy, -1);

		m_isSomeBuildingNeedToBeDestroyed = true;

		std::cout << "New building to destroy !\n";
	}
}

bool BuildingDestruction::BuildingDestructionExistence(const sf::Vector2f& _coordinates, int _buildingID)
{
	if (m_listOfBuildingsToDestroy != nullptr)
	{
		if (m_listOfBuildingsToDestroy->first != nullptr)
		{
			LinkedListClass::sElement* currentElement = m_listOfBuildingsToDestroy->first;

			for (currentElement = m_listOfBuildingsToDestroy->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((BuildingToDestroyData*)currentElement->data)->m_coordinates == _coordinates
					&& ((BuildingToDestroyData*)currentElement->data)->m_ID == _buildingID)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void BuildingDestruction::UpdateBuildingDestruction(BuildingManagement* _builds, Map* _map)
{
	if (m_isSomeBuildingNeedToBeDestroyed)
	{
		bool isRemovingElementNeeded = false;

		if (m_listOfBuildingsToDestroy != nullptr)
		{
			if (m_listOfBuildingsToDestroy->first != nullptr)
			{
				LinkedListClass::sElement* currentElement = m_listOfBuildingsToDestroy->first;

				for (currentElement = m_listOfBuildingsToDestroy->first; currentElement != NULL; currentElement = currentElement->next)
				{
					switch (((BuildingToDestroyData*)currentElement->data)->m_ID)
					{
					case BUILDING_VINES:

						// If there is a worker in the building place, we launch the destruction
						if (_builds->m_vines.GetWorkerIsThere(((BuildingToDestroyData*)currentElement->data)->m_coordinates))
						{
							((BuildingToDestroyData*)currentElement->data)->m_destructionTimer -= TimeManagement::GetSingleton()->GetFrameTime();

							// When the time is elapsed, we destroy the building
							if (((BuildingToDestroyData*)currentElement->data)->m_destructionTimer <= 0.0f)
							{
								if (_builds->m_vines.DestroyedBuildingSelected(((BuildingToDestroyData*)currentElement->data)->m_coordinates) == true)
								{
									std::cout << "Building Destroyed";
									RemoveBuildingOnMap(_builds, ((BuildingToDestroyData*)currentElement->data)->m_ID, (sf::Vector2i)((BuildingToDestroyData*)currentElement->data)->m_coordinates);
									ListOfAnnualProductions::GetSingleton()->AddResourceNumberOfBuilding(TimeManagement::GetSingleton()->GetCurrentYear(), TypesOfRessources::BUNCH_OF_GRAPE, 1);
								
									currentElement->status = ELEMENT_DELETION_REQUIRED;
									isRemovingElementNeeded = true;
								}
								else
								{
									std::cout << "Can't destroyed this building\n\n\n";
								}
							}
						}

						break;
						case BUILDING_GRAPE_STOMPING_VATS:

							// If there is a worker in the building place, we launch the destruction
							if (_builds->m_stompingVats.GetWorkerIsThere(((BuildingToDestroyData*)currentElement->data)->m_coordinates))
							{
								((BuildingToDestroyData*)currentElement->data)->m_destructionTimer -= TimeManagement::GetSingleton()->GetFrameTime();

								// When the time is elapsed, we destroy the building
								if (((BuildingToDestroyData*)currentElement->data)->m_destructionTimer <= 0.0f)
								{
									if (_builds->m_stompingVats.DestroyedBuildingSelected(((BuildingToDestroyData*)currentElement->data)->m_coordinates) == true)
									{
										std::cout << "Building Destroyed";
										RemoveBuildingOnMap(_builds, ((BuildingToDestroyData*)currentElement->data)->m_ID, (sf::Vector2i)((BuildingToDestroyData*)currentElement->data)->m_coordinates);
										ListOfAnnualProductions::GetSingleton()->AddResourceNumberOfBuilding(TimeManagement::GetSingleton()->GetCurrentYear(), TypesOfRessources::GRAPES_MUST, 1);

										currentElement->status = ELEMENT_DELETION_REQUIRED;
										isRemovingElementNeeded = true;
									}
									else
									{
										std::cout << "Can't destroyed this building\n\n\n";
									}
								}
							}

							break;
						case BUILDING_WINE_PRESS:

							// If there is a worker in the building place, we launch the destruction
							if (_builds->m_winePress.GetWorkerIsThere(((BuildingToDestroyData*)currentElement->data)->m_coordinates))
							{
								((BuildingToDestroyData*)currentElement->data)->m_destructionTimer -= TimeManagement::GetSingleton()->GetFrameTime();

								// When the time is elapsed, we destroy the building
								if (((BuildingToDestroyData*)currentElement->data)->m_destructionTimer <= 0.0f)
								{
									if (_builds->m_winePress.DestroyedBuildingSelected(((BuildingToDestroyData*)currentElement->data)->m_coordinates) == true)
									{
										std::cout << "Building Destroyed";
										RemoveBuildingOnMap(_builds, ((BuildingToDestroyData*)currentElement->data)->m_ID, (sf::Vector2i)((BuildingToDestroyData*)currentElement->data)->m_coordinates);
										ListOfAnnualProductions::GetSingleton()->AddResourceNumberOfBuilding(TimeManagement::GetSingleton()->GetCurrentYear(), TypesOfRessources::GRAPE_JUICE, 1);

										currentElement->status = ELEMENT_DELETION_REQUIRED;
										isRemovingElementNeeded = true;
									}
									else
									{
										std::cout << "Can't destroyed this building\n\n\n";
									}
								}
							}

							break;
						case BUILDING_WINE_STOREHOUSE:

							// If there is a worker in the building place, we launch the destruction
							if (_builds->m_wineStorehouse.GetWorkerIsThere(((BuildingToDestroyData*)currentElement->data)->m_coordinates))
							{
								((BuildingToDestroyData*)currentElement->data)->m_destructionTimer -= TimeManagement::GetSingleton()->GetFrameTime();

								// When the time is elapsed, we destroy the building
								if (((BuildingToDestroyData*)currentElement->data)->m_destructionTimer <= 0.0f)
								{
									if (_builds->m_wineStorehouse.DestroyedBuildingSelected(((BuildingToDestroyData*)currentElement->data)->m_coordinates) == true)
									{
										std::cout << "Building Destroyed";
										RemoveBuildingOnMap(_builds, ((BuildingToDestroyData*)currentElement->data)->m_ID, (sf::Vector2i)((BuildingToDestroyData*)currentElement->data)->m_coordinates);
										ListOfAnnualProductions::GetSingleton()->AddResourceNumberOfBuilding(TimeManagement::GetSingleton()->GetCurrentYear(), TypesOfRessources::AMPHORA_OF_WINE, 1);

										currentElement->status = ELEMENT_DELETION_REQUIRED;
										isRemovingElementNeeded = true;
									}
									else
									{
										std::cout << "Can't destroyed this building\n\n\n";
									}
								}
							}

							break;

						case BUILDING_STOREHOUSE:

							// If there is a worker in the building place, we launch the destruction
							if (_builds->m_storehouse.GetWorkerIsThere(((BuildingToDestroyData*)currentElement->data)->m_coordinates))
							{
								((BuildingToDestroyData*)currentElement->data)->m_destructionTimer -= TimeManagement::GetSingleton()->GetFrameTime();

								// When the time is elapsed, we destroy the building
								if (((BuildingToDestroyData*)currentElement->data)->m_destructionTimer <= 0.0f)
								{
									if (_builds->m_storehouse.DestroyedBuildingSelected(((BuildingToDestroyData*)currentElement->data)->m_coordinates) == true)
									{
										std::cout << "Building Destroyed";
										RemoveBuildingOnMap(_builds, ((BuildingToDestroyData*)currentElement->data)->m_ID, (sf::Vector2i)((BuildingToDestroyData*)currentElement->data)->m_coordinates);

										currentElement->status = ELEMENT_DELETION_REQUIRED;
										isRemovingElementNeeded = true;
									}
									else
									{
										std::cout << "Can't destroyed this building\n\n\n";
									}
								}
							}

							break;

						case BUILDING_STALL:

							// If there is a worker in the building place, we launch the destruction
							if (_builds->m_stall->GetWorkerIsThere(((BuildingToDestroyData*)currentElement->data)->m_coordinates))
							{
								((BuildingToDestroyData*)currentElement->data)->m_destructionTimer -= TimeManagement::GetSingleton()->GetFrameTime();

								// When the time is elapsed, we destroy the building
								if (((BuildingToDestroyData*)currentElement->data)->m_destructionTimer <= 0.0f)
								{
									if (_builds->m_stall->DestroyedBuildingSelected(((BuildingToDestroyData*)currentElement->data)->m_coordinates) == true)
									{
										std::cout << "Building Destroyed";
										RemoveBuildingOnMap(_builds, ((BuildingToDestroyData*)currentElement->data)->m_ID, (sf::Vector2i)((BuildingToDestroyData*)currentElement->data)->m_coordinates);

										currentElement->status = ELEMENT_DELETION_REQUIRED;
										isRemovingElementNeeded = true;
									}
									else
									{
										std::cout << "Can't destroyed this building\n\n\n";
									}
								}
							}

							break;
						case BUILDING_PATH:
							
							((BuildingToDestroyData*)currentElement->data)->m_destructionTimer -= TimeManagement::GetSingleton()->GetFrameTime();

							// When the time is elapsed, we destroy the building
							if (((BuildingToDestroyData*)currentElement->data)->m_destructionTimer <= 0.0f)
							{
								std::cout << "Building Destroyed";
								RemoveBuildingOnMap(_builds, ((BuildingToDestroyData*)currentElement->data)->m_ID, (sf::Vector2i)((BuildingToDestroyData*)currentElement->data)->m_coordinates);

								currentElement->status = ELEMENT_DELETION_REQUIRED;
								isRemovingElementNeeded = true;
							}

							break;
						case BUILDING_PAVED_PATH:

							((BuildingToDestroyData*)currentElement->data)->m_destructionTimer -= TimeManagement::GetSingleton()->GetFrameTime();

							// When the time is elapsed, we destroy the building
							if (((BuildingToDestroyData*)currentElement->data)->m_destructionTimer <= 0.0f)
							{
								std::cout << "Building Destroyed";
								RemoveBuildingOnMap(_builds, ((BuildingToDestroyData*)currentElement->data)->m_ID, (sf::Vector2i)((BuildingToDestroyData*)currentElement->data)->m_coordinates);

								currentElement->status = ELEMENT_DELETION_REQUIRED;
								isRemovingElementNeeded = true;
							}

							break;
						case BUILDING_ROAD:

							((BuildingToDestroyData*)currentElement->data)->m_destructionTimer -= TimeManagement::GetSingleton()->GetFrameTime();

							// When the time is elapsed, we destroy the building
							if (((BuildingToDestroyData*)currentElement->data)->m_destructionTimer <= 0.0f)
							{

								std::cout << "Building Destroyed";
								RemoveBuildingOnMap(_builds, ((BuildingToDestroyData*)currentElement->data)->m_ID, (sf::Vector2i)((BuildingToDestroyData*)currentElement->data)->m_coordinates);

								currentElement->status = ELEMENT_DELETION_REQUIRED;
								isRemovingElementNeeded = true;
							}

							break;
					default:
						break;
					}
				}
			}
		}

		if (isRemovingElementNeeded)
		{
			RemoveElementsOfLinkedList(m_listOfBuildingsToDestroy);

			// If the list is empty, we don't visite inside the function
			if (m_listOfBuildingsToDestroy->first == nullptr)
			{
				m_isSomeBuildingNeedToBeDestroyed = false;
			}
		}
	}
}

void BuildingDestruction::RemoveBuildingOnMap(BuildingManagement* _builds, const int& _typeOfBuilding, const sf::Vector2i& _mapPosition)
{
	Map* pMap = Map::GetSingleton();

	for (int y = 0; y < _builds->m_buildings[_typeOfBuilding].GetSize().y; y++)
	{
		for (int x = 0; x < _builds->m_buildings[_typeOfBuilding].GetSize().x; x++)
		{
			if (pMap->IsCoordinatesIsInMap(sf::Vector2i(_mapPosition.x - x, _mapPosition.y - y)))
			{
				// Set the correct collision, building ID, and sprite ID
				pMap->GetMap()[ZERO_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = NO_COLLISION;
				pMap->GetMap()[ZERO_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = -1;
				pMap->GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x] = 1;

				pMap->GetMap()[FIRST_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = NO_COLLISION;
				pMap->GetMap()[FIRST_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = -1;
				pMap->GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x] = 0;
			}
			else
			{
				std::cout << "\n\n\n\tError during building destruction\n\n\n";
			}
		}
	}
}

void BuildingDestruction::SavingForFile(std::ofstream* _file)
{
	// Save the number of buildings listed to be destroyed
	_file->write((char*)&m_listOfBuildingsToDestroy->size, sizeof(int));

	// Save the content of the list
	if (m_listOfBuildingsToDestroy != nullptr)
	{
		if (m_listOfBuildingsToDestroy->first != nullptr)
		{
			LinkedListClass::sElement* currentElement = m_listOfBuildingsToDestroy->first;

			for (currentElement = m_listOfBuildingsToDestroy->first; currentElement != NULL; currentElement = currentElement->next)
			{
				_file->write((char*)(BuildingToDestroyData*)currentElement->data, sizeof(BuildingToDestroyData));
			}
		}
	}
}

void BuildingDestruction::LoadingFromFile(std::ifstream* _file)
{
	// Clear the current list
	if (m_listOfBuildingsToDestroy != nullptr)
	{
		FreeLinkedList(m_listOfBuildingsToDestroy);
	}


	// We reinit the list
	m_listOfBuildingsToDestroy = LinkedListInitialisation();


	// Load the number of buildings
	int previousListSize(RESET);
	_file->read((char*)&previousListSize, sizeof(int));

	// We add every workers data to the list
	for (int i = RESET; i < previousListSize; i++)
	{
		LinkedListClass::sElement* newBuildingToDestroy = new LinkedListClass::sElement;
		newBuildingToDestroy->data = new BuildingToDestroyData;

		_file->read((char*)(BuildingToDestroyData*)newBuildingToDestroy->data, sizeof(BuildingToDestroyData));

		newBuildingToDestroy->status = ELEMENT_ACTIVE;

		m_isSomeBuildingNeedToBeDestroyed = true;

		std::cout << "New building to destroy !\n";

		if (i == 0)
		{
			// Add this building at the beggining of the list
			AddElementToLinkedList(m_listOfBuildingsToDestroy, newBuildingToDestroy, 1);
		}
		else
		{
			// Add this building at the end of the list
			AddElementToLinkedList(m_listOfBuildingsToDestroy, newBuildingToDestroy, -1);
		}
	}
}