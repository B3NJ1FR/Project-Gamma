#pragma once

#include "Common.hpp"


class Money
{
private :
	int m_playerMoney;
	sf::Sprite m_sprite;

	// Constructor
	Money();

public:
	// Destructor
	~Money();

	// Singleton
	static Money* GetSingleton();
	
	// Setters
	inline void SetSpriteScale(const sf::Vector2f& _scale) { m_sprite.setScale(_scale); };
	inline void SetInitialQuantity(int _quantityToAdd) { m_playerMoney = _quantityToAdd; };

	// Getters
	inline sf::Sprite GetSprite() const { return m_sprite; };
	inline int GetMoneyQuantity() const { return m_playerMoney; };

	// Methods
	void Initialise();
	inline void AddMoney(int _quantityToAdd = 1) { m_playerMoney += _quantityToAdd; };
	inline void SubtractMoney(int _quantityToRemove = 1) { m_playerMoney -= _quantityToRemove; };
};

