#include "Stalls.h"
#include "EnumPurchaserStatus.h"
#include "GameDefinitions.h"
#include "Money.h"
#include "Map.h"
#include "TimeManagement.h"
#include "RessourcesManager.h"



Stalls::Stalls(Buildings *_specificBuildingConcerned)
{	
	m_building = _specificBuildingConcerned;

	m_storage = nullptr;

	m_internalState = InternalState::STATE_INIT;

	m_mapPosition = sf::Vector2f(0, 0);
	m_constructionState = BuildingStatus::BUILDING_DESTROYED;
	m_actualState = StallStatus::STALL_NOT_CONSTRUCTED;

	m_quantitativeThreshold = RESET;
	m_maximalQuantity = RESET;
	m_internalRessourceCounter = RESET;

	m_numberOfWorkersNeededToWorks = RESET;
	m_currentNumberOfWorkersPresent = RESET;

	m_lifeTime = RESET;
	m_actualProductionTime = RESET;

	m_isChangingSprite = false;
	m_hasBeenBuilt = false;
	m_isWorkerThere = false;
	m_isPurchaserThere = false;

	m_priceAccepted = RESET;
	m_internalImportRessourceCounterSaved = RESET;
	m_ressourceQuantityToSell = RESET;
	m_isNewMerchantNeeded = false;

	m_storehousesCoordinates = nullptr;
	m_numberStorehousesCoordinates = 0;
	
	m_isOfferAccepted = false;
}


Stalls::~Stalls()
{
	if (m_building != nullptr)
	{
		m_building = nullptr;
	}

	if (m_storehousesCoordinates != nullptr)
	{
		if (m_numberStorehousesCoordinates > 1)
		{
			delete[] m_storehousesCoordinates;
		}
		else
		{
			delete m_storehousesCoordinates;
		}

		m_storehousesCoordinates = nullptr;
	}

	ClearStorages();
}


void Stalls::InitialisationStall(Buildings *_stallBuildingConcerned)
{
	m_building = _stallBuildingConcerned;
}

void Stalls::SetConstructionStatus(const enum BuildingStatus &_newStatus)
{
	m_constructionState = _newStatus;
}

void Stalls::SetStatus(const enum StallStatus &_newStatus)
{
	m_actualState = _newStatus;
}

void Stalls::SetIsNewMerchantNeeded(const bool &_newStatus)
{
	m_isNewMerchantNeeded = _newStatus;
}


void Stalls::AddNewBuilding(sf::Vector2f _mapPosition)
{
	// Save the position in map
	m_mapPosition = _mapPosition;

	// Allocation of the storage
	m_storage = new Storage();
	m_storage->AddNewResourceToStorage(Ressources::GetNameFromEnum(AMPHORA_OF_WINE), ResourceData::RESOURCE_NEEDED_N_PRODUCED);
	m_storage->SetName("Stall");

	m_internalState = InternalState::STATE_INIT;

	// Init of the building construction status after being placed on map
	m_constructionState = PLANNED;
	m_actualState = STALL_WAITING;

	// A MODIFIER PAR VALEUR SEUIL
	m_quantitativeThreshold = this->m_building->GetRessourceQuantityNeeded();

	// A CONFIGURER + RELIER AU BATIMENT
	m_maximalQuantity = 100;
	m_internalRessourceCounter = RESET;

	m_lifeTime = RESET;
	m_actualProductionTime = RESET;

	m_isChangingSprite = false;
	m_hasBeenBuilt = false;
	m_isWorkerThere = false;
	m_isPurchaserThere = false;
	m_isNewMerchantNeeded = false;

	m_internalImportRessourceCounterSaved = RESET;
	m_ressourceQuantityToSell = RESET;
}

