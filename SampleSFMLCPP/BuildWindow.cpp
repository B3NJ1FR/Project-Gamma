#include "BuildWindow.h"
#include "GameDefinitions.h"

BuildWindow::BuildWindow()
{
	this->IDChosenBuilding = RESET;
	this->previousIDChosenBuilding = -1;
	this->scrollBuildingList = 80;
	this->isBuildingCaseOccupied = false;
	this->isNewBuildingHasBeenConstructed = false;

	this->InitSpritesBuildWindow();
}

BuildWindow::~BuildWindow()
{

}


void BuildWindow::InitTextsBuildWindow(sf::Font *_font)
{
	this->textBuildingCaseOccupied = nullptr;
	this->textBuildingCaseOccupied = new sf::Text[2];

	LoadTextString(&this->textBuildingCaseOccupied[0], "Place already occupied", _font, 40, sf::Color::White);
	LoadTextString(&this->textBuildingCaseOccupied[1], "Out of territory", _font, 40, sf::Color::White);


	this->textBuildingHelps = nullptr;
	this->textBuildingHelps = new sf::Text[BUILD_WINDOW_HELP_NB_OF_TEXTS];

	LoadTextString(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_SIZE_X], "", _font, 40, sf::Color::White, 3);
	LoadTextString(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_SIZE_Y], "", _font, 40, sf::Color::White, 0);
	LoadTextString(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_SIZE_LIAISON], " x ", _font, 40, sf::Color::White, 2);
	LoadTextString(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_RESSOURCE_ENTERING], "", _font, 40, sf::Color::White);
	LoadTextString(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_RESSOURCE_EXITING], "", _font, 40, sf::Color::White);
	LoadTextString(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_DESCRIPTION], "", _font, 40, sf::Color::White);
	LoadTextString(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_MONEY_COST], "", _font, 40, sf::Color::White);
}

void BuildWindow::InitSpritesBuildWindow()
{
	this->buildingUI = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/old_scroll_test.png", 0);
	this->buildingUIclosed = LoadSprite("Data/Assets/Sprites/Menu/old_scroll_closed.png", 0);
	this->buildingUIdestroyBuildings = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/destroy_buildings.png", 0);
	this->contour = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/contour_selection.png", 0);
	this->blackFilter = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/black_layer.png", 0);
}

sf::Sprite BuildWindow::GetBuildingUI()
{
	return this->buildingUI;
}

sf::Sprite BuildWindow::GetBuildingUIClosed()
{
	return this->buildingUIclosed;
}

sf::Vector2i BuildWindow::GetBuildingCheckboxSelected()
{
	return this->buildingCaseSelected;
}

bool BuildWindow::GetIsNewBuildingHasBeenConstructed()
{
	return this->isNewBuildingHasBeenConstructed;
}

float BuildWindow::GetScrollBuildingList()
{
	return this->scrollBuildingList;
}

int BuildWindow::GetIDChosenBuilding()
{
	return this->IDChosenBuilding;
}

bool BuildWindow::GetIsBuildingCaseOccupied()
{
	return this->isBuildingCaseOccupied;
}



void BuildWindow::SetIsNewBuildingHasBeenConstructed(const bool &_isNewBuildingHasBeenConstructed)
{
	this->isNewBuildingHasBeenConstructed = _isNewBuildingHasBeenConstructed;
}

void BuildWindow::SetScrollBuildingList(const float &_scrollBuildingList)
{
	this->scrollBuildingList = _scrollBuildingList;
}

void BuildWindow::SetIDChosenBuilding(const int &_buildingID)
{
	this->IDChosenBuilding = _buildingID;
}



