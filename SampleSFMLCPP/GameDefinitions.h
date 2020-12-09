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
public:
	// Shortcut of the window address
	sf::RenderWindow *m_window;

	enum CurrentGameState m_actualGameState;
	enum CurrentGameState m_previousGameState;

	// Tutorial Window
	TutorialWindow *m_tutorialWindow;

	// Buildings
	BuildingManagement m_builds;

	// Sprites Array
	sf::Sprite* m_spriteArray;

	// Map
	Map m_map;

	// Camera
	sf::Vector3f m_camera;
	sf::Vector2f m_scale;

	// Ressources
	Ressources* m_ressources;
	unsigned int m_numberTypesOfRessources;

	// Fonts
	sf::Font m_generalFont;
	sf::Font m_charlemagneFont;

	//Debugger
	bool m_isDebuggerModeActive;
	sf::Vector2f m_mouseClicked;
	sf::Text m_debbugTexts[12];

	// General UI
	sf::Text m_UITexts[10];

	// Time management
	TimeManagement *m_time;

	// Workers
	WorkersList *m_workersList;
	Purchasers *m_purchasers;
	MainCharacter *m_mainCharacter;

	// Selling Window
	SellingWindow *m_sellingWindow;
	
	// Construction Mode
	BuildWindow m_buildWindow; // Verifier pour la money
	BuildingsListPlanned *buildingsListPlanned;
	
	
	//VillaManagement villaManagement;


	//// Money
	//Money money;




	//// Save and Load
	//SavingGame save;
	//LoadingGame load;

	//// Pause
	//PauseWindow pauseWindow;

	//// Temporary
	//Pathfinding path;

public :
	// Constructor & Destructor
	Game();
	~Game();

	// Setters
	inline void SetWindowMemoryAddress(sf::RenderWindow* window);

	// Methods
	void SpritesInitialisation();
	void TextsInit();
	void RessourcesInitialisation();
};
