#include "GameInput.h"
#include "GameDefinitions.h"

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
	if (_game->actualGameState != PAUSE_WINDOW
		&& _game->actualGameState != TUTORIAL_MODE)
	{
		sf::Event event;
		while (_game->window->pollEvent(event))
		{
			// Closing by pressing the Close button
			if (event.type == sf::Event::Closed)
			{
				exit(EXIT_SUCCESS);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				// If we pressed the escape key, we close the game
				if (event.key.code == sf::Keyboard::Escape && _game->actualGameState == VILLA_MANAGEMENT)
				{
					_game->actualGameState = NORMAL_MODE;
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					_game->actualGameState = PAUSE_WINDOW;
				}

				// When the B keybutton is pressed, we change the state of the game
				if (event.key.code == sf::Keyboard::B && _game->actualGameState == VILLA_MANAGEMENT)
				{
					_game->actualGameState = BUILD_MODE;
				}
				else if (event.key.code == sf::Keyboard::B && _game->actualGameState == BUILD_MODE)
				{
					_game->mainCharacter->SetMainCharacterEndingPosition(_game->buildingsListPlanned->GetBuildingPositionInMap(), _game->map);
					_game->mainCharacter->SetMainCharacterStatus(IDLE, true);

					_game->actualGameState = NORMAL_MODE;
					_game->time->SetTypeOfAcceleration(GAME_NORMAL_SPEED);
				}
				else if (event.key.code == sf::Keyboard::B && _game->actualGameState == NORMAL_MODE)
				{
					_game->mainCharacter->SetIsMainCharacterSelected(_game->mainCharacter->GetIsMainCharacterSelected() ? false : true);
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
					_game->isDebuggerModeActive = !_game->isDebuggerModeActive;
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
					if (_game->time->GetTypeOfAcceleration() != GAME_PAUSE)
					{
						_game->time->SetTypeOfAcceleration(GAME_PAUSE);
					}
					else
					{
						_game->time->SetTypeOfAcceleration(GAME_NORMAL_SPEED);
					}
				}


				// Touche placé sur O, mais à changer	
				if (event.key.code == sf::Keyboard::O
					&& _game->actualGameState == NORMAL_MODE)
				{
					// Prix temporaire
					if (_game->money.GetMoneyQuantity() - 1000 >= 0)
					{
						_game->money.SubtractMoney(1000);

						sf::Vector2i value = { rand() % _game->numberColumns, rand() % _game->numberLines };

						// We spawn the workers on the road
						while (_game->map[ZERO_FLOOR + COLLISIONS_ID][value.y][value.x] != ROAD)
						{
							value.x = rand() % _game->numberColumns;
							value.y = rand() % _game->numberLines;
						}

						_game->workersList->AddNewWorkersToList(sf::Vector2f(value));
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
				// TEST
				if (event.key.code == sf::Keyboard::Add
					|| event.key.code == sf::Keyboard::A)
				{
					_game->workersList->ChangeWorkerNumberSelectedAdd();
				}
				// TEST
				if (event.key.code == sf::Keyboard::Subtract
					|| event.key.code == sf::Keyboard::E)
				{
					_game->workersList->ChangeWorkerNumberSelectedSubtract();
				}


				if (event.key.code == sf::Keyboard::M)
				{
					_game->save.SaveTheGame(_game);
				}
				if (event.key.code == sf::Keyboard::L)
				{
					_game->load.LoadTheGame(_game);
				}

				if (_game->actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num1)
				{
					_game->buildWindow.SetIDChosenBuilding(BUILDING_VINES);
					std::cout << "You've choose the vines\n";
				}
				else if (_game->actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num2)
				{
					_game->buildWindow.SetIDChosenBuilding(BUILDING_GRAPE_STOMPING_VATS);
					std::cout << "You've choose the stomping vats\n";
				}
				else if (_game->actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num3)
				{
					_game->buildWindow.SetIDChosenBuilding(BUILDING_WINE_PRESS);
					std::cout << "You've choose the wine press\n";
				}
				else if (_game->actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num4)
				{
					_game->buildWindow.SetIDChosenBuilding(BUILDING_WINE_STOREHOUSE);
					std::cout << "You've choose the wine storehouse\n";
				}
				else if (_game->actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num5)
				{
					_game->buildWindow.SetIDChosenBuilding(BUILDING_STOREHOUSE);
					std::cout << "You've choose the \n";
				}
				else if (_game->actualGameState == BUILD_MODE
					&& event.key.code == sf::Keyboard::Num6)
				{
					_game->buildWindow.SetIDChosenBuilding(BUILDING_STALL);
					std::cout << "You've choose the stall\n";
				}

			}

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (_game->actualGameState != BUILD_MODE)
				{
					if (_game->camera.z + (event.mouseWheelScroll.delta / 20) >= MAX_DEZOOMING && (_game->camera.z + (event.mouseWheelScroll.delta / 20) <= MAX_ZOOMING))
					{
						int profondeur = 1;

						_game->camera.z += event.mouseWheelScroll.delta / 20;

						_game->scale.x = 1 / (profondeur - _game->camera.z);
						_game->scale.y = 1 / (profondeur - _game->camera.z);
					}
				}
				else
				{
					sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);

					// We verify that mouse is in the building UI area
					if (mousePosition.x > SCREEN_WIDTH - _game->buildWindow.GetBuildingUI().getGlobalBounds().width
						&& mousePosition.x < SCREEN_WIDTH
						&& mousePosition.y > SCREEN_HEIGHT - _game->buildWindow.GetBuildingUI().getGlobalBounds().height
						&& mousePosition.y < SCREEN_HEIGHT)
					{
						// We check if the scrolling doesn't leave the area
						// The max is dynamically calculated in function of the number of building present in the game
						if (_game->buildWindow.GetScrollBuildingList() - (event.mouseWheelScroll.delta * 5) >= -130 * (_game->numberOfBuilding / 2) + 80
							&& (_game->buildWindow.GetScrollBuildingList() - (event.mouseWheelScroll.delta * 5) <= 80))
						{
							_game->buildWindow.SetScrollBuildingList(_game->buildWindow.GetScrollBuildingList() - (event.mouseWheelScroll.delta * 5));
						}
					}
					else
					{
						if (_game->camera.z + (event.mouseWheelScroll.delta / 20) >= MAX_DEZOOMING && (_game->camera.z + (event.mouseWheelScroll.delta / 20) <= MAX_ZOOMING))
						{
							int profondeur = 1;

							_game->camera.z += event.mouseWheelScroll.delta / 20;

							_game->scale.x = 1 / (profondeur - _game->camera.z);
							_game->scale.y = 1 / (profondeur - _game->camera.z);
						}
					}
				}

			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (_game->actualGameState == NORMAL_MODE)
					{
						_game->buildWindow.InputPickUpCaseClicked(*_game->window, false, sf::Vector2f(_game->camera.x, _game->camera.y), _game->scale);

						// Security to avoid an array exit
						if (_game->buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines), _game->buildWindow.GetBuildingCheckboxSelected()))
						{
							if (_game->mainCharacter->GetIsMainCharacterSelected() == true)
							{
								_game->mainCharacter->SetMainCharacterEndingPosition(_game->buildWindow.GetBuildingCheckboxSelected(), _game->map);

								_game->mainCharacter->SetMainCharacterStatus(IDLE, true);
							}
							else
							{
								_game->workersList->WorkerListSetEndPosition(_game->buildWindow.GetBuildingCheckboxSelected(), _game->map);
							}
						}

						_game->time->InputTimeManagement(*_game->window);
					}
					else if (_game->actualGameState == BUILD_MODE)
					{
						_game->buildWindow.InputBuildingModeOldScrollUI(_game->buildWindow.GetScrollBuildingList(), *_game->window);
					}
					else if (_game->actualGameState == TEST_PATHFINDING_MODE)
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
					else if (_game->actualGameState == VILLA_MANAGEMENT)
					{
						_game->villaManagement.InputVillaManagement(&_game->actualGameState, _game->time, *_game->window);
					}
				}
			}


		}


		CameraInputs(&_game->camera, _game->time->GetContinuousFrameTime(), sf::Vector2i(_game->numberColumns, _game->numberLines));


		// Case clicked in build mode
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (_game->actualGameState == BUILD_MODE)
			{
				_game->buildWindow.InputBuildWindow(_game);
			}
			else if (_game->actualGameState == SELLING_WINDOW)
			{
				_game->sellingWindow->InputSellingWindow(&_game->stall->isOfferAccepted, &_game->actualGameState, _game->stall, *_game->window);
			}
		}
	}
	else if (_game->actualGameState == PAUSE_WINDOW)
	{
		_game->pauseWindow.InputPauseWindow(_game, &_game->save, &_game->load);
	}
	else if (_game->actualGameState == TUTORIAL_MODE)
	{
		_game->tutorialWindow->InputTutorialWindow(&_game->actualGameState, *_game->window);
	}
}