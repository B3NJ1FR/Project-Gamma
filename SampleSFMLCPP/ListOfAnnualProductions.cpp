#include "ListOfAnnualProductions.h"
#include "RessourcesManager.h"
#include "Money.h"
#include "Fonts.h"

ListOfAnnualProductions::ListOfAnnualProductions()
{
	m_listOfAnnualResourcesData.clear();

	m_time = TimeManagement::GetSingleton();

	m_papyrusBackground = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Production_Summary/sellingWindow_background.png", 1);
	m_greenCheck = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Production_Summary/greenCheck.png", 1);
	m_redCross = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Production_Summary/redCross.png", 1);
	m_leftArrow = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/arrow_previous.png", 1);
	m_rightArrow = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/arrow_next.png", 1);
	m_greenCheck.setScale(sf::Vector2f(0.4f, 0.4f));
	m_redCross.setScale(sf::Vector2f(0.4f, 0.4f));
	m_leftArrow.setScale(sf::Vector2f(0.65f, 0.65f));
	m_rightArrow.setScale(sf::Vector2f(0.65f, 0.65f));

	m_font = Fonts::GetSingleton()->GetCharlemagneFont();
	LoadTextString(&m_textPapyrusTitle, "Production Summary", &m_font, 35, sf::Color::Black, 1);

	// Allocation of the text array
	m_textsCategoriesTitles = new sf::Text[5];

	LoadTextString(&m_textsCategoriesTitles[0], TransformStringToCenteredOne("Number\nof Buildings"), &m_font, 15, sf::Color::Black, 1);
	LoadTextString(&m_textsCategoriesTitles[1], TransformStringToCenteredOne("Quantity\nProduced"), &m_font, 15, sf::Color::Black, 1);
	LoadTextString(&m_textsCategoriesTitles[2], TransformStringToCenteredOne("Comparison\nwith last year"), &m_font, 15, sf::Color::Black, 1);
	LoadTextString(&m_textsCategoriesTitles[3], TransformStringToCenteredOne("Can be\nsold", 1), &m_font, 15, sf::Color::Black, 1);
	LoadTextString(&m_textsCategoriesTitles[4], TransformStringToCenteredOne("Previous merchant\nPrice"), &m_font, 15, sf::Color::Black, 1);
	LoadTextString(&m_textArrowPrevYear, TransformStringToCenteredOne("Previous\nYear", 2), &m_font, 25, sf::Color(236, 150, 55), 1);
	LoadTextString(&m_textArrowNextYear, TransformStringToCenteredOne("Next\nYear", 1), &m_font, 25, sf::Color(236, 150, 55), 1);

	m_textsDataMaxSize = RESET;

	m_isLeftArrowActived = true;
	m_isRightArrowActived = true;
	m_yearAsChanged = false;

	m_currentYearDisplayed = TimeManagement::GetSingleton()->GetCurrentYear();

	// TESTS
	CreateNewYearInDataMap(m_currentYearDisplayed);
	/*CreateNewYearInDataMap(1);
	CreateNewYearInDataMap(2);
	CreateNewYearInDataMap(3);
	CreateNewYearInDataMap(4);
	CreateNewYearInDataMap(5);
	CreateNewYearInDataMap(6);
	CreateNewYearInDataMap(7);*/

	AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, 0);
	AddResourceToYear(TypesOfRessources::GRAPES_MUST, 0);
	AddResourceToYear(TypesOfRessources::GRAPE_JUICE, 0);
	AddResourceToYear(TypesOfRessources::GRAPE_MARC, 0);
	AddResourceToYear(TypesOfRessources::DOLIUM, 0);
	AddResourceToYear(TypesOfRessources::AMPHORA_OF_WINE, 0);

	/*AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, 1);
	AddResourceToYear(TypesOfRessources::GRAPES_MUST, 1);
	AddResourceToYear(TypesOfRessources::GRAPE_JUICE, 1);
	AddResourceToYear(TypesOfRessources::GRAPE_MARC, 1);
	AddResourceToYear(TypesOfRessources::DOLIUM, 1);
	AddResourceToYear(TypesOfRessources::AMPHORA_OF_WINE, 1);

	AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, 2);
	AddResourceToYear(TypesOfRessources::GRAPES_MUST, 2);
	AddResourceToYear(TypesOfRessources::GRAPE_JUICE, 2);
	AddResourceToYear(TypesOfRessources::GRAPE_MARC, 2);
	AddResourceToYear(TypesOfRessources::DOLIUM, 2);
	AddResourceToYear(TypesOfRessources::AMPHORA_OF_WINE, 2);

	AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, 3);
	AddResourceToYear(TypesOfRessources::GRAPES_MUST, 3);
	AddResourceToYear(TypesOfRessources::GRAPE_JUICE, 3);
	AddResourceToYear(TypesOfRessources::GRAPE_MARC, 3);
	AddResourceToYear(TypesOfRessources::DOLIUM, 3);
	AddResourceToYear(TypesOfRessources::AMPHORA_OF_WINE, 3);

	AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, 4);
	AddResourceToYear(TypesOfRessources::GRAPES_MUST, 4);
	AddResourceToYear(TypesOfRessources::GRAPE_JUICE, 4);
	AddResourceToYear(TypesOfRessources::GRAPE_MARC, 4);
	AddResourceToYear(TypesOfRessources::DOLIUM, 4);
	AddResourceToYear(TypesOfRessources::AMPHORA_OF_WINE, 4);

	AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, 5);
	AddResourceToYear(TypesOfRessources::GRAPES_MUST, 5);
	AddResourceToYear(TypesOfRessources::GRAPE_JUICE, 5);
	AddResourceToYear(TypesOfRessources::GRAPE_MARC, 5);
	AddResourceToYear(TypesOfRessources::DOLIUM, 5);
	AddResourceToYear(TypesOfRessources::AMPHORA_OF_WINE, 5);

	AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, 6);
	AddResourceToYear(TypesOfRessources::GRAPES_MUST, 6);
	AddResourceToYear(TypesOfRessources::GRAPE_JUICE, 6);
	AddResourceToYear(TypesOfRessources::GRAPE_MARC, 6);
	AddResourceToYear(TypesOfRessources::DOLIUM, 6);
	AddResourceToYear(TypesOfRessources::AMPHORA_OF_WINE, 6);

	AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, 7);
	AddResourceToYear(TypesOfRessources::GRAPES_MUST, 7);
	AddResourceToYear(TypesOfRessources::GRAPE_JUICE, 7);
	AddResourceToYear(TypesOfRessources::GRAPE_MARC, 7);
	AddResourceToYear(TypesOfRessources::DOLIUM, 7);
	AddResourceToYear(TypesOfRessources::AMPHORA_OF_WINE, 7);*/

	internalState = InternalState::STATE_INIT;
}

