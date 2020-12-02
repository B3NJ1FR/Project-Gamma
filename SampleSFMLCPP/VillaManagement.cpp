#include "VillaManagement.h"
#include "GameDefinitions.h"


VillaManagement::VillaManagement()
{
	// Initialisation of the sprites
	this->backgroundSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification test.png", sf::Vector2f(0, 0), 0);
	//this->backgroundSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Benjamain_Project_Vignioble.png", sf::Vector2f(0, 0), 0);

	this->tableSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png", sf::Vector2f(0, 0), 0);
	this->bookshelfSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png", sf::Vector2f(0, 0), 0);
	this->entranceSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png", sf::Vector2f(0, 0), 0);

	// Dupplicating the scale of the sprites because it resolution is 960x540
	this->backgroundSprite.setScale(sf::Vector2f(2, 2));

	this->tableSprite.setScale(sf::Vector2f(2, 2));
	this->bookshelfSprite.setScale(sf::Vector2f(2, 2));
	this->entranceSprite.setScale(sf::Vector2f(2, 2));

	this->imageTable.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png");
	this->imageBookshelf.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png");
	this->imageEntrance.loadFromFile("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png");
}

VillaManagement::~VillaManagement()
{

}


void VillaManagement::InputVillaManagement(enum CurrentGameState *_state, TimeManagement *_time, sf::RenderWindow &_window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
	
	// Verification if the mouse position is in the screen to avoid any problems
	if (mousePosition.x / 2 >= 0
		&& mousePosition.x / 2 < SCENE_BASIC_RESOLUTION_WIDTH
		&& mousePosition.y / 2 >= 0
		&& mousePosition.y / 2 < SCENE_BASIC_RESOLUTION_HEIGHT)
	{
		// We get the color of the pixel cliqued, and we test it opacity, and consequently the object cliqued
		if (this->imageTable.getPixel(mousePosition.x / 2, mousePosition.y / 2).a != 0)
		{
			std::cout << "Table cliqued\n";
			*(_state) = BUILD_MODE;

			_time->SetTypeOfAcceleration(GAME_PAUSE);
		}
		else if(this->imageBookshelf.getPixel(mousePosition.x / 2, mousePosition.y / 2).a != 0)
		{
			std::cout << "Bookshelf cliqued\n";
			//*(_state) = ESTATE_DATA_N_STATISTICS;
		}
		else if (this->imageEntrance.getPixel(mousePosition.x / 2, mousePosition.y / 2).a != 0)
		{
			//std::cout << "Entrance / Exit cliqued\n";
			//*(_state) = NORMAL_MODE;
		}
	}
}
void VillaManagement::DisplayVillaManagement(sf::RenderWindow &_window)
{
	BlitSprite(this->backgroundSprite, 0, _window);

	BlitSprite(this->tableSprite, 0, _window);
	
	BlitSprite(this->bookshelfSprite, 0, _window);

	BlitSprite(this->entranceSprite, 0, _window);
}