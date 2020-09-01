#ifndef BUILD_WINDOW__H
#define BUILD_WINDOW__H

#include "Common.hpp"


class BuildWindow
{
private:
	int IDChosenBuilding;
	sf::Vector2i buildingCaseSelected;
	bool isBuildingCaseOccupied;
	bool isNewBuildingHasBeenConstructed;
	sf::Text *textBuildingCaseOccupied;

	sf::Sprite buildingUI;
	sf::Sprite buildingUIclosed;
	sf::Sprite buildingUIdestroyBuildings;
	sf::Sprite blackFilter;
	sf::Sprite contour;
	float scrollBuildingList;

public:
	BuildWindow();
	~BuildWindow();

	void InitTextsBuildWindow(sf::Font *_font);
	void InitSpritesBuildWindow();

	sf::Sprite GetBuildingUI();
	sf::Sprite GetBuildingUIClosed();
	sf::Vector2i GetBuildingCheckboxSelected();
	bool GetIsNewBuildingHasBeenConstructed();
	float GetScrollBuildingList();
	int GetIDChosenBuilding();
	bool GetIsBuildingCaseOccupied();

	void SetIsNewBuildingHasBeenConstructed(const bool &_isNewBuildingHasBeenConstructed);
	void SetScrollBuildingList(const float &_scrollBuildingList);
	void SetIDChosenBuilding(const int &_buildingID);

	bool IsBuildingCheckboxIsInMap(const sf::Vector2i &_mapSize);

	void InputBuildingModeOldScrollUI(const float &_scrollDelta, const sf::RenderWindow &_window);
	void InputPickUpCaseClicked(sf::RenderWindow &_window, bool _isBuildingUINeeded, const sf::Vector2f &_camera, const sf::Vector2f &_cameraScale);
	void SetOrRemoveBuildingOnMap(struct Game *_game, bool _isConstructing, enum Floors _floorFocused, int _typeOfBuilding, sf::Vector3i _statsToApply);
	
	void InputBuildWindow(struct Game *_game);
	void UpdateBuildWindow(struct Game *_game);
	void DisplayBuildWindow(struct Game *_game);
};



#endif // !BUILD_WINDOW__H