#include "Workers.h"
#include "BuildingManagement.h"
#include "Ressources.h"
#include "RessourcesManager.h"


Workers::Workers()
{
	m_actualStatus = IDLE;
	m_isLauchingMovement = false;
	m_isItWorkingPlace = false;
	m_isWorkerWasWorkingInBuilding = false;
	m_isEnteredIntoBuilding = false;

	m_isPressingEnd = false;
	m_isPressingStart = false;

	m_storage = new Storage();
	m_storage->SetName("Worker");

	std::cout << "Ressources : " << m_storage->GetName() << ' ' << m_storage->GetNumberOfResources() << std::endl;

	m_targetedBuilding = nullptr;

	SetTimeToDeposit(RESET);

	std::cout << "Ouvrier initialisé\n\n";
}



Workers::~Workers()
{
	if (m_targetedBuilding != nullptr)
	{
		delete[] m_targetedBuilding;
	}

	if (m_storage != nullptr)
	{
		delete m_storage;
	}
}






void Workers::SetEndingPosition(const sf::Vector2i& _mapPosition)
{
	Map* pMap = Map::GetSingleton();

	if (pMap->IsCoordinatesIsInMap(_mapPosition)) m_mapEndPosition = (sf::Vector2f)_mapPosition;

	// If this place is a building
	if (pMap->GetMap()[FIRST_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		m_isItWorkingPlace = true;
		m_currentBuilding = (enum TypeOfBuilding)pMap->GetMap()[FIRST_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x];

		//std::cout << "This is a working place : " << _map[FIRST_FLOOR + BUILDING_ID][(int)mapEndPosition.y][(int)mapEndPosition.x] << std::endl;
	}
	else if (pMap->GetMap()[ZERO_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x] >= 0)
	{
		// We change the worker's status to working
		m_isItWorkingPlace = true;
		m_currentBuilding = (enum TypeOfBuilding)pMap->GetMap()[ZERO_FLOOR + BUILDING_ID][(int)m_mapEndPosition.y][(int)m_mapEndPosition.x];

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


void Workers::InitPathfinding()
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

void Workers::UpdatePathAndActivities(Map* _map, BuildingManagement* _builds)
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

			m_isEnteredIntoBuilding = false;
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

		if (_map->IsCoordinatesIsInMap((sf::Vector2i)m_mapPosition))
		{		
			// Speed modification depending on the type of soil
			if (_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == PATH)
			{
				speed = TimeManagement::GetSingleton()->GetFrameTime() * 2.25f;
			}
			else if (_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == STONE_PATH)
			{
				speed = TimeManagement::GetSingleton()->GetFrameTime() * 3.5f;
			}
			else if (_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] == ROAD)
			{
				speed = TimeManagement::GetSingleton()->GetFrameTime() * 5;
			}
			else
			{
				speed = TimeManagement::GetSingleton()->GetFrameTime() * 1.5f;
			}

			//std::cout << "MOVEMENT\n";
			m_path->WalkProcess(&m_mapPosition, speed);
		}
		


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
						&& !m_storage->IsStorageEmpty())
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
					if (!m_storage->IsStorageEmpty())
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
				if (!m_storage->IsStorageEmpty())
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
				if (!m_isEnteredIntoBuilding)
				{
					_builds->m_vines.WorkerEnteringInThisPosition(m_mapPosition);

					m_isEnteredIntoBuilding = true;
				}

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
				if (!m_isEnteredIntoBuilding)
				{
					_builds->m_stompingVats.WorkerEnteringInThisPosition(m_mapPosition);
					
					m_isEnteredIntoBuilding = true;
				}

				m_isWorkerWasWorkingInBuilding = true;
				//std::cout << "Working ...\n";

				if (_builds->m_stompingVats.CheckHasProducedRessource(m_mapPosition) == true)
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
				if (!m_isEnteredIntoBuilding)
				{
					_builds->m_winePress.WorkerEnteringInThisPosition(m_mapPosition);

					m_isEnteredIntoBuilding = true;
				}

				m_isWorkerWasWorkingInBuilding = true;
				//std::cout << "Working ...\n";

				if (_builds->m_winePress.CheckHasProducedRessource(m_mapPosition) == true)
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
				if (!m_isEnteredIntoBuilding)
				{
					_builds->m_wineStorehouse.WorkerEnteringInThisPosition(m_mapPosition);

					m_isEnteredIntoBuilding = true;
				}

				m_isWorkerWasWorkingInBuilding = true;
				//std::cout << "Working ...\n";
							   				 
				if (_builds->m_wineStorehouse.CheckHasProducedRessource(m_mapPosition) == true)
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
				if (!m_isEnteredIntoBuilding)
				{
					_builds->m_storehouse.WorkerEnteringInThisPosition(m_mapPosition);

					m_isEnteredIntoBuilding = true;
				}

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
				if (!m_isEnteredIntoBuilding)
				{
					_builds->m_stall->WorkerEnteringInThisPosition(m_mapPosition);

					m_isEnteredIntoBuilding = true;
				}

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
			if (!m_storage->IsStorageEmpty())
			{
				if (m_storage->IsResourceExistHere(BUNCH_OF_GRAPE))
				{
					// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
					if (_builds->m_vines.UpdateRessourcePickuping(m_mapPosition))
					{
						Storage::TransferOfTheWholeResource(_builds->m_vines.GetStorage(m_mapPosition), m_storage, Ressources::GetNameFromEnum(BUNCH_OF_GRAPE));

						_builds->m_vines.RessourcePickedUp(m_mapPosition);

						*(m_targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;

						m_isItWorkingPlace = false;

						sf::Vector2i targetedPosition = _builds->m_stompingVats.FindNearestBuilding(m_mapPosition);

						if (targetedPosition != sf::Vector2i(RESET, RESET))
						{
							SetEndingPosition(targetedPosition);
							ActiveLauchingMovement();
						}

						SetWorkerStatus(IDLE);

						std::cout << "Worker, picked up ressources already carried\n";
					}
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				if (_builds->m_vines.UpdateRessourcePickuping(m_mapPosition))
				{
					// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
					m_storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(BUNCH_OF_GRAPE));
					Storage::TransferOfTheWholeResource(_builds->m_vines.GetStorage(m_mapPosition), m_storage, Ressources::GetNameFromEnum(BUNCH_OF_GRAPE));

					_builds->m_vines.RessourcePickedUp(m_mapPosition);

					m_targetedBuilding = new enum TypeOfBuilding;
					*(m_targetedBuilding) = BUILDING_GRAPE_STOMPING_VATS;

					m_isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _builds->m_stompingVats.FindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition);
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
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
			// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

			// That mean the worker has already picked up a ressource
			if (!m_storage->IsStorageEmpty())
			{
				if (m_storage->IsResourceExistHere(GRAPES_MUST))
				{
					// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
					if (_builds->m_stompingVats.UpdateRessourcePickuping(m_mapPosition))
					{
						Storage::TransferOfTheWholeResource(_builds->m_stompingVats.GetStorage(m_mapPosition), m_storage, Ressources::GetNameFromEnum(GRAPES_MUST));

						_builds->m_stompingVats.RessourcePickedUp(m_mapPosition);

						*(m_targetedBuilding) = BUILDING_WINE_PRESS;

						m_isItWorkingPlace = false;

						sf::Vector2i targetedPosition = _builds->m_winePress.FindNearestBuilding(m_mapPosition);

						if (targetedPosition != sf::Vector2i(RESET, RESET))
						{
							SetEndingPosition(targetedPosition);
							ActiveLauchingMovement();
						}

						SetWorkerStatus(IDLE);

						std::cout << "Worker, picked up ressources already carried\n";
					}
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				if (_builds->m_stompingVats.UpdateRessourcePickuping(m_mapPosition))
				{
					// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
					m_storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(GRAPES_MUST));
					Storage::TransferOfTheWholeResource(_builds->m_stompingVats.GetStorage(m_mapPosition), m_storage, Ressources::GetNameFromEnum(GRAPES_MUST));

					_builds->m_stompingVats.RessourcePickedUp(m_mapPosition);

					m_targetedBuilding = new enum TypeOfBuilding;
					*(m_targetedBuilding) = BUILDING_WINE_PRESS;

					m_isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _builds->m_winePress.FindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition);
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_builds->m_stompingVats.CheckHasProducedRessource(m_mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}

			// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

			//// That mean the worker has already picked up a ressource
			//if (m_ressourceHeld != nullptr)
			//{
			//	// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
			//	if (*(m_ressourceHeld) == GRAPES_MUST)
			//	{
			//		int ressourceProduced = _builds->m_stompingVats.UpdateRessourcePickuping(m_mapPosition, _time->GetFrameTime());

			//		*(m_quantityRessourceHeld) += ressourceProduced;
			//		*(m_targetedBuilding) = BUILDING_WINE_PRESS;

			//		m_isItWorkingPlace = false;

			//		sf::Vector2i targetedPosition = _builds->m_winePress.FindNearestBuilding(m_mapPosition);

			//		if (targetedPosition != sf::Vector2i(RESET, RESET))
			//		{
			//			SetEndingPosition(targetedPosition, _map->GetMap());
			//			ActiveLauchingMovement();
			//		}

			//		SetWorkerStatus(IDLE);

			//		std::cout << "Worker, picked up ressources\n";
			//	}
			//	else
			//	{
			//		std::cout << "The current worker already has a ressource in hands\n\n";
			//		SetWorkerStatus(IDLE);
			//	}
			//}
			//else
			//{
			//	int ressourceProduced = _builds->m_stompingVats.UpdateRessourcePickuping(m_mapPosition, _time->GetFrameTime());

			//	// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
			//	if (ressourceProduced != 0 && ressourceProduced > 0)
			//	{
			//		m_ressourceHeld = new enum TypesOfRessources;
			//		m_quantityRessourceHeld = new int;
			//		m_targetedBuilding = new enum TypeOfBuilding;

			//		*(m_ressourceHeld) = GRAPES_MUST;
			//		*(m_quantityRessourceHeld) = ressourceProduced;
			//		*(m_targetedBuilding) = BUILDING_WINE_PRESS;

			//		m_isItWorkingPlace = false;
			//		
			//		sf::Vector2i targetedPosition = _builds->m_winePress.FindNearestBuilding(m_mapPosition);

			//		if (targetedPosition != sf::Vector2i(RESET, RESET))
			//		{
			//			SetEndingPosition(targetedPosition, _map->GetMap());
			//			ActiveLauchingMovement();
			//		}

			//		SetWorkerStatus(IDLE);

			//		std::cout << "Worker, picked up ressources\n";
			//	}
			//	// We check if the building is still in the produced state
			//	else if (_builds->m_stompingVats.CheckHasProducedRessource(m_mapPosition) == true)
			//	{
			//		SetWorkerStatus(PICKUP_RESSOURCES);
			//	}
			//	// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
			//	else if (_builds->m_stompingVats.CheckHasProducedRessource(m_mapPosition) == false)
			//	{
			//		// For security, we reaffect the worker to the WORKING status
			//		SetWorkerStatus(WORKING);

			//		std::cout << "Worker, error during pick up of ressources\n";
			//	}
			//}

		}
		else if (m_currentBuilding == BUILDING_WINE_PRESS)
		{
			// That mean the worker has already picked up a ressource
			if (!m_storage->IsStorageEmpty())
			{
				if (m_storage->IsResourceExistHere(GRAPE_JUICE))
				{
					// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
					if (_builds->m_winePress.UpdateRessourcePickuping(m_mapPosition))
					{
						Storage::TransferOfTheWholeResource(_builds->m_winePress.GetStorage(m_mapPosition), m_storage, Ressources::GetNameFromEnum(GRAPE_JUICE));

						_builds->m_winePress.RessourcePickedUp(m_mapPosition);

						*(m_targetedBuilding) = BUILDING_WINE_STOREHOUSE;

						m_isItWorkingPlace = false;

						sf::Vector2i targetedPosition = _builds->m_wineStorehouse.FindNearestBuilding(m_mapPosition);

						if (targetedPosition != sf::Vector2i(RESET, RESET))
						{
							SetEndingPosition(targetedPosition);
							ActiveLauchingMovement();
						}

						SetWorkerStatus(IDLE);

						std::cout << "Worker, picked up ressources already carried\n";
					}
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				if (_builds->m_winePress.UpdateRessourcePickuping(m_mapPosition))
				{
					// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
					m_storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(GRAPE_JUICE));
					Storage::TransferOfTheWholeResource(_builds->m_winePress.GetStorage(m_mapPosition), m_storage, Ressources::GetNameFromEnum(GRAPE_JUICE));

					_builds->m_winePress.RessourcePickedUp(m_mapPosition);

					m_targetedBuilding = new enum TypeOfBuilding;
					*(m_targetedBuilding) = BUILDING_WINE_STOREHOUSE;

					m_isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _builds->m_wineStorehouse.FindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition);
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_builds->m_winePress.CheckHasProducedRessource(m_mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}

			// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

			//// That mean the worker has already picked up a ressource
			//if (m_ressourceHeld != nullptr)
			//{
			//	// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
			//	if (*(m_ressourceHeld) == GRAPE_JUICE)
			//	{
			//		int ressourceProduced = _builds->m_winePress.UpdateRessourcePickuping(m_mapPosition, _time->GetFrameTime());

			//		*(m_quantityRessourceHeld) += ressourceProduced;
			//		*(m_targetedBuilding) = BUILDING_WINE_STOREHOUSE;
			//		
			//		m_isItWorkingPlace = false;

			//		sf::Vector2i targetedPosition = _builds->m_wineStorehouse.FindNearestBuilding(m_mapPosition);

			//		if (targetedPosition != sf::Vector2i(RESET, RESET))
			//		{
			//			SetEndingPosition(targetedPosition, _map->GetMap());
			//			ActiveLauchingMovement();
			//		}

			//		SetWorkerStatus(IDLE);

			//		std::cout << "Worker, picked up ressources\n";

			//		
			//	}
			//	else
			//	{
			//		std::cout << "The current worker already has a ressource in hands\n\n";
			//		SetWorkerStatus(IDLE);
			//	}
			//}
			//else
			//{
			//	int ressourceProduced = _builds->m_winePress.UpdateRessourcePickuping(m_mapPosition, _time->GetFrameTime());

			//	// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
			//	if (ressourceProduced != 0 && ressourceProduced > 0)
			//	{
			//		m_ressourceHeld = new enum TypesOfRessources;
			//		m_quantityRessourceHeld = new int;
			//		m_targetedBuilding = new enum TypeOfBuilding;

			//		*(m_ressourceHeld) = GRAPE_JUICE;
			//		*(m_quantityRessourceHeld) = ressourceProduced;
			//		*(m_targetedBuilding) = BUILDING_WINE_STOREHOUSE;

			//		m_isItWorkingPlace = false;

			//		sf::Vector2i targetedPosition = _builds->m_wineStorehouse.FindNearestBuilding(m_mapPosition);

			//		if (targetedPosition != sf::Vector2i(RESET, RESET))
			//		{
			//			SetEndingPosition(targetedPosition, _map->GetMap());
			//			ActiveLauchingMovement();
			//		}

			//		SetWorkerStatus(IDLE);

			//		std::cout << "Worker, picked up ressources\n";
			//	}
			//	// We check if the building is still in the produced state
			//	else if (_builds->m_winePress.CheckHasProducedRessource(m_mapPosition) == true)
			//	{
			//		SetWorkerStatus(PICKUP_RESSOURCES);
			//	}
			//	// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
			//	else if (_builds->m_winePress.CheckHasProducedRessource(m_mapPosition) == false)
			//	{
			//		// For security, we reaffect the worker to the WORKING status
			//		SetWorkerStatus(WORKING);

			//		std::cout << "Worker, error during pick up of ressources\n";
			//	}
			//}

		}
		else if (m_currentBuilding == BUILDING_WINE_STOREHOUSE)
		{
			// That mean the worker has already picked up a ressource
			if (!m_storage->IsStorageEmpty())
			{
				if (m_storage->IsResourceExistHere(AMPHORA_OF_WINE))
				{
					// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
					if (_builds->m_wineStorehouse.UpdateRessourcePickuping(m_mapPosition))
					{
						Storage::TransferOfTheWholeResource(_builds->m_wineStorehouse.GetStorage(m_mapPosition), m_storage, Ressources::GetNameFromEnum(AMPHORA_OF_WINE));

						_builds->m_wineStorehouse.RessourcePickedUp(m_mapPosition);

						*(m_targetedBuilding) = BUILDING_STOREHOUSE;

						m_isItWorkingPlace = false;

						sf::Vector2i targetedPosition = _builds->m_storehouse.FindNearestBuilding(m_mapPosition);

						if (targetedPosition != sf::Vector2i(RESET, RESET))
						{
							SetEndingPosition(targetedPosition);
							ActiveLauchingMovement();
						}

						SetWorkerStatus(IDLE);

						std::cout << "Worker, picked up ressources already carried\n";
					}
				}
				else
				{
					std::cout << "The current worker already has a ressource in hands\n\n";
					SetWorkerStatus(IDLE);
				}
			}
			else
			{
				if (_builds->m_wineStorehouse.UpdateRessourcePickuping(m_mapPosition))
				{
					// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
					m_storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(AMPHORA_OF_WINE));
					Storage::TransferOfTheWholeResource(_builds->m_wineStorehouse.GetStorage(m_mapPosition), m_storage, Ressources::GetNameFromEnum(AMPHORA_OF_WINE));

					_builds->m_wineStorehouse.RessourcePickedUp(m_mapPosition);

					m_targetedBuilding = new enum TypeOfBuilding;
					*(m_targetedBuilding) = BUILDING_STOREHOUSE;

					m_isItWorkingPlace = false;

					sf::Vector2i targetedPosition = _builds->m_storehouse.FindNearestBuilding(m_mapPosition);

					if (targetedPosition != sf::Vector2i(RESET, RESET))
					{
						SetEndingPosition(targetedPosition);
						ActiveLauchingMovement();
					}

					SetWorkerStatus(IDLE);

					std::cout << "Worker, picked up ressources\n";
				}
				// We check if the building is still in the produced state
				// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
				else if (_builds->m_wineStorehouse.CheckHasProducedRessource(m_mapPosition) == false)
				{
					// For security, we reaffect the worker to the WORKING status
					SetWorkerStatus(WORKING);

					std::cout << "Worker, error during pick up of ressources\n";
				}
			}



			//// That mean the worker has already picked up a ressource
			//if (m_ressourceHeld != nullptr)
			//{
			//	// TEMPORAIRE -> DEVOIR METTRE UNE QUANTITÉ MAX A TRANSPORTER
			//	if (*(m_ressourceHeld) == AMPHORA_OF_WINE)
			//	{
			//		int ressourceProduced = _builds->m_wineStorehouse.UpdateRessourcePickuping(m_mapPosition, _time->GetFrameTime());

			//		*(m_quantityRessourceHeld) += ressourceProduced;
			//		*(m_targetedBuilding) = BUILDING_STOREHOUSE;
			//		
			//		m_isItWorkingPlace = false;

			//		sf::Vector2i targetedPosition = _builds->m_storehouse.FindNearestBuilding(m_mapPosition);

			//		if (targetedPosition != sf::Vector2i(RESET, RESET))
			//		{
			//			SetEndingPosition(targetedPosition, _map->GetMap());
			//			ActiveLauchingMovement();
			//		}

			//		SetWorkerStatus(IDLE);

			//		std::cout << "Worker, picked up ressources\n";

			//		
			//	}
			//	else
			//	{
			//		std::cout << "The current worker already has a ressource in hands\n\n";
			//		SetWorkerStatus(IDLE);
			//	}
			//}
			//else
			//{
			//	int ressourceProduced = _builds->m_wineStorehouse.UpdateRessourcePickuping(m_mapPosition, _time->GetFrameTime());

			//	// If this variable is higher than 0, that mean that the building has produced some ressources and the worker picked up it
			//	if (ressourceProduced != 0 && ressourceProduced > 0)
			//	{
			//		m_ressourceHeld = new enum TypesOfRessources;
			//		m_quantityRessourceHeld = new int;
			//		m_targetedBuilding = new enum TypeOfBuilding;

			//		*(m_ressourceHeld) = AMPHORA_OF_WINE;
			//		*(m_quantityRessourceHeld) = ressourceProduced;
			//		*(m_targetedBuilding) = BUILDING_STOREHOUSE;

			//		m_isItWorkingPlace = false;

			//		sf::Vector2i targetedPosition = _builds->m_storehouse.FindNearestBuilding(m_mapPosition);

			//		if (targetedPosition != sf::Vector2i(RESET, RESET))
			//		{
			//			SetEndingPosition(targetedPosition, _map->GetMap());
			//			ActiveLauchingMovement();
			//		}

			//		SetWorkerStatus(IDLE);

			//		std::cout << "Worker, picked up ressources\n";
			//	}
			//	// We check if the building is still in the produced state
			//	else if (_builds->m_wineStorehouse.CheckHasProducedRessource(m_mapPosition) == true)
			//	{
			//		SetWorkerStatus(PICKUP_RESSOURCES);
			//	}
			//	// If the result is false, that mean the building isn't there or has been destroyed or isn't ready to produce
			//	else if (_builds->m_wineStorehouse.CheckHasProducedRessource(m_mapPosition) == false)
			//	{
			//		// For security, we reaffect the worker to the WORKING status
			//		SetWorkerStatus(WORKING);

			//		std::cout << "Worker, error during pick up of ressources\n";
			//	}
			//}
		}
		else
		{
			std::cout << "ERROR, the worker don't find a building where works\n";
		}

		break;

	case DEPOSIT_RESSOURCES:
		
		if (m_storage != nullptr)
		{
			for (int i = 0; i < m_storage->GetNumberOfResources(); i++)
			{
				switch (Ressources::GetEnumFromName(m_storage->GetResource(i)->GetName()))
				{
				case GRAPE_VINE:
					break;
				case BUNCH_OF_GRAPE:

					if (_builds->m_stompingVats.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true))
					{
						m_timeToDeposit += TimeManagement::GetSingleton()->GetFrameTime();

						if (m_timeToDeposit >= _builds->m_buildings[BUILDING_GRAPE_STOMPING_VATS].GetDepositingTimeCost())
						{
							std::cout << "Worker, want to deposit ressources\n";

							Storage::TransferOfTheWholeResource(m_storage, _builds->m_stompingVats.GetStorage(m_mapPosition), Ressources::GetNameFromEnum(BUNCH_OF_GRAPE));
							m_storage->RemoveResourceOfStorage(Ressources::GetNameFromEnum(BUNCH_OF_GRAPE));

							std::cout << "Deposit ressources !\n";
							SetTimeToDeposit(RESET);

							// Reset of the main data
							delete m_targetedBuilding;
							m_targetedBuilding = nullptr;

							m_timeToDeposit = RESET;

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
						m_timeToDeposit += TimeManagement::GetSingleton()->GetFrameTime();

						if (m_timeToDeposit >= _builds->m_buildings[BUILDING_WINE_PRESS].GetDepositingTimeCost())
						{
							std::cout << "Worker, want to deposit ressources\n";

							Storage::TransferOfTheWholeResource(m_storage, _builds->m_winePress.GetStorage(m_mapPosition), Ressources::GetNameFromEnum(GRAPES_MUST));
							m_storage->RemoveResourceOfStorage(Ressources::GetNameFromEnum(GRAPES_MUST));

							SetTimeToDeposit(RESET);

							// Reset of the main data
							delete m_targetedBuilding;
							m_targetedBuilding = nullptr;

							m_timeToDeposit = RESET;

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
						m_timeToDeposit += TimeManagement::GetSingleton()->GetFrameTime();

						if (m_timeToDeposit >= _builds->m_buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
						{
							std::cout << "Worker, want to deposit ressources\n";

							Storage::TransferOfTheWholeResource(m_storage, _builds->m_wineStorehouse.GetStorage(m_mapPosition), Ressources::GetNameFromEnum(GRAPE_JUICE));
							m_storage->RemoveResourceOfStorage(Ressources::GetNameFromEnum(GRAPE_JUICE));

							SetTimeToDeposit(RESET);

							// Reset of the main data
							delete m_targetedBuilding;
							m_targetedBuilding = nullptr;

							m_timeToDeposit = RESET;

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
					if (_builds->m_wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(m_mapPosition, false, true))
					{
						m_timeToDeposit += TimeManagement::GetSingleton()->GetFrameTime();

						if (m_timeToDeposit >= _builds->m_buildings[BUILDING_WINE_STOREHOUSE].GetDepositingTimeCost())
						{
							std::cout << "Worker, want to deposit ressources\n";

							Storage::TransferOfTheWholeResource(m_storage, _builds->m_wineStorehouse.GetStorage(m_mapPosition), Ressources::GetNameFromEnum(AMPHORAS));
							m_storage->RemoveResourceOfStorage(Ressources::GetNameFromEnum(AMPHORAS));

							SetTimeToDeposit(RESET);

							// Reset of the main data
							delete m_targetedBuilding;
							m_targetedBuilding = nullptr;

							m_timeToDeposit = RESET;

							SetWorkerStatus(WORKING);
						}
					}
					else
					{
						SetWorkerStatus(IDLE);
					}

					break;
				case AMPHORA_OF_WINE:

					if (_builds->m_storehouse.ConfirmStorehousePresenceAtPosition(m_mapPosition, false, true))
					{
						m_timeToDeposit += TimeManagement::GetSingleton()->GetFrameTime();

						if (m_timeToDeposit >= _builds->m_buildings[BUILDING_STOREHOUSE].GetDepositingTimeCost())
						{
							std::cout << "Worker, deposit amphora of wine ressources\n";

							Storage::TransferOfTheWholeResource(m_storage, _builds->m_storehouse.GetStorage(m_mapPosition), Ressources::GetNameFromEnum(AMPHORA_OF_WINE));
							m_storage->RemoveResourceOfStorage(Ressources::GetNameFromEnum(AMPHORA_OF_WINE));

							SetTimeToDeposit(RESET);

							// Reset of the main data
							delete m_targetedBuilding;
							m_targetedBuilding = nullptr;

							m_timeToDeposit = RESET;

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

void Workers::DestroyStorage()
{
	if (m_storage != nullptr)
	{
		delete m_storage;
		m_storage = nullptr;
	}
}

void Workers::CreateNewStorage()
{
	DestroyStorage();

	if (m_storage == nullptr)
	{
		m_storage = new Storage();
		m_storage->SetName("Worker");
	}
}