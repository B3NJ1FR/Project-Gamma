#pragma once

#include "Entities.h"
#include "GameDefinitions.h"
#include "Map.h"
#include "BuildingsListPlanned.h"


class MainCharacter : public Entities
{
private:
	sf::Sprite m_sprite;

	WorkerStatus m_actualStatus;
	
	bool m_isLauchingMovement; // Temporaire
	bool m_isItWorkingPlace;

	float m_buildingTimer;
	float m_waitingTimer;

	enum TypeOfBuilding m_actualBuilding;
	enum TypeOfBuilding *m_targetedBuilding;

	//float timeToDeposit;

	bool m_isMainCharacterSelected;
	bool m_isCurrentlyBuilding;

public:
	// Constructor & Destructor
	MainCharacter();
	~MainCharacter();

	void InitPathfinding(Game *_game);

	// Setters
	void SetMainCharacterPosition(const sf::Vector2f &_mapPosition);
	void SetMainCharacterEndingPosition(const sf::Vector2i& _mapPosition, Map* _map);
	void SetSpriteScale(const sf::Vector2f &_newScale);
	void SetMainCharacterStatus(const enum WorkerStatus &_newStatus, const bool &_isLaunchingMovement = false);
	void SetIsMainCharacterSelected(const bool &_isMainCharacterSelected);
	void SetWorkerIsInWorkingPlace(const bool &_isItWorkingPlace);
	void SetIsCurrentlyBuilding(const bool &_isCurrentlyBuilding);

	// Getters
	inline sf::Vector2f GetMainCharacterPosition() const;
	inline sf::Vector2f GetMainCharacterEndingPosition() const;
	inline sf::Sprite GetSprite() const;
	inline bool GetIsMainCharacterSelected() const;
	inline enum WorkerStatus GetWorkerStatus() const;
	inline bool GetIsCurrentlyBuilding() const;
	
	inline bool IsMainCharacterPosition(const sf::Vector2i &_mapPosition) const;
	void UpdatePathAndActivities(Game *_game);

	//bool isPressingStart; // Temporaire
	//bool isPressingEnd; // Temporaire
};