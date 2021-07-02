#pragma once

#include "Common.hpp"
#include "LinkedList.h"
#include "Map.h"
#include "BuildingManagement.h"

struct BuildingToDestroyData
{
	sf::Vector2f m_coordinates;
	int m_ID;
	float m_destructionTimer;
};


class BuildingDestruction : public LinkedListClass
{
private:
	bool m_isSomeBuildingNeedToBeDestroyed;
	LinkedListClass::sLinkedList* m_listOfBuildingsToDestroy;

	// Constructor
	BuildingDestruction();

public:
	// Destructor
	~BuildingDestruction();

	// Getters
	static BuildingDestruction* GetSingleton();

	// Methods
	void InitialisationBuildingsToDestroy();
	void AddNewBuildingToDestroy(const sf::Vector2f& _coordinates, int _buildingID);
	bool BuildingDestructionExistence(const sf::Vector2f& _coordinates, int _buildingID);
	void UpdateBuildingDestruction(BuildingManagement* _builds, Map* _map);
	void RemoveBuildingOnMap(BuildingManagement* _builds, const int& _typeOfBuilding, const sf::Vector2i& _mapPosition);

	void SavingForFile(std::ofstream* _file);
	void LoadingFromFile(std::ifstream* _file);
};

