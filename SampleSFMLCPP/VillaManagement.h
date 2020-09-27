#ifndef VILLA_MANAGEMENT__H
#define VILLA_MANAGEMENT__H

#include "TimeManagement.h"

#define SCENE_BASIC_RESOLUTION_WIDTH 960
#define SCENE_BASIC_RESOLUTION_HEIGHT 540

class VillaManagement
{
private:
	sf::Sprite backgroundSprite;
	sf::Sprite tableSprite;
	sf::Sprite bookshelfSprite;
	sf::Sprite entranceSprite;

	sf::Image imageTable;
	sf::Image imageBookshelf;
	sf::Image imageEntrance;

public:
	VillaManagement();
	~VillaManagement();

	void InputVillaManagement(enum GameState *_state, TimeManagement *_time, sf::RenderWindow &_window);
	void DisplayVillaManagement(sf::RenderWindow &_window);
};



#endif // !VILLA_MANAGEMENT__H