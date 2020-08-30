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

	void InputPickUpCaseClicked(sf::RenderWindow &_window, bool _isBuildingUINeeded, const sf::Vector2f &_camera, const sf::Vector2f &_cameraScale);
	void SetOrRemoveBuildingOnMap(struct Game *_game, bool _isConstructing, enum Floors _floorFocused, int _typeOfBuilding, sf::Vector3i _statsToApply);
	void InputBuildWindow(struct Game *_game);
	void UpdateBuildWindow();
	void DisplayBuildWindow();
};



#endif // !BUILD_WINDOW__H