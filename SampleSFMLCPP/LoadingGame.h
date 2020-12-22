#pragma once

#include "Common.hpp"

class LoadingGame
{
private :

public :
	// Constructor & Destructor
	LoadingGame();
	~LoadingGame();

	// Methods
	void LoadTheGame(struct Game *_game);
};