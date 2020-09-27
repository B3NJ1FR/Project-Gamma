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
						if (z % 3 == SPRITE_ID)
						{
							if (_game->actualGameState != BUILD_MODE)
							{
								if (!(z == FIRST_FLOOR + SPRITE_ID
									&& _game->map[FIRST_FLOOR + COLLISIONS_ID][y][x] == BUILDING_GHOST))
								{
									sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
									sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

									_game->spriteArray[_game->map[z][y][x]].setScale(_game->scale);

									BlitSprite(_game->spriteArray[_game->map[z][y][x]],
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z),
										0, *_game->window);
								}
							}
							else if (_game->actualGameState == BUILD_MODE)
							{
								if (z == ZERO_FLOOR + SPRITE_ID)
								{
									if (_game->map[z - 2][y][x] == BUILDING_GHOST)
									{
										sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
										sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);
										
										int groundToDisplay(RESET);

										// We get the ground of the building concerned
										switch (_game->map[z - 1][y][x])
										{
										case BUILDING_VINES:
											groundToDisplay = 7;
											break;
										case BUILDING_GRAPE_STOMPING_VATS:
											groundToDisplay = 25;
											break;
										case BUILDING_WINE_PRESS:
											groundToDisplay = 23;
											break;
										case BUILDING_WINE_STOREHOUSE:
											groundToDisplay = 24;
											break;
										case BUILDING_STOREHOUSE:
											groundToDisplay = 22;
											break;
										case BUILDING_STALL:
											groundToDisplay = 20;
											break;
											/*case BUILDING_VILLA:
												break;
											case BUILDING_DORMITORY:
												break;*/
										default:
											break;
										}

										_game->spriteArray[groundToDisplay].setScale(_game->scale);

										BlitSprite(_game->spriteArray[groundToDisplay],
											(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
											(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z),
											0, *_game->window);
									}
									else
									{
										sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
										sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

										_game->spriteArray[_game->map[z][y][x]].setScale(_game->scale);

										BlitSprite(_game->spriteArray[_game->map[z][y][x]],
											(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
											(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z),
											0, *_game->window);
									}
								}
								else if (z == FIRST_FLOOR + SPRITE_ID)
								{
									if (_game->map[FIRST_FLOOR + COLLISIONS_ID][y][x] == BUILDING_GHOST)
									{
										sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
										sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

										int buildingToDisplay(RESET);

										// We get the building concerned
										switch (_game->map[z - 1][y][x])
										{
										case BUILDING_VINES:
											buildingToDisplay = 32;
											break;
										case BUILDING_GRAPE_STOMPING_VATS:
											buildingToDisplay = 26;
											break;
										case BUILDING_WINE_PRESS:
											buildingToDisplay = 30;
											break;
										case BUILDING_WINE_STOREHOUSE:
											buildingToDisplay = 31;
											break;
										case BUILDING_STOREHOUSE:
											buildingToDisplay = 29;
											break;
										case BUILDING_STALL:
											buildingToDisplay = 27;
											break;
											/*case BUILDING_VILLA:
												break;
											case BUILDING_DORMITORY:
												break;*/
										default:
											break;
										}

										_game->spriteArray[buildingToDisplay].setScale(_game->scale);

										BlitSprite(_game->spriteArray[buildingToDisplay],
											(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
											(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z),
											0, *_game->window);
									}
									else
									{
										sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
										sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

										_game->spriteArray[_game->map[z][y][x]].setScale(_game->scale);

										BlitSprite(_game->spriteArray[_game->map[z][y][x]],
											(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
											(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z),
											0, *_game->window);
									}
								}
							}
						}


						


						// Display of the gear which is rotating when a worker is in a building
						if (z == FIRST_FLOOR + SPRITE_ID)
						{
							sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
							sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

							if (_game->vines.ConfirmVinePresenceAtPosition(sf::Vector2f(x, y)))
							{
								if (_game->vines.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->stompingVats.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z) - 110,
										(int)(_game->time->GetGeneralTime() * 100) % 180, *_game->window);
								}
							}
							else if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->stompingVats.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->stompingVats.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z) - 110,
										(int)(_game->time->GetGeneralTime() * 100) % 180, *_game->window);
								}
							}
							else if (_game->winePress.ConfirmSpecificBuildingPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->winePress.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->winePress.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z) - 110,
										(int)(_game->time->GetGeneralTime() * 100) % 180, *_game->window);
								}

							}
							else if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->wineStorehouse.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->wineStorehouse.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z) - 110,
										(int)(_game->time->GetGeneralTime() * 100) % 180, *_game->window);
								}
							}
							else if (_game->stall->ConfirmPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->stall->GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->wineStorehouse.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z) - 110,
										(int)(_game->time->GetGeneralTime() * 100) % 180, *_game->window);
								}
							}
							else if (_game->storehouse.ConfirmStorehousePresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->storehouse.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->wineStorehouse.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z) - 110,
										(int)(_game->time->GetGeneralTime() * 100) % 180, *_game->window);
								}
							}
						}

					}				


					if (_game->actualGameState == BUILD_MODE)
					{
						if (z == SECOND_FLOOR + SPRITE_ID)
						{
							// Display of the buildings ghost in the Build Mode
							if (_game->map[z - SECOND_FLOOR][y][x] != 0 && _game->map[z - SECOND_FLOOR][y][x] > 0
								&& _game->buildWindow.GetBuildingCheckboxSelected().x == x && _game->buildWindow.GetBuildingCheckboxSelected().y == y)
							{
								sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
								sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

								_game->spriteArray[_game->map[z - SECOND_FLOOR][y][x]].setScale(_game->scale);

								if (_game->buildWindow.GetIsBuildingCaseOccupied() == true)
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


					
					if (z == FIRST_FLOOR + SPRITE_ID)
					{
						// Display of the main character
						if (_game->mainCharacter->IsMainCharacterPosition(sf::Vector2i(x, y)) == true)
						{
							sf::Vector2f tileCoordinates = WorldToScreen(_game->mainCharacter->GetMainCharacterPosition());
							sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

							_game->mainCharacter->SetSpriteScale(_game->scale);

							BlitSprite(_game->mainCharacter->GetSprite(),
								(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->camera.z),
								(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->camera.z) - 16,
								0, *_game->window);
						}

						// Display of the workers
						_game->workersList->DisplayWorkersSprite(sf::Vector2i(x, y), _game->camera, _game->scale, *_game->window);
					}
				}
			}
		}
	}
}


