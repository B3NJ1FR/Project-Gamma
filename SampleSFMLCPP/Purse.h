#pragma once

#include "Common.hpp"
#include "Money.h"
#include "EnumInternalState.h"
#include "EnumVillaManagementStateMachine.h"

class Purse
{
private:
	sf::Sprite m_papyrusBackground;
	sf::Sprite m_returnButton;

	InternalState internalState;

	sf::Text m_textMoneyName;
	sf::Text m_textQtyOfMoney;
	sf::Font m_font;

	// Constructor
	Purse();

public:
	// Destructor
	~Purse();

	// Singleton
	static Purse* GetSingleton();

	
	// Methods
	void Input(sf::Event _event, sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);
	void UpdateTextsContent();
	void Update(VillaManagementStateMachine* _internalStateMachine);
	void Display(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);
};