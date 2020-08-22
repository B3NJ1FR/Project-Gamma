#include "LoadingGame.h"
#include "GameDefinitions.h"

LoadingGame::LoadingGame()
{

}

LoadingGame::~LoadingGame()
{

}

void LoadingGame::LoadTheGame(struct Game *_game)
{
	// A save :
	// 
	// - la map et toutes ces couches
	// - les ressources
	// - l'argent
	// - le temps �coul�s / calendrier
	// - les ouvriers (leurs nombres, leur position, etc)
	// - les vignes
	// - les b�timents sp�cifiques
	// - les entrepots
	// - l'�tale
	// 
	// 

	std::ifstream saveFile("Data/Configurations/Save.data", std::ios::beg | std::ios::binary);

	std::string temporaryString;
	temporaryString.erase();
	int temporaryNumber(RESET);
	float temporaryFloatingNumber(RESET);

	if (!saveFile.is_open())
	{
		std::cout << "Error accessing Save.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	// Loading the map size
	saveFile.read((char *) &_game->numberColumns, sizeof(unsigned short));
	saveFile.read((char *) &_game->numberLines, sizeof(unsigned short));
	saveFile.read((char *) &_game->numberLayers, sizeof(unsigned short));

	std::cout << "Map size loaded : " << _game->numberColumns << " " << _game->numberLines << " " << _game->numberLayers << std::endl;

	// Loading the map content
	for (int z = 0; z < _game->numberLayers; z++)
	{
		for (int y = 0; y < _game->numberLines; y++)
		{
			for (int x = 0; x < _game->numberColumns; x++)
			{
				saveFile.read((char *) &_game->map[z][y][x], sizeof(unsigned short));
			}
		}
	}

	std::cout << "Map loaded !\n";


	// Loading of the ressources data
	saveFile.read((char *) &_game->numberTypesOfRessources, sizeof(unsigned int));

	for (int i = 0; i < _game->numberTypesOfRessources; i++)
	{
		int quantityOwned(RESET);

		saveFile.read((char *) &quantityOwned, sizeof(int));
		_game->ressources[i].SetInitialQuantityOwned(quantityOwned);
	}

	std::cout << "Ressources loaded !\n";

	// Loading of the money data
	int money(RESET);
	saveFile.read((char *) &money, sizeof(int));
	_game->money.SetInitialQuantity(money);

	std::cout << "Money loaded !\n";

	// Loading of the time elapsed
	_game->time->LoadingTimeFromFile(&saveFile);

	std::cout << "Time loaded !\n";

	// Saving of the workers data
	_game->workersList->LoadingWorkersListFromFile(&saveFile);

	std::cout << "Workers loaded !\n";

	std::cout << "\n\nGame loaded successfully !\n\n\n";

	saveFile.close();
}