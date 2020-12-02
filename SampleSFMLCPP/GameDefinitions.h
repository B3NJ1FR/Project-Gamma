#pragma once

#include "LoadingScreen.h"
#include "BuildingManagement.h"
#include "Map.h"

#include "Money.h"
#include "TimeManagement.h"
#include "Pathfinding.h"
#include "WorkersList.h"
#include "Purchasers.h"
#include "MainCharacter.h"
#include "SellingWindow.h"
#include "VillaManagement.h"
#include "SavingGame.h"
#include "LoadingGame.h"
#include "PauseWindow.h"
#include "BuildWindow.h"
#include "TutorialWindow.h"
#include "BuildingsListPlanned.h"

#define MAX_ZOOMING 0.3f
#define MAX_DEZOOMING -0.3f

#define CAMERA_SPEED 5

enum CurrentGameState
{
	TUTORIAL_MODE,
	NORMAL_MODE,
	BUILD_MODE,
	TEST_PATHFINDING_MODE,
	SELLING_WINDOW,
	PAUSE_WINDOW,
	VILLA_MANAGEMENT,
	ESTATE_DATA_N_STATISTICS,
};




struct Game
{
	sf::RenderWindow *window;

	enum CurrentGameState actualGameState;
	enum CurrentGameState previousGameState;

	TutorialWindow *tutorialWindow;

	// Buildings
	BuildingManagement builds;

	sf::Sprite* spriteArray;

	// Map
	Map map;

	// Camera
	sf::Vector3f camera;
	sf::Vector2f scale;

	//Debugger
	bool isDebuggerModeActive;
	sf::Vector2f mouseClicked;
	sf::Text debbugTexts[12];

	sf::Font generalFont;
	sf::Font charlemagneFont;

	// Selling Window
	SellingWindow *sellingWindow;
	
	// Construction Mode
	BuildWindow buildWindow;
	BuildingsListPlanned *buildingsListPlanned;
	VillaManagement villaManagement;


	// General UI
	sf::Text UITexts[10];

	// Debugger



	// Money
	Money money;

	// Ressources
	Ressources *ressources;
	unsigned int numberTypesOfRessources;
	
	// Time management
	TimeManagement *time;


	// Workers
	WorkersList *workersList;
	Purchasers *purchasers;
	MainCharacter *mainCharacter;


	// Save and Load
	SavingGame save;
	LoadingGame load;

	// Pause
	PauseWindow pauseWindow;

	// Temporary
	Pathfinding path;

	Game(LoadingScreen* _loadingScreen);
};
