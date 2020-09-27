#include "TutorialWindow.h"
#include "GameDefinitions.h"

TutorialWindow::TutorialWindow(sf::Font *_font)
{
	this->backgroundMessage = LoadSprite("Data/Assets/Sprites/Menu/TutorialWindow/backgroundMessage.png", 1);
	this->actualMessages = RESET;
	this->counterMessages = RESET;
	this->isMessageHasChanged = true;

	this->messages = nullptr;
	this->pressSpaceMessage = "Press Space to Continue";


	this->InitTextOfTutorialFromFile();
	
	LoadTextString(&this->text, this->messages[this->actualMessages], _font, 18, sf::Color::Black, 1);
	LoadTextString(&this->textPressSpace, this->pressSpaceMessage, _font, 17, sf::Color::Black, 1);
}

TutorialWindow::~TutorialWindow()
{

}

void TutorialWindow::InitTextOfTutorialFromFile()
{
	std::ifstream tutorialFile("Data/Configurations/Tutorial.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();	

	if (!tutorialFile.is_open())
	{
		std::cout << "Error accessing Tutorial.data file" << std::endl;

		exit(EXIT_FAILURE);
	}


	while (!tutorialFile.eof())
	{
		tutorialFile >> temporaryString;

		if (temporaryString == "NEW_MESSAGE")
		{
			this->counterMessages++;
		}
	}

	if (this->counterMessages != 0)
	{
		std::string *finalString = new std::string[this->counterMessages];

		for (int i = 0; i < this->counterMessages; i++)
		{
			finalString[i].erase();
		}


		tutorialFile.seekg(0, std::ios::beg);

		short int counter(-1);

		while (!tutorialFile.eof())
		{
			tutorialFile >> temporaryString;

			if (temporaryString == "NEW_MESSAGE")
			{
				counter++;
			}
			else
			{
				if (counter != -1)
				{
					finalString[counter] += temporaryString;
					finalString[counter] += " ";
				}
			}
		}



		for (int i = 0; i < this->counterMessages; i++)
		{
			std::cout << finalString[i] << std::endl << std::endl;
		}

		this->messages = finalString;
	}
	
	tutorialFile.close();
}


void TutorialWindow::InputTutorialWindow(enum GameState *_state, sf::RenderWindow &_window)
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
			if (event.key.code == sf::Keyboard::Escape)
			{
				*(_state) = PAUSE_WINDOW;
			}

			// If we pressed the escape key, we close the game
			if (event.key.code == sf::Keyboard::Space)
			{
				if (this->actualMessages + 1 <= this->counterMessages - 1)
				{
					this->actualMessages += 1;
					this->isMessageHasChanged = true;
				}
				else
				{
					*(_state) = NORMAL_MODE;
					std::cout << "THERE\N";
				}
			}
		}
	}
}

void TutorialWindow::UpdateTutorialWindow(sf::Font *_font)
{
	if (this->isMessageHasChanged == true
		&& this->actualMessages <= this->counterMessages - 1)
	{
		this->messages[this->actualMessages] = ConvertStringIntoParagraph(this->messages[this->actualMessages], NUMBER_OF_CARACTERS_PER_LINE);

		LoadTextString(&this->text, this->messages[this->actualMessages], _font, 18, sf::Color::Black, 1);

		this->isMessageHasChanged = false;
	}
}


void TutorialWindow::DisplayTutorialWindow(sf::RenderWindow &_window)
{
	BlitSprite(this->backgroundMessage, this->backgroundMessage.getGlobalBounds().width, SCREEN_HEIGHT - this->backgroundMessage.getGlobalBounds().height, 0, _window);
	
	BlitString(this->text, this->backgroundMessage.getGlobalBounds().width - 5, SCREEN_HEIGHT - this->backgroundMessage.getGlobalBounds().height - 10, _window);
	BlitString(this->textPressSpace, this->backgroundMessage.getGlobalBounds().width - 5, SCREEN_HEIGHT - this->backgroundMessage.getGlobalBounds().height + 95, _window);
}
