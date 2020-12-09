#include "PauseWindow.h"
#include "GameDefinitions.h"

PauseWindow::PauseWindow()
{
	this->blackBackground = LoadSprite("Data/Assets/Sprites/Menu/blackbackground.png", 0);
	this->resumeButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Resume.png", 1);
	this->loadButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Load.png", 1);
	this->saveButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Save.png", 1);
	this->saveNQuitButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Save_N_Quit.png", 1);
	this->quitButton = LoadSprite("Data/Assets/Sprites/Menu/Pause_Window_Button_Quit.png", 1);
}

PauseWindow::~PauseWindow()
{

}

void PauseWindow::DisplayPauseWindow(sf::RenderWindow &_window)
{
	sf::Color color = { 255, 255, 255, 175 };
	this->blackBackground.setColor(color);
	BlitSprite(this->blackBackground, 0, 0, 0, _window);

	sf::Color newColor = { 255, 255, 255, 255 };
	this->blackBackground.setColor(newColor);


	BlitSprite(this->resumeButton, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6, 0, _window);
	BlitSprite(this->loadButton, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 6) * 2, 0, _window);
	BlitSprite(this->saveButton, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 6) * 3, 0, _window);
	BlitSprite(this->saveNQuitButton, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 6) * 4, 0, _window);
	BlitSprite(this->quitButton, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 6) * 5, 0, _window);

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
				if (mousePostionAtScreen.x > SCREEN_WIDTH / 2 - (this->resumeButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < SCREEN_WIDTH / 2 + (this->resumeButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y > SCREEN_HEIGHT / 6 - (this->resumeButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < SCREEN_HEIGHT / 6 + (this->resumeButton.getGlobalBounds().height / 2))
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
				if (mousePostionAtScreen.x > SCREEN_WIDTH / 2 - (this->saveButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < SCREEN_WIDTH / 2 + (this->saveButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y >(SCREEN_HEIGHT / 6) * 2 - (this->loadButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < (SCREEN_HEIGHT / 6) * 2 + (this->loadButton.getGlobalBounds().height / 2))
				{
					_load->LoadTheGame(_game);
				}

				// Button Save
				if (mousePostionAtScreen.x > SCREEN_WIDTH / 2 - (this->saveButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < SCREEN_WIDTH / 2 + (this->saveButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y >(SCREEN_HEIGHT / 6) * 3 - (this->saveButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < (SCREEN_HEIGHT / 6) * 3 + (this->saveButton.getGlobalBounds().height / 2))
				{
					_save->SaveTheGame(_game);
				}

				// Button Save & Quit
				if (mousePostionAtScreen.x > SCREEN_WIDTH / 2 - (this->saveNQuitButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < SCREEN_WIDTH / 2 + (this->saveNQuitButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y >(SCREEN_HEIGHT / 6) * 4 - (this->saveNQuitButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < (SCREEN_HEIGHT / 6) * 4 + (this->saveNQuitButton.getGlobalBounds().height / 2))
				{
					_save->SaveTheGame(_game);
					exit(EXIT_SUCCESS);
				}

				// Button Quit
				if (mousePostionAtScreen.x > SCREEN_WIDTH / 2 - (this->saveNQuitButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < SCREEN_WIDTH / 2 + (this->saveNQuitButton.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y >(SCREEN_HEIGHT / 6) * 5 - (this->saveNQuitButton.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < (SCREEN_HEIGHT / 6) * 5 + (this->saveNQuitButton.getGlobalBounds().height / 2))
				{
					exit(EXIT_SUCCESS);
				}
			}
		}
	}
}
