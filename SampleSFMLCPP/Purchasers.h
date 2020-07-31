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

	int TimeToTravel();
	sf::Vector2i GetUnitPriceScope();
	sf::Vector2i GetUnitQuantityRessourceScope();

	//void InitPathfinding(struct Game *_game);

	//void SetWorkerPosition(const sf::Vector2f &_mapPosition);
	//void SetWorkerStatus(const enum WorkerStatus &_newStatus);
	//void SetWorkerMoneyValue(const int &_moneyValue);
	//void SetWorkerMoneyCostPerMonth(const int &_moneyCostPerMonth);
	//void SetTimeToDeposit(const float &_time);
	//void AddTimeToDeposit(const float &_frametime);

	//void SetEndingPosition(sf::Vector2i _mapPosition, unsigned short ***_map);
	//sf::Vector2f GetWorkerPosition();
	//int GetWorkerMoneyValue();
	//int GetWorkerMoneyCostPerMonth();
	//enum WorkerStatus GetWorkerStatus();
	//float GetTimeToDeposit();

	//void DisplayWorker();
	//void UpdatePathAndActivities(struct Game *_game);

	//void ActiveLauchingMovement(); // TEMPORAIRE
	//bool isPressingStart; // Temporaire
	//bool isPressingEnd; // Temporaire
};



#endif // !PURCHASERS__H