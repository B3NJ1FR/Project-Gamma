#pragma once

#include "Common.hpp"
#include "SavingGame.h"
#include "LoadingGame.h"


class PauseWindow
{
private:

	sf::Sprite m_blackBackground;
	sf::Sprite m_resumeButton;
	sf::Sprite m_loadButton;
	sf::Sprite m_saveButton;
	sf::Sprite m_saveNQuitButton;
	sf::Sprite m_quitButton;


public:
	// Constructor & Destructor
	PauseWindow();
	~PauseWindow();

	// Methods
	void InputPauseWindow(struct Game *_game, SavingGame *_save, LoadingGame *_load);
	void DisplayPauseWindow(sf::RenderWindow &_window);
};