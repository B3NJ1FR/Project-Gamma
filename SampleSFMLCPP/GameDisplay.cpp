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
							(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _game->camera.z),
							(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _game->camera.z),
							0, *_game->window);
					}


					if (_game->actualGameState == BUILD_MODE
						&& _game->map[z][y][x] != 0 && _game->map[z][y][x] > 0
						&& _game->buildingCaseSelected.x == x && _game->buildingCaseSelected.y == y)
					{
						sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
						sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

						_game->spriteArray[_game->map[z][y][x]].setScale(_game->scale);
						_game->spriteArray[_game->map[z][y][x]].setColor(sf::Color::Red);

						BlitSprite(_game->spriteArray[_game->map[z][y][x]],
							(1920 / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
							(1080 / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z),
							0, *_game->window);
						_game->spriteArray[_game->map[z][y][x]].setColor(sf::Color::White);
					}
				}
			}
		}
	}
}


void DisplayUINormalMode(struct Game *_game)
{
	// Display of the building selection UI
	BlitSprite(_game->buildingUIclosed, 1920 - _game->buildingUIclosed.getGlobalBounds().width, 1080 - _game->buildingUIclosed.getGlobalBounds().height, 0, *_game->window);

}

void DisplayUIBuildingMode(struct Game *_game)
{
	// Display of the building selection UI
	BlitSprite(_game->buildingUI, 1920 - _game->buildingUI.getGlobalBounds().width, 1080 - _game->buildingUI.getGlobalBounds().height, 0, *_game->window);

	for (int i = 0; i < _game->numberOfBuilding; i++)
	{
		BlitSprite(_game->buildings[i].GetIcon(), (1920 - _game->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (1080 - _game->buildingUI.getGlobalBounds().height) + 80 + 130 * (i / 2), 0, *_game->window);

		if (_game->money.GetMoneyQuantity() < _game->buildings[i].GetConstructionCost())
		{
			sf::Color color = { 255, 255, 255, 150 };
			_game->blackFilter.setColor(color);

			BlitSprite(_game->blackFilter, (1920 - _game->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (1080 - _game->buildingUI.getGlobalBounds().height) + 80 + 130 * (i / 2), 0, *_game->window);
		}
	}

	// Display of the contour when a building is selected to be built
	BlitSprite(_game->contour, (1920 - _game->buildingUI.getGlobalBounds().width) + 143 + (_game->IDChosenBuilding % 2) * 109 - 4, (1080 - _game->buildingUI.getGlobalBounds().height) + 80 + 130 * (_game->IDChosenBuilding / 2) - 4, 0, *_game->window);
	

	// Display of the building chosen
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);

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

	//BlitSprite(_game->workerTest, (150 + (_game->workerPosition.x * (TILE_SIZE + 3))) + 16, (50 + (_game->workerPosition.y * (TILE_SIZE + 3))) + 16, 0, *_game->window);
	
	//sf::Vector2f tileCoordinates = WorldToScreen(_game->workers->GetWorkerPosition().x, _game->workers->GetWorkerPosition().y); // Faire une version stack
	//sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y); // Faire une version stack
	//
	//BlitSprite(_game->workerTest,
	//			(1920 / 2) + (tileCoordinates.x + cameraIso.x /*- ((z / 5) * DIMENSION_THREE_POS_X)*/) / (1 - _game->camera.z),
	//			(1080 / 2) + (tileCoordinates.y + cameraIso.y /*- ((z / 5) * DIMENSION_THREE_POS_X)*/ + TILE_HEIGHT) / (1 - _game->camera.z),
	//			0, *_game->window);

	_game->workersList->DisplayWorkersSprite(_game->camera, &_game->workerTest, *_game->window);
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
	
	DisplayUIGeneral(_game);


	if (_game->isDebuggerModeActive == true)
	{
		DisplayDebugger(_game);
	}

	_game->window->display();
}