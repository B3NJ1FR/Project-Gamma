#include "GameUpdate.h"
#include "GameDefinitions.h"


void UpdateTexts(struct Game *_game)
{
	// Text displayed only when the debugger mode is active
	if (_game->isDebuggerModeActive == true)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);

		UpdateDynamicsTexts(&_game->debbugTexts[1], mousePosition.x, mousePosition.y);
		UpdateDynamicsTexts(&_game->debbugTexts[3], (int)_game->mouseClicked.x, (int)_game->mouseClicked.y);
		UpdateDynamicsTexts(&_game->debbugTexts[5], _game->buildWindow.GetBuildingCheckboxSelected().x, _game->buildWindow.GetBuildingCheckboxSelected().y);

		// Security to avoid an array exit
		if (_game->buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines)))
		{
			UpdateDynamicsTexts(&_game->debbugTexts[7], _game->map[FIRST_FLOOR + COLLISIONS_ID][_game->buildWindow.GetBuildingCheckboxSelected().y][_game->buildWindow.GetBuildingCheckboxSelected().x]);
			UpdateDynamicsTexts(&_game->debbugTexts[9], _game->map[FIRST_FLOOR + BUILDING_ID][_game->buildWindow.GetBuildingCheckboxSelected().y][_game->buildWindow.GetBuildingCheckboxSelected().x]);
			UpdateDynamicsTexts(&_game->debbugTexts[11], _game->map[FIRST_FLOOR + SPRITE_ID][_game->buildWindow.GetBuildingCheckboxSelected().y][_game->buildWindow.GetBuildingCheckboxSelected().x]);
		}
	}

	// UI Texts concerning sesterces and ressources
	UpdateDynamicsTexts(&_game->UITexts[1], _game->money.GetMoneyQuantity());
	UpdateDynamicsTexts(&_game->UITexts[3], _game->ressources[BUNCH_OF_GRAPE].GetQuantityOwned());
	UpdateDynamicsTexts(&_game->UITexts[5], _game->ressources[GRAPES_MUST].GetQuantityOwned());
	UpdateDynamicsTexts(&_game->UITexts[7], _game->ressources[GRAPE_JUICE].GetQuantityOwned());
	UpdateDynamicsTexts(&_game->UITexts[9], _game->ressources[AMPHORA_OF_WINE].GetQuantityOwned());

}



void UpdateBuildingsTimers(struct Game *_game)
{
	   
}


void GameUpdate(struct Game *_game)
{
	if (_game->actualGameState != PAUSE_WINDOW)
	{
		if (_game->actualGameState == BUILD_MODE)
		{
			_game->buildWindow.UpdateBuildWindow(_game);
		}
		else if (_game->actualGameState == TEST_PATHFINDING_MODE)
		{
			//_game->path.MainPathfinding();
		}
		else if (_game->actualGameState == SELLING_WINDOW
			&& _game->purchasers != nullptr)
		{
			_game->sellingWindow->UpdateQuantityConvertedToSell(_game->purchasers, _game->stall->GetActualRessourcesStocked());
			_game->sellingWindow->UpdateSellingWindowTexts(_game->purchasers);
		}
		else
		{
			_game->workersList->UpdateWorkersLife(_game);
		}

		_game->time->UpdateFrameTime();


		if (_game->actualGameState != BUILD_MODE)
		{
			_game->time->UpdateGeneralTime();
			_game->time->UpdateMonthToDisplay();

			_game->vines.UpdateVineLife(_game->time->GetFrameTime(), _game->time->GetActualMonth());
			_game->vines.UpdateVineSprite(_game->map);
			_game->vines.UpdateVineProduction(&_game->ressources[BUNCH_OF_GRAPE]);

			_game->stompingVats.UpdateBuildingConstruction(_game->time->GetFrameTime());
			_game->stompingVats.UpdateInternalCycles(_game->time->GetFrameTime(), &_game->ressources[BUNCH_OF_GRAPE], &_game->ressources[GRAPES_MUST]);

			_game->winePress.UpdateBuildingConstruction(_game->time->GetFrameTime());
			_game->winePress.UpdateInternalCycles(_game->time->GetFrameTime(), &_game->ressources[GRAPES_MUST], &_game->ressources[GRAPE_JUICE]);

			_game->wineStorehouse.UpdateBuildingConstruction(_game->time->GetFrameTime());
			_game->wineStorehouse.UpdateInternalCycles(_game->time->GetFrameTime(), &_game->ressources[GRAPE_JUICE], &_game->ressources[AMPHORA_OF_WINE]);

			_game->stall->UpdateBuildingConstruction(_game->time->GetFrameTime());
			_game->stall->UpdateInternalCycles(&_game->money, &_game->actualGameState, _game->time->GetFrameTime(), &_game->ressources[AMPHORA_OF_WINE], _game->purchasers);

			if (_game->stall->GetStatus() == STALL_SEND_REQUEST_PURCHASER
				&& _game->stall->GetIsNewMerchantNeeded() == true)
			{
				if (_game->purchasers != nullptr)
				{
					delete _game->purchasers;
					_game->purchasers = nullptr;

					std::cout << "Suppression of this actual merchant\n\n";
				}

				_game->stall->SetIsNewMerchantNeeded(false);

				_game->purchasers = new Purchasers;
				_game->purchasers->Initialisation(_game->stall->GetActualRessourcesStocked());
			}
			else if (_game->stall->GetStatus() == STALL_OFFER_HANDLED
				&& _game->stall->GetIsNewMerchantNeeded() == true)
			{
				if (_game->purchasers != nullptr)
				{
					delete _game->purchasers;
					_game->purchasers = nullptr;

					std::cout << "Suppression of this merchant\n\n";
				}

				_game->stall->SetIsNewMerchantNeeded(false);
			}
		}

		UpdateTexts(_game);
	}
}