#include "BuildWindow.h"


BuildWindow::BuildWindow()
{
	m_IDChosenBuilding = RESET;
	m_previousIDChosenBuilding = -1;
	m_scrollBuildingList = 80;

	m_isBuildingCaseOccupied = false;

	m_textBuildingCaseOccupied = nullptr;
	m_textBuildingHelps = nullptr;

	// Initialisation of the sprites
	InitSpritesBuildWindow();
}

BuildWindow::~BuildWindow()
{

}

void BuildWindow::InitTextsBuildWindow(sf::Font *_font)
{
	m_textBuildingCaseOccupied = new sf::Text[2];

	LoadTextString(&m_textBuildingCaseOccupied[0], "Place already occupied", _font, 40, sf::Color::White, 2);
	LoadTextString(&m_textBuildingCaseOccupied[1], "Out of territory", _font, 40, sf::Color::White, 2);


	m_textBuildingHelps = new sf::Text[BUILD_WINDOW_HELP_NB_OF_TEXTS];

	LoadTextString(&m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_X], "", _font, 20, sf::Color::Black, 3);
	LoadTextString(&m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_Y], "", _font, 20, sf::Color::Black, 0);
	LoadTextString(&m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_LIAISON], " x ", _font, 16, sf::Color::Black, 2);
	LoadTextString(&m_textBuildingHelps[BUILD_WINDOW_HELP_DESCRIPTION], "", _font, 13, sf::Color::Black);
	LoadTextString(&m_textBuildingHelps[BUILD_WINDOW_HELP_MONEY_COST], "", _font, 20, sf::Color::Black, 3);
}

void BuildWindow::InitSpritesBuildWindow()
{
	m_buildingUI = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/old_scroll_test.png", 0);
	m_buildingUIclosed = LoadSprite("Data/Assets/Sprites/Menu/old_scroll_closed.png", 0);
	m_buildingUIdestroyBuildings = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/destroy_buildings.png", 0);
	m_contour = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/contour_selection.png", 0);
	m_blackFilter = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/black_layer.png", 0);
	m_enteringArrow = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/left_arrow.png", 1);
	m_exitingArrow = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/right_arrow.png", 1);
	m_separationLine = LoadSprite("Data/Assets/Sprites/Menu/BuildingModeWindow/separation.png", 1);
}

sf::Sprite BuildWindow::GetBuildingUI() const
{
	return m_buildingUI;
}

sf::Sprite BuildWindow::GetBuildingUIClosed() const
{
	return m_buildingUIclosed;
}

sf::Vector2i BuildWindow::GetBuildingCheckboxSelected() const
{
	return m_buildingCaseSelected;
}


float BuildWindow::GetScrollBuildingList() const
{
	return m_scrollBuildingList;
}

int BuildWindow::GetIDChosenBuilding() const
{
	return m_IDChosenBuilding;
}

bool BuildWindow::GetIsBuildingCaseOccupied() const
{
	return m_isBuildingCaseOccupied;
}




void BuildWindow::SetScrollBuildingList(const float &_scrollBuildingList)
{
	m_scrollBuildingList = _scrollBuildingList;
}

void BuildWindow::SetIDChosenBuilding(const int &_buildingID)
{
	m_IDChosenBuilding = _buildingID;
}



bool BuildWindow::IsBuildingCheckboxIsInMap(const sf::Vector2i &_mapSize, const sf::Vector2i &_mapPosition)
{
	if (_mapPosition.x >= 0
		&& _mapPosition.x < _mapSize.x
		&& _mapPosition.y >= 0
		&& _mapPosition.y < _mapSize.y)
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
		if (!(mousePosition.x > SCREEN_WIDTH - m_buildingUI.getGlobalBounds().width * 2
			&& mousePosition.x < SCREEN_WIDTH
			&& mousePosition.y > SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height
			&& mousePosition.y < SCREEN_HEIGHT))
		{
			sf::Vector2f cameraIso = WorldToScreen(_camera.x, _camera.y);

			m_buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (SCREEN_WIDTH / 2)) - cameraIso.x * _cameraScale.x),
				((mousePosition.y - (SCREEN_HEIGHT / 2)) - cameraIso.y * _cameraScale.y),
				_cameraScale);
		}
		else
		{
			m_buildingCaseSelected = { -1, -1 };
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

			m_buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (SCREEN_WIDTH / 2)) - cameraIso.x * _cameraScale.x),
				((mousePosition.y - (SCREEN_HEIGHT / 2)) - cameraIso.y * _cameraScale.y),
				_cameraScale);
		}
		else
		{
			m_buildingCaseSelected = { -1, -1 };
		}
	}
}


