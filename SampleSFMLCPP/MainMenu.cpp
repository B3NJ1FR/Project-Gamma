#include "MainMenu.h"


MainMenu::MainMenu()
{
	// Initialisation of the sprites
	logo = LoadSprite("Data/Assets/Vindemia Antiquus_Icon.png", 0);
	background = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Background.png", 0);
	buttonNewGame = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_New_Game.png", 1);
	buttonContinue = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Continue.png", 1);
	buttonOptions = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Options.png", 1);
	buttonQuit = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Quit.png", 1);
	buttonValidate = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Continue.png", 1);
	buttonReturn = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Quit.png", 1);
	warningMessage = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Message.png", 0);

	font.loadFromFile("Data/Fonts/arial.ttf");


	// ***************** VERSION NUMBER LOADING ***************** //

	std::ifstream settingsFile("Data/Configurations/Settings.data", std::ios::in);

	std::string temporaryString;
	std::string versionString;
	temporaryString.erase();

	if (!settingsFile.is_open())
	{
		std::cout << "Error accessing Settings.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!settingsFile.eof())
	{
		settingsFile >> temporaryString;

		if (temporaryString == "VERSION")
		{
			// Read the version number
			settingsFile >> versionString;

			versionString = "Version " + versionString;

			std::cout << "Game " << versionString << std::endl;
		}
	}

	settingsFile.close();

	versionNumber = nullptr;
	versionNumber = new sf::Text;
	resolution = nullptr;
	fullscreen = nullptr;
	temporaryResolution = -1;

	LoadTextString(versionNumber, versionString, &this->font, 35, sf::Color::White, sf::Vector2f(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50));

	isNewGameClicked = false;
	isLoadGameClicked = false;
}

MainMenu::~MainMenu()
{
	// Deletion of the sprites
	delete (background.getTexture());
	delete (buttonNewGame.getTexture());
	delete (buttonContinue.getTexture());
	delete (buttonOptions.getTexture());
	delete (buttonQuit.getTexture());
	delete (warningMessage.getTexture());

	delete versionNumber;
	versionNumber = nullptr;
	delete resolution;
	resolution = nullptr;
	delete fullscreen;
	resolution = nullptr;
}


void MainMenu::InputMainMenu(sf::RenderWindow &_window, enum GeneralState *_generalState, System *_system)
{
	sf::Event event;
	while (_window.pollEvent(event))
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
				exit(EXIT_SUCCESS);
			}			

			if (event.key.code == sf::Keyboard::Space && isNewGameClicked == true)
			{
				*(_generalState) = MAIN_STATE_GAME;

				this->~MainMenu();
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (state != MAIN_MENU_OPTIONS)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					sf::Vector2i mousePostionAtScreen = sf::Mouse::getPosition(_window);

					// Button New Game
					if (mousePostionAtScreen.x > 400 - (buttonNewGame.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.x < 400 + (buttonNewGame.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.y > 200 - (buttonNewGame.getGlobalBounds().height / 2)
						&& mousePostionAtScreen.y < 200 + (buttonNewGame.getGlobalBounds().height / 2))
					{
						std::cout << "\n\nLaunching a new game...\n\n\n";
						isNewGameClicked = true;
						isLoadGameClicked = false;
					}

					// Button Continue
					if (mousePostionAtScreen.x > 400 - (buttonContinue.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.x < 400 + (buttonContinue.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.y > 400 - (buttonContinue.getGlobalBounds().height / 2)
						&& mousePostionAtScreen.y < 400 + (buttonContinue.getGlobalBounds().height / 2))
					{
						std::cout << "\n\nLaunching old game...\n\n\n";
						isNewGameClicked = false;
						isLoadGameClicked = true;
					}

					// Button Options	
					if (mousePostionAtScreen.x > 400 - (buttonOptions.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.x < 400 + (buttonOptions.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.y > 600 - (buttonOptions.getGlobalBounds().height / 2)
						&& mousePostionAtScreen.y < 600 + (buttonOptions.getGlobalBounds().height / 2))
					{
						std::cout << "\n\nOptions ...\n\n\n";
						state = MAIN_MENU_OPTIONS;

						resolution = new sf::Text;
						fullscreen = new sf::Text;
						LoadTextString(resolution, _system->m_listOfScreenResolutions[_system->m_currentScreenResolution], &font, 40, sf::Color::Black, 1);

						if (_system->m_isFullscreen)
						{
							LoadTextString(fullscreen, "Fullscreen : ON", &font, 40, sf::Color::Black, 1);
						}
						else
						{
							LoadTextString(fullscreen, "Fullscreen : OFF", &font, 40, sf::Color::Black, 1);
						}

					}

					// Button Quit
					if (mousePostionAtScreen.x > 400 - (buttonQuit.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.x < 400 + (buttonQuit.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.y > 900 - (buttonQuit.getGlobalBounds().height / 2)
						&& mousePostionAtScreen.y < 900 + (buttonQuit.getGlobalBounds().height / 2))
					{
						std::cout << "\n\nSee you soon !\n\n\n";
						this->~MainMenu();
						_window.close();
						exit(EXIT_SUCCESS);
					}
				}
			}
			else
			{
				if (event.key.code == sf::Mouse::Left)
				{
					sf::Vector2i mousePostionAtScreen = sf::Mouse::getPosition(_window);

					// Change resolution displayed
					if (mousePostionAtScreen.x > 400 - 100
						&& mousePostionAtScreen.x < 400 + 100
						&& mousePostionAtScreen.y > 200 - 30
						&& mousePostionAtScreen.y < 200 + 30)
					{
						temporaryResolution = (temporaryResolution == -1) ? _system->m_currentScreenResolution + 1 : (temporaryResolution + 1) % SR_NUMBER_MAX;
						
						LoadTextString(resolution, _system->m_listOfScreenResolutions[temporaryResolution], &this->font, 40, sf::Color::Black, 1);
					}
					
					// Change fullscreen displayed
					if (mousePostionAtScreen.x > 400 - 100
						&& mousePostionAtScreen.x < 400 + 100
						&& mousePostionAtScreen.y > 300 - 30
						&& mousePostionAtScreen.y < 300 + 30)
					{
						_system->m_isFullscreen = !_system->m_isFullscreen;

						if (_system->m_isFullscreen)
						{
							LoadTextString(fullscreen, "Fullscreen : ON", &font, 40, sf::Color::Black, 1);
						}
						else
						{
							LoadTextString(fullscreen, "Fullscreen : OFF", &font, 40, sf::Color::Black, 1);
						}
					}


					// Button Validate
					if (mousePostionAtScreen.x > 400 - (buttonValidate.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.x < 400 + (buttonValidate.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.y > 600 - (buttonValidate.getGlobalBounds().height / 2)
						&& mousePostionAtScreen.y < 600 + (buttonValidate.getGlobalBounds().height / 2))
					{
						_system->ChangeWindowResolution(_system->FindWindowResolution((ListOfResolutions)temporaryResolution),
														(ListOfResolutions)temporaryResolution);
					}

					// Button Return
					if (mousePostionAtScreen.x > 400 - (buttonReturn.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.x < 400 + (buttonReturn.getGlobalBounds().width / 2)
						&& mousePostionAtScreen.y > 900 - (buttonReturn.getGlobalBounds().height / 2)
						&& mousePostionAtScreen.y < 900 + (buttonReturn.getGlobalBounds().height / 2))
					{
						state = MAIN_MENU_OPERATIONAL;
					}

				}
			}
		}
	}
}


void MainMenu::UpdateMainMenu(enum GeneralState *_generalState)
{
	if (isLoadGameClicked == true)
	{
		*(_generalState) = MAIN_STATE_LOAD_GAME;

		this->~MainMenu();
	}
}


void MainMenu::DisplayMainMenu(sf::RenderWindow &_window)
{
	_window.clear();

	// Blit the background
	BlitSprite(background, 0, 0, 0, _window);


	// Blit the logo
	BlitSprite(logo, 1920 - logo.getGlobalBounds().width - 20, 0, 0, _window);



	// Blit the buttons
	BlitSprite(buttonNewGame, 400, 200, 0, _window);
	BlitSprite(buttonContinue, 400, 400, 0, _window);
	BlitSprite(buttonOptions, 400, 600, 0, _window);
	BlitSprite(buttonQuit, 400, 900, 0, _window);


	// Blit the version number
	if (versionNumber != nullptr)
	{
		BlitString(*versionNumber, _window);
	}



	if (this->isNewGameClicked == true)
	{
		BlitSprite(warningMessage, 0, 0, 0, _window);
	}

	_window.display();

}


void MainMenu::DisplayMainMenuOptions(sf::RenderWindow& _window)
{
	_window.clear();

	// Blit the background
	BlitSprite(background, 0, 0, 0, _window);


	// Blit the logo
	BlitSprite(logo, 1920 - logo.getGlobalBounds().width - 20, 0, 0, _window);

	if (resolution != nullptr)
	{
		BlitString(*resolution, 400, 200, _window);
	}
	
	if (fullscreen != nullptr)
	{
		BlitString(*fullscreen, 400, 300, _window);
	}


	// Blit the buttons
	BlitSprite(buttonValidate, 400, 600, 0, _window);
	BlitSprite(buttonReturn, 400, 900, 0, _window);


	// Blit the version number
	if (versionNumber != nullptr)
	{
		BlitString(*versionNumber, _window);
	}

	_window.display();

}



void MainMenu::MainMenuState(System *_system, enum GeneralState *_state)
{
	// Input
	InputMainMenu(_system->m_window, _state, _system);

	// Update
	UpdateMainMenu(_state);

	// Display
	switch (state)
	{
	case MAIN_MENU_OPERATIONAL:
		DisplayMainMenu(_system->m_window);
		break;
	case MAIN_MENU_OPTIONS:
		DisplayMainMenuOptions(_system->m_window);
		break;
	case MAIN_MENU_EXIT:
		DisplayMainMenu(_system->m_window);
		break;
	default:
		DisplayMainMenu(_system->m_window);
		break;
	}
}