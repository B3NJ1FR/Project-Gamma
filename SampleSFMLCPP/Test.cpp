#include "Test.h"

Test::Test()
{
	//// Initialisation of the sprites
	//logo = LoadSprite("Data/Assets/Vindemia Antiquus_Icon.png", 0);
	//background = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Background.png", 1);
	//buttonNewGame = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_New_Game.png", 1);
	//buttonContinue = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Continue.png", 1);
	//buttonOptions = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Options.png", 1);
	//buttonQuit = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Quit.png", 1);
	//buttonValidate = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Continue.png", 1);
	//buttonReturn = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Button_Quit.png", 1);
	//warningMessage = LoadSprite("Data/Assets/Sprites/Menu/Main_Menu_Message.png", 1);

	//font.loadFromFile("Data/Fonts/arial.ttf");


	//// ***************** VERSION NUMBER LOADING ***************** //

	//std::ifstream settingsFile("Data/Configurations/Settings.data", std::ios::in);

	//std::string temporaryString;
	//std::string versionString = "AAAAA";
	//temporaryString.erase();

	//if (!settingsFile.is_open())
	//{
	//	std::cout << "Error accessing Settings.data file" << std::endl;

	//	exit(EXIT_FAILURE);
	//}

	//while (!settingsFile.eof())
	//{
	//	settingsFile >> temporaryString;

	//	if (temporaryString == "VERSION")
	//	{
	//		// Read the version number
	//		settingsFile >> versionString;

	//		versionString = "Version " + versionString;

	//		std::cout << "Game " << versionString << std::endl;
	//	}
	//}

	//settingsFile.close();

	//versionNumber = nullptr;
	//versionNumber = new sf::Text;
	//resolution = nullptr;
	//fullscreen = nullptr;
	//temporaryResolution = -1;

	//LoadTextString(&versionNumber, versionString, &font, 35, sf::Color::White, sf::Vector2f(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50));

	//isNewGameClicked = false;
	//isLoadGameClicked = false;
	//fullscreen = new sf::Text;
}

Test::~Test()
{
	// Deletion of the sprites
	/*delete (background.getTexture());
	delete (buttonNewGame.getTexture());
	delete (buttonContinue.getTexture());
	delete (buttonOptions.getTexture());
	delete (buttonQuit.getTexture());
	delete (warningMessage.getTexture());

	if (versionNumber != nullptr)
	{
		delete versionNumber;
		versionNumber = nullptr;
	}
	if (resolution != nullptr)
	{
		delete resolution;
		resolution = nullptr;
	}

	if (fullscreen != nullptr)
	{
		delete fullscreen;
		fullscreen = nullptr;
	}*/
}