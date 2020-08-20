#include "MainMenu.h"


MainMenu::MainMenu()
{
	// Initialisation of the sprites
	this->background = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Background.png", 0);
	this->buttonPlay = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Play.png", 1);
	this->buttonOptions = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Options.png", 1);
	this->buttonQuit = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Quit.png", 1);
	this->warningMessage = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Message.png", 0);

	this->font.loadFromFile("Data/Fonts/arial.ttf");


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

	this->versionNumber = nullptr;
	this->versionNumber = new sf::Text;

	LoadTextString(this->versionNumber, versionString, &this->font, 35, sf::Color::White, sf::Vector2f(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50));

	this->isPlayClicked = false;
}

MainMenu::~MainMenu()
{
	// Deletion of the sprites
	delete (this->background.getTexture());
	delete (this->buttonPlay.getTexture());
	delete (this->buttonOptions.getTexture());
	delete (this->buttonQuit.getTexture());
	delete (this->warningMessage.getTexture());

	//delete this->versionNumber;
	this->versionNumber = nullptr;
}


void MainMenu::InputMainMenu(sf::RenderWindow &_window, enum GeneralState *_generalState)
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

			if (event.key.code == sf::Keyboard::Space && this->isPlayClicked == true)
			{
				*(_generalState) = MAIN_STATE_GAME;

				this->~MainMenu();
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			// If we pressed the escape key, we close the game
			if (event.key.code == sf::Mouse::Left)
			{
				sf::Vector2i mousePostionAtScreen = sf::Mouse::getPosition(_window);

				// Button Play
				if (mousePostionAtScreen.x > 400 - (buttonPlay.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < 400 + (buttonPlay.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y > 300 - (buttonPlay.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < 300 + (buttonPlay.getGlobalBounds().height / 2))
				{
					std::cout << "\n\nGame launching ...\n\n\n";
					this->isPlayClicked = true;
				}

				//// Button Options
				//if (mousePostionAtScreen.x > 400 - (buttonOptions.getGlobalBounds().width / 2)
				//	&& mousePostionAtScreen.x < 400 + (buttonOptions.getGlobalBounds().width / 2)
				//	&& mousePostionAtScreen.y > 600 - (buttonOptions.getGlobalBounds().height / 2)
				//	&& mousePostionAtScreen.y < 600 + (buttonOptions.getGlobalBounds().height / 2))
				//{
				//	std::cout << "\n\nOptions ...\n\n\n";
				//}

				// Button Quit
				if (mousePostionAtScreen.x > 400 - (buttonQuit.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.x < 400 + (buttonQuit.getGlobalBounds().width / 2)
					&& mousePostionAtScreen.y > 900 - (buttonQuit.getGlobalBounds().height / 2)
					&& mousePostionAtScreen.y < 900 + (buttonQuit.getGlobalBounds().height / 2))
				{
					std::cout << "\n\nSee you soon !\n\n\n";
					exit(EXIT_SUCCESS);
				}
			}
		}
	}
}


void MainMenu::UpdateMainMenu()
{

}


void MainMenu::DisplayMainMenu(sf::RenderWindow &_window)
{
	_window.clear();

	// Blit the background
	BlitSprite(this->background, 0, 0, 0, _window);


	// Blit the logo



	// Blit the buttons
	BlitSprite(this->buttonPlay, 400, 300, 0, _window);
	//BlitSprite(this->buttonOptions, 400, 600, 0, _window);
	BlitSprite(this->buttonQuit, 400, 900, 0, _window);


	// Blit the version number
	if (this->versionNumber != nullptr)
	{
		BlitString(*this->versionNumber, _window);
	}



	if (this->isPlayClicked == true)
	{
		BlitSprite(this->warningMessage, 0, 0, 0, _window);
	}

	_window.display();

}



void MainMenuState(Data *_data)
{
	// Input
	_data->mainMenu->menu.InputMainMenu(_data->system->window, &_data->state);

	// Update
	_data->mainMenu->menu.UpdateMainMenu();

	// Display
	_data->mainMenu->menu.DisplayMainMenu(_data->system->window);
	
}