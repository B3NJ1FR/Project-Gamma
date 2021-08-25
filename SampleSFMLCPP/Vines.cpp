#include "Vines.h"
#include "GameDefinitions.h"
#include "ErrorsLogFile.h"
#include "TimeManagement.h"
#include "RessourcesManager.h"
#include "ListOfAnnualProductions.h"



Vines::Vines()
{
	m_list = nullptr;
	m_vineBuilding = nullptr;
}

Vines::~Vines()
{
	ClearStorages();
}


Storage* Vines::GetStorage(const sf::Vector2f& _mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (((Vines::sVines*)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((Vines::sVines*)currentElement->data)->storage != nullptr)
					{
						return ((Vines::sVines*)currentElement->data)->storage;
					}
				}
			}
		}
	}

	return nullptr;
}

void Vines::InitialisationVines(Buildings *_vine)
{
	std::cout << "List before : " << m_list << std::endl;

	m_list = LinkedListInitialisation();

	std::cout << "List " << m_list << " Size : " << m_list->size << " Real First : " << m_list->first << " & Last : " << m_list->last << std::endl;

	m_vineBuilding = _vine;
}

void Vines::ReadVineLinkedList()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			int positionCounter(1);

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				//std::cout << "Vine : " << positionCounter << "/" << list->size << "  -  Position : " << ((Vines::sVines *)currentElement->data)->mapPosition.x << " " << ((Vines::sVines *)currentElement->data)->mapPosition.y << std::endl;
				positionCounter++;
			}

			//std::cout << std::endl << std::endl << std::endl;
		}
	}
}

void Vines::AddNewVineToList(sf::Vector2f _mapPosition)
{
	LinkedListClass::sElement* newVine = new LinkedListClass::sElement;
	newVine->data = new Vines::sVines;

	// Save the position in map
	((Vines::sVines *)newVine->data)->mapPosition = _mapPosition;

	// Init of the vine life
	((Vines::sVines *)newVine->data)->generalState = PLANTED;


	// En fonction de la période actuelle, mettre l'annuel state concerné en conséquence
	((Vines::sVines *)newVine->data)->annualState = NEED_PRUNE;
	((Vines::sVines *)newVine->data)->internalState = InternalState::STATE_INIT;
	((Vines::sVines *)newVine->data)->previousMonth = TimeManagement::GetSingleton()->GetCurrentMonth();

	// Allocation of the storage
	((Vines::sVines *)newVine->data)->storage = new Storage();
	((Vines::sVines *)newVine->data)->storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(BUNCH_OF_GRAPE), ResourceData::RESOURCE_PRODUCED);
	((Vines::sVines *)newVine->data)->storage->SetName("Vines");

	((Vines::sVines *)newVine->data)->lifeTime = RESET;
	((Vines::sVines *)newVine->data)->actualProductionTime = RESET;
	((Vines::sVines *)newVine->data)->secondaryTime = RESET;

	((Vines::sVines *)newVine->data)->isChangingSprite = false;
	((Vines::sVines *)newVine->data)->isProduced = false;
	((Vines::sVines *)newVine->data)->isWorkerThere = false;
	((Vines::sVines *)newVine->data)->isProdCanBeCollected = false;


	((Vines::sVines *)newVine->data)->isPruned = false;
	((Vines::sVines *)newVine->data)->isPloughed = false;
	((Vines::sVines *)newVine->data)->isWeeded = false;
	((Vines::sVines *)newVine->data)->isCared = false;

	((Vines::sVines *)newVine->data)->numberOfWorkersNeededToWorks = m_vineBuilding->GetNumberWorkersNeeded();
	((Vines::sVines *)newVine->data)->currentNumberOfWorkersPresent = 0;

	newVine->status = ELEMENT_ACTIVE;

	// Add this new vine at the end of the list
	AddElementToLinkedList(m_list, newVine, -1);

	//ReadVineLinkedList();
	//ReadLinkedList(list);
}

