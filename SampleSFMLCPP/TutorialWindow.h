#ifndef TUTORIAL_WINDOW__H
#define TUTORIAL_WINDOW__H

#include "Common.hpp"

#define NUMBER_OF_CARACTERS_PER_LINE 38

class TutorialWindow
{
private:

	sf::Sprite m_backgroundMessage;

	std::string *m_messages;
	std::string m_pressSpaceMessage;

	sf::Text m_text;
	sf::Text m_textPressSpace;

	short int m_counterMessages;
	short int m_actualMessages;

	bool m_isMessageHasChanged;

public:
	TutorialWindow(sf::Font *_font);
	~TutorialWindow();

	void InitTextOfTutorialFromFile();

	void InputTutorialWindow(enum CurrentGameState *_state, sf::RenderWindow &_window);
	void UpdateTutorialWindow(sf::Font *_font);
	void DisplayTutorialWindow(sf::RenderWindow &_window);
};



#endif // !TUTORIAL_WINDOW__H