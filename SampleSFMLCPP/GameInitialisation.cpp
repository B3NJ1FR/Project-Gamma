#include "GameInitialisation.h"
#include "GameDefinitions.h"
#include "InitBuildingsFromFile.h"
#include "LinkedList.h"
#include "Pathfinding.h" // Temporaire


void MapInitialisation(struct Game *_game)
{
	std::ifstream mapInitialisationFile("Data/Configurations/Settings.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();
	
	bool isMapHasBeenDefined = false;

	if (!mapInitialisationFile.is_open())
	{
		std::cout << "Error accessing Settings.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!mapInitialisationFile.eof())
	{
		mapInitialisationFile >> temporaryString;

		if (temporaryString == "MAP"
			&& isMapHasBeenDefined == false)
		{
			// Read the map size infos
			mapInitialisationFile >> _game->numberColumns >> _game->numberLines >> _game->numberLayers;

			std::cout << "Map size : " << _game->numberColumns << " " << _game->numberLines << " " << _game->numberLayers << std::endl;

			isMapHasBeenDefined = true;
			temporaryString.erase();
		}
	}


	// Temporary, to define into a config file in the futur
	/*_game->numberLayers = THIRD_FLOOR;
	_game->numberLines = 35;
	_game->numberColumns = 40;*/

	if (isMapHasBeenDefined == true)
	{
		_game->map = new unsigned short **[_game->numberLayers];

		for (int i = 0; i < _game->numberLayers; i++)
		{
			_game->map[i] = new unsigned short *[_game->numberLines];
		}

		for (int i = 0; i < _game->numberLayers; i++)
		{
			for (int j = 0; j < _game->numberLines; j++)
			{
				_game->map[i][j] = new unsigned short[_game->numberColumns];
			}
		}

		for (int z = 0; z < _game->numberLayers; z++)
		{
			for (int y = 0; y < _game->numberLines; y++)
			{
				for (int x = 0; x < _game->numberColumns; x++)
				{
					_game->map[z][y][x] = RESET;

					if (z == (ZERO_FLOOR + COLLISIONS_ID) && z % 3 == COLLISIONS_ID)
					{
						// Collisions of the road
						if (y == 4)
						{
							_game->map[z][y][x] = ROAD;
						}
						else if (y == 5)
						{
							_game->map[z][y][x] = ROAD;
						}
					}

					if (z == (ZERO_FLOOR + SPRITE_ID) && z % 3 == SPRITE_ID)
					{
						// Display of the road
						if (y == 4)
						{
							_game->map[z][y][x] = 5;
						}
						else if (y == 5)
						{
							_game->map[z][y][x] = 6;
						}
						else
						{
							_game->map[z][y][x] = rand() % 2 + 1;
						}
					}

					if (z == (FIRST_FLOOR + SPRITE_ID))
					{
						if (y <= 3)
						{
							bool randomNumber(rand() % 2);

							if (randomNumber == 1)
							{
								_game->map[z][y][x] = 11;
								_game->map[z - 2][y][x] = COLLISION;
							}
							else
							{
								_game->map[z][y][x] = 0;
							}
						}
						
					}

				}
			}
		}
	}
	else
	{
		// ERROR LOG
		std::cout << "ERROR OF MAP DEFINITION\n\n\n";
	}

	
}


void SpritesInitialisation(struct Game *_game)
{
	std::ifstream spritesFile("Data/Configurations/Sprites.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();

	int temporaryNumber(RESET);
	int maximalNumberOfSprites(RESET);
	int previousID(RESET);

	if (!spritesFile.is_open())
	{
		std::cout << "Error accessing Sprites.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!spritesFile.eof())
	{
		spritesFile >> temporaryString;

		if (temporaryString == "SPRITE")
		{
			int temporaryID(RESET);
			spritesFile >> temporaryID;

			if ((temporaryID == 0 && previousID == 0)
				|| temporaryID > previousID)
			{
				maximalNumberOfSprites = temporaryID;
				previousID = temporaryID;
			}
			else
			{
				std::cout << std::endl << std::endl << "\t\tSTEP 1 : The sprite file has an error with the ID : " << temporaryID << std::endl << std::endl << std::endl;
			}
		}
	}

	// Dynamic allocation of the array sprite's
	maximalNumberOfSprites += 1;
	_game->spriteArray = new sf::Sprite[maximalNumberOfSprites];

	// Reset the reading cursor at the begging
	spritesFile.seekg(0, std::ios::beg);
	
	// Reading and loading of the sprites
	while (!spritesFile.eof())
	{
		spritesFile >> temporaryString;

		if (temporaryString == "SPRITE")
		{
			int temporaryID(RESET);
			spritesFile >> temporaryID;

			if (temporaryID < maximalNumberOfSprites)
			{
				spritesFile >> temporaryNumber;
				spritesFile >> temporaryString;
				
				std::cout << temporaryID << " - Sprite " << temporaryString << std::endl;
				_game->spriteArray[temporaryID] = LoadSprite(temporaryString, temporaryNumber);
			}
			else
			{
				std::cout << std::endl << std::endl << "\t\tSTEP 2 : The sprite file has an error with the ID : " << temporaryID << std::endl << std::endl << std::endl;
			}
		}
	}

	spritesFile.close();
}


void TextsInit(struct Game *_game)
{
	//_game->generalFont.loadFromFile("Data/Fonts/Roman SD.ttf");
	//_game->generalFont.loadFromFile("Data/Fonts/CharlemagneStd-Bold.otf");
	_game->generalFont.loadFromFile("Data/Fonts/arial.ttf");
	
	// ---------------- DEBUGGER TEXTS ---------------- 
	LoadTextString(&_game->debbugTexts[0], "Mouse position :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(0, 85));
	LoadTextString(&_game->debbugTexts[1], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(250, 85));
	
	LoadTextString(&_game->debbugTexts[2], "Click position :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(0, 115));
	LoadTextString(&_game->debbugTexts[3], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(250, 115));

	LoadTextString(&_game->debbugTexts[4], "Case coordinates :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(0, 145));
	LoadTextString(&_game->debbugTexts[5], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(250, 145));

	LoadTextString(&_game->debbugTexts[6], "Collisions :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(0, 175));
	LoadTextString(&_game->debbugTexts[7], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(150, 175));
	LoadTextString(&_game->debbugTexts[8], "Building ID :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(300, 175));
	LoadTextString(&_game->debbugTexts[9], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(460, 175));
	LoadTextString(&_game->debbugTexts[10], "Sprite ID :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(600, 175));
	LoadTextString(&_game->debbugTexts[11], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(730, 175));


	// ---------------- UI TEXTS ---------------- 
	LoadTextString(&_game->UITexts[0], "Sesterces :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(130, 33));
	LoadTextString(&_game->UITexts[1], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(280, 33));

	LoadTextString(&_game->UITexts[2], "Bunchs of Grape :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(400, 33));
	LoadTextString(&_game->UITexts[3], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(630, 33));

	LoadTextString(&_game->UITexts[4], "Grapes Must :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(800, 33));
	LoadTextString(&_game->UITexts[5], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(975, 33));

	LoadTextString(&_game->UITexts[6], "Grape Juice :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(1100, 33));
	LoadTextString(&_game->UITexts[7], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(1270, 33));

	LoadTextString(&_game->UITexts[8], "Amphora of Wine :", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(1400, 33));
	LoadTextString(&_game->UITexts[9], "", &_game->generalFont, 30, sf::Color::White, sf::Vector2f(1640, 33));

}


void RessourcesInitialisation(struct Game *_game)
{
	std::ifstream ressourcesFile("Data/Configurations/Ressources.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();

	int temporaryNumber(RESET);
	int maximalNumberOfRessources(RESET);
	int previousID(RESET);

	if (!ressourcesFile.is_open())
	{
		std::cout << "Error accessing Ressources.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!ressourcesFile.eof())
	{
		ressourcesFile >> temporaryString;

		if (temporaryString == "RESSOURCE")
		{
			int temporaryID(RESET);
			ressourcesFile >> temporaryID;

			if ((temporaryID == 0 && previousID == 0)
				|| temporaryID > previousID)
			{
				maximalNumberOfRessources = temporaryID;
				previousID = temporaryID;
			}
			else
			{
				std::cout << std::endl << std::endl << "\t\tSTEP 1 : The sprite file has an error with the ID : " << temporaryID << std::endl << std::endl << std::endl;
			}
		}
	}

	// Dynamic allocation of the array sprite's
	maximalNumberOfRessources += 1;
	_game->ressources = new Ressources[maximalNumberOfRessources];

	// Reset the reading cursor at the begging
	ressourcesFile.seekg(0, std::ios::beg);

	std::cout << "\n\n\n\tRessources loading ...\n\n";

	// Reading and loading of the sprites
	while (!ressourcesFile.eof())
	{
		ressourcesFile >> temporaryString;

		if (temporaryString == "RESSOURCE")
		{
			// We pickup the ressource id
			int temporaryID(RESET);
			ressourcesFile >> temporaryID;
			
			// We pickup the ressource name
			ressourcesFile >> temporaryString;
			_game->ressources[temporaryID].SetName(temporaryString);

			// We pickup the starting quantity
			ressourcesFile >> temporaryNumber;
			_game->ressources[temporaryID].SetInitialQuantityOwned(temporaryNumber);
			std::cout << temporaryID << "/" << maximalNumberOfRessources << " - " << temporaryString << " " << temporaryNumber << std::endl;


			ressourcesFile >> temporaryString;

			if (temporaryString == "SPRITE")
			{
				// We pickup the ressource name
				ressourcesFile >> temporaryString;
				_game->ressources[temporaryID].SetSprite(temporaryString);
			}
		}
	}

	ressourcesFile.close();

	std::cout << "\n\tRessources loaded !\n\n\n";
}


void GameInitialisation(struct Game *_game)
{
	_game->actualGameState = NORMAL_MODE;

	_game->numberOfBuilding = RESET;
	
	InitBuildingsFromFile(_game);
	MapInitialisation(_game);
	SpritesInitialisation(_game);
	TextsInit(_game);
	RessourcesInitialisation(_game);

	_game->time = new TimeManagement(&_game->generalFont);
	_game->workersList = new WorkersList;
	_game->purchasers = new Purchasers;
	_game->stall = new Stalls(&_game->generalFont, &_game->buildings[BUILDING_STALL]);

	
	_game->camera.x = -15;
	_game->camera.y = -15;
	_game->camera.z = RESET;
	
	_game->scale.x = 1 / (1 - _game->camera.z);
	_game->scale.y = 1 / (1 - _game->camera.z);

	_game->isDebuggerModeActive = false;

	_game->IDChosenBuilding = RESET;

	_game->isBuildingCaseOccupied = false;

	_game->vines.InitialisationVines(&_game->buildings[BUILDING_VINES]);
	_game->stompingVats.InitialisationSpeBuilding(&_game->buildings[BUILDING_GRAPE_STOMPING_VATS]);
	_game->winePress.InitialisationSpeBuilding(&_game->buildings[BUILDING_WINE_PRESS]);
	_game->wineStorehouse.InitialisationSpeBuilding(&_game->buildings[BUILDING_WINE_STOREHOUSE]);

	_game->workersList->InitialisationWorkersList();

	_game->buildingUI = LoadSprite("Data/Assets/Sprites/Menu/old_scroll_test.png", 0);
	_game->buildingUIclosed = LoadSprite("Data/Assets/Sprites/Menu/old_scroll_closed.png", 0);
	_game->buildingUIdestroyBuildings = LoadSprite("Data/Assets/Sprites/Menu/destroy_buildings.png", 0);
	_game->blackFilter = LoadSprite("Data/Assets/Sprites/Menu/black_layer.png", 0);
	_game->contour = LoadSprite("Data/Assets/Sprites/Menu/contour_selection.png", 0);
	_game->workerTest = LoadSprite("Data/Assets/Sprites/Entities/worker_test.png", 5);

	_game->workersIcons[0] = LoadSprite("Data/Assets/Sprites/Entities/worker_selected.png", 1);
	_game->workersIcons[1] = LoadSprite("Data/Assets/Sprites/Entities/worker_waiting.png", 1);
	_game->workersIcons[2] = LoadSprite("Data/Assets/Sprites/Entities/worker_working.png", 1);
	_game->workersIcons[3] = LoadSprite("Data/Assets/Sprites/Entities/worker_pickuping.png", 1);
	_game->workersIcons[4] = LoadSprite("Data/Assets/Sprites/Entities/worker_depositing.png", 1);

}

