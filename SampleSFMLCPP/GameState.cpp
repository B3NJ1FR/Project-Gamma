#include "GameState.h"
#include "GameDefinitions.h"

#include "GameInitialisation.h"
#include "GameInput.h"
#include "GameDisplay.h"
#include "GameUpdate.h"

#include "LoadingGame.h"



void GameState(struct Game *_game, enum GeneralState *_generalState)
{
	// Initialisation
	GameInitialisation(_game);

	if (*(_generalState) == MAIN_STATE_LOAD_GAME)
	{
		// Load the game with the last save from the Save.txt's file
		_game->load.LoadTheGame(_game);

		*(_generalState) = MAIN_STATE_GAME;
	}

	while (*(_generalState) == MAIN_STATE_GAME)
	{
		// Input
		GameInput(_game);

		// Update
		GameUpdate(_game);

		// Display
		GameDisplay(_game);
	}
}