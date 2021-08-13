#include "ListOfAnnualCostsNRevenues.h"
#include "Fonts.h"
#include "TimeManagement.h"
#include "Money.h"

ListOfAnnualCostsNRevenues::ListOfAnnualCostsNRevenues()
{
	m_papyrusBackground = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Costs_n_Revenues/sellingWindow_background.png", 1);
	m_separationLine = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Costs_n_Revenues/separationLine.png", 1);
	m_leftArrow = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/arrow_previous.png", 1);
	m_rightArrow = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/arrow_next.png", 1);
	m_leftArrow.setScale(sf::Vector2f(0.65f, 0.65f));
	m_rightArrow.setScale(sf::Vector2f(0.65f, 0.65f));

	m_font = Fonts::GetSingleton()->GetCharlemagneFont();

	m_textPapyrusTitles = new sf::Text[3];
	LoadTextString(&m_textPapyrusTitles[0], "Costs", &m_font, 35, sf::Color::Black, 1);
	LoadTextString(&m_textPapyrusTitles[1], "Revenues", &m_font, 35, sf::Color::Black, 1);
	LoadTextString(&m_textPapyrusTitles[2], "Final Revenue", &m_font, 35, sf::Color::Black, 1);
	
	// Allocation of the text array
	m_textsCategoriesTitles = new sf::Text[(int)(EnumListOfCostsNRevenues::MAX_SIZE)];

	LoadTextString(&m_textsCategoriesTitles[(int)EnumListOfCostsNRevenues::PURCHASING_OF_WORKERS], TransformStringToCenteredOne("Purchasing of Workers"), &m_font, 20, sf::Color::Black, 5);
	LoadTextString(&m_textsCategoriesTitles[(int)EnumListOfCostsNRevenues::BUILDING_CONSTRUCTION], TransformStringToCenteredOne("Building Construction"), &m_font, 20, sf::Color::Black, 5);
	LoadTextString(&m_textsCategoriesTitles[(int)EnumListOfCostsNRevenues::SALES_OF_AMPHORAS_OF_WINE], TransformStringToCenteredOne("Sales of Amphoras of Wine"), &m_font, 20, sf::Color::Black, 5);

	m_textsCategoriesValues = new sf::Text[(int)(EnumListOfCostsNRevenues::MAX_SIZE)];

	LoadTextString(&m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::PURCHASING_OF_WORKERS], "", &m_font, 23, sf::Color::Black, 5);
	LoadTextString(&m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::BUILDING_CONSTRUCTION], "", &m_font, 23, sf::Color::Black, 5);
	LoadTextString(&m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::SALES_OF_AMPHORAS_OF_WINE], "", &m_font, 23, sf::Color::Black, 5);

	LoadTextString(&m_textArrowPrevYear, TransformStringToCenteredOne("Previous\nYear", 2), &m_font, 25, sf::Color(236, 150, 55), 1);
	LoadTextString(&m_textArrowNextYear, TransformStringToCenteredOne("Next\nYear", 1), &m_font, 25, sf::Color(236, 150, 55), 1);

	m_currentYearDisplayed = TimeManagement::GetSingleton()->GetCurrentYear();
	CreateNewYearInDataMap(m_currentYearDisplayed);
	internalState = InternalState::STATE_INIT;
}

ListOfAnnualCostsNRevenues::~ListOfAnnualCostsNRevenues()
{
	m_annualCNRDataMap.clear();

	if (m_textPapyrusTitles != nullptr)
	{
		//delete m_textPapyrusTitles;
		m_textPapyrusTitles = nullptr;
	}
}

ListOfAnnualCostsNRevenues* ListOfAnnualCostsNRevenues::GetSingleton()
{
	static ListOfAnnualCostsNRevenues uniqueInstance;
	return &uniqueInstance;
}

