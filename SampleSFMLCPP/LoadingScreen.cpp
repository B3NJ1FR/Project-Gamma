#include "LoadingScreen.h"

LoadingScreen::LoadingScreen()
{
	m_loadingPicture = LoadSprite("Data/Assets/Sprites/Menu/LoadingScreen/loadingBackground.png", 1);
}

LoadingScreen::~LoadingScreen()
{
	delete (m_loadingPicture.getTexture());
}


void LoadingScreen::DisplayLoadingScreen(sf::RenderWindow &_window, const sf::Vector2i &_screenResolution)
{
	_window.clear();

	// Blit the loading picture
	BlitSprite(m_loadingPicture, _screenResolution.x / 2, _screenResolution.y / 2, 0, _window);

	_window.display();
}