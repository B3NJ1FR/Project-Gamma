#pragma once

#include "Common.hpp"
#include "Purchasers.h"
#include "Stalls.h"


class SellingWindow
{
private:

	sf::Sprite m_sellingWindowBackground;
	sf::Sprite m_sellingWindowAcceptButton;
	sf::Sprite m_sellingWindowRejectButton;
	sf::Sprite m_sellingWindowScrollButton;
	sf::Sprite m_sellingWindowScrollLine;
	sf::Text m_sellingWindowProvenance;
	sf::Text m_sellingWindowPrice[3];
	sf::Text m_sellingWindowRessourceQuantity[6];

	int m_sellingWindowScrollButtonPosition;
	int m_sellingWindowScrollButtonMax;
	int m_quantityConvertedToSell;
	int m_priceAccepted;

	bool m_wasCursorPressed;

public:
	// Constructor & Destructor
	SellingWindow(sf::Font *_font, const sf::Vector2i& _screenResolution);
	~SellingWindow();

	// Function that manages the cursor data (quantity selected, minimals, maximals and current price)
	void UpdateQuantityConvertedToSell(Purchasers* _purchasers, const int& _quantityStocked, const sf::Vector2i& _screenResolution);
	void UpdateSellingWindowTexts(Purchasers *_purchasers);
	void DisplaySellingWindow(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution);
	void InputSellingWindow(enum CurrentGameState *_state, Stalls *_stall, sf::RenderWindow &_window, const sf::Vector2i& _screenResolution);
};