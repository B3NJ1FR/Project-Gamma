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
	else if (_originPosition == 6)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = origin.x - TILE_WIDTH;
		origin.y = origin.y;

		temporarySprite.setOrigin(origin);
	}
	else if (_originPosition == 7)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = origin.x - TILE_WIDTH / 2;
		origin.y = origin.y;

		temporarySprite.setOrigin(origin);
	}
	else if (_originPosition == 8)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = TILE_WIDTH;
		origin.y = origin.y;

		temporarySprite.setOrigin(origin);
	}
	else if (_originPosition == 9)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = TILE_WIDTH / 2;
		origin.y = origin.y;

		temporarySprite.setOrigin(origin);
	}
	else if (_originPosition == 10)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = 0;
		origin.y = origin.y ;

		temporarySprite.setOrigin(origin);
	}

	return temporarySprite;
}

sf::Sprite LoadSprite(const std::string _stringFileName, const sf::Vector2f &_screenPosition, int _originPosition = 0)
{
	sf::Texture* temporaryTexture = new sf::Texture;
	temporaryTexture->loadFromFile(_stringFileName);

	sf::Sprite temporarySprite;
	temporarySprite.setTexture(*temporaryTexture);

	temporarySprite.setPosition(_screenPosition);

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
	else if (_originPosition == 6)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = origin.x - TILE_WIDTH;
		origin.y = origin.y;

		temporarySprite.setOrigin(origin);
	}
	else if (_originPosition == 7)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = origin.x - TILE_WIDTH / 2;
		origin.y = origin.y;

		temporarySprite.setOrigin(origin);
	}
	else if (_originPosition == 8)
	{
		sf::Vector2f origin = { (float)temporaryTexture->getSize().x, (float)temporaryTexture->getSize().y };

		origin.x = TILE_WIDTH;
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

void LoadTextString(sf::Text *_textToInitialise, std::string _string, sf::Font *_font, int _characterSize, sf::Color _color, int _originPosition)
{
	_textToInitialise->setFont(*_font);

	_textToInitialise->setString(_string);

	_textToInitialise->setCharacterSize(_characterSize);

	_textToInitialise->setFillColor(_color);

	if (_originPosition == 1)
	{
		sf::Vector2f origin = { (float)_textToInitialise->getGlobalBounds().width, (float)_textToInitialise->getGlobalBounds().height };

		origin.x = origin.x / 2;
		origin.y = origin.y / 2;

		_textToInitialise->setOrigin(origin);
	}
	else if (_originPosition == 2)
	{
		sf::Vector2f origin = { (float)_textToInitialise->getGlobalBounds().width, (float)_textToInitialise->getGlobalBounds().height };

		origin.x = origin.x / 2;
		origin.y = RESET;

		_textToInitialise->setOrigin(origin);
	}
	else if (_originPosition == 3)
	{
		sf::Vector2f origin = { (float)_textToInitialise->getGlobalBounds().width, (float)_textToInitialise->getGlobalBounds().height };

		origin.x = origin.x;
		origin.y = RESET;

		_textToInitialise->setOrigin(origin);
	}
	else if (_originPosition == 4)
	{
		sf::Vector2f origin = { (float)_textToInitialise->getGlobalBounds().width, (float)_textToInitialise->getGlobalBounds().height };

		origin.x = origin.x;
		origin.y = origin.y / 2;

		_textToInitialise->setOrigin(origin);
	}
	else if (_originPosition == 5)
	{
		sf::Vector2f origin = { (float)_textToInitialise->getGlobalBounds().width, (float)_textToInitialise->getGlobalBounds().height };

		origin.x = RESET;
		origin.y = origin.y / 2;

		_textToInitialise->setOrigin(origin);
	}
}


void LoadTextString(sf::Text *_textToInitialise, const char * _string, sf::Font *_font, int _characterSize, sf::Color _color, sf::Vector2f _position)
{
	_textToInitialise->setFont(*_font);

	_textToInitialise->setString(_string);

	_textToInitialise->setCharacterSize(_characterSize);

	_textToInitialise->setFillColor(_color);

	_textToInitialise->setPosition(_position);
}


void LoadTextString(sf::Text *_textToInitialise, std::string _string, sf::Font *_font, int _characterSize, sf::Color _color, sf::Vector2f _position)
{
	_textToInitialise->setFont(*_font);

	_textToInitialise->setString(_string);

	_textToInitialise->setCharacterSize(_characterSize);

	_textToInitialise->setFillColor(_color);

	_textToInitialise->setPosition(_position);
}

void LoadTextString(sf::Text *_textToInitialise, std::string _string)
{
	_textToInitialise->setString(_string);
}

void LoadTextString(sf::Text *_textToInitialise, const char * _string)
{
	_textToInitialise->setString(_string);
}

void ChangeTextStringOrigin(sf::Text* _textToModify, int _originPosition)
{
	if (_originPosition == 1)
	{
		sf::Vector2f origin = { (float)_textToModify->getGlobalBounds().width, (float)_textToModify->getGlobalBounds().height };

		origin.x = origin.x / 2;
		origin.y = origin.y / 2;

		_textToModify->setOrigin(origin);
	}
	else if (_originPosition == 2)
	{
		sf::Vector2f origin = { (float)_textToModify->getGlobalBounds().width, (float)_textToModify->getGlobalBounds().height };

		origin.x = origin.x / 2;
		origin.y = RESET;

		_textToModify->setOrigin(origin);
	}
	else if (_originPosition == 3)
	{
		sf::Vector2f origin = { (float)_textToModify->getGlobalBounds().width, (float)_textToModify->getGlobalBounds().height };

		origin.x = origin.x;
		origin.y = RESET;

		_textToModify->setOrigin(origin);
	}
	else if (_originPosition == 4)
	{
		sf::Vector2f origin = { (float)_textToModify->getGlobalBounds().width, (float)_textToModify->getGlobalBounds().height };

		origin.x = origin.x;
		origin.y = origin.y / 2;

		_textToModify->setOrigin(origin);
	}
	else if (_originPosition == 5)
	{
		sf::Vector2f origin = { (float)_textToModify->getGlobalBounds().width, (float)_textToModify->getGlobalBounds().height };

		origin.x = RESET;
		origin.y = origin.y / 2;

		_textToModify->setOrigin(origin);
	}
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

std::string ConvertStringIntoParagraph(std::string _string, const int &_maximalCharactersLength)
{
	std::size_t spacesPositions;
	std::size_t previousSpacesPositions;

	int positionInString(RESET);
	int maximalCharactersPerLine(_maximalCharactersLength);

	std::string temporaryString;
	temporaryString.erase();

	temporaryString.append(_string);

	spacesPositions = temporaryString.find_first_of(" ");

	int positionBackToTheLine = temporaryString.find('\n');

	while (positionInString + maximalCharactersPerLine < temporaryString.length())
	{
		while (temporaryString.find_first_of(" ", spacesPositions + 1) <= positionInString + maximalCharactersPerLine)
		{
			previousSpacesPositions = spacesPositions;
			spacesPositions = temporaryString.find(" ", spacesPositions + 1);
		}

		// If the \n character is in this line, we set the cursor to the \n's position
		if (positionBackToTheLine > positionInString
			&& positionBackToTheLine <= positionInString + maximalCharactersPerLine)
		{
			positionInString = positionBackToTheLine;
		}
		else
		{
			// If the space position is equal to the maximal lengh needed, we can put a return at this current position
			if (spacesPositions == positionInString + maximalCharactersPerLine)
			{
				positionInString = temporaryString.find(" ", spacesPositions);
			}
			// If the space position is higher than the maximal lengh needed, we take the previous position to put it a return
			else if (spacesPositions > positionInString + maximalCharactersPerLine)
			{
				positionInString = temporaryString.find(" ", previousSpacesPositions);
			}
			// If the space position is less than the maximal lengh needed, we can put a return at this current position
			else if (spacesPositions < positionInString + maximalCharactersPerLine
				&& spacesPositions > positionInString)
			{
				positionInString = temporaryString.find(" ", spacesPositions);
			}

			temporaryString.replace(positionInString, 1, "\n", 1);
		}		
	}

	return temporaryString;
}

std::string TransformStringToVerticalOne(std::string _string)
{
	std::string str = _string;

	if (str.length() > 1)
	{
		for (int i = str.length() - 1; i > 0; i--)
		{
			str.insert(i, "\n");
		}
	}
	return str;
}

std::string TransformStringToCenteredOne(std::string _stringToCenter, int _forceOffset)
{
	std::string temporaryString = _stringToCenter;

	if (temporaryString.find("_") != -1)
	{
		temporaryString.replace(temporaryString.find("_"), 1, "\n", 1);
	}

	if (temporaryString.find("\n") != -1)
	{
		int firstWordLengh = temporaryString.find("\n");
		int secondWordLengh = temporaryString.length() - firstWordLengh - 1;

		if (firstWordLengh == secondWordLengh)
		{
			std::string newString;
			newString.erase();

			char buffer[255];
			char buffer2[255];

			temporaryString.copy(buffer, firstWordLengh + 1, 0);
			buffer[firstWordLengh + 1] = '\0';
			temporaryString.copy(buffer2, secondWordLengh, temporaryString.find("\n") + 1);
			buffer2[secondWordLengh] = '\0';

			if (_forceOffset > 0)
			{
				for (int i = 0; i < _forceOffset; i++)
				{
					newString.append(" ");
				}
			}

			temporaryString.erase();
			temporaryString = buffer + newString + buffer2;
		}
		else if (firstWordLengh < secondWordLengh)
		{
			int differenceBetweenWords = secondWordLengh - firstWordLengh;

			std::string newString;
			newString.erase();

			for (int i = 0; i < ((differenceBetweenWords / 2) - 1); i++)
			{
				newString.append("\t");
			}

			if (differenceBetweenWords % 2 == 1
				&& differenceBetweenWords / 2 != 0)
			{
				newString.append(" ");
			}

			if (_forceOffset > 0)
			{
				for (int i = 0; i < _forceOffset; i++)
				{
					newString.append(" ");
				}
			}

			temporaryString = newString + temporaryString;
		}
		else if (firstWordLengh > secondWordLengh)
		{
			int differenceBetweenWords = firstWordLengh - secondWordLengh;

			std::string newString;
			newString.erase();

			char buffer[255];
			char buffer2[255];

			temporaryString.copy(buffer, firstWordLengh + 1, 0);
			buffer[firstWordLengh + 1] = '\0';
			temporaryString.copy(buffer2, secondWordLengh, temporaryString.find("\n") + 1);
			buffer2[secondWordLengh] = '\0';

			for (int i = 0; i < ((differenceBetweenWords / 2) - 1); i++)
			{
				newString.append("\t");
			}

			if (differenceBetweenWords % 2 == 1)
			{
				newString.append(" ");
			}

			if (_forceOffset > 0)
			{
				for (int i = 0; i < _forceOffset; i++)
				{
					newString.append(" ");
				}
			}

			temporaryString.erase();
			temporaryString = buffer + newString + buffer2;

			//std::cout << temporaryString << std::endl;
		}
	}

	return temporaryString;
}


void ColorStringAccordingToItsValue(sf::Text* _textString, int _value, sf::Color _negativeColor, sf::Color _positiveColor, sf::Color _normalColor)
{
	if (_value == 0)
	{
		_textString->setFillColor(_normalColor);
	}
	else
	{
		_textString->setFillColor((_value > 0) ? _positiveColor : _negativeColor);
	}
}

// That function permit to print the picture at screen to some position (x and y), and if wanted, rotating it
void BlitSprite(sf::Sprite _sprite, const float &_posX, const float &_posY, const float &_angle, sf::RenderWindow &_window)
{
	// Assignment of the position to the sprite
	_sprite.setPosition((float)_posX, (float)_posY);

	// Rotating the sprite if wanted
	_sprite.setRotation(_angle);

	_window.draw(_sprite);
}

void BlitSprite(sf::Sprite _sprite, const float& _posX, const float& _posY, sf::RenderWindow& _window)
{
	// Assignment of the position to the sprite
	_sprite.setPosition((float)_posX, (float)_posY);

	_window.draw(_sprite);
}

void BlitSprite(sf::Sprite _sprite, const sf::Vector2f &_position, const float &_angle, sf::RenderWindow &_window)
{
	// Assignment of the position to the sprite
	_sprite.setPosition(_position);

	// Rotating the sprite if wanted
	_sprite.setRotation(_angle);

	_window.draw(_sprite);
}


void BlitSprite(sf::Sprite _sprite, const float &_angle, sf::RenderWindow &_window)
{
	// Rotating the sprite if wanted
	_sprite.setRotation(_angle);

	_window.draw(_sprite);
}

void BlitSprite(sf::Sprite _sprite, sf::RenderWindow& _window)
{
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


void SavingStringIntoBinaryFile(std::ofstream* _file, std::string _stringConcerned)
{
	std::string temporaryString = _stringConcerned;
	int stringLength = temporaryString.size();

	_file->write((char*)&stringLength, sizeof(int));
	_file->write(temporaryString.c_str(), stringLength);
}

std::string LoadingStringFromBinaryFile(std::ifstream* _file)
{
	std::string stringToLoad;
	int stringLength = 0;

	_file->read((char*)&stringLength, sizeof(int));
	stringToLoad.resize(stringLength);
	_file->read((char*)stringToLoad.c_str(), stringLength);

	return stringToLoad;
}