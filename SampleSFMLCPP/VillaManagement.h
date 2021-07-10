#pragma once

#include "TimeManagement.h"
#include "EnumVillaManagementStateMachine.h"

#define SCENE_BASIC_RESOLUTION_WIDTH 960
#define SCENE_BASIC_RESOLUTION_HEIGHT 540

class VillaManagement
{
private:
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_tableSprite;
	sf::Sprite m_bookshelfSprite;
	sf::Sprite m_entranceSprite;

	sf::Sprite m_greyBackground;

	sf::Image m_imageDomainPlan;
	sf::Image m_imagePapyrus;
	sf::Image m_imageBook;
	sf::Image m_imagePurse;
	sf::Image m_imageEntrance;

	VillaManagementStateMachine internalStateMachine;

public:
	// Constructor & Destructor
	VillaManagement();
	~VillaManagement();

	// Getters
	inline VillaManagementStateMachine GetInternalStateMachine() { return internalStateMachine; }

	// Setters
	inline void SetInternalStateMachine(VillaManagementStateMachine _value) { internalStateMachine = _value; }

	// Methods
	void UpdateVillaManagement();
	void InputVillaManagement(enum CurrentGameState *_state, sf::RenderWindow &_window);
	void DisplayVillaManagement(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution);
};