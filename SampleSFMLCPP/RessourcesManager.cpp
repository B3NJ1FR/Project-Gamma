#include "RessourcesManager.h"


RessourcesManager::RessourcesManager()
{

}

RessourcesManager::~RessourcesManager()
{
	m_vectorStorages.clear();
}

RessourcesManager* RessourcesManager::GetSingleton()
{
	static RessourcesManager uniqueInstance;

	return &uniqueInstance;
}


void RessourcesManager::AddNewStorage(Storage* _newStorage)
{
	if (_newStorage != nullptr)
	{
		m_vectorStorages.push_back(_newStorage);
	}
}

void RessourcesManager::RemoveStorage(Storage* _oldStorage)
{
	for (std::vector<Storage*>::iterator iterator = m_vectorStorages.begin(); iterator != m_vectorStorages.end(); iterator++)
	{
		// Remove the storage from the vector
		if (*iterator == _oldStorage)
		{
			m_vectorStorages.erase(iterator);
			break;
		}
	}
}

unsigned int RessourcesManager::GetResourceQuantity(std::string _ressourceName)
{
	unsigned int sumOfRessources = 0;

	for (std::vector<Storage*>::iterator iterator = m_vectorStorages.begin(); iterator != m_vectorStorages.end(); iterator++)
	{
		int ressourceQuantity = (*iterator)->GetResourceQuantity(_ressourceName);

		if (ressourceQuantity >= 0)
		{
			sumOfRessources += ressourceQuantity;
		}
	}

	return sumOfRessources;
}


void RessourcesManager::DisplayAllStoragesContent()
{
	unsigned int sumOfRessources = 0;

	for (std::vector<Storage*>::iterator iterator = m_vectorStorages.begin(); iterator != m_vectorStorages.end(); iterator++)
	{
		std::cout << *(*iterator);
	}

	std::cout << std::endl << std::endl;
}

void RessourcesManager::DisplayBuildingsStoragesContent()
{
	unsigned int sumOfRessources = 0;

	for (std::vector<Storage*>::iterator iterator = m_vectorStorages.begin(); iterator != m_vectorStorages.end(); iterator++)
	{
		if ((*iterator)->GetName() != "Worker")
		{
			std::cout << *(*iterator);
		}
	}

	std::cout << std::endl << std::endl;
}

void RessourcesManager::DisplayWorkersStoragesContent()
{
	unsigned int sumOfRessources = 0;

	for (std::vector<Storage*>::iterator iterator = m_vectorStorages.begin(); iterator != m_vectorStorages.end(); iterator++)
	{
		if ((*iterator)->GetName() == "Worker")
		{
			std::cout << *(*iterator);
		}
	}

	std::cout << std::endl << std::endl;
}