#ifndef PAUSE_WINDOW__H
#define PAUSE_WINDOW__H

#include "Common.hpp"
#include "SavingGame.h"
#include "LoadingGame.h"


class PauseWindow
{
private:

	sf::Sprite blackBackground;
	sf::Sprite resumeButton;
	sf::Sprite loadButton;
	sf::Sprite saveButton;
	sf::Sprite saveNQuitButton;
	sf::Sprite quitButton;


public:
	PauseWindow();
	~PauseWindow();


	void DisplayPauseWindow(sf::RenderWindow &_window);
	void InputPauseWindow(struct Game *_game, SavingGame *_save, LoadingGame *_load);
};



#endif // !PAUSE_WINDOW__H