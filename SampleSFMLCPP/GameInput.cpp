#include "GameInput.h"
#include "GameDefinitions.h"

void CameraInputs(sf::Vector3f *_camera, const float &_frametime)
{
	// Camera Inputs

	// Blocage cam�ra :
	// - prendre en compte le zoom
	// - prendre en compte la taille de la map qui peut fluctuer
	// - prendre en compte que soit x soit y peut �tre atteint et pas les deux en mm temps

	/*sf::Vector2i coordinates = { (int)((_x + _y * 2) / (TILE_WIDTH * _scale.x)),
								 (int)((_y * 2 - _x) / (TILE_WIDTH * _scale.x)) };*/
	//std::cout << "Camx : " << (_camera->x + _camera->y * 2) << std::endl;
	//std::cout << "Camy : " << (_camera->y * 2 - _camera->x) << std::endl << std::endl;

	////if (_camera->y + CAMERA_SPEED * _frametime < -12
	//if ((_camera->x + CAMERA_SPEED * _frametime + (_camera->y + CAMERA_SPEED * _frametime) * 2) < -12
	//	&& ((_camera->y + CAMERA_SPEED * _frametime) * 2 - (_camera->x + CAMERA_SPEED * _frametime)) < -10)
	//{
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	//	{
	//		_camera->x += CAMERA_SPEED * _frametime;
	//		_camera->y += CAMERA_SPEED * _frametime;
	//	}
	//}

	//if ((_camera->x - CAMERA_SPEED * _frametime + (_camera->y - CAMERA_SPEED * _frametime) * 2) > -135
	//	&& ((_camera->y - CAMERA_SPEED * _frametime) * 2 - (_camera->x - CAMERA_SPEED * _frametime)) > -60)
	//{
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	//	{
	//		_camera->x -= CAMERA_SPEED * _frametime;
	//		_camera->y -= CAMERA_SPEED * _frametime;
	//	}
	//}

	//if ((_camera->x + CAMERA_SPEED * _frametime + (_camera->y - CAMERA_SPEED * _frametime) * 2) > -70
	//	&& ((_camera->y - CAMERA_SPEED * _frametime) * 2 - (_camera->x + CAMERA_SPEED * _frametime)) > -70)
	//{
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	//	{
	//		_camera->x += CAMERA_SPEED * _frametime;
	//		_camera->y -= CAMERA_SPEED * _frametime;
	//	}
	//}

	//if ((_camera->x - CAMERA_SPEED * _frametime + (_camera->y + CAMERA_SPEED * _frametime) * 2) < -100
	//	&& ((_camera->y + CAMERA_SPEED * _frametime) * 2 - (_camera->x - CAMERA_SPEED * _frametime)) > -30)
	//{
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	//	{
	//		_camera->x -= CAMERA_SPEED * _frametime;
	//		_camera->y += CAMERA_SPEED * _frametime;
	//	}
	//}



	// ****************************************************

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_camera->x += CAMERA_SPEED * _frametime;
		_camera->y += CAMERA_SPEED * _frametime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_camera->x -= CAMERA_SPEED * _frametime;
		_camera->y -= CAMERA_SPEED * _frametime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_camera->x += CAMERA_SPEED * _frametime;
		_camera->y -= CAMERA_SPEED * _frametime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_camera->x -= CAMERA_SPEED * _frametime;
		_camera->y += CAMERA_SPEED * _frametime;
	}

}


void InputPickUpCaseClicked(struct Game *_game, bool _isBuildingUINeeded)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);
	//std::cout << "Mouse Position : " << mousePosition.x << " & " << mousePosition.y << std::endl;

	if (_isBuildingUINeeded)
	{
		// If the mouse if out of the building UI, we can try to place a building
		if (!(mousePosition.x > 1920 - _game->buildingUI.getGlobalBounds().width
			&& mousePosition.x < 1920
			&& mousePosition.y > 1080 - _game->buildingUI.getGlobalBounds().height
			&& mousePosition.y < 1080))
		{
			sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

			_game->buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (1920 / 2)) - cameraIso.x * _game->scale.x),
				((mousePosition.y - (1080 / 2)) - cameraIso.y * _game->scale.y),
				_game->scale);
		}
		else
		{
			_game->buildingCaseSelected = { -1, -1 };
		}
	}
	else
	{
		// If the mouse if out of the building UI, we can try to place a building
		if (!(mousePosition.x > 1920
			&& mousePosition.x < 1920
			&& mousePosition.y > 1080
			&& mousePosition.y < 1080))
		{
			sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

			_game->buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (1920 / 2)) - cameraIso.x * _game->scale.x),
				((mousePosition.y - (1080 / 2)) - cameraIso.y * _game->scale.y),
				_game->scale);
		}
		else
		{
			_game->buildingCaseSelected = { -1, -1 };
		}
	}

	

	//std::cout << "Case : " << _game->buildingCaseSelected.x << " & " << _game->buildingCaseSelected.y << std::endl << std::endl;
}

