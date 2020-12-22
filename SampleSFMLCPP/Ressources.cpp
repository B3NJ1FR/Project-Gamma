#include "Ressources.h"


Ressources::Ressources()
{
	m_quantityOwned = RESET;
	m_name = "";
}

Ressources::~Ressources()
{

}


void Ressources::AddOrSubtractQuantityOwned(int _quantityToAdd)
{
	if (m_quantityOwned + _quantityToAdd >= 0)
	{
		m_quantityOwned += _quantityToAdd;
	}
}