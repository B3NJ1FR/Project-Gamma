#include "System.h"


System::System()
{
	for (int i = 0; i < SR_NUMBER_MAX; i++)
	{
		switch (i)
		{
		case SR_1280X720:
			m_listOfScreenResolutions[i] = "1280 x 720";
			break;
		case SR_1366X768:
			m_listOfScreenResolutions[i] = "1366 x 768";
			break;
		case SR_1440X900:
			m_listOfScreenResolutions[i] = "1440 x 900";
			break;
		case SR_1600X1200:
			m_listOfScreenResolutions[i] = "1600 x 1200";
			break;
		case SR_1680X1050:
			m_listOfScreenResolutions[i] = "1680 x 1050";
			break;
		case SR_1920X1080:
			m_listOfScreenResolutions[i] = "1920 x 1080";
			break;
		case SR_2048X1080:
			m_listOfScreenResolutions[i] = "2048 x 1080";
			break;
		case SR_2560X1440:
			m_listOfScreenResolutions[i] = "2560 x 1440";
			break;
		case SR_3440X1440:
			m_listOfScreenResolutions[i] = "3440 x 1440";
			break;
		default:
			m_listOfScreenResolutions[i] = "1920 x 1080";
			break;
		}
	}

	m_isFullscreen = true;
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


enum ListOfResolutions System::FindEnumFromResolution(const sf::Vector2i& _screenSize) const
{
	if (_screenSize.x == 1280 && _screenSize.y == 720)
	{
		return SR_1280X720;
	}
	else if (_screenSize.x == 1366 && _screenSize.y == 768)
	{
		return SR_1366X768;
	}
	else if (_screenSize.x == 1440 && _screenSize.y == 900)
	{
		return SR_1440X900;
	}
	else if (_screenSize.x == 1600 && _screenSize.y == 1200)
	{
		return SR_1600X1200;
	}
	else if (_screenSize.x == 1680 && _screenSize.y == 1050)
	{
		return SR_1680X1050;
	}
	else if (_screenSize.x == 1920 && _screenSize.y == 1080)
	{
		return SR_1920X1080;
	}
	else if (_screenSize.x == 2048 && _screenSize.y == 1080)
	{
		return SR_2048X1080;
	}
	else if (_screenSize.x == 2560 && _screenSize.y == 1440)
	{
		return SR_2560X1440;
	}
	else if (_screenSize.x == 3440 && _screenSize.y == 1440)
	{
		return SR_3440X1440;
	}
	else
	{
		return SR_1920X1080;
	}
}


void System::ReadScreenResolutionFromFile()
{
	std::ifstream settingsFile("Data/Configurations/Settings.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();
	int screenSizeX = 0, screenSizeY = 0;
	bool isModificationNeeded = false;
	bool isScreenSizeHasBeenFound = false;

	if (!settingsFile.is_open())
	{
		std::cout << "Error accessing Settings.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!settingsFile.eof())
	{
		settingsFile >> temporaryString;

		if (temporaryString == "SCREEN_RESOLUTION")
		{
			isScreenSizeHasBeenFound = true;

			// Read the version number
			settingsFile >> screenSizeX >> screenSizeY;

			if (screenSizeX > 0 && screenSizeY > 0)
			{
				m_screenResolution.x = screenSizeX;
				m_screenResolution.y = screenSizeY;

				m_currentScreenResolution = FindEnumFromResolution(m_screenResolution);
			}
			else
			{
				isModificationNeeded = true;

				m_screenResolution.x = 1920;
				m_screenResolution.y = 1080;

				m_currentScreenResolution = FindEnumFromResolution(m_screenResolution);
			}
		}
		else if (temporaryString == "FULLSCREEN")
		{
			// Read the version number
			settingsFile >> temporaryString;

			if (temporaryString == "ON")
			{
				m_isFullscreen = true;
			}
			else if (temporaryString == "OFF")
			{
				m_isFullscreen = false;
			}
			else
			{
				m_isFullscreen = true;
			}
		}
	}
	
	if (!isScreenSizeHasBeenFound)
	{
		isModificationNeeded = true;

		m_screenResolution.x = 1920;
		m_screenResolution.y = 1080;

		m_currentScreenResolution = FindEnumFromResolution(m_screenResolution);
	}


	settingsFile.close();

	if (isModificationNeeded)
	{
		std::fstream settingsFile("Data/Configurations/Settings.data");

		std::string temporaryString;
		temporaryString.erase();

		if (!settingsFile.is_open())
		{
			std::cout << "Error accessing Settings.data file" << std::endl;

			exit(EXIT_FAILURE);
		}

		while (!settingsFile.eof())
		{
			settingsFile >> temporaryString;

			if (temporaryString == "SCREEN_RESOLUTION")
			{
				// Write the screen size
				int screenSizeX = m_screenResolution.x, screenSizeY = m_screenResolution.y;
				settingsFile << screenSizeX << " " << screenSizeY;

				break;
			}
		}

		if (!isScreenSizeHasBeenFound)
		{
			settingsFile << "SCREEN_RESOLUTION " << m_screenResolution.x << " " << m_screenResolution.y;
		}


		settingsFile.close();
	}
}

void System::PrimaryWindowInitialisation()
{
	// Set the maximal frame time limit at 60 FPS
	m_window.setFramerateLimit(60);

	// Set the window's size
	if (m_isFullscreen)
	{
		m_window.create(sf::VideoMode(m_screenResolution.x, m_screenResolution.y, 32), "Vindemia Antiquus", sf::Style::Fullscreen);
	}
	else
	{
		m_window.create(sf::VideoMode(m_screenResolution.x, m_screenResolution.y, 32), "Vindemia Antiquus");
	}


	// Set the game's icon
	sf::Image gameIcon;
	gameIcon.loadFromFile("Data/Assets/Vindemia Antiquus_Icon.png");
	m_window.setIcon(gameIcon.getSize().x, gameIcon.getSize().y, gameIcon.getPixelsPtr());

	std::cout << "Window's creation succeeded\n";
}

void System::ChangeWindowResolution(const sf::Vector2i& _newResolution, enum ListOfResolutions _screenResInList)
{
	if (_newResolution.x > 0 && _newResolution.y > 0)
	{
		m_currentScreenResolution = _screenResInList;

		// Set the maximal frame time limit at 60 FPS
		m_window.setFramerateLimit(60);

		// Set the window's size
		m_screenResolution = _newResolution;
		if (m_isFullscreen)
		{
			m_window.create(sf::VideoMode(m_screenResolution.x, m_screenResolution.y, 32), "Vindemia Antiquus", sf::Style::Fullscreen);
		}
		else
		{
			m_window.create(sf::VideoMode(m_screenResolution.x, m_screenResolution.y, 32), "Vindemia Antiquus");
		}

		// Set the game's icon
		sf::Image gameIcon;
		gameIcon.loadFromFile("Data/Assets/Vindemia Antiquus_Icon.png");
		m_window.setIcon(gameIcon.getSize().x, gameIcon.getSize().y, gameIcon.getPixelsPtr());

		std::cout << "New window's creation succeeded\n";
	}
}