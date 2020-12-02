#pragma once

#include "Common.hpp"

#include "Buildings.h"
#include "Vines.h"
#include "SpecificsBuildings.h"
#include "Stalls.h"
#include "Storehouse.h"


class BuildingManagement
{
private :
	unsigned short numberOfBuilding;
	Buildings* buildings;
	Vines vines;
	SpecificsBuildings stompingVats;
	SpecificsBuildings winePress;
	SpecificsBuildings wineStorehouse;
	Stalls* stall;
	Storehouse storehouse;
	sf::Text* buildingsNameTexts;

public:
	// Constructor & Destructor
	BuildingManagement();
	~BuildingManagement();

	// Methods
	void InitBuildingsFromFile();
};

