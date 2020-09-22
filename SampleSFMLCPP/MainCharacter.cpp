#include "MainCharacter.h"
#include "GameDefinitions.h"


MainCharacter::MainCharacter()
{
	this->sprite = LoadSprite("Data/Assets/Sprites/Entities/Main_Character.png", 5);

	// A CONFIG / SET TEMPORAIRE
	this->mapPosition = sf::Vector2f(10, 10);
	
	this->actualStatus = IDLE;
	this->isLauchingMovement = false;
	this->isItWorkingPlace = false;

	this->isPressingEnd = false;
	this->isPressingStart = false;


	this->isMainCharacterSelected = true;
	this->isCurrentlyBuilding = false;


	std::cout << "Ouvrier initialisé\n\n";
}

MainCharacter::~MainCharacter()
{

}


void MainCharacter::InitPathfinding(struct Game *_game)
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


void MainCharacter::SetMainCharacterPosition(const sf::Vector2f &_mapPosition)
{
	this->mapPosition = _mapPosition;
}


void MainCharacter::SetMainCharacterEndingPosition(sf::Vector2i _mapPosition, unsigned short ***_map)
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

void MainCharacter::SetSpriteScale(const sf::Vector2f &_newScale)
{
	this->sprite.setScale(sf::Vector2f(_newScale.x * 1.3f, _newScale.y * 1.3f));
}

void MainCharacter::SetMainCharacterStatus(const enum WorkerStatus &_newStatus, const bool &_isLaunchingMovement)
{
	this->actualStatus = _newStatus;

	if (_isLaunchingMovement == true)
	{
		this->isLauchingMovement = true;
	}
}

void MainCharacter::SetIsMainCharacterSelected(const bool &_isMainCharacterSelected)
{
	this->isMainCharacterSelected = _isMainCharacterSelected;
}

void MainCharacter::SetWorkerIsInWorkingPlace(const bool &_isItWorkingPlace)
{
	this->isItWorkingPlace = _isItWorkingPlace;
}

void MainCharacter::SetIsCurrentlyBuilding(const bool &_isCurrentlyBuilding)
{
	this->isCurrentlyBuilding = _isCurrentlyBuilding;
}



sf::Vector2f MainCharacter::GetMainCharacterPosition()
{
	return this->mapPosition;
}

bool MainCharacter::IsMainCharacterPosition(const sf::Vector2i &_mapPosition)
{
	return (sf::Vector2i(this->mapPosition) == _mapPosition) ? true : false;
}

sf::Vector2f MainCharacter::GetMainCharacterEndingPosition()
{
	return this->mapEndPosition;
}

sf::Sprite MainCharacter::GetSprite()
{
	return this->sprite;
}

bool MainCharacter::GetIsMainCharacterSelected()
{
	return this->isMainCharacterSelected;
}

enum WorkerStatus MainCharacter::GetWorkerStatus()
{
	return this->actualStatus;
}

bool MainCharacter::GetIsCurrentlyBuilding()
{
	return this->isCurrentlyBuilding;
}

