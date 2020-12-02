#ifndef STALLS__H
#define STALLS__H

#include "Ressources.h"
#include "Buildings.h"
#include "Storehouse.h"
#include "Purchasers.h"


enum StallStatus
{
	STALL_WAITING,
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
	int internalRessourceCounter;

	float lifeTime;
	float actualProductionTime;

	bool isChangingSprite;
	bool hasBeenBuilt;
	bool isWorkerThere;
	bool isPurchaserThere;

	int priceAccepted;
	int internalImportRessourceCounterSaved;
	int ressourceQuantityToSell;
	bool isNewMerchantNeeded;

	sf::Vector2f *storehousesCoordinates;
	int numberStorehousesCoordinates;

public:
	
	bool isOfferAccepted;

	Stalls(Buildings *_specificBuildingConcerned);
	~Stalls();

	void InitialisationStall(Buildings *_specificBuildingConcerned);

	void SetConstructionStatus(const enum BuildingStatus &_newStatus);
	void SetStatus(const enum StallStatus &_newStatus);
	void SetIsNewMerchantNeeded(const bool &_newStatus);
	void AddNewBuilding(sf::Vector2f _mapPosition);
	void AddStorehousePosition(const sf::Vector2f &_mapPosition);
	void SetRessourceQuantityToSell(const int &_quantity = RESET);

	void PickUpPriceAccepted(const int &_price);

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);
	bool ConfirmPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates = false, const bool &_thisIsAWorker = false);

	bool GetWorkerIsThere(const sf::Vector2f &_mapPosition);
	enum StallStatus GetStatus();
	enum BuildingStatus GetConstructionStatus();
	int GetActualRessourcesStocked();
	bool GetIsNewMerchantNeeded();
	sf::Vector2i GetMapPosition();

	void UpdateBuildingConstruction(const float &_frametime);
	void UpdateInternalCycles(class Money *_money, enum CurrentGameState *_state, const float &_frametime, Ressources *_ressource, Purchasers *_purchasers, Storehouse *_storehouse);
	void UpdateBuildingSprite(unsigned short ***_map);


	void SavingStallForFile(std::ofstream *_file);
	void LoadingStallFromFile(std::ifstream *_file);

};

#endif // !STALLS__H