#pragma once

#include "Common.hpp"
#include "System.h"
#include "GameDefinitions.h"
#include "MainMenu.h"
#include "LoadingScreen.h"

struct Data
{
	System* system;
	Game* game;
	MainMenu* mainMenu;
	LoadingScreen* loadingScreen;

	GeneralState state;

	Data();
	~Data();
};