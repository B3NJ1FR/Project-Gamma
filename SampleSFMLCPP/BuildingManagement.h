#pragma once

#include "Common.hpp"
#include <vector>
#include "Vines.h"
#include "SpecificsBuildings.h"
#include "Stalls.h"
#include "Storehouse.h"
#include "Map.h"


enum class FloorsInBuildingSprites
{
	FIBS_GROUND,
	FIBS_STUDS,
	FIBS_MAIN_FLOOR,
	
	FIBS_ENUM_SIZE,
};


class DoubleVectorsOfSprites : public std::vector<std::vector<sf::Sprite>> {};

class BuildingManagement
{
private :
	unsigned short m_numberOfBuilding;

	std::vector<std::string> *m_vBuildingsSpritesFiles = nullptr;
	DoubleVectorsOfSprites m_vBuildingsSprites;

public :
	Buildings* m_buildings;
	Vines m_vines;
	SpecificsBuildings m_stompingVats;
	SpecificsBuildings m_winePress;
	SpecificsBuildings m_wineStorehouse;
	Stalls* m_stall;
	Storehouse m_storehouse;
	sf::Text* m_buildingsNameTexts;

	// Constructor & Destructor
	BuildingManagement();
	~BuildingManagement();

	// Getters
	inline unsigned short GetNumberOfBuildings() const { return m_numberOfBuilding; };
	sf::Sprite GetSpriteFromBuildID(int _buildingID, int _index) const;

	// Setters
	inline void SetNumberOfBuildings(unsigned short _numberOfBuildings) { m_numberOfBuilding = _numberOfBuildings; };
	
	// Methods
	void InitBuildingsFromFile();
	void InitBuildingsSpritesFromFile();
	void UpdateBuildingManagement(Map* _map);
	
};

