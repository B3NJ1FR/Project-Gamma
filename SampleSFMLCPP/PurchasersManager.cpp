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
	DestroyCurrentPurchaser();
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
		}
	}

	purchaserFile.close();

	std::cout << "New merchant created\n\n";
}


void PurchasersManager::Update(Stalls* _stall)
{
	if (m_isMerchantSpawned)
	{

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
				m_isMerchantSpawned = true;
				m_currentPurchaser = new Purchasers();
				m_currentPurchaser->Initialisation(_stall);
				//
			}
		}
	}
}


void PurchasersManager::AskToDestroyCurrentPurchaser()
{
	if (m_isMerchantSpawned)
	{
		DestroyCurrentPurchaser();
	}

	m_currentTimeBeforeSpawn = 0.0f;
}

void PurchasersManager::DestroyCurrentPurchaser()
{
	if (m_currentPurchaser != nullptr)
	{
		delete m_currentPurchaser;
	}
}


void PurchasersManager::SavingForFile(std::ofstream* _file)
{

}

void PurchasersManager::LoadingFromFile(std::ifstream* _file)
{

}