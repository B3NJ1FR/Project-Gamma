#include "Stalls.h"
#include "GameDefinitions.h"



Stalls::Stalls(Buildings *_specificBuildingConcerned)
{	
	this->building = _specificBuildingConcerned;

	this->constructionState = BUILDING_DESTROYED;
	this->hasBeenBuilt = false;
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
	this->actualState = STALL_READY_TO_WORKS;

	// A MODIFIER PAR VALEUR SEUIL
	this->quantitativeThreshold = this->building->GetRessourceQuantityNeeded();

	// A CONFIGURER + RELIER AU BATIMENT
	this->maximalQuantity = 100;
	this->internalImportRessourceCounter = RESET;

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
		this->actualState = STALL_READY_TO_WORKS;

		// A MODIFIER PAR VALEUR SEUIL
		this->quantitativeThreshold = this->building->GetRessourceQuantityNeeded();

		// A CONFIGURER + RELIER AU BATIMENT
		this->maximalQuantity = 100;
		this->internalImportRessourceCounter = RESET;

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


bool Stalls::ConfirmPresenceAtWorkerPosition(const sf::Vector2f &_mapPosition)
{
	// If the building has produced the ressources, we manage it
	if (this->mapPosition == _mapPosition)
	{
		this->isWorkerThere = true;
		return true;
	}
	else
	{
		this->isWorkerThere = false;
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
	return this->internalImportRessourceCounter;
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
	if (this->constructionState != BUILT)
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

}




void Stalls::UpdateInternalCycles(class Money *_money, enum GameState *_state, const float &_frametime, Ressources *_ressourceSent, Purchasers *_purchasers)
{
	if (this->constructionState == BUILT)
	{
		switch (this->actualState)
		{
		case STALL_READY_TO_WORKS:
			
			if (_ressourceSent->GetQuantityOwned() > 0
				&& internalImportRessourceCounter >= 0)
			{
				this->actualState = STALL_FILLING;
			}
			
			break;

		case STALL_FILLING:
			
			if (_ressourceSent->GetQuantityOwned() - 1 >= 0
				&& this->internalImportRessourceCounter + 1 <= this->maximalQuantity)
			{
				_ressourceSent->SubtractQuantityOwned(1);
				this->internalImportRessourceCounter += 1;
			}


			if (this->internalImportRessourceCounter >= this->quantitativeThreshold
				&& (_ressourceSent->GetQuantityOwned() == 0
					|| this->internalImportRessourceCounter == this->maximalQuantity))
			{
				this->isNewMerchantNeeded = true;

				this->internalImportRessourceCounterSaved = this->internalImportRessourceCounter;

				this->actualState = STALL_SEND_REQUEST_PURCHASER;
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
			// - Vente des amphores en échange de sesterces
			
			if (this->isOfferAccepted == true)
			{
				// We pre-call the destruction of this merchant
				if (_purchasers != nullptr)
				{
					this->isNewMerchantNeeded = true;
				}

				// We exchange the amphoras of wine against sesterce money
				if (this->internalImportRessourceCounter - 1 >= this->internalImportRessourceCounterSaved - this->ressourceQuantityToSell)
				{
					_money->AddMoney(this->priceAccepted);
					this->internalImportRessourceCounter -= 1;
				}
				else
				{
					this->actualState = STALL_READY_TO_WORKS;
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
			this->actualState = STALL_READY_TO_WORKS;
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
		_file->write((char *)&this->internalImportRessourceCounter, sizeof(int));


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
		_file->read((char *)&this->internalImportRessourceCounter, sizeof(int));
			  
			  
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