ListOfAnnualProductions::~ListOfAnnualProductions()
{
	m_listOfAnnualResourcesData.clear();
}

ListOfAnnualProductions* ListOfAnnualProductions::GetSingleton()
{
	static ListOfAnnualProductions uniqueInstance;
	return &uniqueInstance;
}


void ListOfAnnualProductions::CreateNewYearInDataMap(unsigned int _yearNumber)
{
	if (_yearNumber < 0) return;

	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);
	if (it == m_listOfAnnualResourcesData.end())
	{
		LinkedListClass::sLinkedList* list = LinkedListInitialisation();
		m_listOfAnnualResourcesData[_yearNumber] = list;
		m_currentYearDisplayed = _yearNumber;
		std::cout << "[ListOfAnnualProductions] - New year created " << m_currentYearDisplayed << "\n\n";

		AddResourceToYear(TypesOfRessources::BUNCH_OF_GRAPE, m_currentYearDisplayed);
		AddResourceToYear(TypesOfRessources::GRAPES_MUST, m_currentYearDisplayed);
		AddResourceToYear(TypesOfRessources::GRAPE_JUICE, m_currentYearDisplayed);
		AddResourceToYear(TypesOfRessources::GRAPE_MARC, m_currentYearDisplayed);
	}
	else
	{
		std::cout << "[ListOfAnnualProductions] - Error when creating new year in Data Map because the year " << _yearNumber << " already exist";
	}
}

