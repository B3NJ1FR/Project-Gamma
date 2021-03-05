#pragma once

#include "Entities.h"
#include "Storage.h"
#include "BuildingManagement.h"


enum class PurchaserStatus
{
	IDLE,
	WAITING_MOVEMENT,
	MOVEMENT,
	WORKING,
	WAITING_RESOURCES,
	PICKUP_RESSOURCES,
	END_OF_LIFE,
};

class Purchasers : public Entities
{
private:
	//enum WorkerCaste caste; // FUTURE UPDATE
	//int happiness; // FUTURE UPDATE
	//int age; // FUTURE UPDATE

	// - origine / provenance
	// - min / max -> prix /u
	// - min / max -> quantité
	// - min / max -> temps de déplacement
	// - décalage entre plus l'on vend, moins on gagne
	// - si une offre est refusée, la prochaine est un peu plus cher

	PurchaserStatus m_actualStatus;

	Storage* m_storage = nullptr;

	std::string m_provenance;

	bool m_isLauchingMovement;

	int m_minimalMoneyValueForRessource;
	int m_maximalMoneyValueForRessource;

	int m_minimalRessourceQuantity;
	int m_actualRessourceQuantity;
	int m_maximalRessourceQuantity;

	enum TypesOfRessources m_ressourceID;

	int m_counterOfRefusedOffers;
	bool m_isPreviousOfferHasBeenRefused;

	bool m_isStallExist;

	//bool isLauchingMovement; // Temporaire
	//bool isItWorkingPlace;


	//enum TypeOfBuilding actualBuilding;
	//enum TypeOfBuilding *targetedBuilding;

	//float timeToDeposit;

public:
	// Constructor & Destructor
	Purchasers();
	~Purchasers();

	// Setters
	inline void SetActualQuantityStored(const int& _quantity);
	inline void SetStatus(enum PurchaserStatus _newStatus) { m_actualStatus = _newStatus; };
	// Getters
	sf::Vector2i GetUnitPriceScope();
	inline sf::Vector2i GetUnitQuantityRessourceScope() const { return sf::Vector2i(m_minimalRessourceQuantity, m_maximalRessourceQuantity); };
	inline std::string GetProvenanceName() const { return m_provenance; };

	// Methods
	void Initialisation(Stalls* _stall);
	void InitialisationFromDataFile();
	void InitPathfinding();
	sf::Vector2f FindWherePurchaserMustStopItself();
	sf::Vector2f FindEndRoad();

	void UpdateLife(struct Game* _game);
	void UpdatePathAndActivities(Map* _map, BuildingManagement* _builds);

	int RandomiseData(const sf::Vector2i& _data);

	int TimeToTravel();
	inline void IsOfferHasBeenRefused() { m_isPreviousOfferHasBeenRefused = true; };
	void ClearStorage();

	// Save
	void SavingPurchasersForFile(std::ofstream *_file);
	void LoadingPurchasersFromFile(std::ifstream *_file);
};