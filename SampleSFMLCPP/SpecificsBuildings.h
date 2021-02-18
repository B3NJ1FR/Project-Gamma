#pragma once

#include "Ressources.h"
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

		int quantitativeThreshold; // Seuil minimum de lancement
		int maximalQuantity; // Seuil maximal
		int internalImportRessourceCounter;
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
	};
	typedef struct BuildingData sBuildingData;

	// Constructor & Destructor
	SpecificsBuildings();
	~SpecificsBuildings();

	void InitialisationSpeBuilding(Buildings *_specificBuildingConcerned);
	void AddNewBuildingToList(sf::Vector2f _mapPosition);
	void UpdateBuildingConstruction(const float &_frametime);
	void UpdateInternalCycles(const float &_frametime, Ressources *_ressource, Ressources *_ressourceProduced);
	void UpdateBuildingSprite(unsigned short ***_map, const enum TypeOfBuilding &_building);
	void UpdateBuildingProduction(Ressources *_ressource);

	bool GetWorkerIsThere(const sf::Vector2f &_mapPosition);
	inline sf::Sprite GetSpriteWorkerIsThere() const { return m_workerIsThereSprite; };
	inline sf::Sprite GetSpriteWorkerInside() const { return m_workerInsideSprite; };
	int GetNumberOfWorkersPresents(const sf::Vector2f& _mapPosition) const;
	bool IsBuildingIsWorking(const sf::Vector2f& _mapPosition) const;

	bool ConfirmSpecificBuildingPresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates = false, const bool &_thisIsAWorker = false);
	void WorkerEnteringInThisPosition(const sf::Vector2f& _mapPosition);
	void WorkerLeavingThisPosition(const sf::Vector2f &_mapPosition);
	bool CheckSpecificBuildingHasProducedRessource(const sf::Vector2f &_mapPosition);
	bool CheckSpecificsBuildingsHasBeenBuilt(const sf::Vector2f &_mapPosition);
	int SpecificsBuildingsSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime);
	sf::Vector2i SpecificsBuildingsFindNearestBuilding(const sf::Vector2f &_mapPosition);

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);



	void SavingSpecificsBuildingsListForFile(std::ofstream *_file);
	void LoadingSpecificsBuildingsListFromFile(std::ifstream *_file);

private:
	LinkedListClass::sLinkedList *m_list;
	Buildings *m_building;
	sf::Sprite m_workerIsThereSprite;
	sf::Sprite m_workerInsideSprite;
};