#include "VillaManagement.h"

// Pnj dedans -> Ouverture img interieur villa -> Soit Build
//											   -> Soit Stats production


VillaManagement::VillaManagement()
{
	this->backgroundSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/blackbackground.png", 0);
}

VillaManagement::~VillaManagement()
{

}


void VillaManagement::InputVillaManagement(enum GameState *_state, sf::RenderWindow &_window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
}
void VillaManagement::DisplayVillaManagement(sf::RenderWindow &_window)
{
	BlitSprite(this->backgroundSprite, 0, 0, 0, _window);
}