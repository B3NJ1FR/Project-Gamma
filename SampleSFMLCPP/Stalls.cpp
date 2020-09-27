#include "Stalls.h"
#include "GameDefinitions.h"



Stalls::Stalls(Buildings *_specificBuildingConcerned)
{	
	this->building = _specificBuildingConcerned;

	this->constructionState = BUILDING_DESTROYED;
	this->hasBeenBuilt = false;
	this->storehousesCoordinates = nullptr;
}


Stalls::~Stalls()
{
}


void Stalls::InitialisationStall(Buildings *_stallBuildingConcerned)
{
	this->building = _stallBuildingConcerned;

	this->constructionState = BUILDING_DESTROYED;
	this->hasBeenBuilt = false;
}



void Stalls::SetConstructionStatus(const enum BuildingStatus &_newStatus)
{
	this->constructionState = _newStatus;
}


void Stalls::SetStatus(const enum StallStatus &_newStatus)
{
	this->actualState = _newStatus;
}


void Stalls::SetIsNewMerchantNeeded(const bool &_newStatus)
{
	this->isNewMerchantNeeded = _newStatus;
}


void Stalls::AddNewBuilding(sf::Vector2f _mapPosition)
{
	// Save the position in map
	this->mapPosition = _mapPosition;

	// Init of the building construction status after being placed on map
	this->constructionState = PLANNED;
	this->actualState = STALL_WAITING;

	// A MODIFIER PAR VALEUR SEUIL
	this->quantitativeThreshold = this->building->GetRessourceQuantityNeeded();

	// A CONFIGURER + RELIER AU BATIMENT
	this->maximalQuantity = 100;
	this->internalRessourceCounter = RESET;

	this->lifeTime = RESET;
	this->actualProductionTime = RESET;

	this->isChangingSprite = false;
	this->hasBeenBuilt = false;
	this->isWorkerThere = false;
	this->isPurchaserThere = false;
	this->isNewMerchantNeeded = false;

	this->internalImportRessourceCounterSaved = RESET;
	this->ressourceQuantityToSell = RESET;
}

void Stalls::AddStorehousePosition(const sf::Vector2f &_mapPosition)
{
	if (this->storehousesCoordinates == nullptr)
	{
		this->numberStorehousesCoordinates = 1;

		this->storehousesCoordinates = new sf::Vector2f;
		*this->storehousesCoordinates = _mapPosition;

		std::cout << this->storehousesCoordinates[0].x << " " << this->storehousesCoordinates[0].y << std::endl;
	}
	else
	{
		// We create a new list
		sf::Vector2f *newListOfCoordinates = new sf::Vector2f[this->numberStorehousesCoordinates + 1];

		// We copy the old data
		for (int i = 0; i < this->numberStorehousesCoordinates; i++)
		{
			newListOfCoordinates[i] = this->storehousesCoordinates[i];
		}

		// We add the new position at the end of the list
		newListOfCoordinates[this->numberStorehousesCoordinates + 1] = _mapPosition;

		this->numberStorehousesCoordinates += 1;

		// We delete the actual memory location of the "storehousesCoordinates" variable, and add the adress of the new list
		delete this->storehousesCoordinates;
		this->storehousesCoordinates = nullptr;
		this->storehousesCoordinates = newListOfCoordinates;


		for (int i = 0; i < this->numberStorehousesCoordinates; i++)
		{
			std::cout << this->storehousesCoordinates[i].x << " " << this->storehousesCoordinates[i].y << std::endl;
		}
	}
}


void Stalls::SetRessourceQuantityToSell(const int &_quantity)
{
	this->ressourceQuantityToSell = _quantity;
}


void Stalls::PickUpPriceAccepted(const int &_price)
{
	this->priceAccepted = _price;
}


bool Stalls::DestroyedBuildingSelected(const sf::Vector2f &_mapPosition)
{
	if (this->mapPosition == _mapPosition)
	{
		// Reset of the building construction status after being placed on map
		this->constructionState = PLANNED;
		this->actualState = STALL_WAITING;

		// A MODIFIER PAR VALEUR SEUIL
		this->quantitativeThreshold = this->building->GetRessourceQuantityNeeded();

		// A CONFIGURER + RELIER AU BATIMENT
		this->maximalQuantity = 100;
		this->internalRessourceCounter = RESET;

		this->lifeTime = RESET;
		this->actualProductionTime = RESET;

		this->isChangingSprite = false;
		this->hasBeenBuilt = false;
		this->isWorkerThere = false;
		this->isPurchaserThere = false;
		this->isNewMerchantNeeded = false;

		this->internalImportRessourceCounterSaved = RESET;
		this->ressourceQuantityToSell = RESET;

		return true;
	}
	else
	{
		return false;
	}

}


