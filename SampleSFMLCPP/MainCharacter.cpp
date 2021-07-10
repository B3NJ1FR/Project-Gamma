#include "MainCharacter.h"
#include "Map.h"
#include "GameDefinitions.h"

// WARNING UPDATE ACTIVITIES

MainCharacter::MainCharacter()
{
	m_sprite = LoadSprite("Data/Assets/Sprites/Entities/Main_Character.png", 5);
	m_spriteCharaSelected = LoadSprite("Data/Assets/Sprites/Entities/worker_selected.png", 1);

	// A CONFIG / SET TEMPORAIRE
	m_mapPosition = sf::Vector2f(10, 10);
	
	m_actualStatus = WorkerStatus::IDLE;
	m_isLauchingMovement = false;
	m_isItWorkingPlace = false;

	m_buildingTimer = RESET;
	m_waitingTimer = RESET;

	isPressingEnd = false;
	isPressingStart = false;


	m_isMainCharacterSelected = true;
	m_isCurrentlyBuilding = false;


	std::cout << "Ouvrier initialisé\n\n";
}

MainCharacter::~MainCharacter()
{
	if (m_targetedBuilding != nullptr)
	{
		delete[] m_targetedBuilding;
	}
}


void MainCharacter::InitPathfinding()
{
	if (m_isLauchingMovement == true)
	{
		if (m_path != nullptr)
		{
			delete m_path;
			m_path = nullptr;
		}

		m_path = new Pathfinding;

		m_path->InitMapCopyPathfinding(FIRST_FLOOR + COLLISIONS_ID); // TEMPORAIRE

		m_path->SetPathStartingPosition((sf::Vector2i)m_mapPosition); // TEMPORAIRE
		m_path->SetPathEndingPosition((sf::Vector2i)m_mapEndPosition); // TEMPORAIRE
	}
}



