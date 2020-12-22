#pragma once

#include "Entities.h"
#include "Ressources.h"


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

	WorkerStatus m_actualStatus;

	std::string m_provenance;

	int m_minimalMoneyValueForRessource;
	int m_maximalMoneyValueForRessource;

	int m_minimalRessourceQuantity;
	int m_actualRessourceQuantity;
	int m_maximalRessourceQuantity;

	enum TypesOfRessources m_ressourceID;

	int m_minimalMovementTime;
	int m_maximalMovementTime;

	bool m_isPreviousOfferHasBeenRefused;

	

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

	// Getters
	sf::Vector2i GetUnitPriceScope();
	inline sf::Vector2i GetUnitQuantityRessourceScope() const { return sf::Vector2i(m_minimalRessourceQuantity, m_maximalRessourceQuantity); };
	inline std::string GetProvenanceName() const { return m_provenance; };

	// Methods
	void Initialisation(const int& _actualQuantity);
	int RandomiseData(const sf::Vector2i& _data);

	int TimeToTravel();
	inline void IsOfferHasBeenRefused() { m_isPreviousOfferHasBeenRefused = true; };

	// Save
	void SavingPurchasersForFile(std::ofstream *_file);
	void LoadingPurchasersFromFile(std::ifstream *_file);
};