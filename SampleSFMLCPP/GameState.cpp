#include "GameState.h"
#include "GameDefinitions.h"
#include "GameInitialisation.h"
#include "GameInput.h"
#include "GameDisplay.h"
#include "GameUpdate.h"



void GameState(struct Game *_game, enum GeneralState *_generalState)
{
	// Initialisation
	GameInitialisation(_game);

	_game->numberOfBuilding;

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