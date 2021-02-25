#include "Storage.h"
#include "RessourcesManager.h"

Storage::Storage()
{
	m_mapOfResources.clear();
	RessourcesManager::GetSingleton()->AddNewStorage(this);
}

Storage::Storage(int _quantityOfRessources, enum TypesOfRessources _ressources, ...)
{
	int resultSum = 0;

	//déclaration de l’argument variadique
	va_list arguments;

	//démarrage de la lecture (au premier argument) 
	va_start(arguments, _quantityOfRessources);

	m_mapOfResources.clear();

	for (int i = 0; i < _quantityOfRessources; i++)
	{
		Ressources* temporaryRess = new Ressources(_ressources);
		m_mapOfResources.insert(std::pair<std::string, Ressources*>(temporaryRess->GetName(), temporaryRess));
		
		_ressources = va_arg(arguments, TypesOfRessources);
	}

	va_end(arguments);

	RessourcesManager::GetSingleton()->AddNewStorage(this);

	/*std::cout << "\tBuilding : " << m_storageName;

	for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin();
		iterator != m_mapOfResources.end();
		iterator++)
	{
		if (iterator->second != nullptr)
		{
			std::cout << "\n\t\t" << iterator->first << " : " << ((Ressources *)iterator->second)->GetQuantityOwned();
		}
	}
	std::cout << "\n\n";*/
}

Storage::~Storage()
{
	for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin(); iterator != m_mapOfResources.end(); iterator++)
	{
		if (iterator->second != nullptr)
		{
			// Remove the storage from the list
			RessourcesManager::GetSingleton()->RemoveStorage(this);

			// Delete the storage from the list
			delete iterator->second;
		}
	}
}

Ressources* Storage::operator[] (int _index)
{
	int counter = 0;

	for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin();
		iterator != m_mapOfResources.end();
		iterator++)
	{
		if (counter == _index)
		{
			return iterator->second;
		}

		counter++;
	}

	return nullptr;
}

std::ostream& operator<< (std::ostream& _ostream, const Storage& _storageToDisplay)
{
	_ostream << "\tBuilding : " << _storageToDisplay.GetName();

	if (_storageToDisplay.GetNumberOfResources() > 0)
	{
		_ostream << ' ' << _storageToDisplay.GetNumberOfResources();

		TypeMapStringResources mapOfResources = _storageToDisplay.GetMapOfResources();

		for (TypeMapStringResources::iterator iterator = mapOfResources.begin(); iterator != mapOfResources.end(); iterator++)
		{
			_ostream << '\n';
			if (iterator->second != nullptr)
			{
				_ostream << "\t\t" << iterator->first << " : " << iterator->second->GetQuantityOwned();
			}
		}
	}

	_ostream << "\n\n";

	return _ostream;
}

void Storage::AddNewResourceToStorage(std::string _ressourceName)
{
	TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

	// Verification if the new ressources doesn't already exist in the map
	if (iterator == m_mapOfResources.end())
	{
		Ressources* temporaryRess = new Ressources(_ressourceName);
		m_mapOfResources.insert(std::pair<std::string, Ressources*>(temporaryRess->GetName(), temporaryRess));
	}
}

void Storage::RemoveResourceOfStorage(std::string _ressourceName)
{
	TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

	// Verification if the new ressources doesn't already exist in the map
	if (iterator != m_mapOfResources.end())
	{
		m_mapOfResources.erase(iterator);
	}
}

void Storage::AddOrSubtractResource(std::string _ressourceName, int _quantity)
{
	TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

	if (iterator != m_mapOfResources.end())
	{
		((Ressources*)iterator->second)->AddOrSubtractQuantityOwned(_quantity);
	}
}


int Storage::GetResourceQuantity(std::string _ressourceName) 
{
	TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

	if (iterator != m_mapOfResources.end())
	{
		return ((Ressources*)iterator->second)->GetQuantityOwned();
	}

	return -1;
}

bool Storage::IsResourceExistHere(std::string _ressourceName)
{
	return (m_mapOfResources.find(_ressourceName) != m_mapOfResources.end()) ? true : false;
}

bool Storage::IsResourceExistHere(enum TypesOfRessources _ressourceName)
{
	return (m_mapOfResources.find(Ressources::GetNameFromEnum(_ressourceName)) != m_mapOfResources.end()) ? true : false;
}

bool Storage::IsStorageEmpty()
{
	return (m_mapOfResources.size() == 0) ? true : false;
}

bool Storage::TransferOfResource(Storage* _source, Storage* _destination, std::string _ressourceName, unsigned int _quantity)
{
	if (_quantity <= _source->GetResourceQuantity(_ressourceName))
	{
		_source->AddOrSubtractResource(_ressourceName, -(int)_quantity);
		_destination->AddOrSubtractResource(_ressourceName, _quantity);

		return true;
	}

	return false;
}


bool Storage::TransferOfTheWholeResource(Storage* _source, Storage* _destination, std::string _ressourceName)
{
	int quantity = _source->GetResourceQuantity(_ressourceName);

	_destination->AddOrSubtractResource(_ressourceName, quantity);
	_source->AddOrSubtractResource(_ressourceName, -quantity);

	return true;
}