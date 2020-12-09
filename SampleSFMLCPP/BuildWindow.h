#pragma once

#include "Common.hpp"
#include "GameDefinitions.h"

#define BUILD_WINDOW_HELP_NB_OF_TEXTS 5


enum BuildWindowTextBuildingHelps
{
	BUILD_WINDOW_HELP_SIZE_X,
	BUILD_WINDOW_HELP_SIZE_Y,
	BUILD_WINDOW_HELP_SIZE_LIAISON,
	BUILD_WINDOW_HELP_DESCRIPTION,
	BUILD_WINDOW_HELP_MONEY_COST,
};

class BuildWindow
{
private:
	int m_IDChosenBuilding;
	int m_previousIDChosenBuilding;
	sf::Vector2i m_buildingCaseSelected;
	bool m_isBuildingCaseOccupied;
	sf::Text *m_textBuildingCaseOccupied;
	sf::Text *m_textBuildingHelps;

	sf::Sprite m_buildingUI;
	sf::Sprite m_buildingUIclosed;
	sf::Sprite m_buildingUIdestroyBuildings;
	sf::Sprite m_blackFilter;
	sf::Sprite m_contour;
	sf::Sprite m_enteringArrow;
	sf::Sprite m_exitingArrow;
	sf::Sprite m_separationLine;
	float m_scrollBuildingList;

public:
	// Constructor & Destructor
	BuildWindow();
	~BuildWindow();
	
	void InitTextsBuildWindow(sf::Font *_font);
	void InitSpritesBuildWindow();

	// Getters
	inline sf::Sprite GetBuildingUI() const;
	inline sf::Sprite GetBuildingUIClosed() const;
	inline sf::Vector2i GetBuildingCheckboxSelected() const;
	inline float GetScrollBuildingList() const;
	inline int GetIDChosenBuilding() const;
	inline bool GetIsBuildingCaseOccupied() const;

	// Setters
	void SetScrollBuildingList(const float &_scrollBuildingList);
	void SetIDChosenBuilding(const int &_buildingID);

	// Methods
	bool IsBuildingCheckboxIsInMap(const sf::Vector2i &_mapSize, const sf::Vector2i &_mapPosition);

	void InputBuildingModeOldScrollUI(const float &_scrollDelta, const sf::RenderWindow &_window);
	void InputPickUpCaseClicked(sf::RenderWindow &_window, bool _isBuildingUINeeded, const sf::Vector2f &_camera, const sf::Vector2f &_cameraScale);
	
	void SetBuildingOnMap(Game *_game, const enum Floors &_floorFocused, const int &_typeOfBuilding, const enum TypesOfCollisions &_collisionID, const sf::Vector2i &_mapPosition);
	void SetGhostBuildingOnMap(Game *_game, const int &_typeOfBuilding, const sf::Vector2i &_mapPosition);
	void RemoveBuildingOnMap(Game *_game, enum Floors _floorFocused, const int &_typeOfBuilding, const sf::Vector3i &_statsToApply, const sf::Vector2i &_mapPosition);
	
	void UpdateTextsBuildWindow(Game *_game);

	void InputBuildWindow(Game *_game);
	void UpdateBuildWindow(Game *_game);
	void DisplayBuildWindow(Game *_game);
};