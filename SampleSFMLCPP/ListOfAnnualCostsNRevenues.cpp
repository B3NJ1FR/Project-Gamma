#include "ListOfAnnualCostsNRevenues.h"
#include "Fonts.h"

ListOfAnnualCostsNRevenues::ListOfAnnualCostsNRevenues()
{
	m_papyrusBackground = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Costs_n_Revenues/sellingWindow_background.png", 1);

	m_font = Fonts::GetSingleton()->GetCharlemagneFont();

	m_textPapyrusTitles = new sf::Text[3];
	LoadTextString(&m_textPapyrusTitles[0], "Costs", &m_font, 35, sf::Color::Black, 1);
	LoadTextString(&m_textPapyrusTitles[1], "Revenues", &m_font, 35, sf::Color::Black, 1);
	LoadTextString(&m_textPapyrusTitles[2], "Final Revenue", &m_font, 35, sf::Color::Black, 1);

	internalState = InternalState::STATE_INIT;
}

ListOfAnnualCostsNRevenues::~ListOfAnnualCostsNRevenues()
{
	m_listOfAnnualCNRData.clear();

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
	/*AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);
	if (it == m_listOfAnnualResourcesData.end())
	{
		m_listOfAnnualResourcesData[_yearNumber] = LinkedListInitialisation();
	}
	else
	{
		std::cout << "[ListOfAnnualProductions] - Error when creating new year in Data Map because the year " << _yearNumber << " already exist";
	}*/
}

void ListOfAnnualCostsNRevenues::Update()
{
	int yearNumber = 0;

	switch (internalState)
	{
	case InternalState::STATE_INIT:

		m_stringTextYear = TransformStringToVerticalOne(std::string("Year " + std::to_string(yearNumber)));
		LoadTextString(&m_textYear, m_stringTextYear, &m_font, 45, sf::Color::Black, 1);

		internalState = InternalState::STATE_UPDATE;
		break;
	case InternalState::STATE_UPDATE:
		break;
	case InternalState::STATE_EXIT:
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

	// Display of the background
	BlitSprite(m_papyrusBackground, (float)(_screenResolution.x / 2), (float)(_screenResolution.y / 2), 0, _window);

	// Display papyrus title
	sf::Vector2f screenCenter = sf::Vector2f(_screenResolution.x / 2, _screenResolution.y / 2);
	BlitString(m_textPapyrusTitles[0], screenCenter.x - 300.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 200.0f, _window);
	BlitString(m_textPapyrusTitles[1], screenCenter.x + 300.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 200.0f, _window);
	BlitString(m_textPapyrusTitles[2], screenCenter.x - 100.0f, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 520.0f, _window);

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
