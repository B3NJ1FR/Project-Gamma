#include "Workers.h"
#include "BuildingManagement.h"


Workers::Workers()
{
	m_actualStatus = IDLE;
	m_isLauchingMovement = false;
	m_isItWorkingPlace = false;
	m_isWorkerWasWorkingInBuilding = false;

	m_isPressingEnd = false;
	m_isPressingStart = false;


	m_ressourceHeld = nullptr;
	m_quantityRessourceHeld = nullptr;
	m_targetedBuilding = nullptr;

	SetTimeToDeposit(RESET);

	std::cout << "Ouvrier initialisé\n\n";
}



Workers::~Workers()
{
	if (m_ressourceHeld != nullptr)
	{
		delete[] m_ressourceHeld;
	}

	if (m_targetedBuilding != nullptr)
	{
		delete[] m_targetedBuilding;
	}
}






void Workers::SetEndingPosition(const sf::Vector2i& _mapPosition, unsigned short*** _map)
{
	if (_mapPosition.x >= 0 && _mapPosition.y >= 0)
	{
		m_mapEndPosition = (sf::Vector2f)_mapPosition;
	}

	//
	// CRASH SORTIE DE TABLEAU A CAUSE DE X > 100000
	//

	// If this place is a building
	if (_map[FIRST_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		m_isItWorkingPlace = true;
		m_currentBuilding = (enum TypeOfBuilding)_map[FIRST_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x];

		//std::cout << "This is a working place : " << _map[FIRST_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x] << std::endl;
	}
	else if (_map[ZERO_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		m_isItWorkingPlace = true;
		m_currentBuilding = (enum TypeOfBuilding)_map[ZERO_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x];

		//std::cout << "This is a working place : " << _map[FIRST_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x] << std::endl;
	}
	else
	{
		m_isItWorkingPlace = false;
	}
}



void Workers::DisplayWorker()
{

}

void Workers::ActiveLauchingMovement() // TEMPORAIRE
{
	std::cout << "Chemin lance\n\n";
	m_isLauchingMovement = true;
}


void Workers::InitPathfinding(Map *_map)
{
	if (m_isLauchingMovement == true)
	{
		if (m_path != nullptr)
		{
			delete m_path;
			m_path = nullptr;
		}

		m_path = new Pathfinding;

		m_path->InitMapCopyPathfinding(sf::Vector2i(_map->GetNumberOfColumns(), _map->GetNumberOfLines()), _map->GetMap(), (FIRST_FLOOR + COLLISIONS_ID)); // TEMPORAIRE
		
		m_path->SetPathStartingPosition((sf::Vector2i)m_mapPosition); // TEMPORAIRE
		m_path->SetPathEndingPosition((sf::Vector2i)m_mapEndPosition); // TEMPORAIRE
	}
}

void Workers::UpdatePathAndActivities(Map* _map, TimeManagement* _time, BuildingManagement* _builds, Ressources* _ressources)
{
	float speed(RESET);

	switch (m_actualStatus)
	{
	case IDLE:

		//std::cout << "Idle\n";

		if (m_isWorkerWasWorkingInBuilding)
		{
			// We send at the vines building the confirmation that a worker is there
			if (_builds->m_vines.ConfirmVinePresenceAtPosition(m_mapPosition))
			{
				_builds->m_vines.WorkerLeavingThisPosition(m_mapPosition);
			}
			// We send at the stomping vats building the confirmation that a worker is there
			else if (_builds->m_stompingVats.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition))
			{
				_builds->m_stompingVats.WorkerLeavingThisPosition(m_mapPosition);
			}
			// We send at the wine press building the confirmation that a worker is there
			else if (_builds->m_winePress.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition))
			{
				_builds->m_winePress.WorkerLeavingThisPosition(m_mapPosition);
			}
			// We send at the wine storehouse building the confirmation that a worker is there
			else if (_builds->m_wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition))
			{
				_builds->m_wineStorehouse.WorkerLeavingThisPosition(m_mapPosition);
			}
			// We send at the storehouse building the confirmation that a worker is there
			else if (_builds->m_storehouse.ConfirmStorehousePresenceAtPosition(m_mapPosition))
			{
				_builds->m_storehouse.WorkerLeavingThisPosition(m_mapPosition);
			}
			// We send at the wine storehouse building the confirmation that a worker is there
			else if (_builds->m_stall->ConfirmPresenceAtPosition(m_mapPosition))
			{
				_builds->m_stall->WorkerLeavingThisPosition(m_mapPosition);
			}

			m_isWorkerWasWorkingInBuilding = false;
		}

		if (m_isLauchingMovement == true)
		{
			std::cout << "Changement de status vers Waiting Movement\n";

			SetWorkerStatus(WAITING_MOVEMENT);
			m_isLauchingMovement = false;
		}

		break;
		
	case WAITING_MOVEMENT:

		//std::cout << "WAITING MOVEMENT\n";
		m_path->MainStatePathfinding();

		// In the case that we find a way, we start to move
		if (m_path->GetActualStatus() == PATHFINDING_FIND_WAY_FIRST
			|| m_path->GetActualStatus() == PATHFINDING_FIND_WAY)
		{
			SetWorkerStatus(MOVEMENT);
		}
		// In the case that we don't find any way to join the target, we stay where we're
		else if (m_path->GetActualStatus() == PATHFINDING_FIND_NO_WAY)
		{
			SetWorkerStatus(IDLE);
			delete m_path;
			m_path = nullptr;

			// TO OPTIMIZE
			std::cout << "No Path find ...\n\n";
		}

		break;

	case MOVEMENT:
		
		// Speed modification depending on the type of soil
		if (_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == PATH)
		{
			speed = _time->GetFrameTime() * 2.25f;
		}
		else if (_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == STONE_PATH)
		{
			speed = _time->GetFrameTime() * 3.5f;
		}
		else if(_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == ROAD)
		{
			speed = _time->GetFrameTime() * 5;
		}
		else
		{
			speed = _time->GetFrameTime() * 1.5f;
		}

		//std::cout << "MOVEMENT\n";
		m_path->WalkProcess(&m_mapPosition, speed);



		// If the path ask to be deleted, that mean that the worker has reached his destination
		if (m_path->GetActualStatus() == PATHFINDING_NEED_TO_BE_DELETED)
		{
			// If at the initialisation we find that location is a working place, we change the status of the worker to WORKING
			if (m_isItWorkingPlace == true)
			{
				// If that place is what which were targeted and the worker carry some ressource, he need to deposit them
				if (m_targetedBuilding != nullptr)
				{
					if (*(m_targetedBuilding) == m_currentBuilding
						&& m_ressourceHeld != nullptr)
					{
						SetWorkerStatus(DEPOSIT_RESSOURCES);
					}
					else
					{
						switch (m_currentBuilding)
						{
						case BUILDING_VINES:
							SetWorkerStatus((_builds->m_vines.CheckVineHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_GRAPE_STOMPING_VATS:
							SetWorkerStatus((_builds->m_stompingVats.CheckSpecificsBuildingsHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_WINE_PRESS:
							SetWorkerStatus((_builds->m_winePress.CheckSpecificsBuildingsHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_WINE_STOREHOUSE:
							SetWorkerStatus((_builds->m_wineStorehouse.CheckSpecificsBuildingsHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_STOREHOUSE:
							SetWorkerStatus((_builds->m_storehouse.CheckStorehouseHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_STALL:
							SetWorkerStatus((_builds->m_stall->GetConstructionStatus() == PLANNED || _builds->m_stall->GetConstructionStatus() == CONSTRUCTION) ? BUILDING : WORKING);

							break;
							/*case BUILDING_VILLA:
								SetWorkerStatus((_builds->vines.CheckVineHasBeenBuilt(mapPosition) == false) ? BUILDING : WORKING);

								break;*/
							/*case BUILDING_DORMITORY:
								SetWorkerStatus((_builds->vines.CheckVineHasBeenBuilt(mapPosition) == false) ? BUILDING : WORKING);

								break;*/
						default:
							SetWorkerStatus(IDLE);
							break;
						}
					}
				}
				else
				{
					// Temporaire
					if (m_ressourceHeld != nullptr)
					{
						// A REVOIR CAR IL DEPOSE LES RESSOURCES AU SOL
						SetWorkerStatus(DEPOSIT_RESSOURCES);
					}
					else
					{
						switch (m_currentBuilding)
						{
						case BUILDING_VINES:
							SetWorkerStatus((_builds->m_vines.CheckVineHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_GRAPE_STOMPING_VATS:
							SetWorkerStatus((_builds->m_stompingVats.CheckSpecificsBuildingsHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_WINE_PRESS:
							SetWorkerStatus((_builds->m_winePress.CheckSpecificsBuildingsHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_WINE_STOREHOUSE:
							SetWorkerStatus((_builds->m_wineStorehouse.CheckSpecificsBuildingsHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_STOREHOUSE:
							SetWorkerStatus((_builds->m_storehouse.CheckStorehouseHasBeenBuilt(m_mapPosition) == false) ? BUILDING : WORKING);

							break;
						case BUILDING_STALL:
							SetWorkerStatus((_builds->m_stall->GetConstructionStatus() == PLANNED || _builds->m_stall->GetConstructionStatus() == CONSTRUCTION) ? BUILDING : WORKING);

							break;
						/*case BUILDING_VILLA:
							SetWorkerStatus((_builds->vines.CheckVineHasBeenBuilt(mapPosition) == false) ? BUILDING : WORKING);

							break;*/
						/*case BUILDING_DORMITORY:
							SetWorkerStatus((_builds->vines.CheckVineHasBeenBuilt(mapPosition) == false) ? BUILDING : WORKING);

							break;*/
						default:
							SetWorkerStatus(IDLE);
							break;
						}
					}
				}				
			}
			// Else, the worker just wait at the IDLE status
			else
			{
				if (m_ressourceHeld != nullptr)
				{
					// A REVOIR CAR IL DEPOSE LES RESSOURCES AU SOL
					SetWorkerStatus(DEPOSIT_RESSOURCES);
				}
				else
				{
					SetWorkerStatus(IDLE);
				}
			}

			// We delete the path and init his pointer to null
			delete m_path;
			m_path = nullptr;
		}

		break;
		
	case BUILDING:

		//std::cout << "Building ..\n";

		switch (m_currentBuilding)
		{
		case BUILDING_VINES:
			if (_builds->m_vines.CheckVineHasBeenBuilt(m_mapPosition) == false)
			{
				// If the result is false, that mean the building isn't there or has been destroyed
				if (_builds->m_vines.ConfirmVinePresenceAtPosition(m_mapPosition, true) == false)
				{
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				SetWorkerStatus(WORKING);
			}

			break;
		case BUILDING_GRAPE_STOMPING_VATS:
			if (_builds->m_stompingVats.CheckSpecificsBuildingsHasBeenBuilt(m_mapPosition) == false)
			{
				// If the result is false, that mean the building isn't there or has been destroyed
				if (_builds->m_stompingVats.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true) == false)
				{
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				SetWorkerStatus(WORKING);
			}

			break;
		case BUILDING_WINE_PRESS:
			if (_builds->m_winePress.CheckSpecificsBuildingsHasBeenBuilt(m_mapPosition) == false)
			{
				// If the result is false, that mean the building isn't there or has been destroyed
				if (_builds->m_winePress.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true) == false)
				{
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				SetWorkerStatus(WORKING);
			}

			break;
		case BUILDING_WINE_STOREHOUSE:
			if (_builds->m_wineStorehouse.CheckSpecificsBuildingsHasBeenBuilt(m_mapPosition) == false)
			{
				// If the result is false, that mean the building isn't there or has been destroyed
				if (_builds->m_wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true) == false)
				{
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				SetWorkerStatus(WORKING);
			}

			break;
		case BUILDING_STOREHOUSE:
			if (_builds->m_storehouse.CheckStorehouseHasBeenBuilt(m_mapPosition) == false)
			{
				// If the result is false, that mean the building isn't there or has been destroyed
				if (_builds->m_storehouse.ConfirmStorehousePresenceAtPosition(m_mapPosition, false, true) == false)
				{
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				SetWorkerStatus(WORKING);
			}

			break;
		case BUILDING_STALL:
			if (_builds->m_stall->GetConstructionStatus() == PLANNED || _builds->m_stall->GetConstructionStatus() == CONSTRUCTION)
			{
				// If the result is false, that mean the building isn't there or has been destroyed
				if (_builds->m_stall->ConfirmPresenceAtPosition(m_mapPosition, false, true) == false)
				{
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				SetWorkerStatus(WORKING);
			}

			break;
			/*case BUILDING_VILLA:
				SetWorkerStatus((_builds->vines.CheckVineHasBeenBuilt(mapPosition) == false) ? BUILDING : WORKING);

				break;*/
			/*case BUILDING_DORMITORY:
				SetWorkerStatus((_builds->vines.CheckVineHasBeenBuilt(mapPosition) == false) ? BUILDING : WORKING);

				break;*/
		default:
			SetWorkerStatus(IDLE);
			break;
		}

		break;

	case WORKING:
		
		if (m_currentBuilding == BUILDING_VINES)
		{
			// We send at the vines building the confirmation that a worker is there
			if (_builds->m_vines.ConfirmVinePresenceAtPosition(m_mapPosition, true) == true)
			{
				m_isWorkerWasWorkingInBuilding = true;
				//std::cout << "Working ...\n";

				if (_builds->m_vines.CheckVineHasProducedRessource(m_mapPosition) == true)
				{
					SetWorkerStatus(PICKUP_RESSOURCES);
				}
			}
			// If the result is false, that mean the building isn't there or has been destroyed
			else
			{
				SetWorkerStatus(IDLE);
			}

		}
		else if (m_currentBuilding == BUILDING_GRAPE_STOMPING_VATS)
		{
			// We send at the stomping vats building the confirmation that a worker is there
			if (_builds->m_stompingVats.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true) == true)
			{
				m_isWorkerWasWorkingInBuilding = true;
				//std::cout << "Working ...\n";

				if (_builds->m_stompingVats.CheckSpecificBuildingHasProducedRessource(m_mapPosition) == true)
				{
					SetWorkerStatus(PICKUP_RESSOURCES);
				}

			}
			else
			{
				SetWorkerStatus(IDLE);
			}

		}
		else if (m_currentBuilding == BUILDING_WINE_PRESS)
		{
			// We send at the wine press building the confirmation that a worker is there
			if (_builds->m_winePress.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true) == true)
			{
				m_isWorkerWasWorkingInBuilding = true;
				//std::cout << "Working ...\n";

				if (_builds->m_winePress.CheckSpecificBuildingHasProducedRessource(m_mapPosition) == true)
				{
					SetWorkerStatus(PICKUP_RESSOURCES);
				}


			}
			else
			{
				SetWorkerStatus(IDLE);
			}

		}
		else if (m_currentBuilding == BUILDING_WINE_STOREHOUSE)
		{
			// We send at the wine storehouse building the confirmation that a worker is there
			if (_builds->m_wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true) == true)
			{
				m_isWorkerWasWorkingInBuilding = true;
				//std::cout << "Working ...\n";
							   				 
				if (_builds->m_wineStorehouse.CheckSpecificBuildingHasProducedRessource(m_mapPosition) == true)
				{
					SetWorkerStatus(PICKUP_RESSOURCES);
				}
			}
			else
			{
				SetWorkerStatus(IDLE);
			}

		}
		else if (m_currentBuilding == BUILDING_STOREHOUSE)
		{
			// We send at the storehouse building the confirmation that a worker is there
			if (_builds->m_storehouse.ConfirmStorehousePresenceAtPosition(m_mapPosition, false, true) == true)
			{
				m_isWorkerWasWorkingInBuilding = true;
				//std::cout << "Working ...\n";
			}
			else
			{
				SetWorkerStatus(IDLE);
			}

		}
		else if (m_currentBuilding == BUILDING_STALL)
		{
			// We send at the wine storehouse building the confirmation that a worker is there
			if (_builds->m_stall->ConfirmPresenceAtPosition(m_mapPosition, false, true) == true)
			{
				m_isWorkerWasWorkingInBuilding = true;
				//std::cout << "Working ...\n";
			}
			else
			{
				SetWorkerStatus(IDLE);
			}

		}
		else
		{
			std::cout << "ERROR, the worker don't find a building where works\n";
		}



		break;
	
	case PICKUP_RESSOURCES:

		if (m_currentBuilding == BUILDING_VINES)
		{
			// That mean the worker has already picked up a ressource
			if (m_ressourceHeld != nullptr)
			{
				// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
				if (*(m_ressourceHeld) == BUNCH_OF_GRAPE)
				{
					int ressourceProduced = _builds->m_vines.VinesSendRessourceProducedToPresentWorker(m_mapPosition, _time->GetFrameTime());

					*(m_quantityRessourceHeld) += ressourceProduced;
					*(m_targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;
					
					m_isItWorkingPlace = false;
					
					sf::Vector2i targetedPosition = _builds->m_stompingVats.SpecificsBuildingsFindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition, _map->GetMap());
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				int ressourceProduced = _builds->m_vines.VinesSendRessourceProducedToPresentWorker(m_mapPosition, _time->GetFrameTime());

				// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
				if (ressourceProduced != 0 && ressourceProduced > 0)
				{
					m_ressourceHeld = new enum TypesOfRessources;
					m_quantityRessourceHeld = new int;
					m_targetedBuilding = new enum TypeOfBuilding;

					*(m_ressourceHeld) = BUNCH_OF_GRAPE;
					*(m_quantityRessourceHeld) = ressourceProduced;
					*(m_targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;

					m_isItWorkingPlace = false;

					
					sf::Vector2i targetedPosition = _builds->m_stompingVats.SpecificsBuildingsFindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition, _map->GetMap());
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				else if (_builds->m_vines.CheckVineHasProducedRessource(m_mapPosition) == true)
				{
					SetWorkerStatus(PICKUP_RESSOURCES);
				}
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_builds->m_vines.CheckVineHasProducedRessource(m_mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}
			

		}
		else if (m_currentBuilding == BUILDING_GRAPE_STOMPING_VATS)
		{
			// That mean the worker has already picked up a ressource
			if (m_ressourceHeld != nullptr)
			{
				// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
				if (*(m_ressourceHeld) == GRAPES_MUST)
				{
					int ressourceProduced = _builds->m_stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(m_mapPosition, _time->GetFrameTime());

					*(m_quantityRessourceHeld) += ressourceProduced;
					*(m_targetedBuilding) = BUILDING_WINE_PRESS;

					m_isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _builds->m_winePress.SpecificsBuildingsFindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition, _map->GetMap());
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				int ressourceProduced = _builds->m_stompingVats.SpecificsBuildingsSendRessourceProducedToPresentWorker(m_mapPosition, _time->GetFrameTime());

				// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
				if (ressourceProduced != 0 && ressourceProduced > 0)
				{
					m_ressourceHeld = new enum TypesOfRessources;
					m_quantityRessourceHeld = new int;
					m_targetedBuilding = new enum TypeOfBuilding;

					*(m_ressourceHeld) = GRAPES_MUST;
					*(m_quantityRessourceHeld) = ressourceProduced;
					*(m_targetedBuilding) = BUILDING_WINE_PRESS;

					m_isItWorkingPlace = false;
					
					sf::Vector2i targetedPosition = _builds->m_winePress.SpecificsBuildingsFindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition, _map->GetMap());
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				else if (_builds->m_stompingVats.CheckSpecificBuildingHasProducedRessource(m_mapPosition) == true)
				{
					SetWorkerStatus(PICKUP_RESSOURCES);
				}
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_builds->m_stompingVats.CheckSpecificBuildingHasProducedRessource(m_mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}

		}
		else if (m_currentBuilding == BUILDING_WINE_PRESS)
		{
			// That mean the worker has already picked up a ressource
			if (m_ressourceHeld != nullptr)
			{
				// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
				if (*(m_ressourceHeld) == GRAPE_JUICE)
				{
					int ressourceProduced = _builds->m_winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(m_mapPosition, _time->GetFrameTime());

					*(m_quantityRessourceHeld) += ressourceProduced;
					*(m_targetedBuilding) = BUILDING_WINE_STOREHOUSE;
					
					m_isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _builds->m_wineStorehouse.SpecificsBuildingsFindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition, _map->GetMap());
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";

					
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				int ressourceProduced = _builds->m_winePress.SpecificsBuildingsSendRessourceProducedToPresentWorker(m_mapPosition, _time->GetFrameTime());

				// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
				if (ressourceProduced != 0 && ressourceProduced > 0)
				{
					m_ressourceHeld = new enum TypesOfRessources;
					m_quantityRessourceHeld = new int;
					m_targetedBuilding = new enum TypeOfBuilding;

					*(m_ressourceHeld) = GRAPE_JUICE;
					*(m_quantityRessourceHeld) = ressourceProduced;
					*(m_targetedBuilding) = BUILDING_WINE_STOREHOUSE;

					m_isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _builds->m_wineStorehouse.SpecificsBuildingsFindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition, _map->GetMap());
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				else if (_builds->m_winePress.CheckSpecificBuildingHasProducedRessource(m_mapPosition) == true)
				{
					SetWorkerStatus(PICKUP_RESSOURCES);
				}
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_builds->m_winePress.CheckSpecificBuildingHasProducedRessource(m_mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}

		}
		else if (m_currentBuilding == BUILDING_WINE_STOREHOUSE)
		{
			// That mean the worker has already picked up a ressource
			if (m_ressourceHeld != nullptr)
			{
				// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
				if (*(m_ressourceHeld) == AMPHORA_OF_WINE)
				{
					int ressourceProduced = _builds->m_wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(m_mapPosition, _time->GetFrameTime());

					*(m_quantityRessourceHeld) += ressourceProduced;
					*(m_targetedBuilding) = BUILDING_STOREHOUSE;
					
					m_isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _builds->m_storehouse.StorehouseFindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition, _map->GetMap());
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";

					
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				int ressourceProduced = _builds->m_wineStorehouse.SpecificsBuildingsSendRessourceProducedToPresentWorker(m_mapPosition, _time->GetFrameTime());

				// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
				if (ressourceProduced != 0 && ressourceProduced > 0)
				{
					m_ressourceHeld = new enum TypesOfRessources;
					m_quantityRessourceHeld = new int;
					m_targetedBuilding = new enum TypeOfBuilding;

					*(m_ressourceHeld) = AMPHORA_OF_WINE;
					*(m_quantityRessourceHeld) = ressourceProduced;
					*(m_targetedBuilding) = BUILDING_STOREHOUSE;

					m_isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _builds->m_storehouse.StorehouseFindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition, _map->GetMap());
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				else if (_builds->m_wineStorehouse.CheckSpecificBuildingHasProducedRessource(m_mapPosition) == true)
				{
					SetWorkerStatus(PICKUP_RESSOURCES);
				}
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_builds->m_wineStorehouse.CheckSpecificBuildingHasProducedRessource(m_mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					SetWorkerStatus(WORKING);

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
		
		if (m_ressourceHeld != nullptr)
		{
			switch (*(m_ressourceHeld))
			{
			case GRAPE_VINE:
				break;
			case BUNCH_OF_GRAPE:

				if (_builds->m_stompingVats.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true))
				{
					AddTimeToDeposit(_time->GetFrameTime());

					if (GetTimeToDeposit() >= _builds->m_buildings[BUILDING_GRAPE_STOMPING_VATS].GetDepositingTimeCost())
					{
						std::cout << "Worker, want to deposit ressources\n";

						_ressources[*(m_ressourceHeld)].AddOrSubtractQuantityOwned(*(m_quantityRessourceHeld));

						SetTimeToDeposit(RESET);

						// Reset of the main data
						delete m_ressourceHeld;
						delete m_quantityRessourceHeld;
						delete m_targetedBuilding;

						m_ressourceHeld = nullptr;
						m_quantityRessourceHeld = nullptr;
						m_targetedBuilding = nullptr;

						SetWorkerStatus(WORKING);
					}
				}
				else
				{
					SetWorkerStatus(IDLE);
				}

				break;
			case GRAPES_MUST:

				if (_builds->m_winePress.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true))
				{
					AddTimeToDeposit(_time->GetFrameTime());

					if (GetTimeToDeposit() >= _builds->m_buildings[BUILDING_WINE_PRESS].GetDepositingTimeCost())
					{
						std::cout << "Worker, want to deposit ressources\n";

						_ressources[*(m_ressourceHeld)].AddOrSubtractQuantityOwned(*(m_quantityRessourceHeld));

						SetTimeToDeposit(RESET);

						// Reset of the main data
						delete m_ressourceHeld;
						delete m_quantityRessourceHeld;
						delete m_targetedBuilding;

						m_ressourceHeld = nullptr;
						m_quantityRessourceHeld = nullptr;
						m_targetedBuilding = nullptr;

						SetWorkerStatus(WORKING);
					}
				}
				else
				{
					SetWorkerStatus(IDLE);
				}

				break;
			case GRAPE_JUICE:

				if (_builds->m_wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true))
				{
					AddTimeToDeposit(_time->GetFrameTime());

					if (GetTimeToDeposit() >= _builds->m_buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
					{
						std::cout << "Worker, want to deposit ressources\n";

						_ressources[*(m_ressourceHeld)].AddOrSubtractQuantityOwned(*(m_quantityRessourceHeld));

						SetTimeToDeposit(RESET);

						// Reset of the main data
						delete m_ressourceHeld;
						delete m_quantityRessourceHeld;
						delete m_targetedBuilding;

						m_ressourceHeld = nullptr;
						m_quantityRessourceHeld = nullptr;
						m_targetedBuilding = nullptr;

						SetWorkerStatus(WORKING);
					}
				}
				else
				{
					SetWorkerStatus(IDLE);
				}

				break;
			case PURE_GRAPE_JUICE:

				break;
			case GRAPE_MARC:

				break;
			case DOLIUM:

				break;
			case AMPHORAS:

				AddTimeToDeposit(_time->GetFrameTime());

				if (GetTimeToDeposit() >= _builds->m_buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
				{
					std::cout << "Worker, want to deposit ressources\n";

					_ressources[*(m_ressourceHeld)].AddOrSubtractQuantityOwned(*(m_quantityRessourceHeld));

					SetTimeToDeposit(RESET);

					// Reset of the main data
					delete m_ressourceHeld;
					delete m_quantityRessourceHeld;
					delete m_targetedBuilding;

					m_ressourceHeld = nullptr;
					m_quantityRessourceHeld = nullptr;
					m_targetedBuilding = nullptr;

					SetWorkerStatus(WORKING);
				}

				break;
			case AMPHORA_OF_WINE:
				
				if (_builds->m_storehouse.ConfirmStorehousePresenceAtPosition(m_mapPosition, false, true))
				{
					AddTimeToDeposit(_time->GetFrameTime());

					if (GetTimeToDeposit() >= _builds->m_buildings[BUILDING_STOREHOUSE].GetDepositingTimeCost())
					{
						std::cout << "Worker, deposit amphora of wine ressources\n";

						_ressources[*(m_ressourceHeld)].AddOrSubtractQuantityOwned(*(m_quantityRessourceHeld));

						SetTimeToDeposit(RESET);

						// Reset of the main data
						delete m_ressourceHeld;
						delete m_quantityRessourceHeld;
						delete m_targetedBuilding;

						m_ressourceHeld = nullptr;
						m_quantityRessourceHeld = nullptr;
						m_targetedBuilding = nullptr;

						SetWorkerStatus(WORKING);
					}
				}
				else
				{
					SetWorkerStatus(IDLE);
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
			SetWorkerStatus(WORKING);

			std::cout << "Worker, error during deposit of the ressources\n";
		}

		break;

	default:
		break;
	}
}

