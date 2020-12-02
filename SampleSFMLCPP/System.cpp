#include "System.h"


System::System()
{
	for (int i = 0; i < SR_NUMBER_MAX; i++)
	{
		switch (i)
		{
		case SR_1280X720:
			listOfScreenResolutions[i] = "1280 x 720";
			break;
		case SR_1366X768:
			listOfScreenResolutions[i] = "1366 x 768";
			break;
		case SR_1440X900:
			listOfScreenResolutions[i] = "1440 x 900";
			break;
		case SR_1600X1200:
			listOfScreenResolutions[i] = "1600 x 1200";
			break;
		case SR_1680X1050:
			listOfScreenResolutions[i] = "1680 x 1050";
			break;
		case SR_1920X1080:
			listOfScreenResolutions[i] = "1920 x 1080";
			break;
		case SR_2048X1080:
			listOfScreenResolutions[i] = "2048 x 1080";
			break;
		case SR_2560X1440:
			listOfScreenResolutions[i] = "2560 x 1440";
			break;
		case SR_3440X1440:
			listOfScreenResolutions[i] = "3440 x 1440";
			break;
		default:
			listOfScreenResolutions[i] = "1920 x 1080";
			break;
		}
	}

	screenResolution = sf::Vector2i(1920, 1080);
	currentScreenResolution = SR_1920X1080;
}


void System::RandomTime()
{
	srand((unsigned int)time(NULL));
	rand();
	rand();
	rand();
	rand();
}

sf::Vector2i System::FindWindowResolution(enum ListOfResolutions _screenResolution) const
{
	switch (_screenResolution)
	{
	case SR_1280X720:
		return sf::Vector2i(1280, 720);
		break;
	case SR_1366X768:
		return sf::Vector2i(1366, 768);
		break;
	case SR_1440X900:
		return sf::Vector2i(1440, 900);
		break;
	case SR_1600X1200:
		return sf::Vector2i(1600, 1200);
		break;
	case SR_1680X1050:
		return sf::Vector2i(1680, 1050);
		break;
	case SR_1920X1080:
		return sf::Vector2i(1920, 1080);
		break;
	case SR_2048X1080:
		return sf::Vector2i(2048, 1080);
		break;
	case SR_2560X1440:
		return sf::Vector2i(2560, 1440);
		break;
	case SR_3440X1440:
		return sf::Vector2i(3440, 1440);
		break;
	default:
		return sf::Vector2i(1920, 1080);
		break;
	}
}

void System::PrimaryWindowInitialisation()
{
	// Set the maximal frame time limit at 60 FPS
	window.setFramerateLimit(60);

	// Set the window's size
	window.create(sf::VideoMode(screenResolution.x, screenResolution.y, 32), "Vindemia Antiquus", sf::Style::Fullscreen);

	// Set the game's icon
	sf::Image gameIcon;
	gameIcon.loadFromFile("Data/Assets/Vindemia Antiquus_Icon.png");
	window.setIcon(gameIcon.getSize().x, gameIcon.getSize().y, gameIcon.getPixelsPtr());

	std::cout << "Window's creation succeeded\n";

}

void System::ChangeWindowResolution(const sf::Vector2i& _newResolution, enum ListOfResolutions _screenResInList)
{
	if (_newResolution.x > 0 && _newResolution.y > 0)
	{
		currentScreenResolution = _screenResInList;

		// Set the maximal frame time limit at 60 FPS
		window.setFramerateLimit(60);

		// Set the window's size
		screenResolution = _newResolution;
		window.create(sf::VideoMode(screenResolution.x, screenResolution.y, 32), "Vindemia Antiquus");

		// Set the game's icon
		sf::Image gameIcon;
		gameIcon.loadFromFile("Data/Assets/Vindemia Antiquus_Icon.png");
		window.setIcon(gameIcon.getSize().x, gameIcon.getSize().y, gameIcon.getPixelsPtr());

		std::cout << "New window's creation succeeded\n";
	}
}