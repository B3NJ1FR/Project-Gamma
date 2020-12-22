#include "Stalls.h"
//#include "GameDefinitions.h"
#include "Money.h"
#include "Map.h"



Stalls::Stalls(Buildings *_specificBuildingConcerned)
{	
	this->m_building = _specificBuildingConcerned;

	this->m_constructionState = BUILDING_DESTROYED;
	this->m_hasBeenBuilt = false;
	this->m_storehousesCoordinates = nullptr;
}


Stalls::~Stalls()
{
}


void Stalls::InitialisationStall(Buildings *_stallBuildingConcerned)
{
	this->m_building = _stallBuildingConcerned;

	this->m_constructionState = BUILDING_DESTROYED;
	this->m_hasBeenBuilt = false;
}



void Stalls::SetConstructionStatus(const enum BuildingStatus &_newStatus)
{
	this->m_constructionState = _newStatus;
}


void Stalls::SetStatus(const enum StallStatus &_newStatus)
{
	this->m_actualState = _newStatus;
}


void Stalls::SetIsNewMerchantNeeded(const bool &_newStatus)
{
	this->m_isNewMerchantNeeded = _newStatus;
}


void Stalls::AddNewBuilding(sf::Vector2f _mapPosition)
{
	// Save the position in map
	this->m_mapPosition = _mapPosition;

	// Init of the building construction status after being placed on map
	this->m_constructionState = PLANNED;
	this->m_actualState = STALL_WAITING;

	// A MODIFIER PAR VALEUR SEUIL
	this->m_quantitativeThreshold = this->m_building->GetRessourceQuantityNeeded();

	// A CONFIGURER + RELIER AU BATIMENT
	this->m_maximalQuantity = 100;
	this->m_internalRessourceCounter = RESET;

	this->m_lifeTime = RESET;
	this->m_actualProductionTime = RESET;

	this->m_isChangingSprite = false;
	this->m_hasBeenBuilt = false;
	this->m_isWorkerThere = false;
	this->m_isPurchaserThere = false;
	this->m_isNewMerchantNeeded = false;

	this->m_internalImportRessourceCounterSaved = RESET;
	this->m_ressourceQuantityToSell = RESET;
}

void Stalls::AddStorehousePosition(const sf::Vector2f &_mapPosition)
{
	if (this->m_storehousesCoordinates == nullptr)
	{
		this->m_numberStorehousesCoordinates = 1;

		this->m_storehousesCoordinates = new sf::Vector2f;
		*this->m_storehousesCoordinates = _mapPosition;

		std::cout << this->m_storehousesCoordinates[0].x << " " << this->m_storehousesCoordinates[0].y << std::endl;
	}
	else
	{
		// We create a new list
		sf::Vector2f *newListOfCoordinates = new sf::Vector2f[this->m_numberStorehousesCoordinates + 1];

		// We copy the old data
		for (int i = 0; i < this->m_numberStorehousesCoordinates; i++)
		{
			newListOfCoordinates[i] = this->m_storehousesCoordinates[i];
		}

		// We add the new position at the end of the list
		newListOfCoordinates[this->m_numberStorehousesCoordinates + 1] = _mapPosition;

		this->m_numberStorehousesCoordinates += 1;

		// We delete the actual memory location of the "storehousesCoordinates" variable, and add the adress of the new list
		delete this->m_storehousesCoordinates;
		this->m_storehousesCoordinates = nullptr;
		this->m_storehousesCoordinates = newListOfCoordinates;


		for (int i = 0; i < this->m_numberStorehousesCoordinates; i++)
		{
			std::cout << this->m_storehousesCoordinates[i].x << " " << this->m_storehousesCoordinates[i].y << std::endl;
		}
	}
}


void Stalls::SetRessourceQuantityToSell(const int &_quantity)
{
	this->m_ressourceQuantityToSell = _quantity;
}


void Stalls::PickUpPriceAccepted(const int &_price)
{
	this->m_priceAccepted = _price;
}


bool Stalls::DestroyedBuildingSelected(const sf::Vector2f &_mapPosition)
{
	if (this->m_mapPosition == _mapPosition)
	{
		// Reset of the building construction status after being placed on map
		this->m_constructionState = PLANNED;
		this->m_actualState = STALL_WAITING;

		// A MODIFIER PAR VALEUR SEUIL
		this->m_quantitativeThreshold = this->m_building->GetRessourceQuantityNeeded();

		// A CONFIGURER + RELIER AU BATIMENT
		this->m_maximalQuantity = 100;
		this->m_internalRessourceCounter = RESET;

		this->m_lifeTime = RESET;
		this->m_actualProductionTime = RESET;

		this->m_isChangingSprite = false;
		this->m_hasBeenBuilt = false;
		this->m_isWorkerThere = false;
		this->m_isPurchaserThere = false;
		this->m_isNewMerchantNeeded = false;

		this->m_internalImportRessourceCounterSaved = RESET;
		this->m_ressourceQuantityToSell = RESET;

		return true;
	}
	else
	{
		return false;
	}

}


