#pragma once

#include "TimeManagement.h"

#define SCENE_BASIC_RESOLUTION_WIDTH 960
#define SCENE_BASIC_RESOLUTION_HEIGHT 540

class VillaManagement
{
private:
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_tableSprite;
	sf::Sprite m_bookshelfSprite;
	sf::Sprite m_entranceSprite;

	sf::Image m_imageTable;
	sf::Image m_imageBookshelf;
	sf::Image m_imageEntrance;

public:
	// Constructor & Destructor
	VillaManagement();
	~VillaManagement();

	// Methods
	void InputVillaManagement(enum CurrentGameState *_state, sf::RenderWindow &_window);
	void DisplayVillaManagement(sf::RenderWindow &_window);
};