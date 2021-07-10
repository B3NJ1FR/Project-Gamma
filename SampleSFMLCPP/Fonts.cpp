#include "Fonts.h"

Fonts::Fonts()
{
	m_generalFont.loadFromFile("Data/Fonts/arial.ttf");
	m_charlemagneFont.loadFromFile("Data/Fonts/CharlemagneStd-Bold.otf");
	std::cout << "CREAAATION\n\n\n\n\n\n";
}

Fonts::~Fonts()
{
	std::cout << "DESTROYYED\n\n\n\n\n\n";
}

Fonts* Fonts::GetSingleton()
{
	static Fonts uniqueInstance;
	return &uniqueInstance;
}