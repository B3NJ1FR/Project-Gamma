#ifndef GAME_DEFINITIONS__H
#define GAME_DEFINITIONS__H

#include "Money.h"
#include "TimeManagement.h"
#include "Vines.h"
#include "SpecificsBuildings.h"
#include "Pathfinding.h"
#include "WorkersList.h"

#define MAX_ZOOMING 0.3f
#define MAX_DEZOOMING -0.3f

#define CAMERA_SPEED 5

enum Dimensions
{
	COLLISIONS_ID,
	BUILDING_ID,
	SPRITE_ID,
};

enum Floors
{
	ZERO_FLOOR,
	FIRST_FLOOR = 3,
	SECOND_FLOOR = 6,
	THIRD_FLOOR = 9,
};

enum TypesOfCollisions
{
	NO_COLLISION,
	COLLISION,
	BUILDING_ENTRANCE,
	BUILDING_EXIT,
	PATH,
	STONE_PATH,
	ROAD,
};

enum GameState
{
	NORMAL_MODE,
	BUILD_MODE,
	TEST_PATHFINDING_MODE,
};


struct Game
{
	sf::RenderWindow *window;

	enum GameState actualGameState;

	// Buildings
	unsigned short numberOfBuilding;
	Buildings *buildings;
	Vines vines;
	SpecificsBuildings stompingVats;
	SpecificsBuildings winePress;
	SpecificsBuildings wineStorehouse;
	

	sf::Sprite *spriteArray;

	// Map
	unsigned short ***map;
	unsigned short numberLayers;
	unsigned short numberLines;
	unsigned short numberColumns;

	// Camera
	sf::Vector3f camera;
	sf::Vector2f scale;

	//Debugger
	bool isDebuggerModeActive;
	sf::Vector2f mouseClicked;
	sf::Text debbugTexts[12];

	sf::Font generalFont;

	
	// Construction Mode
	int IDChosenBuilding;
	sf::Vector2i buildingCaseSelected;
	bool isBuildingCaseOccupied;

	sf::Sprite buildingUI;
	sf::Sprite buildingUIclosed;
	sf::Sprite buildingUIdestroyBuildings;
	sf::Sprite blackFilter;
	sf::Sprite contour;

	// General UI
	sf::Text UITexts[10];

	// Debugger



	// Money
	Money money;

	// Ressources
	Ressources *ressources;
	
	// Time management
	TimeManagement time;


	// Workers
	sf::Sprite workerTest;
	sf::Sprite workersIcons[5];
	WorkersList *workersList;


	// Temporary

	Pathfinding path;
};

#endif