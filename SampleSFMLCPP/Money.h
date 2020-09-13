#ifndef MONEY__H
#define MONEY__H

#include "Common.hpp"


class Money
{
private :
	int playerMoney;
	sf::Sprite sprite;

public:
	Money();
	~Money();

	void SetSpriteScale(const sf::Vector2f &_scale);
	void SetInitialQuantity(int _quantityToAdd);
	void AddMoney(int _quantityToAdd = 1);
	void SubtractMoney(int _quantityToRemove = 1);

	sf::Sprite GetSprite();
	int GetMoneyQuantity();
};

#endif //!MONEY__H
