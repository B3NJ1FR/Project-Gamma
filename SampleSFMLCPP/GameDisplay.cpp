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
					if (_game->m_map[z][y][x] != 0 && _game->m_map[z][y][x] > 0)
					{
						if (z % 3 == SPRITE_ID)
						{
							if (_game->m_actualGameState != BUILD_MODE)
							{
								if (!(z == FIRST_FLOOR + SPRITE_ID
									&& _game->m_map[FIRST_FLOOR + COLLISIONS_ID][y][x] == BUILDING_GHOST))
								{
									sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
									sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);

									_game->m_spriteArray[_game->m_map[z][y][x]].setScale(_game->m_scale);

									BlitSprite(_game->m_spriteArray[_game->m_map[z][y][x]],
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z),
										0, *_game->m_window);
								}
							}
							else if (_game->m_actualGameState == BUILD_MODE)
							{
								if (z == ZERO_FLOOR + SPRITE_ID)
								{
									if (_game->m_map[z - 2][y][x] == BUILDING_GHOST)
									{
										sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
										sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);
										
										int groundToDisplay(RESET);

										// We get the ground of the building concerned
										switch (_game->m_map[z - 1][y][x])
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

										_game->m_spriteArray[groundToDisplay].setScale(_game->m_scale);

										BlitSprite(_game->m_spriteArray[groundToDisplay],
											(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
											(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z),
											0, *_game->m_window);
									}
									else
									{
										sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
										sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);

										_game->m_spriteArray[_game->m_map[z][y][x]].setScale(_game->m_scale);

										BlitSprite(_game->m_spriteArray[_game->m_map[z][y][x]],
											(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
											(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z),
											0, *_game->m_window);
									}
								}
								else if (z == FIRST_FLOOR + SPRITE_ID)
								{
									if (_game->m_map[FIRST_FLOOR + COLLISIONS_ID][y][x] == BUILDING_GHOST)
									{
										sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
										sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);

										int buildingToDisplay(RESET);

										// We get the building concerned
										switch (_game->m_map[z - 1][y][x])
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

										_game->m_spriteArray[buildingToDisplay].setScale(_game->m_scale);

										BlitSprite(_game->m_spriteArray[buildingToDisplay],
											(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
											(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z),
											0, *_game->m_window);
									}
									else
									{
										sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
										sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);

										_game->m_spriteArray[_game->m_map[z][y][x]].setScale(_game->m_scale);

										BlitSprite(_game->m_spriteArray[_game->m_map[z][y][x]],
											(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
											(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z),
											0, *_game->m_window);
									}
								}
							}
						}


						


						// Display of the gear which is rotating when a worker is in a building
						if (z == FIRST_FLOOR + SPRITE_ID)
						{
							sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
							sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);

							if (_game->vines.ConfirmVinePresenceAtPosition(sf::Vector2f(x, y)))
							{
								if (_game->vines.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->stompingVats.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z) - 110,
										(int)(_game->m_time->GetGeneralTime() * 100) % 180, *_game->m_window);
								}
							}
							else if (_game->stompingVats.ConfirmSpecificBuildingPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->stompingVats.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->stompingVats.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z) - 110,
										(int)(_game->m_time->GetGeneralTime() * 100) % 180, *_game->m_window);
								}
							}
							else if (_game->winePress.ConfirmSpecificBuildingPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->winePress.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->winePress.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z) - 110,
										(int)(_game->m_time->GetGeneralTime() * 100) % 180, *_game->m_window);
								}

							}
							else if (_game->wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->wineStorehouse.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->wineStorehouse.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z) - 110,
										(int)(_game->m_time->GetGeneralTime() * 100) % 180, *_game->m_window);
								}
							}
							else if (_game->stall->ConfirmPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->stall->GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->wineStorehouse.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z) - 110,
										(int)(_game->m_time->GetGeneralTime() * 100) % 180, *_game->m_window);
								}
							}
							else if (_game->storehouse.ConfirmStorehousePresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (_game->storehouse.GetWorkerIsThere(sf::Vector2f(x, y)))
								{
									BlitSprite(_game->wineStorehouse.GetSpriteWorkerIsThere(),
										(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
										(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z) - 110,
										(int)(_game->m_time->GetGeneralTime() * 100) % 180, *_game->m_window);
								}
							}
						}

					}				


					if (_game->m_actualGameState == BUILD_MODE)
					{
						if (z == SECOND_FLOOR + SPRITE_ID)
						{
							// Display of the buildings ghost in the Build Mode
							if (_game->m_map[z - SECOND_FLOOR][y][x] != 0 && _game->m_map[z - SECOND_FLOOR][y][x] > 0
								&& _game->m_buildWindow.GetBuildingCheckboxSelected().x == x && _game->m_buildWindow.GetBuildingCheckboxSelected().y == y)
							{
								sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
								sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);

								_game->m_spriteArray[_game->m_map[z - SECOND_FLOOR][y][x]].setScale(_game->m_scale);

								if (_game->m_buildWindow.GetIsBuildingCaseOccupied() == true)
								{
									_game->m_spriteArray[_game->m_map[z - SECOND_FLOOR][y][x]].setColor(sf::Color::Red);
								}
								else
								{
									_game->m_spriteArray[_game->m_map[z - SECOND_FLOOR][y][x]].setColor(sf::Color::Green);
								}

								BlitSprite(_game->m_spriteArray[_game->m_map[z - SECOND_FLOOR][y][x]],
									(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
									(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z),
									0, *_game->m_window);

								_game->m_spriteArray[_game->m_map[z - SECOND_FLOOR][y][x]].setColor(sf::Color::White);
							}
						}
					}


					
					if (z == FIRST_FLOOR + SPRITE_ID)
					{
						// Display of the main character
						if (_game->m_mainCharacter->IsMainCharacterPosition(sf::Vector2i(x, y)) == true)
						{
							sf::Vector2f tileCoordinates = WorldToScreen(_game->m_mainCharacter->GetMainCharacterPosition());
							sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);

							_game->m_mainCharacter->SetSpriteScale(_game->m_scale);

							BlitSprite(_game->m_mainCharacter->GetSprite(),
								(SCREEN_WIDTH / 2) + (tileCoordinates.x + cameraIso.x) / (1 - _game->m_camera.z),
								(SCREEN_HEIGHT / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - _game->m_camera.z) - 16,
								0, *_game->m_window);
						}

						// Display of the workers
						_game->m_workersList->DisplayWorkersSprite(sf::Vector2i(x, y), _game->m_camera, _game->m_scale, *_game->m_window);
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
	BlitSprite(_game->money.GetSprite(), 75, 50, 0, *_game->m_window);

	// Display of the sesterces quantity owned
	BlitString(_game->m_UITexts[0], *_game->m_window);
	BlitString(_game->m_UITexts[1], *_game->m_window);
}


