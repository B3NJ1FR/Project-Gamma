#include "Buildings.h"


Buildings::Buildings()
{
	// Default initialisation
	m_size = { 0, 0 };
	m_enterPosition = { 0, 0 };
	m_exitPosition = { 0, 0 };

	m_id = RESET;

	m_moneyCostToBuild = RESET;
	m_timeToBuild = RESET;
	m_timeToProduce = RESET;

	m_timeToPickUpRessources = RESET;
	m_timeToDeposeRessources = RESET;

	m_numberRessourceNeeded = RESET;
	m_numberRessourceProduced = RESET;

	m_ressourceIDNeeded = nullptr;
	m_ressourceQuantityNeeded = nullptr;

	m_ressourceIDProduced = nullptr;
	m_ressourceQuantityProduced = nullptr;
}



Buildings::~Buildings()
{

}

void Buildings::SetRessourceIDNeeded(int* _resssourceID, const unsigned char& _numberRessources)
{
	if (m_ressourceIDNeeded != nullptr)
	{
		delete m_ressourceIDNeeded;
		m_ressourceIDNeeded = nullptr;
	}

	m_numberRessourceNeeded = _numberRessources;

	m_ressourceIDNeeded = _resssourceID;
}

void Buildings::SetRessourceQuantityNeeded(int* _resssourceQuantity)
{
	if (m_ressourceQuantityNeeded != nullptr)
	{
		delete m_ressourceQuantityNeeded;
		m_ressourceQuantityNeeded = nullptr;
	}

	m_ressourceQuantityNeeded = _resssourceQuantity;
}

void Buildings::SetRessourceIDProduced(int* _resssourceID, const unsigned char& _numberRessources)
{
	if (m_ressourceIDProduced != nullptr)
	{
		delete m_ressourceIDProduced;
		m_ressourceIDProduced = nullptr;
	}

	m_numberRessourceProduced = _numberRessources;

	m_ressourceIDProduced = _resssourceID;
}

void Buildings::SetRessourceQuantityProduced(int* _resssourceQuantity)
{
	if (m_ressourceQuantityProduced != nullptr)
	{
		delete m_ressourceQuantityProduced;
		m_ressourceQuantityProduced = nullptr;
	}

	m_ressourceQuantityProduced = _resssourceQuantity;
}



int Buildings::GetRessourceIDNeeded(const unsigned char& _value) const
{
	if (_value - 1 >= 0 && (unsigned)(_value - 1) < m_numberRessourceNeeded)
	{
		return m_ressourceIDNeeded[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}

int Buildings::GetRessourceQuantityNeeded(const unsigned char& _value) const
{
	if (_value - 1 >= 0 && (unsigned)(_value - 1) < m_numberRessourceNeeded)
	{
		return m_ressourceQuantityNeeded[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}

int Buildings::GetRessourceIDProduced(const unsigned char& _value) const
{
	if (_value - 1 >= 0 && (unsigned)(_value - 1) < m_numberRessourceProduced)
	{
		return m_ressourceIDProduced[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}

int Buildings::GetRessourceQuantityProduced(const unsigned char& _value) const
{
	if (_value - 1 >= 0 && (unsigned)(_value - 1) < m_numberRessourceProduced)
	{
		return m_ressourceQuantityProduced[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}
