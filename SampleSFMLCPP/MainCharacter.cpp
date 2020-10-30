#include "MainCharacter.h"
#include "GameDefinitions.h"
#include "BuildingsListPlanned.h"


MainCharacter::MainCharacter()
{
	sprite = LoadSprite("Data/Assets/Sprites/Entities/Main_Character.png", 5);

	// A CONFIG / SET TEMPORAIRE
	mapPosition = sf::Vector2f(10, 10);
	
	actualStatus = IDLE;
	isLauchingMovement = false;
	isItWorkingPlace = false;

	buildingTimer = RESET;
	waitingTimer = RESET;

	isPressingEnd = false;
	isPressingStart = false;


	isMainCharacterSelected = true;
	isCurrentlyBuilding = false;


	std::cout << "Ouvrier initialisé\n\n";
}

MainCharacter::~MainCharacter()
{

}


void MainCharacter::InitPathfinding(struct Game *_game)
{
	if (isLauchingMovement == true)
	{
		if (path != nullptr)
		{
			delete path;
			path = nullptr;
		}

		path = new Pathfinding;

		path->InitMapCopyPathfinding(sf::Vector2i(_game->numberColumns, _game->numberLines), _game->map, (FIRST_FLOOR + COLLISIONS_ID)); // TEMPORAIRE

		path->SetPathStartingPosition((sf::Vector2i)mapPosition); // TEMPORAIRE
		path->SetPathEndingPosition((sf::Vector2i)mapEndPosition); // TEMPORAIRE
	}
}


void MainCharacter::SetMainCharacterPosition(const sf::Vector2f &_mapPosition)
{
	mapPosition = _mapPosition;
}