void Vines::UpdateVineLife()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			TimeManagement* pTimeManagement = TimeManagement::GetSingleton();

			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				Vines::sVines* vineData = ((Vines::sVines*)currentElement->data);

				switch (vineData->generalState)
				{
				case PLANTED:

					if (vineData->isWorkerThere == true)
					{
						vineData->lifeTime += pTimeManagement->GetFrameTime();
						vineData->isWorkerThere = false;
					}

					// If the vine life is higher than the construction time, we launch it's growthing
					if (vineData->lifeTime >= m_vineBuilding->GetConstructionTimeCost())
					{
						//std::cout << "Vines built ! " << vineData->lifeTime << " " << vineBuilding->GetConstructionTimeCost() << std::endl;
						vineData->generalState = THREE_YEARS_GROWTHING;
						vineData->isChangingSprite = true;
						vineData->isWorkerThere = false;
					}

					break;
				case THREE_YEARS_GROWTHING:

					vineData->lifeTime += pTimeManagement->GetFrameTime();
					
					// If the vine life is higher than the three years of growthing, we call that this is ready to produce
					if (vineData->lifeTime >= VINE_LIFE_3Y_GROWTHING_TO_READY_TO_PRODUCE)
					{
						//std::cout << "Vines ready to produce !\n\n";
						vineData->generalState = READY_TO_PRODUCE;
						vineData->isChangingSprite = true;
						vineData->annualState = CARED; // Temporaire
						vineData->isWorkerThere = false;
					}

					break;
				case READY_TO_PRODUCE:

					vineData->lifeTime += pTimeManagement->GetFrameTime();

					if (vineData->previousMonth != pTimeManagement->GetCurrentMonth())
					{
						vineData->previousMonth = pTimeManagement->GetCurrentMonth();

						switch (vineData->previousMonth)
						{
						case IANUARIUS:
							vineData->annualState = NEED_PRUNE;
							vineData->internalState = InternalState::STATE_INIT;
							break;
						case FEBRUARIUS:
							break;
						case MARTIUS:
							vineData->annualState = NEED_PLOUGHING;
							vineData->internalState = InternalState::STATE_INIT;
							break;
						case APRILIS:
							break;
						case MAïUS:
							vineData->annualState = NEED_WEEDING;
							vineData->internalState = InternalState::STATE_INIT;
							break;
						case IUNIUS:
							break;
						case QUINTILIS:
							vineData->annualState = NEED_CARE;
							vineData->internalState = InternalState::STATE_INIT;
							break;
						case SEXTILIS:
							break;
						case SEPTEMBER:
							vineData->annualState = NEED_HARVEST;
							vineData->internalState = InternalState::STATE_INIT;
							break;
						case OCTOBER:
							break;
						case NOVEMBER:
							// If the production hasn't been recolted, the grapes are rotting
							if (vineData->annualState == NEED_HARVEST)
							{
								vineData->annualState = ROTTEN_HARVESTS;
								vineData->internalState = InternalState::STATE_INIT;
							}
							break;
						case DECEMBER:
							break;
						default:
							break;
						}
					}
					



					switch (vineData->annualState)
					{
					case NEED_PRUNE:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								if (vineData->isWorkerThere == true)
								{
									// Changer de status
									vineData->isPruned = true;
									vineData->annualState = PRUNED;
									vineData->internalState = InternalState::STATE_INIT;
								}
								break;
							default:
								break;
							}

						break;
					case PRUNED:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								vineData->internalState = InternalState::STATE_EXIT;
								break;
							case InternalState::STATE_EXIT:

								vineData->internalState = InternalState::STATE_WAITING;
								break;
							case InternalState::STATE_WAITING:

								break;
							default:
								break;
							}
						break;
					case NEED_PLOUGHING:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								if (vineData->isWorkerThere == true)
								{
									// Changer de status
									vineData->isPloughed = true;
									vineData->annualState = PLOUGHED;
									vineData->internalState = InternalState::STATE_INIT;
								}
								break;
							default:
								break;
							}

						break;
					case PLOUGHED:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								vineData->internalState = InternalState::STATE_EXIT;
								break;
							case InternalState::STATE_EXIT:

								vineData->internalState = InternalState::STATE_WAITING;
								break;
							case InternalState::STATE_WAITING:

								break;
							default:
								break;
							}

						break;
					case NEED_WEEDING:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								if (vineData->isWorkerThere == true)
								{
									// Changer de status
									vineData->isWeeded = true;
									vineData->annualState = WEEDED;
									vineData->internalState = InternalState::STATE_INIT;
								}
								break;
							default:
								break;
							}

						break;
					case WEEDED:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								vineData->internalState = InternalState::STATE_EXIT;
								break;
							case InternalState::STATE_EXIT:

								vineData->internalState = InternalState::STATE_WAITING;
								break;
							case InternalState::STATE_WAITING:

								break;
							default:
								break;
							}
						break;
					case NEED_CARE:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								if (vineData->isWorkerThere == true)
								{
									// Changer de status
									vineData->isCared = true;
									vineData->annualState = CARED;
									vineData->internalState = InternalState::STATE_INIT;
								}
								break;
							default:
								break;
							}
						break;
					case CARED:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								vineData->internalState = InternalState::STATE_EXIT;
								break;
							case InternalState::STATE_EXIT:

								vineData->internalState = InternalState::STATE_WAITING;
								break;
							case InternalState::STATE_WAITING:

								break;
							default:
								break;
							}

						break;
					case NEED_HARVEST:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->secondaryTime = RESET;
								vineData->actualProductionTime = RESET;

								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								if (vineData->isWorkerThere == true)
								{
									vineData->secondaryTime += pTimeManagement->GetFrameTime();

									if (vineData->secondaryTime >= m_vineBuilding->GetPickupingTimeCost())
									{
										vineData->secondaryTime = RESET;
										vineData->isProduced = true;

										vineData->annualState = HARVESTED;
										vineData->internalState = InternalState::STATE_INIT;
									}
								}

								break;
							default:
								break;
							}

						break;
					case HARVESTED:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;
								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								vineData->internalState = InternalState::STATE_EXIT;
								break;
							case InternalState::STATE_EXIT:

								// Reset of the monthly states for this current year
								vineData->isPruned = false;
								vineData->isPloughed = false;
								vineData->isWeeded = false;
								vineData->isCared = false;

								vineData->internalState = InternalState::STATE_WAITING;
								break;
							case InternalState::STATE_WAITING:

								break;
							default:
								break;
							}

						break;
					case ROTTEN_HARVESTS:
						
							switch (vineData->internalState)
							{
							case InternalState::STATE_INIT:

								vineData->isChangingSprite = true;

								// Reset of the monthly states for this current year
								vineData->isPruned = false;
								vineData->isPloughed = false;
								vineData->isWeeded = false;
								vineData->isCared = false;

								vineData->internalState = InternalState::STATE_UPDATE;
								break;
							case InternalState::STATE_UPDATE:

								vineData->internalState = InternalState::STATE_EXIT;
								break;
							case InternalState::STATE_EXIT:

								vineData->internalState = InternalState::STATE_WAITING;
								break;
							case InternalState::STATE_WAITING:

								break;
							default:
								break;
							}
						break;
					case PUTTING_IN_WINE_STOREHOUSE:
						break;
					default:
						break;
					}
				
					
					break;
				case NOT_MAINTAINED:
					break;
				case TOO_OLD:
					break;
				default:
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


