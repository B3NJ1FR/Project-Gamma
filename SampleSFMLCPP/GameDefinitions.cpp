#include "GameDefinitions.h"

Game::Game(const sf::Vector2i& _screenResolution)
{
	if (m_screenReso == nullptr)
	{
		m_screenReso = new sf::Vector2i();
	}

	*m_screenReso = _screenResolution;

	m_actualGameState = TUTORIAL_MODE;

	SpritesInitialisation();
	TextsInit();
	RessourcesInitialisation();

	m_time = TimeManagement::GetSingleton();
	m_time->Initialisation(&m_charlemagneFont, *m_screenReso);

	m_tutorialWindow = new TutorialWindow(&m_charlemagneFont);
	m_workersList = new WorkersList;
	m_mainCharacter = new MainCharacter; // Vérifier le code après la remise en route de BuildManagement
	m_sellingWindow = new SellingWindow(&m_generalFont, *m_screenReso);
	m_buildingsListPlanned = new BuildingsListPlanned();
	m_purchasers = nullptr;
	m_managerBetweenWorkersAndMain = new ManagerBetweenWorkersAndMain(*m_screenReso);


	m_camera.x = -15;
	m_camera.y = -15;
	m_camera.z = RESET;

	m_scale.x = 1 / (1 - m_camera.z);
	m_scale.y = 1 / (1 - m_camera.z);

	m_isDebuggerModeActive = false;



	//m_workersList->InitialisationWorkersList();

	m_buildWindow.InitTextsBuildWindow(&m_charlemagneFont);
}


Game::~Game()
{
	std::cout << "\t--------------------------\n\n\n\n\n\t\tGame Destroyed\n\n\n\n\n\t--------------------------\n\n\n";

	if (m_tutorialWindow != nullptr)
	{
		delete m_tutorialWindow;
		m_tutorialWindow = nullptr;
	}

	if (m_spriteArray != nullptr)
	{
		for (int i = 0; i < m_maximalNumberOfSprites; i++)
		{
			delete (m_spriteArray[i].getTexture());
		}
		delete [] m_spriteArray;
	}

	if (m_ressources != nullptr)
	{
		delete [] m_ressources;
	}

	if (m_screenReso != nullptr)
	{
		delete m_screenReso;
	}

	if (m_managerBetweenWorkersAndMain != nullptr)
	{
		delete m_managerBetweenWorkersAndMain;
	}
}



void Game::SetWindowMemoryAddress(sf::RenderWindow* _window)
{
	m_window = _window;

	std::cout << "Memory Adress : " << m_window << "\n";
}

void Game::SetGeneralState(GeneralState* _state)
{
	m_generalState = _state;
}



