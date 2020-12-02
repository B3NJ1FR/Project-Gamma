#pragma once

#include "Definitions.hpp"

enum ListOfResolutions
{
	SR_1280X720, // 16:9
	SR_1366X768, // 16:9
	SR_1440X900, // 8:5
	SR_1600X1200, // 4:3
	SR_1680X1050, // 8:5
	SR_1920X1080, // 16:9
	SR_2048X1080, // 17:9
	SR_2560X1440, // 16:9
	SR_3440X1440, // 21:9

	SR_NUMBER_MAX,
};


struct System
{
	// Window Management
	enum ListOfResolutions currentScreenResolution;
	std::string listOfScreenResolutions[SR_NUMBER_MAX];
	sf::Vector2i screenResolution;
	sf::RenderWindow window;

	System();

	void RandomTime();
	sf::Vector2i FindWindowResolution(enum ListOfResolutions _screenResolution) const;
	void PrimaryWindowInitialisation();
	void ChangeWindowResolution(const sf::Vector2i& _newResolution, enum ListOfResolutions _screenResInList);
};

