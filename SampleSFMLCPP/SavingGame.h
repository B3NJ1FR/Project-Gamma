#pragma once

#include "Common.hpp"

class SavingGame
{
private :

public :
	// Constructor & Destructor
	SavingGame();
	~SavingGame();

	// Methods
	void SaveTheGame(struct Game *_game);
};
