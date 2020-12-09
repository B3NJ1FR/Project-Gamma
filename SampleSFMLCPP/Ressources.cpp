#include "Ressources.h"


Ressources::Ressources()
{
	m_quantityOwned = RESET;
	m_name = "";
}

Ressources::~Ressources()
{

}


void Ressources::SetSprite(const std::string _spriteFileName, const int _spriteOriginPosition)
{
	m_sprite = LoadSprite(_spriteFileName, _spriteOriginPosition);
}

void Ressources::SetName(const std::string& _name)
{
	m_name = _name;
}

void Ressources::SetInitialQuantityOwned(const int _quantityOwned)
{
	m_quantityOwned = _quantityOwned;
}


void Ressources::AddOrSubtractQuantityOwned(const int _quantityToAdd)
{
	if (m_quantityOwned + _quantityToAdd >= 0)
	{
		m_quantityOwned += _quantityToAdd;
	}
}