void ListOfAnnualProductions::AddResourceToYear(TypesOfRessources _resource, unsigned int _yearNumber)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it == m_listOfAnnualResourcesData.end())
	{
		std::cout << "[ListOfAnnualProductions] - Error when adding resources to year because the year : " << _yearNumber << " doesn't exist in the data map, but has now been created.";

		// We create the year in the Data Map
		CreateNewYearInDataMap(_yearNumber);
	}

	// We verify that the resource doesn't already exist in the list
	if (!IsResourceExistInLinkedList(m_listOfAnnualResourcesData[_yearNumber], _resource))
	{
		LinkedListClass::sElement* newResource = new LinkedListClass::sElement;
		newResource->data = new sAnnualResourceData;

		((sAnnualResourceData*)newResource->data)->m_resource = _resource;

		((sAnnualResourceData*)newResource->data)->m_numberOfBuilding = 0;

		((sAnnualResourceData*)newResource->data)->m_quantityProduced = 0;
		((sAnnualResourceData*)newResource->data)->m_comparisonWithLastYear = 0;

		((sAnnualResourceData*)newResource->data)->m_isCanBeSold = false;
		((sAnnualResourceData*)newResource->data)->m_previousMerchantPrice = 0;

		newResource->status = ELEMENT_ACTIVE;

		// Add this new resource at the end of the list of resources produced this year
		AddElementToLinkedList(m_listOfAnnualResourcesData[_yearNumber], newResource, -1);
	}
}

void ListOfAnnualProductions::AddResourceDataToYear(sAnnualResourceData _resourceData, unsigned int _yearNumber)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resourceData.m_resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_numberOfBuilding = _resourceData.m_numberOfBuilding;

						((sAnnualResourceData*)currentElement->data)->m_quantityProduced = _resourceData.m_quantityProduced;
						((sAnnualResourceData*)currentElement->data)->m_comparisonWithLastYear = _resourceData.m_comparisonWithLastYear;

						((sAnnualResourceData*)currentElement->data)->m_isCanBeSold = _resourceData.m_isCanBeSold;
						((sAnnualResourceData*)currentElement->data)->m_previousMerchantPrice = _resourceData.m_previousMerchantPrice;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourceNumberOfBuilding(unsigned int _yearNumber, TypesOfRessources _resource, int _numberOfBuilding)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_numberOfBuilding = _numberOfBuilding;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourceQuantityProduced(unsigned int _yearNumber, TypesOfRessources _resource, int _quantityProduced)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_quantityProduced = _quantityProduced;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourceComparisonWithLastYear(unsigned int _yearNumber, TypesOfRessources _resource, int _comparisonWithLastYear)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_comparisonWithLastYear = _comparisonWithLastYear;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourceIsCanBeSold(unsigned int _yearNumber, TypesOfRessources _resource, bool _isCanBeSold)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_isCanBeSold = _isCanBeSold;
					}
				}
			}
		}
	}
}

void ListOfAnnualProductions::SetResourcePreviousMerchantPrice(unsigned int _yearNumber, TypesOfRessources _resource, int _previousMerchantPrice)
{
	AnnualResourcesProducedMapData::iterator it = m_listOfAnnualResourcesData.find(_yearNumber);

	if (it != m_listOfAnnualResourcesData.end())
	{
		if (m_listOfAnnualResourcesData[_yearNumber] != nullptr)
		{
			if (m_listOfAnnualResourcesData[_yearNumber]->first != nullptr)
			{
				for (LinkedListClass::sElement* currentElement = m_listOfAnnualResourcesData[_yearNumber]->first; currentElement != nullptr; currentElement = currentElement->next)
				{
					if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource)
					{
						((sAnnualResourceData*)currentElement->data)->m_previousMerchantPrice = _previousMerchantPrice;
					}
				}
			}
		}
	}
}


