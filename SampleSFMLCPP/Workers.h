#ifndef WORKERS__H
#define WORKERS__H

#include "Entities.h"
#include "Vines.h"
#include "SpecificsBuildings.h"


enum WorkerStatus
{
	IDLE,
	WAITING_MOVEMENT,
	MOVEMENT,
	WORKING,
	PICKUP_RESSOURCES,
	DEPOSIT_RESSOURCES,
};

enum WorkerCaste
{
	NORMAL_WORKER,
	SLAVE,
};

class Workers : public Entities
{
private :
	//enum WorkerCaste caste; // FUTURE UPDATE
	//int happiness; // FUTURE UPDATE
	//int age; // FUTURE UPDATE

	enum WorkerStatus actualStatus;
	
	int moneyValue;
	int moneyCostPerMonth;

	bool isLauchingMovement; // Temporaire
	bool isItWorkingPlace;

	enum TypesOfRessources *ressourceHeld;
	int *quantityRessourceHeld;

	enum TypeOfBuilding actualBuilding;
	enum TypeOfBuilding *targetedBuilding;

	float timeToDeposit;

public:
	Workers();
	~Workers();

	void InitPathfinding(struct Game *_game);

	void SetWorkerPosition(const sf::Vector2f &_mapPosition);
	void SetWorkerStatus(const enum WorkerStatus &_newStatus);
	void SetWorkerMoneyValue(const int &_moneyValue);
	void SetWorkerMoneyCostPerMonth(const int &_moneyCostPerMonth);
	void SetTimeToDeposit(const float &_time);
	void AddTimeToDeposit(const float &_frametime);

	void SetEndingPosition(sf::Vector2i _mapPosition, unsigned short ***_map);
	sf::Vector2f GetWorkerPosition();
	sf::Vector2f GetWorkerEndingPosition();
	int GetWorkerMoneyValue();
	int GetWorkerMoneyCostPerMonth();
	bool GetWorkerIsInWorkingPlace();
	enum WorkerStatus GetWorkerStatus();
	float GetTimeToDeposit();

	void DisplayWorker();
	void UpdatePathAndActivities(struct Game *_game);

	void ActiveLauchingMovement(); // TEMPORAIRE
	bool isPressingStart; // Temporaire
	bool isPressingEnd; // Temporaire
};



#endif // !WORKERS__H