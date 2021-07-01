#pragma once

#include "Common.hpp"
#include "Purchasers.h"
#include "Stalls.h"

enum class TypeOfClickStatus
{
	CLICK_NONE,
	CLICK_CURSOR,
	CLICK_PLUS_MINUS,
};


class SellingWindow
{
private:

	sf::Sprite m_sellingWindowBackground;
	sf::Sprite m_sellingWindowAcceptButton;
	sf::Sprite m_sellingWindowRejectButton;
	sf::Sprite m_sellingWindowScrollButton;
	sf::Sprite m_sellingWindowScrollLine;
	sf::Sprite m_sellingWindowArrowPlusOne;
	sf::Sprite m_sellingWindowArrowPlusTen;
	sf::Sprite m_sellingWindowArrowMinusOne;
	sf::Sprite m_sellingWindowArrowMinusTen;
	sf::Text m_sellingWindowProvenance;
	sf::Text m_sellingWindowPrice[3];
	sf::Text m_sellingWindowRessourceQuantity[6];

	int m_sellingWindowScrollButtonPosition;
	int m_sellingWindowScrollButtonMax;
	int m_quantityConvertedToSell;
	int m_priceAccepted;

	TypeOfClickStatus currentTypeOfClickStatus;

	bool m_wasCursorPressed;
	bool m_windowOpening;

public:
	// Constructor & Destructor
	SellingWindow(sf::Font *_font, const sf::Vector2i& _screenResolution);
	~SellingWindow();

	// Function that manages the cursor data (quantity selected, minimals, maximals and current price)
	void IsQuantityToSellIsCorrect(Purchasers* _purchasers, const int& _quantityStocked, sf::Vector2i& _screenResolution);
	
	void InputSellingWindowButtons(enum CurrentGameState* _state, Stalls* _stall, sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);
	void InputSellingWindow(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution);
	void UpdateWindowAtOpening(Purchasers* _purchasers, const sf::Vector2i& _screenResolution);
	void UpdateQuantityConvertedToSell(Purchasers* _purchasers, const int& _quantityStocked, const sf::Vector2i& _screenResolution);
	void UpdateSellingWindowTexts(Purchasers* _purchasers);
	void DisplaySellingWindow(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);
};