void DisplayUINormalMode(struct Game *_game)
{
	// Display of the building selection UI
//	BlitSprite(_game->buildWindow.GetBuildingUIClosed(), SCREEN_WIDTH - _game->buildWindow.GetBuildingUIClosed().getGlobalBounds().width, SCREEN_HEIGHT - _game->buildWindow.GetBuildingUIClosed().getGlobalBounds().height, 0, *_game->window);
}


void DisplayUIGeneral(struct Game *_game)
{
	BlitSprite(_game->money.GetSprite(), 75, 50, 0, *_game->window);

	// Display of the sesterces quantity owned
	BlitString(_game->UITexts[0], *_game->window);
	BlitString(_game->UITexts[1], *_game->window);
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



void GameDisplay(struct Game *_game)
{
	_game->window->clear();

	DisplayDecor(_game);

	if (_game->actualGameState == NORMAL_MODE)
	{
		DisplayUINormalMode(_game);
		_game->previousGameState = NORMAL_MODE;
	}
	else if (_game->actualGameState == BUILD_MODE)
	{
		_game->buildWindow.DisplayBuildWindow(_game);
		//DisplayUIBuildingMode(_game);
		_game->previousGameState = BUILD_MODE;
	}
	else if (_game->actualGameState == TEST_PATHFINDING_MODE)
	{
		_game->path.DisplayPathfinding(*_game->window);
	}
	else if (_game->actualGameState == SELLING_WINDOW)
	{
		_game->sellingWindow->DisplaySellingWindow(*_game->window);
		_game->previousGameState = SELLING_WINDOW;
	}
	else if (_game->actualGameState == VILLA_MANAGEMENT)
	{
		_game->villaManagement.DisplayVillaManagement(*_game->window);
		_game->previousGameState = VILLA_MANAGEMENT;
	}
	else if (_game->actualGameState == ESTATE_DATA_N_STATISTICS)
	{
		//_game->villaManagement.DisplayVillaManagement(*_game->window);
		_game->previousGameState = ESTATE_DATA_N_STATISTICS;
	}

	if (_game->actualGameState == TUTORIAL_MODE)
	{
		_game->tutorialWindow->DisplayTutorialWindow(*_game->window);
		_game->previousGameState = TUTORIAL_MODE;
	}

	DisplayUIGeneral(_game);
	_game->time->DisplayUITime(*_game->window);

	if (_game->isDebuggerModeActive == true)
	{
		DisplayDebugger(_game);
	}

	if (_game->actualGameState == PAUSE_WINDOW)
	{
		if (_game->previousGameState == NORMAL_MODE)
		{
			DisplayUINormalMode(_game);
		}
		else if (_game->previousGameState == BUILD_MODE)
		{
			_game->buildWindow.DisplayBuildWindow(_game);
			//DisplayUIBuildingMode(_game);
		}
		else if (_game->previousGameState == SELLING_WINDOW)
		{
			_game->sellingWindow->DisplaySellingWindow(*_game->window);
		}
		else if (_game->previousGameState == VILLA_MANAGEMENT)
		{
			_game->villaManagement.DisplayVillaManagement(*_game->window);
		}
		else if (_game->previousGameState == ESTATE_DATA_N_STATISTICS)
		{
			//_game->villaManagement.DisplayVillaManagement(*_game->window);
		}

		if (_game->previousGameState == TUTORIAL_MODE)
		{
			_game->tutorialWindow->DisplayTutorialWindow(*_game->window);
		}

		_game->pauseWindow.DisplayPauseWindow(*_game->window);
	}

	_game->window->display();
}