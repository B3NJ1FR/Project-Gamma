#include "Initialisation.hpp"
#include "MainMenu.h"
#include "LoadingScreen.h"
#include "GameDefinitions.h"

void randomTime()
{
	srand((unsigned int)time(NULL));
	rand();
	rand();
	rand();
	rand();

}

void PrimaryWindowInitialisation(Data *_data)
{
	// Set the maximal frame time limit at 60 FPS
	_data->system->window.setFramerateLimit(60);

	// 1920 x 1080 window size's
	_data->system->window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "Vindemia Antiquus", sf::Style::Fullscreen);

	// 1280 x 720 window size's
	//_data->system.window.create(sf::VideoMode(1280, 720, 32), "Vindemia Antiquus");
	
	// Set the game's icon
	sf::Image gameIcon;
	gameIcon.loadFromFile("Data/Assets/Vindemia Antiquus_Icon.png"); 
	_data->system->window.setIcon(gameIcon.getSize().x, gameIcon.getSize().y, gameIcon.getPixelsPtr());

	std::cout << "Window's creation succeeded\n";
}


void Init(Data *_data)
{
	// A activer
	//randomTime();

	// Allocation of every parts
	_data->system = new struct System;
	_data->mainMenu = new struct Menu;
	_data->loadingScreen = new struct LoadingScreen;


	// Part to move after having pressed the play button's
	_data->game = new struct Game;
	
	// Shortcut of the window address put into the game struct
	_data->game->window = &_data->system->window;

	
	_data->state = MAIN_STATE_MENU;

	PrimaryWindowInitialisation(_data);
}

