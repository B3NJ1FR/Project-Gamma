#pragma once

#include "Common.hpp"
#include "Map.h"
#include "BuildingManagement.h"

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
	inline sf::Sprite GetBuildingUI() const { return m_buildingUI; };
	inline sf::Sprite GetBuildingUIClosed() const { return m_buildingUIclosed; };
	inline sf::Vector2i GetBuildingCheckboxSelected() const { return m_buildingCaseSelected; };

	inline float GetScrollBuildingList() const { return m_scrollBuildingList; };
	inline int GetIDChosenBuilding() const { return m_IDChosenBuilding; };

	inline bool GetIsBuildingCaseOccupied() const { return m_isBuildingCaseOccupied; };

	// Setters
	inline void SetScrollBuildingList(const float& _scrollBuildingList) { m_scrollBuildingList = _scrollBuildingList; };
	inline void SetIDChosenBuilding(const int& _buildingID) { m_IDChosenBuilding = _buildingID; };

	// Methods
	bool IsBuildingCheckboxIsInMap(const sf::Vector2i &_mapSize, const sf::Vector2i &_mapPosition);

	void InputBuildingModeOldScrollUI(const float &_scrollDelta, const sf::RenderWindow &_window);
	void InputPickUpCaseClicked(sf::RenderWindow &_window, bool _isBuildingUINeeded, const sf::Vector2f &_camera, const sf::Vector2f &_cameraScale);
	
	void SetBuildingOnMap(Map* _map, BuildingManagement* _builds, enum Floors _floorFocused, int _typeOfBuilding, enum TypesOfCollisions _collisionID, const sf::Vector2i &_mapPosition);
	void SetGhostBuildingOnMap(struct Game *_game, const int &_typeOfBuilding, const sf::Vector2i &_mapPosition);
	void RemoveBuildingOnMap(struct Game *_game, enum Floors _floorFocused, const int &_typeOfBuilding, const sf::Vector3i &_statsToApply, const sf::Vector2i &_mapPosition);
	
	void UpdateTextsBuildWindow(struct Game *_game);

	void InputBuildWindow(struct Game *_game);
	void UpdateBuildWindow(struct Game *_game);
	void DisplayBuildWindow(struct Game *_game);
};