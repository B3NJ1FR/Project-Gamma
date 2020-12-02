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
	int sellingWindowScrollButtonMax;
	int quantityConvertedToSell;
	int priceAccepted;
	bool wasCursorPressed;

public:
	SellingWindow(sf::Font *_font);
	~SellingWindow();

	// Function that manages the cursor data (quantity selected, minimals, maximals and current price)
	void UpdateQuantityConvertedToSell(Purchasers *_purchasers, const int &_quantityStocked);
	void UpdateSellingWindowTexts(Purchasers *_purchasers);
	void DisplaySellingWindow(sf::RenderWindow &_window);
	void InputSellingWindow(bool *_isOfferAccepted, enum CurrentGameState *_state, Stalls *_stall, sf::RenderWindow &_window);
};



#endif // !SELLING_WINDOW__H