#include "Stalls.h"
#include "GameDefinitions.h"



Stalls::Stalls(sf::Font *_font)
{
	this->sellingWindowBackground = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_background.png", 1);

	this->sellingWindowAcceptButton = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_acceptButton.png", 1);
	this->sellingWindowRejectButton = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_refuseButton.png", 1);

	this->sellingWindowScrollButton = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_ScrollButton.png", 1);
	this->sellingWindowScrollLine = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_ScrollLine.png", 1);
	
	LoadTextString(&this->sellingWindowProvenance, "Nemausus", _font, 50, sf::Color::Black, sf::Vector2f(1920/2, 1080/2 - 150));

	LoadTextString(&this->sellingWindowPrice[0], "", _font, 30, sf::Color::White, sf::Vector2f(0, 85));
	LoadTextString(&this->sellingWindowPrice[1], "", _font, 30, sf::Color::White, sf::Vector2f(250, 85));
	LoadTextString(&this->sellingWindowPrice[2], "", _font, 30, sf::Color::White, sf::Vector2f(250, 85));

	LoadTextString(&this->sellingWindowRessourceQuantity[0], "", _font, 30, sf::Color::White, sf::Vector2f(0, 85));
	LoadTextString(&this->sellingWindowRessourceQuantity[1], "", _font, 30, sf::Color::White, sf::Vector2f(250, 85));
	LoadTextString(&this->sellingWindowRessourceQuantity[2], "", _font, 30, sf::Color::White, sf::Vector2f(250, 85));
}


Stalls::~Stalls()
{
}



void Stalls::InitialisationStall(Buildings *_stallBuildingConcerned)
{
	/*std::cout << "List before : " << this->list << std::endl;

	this->list = LinkedListInitialisation();

	std::cout << "List " << this->list << " Size : " << this->list->size << " Real First : " << this->list->first << " & Last : " << this->list->last << std::endl << std::endl;*/

	this->building = _stallBuildingConcerned;
}

void Stalls::DisplaySellingWindow(struct Game *_game)
{
	// Display of the background
	BlitSprite(this->sellingWindowBackground, 1920 / 2, 1080 / 2, 0, *_game->window);

	BlitSprite(this->sellingWindowAcceptButton, 1920 / 2 + 200, 1080 / 2 + 125, 0, *_game->window);
	BlitSprite(this->sellingWindowRejectButton, 1920 / 2 - 200, 1080 / 2 + 125, 0, *_game->window);

	BlitSprite(this->sellingWindowScrollLine, 1920 / 2, 1080 / 2, 0, *_game->window);
	BlitSprite(this->sellingWindowScrollButton, 1920 / 2, 1080 / 2, 0, *_game->window);


	BlitString(this->sellingWindowProvenance, *_game->window);

	BlitString(this->sellingWindowPrice[0], *_game->window);
	BlitString(this->sellingWindowPrice[1], *_game->window);
	BlitString(this->sellingWindowPrice[2], *_game->window);

	BlitString(this->sellingWindowRessourceQuantity[0], *_game->window);
	BlitString(this->sellingWindowRessourceQuantity[1], *_game->window);
	BlitString(this->sellingWindowRessourceQuantity[2], *_game->window);

}


void Stalls::InputSellingWindow(struct Game *_game, bool *_isOfferAccepted)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);

	// Button Accept
	if (mousePosition.x > 1920 / 2 + 200 - (this->sellingWindowAcceptButton.getGlobalBounds().width / 2)
		&& mousePosition.x < 1920 / 2 + 200 + (this->sellingWindowAcceptButton.getGlobalBounds().width / 2)
		&& mousePosition.y > 1080 / 2 + 125 - (this->sellingWindowAcceptButton.getGlobalBounds().height / 2)
		&& mousePosition.y < 1080 / 2 + 125 + (this->sellingWindowAcceptButton.getGlobalBounds().height / 2))
	{
		std::cout << "Marchant offer accepted !\n\n";
		*(_isOfferAccepted) = true;
	}

	// Button Refuse	
	if (mousePosition.x > 1920 / 2 - 200 - (this->sellingWindowRejectButton.getGlobalBounds().width / 2)
		&& mousePosition.x < 1920 / 2 - 200 + (this->sellingWindowRejectButton.getGlobalBounds().width / 2)
		&& mousePosition.y > 1080 / 2 + 125 - (this->sellingWindowRejectButton.getGlobalBounds().height / 2)
		&& mousePosition.y < 1080 / 2 + 125 + (this->sellingWindowRejectButton.getGlobalBounds().height / 2))
	{
		std::cout << "Marchant offer refused !\n\n";
		*(_isOfferAccepted) = false;
	}


	// Click and Drag
	// Click à un endroit de la ligne sur la hauteur du curseur

	// Cursor movement
	if (mousePosition.x > 1920 / 2 - (this->sellingWindowScrollButton.getGlobalBounds().width / 2)
		&& mousePosition.x < 1920 / 2 + (this->sellingWindowScrollButton.getGlobalBounds().width / 2)
		&& mousePosition.y > 1080 / 2 - (this->sellingWindowScrollButton.getGlobalBounds().height / 2)
		&& mousePosition.y < 1080 / 2 + (this->sellingWindowScrollButton.getGlobalBounds().height / 2))
	{
		
	}

}