void Vines::UpdateVineSprite()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Vines::sVines *)currentElement->data)->isChangingSprite == true)
				{
					if (((Vines::sVines *)currentElement->data)->generalState == THREE_YEARS_GROWTHING
						&& ((Vines::sVines *)currentElement->data)->hasBeenBuilt == false)
					{
						((Vines::sVines *)currentElement->data)->hasBeenBuilt = true;
						((Vines::sVines *)currentElement->data)->isChangingSprite = false;

						sf::Vector2i currentMapPosition = (sf::Vector2i)((Vines::sVines*)currentElement->data)->mapPosition;

						// Set the correct sprite id for this building
						Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 3;

						Map::GetSingleton()->GetMap()[FIRST_FLOOR + COLLISIONS_ID][currentMapPosition.y][currentMapPosition.x] = COLLISION;
					}
					else
					{
						sf::Vector2i currentMapPosition = (sf::Vector2i)((Vines::sVines*)currentElement->data)->mapPosition;

						switch (((Vines::sVines *)currentElement->data)->generalState)
						{
						case PLANTED:
							break;

						case THREE_YEARS_GROWTHING:
							Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 4;
							break;

						case READY_TO_PRODUCE:

							switch (((Vines::sVines *)currentElement->data)->annualState)
							{
							case NEED_PRUNE:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 6;
								break;

							case PRUNED:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 5;
								break;

							case NEED_PLOUGHING:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 7;
								break;

							case PLOUGHED:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 5;
								break;

							case NEED_WEEDING:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 8;
								break;

							case WEEDED:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 5;
								break;

							case NEED_CARE:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 9;
								break;

							case CARED:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 5;
								break;

							case NEED_HARVEST:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 10;
								break;

							case HARVESTED:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 5;
								break;

							case ROTTEN_HARVESTS:
								Map::GetSingleton()->GetMap()[FIRST_FLOOR + SPRITE_ID][currentMapPosition.y][currentMapPosition.x] = 5;
								break;

							case PUTTING_IN_WINE_STOREHOUSE:
								break;

							default:
								break;
							}


							break;
						case NOT_MAINTAINED:
							break;
						case TOO_OLD:
							break;
						default:
							break;
						}

						((Vines::sVines *)currentElement->data)->isChangingSprite = false;
					}

					
				}
			}
		}
	}
}


