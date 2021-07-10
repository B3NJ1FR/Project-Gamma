#include "VillaManagement.h"
#include "GameDefinitions.h"
#include "ListOfAnnualProductions.h"


VillaManagement::VillaManagement()
{
	// Initialisation of the sprites
	m_backgroundSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification test.png", sf::Vector2f(0, 0), 0);
	//backgroundSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Benjamain_Project_Vignioble.png", sf::Vector2f(0, 0), 0);

	m_tableSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png", sf::Vector2f(0, 0), 0);
	m_bookshelfSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png", sf::Vector2f(0, 0), 0);
	m_entranceSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png", sf::Vector2f(0, 0), 0);

	m_greyBackground = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/blackbackground.png", sf::Vector2f(0, 0), 0);

	// Dupplicating the scale of the sprites because it resolution is 960x540
	m_backgroundSprite.setScale(sf::Vector2f(2, 2));

	m_tableSprite.setScale(sf::Vector2f(2, 2));
	m_bookshelfSprite.setScale(sf::Vector2f(2, 2));
	m_entranceSprite.setScale(sf::Vector2f(2, 2));

	m_imageDomainPlan.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png");
	m_imagePapyrus.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png");
	m_imageBook.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png");
	m_imagePurse.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png");
	m_imageEntrance.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png");

	internalStateMachine = VillaManagementStateMachine::NORMAL_STATE;
}

VillaManagement::~VillaManagement()
{

}


void VillaManagement::InputVillaManagement(enum CurrentGameState *_state, sf::RenderWindow &_window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
	mousePosition = sf::Vector2i(mousePosition.x / 2, mousePosition.y / 2);


	// Verification if the mouse position is in the screen to avoid any problems
	if (mousePosition.x >= 0
		&& mousePosition.x < SCENE_BASIC_RESOLUTION_WIDTH
		&& mousePosition.y >= 0
		&& mousePosition.y < SCENE_BASIC_RESOLUTION_HEIGHT)
	{
		// We get the color of the pixel cliqued, and we test it opacity, and consequently the object cliqued
		if (m_imageDomainPlan.getPixel(mousePosition.x, mousePosition.y).a != 0)
		{
			std::cout << "Table cliqued\n";
			*_state = BUILD_MODE;
			SetInternalStateMachine(VillaManagementStateMachine::BUILD_MODE_STATE);
		}
		/*else if(m_imagePapyrus.getPixel(mousePosition.x, mousePosition.y).a != 0)
		{
			std::cout << "Bookshelf cliqued\n";
			SetInternalStateMachine(VillaManagementStateMachine::COSTS_N_REVENUES_STATE);
		}*/
		else if (m_imageBook.getPixel(mousePosition.x, mousePosition.y).a != 0)
		{
			//std::cout << "Entrance / Exit cliqued\n";
			SetInternalStateMachine(VillaManagementStateMachine::PRODUCTION_SUMMARY_STATE);
		}
		//else if (m_imagePurse.getPixel(mousePosition.x, mousePosition.y).a != 0)
		//{
		//	//std::cout << "Entrance / Exit cliqued\n";
		//	SetInternalStateMachine(VillaManagementStateMachine::DISPLAY_MONEY_STATE);
		//}
		//else if (m_imageEntrance.getPixel(mousePosition.x, mousePosition.y).a != 0)
		//{
		//	//std::cout << "Entrance / Exit cliqued\n";
		//	SetInternalStateMachine(VillaManagementStateMachine::QUIT_STATE);
		//	*_state = NORMAL_MODE;
		//}
	}
}
void VillaManagement::UpdateVillaManagement()
{
	if (internalStateMachine == VillaManagementStateMachine::PRODUCTION_SUMMARY_STATE)
	{
		ListOfAnnualProductions::GetSingleton()->Update();
	}
}

void VillaManagement::DisplayVillaManagement(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution)
{
	BlitSprite(m_backgroundSprite, _window);

	BlitSprite(m_tableSprite, _window);
	
	BlitSprite(m_bookshelfSprite, _window);

	BlitSprite(m_entranceSprite, _window);

	if (internalStateMachine == VillaManagementStateMachine::COSTS_N_REVENUES_STATE || internalStateMachine == VillaManagementStateMachine::PRODUCTION_SUMMARY_STATE)
	{
		BlitSprite(m_greyBackground, _window);

		if (internalStateMachine == VillaManagementStateMachine::PRODUCTION_SUMMARY_STATE)
		{
			ListOfAnnualProductions::GetSingleton()->Display(_window, _screenResolution);
		}
		else
		{

		}
	}
}