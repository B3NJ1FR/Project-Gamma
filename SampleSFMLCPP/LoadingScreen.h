#pragma once

#include "Common.hpp"

class LoadingScreen
{
private :
	sf::Sprite m_loadingPicture;

public :
	//Constructor & Destructor
	LoadingScreen();
	~LoadingScreen();

	void DisplayLoadingScreen(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution);
};