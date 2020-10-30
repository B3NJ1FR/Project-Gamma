#include "BuildingsListPlanned.h"

BuildingsListPlanned::BuildingsListPlanned()
{
	this->listOfBuildingsPlanned = LinkedListInitialisation();
}

BuildingsListPlanned::~BuildingsListPlanned()
{	

}

void BuildingsListPlanned::ReadBuildingsPlannedToList()
{
	if (this->listOfBuildingsPlanned != nullptr)
	{
		if (this->listOfBuildingsPlanned->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->listOfBuildingsPlanned->first;
			int position(1);

			for (currentElement = this->listOfBuildingsPlanned->first; currentElement != NULL; currentElement = currentElement->next)
			{
				std::cout << position << "/" << this->listOfBuildingsPlanned->size << " - ID : " << ((struct DataBuildings *)currentElement->data)->buildingID << " - Map Position : " << ((struct DataBuildings *)currentElement->data)->mapPosition.x << " " << ((struct DataBuildings *)currentElement->data)->mapPosition.y << std::endl;

				position++;
			}
		}
	}

	std::cout << std::endl << std::endl;
}

sf::Vector2i BuildingsListPlanned::GetBuildingPositionInMap()
{
	if (this->listOfBuildingsPlanned != nullptr)
	{
		if (this->listOfBuildingsPlanned->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->listOfBuildingsPlanned->first;

			return ((struct DataBuildings *)currentElement->data)->mapPosition;
		}
		else
		{
			return sf::Vector2i(-1, -1);
		}
	}
	else
	{
		return sf::Vector2i(-1, -1);
	}
}

sf::Vector2i BuildingsListPlanned::GetBuildingSize()
{
	if (this->listOfBuildingsPlanned != nullptr)
	{
		if (this->listOfBuildingsPlanned->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->listOfBuildingsPlanned->first;

			return ((struct DataBuildings *)currentElement->data)->buildingSize;
		}
		else
		{
			return sf::Vector2i(-1, -1);
		}
	}
	else
	{
		return sf::Vector2i(-1, -1);
	}
}

int BuildingsListPlanned::GetBuildingID()
{
	if (this->listOfBuildingsPlanned != nullptr)
	{
		if (this->listOfBuildingsPlanned->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = this->listOfBuildingsPlanned->first;

			return ((struct DataBuildings *)currentElement->data)->buildingID;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

void BuildingsListPlanned::AddBuildingPlannedToList(const sf::Vector2i &_mapPosition, const enum TypeOfBuilding &_buildingID, const sf::Vector2i &_buildingSize)
{
	LinkedListClass::sElement* newBuilding = new LinkedListClass::sElement;
	newBuilding->data = new struct DataBuildings;

	// Save the position in map
	((struct DataBuildings *)newBuilding->data)->mapPosition = _mapPosition;

	// Init of the worker status
	((struct DataBuildings *)newBuilding->data)->buildingID = _buildingID;
	
	((struct DataBuildings *)newBuilding->data)->buildingSize = _buildingSize;
	
	newBuilding->status = ELEMENT_ACTIVE;


	// Add this new building at the end of the list
	this->AddElementToLinkedList(this->listOfBuildingsPlanned, newBuilding, -1);
	//this->ReadBuildingsPlannedToList();
}


void BuildingsListPlanned::DeleteCurrentFirstBuildingInList()
{
	if (this->listOfBuildingsPlanned != nullptr)
	{
		if (this->listOfBuildingsPlanned->first != nullptr)
		{
			// Deletion of the first element of the list
			RemoveElementsOfLinkedList(this->listOfBuildingsPlanned, true, 1);
		}
	}
}

inline bool BuildingsListPlanned::IsBuildingListIsEmpty()
{
	return ((this->listOfBuildingsPlanned != nullptr) && (this->listOfBuildingsPlanned->first != nullptr)) ? false : true;
}