#ifndef COMMON__HPP
#define COMMON__HPP

#include "Definitions.hpp"

#define TILE_WIDTH 64
#define TILE_HEIGHT 32


// This function allow to precharging the sprites for the pictures that will appear at screen
// Also, we add the posibility to center the picture origin
sf::Sprite LoadSprite(const std::string _stringFileName, int _originPosition);

void LoadTextString(sf::Text *_textToInitialise, const char * _string, sf::Font *_font, int _characterSize, sf::Color _color);
void LoadTextString(sf::Text *_textToInitialise, const char * _string, sf::Font *_font, int _characterSize, sf::Color _color, sf::Vector2f _position);

void UpdateDynamicsTexts(sf::Text* _textString, const int &_variable);

void UpdateDynamicsTexts(sf::Text* _textString, const int &_variable1, const int &_variable2);

// That function allow to know if the player has clicked on the "button" picture's
bool IsMouseOnSprite(sf::Vector2i _mousePosition, sf::Sprite *_sprite, int _originPosition);

// That function permit to print the picture at screen to some position (x and y), and if wanted, rotating it
void BlitSprite(sf::Sprite _sprite, const float _posX, const float _posY, const float _angle, sf::RenderWindow &_window);

void BlitString(sf::Text _textString, const int &_posX, const int &_posY, sf::RenderWindow &_window);
void BlitString(sf::Text _textString, sf::RenderWindow &_window);

sf::Vector2f WorldToScreen(float _x, float _y);

sf::Vector2i ScreenToTileMouse(float _x, float _y, sf::Vector2f _scale);

#endif // !COMMON__HPP

