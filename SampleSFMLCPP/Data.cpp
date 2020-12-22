#include "Data.h"


// TEMPORARY
#include "GameInput.h"
#include "GameUpdate.h"

Data::Data()
{
	// Allocation of every parts
	m_system = new System;
	m_mainMenu = new MainMenu();
	m_loadingScreen = new LoadingScreen;

	m_state = MAIN_STATE_MENU;

	m_system->ReadScreenResolutionFromFile();
	m_system->PrimaryWindowInitialisation();


	// A activer
	//system->RandomTime();
}

Data::~Data()
{
	// Desallocation of every parts
	delete m_system;
	delete m_mainMenu;
	delete m_loadingScreen;
	delete m_game;
}





void Data::GameState()
{
	// Display the loading screen during the game loading
	m_loadingScreen->DisplayLoadingScreen(m_system->m_window);

	// Initialisation
	m_game = new Game();

	// Shortcut of the window address put into the game struct
	std::cout << "Memory Adress before: " << &m_system->m_window;
	m_game->SetWindowMemoryAddress(&m_system->m_window);
	//m_game->m_window = &m_system->window;
	

	if (m_state == MAIN_STATE_LOAD_GAME)
	{
		// Load the game with the last save from the Save.txt's file
		m_game->m_load.LoadTheGame(m_game);

		m_state = MAIN_STATE_GAME;
	}

	while (m_state == MAIN_STATE_GAME)
	{
		// Input
		// TEMPORARY
		GameInput(m_game);

		// Update
		// TEMPORARY
		GameUpdate(m_game);

		// Display
		m_game->Display();
	}
}