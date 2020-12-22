#include "Data.h"

int main()
{
	Data data;

	while (data.m_system->m_window.isOpen())
	{	
		switch (data.m_state)
		{
		case MAIN_STATE_MENU:
			data.m_mainMenu->MainMenuState(data.m_system, &data.m_state);
			break;
		case MAIN_STATE_LOAD_GAME:
			data.GameState();
			break;
		case MAIN_STATE_GAME:
			data.GameState();
			break;
		case MAIN_STATE_EXIT:
			break;
		default:
			std::cout << "Error main state\n\n";
			data.m_mainMenu->MainMenuState(data.m_system, &data.m_state);
			break;
		}
	}


	return 0;
}


