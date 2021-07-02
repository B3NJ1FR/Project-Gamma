#include "BuildWindow.h"
#include "GameDefinitions.h"
#include "BuildingDestruction.h"
#include "RessourcesManager.h"



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

	InitialisationListOfPreviousID();
}

BuildWindow::~BuildWindow()
{
	delete (m_buildingUI.getTexture());
	delete (m_buildingUIclosed.getTexture());
	delete (m_buildingUIdestroyBuildings.getTexture());
	delete (m_blackFilter.getTexture());
	delete (m_contour.getTexture());
	delete (m_enteringArrow.getTexture());
	delete (m_exitingArrow.getTexture());
	delete (m_separationLine.getTexture());

	if (m_textBuildingCaseOccupied != nullptr)
	{
		//delete m_textBuildingCaseOccupied;
		m_textBuildingCaseOccupied = nullptr;
	}

	if (m_textBuildingHelps != nullptr)
	{
		//delete m_textBuildingHelps;
		m_textBuildingHelps = nullptr;
	}

	if (m_listOfPreviousID != nullptr) FreeLinkedList(m_listOfPreviousID);
}


void BuildWindow::InitialisationListOfPreviousID()
{
	std::cout << "List before : " << m_listOfPreviousID << std::endl;

	m_listOfPreviousID = LinkedListInitialisation();

	std::cout << "List " << m_listOfPreviousID << " Size : " << m_listOfPreviousID->size << " Real First : " << m_listOfPreviousID->first << " & Last : " << m_listOfPreviousID->last << std::endl;

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




void BuildWindow::InputPickUpCaseClicked(sf::RenderWindow &_window, const sf::Vector2i &_screenResolution, bool _isBuildingUINeeded, const sf::Vector2f &_camera, const sf::Vector2f &_cameraScale)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
	//std::cout << "Mouse Position : " << mousePosition.x << " & " << mousePosition.y << std::endl;

	if (_isBuildingUINeeded)
	{
		// If the mouse if out of the building UI, we can try to place a building
		if (!(mousePosition.x > _screenResolution.x - m_buildingUI.getGlobalBounds().width * 2
			&& mousePosition.x < _screenResolution.x
			&& mousePosition.y > _screenResolution.y - m_buildingUI.getGlobalBounds().height
			&& mousePosition.y < _screenResolution.y))
		{
			sf::Vector2f cameraIso = WorldToScreen(_camera.x, _camera.y);

			m_buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (_screenResolution.x / 2)) - cameraIso.x * _cameraScale.x),
				((mousePosition.y - (_screenResolution.y / 2)) - cameraIso.y * _cameraScale.y),
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
		if (!(mousePosition.x > _screenResolution.x
			&& mousePosition.x < _screenResolution.x
			&& mousePosition.y > _screenResolution.y
			&& mousePosition.y < _screenResolution.y))
		{
			sf::Vector2f cameraIso = WorldToScreen(_camera.x, _camera.y);

			m_buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (_screenResolution.x / 2)) - cameraIso.x * _cameraScale.x),
				((mousePosition.y - (_screenResolution.y / 2)) - cameraIso.y * _cameraScale.y),
				_cameraScale);
		}
		else
		{
			m_buildingCaseSelected = { -1, -1 };
		}
	}
}


// Building Mode : Inputs management to select the building wanted in the "old scroll" UI
void BuildWindow::InputBuildingModeOldScrollUI(const float &_scrollDelta, const sf::RenderWindow &_window, const sf::Vector2i &_screenResolution)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);

	// Management of the horizontal click to get the number of the case selected, if the player has selected a case and not outside
	if ((mousePosition.x - (_screenResolution.x - (int)m_buildingUI.getGlobalBounds().width + 143)) % 109 > 0
		&& (mousePosition.x - (_screenResolution.x - (int)m_buildingUI.getGlobalBounds().width + 143)) % 109 < 64)
	{
		// Only 0 or 1 can be get because we just display two icons

		// Management of the vertical click to get the number of the case selected, if the player has selected a case and not outside
		if ((mousePosition.y - (_screenResolution.y - (int)m_buildingUI.getGlobalBounds().height + (int)_scrollDelta)) % 130 > 0
			&& (mousePosition.y - (_screenResolution.y - (int)m_buildingUI.getGlobalBounds().height + (int)_scrollDelta)) % 130 < 64)
		{
			bool numberCaseWidth = (mousePosition.x - (_screenResolution.x - (int)m_buildingUI.getGlobalBounds().width + 143)) / 109;
			int numberCaseHeight = (mousePosition.y - (_screenResolution.y - (int)m_buildingUI.getGlobalBounds().height + (int)_scrollDelta)) / 130;

			m_IDChosenBuilding = numberCaseWidth + (numberCaseHeight * 2);
		}
	}
}

