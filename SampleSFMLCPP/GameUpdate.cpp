#include "GameUpdate.h"
#include "GameDefinitions.h"
#include "RessourcesManager.h"


void UpdateTexts(struct Game *_game)
{
	// Text displayed only when the debugger mode is active
	if (_game->m_isDebuggerModeActive == true)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->m_window);

		UpdateDynamicsTexts(&_game->m_debbugTexts[1], mousePosition.x, mousePosition.y);
		UpdateDynamicsTexts(&_game->m_debbugTexts[3], (int)_game->m_mouseClicked.x, (int)_game->m_mouseClicked.y);
		UpdateDynamicsTexts(&_game->m_debbugTexts[5], _game->m_buildWindow.GetBuildingCheckboxSelected().x, _game->m_buildWindow.GetBuildingCheckboxSelected().y);

		// Security to avoid an array exit
		if (_game->m_map.IsCoordinatesIsInMap(_game->m_buildWindow.GetBuildingCheckboxSelected()))
		{
			UpdateDynamicsTexts(&_game->m_debbugTexts[7], _game->m_map.GetMap()[FIRST_FLOOR + COLLISIONS_ID][_game->m_buildWindow.GetBuildingCheckboxSelected().y][_game->m_buildWindow.GetBuildingCheckboxSelected().x]);
			UpdateDynamicsTexts(&_game->m_debbugTexts[9], _game->m_map.GetMap()[FIRST_FLOOR + BUILDING_ID][_game->m_buildWindow.GetBuildingCheckboxSelected().y][_game->m_buildWindow.GetBuildingCheckboxSelected().x]);
			UpdateDynamicsTexts(&_game->m_debbugTexts[11], _game->m_map.GetMap()[FIRST_FLOOR + SPRITE_ID][_game->m_buildWindow.GetBuildingCheckboxSelected().y][_game->m_buildWindow.GetBuildingCheckboxSelected().x]);
		}
	}

	// UI Texts concerning sesterces and ressources
	UpdateDynamicsTexts(&_game->m_UITexts[1], _game->m_money.GetMoneyQuantity());
	UpdateDynamicsTexts(&_game->m_UITexts[3], RessourcesManager::GetSingleton()->GetResourceQuantity(Ressources::GetNameFromEnum(BUNCH_OF_GRAPE)));
	UpdateDynamicsTexts(&_game->m_UITexts[5], RessourcesManager::GetSingleton()->GetResourceQuantity(Ressources::GetNameFromEnum(GRAPES_MUST)));
	UpdateDynamicsTexts(&_game->m_UITexts[7], RessourcesManager::GetSingleton()->GetResourceQuantity(Ressources::GetNameFromEnum(GRAPE_JUICE)));
	UpdateDynamicsTexts(&_game->m_UITexts[9], RessourcesManager::GetSingleton()->GetResourceQuantity(Ressources::GetNameFromEnum(AMPHORA_OF_WINE)));

}



void UpdateBuildingsTimers(struct Game *_game)
{
	   
}


