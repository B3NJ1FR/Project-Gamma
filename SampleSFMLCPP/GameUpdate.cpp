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
		UpdateDynamicsTexts(&_game->debbugTexts[5], _game->buildingCaseSelected.x, _game->buildingCaseSelected.y);

		// Security to avoid an array exit
		if (_game->buildingCaseSelected.x >= 0
			&& _game->buildingCaseSelected.x < _game->numberColumns
			&& _game->buildingCaseSelected.y >= 0
			&& _game->buildingCaseSelected.y < _game->numberLines)
		{
			UpdateDynamicsTexts(&_game->debbugTexts[7], _game->map[FIRST_FLOOR + COLLISIONS_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x]);
			UpdateDynamicsTexts(&_game->debbugTexts[9], _game->map[FIRST_FLOOR + BUILDING_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x]);
			UpdateDynamicsTexts(&_game->debbugTexts[11], _game->map[FIRST_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x]);
		}
	}

	// UI Texts concerning sesterces and ressources
	UpdateDynamicsTexts(&_game->UITexts[1], _game->money.GetMoneyQuantity());
	UpdateDynamicsTexts(&_game->UITexts[3], _game->ressources[BUNCH_OF_GRAPE].GetQuantityOwned());
	UpdateDynamicsTexts(&_game->UITexts[5], _game->ressources[GRAPES_MUST].GetQuantityOwned());
	UpdateDynamicsTexts(&_game->UITexts[7], _game->ressources[GRAPE_JUICE].GetQuantityOwned());
	UpdateDynamicsTexts(&_game->UITexts[9], _game->ressources[AMPHORA_OF_WINE].GetQuantityOwned());

}

void UpdateBuildingMode(struct Game *_game)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);
	sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

	//std::cout << "Mouse Position : " << mousePosition.x << " & " << mousePosition.y << std::endl;

	_game->buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (1920 / 2)) - cameraIso.x * _game->scale.x),
													((mousePosition.y - (1080 / 2)) - cameraIso.y * _game->scale.y),
													_game->scale);

	//std::cout << "Case : " << _game->buildingCaseSelected.x << " & " << _game->buildingCaseSelected.y << std::endl << std::endl;

	// Security to avoid an array exit
	if (_game->buildingCaseSelected.x >= 0
		&& _game->buildingCaseSelected.x < _game->numberColumns
		&& _game->buildingCaseSelected.y >= 0
		&& _game->buildingCaseSelected.y < _game->numberLines)
	{
		if (_game->IDChosenBuilding >= 0
			&& _game->IDChosenBuilding < _game->numberOfBuilding)
		{
			bool isAreaEmpty = true;

			for (int y = 0; y < _game->buildings[_game->IDChosenBuilding].GetSize().y; y++)
			{
				for (int x = 0; x < _game->buildings[_game->IDChosenBuilding].GetSize().x; x++)
				{
					if (_game->buildingCaseSelected.x - x >= 0
						&& _game->buildingCaseSelected.x - x < _game->numberColumns
						&& _game->buildingCaseSelected.y - y >= 0
						&& _game->buildingCaseSelected.y - y < _game->numberLines)
					{
						// Check case occupation concerning collisions
						if (_game->map[FIRST_FLOOR][_game->buildingCaseSelected.y - y][_game->buildingCaseSelected.x - x] != NO_COLLISION)
						{
							// The case is occupied
							isAreaEmpty = false;

							// We cut the for loop
							y = _game->buildings[_game->IDChosenBuilding].GetSize().y;
							x = _game->buildings[_game->IDChosenBuilding].GetSize().x;
						}
					}
					else
					{
						// The case is occupied
						isAreaEmpty = false;

						// We cut the for loop
						y = _game->buildings[_game->IDChosenBuilding].GetSize().y;
						x = _game->buildings[_game->IDChosenBuilding].GetSize().x;
					}					
				}
			}

			// If we didn't found an occupied place, we call that the place is empty
			if (isAreaEmpty)
			{
				_game->isBuildingCaseOccupied = false;
			}
			else
			{
				_game->isBuildingCaseOccupied = true;
			}
		}
	}
	else
	{
		_game->isBuildingCaseOccupied = true;
	}
}


void UpdateBuildingsTimers(struct Game *_game)
{
	   
}


void GameUpdate(struct Game *_game)
{
	if (_game->actualGameState == BUILD_MODE)
	{
		UpdateBuildingMode(_game);
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
	_game->time->UpdateMonthToDisplay();

	if (_game->actualGameState != BUILD_MODE)
	{
		_game->time->UpdateGeneralTime();

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