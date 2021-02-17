#pragma once

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
	Buildings *m_building;
	

	sf::Vector2f m_mapPosition;
	enum BuildingStatus m_constructionState;
	enum StallStatus m_actualState;

	int m_quantitativeThreshold; // Minimal threshold of launching
	int m_maximalQuantity; // Maximal threshold
	int m_internalRessourceCounter;

	float m_lifeTime;
	float m_actualProductionTime;

	bool m_isChangingSprite;
	bool m_hasBeenBuilt;
	bool m_isWorkerThere;
	bool m_isPurchaserThere;

	int m_priceAccepted;
	int m_internalImportRessourceCounterSaved;
	int m_ressourceQuantityToSell;
	bool m_isNewMerchantNeeded;

	sf::Vector2f *m_storehousesCoordinates;
	int m_numberStorehousesCoordinates;

public:
	
	bool m_isOfferAccepted;

	Stalls(Buildings *_specificBuildingConcerned);
	~Stalls();

	void InitialisationStall(Buildings *_specificBuildingConcerned);

	void SetConstructionStatus(const enum BuildingStatus &_newStatus);
	void SetStatus(const enum StallStatus &_newStatus);
	void SetIsNewMerchantNeeded(const bool &_newStatus);
	void AddNewBuilding(sf::Vector2f _mapPosition);
	void AddStorehousePosition(const sf::Vector2f &_mapPosition);
	inline void SetRessourceQuantityToSell(const int& _quantity = RESET) { m_ressourceQuantityToSell = _quantity; };
	inline void PickUpPriceAccepted(const int& _price) { m_priceAccepted = _price; };

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);
	bool ConfirmPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates = false, const bool &_thisIsAWorker = false);
	void WorkerLeavingThisPosition(const sf::Vector2f& _mapPosition);

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