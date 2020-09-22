#ifndef MAIN_CHARACTER__H
#define MAIN_CHARACTER__H

#include "Entities.h"


class MainCharacter : public Entities
{
private:
	sf::Sprite sprite;

	enum WorkerStatus actualStatus;
	
	bool isLauchingMovement; // Temporaire
	bool isItWorkingPlace;

	enum TypeOfBuilding actualBuilding;
	enum TypeOfBuilding *targetedBuilding;

	//float timeToDeposit;

	bool isMainCharacterSelected;
	bool isCurrentlyBuilding;

public:
	MainCharacter();
	~MainCharacter();

	void InitPathfinding(struct Game *_game);

	void SetMainCharacterPosition(const sf::Vector2f &_mapPosition);
	void SetMainCharacterEndingPosition(sf::Vector2i _mapPosition, unsigned short ***_map);
	void SetSpriteScale(const sf::Vector2f &_newScale);
	void SetMainCharacterStatus(const enum WorkerStatus &_newStatus, const bool &_isLaunchingMovement = false);
	void SetIsMainCharacterSelected(const bool &_isMainCharacterSelected);
	void SetWorkerIsInWorkingPlace(const bool &_isItWorkingPlace);
	void SetIsCurrentlyBuilding(const bool &_isCurrentlyBuilding);

	sf::Vector2f GetMainCharacterPosition();
	bool IsMainCharacterPosition(const sf::Vector2i &_mapPosition);
	sf::Vector2f GetMainCharacterEndingPosition();
	sf::Sprite GetSprite();
	bool GetIsMainCharacterSelected();
	//bool GetWorkerIsInWorkingPlace();
	enum WorkerStatus GetWorkerStatus();
	bool GetIsCurrentlyBuilding();

	void UpdatePathAndActivities(struct Game *_game);

	//bool isPressingStart; // Temporaire
	//bool isPressingEnd; // Temporaire
};



#endif // !MAIN_CHARACTER__H