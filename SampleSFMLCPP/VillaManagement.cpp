#include "VillaManagement.h"
#include "GameDefinitions.h"


VillaManagement::VillaManagement()
{
	// Initialisation of the sprites
	this->backgroundSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification test.png", 0);

	this->tableSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification table.png", sf::Vector2f(700, 650), 0);
	this->bookshelfSprite = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/salle plannification bibliotheque.png", sf::Vector2f(30, 110), 0);

	// Dupplicating the scale of the sprites because it resolution is 960x540
	this->backgroundSprite.setScale(sf::Vector2f(2, 2));
	this->tableSprite.setScale(sf::Vector2f(2, 2));
	this->bookshelfSprite.setScale(sf::Vector2f(2, 2));
}

VillaManagement::~VillaManagement()
{

}


void VillaManagement::InputVillaManagement(enum GameState *_state, sf::RenderWindow &_window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
	
	if (mousePosition.x > this->tableSprite.getPosition().x 
		&& mousePosition.x <  this->tableSprite.getPosition().x + this->tableSprite.getGlobalBounds().width
		&& mousePosition.y >  this->tableSprite.getPosition().y
		&& mousePosition.y < this->tableSprite.getPosition().y + this->tableSprite.getGlobalBounds().width)
	{
		std::cout << "Table cliquée\n";
		*(_state) = BUILD_MODE;
	}
	else if (mousePosition.x > this->bookshelfSprite.getPosition().x
		&& mousePosition.x <  this->bookshelfSprite.getPosition().x + this->bookshelfSprite.getGlobalBounds().width
		&& mousePosition.y >  this->bookshelfSprite.getPosition().y
		&& mousePosition.y < this->bookshelfSprite.getPosition().y + this->bookshelfSprite.getGlobalBounds().width)
	{
		std::cout << "Bibliothèque cliquée\n";
		//*(_state) = ESTATE_DATA_N_STATISTICS;
	}
}
void VillaManagement::DisplayVillaManagement(sf::RenderWindow &_window)
{
	BlitSprite(this->backgroundSprite, 0, 0, 0, _window);

	BlitSprite(this->tableSprite, 0, _window);
	BlitSprite(this->bookshelfSprite, 0, _window);
}