#pragma once

#include "Common.hpp"
#include "System.h"
#include "GameDefinitions.h"
#include "MainMenu.h"
#include "LoadingScreen.h"

struct Data
{
	System* m_system;
	Game* m_game;
	MainMenu* m_mainMenu;
	LoadingScreen* m_loadingScreen;

	GeneralState m_state;

	// Constructor & Destructor
	Data();
	~Data();

	// Methods
	void GameState();
};