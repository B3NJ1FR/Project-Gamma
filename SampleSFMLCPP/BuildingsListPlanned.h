#pragma once

#include "LinkedList.h"


class BuildingsListPlanned : public LinkedListClass
{
private:	

	struct DataBuildings
	{
		sf::Vector2i mapPosition;

		sf::Vector2i buildingSize;
		int buildingID; // enum TypeOfBuilding
	};

	LinkedListClass::sLinkedList *listOfBuildingsPlanned;

public:
	// Constructor & Destructor
	BuildingsListPlanned();
	~BuildingsListPlanned();

	void ReadBuildingsPlannedToList();

	// Getters
	sf::Vector2i GetBuildingPositionInMap();
	sf::Vector2i GetBuildingSize();
	int GetBuildingID();

	// Methods
	void AddBuildingPlannedToList(const sf::Vector2i &_mapPosition, const enum TypeOfBuilding &_buildingID, const sf::Vector2i &_buildingSize);

	void DeleteCurrentFirstBuildingInList();
	inline bool IsBuildingListIsEmpty();
};