bool ListOfAnnualProductions::IsResourceExistInLinkedList(LinkedListClass::sLinkedList *_list, TypesOfRessources _resource)
{
	if (_list != nullptr)
	{
		if (_list->first != nullptr)
		{
			for (LinkedListClass::sElement* currentElement = _list->first; currentElement != nullptr; currentElement = currentElement->next)
			{
				if (((sAnnualResourceData*)currentElement->data)->m_resource == _resource) return true;
			}
		}
	}

	return false;
}

void ListOfAnnualProductions::ClearTexts()
{
	if (m_textsData != nullptr) delete m_textsData;
}

void ListOfAnnualProductions::UpdateTextsContent()
{
	// If the current size is lower than which needed, we reallocate the array (avoid to allocate the array every time)
	if (m_textsDataMaxSize < m_listOfAnnualResourcesData[m_currentYearDisplayed]->size)
	{
		m_textsDataMaxSize = m_listOfAnnualResourcesData[m_currentYearDisplayed]->size;

		// Desallocation
		ClearTexts();

		// Allocation of the text array
		m_textsData = new sf::Text * [m_textsDataMaxSize];

		for (int i = 0; i < m_textsDataMaxSize; i++)
		{
			m_textsData[i] = new sf::Text[4];
		}
	}

	LinkedListClass::sElement* currentElement;
	int counter;

	for (currentElement = m_listOfAnnualResourcesData[m_currentYearDisplayed]->first, counter = 0;
		currentElement != nullptr;
		currentElement = currentElement->next, counter++)
	{
		sAnnualResourceData* curResource = ((sAnnualResourceData*)currentElement->data);

		for (int i = 0; i < 4; i++) LoadTextString(&m_textsData[counter][i], "", &m_font, 25, sf::Color::Black);

		curResource->m_comparisonWithLastYear = (rand() % (200)) - 100; // TEMPORARY / TEMPORAIRE / POUR TESTER
		curResource->m_isCanBeSold = rand() % 2; // TEMPORARY / TEMPORAIRE / POUR TESTER
		curResource->m_previousMerchantPrice = 1;

		for (int i = 0; i < counter; i++) curResource->m_previousMerchantPrice *= 10;

		UpdateDynamicsTexts(&m_textsData[counter][0], curResource->m_numberOfBuilding);
		UpdateDynamicsTexts(&m_textsData[counter][1], curResource->m_quantityProduced);
		UpdateDynamicsTexts(&m_textsData[counter][2], curResource->m_comparisonWithLastYear);
		UpdateDynamicsTexts(&m_textsData[counter][3], curResource->m_previousMerchantPrice);

		// Center the first three texts
		for (int i = 0; i < 3; i++) ChangeTextStringOrigin(&m_textsData[counter][i], 1);

		ChangeTextStringOrigin(&m_textsData[counter][3], 4);

		ColorStringAccordingToItsValue(&m_textsData[counter][2], curResource->m_comparisonWithLastYear, sf::Color(215, 77, 77, 255), sf::Color(95, 210, 95, 255));
	}
}

void ListOfAnnualProductions::Input(sf::Event _event, sf::RenderWindow& _window, const sf::Vector2i& _screenResolution)
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

