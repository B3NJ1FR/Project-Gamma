#include "SpecificsBuildings.h"



SpecificsBuildings::SpecificsBuildings()
{
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


//void Vines::ReadVineLinkedList()
//{
//	if (this->list != nullptr)
//	{
//		if (this->list->first != nullptr)
//		{
//			LinkedListClass::sElement *currentElement = this->list->first;
//
//			int positionCounter(1);
//
//			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				std::cout << "Vine : " << positionCounter << "/" << this->list->size << "  -  Position : " << ((Vines::sVines *)currentElement->data)->mapPosition.x << " " << ((Vines::sVines *)currentElement->data)->mapPosition.y << std::endl;
//				positionCounter++;
//			}
//
//			std::cout << std::endl << std::endl << std::endl;
//		}
//	}
//}

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

	((SpecificsBuildings::sBuildingData *)newBuilding->data)->isChangingSprite = false;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->isProduced = false;
	((SpecificsBuildings::sBuildingData *)newBuilding->data)->isWorkerThere = false;

	newBuilding->status = ELEMENT_ACTIVE;

	// Add this new vine at the end of the list
	this->AddElementToLinkedList(this->list, newBuilding, -1);

	//this->ReadVineLinkedList();
	this->ReadLinkedList(this->list);
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

						if (_ressourceSent->GetQuantityOwned() - 1 > 0)
						{
							_ressourceSent->SubtractQuantityOwned(1);
							((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter += 1;
						}

						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter >= ((SpecificsBuildings::sBuildingData *)currentElement->data)->maximalQuantity)
						{
							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_WORKS;
						}

						break;
					case BUILDING_WORKS:

						((SpecificsBuildings::sBuildingData *)currentElement->data)->actualProductionTime += _frametime;

						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->actualProductionTime > this->building->GetProductionTimeCost())
						{
							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_COLLECTING_PRODUCTION;
							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualProductionTime = RESET;
						}

						break;
					case BUILDING_COLLECTING_PRODUCTION:
						
						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter > 0)
						{
							_ressourceProduced->AddQuantityOwned(1);
							((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter -= 1;
						}
						else if (((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter == 0)
						{
							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_NEED_TO_BE_CLEANED;
							((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter = RESET;

						}
						else if (((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter < 0)
						{
							// ERROR LOG
							((SpecificsBuildings::sBuildingData *)currentElement->data)->actualState = BUILDING_NEED_TO_BE_CLEANED;
							((SpecificsBuildings::sBuildingData *)currentElement->data)->internalImportRessourceCounter = RESET;
						}

						break;
					case BUILDING_NEED_TO_BE_CLEANED:

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
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->constructionState != BUILT)
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
							std::cout << "Building launched ! " << ((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime << " " << this->building->GetConstructionTimeCost() << std::endl;
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

						// If the building life is higher than the construction time, we launch it's growthing
						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime >= this->building->GetConstructionTimeCost())
						{
							std::cout << "Building built ! " << ((SpecificsBuildings::sBuildingData *)currentElement->data)->lifeTime << " " << this->building->GetConstructionTimeCost() << std::endl;
							((SpecificsBuildings::sBuildingData *)currentElement->data)->constructionState = BUILT;
							((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = true;
						}

						break;
					case BUILT:

						break;
					case BUILDING_NOT_MAINTAINED:
						break;

					default:
						// ERROR LOG
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


void SpecificsBuildings::UpdateBuildingSprite(unsigned short ***_map)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				/*if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite == true)
				{
					switch (((SpecificsBuildings::sBuildingData *)currentElement->data)->generalState)
					{
					case PLANTED:
						break;
					case THREE_YEARS_GROWTHING:
						_map[3 + 2][(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y]
							[(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x] = 8;
						break;
					case READY_TO_PRODUCE:
						_map[3 + 2][(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y]
							[(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x] = 9;
						break;
					case NOT_MAINTAINED:
						break;
					case TOO_OLD:
						break;
					default:
						break;
					}

					((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = false;
				}*/
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
					// Add quantity produced to the ressource targeted
					_ressource->AddQuantityOwned(this->building->GetRessourceQuantityProduced());

					// Launch the feedback animation of producing


					((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = false;
				}
			}
		}
	}
}

bool SpecificsBuildings::ConfirmSpecificBuildingPresenceAtWorkerPosition(const sf::Vector2f &_mapPosition)
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
					((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
					return true;
				}
				else if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x - 1 == _mapPosition.x
					&& ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y == _mapPosition.y)
				{
					((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
					return true;
				}
				else if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x - 1 == _mapPosition.x
					&& ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y - 1 == _mapPosition.y)
				{
					((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
					return true;
				}
				else if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x == _mapPosition.x
					&& ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y - 1== _mapPosition.y)
				{
					((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
					return true;
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


int SpecificsBuildings::SpecificsBuildingsSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition)
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
						// Launch the feedback animation of producing

						((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = false;


						return this->building->GetRessourceQuantityProduced();
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
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			float lastLowerDistance(RESET);
			sf::Vector2i buildingPosition = { RESET, RESET };

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
	}
}