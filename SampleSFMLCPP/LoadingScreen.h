#pragma once

#include "Common.hpp"

class LoadingScreen
{
private :
	sf::Sprite loadingPicture;

public :
	LoadingScreen();
	~LoadingScreen();

	void DisplayLoadingScreen(sf::RenderWindow &_window);
};