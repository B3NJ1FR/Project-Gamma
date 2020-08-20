#ifndef MAIN_MENU__H
#define MAIN_MENU__H

#include "Common.hpp"

enum MainMenuState
{
	MAIN_MENU_OPERATIONAL,
	MAIN_MENU_EXIT,
};

class MainMenu
{
private:
	enum MainMenuState state;

	sf::Sprite background;
	sf::Sprite buttonPlay;
	sf::Sprite buttonOptions;
	sf::Sprite buttonQuit;

	sf::Text versionNumber;
	sf::Font font;

public:

	MainMenu();
	~MainMenu();

	void InputMainMenu(sf::RenderWindow &_window, enum GeneralState *_generalState);
	void UpdateMainMenu();
	void DisplayMainMenu(sf::RenderWindow &_window);


};


struct Menu
{
	MainMenu menu;
};

void MainMenuState(Data *_data);


#endif // !MAIN_MENU__H