void ListOfAnnualProductions::Update(VillaManagementStateMachine* _internalStateMachine)
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

		if (m_listOfAnnualResourcesData[m_currentYearDisplayed] != nullptr)
		{
			if (m_listOfAnnualResourcesData[m_currentYearDisplayed]->first != nullptr)
			{
				UpdateTextsContent();
			}
		}
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

			if (m_listOfAnnualResourcesData[m_currentYearDisplayed] != nullptr)
			{
				if (m_listOfAnnualResourcesData[m_currentYearDisplayed]->first != nullptr)
				{
					UpdateTextsContent();
				}
			}
		}

		break;
	case InternalState::STATE_EXIT:
		// Désallouer le tableau de texts
		internalState = InternalState::STATE_INIT;
		*_internalStateMachine = VillaManagementStateMachine::NORMAL_STATE;
		break;

	case InternalState::STATE_WAITING:
		break;

	default:
		break;
	}
}

void ListOfAnnualProductions::Display(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution)
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
	BlitString(m_textPapyrusTitle, screenCenter.x, screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 200.0f, _window);

	// Display categories titles
	float positionHeight = screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + 250.0f;
	BlitString(m_textsCategoriesTitles[0], screenCenter.x - 245.0f, positionHeight, _window);
	BlitString(m_textsCategoriesTitles[1], screenCenter.x - 95.0f, positionHeight, _window);
	BlitString(m_textsCategoriesTitles[2], screenCenter.x + 60.0f, positionHeight, _window);
	BlitString(m_textsCategoriesTitles[3], screenCenter.x + 200.0f, positionHeight, _window);
	BlitString(m_textsCategoriesTitles[4], screenCenter.x + 370.0f, positionHeight, _window);

	// Display of the resources data
	if (m_listOfAnnualResourcesData[m_currentYearDisplayed] != nullptr)
	{
		if (m_listOfAnnualResourcesData[m_currentYearDisplayed]->first != nullptr)
		{
			LinkedListClass::sElement* currentElement;
			int counter;

			sf::Sprite spriteSesterce = Money::GetSingleton()->GetSprite();
			spriteSesterce.setScale(sf::Vector2f(0.45f, 0.45f));
			
			float backgroundHeightAvailable = m_papyrusBackground.getGlobalBounds().height - PAPYRUS_BG_OFFSET_TOP - PAPYRUS_BG_OFFSET_BOT;
			float spaceBetweenResources = backgroundHeightAvailable / m_listOfAnnualResourcesData[m_currentYearDisplayed]->size;

			for (currentElement = m_listOfAnnualResourcesData[m_currentYearDisplayed]->first, counter = 0;
				currentElement != nullptr;
				currentElement = currentElement->next, counter++)
			{
				sAnnualResourceData* curResource = ((sAnnualResourceData*)currentElement->data);
				float currentHeight = screenCenter.y - (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) + PAPYRUS_BG_OFFSET_TOP + (counter * spaceBetweenResources);

				BlitSprite(RessourcesManager::GetSingleton()->GetResourceSprite(curResource->m_resource), screenCenter.x - 425.0f, currentHeight - 10.0f, _window);

				for (int i = 0; i < 4; i++)
				{

					if (i == 3)
					{
						BlitString(m_textsData[counter][i], screenCenter.x + 400.0f, currentHeight, _window);
					}
					else
					{
						BlitString(m_textsData[counter][i], screenCenter.x - 400.0f + (i + 1) * 150, currentHeight, _window);
					}
				}

				if (curResource->m_isCanBeSold)
				{
					BlitSprite(m_greenCheck, screenCenter.x + 200, currentHeight + 5.0f, _window);
				}
				else
				{
					BlitSprite(m_redCross, screenCenter.x + 200, currentHeight + 5.0f, _window);
				}

				// Display the money at the end of the line
				BlitSprite(spriteSesterce, screenCenter.x + 425.0f, currentHeight + 7.0f, _window);
			}
		}
	}

	// Display of the year number on the papyrus border
	BlitString(m_textYear, screenCenter.x - 550.0f, screenCenter.y, _window);
	BlitString(m_textYear, screenCenter.x + 555.0f, screenCenter.y, _window);
}

void ListOfAnnualProductions::SavingDataForFile(std::ofstream* _file)
{

}

void ListOfAnnualProductions::LoadingDataFromFile(std::ifstream* _file)
{

}