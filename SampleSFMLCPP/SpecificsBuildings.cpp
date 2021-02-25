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
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We remove all storages
				if (((sBuildingData*)currentElement->data)->storage != nullptr)
				{
					// Delete the storage from the list
					delete ((sBuildingData*)currentElement->data)->storage;
					((sBuildingData*)currentElement->data)->storage = nullptr;
				}
			}
		}
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

void SpecificsBuildings::AddNewBuildingToList(sf::Vector2f _mapPosition, enum TypesOfRessources _ressource)
{
	LinkedListClass::sElement* newBuilding = new LinkedListClass::sElement;
	newBuilding->data = new sBuildingData;

	// Save the position in map
	((sBuildingData *)newBuilding->data)->mapPosition = _mapPosition;

	// Init of the building construction status after being placed on map
	((sBuildingData *)newBuilding->data)->constructionState = BuildingStatus::PLANNED;
	((sBuildingData *)newBuilding->data)->actualState = MainBuildingStatus::BUILDING_READY_TO_PRODUCE;

	// Allocation of the storage
	((sBuildingData *)newBuilding->data)->storage = new Storage(1, _ressource);
	((sBuildingData *)newBuilding->data)->storage->SetName("Specific");

	// A MODIFIER PAR VALEUR SEUIL
	((sBuildingData *)newBuilding->data)->quantitativeThreshold = m_building->GetRessourceQuantityNeeded();
	// A CONFIGURER
	((sBuildingData *)newBuilding->data)->maximalQuantity = 5;
	((sBuildingData *)newBuilding->data)->internalImportRessourceCounter = RESET;
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

	newBuilding->status = ELEMENT_ACTIVE;

	// Add this new vine at the end of the list
	AddElementToLinkedList(m_list, newBuilding, -1);

	//ReadVineLinkedList();
	//ReadLinkedList(list);
}


void SpecificsBuildings::UpdateInternalCycles(const float &_frametime, Ressources *_ressourceSent, Ressources *_ressourceProduced)
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
					switch (((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState)
					{
					case BUILDING_READY_TO_PRODUCE:

						// If the workers are there, we launch the next state
						if (_ressourceSent->GetQuantityOwned() > ((SpecificsBuildings::sBuildingData *)currentElement->data)->quantitativeThreshold)
						{
							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_FILLING;
						}

						break;
					case BUILDING_FILLING:

						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere == true)
						{
							if (_ressourceSent->GetQuantityOwned() - 1 >= 0)
							{
								_ressourceSent->AddOrSubtractQuantityOwned(-1);
								((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter += 1;
							}
						}

						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->maximalQuantity)
						{
							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_WORKS;
						}

						break;
					case BUILDING_WORKS:

						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere == true)
						{
							// We verify the number of worker present in the building is higher or equal than which is needed
							if (((SpecificsBuildings::sBuildingData*)currentElement->data)->currentNumberOfWorkersPresent
								>= ((SpecificsBuildings::sBuildingData*)currentElement->data)->numberOfWorkersNeededToWorks)
							{
								((SpecificsBuildings::sBuildingData*)currentElement->data)->actualProductionTime += _frametime;

								if (((SpecificsBuildings::sBuildingData*)currentElement->data)->actualProductionTime > m_building->GetProductionTimeCost())
								{
									((SpecificsBuildings::sBuildingData*)currentElement->data)->actualState = BUILDING_COLLECTING_PRODUCTION;
									((SpecificsBuildings::sBuildingData*)currentElement->data)->actualProductionTime = RESET;
								}
							}
						}

						break;
					case BUILDING_COLLECTING_PRODUCTION:
						
						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter > 0
							&& ((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere == true)
						{
							((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = true;
						}

						break;
					case BUILDING_NEED_TO_BE_CLEANED:

						((SpecificsBuildings::sBuildingData  *)currentElement->data)->isChangingSprite = true;
						((SpecificsBuildings::sBuildingData  *)currentElement->data)->isWorkerThere = false;

						((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter = RESET;

						((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_READY_TO_PRODUCE;

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

void SpecificsBuildings::UpdateBuildingConstruction(const float &_frametime)
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
						((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime += _frametime;
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


void SpecificsBuildings::UpdateBuildingSprite(unsigned short ***_map, const enum TypeOfBuilding &_building)
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
						((SpecificsBuildings::sBuildingData *)currentElement->data)->hasBeenBuilt = true;
						((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = false;

						switch (_building)
						{
						case BUILDING_GRAPE_STOMPING_VATS:

							_map[FIRST_FLOOR + SPRITE_ID][(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y]
								[(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x] = 4;
							break;
						case BUILDING_WINE_PRESS:
							_map[FIRST_FLOOR + SPRITE_ID][(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y]
								[(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x] = 17;
							break;
						case BUILDING_WINE_STOREHOUSE:
							_map[FIRST_FLOOR + SPRITE_ID][(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y]
								[(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x] = 16;
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}


void SpecificsBuildings::UpdateBuildingProduction(Ressources *_ressource)
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
					//// Add quantity produced to the ressource targeted
					//_ressource->AddQuantityOwned(building->GetRessourceQuantityProduced());

					//// Launch the feedback animation of producing


					//((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = false;
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
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
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

bool SpecificsBuildings::UpdateRessourcePickuping(const sf::Vector2f &_mapPosition, const float &_frametime, enum TypesOfRessources _ressource)
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
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
					{
						((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime += _frametime;

						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime >= m_building->GetPickupingTimeCost())
						{
							// Launch the feedback animation of producing

							((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = true;
							((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = false;

							((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime = RESET;

							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_NEED_TO_BE_CLEANED;

							return m_building->GetRessourceQuantityProduced() * ((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter;

							int quantityProduced = m_building->GetRessourceQuantityProduced();

							((SpecificsBuildings::sBuildingData *)currentElement->data)->storage->AddOrSubtractResource(Ressources::GetNameFromEnum(_ressource), quantityProduced);
							return true;
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
				//std::cout << "Map : " << positionCounter << "/" << list->size << " -> "<< ((Vines::sVines *)currentElement->data)->mapPosition.x << " " << ((Vines::sVines *)currentElement->data)->mapPosition.y << std::endl;

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
			}
		}
	}
}


void SpecificsBuildings::LoadingSpecificsBuildingsListFromFile(std::ifstream *_file)
{
	// Delete every vines
	if (m_list != nullptr)
	{
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