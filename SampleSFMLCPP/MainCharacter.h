#pragma once

#include "Entities.h"
#include "BuildingsListPlanned.h"
#include "Map.h"
//#include "GameDefinitions.h"


class MainCharacter : public Entities
{
private:
	sf::Sprite m_sprite;
	sf::Sprite m_spriteCharaSelected;

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

	void InitPathfinding(Map * _map);

	// Setters
	inline void SetMainCharacterPosition(const sf::Vector2f& _mapPosition) { m_mapPosition = _mapPosition; };
	void SetMainCharacterEndingPosition(const sf::Vector2i& _mapPosition, Map* _map);
	inline void SetSpriteScale(const sf::Vector2f& _newScale) { m_sprite.setScale(sf::Vector2f(_newScale.x * 1.3f, _newScale.y * 1.3f)); };
	void SetMainCharacterStatus(const enum WorkerStatus &_newStatus, const bool &_isLaunchingMovement = false);
	inline void SetIsMainCharacterSelected(const bool& _isMainCharacterSelected) { m_isMainCharacterSelected = _isMainCharacterSelected; };
	inline void SetWorkerIsInWorkingPlace(const bool& _isItWorkingPlace) { m_isItWorkingPlace = _isItWorkingPlace; };
	inline void SetIsCurrentlyBuilding(const bool& _isCurrentlyBuilding) { m_isCurrentlyBuilding = _isCurrentlyBuilding; };

	// Getters
	inline sf::Vector2f GetMainCharacterPosition() const { return m_mapPosition; };

	inline sf::Vector2f GetMainCharacterEndingPosition() const { return m_mapEndPosition; };
	inline sf::Sprite GetSprite() const { return m_sprite; };
	inline bool GetIsMainCharacterSelected() const { return m_isMainCharacterSelected; };
	inline enum WorkerStatus GetWorkerStatus() const { return m_actualStatus; };
	inline bool GetIsCurrentlyBuilding() const { return m_isCurrentlyBuilding; };
	
	inline bool IsMainCharacterPosition(const sf::Vector2i& _mapPosition) const { return (sf::Vector2i(m_mapPosition) == _mapPosition) ? true : false; };
	void DisplayMainCharacter(const sf::Vector2i& _actualPosition, const sf::Vector3f& _cameraPosition, const sf::Vector2i& _screenSize, const sf::Vector2f& _gameScale, sf::RenderWindow& _window);
	void UpdatePathAndActivities(struct Game *_game); // WARNING NEED TO BE VERIFIED IF NOT WORKING
	//void UpdatePathAndActivities(Map* _map, TimeManagement* _time, BuildingManagement* _builds, BuildingsListPlanned* _buildingsList, BuildWindow* _buildWindow, Ressources* _ressources);
	//bool isPressingStart; // Temporaire
	//bool isPressingEnd; // Temporaire
};