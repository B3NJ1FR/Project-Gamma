#include "Vines.h"
#include "GameDefinitions.h"
#include "ErrorsLogFile.h"
#include "TimeManagement.h"



Vines::Vines()
{

}



Vines::~Vines()
{

}

void Vines::InitialisationVines(Buildings *_vine)
{
	std::cout << "List before : " << this->list << std::endl;

	this->list = LinkedListInitialisation();

	std::cout << "List " << this->list << " Size : " << this->list->size << " Real First : " << this->list->first << " & Last : " << this->list->last << std::endl;

	this->vineBuilding = _vine;
}

void Vines::ReadVineLinkedList()
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			int positionCounter(1);

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				//std::cout << "Vine : " << positionCounter << "/" << this->list->size << "  -  Position : " << ((Vines::sVines *)currentElement->data)->mapPosition.x << " " << ((Vines::sVines *)currentElement->data)->mapPosition.y << std::endl;
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

	newVine->status = ELEMENT_ACTIVE;

	// Add this new vine at the end of the list
	this->AddElementToLinkedList(this->list, newVine, -1);

	//this->ReadVineLinkedList();
	//this->ReadLinkedList(this->list);
}

void Vines::UpdateVineLife(const float &_frametime, const enum MonthsInOneYear &_actualMonth)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->list->first;

			//std::cout << "Time : " << _lapsedFrameTime << std::endl;

			for (currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
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
					if (((Vines::sVines *)currentElement->data)->lifeTime >= this->vineBuilding->GetConstructionTimeCost())
					{
						//std::cout << "Vines built ! " << ((Vines::sVines *)currentElement->data)->lifeTime << " " << this->vineBuilding->GetConstructionTimeCost() << std::endl;
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
						
						if (((Vines::sVines *)currentElement->data)->actualProductionTime >= this->vineBuilding->GetProductionTimeCost())
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
			//std::cout << "List : " << this->list->first << std::endl;
		}
	}
}


void Vines::UpdateVineSprite(unsigned short ***_map)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
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
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((Vines::sVines *)currentElement->data)->isProduced == true)
				{
					// Add quantity produced to the ressource targeted
					//_ressource->AddQuantityOwned(this->vineBuilding->GetRessourceQuantityProduced());

					// Launch the feedback animation of producing


					//((Vines::sVines *)currentElement->data)->isProduced = false;
				}
			}
		}
	}
}

bool Vines::ConfirmVinePresenceAtWorkerPosition(const sf::Vector2f &_mapPosition)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				if (((Vines::sVines *)currentElement->data)->mapPosition == _mapPosition)
				{
					((Vines::sVines *)currentElement->data)->isWorkerThere = true;

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

bool Vines::CheckVineHasProducedRessource(const sf::Vector2f &_mapPosition)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
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

int Vines::VinesSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime)
{
	if (this->list != nullptr)
	{
		if (this->list->first != nullptr)
		{
			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
			{
				// If the building has produced the ressources, we manage it
				if (((Vines::sVines *)currentElement->data)->mapPosition == _mapPosition)
				{
					if (((Vines::sVines *)currentElement->data)->isProduced == true)
					{
						((Vines::sVines *)currentElement->data)->secondaryTime += _frametime;

						if (((Vines::sVines *)currentElement->data)->secondaryTime >= this->vineBuilding->GetPickupingTimeCost())
						{
							// Changer de sprite
							((Vines::sVines *)currentElement->data)->isChangingSprite = true;
							((Vines::sVines *)currentElement->data)->annualState = HARVESTED;

							((Vines::sVines *)currentElement->data)->secondaryTime = RESET;

							// Launch the feedback animation of producing


							((Vines::sVines *)currentElement->data)->isProduced = false;


							return this->vineBuilding->GetRessourceQuantityProduced();

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


bool Vines::DestroyedBuildingSelected(const sf::Vector2f &_mapPosition)
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
				if (((Vines::sVines *)currentElement->data)->mapPosition == _mapPosition
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




void Vines::SavingVinesListForFile(std::ofstream *_file)
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
				_file->write((char *)(Vines::sVines *)currentElement->data, sizeof(sVines));
			}
		}
	}
}


void Vines::LoadingVinesListFromFile(std::ifstream *_file)
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
		LinkedListClass::sElement* newVine = new LinkedListClass::sElement;
		newVine->data = new Vines::sVines;
		
		_file->read((char *)(Vines::sVines *)newVine->data, sizeof(sVines));

		newVine->status = ELEMENT_ACTIVE;
		
		if (i == 0)
		{
			// Add this worker at the top of the list
			this->AddElementToLinkedList(this->list, newVine, 1);
		}
		else
		{
			// Add this worker at the end of the list
			this->AddElementToLinkedList(this->list, newVine, -1);
		}
	}
}