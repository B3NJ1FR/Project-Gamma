#include "Money.h"

Money::Money()
{
	m_playerMoney = 4450;

	m_sprite = LoadSprite("Data/Assets/Sprites/Menu/sesterce.png", 1);
}

Money::~Money()
{
	delete (m_sprite.getTexture());
}


Money* Money::GetSingleton()
{
	static Money uniqueInstance;
	return &uniqueInstance;
}