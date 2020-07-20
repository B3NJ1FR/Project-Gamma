#include "Display.hpp"


void Display(Data *_data, MainMenu *_mainMenu)
{
	sf::Color color = { 0, 0, 0, 0 };
	_data->system->window.clear(color);


		
	//_mainMenu->DisplayMainMenu(_data);

	_data->system->window.display();
}

