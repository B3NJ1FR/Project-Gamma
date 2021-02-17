#pragma once

#include "Common.hpp"
#include "System.h"
#include "GameDefinitions.h"
#include "MainMenu.h"
#include "LoadingScreen.h"

struct Data
{
	System* m_system = nullptr;
	Game* m_game = nullptr;
	MainMenu* m_mainMenu = nullptr;
	LoadingScreen* m_loadingScreen = nullptr;

	GeneralState m_state;

	// Constructor & Destructor
	Data();
	~Data();


	// Create
	void CreateNewSystem();
	void CreateNewMainMenu();
	void CreateNewLoadingScreen();
	void CreateNewGame();

	// Delete
	void DeleteMainMenu();
	void DeleteGame();

	// Methods
	void GameState();
};