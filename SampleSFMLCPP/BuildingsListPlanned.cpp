#include "BuildingsListPlanned.h"

BuildingsListPlanned::BuildingsListPlanned()
{
	m_listOfBuildingsPlanned = LinkedListInitialisation();
}

BuildingsListPlanned::~BuildingsListPlanned()
{	
	if (m_listOfBuildingsPlanned != nullptr)
	{
		LinkedListClass::FreeLinkedList(m_listOfBuildingsPlanned);

		delete m_listOfBuildingsPlanned;
	}
}

void BuildingsListPlanned::ReadBuildingsPlannedToList()
{
	if (m_listOfBuildingsPlanned != nullptr)
	{
		if (m_listOfBuildingsPlanned->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_listOfBuildingsPlanned->first;
			int position(1);

			for (currentElement = m_listOfBuildingsPlanned->first; currentElement != NULL; currentElement = currentElement->next)
			{
				std::cout << position << "/" << m_listOfBuildingsPlanned->size << " - ID : " << ((struct DataBuildings *)currentElement->data)->m_buildingID << " - Map Position : " << ((struct DataBuildings *)currentElement->data)->m_mapPosition.x << " " << ((struct DataBuildings *)currentElement->data)->m_mapPosition.y << std::endl;

				position++;
			}
		}
	}

	std::cout << std::endl << std::endl;
}

sf::Vector2i BuildingsListPlanned::GetBuildingPositionInMap() const
{
	if (m_listOfBuildingsPlanned != nullptr)
	{
		if (m_listOfBuildingsPlanned->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_listOfBuildingsPlanned->first;

			return ((struct DataBuildings *)currentElement->data)->m_mapPosition;
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

sf::Vector2i BuildingsListPlanned::GetBuildingSize() const
{
	if (m_listOfBuildingsPlanned != nullptr)
	{
		if (m_listOfBuildingsPlanned->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_listOfBuildingsPlanned->first;

			return ((struct DataBuildings *)currentElement->data)->m_buildingSize;
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

int BuildingsListPlanned::GetBuildingID() const
{
	if (m_listOfBuildingsPlanned != nullptr)
	{
		if (m_listOfBuildingsPlanned->first != nullptr)
		{
			LinkedListClass::sElement *currentElement = m_listOfBuildingsPlanned->first;

			return ((struct DataBuildings *)currentElement->data)->m_buildingID;
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
	((struct DataBuildings *)newBuilding->data)->m_mapPosition = _mapPosition;

	// Init of the worker status
	((struct DataBuildings *)newBuilding->data)->m_buildingID = _buildingID;
	
	((struct DataBuildings *)newBuilding->data)->m_buildingSize = _buildingSize;
	
	newBuilding->status = ELEMENT_ACTIVE;


	// Add this new building at the end of the list
	AddElementToLinkedList(m_listOfBuildingsPlanned, newBuilding, -1);
	//ReadBuildingsPlannedToList();
}


void BuildingsListPlanned::DeleteCurrentFirstBuildingInList()
{
	if (m_listOfBuildingsPlanned != nullptr)
	{
		if (m_listOfBuildingsPlanned->first != nullptr)
		{
			// Deletion of the first element of the list
			RemoveElementsOfLinkedList(m_listOfBuildingsPlanned, true, 1);
		}
	}
}