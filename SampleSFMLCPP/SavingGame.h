#ifndef SAVING_GAME__H
#define SAVING_GAME__H

#include "Common.hpp"

class SavingGame
{
private :

public :
	SavingGame();
	~SavingGame();

	void SaveTheGame(struct Game *_game);
};

#endif // !SAVING_GAME__H
