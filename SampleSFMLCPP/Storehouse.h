#pragma once

#include "Storage.h"
#include "SpecificsBuildings.h"
#include "LinkedList.h"


class Storehouse : public LinkedListClass
{
public:
	struct StorehouseData
	{
		sf::Vector2f mapPosition;
		enum BuildingStatus constructionState;

		Storage* storage = nullptr;

		float lifeTime;

		bool isChangingSprite;
		bool hasBeenBuilt;
		bool isWorkerThere;

		int numberOfWorkersNeededToWorks;
		int currentNumberOfWorkersPresent;
		
		int maximalQuantity; // Maximal threshold
		int internalRessourceCounter;
	};
	typedef struct StorehouseData sStorehouseData;

	// Constructor & Destructor
	Storehouse();
	~Storehouse();

	// Getters
	Storage* GetStorage(const sf::Vector2f& _mapPosition);

	void InitialisationStorehouse(Buildings *_specificBuildingConcerned);
	
	void AddNewBuildingToList(sf::Vector2f _mapPosition);
	void UpdateBuildingConstruction();
	void UpdateInternalCycles();
	void UpdateBuildingSprite();
	//void UpdateBuildingProduction(Ressources *_ressource);

	bool IsBuildingIsWorking(const sf::Vector2f& _mapPosition) const;
	bool ConfirmStorehousePresenceAtPosition(const sf::Vector2f &_mapPosition, const bool &_isPreciseCoordinates = false, const bool &_thisIsAWorker = false);
	void WorkerEnteringInThisPosition(const sf::Vector2f& _mapPosition);
	void WorkerLeavingThisPosition(const sf::Vector2f& _mapPosition);
	int GetNumberOfWorkersPresents(const sf::Vector2f& _mapPosition) const;
	bool GetWorkerIsThere(const sf::Vector2f &_mapPosition);
	bool CheckStorehouseHasBeenBuilt(const sf::Vector2f &_mapPosition);
	//bool CheckSpecificBuildingHasProducedRessource(const sf::Vector2f &_mapPosition);
	int UpdateRessourcePickuping(const sf::Vector2f &_mapPosition, const float &_frametime);
	sf::Vector2i FindNearestBuilding(const sf::Vector2f &_mapPosition);

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);
	void ClearStorages();

	void SavingForFile(std::ofstream *_file);
	void LoadingFromFile(std::ifstream *_file);

private:
	LinkedListClass::sLinkedList *m_list;
	Buildings *m_building;
};