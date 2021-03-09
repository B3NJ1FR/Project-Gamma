#pragma once

#include "Purchasers.h"
#include "Stalls.h"

class PurchasersManager
{
private:
	Purchasers* m_currentPurchaser = nullptr;

	int m_minRangeTimeForSpawn;
	int m_maxRangeTimeForSpawn;
	float m_currentTimeBeforeSpawn;

	bool m_isMerchantSpawned;

	// Constructor
	PurchasersManager();

public:
	// Destructor
	~PurchasersManager();

	static PurchasersManager* GetSingleton();
	void Initialisation();

	// Setters

	// Getters
	Purchasers* GetPurchasers() const { return m_currentPurchaser; }
	bool IsPurchaserExist() const { return (m_currentPurchaser != nullptr) ? true : false; }

	// Methods
	void Update(BuildingManagement* _builds, Stalls* _stall);
	void AskToRemoveCurrentPurchaser();
	void RemoveCurrentPurchaser();

	// Save
	void SavingForFile(std::ofstream* _file);
	void LoadingFromFile(std::ifstream* _file);
};

