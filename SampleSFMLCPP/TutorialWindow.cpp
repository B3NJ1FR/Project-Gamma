#include "TutorialWindow.h"
#include "GameDefinitions.h"

TutorialWindow::TutorialWindow(sf::Font *_font)
{
	m_backgroundMessage = LoadSprite("Data/Assets/Sprites/Menu/TutorialWindow/backgroundMessage.png", 1);
	m_actualMessages = RESET;
	m_counterMessages = RESET;
	m_isMessageHasChanged = true;
	m_isTutorielDisplayFinished = false;

	m_messages = nullptr;

	InitTextOfTutorialFromFile();

	sf::Color burgundy = sf::Color(100, 13, 13);
	LoadTextString(&m_text, m_messages[m_actualMessages], _font, 18, burgundy, 1);
	LoadTextString(&m_textPressSpace, "Press Space to Continue", _font, 17, burgundy, 1);
}

TutorialWindow::~TutorialWindow()
{
	delete (m_backgroundMessage.getTexture());

	if (m_messages != nullptr)
	{
		delete [] m_messages;
		m_messages = nullptr;
	}
	std::cout << "[Game] - TutorialWindow destroyed\n";
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
					if (temporaryString == "[SAUT_LIGNE]"
						|| temporaryString == "[BREAK_LINE]")
					{
						finalString[counter] += "\n";
					}
					else
					{
						finalString[counter] += temporaryString;
						finalString[counter] += " ";
					}

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
					m_isTutorielDisplayFinished = true;
					*(_state) = NORMAL_MODE;
				}
			}
		}
	}
}

void TutorialWindow::UpdateTutorialWindow(enum CurrentGameState* _state, sf::Font *_font)
{
	if (m_isTutorielDisplayFinished)
	{
		*(_state) = NORMAL_MODE;
	}

	if (m_isMessageHasChanged == true
		&& m_actualMessages <= m_counterMessages - 1)
	{
		m_messages[m_actualMessages] = ConvertStringIntoParagraph(m_messages[m_actualMessages], NUMBER_OF_CARACTERS_PER_LINE);

		m_text.setString(m_messages[m_actualMessages]);
		ChangeTextStringOrigin(&m_text, 1);

		m_isMessageHasChanged = false;
	}
}


void TutorialWindow::DisplayTutorialWindow(sf::RenderWindow &_window)
{
	BlitSprite(m_backgroundMessage, (int)m_backgroundMessage.getGlobalBounds().width, SCREEN_HEIGHT - (int)m_backgroundMessage.getGlobalBounds().height, 0, _window);
	
	BlitString(m_text, (int)m_backgroundMessage.getGlobalBounds().width - 5, SCREEN_HEIGHT - (int)m_backgroundMessage.getGlobalBounds().height - 10, _window);
	BlitString(m_textPressSpace, (int)m_backgroundMessage.getGlobalBounds().width - 5, SCREEN_HEIGHT - (int)m_backgroundMessage.getGlobalBounds().height + 95, _window);
}


void TutorialWindow::SavingTutorialProgressionForFile(std::ofstream* _file)
{
	bool isTutorialFinished = m_isTutorielDisplayFinished;
	_file->write((char*)&isTutorialFinished, sizeof(bool));

	if (!m_isTutorielDisplayFinished)
	{
		short counterOfMessages = m_actualMessages;
		_file->write((char*)&counterOfMessages, sizeof(short));
	}
}

void TutorialWindow::LoadingTutorialProgressionForFile(std::ifstream* _file)
{
	bool isTutorialFinished = false;
	_file->read((char*)&isTutorialFinished, sizeof(bool));
	m_isTutorielDisplayFinished = isTutorialFinished;

	if (!m_isTutorielDisplayFinished)
	{
		short counterOfMessages = 0;
		_file->read((char*)&counterOfMessages, sizeof(short));
		m_actualMessages = counterOfMessages;

		m_isMessageHasChanged = true;
	}
}