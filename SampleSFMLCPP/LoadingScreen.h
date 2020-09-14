#ifndef LOADING_SCREEN__H
#define LOADING_SCREEN__H

#include "Common.hpp"

class LoadingScreenClass
{
private :
	sf::Sprite loadingPicture;

public :
	LoadingScreenClass();
	~LoadingScreenClass();

	void DisplayLoadingScreen(sf::RenderWindow &_window);
};

struct LoadingScreen
{
	LoadingScreenClass loadingScreen;
};

#endif // !LOADING_SCREEN__H