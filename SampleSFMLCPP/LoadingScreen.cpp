#include "LoadingScreen.h"

LoadingScreenClass::LoadingScreenClass()
{
	this->loadingPicture = LoadSprite("Data/Assets/Sprites/Menu/LoadingScreen/loadingBackground.png", 0);
}

LoadingScreenClass::~LoadingScreenClass()
{
	delete (this->loadingPicture.getTexture());
}


void LoadingScreenClass::DisplayLoadingScreen(sf::RenderWindow &_window)
{
	_window.clear();

	// Blit the loading picture
	BlitSprite(loadingPicture, 0, 0, 0, _window);

	_window.display();
}