// Building Mode : Inputs management to select the building wanted in the "old scroll" UI
void InputBuildingModeOldScrollUI(int *_IDChosenBuilding, sf::Vector2i _sizeOldScrollUI, const sf::RenderWindow &_window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);

	// Management of the horizontal click to get the number of the case selected, if the player has selected a case and not outside
	if ((mousePosition.x - (1920 - _sizeOldScrollUI.x + 143)) % 109 > 0
		&& (mousePosition.x - (1920 - _sizeOldScrollUI.x + 143)) % 109 < 64)
	{
		// Only 0 or 1 can be get because we just display two icons

		// Management of the vertical click to get the number of the case selected, if the player has selected a case and not outside
		if ((mousePosition.y - (1080 - _sizeOldScrollUI.y + 80)) % 130 > 0
			&& (mousePosition.y - (1080 - _sizeOldScrollUI.y + 80)) % 130 < 64)
		{
			bool numberCaseWidth = (mousePosition.x - (1920 - _sizeOldScrollUI.x + 143)) / 109;
			int numberCaseHeight = (mousePosition.y - (1080 - _sizeOldScrollUI.y + 80)) / 130;

			*_IDChosenBuilding = numberCaseWidth + (numberCaseHeight * 2);
		}
	}
}


void GameInput(struct Game *_game)
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
			if (event.key.code == sf::Keyboard::Escape)
			{
				exit(EXIT_SUCCESS);
			}


			// When the B keybutton is pressed, we change the state of the game
			if (event.key.code == sf::Keyboard::B && _game->actualGameState == NORMAL_MODE)
			{
				_game->actualGameState = BUILD_MODE;
			}
			else if (event.key.code == sf::Keyboard::B && _game->actualGameState == BUILD_MODE)
			{
				_game->actualGameState = NORMAL_MODE;
			}

			// When the T keybutton is pressed, we change the state of the game
			if (event.key.code == sf::Keyboard::T && _game->actualGameState == NORMAL_MODE)
			{
				_game->actualGameState = TEST_PATHFINDING_MODE;
			}
			else if (event.key.code == sf::Keyboard::T && _game->actualGameState == TEST_PATHFINDING_MODE)
			{
				_game->actualGameState = NORMAL_MODE;
			}


			
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

			if (event.key.code == sf::Keyboard::Y && _game->actualGameState == TEST_PATHFINDING_MODE)
			{
				std::cout << "Demande de lancemement de chemin\n";
				_game->workers->ActiveLauchingMovement();
			}
			if (event.key.code == sf::Keyboard::U && _game->actualGameState == TEST_PATHFINDING_MODE)
			{
				std::cout << "Depart chemin\n";
				_game->workers->isPressingStart = true;
			}
			if (event.key.code == sf::Keyboard::I && _game->actualGameState == TEST_PATHFINDING_MODE)
			{
				std::cout << "Fin chemin\n";
				_game->workers->isPressingEnd = true;
			}
			
			// TEST		
			if (event.key.code == sf::Keyboard::Y)
			{
				/*_game->vines.RemoveElementsOfLinkedList(true, 10);
				_game->vines.ReadLinkedList();*/
			}
			// TEST
			if (event.key.code == sf::Keyboard::U)
			{
				std::cout << _game->ressources[BUNCH_OF_GRAPE].GetName() << " : " << _game->ressources[BUNCH_OF_GRAPE].GetQuantityOwned() << std::endl;
			}
			// TEST
			if (event.key.code == sf::Keyboard::I)
			{
				std::cout << _game->buildings[3].GetRessourceIDProduced() << " " << _game->buildings[3].GetRessourceQuantityProduced() << std::endl;
				//std::cout << _game->buildings[3].GetRessourceIDNeeded() << " " << _game->buildings[3].GetRessourceIDNeeded(2) << std::endl;
			}


			if (_game->actualGameState == BUILD_MODE
				&& event.key.code == sf::Keyboard::Num1)
			{
				_game->IDChosenBuilding = 0;
				std::cout << "You've choose the vines\n";
			}
			else if (_game->actualGameState == BUILD_MODE
				&& event.key.code == sf::Keyboard::Num2)
			{
				_game->IDChosenBuilding = 1;
				std::cout << "You've choose the stomping vats\n";
			}
			else if (_game->actualGameState == BUILD_MODE
				&& event.key.code == sf::Keyboard::Num3)
			{
				_game->IDChosenBuilding = 2;
				std::cout << "You've choose the wine press\n";
			}
			else if (_game->actualGameState == BUILD_MODE
				&& event.key.code == sf::Keyboard::Num4)
			{
				_game->IDChosenBuilding = 3;
				std::cout << "You've choose the wine storehouse\n";
			}
			/*else if (_game->actualGameState == BUILD_MODE
				&& event.key.code == sf::Keyboard::Num5)
			{
				_game->IDChosenBuilding = 4;
				std::cout << "You've choose the fouloir\n";
			}
			else if (_game->actualGameState == BUILD_MODE
				&& event.key.code == sf::Keyboard::Num6)
			{
				_game->IDChosenBuilding = 5;
				std::cout << "You've choose the fouloir\n";
			}*/

		}

		if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (_game->camera.z + (event.mouseWheelScroll.delta / 20) >= MAX_DEZOOMING && (_game->camera.z + (event.mouseWheelScroll.delta / 20) <= MAX_ZOOMING))
			{
				int profondeur = 1;

				_game->camera.z += event.mouseWheelScroll.delta / 20;

				_game->scale.x = 1 / (profondeur - _game->camera.z);
				_game->scale.y = 1 / (profondeur - _game->camera.z);
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (_game->actualGameState == NORMAL_MODE)
				{
					InputPickUpCaseClicked(_game, false);

					// Security to avoid an array exit
					if (_game->buildingCaseSelected.x >= 0
						&& _game->buildingCaseSelected.x < _game->numberColumns
						&& _game->buildingCaseSelected.y >= 0
						&& _game->buildingCaseSelected.y < _game->numberLines)
					{
						_game->workers->SetEndingPosition(_game->buildingCaseSelected, _game->map);

						_game->workers->SetWorkerStatus(IDLE);
						_game->workers->ActiveLauchingMovement();
					}
				}
				else if (_game->actualGameState == BUILD_MODE)
				{
					InputBuildingModeOldScrollUI(&_game->IDChosenBuilding, sf::Vector2i((int)_game->buildingUI.getGlobalBounds().width, (int)_game->buildingUI.getGlobalBounds().height), *_game->window);
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

			}
		}
	}


	CameraInputs(&_game->camera, _game->time.GetFrameTime());


	// Case clicked in build mode
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (_game->actualGameState == BUILD_MODE)
		{
			// Verification
			InputPickUpCaseClicked(_game, true);

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

					// Money verification
					if (_game->money.GetMoneyQuantity() >= _game->buildings[_game->IDChosenBuilding].GetConstructionCost())
					{
						// Collisions verifications
						if (_game->map[ZERO_FLOOR + COLLISIONS_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] == ROAD)
						{
							isAreaEmpty = false;
						}
						else
						{
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
										// Check case occupation concerning roads
										else if (_game->map[ZERO_FLOOR + COLLISIONS_ID][_game->buildingCaseSelected.y - y][_game->buildingCaseSelected.x - x] == ROAD)
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
						}
					}
					else
					{
						isAreaEmpty = false;
					}

					// If we didn't found an occupied place, we call that is an empty place
					(isAreaEmpty) ? _game->isBuildingCaseOccupied = false : _game->isBuildingCaseOccupied = true;

					// Add the building is the place is empty
					if (_game->isBuildingCaseOccupied == false)
					{
						for (int y = 0; y < _game->buildings[_game->IDChosenBuilding].GetSize().y; y++)
						{
							for (int x = 0; x < _game->buildings[_game->IDChosenBuilding].GetSize().x; x++)
							{
								if (_game->buildingCaseSelected.x - x >= 0
									&& _game->buildingCaseSelected.x - x < _game->numberColumns
									&& _game->buildingCaseSelected.y - y >= 0
									&& _game->buildingCaseSelected.y - y < _game->numberLines)
								{
									// Set the correct collision
									_game->map[FIRST_FLOOR + COLLISIONS_ID][_game->buildingCaseSelected.y - y][_game->buildingCaseSelected.x - x] = COLLISION;

									// Set the correct building ID
									_game->map[FIRST_FLOOR + BUILDING_ID][_game->buildingCaseSelected.y - y][_game->buildingCaseSelected.x - x] = 0;
								}
								else
								{
									std::cout << "\n\n\n\tError during building placement\n\n\n";
								}
							}
						}

						// If the building selected is the vines, we add informations to the concerned linkedlist
						if (_game->IDChosenBuilding == 0)
						{
							_game->vines.AddNewVineToList((sf::Vector2f)_game->buildingCaseSelected);
						}
						else if (_game->IDChosenBuilding == 1)
						{
							_game->stompingVats.AddNewBuildingToList((sf::Vector2f)_game->buildingCaseSelected);
						}
						else if (_game->IDChosenBuilding == 2)
						{
							_game->winePress.AddNewBuildingToList((sf::Vector2f)_game->buildingCaseSelected);
						}
						else if (_game->IDChosenBuilding == 3)
						{
							_game->wineStorehouse.AddNewBuildingToList((sf::Vector2f)_game->buildingCaseSelected);
						}

						// We remove the money needed to construct the building
						_game->money.SubtractMoney(_game->buildings[_game->IDChosenBuilding].GetConstructionCost());


						// Set the correct sprite ID 
						_game->map[FIRST_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 3 + _game->IDChosenBuilding;
						_game->map[ZERO_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 7;
					}
				}
			}			
		}
		
	}
	
 
}