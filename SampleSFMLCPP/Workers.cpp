#include "Workers.h"
#include "GameDefinitions.h"



Workers::Workers()
{
	this->actualStatus = IDLE;
	this->isLauchingMovement = false;
	this->isItWorkingPlace = false;

	this->isPressingEnd = false;
	this->isPressingStart = false;

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
	if (_map[FIRST_FLOOR + COLLISIONS_ID][_mapPosition.y][_mapPosition.x] != 0)
	{
		// We change the worker's status to working
		this->isItWorkingPlace = true;

		std::cout << "This is a working place : " << _map[FIRST_FLOOR + COLLISIONS_ID][_mapPosition.y][_mapPosition.x] << std::endl;
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

			this->actualStatus = WAITING_MOVEMENT;
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
			this->actualStatus = MOVEMENT;
		}
		// In the case that we don't find any way to join the target, we stay where we're
		else if (this->path->GetActualStatus() == PATHFINDING_FIND_NO_WAY)
		{
			this->actualStatus = IDLE;
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
			if (this->isItWorkingPlace == true)
			{
				this->actualStatus = WORKING;
			}
			else
			{
				this->actualStatus = IDLE;
			}

			delete this->path;
			this->path = nullptr;
		}

		break;

	case WORKING:

		if (_vines->CheckVinePresenceAtPosition((sf::Vector2f)this->mapPosition) == true)
		{
			//std::cout << "Working ...\n";
		}
		else if (_buildings->CheckSpecificBuildingPresenceAtPosition((sf::Vector2f)this->mapPosition) == true)
		{
			//std::cout << "Working ...\n";
		}

		break;
	case PICKUP_RESSOURCES:
		break;
	case DEPOSIT_RESSOURCES:
		break;
	default:
		break;
	}
}

