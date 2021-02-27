#include "Vines.h"
#include "GameDefinitions.h"
#include "ErrorsLogFile.h"
#include "TimeManagement.h"
#include "RessourcesManager.h"



Vines::Vines()
{

}



Vines::~Vines()
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
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

void Vines::UpdateVineLife(const float &_frametime, enum MonthsInOneYear _actualMonth)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_list->first;

			//std::cout << "Time : " << _lapsedFrameTime << std::endl;

			for (currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				switch (((Vines::sVines *)currentElement->data)->generalState)
				{
				case PLANTED:

					if (((Vines::sVines *)currentElement->data)->isWorkerThere == true)
					{
						((Vines::sVines *)currentElement->data)->lifeTime += _frametime;
						((Vines::sVines *)currentElement->data)->isWorkerThere = false;
					}

					// If the vine life is higher than the construction time, we launch it's growthing
					if (((Vines::sVines *)currentElement->data)->lifeTime >= m_vineBuilding->GetConstructionTimeCost())
					{
						//std::cout << "Vines built ! " << ((Vines::sVines *)currentElement->data)->lifeTime << " " << vineBuilding->GetConstructionTimeCost() << std::endl;
						((Vines::sVines *)currentElement->data)->generalState = THREE_YEARS_GROWTHING;
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						((Vines::sVines *)currentElement->data)->isWorkerThere = false;
					}

					break;
				case THREE_YEARS_GROWTHING:

					((Vines::sVines *)currentElement->data)->lifeTime += _frametime;
					
					// If the vine life is higher than the three years of growthing, we call that this is ready to produce
					if (((Vines::sVines *)currentElement->data)->lifeTime >= VINE_LIFE_3Y_GROWTHING_TO_READY_TO_PRODUCE)
					{
						//std::cout << "Vines ready to produce !\n\n";
						((Vines::sVines *)currentElement->data)->generalState = READY_TO_PRODUCE;
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						((Vines::sVines *)currentElement->data)->annualState = CARED; // Temporaire
						((Vines::sVines *)currentElement->data)->isWorkerThere = false;
					}

					break;
				case READY_TO_PRODUCE:

					((Vines::sVines *)currentElement->data)->lifeTime += _frametime;

					switch (_actualMonth)
					{
					case IANUARIUS:
						((Vines::sVines *)currentElement->data)->annualState = NEED_PRUNE;
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						break;
					case FEBRUARIUS:
						break;
					case MARTIUS:
						((Vines::sVines *)currentElement->data)->annualState = NEED_PLOUGHING;
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						break;
					case APRILIS:
						break;
					case MAïUS:
						((Vines::sVines *)currentElement->data)->annualState = NEED_WEEDING;
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						break;
					case IUNIUS:
						break;
					case QUINTILIS:
						((Vines::sVines *)currentElement->data)->annualState = NEED_CARE;
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						break;
					case SEXTILIS:
						break;
					case SEPTEMBER:
						((Vines::sVines *)currentElement->data)->annualState = NEED_HARVEST;
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						break;
					case OCTOBER:
						break;
					case NOVEMBER:
						break;
					case DECEMBER:
						break;
					default:
						break;
					}


					if (((Vines::sVines *)currentElement->data)->annualState == NEED_PRUNE
						&& ((Vines::sVines *)currentElement->data)->isWorkerThere == true)
					{
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						((Vines::sVines *)currentElement->data)->isWorkerThere = false;

						// Changer de status
						((Vines::sVines *)currentElement->data)->annualState = PRUNED;
						((Vines::sVines *)currentElement->data)->isPruned = true;
					}

					if (((Vines::sVines *)currentElement->data)->annualState == PRUNED)
					{

					}


					if (((Vines::sVines *)currentElement->data)->annualState == NEED_PLOUGHING
						&& ((Vines::sVines *)currentElement->data)->isWorkerThere == true)
					{
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						((Vines::sVines *)currentElement->data)->isWorkerThere = false;

						// Changer de status
						((Vines::sVines *)currentElement->data)->annualState = PLOUGHED;
						((Vines::sVines *)currentElement->data)->isPloughed = true;
					}

					if (((Vines::sVines *)currentElement->data)->annualState == PLOUGHED)
					{

					}


					if (((Vines::sVines *)currentElement->data)->annualState == NEED_WEEDING
						&& ((Vines::sVines *)currentElement->data)->isWorkerThere == true)
					{
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						((Vines::sVines *)currentElement->data)->isWorkerThere = false;

						// Changer de status
						((Vines::sVines *)currentElement->data)->annualState = WEEDED;
						((Vines::sVines *)currentElement->data)->isWeeded = true;
					}

					if (((Vines::sVines *)currentElement->data)->annualState == WEEDED)
					{

					}


					if (((Vines::sVines *)currentElement->data)->annualState == NEED_CARE
						&& ((Vines::sVines *)currentElement->data)->isWorkerThere == true)
					{
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						((Vines::sVines *)currentElement->data)->isWorkerThere = false;

						// Changer de status
						((Vines::sVines *)currentElement->data)->annualState = CARED;
						((Vines::sVines *)currentElement->data)->isCared = true;
					}

					if (((Vines::sVines *)currentElement->data)->annualState == CARED)
					{
						/*((Vines::sVines *)currentElement->data)->actualProductionTime += _frametime;
						
						if (((Vines::sVines *)currentElement->data)->actualProductionTime >= vineBuilding->GetProductionTimeCost())
						{
							((Vines::sVines *)currentElement->data)->actualProductionTime = RESET;
							((Vines::sVines *)currentElement->data)->isChangingSprite = true;
							((Vines::sVines *)currentElement->data)->annualState = NEED_HARVEST;
						}
						else
						{
							((Vines::sVines *)currentElement->data)->isWorkerThere = false;
						}*/
					}



					if (((Vines::sVines *)currentElement->data)->annualState == NEED_HARVEST
						&& ((Vines::sVines *)currentElement->data)->isWorkerThere == true)
					{
						((Vines::sVines *)currentElement->data)->secondaryTime = RESET;
						((Vines::sVines *)currentElement->data)->actualProductionTime = RESET;

						((Vines::sVines *)currentElement->data)->isProduced = true;
						((Vines::sVines *)currentElement->data)->isWorkerThere = false;						
					}

					if (((Vines::sVines *)currentElement->data)->annualState == HARVESTED)
					{
						((Vines::sVines *)currentElement->data)->isChangingSprite = true;
						((Vines::sVines *)currentElement->data)->isWorkerThere = false;

						// Changer de status
						//((Vines::sVines *)currentElement->data)->annualState = CARED; // Temporaire


						// Reset of the monthly states
						((Vines::sVines *)currentElement->data)->isPruned = false;
						((Vines::sVines *)currentElement->data)->isPloughed = false;
						((Vines::sVines *)currentElement->data)->isWeeded = false;
						((Vines::sVines *)currentElement->data)->isCared = false;
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


void Vines::UpdateVineSprite(unsigned short ***_map)
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

						_map[FIRST_FLOOR + SPRITE_ID][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
								[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 3;
					}
					else
					{
						switch (((Vines::sVines *)currentElement->data)->generalState)
						{
						case PLANTED:
							break;
						case THREE_YEARS_GROWTHING:
							_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
								[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 8;
							break;
						case READY_TO_PRODUCE:


							switch (((Vines::sVines *)currentElement->data)->annualState)
							{
							case NEED_PRUNE:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 12;
								break;
							case PRUNED:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 9;
								break;
							case NEED_PLOUGHING:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 13;
								break;
							case PLOUGHED:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 9;
								break;
							case NEED_WEEDING:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 14;
								break;
							case WEEDED:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 9;
								break;
							case NEED_CARE:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 15;
								break;
							case CARED:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 9;
								break;
							case NEED_HARVEST:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 10;
								break;
							case HARVESTED:
								_map[3 + 2][(int)((Vines::sVines *)currentElement->data)->mapPosition.y]
									[(int)((Vines::sVines *)currentElement->data)->mapPosition.x] = 9;
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


void Vines::UpdateVineProduction(Ressources *_ressource)
{
	if (m_list != nullptr)
	{
		if (m_list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = m_list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((Vines::sVines *)currentElement->data)->isProduced == true)
				{
					// Add quantity produced to the ressource targeted
					//_ressource->AddQuantityOwned(vineBuilding->GetRessourceQuantityProduced());

					// Launch the feedback animation of producing


					//((Vines::sVines *)currentElement->data)->isProduced = false;
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
					if (((Vines::sVines *)currentElement->data)->isProduced == true)
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

bool Vines::UpdateRessourcePickuping(const sf::Vector2f &_mapPosition, const float &_frametime)
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
					if (((Vines::sVines *)currentElement->data)->isProduced == true)
					{
						((Vines::sVines *)currentElement->data)->secondaryTime += _frametime;

						if (((Vines::sVines *)currentElement->data)->secondaryTime >= m_vineBuilding->GetPickupingTimeCost())
						{
							// Changer de sprite
							((Vines::sVines *)currentElement->data)->isChangingSprite = true;
							((Vines::sVines *)currentElement->data)->annualState = HARVESTED;

							((Vines::sVines *)currentElement->data)->secondaryTime = RESET;

							// Launch the feedback animation of producing


							((Vines::sVines *)currentElement->data)->isProduced = false;

							int quantityProduced = m_vineBuilding->GetRessourceQuantityProduced();

							((Vines::sVines*)currentElement->data)->storage->AddOrSubtractResource(Ressources::GetNameFromEnum(BUNCH_OF_GRAPE), quantityProduced);
							
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


bool Vines::DestroyedBuildingSelected(const sf::Vector2f &_mapPosition)
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




void Vines::SavingVinesListForFile(std::ofstream *_file)
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
				_file->write((char *)(Vines::sVines *)currentElement->data, sizeof(sVines));
			}
		}
	}
}


void Vines::LoadingVinesListFromFile(std::ifstream *_file)
{
	// Delete every vines
	if (m_list != nullptr)
	{
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