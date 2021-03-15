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
	va_start(arguments, _ressources);

	m_mapOfResources.clear();

	for (int i = 0; i < _quantityOfRessources; i++)
	{
		Ressources* temporaryRess = new Ressources(_ressources);
		m_mapOfResources.insert(std::pair<std::string, std::pair<Ressources*, ResourceData>>(temporaryRess->GetName(), std::pair<Ressources*, ResourceData>(temporaryRess, ResourceData::RESOURCE_NORMAL)));
		
		_ressources = va_arg(arguments, TypesOfRessources);
	}

	va_end(arguments);

	RessourcesManager::GetSingleton()->AddNewStorage(this);
}

Storage::~Storage()
{
	// Remove the storage from the list
	RessourcesManager::GetSingleton()->RemoveStorage(this);

	if (!m_mapOfResources.empty())
	{
		for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin(); iterator != m_mapOfResources.end(); iterator++)
		{
			if (iterator->second.first != nullptr)
			{
				// Delete the storage from the list
				delete iterator->second.first;
			}
		}
	}
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
			if (iterator->second.first != nullptr)
			{
				_ostream << "\t\t" << iterator->first << " : " << iterator->second.first->GetQuantityOwned() << '(' << iterator->second.first->GetQuantityReserved() << ") - ";

				switch (iterator->second.second)
				{
				case ResourceData::RESOURCE_NOT_DEFINED:
					_ostream << "RESOURCE_NOT_DEFINED";
					break;
				case ResourceData::RESOURCE_NORMAL:
					_ostream << "RESOURCE_NORMAL";
					break;
				case ResourceData::RESOURCE_NEEDED:
					_ostream << "RESOURCE_NEEDED";
					break;
				case ResourceData::RESOURCE_PRODUCED:
					_ostream << "RESOURCE_PRODUCED";
					break;
				case ResourceData::RESOURCE_NEEDED_N_PRODUCED:
					_ostream << "RESOURCE_NEEDED_N_PRODUCED";
					break;
				default:
					break;
				}
			}
		}
	}

	_ostream << "\n\n";

	return _ostream;
}

void Storage::AddNewResourceToStorage(std::string _ressourceName, ResourceData _data)
{
	if (m_mapOfResources.size() > 0)
	{
		TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

		// Verification if the new ressources doesn't already exist in the map
		if (iterator == m_mapOfResources.end())
		{
			Ressources* temporaryRess = new Ressources(_ressourceName);
			m_mapOfResources.insert(std::pair<std::string, std::pair<Ressources*, ResourceData>>(temporaryRess->GetName(), std::pair<Ressources*, ResourceData>(temporaryRess, _data)));
		}
	}
	else
	{
		Ressources* temporaryRess = new Ressources(_ressourceName);
		m_mapOfResources.insert(std::pair<std::string, std::pair<Ressources*, ResourceData>>(temporaryRess->GetName(), std::pair<Ressources*, ResourceData>(temporaryRess, _data)));
	}
}

void Storage::AddNewResourceToStorage(std::string _ressourceName, Ressources _ressource, ResourceData _data)
{
	if (m_mapOfResources.size() > 0)
	{
		TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

		// Verification if the new ressources doesn't already exist in the map
		if (iterator == m_mapOfResources.end())
		{
			Ressources* temporaryRess = new Ressources(_ressource);
			m_mapOfResources.insert(std::pair<std::string, std::pair<Ressources*, ResourceData>>(temporaryRess->GetName(), std::pair<Ressources*, ResourceData>(temporaryRess, _data)));
		}
	}
	else
	{
		Ressources* temporaryRess = new Ressources(_ressource);
		m_mapOfResources.insert(std::pair<std::string, std::pair<Ressources*, ResourceData>>(temporaryRess->GetName(), std::pair<Ressources*, ResourceData>(temporaryRess, _data)));
	}
}

void Storage::AddNewResourceToStorage(int _quantityOfRessources, enum TypesOfRessources _ressources, ...)
{
	int resultSum = 0;

	//déclaration de l’argument variadique
	va_list arguments;

	//démarrage de la lecture (au premier argument) 
	va_start(arguments, _ressources);

	m_mapOfResources.clear();

	for (int i = 0; i < _quantityOfRessources; i++)
	{
		TypeMapStringResources::iterator iterator = m_mapOfResources.find(Ressources::GetNameFromEnum(_ressources));

		// Verification if the new ressources doesn't already exist in the map
		if (iterator == m_mapOfResources.end())
		{
			Ressources* temporaryRess = new Ressources(_ressources);
			m_mapOfResources.insert(std::pair<std::string, std::pair<Ressources*, ResourceData>>(temporaryRess->GetName(), std::pair<Ressources*, ResourceData>(temporaryRess, ResourceData::RESOURCE_NORMAL)));
			std::cout << "new storage : " <<temporaryRess->GetQuantityReserved() << std::endl;
		}

		_ressources = va_arg(arguments, enum TypesOfRessources);
	}

	va_end(arguments);

	
}

void Storage::RemoveResourceOfStorage(std::string _ressourceName)
{
	if (m_mapOfResources.size() > 0)
	{
		TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

		// Verification if the new ressources doesn't already exist in the map
		if (iterator != m_mapOfResources.end())
		{
			m_mapOfResources.erase(iterator);
		}
	}
}

