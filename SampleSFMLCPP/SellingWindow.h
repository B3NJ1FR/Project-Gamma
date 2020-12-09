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
	sf::Text m_sellingWindowRessourceQuantity[4];

	int m_sellingWindowScrollButtonPosition;
	int m_sellingWindowScrollButtonMax;
	int m_quantityConvertedToSell;
	int m_priceAccepted;

	bool m_wasCursorPressed;

public:
	SellingWindow(sf::Font *_font);
	~SellingWindow();

	// Function that manages the cursor data (quantity selected, minimals, maximals and current price)
	void UpdateQuantityConvertedToSell(Purchasers *_purchasers, const int &_quantityStocked);
	void UpdateSellingWindowTexts(Purchasers *_purchasers);
	void DisplaySellingWindow(sf::RenderWindow &_window);
	void InputSellingWindow(bool *_isOfferAccepted, enum CurrentGameState *_state, Stalls *_stall, sf::RenderWindow &_window);
};