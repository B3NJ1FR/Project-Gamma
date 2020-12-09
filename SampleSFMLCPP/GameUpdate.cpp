#include "GameUpdate.h"
#include "GameDefinitions.h"


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
		if (_game->m_buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines), _game->m_buildWindow.GetBuildingCheckboxSelected()))
		{
			UpdateDynamicsTexts(&_game->m_debbugTexts[7], _game->m_map[FIRST_FLOOR + COLLISIONS_ID][_game->m_buildWindow.GetBuildingCheckboxSelected().y][_game->m_buildWindow.GetBuildingCheckboxSelected().x]);
			UpdateDynamicsTexts(&_game->m_debbugTexts[9], _game->m_map[FIRST_FLOOR + BUILDING_ID][_game->m_buildWindow.GetBuildingCheckboxSelected().y][_game->m_buildWindow.GetBuildingCheckboxSelected().x]);
			UpdateDynamicsTexts(&_game->m_debbugTexts[11], _game->m_map[FIRST_FLOOR + SPRITE_ID][_game->m_buildWindow.GetBuildingCheckboxSelected().y][_game->m_buildWindow.GetBuildingCheckboxSelected().x]);
		}
	}

	// UI Texts concerning sesterces and ressources
	UpdateDynamicsTexts(&_game->m_UITexts[1], _game->money.GetMoneyQuantity());
	UpdateDynamicsTexts(&_game->m_UITexts[3], _game->m_ressources[BUNCH_OF_GRAPE].GetQuantityOwned());
	UpdateDynamicsTexts(&_game->m_UITexts[5], _game->m_ressources[GRAPES_MUST].GetQuantityOwned());
	UpdateDynamicsTexts(&_game->m_UITexts[7], _game->m_ressources[GRAPE_JUICE].GetQuantityOwned());
	UpdateDynamicsTexts(&_game->m_UITexts[9], _game->m_ressources[AMPHORA_OF_WINE].GetQuantityOwned());

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
			_game->m_tutorialWindow->UpdateTutorialWindow(&_game->m_charlemagneFont);
		}
		else if (_game->m_actualGameState == TEST_PATHFINDING_MODE)
		{
			//_game->path.MainPathfinding();
		}
		else if (_game->m_actualGameState == SELLING_WINDOW
			&& _game->m_purchasers != nullptr)
		{
			_game->m_sellingWindow->UpdateQuantityConvertedToSell(_game->m_purchasers, _game->stall->GetActualRessourcesStocked());
			_game->m_sellingWindow->UpdateSellingWindowTexts(_game->m_purchasers);
		}
		else
		{
			// Update the workers
			_game->m_workersList->UpdateWorkersLife(_game);
			
			// Update the main character
			_game->m_mainCharacter->InitPathfinding(_game);			
			_game->m_mainCharacter->UpdatePathAndActivities(_game);
		}

		_game->m_time->UpdateFrameTime();


		if (_game->m_actualGameState != BUILD_MODE)
		{
			_game->m_time->UpdateGeneralTime();
			_game->m_time->UpdateMonthToDisplay();

			_game->vines.UpdateVineLife(_game->m_time->GetFrameTime(), _game->m_time->GetActualMonth());
			_game->vines.UpdateVineSprite(_game->m_map);
			_game->vines.UpdateVineProduction(&_game->m_ressources[BUNCH_OF_GRAPE]);

			_game->stompingVats.UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->stompingVats.UpdateBuildingSprite(_game->m_map, BUILDING_GRAPE_STOMPING_VATS);
			_game->stompingVats.UpdateInternalCycles(_game->m_time->GetFrameTime(), &_game->m_ressources[BUNCH_OF_GRAPE], &_game->m_ressources[GRAPES_MUST]);

			_game->winePress.UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->winePress.UpdateBuildingSprite(_game->m_map, BUILDING_WINE_PRESS);
			_game->winePress.UpdateInternalCycles(_game->m_time->GetFrameTime(), &_game->m_ressources[GRAPES_MUST], &_game->m_ressources[GRAPE_JUICE]);

			_game->wineStorehouse.UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->wineStorehouse.UpdateBuildingSprite(_game->m_map, BUILDING_WINE_STOREHOUSE);
			_game->wineStorehouse.UpdateInternalCycles(_game->m_time->GetFrameTime(), &_game->m_ressources[GRAPE_JUICE], &_game->m_ressources[AMPHORA_OF_WINE]);

			_game->stall->UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->stall->UpdateBuildingSprite(_game->m_map);
			_game->stall->UpdateInternalCycles(&_game->money, &_game->m_actualGameState, _game->m_time->GetFrameTime(), &_game->m_ressources[AMPHORA_OF_WINE], _game->m_purchasers, &_game->storehouse);

			_game->storehouse.UpdateBuildingConstruction(_game->m_time->GetFrameTime());
			_game->storehouse.UpdateBuildingSprite(_game->m_map);
			_game->storehouse.UpdateInternalCycles(_game->m_time->GetFrameTime(), &_game->m_ressources[AMPHORA_OF_WINE]);
			

			if (_game->stall->GetStatus() == STALL_SEND_REQUEST_PURCHASER
				&& _game->stall->GetIsNewMerchantNeeded() == true)
			{
				if (_game->m_purchasers != nullptr)
				{
					delete _game->m_purchasers;
					_game->m_purchasers = nullptr;

					std::cout << "Suppression of this actual merchant\n\n";
				}

				_game->stall->SetIsNewMerchantNeeded(false);

				_game->m_purchasers = new Purchasers;
				_game->m_purchasers->Initialisation(_game->stall->GetActualRessourcesStocked());
			}
			else if (_game->stall->GetStatus() == STALL_OFFER_HANDLED
				&& _game->stall->GetIsNewMerchantNeeded() == true)
			{
				if (_game->m_purchasers != nullptr)
				{
					delete _game->m_purchasers;
					_game->m_purchasers = nullptr;

					std::cout << "Suppression of this merchant\n\n";
				}

				_game->stall->SetIsNewMerchantNeeded(false);
			}
		}

		UpdateTexts(_game);
	}
}