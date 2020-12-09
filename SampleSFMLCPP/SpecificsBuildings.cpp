#include "SpecificsBuildings.h"
#include "GameDefinitions.h"



SpecificsBuildings::SpecificsBuildings()
{
	this->workerIsThereSprite = LoadSprite("Data/Assets/Sprites/Entities/worker_working.png", 1);
}


SpecificsBuildings::~SpecificsBuildings()
{
}

void SpecificsBuildings::InitialisationSpeBuilding(Buildings *_specificBuildingConcerned)
{
	std::cout << "List before : " << this->list << std::endl;

	this->list = LinkedListInitialisation();

	std::cout << "List " << this->list << " Size : " << this->list->size << " Real First : " << this->list->first << " & Last : " << this->list->last << std::endl << std::endl;

	this->building = _specificBuildingConcerned;

}


void SpecificsBuildings::AddNewBuildingToList(sf::Vector2f _mapPosition)
{
	LinkedListClass::sElement* newBuilding = new LinkedListClass::sElement;
	newBuilding->data = new SpecificsBuildings::sBuildingData;

	// Save the position in map
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->mapPosition = _mapPosition;

	// Init of the building construction status after being placed on map
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->constructionState = PLANNED;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->actualState = BUILDING_READY_TO_PRODUCE;

	// A MODIFIER PAR VALEUR SEUIL
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->quantitativeThreshold = this->building->GetRessourceQuantityNeeded();
	// A CONFIGURER
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->maximalQuantity = 5;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->internalImportRessourceCounter = RESET;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->internalExportRessourceCounter = RESET;

	((SpecificsBuildings::sBuildingData *)newBuilding->data)->lifeTime = RESET;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->actualProductionTime = RESET;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->secondaryTime = RESET;

	((SpecificsBuildings::sBuildingData *)newBuilding->data)->isChangingSprite = false;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->hasBeenBuilt = false;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->isProduced = false;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->isWorkerThere = false;

	newBuilding->status = ELEMENT_ACTIVE;

	// Add this new vine at the end of the list
	this->AddElementToLinkedList(this->list, newBuilding, -1);

	//this->ReadVineLinkedList();
	//this->ReadLinkedList(this->list);
}


void SpecificsBuildings::UpdateInternalCycles(const float &_frametime, Ressources *_ressourceSent, Ressources *_ressourceProduced)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
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
							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualProductionTime += _frametime;

							if (((SpecificsBuildings::sBuildingData *)currentElement->data)->actualProductionTime > this->building->GetProductionTimeCost())
							{
								((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_COLLECTING_PRODUCTION;
								((SpecificsBuildings::sBuildingData *)currentElement->data)->actualProductionTime = RESET;
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
			//std::cout << "List : " << this->list->first << std::endl;
		}
	}
}

void SpecificsBuildings::UpdateBuildingConstruction(const float &_frametime)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			//std::cout << "Time : " << _lapsedFrameTime << std::endl;

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
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
						//std::cout << "Building launched ! " << ((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime << " " << this->building->GetConstructionTimeCost() << std::endl;
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
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime >= this->building->GetConstructionTimeCost())
					{
						//std::cout << "Building built ! " << ((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime << " " << this->building->GetConstructionTimeCost() << std::endl;
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
			//std::cout << "List : " << this->list->first << std::endl;
		}
	}
}


void SpecificsBuildings::UpdateBuildingSprite(unsigned short ***_map, const enum TypeOfBuilding &_building)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
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
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
				{
					//// Add quantity produced to the ressource targeted
					//_ressource->AddQuantityOwned(this->building->GetRessourceQuantityProduced());

					//// Launch the feedback animation of producing


					//((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = false;
				}
			}
		}
	}
}


bool SpecificsBuildings::GetWorkerIsThere(const sf::Vector2f &_mapPosition)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x - this->building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y - this->building->GetSize().y)
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

sf::Sprite SpecificsBuildings::GetSpriteWorkerIsThere()
{
	return this->workerIsThereSprite;
}

bool SpecificsBuildings::ConfirmSpecificBuildingPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates, const bool &_thisIsAWorker)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x - this->building->GetSize().x
					&& _mapPosition.y <= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y - this->building->GetSize().y)
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


bool SpecificsBuildings::CheckSpecificBuildingHasProducedRessource(const sf::Vector2f &_mapPosition)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
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
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
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

int SpecificsBuildings::SpecificsBuildingsSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
					{
						((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime += _frametime;

						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime >= this->building->GetPickupingTimeCost())
						{
							// Launch the feedback animation of producing

							((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = true;
							((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = false;

							((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime = RESET;

							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_NEED_TO_BE_CLEANED;

							return this->building->GetRessourceQuantityProduced() * ((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter;
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


sf::Vector2i SpecificsBuildings::SpecificsBuildingsFindNearestBuilding(const sf::Vector2f &_mapPosition)
{
	sf::Vector2i buildingPosition = { RESET, RESET };

	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			float lastLowerDistance(RESET);

			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (currentElement == this->list->first)
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
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			int positionCounter(1);
			bool isBuildingFind(false);

			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				//std::cout << "Map : " << positionCounter << "/" << this->list->size << " -> "<< ((Vines::sVines *)currentElement->data)->mapPosition.x << " " << ((Vines::sVines *)currentElement->data)->mapPosition.y << std::endl;

				// If the building position is identical to which send, we save his position in the linked list
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition
					&& isBuildingFind == false)
				{
					isBuildingFind = true;
				}

				if (isBuildingFind == false)
				{
					positionCounter++;
				}
			}

			//std::cout << std::endl;

			// After having saved the building's position, we ask to destroy it
			if (isBuildingFind == true)
			{
				RemoveElementsOfLinkedList(this->list, true, positionCounter);

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
	_file->write((char *)&this->list->size, sizeof(int));

	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				_file->write((char *)(SpecificsBuildings::sBuildingData *)currentElement->data, sizeof(sBuildingData));
			}
		}
	}
}


void SpecificsBuildings::LoadingSpecificsBuildingsListFromFile(std::ifstream *_file)
{
	// Delete every vines
	if (this->list != nullptr)
	{
		this->FreeLinkedList(this->list);
	}


	// We reinit the vines list
	this->list = LinkedListInitialisation();


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
			this->AddElementToLinkedList(this->list, newBuilding, 1);
		}
		else
		{
			// Add this worker at the end of the list
			this->AddElementToLinkedList(this->list, newBuilding, -1);
		}
	}
}