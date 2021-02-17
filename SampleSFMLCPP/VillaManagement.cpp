#include "VillaManagement.h"
#include "GameDefinitions.h"


VillaManagement::VillaManagement()
{
	// Initialisation of the sprites
	m_backgroundSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification test.png", sf::Vector2f(0, 0), 0);
	//backgroundSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Benjamain_Project_Vignioble.png", sf::Vector2f(0, 0), 0);

	m_tableSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png", sf::Vector2f(0, 0), 0);
	m_bookshelfSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png", sf::Vector2f(0, 0), 0);
	m_entranceSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png", sf::Vector2f(0, 0), 0);

	// Dupplicating the scale of the sprites because it resolution is 960x540
	m_backgroundSprite.setScale(sf::Vector2f(2, 2));

	m_tableSprite.setScale(sf::Vector2f(2, 2));
	m_bookshelfSprite.setScale(sf::Vector2f(2, 2));
	m_entranceSprite.setScale(sf::Vector2f(2, 2));

	m_imageTable.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png");
	m_imageBookshelf.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png");
	m_imageEntrance.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png");
}

VillaManagement::~VillaManagement()
{

}


void VillaManagement::InputVillaManagement(enum CurrentGameState *_state, sf::RenderWindow &_window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
	
	// Verification if the mouse position is in the screen to avoid any problems
	if (mousePosition.x / 2 >= 0
		&& mousePosition.x / 2 < SCENE_BASIC_RESOLUTION_WIDTH
		&& mousePosition.y / 2 >= 0
		&& mousePosition.y / 2 < SCENE_BASIC_RESOLUTION_HEIGHT)
	{
		// We get the color of the pixel cliqued, and we test it opacity, and consequently the object cliqued
		if (m_imageTable.getPixel(mousePosition.x / 2, mousePosition.y / 2).a != 0)
		{
			std::cout << "Table cliqued\n";
			*(_state) = BUILD_MODE;
		}
		else if(m_imageBookshelf.getPixel(mousePosition.x / 2, mousePosition.y / 2).a != 0)
		{
			std::cout << "Bookshelf cliqued\n";
			//*(_state) = ESTATE_DATA_N_STATISTICS;
		}
		else if (m_imageEntrance.getPixel(mousePosition.x / 2, mousePosition.y / 2).a != 0)
		{
			//std::cout << "Entrance / Exit cliqued\n";
			//*(_state) = NORMAL_MODE;
		}
	}
}
void VillaManagement::DisplayVillaManagement(sf::RenderWindow &_window)
{
	BlitSprite(m_backgroundSprite, _window);

	BlitSprite(m_tableSprite, _window);
	
	BlitSprite(m_bookshelfSprite, _window);

	BlitSprite(m_entranceSprite, _window);
}