//void Stalls::AddNewBuildingToList(sf::Vector2f _mapPosition)
//{
//	LinkedListClass::sElement* newBuilding = new LinkedListClass::sElement;
//	newBuilding->data = new SpecificsBuildings::sBuildingData;
//
//	// Save the position in map
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->mapPosition = _mapPosition;
//
//	// Init of the building construction status after being placed on map
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->constructionState = PLANNED;
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->actualState = BUILDING_READY_TO_PRODUCE;
//
//	// A MODIFIER PAR VALEUR SEUIL
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->quantitativeThreshold = this->building->GetRessourceQuantityNeeded();
//	// A CONFIGURER
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->maximalQuantity = 5;
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->internalImportRessourceCounter = RESET;
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->internalExportRessourceCounter = RESET;
//
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->lifeTime = RESET;
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->actualProductionTime = RESET;
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->secondaryTime = RESET;
//
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->isChangingSprite = false;
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->isProduced = false;
//	((SpecificsBuildings::sBuildingData *)newBuilding->data)->isWorkerThere = false;
//
//	newBuilding->status = ELEMENT_ACTIVE;
//
//	// Add this new vine at the end of the list
//	this->AddElementToLinkedList(this->list, newBuilding, -1);
//
//	//this->ReadVineLinkedList();
//	this->ReadLinkedList(this->list);
//}


void Stalls::UpdateInternalCycles(enum GameState *_state, const float &_frametime, Ressources *_ressourceSent, Purchasers *_purchasers)
{
	if (this->constructionState == BUILT)
	{
		switch (this->actualState)
		{
		case STALL_READY_TO_WORKS:

			if (_ressourceSent->GetQuantityOwned() > 0
				&& internalImportRessourceCounter == 0)
			{
				this->actualState = STALL_FILLING;
			}
			
			break;

		case STALL_FILLING:

			if (_ressourceSent->GetQuantityOwned() - 1 > 0
				&& this->internalImportRessourceCounter + 1 <= this->maximalQuantity)
			{
				_ressourceSent->SubtractQuantityOwned(1);
				this->internalImportRessourceCounter += 1;
			}

			if (internalImportRessourceCounter >= this->quantitativeThreshold)
			{
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

		case STALL_OFFER_ACCEPTED:

			// - Animation de vente des amphores
			// - Vente des amphores en échange de sesterces
			
			this->actualState = STALL_READY_TO_WORKS;

			break;

		default:
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
//
//bool Stalls::ConfirmSpecificBuildingPresenceAtWorkerPosition(const sf::Vector2f &_mapPosition)
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
//					((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
//					return true;
//				}
//				else if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x - 1 == _mapPosition.x
//					&& ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y == _mapPosition.y)
//				{
//					((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
//					return true;
//				}
//				else if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x - 1 == _mapPosition.x
//					&& ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y - 1 == _mapPosition.y)
//				{
//					((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
//					return true;
//				}
//				else if (((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.x == _mapPosition.x
//					&& ((SpecificsBuildings::sBuildingData *)currentElement->data)->mapPosition.y - 1 == _mapPosition.y)
//				{
//					((SpecificsBuildings::sBuildingData *)currentElement->data)->isWorkerThere = true;
//					return true;
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