bool Stalls::ConfirmPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates, const bool &_thisIsAWorker)
{
	if (_mapPosition.x <= this->mapPosition.x
		&& _mapPosition.x >= this->mapPosition.x - this->building->GetSize().x
		&& _mapPosition.y <= this->mapPosition.y
		&& _mapPosition.y >= this->mapPosition.y - this->building->GetSize().y)
	{
		if (_isPreciseCoordinates)
		{
			if (_mapPosition == this->mapPosition)
			{
				if (_thisIsAWorker)
				{
					this->isWorkerThere = true;
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
				this->isWorkerThere = true;
			}

			return true;
		}
	}
	else
	{
		return false;
	}
}


bool Stalls::GetWorkerIsThere(const sf::Vector2f &_mapPosition)
{
	// We verify if the player location is between the origin and the max size of the building concerned
	if (_mapPosition == this->mapPosition)
	{
		return this->isWorkerThere;
	}
	else
	{
		return false;
	}			
}





enum StallStatus Stalls::GetStatus()
{
	return this->actualState;
}

enum BuildingStatus Stalls::GetConstructionStatus()
{
	return this->constructionState;
}

int Stalls::GetActualRessourcesStocked()
{
	return this->internalRessourceCounter;
}


bool Stalls::GetIsNewMerchantNeeded()
{
	return this->isNewMerchantNeeded;
}


sf::Vector2i Stalls::GetMapPosition()
{
	return (sf::Vector2i)this->mapPosition;
}







void Stalls::UpdateBuildingConstruction(const float &_frametime)
{
	switch (this->constructionState)
	{
	case PLANNED:
		// Plannification of the building construction

		// If we've the workers accredited to the building construction, we can pass the state to CONSTRUCTION
		//std::cout << "Planned ! " << std::endl;

		// TEMPORARY
		if (this->isWorkerThere == true)
		{
			std::cout << "Building launched ! " << this->lifeTime << " " << this->building->GetConstructionTimeCost() << std::endl;
			this->constructionState = CONSTRUCTION;
			this->lifeTime = RESET;
		}

		break;
	case CONSTRUCTION:

		if (this->isWorkerThere == true)
		{
			this->lifeTime += _frametime;
			this->isWorkerThere = false;
		}

		// If the building life is higher than the construction time, we launch it's growthing
		if (this->lifeTime >= this->building->GetConstructionTimeCost())
		{
			std::cout << "Building built ! " << this->lifeTime << " " << this->building->GetConstructionTimeCost() << std::endl;
			this->constructionState = BUILT;
			this->isChangingSprite = true;
		}

		break;
	case BUILT:
		if (this->hasBeenBuilt == false)
		{
			this->isChangingSprite = true;
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




void Stalls::UpdateInternalCycles(class Money *_money, enum GameState *_state, const float &_frametime, Ressources *_ressourceSent, Purchasers *_purchasers, Storehouse *_storehouse)
{
	if (this->constructionState == BUILT)
	{
		switch (this->actualState)
		{
		case STALL_WAITING:
			
			if (this->storehousesCoordinates != nullptr)
			{
				for (int i = 0; i < this->numberStorehousesCoordinates; i++)
				{
					std::cout << _storehouse->GetNumberResourcesStocked(this->storehousesCoordinates[i]) << std::endl;

					if (_storehouse->GetNumberResourcesStocked(this->storehousesCoordinates[i]) >= this->quantitativeThreshold)
					{
						this->isNewMerchantNeeded = true;

						this->internalImportRessourceCounterSaved = _storehouse->GetNumberResourcesStocked(this->storehousesCoordinates[i]);

						this->actualState = STALL_SEND_REQUEST_PURCHASER;
					}
				}
				
			}
			
			break;

		case STALL_SEND_REQUEST_PURCHASER:
			
			this->actualProductionTime += _frametime;

			if (_purchasers != nullptr)
			{
				if (this->actualProductionTime >= _purchasers->TimeToTravel())
				{
					this->actualState = STALL_PURCHASER_IS_PRESENT;
					*(_state) = SELLING_WINDOW;
					this->actualProductionTime = RESET;
				}
			}

			break;

		case STALL_PURCHASER_IS_PRESENT:
			
			break;

		case STALL_OFFER_HANDLED:
			
			// - Animation de vente des amphores
			// - Vente des amphores en �change de sesterces
			
			if (this->isOfferAccepted == true)
			{
				// We pre-call the destruction of this merchant
				if (_purchasers != nullptr)
				{
					this->isNewMerchantNeeded = true;
				}

				if (this->storehousesCoordinates != nullptr)
				{
					// We exchange the amphoras of wine against sesterce money
					if (_storehouse->GetNumberResourcesStocked(*this->storehousesCoordinates) - 1 >= this->internalImportRessourceCounterSaved - this->ressourceQuantityToSell)
					{
						_money->AddMoney(this->priceAccepted);
						_storehouse->AddNumberResourcesStocked(*this->storehousesCoordinates, -1);
					}
					else
					{
						this->actualState = STALL_WAITING;
					}
				}
			}
			else
			{
				// We ask the destruction of this merchant
				if (_purchasers != nullptr)
				{
					this->isNewMerchantNeeded = true;
				}	
				
				this->actualState = STALL_SEND_REQUEST_PURCHASER;
			}

			break;

		default:

			// ERROR LOG
			this->actualState = STALL_WAITING;
			break;
		}
	}
		
}

void Stalls::UpdateBuildingSprite(unsigned short ***_map)
{
	if (this->isChangingSprite == true)
	{
		if (this->constructionState == BUILT
			&& this->hasBeenBuilt == false
			&& this->isChangingSprite == true)
		{
			this->hasBeenBuilt = true;
			this->isChangingSprite = false;

			_map[FIRST_FLOOR + SPRITE_ID][(int)this->mapPosition.y][(int)this->mapPosition.x] = 18;
		}
	}
}


void Stalls::SavingStallForFile(std::ofstream *_file)
{
	_file->write((char *)&this->constructionState, sizeof(enum BuildingStatus));

	if (this->constructionState != BUILDING_DESTROYED)
	{
		// Save the map position
		_file->write((char *)&this->mapPosition.x, sizeof(float));
		_file->write((char *)&this->mapPosition.y, sizeof(float));

		_file->write((char *)&this->actualState, sizeof(enum StallStatus));


		_file->write((char *)&this->quantitativeThreshold, sizeof(int));
		_file->write((char *)&this->maximalQuantity, sizeof(int));
		_file->write((char *)&this->internalRessourceCounter, sizeof(int));


		_file->write((char *)&this->lifeTime, sizeof(float));
		_file->write((char *)&this->actualProductionTime, sizeof(float));


		_file->write((char *)&this->isChangingSprite, sizeof(bool));
		_file->write((char *)&this->isWorkerThere, sizeof(bool));
		_file->write((char *)&this->isPurchaserThere, sizeof(bool));


		_file->write((char *)&this->priceAccepted, sizeof(int));
		_file->write((char *)&this->internalImportRessourceCounterSaved, sizeof(int));
		_file->write((char *)&this->ressourceQuantityToSell, sizeof(int));
		_file->write((char *)&this->isNewMerchantNeeded, sizeof(bool));
	}
}


void Stalls::LoadingStallFromFile(std::ifstream *_file)
{
	_file->read((char *)&this->constructionState, sizeof(enum BuildingStatus));

	if (this->constructionState != BUILDING_DESTROYED)
	{
		// Save the map position
		_file->read((char *)&this->mapPosition.x, sizeof(float));
		_file->read((char *)&this->mapPosition.y, sizeof(float));
			  
		_file->read((char *)&this->actualState, sizeof(enum StallStatus));

			 
		_file->read((char *)&this->quantitativeThreshold, sizeof(int));
		_file->read((char *)&this->maximalQuantity, sizeof(int));
		_file->read((char *)&this->internalRessourceCounter, sizeof(int));
			  
			  
		_file->read((char *)&this->lifeTime, sizeof(float));
		_file->read((char *)&this->actualProductionTime, sizeof(float));
			   
			   
		_file->read((char *)&this->isChangingSprite, sizeof(bool));
		_file->read((char *)&this->isWorkerThere, sizeof(bool));
		_file->read((char *)&this->isPurchaserThere, sizeof(bool));
			   
			   
		_file->read((char *)&this->priceAccepted, sizeof(int));
		_file->read((char *)&this->internalImportRessourceCounterSaved, sizeof(int));
		_file->read((char *)&this->ressourceQuantityToSell, sizeof(int));
		_file->read((char *)&this->isNewMerchantNeeded, sizeof(bool));
	}
}