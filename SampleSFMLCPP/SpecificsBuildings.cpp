#include "SpecificsBuildings.h"
#include "GameDefinitions.h"
#include "RessourcesManager.h"



SpecificsBuildings::SpecificsBuildings()
{
	m_workerIsThereSprite = LoadSprite("Data/Assets/Sprites/Entities/worker_working.png", 1);
	m_workerInsideSprite = LoadSprite("Data/Assets/Sprites/Entities/worker_inside_building.png", 1);
}


SpecificsBuildings::~SpecificsBuildings()
{
	if (m_list != nullptr)
	{
		ClearStorages();
	}
}



Storage* SpecificsBuildings::GetStorage(const sf::Vector2f& _mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					if (((sBuildingData*)currentElement->data)->storage != nullptr)
					{
						return ((sBuildingData*)currentElement->data)->storage;
					}
				}
			}
		}
	}

	return nullptr;
}


void SpecificsBuildings::InitialisationSpeBuilding(Buildings* _specificBuildingConcerned)
{
	std::cout << "List before : " << m_list << std::endl;

	m_list = LinkedListInitialisation();

	std::cout << "List " << m_list << " Size : " << m_list->size << " Real First : " << m_list->first << " & Last : " << m_list->last << std::endl << std::endl;

	m_building = _specificBuildingConcerned;

}

void SpecificsBuildings::AddNewBuildingToList(sf::Vector2f _mapPosition)
{
	LinkedListClass::sElement* newBuilding = new LinkedListClass::sElement;
	newBuilding->data = new sBuildingData;

	// Save the position in map
	((sBuildingData *)newBuilding->data)->mapPosition = _mapPosition;

	// Init of the building construction status after being placed on map
	((sBuildingData *)newBuilding->data)->constructionState = BuildingStatus::PLANNED;
	((sBuildingData *)newBuilding->data)->actualState = MainBuildingStatus::BUILDING_READY_TO_PRODUCE;

	// Allocation of the storage
	((sBuildingData *)newBuilding->data)->storage = new Storage();
	((sBuildingData *)newBuilding->data)->storage->SetName("Specific");

	// A MODIFIER PAR VALEUR SEUIL
	((sBuildingData *)newBuilding->data)->quantitativeThreshold = m_building->GetRessourceQuantityNeeded();
	// A CONFIGURER
	((sBuildingData *)newBuilding->data)->maximalQuantity = 5;
	((sBuildingData *)newBuilding->data)->internalExportRessourceCounter = RESET;

	((sBuildingData *)newBuilding->data)->numberOfWorkersNeededToWorks = m_building->GetNumberWorkersNeeded();
	((sBuildingData *)newBuilding->data)->currentNumberOfWorkersPresent = 0;
	
	((sBuildingData *)newBuilding->data)->lifeTime = RESET;
	((sBuildingData *)newBuilding->data)->actualProductionTime = RESET;
	((sBuildingData *)newBuilding->data)->secondaryTime = RESET;

	((sBuildingData *)newBuilding->data)->isChangingSprite = false;
	((sBuildingData *)newBuilding->data)->hasBeenBuilt = false;
	((sBuildingData *)newBuilding->data)->isProduced = false;
	((sBuildingData *)newBuilding->data)->isWorkerThere = false;
	((sBuildingData *)newBuilding->data)->isProdCanBeCollected = false;

	newBuilding->status = ELEMENT_ACTIVE;

	// Add this new vine at the end of the list
	AddElementToLinkedList(m_list, newBuilding, -1);

	//ReadVineLinkedList();
	//ReadLinkedList(list);
}


