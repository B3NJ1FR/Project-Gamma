#include "Fonts.h"

Fonts::Fonts()
{
	m_generalFont.loadFromFile("Data/Fonts/arial.ttf");
	m_charlemagneFont.loadFromFile("Data/Fonts/CharlemagneStd-Bold.otf");
}

Fonts::~Fonts()
{
}

Fonts* Fonts::GetSingleton()
{
	static Fonts uniqueInstance;
	return &uniqueInstance;
}