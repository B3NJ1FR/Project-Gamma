#include "GameDefinitions.h"

Game::Game(LoadingScreen* _loadingScreen)
{
	// Display the loading screen during the game loading
	_loadingScreen->DisplayLoadingScreen(*window);

	actualGameState = TUTORIAL_MODE;

	MapInitialisation(_game);
	SpritesInitialisation(_game);
	TextsInit(_game);
	RessourcesInitialisation(_game);

	time = new TimeManagement(&charlemagneFont);
	tutorialWindow = new TutorialWindow(&charlemagneFont);
	workersList = new WorkersList;
	mainCharacter = new MainCharacter;
	sellingWindow = new SellingWindow(&generalFont);
	buildingsListPlanned = new BuildingsListPlanned();
	purchasers = nullptr;


	camera.x = -15;
	camera.y = -15;
	camera.z = RESET;

	scale.x = 1 / (1 - camera.z);
	scale.y = 1 / (1 - camera.z);

	isDebuggerModeActive = false;



	workersList->InitialisationWorkersList();

	buildWindow.InitTextsBuildWindow(&charlemagneFont);
}