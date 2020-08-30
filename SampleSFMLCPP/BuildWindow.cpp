#include "BuildWindow.h"
#include "GameDefinitions.h"

BuildWindow::BuildWindow()
{

}

BuildWindow::~BuildWindow()
{

}

void BuildWindow::InputPickUpCaseClicked(sf::RenderWindow &_window, bool _isBuildingUINeeded, const sf::Vector2f &_camera, const sf::Vector2f &_cameraScale)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
	//std::cout << "Mouse Position : " << mousePosition.x << " & " << mousePosition.y << std::endl;

	if (_isBuildingUINeeded)
	{
		// If the mouse if out of the building UI, we can try to place a building
		if (!(mousePosition.x > SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width
			&& mousePosition.x < SCREEN_WIDTH
			&& mousePosition.y > SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height
			&& mousePosition.y < SCREEN_HEIGHT))
		{
			sf::Vector2f cameraIso = WorldToScreen(_camera.x, _camera.y);

			this->buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (SCREEN_WIDTH / 2)) - cameraIso.x * _cameraScale.x),
				((mousePosition.y - (SCREEN_HEIGHT / 2)) - cameraIso.y * _cameraScale.y),
				_cameraScale);
		}
		else
		{
			this->buildingCaseSelected = { -1, -1 };
		}
	}
	else
	{
		// If the mouse if out of the building UI, we can try to place a building
		if (!(mousePosition.x > SCREEN_WIDTH
			&& mousePosition.x < SCREEN_WIDTH
			&& mousePosition.y > SCREEN_HEIGHT
			&& mousePosition.y < SCREEN_HEIGHT))
		{
			sf::Vector2f cameraIso = WorldToScreen(_camera.x, _camera.y);

			this->buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (SCREEN_WIDTH / 2)) - cameraIso.x * _cameraScale.x),
				((mousePosition.y - (SCREEN_HEIGHT / 2)) - cameraIso.y * _cameraScale.y),
				_cameraScale);
		}
		else
		{
			this->buildingCaseSelected = { -1, -1 };
		}
	}
}