void GameUpdate(struct Game *_game)
{
	if (_game->m_actualGameState != PAUSE_WINDOW)
	{
		if (_game->m_actualGameState == BUILD_MODE)
		{
			_game->m_buildWindow.UpdateBuildWindow(_game);
		}
		else if (_game->m_actualGameState == TUTORIAL_MODE)
		{
			_game->m_tutorialWindow->UpdateTutorialWindow(&_game->m_actualGameState, &_game->m_charlemagneFont);
		}
		else if (_game->m_actualGameState == TEST_PATHFINDING_MODE)
		{
			//_game->path.MainPathfinding();
		}
		else if (_game->m_actualGameState == SELLING_WINDOW
			&& _game->m_purchasers != nullptr)
		{
			//_game->m_sellingWindow->UpdateQuantityConvertedToSell(_game->m_purchasers, _game->m_builds.m_stall->GetActualRessourcesStocked(), *_game->m_screenReso);
			_game->m_sellingWindow->UpdateQuantityConvertedToSell(_game->m_purchasers, _game->m_ressources[AMPHORA_OF_WINE].GetQuantityOwned(), *_game->m_screenReso);
			_game->m_sellingWindow->UpdateSellingWindowTexts(_game->m_purchasers);
		}
		else
		{
			// Update the workers
			_game->m_workersList->UpdateWorkersLife(_game);
			
			// Update the main character
			_game->m_mainCharacter->InitPathfinding(&_game->m_map);			
			_game->m_mainCharacter->UpdatePathAndActivities(_game);
		}

		_game->m_time->UpdateFrameTime();


		if (_game->m_actualGameState != BUILD_MODE)
		{
			_game->m_time->UpdateGeneralTime();
			_game->m_time->UpdateMonthToDisplay();


			_game->m_builds.UpdateBuildingManagement(&_game->m_map);

			_game->m_builds.m_vines.UpdateVineLife(_game->m_time->GetFrameTime(), _game->m_time->GetActualMonth());
			_game->m_builds.m_vines.UpdateVineSprite(_game->m_map.GetMap());
			_game->m_builds.m_vines.UpdateVineProduction(&_game->m_ressources[BUNCH_OF_GRAPE]);

			_game->m_builds.m_stompingVats.UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->m_builds.m_stompingVats.UpdateBuildingSprite(_game->m_map.GetMap(), BUILDING_GRAPE_STOMPING_VATS);
			_game->m_builds.m_stompingVats.UpdateInternalCycles(_game->m_time->GetFrameTime());
			_game->m_builds.m_stompingVats.UpdateBuildingProduction();

			_game->m_builds.m_winePress.UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->m_builds.m_winePress.UpdateBuildingSprite(_game->m_map.GetMap(), BUILDING_WINE_PRESS);
			_game->m_builds.m_winePress.UpdateInternalCycles(_game->m_time->GetFrameTime());
			_game->m_builds.m_winePress.UpdateBuildingProduction();

			_game->m_builds.m_wineStorehouse.UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->m_builds.m_wineStorehouse.UpdateBuildingSprite(_game->m_map.GetMap(), BUILDING_WINE_STOREHOUSE);
			_game->m_builds.m_wineStorehouse.UpdateInternalCycles(_game->m_time->GetFrameTime());
			_game->m_builds.m_wineStorehouse.UpdateBuildingProduction();

			_game->m_builds.m_stall->UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->m_builds.m_stall->UpdateBuildingSprite(_game->m_map.GetMap());
			_game->m_builds.m_stall->UpdateInternalCycles(&_game->m_money, &_game->m_actualGameState, _game->m_time->GetFrameTime(), &_game->m_ressources[AMPHORA_OF_WINE], _game->m_purchasers, &_game->m_builds.m_storehouse);

			_game->m_builds.m_storehouse.UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->m_builds.m_storehouse.UpdateBuildingSprite(_game->m_map.GetMap());
			_game->m_builds.m_storehouse.UpdateInternalCycles(_game->m_time->GetFrameTime());
			

			if (_game->m_builds.m_stall->GetStatus() == STALL_SEND_REQUEST_PURCHASER
				&& _game->m_builds.m_stall->GetIsNewMerchantNeeded() == true)
			{
				if (_game->m_purchasers != nullptr)
				{
					delete _game->m_purchasers;
					_game->m_purchasers = nullptr;

					std::cout << "Suppression of this actual merchant\n\n";
				}

				_game->m_builds.m_stall->SetIsNewMerchantNeeded(false);

				_game->m_purchasers = new Purchasers;
				_game->m_purchasers->Initialisation(_game->m_builds.m_stall->GetActualRessourcesStocked());
			}
			else if (_game->m_builds.m_stall->GetStatus() == STALL_OFFER_HANDLED
				&& _game->m_builds.m_stall->GetIsNewMerchantNeeded() == true)
			{
				if (_game->m_purchasers != nullptr)
				{
					delete _game->m_purchasers;
					_game->m_purchasers = nullptr;

					std::cout << "Suppression of this merchant\n\n";
				}

				_game->m_builds.m_stall->SetIsNewMerchantNeeded(false);
			}
		}

		UpdateTexts(_game);
	}
}