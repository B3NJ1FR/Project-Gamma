#ifndef TUTORIAL_WINDOW__H
#define TUTORIAL_WINDOW__H

#include "Common.hpp"

#define NUMBER_OF_CARACTERS_PER_LINE 38

class TutorialWindow
{
private:

	sf::Sprite backgroundMessage;

	std::string *messages;
	std::string pressSpaceMessage;

	sf::Text text;
	sf::Text textPressSpace;

	short int counterMessages;
	short int actualMessages;

	bool isMessageHasChanged;

public:
	TutorialWindow(sf::Font *_font);
	~TutorialWindow();

	void InitTextOfTutorialFromFile();

	void InputTutorialWindow(enum GameState *_state, sf::RenderWindow &_window);
	void UpdateTutorialWindow(sf::Font *_font);
	void DisplayTutorialWindow(sf::RenderWindow &_window);
};



#endif // !TUTORIAL_WINDOW__H