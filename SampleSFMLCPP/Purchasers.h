#ifndef PURCHASERS__H
#define PURCHASERS__H

#include "Entities.h"
#include "Ressources.h"
#include "Workers.h"


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

	enum WorkerStatus actualStatus;

	std::string provenance;

	int minimalMoneyValueForRessource;
	int maximalMoneyValueForRessource;

	int minimalRessourceQuantity;
	int actualRessourceQuantity;
	int maximalRessourceQuantity;

	enum TypesOfRessources ressourceID;

	int minimalMovementTime;
	int maximalMovementTime;

	bool isPreviousOfferHasBeenRefused;

	

	//bool isLauchingMovement; // Temporaire
	//bool isItWorkingPlace;


	//enum TypeOfBuilding actualBuilding;
	//enum TypeOfBuilding *targetedBuilding;

	//float timeToDeposit;

public:
	Purchasers();
	~Purchasers();

	void Initialisation(const int &_actualQuantity);
	int RandomiseData(const sf::Vector2i &_data);

	int TimeToTravel();
	void SetActualQuantityStored(const int &_quantity);
	sf::Vector2i GetUnitPriceScope();
	sf::Vector2i GetUnitQuantityRessourceScope();
	std::string GetProvenanceName();
	void IsOfferHasBeenRefused();
};



#endif // !PURCHASERS__H