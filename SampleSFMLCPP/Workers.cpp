#include "Workers.h"
#include "GameDefinitions.h"



Workers::Workers()
{
	this->actualStatus = IDLE;
	this->isLauchingMovement = false;
	this->isItWorkingPlace = false;

	this->isPressingEnd = false;
	this->isPressingStart = false;


	this->ressourceHeld = nullptr;
	this->quantityRessourceHeld = nullptr;
	this->targetedBuilding = nullptr;

	this->SetTimeToDeposit(RESET);

	std::cout << "Ouvrier initialisé\n\n";
}



Workers::~Workers()
{

}

void Workers::SetWorkerPosition(const sf::Vector2f &_mapPosition)
{
	this->mapPosition = _mapPosition;
}

void Workers::SetWorkerStatus(const enum WorkerStatus &_newStatus)
{
	this->actualStatus = _newStatus;
}


void Workers::SetWorkerMoneyValue(const int &_moneyValue)
{
	this->moneyValue = _moneyValue;
}


void Workers::SetWorkerMoneyCostPerMonth(const int &_moneyCostPerMonth)
{
	this->moneyCostPerMonth = _moneyCostPerMonth;
}

void Workers::SetWorkerIsInWorkingPlace(const bool &_isItWorkingPlace)
{
	this->isItWorkingPlace = _isItWorkingPlace;
}



sf::Vector2f Workers::GetWorkerPosition()
{
	return this->mapPosition;
}

sf::Vector2f Workers::GetWorkerEndingPosition()
{
	return this->mapEndPosition;
}

int Workers::GetWorkerMoneyValue()
{
	return this->moneyValue;
}

int Workers::GetWorkerMoneyCostPerMonth()
{
	return this->moneyCostPerMonth;
}

bool Workers::GetWorkerIsInWorkingPlace()
{
	return this->isItWorkingPlace;
}

enum WorkerStatus Workers::GetWorkerStatus()
{
	return this->actualStatus;
}

void Workers::DisplayWorker()
{

}

void Workers::SetTimeToDeposit(const float &_time)
{
	// Get the pickuping cost in term of time necessary
	this->timeToDeposit = _time;
}

void Workers::AddTimeToDeposit(const float &_frametime)
{
	// Get the depositing cost in term of time
	this->timeToDeposit += _frametime;
}

float Workers::GetTimeToDeposit()
{
	// Get the pickuping cost in term of time necessary
	return this->timeToDeposit;
}


void Workers::ActiveLauchingMovement() // TEMPORAIRE
{
	std::cout << "Chemin lance\n\n";
	this->isLauchingMovement = true;
}