void Stalls::AddStorehousePosition(const sf::Vector2f &_mapPosition)
{
	if (m_storehousesCoordinates == nullptr)
	{
		m_numberStorehousesCoordinates = 1;

		m_storehousesCoordinates = new sf::Vector2f;
		*m_storehousesCoordinates = _mapPosition;

		std::cout << "New storage added to the stall (first) : " << m_storehousesCoordinates[0].x << " " << m_storehousesCoordinates[0].y << std::endl;
	}
	else
	{
		// We create a new list
		sf::Vector2f *newListOfCoordinates = new sf::Vector2f[m_numberStorehousesCoordinates + 1]();

		// We copy the old data
		for (int i = 0; i < m_numberStorehousesCoordinates; i++)
		{
			newListOfCoordinates[i] = m_storehousesCoordinates[i];
		}

		// We add the new position at the end of the list
		newListOfCoordinates[m_numberStorehousesCoordinates + 1] = _mapPosition;

		m_numberStorehousesCoordinates += 1;

		// We delete the actual memory location of the "storehousesCoordinates" variable, and add the adress of the new list
		delete m_storehousesCoordinates;
		m_storehousesCoordinates = nullptr;
		m_storehousesCoordinates = newListOfCoordinates;


		for (int i = 0; i < m_numberStorehousesCoordinates; i++)
		{
			std::cout << "New storage added to the stall : " << m_storehousesCoordinates[i].x << " " << m_storehousesCoordinates[i].y << std::endl;
		}
	}
}


bool Stalls::DestroyedBuildingSelected(const sf::Vector2f &_mapPosition)
{
	if (m_mapPosition == _mapPosition)
	{
		// Reset of the building construction status after being placed on map
		m_constructionState = BUILDING_DESTROYED;
		m_actualState = STALL_WAITING;

		// A MODIFIER PAR VALEUR SEUIL
		m_quantitativeThreshold = m_building->GetRessourceQuantityNeeded();

		// A CONFIGURER + RELIER AU BATIMENT
		m_maximalQuantity = 100;
		m_internalRessourceCounter = RESET;

		m_lifeTime = RESET;
		m_actualProductionTime = RESET;

		m_isChangingSprite = false;
		m_hasBeenBuilt = false;
		m_isWorkerThere = false;
		m_isPurchaserThere = false;
		m_isNewMerchantNeeded = false;

		m_internalImportRessourceCounterSaved = RESET;
		m_ressourceQuantityToSell = RESET;

		return true;
	}
	else
	{
		return false;
	}

}

bool Stalls::IsBuildingIsWorking(const sf::Vector2f& _mapPosition) const
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		if (m_currentNumberOfWorkersPresent >= m_numberOfWorkersNeededToWorks)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Stalls::ConfirmPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates, const bool &_thisIsAWorker)
{
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		if (_isPreciseCoordinates)
		{
			if (_mapPosition == m_mapPosition)
			{
				if (_thisIsAWorker)
				{
					m_isWorkerThere = true;
				}

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (_thisIsAWorker)
			{
				m_isWorkerThere = true;
			}

			return true;
		}
	}
	else
	{
		return false;
	}
}

void Stalls::WorkerEnteringInThisPosition(const sf::Vector2f& _mapPosition)
{
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		m_currentNumberOfWorkersPresent += 1;
	}
}


void Stalls::WorkerLeavingThisPosition(const sf::Vector2f& _mapPosition)
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		m_currentNumberOfWorkersPresent -= 1;

		if (m_currentNumberOfWorkersPresent == 0)
		{
			m_isWorkerThere = false;
		}
		else if (m_currentNumberOfWorkersPresent < 0)
		{
			m_currentNumberOfWorkersPresent = 0;
		}
	}
}


int Stalls::GetNumberOfWorkersPresents(const sf::Vector2f& _mapPosition) const
{
	if (_mapPosition == m_mapPosition)
	{
		return m_currentNumberOfWorkersPresent;
	}
}


bool Stalls::GetWorkerIsThere(const sf::Vector2f &_mapPosition)
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition == m_mapPosition)
	{
		return m_isWorkerThere;
	}
	else
	{
		return false;
	}			
}


enum StallStatus Stalls::GetStatus()
{
	return m_actualState;
}

enum BuildingStatus Stalls::GetConstructionStatus()
{
	return m_constructionState;
}

int Stalls::GetActualRessourcesStocked()
{
	return m_internalRessourceCounter;
}


bool Stalls::GetIsNewMerchantNeeded()
{
	return m_isNewMerchantNeeded;
}

int Stalls::GetRessourceStocked() const
{
	return RessourcesManager::GetSingleton()->GetResourceQuantity(Ressources::GetNameFromEnum(AMPHORA_OF_WINE), "Storehouse");
}