void SpecificsBuildings::UpdateInternalCycles()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->constructionState == BUILT)
				{
					Storage* storage = ((SpecificsBuildings::sBuildingData*)currentElement->data)->storage;
					std::vector<Ressources*> arrayOfResources;
					int counter = 0;
					bool isRelaunchProduction = false;

					switch (((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState)
					{
					case BUILDING_READY_TO_PRODUCE:

						arrayOfResources = storage->GetResourceFromData(ResourceData::RESOURCE_NEEDED);

						// We verify that we have enough of each resource to start the production
						for (Ressources* resource : arrayOfResources)
						{
							// DANS LE CAS DE 2 RESSOURCES OU PLUS REQUISES, NE FONCTIONNERA PAS
							// A MODIFIER
							if (resource->GetQuantityOwned() > ((SpecificsBuildings::sBuildingData*)currentElement->data)->quantitativeThreshold)
							{
								++counter;
							}
						}
						arrayOfResources.clear();

						// DANS LE CAS DE 2 RESSOURCES OU PLUS REQUISES, NE FONCTIONNERA PAS
						// A MODIFIER
						if (counter >= 1)
						{
							((SpecificsBuildings::sBuildingData*)currentElement->data)->actualState = BUILDING_FILLING;
							std::cout << "Building start to fill\n";
						}

						break;
					case BUILDING_FILLING:

						// If the workers are there, we start to fill the building
						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere == true)
						{
							arrayOfResources = storage->GetResourceFromData(ResourceData::RESOURCE_NEEDED);

							// We verify that we have enough of each resource to start the production
							for (Ressources* resource : arrayOfResources)
							{
								if (resource->GetQuantityOwned() - 1 >= 0)
								{
									resource->TransferFromOwnedToReserved(1);

									// DANS LE CAS DE 2 RESSOURCES OU PLUS REQUISES, NE FONCTIONNERA PAS
									// A MODIFIER

									if ((resource->GetQuantityOwned() == 0 || resource->GetQuantityReserved() == ((SpecificsBuildings::sBuildingData*)currentElement->data)->maximalQuantity)
										&& resource->GetQuantityReserved() >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->quantitativeThreshold)
									{
										((SpecificsBuildings::sBuildingData*)currentElement->data)->actualState = BUILDING_WORKS;
										std::cout << "Building start to work\n";
									}
								}
							}
							arrayOfResources.clear();

						}

						break;
					case BUILDING_WORKS:

						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere == true)
						{
							// We verify the number of worker present in the building is higher or equal than which is needed
							if (((SpecificsBuildings::sBuildingData*)currentElement->data)->currentNumberOfWorkersPresent
								>= ((SpecificsBuildings::sBuildingData*)currentElement->data)->numberOfWorkersNeededToWorks)
							{
								((SpecificsBuildings::sBuildingData*)currentElement->data)->actualProductionTime += TimeManagement::GetSingleton()->GetFrameTime();

								if (((SpecificsBuildings::sBuildingData*)currentElement->data)->actualProductionTime > m_building->GetProductionTimeCost())
								{
									((SpecificsBuildings::sBuildingData*)currentElement->data)->actualProductionTime = RESET;
									((SpecificsBuildings::sBuildingData*)currentElement->data)->secondaryTime = RESET;
									((SpecificsBuildings::sBuildingData*)currentElement->data)->isProduced = true;

									((SpecificsBuildings::sBuildingData*)currentElement->data)->actualState = BUILDING_COLLECTING_PRODUCTION;
									
									std::cout << "Building collection production\n";
								}
							}
						}

						break;
					case BUILDING_COLLECTING_PRODUCTION:
						
						

						break;
					case BUILDING_NEED_TO_BE_CLEANED:

						((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = true;
						((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = false;
						
						arrayOfResources = storage->GetResourceFromData(ResourceData::RESOURCE_NEEDED);

						// If we have enough of each resource to re-start the production, we re-launch it
						for (Ressources* resource : arrayOfResources)
						{
							if (resource->GetQuantityReserved() >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->quantitativeThreshold)
							{
								isRelaunchProduction = true;
							}
						}
						arrayOfResources.clear();

						((SpecificsBuildings::sBuildingData*)currentElement->data)->actualState = isRelaunchProduction ? BUILDING_WORKS : BUILDING_READY_TO_PRODUCE;

						break;
					default:
						break;
					}
				}
			}
		}
		else
		{
			//std::cout << "List : " << list->first << std::endl;
		}
	}
}

