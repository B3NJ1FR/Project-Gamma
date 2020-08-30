#include "Common.hpp"
#include "Initialisation.hpp"
#include "Update.hpp"
#include "Display.hpp"
#include "Input.hpp"
#include "MainMenu.h"
#include "GameState.h"


int main()
{
	Data data;
	
	Init(&data);

	while (data.system->window.isOpen())
	{	
		switch (data.state)
		{
		case MAIN_STATE_MENU:
			MainMenuState(&data);
			break;
		case MAIN_STATE_LOAD_GAME:
			GameState(data.game, &data.state);
			break;
		case MAIN_STATE_GAME:
			GameState(data.game, &data.state);
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