bool BuildWindow::IsBuildingCheckboxIsInMap(const sf::Vector2i &_mapSize)
{
	if (this->buildingCaseSelected.x >= 0
		&& this->buildingCaseSelected.x < _mapSize.x
		&& this->buildingCaseSelected.y >= 0
		&& this->buildingCaseSelected.y < _mapSize.y)
	{
		return true;
	}
	else
	{
		return false;
	}
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


// Building Mode : Inputs management to select the building wanted in the "old scroll" UI
void BuildWindow::InputBuildingModeOldScrollUI(const float &_scrollDelta, const sf::RenderWindow &_window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);

	// Management of the horizontal click to get the number of the case selected, if the player has selected a case and not outside
	if ((mousePosition.x - (SCREEN_WIDTH - (int)this->buildingUI.getGlobalBounds().width + 143)) % 109 > 0
		&& (mousePosition.x - (SCREEN_WIDTH - (int)this->buildingUI.getGlobalBounds().width + 143)) % 109 < 64)
	{
		// Only 0 or 1 can be get because we just display two icons

		// Management of the vertical click to get the number of the case selected, if the player has selected a case and not outside
		if ((mousePosition.y - (SCREEN_HEIGHT - (int)this->buildingUI.getGlobalBounds().height + (int)_scrollDelta)) % 130 > 0
			&& (mousePosition.y - (SCREEN_HEIGHT - (int)this->buildingUI.getGlobalBounds().height + (int)_scrollDelta)) % 130 < 64)
		{
			bool numberCaseWidth = (mousePosition.x - (SCREEN_WIDTH - (int)this->buildingUI.getGlobalBounds().width + 143)) / 109;
			int numberCaseHeight = (mousePosition.y - (SCREEN_HEIGHT - (int)this->buildingUI.getGlobalBounds().height + (int)_scrollDelta)) / 130;

			this->IDChosenBuilding = numberCaseWidth + (numberCaseHeight * 2);
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
				if (_game->buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines)))
				{
					// Set the correct collision
					_game->map[_floorFocused + COLLISIONS_ID][this->buildingCaseSelected.y - y][this->buildingCaseSelected.x - x] = _statsToApply.x;

					// Set the correct building ID
					_game->map[_floorFocused + BUILDING_ID][this->buildingCaseSelected.y - y][this->buildingCaseSelected.x - x] = _statsToApply.y;
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
		_game->map[_floorFocused + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = _statsToApply.z;

		// EN FAIRE UNE FONCTION
		for (int y = 0; y < _game->buildings[_typeOfBuilding].GetSize().y; y++)
		{
			for (int x = 0; x < _game->buildings[_typeOfBuilding].GetSize().x; x++)
			{
				if (_game->buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines)))
				{
					// Set the correct collision
					_game->map[_floorFocused + COLLISIONS_ID][this->buildingCaseSelected.y - y][this->buildingCaseSelected.x - x] = _statsToApply.x;

					// Set the correct building ID
					_game->map[_floorFocused + BUILDING_ID][this->buildingCaseSelected.y - y][this->buildingCaseSelected.x - x] = _statsToApply.y;
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
	if (_game->buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines)))
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
			else if (_game->money.GetMoneyQuantity() >= _game->buildings[this->IDChosenBuilding].GetConstructionCost())
			{
				// Collisions verifications
				if (_game->map[ZERO_FLOOR + COLLISIONS_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] == ROAD)
				{
					isAreaEmpty = false;
				}
				else
				{
					for (int y = 0; y < _game->buildings[this->IDChosenBuilding].GetSize().y; y++)
					{
						for (int x = 0; x < _game->buildings[this->IDChosenBuilding].GetSize().x; x++)
						{
							if (_game->buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines)))
							{
								// Check case occupation concerning collisions
								if (_game->map[FIRST_FLOOR][this->buildingCaseSelected.y - y][this->buildingCaseSelected.x - x] != NO_COLLISION)
								{
									// The case is occupied
									isAreaEmpty = false;

									// We cut the for loop
									y = _game->buildings[this->IDChosenBuilding].GetSize().y;
									x = _game->buildings[this->IDChosenBuilding].GetSize().x;
								}
								// Check case occupation concerning roads
								else if (_game->map[ZERO_FLOOR + COLLISIONS_ID][this->buildingCaseSelected.y - y][this->buildingCaseSelected.x - x] == ROAD)
								{
									// The case is occupied
									isAreaEmpty = false;

									// We cut the for loop
									y = _game->buildings[this->IDChosenBuilding].GetSize().y;
									x = _game->buildings[this->IDChosenBuilding].GetSize().x;
								}
							}
							else
							{
								// The case is occupied
								isAreaEmpty = false;

								// We cut the for loop
								y = _game->buildings[this->IDChosenBuilding].GetSize().y;
								x = _game->buildings[this->IDChosenBuilding].GetSize().x;
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
				this->SetOrRemoveBuildingOnMap(_game, true, FIRST_FLOOR, this->IDChosenBuilding, sf::Vector3i(COLLISION, this->IDChosenBuilding, RESET));


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
				_game->money.SubtractMoney(_game->buildings[this->IDChosenBuilding].GetConstructionCost());


				// Set the correct sprite ID 

				if (this->IDChosenBuilding == 2)
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 17;
					_game->map[ZERO_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 23;
				}
				else if (this->IDChosenBuilding == 3)
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 16;
					_game->map[ZERO_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 24;
				}
				else if (this->IDChosenBuilding == 4)
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 19;
					_game->map[ZERO_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 22;
				}
				else if (this->IDChosenBuilding == 5)
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 18;
					_game->map[ZERO_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 20;
				}
				else
				{
					_game->map[FIRST_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 3 + this->IDChosenBuilding;
					_game->map[ZERO_FLOOR + SPRITE_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] = 7;
				}

				// Update the workers path if there is a modification
				this->isNewBuildingHasBeenConstructed = true;
			}
		}
		else if (this->IDChosenBuilding == _game->numberOfBuilding)
		{
			bool isOccupiedArea = false;

			// Collisions verifications
			if (_game->map[FIRST_FLOOR + COLLISIONS_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x] != NO_COLLISION)
			{
				isOccupiedArea = true;
			}

			if (isOccupiedArea == true)
			{
				int buildingIDFocused;

				buildingIDFocused = _game->map[FIRST_FLOOR + BUILDING_ID][this->buildingCaseSelected.y][this->buildingCaseSelected.x];

				switch (buildingIDFocused)
				{
				case BUILDING_VINES:

					if (_game->vines.DestroyedBuildingSelected((sf::Vector2f)this->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;
				case BUILDING_GRAPE_STOMPING_VATS:

					if (_game->stompingVats.DestroyedBuildingSelected((sf::Vector2f)this->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;
				case BUILDING_WINE_PRESS:

					if (_game->winePress.DestroyedBuildingSelected((sf::Vector2f)this->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;
				case BUILDING_WINE_STOREHOUSE:

					if (_game->wineStorehouse.DestroyedBuildingSelected((sf::Vector2f)this->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;

				case BUILDING_STOREHOUSE:

					if (_game->storehouse.DestroyedBuildingSelected((sf::Vector2f)this->buildingCaseSelected) == true)
					{
						SetOrRemoveBuildingOnMap(_game, false, FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET));
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;

				case BUILDING_STALL:

					if (_game->stall->DestroyedBuildingSelected((sf::Vector2f)this->buildingCaseSelected) == true)
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

void BuildWindow::UpdateBuildWindow(struct Game *_game)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);
	sf::Vector2f cameraIso = WorldToScreen(_game->camera.x, _game->camera.y);

	//std::cout << "Mouse Position : " << mousePosition.x << " & " << mousePosition.y << std::endl;

	this->buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (SCREEN_WIDTH / 2)) - cameraIso.x * _game->scale.x),
								((mousePosition.y - (SCREEN_HEIGHT / 2)) - cameraIso.y * _game->scale.y),
								_game->scale);

	//std::cout << "Case : " << _game->buildingCaseSelected.x << " & " << _game->buildingCaseSelected.y << std::endl << std::endl;

	// Security to avoid an array exit
	if (_game->buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines)))
	{
		if (this->IDChosenBuilding >= 0
			&& this->IDChosenBuilding < _game->numberOfBuilding)
		{
			bool isAreaEmpty = true;

			for (int y = 0; y < _game->buildings[this->IDChosenBuilding].GetSize().y; y++)
			{
				for (int x = 0; x < _game->buildings[this->IDChosenBuilding].GetSize().x; x++)
				{
					if (_game->buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines)))
					{
						// Check case occupation concerning collisions
						if (_game->map[FIRST_FLOOR][this->buildingCaseSelected.y - y][this->buildingCaseSelected.x - x] != NO_COLLISION)
						{
							// The case is occupied
							isAreaEmpty = false;

							// We cut the for loop
							y = _game->buildings[this->IDChosenBuilding].GetSize().y;
							x = _game->buildings[this->IDChosenBuilding].GetSize().x;
						}
					}
					else
					{
						// The case is occupied
						isAreaEmpty = false;

						// We cut the for loop
						y = _game->buildings[this->IDChosenBuilding].GetSize().y;
						x = _game->buildings[this->IDChosenBuilding].GetSize().x;
					}
				}
			}

			// If we didn't found an occupied place, we call that the place is empty
			if (isAreaEmpty)
			{
				this->isBuildingCaseOccupied = false;
			}
			else
			{
				this->isBuildingCaseOccupied = true;
			}
		}
	}
	else
	{
		this->isBuildingCaseOccupied = true;
	}
}

void BuildWindow::UpdateTextsBuildWindow(struct Game *_game)
{
	if (this->previousIDChosenBuilding != this->IDChosenBuilding)
	{
		UpdateDynamicsTexts(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_SIZE_X], _game->buildings[this->IDChosenBuilding].GetSize().x);
		UpdateDynamicsTexts(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_SIZE_Y], _game->buildings[this->IDChosenBuilding].GetSize().y);
		
		UpdateDynamicsTexts(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_MONEY_COST], _game->buildings[this->IDChosenBuilding].GetConstructionCost());

		// Texts
		// UpdateDynamicsTexts(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_RESSOURCE_ENTERING], );
		// UpdateDynamicsTexts(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_RESSOURCE_EXITING], );
		LoadTextString(&this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_DESCRIPTION], _game->buildings[this->IDChosenBuilding].GetDescription());
	}
}

void BuildWindow::DisplayBuildWindow(struct Game *_game)
{
	// Display of the building selection UI
	BlitSprite(this->buildingUI, SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width, SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height, 0, *_game->window);
	BlitSprite(this->buildingUI, SCREEN_WIDTH - (this->buildingUI.getGlobalBounds().width * 2), SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height, 0, *_game->window);

	if (this->IDChosenBuilding <= _game->numberOfBuilding)
	{
		BlitSprite(_game->buildings[this->IDChosenBuilding].GetIcon(), (SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width * 2) + 135, (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 125, 0, *_game->window);

		if (_game->buildingsNameTexts != nullptr)
		{
			// Display the name
			BlitString(_game->buildingsNameTexts[this->IDChosenBuilding], (SCREEN_WIDTH - (this->buildingUI.getGlobalBounds().width * 2) + (this->buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 75, *_game->window);

			// Display the size
			BlitString(this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_SIZE_X], (SCREEN_WIDTH - (this->buildingUI.getGlobalBounds().width * 2) + (this->buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 75, *_game->window);
			BlitString(this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_SIZE_Y], (SCREEN_WIDTH - (this->buildingUI.getGlobalBounds().width * 2) + (this->buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 75, *_game->window);
			BlitString(this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_SIZE_LIAISON], (SCREEN_WIDTH - (this->buildingUI.getGlobalBounds().width * 2) + (this->buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 75, *_game->window);

			// Display the entering ressource logo
			BlitString(this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_RESSOURCE_ENTERING], (SCREEN_WIDTH - (this->buildingUI.getGlobalBounds().width * 2) + (this->buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 75, *_game->window);

			// Display the exiting ressource logo
			BlitString(this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_RESSOURCE_EXITING], (SCREEN_WIDTH - (this->buildingUI.getGlobalBounds().width * 2) + (this->buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 75, *_game->window);

			// Display the description
			BlitString(this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_DESCRIPTION], (SCREEN_WIDTH - (this->buildingUI.getGlobalBounds().width * 2) + (this->buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 75, *_game->window);

			// Display the money cost of the building
			BlitString(this->textBuildingCaseOccupied[BUILD_WINDOW_HELP_MONEY_COST], (SCREEN_WIDTH - (this->buildingUI.getGlobalBounds().width * 2) + (this->buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 75, *_game->window);
		}
	}







	for (int i = 0; i < _game->numberOfBuilding; i++)
	{
		if ((SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + this->scrollBuildingList >= (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height + 40)
			&& (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + this->scrollBuildingList <= SCREEN_HEIGHT - _game->buildings[i].GetIcon().getGlobalBounds().height - 40)
		{
			BlitSprite(_game->buildings[i].GetIcon(), (SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + this->scrollBuildingList, 0, *_game->window);

			if (_game->money.GetMoneyQuantity() < _game->buildings[i].GetConstructionCost())
			{
				sf::Color color = { 255, 255, 255, 150 };
				this->blackFilter.setColor(color);

				BlitSprite(this->blackFilter, (SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + this->scrollBuildingList, 0, *_game->window);
			}

			//std::cout << "Stall : " << _game->stall->GetStatus() << std::endl;

			if (i == BUILDING_STALL
				&& _game->stall->GetConstructionStatus() != BUILDING_DESTROYED)
			{
				sf::Color color = { 255, 255, 255, 150 };
				this->blackFilter.setColor(color);

				BlitSprite(this->blackFilter, (SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + this->scrollBuildingList, 0, *_game->window);
			}

			if (_game->buildingsNameTexts != nullptr)
			{
				BlitString(_game->buildingsNameTexts[i], (SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109 + 32, (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (i / 2) + this->scrollBuildingList + 70, *_game->window);
			}
		}

	}

	// Display of the destroy button
	if ((SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (_game->numberOfBuilding / 2) + this->scrollBuildingList >= (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height + 40)
		&& (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (_game->numberOfBuilding / 2) + this->scrollBuildingList <= SCREEN_HEIGHT - this->buildingUIdestroyBuildings.getGlobalBounds().height - 40)
	{
		BlitSprite(this->buildingUIdestroyBuildings, (SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width) + 143 + (_game->numberOfBuilding % 2) * 109, (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (_game->numberOfBuilding / 2) + this->scrollBuildingList, 0, *_game->window);
	}

	if (this->IDChosenBuilding <= _game->numberOfBuilding)
	{
		if ((SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (this->IDChosenBuilding / 2) + this->scrollBuildingList >= (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height + 40)
			&& (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (this->IDChosenBuilding / 2) + this->scrollBuildingList <= SCREEN_HEIGHT - this->contour.getGlobalBounds().height - 40)
		{
			// Display of the contour when a building is selected to be built
			BlitSprite(this->contour, (SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width) + 143 + (this->IDChosenBuilding % 2) * 109 - 4, (SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height) + 130 * (this->IDChosenBuilding / 2) - 4 + this->scrollBuildingList, 0, *_game->window);
		}
	}

	// Display of the building chosen
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->window);

	// Display the building choosen on the mouse
	if (this->IDChosenBuilding == 2)
	{
		if (this->isBuildingCaseOccupied == false)
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
	else if (this->IDChosenBuilding == 3)
	{
		if (this->isBuildingCaseOccupied == false)
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
	else if (this->IDChosenBuilding == 4)
	{
		if (this->isBuildingCaseOccupied == false)
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
	else if (this->IDChosenBuilding == 5)
	{
		if (this->isBuildingCaseOccupied == false)
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
	else if (this->IDChosenBuilding != _game->numberOfBuilding)
	{
		if (this->isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->spriteArray[3 + this->IDChosenBuilding].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->spriteArray[3 + this->IDChosenBuilding].setColor(color);
		}

		BlitSprite(_game->spriteArray[3 + this->IDChosenBuilding], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->window);

		_game->spriteArray[3 + this->IDChosenBuilding].setColor(sf::Color::White);
	}


	// Display the "Out of territory" and "Place already occupied" messages
	if (this->isBuildingCaseOccupied == true
		&& !(mousePosition.x > SCREEN_WIDTH - this->buildingUI.getGlobalBounds().width
			&& mousePosition.x < SCREEN_WIDTH
			&& mousePosition.y > SCREEN_HEIGHT - this->buildingUI.getGlobalBounds().height
			&& mousePosition.y < SCREEN_HEIGHT))
	{
		if (!_game->buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->numberColumns, _game->numberLines)))
		{
			BlitString(this->textBuildingCaseOccupied[1], (SCREEN_WIDTH / 2), SCREEN_HEIGHT - 100, *_game->window);
		}
		else if (this->textBuildingCaseOccupied != nullptr)
		{
			BlitString(this->textBuildingCaseOccupied[0], (SCREEN_WIDTH / 2), SCREEN_HEIGHT - 100, *_game->window);
		}
	}
}