void ListOfAnnualCostsNRevenues::CreateNewYearInDataMap(unsigned int _yearNumber)
{
	if (_yearNumber < 0) return;

	AnnualCostsNRevenuesMapData::iterator it = m_annualCNRDataMap.find(_yearNumber);
	if (it == m_annualCNRDataMap.end())
	{
		m_annualCNRDataMap[_yearNumber] = new sAnnualCostsNRevenues;
		m_currentYearDisplayed = _yearNumber;
		std::cout << "[ListOfAnnualCostsNRevenues] - New year created " << m_currentYearDisplayed << "\n\n";
	}
	else
	{
		std::cout << "[ListOfAnnualCostsNRevenues] - Error when creating new year in Data Map because the year " << _yearNumber << " already exist";
	}
}

void ListOfAnnualCostsNRevenues::AddCNRValueToYear(EnumListOfCostsNRevenues _costOrRevenuesWanted, int _value, unsigned int _yearNumber)
{
	AnnualCostsNRevenuesMapData::iterator it = m_annualCNRDataMap.find(_yearNumber);

	if (it == m_annualCNRDataMap.end())
	{
		std::cout << "[ListOfAnnualCostsNRevenues] - Error when adding resources to year because the year : " << _yearNumber << " doesn't exist in the data map, but has now been created.";

		// We create the year in the Data Map
		CreateNewYearInDataMap(_yearNumber);
	}

	switch (_costOrRevenuesWanted)
	{
	case EnumListOfCostsNRevenues::PURCHASING_OF_WORKERS:
		m_annualCNRDataMap[_yearNumber]->m_purchasingWorkers += _value;
		break;
	case EnumListOfCostsNRevenues::BUILDING_CONSTRUCTION:
		m_annualCNRDataMap[_yearNumber]->m_buildingConstruction += _value;
		break;
	case EnumListOfCostsNRevenues::SALES_OF_AMPHORAS_OF_WINE:
		m_annualCNRDataMap[_yearNumber]->m_salesOfAmphorasOfWine += _value;
		break;
	default:
		break;
	}
}


