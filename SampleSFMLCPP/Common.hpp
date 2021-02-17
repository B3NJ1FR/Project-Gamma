#pragma once

#include "Definitions.hpp"

enum GeneralState
{
	MAIN_STATE_MENU,
	MAIN_STATE_LOAD_GAME,
	MAIN_STATE_GAME,
	MAIN_STATE_EXIT,
};


#define TILE_WIDTH 64
#define TILE_HEIGHT 32

// This function allow to precharging the sprites for the pictures that will appear at screen
// Also, we add the posibility to center the picture origin
sf::Sprite LoadSprite(const std::string _stringFileName, int _originPosition);
sf::Sprite LoadSprite(const std::string _stringFileName, const sf::Vector2f &_screenPosition, int _originPosition);

void LoadTextString(sf::Text *_textToInitialise, const char * _string, sf::Font *_font, int _characterSize, sf::Color _color);
void LoadTextString(sf::Text *_textToInitialise, std::string _string, sf::Font *_font, int _characterSize, sf::Color _color, int _originPosition = 0);
void LoadTextString(sf::Text *_textToInitialise, const char * _string, sf::Font *_font, int _characterSize, sf::Color _color, sf::Vector2f _position);
void LoadTextString(sf::Text *_textToInitialise, std::string _string, sf::Font *_font, int _characterSize, sf::Color _color, sf::Vector2f _position);
void LoadTextString(sf::Text *_textToInitialise, std::string _string);
void LoadTextString(sf::Text *_textToInitialise, const char * _string);

void UpdateDynamicsTexts(sf::Text* _textString, const int &_variable);

void UpdateDynamicsTexts(sf::Text* _textString, const int &_variable1, const int &_variable2);

std::string ConvertStringIntoParagraph(std::string _string, const int &_maximalCharactersLength);

// That function allow to know if the player has clicked on the "button" picture's
bool IsMouseOnSprite(sf::Vector2i _mousePosition, sf::Sprite *_sprite, int _originPosition);

// That function permit to print the picture at screen to some position (x and y), and if wanted, rotating it
void BlitSprite(sf::Sprite _sprite, const float &_posX, const float &_posY, const float &_angle, sf::RenderWindow &_window);
void BlitSprite(sf::Sprite _sprite, const float &_posX, const float &_posY, sf::RenderWindow& _window);
void BlitSprite(sf::Sprite _sprite, const sf::Vector2f& _position, const float &_angle, sf::RenderWindow &_window);
void BlitSprite(sf::Sprite _sprite, const float &_angle, sf::RenderWindow &_window);
void BlitSprite(sf::Sprite _sprite, sf::RenderWindow& _window);

void BlitString(sf::Text _textString, const int &_posX, const int &_posY, sf::RenderWindow &_window);
void BlitString(sf::Text _textString, sf::RenderWindow &_window);

sf::Vector2f WorldToScreen(float _x, float _y);
sf::Vector2f WorldToScreen(sf::Vector2f _position);

sf::Vector2i ScreenToTileMouse(float _x, float _y, sf::Vector2f _scale);
sf::Vector2i ScreenToTileMouse(sf::Vector2f _position, sf::Vector2f _scale);

float DistanceFormula(const float &_1stX, const float &_2ndX, const float &_1stY, const float &_2ndY);
float DistanceFormula(const sf::Vector2f &_1stMapPosition, const sf::Vector2f &_2ndMapPosition);