void DisplayDebugger(struct Game *_game)
{	
	// Display of the mouse position in real time
	BlitString(_game->m_debbugTexts[0], *_game->m_window);
	BlitString(_game->m_debbugTexts[1], *_game->m_window);

	// Display of the last click position
	BlitString(_game->m_debbugTexts[2], *_game->m_window);
	BlitString(_game->m_debbugTexts[3], *_game->m_window);
	
	// Display of the last case coordinates
	BlitString(_game->m_debbugTexts[4], *_game->m_window);
	BlitString(_game->m_debbugTexts[5], *_game->m_window);


	// Display of the last case collisions
	BlitString(_game->m_debbugTexts[6], *_game->m_window);
	BlitString(_game->m_debbugTexts[7], *_game->m_window);

	// Display of the last case building id
	BlitString(_game->m_debbugTexts[8], *_game->m_window);
	BlitString(_game->m_debbugTexts[9], *_game->m_window);

	// Display of the last case sprite id
	BlitString(_game->m_debbugTexts[10], *_game->m_window);
	BlitString(_game->m_debbugTexts[11], *_game->m_window);


	// Display of the bunchs of grape quantity owned
	BlitString(_game->m_UITexts[2], *_game->m_window);
	BlitString(_game->m_UITexts[3], *_game->m_window);
	// Display of the grape must quantity owned
	BlitString(_game->m_UITexts[4], *_game->m_window);
	BlitString(_game->m_UITexts[5], *_game->m_window);
	// Display of the grape juice quantity owned
	BlitString(_game->m_UITexts[6], *_game->m_window);
	BlitString(_game->m_UITexts[7], *_game->m_window);
	// Display of the amphora of wine quantity owned
	BlitString(_game->m_UITexts[8], *_game->m_window);
	BlitString(_game->m_UITexts[9], *_game->m_window);

}