void Vines::UpdateVineProduction()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((Vines::sVines*)currentElement->data)->isProduced == true)
				{
					Storage* storage = ((Vines::sVines*)currentElement->data)->storage;
					std::vector<Ressources*> arrayOfResources;

					arrayOfResources = storage->GetResourceFromData(ResourceData::RESOURCE_PRODUCED);

					// DANS LE CAS DE 2 RESSOURCES OU PLUS PRODUITES, NE FONCTIONNERA PAS
					// A MODIFIER
					int quantityProduced = m_vineBuilding->GetRessourceQuantityProduced();

					// We verify that we have enough of each resource to start the production
					for (Ressources* resource : arrayOfResources)
					{
						// We add the resources created during this production
						resource->AddOrSubtractQuantityOwned(quantityProduced);
						ListOfAnnualProductions::GetSingleton()->AddResourceQuantityProduced(TimeManagement::GetSingleton()->GetCurrentYear(), Ressources::GetEnumFromName(resource->GetName()), quantityProduced);

						((Vines::sVines*)currentElement->data)->isProduced = false;
						((Vines::sVines*)currentElement->data)->isProdCanBeCollected = true;
						((Vines::sVines*)currentElement->data)->secondaryTime = RESET;
					}

					arrayOfResources.clear();

					// Launch the feedback animation of producing
				}
			}
		}
	}
}

bool Vines::IsBuildingIsWorking(const sf::Vector2f& _mapPosition) const
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (((Vines::sVines*)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((Vines::sVines*)currentElement->data)->currentNumberOfWorkersPresent
						>= ((Vines::sVines*)currentElement->data)->numberOfWorkersNeededToWorks)
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

bool Vines::ConfirmVinePresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_thisIsAWorker)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Vines::sVines *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (_thisIsAWorker)
					{
						((Vines::sVines *)currentElement->data)->isWorkerThere = true;
					}

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

void Vines::WorkerEnteringInThisPosition(const sf::Vector2f& _mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Vines::sVines*)currentElement->data)->mapPosition == _mapPosition)
				{
					((Vines::sVines*)currentElement->data)->currentNumberOfWorkersPresent += 1;

					break;
				}
			}
		}
	}
}
void Vines::WorkerLeavingThisPosition(const sf::Vector2f& _mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Vines::sVines*)currentElement->data)->mapPosition == _mapPosition)
				{
					((Vines::sVines*)currentElement->data)->currentNumberOfWorkersPresent -= 1;
					
					if (((Vines::sVines*)currentElement->data)->currentNumberOfWorkersPresent == 0)
					{
						((Vines::sVines*)currentElement->data)->isWorkerThere = false;
					}
					else if (((Vines::sVines*)currentElement->data)->currentNumberOfWorkersPresent < 0)
					{
						((Vines::sVines*)currentElement->data)->currentNumberOfWorkersPresent = 0;
					}

					break;
				}
			}
		}
	}
}

