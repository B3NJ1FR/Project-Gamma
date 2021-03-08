#pragma once

#include "EnumInternalState.h"

#include "Storage.h"
#include "Buildings.h"
#include "Money.h"
#include "Storehouse.h"


enum StallStatus
{
	STALL_NOT_CONSTRUCTED = -1,
	STALL_WAITING,
	STALL_PURCHASER_IS_PRESENT,
	STALL_OFFER_HANDLED,
};

class Purchasers;

class Stalls
{
private:
	Buildings *m_building;

	Storage* m_storage = nullptr;

	InternalState m_internalState;

	sf::Vector2f m_mapPosition;
	enum BuildingStatus m_constructionState;
	enum StallStatus m_actualState;

	int m_quantitativeThreshold; // Minimal threshold of launching
	int m_maximalQuantity; // Maximal threshold
	int m_internalRessourceCounter;

	int m_numberOfWorkersNeededToWorks;
	int m_currentNumberOfWorkersPresent;

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

	// Setters
	void SetConstructionStatus(const enum BuildingStatus &_newStatus);
	void SetStatus(const enum StallStatus &_newStatus);
	void SetIsNewMerchantNeeded(const bool &_newStatus);
	inline void SetIsPurchaserIsThere(bool _newStatus) { m_isPurchaserThere = _newStatus; }
	inline void SetIsOfferAccepted(bool _newStatus) { m_isOfferAccepted = _newStatus; }
	void AddNewBuilding(sf::Vector2f _mapPosition);
	void AddStorehousePosition(const sf::Vector2f &_mapPosition);
	inline void SetRessourceQuantityToSell(const int& _quantity = RESET) { m_ressourceQuantityToSell = _quantity; };
	inline void PickUpPriceAccepted(const int& _price) { m_priceAccepted = _price; };

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);
	bool IsBuildingIsWorking(const sf::Vector2f& _mapPosition) const;
	bool ConfirmPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates = false, const bool &_thisIsAWorker = false);
	void WorkerEnteringInThisPosition(const sf::Vector2f& _mapPosition);
	void WorkerLeavingThisPosition(const sf::Vector2f& _mapPosition);

	// Getters
	int GetNumberOfWorkersPresents(const sf::Vector2f& _mapPosition) const;
	bool GetWorkerIsThere(const sf::Vector2f &_mapPosition);
	enum StallStatus GetStatus();
	enum BuildingStatus GetConstructionStatus();
	int GetActualRessourcesStocked();
	bool GetIsNewMerchantNeeded();
	inline sf::Vector2i GetMapPosition() const { return (sf::Vector2i)m_mapPosition; }
	inline Storage* GetStorage() const { return m_storage; }
	int GetRessourceStocked() const;

	void UpdateBuildingConstruction();
	void UpdateInternalCycles(Money *_money, enum CurrentGameState *_state, Purchasers *_purchasers, Storehouse *_storehouse);
	void UpdateBuildingSprite(unsigned short ***_map);

	void ClearStorages();

	void SavingStallForFile(std::ofstream *_file);
	void LoadingStallFromFile(std::ifstream *_file);

};