// Building Mode : Inputs management to select the building wanted in the "old scroll" UI
void BuildWindow::InputBuildingModeOldScrollUI(const float &_scrollDelta, const sf::RenderWindow &_window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);

	// Management of the horizontal click to get the number of the case selected, if the player has selected a case and not outside
	if ((mousePosition.x - (SCREEN_WIDTH - (int)m_buildingUI.getGlobalBounds().width + 143)) % 109 > 0
		&& (mousePosition.x - (SCREEN_WIDTH - (int)m_buildingUI.getGlobalBounds().width + 143)) % 109 < 64)
	{
		// Only 0 or 1 can be get because we just display two icons

		// Management of the vertical click to get the number of the case selected, if the player has selected a case and not outside
		if ((mousePosition.y - (SCREEN_HEIGHT - (int)m_buildingUI.getGlobalBounds().height + (int)_scrollDelta)) % 130 > 0
			&& (mousePosition.y - (SCREEN_HEIGHT - (int)m_buildingUI.getGlobalBounds().height + (int)_scrollDelta)) % 130 < 64)
		{
			bool numberCaseWidth = (mousePosition.x - (SCREEN_WIDTH - (int)m_buildingUI.getGlobalBounds().width + 143)) / 109;
			int numberCaseHeight = (mousePosition.y - (SCREEN_HEIGHT - (int)m_buildingUI.getGlobalBounds().height + (int)_scrollDelta)) / 130;

			m_IDChosenBuilding = numberCaseWidth + (numberCaseHeight * 2);
		}
	}
}

void BuildWindow::SetBuildingOnMap(Game *_game, const enum Floors &_floorFocused, const int &_typeOfBuilding, const enum TypesOfCollisions &_collisionID, const sf::Vector2i &_mapPosition)
{
	for (int y = 0; y < _game->m_builds.buildings[_typeOfBuilding].GetSize().y; y++)
	{
		for (int x = 0; x < _game->m_builds.buildings[_typeOfBuilding].GetSize().x; x++)
		{
			if (_game->m_buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->m_map.GetNumberOfColumns(), _game->m_map.GetNumberOfLines()), _mapPosition))
			{
				// Set the collisions and buildings id for the building
				_game->m_map.GetMap()[_floorFocused + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = _collisionID;
				_game->m_map.GetMap()[_floorFocused + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = _typeOfBuilding;
				
				// Set the building sprite
				switch (_typeOfBuilding)
				{
				case BUILDING_VINES:
					_game->m_map.GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 32;
					break;
				case BUILDING_GRAPE_STOMPING_VATS:
					_game->m_map.GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 26;
					break;
				case BUILDING_WINE_PRESS:
					_game->m_map.GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 30;
					break;
				case BUILDING_WINE_STOREHOUSE:
					_game->m_map.GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 31;
					break;
				case BUILDING_STOREHOUSE:
					_game->m_map.GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 29;
					break;
				case BUILDING_STALL:
					_game->m_map.GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 27;
					break;
				/*case BUILDING_VILLA:
					break;
				case BUILDING_DORMITORY:
					break;*/
				default:
					break;
				}
				

				// Set the collisions and buildings id for the ground
				_game->m_map.GetMap()[ZERO_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = _collisionID;
				_game->m_map.GetMap()[ZERO_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = _typeOfBuilding;

				// Set the ground sprite adapted to the building selected
				switch (_typeOfBuilding)
				{
				case BUILDING_VINES:
					_game->m_map.GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 7;
					break;
				case BUILDING_GRAPE_STOMPING_VATS:
					_game->m_map.GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 25;
					break;
				case BUILDING_WINE_PRESS:
					_game->m_map.GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 23;
					break;
				case BUILDING_WINE_STOREHOUSE:
					_game->m_map.GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 24;
					break;
				case BUILDING_STOREHOUSE:
					_game->m_map.GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 22;
					break;
				case BUILDING_STALL:
					_game->m_map.GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = 20;
					break;
					/*case BUILDING_VILLA:
						break;
					case BUILDING_DORMITORY:
						break;*/
				default:
					break;
				}

			}
			else
			{
				std::cout << "\n\n\n\tError during building placement\n\n\n";
			}
		}
	}
}


void BuildWindow::SetGhostBuildingOnMap(Game *_game, const int &_typeOfBuilding, const sf::Vector2i &_mapPosition)
{
	for (int y = 0; y < _game->m_builds.buildings[_typeOfBuilding].GetSize().y; y++)
	{
		for (int x = 0; x < _game->m_builds.buildings[_typeOfBuilding].GetSize().x; x++)
		{
			if (x == 0 && y == 0)
			{
				// Set the collisions and buildings id for the building
				_game->m_map.GetMap()[FIRST_FLOOR + COLLISIONS_ID][_mapPosition.y][_mapPosition.x] = BUILDING_GHOST;
				_game->m_map.GetMap()[FIRST_FLOOR + BUILDING_ID][_mapPosition.y][_mapPosition.x] = _typeOfBuilding;
				_game->m_map.GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] = _game->m_map.GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x];


				// Set the collisions and buildings id for the ground
				_game->m_map.GetMap()[ZERO_FLOOR + COLLISIONS_ID][_mapPosition.y][_mapPosition.x] = BUILDING_GHOST;
				_game->m_map.GetMap()[ZERO_FLOOR + BUILDING_ID][_mapPosition.y][_mapPosition.x] = _typeOfBuilding;
			}
			else if (_game->m_buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->m_map.GetNumberOfColumns(), _game->m_map.GetNumberOfLines()), _mapPosition))
			{
				// Set the collisions and buildings id for the building
				_game->m_map.GetMap()[FIRST_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = BUILDING_GHOST;
				
				// Set the collisions and buildings id for the ground
				_game->m_map.GetMap()[ZERO_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = BUILDING_GHOST;
			}
			else
			{
				std::cout << "\n\n\n\tError during building placement\n\n\n";
			}
		}
	}
}

