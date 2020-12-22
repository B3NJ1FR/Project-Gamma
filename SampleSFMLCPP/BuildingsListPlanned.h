#pragma once

#include "LinkedList.h"


class BuildingsListPlanned : public LinkedListClass
{
private:	

	struct DataBuildings
	{
		sf::Vector2i m_mapPosition;

		sf::Vector2i m_buildingSize;
		int m_buildingID; // enum TypeOfBuilding
	};

	LinkedListClass::sLinkedList *m_listOfBuildingsPlanned;

public:
	// Constructor & Destructor
	BuildingsListPlanned();
	~BuildingsListPlanned();

	void ReadBuildingsPlannedToList();

	// Getters
	sf::Vector2i GetBuildingPositionInMap() const;
	sf::Vector2i GetBuildingSize() const;
	int GetBuildingID() const;

	// Methods
	void AddBuildingPlannedToList(const sf::Vector2i &_mapPosition, const enum TypeOfBuilding &_buildingID, const sf::Vector2i &_buildingSize);

	void DeleteCurrentFirstBuildingInList();
	inline bool IsBuildingListIsEmpty() { return ((m_listOfBuildingsPlanned != nullptr) && (m_listOfBuildingsPlanned->first != nullptr)) ? false : true; };
};