void Stalls::UpdateBuildingConstruction()
{
	switch (m_constructionState)
	{
	case PLANNED:
		// Plannification of the building construction

		// If we've the workers accredited to the building construction, we can pass the state to CONSTRUCTION
		//std::cout << "Planned ! " << std::endl;

		// TEMPORARY
		if (m_isWorkerThere == true)
		{
			std::cout << "Building launched ! " << m_lifeTime << " " << m_building->GetConstructionTimeCost() << std::endl;
			m_constructionState = CONSTRUCTION;
			m_lifeTime = RESET;
		}

		break;
	case CONSTRUCTION:

		if (m_isWorkerThere == true)
		{
			m_lifeTime += TimeManagement::GetSingleton()->GetFrameTime();
			m_isWorkerThere = false;
		}

		// If the building life is higher than the construction time, we launch it's growthing
		if (m_lifeTime >= m_building->GetConstructionTimeCost())
		{
			std::cout << "Building built ! " << m_lifeTime << " " << m_building->GetConstructionTimeCost() << std::endl;
			m_constructionState = BUILT;
			m_isChangingSprite = true;
		}

		break;
	case BUILT:
		if (m_hasBeenBuilt == false)
		{
			m_isChangingSprite = true;
			std::cout << "Building successfully constructed ! " << std::endl;
		}

		break;
	case BUILDING_NOT_MAINTAINED:
		break;

	default:
		// ERROR LOG
		break;
	}
}




