#include "Ressources.h"



Ressources::Ressources() 
{
	m_quantityReserved = RESET;
	m_quantityOwned = RESET;
	m_name = "";
}


Ressources::Ressources(std::string _name)
{
	m_quantityReserved = RESET;
	m_quantityOwned = RESET;
	m_name = _name;
}


Ressources::Ressources(enum TypesOfRessources _name)
{
	m_quantityOwned = RESET;
	m_name = GetNameFromEnum(_name);
}

Ressources::Ressources(const Ressources& _ressourceToCopy)
{
	m_quantityOwned = _ressourceToCopy.GetQuantityOwned();
	m_quantityReserved = _ressourceToCopy.GetQuantityReserved();
	m_name = _ressourceToCopy.GetName();
	m_sprite = _ressourceToCopy.GetSprite();
}
Ressources::~Ressources()
{

}

void Ressources::SetSprite(sf::Sprite _spriteToCopy)
{
	sf::Texture* temporaryTexture = new sf::Texture(*_spriteToCopy.getTexture());
	m_sprite.setTexture(*temporaryTexture);
}

void Ressources::AddOrSubtractQuantityOwned(int _quantityToAdd)
{
	if (m_quantityOwned + _quantityToAdd >= 0)
	{
		m_quantityOwned += _quantityToAdd;
	}
}
void Ressources::AddOrSubtractQuantityReserved(int _quantityToAdd)
{
	if (m_quantityReserved + _quantityToAdd >= 0)
	{
		m_quantityReserved += _quantityToAdd;
	}
}


std::string Ressources::GetNameFromEnum(enum TypesOfRessources _enumName)
{
	switch (_enumName)
	{
	case GRAPE_VINE:
		return std::string("GRAPE_VINE");
		break;
	case BUNCH_OF_GRAPE:
		return std::string("BUNCH_OF_GRAPE");
		break;
	case GRAPES_MUST:
		return std::string("GRAPES_MUST");
		break;
	case GRAPE_JUICE:
		return std::string("GRAPE_JUICE");
		break;
	case PURE_GRAPE_JUICE:
		return std::string("PURE_GRAPE_JUICE");
		break;
	case GRAPE_MARC:
		return std::string("GRAPE_MARC");
		break;
	case DOLIUM:
		return std::string("DOLIUM");
		break;
	case AMPHORAS:
		return std::string("AMPHORAS");
		break;
	case AMPHORA_OF_WINE:
		return std::string("AMPHORA_OF_WINE");
		break;
	case TOOLS:
		return std::string("TOOLS");
		break;
	case WICKER_BASKET:
		return std::string("WICKER_BASKET");
		break;
	default:
		return std::string("RESSOURCE_NOT_DEFINED");
		break;
	}
}

enum TypesOfRessources Ressources::GetEnumFromName(std::string _enumName)
{
	if (_enumName == "GRAPE_VINE")
	{
		return GRAPE_VINE;
	}
	else if (_enumName == "BUNCH_OF_GRAPE")
	{
		return BUNCH_OF_GRAPE;
	}
	else if (_enumName == "GRAPES_MUST")
	{
		return GRAPES_MUST;
	}
	else if (_enumName == "GRAPE_JUICE")
	{
		return GRAPE_JUICE;
	}
	else if (_enumName == "PURE_GRAPE_JUICE")
	{
		return PURE_GRAPE_JUICE;
	}
	else if (_enumName == "GRAPE_MARC")
	{
		return GRAPE_MARC;
	}
	else if (_enumName == "DOLIUM")
	{
		return DOLIUM;
	}
	else if (_enumName == "AMPHORAS")
	{
		return AMPHORAS;
	}
	else if (_enumName == "AMPHORA_OF_WINE")
	{
		return AMPHORA_OF_WINE;
	}
	else if (_enumName == "TOOLS")
	{
		return TOOLS;
	}
	else if (_enumName == "WICKER_BASKET")
	{
		return WICKER_BASKET;
	}
	else
	{
		return RESSOURCE_NOT_DEFINED;
	}
}


void Ressources::TransferFromOwnedToReserved(unsigned int _quantity)
{
	if (m_quantityOwned - _quantity >= 0)
	{
		m_quantityOwned -= _quantity;
		m_quantityReserved += _quantity;
	}
}

void Ressources::TransferFromReservedToQuantity(unsigned int _quantity)
{
	if (m_quantityReserved - _quantity >= 0)
	{
		m_quantityReserved -= _quantity;
		m_quantityOwned += _quantity;
	}
}


void Ressources::SavingForFile(std::ofstream* _file)
{
	// Save the resource name
	SavingStringIntoBinaryFile(_file, m_name);
	_file->write((char*)&m_quantityOwned, sizeof(int));
	_file->write((char*)&m_quantityReserved, sizeof(int));
}

void Ressources::LoadingFromFile(std::ifstream* _file)
{
	// Save the resource name
	m_name = LoadingStringFromBinaryFile(_file);
	_file->read((char*)&m_quantityOwned, sizeof(int));
	_file->read((char*)&m_quantityReserved, sizeof(int));
}