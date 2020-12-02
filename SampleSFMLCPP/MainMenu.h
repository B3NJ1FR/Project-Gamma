#pragma once

#include "Common.hpp"
#include "System.h"

enum MainMenuState
{
	MAIN_MENU_OPERATIONAL,
	MAIN_MENU_OPTIONS,
	MAIN_MENU_EXIT,
};

class MainMenu
{
private:
	enum MainMenuState state;

	sf::Sprite logo;
	sf::Sprite background;
	sf::Sprite buttonNewGame;
	sf::Sprite buttonContinue;
	sf::Sprite buttonOptions;
	sf::Sprite buttonQuit;
	sf::Sprite buttonValidate;
	sf::Sprite buttonReturn;
	sf::Sprite warningMessage;

	sf::Text *versionNumber;
	sf::Text *resolution;
	sf::Font font;

	int temporaryResolution;
	bool isNewGameClicked;
	bool isLoadGameClicked;

public:

	MainMenu();
	~MainMenu();

	void InputMainMenu(sf::RenderWindow &_window, enum GeneralState *_generalState, System *_system);
	void UpdateMainMenu(enum GeneralState *_generalState);
	void DisplayMainMenu(sf::RenderWindow &_window);
	void DisplayMainMenuOptions(sf::RenderWindow &_window);

	void MainMenuState(System* _system, enum GeneralState* _state);
};