void SpecificsBuildings::UpdateBuildingConstruction()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			//std::cout << "Time : " << _lapsedFrameTime << std::endl;

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				switch (((SpecificsBuildings::sBuildingData *)currentElement->data)->constructionState)
				{
				case PLANNED:
					// Plannification of the building construction

					// If we've the workers accredited to the building construction, we can pass the state to CONSTRUCTION
					//std::cout << "Planned ! " << std::endl;

					// TEMPORARY
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere == true)
					{
						//std::cout << "Building launched ! " << ((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime << " " << building->GetConstructionTimeCost() << std::endl;
						((SpecificsBuildings::sBuildingData *)currentElement->data)->constructionState = CONSTRUCTION;
						((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime = RESET;
					}

					break;
				case CONSTRUCTION:

					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere == true)
					{
						((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime += TimeManagement::GetSingleton()->GetFrameTime();
						((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = false;
					}

					//std::cout << "Constructing ...\n";

					// If the building life is higher than the construction time, we launch it's growthing
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime >= m_building->GetConstructionTimeCost())
					{
						//std::cout << "Building built ! " << ((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime << " " << building->GetConstructionTimeCost() << std::endl;
						((SpecificsBuildings::sBuildingData *)currentElement->data)->constructionState = BUILT;
						//((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = true;
					}

					break;
				case BUILT:
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->hasBeenBuilt == false)
					{
						std::cout << "Building successfully constructed ! " << std::endl;
						((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = true;

					}

					break;
				case BUILDING_NOT_MAINTAINED:
					break;

				default:
					// ERROR LOG
					break;
				}
			}
		}
		else
		{
			//std::cout << "List : " << list->first << std::endl;
		}
	}
}


void SpecificsBuildings::UpdateBuildingSprite()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite == true)
				{
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->constructionState == BUILT
						&& ((SpecificsBuildings::sBuildingData *)currentElement->data)->hasBeenBuilt == false)
					{
						Map* pMap = Map::GetSingleton();
						sf::Vector2i mapPosition = (sf::Vector2i)((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition;
						unsigned short buildingSpriteID = RESET;

						((SpecificsBuildings::sBuildingData *)currentElement->data)->hasBeenBuilt = true;
						((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = false;

						for (int y = 0; y < m_building->GetSize().y; y++)
						{
							for (int x = 0; x < m_building->GetSize().x; x++)
							{
								if (pMap->IsCoordinatesIsInMap(sf::Vector2i(mapPosition.x - x, mapPosition.y - y)))
								{
									buildingSpriteID = (unsigned short)m_building->GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_MAIN_FLOOR][m_building->GetSize().y - 1 - y][m_building->GetSize().x - 1 - x];

									if (buildingSpriteID >= 0)
									{
										// Set the correct sprite id for this building
										pMap->GetMap()[FIRST_FLOOR + SPRITE_ID][mapPosition.y - y][mapPosition.x - x] = buildingSpriteID;

										pMap->GetMap()[FIRST_FLOOR + COLLISIONS_ID][mapPosition.y - y][mapPosition.x - x] = COLLISION;
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
		}
	}
}


void SpecificsBuildings::UpdateBuildingProduction()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
				{
					Storage* storage = ((SpecificsBuildings::sBuildingData*)currentElement->data)->storage;
					std::vector<Ressources*> arrayOfResources;

					arrayOfResources = storage->GetResourceFromData(ResourceData::RESOURCE_NEEDED);

					// We verify that we have enough of each resource to start the production
					for (Ressources* resource : arrayOfResources)
					{
						if (resource->GetQuantityReserved() > 0)
						{
							// We delete the resources reserved to this production
							resource->AddOrSubtractQuantityReserved(-((SpecificsBuildings::sBuildingData*)currentElement->data)->quantitativeThreshold);
						}
					}
					arrayOfResources.clear();

					arrayOfResources = storage->GetResourceFromData(ResourceData::RESOURCE_PRODUCED);

					// DANS LE CAS DE 2 RESSOURCES OU PLUS PRODUITES, NE FONCTIONNERA PAS
					// A MODIFIER
					int quantityProduced = m_building->GetRessourceQuantityProduced();

					// We verify that we have enough of each resource to start the production
					for (Ressources* resource : arrayOfResources)
					{
						// We add the resources created during this production
						resource->AddOrSubtractQuantityOwned(quantityProduced);

						// DANS LE CAS DE 2 RESSOURCES OU PLUS PRODUITES, NE FONCTIONNERA PAS
						// A MODIFIER
						((SpecificsBuildings::sBuildingData*)currentElement->data)->isProduced = false;
						((SpecificsBuildings::sBuildingData*)currentElement->data)->isProdCanBeCollected = true;
						std::cout << "Here";
					}

					arrayOfResources.clear();

					// Launch the feedback animation of producing
				}
			}
		}
	}
}


bool SpecificsBuildings::GetWorkerIsThere(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					return ((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere;
				}
			}

			return false;

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


int SpecificsBuildings::GetNumberOfWorkersPresents(const sf::Vector2f& _mapPosition) const
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					return ((SpecificsBuildings::sBuildingData*)currentElement->data)->currentNumberOfWorkersPresent;
				}
			}

			return 0;

		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