void Stalls::UpdateInternalCycles(Money *_money, enum CurrentGameState *_state, Purchasers *_purchasers, Storehouse *_storehouse)
{
	if (m_constructionState == BUILT)
	{
		switch (m_actualState)
		{
		case STALL_WAITING:
			
			// - Si recoit information qu'il y a un purchaser qui attend : 
			//		- Check la quantité à vendre
			// - Afficher Selling Window avec la quantité max à vendre
			// - Une fois la quantité validée :
			//		- Transfert des entrepots vers stall
			//		- Quand Transfert fini :
			//			- Transfert de stall vers purchaser
			//			- Purchaser part
			//			- Quand Purchaser parti -> le delete

			if (m_isPurchaserThere)
			{
				std::cout << "Here\n";
				SetStatus(STALL_PURCHASER_IS_PRESENT);
				*(_state) = SELLING_WINDOW;
			}

			break;

		case STALL_PURCHASER_IS_PRESENT:
			
			break;

		case STALL_OFFER_HANDLED:
			
			// - Animation de vente des amphores
			// - Vente des amphores en échange de sesterces

			if (m_isOfferAccepted == true)
			{
				// - Une fois la quantité validée :
				//		- Transfert des entrepots vers stall
				//		- Quand Transfert fini :
				//			- Transfert de stall vers purchaser
				//			- Purchaser part
				//			- Quand Purchaser parti -> le delete


				// PROBLEME DU SWITCH
				switch (m_internalState)
				{
				case InternalState::STATE_INIT:

					if (_purchasers != nullptr)
					{
						_purchasers->SetStatus(PurchaserStatus::WAITING_RESOURCES);
					}

					// Transfer of the resources from the differents storages to the stall
					if (m_storehousesCoordinates != nullptr)
					{
						int quantityToSell = m_ressourceQuantityToSell;
						m_actualProductionTime = 0.0f;

						for (size_t i = 0; i < m_numberStorehousesCoordinates; i++)
						{
							Storage* storage = _storehouse->GetStorage(m_storehousesCoordinates[i]);
							Ressources* resource = storage->GetResource(Ressources::GetNameFromEnum(AMPHORA_OF_WINE));

							if (resource->GetQuantityOwned() >= quantityToSell)
							{
								Storage::TransferOfResource(storage, m_storage, Ressources::GetNameFromEnum(AMPHORA_OF_WINE), m_ressourceQuantityToSell);
								quantityToSell = 0;
							}
							else if (resource->GetQuantityOwned() != 0
								&& resource->GetQuantityOwned() < quantityToSell)
							{
								int newQuantityAdded = resource->GetQuantityOwned();
								Storage::TransferOfTheWholeResource(storage, m_storage, Ressources::GetNameFromEnum(AMPHORA_OF_WINE));

								quantityToSell -= newQuantityAdded;
							}

							// If we have finished to transfer the resources, we brake the for loop
							if (quantityToSell == 0) break;
						}
					}

					// Transfer the resources into the reserve to securise them
					m_storage->GetResource(Ressources::GetNameFromEnum(AMPHORA_OF_WINE))->TransferFromOwnedToReserved(m_storage->GetResource(Ressources::GetNameFromEnum(AMPHORA_OF_WINE))->GetQuantityOwned());

					m_internalState = InternalState::STATE_UPDATE;
					break;
				case InternalState::STATE_UPDATE:

					// Transfer resources from the stall to the purchaser
					if (m_storage->GetResource(Ressources::GetNameFromEnum(AMPHORA_OF_WINE))->GetQuantityReserved() - 1 >= 0)
					{
						m_actualProductionTime += TimeManagement::GetSingleton()->GetFrameTime();

						if (m_actualProductionTime > m_building->GetPickupingTimeCost())
						{
							m_actualProductionTime = 0.0f;
							if (_purchasers != nullptr)
							{
								_purchasers->SetStatus(PurchaserStatus::PICKUP_RESSOURCES);
							}

							m_storage->GetResource(Ressources::GetNameFromEnum(AMPHORA_OF_WINE))->AddOrSubtractQuantityReserved(-1);
						}
					}
					else
					{
						// Once the transfer is done, the purchaser pays the money negociated to the vineyard
						_money->AddMoney(m_priceAccepted * m_ressourceQuantityToSell);
						if (_purchasers != nullptr)
						{
							// Ask to the purchaser to leave the place
							_purchasers->SetStatus(PurchaserStatus::IDLE);
							_purchasers->SetCanLeaveTheMap(true);
						}

						m_isPurchaserThere = false;
						m_ressourceQuantityToSell = RESET;

						// Wait a new purchaser
						m_internalState = InternalState::STATE_INIT;
						m_actualState = STALL_WAITING;
					}

					break;
				default:
					break;
				}
			}
			else
			{
				// A MODIFIER EN : le purchaser réoffre une autre proposition, et si elle est refusée, là, il part
				// Ask to the purchaser to leave the place
				if (_purchasers != nullptr)
				{
					_purchasers->SetStatus(PurchaserStatus::IDLE);
					_purchasers->SetCanLeaveTheMap(true);
				}

				m_isPurchaserThere = false;
				m_ressourceQuantityToSell = RESET;

				// Wait a new purchaser
				m_internalState = InternalState::STATE_INIT;
				m_actualState = STALL_WAITING;
			}

			break;

		default:

			// ERROR LOG
			m_actualState = STALL_WAITING;
			break;
		}
	}
		
}

void Stalls::UpdateBuildingSprite()
{
	if (m_isChangingSprite == true)
	{
		if (m_constructionState == BUILT
			&& m_hasBeenBuilt == false
			&& m_isChangingSprite == true)
		{
			Map* pMap = Map::GetSingleton();
			unsigned short buildingSpriteID = RESET;

			m_hasBeenBuilt = true;
			m_isChangingSprite = false;

			for (int y = 0; y < m_building->GetSize().y; y++)
			{
				for (int x = 0; x < m_building->GetSize().x; x++)
				{
					if (pMap->IsCoordinatesIsInMap(sf::Vector2i(m_mapPosition.x - x, m_mapPosition.y - y)))
					{
						buildingSpriteID = (unsigned short)m_building->GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_MAIN_FLOOR][m_building->GetSize().y - 1 - y][m_building->GetSize().x - 1 - x];

						if (buildingSpriteID >= 0)
						{
							// Set the correct sprite id for this building
							pMap->GetMap()[FIRST_FLOOR + SPRITE_ID][(int)m_mapPosition.y - y][(int)m_mapPosition.x - x] = buildingSpriteID;

							pMap->GetMap()[FIRST_FLOOR + COLLISIONS_ID][(int)m_mapPosition.y - y][(int)m_mapPosition.x - x] = COLLISION;
						}
					}
				}
			}
		}
	}
}