void MainCharacter::SetMainCharacterEndingPosition(sf::Vector2i _mapPosition, unsigned short ***_map)
{
	if (_mapPosition.x >= 0 && _mapPosition.y >= 0)
	{
		mapEndPosition = (sf::Vector2f)_mapPosition;
	}

	//
	// CRASH SORTIE DE TABLEAU A CAUSE DE X > 100000
	//

	// If this place is a building
	if (_map[FIRST_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		isItWorkingPlace = true;
		actualBuilding = (enum TypeOfBuilding)_map[FIRST_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x];

		//std::cout << "This is a working place : " << _map[FIRST_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x] << std::endl;
	}
	else if (_map[ZERO_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		isItWorkingPlace = true;
		actualBuilding = (enum TypeOfBuilding)_map[ZERO_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x];

		//std::cout << "This is a working place : " << _map[FIRST_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x] << std::endl;
	}
	else
	{
		isItWorkingPlace = false;
	}
}

void MainCharacter::SetSpriteScale(const sf::Vector2f &_newScale)
{
	sprite.setScale(sf::Vector2f(_newScale.x * 1.3f, _newScale.y * 1.3f));
}

void MainCharacter::SetMainCharacterStatus(const enum WorkerStatus &_newStatus, const bool &_isLaunchingMovement)
{
	actualStatus = _newStatus;

	if (_isLaunchingMovement == true)
	{
		isLauchingMovement = true;
	}
}

void MainCharacter::SetIsMainCharacterSelected(const bool &_isMainCharacterSelected)
{
	isMainCharacterSelected = _isMainCharacterSelected;
}

void MainCharacter::SetWorkerIsInWorkingPlace(const bool &_isItWorkingPlace)
{
	isItWorkingPlace = _isItWorkingPlace;
}

void MainCharacter::SetIsCurrentlyBuilding(const bool &_isCurrentlyBuilding)
{
	isCurrentlyBuilding = _isCurrentlyBuilding;
}



sf::Vector2f MainCharacter::GetMainCharacterPosition()
{
	return mapPosition;
}

bool MainCharacter::IsMainCharacterPosition(const sf::Vector2i &_mapPosition)
{
	return (sf::Vector2i(mapPosition) == _mapPosition) ? true : false;
}

sf::Vector2f MainCharacter::GetMainCharacterEndingPosition()
{
	return mapEndPosition;
}

sf::Sprite MainCharacter::GetSprite()
{
	return sprite;
}

bool MainCharacter::GetIsMainCharacterSelected()
{
	return isMainCharacterSelected;
}

enum WorkerStatus MainCharacter::GetWorkerStatus()
{
	return actualStatus;
}

bool MainCharacter::GetIsCurrentlyBuilding()
{
	return isCurrentlyBuilding;
}

void MainCharacter::UpdatePathAndActivities(struct Game *_game)
{
	float speed(RESET);

	switch (actualStatus)
	{
	case IDLE:

		if (isLauchingMovement == true)
		{
			//std::cout << "Changement de status vers Waiting Movement\n";

			SetMainCharacterStatus(WAITING_MOVEMENT);
			isLauchingMovement = false;
		}
		else
		{
			waitingTimer += _game->time->GetFrameTime();

			// If the main character is waiting, but have buildings planned to be built, we check and launch his movement
			if (isCurrentlyBuilding)
			{
				// Test if the list is empty or not
				if (_game->buildingsListPlanned->IsBuildingListIsEmpty())
				{
					isCurrentlyBuilding = false;
					waitingTimer = RESET;
				}
				else
				{
					if (waitingTimer > 3)
					{
						// Set the position of the next building plannified
						SetMainCharacterEndingPosition(_game->buildingsListPlanned->GetBuildingPositionInMap(), _game->map);

						SetMainCharacterStatus(IDLE, true);

						waitingTimer = RESET;
					}
				}
			}
		}
		

		break;

	case WAITING_MOVEMENT:

		//std::cout << "WAITING MOVEMENT\n";
		path->MainStatePathfinding();

		// In the case that we find a way, we start to move
		if (path->GetActualStatus() == PATHFINDING_FIND_WAY_FIRST
			|| path->GetActualStatus() == PATHFINDING_FIND_WAY)
		{
			SetMainCharacterStatus(MOVEMENT);
		}
		// In the case that we don't find any way to join the target, we stay where we're
		else if (path->GetActualStatus() == PATHFINDING_FIND_NO_WAY)
		{
			SetMainCharacterStatus(IDLE);
			delete path;
			path = nullptr;
		}

		break;

	case MOVEMENT:

		// Speed modification depending on the type of soil
		if (_game->map[ZERO_FLOOR + COLLISIONS_ID][(int)mapPosition.y][(int)mapPosition.x] == PATH)
		{
			speed = _game->time->GetFrameTime() * 2.25f;
		}
		else if (_game->map[ZERO_FLOOR + COLLISIONS_ID][(int)mapPosition.y][(int)mapPosition.x] == STONE_PATH)
		{
			speed = _game->time->GetFrameTime() * 3.5f;
		}
		else if (_game->map[ZERO_FLOOR + COLLISIONS_ID][(int)mapPosition.y][(int)mapPosition.x] == ROAD)
		{
			speed = _game->time->GetFrameTime() * 5;
		}
		else
		{
			speed = _game->time->GetFrameTime() * 1.5f;
		}

		//std::cout << "MOVEMENT\n";
		path->WalkProcess(&mapPosition, speed);



		// If the path ask to be deleted, that mean that the character has reached his destination
		if (path->GetActualStatus() == PATHFINDING_NEED_TO_BE_DELETED)
		{
			// If at the initialisation we find that location is a working place, we change the status of the worker to WORKING
			if (isItWorkingPlace == true)
			{
				if (isCurrentlyBuilding)
				{
					SetMainCharacterStatus(BUILDING);
				}
				else
				{
					SetMainCharacterStatus(WORKING);
				}
			}
			// Else, the character just wait at the IDLE status
			else
			{
				SetMainCharacterStatus(IDLE);
			}

			// We delete the path and init his pointer to null
			delete path;
			path = nullptr;
		}

		break;

	case BUILDING:
		if (isCurrentlyBuilding == true
			&& _game->buildingsListPlanned->GetBuildingPositionInMap() == (sf::Vector2i)mapPosition)
		{
			buildingTimer += _game->time->GetFrameTime();

			if (buildingTimer >= 2)
			{
				// Security
				if (_game->buildingsListPlanned->GetBuildingID() != -1
					&& _game->buildingsListPlanned->GetBuildingPositionInMap() != sf::Vector2i(-1, -1)
					&& _game->buildingsListPlanned->GetBuildingSize() != sf::Vector2i(-1, -1))
				{
					// Clear the ground by putting sand floor
					for (int y = 0; y < _game->buildingsListPlanned->GetBuildingSize().y; y++)
					{
						for (int x = 0; x < _game->buildingsListPlanned->GetBuildingSize().x; x++)
						{
							_game->map[ZERO_FLOOR + SPRITE_ID]
								[_game->buildingsListPlanned->GetBuildingPositionInMap().y - y]
							[_game->buildingsListPlanned->GetBuildingPositionInMap().x - x] = 1;
						}
					}

					// Modifier couche pour ID
					_game->buildWindow.SetBuildingOnMap(_game, FIRST_FLOOR, _game->buildingsListPlanned->GetBuildingID(), COLLISION, _game->buildingsListPlanned->GetBuildingPositionInMap());




					// We add informations to the concerned linkedlist
					switch (_game->buildingsListPlanned->GetBuildingID())
					{
					case BUILDING_VINES:
						_game->vines.AddNewVineToList((sf::Vector2f)_game->buildingsListPlanned->GetBuildingPositionInMap());
						break;
					case BUILDING_GRAPE_STOMPING_VATS:
						_game->stompingVats.AddNewBuildingToList((sf::Vector2f)_game->buildingsListPlanned->GetBuildingPositionInMap());
						break;
					case BUILDING_WINE_PRESS:
						_game->winePress.AddNewBuildingToList((sf::Vector2f)_game->buildingsListPlanned->GetBuildingPositionInMap());
						break;
					case BUILDING_WINE_STOREHOUSE:
						_game->wineStorehouse.AddNewBuildingToList((sf::Vector2f)_game->buildingsListPlanned->GetBuildingPositionInMap());
						break;
					case BUILDING_STOREHOUSE:
						_game->storehouse.AddNewBuildingToList((sf::Vector2f)_game->buildingsListPlanned->GetBuildingPositionInMap());
						_game->stall->AddStorehousePosition((sf::Vector2f)_game->buildingsListPlanned->GetBuildingPositionInMap());
						break;
					case BUILDING_STALL:
						_game->stall->AddNewBuilding((sf::Vector2f)_game->buildingsListPlanned->GetBuildingPositionInMap());
						break;
						/*case BUILDING_VILLA:
							break;
						case BUILDING_DORMITORY:
							break;*/
					default:
						break;
					}


					// Deletion of the current first building in the list
					_game->buildingsListPlanned->DeleteCurrentFirstBuildingInList();

					//Update the workers's paths
					_game->workersList->CheckAndUpdateWorkersPath(_game->map);


					// Test if the list is empty or not
					if (_game->buildingsListPlanned->IsBuildingListIsEmpty())
					{
						isCurrentlyBuilding = false;
						SetMainCharacterStatus(IDLE);
					}
					else
					{
						// Set the position of the next building plannified
						SetMainCharacterEndingPosition(_game->buildingsListPlanned->GetBuildingPositionInMap(), _game->map);

						SetMainCharacterStatus(IDLE, true);
					}

					// Reset the builder timer of the main character
					buildingTimer -= 2;
				}
				else
				{
					SetMainCharacterStatus(IDLE);
				}
			}
			
		}
		else
		{
			SetMainCharacterStatus(IDLE);
		}

		break;
	case WORKING:

		if (actualBuilding == BUILDING_VILLA)
		{
			if (_game->actualGameState != VILLA_MANAGEMENT)
			{
				_game->actualGameState = VILLA_MANAGEMENT;
			}
			else
			{
				SetMainCharacterStatus(IDLE);
			}
		}
		else
		{
			SetMainCharacterStatus(IDLE);
		}
		//else if (actualBuilding == BUILDING_GRAPE_STOMPING_VATS)
		//{
		//	// We send at the stomping vats building the confirmation that a worker is there
		//	if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition) == true)
		//	{
		//		//std::cout << "Working ...\n";

		//		if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
		//		{
		//			SetWorkerStatus(PICKUP_RESSOURCES);
		//		}

		//	}
		//	else
		//	{

		//	}

		//}
		//else if (actualBuilding == BUILDING_WINE_PRESS)
		//{
		//	// We send at the wine press building the confirmation that a worker is there
		//	if (_game->winePress.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition) == true)
		//	{
		//		//std::cout << "Working ...\n";

		//		if (_game->winePress.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
		//		{
		//			SetWorkerStatus(PICKUP_RESSOURCES);
		//		}


		//	}
		//	else
		//	{

		//	}

		//}
		//else if (actualBuilding == BUILDING_WINE_STOREHOUSE)
		//{
		//	// We send at the wine storehouse building the confirmation that a worker is there
		//	if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition) == true)
		//	{
		//		//std::cout << "Working ...\n";

		//		if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
		//		{
		//			SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//	}
		//	else
		//	{

		//	}

		//}
		//else if (actualBuilding == BUILDING_STOREHOUSE)
		//{
		//	// We send at the storehouse building the confirmation that a worker is there
		//	if (_game->storehouse.ConfirmStorehousePresenceAtWorkerPosition(mapPosition) == true)
		//	{
		//		//std::cout << "Working ...\n";
		//	}
		//	else
		//	{

		//	}

		//}
		//else if (actualBuilding == BUILDING_STALL)
		//{
		//	// We send at the wine storehouse building the confirmation that a worker is there
		//	if (_game->stall->ConfirmPresenceAtWorkerPosition(mapPosition) == true)
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

		//if (actualBuilding == BUILDING_VINES)
		//{
		//	// That mean the worker has already picked up a ressource
		//	if (ressourceHeld != nullptr)
		//	{
		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
		//		if (*(ressourceHeld) == BUNCH_OF_GRAPE)
		//		{
		//			int ressourceProduced = _game->vines.VinesSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());

		//			*(quantityRessourceHeld) += ressourceProduced;
		//			*(targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;

		//			isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->stompingVats.SpecificsBuildingsFindNearestBuilding(mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				SetEndingPosition(targetedPosition, _game->map);
		//				ActiveLauchingMovement();
		//			}

		//			SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		else
		//		{
		//			std::cout << "The current worker already has a ressource in hands\n\n";
		//			SetWorkerStatus(IDLE);
		//		}
		//	}
		//	else
		//	{
		//		int ressourceProduced = _game->vines.VinesSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());

		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
		//		if (ressourceProduced != 0 && ressourceProduced > 0)
		//		{
		//			ressourceHeld = new enum TypesOfRessources;
		//			quantityRessourceHeld = new int;
		//			targetedBuilding = new enum TypeOfBuilding;

		//			*(ressourceHeld) = BUNCH_OF_GRAPE;
		//			*(quantityRessourceHeld) = ressourceProduced;
		//			*(targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;

		//			isItWorkingPlace = false;


		//			sf::Vector2i targetedPosition = _game->stompingVats.SpecificsBuildingsFindNearestBuilding(mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				SetEndingPosition(targetedPosition, _game->map);
		//				ActiveLauchingMovement();
		//			}

		//			SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		// We check if the building is still in the produced state
		//		else if (_game->vines.CheckVineHasProducedRessource(mapPosition) == true)
		//		{
		//			SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
		//		else if (_game->vines.CheckVineHasProducedRessource(mapPosition) == false)
		//		{
		//			// For security, we reaffect the worker to the WORKING status
		//			SetWorkerStatus(WORKING);

		//			std::cout << "Worker, error during pick up of ressources\n";
		//		}
		//	}


		//}
		//else if (actualBuilding == BUILDING_GRAPE_STOMPING_VATS)
		//{
		//	// That mean the worker has already picked up a ressource
		//	if (ressourceHeld != nullptr)
		//	{
		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
		//		if (*(ressourceHeld) == GRAPES_MUST)
		//		{
		//			int ressourceProduced = _game->stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());

		//			*(quantityRessourceHeld) += ressourceProduced;
		//			*(targetedBuilding) = BUILDING_WINE_PRESS;

		//			isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->winePress.SpecificsBuildingsFindNearestBuilding(mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				SetEndingPosition(targetedPosition, _game->map);
		//				ActiveLauchingMovement();
		//			}

		//			SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		else
		//		{
		//			std::cout << "The current worker already has a ressource in hands\n\n";
		//			SetWorkerStatus(IDLE);
		//		}
		//	}
		//	else
		//	{
		//		int ressourceProduced = _game->stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());

		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
		//		if (ressourceProduced != 0 && ressourceProduced > 0)
		//		{
		//			ressourceHeld = new enum TypesOfRessources;
		//			quantityRessourceHeld = new int;
		//			targetedBuilding = new enum TypeOfBuilding;

		//			*(ressourceHeld) = GRAPES_MUST;
		//			*(quantityRessourceHeld) = ressourceProduced;
		//			*(targetedBuilding) = BUILDING_WINE_PRESS;

		//			isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->winePress.SpecificsBuildingsFindNearestBuilding(mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				SetEndingPosition(targetedPosition, _game->map);
		//				ActiveLauchingMovement();
		//			}

		//			SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		// We check if the building is still in the produced state
		//		else if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
		//		{
		//			SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
		//		else if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(mapPosition) == false)
		//		{
		//			// For security, we reaffect the worker to the WORKING status
		//			SetWorkerStatus(WORKING);

		//			std::cout << "Worker, error during pick up of ressources\n";
		//		}
		//	}

		//}
		//else if (actualBuilding == BUILDING_WINE_PRESS)
		//{
		//	// That mean the worker has already picked up a ressource
		//	if (ressourceHeld != nullptr)
		//	{
		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
		//		if (*(ressourceHeld) == GRAPE_JUICE)
		//		{
		//			int ressourceProduced = _game->winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());

		//			*(quantityRessourceHeld) += ressourceProduced;
		//			*(targetedBuilding) = BUILDING_WINE_STOREHOUSE;

		//			isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->wineStorehouse.SpecificsBuildingsFindNearestBuilding(mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				SetEndingPosition(targetedPosition, _game->map);
		//				ActiveLauchingMovement();
		//			}

		//			SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";


		//		}
		//		else
		//		{
		//			std::cout << "The current worker already has a ressource in hands\n\n";
		//			SetWorkerStatus(IDLE);
		//		}
		//	}
		//	else
		//	{
		//		int ressourceProduced = _game->winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());

		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
		//		if (ressourceProduced != 0 && ressourceProduced > 0)
		//		{
		//			ressourceHeld = new enum TypesOfRessources;
		//			quantityRessourceHeld = new int;
		//			targetedBuilding = new enum TypeOfBuilding;

		//			*(ressourceHeld) = GRAPE_JUICE;
		//			*(quantityRessourceHeld) = ressourceProduced;
		//			*(targetedBuilding) = BUILDING_WINE_STOREHOUSE;

		//			isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->wineStorehouse.SpecificsBuildingsFindNearestBuilding(mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				SetEndingPosition(targetedPosition, _game->map);
		//				ActiveLauchingMovement();
		//			}

		//			SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		// We check if the building is still in the produced state
		//		else if (_game->winePress.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
		//		{
		//			SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
		//		else if (_game->winePress.CheckSpecificBuildingHasProducedRessource(mapPosition) == false)
		//		{
		//			// For security, we reaffect the worker to the WORKING status
		//			SetWorkerStatus(WORKING);

		//			std::cout << "Worker, error during pick up of ressources\n";
		//		}
		//	}

		//}
		//else if (actualBuilding == BUILDING_WINE_STOREHOUSE)
		//{
		//	// That mean the worker has already picked up a ressource
		//	if (ressourceHeld != nullptr)
		//	{
		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
		//		if (*(ressourceHeld) == AMPHORA_OF_WINE)
		//		{
		//			int ressourceProduced = _game->wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());

		//			*(quantityRessourceHeld) += ressourceProduced;
		//			*(targetedBuilding) = BUILDING_STOREHOUSE;

		//			isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->storehouse.StorehouseFindNearestBuilding(mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				SetEndingPosition(targetedPosition, _game->map);
		//				ActiveLauchingMovement();
		//			}

		//			SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";


		//		}
		//		else
		//		{
		//			std::cout << "The current worker already has a ressource in hands\n\n";
		//			SetWorkerStatus(IDLE);
		//		}
		//	}
		//	else
		//	{
		//		int ressourceProduced = _game->wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());

		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
		//		if (ressourceProduced != 0 && ressourceProduced > 0)
		//		{
		//			ressourceHeld = new enum TypesOfRessources;
		//			quantityRessourceHeld = new int;
		//			targetedBuilding = new enum TypeOfBuilding;

		//			*(ressourceHeld) = AMPHORA_OF_WINE;
		//			*(quantityRessourceHeld) = ressourceProduced;
		//			*(targetedBuilding) = BUILDING_STOREHOUSE;

		//			isItWorkingPlace = false;

		//			sf::Vector2i targetedPosition = _game->storehouse.StorehouseFindNearestBuilding(mapPosition);

		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
		//			{
		//				SetEndingPosition(targetedPosition, _game->map);
		//				ActiveLauchingMovement();
		//			}

		//			SetWorkerStatus(IDLE);

		//			std::cout << "Worker, picked up ressources\n";
		//		}
		//		// We check if the building is still in the produced state
		//		else if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
		//		{
		//			SetWorkerStatus(PICKUP_RESSOURCES);
		//		}
		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
		//		else if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(mapPosition) == false)
		//		{
		//			// For security, we reaffect the worker to the WORKING status
		//			SetWorkerStatus(WORKING);

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

		//if (ressourceHeld != nullptr)
		//{
		//	switch (*(ressourceHeld))
		//	{
		//	case GRAPE_VINE:
		//		break;
		//	case BUNCH_OF_GRAPE:

		//		if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition))
		//		{
		//			AddTimeToDeposit(_game->time->GetFrameTime());

		//			if (GetTimeToDeposit() >= _game->buildings[BUILDING_GRAPE_STOMPING_VATS].GetDepositingTimeCost())
		//			{
		//				std::cout << "Worker, want to deposit ressources\n";

		//				_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));

		//				SetTimeToDeposit(RESET);

		//				// Reset of the main data
		//				delete ressourceHeld;
		//				delete quantityRessourceHeld;
		//				delete targetedBuilding;

		//				ressourceHeld = nullptr;
		//				quantityRessourceHeld = nullptr;
		//				targetedBuilding = nullptr;

		//				SetWorkerStatus(WORKING);
		//			}
		//		}
		//		else
		//		{
		//			SetWorkerStatus(IDLE);
		//		}

		//		break;
		//	case GRAPES_MUST:

		//		if (_game->winePress.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition))
		//		{
		//			AddTimeToDeposit(_game->time->GetFrameTime());

		//			if (GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_PRESS].GetDepositingTimeCost())
		//			{
		//				std::cout << "Worker, want to deposit ressources\n";

		//				_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));

		//				SetTimeToDeposit(RESET);

		//				// Reset of the main data
		//				delete ressourceHeld;
		//				delete quantityRessourceHeld;
		//				delete targetedBuilding;

		//				ressourceHeld = nullptr;
		//				quantityRessourceHeld = nullptr;
		//				targetedBuilding = nullptr;

		//				SetWorkerStatus(WORKING);
		//			}
		//		}
		//		else
		//		{
		//			SetWorkerStatus(IDLE);
		//		}

		//		break;
		//	case GRAPE_JUICE:

		//		if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition))
		//		{
		//			AddTimeToDeposit(_game->time->GetFrameTime());

		//			if (GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
		//			{
		//				std::cout << "Worker, want to deposit ressources\n";

		//				_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));

		//				SetTimeToDeposit(RESET);

		//				// Reset of the main data
		//				delete ressourceHeld;
		//				delete quantityRessourceHeld;
		//				delete targetedBuilding;

		//				ressourceHeld = nullptr;
		//				quantityRessourceHeld = nullptr;
		//				targetedBuilding = nullptr;

		//				SetWorkerStatus(WORKING);
		//			}
		//		}
		//		else
		//		{
		//			SetWorkerStatus(IDLE);
		//		}

		//		break;
		//	case PURE_GRAPE_JUICE:

		//		break;
		//	case GRAPE_MARC:

		//		break;
		//	case DOLIUM:

		//		break;
		//	case AMPHORAS:

		//		AddTimeToDeposit(_game->time->GetFrameTime());

		//		if (GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
		//		{
		//			std::cout << "Worker, want to deposit ressources\n";

		//			_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));

		//			this->SetTimeToDeposit(RESET);

		//			// Reset of the main data
		//			delete ressourceHeld;
		//			delete quantityRessourceHeld;
		//			delete targetedBuilding;

		//			ressourceHeld = nullptr;
		//			quantityRessourceHeld = nullptr;
		//			targetedBuilding = nullptr;

		//			SetWorkerStatus(WORKING);
		//		}

		//		break;
		//	case AMPHORA_OF_WINE:

		//		if (_game->storehouse.ConfirmStorehousePresenceAtWorkerPosition(mapPosition))
		//		{
		//			AddTimeToDeposit(_game->time->GetFrameTime());

		//			if (GetTimeToDeposit() >= _game->buildings[BUILDING_STOREHOUSE].GetDepositingTimeCost())
		//			{
		//				std::cout << "Worker, deposit amphora of wine ressources\n";

		//				_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));

		//				SetTimeToDeposit(RESET);

		//				// Reset of the main data
		//				delete ressourceHeld;
		//				delete quantityRessourceHeld;
		//				delete targetedBuilding;

		//				ressourceHeld = nullptr;
		//				quantityRessourceHeld = nullptr;
		//				targetedBuilding = nullptr;

		//				SetWorkerStatus(WORKING);
		//			}
		//		}
		//		else
		//		{
		//			SetWorkerStatus(IDLE);
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
		//	SetWorkerStatus(WORKING);

		//	std::cout << "Worker, error during deposit of the ressources\n";
		//}

		break;

	default:
		break;
	}
}
