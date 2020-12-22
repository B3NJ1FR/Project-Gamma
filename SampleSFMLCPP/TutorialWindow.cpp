#include "TutorialWindow.h"
#include "GameDefinitions.h"

TutorialWindow::TutorialWindow(sf::Font *_font)
{
	m_backgroundMessage = LoadSprite("Data/Assets/Sprites/Menu/TutorialWindow/backgroundMessage.png", 1);
	m_actualMessages = RESET;
	m_counterMessages = RESET;
	m_isMessageHasChanged = true;

	m_messages = nullptr;
	m_pressSpaceMessage = "Press Space to Continue";


	InitTextOfTutorialFromFile();
	
	LoadTextString(&m_text, m_messages[m_actualMessages], _font, 18, sf::Color::Black, 1);
	LoadTextString(&m_textPressSpace, m_pressSpaceMessage, _font, 17, sf::Color::Black, 1);
}

TutorialWindow::~TutorialWindow()
{
	if (m_messages != nullptr)
	{
		delete m_messages;
	}
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
			m_counterMessages++;
		}
	}

	if (m_counterMessages > 0)
	{
		std::string *finalString = new std::string[m_counterMessages];

		for (int i = 0; i < m_counterMessages; i++)
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



		for (int i = 0; i < m_counterMessages; i++)
		{
			std::cout << finalString[i] << std::endl << std::endl;
		}

		m_messages = finalString;
	}
	
	tutorialFile.close();
}


void TutorialWindow::InputTutorialWindow(enum CurrentGameState *_state, sf::RenderWindow &_window)
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
				if (m_actualMessages + 1 <= m_counterMessages - 1)
				{
					m_actualMessages += 1;
					m_isMessageHasChanged = true;
				}
				else
				{
					*(_state) = NORMAL_MODE;
				}
			}
		}
	}
}

void TutorialWindow::UpdateTutorialWindow(sf::Font *_font)
{
	if (m_isMessageHasChanged == true
		&& m_actualMessages <= m_counterMessages - 1)
	{
		m_messages[m_actualMessages] = ConvertStringIntoParagraph(m_messages[m_actualMessages], NUMBER_OF_CARACTERS_PER_LINE);

		LoadTextString(&m_text, m_messages[m_actualMessages], _font, 18, sf::Color::Black, 1);

		m_isMessageHasChanged = false;
	}
}


void TutorialWindow::DisplayTutorialWindow(sf::RenderWindow &_window)
{
	BlitSprite(m_backgroundMessage, m_backgroundMessage.getGlobalBounds().width, SCREEN_HEIGHT - m_backgroundMessage.getGlobalBounds().height, 0, _window);
	
	BlitString(m_text, m_backgroundMessage.getGlobalBounds().width - 5, SCREEN_HEIGHT - m_backgroundMessage.getGlobalBounds().height - 10, _window);
	BlitString(m_textPressSpace, m_backgroundMessage.getGlobalBounds().width - 5, SCREEN_HEIGHT - m_backgroundMessage.getGlobalBounds().height + 95, _window);
}