void BuildWindow::SetOrRemoveBuildingOnMap(struct Game *_game, bool _isConstructing, enum Floors _floorFocused, int _typeOfBuilding, sf::Vector3i _statsToApply)
{
	if (_isConstructing == true)
	{
		for (int y = 0; y < _game->buildings[_typeOfBuilding].GetSize().y; y++)
		{
			for (int x = 0; x < _game->buildings[_typeOfBuilding].GetSize().x; x++)
			{
				if (this->buildingCaseSelected.x - x >= 0
					&& this->buildingCaseSelected.x - x < _game->numberColumns
					&& this->buildingCaseSelected.y - y >= 0
					&& this->buildingCaseSelected.y - y < _game->numberLines)
				{
					// Set the correct collision
					_game->map[_floorFocused + COLLISIONS_ID][_game->buildingCaseSelected.y - y][_game->buildingCaseSelected.x - x] = _statsToApply.x;

					// Set the correct building ID
					_game->map[_floorFocused + BUILDING_ID][_game->buildingCaseSelected.y - y][_game->buildingCaseSelected.x - x] = _statsToApply.y;
				}
				else
				{
					std::cout << "\n\n\n\tError during building placement\n\n\n";
				}
			}
		}
	}
	else
	{
		_game->map[_floorFocused + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = _statsToApply.z;

		// EN FAIRE UNE FONCTION
		for (int y = 0; y < _game->buildings[_typeOfBuilding].GetSize().y; y++)
		{
			for (int x = 0; x < _game->buildings[_typeOfBuilding].GetSize().x; x++)
			{
				if (this->buildingCaseSelected.x - x >= 0
					&& this->buildingCaseSelected.x - x < _game->numberColumns
					&& this->buildingCaseSelected.y - y >= 0
					&& this->buildingCaseSelected.y - y < _game->numberLines)
				{
					// Set the correct collision
					_game->map[_floorFocused + COLLISIONS_ID][_game->buildingCaseSelected.y - y][_game->buildingCaseSelected.x - x] = _statsToApply.x;

					// Set the correct building ID
					_game->map[_floorFocused + BUILDING_ID][_game->buildingCaseSelected.y - y][_game->buildingCaseSelected.x - x] = _statsToApply.y;
				}
				else
				{
					std::cout << "\n\n\n\tError during building destruction\n\n\n";
				}
			}
		}
	}
}

void BuildWindow::InputBuildWindow(struct Game *_game)
{
	// Verification
	this->InputPickUpCaseClicked(*_game->window, true, sf::Vector2f(_game->camera.x, _game->camera.y), _game->scale);

	// Security to avoid an array exit
	if (this->buildingCaseSelected.x >= 0
		&& this->buildingCaseSelected.x < _game->numberColumns
		&& this->buildingCaseSelected.y >= 0
		&& this->buildingCaseSelected.y < _game->numberLines)
	{
		if (this->IDChosenBuilding >= 0
			&& this->IDChosenBuilding < _game->numberOfBuilding)
		{
			bool isAreaEmpty = true;

			// If we've already built the stall, we cant built it again
			if (this->IDChosenBuilding == BUILDING_STALL && _game->stall->GetConstructionStatus() != BUILDING_DESTROYED)
			{
				isAreaEmpty = false;
			}
			// Money verification
			else if (_game->money.GetMoneyQuantity() >= _game->buildings[_game->IDChosenBuilding].GetConstructionCost())
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
							if (this->buildingCaseSelected.x - x >= 0
								&& this->buildingCaseSelected.x - x < _game->numberColumns
								&& this->buildingCaseSelected.y - y >= 0
								&& this->buildingCaseSelected.y - y < _game->numberLines)
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
			this->isBuildingCaseOccupied = (isAreaEmpty) ? false : true;

			// Add the building if the place is empty
			if (this->isBuildingCaseOccupied == false)
			{
				this->SetOrRemoveBuildingOnMap(_game, true, FIRST_FLOOR, _game->IDChosenBuilding, sf::Vector3i(COLLISION, this->IDChosenBuilding, RESET));


				// If the building selected is the vines, we add informations to the concerned linkedlist
				if (this->IDChosenBuilding == BUILDING_VINES)
				{
					_game->vines.AddNewVineToList((sf::Vector2f)this->buildingCaseSelected);
				}
				else if (this->IDChosenBuilding == BUILDING_GRAPE_STOMPING_VATS)
				{
					_game->stompingVats.AddNewBuildingToList((sf::Vector2f)this->buildingCaseSelected);
				}
				else if (this->IDChosenBuilding == BUILDING_WINE_PRESS)
				{
					_game->winePress.AddNewBuildingToList((sf::Vector2f)this->buildingCaseSelected);
				}
				else if (this->IDChosenBuilding == BUILDING_WINE_STOREHOUSE)
				{
					_game->wineStorehouse.AddNewBuildingToList((sf::Vector2f)this->buildingCaseSelected);
				}
				else if (this->IDChosenBuilding == BUILDING_STOREHOUSE)
				{
					_game->storehouse.AddNewBuildingToList((sf::Vector2f)this->buildingCaseSelected);
				}
				else if (this->IDChosenBuilding == BUILDING_STALL)
				{
					_game->stall->AddNewBuilding((sf::Vector2f)this->buildingCaseSelected);
				}

				// We remove the money needed to construct the building
				_game->money.SubtractMoney(_game->buildings[_game->IDChosenBuilding].GetConstructionCost());


				// Set the correct sprite ID 

				if (this->IDChosenBuilding == 2)
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 17;
					_game->map[ZERO_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 23;
				}
				else if (this->IDChosenBuilding == 3)
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 16;
					_game->map[ZERO_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 24;
				}
				else if (this->IDChosenBuilding == 4)
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 19;
					_game->map[ZERO_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 22;
				}
				else if (this->IDChosenBuilding == 5)
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 18;
					_game->map[ZERO_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 20;
				}
				else
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 3 + _game->IDChosenBuilding;
					_game->map[ZERO_FLOOR + SPRITE_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] = 7;
				}

				// Update the workers path if there is a modification
				this->isNewBuildingHasBeenConstructed = true;
			}
		}
		else if (this->IDChosenBuilding == _game->numberOfBuilding)
		{
			bool isOccupiedArea = false;

			// Collisions verifications
			if (_game->map[FIRST_FLOOR + COLLISIONS_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x] != NO_COLLISION)
			{
				isOccupiedArea = true;
			}

			if (isOccupiedArea == true)
			{
				int buildingIDFocused;

				buildingIDFocused = _game->map[FIRST_FLOOR + BUILDING_ID][_game->buildingCaseSelected.y][_game->buildingCaseSelected.x];

				switch (buildingIDFocused)
				{
				case BUILDING_VINES:

					if (_game->vines.DestroyedBuildingSelected((sf::Vector2f)_game->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;
				case BUILDING_GRAPE_STOMPING_VATS:

					if (_game->stompingVats.DestroyedBuildingSelected((sf::Vector2f)_game->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;
				case BUILDING_WINE_PRESS:

					if (_game->winePress.DestroyedBuildingSelected((sf::Vector2f)_game->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;
				case BUILDING_WINE_STOREHOUSE:

					if (_game->wineStorehouse.DestroyedBuildingSelected((sf::Vector2f)_game->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;

				case BUILDING_STOREHOUSE:

					if (_game->storehouse.DestroyedBuildingSelected((sf::Vector2f)_game->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;

				case BUILDING_STALL:

					if (_game->stall->DestroyedBuildingSelected((sf::Vector2f)_game->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;

				default:
					break;
				}
			}
			else
			{

			}

		}
	}
}

void BuildWindow::UpdateBuildWindow()
{

}

void BuildWindow::DisplayBuildWindow()
{

}