void ListOfAnnualCostsNRevenues::Input(sf::Event _event, sf::RenderWindow& _window, const sf::Vector2i& _screenResolution)
{
	if (_event.type == sf::Event::KeyPressed
		&& _event.key.code == sf::Keyboard::Escape)
	{
		internalState = InternalState::STATE_EXIT;
	}

	if (_event.type == sf::Event::MouseButtonPressed
		&& _event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
		sf::Vector2f screenCenter = sf::Vector2f(_screenResolution.x / 2, _screenResolution.y / 2);

		if (m_isLeftArrowActived)
		{
			// Button to access to the previous year
			if (mousePosition.x > screenCenter.x - ARROW_CHANGE_YEAR_OFFSET_X - (m_leftArrow.getGlobalBounds().width / 2)
				&& mousePosition.x < screenCenter.x - ARROW_CHANGE_YEAR_OFFSET_X + (m_leftArrow.getGlobalBounds().width / 2)
				&& mousePosition.y > screenCenter.y - ARROW_CHANGE_YEAR_OFFSET_Y - (m_leftArrow.getGlobalBounds().height / 2)
				&& mousePosition.y < screenCenter.y + ARROW_CHANGE_YEAR_OFFSET_Y + (m_textArrowPrevYear.getGlobalBounds().height / 2))
			{
				if (m_currentYearDisplayed != 0) m_currentYearDisplayed -= 1;
				m_yearAsChanged = true;
			}
		}

		if (m_isRightArrowActived)
		{
			// Button to access to the next year
			if (mousePosition.x > screenCenter.x + ARROW_CHANGE_YEAR_OFFSET_X - (m_rightArrow.getGlobalBounds().width / 2)
				&& mousePosition.x < screenCenter.x + ARROW_CHANGE_YEAR_OFFSET_X + (m_rightArrow.getGlobalBounds().width / 2)
				&& mousePosition.y > screenCenter.y - ARROW_CHANGE_YEAR_OFFSET_Y - (m_rightArrow.getGlobalBounds().height / 2)
				&& mousePosition.y < screenCenter.y + ARROW_CHANGE_YEAR_OFFSET_Y + (m_textArrowNextYear.getGlobalBounds().height / 2))
			{
				if (m_currentYearDisplayed < TimeManagement::GetSingleton()->GetCurrentYear()) m_currentYearDisplayed += 1;
				m_yearAsChanged = true;
			}
		}
	}
}
void ListOfAnnualCostsNRevenues::UpdateTextsContent()
{
	if (m_annualCNRDataMap[m_currentYearDisplayed] != nullptr)
	{
		sAnnualCostsNRevenues* curCNR = m_annualCNRDataMap[m_currentYearDisplayed];

		curCNR->m_purchasingWorkers = (rand() % (200)) - 100; /* TEMPORARY / TEMPORAIRE / POUR TESTER*/
		curCNR->m_buildingConstruction = (rand() % (200)) - 100; /* TEMPORARY / TEMPORAIRE / POUR TESTER*/
		curCNR->m_salesOfAmphorasOfWine = (rand() % (200)) - 100; /* TEMPORARY / TEMPORAIRE / POUR TESTER*/

		UpdateDynamicsTexts(&m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::PURCHASING_OF_WORKERS], curCNR->m_purchasingWorkers);
		UpdateDynamicsTexts(&m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::BUILDING_CONSTRUCTION], curCNR->m_buildingConstruction);
		UpdateDynamicsTexts(&m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::SALES_OF_AMPHORAS_OF_WINE], curCNR->m_salesOfAmphorasOfWine);

		// Center the first three texts
		for (int i = 0; i < (int)EnumListOfCostsNRevenues::MAX_SIZE; i++) ChangeTextStringOrigin(&m_textsCategoriesValues[i], 4);

		// Coloration of the value depending if they are under or higher of 0
		sf::Color green = sf::Color(95, 210, 95, 255);
		sf::Color red = sf::Color(215, 77, 77, 255);
		ColorStringAccordingToItsValue(&m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::PURCHASING_OF_WORKERS], curCNR->m_purchasingWorkers, red, green);
		ColorStringAccordingToItsValue(&m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::BUILDING_CONSTRUCTION], curCNR->m_buildingConstruction, red, green);
		ColorStringAccordingToItsValue(&m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::SALES_OF_AMPHORAS_OF_WINE], curCNR->m_salesOfAmphorasOfWine, red, green);
	}
}
void ListOfAnnualCostsNRevenues::Update(VillaManagementStateMachine* _internalStateMachine)
{
	switch (internalState)
	{
	case InternalState::STATE_INIT:
		m_currentYearDisplayed = TimeManagement::GetSingleton()->GetCurrentYear();

		if (m_currentYearDisplayed == 0) m_isLeftArrowActived = false;
		else m_isLeftArrowActived = true;

		if (m_currentYearDisplayed == TimeManagement::GetSingleton()->GetCurrentYear()) m_isRightArrowActived = false;
		else m_isRightArrowActived = true;

		m_stringTextYear = TransformStringToVerticalOne(std::string("Year " + std::to_string(m_currentYearDisplayed)));
		LoadTextString(&m_textYear, m_stringTextYear, &m_font, 45, sf::Color::Black, 1);

		UpdateTextsContent();

		internalState = InternalState::STATE_UPDATE;

		break;
	case InternalState::STATE_UPDATE:
		if (m_yearAsChanged)
		{
			if (m_currentYearDisplayed == 0) m_isLeftArrowActived = false;
			else m_isLeftArrowActived = true;

			if (m_currentYearDisplayed == TimeManagement::GetSingleton()->GetCurrentYear()) m_isRightArrowActived = false;
			else m_isRightArrowActived = true;

			m_stringTextYear = TransformStringToVerticalOne(std::string("Year " + std::to_string(m_currentYearDisplayed)));
			LoadTextString(&m_textYear, m_stringTextYear, &m_font, 45, sf::Color::Black, 1);

			m_yearAsChanged = false;
			
			UpdateTextsContent();
		}
		break;
	case InternalState::STATE_EXIT:
		internalState = InternalState::STATE_INIT;
		*_internalStateMachine = VillaManagementStateMachine::NORMAL_STATE;
		break;
		// Desallocation du tableau de texts
		if (m_textPapyrusTitles != nullptr)
		{
			//delete m_textPapyrusTitles;
			m_textPapyrusTitles = nullptr;
		}
		break;
	case InternalState::STATE_WAITING:
		break;
	default:
		break;
	}
}