void GameDisplay(struct Game *_game)
{
	_game->m_window->clear();

	DisplayDecor(_game);

	if (_game->m_actualGameState == NORMAL_MODE)
	{
		DisplayUINormalMode(_game);
		_game->m_previousGameState = NORMAL_MODE;
	}
	else if (_game->m_actualGameState == BUILD_MODE)
	{
		_game->m_buildWindow.DisplayBuildWindow(_game);
		//DisplayUIBuildingMode(_game);
		_game->m_previousGameState = BUILD_MODE;
	}
	else if (_game->m_actualGameState == TEST_PATHFINDING_MODE)
	{
		_game->path.DisplayPathfinding(*_game->m_window);
	}
	else if (_game->m_actualGameState == SELLING_WINDOW)
	{
		_game->m_sellingWindow->DisplaySellingWindow(*_game->m_window);
		_game->m_previousGameState = SELLING_WINDOW;
	}
	else if (_game->m_actualGameState == VILLA_MANAGEMENT)
	{
		_game->villaManagement.DisplayVillaManagement(*_game->m_window);
		_game->m_previousGameState = VILLA_MANAGEMENT;
	}
	else if (_game->m_actualGameState == ESTATE_DATA_N_STATISTICS)
	{
		//_game->villaManagement.DisplayVillaManagement(*_game->window);
		_game->m_previousGameState = ESTATE_DATA_N_STATISTICS;
	}

	if (_game->m_actualGameState == TUTORIAL_MODE)
	{
		_game->m_tutorialWindow->DisplayTutorialWindow(*_game->m_window);
		_game->m_previousGameState = TUTORIAL_MODE;
	}

	DisplayUIGeneral(_game);
	_game->m_time->DisplayUITime(*_game->m_window);

	if (_game->m_isDebuggerModeActive == true)
	{
		DisplayDebugger(_game);
	}

	if (_game->m_actualGameState == PAUSE_WINDOW)
	{
		if (_game->m_previousGameState == NORMAL_MODE)
		{
			DisplayUINormalMode(_game);
		}
		else if (_game->m_previousGameState == BUILD_MODE)
		{
			_game->m_buildWindow.DisplayBuildWindow(_game);
			//DisplayUIBuildingMode(_game);
		}
		else if (_game->m_previousGameState == SELLING_WINDOW)
		{
			_game->m_sellingWindow->DisplaySellingWindow(*_game->m_window);
		}
		else if (_game->m_previousGameState == VILLA_MANAGEMENT)
		{
			_game->villaManagement.DisplayVillaManagement(*_game->m_window);
		}
		else if (_game->m_previousGameState == ESTATE_DATA_N_STATISTICS)
		{
			//_game->villaManagement.DisplayVillaManagement(*_game->window);
		}

		if (_game->m_previousGameState == TUTORIAL_MODE)
		{
			_game->m_tutorialWindow->DisplayTutorialWindow(*_game->m_window);
		}

		_game->pauseWindow.DisplayPauseWindow(*_game->m_window);
	}

	_game->m_window->display();
}