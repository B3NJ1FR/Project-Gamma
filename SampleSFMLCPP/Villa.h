#pragma once

#include "Storage.h"
#include "Buildings.h"

enum class VillaBuildingStatus
{
	VILLA_NOT_PLANNED,
	VILLA_PLANNED,
	VILLA_CONSTRUCTION,
	VILLA_BUILT,
	VILLA_BUILDING_NOT_MAINTAINED,
	VILLA_BUILDING_DESTROYED,
};


class Villa
{
private:
	sf::Vector2i m_mapPosition;
	enum VillaBuildingStatus m_constructionState;

	int m_numberOfWorkersNeededToWorks;
	int m_currentNumberOfWorkersPresent;

	float m_lifeTime;

	bool m_isChangingSprite;
	bool m_hasBeenBuilt;
	bool m_isWorkerThere;

	// Constructor
	Villa();

public :
	// Destructor
	~Villa();

	// Singleton
	static Villa* GetSingleton();

	// Getters
	void Initialisation(Buildings * _buildingConcerned);
	void AddNewBuilding(sf::Vector2i _mapPosition);
	void UpdateBuildingConstruction();
	void UpdateBuildingSprite();

	bool GetWorkerIsThere(const sf::Vector2i &_mapPosition) const;
	inline sf::Sprite GetSpriteWorkerIsThere() const { return m_workerIsThereSprite; };
	inline sf::Sprite GetSpriteWorkerInside() const { return m_workerInsideSprite; };
	int GetNumberOfWorkersPresents(const sf::Vector2i& _mapPosition) const;
	bool IsBuildingIsWorking(const sf::Vector2i& _mapPosition) const;
	inline sf::Vector2i GetMapPosition() const { return m_mapPosition; };

	// Other functions
	bool ConfirmSpecificBuildingPresenceAtPosition(const sf::Vector2i &_mapPosition, const bool &_isPreciseCoordinates = false, const bool &_thisIsAWorker = false);
	void WorkerEnteringInThisPosition(const sf::Vector2i& _mapPosition);
	void WorkerLeavingThisPosition(const sf::Vector2i &_mapPosition);
	bool CheckBuildingHasBeenBuilt(const sf::Vector2i &_mapPosition);
	void ForceBuildingConstructionWithoutWorkers(sf::Vector2i _mapPosition);

	void SavingVillaForFile(std::ofstream *_file);
	void LoadingVillaFromFile(std::ifstream *_file);

private:
	Buildings *m_building;
	sf::Sprite m_workerIsThereSprite;
	sf::Sprite m_workerInsideSprite;
};