int Vines::GetNumberOfWorkersPresents(const sf::Vector2f& _mapPosition) const
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition == ((Vines::sVines*)currentElement->data)->mapPosition)
				{
					return ((Vines::sVines*)currentElement->data)->currentNumberOfWorkersPresent;
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

bool Vines::GetWorkerIsThere(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// We verify if the player location is between the origin and the max size of the building concerned
				if (_mapPosition == ((Vines::sVines *)currentElement->data)->mapPosition)
				{
					return ((Vines::sVines *)currentElement->data)->isWorkerThere;
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

bool Vines::CheckVineHasBeenBuilt(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Vines::sVines *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((Vines::sVines *)currentElement->data)->generalState == PLANTED)
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

bool Vines::CheckVineHasProducedRessource(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Vines::sVines *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((Vines::sVines *)currentElement->data)->isProdCanBeCollected == true)
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

bool Vines::UpdateRessourcePickuping(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((Vines::sVines *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((Vines::sVines *)currentElement->data)->isProdCanBeCollected == true)
					{
						((Vines::sVines *)currentElement->data)->secondaryTime += TimeManagement::GetSingleton()->GetFrameTime();

						if (((Vines::sVines *)currentElement->data)->secondaryTime >= m_vineBuilding->GetPickupingTimeCost())
						{
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

		return false;
	}
	
	return false;
}


// Check if the storage is still containing resources produced but not picked up 
// If it's the case, we leave the "isProdCanBeCollected" value to true to allow to an other worker to pick up the resources
void Vines::RessourcePickedUp(const sf::Vector2f& _mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((Vines::sVines*)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((Vines::sVines*)currentElement->data)->isProdCanBeCollected == true)
					{
						Storage* storage = ((Vines::sVines*)currentElement->data)->storage;
						std::vector<Ressources*> arrayOfResources = storage->GetResourceFromData(ResourceData::RESOURCE_PRODUCED);

						// We verify that we have enough of each resource to start the production
						for (Ressources* resource : arrayOfResources)
						{
							// We add the resources created during this production
							if (resource->GetQuantityOwned() == 0)
							{
								// DANS LE CAS DE 2 RESSOURCES OU PLUS PRODUITES, NE FONCTIONNERA PAS
								// A MODIFIER
								((Vines::sVines*)currentElement->data)->isProdCanBeCollected = false;
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


bool Vines::DestroyedBuildingSelected(const sf::Vector2f &_mapPosition)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			bool isBuildingFind = false;
			
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				//std::cout << "Map : " << positionCounter << "/" << list->size << " -> "<< ((Vines::sVines *)currentElement->data)->mapPosition.x << " " << ((Vines::sVines *)currentElement->data)->mapPosition.y << std::endl;

				// If the building position is identical to which send, we save his position in the linked list
				if (((Vines::sVines *)currentElement->data)->mapPosition == _mapPosition
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

void Vines::ClearStorages()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				// We remove all storages
				if (((Vines::sVines*)currentElement->data)->storage != nullptr)
				{
					// Delete the storage from the list
					delete ((Vines::sVines*)currentElement->data)->storage;
					((Vines::sVines*)currentElement->data)->storage = nullptr;
				}
			}
		}
	}
}


void Vines::SavingVinesListForFile(std::ofstream *_file)
{
	// Save the number of vines
	_file->write((char *)&m_list->size, sizeof(int));
	
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			for (currentElement = m_list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				Vines::sVines* vinesData = (Vines::sVines*)currentElement->data;
				_file->write((char *)vinesData, sizeof(sVines));
				_file->write((char *)&vinesData->storage, sizeof(Storage*));

				if (vinesData->storage != nullptr)
				{
					vinesData->storage->SavingForFile(_file);
				}
			}
		}
	}
}


void Vines::LoadingVinesListFromFile(std::ifstream *_file)
{
	// Delete every vines
	if (m_list != nullptr)
	{
		ClearStorages();
		FreeLinkedList(m_list);
	}


	// We reinit the vines list
	m_list = LinkedListInitialisation();


	// Load the number of vines
	int previousListSize(RESET);
	_file->read((char *)&previousListSize, sizeof(int));

	// We add every workers data to the list
	for (int i = RESET; i < previousListSize; i++)
	{
		LinkedListClass::sElement* newVine = new LinkedListClass::sElement;
		newVine->data = new Vines::sVines;
		
		_file->read((char *)(Vines::sVines *)newVine->data, sizeof(sVines));

		// Load the storage of the vine
		Storage* vineStorage = nullptr;
		_file->read((char*)&vineStorage, sizeof(Storage*));

		if (vineStorage != nullptr)
		{
			((Vines::sVines*)newVine->data)->storage = new Storage();
			((Vines::sVines*)newVine->data)->storage->LoadingFromFile(_file);
		}

		newVine->status = ELEMENT_ACTIVE;
		
		if (i == 0)
		{
			// Add this worker at the top of the list
			AddElementToLinkedList(m_list, newVine, 1);
		}
		else
		{
			// Add this worker at the end of the list
			AddElementToLinkedList(m_list, newVine, -1);
		}
	}
}