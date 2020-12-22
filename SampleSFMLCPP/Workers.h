#pragma once

#include "Entities.h"
#include "Map.h"
#include "TimeManagement.h"
#include "Ressources.h"
#include "BuildingManagement.h"

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

	WorkerStatus m_actualStatus;
	
	int m_moneyValue;
	int m_moneyCostPerMonth;

	bool m_isLauchingMovement; // Temporaire
	bool m_isItWorkingPlace;
	bool m_isWorkerWasWorkingInBuilding;

	enum TypesOfRessources *m_ressourceHeld;
	int *m_quantityRessourceHeld;

	TypeOfBuilding m_currentBuilding;
	TypeOfBuilding *m_targetedBuilding;

	float m_timeToDeposit;

public:
	// Constructor & Destructor
	Workers();
	~Workers();

	// Setters
	void SetWorkerPosition(const sf::Vector2f& _mapPosition) { m_mapPosition = _mapPosition; };
	void SetWorkerStatus(const enum WorkerStatus& _newStatus) { m_actualStatus = _newStatus; };

	void SetWorkerMoneyValue(const int& _moneyValue) { m_moneyValue = _moneyValue; };

	void SetWorkerMoneyCostPerMonth(const int& _moneyCostPerMonth) { m_moneyCostPerMonth = _moneyCostPerMonth; };
	void SetWorkerIsInWorkingPlace(const bool& _isItWorkingPlace) { m_isItWorkingPlace = _isItWorkingPlace; };
	void SetTimeToDeposit(const float& _time) { m_timeToDeposit = _time; };
	void AddTimeToDeposit(const float& _frametime) { m_timeToDeposit += _frametime; };
	void SetEndingPosition(const sf::Vector2i& _mapPosition, unsigned short ***_map);

	// Getters
	inline sf::Vector2f GetWorkerPosition() const { return m_mapPosition; };
	inline sf::Vector2f GetWorkerEndingPosition() const { return m_mapEndPosition; };
	inline int GetWorkerMoneyValue() const { return m_moneyValue; };
	inline int GetWorkerMoneyCostPerMonth() const { return m_moneyCostPerMonth; };
	inline bool GetWorkerIsInWorkingPlace() const { return m_isItWorkingPlace; };
	inline enum WorkerStatus GetWorkerStatus() const { return m_actualStatus; };
	inline float GetTimeToDeposit() const { return m_timeToDeposit; };
	inline enum TypeOfBuilding GetWorkerActualBuilding() const { return m_currentBuilding; };

	// Methods
	void InitPathfinding(Map* _map);

	void DisplayWorker();
	void UpdatePathAndActivities(Map* _map, TimeManagement* _time, BuildingManagement* _builds, Ressources* _ressources);

	void ActiveLauchingMovement(); // TEMPORAIRE
	bool m_isPressingStart; // Temporaire
	bool m_isPressingEnd; // Temporaire
};