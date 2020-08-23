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

	int quantitativeThreshold; // Minimal threshold of launching
	int maximalQuantity; // Maximal threshold
	int internalImportRessourceCounter;

	float lifeTime;
	float actualProductionTime;

	bool isChangingSprite;
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

	void InitialisationStall(Buildings *_specificBuildingConcerned);

	void SetStatus(const enum StallStatus &_newStatus);
	void SetIsNewMerchantNeeded(const bool &_newStatus);
	void AddNewBuilding(sf::Vector2f _mapPosition);
	void SetRessourceQuantityToSell(const int &_quantity = RESET);
	void PickUpPriceAccepted(const int &_price);
	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);
	bool ConfirmPresenceAtWorkerPosition(const sf::Vector2f &_mapPosition);

	enum StallStatus GetStatus();
	enum BuildingStatus GetConstructionStatus();
	int GetActualRessourcesStocked();
	bool GetIsNewMerchantNeeded();
	sf::Vector2i GetMapPosition();

	void UpdateBuildingConstruction(const float &_frametime);
	void UpdateInternalCycles(class Money *_money, enum GameState *_state, const float &_frametime, Ressources *_ressource, Purchasers *_purchasers);


	void SavingStallForFile(std::ofstream *_file);
	void LoadingStallFromFile(std::ifstream *_file);

};

#endif // !STALLS__H