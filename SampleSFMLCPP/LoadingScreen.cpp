#include "LoadingScreen.h"

LoadingScreen::LoadingScreen()
{
	loadingPicture = LoadSprite("Data/Assets/Sprites/Menu/LoadingScreen/loadingBackground.png", 0);
}

LoadingScreen::~LoadingScreen()
{
	delete (loadingPicture.getTexture());
}


void LoadingScreen::DisplayLoadingScreen(sf::RenderWindow &_window)
{
	_window.clear();

	// Blit the loading picture
	BlitSprite(loadingPicture, 0, 0, 0, _window);

	_window.display();
}