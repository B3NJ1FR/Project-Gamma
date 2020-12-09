#pragma once

#include "Common.hpp"

#include "Buildings.h"
#include "Vines.h"
#include "SpecificsBuildings.h"
#include "Stalls.h"
#include "Storehouse.h"


struct BuildingManagement
{
public :
	unsigned short numberOfBuilding;
	Buildings* buildings;
	Vines vines;
	SpecificsBuildings stompingVats;
	SpecificsBuildings winePress;
	SpecificsBuildings wineStorehouse;
	Stalls* stall;
	Storehouse storehouse;
	sf::Text* buildingsNameTexts;

	// Constructor & Destructor
	BuildingManagement();
	~BuildingManagement();

	// Methods
	void InitBuildingsFromFile();
};

