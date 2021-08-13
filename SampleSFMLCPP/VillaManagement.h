#pragma once

#include "TimeManagement.h"
#include "EnumVillaManagementStateMachine.h"

#define SCENE_BASIC_RESOLUTION_WIDTH 960
#define SCENE_BASIC_RESOLUTION_HEIGHT 540

enum class SpriteElements
{
	DOMAIN_PLAN,
	PAPYRUS_COSTS,
	BOOK_PRODUCTION,
	PURSE,
	QUIT_VILLA,

	NB_MAX_ELEMENTS,
};

class VillaManagement
{
private:
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_greyBackground;
	sf::Sprite m_spriteElements[(int)SpriteElements::NB_MAX_ELEMENTS];
	sf::Sprite m_spriteElementsFeedbacks[(int)SpriteElements::NB_MAX_ELEMENTS];

	sf::Image m_imageDomainPlan;
	sf::Image m_imagePapyrus;
	sf::Image m_imageBook;
	sf::Image m_imagePurse;
	sf::Image m_imageEntrance;


	bool m_isFeedbackActive[(int)SpriteElements::NB_MAX_ELEMENTS];

	VillaManagementStateMachine m_internalStateMachine;

public:
	// Constructor & Destructor
	VillaManagement();
	~VillaManagement();

	// Getters
	inline VillaManagementStateMachine GetInternalStateMachine() const { return m_internalStateMachine; }

	// Setters
	inline void SetInternalStateMachine(VillaManagementStateMachine _value) { m_internalStateMachine = _value; }

	// Methods
	void UpdateVillaManagement();
	void InputVillaManagement(enum CurrentGameState *_state, sf::RenderWindow &_window, const sf::Vector2i& _screenResolution);
	void DisplayVillaManagement(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution);
};