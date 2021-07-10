#pragma once

#include "Common.hpp"

class Fonts
{
private :
	sf::Font m_generalFont;
	sf::Font m_charlemagneFont;

	Fonts();
public:

	// Destructor
	~Fonts();

	// Singleton
	static Fonts* GetSingleton();

	// Getters
	inline sf::Font GetCharlemagneFont() { return m_charlemagneFont; };
};

