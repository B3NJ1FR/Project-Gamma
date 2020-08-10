#include "Stalls.h"
#include "GameDefinitions.h"



Stalls::Stalls(Buildings *_specificBuildingConcerned)
{	
	this->building = _specificBuildingConcerned;
}


Stalls::~Stalls()
{
}



void Stalls::SetStatus(const enum StallStatus &_newStatus)
{
	this->actualState = _newStatus;
}

enum StallStatus Stalls::GetStatus()
{
	return this->actualState;
}

int Stalls::GetActualRessourcesStocked()
{
	return this->internalImportRessourceCounter;
}


bool Stalls::GetIsNewMerchantNeeded()
{
	return this->isNewMerchantNeeded;
}


void Stalls::SetIsNewMerchantNeeded(const bool &_newStatus)
{
	this->isNewMerchantNeeded = _newStatus;
}


void Stalls::InitialisationStall(Buildings *_stallBuildingConcerned)
{
	this->building = _stallBuildingConcerned;
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
	this->isWorkerThere = false;
	this->isPurchaserThere = false;
	this->isNewMerchantNeeded = false;

	this->internalImportRessourceCounterSaved = RESET;
	this->ressourceQuantityToSell = RESET;
}

void Stalls::PickUpPriceAccepted(const int &_price)
{
	this->priceAccepted = _price;
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
			
			// Importer depuis un fichier les caractéristiques des vendeurs
			//
			// - origine / provenance
			// - min / max -> prix /u
			// - min / max -> quantité
			// - min / max -> temps de déplacement
			// - décalage entre plus l'on vend, moins on gagne
			// - si une offre est refusée, la prochaine est un peu plus cher


			this->actualProductionTime += _frametime;

			if (this->actualProductionTime >= _purchasers->TimeToTravel())
			{
				this->actualState = STALL_PURCHASER_IS_PRESENT;
				*(_state) = SELLING_WINDOW;
				this->actualProductionTime = RESET;
			}

			break;

		case STALL_PURCHASER_IS_PRESENT:

			// - Changer state du jeu
			// - Afficher une notif
			// - Afficher une fenetre avec infos :
			//		- origine / provenance
			//		- prix / u
			//		- un scroll ou le joueur choisi la quantité qu'il veut vendre (cloisonné entre le min et max de l'acheteur)
			//		- bouton Accepter Vente
			//		- bouton Refuser Vente
			



			//if (this->internalImportRessourceCounter > 0)
			//{
			//	_ressourceProduced->AddQuantityOwned(1);
			//	this->internalImportRessourceCounter -= 1;
			//}
			//else if (this->internalImportRessourceCounter == 0)
			//{
			//	this->actualState = BUILDING_OFFER_ACCEPTED;
			//	this->internalImportRessourceCounter = RESET;
			//}
			//else if (this->internalImportRessourceCounter < 0)
			//{
			//	// ERROR LOG
			//	this->actualState = BUILDING_OFFER_ACCEPTED;
			//	this->internalImportRessourceCounter = RESET;
			//}

			break;

		case STALL_OFFER_HANDLED:
			
			// - Animation de vente des amphores
			// - Vente des amphores en échange de sesterces
			
			if (this->isOfferAccepted == true)
			{
				if (_purchasers != nullptr)
				{
					this->isNewMerchantNeeded = true;
				}

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

			break;
		case BUILDING_NOT_MAINTAINED:
			break;

		default:
			// ERROR LOG
			break;
		}
	}
		
}


//void Stalls::UpdateBuildingSprite(unsigned short ***_map)
//{
//	if (this->list != nullptr)
//	{
//		if (this->list->first != nullptr)
//		{
//			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				/*if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite == true)
//				{
//					switch (((SpecificsBuildings::sBuildingData *)currentElement->data)->generalState)
//					{
//					case PLANTED:
//						break;
//					case THREE_YEARS_GROWTHING:
//						_map[3 + 2][(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y]
//							[(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x] = 8;
//						break;
//					case READY_TO_PRODUCE:
//						_map[3 + 2][(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y]
//							[(int)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x] = 9;
//						break;
//					case NOT_MAINTAINED:
//						break;
//					case TOO_OLD:
//						break;
//					default:
//						break;
//					}
//
//					((SpecificsBuildings::sBuildingData *)currentElement->data)->isChangingSprite = false;
//				}*/
//			}
//		}
//	}
//}
//
//
//void Stalls::UpdateBuildingProduction(Ressources *_ressource)
//{
//	if (this->list != nullptr)
//	{
//		if (this->list->first != nullptr)
//		{
//			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				// If the building has produced the ressources, we manage it
//				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
//				{
//					// Add quantity produced to the ressource targeted
//					_ressource->AddQuantityOwned(this->building->GetRessourceQuantityProduced());
//
//					// Launch the feedback animation of producing
//
//
//					((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = false;
//				}
//			}
//		}
//	}
//}

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


