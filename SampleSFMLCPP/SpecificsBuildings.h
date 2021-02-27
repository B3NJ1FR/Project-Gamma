#pragma once

#include "Storage.h"
#include "Buildings.h"
#include "LinkedList.h"

enum BuildingStatus
{
	PLANNED,
	CONSTRUCTION,
	BUILT,
	BUILDING_NOT_MAINTAINED,
	BUILDING_DESTROYED,
};


enum MainBuildingStatus
{
	BUILDING_READY_TO_PRODUCE,
	BUILDING_FILLING,
	BUILDING_WORKS,
	BUILDING_COLLECTING_PRODUCTION,
	BUILDING_NEED_TO_BE_CLEANED,
};


class SpecificsBuildings : public LinkedListClass
{
public:	
	struct BuildingData
	{
		sf::Vector2f mapPosition;
		enum BuildingStatus constructionState;
		enum MainBuildingStatus actualState;

		Storage* storage = nullptr;

		int quantitativeThreshold; // Seuil minimum de lancement
		int maximalQuantity; // Seuil maximal
		int internalExportRessourceCounter;

		int numberOfWorkersNeededToWorks;
		int currentNumberOfWorkersPresent;

		float lifeTime;
		float actualProductionTime;
		float secondaryTime;

		bool isChangingSprite;
		bool hasBeenBuilt;
		bool isProduced;
		bool isWorkerThere;
		bool isProductionTransformed;
	};
	typedef struct BuildingData sBuildingData;

	// Constructor & Destructor
	SpecificsBuildings();
	~SpecificsBuildings();

	// Getters
	Storage* GetStorage(const sf::Vector2f& _mapPosition);

	void InitialisationSpeBuilding(Buildings *_specificBuildingConcerned);
	void AddNewBuildingToList(sf::Vector2f _mapPosition);
	void UpdateBuildingConstruction(const float &_frametime);
	void UpdateInternalCycles(const float &_frametime);
	void UpdateBuildingSprite(unsigned short ***_map, const enum TypeOfBuilding &_building);
	void UpdateBuildingProduction();

	bool GetWorkerIsThere(const sf::Vector2f &_mapPosition);
	inline sf::Sprite GetSpriteWorkerIsThere() const { return m_workerIsThereSprite; };
	inline sf::Sprite GetSpriteWorkerInside() const { return m_workerInsideSprite; };
	int GetNumberOfWorkersPresents(const sf::Vector2f& _mapPosition) const;
	bool IsBuildingIsWorking(const sf::Vector2f& _mapPosition) const;

	bool ConfirmSpecificBuildingPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates = false, const bool &_thisIsAWorker = false);
	void WorkerEnteringInThisPosition(const sf::Vector2f& _mapPosition);
	void WorkerLeavingThisPosition(const sf::Vector2f &_mapPosition);
	bool CheckHasProducedRessource(const sf::Vector2f &_mapPosition);
	bool CheckSpecificsBuildingsHasBeenBuilt(const sf::Vector2f &_mapPosition);
	bool UpdateRessourcePickuping(const sf::Vector2f &_mapPosition, const float &_frametime);
	sf::Vector2i FindNearestBuilding(const sf::Vector2f &_mapPosition);

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);



	void SavingSpecificsBuildingsListForFile(std::ofstream *_file);
	void LoadingSpecificsBuildingsListFromFile(std::ifstream *_file);

private:
	LinkedListClass::sLinkedList *m_list;
	Buildings *m_building;
	sf::Sprite m_workerIsThereSprite;
	sf::Sprite m_workerInsideSprite;
};