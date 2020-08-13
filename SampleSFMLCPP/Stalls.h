#ifndef STALLS__H
#define STALLS__H

#include "Ressources.h"
#include "Buildings.h"
#include "Purchasers.h"


enum StallStatus
{
	STALL_READY_TO_WORKS,
	STALL_FILLING,
	STALL_SEND_REQUEST_PURCHASER,
	STALL_PURCHASER_IS_PRESENT,
	STALL_OFFER_HANDLED,
};


class Stalls
{
private:
	Buildings *building;
	

	sf::Vector2f mapPosition;
	enum BuildingStatus constructionState;
	enum StallStatus actualState;

	int quantitativeThreshold; // Seuil minimum de lancement
	int maximalQuantity; // Seuil maximal
	int internalImportRessourceCounter;

	float lifeTime;
	float actualProductionTime;
	//float secondaryTime;

	bool isChangingSprite;
	//bool isProduced;
	bool isWorkerThere;
	bool isPurchaserThere;

	int priceAccepted;
	int internalImportRessourceCounterSaved;
	int ressourceQuantityToSell;
	bool isNewMerchantNeeded;
public:
	
	bool isOfferAccepted;

	Stalls(Buildings *_specificBuildingConcerned);
	~Stalls();

	void SetStatus(const enum StallStatus &_newStatus);
	enum StallStatus GetStatus();
	int GetActualRessourcesStocked();
	bool GetIsNewMerchantNeeded();
	void SetIsNewMerchantNeeded(const bool &_newStatus);
	void InitialisationStall(Buildings *_specificBuildingConcerned);
	void AddNewBuilding(sf::Vector2f _mapPosition);
	void PickUpPriceAccepted(const int &_price);
	void UpdateBuildingConstruction(const float &_frametime);
	void UpdateInternalCycles(class Money *_money, enum GameState *_state, const float &_frametime, Ressources *_ressource, Purchasers *_purchasers);
	//void UpdateBuildingSprite(unsigned short ***_map);
	//void UpdateBuildingProduction(Ressources *_ressource);

	bool ConfirmPresenceAtWorkerPosition(const sf::Vector2f &_mapPosition);
	//bool CheckSpecificBuildingHasProducedRessource(const sf::Vector2f &_mapPosition);
	//int SpecificsBuildingsSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime);
	//sf::Vector2i SpecificsBuildingsFindNearestBuilding(const sf::Vector2f &_mapPosition);

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);

	void SetRessourceQuantityToSell(const int &_quantity = RESET);
};

#endif // !STALLS__H