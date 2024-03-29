#include "PauseWindow.h"
#include "GameDefinitions.h"

PauseWindow::PauseWindow()
{
	m_blackBackground = LoadSprite("Data/Assets/Sprites/Menu/blackbackground.png", 0);
	m_resumeButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Resume.png", 1);
	m_loadButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Load.png", 1);
	m_saveButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Save.png", 1);
	m_saveNQuitButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Save_N_Quit.png", 1);
	m_quitButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Quit.png", 1);
}

PauseWindow::~PauseWindow()
{
	delete (m_blackBackground.getTexture());
	delete (m_resumeButton.getTexture());
	delete (m_loadButton.getTexture());
	delete (m_saveButton.getTexture());
	delete (m_saveNQuitButton.getTexture());
	delete (m_quitButton.getTexture());

}

void PauseWindow::DisplayPauseWindow(sf::RenderWindow &_window, const sf::Vector2i &_screenResolution)
{
	sf::Color color = { 255, 255, 255, 175 };
	m_blackBackground.setColor(color);
	BlitSprite(m_blackBackground, 0, 0, 0, _window);

	sf::Color newColor = { 255, 255, 255, 255 };
	m_blackBackground.setColor(newColor);

	sf::Vector2f newScale = { _screenResolution.y / 1080.f, _screenResolution.y / 1080.f };

	m_resumeButton.setScale(newScale);
	m_loadButton.setScale(newScale);
	m_saveButton.setScale(newScale);
	m_saveNQuitButton.setScale(newScale);
	m_quitButton.setScale(newScale);

	BlitSprite(m_resumeButton, _screenResolution.x / 2, _screenResolution.y / 6, 0, _window);
	BlitSprite(m_loadButton, _screenResolution.x / 2, (_screenResolution.y / 6) * 2, 0, _window);
	BlitSprite(m_saveButton, _screenResolution.x / 2, (_screenResolution.y / 6) * 3, 0, _window);
	BlitSprite(m_saveNQuitButton, _screenResolution.x / 2, (_screenResolution.y / 6) * 4, 0, _window);
	BlitSprite(m_quitButton, _screenResolution.x / 2, (_screenResolution.y / 6) * 5, 0, _window);

}

void PauseWindow::InputPauseWindow(struct Game *_game, SavingGame *_save, LoadingGame *_load)
{
	sf::Event event;
	while (_game->m_window->pollEvent(event))
	{
		// Closing by pressing the Close button
		if (event.type == sf::Event::Closed)
		{
			exit(EXIT_SUCCESS);
		}

		if (event.type == sf::Event::KeyPressed)
		{
			// If we pressed the escape key, we close the game
			if (event.key.code == sf::Keyboard::Escape)
			{
				if (_game->m_previousGameState == TUTORIAL_MODE)
				{
					_game->m_actualGameState = TUTORIAL_MODE;
				}
				else if (_game->m_previousGameState == NORMAL_MODE)
				{
					_game->m_actualGameState = NORMAL_MODE;
				}
				else if (_game->m_previousGameState == BUILD_MODE)
				{
					_game->m_actualGameState = BUILD_MODE;
				}
				else if (_game->m_previousGameState == SELLING_WINDOW)
				{
					_game->m_actualGameState = SELLING_WINDOW;
				}
				else if (_game->m_previousGameState == VILLA_MANAGEMENT)
				{
					_game->m_actualGameState = VILLA_MANAGEMENT;
				}
				else if (_game->m_previousGameState == ESTATE_DATA_N_STATISTICS)
				{
					_game->m_actualGameState = ESTATE_DATA_N_STATISTICS;
				}
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			// If we pressed the escape key, we close the game
			if (event.key.code == sf::Mouse::Left)
			{
				sf::Vector2i mousePostionAtScreen = sf::Mouse::getPosition(*_game->m_window);

				// Button Resume
				if (mousePostionAtScreen.x > _game->m_screenReso->x / 2 - (m_resumeButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < _game->m_screenReso->x / 2 + (m_resumeButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y > _game->m_screenReso->y / 6 - (m_resumeButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < _game->m_screenReso->y / 6 + (m_resumeButton.getGlobalBounds().height / 2))
				{
					if (_game->m_previousGameState == TUTORIAL_MODE)
					{
						_game->m_actualGameState = TUTORIAL_MODE;
					}
					else if (_game->m_previousGameState == NORMAL_MODE)
					{
						_game->m_actualGameState = NORMAL_MODE;
					}
					else if (_game->m_previousGameState == BUILD_MODE)
					{
						_game->m_actualGameState = BUILD_MODE;
					}
					else if (_game->m_previousGameState == SELLING_WINDOW)
					{
						_game->m_actualGameState = SELLING_WINDOW;
					}
					else if (_game->m_previousGameState == VILLA_MANAGEMENT)
					{
						_game->m_actualGameState = VILLA_MANAGEMENT;
					}
					else if (_game->m_previousGameState == ESTATE_DATA_N_STATISTICS)
					{
						_game->m_actualGameState = ESTATE_DATA_N_STATISTICS;
					}
				}

				// Button Load
				if (mousePostionAtScreen.x > _game->m_screenReso->x / 2 - (m_saveButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < _game->m_screenReso->x / 2 + (m_saveButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y >(_game->m_screenReso->y / 6) * 2 - (m_loadButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < (_game->m_screenReso->y / 6) * 2 + (m_loadButton.getGlobalBounds().height / 2))
				{
					_load->LoadTheGame(_game);
				}

				// Button Save
				if (mousePostionAtScreen.x > _game->m_screenReso->x / 2 - (m_saveButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < _game->m_screenReso->x / 2 + (m_saveButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y >(_game->m_screenReso->y / 6) * 3 - (m_saveButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < (_game->m_screenReso->y / 6) * 3 + (m_saveButton.getGlobalBounds().height / 2))
				{
					_save->SaveTheGame(_game);
				}

				// Button Save & Quit
				if (mousePostionAtScreen.x > _game->m_screenReso->x / 2 - (m_saveNQuitButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < _game->m_screenReso->x / 2 + (m_saveNQuitButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y >(_game->m_screenReso->y / 6) * 4 - (m_saveNQuitButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < (_game->m_screenReso->y / 6) * 4 + (m_saveNQuitButton.getGlobalBounds().height / 2))
				{
					_save->SaveTheGame(_game);
					_game->m_window->close();
					exit(EXIT_SUCCESS);
				}

				// Button Quit
				if (mousePostionAtScreen.x > _game->m_screenReso->x / 2 - (m_saveNQuitButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < _game->m_screenReso->x / 2 + (m_saveNQuitButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y >(_game->m_screenReso->y / 6) * 5 - (m_saveNQuitButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < (_game->m_screenReso->y / 6) * 5 + (m_saveNQuitButton.getGlobalBounds().height / 2))
				{
					*_game->m_generalState = MAIN_STATE_MENU;

					std::cout << "\n\n\n\nBack to the Main Menu ....\n\n\n\n\n";
				}
			}
		}
	}
}
