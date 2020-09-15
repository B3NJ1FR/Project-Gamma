#ifndef VILLA_MANAGEMENT__H
#define VILLA_MANAGEMENT__H

#include "Common.hpp"

class VillaManagement
{
private:
	sf::Sprite backgroundSprite;

	

public:
	VillaManagement();
	~VillaManagement();

	void InputVillaManagement(enum GameState *_state, sf::RenderWindow &_window);
	void DisplayVillaManagement(sf::RenderWindow &_window);
};



#endif // !VILLA_MANAGEMENT__H