void Stalls::ClearStorages()
{
	if (m_storage != nullptr)
	{
		// Delete the storage from the list
		delete m_storage;
		m_storage = nullptr;
	}
}


void Stalls::SavingStallForFile(std::ofstream *_file)
{
	// Saving the storage
	_file->write((char*)&m_storage, sizeof(Storage*));

	if (m_storage != nullptr)
	{
		m_storage->SavingForFile(_file);
	}

	_file->write((char *)&m_internalState, sizeof(InternalState));
	_file->write((char *)&m_constructionState, sizeof(enum BuildingStatus));

	if (m_constructionState != BUILDING_DESTROYED)
	{
		// Save the map position
		_file->write((char *)&m_mapPosition.x, sizeof(float));
		_file->write((char *)&m_mapPosition.y, sizeof(float));

		_file->write((char *)&m_actualState, sizeof(enum StallStatus));

		_file->write((char *)&m_quantitativeThreshold, sizeof(int));
		_file->write((char *)&m_maximalQuantity, sizeof(int));
		_file->write((char *)&m_internalRessourceCounter, sizeof(int));

		_file->write((char*)&m_numberOfWorkersNeededToWorks, sizeof(int));
		_file->write((char*)&m_currentNumberOfWorkersPresent, sizeof(int));

		_file->write((char *)&m_lifeTime, sizeof(float));
		_file->write((char *)&m_actualProductionTime, sizeof(float));

		_file->write((char *)&m_isChangingSprite, sizeof(bool));
		_file->write((char *)&m_hasBeenBuilt, sizeof(bool));
		_file->write((char *)&m_isWorkerThere, sizeof(bool));
		_file->write((char *)&m_isPurchaserThere, sizeof(bool));

		_file->write((char *)&m_priceAccepted, sizeof(int));
		_file->write((char *)&m_internalImportRessourceCounterSaved, sizeof(int));
		_file->write((char *)&m_ressourceQuantityToSell, sizeof(int));
		_file->write((char *)&m_isNewMerchantNeeded, sizeof(bool));

		_file->write((char*)&m_isOfferAccepted, sizeof(bool));
	}
}


void Stalls::LoadingStallFromFile(std::ifstream *_file)
{
	// Load the storage of the stall
	Storage* stallStorage = nullptr;
	_file->read((char*)&stallStorage, sizeof(Storage*));

	if (stallStorage != nullptr)
	{
		m_storage = new Storage();
		m_storage->LoadingFromFile(_file);
	}

	_file->read((char*)&m_internalState, sizeof(InternalState));
	_file->read((char *)&m_constructionState, sizeof(enum BuildingStatus));

	if (m_constructionState != BUILDING_DESTROYED)
	{
		// Save the map position
		_file->read((char *)&m_mapPosition.x, sizeof(float));
		_file->read((char *)&m_mapPosition.y, sizeof(float));
			  
		_file->read((char *)&m_actualState, sizeof(enum StallStatus));

		_file->read((char *)&m_quantitativeThreshold, sizeof(int));
		_file->read((char *)&m_maximalQuantity, sizeof(int));
		_file->read((char *)&m_internalRessourceCounter, sizeof(int));

		_file->read((char*)&m_numberOfWorkersNeededToWorks, sizeof(int));
		_file->read((char*)&m_currentNumberOfWorkersPresent, sizeof(int));

		_file->read((char *)&m_lifeTime, sizeof(float));
		_file->read((char *)&m_actualProductionTime, sizeof(float));

		_file->read((char *)&m_isChangingSprite, sizeof(bool));
		_file->read((char*)&m_hasBeenBuilt, sizeof(bool));
		_file->read((char *)&m_isWorkerThere, sizeof(bool));
		_file->read((char *)&m_isPurchaserThere, sizeof(bool));

		_file->read((char *)&m_priceAccepted, sizeof(int));
		_file->read((char *)&m_internalImportRessourceCounterSaved, sizeof(int));
		_file->read((char *)&m_ressourceQuantityToSell, sizeof(int));
		_file->read((char *)&m_isNewMerchantNeeded, sizeof(bool));

		_file->read((char *)&m_isOfferAccepted, sizeof(bool));
	}
}