void ListOfAnnualCostsNRevenues::Display(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution)
{
	if (internalState != InternalState::STATE_UPDATE) return;

	sf::Vector2f screenCenter = sf::Vector2f(_screenResolution.x / 2, _screenResolution.y / 2);

	// Display of the background
	BlitSprite(m_papyrusBackground, screenCenter.x, screenCenter.y, 0, _window);

	// Display of the arrows to change the current year selected
	if (m_isLeftArrowActived)
	{
		BlitSprite(m_leftArrow, screenCenter.x - ARROW_CHANGE_YEAR_OFFSET_X, screenCenter.y + ARROW_CHANGE_YEAR_OFFSET_Y, _window);
		BlitString(m_textArrowPrevYear, screenCenter.x - ARROW_CHANGE_YEAR_OFFSET_X, screenCenter.y - ARROW_CHANGE_YEAR_OFFSET_Y, _window);
	}

	if (m_isRightArrowActived)
	{
		BlitSprite(m_rightArrow, screenCenter.x + ARROW_CHANGE_YEAR_OFFSET_X, screenCenter.y + ARROW_CHANGE_YEAR_OFFSET_Y, _window);
		BlitString(m_textArrowNextYear, screenCenter.x + ARROW_CHANGE_YEAR_OFFSET_X, screenCenter.y - ARROW_CHANGE_YEAR_OFFSET_Y, _window);
	}

	// Display papyrus title
	BlitString(m_textPapyrusTitles[0], screenCenter.x - 300.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 200.0f, _window);
	BlitString(m_textPapyrusTitles[1], screenCenter.x + 285.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 200.0f, _window);
	BlitString(m_textPapyrusTitles[2], screenCenter.x - 100.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 520.0f, _window);

	// Display the money at the end of the line
	sf::Sprite spriteSesterce = Money::GetSingleton()->GetSprite();
	spriteSesterce.setScale(sf::Vector2f(0.85f, 0.85f));
	BlitSprite(spriteSesterce, screenCenter.x + 425.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 525.0f, _window);

	// Separation Line
	BlitSprite(m_separationLine, screenCenter.x + 15.0f, screenCenter.y - 20.0f, _window);

	// Categories of costs & revenues Titles
	BlitString(m_textsCategoriesTitles[(int)EnumListOfCostsNRevenues::PURCHASING_OF_WORKERS], screenCenter.x - 475.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 300.0f, _window);
	BlitString(m_textsCategoriesTitles[(int)EnumListOfCostsNRevenues::BUILDING_CONSTRUCTION], screenCenter.x - 475.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 375.0f, _window);
	BlitString(m_textsCategoriesTitles[(int)EnumListOfCostsNRevenues::SALES_OF_AMPHORAS_OF_WINE], screenCenter.x + 40.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 300.0f, _window);

	// Categories of costs & revenues Titles
	BlitString(m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::PURCHASING_OF_WORKERS], screenCenter.x + 5.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 300.0f, _window);
	BlitString(m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::BUILDING_CONSTRUCTION], screenCenter.x + 5.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 373.0f, _window);
	BlitString(m_textsCategoriesValues[(int)EnumListOfCostsNRevenues::SALES_OF_AMPHORAS_OF_WINE], screenCenter.x + 490.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 300.0f, _window);

	// Display of the year number on the papyrus border
	BlitString(m_textYear, screenCenter.x - 550.0f, screenCenter.y, _window);
	BlitString(m_textYear, screenCenter.x + 555.0f, screenCenter.y, _window);
}


void ListOfAnnualCostsNRevenues::SavingDataForFile(std::ofstream* _file)
{

}

void ListOfAnnualCostsNRevenues::LoadingDataFromFile(std::ifstream* _file)
{

}