bool SpecificsBuildings::IsBuildingIsWorking(const sf::Vector2f& _mapPosition) const
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					if (((SpecificsBuildings::sBuildingData*)currentElement->data)->currentNumberOfWorkersPresent
						>= ((SpecificsBuildings::sBuildingData*)currentElement->data)->numberOfWorkersNeededToWorks)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}

			return false;

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool SpecificsBuildings::ConfirmSpecificBuildingPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates, const bool &_thisIsAWorker)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					if (_isPreciseCoordinates)
					{
						if (_mapPosition == ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition)
						{
							if (_thisIsAWorker)
							{
								((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
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
							((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
						}

						return true;
					}
				}
			}

			return false;

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void SpecificsBuildings::WorkerEnteringInThisPosition(const sf::Vector2f& _mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					((SpecificsBuildings::sBuildingData*)currentElement->data)->currentNumberOfWorkersPresent += 1;

					break;
				}
			}
		}
	}
}
void SpecificsBuildings::WorkerLeavingThisPosition(const sf::Vector2f& _mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					((SpecificsBuildings::sBuildingData*)currentElement->data)->currentNumberOfWorkersPresent -= 1;

					if (((SpecificsBuildings::sBuildingData*)currentElement->data)->currentNumberOfWorkersPresent == 0)
					{
						((SpecificsBuildings::sBuildingData*)currentElement->data)->isWorkerThere = false;
					}
					else if (((SpecificsBuildings::sBuildingData*)currentElement->data)->currentNumberOfWorkersPresent < 0)
					{
						((SpecificsBuildings::sBuildingData*)currentElement->data)->currentNumberOfWorkersPresent = 0;
					}

					break;
				}
			}
		}
	}
}

bool SpecificsBuildings::CheckHasProducedRessource(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProdCanBeCollected == true)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}

			return false;

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool SpecificsBuildings::CheckSpecificsBuildingsHasBeenBuilt(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->constructionState == PLANNED
						 || ((SpecificsBuildings::sBuildingData *)currentElement->data)->constructionState == CONSTRUCTION)
					{
						return false;
					}
					else
					{
						return true;
					}
				}
			}

			return true;

		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
}

bool SpecificsBuildings::UpdateRessourcePickuping(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProdCanBeCollected == true)
					{
						((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime += TimeManagement::GetSingleton()->GetFrameTime();

						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime >= m_building->GetPickupingTimeCost())
						{
							// Launch the feedback animation of producing
							((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = true;
							((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime = RESET;

							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_NEED_TO_BE_CLEANED;

							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}

			}

			return false;

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

// Check if the storage is still containing resources produced but not picked up 
// If it's the case, we leave the "isProdCanBeCollected" value to true to allow to an other worker to pick up the resources
void SpecificsBuildings::RessourcePickedUp(const sf::Vector2f& _mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				// We verify the location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					if (((SpecificsBuildings::sBuildingData*)currentElement->data)->isProdCanBeCollected == true)
					{
						Storage* storage = ((SpecificsBuildings::sBuildingData*)currentElement->data)->storage;
						std::vector<Ressources*> arrayOfResources = storage->GetResourceFromData(ResourceData::RESOURCE_PRODUCED);

						// We verify that we have enough of each resource to start the production
						for (Ressources* resource : arrayOfResources)
						{
							// We add the resources created during this production
							if (resource->GetQuantityOwned() == 0)
							{
								// DANS LE CAS DE 2 RESSOURCES OU PLUS PRODUITES, NE FONCTIONNERA PAS
								// A MODIFIER
								((SpecificsBuildings::sBuildingData*)currentElement->data)->isProdCanBeCollected = false;
							}
						}

						arrayOfResources.clear();
					}

					break;
				}
			}

		}

	}
}


sf::Vector2i SpecificsBuildings::FindNearestBuilding(const sf::Vector2f &_mapPosition)
{
	sf::Vector2i buildingPosition = { RESET, RESET };

	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			float lastLowerDistance(RESET);

			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					return (sf::Vector2i)((SpecificsBuildings::sBuildingData*)currentElement->data)->mapPosition;
				}

				if (currentElement == m_list->first)
				{
					float distance = DistanceFormula(_mapPosition, ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition);

					lastLowerDistance = distance;
					buildingPosition = (sf::Vector2i)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition;
				}
				else
				{
					float distance = DistanceFormula(_mapPosition, ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition);

					if (distance < lastLowerDistance)
					{
						lastLowerDistance = distance;
						buildingPosition = (sf::Vector2i)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition;
					}
					else if (distance == lastLowerDistance)
					{

					}
					else
					{

					}

				}
			}

			return buildingPosition;
		}
		else
		{
			return buildingPosition;
		}
	}
	else
	{
		return buildingPosition;
	}
}


