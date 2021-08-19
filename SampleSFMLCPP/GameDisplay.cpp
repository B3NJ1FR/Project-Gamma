#include "GameDefinitions.h"


void Game::DisplayDecor()
{
	for (int z = 0; z < m_map->GetNumberOfLayers(); z++)
	{
		if (z % 3 == SPRITE_ID)
		{
			for (int y = 0; y < m_map->GetNumberOfLines(); y++)
			{
				for (int x = 0; x < m_map->GetNumberOfColumns(); x++)
				{
					if (m_map->GetMap()[z][y][x] != 0 && m_map->GetMap()[z][y][x] > 0)
					{
						if (z % 3 == SPRITE_ID)
						{
							sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
							sf::Vector2f cameraIso = WorldToScreen(m_camera.x, m_camera.y);

							// Z - 1 = BUILDING_ID Layer
							// If we find something else than 65535, that mean there is there a building,
							// So we will search our sprite into the buildings sprites array
							unsigned short buildingIDLayer = m_map->GetMap()[z - 1][y][x];
							sf::Sprite buildingSprite;

							if (buildingIDLayer != 65535)
							{
								buildingSprite = m_builds.GetSpriteFromBuildID(buildingIDLayer, m_map->GetMap()[z][y][x]);
							}
							else
							{
								buildingSprite = m_spriteArray[m_map->GetMap()[z][y][x]];
							}

							buildingSprite.setScale(m_scale);

							BlitSprite(buildingSprite,
								(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z),
								(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z),
								0, *m_window);
						}

						// Display of the gear which is rotating when a worker is in a building
						if (z == FIRST_FLOOR + SPRITE_ID)
						{
							sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
							sf::Vector2f cameraIso = WorldToScreen(m_camera.x, m_camera.y);

							if (m_builds.m_vines.ConfirmVinePresenceAtPosition(sf::Vector2f(x, y)))
							{
								if (m_builds.m_vines.IsBuildingIsWorking(sf::Vector2f(x, y)))
								{
									BlitSprite(m_builds.m_stompingVats.GetSpriteWorkerIsThere(),
										(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z),
										(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 110,
										(int)(m_time->GetGeneralTime() * 100) % 180, *m_window);
								}
							}
							else if (m_builds.m_stompingVats.ConfirmSpecificBuildingPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (m_builds.m_stompingVats.IsBuildingIsWorking(sf::Vector2f(x, y)))
								{
									BlitSprite(m_builds.m_stompingVats.GetSpriteWorkerIsThere(),
										(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z),
										(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 110,
										(int)(m_time->GetGeneralTime() * 100) % 180, *m_window);

									int quantityOfWorkersInside = m_builds.m_stompingVats.GetNumberOfWorkersPresents(sf::Vector2f(x, y));
									
									for (int i = 0; i < quantityOfWorkersInside; i++)
									{
										float position = quantityOfWorkersInside * (i - (quantityOfWorkersInside / 2.0f) + 0.5f) * (25.0f / quantityOfWorkersInside);

										BlitSprite(m_builds.m_stompingVats.GetSpriteWorkerInside(),
											(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z) - position,
											(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 75,
											0, *m_window);
									}
								}
							}
							else if (m_builds.m_winePress.ConfirmSpecificBuildingPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (m_builds.m_winePress.IsBuildingIsWorking(sf::Vector2f(x, y)))
								{
									BlitSprite(m_builds.m_winePress.GetSpriteWorkerIsThere(),
										(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z),
										(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 110,
										(int)(m_time->GetGeneralTime() * 100) % 180, *m_window);
									
									int quantityOfWorkersInside = m_builds.m_winePress.GetNumberOfWorkersPresents(sf::Vector2f(x, y));

									for (int i = 0; i < quantityOfWorkersInside; i++)
									{
										float position = quantityOfWorkersInside * (i - (quantityOfWorkersInside / 2.0f) + 0.5f) * (25.0f / quantityOfWorkersInside);

										BlitSprite(m_builds.m_winePress.GetSpriteWorkerInside(),
											(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z) - position,
											(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 75,
											0, *m_window);
									}
								}

							}
							else if (m_builds.m_wineStorehouse.ConfirmSpecificBuildingPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (m_builds.m_wineStorehouse.IsBuildingIsWorking(sf::Vector2f(x, y)))
								{
									BlitSprite(m_builds.m_wineStorehouse.GetSpriteWorkerIsThere(),
										(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z),
										(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 110,
										(int)(m_time->GetGeneralTime() * 100) % 180, *m_window);
									
									int quantityOfWorkersInside = m_builds.m_wineStorehouse.GetNumberOfWorkersPresents(sf::Vector2f(x, y));

									for (int i = 0; i < quantityOfWorkersInside; i++)
									{
										float position = quantityOfWorkersInside * (i - (quantityOfWorkersInside / 2.0f) + 0.5f) * (25.0f / quantityOfWorkersInside);

										BlitSprite(m_builds.m_wineStorehouse.GetSpriteWorkerInside(),
											(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z) - position,
											(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 75,
											0, *m_window);
									}
								}
							}
							else if (m_builds.m_stall->ConfirmPresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (m_builds.m_stall->IsBuildingIsWorking(sf::Vector2f(x, y)))
								{
									BlitSprite(m_builds.m_stompingVats.GetSpriteWorkerIsThere(),
										(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z),
										(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 110,
										(int)(m_time->GetGeneralTime() * 100) % 180, *m_window);
									
									int quantityOfWorkersInside = m_builds.m_stall->GetNumberOfWorkersPresents(sf::Vector2f(x, y));

									for (int i = 0; i < quantityOfWorkersInside; i++)
									{
										float position = quantityOfWorkersInside * (i - (quantityOfWorkersInside / 2.0f) + 0.5f) * (25.0f / quantityOfWorkersInside);

										BlitSprite(m_builds.m_stompingVats.GetSpriteWorkerInside(),
											(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z) - position,
											(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 75,
											0, *m_window);
									}
								}
							}
							else if (m_builds.m_storehouse.ConfirmStorehousePresenceAtPosition(sf::Vector2f(x, y), true))
							{
								if (m_builds.m_storehouse.IsBuildingIsWorking(sf::Vector2f(x, y)))
								{
									BlitSprite(m_builds.m_stompingVats.GetSpriteWorkerIsThere(),
										(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z),
										(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 110,
										(int)(m_time->GetGeneralTime() * 100) % 180, *m_window);

									int quantityOfWorkersInside = m_builds.m_storehouse.GetNumberOfWorkersPresents(sf::Vector2f(x, y));

									for (int i = 0; i < quantityOfWorkersInside; i++)
									{
										float position = quantityOfWorkersInside * (i - (quantityOfWorkersInside / 2.0f) + 0.5f) * (25.0f / quantityOfWorkersInside);

										BlitSprite(m_builds.m_stompingVats.GetSpriteWorkerInside(),
											(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z) - position,
											(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z) - 75,
											0, *m_window);
									}
								}
							}
						}

					}


					if (m_actualGameState == BUILD_MODE)
					{
						if (z == SECOND_FLOOR + SPRITE_ID)
						{
							// Display of the buildings ghost in the Build Mode
							if (m_map->GetMap()[z - SECOND_FLOOR][y][x] != 0 && m_map->GetMap()[z - SECOND_FLOOR][y][x] > 0
								&& m_buildWindow.GetBuildingCheckboxSelected().x >= x && m_buildWindow.GetBuildingCheckboxSelected().x - m_buildWindow.GetSizeBuildingSelected().x < x
								&& m_buildWindow.GetBuildingCheckboxSelected().y >= y && m_buildWindow.GetBuildingCheckboxSelected().y - m_buildWindow.GetSizeBuildingSelected().y < y)
							{
								sf::Vector2f tileCoordinates = WorldToScreen((float)x, (float)y);
								sf::Vector2f cameraIso = WorldToScreen(m_camera.x, m_camera.y);

								m_spriteArray[m_map->GetMap()[z - SECOND_FLOOR][y][x]].setScale(m_scale);

								if (m_buildWindow.GetIsBuildingCaseOccupied() == true)
								{
									m_spriteArray[m_map->GetMap()[z - SECOND_FLOOR][y][x]].setColor(sf::Color::Red);
								}
								else
								{
									m_spriteArray[m_map->GetMap()[z - SECOND_FLOOR][y][x]].setColor(sf::Color::Green);
								}

								BlitSprite(m_spriteArray[m_map->GetMap()[z - SECOND_FLOOR][y][x]],
									(m_screenReso->x / 2) + (tileCoordinates.x + cameraIso.x) / (1 - m_camera.z),
									(m_screenReso->y / 2) + (tileCoordinates.y + cameraIso.y + TILE_HEIGHT) / (1 - m_camera.z),
									0, *m_window);

								m_spriteArray[m_map->GetMap()[z - SECOND_FLOOR][y][x]].setColor(sf::Color::White);
							}
						}
					}



					if (z == FIRST_FLOOR + SPRITE_ID)
					{
						// Display of the main character
						if (m_mainCharacter->IsMainCharacterPosition(sf::Vector2i(x, y)) == true)
						{
							m_mainCharacter->DisplayMainCharacter(sf::Vector2i(x, y), m_camera, *m_screenReso, m_scale, *m_window);
						}

						// Display of the workers
						m_workersList->DisplayWorkersSprite(sf::Vector2i(x, y), m_camera, m_scale, m_mainCharacter->GetIsMainCharacterSelected(), *m_window);
					}
				}
			}
		}
	}
}

void Game::DisplayUIGeneral()
{
	BlitSprite(m_money->GetSprite(), 75, 50, 0, *m_window);

	// Display of the sesterces quantity owned
	BlitString(m_UITexts[0], *m_window);
	BlitString(m_UITexts[1], *m_window);
}


void Game::DisplayDebugger()
{	
	// Display of the mouse position in real time
	BlitString(m_debbugTexts[0], *m_window);
	BlitString(m_debbugTexts[1], *m_window);

	// Display of the last click position
	BlitString(m_debbugTexts[2], *m_window);
	BlitString(m_debbugTexts[3], *m_window);
	
	// Display of the last case coordinates
	BlitString(m_debbugTexts[4], *m_window);
	BlitString(m_debbugTexts[5], *m_window);


	// Display of the last case collisions
	BlitString(m_debbugTexts[6], *m_window);
	BlitString(m_debbugTexts[7], *m_window);

	// Display of the last case building id
	BlitString(m_debbugTexts[8], *m_window);
	BlitString(m_debbugTexts[9], *m_window);

	// Display of the last case sprite id
	BlitString(m_debbugTexts[10], *m_window);
	BlitString(m_debbugTexts[11], *m_window);


	// Display of the bunchs of grape quantity owned
	BlitString(m_UITexts[2], *m_window);
	BlitString(m_UITexts[3], *m_window);
	// Display of the grape must quantity owned
	BlitString(m_UITexts[4], *m_window);
	BlitString(m_UITexts[5], *m_window);
	// Display of the grape juice quantity owned
	BlitString(m_UITexts[6], *m_window);
	BlitString(m_UITexts[7], *m_window);
	// Display of the amphora of wine quantity owned
	BlitString(m_UITexts[8], *m_window);
	BlitString(m_UITexts[9], *m_window);

}



void Game::Display()
{
	m_window->clear();

	if (m_actualGameState == BUILD_MODE) m_buildWindow.DisplayBackground(*m_window);

	DisplayDecor();

	switch (m_actualGameState)
	{
	case NORMAL_MODE:
		m_previousGameState = NORMAL_MODE;
		break;
	case BUILD_MODE:
		m_buildWindow.DisplayBuildWindow(this);
		//DisplayUIBuildingMode();
		m_previousGameState = BUILD_MODE;
		break;
	case TEST_PATHFINDING_MODE:
		//m_path.DisplayPathfinding(*_game->m_window);
		break;
	case SELLING_WINDOW:
		m_sellingWindow->DisplaySellingWindow(*m_window, *m_screenReso);
		m_previousGameState = SELLING_WINDOW;
		break;
	case PAUSE_WINDOW:
		break;
	case VILLA_MANAGEMENT:
		m_villaManagement.DisplayVillaManagement(*m_window, *m_screenReso);
		m_previousGameState = VILLA_MANAGEMENT;
		break;
	case ESTATE_DATA_N_STATISTICS:
		//villaManagement.DisplayVillaManagement(*window);
		m_previousGameState = ESTATE_DATA_N_STATISTICS;
		break;
	default:
		break;
	}

	if (m_actualGameState == TUTORIAL_MODE)
	{
		m_tutorialWindow->DisplayTutorialWindow(*m_window);
		m_previousGameState = TUTORIAL_MODE;
	}

	DisplayUIGeneral();
	m_time->DisplayCalendar(*m_window, *m_screenReso);

	if (m_actualGameState != VILLA_MANAGEMENT
		&& m_actualGameState != BUILD_MODE)
	{
		m_time->DisplayTimeChanger(*m_window, *m_screenReso);

		// --------------- Display of the manager between main character and workers --------------- 

		if (m_mainCharacter->GetIsMainCharacterSelected())
		{
			m_managerBetweenWorkersAndMain->DisplayInsidePortraitFrame(m_mainCharacter->GetSprite(), *m_window);
		}
		else
		{
			m_managerBetweenWorkersAndMain->DisplayInsidePortraitFrame(m_workersList->GetCurrentWorkerSprite(), *m_window);
		}

		m_managerBetweenWorkersAndMain->DisplayManagerBetweenWorkersAndMain(*m_window);
	}



	if (m_isDebuggerModeActive == true)
	{
		DisplayDebugger();
	}

	if (m_actualGameState == PAUSE_WINDOW)
	{
		if (m_previousGameState != NORMAL_MODE)
		{
			switch (m_previousGameState)
			{
			case BUILD_MODE:
				m_buildWindow.DisplayBuildWindow(this);
				//DisplayUIBuildingMode(_game);
				break;
			case SELLING_WINDOW:
				m_sellingWindow->DisplaySellingWindow(*m_window, *m_screenReso);
				break;
			case VILLA_MANAGEMENT:
				m_villaManagement.DisplayVillaManagement(*m_window, *m_screenReso);
				break;
			case ESTATE_DATA_N_STATISTICS:
				//villaManagement.DisplayVillaManagement(*window);
				break;
			default:
				break;
			}
		}

		if (m_previousGameState == TUTORIAL_MODE)
		{
			m_tutorialWindow->DisplayTutorialWindow(*m_window);
		}

		m_pauseWindow.DisplayPauseWindow(*m_window, *m_screenReso);
	}

	m_window->display();
}