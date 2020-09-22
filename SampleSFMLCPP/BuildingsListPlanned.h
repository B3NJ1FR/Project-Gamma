#ifndef BUILDINGS_LIST_PLANNED__H
#define BUILDINGS_LIST_PLANNED__H

#include "LinkedList.h"


class BuildingsListPlanned : public LinkedListClass
{
private:	

	struct DataBuildings
	{
		sf::Vector2i mapPosition;

		sf::Vector2i buildingSize;
		enum TypeOfBuilding buildingID;
	};

	LinkedListClass::sLinkedList *listOfBuildingsPlanned;

public:
	BuildingsListPlanned();
	~BuildingsListPlanned();

	void ReadBuildingsPlannedToList();

	sf::Vector2i GetBuildingPositionInMap();
	sf::Vector2i GetBuildingSize();
	int GetBuildingID();

	void AddBuildingPlannedToList(const sf::Vector2i &_mapPosition, const enum TypeOfBuilding &_buildingID, const sf::Vector2i &_buildingSize);

	void DeleteCurrentFirstBuildingInList();
	bool IsBuildingListIsEmpty();
};



#endif // !BUILDINGS_LIST_PLANNED__H