#include "LoadingScreen.h"

LoadingScreen::LoadingScreen()
{
	m_loadingPicture = LoadSprite("Data/Assets/Sprites/Menu/LoadingScreen/loadingBackground.png", 0);
}

LoadingScreen::~LoadingScreen()
{
	delete (m_loadingPicture.getTexture());
}


void LoadingScreen::DisplayLoadingScreen(sf::RenderWindow &_window)
{
	_window.clear();

	// Blit the loading picture
	BlitSprite(m_loadingPicture, 0, 0, 0, _window);

	_window.display();
}