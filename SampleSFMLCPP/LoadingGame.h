#ifndef LOADING_GAME__H
#define LOADING_GAME__H

#include "Common.hpp"

class LoadingGame
{
private :

public :
	LoadingGame();
	~LoadingGame();

	void LoadTheGame(struct Game *_game);
};

#endif // !LOADING_GAME__H