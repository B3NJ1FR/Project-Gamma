#include "Storehouse.h"

Storehouse::Storehouse()
{

}

Storehouse::~Storehouse()
{

}


void Storehouse::InitialisationStorehouse(Buildings *_specificBuildingConcerned)
{
	std::cout << "List before : " << m_list << std::endl;

	m_list = LinkedListInitialisation();

	std::cout << "List " << m_list << " Size : " << m_list->size << " Real First : " << m_list->first << " & Last : " << m_list->last << std::endl << std::endl;

	m_building = _specificBuildingConcerned;

}


void Storehouse::AddNewBuildingToList(sf::Vector2f _mapPosition)
{
	LinkedListClass::sElement* newStorehouse = new LinkedListClass::sElement;
	newStorehouse->data = new Storehouse::sStorehouseData;

	// Save the position in map
	((Storehouse::sStorehouseData *)newStorehouse->data)->mapPosition = _mapPosition;

	// Init of the building construction status after being placed on map
	((Storehouse::sStorehouseData *)newStorehouse->data)->constructionState = PLANNED;


	((Storehouse::sStorehouseData *)newStorehouse->data)->lifeTime = RESET;

	((Storehouse::sStorehouseData *)newStorehouse->data)->isChangingSprite = false;
	((Storehouse::sStorehouseData *)newStorehouse->data)->hasBeenBuilt = false;
	((Storehouse::sStorehouseData *)newStorehouse->data)->isWorkerThere = false;

	((Storehouse::sStorehouseData *)newStorehouse->data)->maximalQuantity = m_building->GetRessourceQuantityNeeded();
	((Storehouse::sStorehouseData *)newStorehouse->data)->internalRessourceCounter = RESET;

	newStorehouse->status = ELEMENT_ACTIVE;

	// Add this new vine at the end of the list
	AddElementToLinkedList(m_list, newStorehouse, -1);

	//ReadVineLinkedList();
	//ReadLinkedList(list);
}


void Storehouse::UpdateInternalCycles(const float &_frametime, Ressources *_ressourceSent)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Storehouse::sStorehouseData *)currentElement->data)->constructionState == BUILT)
				{
					// Filling of the building
					if (_ressourceSent->GetQuantityOwned() - 1 >= 0
						&& ((Storehouse::sStorehouseData *)currentElement->data)->internalRessourceCounter + 1 <= ((Storehouse::sStorehouseData *)currentElement->data)->maximalQuantity)
					{
						_ressourceSent->AddOrSubtractQuantityOwned(-1);
						((Storehouse::sStorehouseData *)currentElement->data)->internalRessourceCounter += 1;
					}
				}
			}
		}
	}
}

void Storehouse::UpdateBuildingConstruction(const float &_frametime)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			//std::cout << "Time : " << _lapsedFrameTime << std::endl;

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				switch (((Storehouse::sStorehouseData *)currentElement->data)->constructionState)
				{
				case PLANNED:
					// Plannification of the building construction

					// If we've the workers accredited to the building construction, we can pass the state to CONSTRUCTION
					//std::cout << "Planned ! " << std::endl;

					if (((Storehouse::sStorehouseData *)currentElement->data)->isWorkerThere == true)
					{
						//std::cout << "Building launched ! " << ((Storehouse::sStorehouseData *)currentElement->data)->lifeTime << " " << building->GetConstructionTimeCost() << std::endl;
						((Storehouse::sStorehouseData *)currentElement->data)->constructionState = CONSTRUCTION;
						((Storehouse::sStorehouseData *)currentElement->data)->lifeTime = RESET;
					}

					break;
				case CONSTRUCTION:

					if (((Storehouse::sStorehouseData *)currentElement->data)->isWorkerThere == true)
					{
						((Storehouse::sStorehouseData *)currentElement->data)->lifeTime += _frametime;
						((Storehouse::sStorehouseData *)currentElement->data)->isWorkerThere = false;
					}

					// If the building life is higher than the construction time, we launch it's growthing
					if (((Storehouse::sStorehouseData *)currentElement->data)->lifeTime >= m_building->GetConstructionTimeCost())
					{
						//std::cout << "Building built ! " << ((Storehouse::sStorehouseData *)currentElement->data)->lifeTime << " " << building->GetConstructionTimeCost() << std::endl;
						((Storehouse::sStorehouseData *)currentElement->data)->constructionState = BUILT;
						((Storehouse::sStorehouseData *)currentElement->data)->isChangingSprite = true;
					}

					break;
				case BUILT:
					if (((Storehouse::sStorehouseData *)currentElement->data)->hasBeenBuilt == false)
					{
						((Storehouse::sStorehouseData *)currentElement->data)->isChangingSprite = true;
						std::cout << "Building successfully constructed ! " << std::endl;
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


void Storehouse::UpdateBuildingSprite(unsigned short ***_map)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Storehouse::sStorehouseData *)currentElement->data)->isChangingSprite == true)
				{
					if (((Storehouse::sStorehouseData *)currentElement->data)->constructionState == BUILT
						&& ((Storehouse::sStorehouseData *)currentElement->data)->hasBeenBuilt == false)
					{
						((Storehouse::sStorehouseData *)currentElement->data)->hasBeenBuilt = true;
						((Storehouse::sStorehouseData *)currentElement->data)->isChangingSprite = false;

						_map[3 + 2][(int)((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.y]
							[(int)((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.x] = 19;
					}
				}
			}
		}
	}
}


