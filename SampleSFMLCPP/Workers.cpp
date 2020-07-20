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



sf::Vector2f Workers::GetWorkerPosition()
{
	return this->mapPosition;
}

int Workers::GetWorkerMoneyValue()
{
	return this->moneyValue;
}

int Workers::GetWorkerMoneyCostPerMonth()
{
	return this->moneyCostPerMonth;
}


void Workers::DisplayWorker()
{

}

void Workers::ActiveLauchingMovement() // TEMPORAIRE
{
	std::cout << "Chemin lance\n";
	this->isLauchingMovement = true;
}

void Workers::SetEndingPosition(sf::Vector2i _mapPosition, unsigned short ***_map)
{
	this->mapEndPosition = (sf::Vector2f)_mapPosition;

	// If this place is a building
	if (_map[FIRST_FLOOR + COLLISIONS_ID][(int)this->mapEndPosition.y][(int)this->mapEndPosition.x] != 0)
	{
		// We change the worker's status to working
		this->isItWorkingPlace = true;
		this->actualBuilding = (enum TypeOfBuilding)_map[BUILDING_ID + COLLISIONS_ID][(int)this->mapEndPosition.y][(int)this->mapEndPosition.x];

		std::cout << "This is a working place : " << _map[FIRST_FLOOR + COLLISIONS_ID][(int)this->mapEndPosition.y][(int)this->mapEndPosition.x] << std::endl;
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

void Workers::UpdatePathAndActivities(const float &_frametime, unsigned short ***_map, Vines *_vines, SpecificsBuildings *_buildings)
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
		if (_map[ZERO_FLOOR + COLLISIONS_ID][(int)this->mapPosition.y][(int)this->mapPosition.x] == PATH)
		{
			speed = _frametime * 2.25f;
		}
		else if (_map[ZERO_FLOOR + COLLISIONS_ID][(int)this->mapPosition.y][(int)this->mapPosition.x] == STONE_PATH)
		{
			speed = _frametime * 3.5f;
		}
		else if(_map[ZERO_FLOOR + COLLISIONS_ID][(int)this->mapPosition.y][(int)this->mapPosition.x] == ROAD)
		{
			speed = _frametime * 5;
		}
		else
		{
			speed = _frametime * 1.5f;
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
					// Temporaire pour tester sans bâtiment
					else if (this->ressourceHeld != nullptr)
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
					this->SetWorkerStatus(WORKING);
				}				
			}
			// Else, the worker just wait at the IDLE status
			else
			{
				this->SetWorkerStatus(IDLE);
			}

			// We delete the path and init his pointer to null
			delete this->path;
			this->path = nullptr;
		}

		break;

	case WORKING:

		// We send at the vines building the confirmation that a worker is there
		if (_vines->ConfirmVinePresenceAtWorkerPosition(this->mapPosition) == true)
		{
			//std::cout << "Working ...\n";

			if (_vines->CheckVineHasProducedRessource(this->mapPosition) == true)
			{
				this->SetWorkerStatus(PICKUP_RESSOURCES);
			}
		}
		// If the result is false, that mean the building isn't there or has been destroyed
		else
		{

		}

		if (_buildings->CheckSpecificBuildingPresenceAtPosition(this->mapPosition) == true)
		{
			//std::cout << "Working ...\n";
		}
		else
		{

		}

		break;
	
	case PICKUP_RESSOURCES:

		if (this->isItWorkingPlace == true)
		{
			int ressourceProduced = _vines->VinesSendRessourceProducedToPresentWorker(this->mapPosition);

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

				// TEMPORAIRE - Map position a modifier pour la remplacer par une fonction qui trouve le fouloir le plus proche
				this->SetEndingPosition((sf::Vector2i(10, 10)), _map);
				this->SetWorkerStatus(IDLE);
				this->ActiveLauchingMovement();

				std::cout << "Worker, picked up ressources\n";
			}
			// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
			else
			{
				// For security, we reaffect the worker to the WORKING status
				this->SetWorkerStatus(WORKING);

				std::cout << "Worker, error during pick up of ressources\n";
			}
		}

		break;

	case DEPOSIT_RESSOURCES:
		
		if (this->ressourceHeld != nullptr)
		{
			std::cout << "Worker, deposit ressources\n";

			// Add quantity produced to the ressource targeted
			//_ressource->AddQuantityOwned(this->vineBuilding->GetRessourceQuantityProduced());

			



			// Reset of the main data
			delete this->ressourceHeld;
			delete this->quantityRessourceHeld;
			delete this->targetedBuilding;

			this->ressourceHeld = nullptr;
			this->quantityRessourceHeld = nullptr;
			this->targetedBuilding = nullptr;
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

