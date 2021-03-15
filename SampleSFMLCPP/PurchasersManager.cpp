#include "PurchasersManager.h"
#include "TimeManagement.h"
#include "RessourcesManager.h"


PurchasersManager::PurchasersManager()
{
	m_minRangeTimeForSpawn = RESET;
	m_maxRangeTimeForSpawn = RESET;
	m_currentTimeBeforeSpawn = 0.0f;

	m_isMerchantSpawned = false;
}

PurchasersManager::~PurchasersManager()
{
	std::cout << "Clear the purchaser Manager\n";
	RemoveCurrentPurchaser();
}

PurchasersManager* PurchasersManager::GetSingleton()
{
	static PurchasersManager uniqueInstance;
	return &uniqueInstance;
}

void PurchasersManager::Initialisation()
{
	std::string temporaryString;
	temporaryString.erase();

	std::ifstream purchaserFile("Data/Configurations/Purchasers.data", std::ios::in);

	if (!purchaserFile.is_open())
	{
		std::cout << "Error accessing Purchasers.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!purchaserFile.eof())
	{
		purchaserFile >> temporaryString;

		if (temporaryString == "SPAWN_TIME")
		{
			purchaserFile >> m_minRangeTimeForSpawn >> m_maxRangeTimeForSpawn;
			std::cout << "Spawning time range : " << m_minRangeTimeForSpawn << ' ' << m_maxRangeTimeForSpawn << std::endl;
		}
	}

	purchaserFile.close();

	std::cout << "Initialisation Purchaser Manager\n\n";
}


void PurchasersManager::Update(BuildingManagement* _builds, Stalls* _stall)
{
	if (m_isMerchantSpawned)
	{
		if (m_currentPurchaser != nullptr) m_currentPurchaser->UpdateLife(_builds);
	}
	else
	{
		if (m_currentTimeBeforeSpawn <= 0.0f)
		{
			// Create randomly a new time before new merchant
			m_currentTimeBeforeSpawn = rand() % (m_maxRangeTimeForSpawn - m_minRangeTimeForSpawn + 1) + m_minRangeTimeForSpawn;
		}

		// Waiting, and spawning of a new purchaser
		if (m_currentTimeBeforeSpawn >= 0.0f)
		{
			m_currentTimeBeforeSpawn -= TimeManagement::GetSingleton()->GetFrameTime();

			if (m_currentTimeBeforeSpawn <= 0.0f)
			{
				std::cout << "New purchaser spawned !\n\n";
				m_isMerchantSpawned = true;
				m_currentPurchaser = new Purchasers();
				m_currentPurchaser->Initialisation(_stall);
				m_currentTimeBeforeSpawn = 0.0f;
			}
		}
	}
}


void PurchasersManager::AskToRemoveCurrentPurchaser()
{
	if (m_isMerchantSpawned)
	{
		RemoveCurrentPurchaser();
	}

	m_currentTimeBeforeSpawn = 0.0f;
}

void PurchasersManager::RemoveCurrentPurchaser()
{
	if (m_currentPurchaser != nullptr)
	{
		m_currentPurchaser = nullptr;
	}

	m_isMerchantSpawned = false;
	m_currentTimeBeforeSpawn = 0.0f;
}


void PurchasersManager::SavingForFile(std::ofstream* _file)
{
	// Saving the current purchaser
	Purchasers* purchaser = m_currentPurchaser;
	_file->write((char*)&purchaser, sizeof(Purchasers*));

	if (purchaser != nullptr)
	{
		purchaser->SavingForFile(_file);
	}

	_file->write((char*)&m_minRangeTimeForSpawn, sizeof(int));
	_file->write((char*)&m_maxRangeTimeForSpawn, sizeof(int));
	_file->write((char*)&m_currentTimeBeforeSpawn, sizeof(float));

	_file->write((char*)&m_isMerchantSpawned, sizeof(bool));
}

void PurchasersManager::LoadingFromFile(std::ifstream* _file, Stalls* _stall)
{
	// Loading the current purchaser
	Purchasers* purchaser = nullptr;
	_file->read((char*)&purchaser, sizeof(Purchasers*));

	if (purchaser != nullptr)
	{
		m_currentPurchaser = new Purchasers();
		m_currentPurchaser->Initialisation(_stall);
		m_currentPurchaser->LoadingFromFile(_file);
	}

	_file->read((char*)&m_minRangeTimeForSpawn, sizeof(int));
	_file->read((char*)&m_maxRangeTimeForSpawn, sizeof(int));
	_file->read((char*)&m_currentTimeBeforeSpawn, sizeof(float));

	_file->read((char*)&m_isMerchantSpawned, sizeof(bool));
}