bool Stalls::ConfirmPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates, const bool &_thisIsAWorker)
{
	if (_mapPosition.x <= this->m_mapPosition.x
		&& _mapPosition.x >= this->m_mapPosition.x - this->m_building->GetSize().x
		&& _mapPosition.y <= this->m_mapPosition.y
		&& _mapPosition.y >= this->m_mapPosition.y - this->m_building->GetSize().y)
	{
		if (_isPreciseCoordinates)
		{
			if (_mapPosition == this->m_mapPosition)
			{
				if (_thisIsAWorker)
				{
					this->m_isWorkerThere = true;
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
				this->m_isWorkerThere = true;
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
	if (_mapPosition == this->m_mapPosition)
	{
		return this->m_isWorkerThere;
	}
	else
	{
		return false;
	}			
}





enum StallStatus Stalls::GetStatus()
{
	return this->m_actualState;
}

enum BuildingStatus Stalls::GetConstructionStatus()
{
	return this->m_constructionState;
}

int Stalls::GetActualRessourcesStocked()
{
	return this->m_internalRessourceCounter;
}


bool Stalls::GetIsNewMerchantNeeded()
{
	return this->m_isNewMerchantNeeded;
}


sf::Vector2i Stalls::GetMapPosition()
{
	return (sf::Vector2i)this->m_mapPosition;
}







void Stalls::UpdateBuildingConstruction(const float &_frametime)
{
	switch (this->m_constructionState)
	{
	case PLANNED:
		// Plannification of the building construction

		// If we've the workers accredited to the building construction, we can pass the state to CONSTRUCTION
		//std::cout << "Planned ! " << std::endl;

		// TEMPORARY
		if (this->m_isWorkerThere == true)
		{
			std::cout << "Building launched ! " << this->m_lifeTime << " " << this->m_building->GetConstructionTimeCost() << std::endl;
			this->m_constructionState = CONSTRUCTION;
			this->m_lifeTime = RESET;
		}

		break;
	case CONSTRUCTION:

		if (this->m_isWorkerThere == true)
		{
			this->m_lifeTime += _frametime;
			this->m_isWorkerThere = false;
		}

		// If the building life is higher than the construction time, we launch it's growthing
		if (this->m_lifeTime >= this->m_building->GetConstructionTimeCost())
		{
			std::cout << "Building built ! " << this->m_lifeTime << " " << this->m_building->GetConstructionTimeCost() << std::endl;
			this->m_constructionState = BUILT;
			this->m_isChangingSprite = true;
		}

		break;
	case BUILT:
		if (this->m_hasBeenBuilt == false)
		{
			this->m_isChangingSprite = true;
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
	if (this->m_constructionState == BUILT)
	{
		switch (this->m_actualState)
		{
		case STALL_WAITING:
			
			if (this->m_storehousesCoordinates != nullptr)
			{
				for (int i = 0; i < this->m_numberStorehousesCoordinates; i++)
				{
					std::cout << _storehouse->GetNumberResourcesStocked(this->m_storehousesCoordinates[i]) << std::endl;

					if (_storehouse->GetNumberResourcesStocked(this->m_storehousesCoordinates[i]) >= this->m_quantitativeThreshold)
					{
						this->m_isNewMerchantNeeded = true;

						this->m_internalImportRessourceCounterSaved = _storehouse->GetNumberResourcesStocked(this->m_storehousesCoordinates[i]);

						this->m_actualState = STALL_SEND_REQUEST_PURCHASER;
					}
				}
				
			}
			
			break;

		case STALL_SEND_REQUEST_PURCHASER:
			
			this->m_actualProductionTime += _frametime;

			if (_purchasers != nullptr)
			{
				if (this->m_actualProductionTime >= _purchasers->TimeToTravel())
				{
					this->m_actualState = STALL_PURCHASER_IS_PRESENT;
					//*(_state) = SELLING_WINDOW;
					this->m_actualProductionTime = RESET;
				}
			}

			break;

		case STALL_PURCHASER_IS_PRESENT:
			
			break;

		case STALL_OFFER_HANDLED:
			
			// - Animation de vente des amphores
			// - Vente des amphores en échange de sesterces
			
			if (this->m_isOfferAccepted == true)
			{
				// We pre-call the destruction of this merchant
				if (_purchasers != nullptr)
				{
					this->m_isNewMerchantNeeded = true;
				}

				if (this->m_storehousesCoordinates != nullptr)
				{
					// We exchange the amphoras of wine against sesterce money
					if (_storehouse->GetNumberResourcesStocked(*this->m_storehousesCoordinates) - 1 >= this->m_internalImportRessourceCounterSaved - this->m_ressourceQuantityToSell)
					{
						_money->AddMoney(this->m_priceAccepted);
						_storehouse->AddNumberResourcesStocked(*this->m_storehousesCoordinates, -1);
					}
					else
					{
						this->m_actualState = STALL_WAITING;
					}
				}
			}
			else
			{
				// We ask the destruction of this merchant
				if (_purchasers != nullptr)
				{
					this->m_isNewMerchantNeeded = true;
				}	
				
				this->m_actualState = STALL_SEND_REQUEST_PURCHASER;
			}

			break;

		default:

			// ERROR LOG
			this->m_actualState = STALL_WAITING;
			break;
		}
	}
		
}

void Stalls::UpdateBuildingSprite(unsigned short ***_map)
{
	if (this->m_isChangingSprite == true)
	{
		if (this->m_constructionState == BUILT
			&& this->m_hasBeenBuilt == false
			&& this->m_isChangingSprite == true)
		{
			this->m_hasBeenBuilt = true;
			this->m_isChangingSprite = false;

			_map[FIRST_FLOOR + SPRITE_ID][(int)this->m_mapPosition.y][(int)this->m_mapPosition.x] = 18;
		}
	}
}


void Stalls::SavingStallForFile(std::ofstream *_file)
{
	_file->write((char *)&this->m_constructionState, sizeof(enum BuildingStatus));

	if (this->m_constructionState != BUILDING_DESTROYED)
	{
		// Save the map position
		_file->write((char *)&this->m_mapPosition.x, sizeof(float));
		_file->write((char *)&this->m_mapPosition.y, sizeof(float));

		_file->write((char *)&this->m_actualState, sizeof(enum StallStatus));


		_file->write((char *)&this->m_quantitativeThreshold, sizeof(int));
		_file->write((char *)&this->m_maximalQuantity, sizeof(int));
		_file->write((char *)&this->m_internalRessourceCounter, sizeof(int));


		_file->write((char *)&this->m_lifeTime, sizeof(float));
		_file->write((char *)&this->m_actualProductionTime, sizeof(float));


		_file->write((char *)&this->m_isChangingSprite, sizeof(bool));
		_file->write((char *)&this->m_isWorkerThere, sizeof(bool));
		_file->write((char *)&this->m_isPurchaserThere, sizeof(bool));


		_file->write((char *)&this->m_priceAccepted, sizeof(int));
		_file->write((char *)&this->m_internalImportRessourceCounterSaved, sizeof(int));
		_file->write((char *)&this->m_ressourceQuantityToSell, sizeof(int));
		_file->write((char *)&this->m_isNewMerchantNeeded, sizeof(bool));
	}
}


void Stalls::LoadingStallFromFile(std::ifstream *_file)
{
	_file->read((char *)&this->m_constructionState, sizeof(enum BuildingStatus));

	if (this->m_constructionState != BUILDING_DESTROYED)
	{
		// Save the map position
		_file->read((char *)&this->m_mapPosition.x, sizeof(float));
		_file->read((char *)&this->m_mapPosition.y, sizeof(float));
			  
		_file->read((char *)&this->m_actualState, sizeof(enum StallStatus));

			 
		_file->read((char *)&this->m_quantitativeThreshold, sizeof(int));
		_file->read((char *)&this->m_maximalQuantity, sizeof(int));
		_file->read((char *)&this->m_internalRessourceCounter, sizeof(int));
			  
			  
		_file->read((char *)&this->m_lifeTime, sizeof(float));
		_file->read((char *)&this->m_actualProductionTime, sizeof(float));
			   
			   
		_file->read((char *)&this->m_isChangingSprite, sizeof(bool));
		_file->read((char *)&this->m_isWorkerThere, sizeof(bool));
		_file->read((char *)&this->m_isPurchaserThere, sizeof(bool));
			   
			   
		_file->read((char *)&this->m_priceAccepted, sizeof(int));
		_file->read((char *)&this->m_internalImportRessourceCounterSaved, sizeof(int));
		_file->read((char *)&this->m_ressourceQuantityToSell, sizeof(int));
		_file->read((char *)&this->m_isNewMerchantNeeded, sizeof(bool));
	}
}