void BuildWindow::SetBuildingOnMap(Map *_map, BuildingManagement* _builds, int _typeOfBuilding, enum TypesOfCollisions _collisionID, const sf::Vector2i& _mapPosition)
{
	sf::Vector2i buildingSize = _builds->m_buildings[_typeOfBuilding].GetSize();

	for (int y = 0; y < buildingSize.y; y++)
	{
		for (int x = 0; x < buildingSize.x; x++)
		{
			if (_map->IsCoordinatesIsInMap(sf::Vector2i(_mapPosition.x - x, _mapPosition.y - y)))
			{
				//std::cout << "\n\nEntree dans build on map\n\n";
				_map->GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)_builds->m_buildings[_typeOfBuilding].GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_STUDS][buildingSize.y - 1 - y][buildingSize.x - 1 - x];
				_map->GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)_builds->m_buildings[_typeOfBuilding].GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_GROUND][buildingSize.y - 1 - y][buildingSize.x - 1 - x];

				// Set the collisions and buildings id for the building
				_map->GetMap()[FIRST_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)_collisionID;
				_map->GetMap()[FIRST_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)_typeOfBuilding;

				// Set the collisions and buildings id for the ground
				_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)_collisionID;
				_map->GetMap()[ZERO_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)_typeOfBuilding;
			}
			else
			{
				std::cout << "\n\n\n\tError during building placement\n\n\n";
			}
		}
	}
}


void BuildWindow::SetGhostBuildingOnMap(struct Game *_game, const int &_typeOfBuilding, const sf::Vector2i &_mapPosition)
{
	Buildings& currentBuild = _game->m_builds.m_buildings[_typeOfBuilding];
	for (int y = 0; y < currentBuild.GetSize().y; y++)
	{
		for (int x = 0; x < currentBuild.GetSize().x; x++)
		{
			if (_game->m_map->IsCoordinatesIsInMap(sf::Vector2i(_mapPosition.x - x, _mapPosition.y - y)))
			{
				// Set the collisions and buildings id for the building
				SaveFromMapPreviousSpriteID(sf::Vector2i(_mapPosition.x - x, _mapPosition.y - y), FIRST_FLOOR + SPRITE_ID, _game->m_map->GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x]);
				unsigned short studsSpriteID = (unsigned short)currentBuild.GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_STUDS][currentBuild.GetSize().y - 1 - y][currentBuild.GetSize().x - 1 - x];
				_game->m_map->GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x] = studsSpriteID;

				// Set the collisions and buildings id for the ground
				SaveFromMapPreviousSpriteID(sf::Vector2i(_mapPosition.x - x, _mapPosition.y - y), ZERO_FLOOR + SPRITE_ID, _game->m_map->GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x]);
				unsigned short groundSpriteID = (unsigned short)currentBuild.GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_GROUND][currentBuild.GetSize().y - 1 - y][currentBuild.GetSize().x - 1 - x];
				_game->m_map->GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x] = groundSpriteID;


				// Set the collisions and buildings id for the building
				SaveFromMapPreviousBuildID(sf::Vector2i(_mapPosition.x - x, _mapPosition.y - y), FIRST_FLOOR + BUILDING_ID, _game->m_map->GetMap()[FIRST_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x]);
				_game->m_map->GetMap()[FIRST_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)BUILDING_GHOST;
				_game->m_map->GetMap()[FIRST_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)_typeOfBuilding;

				// Set the collisions and buildings id for the ground
				SaveFromMapPreviousBuildID(sf::Vector2i(_mapPosition.x - x, _mapPosition.y - y), ZERO_FLOOR + BUILDING_ID, _game->m_map->GetMap()[ZERO_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x]);
				_game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)BUILDING_GHOST;
				_game->m_map->GetMap()[ZERO_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = (unsigned short)_typeOfBuilding;
			}
			else
			{
				std::cout << "\n\n\n\tError during building placement\n\n\n";
			}
		}
	}
}

sf::Vector2i BuildWindow::FindRealOriginBuildingPosition(Map* _map, BuildingManagement* _builds, int _typeOfBuilding, const sf::Vector2i& _mapPosition)
{
	if (_typeOfBuilding != 65535)
	{
		sf::Vector2i buildingMapPosition = _mapPosition;

		switch ((TypeOfBuilding)_typeOfBuilding)
		{
		case BUILDING_VINES:
			// OK
			break;
		case BUILDING_GRAPE_STOMPING_VATS:
			buildingMapPosition = _builds->m_stompingVats.FindNearestBuilding((sf::Vector2f)buildingMapPosition);
			break;
		case BUILDING_WINE_PRESS:
			buildingMapPosition = _builds->m_winePress.FindNearestBuilding((sf::Vector2f)buildingMapPosition);
			break;
		case BUILDING_WINE_STOREHOUSE:
			buildingMapPosition = _builds->m_wineStorehouse.FindNearestBuilding((sf::Vector2f)buildingMapPosition);
			break;
		case BUILDING_STOREHOUSE:
			buildingMapPosition = _builds->m_storehouse.FindNearestBuilding((sf::Vector2f)buildingMapPosition);
			break;
		case BUILDING_STALL:
			if (_builds->m_stall != nullptr) buildingMapPosition = _builds->m_stall->GetMapPosition();
			break;
		case BUILDING_VILLA:
			// OK
			break;
		case BUILDING_PATH:
			// OK
			break;
		case BUILDING_PAVED_PATH:
			// OK
			break;
		case BUILDING_ROAD:
			if (_map->GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y][_mapPosition.x] == 1) buildingMapPosition.y += 1;
			break;
		case BUILDING_DORMITORY:
			break;
		default:
			break;
		}

		return buildingMapPosition;
	}
}


