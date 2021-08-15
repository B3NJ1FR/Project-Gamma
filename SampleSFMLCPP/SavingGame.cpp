#include "SavingGame.h"
#include "GameDefinitions.h"
#include "BuildingDestruction.h"
#include "ListOfAnnualCostsNRevenues.h"
#include "ListOfAnnualProductions.h"


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
	_game->m_map->SavingMapFromFile(&saveFile);

	// Saving of the money data
	int money = _game->m_money->GetMoneyQuantity();
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
	_game->m_builds.m_storehouse.SavingForFile(&saveFile);
	std::cout << "Storehouses saved !\n";

	// Saving of the stall data
	_game->m_builds.m_stall->SavingStallForFile(&saveFile);
	std::cout << "Stall saved !\n";

	PurchasersManager::GetSingleton()->SavingForFile(&saveFile);
	std::cout << "Purchasers saved !\n";

	// Saving of the Buildings list planned
	_game->m_buildingsListPlanned->SavingBuildingsListPlannedForFile(&saveFile);
	std::cout << "Buildings list planned saved !\n";

	// Saving of the Tutorial progression
	_game->m_tutorialWindow->SavingTutorialProgressionForFile(&saveFile);
	std::cout << "Tutorial progression saved !\n";

	// Saving of the Building Destruction List
	BuildingDestruction::GetSingleton()->SavingForFile(&saveFile);
	std::cout << "Building Destruction List saved !\n";

	// Saving of the villa data
	_game->m_builds.m_villa->SavingVillaForFile(&saveFile);
	std::cout << "Villa saved !\n";

	_game->m_buildWindow.SavingGhostBuildingsForFile(&saveFile);
	std::cout << "BuildWindow saved !\n";

	ListOfAnnualProductions::GetSingleton()->SavingDataForFile(&saveFile);
	std::cout << "ListOfAnnualProductions saved !\n";

	ListOfAnnualCostsNRevenues::GetSingleton()->SavingDataForFile(&saveFile);
	std::cout << "ListOfAnnualCostsNRevenues saved !\n";

	std::cout << "\n\nGame saved successfully !\n\n\n";

	saveFile.close();
}