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
	// - le temps écoulés / calendrier - OK
	// - les ouvriers (leurs nombres, leur position, etc) - OK
	// - les vignes - OK
	// - les bâtiments spécifiques - OK
	// - les entrepots - OK
	// - l'étale - OK

	std::ofstream saveFile("Data/Configurations/Save.data", std::ios::beg | std::ios::binary);

	std::string temporaryString;
	temporaryString.erase();
	int temporaryNumber(RESET);
	float temporaryFloatingNumber(RESET);

	if (!saveFile.is_open())
	{
		std::cout << "Error accessing Save.data file" << std::endl;

		exit(EXIT_FAILURE);
	}


	// Save the map
	_game->m_map.SavingMapFromFile(&saveFile);

	// Saving of the ressources data
	saveFile.write((char *) &_game->m_numberTypesOfRessources, sizeof(unsigned int));

	for (int i = 0; i < _game->m_numberTypesOfRessources; i++)
	{
		int quantity = _game->m_ressources[i].GetQuantityOwned();
		saveFile.write((char *) &quantity, sizeof(int));
	}

	std::cout << "Ressources saved !\n";

	// Saving of the money data
	int money = _game->m_money.GetMoneyQuantity();
	saveFile.write((char *) &money, sizeof(int));

	std::cout << "Money saved !\n";

	// Saving of the time elapsed
	_game->m_time->SavingTimeFromFile(&saveFile);

	std::cout << "Time saved !\n";

	// Saving of the workers data
	//saveFile.write((char *) &_game->m_workersList, sizeof(WorkersList)); // Marche pas
	_game->m_workersList->SavingWorkersListForFile(&saveFile);

	std::cout << "Workers saved !\n";

	// Saving of the vines data
	_game->m_builds.m_vines.SavingVinesListForFile(&saveFile);

	std::cout << "Vines saved !\n";

	// Saving of the specific buildings data
	_game->m_builds.m_stompingVats.SavingSpecificsBuildingsListForFile(&saveFile);
	_game->m_builds.m_winePress.SavingSpecificsBuildingsListForFile(&saveFile);
	_game->m_builds.m_wineStorehouse.SavingSpecificsBuildingsListForFile(&saveFile);

	std::cout << "Specifics buildings saved !\n";

	// Saving of the storehouses data
	_game->m_builds.m_storehouse.SavingVinesListForFile(&saveFile);

	// Saving of the stall data
	_game->m_builds.m_stall->SavingStallForFile(&saveFile);

	std::cout << "Stall saved !\n";

	if (((_game->m_builds.m_stall->GetStatus() == STALL_SEND_REQUEST_PURCHASER
		&& _game->m_builds.m_stall->GetIsNewMerchantNeeded() == false)
		|| _game->m_builds.m_stall->GetStatus() == STALL_PURCHASER_IS_PRESENT)
		&& _game->m_purchasers != nullptr)
	{
		_game->m_purchasers->SavingPurchasersForFile(&saveFile);
	}

	std::cout << "Purchasers saved !\n";

	std::cout << "\n\nGame saved successfully !\n\n\n";

	saveFile.close();
}