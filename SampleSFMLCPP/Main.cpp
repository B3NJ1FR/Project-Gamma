//#include "Common.hpp"
//#include "Update.hpp"
//#include "Display.hpp"
//#include "Input.hpp"
//#include "MainMenu.h"

#include "Data.h"
//#include "GameState.h"

int main()
{
	Data data;

	while (data.system->window.isOpen())
	{	
		switch (data.state)
		{
		case MAIN_STATE_MENU:
			data.mainMenu->MainMenuState(data.system, &data.state);
			break;
		case MAIN_STATE_LOAD_GAME:
			GameState(data.game, data.loadingScreen, &data.state);
			break;
		case MAIN_STATE_GAME:
			GameState(data.game, data.loadingScreen, &data.state);
			break;
		case MAIN_STATE_EXIT:
			break;
		default:
			std::cout << "Error main state\n\n";
			break;
		}

	}


	return 0;
}