void BuildWindow::RemoveBuildingOnMap(Game *_game, enum Floors _floorFocused, const int &_typeOfBuilding, const sf::Vector3i &_statsToApply, const sf::Vector2i &_mapPosition)
{
	_game->m_map.GetMap()[_floorFocused + SPRITE_ID][_mapPosition.y][_mapPosition.x] = _statsToApply.z;

	// EN FAIRE UNE FONCTION
	for (int y = 0; y < _game->m_builds.buildings[_typeOfBuilding].GetSize().y; y++)
	{
		for (int x = 0; x < _game->m_builds.buildings[_typeOfBuilding].GetSize().x; x++)
		{
			if (_game->m_buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->m_map.GetNumberOfColumns(), _game->m_map.GetNumberOfLines()), _mapPosition))
			{
				// Set the correct collision
				_game->m_map.GetMap()[_floorFocused + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = _statsToApply.x;

				// Set the correct building ID
				_game->m_map.GetMap()[_floorFocused + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = _statsToApply.y;
			}
			else
			{
				std::cout << "\n\n\n\tError during building destruction\n\n\n";
			}
		}
	}
}



void BuildWindow::InputBuildWindow(Game *_game)
{
	// Verification
	InputPickUpCaseClicked(*_game->m_window, true, sf::Vector2f(_game->m_camera.x, _game->m_camera.y), _game->m_scale);

	// Security to avoid an array exit
	if (_game->m_buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->m_map.GetNumberOfColumns(), _game->m_map.GetNumberOfLines()), m_buildingCaseSelected))
	{
		if (m_IDChosenBuilding >= 0
			&& m_IDChosenBuilding < _game->m_builds.numberOfBuilding)
		{
			bool isAreaEmpty = true;

			// If we've already built the stall, we cant built it again
			if (m_IDChosenBuilding == BUILDING_STALL && _game->m_builds.stall->GetConstructionStatus() != BUILDING_DESTROYED)
			{
				isAreaEmpty = false;
			}
			// Money verification
			else if (_game->money.GetMoneyQuantity() >= _game->m_builds.buildings[m_IDChosenBuilding].GetConstructionCost())
			{
				// Collisions verifications
				if (_game->m_map.GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x] == PATH
					|| _game->m_map.GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x] == STONE_PATH
					|| _game->m_map.GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x] == ROAD)
				{
					isAreaEmpty = false;
				}
				else
				{
					for (int y = 0; y < _game->m_builds.buildings[m_IDChosenBuilding].GetSize().y; y++)
					{
						for (int x = 0; x < _game->m_builds.buildings[m_IDChosenBuilding].GetSize().x; x++)
						{
							if (_game->m_buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->m_map.GetNumberOfColumns(), _game->m_map.GetNumberOfLines()), m_buildingCaseSelected))
							{
								// Check case occupation concerning collisions
								if (_game->m_map.GetMap()[FIRST_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y - y][m_buildingCaseSelected.x - x] != NO_COLLISION)
								{
									// The case is occupied
									isAreaEmpty = false;

									// We cut the for loop
									y = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().y;
									x = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().x;
								}
								// Check case occupation concerning roads
								else if (_game->m_map.GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y - y][m_buildingCaseSelected.x - x] != NO_COLLISION)
								{
									// The case is occupied
									isAreaEmpty = false;

									// We cut the for loop
									y = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().y;
									x = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().x;
								}
							}
							else
							{
								// The case is occupied
								isAreaEmpty = false;

								// We cut the for loop
								y = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().y;
								x = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().x;
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
			m_isBuildingCaseOccupied = (isAreaEmpty) ? false : true;

			// Add the building if the place is empty
			if (m_isBuildingCaseOccupied == false)
			{
				////----------------------------------------------------------------------------------------------

				//// Deplacer
				//// Clear the ground by putting sand floor
				//for (int y = 0; y < _game->buildings[IDChosenBuilding].GetSize().y; y++)
				//{
				//	for (int x = 0; x < _game->buildings[IDChosenBuilding].GetSize().x; x++)
				//	{
				//		_game->map[ZERO_FLOOR + SPRITE_ID][buildingCaseSelected.y - y][buildingCaseSelected.x - x] = 1;
				//	}
				//}

				//// Modifier couche pour ID
				//SetOrRemoveBuildingOnMap(_game, true, FIRST_FLOOR, IDChosenBuilding, sf::Vector3i(COLLISION, IDChosenBuilding, RESET));

				//// Deplacer
				//// If the building selected is the vines, we add informations to the concerned linkedlist
				//if (IDChosenBuilding == BUILDING_VINES)
				//{
				//	_game->vines.AddNewVineToList((sf::Vector2f)buildingCaseSelected);
				//}
				//else if (IDChosenBuilding == BUILDING_GRAPE_STOMPING_VATS)
				//{
				//	_game->stompingVats.AddNewBuildingToList((sf::Vector2f)buildingCaseSelected);
				//}
				//else if (IDChosenBuilding == BUILDING_WINE_PRESS)
				//{
				//	_game->winePress.AddNewBuildingToList((sf::Vector2f)buildingCaseSelected);
				//}
				//else if (IDChosenBuilding == BUILDING_WINE_STOREHOUSE)
				//{
				//	_game->wineStorehouse.AddNewBuildingToList((sf::Vector2f)buildingCaseSelected);
				//}
				//else if (IDChosenBuilding == BUILDING_STOREHOUSE)
				//{
				//	_game->storehouse.AddNewBuildingToList((sf::Vector2f)buildingCaseSelected);
				//}
				//else if (IDChosenBuilding == BUILDING_STALL)
				//{
				//	_game->stall->AddNewBuilding((sf::Vector2f)buildingCaseSelected);
				//}

				//// Conserver
				//// We remove the money needed to construct the building
				//_game->money.SubtractMoney(_game->buildings[IDChosenBuilding].GetConstructionCost());
				//
				//// Deplacer
				//// Update the workers path if there is a modification
				//isNewBuildingHasBeenConstructed = true;

				////----------------------------------------------------------------------------------------------

				//----------------------------------------------------------------------------------------------

				if (m_IDChosenBuilding == BUILDING_STALL)
				{
					_game->m_builds.stall->SetConstructionStatus(PLANNED);
				}

				// We add the current building to the planned list
				_game->buildingsListPlanned->AddBuildingPlannedToList(m_buildingCaseSelected, (enum TypeOfBuilding)m_IDChosenBuilding, _game->m_builds.buildings[m_IDChosenBuilding].GetSize());
				
				// We add the temporary collision "BUILDING_GHOST" where the building has been placed
				_game->m_buildWindow.SetGhostBuildingOnMap(_game, (enum TypeOfBuilding)m_IDChosenBuilding, m_buildingCaseSelected);
				

				// We remove the money needed to construct the building
				_game->money.SubtractMoney(_game->m_builds.buildings[m_IDChosenBuilding].GetConstructionCost());
				
				// We change the main character status
				_game->m_mainCharacter->SetIsCurrentlyBuilding(true);

				//----------------------------------------------------------------------------------------------
			}
		}
		// Condition to determine if the destructor mode has been selected
		else if (m_IDChosenBuilding == _game->m_builds.numberOfBuilding)
		{
			bool isOccupiedArea = false;

			// Collisions verifications
			if (_game->m_map.GetMap()[FIRST_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x] != NO_COLLISION)
			{
				isOccupiedArea = true;
			}

			if (isOccupiedArea == true)
			{
				int buildingIDFocused;

				buildingIDFocused = _game->m_map.GetMap()[FIRST_FLOOR + BUILDING_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x];

				switch (buildingIDFocused)
				{
				case BUILDING_VINES:

					if (_game->m_builds.vines.DestroyedBuildingSelected((sf::Vector2f)m_buildingCaseSelected) == true)
					{
						RemoveBuildingOnMap(_game, Floors::FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET), m_buildingCaseSelected);
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;
				case BUILDING_GRAPE_STOMPING_VATS:

					if (_game->m_builds.stompingVats.DestroyedBuildingSelected((sf::Vector2f)m_buildingCaseSelected) == true)
					{
						RemoveBuildingOnMap(_game, Floors::FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET), m_buildingCaseSelected);
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;
				case BUILDING_WINE_PRESS:

					if (_game->m_builds.winePress.DestroyedBuildingSelected((sf::Vector2f)m_buildingCaseSelected) == true)
					{
						RemoveBuildingOnMap(_game, Floors::FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET), m_buildingCaseSelected);
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;
				case BUILDING_WINE_STOREHOUSE:

					if (_game->m_builds.wineStorehouse.DestroyedBuildingSelected((sf::Vector2f)m_buildingCaseSelected) == true)
					{
						RemoveBuildingOnMap(_game, Floors::FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET), m_buildingCaseSelected);
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;

				case BUILDING_STOREHOUSE:

					if (_game->m_builds.storehouse.DestroyedBuildingSelected((sf::Vector2f)m_buildingCaseSelected) == true)
					{
						RemoveBuildingOnMap(_game, Floors::FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET), m_buildingCaseSelected);
					}
					else
					{
						std::cout << "Can't destroyed this building\n\n\n";
					}

					break;

				case BUILDING_STALL:

					if (_game->m_builds.stall->DestroyedBuildingSelected((sf::Vector2f)m_buildingCaseSelected) == true)
					{
						RemoveBuildingOnMap(_game, Floors::FIRST_FLOOR, buildingIDFocused, sf::Vector3i(NO_COLLISION, RESET, RESET), m_buildingCaseSelected);
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

void BuildWindow::UpdateBuildWindow(Game *_game)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->m_window);
	sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);

	//std::cout << "Mouse Position : " << mousePosition.x << " & " << mousePosition.y << std::endl;

	m_buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (SCREEN_WIDTH / 2)) - cameraIso.x * _game->m_scale.x),
								((mousePosition.y - (SCREEN_HEIGHT / 2)) - cameraIso.y * _game->m_scale.y),
								_game->m_scale);

	//std::cout << "Case : " << _game->buildingCaseSelected.x << " & " << _game->buildingCaseSelected.y << std::endl << std::endl;

	// Security to avoid an array exit
	if (_game->m_buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->m_map.GetNumberOfColumns(), _game->m_map.GetNumberOfLines()), m_buildingCaseSelected))
	{
		if (m_IDChosenBuilding >= 0
			&& m_IDChosenBuilding < _game->m_builds.numberOfBuilding)
		{
			bool isAreaEmpty = true;

			for (int y = 0; y < _game->m_builds.buildings[m_IDChosenBuilding].GetSize().y; y++)
			{
				for (int x = 0; x < _game->m_builds.buildings[m_IDChosenBuilding].GetSize().x; x++)
				{
					if (_game->m_buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->m_map.GetNumberOfColumns(), _game->m_map.GetNumberOfLines()), m_buildingCaseSelected))
					{
						// Check case occupation concerning collisions
						if (_game->m_map.GetMap()[FIRST_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y - y][m_buildingCaseSelected.x - x] != NO_COLLISION)
						{
							// The case is occupied
							isAreaEmpty = false;

							// We cut the for loop
							y = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().y;
							x = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().x;
						}
						else if(_game->m_map.GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y - y][m_buildingCaseSelected.x - x] != NO_COLLISION)
						{
							// The case is occupied
							isAreaEmpty = false;

							// We cut the for loop
							y = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().y;
							x = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().x;
						}
					}
					else
					{
						// The case is occupied
						isAreaEmpty = false;

						// We cut the for loop
						y = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().y;
						x = _game->m_builds.buildings[m_IDChosenBuilding].GetSize().x;
					}
				}
			}

			// If we didn't found an occupied place, we call that the place is empty
			if (isAreaEmpty)
			{
				m_isBuildingCaseOccupied = false;
			}
			else
			{
				m_isBuildingCaseOccupied = true;
			}
		}
	}
	else
	{
		m_isBuildingCaseOccupied = true;
	}


	// Text creation if they don't exist
	if (_game->m_builds.buildingsNameTexts == nullptr)
	{
		_game->m_builds.buildingsNameTexts = new sf::Text[_game->m_builds.numberOfBuilding];

		for (int i = 0; i < _game->m_builds.numberOfBuilding; i++)
		{
			LoadTextString(&_game->m_builds.buildingsNameTexts[i], _game->m_builds.buildings[i].GetName(), &_game->m_charlemagneFont, 18, sf::Color::Black, 2);
		}
	}

	// Update the texts
	UpdateTextsBuildWindow(_game);
}

