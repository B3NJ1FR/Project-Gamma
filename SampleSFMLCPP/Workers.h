#pragma once

#include "Entities.h"
#include "BuildingManagement.h"
#include "Map.h"
#include "TimeManagement.h"


//#include "Vines.h"
//#include "SpecificsBuildings.h"


enum WorkerCaste
{
	NORMAL_WORKER,
	SLAVE,
};

class Workers : public Entities, public BuildingManagement
{
private :
	//enum WorkerCaste caste; // FUTURE UPDATE
	//int happiness; // FUTURE UPDATE
	//int age; // FUTURE UPDATE

	enum WorkerStatus m_actualStatus;
	
	int m_moneyValue;
	int m_moneyCostPerMonth;

	bool m_isLauchingMovement; // Temporaire
	bool m_isItWorkingPlace;

	enum TypesOfRessources *m_ressourceHeld;
	int *m_quantityRessourceHeld;

	enum TypeOfBuilding m_actualBuilding;
	enum TypeOfBuilding *m_targetedBuilding;

	float m_timeToDeposit;

public:
	// Constructor & Destructor
	Workers();
	~Workers();

	// Setters
	void SetWorkerPosition(const sf::Vector2f &_mapPosition);
	void SetWorkerStatus(const enum WorkerStatus &_newStatus);
	void SetWorkerMoneyValue(const int &_moneyValue);
	void SetWorkerMoneyCostPerMonth(const int &_moneyCostPerMonth);
	void SetWorkerIsInWorkingPlace(const bool &_isItWorkingPlace);
	void SetTimeToDeposit(const float &_time);
	void AddTimeToDeposit(const float &_frametime);
	void SetEndingPosition(const sf::Vector2i& _mapPosition, unsigned short ***_map);

	// Getters
	inline sf::Vector2f GetWorkerPosition() const;
	inline sf::Vector2f GetWorkerEndingPosition() const;
	inline int GetWorkerMoneyValue() const;
	inline int GetWorkerMoneyCostPerMonth() const;
	inline bool GetWorkerIsInWorkingPlace() const;
	inline enum WorkerStatus GetWorkerStatus() const;
	inline float GetTimeToDeposit() const;
	inline enum TypeOfBuilding GetWorkerActualBuilding() const;

	// Methods
	void InitPathfinding(Map* _map);

	void DisplayWorker();
	void UpdatePathAndActivities(Map* _map, TimeManagement* _time, BuildingManagement* _builds, Ressources* _ressources);

	void ActiveLauchingMovement(); // TEMPORAIRE
	bool m_isPressingStart; // Temporaire
	bool m_isPressingEnd; // Temporaire
};