//bool Stalls::CheckSpecificBuildingHasProducedRessource(const sf::Vector2f &_mapPosition)
//{
//	if (this->list != nullptr)
//	{
//		if (this->list->first != nullptr)
//		{
//			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition)
//				{
//					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
//					{
//						return true;
//					}
//					else
//					{
//						return false;
//					}
//				}
//			}
//
//			return false;
//
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else
//	{
//		return false;
//	}
//}
//
//
//int Stalls::SpecificsBuildingsSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime)
//{
//	if (this->list != nullptr)
//	{
//		if (this->list->first != nullptr)
//		{
//			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				// If the building has produced the ressources, we manage it
//				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition)
//				{
//					if (((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced == true)
//					{
//						((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime += _frametime;
//
//						if (((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime >= this->building->GetPickupingTimeCost())
//						{
//							// Launch the feedback animation of producing
//
//							((SpecificsBuildings::sBuildingData *)currentElement->data)->isProduced = false;
//
//							((SpecificsBuildings::sBuildingData *)currentElement->data)->secondaryTime = RESET;
//
//							return this->building->GetRessourceQuantityProduced();
//						}
//						else
//						{
//							return 0;
//						}
//					}
//					else
//					{
//						return 0;
//					}
//				}
//
//			}
//
//			return 0;
//
//		}
//		else
//		{
//			return 0;
//		}
//	}
//	else
//	{
//		return 0;
//	}
//}
//
//
//sf::Vector2i Stalls::SpecificsBuildingsFindNearestBuilding(const sf::Vector2f &_mapPosition)
//{
//	if (this->list != nullptr)
//	{
//		if (this->list->first != nullptr)
//		{
//			float lastLowerDistance(RESET);
//			sf::Vector2i buildingPosition = { RESET, RESET };
//
//			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				if (currentElement == this->list->first)
//				{
//					float distance = DistanceFormula(_mapPosition, ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition);
//
//					lastLowerDistance = distance;
//					buildingPosition = (sf::Vector2i)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition;
//				}
//				else
//				{
//					float distance = DistanceFormula(_mapPosition, ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition);
//
//					if (distance < lastLowerDistance)
//					{
//						lastLowerDistance = distance;
//						buildingPosition = (sf::Vector2i)((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition;
//					}
//					else if (distance == lastLowerDistance)
//					{
//
//					}
//					else
//					{
//
//					}
//
//				}
//			}
//
//			return buildingPosition;
//
//		}
//	}
//}
//
//
//bool Stalls::DestroyedBuildingSelected(const sf::Vector2f &_mapPosition)
//{
//	if (this->list != nullptr)
//	{
//		if (this->list->first != nullptr)
//		{
//			int positionCounter(1);
//			bool isBuildingFind(false);
//
//			for (LinkedListClass::sElement *currentElement = this->list->first; currentElement != NULL; currentElement = currentElement->next)
//			{
//				//std::cout << "Map : " << positionCounter << "/" << this->list->size << " -> "<< ((Vines::sVines *)currentElement->data)->mapPosition.x << " " << ((Vines::sVines *)currentElement->data)->mapPosition.y << std::endl;
//
//				// If the building position is identical to which send, we save his position in the linked list
//				if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition == _mapPosition
//					&& isBuildingFind == false)
//				{
//					isBuildingFind = true;
//				}
//
//				if (isBuildingFind == false)
//				{
//					positionCounter++;
//				}
//			}
//
//			//std::cout << std::endl;
//
//			// After having saved the building's position, we ask to destroy it
//			if (isBuildingFind == true)
//			{
//				RemoveElementsOfLinkedList(this->list, true, positionCounter);
//
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else
//	{
//		return false;
//	}
//}


void Stalls::SetRessourceQuantityToSell(const int &_quantity)
{
	this->ressourceQuantityToSell = _quantity;
}