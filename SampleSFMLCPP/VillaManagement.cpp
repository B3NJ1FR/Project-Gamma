#include "VillaManagement.h"
#include "GameDefinitions.h"
#include "ListOfAnnualProductions.h"
#include "ListOfAnnualCostsNRevenues.h"


VillaManagement::VillaManagement()
{
	// Initialisation of the sprites
	m_backgroundSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Vignoble_pixel_paint.png", sf::Vector2f(0, 0), 0);
	m_greyBackground = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/greybackground.png", sf::Vector2f(0, 0), 0);

	m_spriteElements[(int)SpriteElements::DOMAIN_PLAN] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/map.png", sf::Vector2f(0, 0), 0);
	m_spriteElements[(int)SpriteElements::PAPYRUS_COSTS] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/parchment.png", sf::Vector2f(0, 0), 0);
	m_spriteElements[(int)SpriteElements::BOOK_PRODUCTION] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/book.png", sf::Vector2f(0, 0), 0);
	m_spriteElements[(int)SpriteElements::PURSE] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/moneys.png", sf::Vector2f(0, 0), 0);
	m_spriteElements[(int)SpriteElements::QUIT_VILLA] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/entrance.png", sf::Vector2f(0, 0), 0);

	m_spriteElementsFeedbacks[(int)SpriteElements::DOMAIN_PLAN] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Feedbacks_Villa_Interior/feedback_map_behind.png", sf::Vector2f(0, 0), 0);
	m_spriteElementsFeedbacks[(int)SpriteElements::PAPYRUS_COSTS] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Feedbacks_Villa_Interior/feedback_parchement.png", sf::Vector2f(0, 0), 0);
	m_spriteElementsFeedbacks[(int)SpriteElements::BOOK_PRODUCTION] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Feedbacks_Villa_Interior/feedback_book_behind.png", sf::Vector2f(0, 0), 0);
	m_spriteElementsFeedbacks[(int)SpriteElements::PURSE] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Feedbacks_Villa_Interior/feedback_moneys_behind.png", sf::Vector2f(0, 0), 0);
	m_spriteElementsFeedbacks[(int)SpriteElements::QUIT_VILLA] = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Feedbacks_Villa_Interior/feedback_entrance_behind.png", sf::Vector2f(0, 0), 0);
	

	// Dupplicating the scale of the sprites because it resolution is 960x540
	m_backgroundSprite.setScale(sf::Vector2f(2, 2));

	for (int i = 0; i < (int)SpriteElements::NB_MAX_ELEMENTS; i++)
	{
		m_spriteElements[i].setScale(sf::Vector2f(2, 2));
		m_spriteElementsFeedbacks[i].setScale(sf::Vector2f(2, 2));
		m_isFeedbackActive[i] = false;
	}

	m_imageDomainPlan.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/map.png");
	m_imagePapyrus.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/parchment.png");
	m_imageBook.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/book.png");
	m_imagePurse.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/moneys.png");
	m_imageEntrance.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/entrance.png");

	m_internalStateMachine = VillaManagementStateMachine::NORMAL_STATE;
}

VillaManagement::~VillaManagement()
{

}