bool SpecificsBuildings::DestroyedBuildingSelected(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			bool isBuildingFind = false;

			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				//std::cout << "Map : " << positionCounter << "/" << list->size << " -> "<< ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x << " " << ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y << std::endl;

				// If the building position is identical to which send, we save his position in the linked list
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition
					&& isBuildingFind == false)
				{
					isBuildingFind = true;
					currentElement->status = ELEMENT_DELETION_REQUIRED;
					break;
				}
			}

			//std::cout << std::endl;

			// After having saved the building's position, we ask to destroy it
			if (isBuildingFind == true)
			{
				RemoveElementsOfLinkedList(m_list);

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}



void SpecificsBuildings::ClearStorages()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We remove all storages
				if (((SpecificsBuildings::sBuildingData*)currentElement->data)->storage != nullptr)
				{
					// Delete the storage from the list
					delete ((SpecificsBuildings::sBuildingData*)currentElement->data)->storage;
					((SpecificsBuildings::sBuildingData*)currentElement->data)->storage = nullptr;
				}
			}
		}
	}
}

void SpecificsBuildings::SavingSpecificsBuildingsListForFile(std::ofstream *_file)
{
	// Save the number of vines
	_file->write((char *)&m_list->size, sizeof(int));

	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				_file->write((char *)(SpecificsBuildings::sBuildingData *)currentElement->data, sizeof(sBuildingData));
				_file->write((char*)&((SpecificsBuildings::sBuildingData*)currentElement->data)->storage, sizeof(Storage*));

				if (((SpecificsBuildings::sBuildingData*)currentElement->data)->storage != nullptr)
				{
					((SpecificsBuildings::sBuildingData*)currentElement->data)->storage->SavingForFile(_file);
				}
			}
		}
	}
}


void SpecificsBuildings::LoadingSpecificsBuildingsListFromFile(std::ifstream *_file)
{
	// Delete every vines
	if (m_list != nullptr)
	{
		ClearStorages();
		FreeLinkedList(m_list);
	}


	// We reinit the vines list
	m_list = LinkedListInitialisation();


	// Save the number of vines
	int previousListSize(RESET);
	_file->read((char *)&previousListSize, sizeof(int));

	// We add every workers data to the list
	for (int i = RESET; i < previousListSize; i++)
	{
		LinkedListClass::sElement* newBuilding = new LinkedListClass::sElement;
		newBuilding->data = new SpecificsBuildings::sBuildingData;

		_file->read((char *)(SpecificsBuildings::sBuildingData *)newBuilding->data, sizeof(sBuildingData));

		// Load the storage of the building
		Storage* buildingStorage = nullptr;
		_file->read((char*)&buildingStorage, sizeof(Storage*));

		if (buildingStorage != nullptr)
		{
			((SpecificsBuildings::sBuildingData *)newBuilding->data)->storage = new Storage();
			((SpecificsBuildings::sBuildingData *)newBuilding->data)->storage->LoadingFromFile(_file);
		}


		newBuilding->status = ELEMENT_ACTIVE;

		if (i == 0)
		{
			// Add this worker at the top of the list
			AddElementToLinkedList(m_list, newBuilding, 1);
		}
		else
		{
			// Add this worker at the end of the list
			AddElementToLinkedList(m_list, newBuilding, -1);
		}
	}
}