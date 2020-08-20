#include "GameDisplay.h"
#include "GameDefinitions.h"


void DisplayDecor(struct Game *_game)
{
	for (int z = 0; z < _game->numberLayers; z++)
	{
		if (z % 3 == SPRITE_ID)
		{
			for (int y = 0; y < _game->numberLines; y++)
			{
				for (int x = 0; x < _game->numberColumns; x++)
				{
					if (_game->map[z][y][x] != 0 && _game->map[z][y][x] > 0)
					{
						sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
						sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

						_game->spriteArray[_game->map[z][y][x]].setScale(_game->scale);

						BlitSprite(_game->spriteArray[_game->map[z][y][x]],
							(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
							(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z),
							0, *_game->window);
					}


					if (_game->actualGameState == BUILD_MODE
						&& z == SECOND_FLOOR + SPRITE_ID)
					{
						if 	(_game->map[z - SECOND_FLOOR][y][x] != 0 && _game->map[z - SECOND_FLOOR][y][x] > 0
							&& _game->buildingCaseSelected.x == x && _game->buildingCaseSelected.y == y)
						{
							sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
							sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

							_game->spriteArray[_game->map[z - SECOND_FLOOR][y][x]].setScale(_game->scale);

							if (_game->isBuildingCaseOccupied == true)
							{
								_game->spriteArray[_game->map[z - SECOND_FLOOR][y][x]].setColor(sf::Color::Red);
							}
							else
							{
								_game->spriteArray[_game->map[z - SECOND_FLOOR][y][x]].setColor(sf::Color::Green);
							}

							BlitSprite(_game->spriteArray[_game->map[z - SECOND_FLOOR][y][x]],
								(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
								(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z),
								0, *_game->window);

							_game->spriteArray[_game->map[z - SECOND_FLOOR][y][x]].setColor(sf::Color::White);
						}
					}
					
				}
			}
		}
	}

	_game->workersList->DisplayWorkersSprite(_game->camera, &_game->workerTest, *_game->window, _game->workersIcons);
}


void DisplayUINormalMode(struct Game *_game)
{
	// Display of the building selection UI
	BlitSprite(_game->buildingUIclosed, SCREEN_WIDTH - _game->buildingUIclosed.getGlobalBounds().width, SCREEN_HEIGHT - _game->buildingUIclosed.getGlobalBounds().height, 0, *_game->window);

}

void DisplayUIBuildingMode(struct Game *_game)
{
	// Display of the building selection UI
	BlitSprite(_game->buildingUI, 1920 - _game->buildingUI.getGlobalBounds().width, 1080 - _game->buildingUI.getGlobalBounds().height, 0, *_game->window);

	for (int i = 0; i < _game->numberOfBuilding; i++)
	{
		if ((SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + _game->scrollBuildingList >= (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height + 40)
			&& (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + _game->scrollBuildingList <= SCREEN_HEIGHT - _game->buildings[i].GetIcon().getGlobalBounds().height - 40)
		{
			BlitSprite(_game->buildings[i].GetIcon(), (SCREEN_WIDTH - _game->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + _game->scrollBuildingList, 0, *_game->window);

			if (_game->money.GetMoneyQuantity() < _game->buildings[i].GetConstructionCost())
			{
				sf::Color color = { 255, 255, 255, 150 };
				_game->blackFilter.setColor(color);

				BlitSprite(_game->blackFilter, (SCREEN_WIDTH - _game->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + _game->scrollBuildingList, 0, *_game->window);
			}

			//std::cout << "Stall : " << _game->stall->GetStatus() << std::endl;

			if (i == BUILDING_STALL
				&& _game->stall->GetConstructionStatus() != BUILDING_DESTROYED)
			{
				sf::Color color = { 255, 255, 255, 150 };
				_game->blackFilter.setColor(color);

				BlitSprite(_game->blackFilter, (SCREEN_WIDTH - _game->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + _game->scrollBuildingList, 0, *_game->window);
			}

			if (_game->buildingsNameTexts != nullptr)
			{
				BlitString(_game->buildingsNameTexts[i], (SCREEN_WIDTH - _game->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109 + 32, (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + _game->scrollBuildingList + 70, *_game->window);
			}
		}

	}
	
	// Display of the destroy button
	if ((SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (_game->numberOfBuilding / 2) + _game->scrollBuildingList >= (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height + 40)
		&& (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (_game->numberOfBuilding / 2) + _game->scrollBuildingList <= SCREEN_HEIGHT - _game->buildingUIdestroyBuildings.getGlobalBounds().height - 40)
	{
		BlitSprite(_game->buildingUIdestroyBuildings, (SCREEN_WIDTH - _game->buildingUI.getGlobalBounds().width) + 143 + (_game->numberOfBuilding % 2) * 109, (1080 - _game->buildingUI.getGlobalBounds().height) + 130 * (_game->numberOfBuilding / 2) + _game->scrollBuildingList, 0, *_game->window);
	}

	if (_game->IDChosenBuilding <= _game->numberOfBuilding)
	{
		if ((SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (_game->IDChosenBuilding / 2) + _game->scrollBuildingList >= (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height + 40)
			&& (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (_game->IDChosenBuilding / 2) + _game->scrollBuildingList <= SCREEN_HEIGHT - _game->contour.getGlobalBounds().height - 40)
		{
			// Display of the contour when a building is selected to be built
			BlitSprite(_game->contour, (SCREEN_WIDTH - _game->buildingUI.getGlobalBounds().width) + 143 + (_game->IDChosenBuilding % 2) * 109 - 4, (SCREEN_HEIGHT - _game->buildingUI.getGlobalBounds().height) + 130 * (_game->IDChosenBuilding / 2) - 4 + _game->scrollBuildingList, 0, *_game->window);
		}
	}

	// Display of the building chosen
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);
	
	if (_game->IDChosenBuilding == 2)
	{
		if (_game->isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->spriteArray[17].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->spriteArray[17].setColor(color);
		}

		BlitSprite(_game->spriteArray[17], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->window);

		_game->spriteArray[17].setColor(sf::Color::White);
	}
	else if (_game->IDChosenBuilding == 3)
	{
		if (_game->isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->spriteArray[16].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->spriteArray[16].setColor(color);
		}

		BlitSprite(_game->spriteArray[16], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->window);

		_game->spriteArray[16].setColor(sf::Color::White);
	}
	else if (_game->IDChosenBuilding == 4)
	{
		if (_game->isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->spriteArray[19].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->spriteArray[19].setColor(color);
		}

		BlitSprite(_game->spriteArray[19], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->window);

		_game->spriteArray[19].setColor(sf::Color::White);
	}
	else if (_game->IDChosenBuilding == 5)
	{
		if (_game->isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->spriteArray[18].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->spriteArray[18].setColor(color);
		}

		BlitSprite(_game->spriteArray[18], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->window);

		_game->spriteArray[18].setColor(sf::Color::White);
	}
	else if (_game->IDChosenBuilding != _game->numberOfBuilding)
	{
		if (_game->isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->spriteArray[3 + _game->IDChosenBuilding].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->spriteArray[3 + _game->IDChosenBuilding].setColor(color);
		}

		BlitSprite(_game->spriteArray[3 + _game->IDChosenBuilding], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->window);

		_game->spriteArray[3 + _game->IDChosenBuilding].setColor(sf::Color::White);
	}

	
	if (_game->isBuildingCaseOccupied == true
		&& !(mousePosition.x > 1920 - _game->buildingUI.getGlobalBounds().width
			&& mousePosition.x < 1920
			&& mousePosition.y > 1080 - _game->buildingUI.getGlobalBounds().height
			&& mousePosition.y < 1080))
	{
		if (!(_game->buildingCaseSelected.x >= 0
			&& _game->buildingCaseSelected.x < _game->numberColumns
			&& _game->buildingCaseSelected.y >= 0
			&& _game->buildingCaseSelected.y < _game->numberLines))
		{
			BlitString(_game->textBuildingCaseOccupied[1], (SCREEN_WIDTH / 2), SCREEN_HEIGHT - 100, *_game->window);
		}
		else if (_game->textBuildingCaseOccupied != nullptr)
		{
			BlitString(_game->textBuildingCaseOccupied[0], (SCREEN_WIDTH / 2), SCREEN_HEIGHT - 100, *_game->window);
		}
	}
}

void DisplayUIGeneral(struct Game *_game)
{
	BlitSprite(_game->money.GetSprite(), 75, 50, 0, *_game->window);

	// Display of the sesterces quantity owned
	BlitString(_game->UITexts[0], *_game->window);
	BlitString(_game->UITexts[1], *_game->window);

	// Display of the bunchs of grape quantity owned
	BlitString(_game->UITexts[2], *_game->window);
	BlitString(_game->UITexts[3], *_game->window);
	// Display of the grape must quantity owned
	BlitString(_game->UITexts[4], *_game->window);
	BlitString(_game->UITexts[5], *_game->window);
	// Display of the grape juice quantity owned
	BlitString(_game->UITexts[6], *_game->window);
	BlitString(_game->UITexts[7], *_game->window);
	// Display of the amphora of wine quantity owned
	BlitString(_game->UITexts[8], *_game->window);
	BlitString(_game->UITexts[9], *_game->window);
}


void DisplayDebugger(struct Game *_game)
{	
	// Display of the mouse position in real time
	BlitString(_game->debbugTexts[0], *_game->window);
	BlitString(_game->debbugTexts[1], *_game->window);

	// Display of the last click position
	BlitString(_game->debbugTexts[2], *_game->window);
	BlitString(_game->debbugTexts[3], *_game->window);
	
	// Display of the last case coordinates
	BlitString(_game->debbugTexts[4], *_game->window);
	BlitString(_game->debbugTexts[5], *_game->window);


	// Display of the last case collisions
	BlitString(_game->debbugTexts[6], *_game->window);
	BlitString(_game->debbugTexts[7], *_game->window);

	// Display of the last case building id
	BlitString(_game->debbugTexts[8], *_game->window);
	BlitString(_game->debbugTexts[9], *_game->window);

	// Display of the last case sprite id
	BlitString(_game->debbugTexts[10], *_game->window);
	BlitString(_game->debbugTexts[11], *_game->window);

}



void GameDisplay(struct Game *_game)
{
	_game->window->clear();

	DisplayDecor(_game);

	if (_game->actualGameState == NORMAL_MODE)
	{
		DisplayUINormalMode(_game);
	}
	else if (_game->actualGameState == BUILD_MODE)
	{
		DisplayUIBuildingMode(_game);
	}
	else if (_game->actualGameState == TEST_PATHFINDING_MODE)
	{
		_game->path.DisplayPathfinding(*_game->window);
	}
	else if (_game->actualGameState == SELLING_WINDOW)
	{
		_game->sellingWindow->DisplaySellingWindow(*_game->window);
	}
	
	DisplayUIGeneral(_game);
	_game->time->DisplayUITime(*_game->window);

	if (_game->isDebuggerModeActive == true)
	{
		DisplayDebugger(_game);
	}

	_game->window->display();
}