void Game::SpritesInitialisation()
{
	std::ifstream spritesFile("Data/Configurations/Sprites.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();

	int temporaryNumber(RESET);
	int previousID(RESET);

	if (!spritesFile.is_open())
	{
		std::cout << "Error accessing Sprites.data file" << std::endl;

		exit(EXIT_FAILURE);
	}

	while (!spritesFile.eof())
	{
		spritesFile >> temporaryString;

		if (temporaryString == "SPRITE")
		{
			int temporaryID(RESET);
			spritesFile >> temporaryID;

			if ((temporaryID == 0 && previousID == 0)
				|| temporaryID > previousID)
			{
				m_maximalNumberOfSprites = temporaryID;
				previousID = temporaryID;
			}
			else
			{
				std::cout << std::endl << std::endl << "\t\tSTEP 1 : The sprite file has an error with the ID : " << temporaryID << std::endl << std::endl << std::endl;
			}
		}
	}

	// Dynamic allocation of the array sprite's
	m_maximalNumberOfSprites += 1;
	m_spriteArray = new sf::Sprite[m_maximalNumberOfSprites];

	// Reset the reading cursor at the begging
	spritesFile.seekg(0, std::ios::beg);

	// Reading and loading of the sprites
	while (!spritesFile.eof())
	{
		spritesFile >> temporaryString;

		if (temporaryString == "SPRITE")
		{
			int temporaryID(RESET);
			spritesFile >> temporaryID;

			if (temporaryID < m_maximalNumberOfSprites)
			{
				spritesFile >> temporaryNumber;
				spritesFile >> temporaryString;

				std::cout << temporaryID << " - Sprite " << temporaryString << std::endl;
				m_spriteArray[temporaryID] = LoadSprite(temporaryString, temporaryNumber);
			}
			else
			{
				std::cout << std::endl << std::endl << "\t\tSTEP 2 : The sprite file has an error with the ID : " << temporaryID << std::endl << std::endl << std::endl;
			}
		}
	}

	spritesFile.close();
}


void Game::TextsInit()
{
	//_game->generalFont.loadFromFile("Data/Fonts/Roman SD.ttf");
	//_game->generalFont.loadFromFile("Data/Fonts/CharlemagneStd-Bold.otf");
	m_generalFont.loadFromFile("Data/Fonts/arial.ttf");
	m_charlemagneFont.loadFromFile("Data/Fonts/CharlemagneStd-Bold.otf");

	// ---------------- DEBUGGER TEXTS ---------------- 
	LoadTextString(&m_debbugTexts[0], "Mouse position :", &m_generalFont, 30, sf::Color::White, sf::Vector2f(0, 85));
	LoadTextString(&m_debbugTexts[1], "", &m_generalFont, 30, sf::Color::White, sf::Vector2f(250, 85));

	LoadTextString(&m_debbugTexts[2], "Click position :", &m_generalFont, 30, sf::Color::White, sf::Vector2f(0, 115));
	LoadTextString(&m_debbugTexts[3], "", &m_generalFont, 30, sf::Color::White, sf::Vector2f(250, 115));

	LoadTextString(&m_debbugTexts[4], "Case coordinates :", &m_generalFont, 30, sf::Color::White, sf::Vector2f(0, 145));
	LoadTextString(&m_debbugTexts[5], "", &m_generalFont, 30, sf::Color::White, sf::Vector2f(250, 145));

	LoadTextString(&m_debbugTexts[6], "Collisions :", &m_generalFont, 30, sf::Color::White, sf::Vector2f(0, 175));
	LoadTextString(&m_debbugTexts[7], "", &m_generalFont, 30, sf::Color::White, sf::Vector2f(150, 175));
	LoadTextString(&m_debbugTexts[8], "Building ID :", &m_generalFont, 30, sf::Color::White, sf::Vector2f(300, 175));
	LoadTextString(&m_debbugTexts[9], "", &m_generalFont, 30, sf::Color::White, sf::Vector2f(460, 175));
	LoadTextString(&m_debbugTexts[10], "Sprite ID :", &m_generalFont, 30, sf::Color::White, sf::Vector2f(600, 175));
	LoadTextString(&m_debbugTexts[11], "", &m_generalFont, 30, sf::Color::White, sf::Vector2f(730, 175));


	// ---------------- UI TEXTS ---------------- 
	LoadTextString(&m_UITexts[0], "Sesterces :", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(130, 33));
	LoadTextString(&m_UITexts[1], "", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(305, 33));

	LoadTextString(&m_UITexts[2], "Bunchs of Grape :", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(435, 33));
	LoadTextString(&m_UITexts[3], "", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(720, 33));

	LoadTextString(&m_UITexts[4], "Grapes Must :", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(840, 33));
	LoadTextString(&m_UITexts[5], "", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(1055, 33));

	LoadTextString(&m_UITexts[6], "Grape Juice :", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(1155, 33));
	LoadTextString(&m_UITexts[7], "", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(1355, 33));

	LoadTextString(&m_UITexts[8], "Amphora of Wine :", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(1500, 33));
	LoadTextString(&m_UITexts[9], "", &m_charlemagneFont, 25, sf::Color::White, sf::Vector2f(1795, 33));

}


void Game::RessourcesInitialisation()
{
	std::ifstream ressourcesFile("Data/Configurations/Ressources.data", std::ios::in);

	std::string temporaryString;
	temporaryString.erase();

	int temporaryNumber(RESET);
	int maximalNumberOfRessources(RESET);
	int previousID(RESET);

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

	// Dynamic allocation of the array sprite's
	m_numberTypesOfRessources = maximalNumberOfRessources + 1;
	m_ressources = new Ressources[m_numberTypesOfRessources];

	// Reset the reading cursor at the begging
	ressourcesFile.seekg(0, std::ios::beg);

	std::cout << "\n\n\n\tRessources loading ...\n\n";

	// Reading and loading of the sprites
	while (!ressourcesFile.eof())
	{
		ressourcesFile >> temporaryString;

		if (temporaryString == "RESSOURCE")
		{
			// We pickup the ressource id
			int temporaryID(RESET);
			ressourcesFile >> temporaryID;

			// We pickup the ressource name
			ressourcesFile >> temporaryString;
			m_ressources[temporaryID].SetName(temporaryString);

			// We pickup the starting quantity
			ressourcesFile >> temporaryNumber;
			m_ressources[temporaryID].SetInitialQuantityOwned(temporaryNumber);
			std::cout << temporaryID << "/" << m_numberTypesOfRessources << " - " << temporaryString << " " << temporaryNumber << std::endl;


			ressourcesFile >> temporaryString;

			if (temporaryString == "SPRITE")
			{
				// We pickup the ressource name
				ressourcesFile >> temporaryString;
				m_ressources[temporaryID].SetSprite(temporaryString);
			}
		}
	}

	ressourcesFile.close();

	std::cout << "\n\tRessources loaded !\n\n\n";
}