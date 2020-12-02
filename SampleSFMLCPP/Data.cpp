#include "Data.h"

Data::Data()
{
	// Allocation of every parts
	system = new System;
	mainMenu = new MainMenu();
	loadingScreen = new LoadingScreen;


	// Part to move after having pressed the play button's
	game = new Game;

	// Shortcut of the window address put into the game struct
	game->window = &system->window;


	state = MAIN_STATE_MENU;

	system->PrimaryWindowInitialisation();


	// A activer
	//system->RandomTime();
}

Data::~Data()
{
	// Desallocation of every parts
	delete system;
	delete mainMenu;
	delete loadingScreen;
	delete game;
}



