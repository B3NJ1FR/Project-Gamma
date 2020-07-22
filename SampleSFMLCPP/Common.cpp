#include "Common.hpp"

// This function allow to precharging the sprites for the pictures that will appear at screen
// Also, we add the posibility to center the picture origin
sf::Sprite LoadSprite(const std::string _stringFileName, int _originPosition = 0)
{
	sf::Texture* temporaryTexture = new sf::Texture;
	temporaryTexture->loadFromFile(_stringFileName);

	sf::Sprite temporarySprite;
	temporarySprite.setTexture(*temporaryTexture);

	// Centré verticalement et horizontalement
	if (_originPosition == 1)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };
		
		origin.x = origin.x / 2;
		origin.y = origin.y / 2;

		temporarySprite.setOrigin(origin);
	}
	// Centré verticalement gauche
	else if (_originPosition == 2)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = 0;
		origin.y = (origin.y / 2);

		temporarySprite.setOrigin(origin);
	}
	// Centré verticalement droite
	else if (_originPosition == 3)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = origin.x;
		origin.y = (origin.y / 2);

		temporarySprite.setOrigin(origin);
	}
	// Centré horizontalement haut
	else if (_originPosition == 4)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = (origin.x / 2);
		origin.y = 0;

		temporarySprite.setOrigin(origin);
	}
	// Centré horizontalement bas
	else if (_originPosition == 5)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = (origin.x / 2);
		origin.y = origin.y;

		temporarySprite.setOrigin(origin);
	}

	return temporarySprite;
}



// That function allow to know if the player has clicked on the "button" picture's
bool IsMouseOnSprite(sf::Vector2i _mousePosition, sf::Sprite *_sprite, int _originPosition)
{
	// DONT WORK
	// DONT WORK
	// DONT WORK
	// DONT WORK
	// DONT WORK


	if (_originPosition)
	{
		std::cout << "Sprite : " << _sprite->getPosition().x << " & " << _sprite->getPosition().x - (_sprite->getGlobalBounds().width / 2) << " " << _sprite->getPosition().x + (_sprite->getGlobalBounds().width / 2) << "\n";
		std::cout << "Sprite h : " << _sprite->getPosition().y << " & " << _sprite->getPosition().y - (_sprite->getGlobalBounds().height / 2) << " " << _sprite->getPosition().y + (_sprite->getGlobalBounds().height / 2) << "\n\n";

		if (_mousePosition.x > _sprite->getPosition().x - (_sprite->getGlobalBounds().width / 2)
			&& _mousePosition.x < _sprite->getPosition().x + (_sprite->getGlobalBounds().width / 2)
			&& _mousePosition.y > _sprite->getPosition().y - (_sprite->getGlobalBounds().height / 2)
			&& _mousePosition.y < _sprite->getPosition().y + (_sprite->getGlobalBounds().height / 2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (_mousePosition.x > _sprite->getPosition().x
			&& _mousePosition.x < _sprite->getPosition().x + (_sprite->getGlobalBounds().width)
			&& _mousePosition.y > _sprite->getPosition().y
			&& _mousePosition.y < _sprite->getPosition().y + (_sprite->getGlobalBounds().height))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void LoadTextString(sf::Text *_textToInitialise, const char * _string, sf::Font *_font, int _characterSize, sf::Color _color)
{
	_textToInitialise->setFont(*_font);

	_textToInitialise->setString(_string);

	_textToInitialise->setCharacterSize(_characterSize);

	_textToInitialise->setFillColor(_color);
}

void LoadTextString(sf::Text *_textToInitialise, const char * _string, sf::Font *_font, int _characterSize, sf::Color _color, sf::Vector2f _position)
{
	_textToInitialise->setFont(*_font);

	_textToInitialise->setString(_string);

	_textToInitialise->setCharacterSize(_characterSize);

	_textToInitialise->setFillColor(_color);

	_textToInitialise->setPosition(_position);
}

void UpdateDynamicsTexts(sf::Text* _textString, const int &_variable)
{
	char tabJoueur[64];

	sprintf_s(tabJoueur, 64, "%d ", _variable);

	_textString->setString(tabJoueur);
}


void UpdateDynamicsTexts(sf::Text* _textString, const int &_variable1, const int &_variable2)
{
	int numberOfCaractersAlreadyUsed;
	char tabJoueur[64];

	numberOfCaractersAlreadyUsed = sprintf_s(tabJoueur, 64, "%d ", _variable1);
	numberOfCaractersAlreadyUsed += sprintf_s(tabJoueur + numberOfCaractersAlreadyUsed, 64 - numberOfCaractersAlreadyUsed, "%d", _variable2);

	_textString->setString(tabJoueur);
}

// That function permit to print the picture at screen to some position (x and y), and if wanted, rotating it
void BlitSprite(sf::Sprite _sprite, const float _posX, const float _posY, const float _angle, sf::RenderWindow &_window)
{
	// Assignment of the position to the sprite
	_sprite.setPosition((float)_posX, (float)_posY);

	// Rotating the sprite if wanted
	_sprite.setRotation(_angle);

	_window.draw(_sprite);
}


void BlitString(sf::Text _textString, const int &_posX, const int &_posY, sf::RenderWindow &_window)
{
	_textString.setPosition((float)_posX, (float)_posY);

	_window.draw(_textString);
}

void BlitString(sf::Text _textString, sf::RenderWindow &_window)
{
	_window.draw(_textString);
}

sf::Vector2f WorldToScreen(float _x, float _y)
{
	sf::Vector2f position;

	position.x = ((_x - _y) * TILE_WIDTH) / 2;
	position.y = (((_x + _y) * TILE_HEIGHT) / 2);

	return position;
}

sf::Vector2f WorldToScreen(sf::Vector2f _position)
{
	sf::Vector2f position;

	position.x = ((_position.x - _position.y) * TILE_WIDTH) / 2;
	position.y = (((_position.x + _position.y) * TILE_HEIGHT) / 2);

	return position;
}


sf::Vector2i ScreenToTileMouse(float _x, float _y, sf::Vector2f _scale)
{
	sf::Vector2i coordinates = { (int)((_x + _y * 2) / (TILE_WIDTH * _scale.x)),
								 (int)((_y * 2 - _x) / (TILE_WIDTH * _scale.x)) };

	return coordinates;
}

sf::Vector2i ScreenToTileMouse(sf::Vector2f _position, sf::Vector2f _scale)
{
	sf::Vector2i coordinates = { (int)((_position.x + _position.y * 2) / (TILE_WIDTH * _scale.x)),
								 (int)((_position.y * 2 - _position.x) / (TILE_WIDTH * _scale.x)) };

	return coordinates;
}


float DistanceFormula(const float &_1stX, const float &_2ndX, const float &_1stY, const float &_2ndY)
{
	return sqrt(pow((_1stX - _2ndX), 2) + pow((_1stY - _2ndY), 2));
}

float DistanceFormula(const sf::Vector2f &_1stMapPosition, const sf::Vector2f &_2ndMapPosition)
{
	return sqrt((pow(abs(_1stMapPosition.x - _2ndMapPosition.x), 2) + pow(abs(_1stMapPosition.y - _2ndMapPosition.y), 2)));
}