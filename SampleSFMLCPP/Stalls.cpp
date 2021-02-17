#include "Stalls.h"
#include "GameDefinitions.h"
#include "Money.h"
#include "Map.h"



Stalls::Stalls(Buildings *_specificBuildingConcerned)
{	
	m_building = _specificBuildingConcerned;

	m_constructionState = BUILDING_DESTROYED;
	m_hasBeenBuilt = false;
	m_storehousesCoordinates = nullptr;
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
}


void Stalls::InitialisationStall(Buildings *_stallBuildingConcerned)
{
	m_building = _stallBuildingConcerned;

	m_constructionState = BUILDING_DESTROYED;
	m_hasBeenBuilt = false;
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
		sf::Vector2f *newListOfCoordinates = new sf::Vector2f[this->m_numberStorehousesCoordinates + 1];

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

void Stalls::WorkerLeavingThisPosition(const sf::Vector2f& _mapPosition)
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition.x <= m_mapPosition.x
		&& _mapPosition.x >= m_mapPosition.x - m_building->GetSize().x
		&& _mapPosition.y <= m_mapPosition.y
		&& _mapPosition.y >= m_mapPosition.y - m_building->GetSize().y)
	{
		m_isWorkerThere = false;
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


sf::Vector2i Stalls::GetMapPosition()
{
	return (sf::Vector2i)m_mapPosition;
}







void Stalls::UpdateBuildingConstruction(const float &_frametime)
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
			m_lifeTime += _frametime;
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




void Stalls::UpdateInternalCycles(class Money *_money, enum CurrentGameState *_state, const float &_frametime, Ressources *_ressourceSent, Purchasers *_purchasers, Storehouse *_storehouse)
{
	if (m_constructionState == BUILT)
	{
		switch (m_actualState)
		{
		case STALL_WAITING:
			
			if (m_storehousesCoordinates != nullptr)
			{
				for (int i = 0; i < m_numberStorehousesCoordinates; i++)
				{
					//std::cout << _storehouse->GetNumberResourcesStocked(m_storehousesCoordinates[i]) << std::endl;

					//if (_storehouse->GetNumberResourcesStocked(m_storehousesCoordinates[i]) >= m_quantitativeThreshold)
					if (_ressourceSent->GetQuantityOwned() >= m_quantitativeThreshold)
					{
						m_isNewMerchantNeeded = true;

						m_internalImportRessourceCounterSaved = _storehouse->GetNumberResourcesStocked(m_storehousesCoordinates[i]);

						std::cout << "Pret a attendre purchaser\n";

						m_actualState = STALL_SEND_REQUEST_PURCHASER;
					}
				}
				
			}
			
			break;

		case STALL_SEND_REQUEST_PURCHASER:
			
			m_actualProductionTime += _frametime;

			if (_purchasers != nullptr)
			{
				if (m_actualProductionTime >= _purchasers->TimeToTravel())
				{
					m_actualState = STALL_PURCHASER_IS_PRESENT;
					*(_state) = SELLING_WINDOW;
					m_actualProductionTime = RESET;
				}
			}

			break;

		case STALL_PURCHASER_IS_PRESENT:
			
			break;

		case STALL_OFFER_HANDLED:
			
			// - Animation de vente des amphores
			// - Vente des amphores en échange de sesterces
			
			if (m_isOfferAccepted == true)
			{
				// We pre-call the destruction of this merchant
				if (_purchasers != nullptr)
				{
					m_isNewMerchantNeeded = true;
				}

				if (m_storehousesCoordinates != nullptr)
				{
					// We exchange the amphoras of wine against sesterce money
					if (_storehouse->GetNumberResourcesStocked(*m_storehousesCoordinates) - 1 >= m_internalImportRessourceCounterSaved - m_ressourceQuantityToSell)
					{
						_money->AddMoney(m_priceAccepted);
						_storehouse->AddNumberResourcesStocked(*m_storehousesCoordinates, -1);
					}
					else
					{
						m_actualState = STALL_WAITING;
					}
				}
			}
			else
			{
				// We ask the destruction of this merchant
				if (_purchasers != nullptr)
				{
					m_isNewMerchantNeeded = true;
				}	
				
				m_actualState = STALL_SEND_REQUEST_PURCHASER;
			}

			break;

		default:

			// ERROR LOG
			m_actualState = STALL_WAITING;
			break;
		}
	}
		
}

void Stalls::UpdateBuildingSprite(unsigned short ***_map)
{
	if (m_isChangingSprite == true)
	{
		if (m_constructionState == BUILT
			&& m_hasBeenBuilt == false
			&& m_isChangingSprite == true)
		{
			m_hasBeenBuilt = true;
			m_isChangingSprite = false;

			_map[FIRST_FLOOR + SPRITE_ID][(int)m_mapPosition.y][(int)m_mapPosition.x] = 18;
		}
	}
}


void Stalls::SavingStallForFile(std::ofstream *_file)
{
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


		_file->write((char *)&m_lifeTime, sizeof(float));
		_file->write((char *)&m_actualProductionTime, sizeof(float));


		_file->write((char *)&m_isChangingSprite, sizeof(bool));
		_file->write((char *)&m_isWorkerThere, sizeof(bool));
		_file->write((char *)&m_isPurchaserThere, sizeof(bool));


		_file->write((char *)&m_priceAccepted, sizeof(int));
		_file->write((char *)&m_internalImportRessourceCounterSaved, sizeof(int));
		_file->write((char *)&m_ressourceQuantityToSell, sizeof(int));
		_file->write((char *)&m_isNewMerchantNeeded, sizeof(bool));
	}
}


void Stalls::LoadingStallFromFile(std::ifstream *_file)
{
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

		_file->read((char *)&m_lifeTime, sizeof(float));
		_file->read((char *)&m_actualProductionTime, sizeof(float));

		_file->read((char *)&m_isChangingSprite, sizeof(bool));
		_file->read((char *)&m_isWorkerThere, sizeof(bool));
		_file->read((char *)&m_isPurchaserThere, sizeof(bool));

		_file->read((char *)&m_priceAccepted, sizeof(int));
		_file->read((char *)&m_internalImportRessourceCounterSaved, sizeof(int));
		_file->read((char *)&m_ressourceQuantityToSell, sizeof(int));
		_file->read((char *)&m_isNewMerchantNeeded, sizeof(bool));
	}
}