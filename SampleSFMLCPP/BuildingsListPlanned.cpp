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

sf::Vector2i BuildingsListPlanned::FindBuildingCorresponding(BuildingManagement* _builds, const sf::Vector2i& _mapPosition, const enum TypeOfBuilding& _buildingID)
{
	sf::Vector2i buildingPosition = { -1, -1 };

	if (m_listOfBuildingsPlanned != nullptr)
	{
		if (m_listOfBuildingsPlanned->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_listOfBuildingsPlanned->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				int buildingID = ((struct DataBuildings*)currentElement->data)->m_buildingID;

				if (_buildingID == buildingID)
				{
					if (_mapPosition.x <= ((struct DataBuildings*)currentElement->data)->m_mapPosition.x
						&& _mapPosition.x >= ((struct DataBuildings*)currentElement->data)->m_mapPosition.x - _builds->m_buildings[buildingID].GetSize().x
						&& _mapPosition.y <= ((struct DataBuildings*)currentElement->data)->m_mapPosition.y
						&& _mapPosition.y >= ((struct DataBuildings*)currentElement->data)->m_mapPosition.y - _builds->m_buildings[buildingID].GetSize().y)
					{
						return (sf::Vector2i)((struct DataBuildings*)currentElement->data)->m_mapPosition;
					}
				}
			}
		}
	}

	return buildingPosition;
}

void BuildingsListPlanned::AddBuildingPlannedToList(const sf::Vector2i &_mapPosition, const enum TypeOfBuilding &_buildingID, const sf::Vector2i &_buildingSize)
{
	LinkedListClass::sElement* newBuilding = new LinkedListClass::sElement;
	newBuilding->data = new struct DataBuildings;

	// Save the position in map
	((struct DataBuildings *)newBuilding->data)->m_mapPosition = _mapPosition;

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

void BuildingsListPlanned::RemoveBuildingAtPrecisePosition(sf::Vector2i& _mapPosition)
{
	if (m_listOfBuildingsPlanned != nullptr)
	{
		if (m_listOfBuildingsPlanned->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = m_listOfBuildingsPlanned->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				int buildingID = ((struct DataBuildings*)currentElement->data)->m_buildingID;

				if (_mapPosition == ((struct DataBuildings*)currentElement->data)->m_mapPosition)
				{
					currentElement->status == ELEMENT_DELETION_REQUIRED;
					break;
				}
			}

			RemoveElementsOfLinkedList(m_listOfBuildingsPlanned);
		}
	}
}

void BuildingsListPlanned::SavingBuildingsListPlannedForFile(std::ofstream* _file)
{
	// Save the number of buildings planned
	int listSize = m_listOfBuildingsPlanned->size;
	_file->write((char*)&listSize, sizeof(int));

	if (m_listOfBuildingsPlanned != nullptr)
	{
		if (m_listOfBuildingsPlanned->first != nullptr)
		{
			LinkedListClass::sElement* currentElement = m_listOfBuildingsPlanned->first;

			for (currentElement = m_listOfBuildingsPlanned->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				// Save the position in map
				sf::Vector2i mapPosition = ((struct DataBuildings*)currentElement->data)->m_mapPosition;
				_file->write((char*)&mapPosition.x, sizeof(int));
				_file->write((char*)&mapPosition.y, sizeof(int));

				// Save the building size
				sf::Vector2i buildingSize = ((struct DataBuildings*)currentElement->data)->m_buildingSize;
				_file->write((char*)&buildingSize.x, sizeof(int));
				_file->write((char*)&buildingSize.y, sizeof(int));

				// Save the building id
				int intNumber = ((struct DataBuildings*)currentElement->data)->m_buildingID;
				_file->write((char*)&intNumber, sizeof(int));
			}
		}
	}
}

void BuildingsListPlanned::LoadingBuildingsListPlannedForFile(std::ifstream* _file)
{
	// Delete the current list
	if (m_listOfBuildingsPlanned != nullptr)
	{
		FreeLinkedList(m_listOfBuildingsPlanned);
	}

	// We reinit the buildings planned list
	m_listOfBuildingsPlanned = LinkedListInitialisation();


	// Load the number of buildings who were currently be built
	int listSize = 0;
	_file->read((char*)&listSize, sizeof(int));

	for (int i = 0; i < listSize; i++)
	{
		LinkedListClass::sElement* newBuilding = new LinkedListClass::sElement;
		newBuilding->data = new struct DataBuildings;

		// Load the position in map
		sf::Vector2i mapPosition;
		_file->read((char*)&mapPosition.x, sizeof(int));
		_file->read((char*)&mapPosition.y, sizeof(int));
		((struct DataBuildings*)newBuilding->data)->m_mapPosition = mapPosition;

		// Load the building size
		sf::Vector2i buildingSize;
		_file->read((char*)&buildingSize.x, sizeof(int));
		_file->read((char*)&buildingSize.y, sizeof(int));
		((struct DataBuildings*)newBuilding->data)->m_buildingSize = buildingSize;

		// Load the building id
		int buildingID(RESET);
		_file->read((char*)&buildingID, sizeof(int));
		((struct DataBuildings*)newBuilding->data)->m_buildingID = buildingID;


		newBuilding->status = ELEMENT_ACTIVE;


		// Add this new building at the end of the list
		AddElementToLinkedList(m_listOfBuildingsPlanned, newBuilding, -1);
	}
}