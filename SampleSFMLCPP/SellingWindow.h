#ifndef SELLING_WINDOW__H
#define SELLING_WINDOW__H

#include "Common.hpp"
#include "Purchasers.h"
#include "Stalls.h"


class SellingWindow
{
private:

	sf::Sprite sellingWindowBackground;
	sf::Sprite sellingWindowAcceptButton;
	sf::Sprite sellingWindowRejectButton;
	sf::Sprite sellingWindowScrollButton;
	sf::Sprite sellingWindowScrollLine;
	sf::Text sellingWindowProvenance;
	sf::Text sellingWindowPrice[3];
	sf::Text sellingWindowRessourceQuantity[4];

	int sellingWindowScrollButtonPosition;
	int quantityConvertedToSell;
	int priceAccepted;
	bool wasCursorPressed;

public:
	SellingWindow(sf::Font *_font);
	~SellingWindow();

	void UpdateQuantityConvertedToSell(Purchasers *_purchasers);
	void UpdateSellingWindowTexts(Purchasers *_purchasers);
	void DisplaySellingWindow(sf::RenderWindow &_window);
	void InputSellingWindow(bool *_isOfferAccepted, enum GameState *_state, Stalls *_stall, sf::RenderWindow &_window);
};



#endif // !SELLING_WINDOW__H