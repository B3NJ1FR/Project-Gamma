#include "RessourcesManager.h"


RessourcesManager::RessourcesManager()
{
}

RessourcesManager::~RessourcesManager()
{
	m_vectorStorages.clear();
	
	for (std::vector<Ressources*>::iterator iterator = m_vectorRessources.begin(); iterator != m_vectorRessources.end(); iterator++)
	{
		if ((*iterator)->GetName() == "")
		{
			m_vectorRessources.erase(iterator);
		}
		else if (*iterator != nullptr)
		{ 
			delete *iterator;
		}
	}

	m_vectorRessources.clear();
}

RessourcesManager* RessourcesManager::GetSingleton()
{
	static RessourcesManager uniqueInstance;

	return &uniqueInstance;
}

void RessourcesManager::Initialisation()
{
	RessourcesInitialisation();
}

void RessourcesManager::RessourcesInitialisation()
{
	std::ifstream ressourcesFile("Data/Configurations/Ressources.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();

	int temporaryNumber = RESET;
	int previousID = RESET;
	int maximalNumberOfRessources = RESET;

	if (!ressourcesFile.is_open())
	{
		std::cout << "Error accessing Ressources.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!ressourcesFile.eof())
	{
		ressourcesFile >> temporaryString;

		if (temporaryString == "RESSOURCE")
		{
			int temporaryID(RESET);
			ressourcesFile >> temporaryID;

			if ((temporaryID == 0 && previousID == 0)
				|| temporaryID > previousID)
			{
				maximalNumberOfRessources = temporaryID;
				previousID = temporaryID;
			}
			else
			{
				std::cout << std::endl << std::endl << "\t\tSTEP 1 : The sprite file has an error with the ID : " << temporaryID << std::endl << std::endl << std::endl;
			}
		}
	}

	std::cout << "\n\n\n\tRessources loading ...\n\n";

	// Reset the reading cursor at the begging
	ressourcesFile.seekg(0, std::ios::beg);

	// Reading and loading of the sprites
	while (!ressourcesFile.eof())
	{
		ressourcesFile >> temporaryString;

		if (temporaryString == "RESSOURCE")
		{
			Ressources* temporaryRess = new Ressources();

			// We pickup the ressource id
			int temporaryID(RESET);
			ressourcesFile >> temporaryID;

			// We pickup the ressource name
			ressourcesFile >> temporaryString;

			// We check the name is corresponding with the enum's id stocked in the code
			if (temporaryString == Ressources::GetNameFromEnum((TypesOfRessources)temporaryID))
			{
				temporaryRess->SetName(temporaryString);
			}

			// We pickup the starting quantity
			ressourcesFile >> temporaryNumber;
			temporaryRess->SetInitialQuantityOwned(temporaryNumber);
			std::cout << temporaryID << "/" << maximalNumberOfRessources << " - " << temporaryRess->GetName() << " " << temporaryNumber << std::endl;

			ressourcesFile >> temporaryString;

			if (temporaryString == "SPRITE")
			{
				// We pickup the ressource name
				ressourcesFile >> temporaryString;
				temporaryRess->SetSprite(temporaryString);
			}

			m_vectorRessources.push_back(temporaryRess);
		}
	}

	ressourcesFile.close();

	std::cout << "\n\tRessources loaded !\n\n\n";
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

unsigned int RessourcesManager::GetResourceQuantity(std::string _ressourceName, std::string _specificBuildingName)
{
	unsigned int sumOfRessources = 0;

	if (m_vectorStorages.size() > 0)
	{
		for (std::vector<Storage*>::iterator iterator = m_vectorStorages.begin(); iterator != m_vectorStorages.end(); iterator++)
		{
			if (_specificBuildingName == std::string("")
				|| _specificBuildingName == (*iterator)->GetName())
			{
				int ressourceQuantity = (*iterator)->GetResourceQuantity(_ressourceName);

				if (ressourceQuantity >= 0)
				{
					sumOfRessources += ressourceQuantity;
				}
			}
		}
	}

	return sumOfRessources;
}

sf::Sprite RessourcesManager::GetResourceSprite(TypesOfRessources _ressourceName)
{
	// Security to avoid any array's exit
	if ((int)_ressourceName < m_vectorRessources.size())
	{
		return m_vectorRessources[(int)_ressourceName]->GetSprite();
	}
}

sf::Sprite RessourcesManager::GetResourceSprite(std::string _ressourceName)
{
	return GetResourceSprite(Ressources::GetEnumFromName(_ressourceName));
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

void RessourcesManager::ClearAllStorages()
{
	for (std::vector<Storage*>::iterator iterator = m_vectorStorages.begin(); iterator != m_vectorStorages.end(); iterator++)
	{
		// Remove the storage from the vector
		if (*iterator != nullptr)
		{
			m_vectorStorages.erase(iterator);
		}
	}
}

void RessourcesManager::CheatAddResourcesToBuilding(std::string _ressourceName, int _quantity, std::string _specificBuildingName)
{
	unsigned int sumOfRessources = 0;

	for (std::vector<Storage*>::iterator iterator = m_vectorStorages.begin(); iterator != m_vectorStorages.end(); iterator++)
	{
		if (_specificBuildingName == std::string("")
			|| _specificBuildingName == (*iterator)->GetName())
		{
			if ((*iterator)->IsResourceExistHere(_ressourceName))
			{
				(*iterator)->AddOrSubtractResource(_ressourceName, _quantity);
				break;
			}
		}
	}
}