void MainCharacter::SetMainCharacterEndingPosition(const sf::Vector2i& _mapPosition, Map *_map)
{
	if (_mapPosition.x >= 0 && _mapPosition.y >= 0
		&& _mapPosition.x < _map->GetNumberOfColumns() && _mapPosition.y < _map->GetNumberOfLines())
	{
		m_mapEndPosition = (sf::Vector2f)_mapPosition;
	}

	// If this place is a building
	if (_map->GetMap()[FIRST_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		m_isItWorkingPlace = true;
		m_actualBuilding = (TypeOfBuilding)_map->GetMap()[FIRST_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x];

		//std::cout << "This is a working place : " << _map[FIRST_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x] << std::endl;
	}
	else if (_map->GetMap()[ZERO_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		m_isItWorkingPlace = true;
		m_actualBuilding = (TypeOfBuilding)_map->GetMap()[ZERO_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x];

		//std::cout << "This is a working place : " << _map[FIRST_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x] << std::endl;
	}
	else
	{
		m_isItWorkingPlace = false;
	}
}


void MainCharacter::SetMainCharacterStatus(const enum WorkerStatus &_newStatus, const bool &_isLaunchingMovement)
{
	m_actualStatus = _newStatus;

	if (_isLaunchingMovement == true)
	{
		m_isLauchingMovement = true;
	}
}


void MainCharacter::DisplayMainCharacter(const sf::Vector2i& _actualPosition, const sf::Vector3f& _cameraPosition, const sf::Vector2i& _screenSize, const sf::Vector2f& _gameScale, sf::RenderWindow& _window)
{
	sf::Vector2f tileCoordinates = WorldToScreen(m_mapPosition);
	sf::Vector2f cameraIso = WorldToScreen(_cameraPosition.x, _cameraPosition.y);

	m_sprite.setScale(_gameScale);

	// Character sprite
	BlitSprite(m_sprite,
		(_screenSize.x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _cameraPosition.z),
		(_screenSize.y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _cameraPosition.z) - 16,
		0, _window);

	if (m_isMainCharacterSelected)
	{
		// Selected icon sprite
		BlitSprite(m_spriteCharaSelected,
			(_screenSize.x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _cameraPosition.z),
			(_screenSize.y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _cameraPosition.z) - 90,
			0, _window);
	}
}

//void MainCharacter::UpdatePathAndActivities(Map* _map, TimeManagement* _time, BuildingManagement* _builds, BuildingsListPlanned* _buildingsListPlanned, BuildWindow* _buildWindow, Ressources* _ressources)
//{
//	float speed(RESET);
//
//	switch (m_actualStatus)
//	{
//	case IDLE:
//
//		if (m_isLauchingMovement == true)
//		{
//			//std::cout << "Changement de status vers Waiting Movement\n";
//
//			SetMainCharacterStatus(WAITING_MOVEMENT);
//			m_isLauchingMovement = false;
//		}
//		else
//		{
//			m_waitingTimer += _time->GetFrameTime();
//
//			// If the main character is waiting, but have buildings planned to be built, we check and launch his movement
//			if (m_isCurrentlyBuilding)
//			{
//				// Test if the list is empty or not
//				if (1 == 1/*_game->buildingsListPlanned->IsBuildingListIsEmpty()*/)
//				{
//					m_isCurrentlyBuilding = false;
//					m_waitingTimer = RESET;
//				}
//				else
//				{
//					if (m_waitingTimer > 3)
//					{
//						// Set the position of the next building plannified
//						SetMainCharacterEndingPosition(_buildingsListPlanned->GetBuildingPositionInMap(), _map);
//
//						SetMainCharacterStatus(IDLE, true);
//
//						m_waitingTimer = RESET;
//					}
//				}
//			}
//		}
//		
//
//		break;
//
//	case WAITING_MOVEMENT:
//
//		//std::cout << "WAITING MOVEMENT\n";
//		m_path->MainStatePathfinding();
//
//		// In the case that we find a way, we start to move
//		if (m_path->GetActualStatus() == PATHFINDING_FIND_WAY_FIRST
//			|| m_path->GetActualStatus() == PATHFINDING_FIND_WAY)
//		{
//			SetMainCharacterStatus(MOVEMENT);
//		}
//		// In the case that we don't find any way to join the target, we stay where we're
//		else if (m_path->GetActualStatus() == PATHFINDING_FIND_NO_WAY)
//		{
//			SetMainCharacterStatus(IDLE);
//			delete m_path;
//			m_path = nullptr;
//		}
//
//		break;
//
//	case MOVEMENT:
//
//		// Speed modification depending on the type of soil
//		if (_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == PATH)
//		{
//			speed = _time->GetFrameTime() * 2.25f;
//		}
//		else if (_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == STONE_PATH)
//		{
//			speed = _time->GetFrameTime() * 3.5f;
//		}
//		else if (_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == ROAD)
//		{
//			speed = _time->GetFrameTime() * 5;
//		}
//		else
//		{
//			speed = _time->GetFrameTime() * 1.5f;
//		}
//
//		//std::cout << "MOVEMENT\n";
//		m_path->WalkProcess(&m_mapPosition, speed);
//
//
//
//		// If the path ask to be deleted, that mean that the character has reached his destination
//		if (m_path->GetActualStatus() == PATHFINDING_NEED_TO_BE_DELETED)
//		{
//			// If at the initialisation we find that location is a working place, we change the status of the worker to WORKING
//			if (m_isItWorkingPlace == true)
//			{
//				if (m_isCurrentlyBuilding)
//				{
//					SetMainCharacterStatus(BUILDING);
//				}
//				else
//				{
//					SetMainCharacterStatus(WORKING);
//				}
//			}
//			// Else, the character just wait at the IDLE status
//			else
//			{
//				SetMainCharacterStatus(IDLE);
//			}
//
//			// We delete the path and init his pointer to null
//			delete m_path;
//			m_path = nullptr;
//		}
//
//		break;
//
//	case BUILDING:
//		if (m_isCurrentlyBuilding == true
//			&& _buildingsListPlanned->GetBuildingPositionInMap() == (sf::Vector2i)m_mapPosition)
//		{
//			m_buildingTimer += _time->GetFrameTime();
//
//			if (m_buildingTimer >= 2)
//			{
//				// Security
//				if (_buildingsListPlanned->GetBuildingID() != -1
//					&& _buildingsListPlanned->GetBuildingPositionInMap() != sf::Vector2i(-1, -1)
//					&& _buildingsListPlanned->GetBuildingSize() != sf::Vector2i(-1, -1))
//				{
//					// Clear the ground by putting sand floor
//					for (int y = 0; y < _buildingsListPlanned->GetBuildingSize().y; y++)
//					{
//						for (int x = 0; x < _buildingsListPlanned->GetBuildingSize().x; x++)
//						{
//							_map->GetMap()[ZERO_FLOOR + SPRITE_ID]
//								[_buildingsListPlanned->GetBuildingPositionInMap().y - y]
//							[_buildingsListPlanned->GetBuildingPositionInMap().x - x] = 1;
//						}
//					}
//
//					// Modifier couche pour ID
//					_buildWindow->SetBuildingOnMap(_map, _builds, Floors::FIRST_FLOOR, _buildingsListPlanned->GetBuildingID(), TypesOfCollisions::COLLISION, _buildingsListPlanned->GetBuildingPositionInMap());
//
//
//
//
//					// We add informations to the concerned linkedlist
//					switch (_buildingsListPlanned->GetBuildingID())
//					{
//					case BUILDING_VINES:
//						_builds->m_vines.AddNewVineToList((sf::Vector2f)_buildingsListPlanned->GetBuildingPositionInMap());
//						break;
//					case BUILDING_GRAPE_STOMPING_VATS:
//						_builds->m_stompingVats.AddNewBuildingToList((sf::Vector2f)_buildingsListPlanned->GetBuildingPositionInMap());
//						break;
//					case BUILDING_WINE_PRESS:
//						_builds->m_winePress.AddNewBuildingToList((sf::Vector2f)_buildingsListPlanned->GetBuildingPositionInMap());
//						break;
//					case BUILDING_WINE_STOREHOUSE:
//						_builds->m_wineStorehouse.AddNewBuildingToList((sf::Vector2f)_buildingsListPlanned->GetBuildingPositionInMap());
//						break;
//					case BUILDING_STOREHOUSE:
//						_builds->m_storehouse.AddNewBuildingToList((sf::Vector2f)_buildingsListPlanned->GetBuildingPositionInMap());
//						_builds->m_stall->AddStorehousePosition((sf::Vector2f)_buildingsListPlanned->GetBuildingPositionInMap());
//						break;
//					case BUILDING_STALL:
//						_builds->m_stall->AddNewBuilding((sf::Vector2f)_buildingsListPlanned->GetBuildingPositionInMap());
//						break;
//						/*case BUILDING_VILLA:
//							break;
//						case BUILDING_DORMITORY:
//							break;*/
//					default:
//						break;
//					}
//
//
//					// Deletion of the current first building in the list
//					_buildingsListPlanned->DeleteCurrentFirstBuildingInList();
//
//					//Update the workers's paths
//					_workersList->CheckAndUpdateWorkersPath(_map->GetMap());
//
//
//					// Test if the list is empty or not
//					if (1 == 1/*_game->buildingsListPlanned->IsBuildingListIsEmpty()*/)
//					{
//						m_isCurrentlyBuilding = false;
//						SetMainCharacterStatus(IDLE);
//					}
//					else
//					{
//						// Set the position of the next building plannified
//						SetMainCharacterEndingPosition(_buildingsListPlanned->GetBuildingPositionInMap(), _map);
//
//						SetMainCharacterStatus(IDLE, true);
//					}
//
//					// Reset the builder timer of the main character
//					m_buildingTimer -= 2;
//				}
//				else
//				{
//					SetMainCharacterStatus(IDLE);
//				}
//			}
//			
//		}
//		else
//		{
//			SetMainCharacterStatus(IDLE);
//		}
//
//		break;
//	case WORKING:
//
//		if (m_actualBuilding == BUILDING_VILLA)
//		{
//			if (_game->m_actualGameState != VILLA_MANAGEMENT)
//			{
//				_game->m_actualGameState = VILLA_MANAGEMENT;
//			}
//			else
//			{
//				SetMainCharacterStatus(IDLE);
//			}
//		}
//		else
//		{
//			SetMainCharacterStatus(IDLE);
//		}
//		//else if (actualBuilding == BUILDING_GRAPE_STOMPING_VATS)
//		//{
//		//	// We send at the stomping vats building the confirmation that a worker is there
//		//	if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition) == true)
//		//	{
//		//		//std::cout << "Working ...\n";
//
//		//		if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
//		//		{
//		//			SetWorkerStatus(PICKUP_RESSOURCES);
//		//		}
//
//		//	}
//		//	else
//		//	{
//
//		//	}
//
//		//}
//		//else if (actualBuilding == BUILDING_WINE_PRESS)
//		//{
//		//	// We send at the wine press building the confirmation that a worker is there
//		//	if (_game->winePress.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition) == true)
//		//	{
//		//		//std::cout << "Working ...\n";
//
//		//		if (_game->winePress.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
//		//		{
//		//			SetWorkerStatus(PICKUP_RESSOURCES);
//		//		}
//
//
//		//	}
//		//	else
//		//	{
//
//		//	}
//
//		//}
//		//else if (actualBuilding == BUILDING_WINE_STOREHOUSE)
//		//{
//		//	// We send at the wine storehouse building the confirmation that a worker is there
//		//	if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition) == true)
//		//	{
//		//		//std::cout << "Working ...\n";
//
//		//		if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
//		//		{
//		//			SetWorkerStatus(PICKUP_RESSOURCES);
//		//		}
//		//	}
//		//	else
//		//	{
//
//		//	}
//
//		//}
//		//else if (actualBuilding == BUILDING_STOREHOUSE)
//		//{
//		//	// We send at the storehouse building the confirmation that a worker is there
//		//	if (_game->storehouse.ConfirmStorehousePresenceAtWorkerPosition(mapPosition) == true)
//		//	{
//		//		//std::cout << "Working ...\n";
//		//	}
//		//	else
//		//	{
//
//		//	}
//
//		//}
//		//else if (actualBuilding == BUILDING_STALL)
//		//{
//		//	// We send at the wine storehouse building the confirmation that a worker is there
//		//	if (_game->stall->ConfirmPresenceAtWorkerPosition(mapPosition) == true)
//		//	{
//		//		//std::cout << "Working ...\n";
//		//	}
//		//	else
//		//	{
//
//		//	}
//
//		//}
//		//else
//		//{
//		//	std::cout << "ERROR, the worker don't find a building where works\n";
//		//}
//
//
//
//		break;
//
//	case PICKUP_RESSOURCES:
//
//		//if (actualBuilding == BUILDING_VINES)
//		//{
//		//	// That mean the worker has already picked up a ressource
//		//	if (ressourceHeld != nullptr)
//		//	{
//		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
//		//		if (*(ressourceHeld) == BUNCH_OF_GRAPE)
//		//		{
//		//			int ressourceProduced = _game->vines.VinesSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());
//
//		//			*(quantityRessourceHeld) += ressourceProduced;
//		//			*(targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;
//
//		//			isItWorkingPlace = false;
//
//		//			sf::Vector2i targetedPosition = _game->stompingVats.SpecificsBuildingsFindNearestBuilding(mapPosition);
//
//		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
//		//			{
//		//				SetEndingPosition(targetedPosition, _game->map);
//		//				ActiveLauchingMovement();
//		//			}
//
//		//			SetWorkerStatus(IDLE);
//
//		//			std::cout << "Worker, picked up ressources\n";
//		//		}
//		//		else
//		//		{
//		//			std::cout << "The current worker already has a ressource in hands\n\n";
//		//			SetWorkerStatus(IDLE);
//		//		}
//		//	}
//		//	else
//		//	{
//		//		int ressourceProduced = _game->vines.VinesSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());
//
//		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
//		//		if (ressourceProduced != 0 && ressourceProduced > 0)
//		//		{
//		//			ressourceHeld = new enum TypesOfRessources;
//		//			quantityRessourceHeld = new int;
//		//			targetedBuilding = new enum TypeOfBuilding;
//
//		//			*(ressourceHeld) = BUNCH_OF_GRAPE;
//		//			*(quantityRessourceHeld) = ressourceProduced;
//		//			*(targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;
//
//		//			isItWorkingPlace = false;
//
//
//		//			sf::Vector2i targetedPosition = _game->stompingVats.SpecificsBuildingsFindNearestBuilding(mapPosition);
//
//		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
//		//			{
//		//				SetEndingPosition(targetedPosition, _game->map);
//		//				ActiveLauchingMovement();
//		//			}
//
//		//			SetWorkerStatus(IDLE);
//
//		//			std::cout << "Worker, picked up ressources\n";
//		//		}
//		//		// We check if the building is still in the produced state
//		//		else if (_game->vines.CheckVineHasProducedRessource(mapPosition) == true)
//		//		{
//		//			SetWorkerStatus(PICKUP_RESSOURCES);
//		//		}
//		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
//		//		else if (_game->vines.CheckVineHasProducedRessource(mapPosition) == false)
//		//		{
//		//			// For security, we reaffect the worker to the WORKING status
//		//			SetWorkerStatus(WORKING);
//
//		//			std::cout << "Worker, error during pick up of ressources\n";
//		//		}
//		//	}
//
//
//		//}
//		//else if (actualBuilding == BUILDING_GRAPE_STOMPING_VATS)
//		//{
//		//	// That mean the worker has already picked up a ressource
//		//	if (ressourceHeld != nullptr)
//		//	{
//		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
//		//		if (*(ressourceHeld) == GRAPES_MUST)
//		//		{
//		//			int ressourceProduced = _game->stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());
//
//		//			*(quantityRessourceHeld) += ressourceProduced;
//		//			*(targetedBuilding) = BUILDING_WINE_PRESS;
//
//		//			isItWorkingPlace = false;
//
//		//			sf::Vector2i targetedPosition = _game->winePress.SpecificsBuildingsFindNearestBuilding(mapPosition);
//
//		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
//		//			{
//		//				SetEndingPosition(targetedPosition, _game->map);
//		//				ActiveLauchingMovement();
//		//			}
//
//		//			SetWorkerStatus(IDLE);
//
//		//			std::cout << "Worker, picked up ressources\n";
//		//		}
//		//		else
//		//		{
//		//			std::cout << "The current worker already has a ressource in hands\n\n";
//		//			SetWorkerStatus(IDLE);
//		//		}
//		//	}
//		//	else
//		//	{
//		//		int ressourceProduced = _game->stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());
//
//		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
//		//		if (ressourceProduced != 0 && ressourceProduced > 0)
//		//		{
//		//			ressourceHeld = new enum TypesOfRessources;
//		//			quantityRessourceHeld = new int;
//		//			targetedBuilding = new enum TypeOfBuilding;
//
//		//			*(ressourceHeld) = GRAPES_MUST;
//		//			*(quantityRessourceHeld) = ressourceProduced;
//		//			*(targetedBuilding) = BUILDING_WINE_PRESS;
//
//		//			isItWorkingPlace = false;
//
//		//			sf::Vector2i targetedPosition = _game->winePress.SpecificsBuildingsFindNearestBuilding(mapPosition);
//
//		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
//		//			{
//		//				SetEndingPosition(targetedPosition, _game->map);
//		//				ActiveLauchingMovement();
//		//			}
//
//		//			SetWorkerStatus(IDLE);
//
//		//			std::cout << "Worker, picked up ressources\n";
//		//		}
//		//		// We check if the building is still in the produced state
//		//		else if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
//		//		{
//		//			SetWorkerStatus(PICKUP_RESSOURCES);
//		//		}
//		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
//		//		else if (_game->stompingVats.CheckSpecificBuildingHasProducedRessource(mapPosition) == false)
//		//		{
//		//			// For security, we reaffect the worker to the WORKING status
//		//			SetWorkerStatus(WORKING);
//
//		//			std::cout << "Worker, error during pick up of ressources\n";
//		//		}
//		//	}
//
//		//}
//		//else if (actualBuilding == BUILDING_WINE_PRESS)
//		//{
//		//	// That mean the worker has already picked up a ressource
//		//	if (ressourceHeld != nullptr)
//		//	{
//		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
//		//		if (*(ressourceHeld) == GRAPE_JUICE)
//		//		{
//		//			int ressourceProduced = _game->winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());
//
//		//			*(quantityRessourceHeld) += ressourceProduced;
//		//			*(targetedBuilding) = BUILDING_WINE_STOREHOUSE;
//
//		//			isItWorkingPlace = false;
//
//		//			sf::Vector2i targetedPosition = _game->wineStorehouse.SpecificsBuildingsFindNearestBuilding(mapPosition);
//
//		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
//		//			{
//		//				SetEndingPosition(targetedPosition, _game->map);
//		//				ActiveLauchingMovement();
//		//			}
//
//		//			SetWorkerStatus(IDLE);
//
//		//			std::cout << "Worker, picked up ressources\n";
//
//
//		//		}
//		//		else
//		//		{
//		//			std::cout << "The current worker already has a ressource in hands\n\n";
//		//			SetWorkerStatus(IDLE);
//		//		}
//		//	}
//		//	else
//		//	{
//		//		int ressourceProduced = _game->winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());
//
//		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
//		//		if (ressourceProduced != 0 && ressourceProduced > 0)
//		//		{
//		//			ressourceHeld = new enum TypesOfRessources;
//		//			quantityRessourceHeld = new int;
//		//			targetedBuilding = new enum TypeOfBuilding;
//
//		//			*(ressourceHeld) = GRAPE_JUICE;
//		//			*(quantityRessourceHeld) = ressourceProduced;
//		//			*(targetedBuilding) = BUILDING_WINE_STOREHOUSE;
//
//		//			isItWorkingPlace = false;
//
//		//			sf::Vector2i targetedPosition = _game->wineStorehouse.SpecificsBuildingsFindNearestBuilding(mapPosition);
//
//		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
//		//			{
//		//				SetEndingPosition(targetedPosition, _game->map);
//		//				ActiveLauchingMovement();
//		//			}
//
//		//			SetWorkerStatus(IDLE);
//
//		//			std::cout << "Worker, picked up ressources\n";
//		//		}
//		//		// We check if the building is still in the produced state
//		//		else if (_game->winePress.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
//		//		{
//		//			SetWorkerStatus(PICKUP_RESSOURCES);
//		//		}
//		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
//		//		else if (_game->winePress.CheckSpecificBuildingHasProducedRessource(mapPosition) == false)
//		//		{
//		//			// For security, we reaffect the worker to the WORKING status
//		//			SetWorkerStatus(WORKING);
//
//		//			std::cout << "Worker, error during pick up of ressources\n";
//		//		}
//		//	}
//
//		//}
//		//else if (actualBuilding == BUILDING_WINE_STOREHOUSE)
//		//{
//		//	// That mean the worker has already picked up a ressource
//		//	if (ressourceHeld != nullptr)
//		//	{
//		//		// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
//		//		if (*(ressourceHeld) == AMPHORA_OF_WINE)
//		//		{
//		//			int ressourceProduced = _game->wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());
//
//		//			*(quantityRessourceHeld) += ressourceProduced;
//		//			*(targetedBuilding) = BUILDING_STOREHOUSE;
//
//		//			isItWorkingPlace = false;
//
//		//			sf::Vector2i targetedPosition = _game->storehouse.StorehouseFindNearestBuilding(mapPosition);
//
//		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
//		//			{
//		//				SetEndingPosition(targetedPosition, _game->map);
//		//				ActiveLauchingMovement();
//		//			}
//
//		//			SetWorkerStatus(IDLE);
//
//		//			std::cout << "Worker, picked up ressources\n";
//
//
//		//		}
//		//		else
//		//		{
//		//			std::cout << "The current worker already has a ressource in hands\n\n";
//		//			SetWorkerStatus(IDLE);
//		//		}
//		//	}
//		//	else
//		//	{
//		//		int ressourceProduced = _game->wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(mapPosition, _game->time->GetFrameTime());
//
//		//		// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
//		//		if (ressourceProduced != 0 && ressourceProduced > 0)
//		//		{
//		//			ressourceHeld = new enum TypesOfRessources;
//		//			quantityRessourceHeld = new int;
//		//			targetedBuilding = new enum TypeOfBuilding;
//
//		//			*(ressourceHeld) = AMPHORA_OF_WINE;
//		//			*(quantityRessourceHeld) = ressourceProduced;
//		//			*(targetedBuilding) = BUILDING_STOREHOUSE;
//
//		//			isItWorkingPlace = false;
//
//		//			sf::Vector2i targetedPosition = _game->storehouse.StorehouseFindNearestBuilding(mapPosition);
//
//		//			if (targetedPosition != sf::Vector2i(RESET, RESET))
//		//			{
//		//				SetEndingPosition(targetedPosition, _game->map);
//		//				ActiveLauchingMovement();
//		//			}
//
//		//			SetWorkerStatus(IDLE);
//
//		//			std::cout << "Worker, picked up ressources\n";
//		//		}
//		//		// We check if the building is still in the produced state
//		//		else if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(mapPosition) == true)
//		//		{
//		//			SetWorkerStatus(PICKUP_RESSOURCES);
//		//		}
//		//		// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
//		//		else if (_game->wineStorehouse.CheckSpecificBuildingHasProducedRessource(mapPosition) == false)
//		//		{
//		//			// For security, we reaffect the worker to the WORKING status
//		//			SetWorkerStatus(WORKING);
//
//		//			std::cout << "Worker, error during pick up of ressources\n";
//		//		}
//		//	}
//		//}
//		//else
//		//{
//		//	std::cout << "ERROR, the worker don't find a building where works\n";
//		//}
//
//		break;
//
//	case DEPOSIT_RESSOURCES:
//
//		//if (ressourceHeld != nullptr)
//		//{
//		//	switch (*(ressourceHeld))
//		//	{
//		//	case GRAPE_VINE:
//		//		break;
//		//	case BUNCH_OF_GRAPE:
//
//		//		if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition))
//		//		{
//		//			AddTimeToDeposit(_game->time->GetFrameTime());
//
//		//			if (GetTimeToDeposit() >= _game->buildings[BUILDING_GRAPE_STOMPING_VATS].GetDepositingTimeCost())
//		//			{
//		//				std::cout << "Worker, want to deposit ressources\n";
//
//		//				_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));
//
//		//				SetTimeToDeposit(RESET);
//
//		//				// Reset of the main data
//		//				delete ressourceHeld;
//		//				delete quantityRessourceHeld;
//		//				delete targetedBuilding;
//
//		//				ressourceHeld = nullptr;
//		//				quantityRessourceHeld = nullptr;
//		//				targetedBuilding = nullptr;
//
//		//				SetWorkerStatus(WORKING);
//		//			}
//		//		}
//		//		else
//		//		{
//		//			SetWorkerStatus(IDLE);
//		//		}
//
//		//		break;
//		//	case GRAPES_MUST:
//
//		//		if (_game->winePress.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition))
//		//		{
//		//			AddTimeToDeposit(_game->time->GetFrameTime());
//
//		//			if (GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_PRESS].GetDepositingTimeCost())
//		//			{
//		//				std::cout << "Worker, want to deposit ressources\n";
//
//		//				_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));
//
//		//				SetTimeToDeposit(RESET);
//
//		//				// Reset of the main data
//		//				delete ressourceHeld;
//		//				delete quantityRessourceHeld;
//		//				delete targetedBuilding;
//
//		//				ressourceHeld = nullptr;
//		//				quantityRessourceHeld = nullptr;
//		//				targetedBuilding = nullptr;
//
//		//				SetWorkerStatus(WORKING);
//		//			}
//		//		}
//		//		else
//		//		{
//		//			SetWorkerStatus(IDLE);
//		//		}
//
//		//		break;
//		//	case GRAPE_JUICE:
//
//		//		if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtWorkerPosition(mapPosition))
//		//		{
//		//			AddTimeToDeposit(_game->time->GetFrameTime());
//
//		//			if (GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
//		//			{
//		//				std::cout << "Worker, want to deposit ressources\n";
//
//		//				_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));
//
//		//				SetTimeToDeposit(RESET);
//
//		//				// Reset of the main data
//		//				delete ressourceHeld;
//		//				delete quantityRessourceHeld;
//		//				delete targetedBuilding;
//
//		//				ressourceHeld = nullptr;
//		//				quantityRessourceHeld = nullptr;
//		//				targetedBuilding = nullptr;
//
//		//				SetWorkerStatus(WORKING);
//		//			}
//		//		}
//		//		else
//		//		{
//		//			SetWorkerStatus(IDLE);
//		//		}
//
//		//		break;
//		//	case PURE_GRAPE_JUICE:
//
//		//		break;
//		//	case GRAPE_MARC:
//
//		//		break;
//		//	case DOLIUM:
//
//		//		break;
//		//	case AMPHORAS:
//
//		//		AddTimeToDeposit(_game->time->GetFrameTime());
//
//		//		if (GetTimeToDeposit() >= _game->buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
//		//		{
//		//			std::cout << "Worker, want to deposit ressources\n";
//
//		//			_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));
//
//		//			this->SetTimeToDeposit(RESET);
//
//		//			// Reset of the main data
//		//			delete ressourceHeld;
//		//			delete quantityRessourceHeld;
//		//			delete targetedBuilding;
//
//		//			ressourceHeld = nullptr;
//		//			quantityRessourceHeld = nullptr;
//		//			targetedBuilding = nullptr;
//
//		//			SetWorkerStatus(WORKING);
//		//		}
//
//		//		break;
//		//	case AMPHORA_OF_WINE:
//
//		//		if (_game->storehouse.ConfirmStorehousePresenceAtWorkerPosition(mapPosition))
//		//		{
//		//			AddTimeToDeposit(_game->time->GetFrameTime());
//
//		//			if (GetTimeToDeposit() >= _game->buildings[BUILDING_STOREHOUSE].GetDepositingTimeCost())
//		//			{
//		//				std::cout << "Worker, deposit amphora of wine ressources\n";
//
//		//				_game->ressources[*(ressourceHeld)].AddQuantityOwned(*(quantityRessourceHeld));
//
//		//				SetTimeToDeposit(RESET);
//
//		//				// Reset of the main data
//		//				delete ressourceHeld;
//		//				delete quantityRessourceHeld;
//		//				delete targetedBuilding;
//
//		//				ressourceHeld = nullptr;
//		//				quantityRessourceHeld = nullptr;
//		//				targetedBuilding = nullptr;
//
//		//				SetWorkerStatus(WORKING);
//		//			}
//		//		}
//		//		else
//		//		{
//		//			SetWorkerStatus(IDLE);
//		//		}
//
//		//		break;
//		//	case TOOLS:
//		//		break;
//		//	case WICKER_BASKET:
//		//		break;
//		//	default:
//		//		break;
//		//	}
//
//		//}
//		//else
//		//{
//		//	// For security, we reaffect the worker to the WORKING status
//		//	SetWorkerStatus(WORKING);
//
//		//	std::cout << "Worker, error during deposit of the ressources\n";
//		//}
//
//		break;
//
//	default:
//		break;
//	}
//}

void MainCharacter::UpdatePathAndActivities(Game* _game)
{
	float speed(RESET);

	switch (m_actualStatus)
	{
	case IDLE:

		if (m_isLauchingMovement == true)
		{
			//std::cout << "Changement de status vers Waiting Movement\n";

			SetMainCharacterStatus(WAITING_MOVEMENT);
			m_isLauchingMovement = false;
		}
		else
		{
			m_waitingTimer += TimeManagement::GetSingleton()->GetFrameTime();

			// If the main character is waiting, but have buildings planned to be built, we check and launch his movement
			if (m_isCurrentlyBuilding)
			{
				// Test if the list is empty or not
				if (_game->m_buildingsListPlanned->IsBuildingListIsEmpty())
				{
					m_isCurrentlyBuilding = false;
					m_waitingTimer = RESET;
				}
				else
				{
					if (m_waitingTimer > 3)
					{
						// Set the position of the next building plannified
						SetMainCharacterEndingPosition(_game->m_buildingsListPlanned->GetBuildingPositionInMap(), _game->m_map);

						SetMainCharacterStatus(IDLE, true);

						m_waitingTimer = RESET;
					}
				}
			}
		}


		break;

	case WAITING_MOVEMENT:

		//std::cout << "WAITING MOVEMENT\n";
		m_path->MainStatePathfinding();

		// In the case that we find a way, we start to move
		if (m_path->GetActualStatus() == PATHFINDING_FIND_WAY_FIRST
			|| m_path->GetActualStatus() == PATHFINDING_FIND_WAY)
		{
			SetMainCharacterStatus(MOVEMENT);
		}
		// In the case that we don't find any way to join the target, we stay where we're
		else if (m_path->GetActualStatus() == PATHFINDING_FIND_NO_WAY)
		{
			SetMainCharacterStatus(IDLE);

			if (m_path != nullptr)
			{
				delete m_path;
				m_path = nullptr;
			}
		}

		break;

	case MOVEMENT:

		// Speed modification depending on the type of soil
		if (_game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == PATH)
		{
			speed = _game->m_time->GetFrameTime() * 2.25f;
		}
		else if (_game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == STONE_PATH)
		{
			speed = _game->m_time->GetFrameTime() * 3.5f;
		}
		else if (_game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == ROAD)
		{
			speed = _game->m_time->GetFrameTime() * 5;
		}
		else
		{
			speed = _game->m_time->GetFrameTime() * 1.5f;
		}

		//std::cout << "MOVEMENT\n";
		m_path->WalkProcess(&m_mapPosition, speed);



		// If the path ask to be deleted, that mean that the character has reached his destination
		if (m_path->GetActualStatus() == PATHFINDING_NEED_TO_BE_DELETED)
		{
			// If at the initialisation we find that location is a working place, we change the status of the worker to WORKING
			if (m_isItWorkingPlace == true)
			{
				if (m_isCurrentlyBuilding)
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
			if (m_path != nullptr)
			{
				delete m_path;
				m_path = nullptr;
			}
		}

		break;

	case BUILDING:
		if (m_isCurrentlyBuilding == true
			&& _game->m_buildingsListPlanned->GetBuildingPositionInMap() == (sf::Vector2i)m_mapPosition)
		{
			m_buildingTimer += _game->m_time->GetFrameTime();

			if (m_buildingTimer >= 2)
			{
				// Security
				if (_game->m_buildingsListPlanned->GetBuildingID() != -1
					&& _game->m_buildingsListPlanned->GetBuildingPositionInMap() != sf::Vector2i(-1, -1)
					&& _game->m_buildingsListPlanned->GetBuildingSize() != sf::Vector2i(-1, -1))
				{
					// Clear the ground by putting sand floor
					for (int y = 0; y < _game->m_buildingsListPlanned->GetBuildingSize().y; y++)
					{
						for (int x = 0; x < _game->m_buildingsListPlanned->GetBuildingSize().x; x++)
						{
							_game->m_map->GetMap()[ZERO_FLOOR + SPRITE_ID]
								[_game->m_buildingsListPlanned->GetBuildingPositionInMap().y - y]
							[_game->m_buildingsListPlanned->GetBuildingPositionInMap().x - x] = 1;
						}
					}

					// Modifier couche pour ID
					if (_game->m_buildingsListPlanned->GetBuildingID() == TypeOfBuilding::BUILDING_PATH
						|| _game->m_buildingsListPlanned->GetBuildingID() == TypeOfBuilding::BUILDING_PAVED_PATH
						|| _game->m_buildingsListPlanned->GetBuildingID() == TypeOfBuilding::BUILDING_ROAD)
					{
						TypesOfCollisions collisionToAskToBeBuilt = TypesOfCollisions::COLLISION;

						switch ((TypeOfBuilding)_game->m_buildingsListPlanned->GetBuildingID())
						{
						case BUILDING_PATH:
							collisionToAskToBeBuilt = TypesOfCollisions::PATH;
							break;
						case BUILDING_PAVED_PATH:
							collisionToAskToBeBuilt = TypesOfCollisions::STONE_PATH;
							break;
						case BUILDING_ROAD:
							collisionToAskToBeBuilt = TypesOfCollisions::ROAD;
							break;
						}

						_game->m_buildWindow.SetBuildingOnMap(_game->m_map, &_game->m_builds, _game->m_buildingsListPlanned->GetBuildingID(), collisionToAskToBeBuilt, _game->m_buildingsListPlanned->GetBuildingPositionInMap());
					}
					else
					{
						_game->m_buildWindow.SetBuildingOnMap(_game->m_map, &_game->m_builds, _game->m_buildingsListPlanned->GetBuildingID(), COLLISION, _game->m_buildingsListPlanned->GetBuildingPositionInMap());
					}



					Storage* storage = nullptr;

					// We add informations to the concerned linkedlist
					switch (_game->m_buildingsListPlanned->GetBuildingID())
					{
					case BUILDING_VINES:
						_game->m_builds.m_vines.AddNewVineToList((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());
						break;
					case BUILDING_GRAPE_STOMPING_VATS:
						_game->m_builds.m_stompingVats.AddNewBuildingToList((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());

						storage = _game->m_builds.m_stompingVats.GetStorage((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());

						storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(BUNCH_OF_GRAPE), ResourceData::RESOURCE_NEEDED);
						storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(GRAPES_MUST), ResourceData::RESOURCE_PRODUCED);
						break;
					case BUILDING_WINE_PRESS:
						_game->m_builds.m_winePress.AddNewBuildingToList((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());

						storage = _game->m_builds.m_winePress.GetStorage((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());

						storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(GRAPES_MUST), ResourceData::RESOURCE_NEEDED);
						storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(GRAPE_JUICE), ResourceData::RESOURCE_PRODUCED);
						break;
					case BUILDING_WINE_STOREHOUSE:
						_game->m_builds.m_wineStorehouse.AddNewBuildingToList((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());

						storage = _game->m_builds.m_wineStorehouse.GetStorage((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());

						storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(GRAPE_JUICE), ResourceData::RESOURCE_NEEDED);
						storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(AMPHORAS), ResourceData::RESOURCE_NEEDED);
						storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(AMPHORA_OF_WINE), ResourceData::RESOURCE_PRODUCED);
						break;
					case BUILDING_STOREHOUSE:
						_game->m_builds.m_storehouse.AddNewBuildingToList((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());

						storage = _game->m_builds.m_storehouse.GetStorage((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());

						storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(AMPHORA_OF_WINE), ResourceData::RESOURCE_NEEDED_N_PRODUCED);
						if (_game->m_builds.m_stall != nullptr) _game->m_builds.m_stall->AddStorehousePosition((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());
						break;
					case BUILDING_STALL:
						if (_game->m_builds.m_stall != nullptr)
						{
							_game->m_builds.m_stall->AddNewBuilding((sf::Vector2f)_game->m_buildingsListPlanned->GetBuildingPositionInMap());

							storage = _game->m_builds.m_stall->GetStorage();
							storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(AMPHORA_OF_WINE), ResourceData::RESOURCE_NEEDED_N_PRODUCED);
						}
						break;
						/*case BUILDING_VILLA:
							break;
						case BUILDING_DORMITORY:
							break;*/
					default:
						break;
					}


					// Deletion of the current first building in the list
					_game->m_buildingsListPlanned->DeleteCurrentFirstBuildingInList();

					//Update the workers's paths
					_game->m_workersList->CheckAndUpdateWorkersPath();


					// Test if the list is empty or not
					if (_game->m_buildingsListPlanned->IsBuildingListIsEmpty())
					{
						m_isCurrentlyBuilding = false;
						SetMainCharacterStatus(IDLE);
					}
					else
					{
						// Set the position of the next building plannified
						SetMainCharacterEndingPosition(_game->m_buildingsListPlanned->GetBuildingPositionInMap(), _game->m_map);

						SetMainCharacterStatus(IDLE, true);
					}

					// Reset the builder timer of the main character
					m_buildingTimer -= 2;
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

		if (m_actualBuilding == BUILDING_VILLA)
		{
			if (_game->m_actualGameState != VILLA_MANAGEMENT)
			{
				_game->m_time->SetTypeOfAcceleration(TypeOfTimeAcceleration::GAME_PAUSE);
				_game->m_actualGameState = VILLA_MANAGEMENT;
				_game->m_villaManagement.SetInternalStateMachine(VillaManagementStateMachine::NORMAL_STATE);
			}

			SetMainCharacterStatus(IDLE);
			std::cout << "End\n";
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