void MainCharacter::UpdatePathAndActivities(struct Game *_game)
{
	float speed(RESET);

	switch (this->actualStatus)
	{
	case IDLE:

		//std::cout << "Idle\n";

		if (this->isLauchingMovement == true)
		{
			std::cout << "Changement de status vers Waiting Movement\n";

			this->SetMainCharacterStatus(WAITING_MOVEMENT);
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
			this->SetMainCharacterStatus(MOVEMENT);
		}
		// In the case that we don't find any way to join the target, we stay where we're
		else if (this->path->GetActualStatus() == PATHFINDING_FIND_NO_WAY)
		{
			this->SetMainCharacterStatus(IDLE);
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
		else if (_game->map[ZERO_FLOOR + COLLISIONS_ID][(int)this->mapPosition.y][(int)this->mapPosition.x] == ROAD)
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
				if (this->isCurrentlyBuilding)
				{
					this->SetMainCharacterStatus(BUILDING);
				}
				else
				{
					// If that place is what which were targeted and the worker carry some ressource, he need to deposit them
					if (this->targetedBuilding != nullptr)
					{
						this->SetMainCharacterStatus(WORKING);
					}
					else
					{
						this->SetMainCharacterStatus(WORKING);
					}
				}
			}
			// Else, the worker just wait at the IDLE status
			else
			{
				this->SetMainCharacterStatus(IDLE);
			}

			// We delete the path and init his pointer to null
			delete this->path;
			this->path = nullptr;
		}

		break;

	case BUILDING:
		
		if (this->isCurrentlyBuilding == true)
		{
			// Security
			if (_game->buildingsListPlanned.GetBuildingID() != -1
				&& _game->buildingsListPlanned.GetBuildingPositionInMap() != sf::Vector2i(-1, -1)
				&& _game->buildingsListPlanned.GetBuildingSize() != sf::Vector2i(-1, -1))
			{
				// Clear the ground by putting sand floor
				for (int y = 0; y < _game->buildingsListPlanned.GetBuildingSize().y; y++)
				{
					for (int x = 0; x < _game->buildingsListPlanned.GetBuildingSize().x; x++)
					{
						_game->map[ZERO_FLOOR + SPRITE_ID]
							[_game->buildingsListPlanned.GetBuildingPositionInMap().y - y]
							[_game->buildingsListPlanned.GetBuildingPositionInMap().x - x] = 1;
					}
				}

				// Modifier couche pour ID
				_game->buildWindow.SetBuildingOnMap(_game, FIRST_FLOOR, _game->buildingsListPlanned.GetBuildingID(), sf::Vector3i(COLLISION, _game->buildingsListPlanned.GetBuildingID(), RESET), _game->buildingsListPlanned.GetBuildingPositionInMap());


				// If the building selected is the vines, we add informations to the concerned linkedlist
				if (_game->buildingsListPlanned.GetBuildingID() == BUILDING_VINES)
				{
					_game->vines.AddNewVineToList((sf::Vector2f)_game->buildingsListPlanned.GetBuildingPositionInMap());
				}
				else if (_game->buildingsListPlanned.GetBuildingID() == BUILDING_GRAPE_STOMPING_VATS)
				{
					_game->stompingVats.AddNewBuildingToList((sf::Vector2f)_game->buildingsListPlanned.GetBuildingPositionInMap());
				}
				else if (_game->buildingsListPlanned.GetBuildingID() == BUILDING_WINE_PRESS)
				{
					_game->winePress.AddNewBuildingToList((sf::Vector2f)_game->buildingsListPlanned.GetBuildingPositionInMap());
				}
				else if (_game->buildingsListPlanned.GetBuildingID() == BUILDING_WINE_STOREHOUSE)
				{
					_game->wineStorehouse.AddNewBuildingToList((sf::Vector2f)_game->buildingsListPlanned.GetBuildingPositionInMap());
				}
				else if (_game->buildingsListPlanned.GetBuildingID() == BUILDING_STOREHOUSE)
				{
					_game->storehouse.AddNewBuildingToList((sf::Vector2f)_game->buildingsListPlanned.GetBuildingPositionInMap());
				}
				else if (_game->buildingsListPlanned.GetBuildingID() == BUILDING_STALL)
				{
					_game->stall->AddNewBuilding((sf::Vector2f)_game->buildingsListPlanned.GetBuildingPositionInMap());
				}


				// Deletion of the current first building in the list
				_game->buildingsListPlanned.DeleteCurrentFirstBuildingInList();
				_game->buildingsListPlanned.ReadBuildingsPlannedToList();

				//Update the workers's paths
				_game->workersList->CheckAndUpdateWorkersPath(_game->map);
					

				// Test if the list is empty or not
				if (_game->buildingsListPlanned.IsBuildingListIsEmpty())
				{
					this->isCurrentlyBuilding =	 false;
				}
				else
				{
					// Set the position of the next building plannified
					this->SetMainCharacterEndingPosition(_game->buildingsListPlanned.GetBuildingPositionInMap(), _game->map);

					this->SetMainCharacterStatus(IDLE, true);
				}
			}
			
		}
		else
		{
			this->SetMainCharacterStatus(IDLE);
		}

		break;
	case WORKING:

		if (this->actualBuilding == BUILDING_VILLA)
		{
			if (_game->actualGameState != VILLA_MANAGEMENT)
			{
				_game->actualGameState = VILLA_MANAGEMENT;
			}
			else
			{
				this->SetMainCharacterStatus(IDLE);
			}
		}
		//else if (this->actualBuilding == BUILDING_GRAPE_STOMPING_VATS)
		//{
		//	// We send at the stomping vats building the confirmation that a worker is there
		//	if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtWorkerPosition(this->mapPosition) == true)
		//	{
		//		//std::cout << "Working ...\n";

		//		if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
		//		{
		//			this->SetWorkerStatus(PICKUP_RESSOURCES);
		//		}

		//	}
		//	else
		//	{

		//	}

		//}
		//else if (this->actualBuilding == BUILDING_WINE_PRESS)
		//{
		//	// We send at the wine press building the confirmation that a worker is there
		//	if (_game->winePress.ConfirmSpecificBuildingPresenceAtWorkerPosition(this->mapPosition) == true)
		//	{
		//		//std::cout << "Working ...\n";

		//		if (_game->winePress.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
		//		{
		//			this->SetWorkerStatus(PICKUP_RESSOURCES);
		//		}


		//	}
		//	else
		//	{

		//	}

		//}
		//else if (this->actualBuilding == BUILDING_WINE_STOREHOUSE)
		//{
		//	// We send at the wine storehouse building the confirmation that a worker is there
		//	if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtWorkerPosition(this->mapPosition) == true)
		//	{
		//		//std::cout << "Working ...\n";

		//		if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
		//		{
		//			this->SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//	}
		//	else
		//	{

		//	}

		//}
		//else if (this->actualBuilding == BUILDING_STOREHOUSE)
		//{
		//	// We send at the storehouse building the confirmation that a worker is there
		//	if (_game->storehouse.ConfirmStorehousePresenceAtWorkerPosition(this->mapPosition) == true)
		//	{
		//		//std::cout << "Working ...\n";
		//	}
		//	else
		//	{

		//	}

		//}
		//else if (this->actualBuilding == BUILDING_STALL)
		//{
		//	// We send at the wine storehouse building the confirmation that a worker is there
		//	if (_game->stall->ConfirmPresenceAtWorkerPosition(this->mapPosition) == true)
		//	{
		//		//std::cout << "Working ...\n";
		//	}
		//	else
		//	{

		//	}

		//}
		//else
		//{
		//	std::cout << "ERROR, the worker don't find a building where works\n";
		//}



		break;

	case PICKUP_RESSOURCES:

		//if (this->actualBuilding == BUILDING_VINES)
		//{
		//	// That mean the worker has already picked up a ressource
		//	if (this->ressourceHeld != nullptr)
		//	{
		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
		//		if (*(this->ressourceHeld) == BUNCH_OF_GRAPE)
		//		{
		//			int ressourceProduced = _game->vines.VinesSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

		//			*(this->quantityRessourceHeld) += ressourceProduced;
		//			*(this->targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;

		//			this->isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->stompingVats.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				this->SetEndingPosition(targetedPosition, _game->map);
		//				this->ActiveLauchingMovement();
		//			}

		//			this->SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		else
		//		{
		//			std::cout << "The current worker already has a ressource in hands\n\n";
		//			this->SetWorkerStatus(IDLE);
		//		}
		//	}
		//	else
		//	{
		//		int ressourceProduced = _game->vines.VinesSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
		//		if (ressourceProduced != 0 && ressourceProduced > 0)
		//		{
		//			this->ressourceHeld = new enum TypesOfRessources;
		//			this->quantityRessourceHeld = new int;
		//			this->targetedBuilding = new enum TypeOfBuilding;

		//			*(this->ressourceHeld) = BUNCH_OF_GRAPE;
		//			*(this->quantityRessourceHeld) = ressourceProduced;
		//			*(this->targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;

		//			this->isItWorkingPlace = false;


		//			sf::Vector2i targetedPosition = _game->stompingVats.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				this->SetEndingPosition(targetedPosition, _game->map);
		//				this->ActiveLauchingMovement();
		//			}

		//			this->SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		// We check if the building is still in the produced state
		//		else if (_game->vines.CheckVineHasProducedRessource(this->mapPosition) == true)
		//		{
		//			this->SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
		//		else if (_game->vines.CheckVineHasProducedRessource(this->mapPosition) == false)
		//		{
		//			// For security, we reaffect the worker to the WORKING status
		//			this->SetWorkerStatus(WORKING);

		//			std::cout << "Worker, error during pick up of ressources\n";
		//		}
		//	}


		//}
		//else if (this->actualBuilding == BUILDING_GRAPE_STOMPING_VATS)
		//{
		//	// That mean the worker has already picked up a ressource
		//	if (this->ressourceHeld != nullptr)
		//	{
		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
		//		if (*(this->ressourceHeld) == GRAPES_MUST)
		//		{
		//			int ressourceProduced = _game->stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

		//			*(this->quantityRessourceHeld) += ressourceProduced;
		//			*(this->targetedBuilding) = BUILDING_WINE_PRESS;

		//			this->isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->winePress.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				this->SetEndingPosition(targetedPosition, _game->map);
		//				this->ActiveLauchingMovement();
		//			}

		//			this->SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		else
		//		{
		//			std::cout << "The current worker already has a ressource in hands\n\n";
		//			this->SetWorkerStatus(IDLE);
		//		}
		//	}
		//	else
		//	{
		//		int ressourceProduced = _game->stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
		//		if (ressourceProduced != 0 && ressourceProduced > 0)
		//		{
		//			this->ressourceHeld = new enum TypesOfRessources;
		//			this->quantityRessourceHeld = new int;
		//			this->targetedBuilding = new enum TypeOfBuilding;

		//			*(this->ressourceHeld) = GRAPES_MUST;
		//			*(this->quantityRessourceHeld) = ressourceProduced;
		//			*(this->targetedBuilding) = BUILDING_WINE_PRESS;

		//			this->isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->winePress.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				this->SetEndingPosition(targetedPosition, _game->map);
		//				this->ActiveLauchingMovement();
		//			}

		//			this->SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		// We check if the building is still in the produced state
		//		else if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
		//		{
		//			this->SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
		//		else if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == false)
		//		{
		//			// For security, we reaffect the worker to the WORKING status
		//			this->SetWorkerStatus(WORKING);

		//			std::cout << "Worker, error during pick up of ressources\n";
		//		}
		//	}

		//}
		//else if (this->actualBuilding == BUILDING_WINE_PRESS)
		//{
		//	// That mean the worker has already picked up a ressource
		//	if (this->ressourceHeld != nullptr)
		//	{
		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
		//		if (*(this->ressourceHeld) == GRAPE_JUICE)
		//		{
		//			int ressourceProduced = _game->winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

		//			*(this->quantityRessourceHeld) += ressourceProduced;
		//			*(this->targetedBuilding) = BUILDING_WINE_STOREHOUSE;

		//			this->isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->wineStorehouse.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				this->SetEndingPosition(targetedPosition, _game->map);
		//				this->ActiveLauchingMovement();
		//			}

		//			this->SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";


		//		}
		//		else
		//		{
		//			std::cout << "The current worker already has a ressource in hands\n\n";
		//			this->SetWorkerStatus(IDLE);
		//		}
		//	}
		//	else
		//	{
		//		int ressourceProduced = _game->winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
		//		if (ressourceProduced != 0 && ressourceProduced > 0)
		//		{
		//			this->ressourceHeld = new enum TypesOfRessources;
		//			this->quantityRessourceHeld = new int;
		//			this->targetedBuilding = new enum TypeOfBuilding;

		//			*(this->ressourceHeld) = GRAPE_JUICE;
		//			*(this->quantityRessourceHeld) = ressourceProduced;
		//			*(this->targetedBuilding) = BUILDING_WINE_STOREHOUSE;

		//			this->isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->wineStorehouse.SpecificsBuildingsFindNearestBuilding(this->mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				this->SetEndingPosition(targetedPosition, _game->map);
		//				this->ActiveLauchingMovement();
		//			}

		//			this->SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		// We check if the building is still in the produced state
		//		else if (_game->winePress.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
		//		{
		//			this->SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
		//		else if (_game->winePress.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == false)
		//		{
		//			// For security, we reaffect the worker to the WORKING status
		//			this->SetWorkerStatus(WORKING);

		//			std::cout << "Worker, error during pick up of ressources\n";
		//		}
		//	}

		//}
		//else if (this->actualBuilding == BUILDING_WINE_STOREHOUSE)
		//{
		//	// That mean the worker has already picked up a ressource
		//	if (this->ressourceHeld != nullptr)
		//	{
		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
		//		if (*(this->ressourceHeld) == AMPHORA_OF_WINE)
		//		{
		//			int ressourceProduced = _game->wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

		//			*(this->quantityRessourceHeld) += ressourceProduced;
		//			*(this->targetedBuilding) = BUILDING_STOREHOUSE;

		//			this->isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->storehouse.StorehouseFindNearestBuilding(this->mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				this->SetEndingPosition(targetedPosition, _game->map);
		//				this->ActiveLauchingMovement();
		//			}

		//			this->SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";


		//		}
		//		else
		//		{
		//			std::cout << "The current worker already has a ressource in hands\n\n";
		//			this->SetWorkerStatus(IDLE);
		//		}
		//	}
		//	else
		//	{
		//		int ressourceProduced = _game->wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(this->mapPosition, _game->time->GetFrameTime());

		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
		//		if (ressourceProduced != 0 && ressourceProduced > 0)
		//		{
		//			this->ressourceHeld = new enum TypesOfRessources;
		//			this->quantityRessourceHeld = new int;
		//			this->targetedBuilding = new enum TypeOfBuilding;

		//			*(this->ressourceHeld) = AMPHORA_OF_WINE;
		//			*(this->quantityRessourceHeld) = ressourceProduced;
		//			*(this->targetedBuilding) = BUILDING_STOREHOUSE;

		//			this->isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->storehouse.StorehouseFindNearestBuilding(this->mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				this->SetEndingPosition(targetedPosition, _game->map);
		//				this->ActiveLauchingMovement();
		//			}

		//			this->SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		// We check if the building is still in the produced state
		//		else if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == true)
		//		{
		//			this->SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
		//		else if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(this->mapPosition) == false)
		//		{
		//			// For security, we reaffect the worker to the WORKING status
		//			this->SetWorkerStatus(WORKING);

		//			std::cout << "Worker, error during pick up of ressources\n";
		//		}
		//	}
		//}
		//else
		//{
		//	std::cout << "ERROR, the worker don't find a building where works\n";
		//}

		break;

	case DEPOSIT_RESSOURCES:

		//if (this->ressourceHeld != nullptr)
		//{
		//	switch (*(this->ressourceHeld))
		//	{
		//	case GRAPE_VINE:
		//		break;
		//	case BUNCH_OF_GRAPE:

		//		if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtWorkerPosition(this->mapPosition))
		//		{
		//			this->AddTimeToDeposit(_game->time->GetFrameTime());

		//			if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_GRAPE_STOMPING_VATS].GetDepositingTimeCost())
		//			{
		//				std::cout << "Worker, want to deposit ressources\n";

		//				_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

		//				this->SetTimeToDeposit(RESET);

		//				// Reset of the main data
		//				delete this->ressourceHeld;
		//				delete this->quantityRessourceHeld;
		//				delete this->targetedBuilding;

		//				this->ressourceHeld = nullptr;
		//				this->quantityRessourceHeld = nullptr;
		//				this->targetedBuilding = nullptr;

		//				this->SetWorkerStatus(WORKING);
		//			}
		//		}
		//		else
		//		{
		//			this->SetWorkerStatus(IDLE);
		//		}

		//		break;
		//	case GRAPES_MUST:

		//		if (_game->winePress.ConfirmSpecificBuildingPresenceAtWorkerPosition(this->mapPosition))
		//		{
		//			this->AddTimeToDeposit(_game->time->GetFrameTime());

		//			if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_PRESS].GetDepositingTimeCost())
		//			{
		//				std::cout << "Worker, want to deposit ressources\n";

		//				_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

		//				this->SetTimeToDeposit(RESET);

		//				// Reset of the main data
		//				delete this->ressourceHeld;
		//				delete this->quantityRessourceHeld;
		//				delete this->targetedBuilding;

		//				this->ressourceHeld = nullptr;
		//				this->quantityRessourceHeld = nullptr;
		//				this->targetedBuilding = nullptr;

		//				this->SetWorkerStatus(WORKING);
		//			}
		//		}
		//		else
		//		{
		//			this->SetWorkerStatus(IDLE);
		//		}

		//		break;
		//	case GRAPE_JUICE:

		//		if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtWorkerPosition(this->mapPosition))
		//		{
		//			this->AddTimeToDeposit(_game->time->GetFrameTime());

		//			if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
		//			{
		//				std::cout << "Worker, want to deposit ressources\n";

		//				_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

		//				this->SetTimeToDeposit(RESET);

		//				// Reset of the main data
		//				delete this->ressourceHeld;
		//				delete this->quantityRessourceHeld;
		//				delete this->targetedBuilding;

		//				this->ressourceHeld = nullptr;
		//				this->quantityRessourceHeld = nullptr;
		//				this->targetedBuilding = nullptr;

		//				this->SetWorkerStatus(WORKING);
		//			}
		//		}
		//		else
		//		{
		//			this->SetWorkerStatus(IDLE);
		//		}

		//		break;
		//	case PURE_GRAPE_JUICE:

		//		break;
		//	case GRAPE_MARC:

		//		break;
		//	case DOLIUM:

		//		break;
		//	case AMPHORAS:

		//		this->AddTimeToDeposit(_game->time->GetFrameTime());

		//		if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
		//		{
		//			std::cout << "Worker, want to deposit ressources\n";

		//			_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

		//			this->SetTimeToDeposit(RESET);

		//			// Reset of the main data
		//			delete this->ressourceHeld;
		//			delete this->quantityRessourceHeld;
		//			delete this->targetedBuilding;

		//			this->ressourceHeld = nullptr;
		//			this->quantityRessourceHeld = nullptr;
		//			this->targetedBuilding = nullptr;

		//			this->SetWorkerStatus(WORKING);
		//		}

		//		break;
		//	case AMPHORA_OF_WINE:

		//		if (_game->storehouse.ConfirmStorehousePresenceAtWorkerPosition(this->mapPosition))
		//		{
		//			this->AddTimeToDeposit(_game->time->GetFrameTime());

		//			if (this->GetTimeToDeposit() >= _game->buildings[BUILDING_STOREHOUSE].GetDepositingTimeCost())
		//			{
		//				std::cout << "Worker, deposit amphora of wine ressources\n";

		//				_game->ressources[*(this->ressourceHeld)].AddQuantityOwned(*(this->quantityRessourceHeld));

		//				this->SetTimeToDeposit(RESET);

		//				// Reset of the main data
		//				delete this->ressourceHeld;
		//				delete this->quantityRessourceHeld;
		//				delete this->targetedBuilding;

		//				this->ressourceHeld = nullptr;
		//				this->quantityRessourceHeld = nullptr;
		//				this->targetedBuilding = nullptr;

		//				this->SetWorkerStatus(WORKING);
		//			}
		//		}
		//		else
		//		{
		//			this->SetWorkerStatus(IDLE);
		//		}

		//		break;
		//	case TOOLS:
		//		break;
		//	case WICKER_BASKET:
		//		break;
		//	default:
		//		break;
		//	}

		//}
		//else
		//{
		//	// For security, we reaffect the worker to the WORKING status
		//	this->SetWorkerStatus(WORKING);

		//	std::cout << "Worker, error during deposit of the ressources\n";
		//}

		break;

	default:
		break;
	}
}
