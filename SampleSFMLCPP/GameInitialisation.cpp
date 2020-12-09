#include "GameInitialisation.h"
#include "GameDefinitions.h"
#include "LoadingScreen.h"
#include "InitBuildingsFromFile.h"
#include "LinkedList.h"
#include "Pathfinding.h" // Temporaire

// -------------------------------------------------------------------------
//								A DESTROY
// -------------------------------------------------------------------------

//
//void GameInitialisation(Game *_game, LoadingScreen *_loadingScreen)
//{
//	//// Display the loading screen during the game loading
//	//_loadingScreen->DisplayLoadingScreen(*_game->window);
//
//
//	//_game->actualGameState = TUTORIAL_MODE;
//
//	//_game->numberOfBuilding = RESET;
//	//
//	//InitBuildingsFromFile(_game);
//	//MapInitialisation(_game);
//	//SpritesInitialisation(_game);
//	//TextsInit(_game);
//	//RessourcesInitialisation(_game);
//
//	//_game->time = new TimeManagement(&_game->charlemagneFont);
//	//_game->tutorialWindow = new TutorialWindow(&_game->charlemagneFont);
//	//_game->workersList = new WorkersList;
//	//_game->mainCharacter = new MainCharacter;
//	//_game->stall = new Stalls(&_game->buildings[BUILDING_STALL]);
//	//_game->sellingWindow = new SellingWindow(&_game->generalFont);
//	//_game->buildingsListPlanned = new BuildingsListPlanned();
//	//_game->purchasers = nullptr;
//
//	//
//	//_game->camera.x = -15;
//	//_game->camera.y = -15;
//	//_game->camera.z = RESET;
//	//
//	//_game->scale.x = 1 / (1 - _game->camera.z);
//	//_game->scale.y = 1 / (1 - _game->camera.z);
//
//	//_game->isDebuggerModeActive = false;
//
//
//
//	//_game->vines.InitialisationVines(&_game->buildings[BUILDING_VINES]);
//	//_game->stompingVats.InitialisationSpeBuilding(&_game->buildings[BUILDING_GRAPE_STOMPING_VATS]);
//	//_game->winePress.InitialisationSpeBuilding(&_game->buildings[BUILDING_WINE_PRESS]);
//	//_game->wineStorehouse.InitialisationSpeBuilding(&_game->buildings[BUILDING_WINE_STOREHOUSE]);
//	//_game->storehouse.InitialisationStorehouse(&_game->buildings[BUILDING_STOREHOUSE]);
//
//	//_game->workersList->InitialisationWorkersList();
//
//
//	//_game->buildingsNameTexts = nullptr;
//	//_game->buildWindow.InitTextsBuildWindow(&_game->charlemagneFont);
//}