int Storehouse::GetNumberResourcesStocked(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Storehouse::sStorehouseData *)currentElement->data)->mapPosition == _mapPosition)
				{
					// Filling of the building
					return ((Storehouse::sStorehouseData *)currentElement->data)->internalRessourceCounter;
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

void Storehouse::AddNumberResourcesStocked(const sf::Vector2f &_mapPosition, const int &_quantity)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Storehouse::sStorehouseData *)currentElement->data)->mapPosition == _mapPosition)
				{
					((Storehouse::sStorehouseData *)currentElement->data)->internalRessourceCounter += _quantity;
				}
			}
		}
	}
}

//void Storehouse::UpdateBuildingProduction(Ressources *_ressource)
//{
//	if (list != nullptr)
//	{
//		if (list->first != nullptr)
//		{
//			for (LinkedListClass::sElement *currentElement = list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				// If the building has produced the ressources, we manage it
//				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
//				{
//					// Add quantity produced to the ressource targeted
//					_ressource->AddQuantityOwned(building->GetRessourceQuantityProduced());
//
//					// Launch the feedback animation of producing
//
//
//					((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = false;
//				}
//			}
//		}
//	}
//}

bool Storehouse::ConfirmStorehousePresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates, const bool &_thisIsAWorker)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building
				if (_mapPosition.x <= ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					if (_isPreciseCoordinates)
					{
						if (_mapPosition == ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition)
						{
							if (_thisIsAWorker)
							{
								((Storehouse::sStorehouseData *)currentElement->data)->isWorkerThere = true;
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
							((Storehouse::sStorehouseData *)currentElement->data)->isWorkerThere = true;
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


void Storehouse::WorkerLeavingThisPosition(const sf::Vector2f& _mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((Storehouse::sStorehouseData*)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((Storehouse::sStorehouseData*)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((Storehouse::sStorehouseData*)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((Storehouse::sStorehouseData*)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					((Storehouse::sStorehouseData*)currentElement->data)->isWorkerThere = false;
				}
			}
		}
	}
}


bool Storehouse::GetWorkerIsThere(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition.x <= ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.x
					&& _mapPosition.x >= ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.x - m_building->GetSize().x
					&& _mapPosition.y <= ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.y
					&& _mapPosition.y >= ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition.y - m_building->GetSize().y)
				{
					return ((Storehouse::sStorehouseData *)currentElement->data)->isWorkerThere;
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

bool Storehouse::CheckStorehouseHasBeenBuilt(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((Storehouse::sStorehouseData *)currentElement->data)->constructionState == PLANNED
						|| ((Storehouse::sStorehouseData *)currentElement->data)->constructionState == CONSTRUCTION)
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
//bool Storehouse::CheckSpecificBuildingHasProducedRessource(const sf::Vector2f &_mapPosition)
//{
//	if (list != nullptr)
//	{
//		if (list->first != nullptr)
//		{
//			for (LinkedListClass::sElement *currentElement = list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition)
//				{
//					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
//					{
//						return true;
//					}
//					else
//					{
//						return false;
//					}
//				}
//			}
//
//			return false;
//
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else
//	{
//		return false;
//	}
//}


int Storehouse::SpecificsBuildingsSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				//// If the building has produced the ressources, we manage it
				//if (((Storehouse::sStorehouseData *)currentElement->data)->mapPosition == _mapPosition)
				//{
				//	if (((Storehouse::sStorehouseData *)currentElement->data)->isProduced == true)
				//	{
				//		((Storehouse::sStorehouseData *)currentElement->data)->secondaryTime += _frametime;

				//		if (((Storehouse::sStorehouseData *)currentElement->data)->secondaryTime >= building->GetPickupingTimeCost())
				//		{
				//			// Launch the feedback animation of producing

				//			((Storehouse::sStorehouseData *)currentElement->data)->isProduced = false;

				//			((Storehouse::sStorehouseData *)currentElement->data)->secondaryTime = RESET;

				//			return building->GetRessourceQuantityProduced();
				//		}
				//		else
				//		{
				//			return 0;
				//		}
				//	}
				//	else
				//	{
				//		return 0;
				//	}
				//}

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


sf::Vector2i Storehouse::StorehouseFindNearestBuilding(const sf::Vector2f &_mapPosition)
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
					float distance = DistanceFormula(_mapPosition, ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition);

					lastLowerDistance = distance;
					buildingPosition = (sf::Vector2i)((Storehouse::sStorehouseData *)currentElement->data)->mapPosition;
				}
				else
				{
					float distance = DistanceFormula(_mapPosition, ((Storehouse::sStorehouseData *)currentElement->data)->mapPosition);

					if (distance < lastLowerDistance)
					{
						lastLowerDistance = distance;
						buildingPosition = (sf::Vector2i)((Storehouse::sStorehouseData *)currentElement->data)->mapPosition;
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


bool Storehouse::DestroyedBuildingSelected(const sf::Vector2f &_mapPosition)
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
				if (((Storehouse::sStorehouseData *)currentElement->data)->mapPosition == _mapPosition
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




void Storehouse::SavingVinesListForFile(std::ofstream *_file)
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
				_file->write((char *)(Storehouse::sStorehouseData *)currentElement->data, sizeof(sStorehouseData));
			}
		}
	}
}


void Storehouse::LoadingVinesListFromFile(std::ifstream *_file)
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
		LinkedListClass::sElement* newStorehouse = new LinkedListClass::sElement;
		newStorehouse->data = new Storehouse::sStorehouseData;

		_file->read((char *)(Storehouse::sStorehouseData *)newStorehouse->data, sizeof(sStorehouseData));

		newStorehouse->status = ELEMENT_ACTIVE;

		if (i == 0)
		{
			// Add this worker at the top of the list
			AddElementToLinkedList(m_list, newStorehouse, 1);
		}
		else
		{
			// Add this worker at the end of the list
			AddElementToLinkedList(m_list, newStorehouse, -1);
		}
	}
}