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
	// - la map et toutes ces couches - OK
	// - les ressources - OK
	// - l'argent - OK
	// - le temps �coul�s / calendrier - OK
	// - les ouvriers (leurs nombres, leur position, etc) - OK
	// - les vignes - OK
	// - les b�timents sp�cifiques - OK
	// - les entrepots - OK
	// - l'�tale - OK

	std::ofstream saveFile("Data/Configurations/Save.data", std::ios::beg | std::ios::trunc | std::ios::binary | std::ios::out);

	std::string temporaryString;
	temporaryString.erase();
	int temporaryNumber(RESET);
	float temporaryFloatingNumber(RESET);

	if (!saveFile.is_open())
	{
		std::cout << "Error accessing Save.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	// Saving the map size
	saveFile.write((char *) &_game->numberColumns, sizeof(unsigned short));
	saveFile.write((char *) &_game->numberLines, sizeof(unsigned short));
	saveFile.write((char *) &_game->numberLayers, sizeof(unsigned short));


	// Saving the map content
	for (int z = 0; z < _game->numberLayers; z++)	
	{
		for (int y = 0; y < _game->numberLines; y++)
		{
			for (int x = 0; x < _game->numberColumns; x++)
			{
				saveFile.write((char *)&_game->map[z][y][x], sizeof(unsigned short));
			}
		}
	}


	// Saving of the ressources data
	saveFile.write((char *) &_game->numberTypesOfRessources, sizeof(unsigned int));

	for (int i = 0; i < _game->numberTypesOfRessources; i++)
	{
		int quantity = _game->ressources[i].GetQuantityOwned();
		saveFile.write((char *) &quantity, sizeof(int));
	}


	// Saving of the money data
	int money = _game->money.GetMoneyQuantity();
	saveFile.write((char *) &money, sizeof(int));


	// Saving of the time elapsed
	_game->time->SavingTimeFromFile(&saveFile);


	// Saving of the workers data
	//saveFile.write((char *) &_game->workersList, sizeof(WorkersList)); // Marche pas
	_game->workersList->SavingWorkersListForFile(&saveFile);


	// Saving of the vines data
	_game->vines.SavingVinesListForFile(&saveFile);

	// Saving of the specific buildings data
	_game->stompingVats.SavingSpecificsBuildingsListForFile(&saveFile);
	_game->winePress.SavingSpecificsBuildingsListForFile(&saveFile);
	_game->wineStorehouse.SavingSpecificsBuildingsListForFile(&saveFile);

	// Saving of the storehouses data
	_game->storehouse.SavingVinesListForFile(&saveFile);

	// Saving of the stall data
	_game->stall->SavingStallForFile(&saveFile);

	if (((_game->stall->GetStatus() == STALL_SEND_REQUEST_PURCHASER
		&& _game->stall->GetIsNewMerchantNeeded() == false)
		|| _game->stall->GetStatus() == STALL_PURCHASER_IS_PRESENT)
		&& _game->purchasers != nullptr)
	{
		_game->purchasers->SavingPurchasersForFile(&saveFile);
	}


	std::cout << "\n\nGame saved successfully !\n\n\n";

	saveFile.close();
}