void Workers::SetEndingPosition(sf::Vector2i _mapPosition, unsigned short ***_map)
{
	if (_mapPosition.x >= 0 && _mapPosition.y >= 0)
	{
		this->mapEndPosition = (sf::Vector2f)_mapPosition;
	}

	//
	// CRASH SORTIE DE TABLEAU A CAUSE DE X > 100000
	//

	// If this place is a building
	if (_map[FIRST_FLOOR + BUILDING_ID][(int)this->mapEndPosition.y][(int)this->mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		this->isItWorkingPlace = true;
		this->actualBuilding = (enum TypeOfBuilding)_map[FIRST_FLOOR + BUILDING_ID][(int)this->mapEndPosition.y][(int)this->mapEndPosition.x];

		//std::cout << "This is a working place : " << _map[FIRST_FLOOR + BUILDING_ID][(int)this->mapEndPosition.y][(int)this->mapEndPosition.x] << std::endl;
	}
	else if (_map[ZERO_FLOOR + BUILDING_ID][(int)this->mapEndPosition.y][(int)this->mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		this->isItWorkingPlace = true;
		this->actualBuilding = (enum TypeOfBuilding)_map[ZERO_FLOOR + BUILDING_ID][(int)this->mapEndPosition.y][(int)this->mapEndPosition.x];

		//std::cout << "This is a working place : " << _map[FIRST_FLOOR + BUILDING_ID][(int)this->mapEndPosition.y][(int)this->mapEndPosition.x] << std::endl;
	}
	else
	{
		this->isItWorkingPlace = false;
	}
}
void Workers::InitPathfinding(struct Game *_game)
{
	if (this->isLauchingMovement == true)
	{
		if (this->path != nullptr)
		{
			delete this->path;
			this->path = nullptr;
		}

		this->path = new Pathfinding;

		this->path->InitMapCopyPathfinding(sf::Vector2i(_game->numberColumns, _game->numberLines), _game->map, (FIRST_FLOOR + COLLISIONS_ID)); // TEMPORAIRE
		
		this->path->SetPathStartingPosition((sf::Vector2i)this->mapPosition); // TEMPORAIRE
		this->path->SetPathEndingPosition((sf::Vector2i)this->mapEndPosition); // TEMPORAIRE
	}
}

void Workers::UpdatePathAndActivities(struct Game *_game)
{
	float speed(RESET);

	switch (this->actualStatus)
	{
	case IDLE:

		//std::cout << "Idle\n";

		if (this->isLauchingMovement == true)
		{
			std::cout << "Changement de status vers Waiting Movement\n";

			this->SetWorkerStatus(WAITING_MOVEMENT);
			this->isLauchingMovement = false;
		}

		break;
		
	case WAITING_MOVEMENT:

		//std::cout << "WAITING MOVEMENT\n";
		this->path->MainStatePathfinding();

		// In the case that we find a way, we start to move
		if (this->path->GetActualStatus() == PATHFINDING_FIND_WAY_FIRST
			|| this->path->GetActualStatus() == PATHFINDING_FIND_WAY)
		{
			this->SetWorkerStatus(MOVEMENT);
		}
		// In the case that we don't find any way to join the target, we stay where we're
		else if (this->path->GetActualStatus() == PATHFINDING_FIND_NO_WAY)
		{
			this->SetWorkerStatus(IDLE);
			delete this->path;
			this->path = nullptr;
		}

		break;

	case MOVEMENT:
		
		// Speed modification depending on the type of soil
		if (_game->map[ZERO_FLOOR + COLLISIONS_ID][(int)this->mapPosition.y][(int)this->mapPosition.x] == PATH)
		{
			speed = _game->time->GetFrameTime() * 2.25f;
		}
		else if (_game->map[ZERO_FLOOR + COLLISIONS_ID][(int)this->mapPosition.y][(int)this->mapPosition.x] == STONE_PATH)
		{
			speed = _game->time->GetFrameTime() * 3.5f;
		}
		else if(_game->map[ZERO_FLOOR + COLLISIONS_ID][(int)this->mapPosition.y][(int)this->mapPosition.x] == ROAD)
		{
			speed = _game->time->GetFrameTime() * 5;
		}
		else
		{
			speed = _game->time->GetFrameTime() * 1.5f;
		}

		//std::cout << "MOVEMENT\n";
		this->path->WalkProcess(&this->mapPosition, speed);



		// If the path ask to be deleted, that mean that the worker has reached his destination
		if (this->path->GetActualStatus() == PATHFINDING_NEED_TO_BE_DELETED)
		{
			// If at the initialisation we find that location is a working place, we change the status of the worker to WORKING
			if (this->isItWorkingPlace == true)
			{
				// If that place is what which were targeted and the worker carry some ressource, he need to deposit them
				if (this->targetedBuilding != nullptr)
				{
					if (*(this->targetedBuilding) == this->actualBuilding
						&& this->ressourceHeld != nullptr)
					{
						this->SetWorkerStatus(DEPOSIT_RESSOURCES);
					}
					else
					{
						this->SetWorkerStatus(WORKING);
					}
				}
				else
				{
					// Temporaire
					if (this->ressourceHeld != nullptr)
					{
						// A REVOIR CAR IL DEPOSE LES RESSOURCES AU SOL
						this->SetWorkerStatus(DEPOSIT_RESSOURCES);
					}
					else
					{
						this->SetWorkerStatus(WORKING);
					}
				}				
			}
			// Else, the worker just wait at the IDLE status
			else
			{
				if (this->ressourceHeld != nullptr)
				{
					// A REVOIR CAR IL DEPOSE LES RESSOURCES AU SOL
					this->SetWorkerStatus(DEPOSIT_RESSOURCES);
				}
				else
				{
					this->SetWorkerStatus(IDLE);
				}
			}

			// We delete the path and init his pointer to null
			delete this->path;
			this->path = nullptr;
		}

		break;

	case WORKING:
		
		if (this->actualBuilding == BUILDING_VINES)
		{
			// We send at the vines building the confirmation that a worker is there
			if (_game->vines.ConfirmVinePresenceAtPosition(this->mapPosition, true) == true)
			{
				//std::cout << "Working ...\n";

				if (_game->vines.CheckVineHasProducedRessource(this->mapPosition) == true)
				{
					this->SetWorkerStatus(PICKUP_RESSOURCES);
				}
			}
			// If the result is false, that mean the building isn't there or has been destroyed
			else
			{

			}

		}
		else if (this->actualBuilding == BUILDING_GRAPE_STOMPING_VATS)
		{
			// We send at the stomping vats building the confirmation that a worker is there
			if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtPosition(this->mapPosition, false, true) == true)
			{
				//std::cout << "Working ...\n";

				if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
				{
					this->SetWorkerStatus(PICKUP_RESSOURCES);
				}

			}
			else
			{
				std::cout << "There\n\n\n\n";
			}

		}
		else if (this->actualBuilding == BUILDING_WINE_PRESS)
		{
			// We send at the wine press building the confirmation that a worker is there
			if (_game->winePress.ConfirmSpecificBuildingPresenceAtPosition(this->mapPosition, false, true) == true)
			{
				//std::cout << "Working ...\n";

				if (_game->winePress.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
				{
					this->SetWorkerStatus(PICKUP_RESSOURCES);
				}


			}
			else
			{

			}

		}
		else if (this->actualBuilding == BUILDING_WINE_STOREHOUSE)
		{
			// We send at the wine storehouse building the confirmation that a worker is there
			if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(this->mapPosition, false, true) == true)
			{
				//std::cout << "Working ...\n";
							   				 
				if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
				{
					this->SetWorkerStatus(PICKUP_RESSOURCES);
				}
			}
			else
			{

			}

		}
		else if (this->actualBuilding == BUILDING_STOREHOUSE)
		{
			// We send at the storehouse building the confirmation that a worker is there
			if (_game->storehouse.ConfirmStorehousePresenceAtPosition(this->mapPosition, false, true) == true)
			{
				//std::cout << "Working ...\n";
			}
			else
			{

			}

		}
		else if (this->actualBuilding == BUILDING_STALL)
		{
			// We send at the wine storehouse building the confirmation that a worker is there
			if (_game->stall->ConfirmPresenceAtPosition(this->mapPosition, false, true) == true)
			{
				//std::cout << "Working ...\n";
			}
			else
			{

			}

		}
		else
		{
			std::cout << "ERROR, the worker don't find a building where works\n";
		}



		break;
	
	case PICKUP_RESSOURCES:

		if (this->actualBuilding == BUILDING_VINES)
		{
			// That mean the worker has already picked up a ressource
			if (this->ressourceHeld != nullptr)
			{
				// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
				if (*(this->ressourceHeld) == BUNCH_OF_GRAPE)
				{
					int ressourceProduced = _game->vines.VinesSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

					*(this->quantityRessourceHeld) += ressourceProduced;
					*(this->targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;
					
					this->isItWorkingPlace = false;
					
					sf::Vector2i targetedPosition = _game->stompingVats.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						this->SetEndingPosition(targetedPosition, _game->map);
						this->ActiveLauchingMovement();
					}

					this->SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					this->SetWorkerStatus(IDLE);
				}
			}
			else
			{
				int ressourceProduced = _game->vines.VinesSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

				// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
				if (ressourceProduced != 0 && ressourceProduced > 0)
				{
					this->ressourceHeld = new enum TypesOfRessources;
					this->quantityRessourceHeld = new int;
					this->targetedBuilding = new enum TypeOfBuilding;

					*(this->ressourceHeld) = BUNCH_OF_GRAPE;
					*(this->quantityRessourceHeld) = ressourceProduced;
					*(this->targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;

					this->isItWorkingPlace = false;

					
					sf::Vector2i targetedPosition = _game->stompingVats.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						this->SetEndingPosition(targetedPosition, _game->map);
						this->ActiveLauchingMovement();
					}

					this->SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				else if (_game->vines.CheckVineHasProducedRessource(this->mapPosition) == true)
				{
					this->SetWorkerStatus(PICKUP_RESSOURCES);
				}
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_game->vines.CheckVineHasProducedRessource(this->mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					this->SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}
			

		}
		else if (this->actualBuilding == BUILDING_GRAPE_STOMPING_VATS)
		{
			// That mean the worker has already picked up a ressource
			if (this->ressourceHeld != nullptr)
			{
				// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
				if (*(this->ressourceHeld) == GRAPES_MUST)
				{
					int ressourceProduced = _game->stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

					*(this->quantityRessourceHeld) += ressourceProduced;
					*(this->targetedBuilding) = BUILDING_WINE_PRESS;

					this->isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _game->winePress.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						this->SetEndingPosition(targetedPosition, _game->map);
						this->ActiveLauchingMovement();
					}

					this->SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					this->SetWorkerStatus(IDLE);
				}
			}
			else
			{
				int ressourceProduced = _game->stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

				// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
				if (ressourceProduced != 0 && ressourceProduced > 0)
				{
					this->ressourceHeld = new enum TypesOfRessources;
					this->quantityRessourceHeld = new int;
					this->targetedBuilding = new enum TypeOfBuilding;

					*(this->ressourceHeld) = GRAPES_MUST;
					*(this->quantityRessourceHeld) = ressourceProduced;
					*(this->targetedBuilding) = BUILDING_WINE_PRESS;

					this->isItWorkingPlace = false;
					
					sf::Vector2i targetedPosition = _game->winePress.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						this->SetEndingPosition(targetedPosition, _game->map);
						this->ActiveLauchingMovement();
					}

					this->SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				else if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
				{
					this->SetWorkerStatus(PICKUP_RESSOURCES);
				}
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					this->SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}

		}
		else if (this->actualBuilding == BUILDING_WINE_PRESS)
		{
			// That mean the worker has already picked up a ressource
			if (this->ressourceHeld != nullptr)
			{
				// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
				if (*(this->ressourceHeld) == GRAPE_JUICE)
				{
					int ressourceProduced = _game->winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

					*(this->quantityRessourceHeld) += ressourceProduced;
					*(this->targetedBuilding) = BUILDING_WINE_STOREHOUSE;
					
					this->isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _game->wineStorehouse.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						this->SetEndingPosition(targetedPosition, _game->map);
						this->ActiveLauchingMovement();
					}

					this->SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";

					
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					this->SetWorkerStatus(IDLE);
				}
			}
			else
			{
				int ressourceProduced = _game->winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

				// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
				if (ressourceProduced != 0 && ressourceProduced > 0)
				{
					this->ressourceHeld = new enum TypesOfRessources;
					this->quantityRessourceHeld = new int;
					this->targetedBuilding = new enum TypeOfBuilding;

					*(this->ressourceHeld) = GRAPE_JUICE;
					*(this->quantityRessourceHeld) = ressourceProduced;
					*(this->targetedBuilding) = BUILDING_WINE_STOREHOUSE;

					this->isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _game->wineStorehouse.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						this->SetEndingPosition(targetedPosition, _game->map);
						this->ActiveLauchingMovement();
					}

					this->SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				else if (_game->winePress.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
				{
					this->SetWorkerStatus(PICKUP_RESSOURCES);
				}
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_game->winePress.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					this->SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}

		}
		else if (this->actualBuilding == BUILDING_WINE_STOREHOUSE)
		{
			// That mean the worker has already picked up a ressource
			if (this->ressourceHeld != nullptr)
			{
				// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
				if (*(this->ressourceHeld) == AMPHORA_OF_WINE)
				{
					int ressourceProduced = _game->wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

					*(this->quantityRessourceHeld) += ressourceProduced;
					*(this->targetedBuilding) = BUILDING_STOREHOUSE;
					
					this->isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _game->storehouse.StorehouseFindNearestBuilding(this->mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						this->SetEndingPosition(targetedPosition, _game->map);
						this->ActiveLauchingMovement();
					}

					this->SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";

					
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					this->SetWorkerStatus(IDLE);
				}
			}
			else
			{
				int ressourceProduced = _game->wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

				// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
				if (ressourceProduced != 0 && ressourceProduced > 0)
				{
					this->ressourceHeld = new enum TypesOfRessources;
					this->quantityRessourceHeld = new int;
					this->targetedBuilding = new enum TypeOfBuilding;

					*(this->ressourceHeld) = AMPHORA_OF_WINE;
					*(this->quantityRessourceHeld) = ressourceProduced;
					*(this->targetedBuilding) = BUILDING_STOREHOUSE;

					this->isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _game->storehouse.StorehouseFindNearestBuilding(this->mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						this->SetEndingPosition(targetedPosition, _game->map);
						this->ActiveLauchingMovement();
					}

					this->SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				else if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
				{
					this->SetWorkerStatus(PICKUP_RESSOURCES);
				}
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					this->SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}
		}
		else
		{
			std::cout << "ERROR, the worker don't find a building where works\n";
		}

		break;

	case DEPOSIT_RESSOURCES:
		
		if (this->ressourceHeld != nullptr)
		{
			switch (*(this->ressourceHeld))
			{
			case GRAPE_VINE:
				break;
			case BUNCH_OF_GRAPE:

				if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtPosition(this->mapPosition, false, true))
				{
					this->AddTimeToDeposit(_game->time->GetFrameTime());

					if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_GRAPE_STOMPING_VATS].GetDepositingTimeCost())
					{
						std::cout << "Worker, want to deposit ressources\n";

						_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

						this->SetTimeToDeposit(RESET);

						// Reset of the main data
						delete this->ressourceHeld;
						delete this->quantityRessourceHeld;
						delete this->targetedBuilding;

						this->ressourceHeld = nullptr;
						this->quantityRessourceHeld = nullptr;
						this->targetedBuilding = nullptr;

						this->SetWorkerStatus(WORKING);
					}
				}
				else
				{
					this->SetWorkerStatus(IDLE);
				}

				break;
			case GRAPES_MUST:

				if (_game->winePress.ConfirmSpecificBuildingPresenceAtPosition(this->mapPosition, false, true))
				{
					this->AddTimeToDeposit(_game->time->GetFrameTime());

					if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_PRESS].GetDepositingTimeCost())
					{
						std::cout << "Worker, want to deposit ressources\n";

						_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

						this->SetTimeToDeposit(RESET);

						// Reset of the main data
						delete this->ressourceHeld;
						delete this->quantityRessourceHeld;
						delete this->targetedBuilding;

						this->ressourceHeld = nullptr;
						this->quantityRessourceHeld = nullptr;
						this->targetedBuilding = nullptr;

						this->SetWorkerStatus(WORKING);
					}
				}
				else
				{
					this->SetWorkerStatus(IDLE);
				}

				break;
			case GRAPE_JUICE:

				if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(this->mapPosition, false, true))
				{
					this->AddTimeToDeposit(_game->time->GetFrameTime());

					if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
					{
						std::cout << "Worker, want to deposit ressources\n";

						_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

						this->SetTimeToDeposit(RESET);

						// Reset of the main data
						delete this->ressourceHeld;
						delete this->quantityRessourceHeld;
						delete this->targetedBuilding;

						this->ressourceHeld = nullptr;
						this->quantityRessourceHeld = nullptr;
						this->targetedBuilding = nullptr;

						this->SetWorkerStatus(WORKING);
					}
				}
				else
				{
					this->SetWorkerStatus(IDLE);
				}

				break;
			case PURE_GRAPE_JUICE:

				break;
			case GRAPE_MARC:

				break;
			case DOLIUM:

				break;
			case AMPHORAS:

				this->AddTimeToDeposit(_game->time->GetFrameTime());

				if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
				{
					std::cout << "Worker, want to deposit ressources\n";

					_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

					this->SetTimeToDeposit(RESET);

					// Reset of the main data
					delete this->ressourceHeld;
					delete this->quantityRessourceHeld;
					delete this->targetedBuilding;

					this->ressourceHeld = nullptr;
					this->quantityRessourceHeld = nullptr;
					this->targetedBuilding = nullptr;

					this->SetWorkerStatus(WORKING);
				}

				break;
			case AMPHORA_OF_WINE:
				
				if (_game->storehouse.ConfirmStorehousePresenceAtPosition(this->mapPosition, false, true))
				{
					this->AddTimeToDeposit(_game->time->GetFrameTime());

					if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_STOREHOUSE].GetDepositingTimeCost())
					{
						std::cout << "Worker, deposit amphora of wine ressources\n";

						_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

						this->SetTimeToDeposit(RESET);

						// Reset of the main data
						delete this->ressourceHeld;
						delete this->quantityRessourceHeld;
						delete this->targetedBuilding;

						this->ressourceHeld = nullptr;
						this->quantityRessourceHeld = nullptr;
						this->targetedBuilding = nullptr;

						this->SetWorkerStatus(WORKING);
					}
				}
				else
				{
					this->SetWorkerStatus(IDLE);
				}

				break;
			case TOOLS:
				break;
			case WICKER_BASKET:
				break;
			default:
				break;
			}
						
		}
		else
		{
			// For security, we reaffect the worker to the WORKING status
			this->SetWorkerStatus(WORKING);

			std::cout << "Worker, error during deposit of the ressources\n";
		}

		break;

	default:
		break;
	}
}

