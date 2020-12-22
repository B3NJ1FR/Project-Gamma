#pragma once

#include "Common.hpp"
#include "Vines.h"
#include "SpecificsBuildings.h"
#include "Stalls.h"
#include "Storehouse.h"


class BuildingManagement
{
private :
	unsigned short m_numberOfBuilding;

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

	// Setters
	inline void SetNumberOfBuildings(unsigned short _numberOfBuildings) { m_numberOfBuilding = _numberOfBuildings; };
	
	// Methods
	void InitBuildingsFromFile();
};