void Storage::AddOrSubtractResource(std::string _ressourceName, int _quantity)
{
	if (m_mapOfResources.size() > 0)
	{
		TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

		if (iterator != m_mapOfResources.end())
		{
			((Ressources*)iterator->second.first)->AddOrSubtractQuantityOwned(_quantity);
		}
	}
}


int Storage::GetResourceQuantity(std::string _ressourceName) 
{
	if (m_mapOfResources.size() > 0)
	{
		TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

		if (iterator != m_mapOfResources.end())
		{
			return ((Ressources*)iterator->second.first)->GetQuantityOwned();
		}

		return -1;
	}

	return -1;
}


std::vector<Ressources*> Storage::GetResourceFromData(ResourceData _ressourceData)
{
	std::vector<Ressources*> arrayToReturn;

	if (_ressourceData == ResourceData::RESOURCE_NEEDED)
	{
		for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin();
			iterator != m_mapOfResources.end();
			iterator++)
		{
			if (iterator->second.second == ResourceData::RESOURCE_NEEDED
				|| iterator->second.second == ResourceData::RESOURCE_NEEDED_N_PRODUCED)
			{
				arrayToReturn.push_back(iterator->second.first);
			}
		}
	}
	else if (_ressourceData == ResourceData::RESOURCE_PRODUCED)
	{
		for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin();
			iterator != m_mapOfResources.end();
			iterator++)
		{
			if (iterator->second.second == ResourceData::RESOURCE_PRODUCED
				|| iterator->second.second == ResourceData::RESOURCE_NEEDED_N_PRODUCED)
			{
				arrayToReturn.push_back(iterator->second.first);
			}
		}
	}
	else if (_ressourceData == ResourceData::RESOURCE_NEEDED_N_PRODUCED)
	{
		for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin();
			iterator != m_mapOfResources.end();
			iterator++)
		{
			if (iterator->second.second == ResourceData::RESOURCE_NEEDED
				|| iterator->second.second == ResourceData::RESOURCE_PRODUCED
				|| iterator->second.second == ResourceData::RESOURCE_NEEDED_N_PRODUCED)
			{
				arrayToReturn.push_back(iterator->second.first);
			}
		}
	}
	else
	{
		for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin();
			iterator != m_mapOfResources.end();
			iterator++)
		{
			if (iterator->second.second == _ressourceData)
			{
				arrayToReturn.push_back(iterator->second.first);
			}
		}
	}

	return arrayToReturn;
}


Ressources* Storage::GetResource(std::string _ressourceName)
{
	TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

	if (iterator != m_mapOfResources.end())
	{
		return iterator->second.first;
	}

	return nullptr;
}

Ressources* Storage::GetResource(int _index)
{
	int counter = 0;

	for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin();
		iterator != m_mapOfResources.end();
		iterator++)
	{
		if (counter == _index)
		{
			return iterator->second.first;
		}

		counter++;
	}

	return nullptr;
}

ResourceData Storage::GetResourceData(std::string _ressourceName)
{
	TypeMapStringResources::iterator iterator = m_mapOfResources.find(_ressourceName);

	if (iterator != m_mapOfResources.end())
	{
		return iterator->second.second;
	}

	return ResourceData::RESOURCE_NOT_DEFINED;
}

ResourceData Storage::GetResourceData(int _index)
{
	int counter = 0;

	for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin();
		iterator != m_mapOfResources.end();
		iterator++)
	{
		if (counter == _index)
		{
			return iterator->second.second;
		}

		counter++;
	}

	return ResourceData::RESOURCE_NOT_DEFINED;
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



void Storage::SavingForFile(std::ofstream* _file)
{
	// Save the storage name
	SavingStringIntoBinaryFile(_file, m_storageName);

	// Save the number of resources present in this storage
	int sizeOfMapOfStorage = m_mapOfResources.size();
	_file->write((char*)&sizeOfMapOfStorage, sizeof(int));

	for (TypeMapStringResources::iterator iterator = m_mapOfResources.begin();
		iterator != m_mapOfResources.end();
		iterator++)
	{
		// Save each resource name
		SavingStringIntoBinaryFile(_file, iterator->first);

		// Save each resource content data
		iterator->second.first->SavingForFile(_file);

		// Save each type of resource data
		_file->write((char*)&iterator->second.second, sizeof(ResourceData));
	}
}


void Storage::LoadingFromFile(std::ifstream* _file)
{
	// Save the storage name
	m_storageName = LoadingStringFromBinaryFile(_file);

	// Save the number of resources present in this storage
	int sizeOfMapOfStorage = 0;
	_file->read((char*)&sizeOfMapOfStorage, sizeof(int));

	std::string resourceName;
	ResourceData resourceData = ResourceData::RESOURCE_NORMAL;

	for (int i = 0; i < sizeOfMapOfStorage; i++)
	{
		Ressources resource;

		// Load each resource name
		resourceName.clear();
		resourceName = LoadingStringFromBinaryFile(_file);

		// Load each resource content data
		resource.LoadingFromFile(_file);

		// Reload the sprite corresponding
		// TO TEST
		resource.SetSprite(RessourcesManager::GetSingleton()->GetResourceSprite(resourceName));

		// Load each type of resource data
		resourceData = ResourceData::RESOURCE_NORMAL;
		_file->read((char*)&resourceData, sizeof(ResourceData));

		AddNewResourceToStorage(resourceName, resource, resourceData);
	}

	/*if (sizeOfMapOfStorage == m_mapOfResources.size())
	{
		std::cout << "[STORAGE] - Successfully loaded !\n";
	}*/
}