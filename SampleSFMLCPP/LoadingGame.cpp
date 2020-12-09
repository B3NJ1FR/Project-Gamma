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
	// - le temps écoulés / calendrier
	// - les ouvriers (leurs nombres, leur position, etc)
	// - les vignes
	// - les bâtiments spécifiques
	// - les entrepots
	// - l'étale
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
				saveFile.read((char *) &_game->m_map[z][y][x], sizeof(unsigned short));
			}
		}
	}

	std::cout << "Map loaded !\n";


	// Loading of the ressources data
	saveFile.read((char *) &_game->m_numberTypesOfRessources, sizeof(unsigned int));

	for (int i = 0; i < _game->m_numberTypesOfRessources; i++)
	{
		int quantityOwned(RESET);

		saveFile.read((char *) &quantityOwned, sizeof(int));
		_game->m_ressources[i].SetInitialQuantityOwned(quantityOwned);
	}

	std::cout << "Ressources loaded !\n";

	// Loading of the money data
	int money(RESET);
	saveFile.read((char *) &money, sizeof(int));
	_game->money.SetInitialQuantity(money);

	std::cout << "Money loaded !\n";

	// Loading of the time elapsed
	_game->m_time->LoadingTimeFromFile(&saveFile);

	std::cout << "Time loaded !\n";

	// Loading of the workers data
	_game->m_workersList->LoadingWorkersListFromFile(&saveFile, _game->m_map);

	std::cout << "Workers loaded !\n";

	// Loading of the vines data
	_game->vines.LoadingVinesListFromFile(&saveFile);

	std::cout << "Vines loaded !\n";
	
	// Saving of the specific buildings data
	_game->stompingVats.LoadingSpecificsBuildingsListFromFile(&saveFile);
	std::cout << "Stomping Vats loaded !\n";

	_game->winePress.LoadingSpecificsBuildingsListFromFile(&saveFile);
	std::cout << "Wine Presses loaded !\n";

	_game->wineStorehouse.LoadingSpecificsBuildingsListFromFile(&saveFile);
	std::cout << "Wine Storehouses loaded !\n";

	// Saving of the storehouses data
	_game->storehouse.LoadingVinesListFromFile(&saveFile);

	std::cout << "Storehouses loaded !\n";

	// Loading of the stall data
	_game->stall->LoadingStallFromFile(&saveFile);

	std::cout << "Stall loaded !\n";

	if ((_game->stall->GetStatus() == STALL_SEND_REQUEST_PURCHASER
		|| _game->stall->GetStatus() == STALL_PURCHASER_IS_PRESENT))
	{
		if (_game->m_purchasers != nullptr)
		{
			delete _game->m_purchasers;
			_game->m_purchasers = nullptr;

			std::cout << "Suppression of this actual merchant\n\n";
		}

		_game->stall->SetIsNewMerchantNeeded(false);

		_game->m_purchasers = new Purchasers;
		_game->m_purchasers->LoadingPurchasersFromFile(&saveFile);

		std::cout << "Purchaser loaded !\n";
	}

	std::cout << "\n\nGame loaded successfully !\n\n\n";

	saveFile.close();
}