void VillaManagement::InputVillaManagement(enum CurrentGameState *_state, sf::RenderWindow &_window, const sf::Vector2i& _screenResolution)
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		// Closing by pressing the Close button
		if (event.type == sf::Event::Closed)
		{
			exit(EXIT_SUCCESS);
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				if (m_internalStateMachine == VillaManagementStateMachine::NORMAL_STATE)
				{
					*(_state) = NORMAL_MODE;
					TimeManagement::GetSingleton()->SetTypeOfAcceleration(TypeOfTimeAcceleration::GAME_NORMAL_SPEED);
				}
			}
		}

		sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
		mousePosition = sf::Vector2i(mousePosition.x / 2, mousePosition.y / 2);

		switch (m_internalStateMachine)
		{
		case VillaManagementStateMachine::NORMAL_STATE:
			// We ask to display the feedback if we've the mouse hover
			m_isFeedbackActive[(int)SpriteElements::DOMAIN_PLAN] = (m_imageDomainPlan.getPixel(mousePosition.x, mousePosition.y).a != 0) ? true : false;
			m_isFeedbackActive[(int)SpriteElements::PAPYRUS_COSTS] = (m_imagePapyrus.getPixel(mousePosition.x, mousePosition.y).a != 0) ? true : false;
			m_isFeedbackActive[(int)SpriteElements::BOOK_PRODUCTION] = (m_imageBook.getPixel(mousePosition.x, mousePosition.y).a != 0) ? true : false;
			m_isFeedbackActive[(int)SpriteElements::PURSE] = (m_imagePurse.getPixel(mousePosition.x, mousePosition.y).a != 0) ? true : false;
			m_isFeedbackActive[(int)SpriteElements::QUIT_VILLA] = (m_imageEntrance.getPixel(mousePosition.x, mousePosition.y).a != 0) ? true : false;

			if (event.type == sf::Event::MouseButtonPressed
				&& event.mouseButton.button == sf::Mouse::Left)
			{
				// Verification if the mouse position is in the screen to avoid any problems
				if (mousePosition.x >= 0
					&& mousePosition.x < SCENE_BASIC_RESOLUTION_WIDTH
					&& mousePosition.y >= 0
					&& mousePosition.y < SCENE_BASIC_RESOLUTION_HEIGHT)
				{
					// We get the color of the pixel cliqued, and we test it opacity, and consequently the object cliqued
					if (m_imageDomainPlan.getPixel(mousePosition.x, mousePosition.y).a != 0)
					{
						//std::cout << "Table cliqued\n";
						*_state = BUILD_MODE;
						SetInternalStateMachine(VillaManagementStateMachine::BUILD_MODE_STATE);
					}
					else if (m_imagePapyrus.getPixel(mousePosition.x, mousePosition.y).a != 0)
					{
						//std::cout << "Bookshelf cliqued\n";
						SetInternalStateMachine(VillaManagementStateMachine::COSTS_N_REVENUES_STATE);
					}
					else if (m_imageBook.getPixel(mousePosition.x, mousePosition.y).a != 0)
					{
						//std::cout << "Entrance / Exit cliqued\n";
						SetInternalStateMachine(VillaManagementStateMachine::PRODUCTION_SUMMARY_STATE);
					}
					else if (m_imagePurse.getPixel(mousePosition.x, mousePosition.y).a != 0)
					{
						//std::cout << "Entrance / Exit cliqued\n";
						SetInternalStateMachine(VillaManagementStateMachine::DISPLAY_MONEY_STATE);
					}
					else if (m_imageEntrance.getPixel(mousePosition.x, mousePosition.y).a != 0)
					{
						//std::cout << "Entrance / Exit cliqued\n";
						SetInternalStateMachine(VillaManagementStateMachine::QUIT_STATE);
						*_state = NORMAL_MODE;
						TimeManagement::GetSingleton()->SetTypeOfAcceleration(TypeOfTimeAcceleration::GAME_NORMAL_SPEED);
					}
				}
			}
			break;
		case VillaManagementStateMachine::BUILD_MODE_STATE:
			break;
		case VillaManagementStateMachine::COSTS_N_REVENUES_STATE:
			ListOfAnnualCostsNRevenues::GetSingleton()->Input(event, _window, _screenResolution);
			break;
		case VillaManagementStateMachine::PRODUCTION_SUMMARY_STATE:
			ListOfAnnualProductions::GetSingleton()->Input(event, _window, _screenResolution);
			break;
		case VillaManagementStateMachine::DISPLAY_MONEY_STATE:
			break;
		case VillaManagementStateMachine::QUIT_STATE:
			break;
		default:
			break;
		}
	}
}

void VillaManagement::UpdateVillaManagement()
{
	if (m_internalStateMachine == VillaManagementStateMachine::PRODUCTION_SUMMARY_STATE)
	{
		ListOfAnnualProductions::GetSingleton()->Update(&m_internalStateMachine);
	}
	else if (m_internalStateMachine == VillaManagementStateMachine::COSTS_N_REVENUES_STATE)
	{
		ListOfAnnualCostsNRevenues::GetSingleton()->Update(&m_internalStateMachine);
	}
}

void VillaManagement::DisplayVillaManagement(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution)
{
	// Display the background + the different elements
	BlitSprite(m_backgroundSprite, _window);

	for (int i = 0; i < (int)SpriteElements::NB_MAX_ELEMENTS; i++)
	{
		BlitSprite(m_spriteElements[i], _window);

		// If the feedback is active, we have to display it
		if (m_isFeedbackActive[i]) BlitSprite(m_spriteElementsFeedbacks[i], _window);
	}

	if (m_internalStateMachine == VillaManagementStateMachine::COSTS_N_REVENUES_STATE || m_internalStateMachine == VillaManagementStateMachine::PRODUCTION_SUMMARY_STATE)
	{
		BlitSprite(m_greyBackground, _window);

		if (m_internalStateMachine == VillaManagementStateMachine::PRODUCTION_SUMMARY_STATE)
		{
			ListOfAnnualProductions::GetSingleton()->Display(_window, _screenResolution);
		}
		else
		{
			ListOfAnnualCostsNRevenues::GetSingleton()->Display(_window, _screenResolution);
		}
	}
}