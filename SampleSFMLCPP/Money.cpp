#include "Money.h"
#include "ListOfAnnualCostsNRevenues.h"
#include "TimeManagement.h"

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

void Money::Initialise()
{
	m_playerMoney = 4450;

	m_sprite = LoadSprite("Data/Assets/Sprites/Menu/sesterce.png", 1);
}


void Money::Earnings(EnumListOfCostsNRevenues _typeOfEarn, int _quantityToAdd)
{
	AddMoney(_quantityToAdd);
	ListOfAnnualCostsNRevenues::GetSingleton()->AddCNRValueToYear(_typeOfEarn, _quantityToAdd, TimeManagement::GetSingleton()->GetCurrentYear());
}

void Money::Payments(EnumListOfCostsNRevenues _typeOfCost, int _quantityToRemove)
{
	SubtractMoney(_quantityToRemove);
	ListOfAnnualCostsNRevenues::GetSingleton()->AddCNRValueToYear(_typeOfCost, -_quantityToRemove, TimeManagement::GetSingleton()->GetCurrentYear());
}