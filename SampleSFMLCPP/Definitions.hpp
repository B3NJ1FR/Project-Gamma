#ifndef DEFINITIONS__HPP
#define DEFINITIONS__HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>

#define RESET 0

enum GeneralState
{
	MAIN_STATE_MENU,
	MAIN_STATE_GAME,
	MAIN_STATE_EXIT,
};


struct System
{
	// Window Management
	sf::RenderWindow window;
};


struct Data
{
	struct System *system;
	struct Game *game;
	struct Menu *mainMenu;

	enum GeneralState state;
};



#endif // !DEFINITIONS__HPP