void BuildWindow::SetGhostDestructionBuildingOnMap(Map *_map, BuildingManagement *_builds, int _typeOfBuilding, const sf::Vector2i& _mapPosition)
{
	if (_typeOfBuilding != 65535)
	{
		std::cout << _typeOfBuilding << ' ' << _builds->m_buildings[_typeOfBuilding].GetSize().x << ' ' << _builds->m_buildings[_typeOfBuilding].GetSize().y << std::endl;

		for (int y = 0; y < _builds->m_buildings[_typeOfBuilding].GetSize().y; y++)
		{
			for (int x = 0; x < _builds->m_buildings[_typeOfBuilding].GetSize().x; x++)
			{
				if (_map->IsCoordinatesIsInMap(sf::Vector2i(_mapPosition.y - y, _mapPosition.x - x)))
				{
					// Set the collisions and buildings id for the building
					SaveFromMapPreviousBuildNSpriteID(sf::Vector2i(_mapPosition.x - x, _mapPosition.y - y), FIRST_FLOOR, _map->GetMap()[FIRST_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x], _map->GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x]);
					_map->GetMap()[FIRST_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = BUILDING_WILL_BE_DESTROYED;
					_map->GetMap()[FIRST_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = -1;
					_map->GetMap()[FIRST_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x] = 0;

					SaveFromMapPreviousBuildNSpriteID(sf::Vector2i(_mapPosition.x - x, _mapPosition.y - y), ZERO_FLOOR, _map->GetMap()[ZERO_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x], _map->GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x]);
					_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][_mapPosition.y - y][_mapPosition.x - x] = BUILDING_WILL_BE_DESTROYED;
					_map->GetMap()[ZERO_FLOOR + BUILDING_ID][_mapPosition.y - y][_mapPosition.x - x] = -1;
					_map->GetMap()[ZERO_FLOOR + SPRITE_ID][_mapPosition.y - y][_mapPosition.x - x] = 1;

					std::cout << "DEVENU GHOST\n";
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
	InputPickUpCaseClicked(*_game->m_window, *_game->m_screenReso, true, sf::Vector2f(_game->m_camera.x, _game->m_camera.y), _game->m_scale);

	// Security to avoid an array exit
	if (_game->m_map->IsCoordinatesIsInMap(m_buildingCaseSelected))
	{
		if (m_IDChosenBuilding >= 0
			&& m_IDChosenBuilding < _game->m_builds.GetNumberOfBuildings())
		{
			bool isAreaEmpty = true;

			// If we've already built the stall, we cant built it again
			if (m_IDChosenBuilding == BUILDING_STALL && _game->m_builds.m_stall->GetConstructionStatus() != BUILDING_DESTROYED)
			{
				isAreaEmpty = false;
			}
			// Money verification
			else if (_game->m_money.GetMoneyQuantity() >= _game->m_builds.m_buildings[m_IDChosenBuilding].GetConstructionCost())
			{
				// Collisions verifications
				if (_game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x] == PATH
					|| _game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x] == STONE_PATH
					|| _game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x] == ROAD)
				{
					isAreaEmpty = false;
				}
				else
				{
					for (int y = 0; y < _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().y; y++)
					{
						for (int x = 0; x < _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().x; x++)
						{
							if (_game->m_map->IsCoordinatesIsInMap(m_buildingCaseSelected))
							{
								// Check case occupation concerning collisions
								if (_game->m_map->GetMap()[FIRST_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y - y][m_buildingCaseSelected.x - x] != NO_COLLISION)
								{
									// The case is occupied
									isAreaEmpty = false;

									// We cut the for loop
									y = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().y;
									x = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().x;
								}
								// Check case occupation concerning roads
								else if (_game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y - y][m_buildingCaseSelected.x - x] != NO_COLLISION)
								{
									// The case is occupied
									isAreaEmpty = false;

									// We cut the for loop
									y = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().y;
									x = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().x;
								}
							}
							else
							{
								// The case is occupied
								isAreaEmpty = false;

								// We cut the for loop
								y = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().y;
								x = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().x;
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
				//----------------------------------------------------------------------------------------------

				if (m_IDChosenBuilding == BUILDING_STALL)
				{
					_game->m_builds.m_stall->SetConstructionStatus(BuildingStatus::PLANNED);
				}

				// We add the current building to the planned list
				_game->m_buildingsListPlanned->AddBuildingPlannedToList(m_buildingCaseSelected, (enum TypeOfBuilding)m_IDChosenBuilding, _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize());
				
				// We add the temporary collision "BUILDING_GHOST" where the building has been placed
				SetGhostBuildingOnMap(_game, (enum TypeOfBuilding)m_IDChosenBuilding, m_buildingCaseSelected);
				

				// We remove the money needed to construct the building
				_game->m_money.SubtractMoney(_game->m_builds.m_buildings[m_IDChosenBuilding].GetConstructionCost());
				
				// We change the main character status
				_game->m_mainCharacter->SetIsCurrentlyBuilding(true);

				//----------------------------------------------------------------------------------------------
			}
		}
		// Condition to determine if the destructor mode has been selected
		else if (m_IDChosenBuilding == _game->m_builds.GetNumberOfBuildings())
		{
			// Collisions verifications
			if (_game->m_map->GetMap()[FIRST_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x] == BUILDING_GHOST)
			{
				int buildingIDFocused = _game->m_map->GetMap()[FIRST_FLOOR + BUILDING_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x];

				// We find the real position of the building
				sf::Vector2i buildingOriginPosition = _game->m_buildingsListPlanned->FindBuildingCorresponding(&_game->m_builds, m_buildingCaseSelected, (enum TypeOfBuilding)buildingIDFocused);
				
				// We remove it from the planned list of buildings to construct
				_game->m_buildingsListPlanned->RemoveBuildingAtPrecisePosition(buildingOriginPosition);

				// Replace on the map the ghost building by the previous version


				// Remove the previous saved position in the ghost list


				// We give back the money for the building construction
				_game->m_money.AddMoney(_game->m_builds.m_buildings[buildingIDFocused].GetConstructionCost());
			}
			else if (_game->m_map->GetMap()[FIRST_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x] != NO_COLLISION)
			{
				int buildingIDFocused = _game->m_map->GetMap()[FIRST_FLOOR + BUILDING_ID][m_buildingCaseSelected.y][m_buildingCaseSelected.x];

				if (buildingIDFocused != 65535)
				{
					sf::Vector2i buildingOriginPosition = FindRealOriginBuildingPosition(_game->m_map, &_game->m_builds, buildingIDFocused, m_buildingCaseSelected);

					BuildingDestruction::GetSingleton()->AddNewBuildingToDestroy((sf::Vector2f)buildingOriginPosition, buildingIDFocused);

					// We add the temporary collision "BUILDING_GHOST" where the building has been placed
					SetGhostDestructionBuildingOnMap(_game->m_map, &_game->m_builds, buildingIDFocused, buildingOriginPosition);
				}
			}
		}
	}
}

void BuildWindow::UpdateBuildWindow(struct Game *_game)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->m_window);
	sf::Vector2f cameraIso = WorldToScreen(_game->m_camera.x, _game->m_camera.y);

	//std::cout << "Mouse Position : " << mousePosition.x << " & " << mousePosition.y << std::endl;

	m_buildingCaseSelected = ScreenToTileMouse(((mousePosition.x - (_game->m_screenReso->x / 2)) - cameraIso.x * _game->m_scale.x),
												((mousePosition.y - (_game->m_screenReso->y / 2)) - cameraIso.y * _game->m_scale.y),
												_game->m_scale);

	//std::cout << "Case : " << _game->buildingCaseSelected.x << " & " << _game->buildingCaseSelected.y << std::endl << std::endl;

	if (m_IDChosenBuilding < _game->m_builds.GetNumberOfBuildings())
	{
		m_sizeBuildingSelected = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize();

		// Security to avoid an array exit
		if (_game->m_map->IsCoordinatesIsInMap(m_buildingCaseSelected))
		{
			if (m_IDChosenBuilding >= 0
				&& m_IDChosenBuilding < _game->m_builds.GetNumberOfBuildings())
			{
				bool isAreaEmpty = true;

				for (int y = 0; y < _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().y; y++)
				{
					for (int x = 0; x < _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().x; x++)
					{
						if (_game->m_map->IsCoordinatesIsInMap(sf::Vector2i(m_buildingCaseSelected.y - y, m_buildingCaseSelected.x - x)))
						{
							// Check cell occupation concerning collisions at the zero and first floors
							if (_game->m_map->GetMap()[FIRST_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y - y][m_buildingCaseSelected.x - x] != NO_COLLISION
								|| _game->m_map->GetMap()[ZERO_FLOOR + COLLISIONS_ID][m_buildingCaseSelected.y - y][m_buildingCaseSelected.x - x] != NO_COLLISION)
							{
								// The case is occupied
								isAreaEmpty = false;

								// We cut the for loop
								y = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().y;
								x = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().x;
							}
						}
						else
						{
							// The case is occupied
							isAreaEmpty = false;

							// We cut the for loop
							y = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().y;
							x = _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().x;
						}
					}
				}

				// If we didn't found an occupied place, we call that the place is empty
				m_isBuildingCaseOccupied = (isAreaEmpty) ? false : true;
			}
		}
		else
		{
			m_isBuildingCaseOccupied = true;
		}

		// In case where the player doesn't have enough money, or this is the stall, and it has been constructed, the cells on ground are displayed in red
		if (_game->m_money.GetMoneyQuantity() < _game->m_builds.m_buildings[m_IDChosenBuilding].GetConstructionCost()
			|| (m_IDChosenBuilding == BUILDING_STALL && _game->m_builds.m_stall->GetConstructionStatus() != BUILDING_DESTROYED))
		{
			m_isBuildingCaseOccupied = true;
		}
	}
	else
	{
		m_sizeBuildingSelected = sf::Vector2i(1, 1);
	}

	// Text creation if they don't exist
	if (_game->m_builds.m_buildingsNameTexts == nullptr)
	{
		_game->m_builds.m_buildingsNameTexts = new sf::Text[_game->m_builds.GetNumberOfBuildings()];

		for (int i = 0; i < _game->m_builds.GetNumberOfBuildings(); i++)
		{
			LoadTextString(&_game->m_builds.m_buildingsNameTexts[i], _game->m_builds.m_buildings[i].GetName(), &_game->m_charlemagneFont, 18, sf::Color::Black, 2);
		}
	}

	// Update the texts
	UpdateTextsBuildWindow(_game);
}

void BuildWindow::UpdateTextsBuildWindow(struct Game *_game)
{
	if (m_previousIDChosenBuilding != m_IDChosenBuilding
		&& m_IDChosenBuilding >= 0 
		&& m_IDChosenBuilding < _game->m_builds.GetNumberOfBuildings())
	{
		// Numerics texts
		UpdateDynamicsTexts(&m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_X], _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().x);
		UpdateDynamicsTexts(&m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_Y], _game->m_builds.m_buildings[m_IDChosenBuilding].GetSize().y);

		UpdateDynamicsTexts(&m_textBuildingHelps[BUILD_WINDOW_HELP_MONEY_COST], _game->m_builds.m_buildings[m_IDChosenBuilding].GetConstructionCost());
		
		// Texts				
		LoadTextString(&m_textBuildingHelps[BUILD_WINDOW_HELP_DESCRIPTION], ConvertStringIntoParagraph(_game->m_builds.m_buildings[m_IDChosenBuilding].GetDescription(), 25));

		m_previousIDChosenBuilding = m_IDChosenBuilding;
	}
}

