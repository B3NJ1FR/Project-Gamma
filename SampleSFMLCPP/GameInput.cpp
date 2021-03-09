#include "GameInput.h"
#include "GameDefinitions.h"
#include "RessourcesManager.h"

void CameraInputs(sf::Vector3f *_camera, const float &_frametime, sf::Vector2i _mapSize)
{
	// Camera Inputs

	// Blocage caméra :
	// - prendre en compte le zoom
	// - prendre en compte la taille de la map qui peut fluctuer
	// - prendre en compte que soit x soit y peut être atteint et pas les deux en mm temps

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (_camera->y + CAMERA_SPEED * _frametime < 0)
		{
			_camera->y += CAMERA_SPEED * _frametime;
		}

		if (_camera->x + CAMERA_SPEED * _frametime < 0)
		{
			_camera->x += CAMERA_SPEED * _frametime;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (_camera->y - CAMERA_SPEED * _frametime > -_mapSize.y)
		{
			_camera->y -= CAMERA_SPEED * _frametime;
		}

		if (_camera->x - CAMERA_SPEED * _frametime > -_mapSize.x)
		{
			_camera->x -= CAMERA_SPEED * _frametime;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (_camera->x + CAMERA_SPEED * _frametime < 0)
		{
			_camera->x += CAMERA_SPEED * _frametime;
		}

		if (_camera->y - CAMERA_SPEED * _frametime > -_mapSize.y)
		{
			_camera->y -= CAMERA_SPEED * _frametime;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (_camera->x - CAMERA_SPEED * _frametime > -_mapSize.x)
		{
			_camera->x -= CAMERA_SPEED * _frametime;
		}

		if (_camera->y + CAMERA_SPEED * _frametime < 0)
		{
			_camera->y += CAMERA_SPEED * _frametime;
		}
	}
}



void GameInput(struct Game *_game)
{
	if (_game->m_actualGameState != PAUSE_WINDOW
		&& _game->m_actualGameState != TUTORIAL_MODE)
	{
		sf::Event event;

		while (_game->m_window->pollEvent(event))
		{
			// Closing by pressing the Close button
			if (event.type == sf::Event::Closed)
			{
				exit(EXIT_SUCCESS);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				// If we pressed the escape key, we close the game
				if (event.key.code == sf::Keyboard::Escape && _game->m_actualGameState == VILLA_MANAGEMENT)
				{
					_game->m_actualGameState = NORMAL_MODE;
					_game->m_time->SetTypeOfAcceleration(TypeOfTimeAcceleration::GAME_NORMAL_SPEED);
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					_game->m_actualGameState = PAUSE_WINDOW;
				}

				// When the B keybutton is pressed, we change the state of the game
				if (event.key.code == sf::Keyboard::B && _game->m_actualGameState == VILLA_MANAGEMENT)
				{
					_game->m_actualGameState = BUILD_MODE;
				}
				else if (event.key.code == sf::Keyboard::B && _game->m_actualGameState == BUILD_MODE)
				{
					if (!_game->m_buildingsListPlanned->IsBuildingListIsEmpty())
					{
						_game->m_mainCharacter->SetMainCharacterEndingPosition(_game->m_buildingsListPlanned->GetBuildingPositionInMap(), _game->m_map);
						_game->m_mainCharacter->SetMainCharacterStatus(IDLE, true);
					}
					else
					{
						_game->m_mainCharacter->SetMainCharacterStatus(IDLE, false);
					}

					_game->m_actualGameState = NORMAL_MODE;
					_game->m_time->SetTypeOfAcceleration(TypeOfTimeAcceleration::GAME_NORMAL_SPEED);
				}
				else if (event.key.code == sf::Keyboard::B && _game->m_actualGameState == NORMAL_MODE)
				{
					/*if (_game->m_workersList->GetNumberOfWorkers() > 0)
					{
						_game->m_mainCharacter->SetIsMainCharacterSelected(_game->m_mainCharacter->GetIsMainCharacterSelected() ? false : true);
					}
					else
					{
						_game->m_mainCharacter->SetIsMainCharacterSelected(true);
					}*/
				}

				/*if (event.key.code == sf::Keyboard::B && _game->actualGameState == NORMAL_MODE)
				{
					_game->actualGameState = BUILD_MODE;

					if (_game->buildingsNameTexts == nullptr)
					{
						_game->buildingsNameTexts = new sf::Text[_game->numberOfBuilding];

						for (int i = 0; i < _game->numberOfBuilding; i++)
						{
							LoadTextString(&_game->buildingsNameTexts[i], _game->buildings[i].GetName(), &_game->charlemagneFont, 18, sf::Color::Black, 2);
						}
					}
				}*/

				// When the T keybutton is pressed, we change the state of the game
				/*if (event.key.code == sf::Keyboard::T && _game->actualGameState == NORMAL_MODE)
				{
					_game->actualGameState = TEST_PATHFINDING_MODE;
				}
				else if (event.key.code == sf::Keyboard::T && _game->actualGameState == TEST_PATHFINDING_MODE)
				{
					_game->actualGameState = NORMAL_MODE;
				}*/
				

				if (event.key.code == sf::Keyboard::F3)
				{
					_game->m_isDebuggerModeActive = !_game->m_isDebuggerModeActive;
				}


				/*if (event.key.code == sf::Keyboard::Y && _game->actualGameState == TEST_PATHFINDING_MODE)
				{
					_game->path.MainStatePathfinding();
				}
				if (event.key.code == sf::Keyboard::U && _game->actualGameState == TEST_PATHFINDING_MODE)
				{
					_game->path.isPressingStart = true;
				}
				if (event.key.code == sf::Keyboard::I && _game->actualGameState == TEST_PATHFINDING_MODE)
				{
					_game->path.isPressingEnd = true;
				}*/


				/*if (event.key.code == sf::Keyboard::K)
				{
					_game->time->SetTypeOfAcceleration(enum TypeOfTimeAcceleration(_game->time->GetTypeOfAcceleration() + 1));
				}*/
				

				// Set the game at the Pause State	
				if (event.key.code == sf::Keyboard::P)
				{
					if (_game->m_time->GetTypeOfAcceleration() != TypeOfTimeAcceleration::GAME_PAUSE)
					{
						_game->m_time->SetTypeOfAcceleration(TypeOfTimeAcceleration::GAME_PAUSE);
					}
					else
					{
						_game->m_time->SetTypeOfAcceleration(TypeOfTimeAcceleration::GAME_NORMAL_SPEED);
					}
				}


				// Touche placé sur O, mais à changer	
				if (event.key.code == sf::Keyboard::O
					&& _game->m_actualGameState == NORMAL_MODE)
				{
					// Prix temporaire
					if (_game->m_money.GetMoneyQuantity() - 1000 >= 0)
					{
						_game->m_money.SubtractMoney(1000);

						sf::Vector2i value = { rand() % _game->m_map->GetNumberOfColumns(), rand() % _game->m_map->GetNumberOfLines() };

						// We spawn the workers on the road
						while (_game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][value.y][value.x] != ROAD)
						{
							value.x = rand() % _game->m_map->GetNumberOfColumns();
							value.y = rand() % _game->m_map->GetNumberOfLines();
						}

						_game->m_workersList->AddNewWorkersToList(sf::Vector2f(value));
					}
				}


				// TEST
				if (event.key.code == sf::Keyboard::U)
				{
					//std::cout << _game->ressources[BUNCH_OF_GRAPE].GetName() << " : " << _game->ressources[BUNCH_OF_GRAPE].GetQuantityOwned() << std::endl;


				}
				// TEST
				if (event.key.code == sf::Keyboard::I)
				{
					//std::cout << _game->buildings[3].GetRessourceIDProduced() << " " << _game->buildings[3].GetRessourceQuantityProduced() << std::endl;
					//std::cout << _game->buildings[3].GetRessourceIDNeeded() << " " << _game->buildings[3].GetRessourceIDNeeded(2) << std::endl;

					/*if (_game->actualGameState == SELLING_WINDOW)
					{
						_game->actualGameState = NORMAL_MODE;
					}
					else
					{
						std::cout << "Window de vente\n";
						_game->actualGameState = SELLING_WINDOW;
					}*/
				}

				if (event.key.code == sf::Keyboard::Add
					|| event.key.code == sf::Keyboard::A)
				{
					_game->m_managerBetweenWorkersAndMain->CheckClickKeys(true, _game->m_mainCharacter, _game->m_workersList);

				}
				if (event.key.code == sf::Keyboard::Subtract
					|| event.key.code == sf::Keyboard::E)
				{
					_game->m_managerBetweenWorkersAndMain->CheckClickKeys(false, _game->m_mainCharacter, _game->m_workersList);					
				}

				// DEBUG RESOURCES
				if (_game->m_isDebuggerModeActive)
				{
					if (event.key.code == sf::Keyboard::I)
					{
						RessourcesManager::GetSingleton()->DisplayAllStoragesContent();
					}
					if (event.key.code == sf::Keyboard::U)
					{
						RessourcesManager::GetSingleton()->DisplayWorkersStoragesContent();
					}
					if (event.key.code == sf::Keyboard::Y)
					{
						RessourcesManager::GetSingleton()->DisplayBuildingsStoragesContent();
					}
					if (event.key.code == sf::Keyboard::T)
					{
						RessourcesManager::GetSingleton()->CheatAddResourcesToBuilding(Ressources::GetNameFromEnum(AMPHORA_OF_WINE), 10, "Storehouse");
					}
				}
				

				/*if (event.key.code == sf::Keyboard::M)
				{
					_game->m_save.SaveTheGame(_game);
				}
				if (event.key.code == sf::Keyboard::L)
				{
					_game->m_load.LoadTheGame(_game);
				}*/

				if (_game->m_actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num1)
				{
					_game->m_buildWindow.SetIDChosenBuilding(BUILDING_VINES);
					std::cout << "You've choose the vines\n";
				}
				else if (_game->m_actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num2)
				{
					_game->m_buildWindow.SetIDChosenBuilding(BUILDING_GRAPE_STOMPING_VATS);
					std::cout << "You've choose the stomping vats\n";
				}
				else if (_game->m_actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num3)
				{
					_game->m_buildWindow.SetIDChosenBuilding(BUILDING_WINE_PRESS);
					std::cout << "You've choose the wine press\n";
				}
				else if (_game->m_actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num4)
				{
					_game->m_buildWindow.SetIDChosenBuilding(BUILDING_WINE_STOREHOUSE);
					std::cout << "You've choose the wine storehouse\n";
				}
				else if (_game->m_actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num5)
				{
					_game->m_buildWindow.SetIDChosenBuilding(BUILDING_STOREHOUSE);
					std::cout << "You've choose the \n";
				}
				else if (_game->m_actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num6)
				{
					_game->m_buildWindow.SetIDChosenBuilding(BUILDING_STALL);
					std::cout << "You've choose the stall\n";
				}

			}

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (_game->m_actualGameState != BUILD_MODE)
				{
					if (_game->m_camera.z + (event.mouseWheelScroll.delta / 20) >= MAX_DEZOOMING && (_game->m_camera.z + (event.mouseWheelScroll.delta / 20) <= MAX_ZOOMING))
					{
						int profondeur = 1;

						_game->m_camera.z += event.mouseWheelScroll.delta / 20;

						_game->m_scale.x = 1 / (profondeur - _game->m_camera.z);
						_game->m_scale.y = 1 / (profondeur - _game->m_camera.z);
					}
				}
				else
				{
					sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->m_window);

					// We verify that mouse is in the building UI area
					if (mousePosition.x > _game->m_screenReso->x - _game->m_buildWindow.GetBuildingUI().getGlobalBounds().width
						&& mousePosition.x < _game->m_screenReso->x
						&& mousePosition.y > _game->m_screenReso->y - _game->m_buildWindow.GetBuildingUI().getGlobalBounds().height
						&& mousePosition.y < _game->m_screenReso->y)
					{
						// We check if the scrolling doesn't leave the area
						// The max is dynamically calculated in function of the number of building present in the game
						if (_game->m_buildWindow.GetScrollBuildingList() - (event.mouseWheelScroll.delta * 25) >= -130 * (_game->m_builds.GetNumberOfBuildings() / 2) + 80
							&& (_game->m_buildWindow.GetScrollBuildingList() - (event.mouseWheelScroll.delta * 25) <= 80))
						{
							_game->m_buildWindow.SetScrollBuildingList(_game->m_buildWindow.GetScrollBuildingList() - (event.mouseWheelScroll.delta * 25));
						}
					}
					else
					{
						if (_game->m_camera.z + (event.mouseWheelScroll.delta / 20) >= MAX_DEZOOMING && (_game->m_camera.z + (event.mouseWheelScroll.delta / 20) <= MAX_ZOOMING))
						{
							int profondeur = 1;

							_game->m_camera.z += event.mouseWheelScroll.delta / 20;

							_game->m_scale.x = 1 / (profondeur - _game->m_camera.z);
							_game->m_scale.y = 1 / (profondeur - _game->m_camera.z);
						}
					}
				}

			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (_game->m_actualGameState == NORMAL_MODE)
					{
						_game->m_buildWindow.InputPickUpCaseClicked(*_game->m_window, *_game->m_screenReso, false, sf::Vector2f(_game->m_camera.x, _game->m_camera.y), _game->m_scale);

						// Security to avoid an array exit
						if (_game->m_map->IsCoordinatesIsInMap(_game->m_buildWindow.GetBuildingCheckboxSelected()))
						{
							if (_game->m_mainCharacter->GetIsMainCharacterSelected() == true)
							{
								_game->m_mainCharacter->SetMainCharacterEndingPosition(_game->m_buildWindow.GetBuildingCheckboxSelected(), _game->m_map);

								_game->m_mainCharacter->SetMainCharacterStatus(IDLE, true);
							}
							else
							{
								_game->m_workersList->WorkerListSetEndPosition(_game->m_buildWindow.GetBuildingCheckboxSelected(), _game->m_map->GetMap());
							}
						}

						_game->m_time->InputTimeManagement(*_game->m_window, *_game->m_screenReso);

						_game->m_managerBetweenWorkersAndMain->CheckClickOnArrows(_game->m_mainCharacter, _game->m_workersList, *_game->m_window);
					}
					else if (_game->m_actualGameState == BUILD_MODE)
					{
						_game->m_buildWindow.InputBuildWindow(_game);
						_game->m_buildWindow.InputBuildingModeOldScrollUI(_game->m_buildWindow.GetScrollBuildingList(), *_game->m_window, *_game->m_screenReso);
					}
					else if (_game->m_actualGameState == TEST_PATHFINDING_MODE)
					{

						//sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);

						//if (_game->path.isPressingStart == true)
						//{
						//	_game->path.isPressingStart = false;
						//	_game->path.SetStartingPosition(sf::Vector2i((int)((mousePosition.x - 150) / (TILE_SIZE + 3)), (int)((mousePosition.y - 50) / (TILE_SIZE + 3))));
						//}
						//else if (_game->path.isPressingEnd == true)
						//{
						//	_game->path.isPressingEnd = false;
						//	_game->path.SetEndingPosition(sf::Vector2i((int)((mousePosition.x - 150) / (TILE_SIZE + 3)), (int)((mousePosition.y - 50) / (TILE_SIZE + 3))));
						//	
						//}
						//else
						//{
						//	// If the mouse if out of the building UI, we can try to place a building

						//	//(int)(250 + (x * (TILE_SIZE + 3))), (int)(250 + (y * (TILE_SIZE + 3)))

						//	_game->path.AddObstacle(sf::Vector2i((int)((mousePosition.x - 150) / (TILE_SIZE + 3)), (int)((mousePosition.y - 50) / (TILE_SIZE + 3))));
						//}

						//if (_game->workers->isPressingStart == true)
						//{
						//	_game->workers->isPressingStart = false;
						//	InputPickUpCaseClicked(_game, false);

						//	// Security to avoid an array exit
						//	if (_game->buildingCaseSelected.x >= 0
						//		&& _game->buildingCaseSelected.x < _game->numberColumns
						//		&& _game->buildingCaseSelected.y >= 0
						//		&& _game->buildingCaseSelected.y < _game->numberLines)
						//	{
						//		_game->workers->SetStartingPosition(_game->buildingCaseSelected);
						//	}
						//}
						//else if (_game->workers->isPressingEnd == true)
						//{
						//	_game->workers->isPressingEnd = false;
						//	InputPickUpCaseClicked(_game, false);

						//	// Security to avoid an array exit
						//	if (_game->buildingCaseSelected.x >= 0
						//		&& _game->buildingCaseSelected.x < _game->numberColumns
						//		&& _game->buildingCaseSelected.y >= 0
						//		&& _game->buildingCaseSelected.y < _game->numberLines)
						//	{
						//		_game->workers->SetEndingPosition(_game->buildingCaseSelected);
						//	}						
						//}
						//else
						//{
						//	InputPickUpCaseClicked(_game, false);

						//	// Security to avoid an array exit
						//	if (_game->buildingCaseSelected.x >= 0
						//		&& _game->buildingCaseSelected.x < _game->numberColumns
						//		&& _game->buildingCaseSelected.y >= 0
						//		&& _game->buildingCaseSelected.y < _game->numberLines)
						//	{
						//		_game->workers->AddObstacle(_game->buildingCaseSelected);
						//	}
						//}					
					}
					else if (_game->m_actualGameState == SELLING_WINDOW)
					{
						_game->m_time->InputTimeManagement(*_game->m_window, *_game->m_screenReso);
					}
					else if (_game->m_actualGameState == VILLA_MANAGEMENT)
					{
						_game->m_villaManagement.InputVillaManagement(&_game->m_actualGameState, *_game->m_window);
					}
				}
			}


		}


		CameraInputs(&_game->m_camera, _game->m_time->GetContinuousFrameTime(), sf::Vector2i(_game->m_map->GetNumberOfColumns(), _game->m_map->GetNumberOfLines()));


		// Case clicked in build mode
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (_game->m_actualGameState == SELLING_WINDOW)
			{
				_game->m_sellingWindow->InputSellingWindow(&_game->m_actualGameState, _game->m_builds.m_stall, *_game->m_window, *_game->m_screenReso);
			}
		}
	}
	else if (_game->m_actualGameState == PAUSE_WINDOW)
	{
		_game->m_pauseWindow.InputPauseWindow(_game, &_game->m_save, &_game->m_load);
	}
	else if (_game->m_actualGameState == TUTORIAL_MODE)
	{
		_game->m_tutorialWindow->InputTutorialWindow(&_game->m_actualGameState, *_game->m_window);
	}
}
