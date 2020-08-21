#include "SavingGame.h"
#include "GameDefinitions.h"

SavingGame::SavingGame()
{

}

SavingGame::~SavingGame()
{

}

void SavingGame::SaveTheGame(struct Game *_game)
{
	// A save :
	// 
	// - la map et toutes ces couches
	// - les ressources
	// - l'argent
	// - le temps écoulés / calendrier
	// - les ouvriers (leurs nombres, leur position, etc)
	// - les vignes
	// - les bâtiments spécifiques
	// - les entrepots
	// - l'étale
	// 
	// 

	std::ofstream saveFile("Data/Configurations/Save.data", std::ios::beg | std::ios::trunc | std::ios::binary);

	std::string temporaryString;
	temporaryString.erase();
	int temporaryNumber(RESET);
	float temporaryFloatingNumber(RESET);

	if (!saveFile.is_open())
	{
		std::cout << "Error accessing Save.data file" << std::endl;

		exit(EXIT_FAILURE);
	}



	saveFile.close();
}