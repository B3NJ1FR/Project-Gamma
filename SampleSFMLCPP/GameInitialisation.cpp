#include "GameInitialisation.h"
#include "GameDefinitions.h"
#include "LoadingScreen.h"
#include "InitBuildingsFromFile.h"
#include "LinkedList.h"
#include "Pathfinding.h" // Temporaire



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
	spriteArray = new sf::Sprite[maximalNumberOfSprites];

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
				spriteArray[temporaryID] = LoadSprite(temporaryString, temporaryNumber);
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
	_game->charlemagneFont.loadFromFile("Data/Fonts/CharlemagneStd-Bold.otf");
	
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
	LoadTextString(&_game->UITexts[0], "Sesterces :", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(130, 33));
	LoadTextString(&_game->UITexts[1], "", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(305, 33));

	LoadTextString(&_game->UITexts[2], "Bunchs of Grape :", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(435, 33));
	LoadTextString(&_game->UITexts[3], "", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(720, 33));

	LoadTextString(&_game->UITexts[4], "Grapes Must :", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(840, 33));
	LoadTextString(&_game->UITexts[5], "", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(1055, 33));

	LoadTextString(&_game->UITexts[6], "Grape Juice :", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(1155, 33));
	LoadTextString(&_game->UITexts[7], "", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(1355, 33));

	LoadTextString(&_game->UITexts[8], "Amphora of Wine :", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(1500, 33));
	LoadTextString(&_game->UITexts[9], "", &_game->charlemagneFont, 25, sf::Color::White, sf::Vector2f(1795, 33));

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
	_game->numberTypesOfRessources = maximalNumberOfRessources + 1;
	_game->ressources = new Ressources[_game->numberTypesOfRessources];

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
			std::cout << temporaryID << "/" << _game->numberTypesOfRessources << " - " << temporaryString << " " << temporaryNumber << std::endl;


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


void GameInitialisation(Game *_game, LoadingScreen *_loadingScreen)
{
	//// Display the loading screen during the game loading
	//_loadingScreen->DisplayLoadingScreen(*_game->window);


	//_game->actualGameState = TUTORIAL_MODE;

	//_game->numberOfBuilding = RESET;
	//
	//InitBuildingsFromFile(_game);
	//MapInitialisation(_game);
	//SpritesInitialisation(_game);
	//TextsInit(_game);
	//RessourcesInitialisation(_game);

	//_game->time = new TimeManagement(&_game->charlemagneFont);
	//_game->tutorialWindow = new TutorialWindow(&_game->charlemagneFont);
	//_game->workersList = new WorkersList;
	//_game->mainCharacter = new MainCharacter;
	//_game->stall = new Stalls(&_game->buildings[BUILDING_STALL]);
	//_game->sellingWindow = new SellingWindow(&_game->generalFont);
	//_game->buildingsListPlanned = new BuildingsListPlanned();
	//_game->purchasers = nullptr;

	//
	//_game->camera.x = -15;
	//_game->camera.y = -15;
	//_game->camera.z = RESET;
	//
	//_game->scale.x = 1 / (1 - _game->camera.z);
	//_game->scale.y = 1 / (1 - _game->camera.z);

	//_game->isDebuggerModeActive = false;



	//_game->vines.InitialisationVines(&_game->buildings[BUILDING_VINES]);
	//_game->stompingVats.InitialisationSpeBuilding(&_game->buildings[BUILDING_GRAPE_STOMPING_VATS]);
	//_game->winePress.InitialisationSpeBuilding(&_game->buildings[BUILDING_WINE_PRESS]);
	//_game->wineStorehouse.InitialisationSpeBuilding(&_game->buildings[BUILDING_WINE_STOREHOUSE]);
	//_game->storehouse.InitialisationStorehouse(&_game->buildings[BUILDING_STOREHOUSE]);

	//_game->workersList->InitialisationWorkersList();


	//_game->buildingsNameTexts = nullptr;
	//_game->buildWindow.InitTextsBuildWindow(&_game->charlemagneFont);
}

