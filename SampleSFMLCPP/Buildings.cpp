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

void Buildings::SetName(const std::string _name)
{
	// Set the building name
	m_name = _name;
}

void Buildings::SetDescription(std::string _description)
{
	// Get the building description
	m_description = _description;
}

void Buildings::SetSizeCaracteristics(const sf::Vector2i _buildingSize)
{
	// Set the building size in term of width and lengh
	m_size = _buildingSize;
}


void Buildings::SetEntranceCaracteristics(const sf::Vector2i _enterPosition)
{
	// Set the building entry position
	m_enterPosition = _enterPosition;
}


void Buildings::SetExitCaracteristics(const sf::Vector2i _exitPosition)
{
	// Set the building exit position
	m_exitPosition = _exitPosition;
}


void Buildings::SetSprite(const std::string& _spriteFileName, int _spriteOriginPosition)
{
	// Set the building sprite
	m_sprite = LoadSprite(_spriteFileName, _spriteOriginPosition);
}


void Buildings::SetIcon(const std::string& _iconFileName, int _iconOriginPosition)
{
	// Set the icon sprite
	m_icon = LoadSprite(_iconFileName, _iconOriginPosition);
}

void Buildings::SetConstructionMoneyCost(const int _moneyCost)
{
	// Set the construction cost in term of money
	m_moneyCostToBuild = _moneyCost;
}


void Buildings::SetConstructionTimeCost(const float _timeCost)
{
	// Set the construction cost in term of time necessary
	m_timeToBuild = _timeCost;
}

void Buildings::SetProductionTimeCost(const float _timeCost)
{
	// Set the production cost in term of time for each ressource produced
	m_timeToProduce = _timeCost;
}

void Buildings::SetPickupingTimeCost(const float _timeCost)
{
	// Set the construction cost in term of time necessary
	m_timeToPickUpRessources = _timeCost;
}

void Buildings::SetDepositingTimeCost(const float _timeCost)
{
	// Set the production cost in term of time for each ressource produced
	m_timeToDeposeRessources = _timeCost;
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
	if (_value - 1 >= 0 && _value - 1 < m_numberRessourceNeeded)
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
	if (_value - 1 >= 0 && _value - 1 < m_numberRessourceNeeded)
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
	if (_value - 1 >= 0 && _value - 1 < m_numberRessourceProduced)
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
	if (_value - 1 >= 0 && _value - 1 < m_numberRessourceProduced)
	{
		return m_ressourceQuantityProduced[_value - 1];
	}
	else
	{
		// ERROR LOG
		return 0;
	}
}