void BuildWindow::UpdateTextsBuildWindow(Game *_game)
{
	if (m_previousIDChosenBuilding != m_IDChosenBuilding
		&& m_IDChosenBuilding >= 0 
		&& m_IDChosenBuilding < _game->m_builds.numberOfBuilding)
	{
		// Numerics texts
		UpdateDynamicsTexts(&m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_X], _game->m_builds.buildings[m_IDChosenBuilding].GetSize().x);
		UpdateDynamicsTexts(&m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_Y], _game->m_builds.buildings[m_IDChosenBuilding].GetSize().y);

		UpdateDynamicsTexts(&m_textBuildingHelps[BUILD_WINDOW_HELP_MONEY_COST], _game->m_builds.buildings[m_IDChosenBuilding].GetConstructionCost());
		
		// Texts				
		LoadTextString(&m_textBuildingHelps[BUILD_WINDOW_HELP_DESCRIPTION], ConvertStringIntoParagraph(_game->m_builds.buildings[m_IDChosenBuilding].GetDescription(), 25));

		m_previousIDChosenBuilding = m_IDChosenBuilding;
	}
}

void BuildWindow::DisplayBuildWindow(struct Game *_game)
{
	// Display of the building selection UI
	BlitSprite(m_buildingUI, SCREEN_WIDTH - m_buildingUI.getGlobalBounds().width, SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height, 0, *_game->m_window);
	
	// Display of the help scroll that indicates every building info
	if (m_IDChosenBuilding < _game->m_builds.numberOfBuilding)
	{
		BlitSprite(m_buildingUI, SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2), SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height, 0, *_game->m_window);

		BlitSprite(_game->m_builds.buildings[m_IDChosenBuilding].GetIcon(), (SCREEN_WIDTH - m_buildingUI.getGlobalBounds().width * 2) + 135, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 135, 0, *_game->m_window);

		// Display of the info paper about the building selected
		if (_game->m_builds.buildingsNameTexts != nullptr)
		{
			// Display the name
			BlitString(_game->m_builds.buildingsNameTexts[m_IDChosenBuilding], (SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 70, *_game->m_window);
			
			BlitSprite(m_separationLine, (SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 120, 0, *_game->m_window);


			// Display the size
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_X], SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + 240, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130, *_game->m_window);
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_LIAISON], SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + 270, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 134, *_game->m_window);
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_Y], SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + 290, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130, *_game->m_window);
			
			// Display the entering ressource logo
			BlitSprite(m_enteringArrow, SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + 230, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 181, 0, *_game->m_window);
			
			if (_game->m_builds.buildings[m_IDChosenBuilding].GetRessourceNumberNeeded() > 0)
			{
				for (int i = 0; i < _game->m_builds.buildings[m_IDChosenBuilding].GetRessourceNumberNeeded(); i++)
				{
					BlitSprite(_game->m_ressources[_game->m_builds.buildings[m_IDChosenBuilding].GetRessourceIDNeeded(i + 1)].GetSprite(), SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + 255 + i * 32, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 165, 0, *_game->m_window);
				}
			}
			
			// Display the exiting ressource logo
			BlitSprite(m_exitingArrow, SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + 230, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 216, 0, *_game->m_window);
			
			if (_game->m_builds.buildings[m_IDChosenBuilding].GetRessourceNumberProduced() > 0)
			{
				for (int i = 0; i < _game->m_builds.buildings[m_IDChosenBuilding].GetRessourceNumberProduced(); i++)
				{
					BlitSprite(_game->m_ressources[_game->m_builds.buildings[m_IDChosenBuilding].GetRessourceIDProduced(i + 1)].GetSprite(), SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + 255 + i * 32, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 200, 0, *_game->m_window);
				}
			}

			BlitSprite(m_separationLine, (SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 250, 0, *_game->m_window);


			// Display the description
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_DESCRIPTION], SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + 120, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 260, *_game->m_window);

			BlitSprite(m_separationLine, (SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)), (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 385, 0, *_game->m_window);


			// Display the money cost of the building
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_MONEY_COST], (SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)) - 30, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 400, *_game->m_window);
			
			_game->money.SetSpriteScale(sf::Vector2f(0.45f, 0.45f));
			BlitSprite(_game->money.GetSprite(), (SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)) + 30, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 412, 0, *_game->m_window);
			_game->money.SetSpriteScale(sf::Vector2f(1, 1));
		}
	}






	// Display of the buildings list
	for (int i = 0; i < _game->m_builds.numberOfBuilding; i++)
	{
		if ((SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList >= (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height + 40)
			&& (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList <= SCREEN_HEIGHT - _game->m_builds.buildings[i].GetIcon().getGlobalBounds().height - 40)
		{
			BlitSprite(_game->m_builds.buildings[i].GetIcon(), (SCREEN_WIDTH - m_buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList, 0, *_game->m_window);

			if (_game->money.GetMoneyQuantity() < _game->m_builds.buildings[i].GetConstructionCost())
			{
				sf::Color color = { 255, 255, 255, 150 };
				m_blackFilter.setColor(color);

				BlitSprite(m_blackFilter, (SCREEN_WIDTH - m_buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList, 0, *_game->m_window);
			}

			//std::cout << "Stall : " << _game->stall->GetStatus() << std::endl;

			if (i == BUILDING_STALL
				&& _game->m_builds.stall->GetConstructionStatus() != BUILDING_DESTROYED)
			{
				sf::Color color = { 255, 255, 255, 150 };
				m_blackFilter.setColor(color);

				BlitSprite(m_blackFilter, (SCREEN_WIDTH - m_buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList, 0, *_game->m_window);
			}

			if (_game->m_builds.buildingsNameTexts != nullptr)
			{
				BlitString(_game->m_builds.buildingsNameTexts[i], (SCREEN_WIDTH - m_buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109 + 32, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList + 70, *_game->m_window);
			}
		}

	}

	// Display of the destroy button
	if ((SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (_game->m_builds.numberOfBuilding / 2) + m_scrollBuildingList >= (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height + 40)
		&& (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (_game->m_builds.numberOfBuilding / 2) + m_scrollBuildingList <= SCREEN_HEIGHT - m_buildingUIdestroyBuildings.getGlobalBounds().height - 40)
	{
		BlitSprite(m_buildingUIdestroyBuildings, (SCREEN_WIDTH - m_buildingUI.getGlobalBounds().width) + 143 + (_game->m_builds.numberOfBuilding % 2) * 109, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (_game->m_builds.numberOfBuilding / 2) + m_scrollBuildingList, 0, *_game->m_window);
	}

	if (m_IDChosenBuilding <= _game->m_builds.numberOfBuilding)
	{
		if ((SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (m_IDChosenBuilding / 2) + m_scrollBuildingList >= (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height + 40)
			&& (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (m_IDChosenBuilding / 2) + m_scrollBuildingList <= SCREEN_HEIGHT - m_contour.getGlobalBounds().height - 40)
		{
			// Display of the contour when a building is selected to be built
			BlitSprite(m_contour, (SCREEN_WIDTH - m_buildingUI.getGlobalBounds().width) + 143 + (m_IDChosenBuilding % 2) * 109 - 4, (SCREEN_HEIGHT - m_buildingUI.getGlobalBounds().height) + 130 * (m_IDChosenBuilding / 2) - 4 + m_scrollBuildingList, 0, *_game->m_window);
		}
	}

	// Display of the building chosen
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->m_window);

	// Display the building choosen on the mouse
	if (m_IDChosenBuilding == 2)
	{
		if (m_isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->m_spriteArray[17].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->m_spriteArray[17].setColor(color);
		}

		BlitSprite(_game->m_spriteArray[17], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->m_window);

		_game->m_spriteArray[17].setColor(sf::Color::White);
	}
	else if (m_IDChosenBuilding == 3)
	{
		if (m_isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->m_spriteArray[16].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->m_spriteArray[16].setColor(color);
		}

		BlitSprite(_game->m_spriteArray[16], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->m_window);

		_game->m_spriteArray[16].setColor(sf::Color::White);
	}
	else if (m_IDChosenBuilding == 4)
	{
		if (m_isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->m_spriteArray[19].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->m_spriteArray[19].setColor(color);
		}

		BlitSprite(_game->m_spriteArray[19], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->m_window);

		_game->m_spriteArray[19].setColor(sf::Color::White);
	}
	else if (m_IDChosenBuilding == 5)
	{
		if (m_isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->m_spriteArray[18].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->m_spriteArray[18].setColor(color);
		}

		BlitSprite(_game->m_spriteArray[18], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->m_window);

		_game->m_spriteArray[18].setColor(sf::Color::White);
	}
	else if (m_IDChosenBuilding != _game->m_builds.numberOfBuilding)
	{
		if (m_isBuildingCaseOccupied == false)
		{
			sf::Color color(255, 255, 255, 150);
			_game->m_spriteArray[3 + m_IDChosenBuilding].setColor(color);
		}
		else
		{
			sf::Color color(255, 0, 0, 150);
			_game->m_spriteArray[3 + m_IDChosenBuilding].setColor(color);
		}

		BlitSprite(_game->m_spriteArray[3 + m_IDChosenBuilding], (float)mousePosition.x, (float)mousePosition.y + TILE_HEIGHT / 2, 0, *_game->m_window);

		_game->m_spriteArray[3 + m_IDChosenBuilding].setColor(sf::Color::White);
	}


	// Display the "Out of territory" and "Place already occupied" messages
	if (m_isBuildingCaseOccupied == true
		&& !(mousePosition.x > SCREEN_WIDTH - (m_buildingUI.getGlobalBounds().width * 2)
			&& mousePosition.x < SCREEN_WIDTH
			&& mousePosition.y > SCREEN_HEIGHT - (m_buildingUI.getGlobalBounds().height * 2)
			&& mousePosition.y < SCREEN_HEIGHT))
	{
		if (!_game->m_buildWindow.IsBuildingCheckboxIsInMap(sf::Vector2i(_game->m_map.GetNumberOfColumns(), _game->m_map.GetNumberOfLines()), m_buildingCaseSelected))
		{
			BlitString(m_textBuildingCaseOccupied[1], 500, SCREEN_HEIGHT - 100, *_game->m_window);
		}
		else if (m_textBuildingCaseOccupied != nullptr)
		{
			BlitString(m_textBuildingCaseOccupied[0], 500, SCREEN_HEIGHT - 100, *_game->m_window);
		}
	}
}