void BuildWindow::DisplayBuildWindow(struct Game *_game)
{
	// Display of the building selection UI
	BlitSprite(m_buildingUI, (float)_game->m_screenReso->x - m_buildingUI.getGlobalBounds().width, (float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height, 0.0f, *_game->m_window);
	
	// Display of the help scroll that indicates every building info
	if (m_IDChosenBuilding < _game->m_builds.GetNumberOfBuildings())
	{
		BlitSprite(m_buildingUI, (float)_game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2), (float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height, 0.0f, *_game->m_window);

		BlitSprite(_game->m_builds.m_buildings[m_IDChosenBuilding].GetIcon(), ((float)_game->m_screenReso->x - m_buildingUI.getGlobalBounds().width * 2) + 135, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 135, 0.0f, *_game->m_window);

		// Display of the info paper about the building selected
		if (_game->m_builds.m_buildingsNameTexts != nullptr)
		{
			// Display the name
			BlitString(_game->m_builds.m_buildingsNameTexts[m_IDChosenBuilding], (_game->m_screenReso->x - (int)(m_buildingUI.getGlobalBounds().width * 2) + ((int)m_buildingUI.getGlobalBounds().width / 2)), (_game->m_screenReso->y - (int)m_buildingUI.getGlobalBounds().height) + 70, *_game->m_window);
			
			BlitSprite(m_separationLine, ((float)_game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)), ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 120.0f, 0.0f, *_game->m_window);


			// Display the size
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_X], _game->m_screenReso->x - ((int)m_buildingUI.getGlobalBounds().width * 2) + 240, (_game->m_screenReso->y - (int)m_buildingUI.getGlobalBounds().height) + 130, *_game->m_window);
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_LIAISON], _game->m_screenReso->x - ((int)m_buildingUI.getGlobalBounds().width * 2) + 270, (_game->m_screenReso->y - (int)m_buildingUI.getGlobalBounds().height) + 134, *_game->m_window);
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_SIZE_Y], _game->m_screenReso->x - ((int)m_buildingUI.getGlobalBounds().width * 2) + 290, (_game->m_screenReso->y - (int)m_buildingUI.getGlobalBounds().height) + 130, *_game->m_window);
			
			// Display the entering ressource logo
			BlitSprite(m_enteringArrow, (float)_game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2) + 230, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 181, 0, *_game->m_window);
			
			if (_game->m_builds.m_buildings[m_IDChosenBuilding].GetRessourceNumberNeeded() > 0)
			{
				for (int i = 0; i < _game->m_builds.m_buildings[m_IDChosenBuilding].GetRessourceNumberNeeded(); i++)
				{
					sf::Sprite tempoSprite = RessourcesManager::GetSingleton()->GetResourceSprite((TypesOfRessources)(_game->m_builds.m_buildings[m_IDChosenBuilding].GetRessourceIDNeeded(i + 1)));
					BlitSprite(tempoSprite, (float)_game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2) + 255 + i * 32, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 165, 0, *_game->m_window);
				}
			}
			
			// Display the exiting ressource logo
			BlitSprite(m_exitingArrow, (float)_game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2) + 230, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 216, 0, *_game->m_window);
			
			if (_game->m_builds.m_buildings[m_IDChosenBuilding].GetRessourceNumberProduced() > 0)
			{
				for (int i = 0; i < _game->m_builds.m_buildings[m_IDChosenBuilding].GetRessourceNumberProduced(); i++)
				{
					sf::Sprite tempoSprite = RessourcesManager::GetSingleton()->GetResourceSprite((TypesOfRessources)(_game->m_builds.m_buildings[m_IDChosenBuilding].GetRessourceIDProduced(i + 1)));
					BlitSprite(tempoSprite, _game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2) + 255 + i * 32, (_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 200, 0, *_game->m_window);
				}
			}

			BlitSprite(m_separationLine, (_game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)), (_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 250, 0, *_game->m_window);


			// Display the description
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_DESCRIPTION], _game->m_screenReso->x - ((int)m_buildingUI.getGlobalBounds().width * 2) + 120, (_game->m_screenReso->y - (int)m_buildingUI.getGlobalBounds().height) + 260, *_game->m_window);

			BlitSprite(m_separationLine, ((float)_game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)), ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 385, 0, *_game->m_window);


			// Display the money cost of the building
			BlitString(m_textBuildingHelps[BUILD_WINDOW_HELP_MONEY_COST], (_game->m_screenReso->x - ((int)m_buildingUI.getGlobalBounds().width * 2) + ((int)m_buildingUI.getGlobalBounds().width / 2)) - 45, (_game->m_screenReso->y - (int)m_buildingUI.getGlobalBounds().height) + 400, *_game->m_window);
			
			_game->m_money.SetSpriteScale(sf::Vector2f(0.45f, 0.45f));
			BlitSprite(_game->m_money.GetSprite(), ((float)_game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2) + (m_buildingUI.getGlobalBounds().width / 2)) + 45, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 412, 0, *_game->m_window);
			_game->m_money.SetSpriteScale(sf::Vector2f(1, 1));
		}
	}




	// Display of the buildings list
	for (int i = 0; i < _game->m_builds.GetNumberOfBuildings(); i++)
	{
		if (((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList >= ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height + 40)
			&& ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList <= (float)_game->m_screenReso->y - _game->m_builds.m_buildings[i].GetIcon().getGlobalBounds().height - 40)
		{
			BlitSprite(_game->m_builds.m_buildings[i].GetIcon(), ((float)_game->m_screenReso->x - m_buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList, 0, *_game->m_window);

			// In case where the player doesn't have enough money, or this is the stall and it has been already constructed, the picture is grayed out
			if (_game->m_money.GetMoneyQuantity() < _game->m_builds.m_buildings[i].GetConstructionCost()
				|| (i == BUILDING_STALL && _game->m_builds.m_stall->GetConstructionStatus() != BUILDING_DESTROYED))
			{
				m_blackFilter.setColor(sf::Color(255, 255, 255, 150));

				BlitSprite(m_blackFilter, ((float)_game->m_screenReso->x - m_buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList, 0, *_game->m_window);
			}

			if (_game->m_builds.m_buildingsNameTexts != nullptr)
			{
				BlitString(_game->m_builds.m_buildingsNameTexts[i], ((float)_game->m_screenReso->x - m_buildingUI.getGlobalBounds().width) + 143 + (i % 2) * 109 + 32, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (i / 2) + m_scrollBuildingList + 70, *_game->m_window);
			}
		}

	}

	// Display of the destroy button
	if ((_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (_game->m_builds.GetNumberOfBuildings() / 2) + m_scrollBuildingList >= (_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height + 40)
		&& (_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (_game->m_builds.GetNumberOfBuildings() / 2) + m_scrollBuildingList <= _game->m_screenReso->y - m_buildingUIdestroyBuildings.getGlobalBounds().height - 40)
	{
		BlitSprite(m_buildingUIdestroyBuildings, ((float)_game->m_screenReso->x - m_buildingUI.getGlobalBounds().width) + 143 + (_game->m_builds.GetNumberOfBuildings() % 2) * 109, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (_game->m_builds.GetNumberOfBuildings() / 2) + m_scrollBuildingList, 0, *_game->m_window);
	}

	if (m_IDChosenBuilding <= _game->m_builds.GetNumberOfBuildings())
	{
		if ((_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (m_IDChosenBuilding / 2) + m_scrollBuildingList >= (_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height + 40)
			&& (_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (m_IDChosenBuilding / 2) + m_scrollBuildingList <= _game->m_screenReso->y - m_contour.getGlobalBounds().height - 40)
		{
			// Display of the contour when a building is selected to be built
			BlitSprite(m_contour, ((float)_game->m_screenReso->x - m_buildingUI.getGlobalBounds().width) + 143 + (m_IDChosenBuilding % 2) * 109 - 4, ((float)_game->m_screenReso->y - m_buildingUI.getGlobalBounds().height) + 130 * (m_IDChosenBuilding / 2) - 4 + m_scrollBuildingList, 0, *_game->m_window);
		}
	}


	sf::Vector2i mousePosition = sf::Mouse::getPosition(*_game->m_window);

	// Display of the building chosen
	if (m_IDChosenBuilding < _game->m_builds.GetNumberOfBuildings())
	{
		Buildings* currentBuilding = &_game->m_builds.m_buildings[m_IDChosenBuilding];

		// Display the building choosen on the mouse
		for (int y = 0; y < currentBuilding->GetSize().y; y++)
		{
			for (int x = 0; x < currentBuilding->GetSize().x; x++)
			{
				unsigned short buildingSpriteID;

				if (m_IDChosenBuilding == TypeOfBuilding::BUILDING_PATH || m_IDChosenBuilding == TypeOfBuilding::BUILDING_PAVED_PATH
					|| m_IDChosenBuilding == TypeOfBuilding::BUILDING_ROAD)
				{
					buildingSpriteID = (unsigned short)currentBuilding->GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_GROUND][currentBuilding->GetSize().y - 1 - y][currentBuilding->GetSize().x - 1 - x];
				}
				else
				{
					buildingSpriteID = (unsigned short)currentBuilding->GetVecBuildingsSpritesID()[(int)FloorsInBuildingSprites::FIBS_MAIN_FLOOR][currentBuilding->GetSize().y - 1 - y][currentBuilding->GetSize().x - 1 - x];
				}

				sf::Sprite buildingSprite = _game->m_builds.GetSpriteFromBuildID(m_IDChosenBuilding, buildingSpriteID);

				if (m_isBuildingCaseOccupied == false)
				{
					sf::Color color(255, 255, 255, 150);
					buildingSprite.setColor(color);
				}
				else
				{
					sf::Color color(255, 0, 0, 150);
					buildingSprite.setColor(color);
				}

				sf::Vector2f tileCoordinates = WorldToScreen(-x, -y);

				BlitSprite(buildingSprite, (float)mousePosition.x + tileCoordinates.x, (float)mousePosition.y + tileCoordinates.y, *_game->m_window);

				buildingSprite.setColor(sf::Color::White);
			}
		}
	}

	// Display the "Out of territory" and "Place already occupied" messages
	if (m_isBuildingCaseOccupied == true
		&& !(mousePosition.x > _game->m_screenReso->x - (m_buildingUI.getGlobalBounds().width * 2)
			&& mousePosition.x < _game->m_screenReso->x
			&& mousePosition.y > _game->m_screenReso->y - (m_buildingUI.getGlobalBounds().height * 2)
			&& mousePosition.y < _game->m_screenReso->y))
	{
		if (!_game->m_map->IsCoordinatesIsInMap(m_buildingCaseSelected))
		{
			BlitString(m_textBuildingCaseOccupied[1], 500, _game->m_screenReso->y - 100, *_game->m_window);
		}
		else if (m_textBuildingCaseOccupied != nullptr)
		{
			BlitString(m_textBuildingCaseOccupied[0], 500, _game->m_screenReso->y - 100, *_game->m_window);
		}
	}
}

void BuildWindow::SaveFromMapPreviousBuildNSpriteID(sf::Vector2i _mapPosition, unsigned short _currentFloor, unsigned short _typeOfBuilding, unsigned short _spriteID)
{
	LinkedListClass::sElement* newElement = new LinkedListClass::sElement;
	newElement->data = new sPreviousPositionIDs;

	// Save the position in map
	((sPreviousPositionIDs*)newElement->data)->mapPosition = _mapPosition;
	((sPreviousPositionIDs*)newElement->data)->currentFloor = _currentFloor;

	((sPreviousPositionIDs*)newElement->data)->typeOfBuilding = _typeOfBuilding;
	((sPreviousPositionIDs*)newElement->data)->spriteID = _spriteID;

	newElement->status = ElementStatus::ELEMENT_ACTIVE;

	// Add this element at the end the list
	if (m_listOfPreviousID == nullptr) m_listOfPreviousID = LinkedListInitialisation();
	AddElementToLinkedList(m_listOfPreviousID, newElement, -1);
}


void BuildWindow::SaveFromMapPreviousBuildID(sf::Vector2i _mapPosition, unsigned short _currentFloor, unsigned short _typeOfBuilding)
{
	LinkedListClass::sElement* newElement = new LinkedListClass::sElement;
	newElement->data = new sPreviousPositionIDs;

	// Save the position in map
	((sPreviousPositionIDs*)newElement->data)->mapPosition = _mapPosition;
	((sPreviousPositionIDs*)newElement->data)->currentFloor = _currentFloor;

	((sPreviousPositionIDs*)newElement->data)->typeOfBuilding = _typeOfBuilding;
	((sPreviousPositionIDs*)newElement->data)->spriteID = 0;

	newElement->status = ElementStatus::ELEMENT_ACTIVE;

	// Add this element at the end the list
	if (m_listOfPreviousID == nullptr) m_listOfPreviousID = LinkedListInitialisation();
	AddElementToLinkedList(m_listOfPreviousID, newElement, -1);
}

void BuildWindow::SaveFromMapPreviousSpriteID(sf::Vector2i _mapPosition, unsigned short _currentFloor, unsigned short _spriteID)
{
	LinkedListClass::sElement* newElement = new LinkedListClass::sElement;
	newElement->data = new sPreviousPositionIDs;

	// Save the position in map
	((sPreviousPositionIDs*)newElement->data)->mapPosition = _mapPosition;
	((sPreviousPositionIDs*)newElement->data)->currentFloor = _currentFloor;

	((sPreviousPositionIDs*)newElement->data)->typeOfBuilding = 0;
	((sPreviousPositionIDs*)newElement->data)->spriteID = _spriteID;

	newElement->status = ElementStatus::ELEMENT_ACTIVE;

	// Add this element at the end the list
	if (m_listOfPreviousID == nullptr) m_listOfPreviousID = LinkedListInitialisation();
	AddElementToLinkedList(m_listOfPreviousID, newElement, -1);
}

void BuildWindow::LoadOnMapPreviousID()
{
	if (m_listOfPreviousID != nullptr)
	{
		if (m_listOfPreviousID->first != nullptr)
		{
			Map* pMap = Map::GetSingleton();

			for (LinkedListClass::sElement* currentElement = m_listOfPreviousID->first; currentElement != NULL; currentElement = currentElement->next)
			{
				sPreviousPositionIDs* pPrevPosID = (sPreviousPositionIDs*)currentElement->data;

				if (pPrevPosID->currentFloor % 3 == COLLISIONS_ID)
				{
					pMap->GetMap()[pPrevPosID->currentFloor + BUILDING_ID][pPrevPosID->mapPosition.y][pPrevPosID->mapPosition.x] = pPrevPosID->typeOfBuilding;
					pMap->GetMap()[pPrevPosID->currentFloor + SPRITE_ID][pPrevPosID->mapPosition.y][pPrevPosID->mapPosition.x] = pPrevPosID->spriteID;
				}
				else if (pPrevPosID->currentFloor % 3 == BUILDING_ID)
				{
					pMap->GetMap()[pPrevPosID->currentFloor][pPrevPosID->mapPosition.y][pPrevPosID->mapPosition.x] = pPrevPosID->typeOfBuilding;
				}
				else if (pPrevPosID->currentFloor % 3 == SPRITE_ID)
				{
					pMap->GetMap()[pPrevPosID->currentFloor][pPrevPosID->mapPosition.y][pPrevPosID->mapPosition.x] = pPrevPosID->spriteID;
				}

				currentElement->status = ElementStatus::ELEMENT_DELETION_REQUIRED;
			}
		}

		// We clear the list
		FreeLinkedList(m_listOfPreviousID);
		m_listOfPreviousID = nullptr;
	}
}


void BuildWindow::SavingGhostBuildingsForFile(std::ofstream* _file)
{
	// Save the number of elements in the list
	_file->write((char*)&m_listOfPreviousID->size, sizeof(int));

	if (m_listOfPreviousID != nullptr)
	{
		if (m_listOfPreviousID->first != nullptr)
		{
			LinkedListClass::sElement* currentElement = m_listOfPreviousID->first;

			for (currentElement = m_listOfPreviousID->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				_file->write((char*)(sPreviousPositionIDs*)currentElement->data, sizeof(sPreviousPositionIDs));
			}
		}
	}
}

void BuildWindow::LoadingGhostBuildingsFromFile(std::ifstream* _file)
{
	// Delete every elements of the list
	if (m_listOfPreviousID != nullptr)
	{
		FreeLinkedList(m_listOfPreviousID);
	}

	// We reinit the list
	m_listOfPreviousID = LinkedListInitialisation();

	// Load the number of elements
	int previousListSize(RESET);
	_file->read((char*)&previousListSize, sizeof(int));

	// We add every element data to the list
	for (int i = RESET; i < previousListSize; i++)
	{
		std::cout << i << std::endl;
		LinkedListClass::sElement* newElement = new LinkedListClass::sElement;
		newElement->data = new sPreviousPositionIDs;

		_file->read((char*)newElement->data, sizeof(sPreviousPositionIDs));

		newElement->status = ElementStatus::ELEMENT_ACTIVE;

		// Add this element at the end the list
		if (m_listOfPreviousID == nullptr) m_listOfPreviousID = LinkedListInitialisation();
		AddElementToLinkedList(m_listOfPreviousID, newElement, -1);
	}

	LoadOnMapPreviousID();
}