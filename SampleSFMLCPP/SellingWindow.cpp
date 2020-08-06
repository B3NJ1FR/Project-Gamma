#include "SellingWindow.h"
#include "GameDefinitions.h"


SellingWindow::SellingWindow(sf::Font *_font)
{
	this->sellingWindowBackground = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_background.png", 1);

	this->sellingWindowAcceptButton = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_acceptButton.png", 1);
	this->sellingWindowRejectButton = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_refuseButton.png", 1);

	this->sellingWindowScrollButton = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_ScrollButton.png", 1);
	this->sellingWindowScrollLine = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_ScrollLine.png", 1);

	LoadTextString(&this->sellingWindowProvenance, "Nemausus", _font, 50, sf::Color::Black, sf::Vector2f(1920 / 2, 1080 / 2 - 150));

	LoadTextString(&this->sellingWindowPrice[0], "", _font, 30, sf::Color::Black, sf::Vector2f(1920 / 2 + 270, 1080 / 2 - 50));
	LoadTextString(&this->sellingWindowPrice[1], "", _font, 30, sf::Color::Black, sf::Vector2f(1920 / 2 - 300, 1080 / 2 - 50));
	LoadTextString(&this->sellingWindowPrice[2], "", _font, 50, sf::Color::Black, sf::Vector2f(1920 / 2, 1080 / 2 - 80));

	LoadTextString(&this->sellingWindowRessourceQuantity[0], "", _font, 30, sf::Color::Black, sf::Vector2f(1920 / 2 - 300, 1080 / 2 - 20));
	LoadTextString(&this->sellingWindowRessourceQuantity[1], "", _font, 30, sf::Color::Black, sf::Vector2f(1920 / 2 + 270, 1080 / 2 - 20));
	LoadTextString(&this->sellingWindowRessourceQuantity[2], "Quantity to sell :", _font, 30, sf::Color::Black, sf::Vector2f(1920 / 2 - 250, 1080 / 2 + 20));
	LoadTextString(&this->sellingWindowRessourceQuantity[3], "", _font, 30, sf::Color::Black, sf::Vector2f(1920 / 2, 1080 / 2 + 20));

	this->sellingWindowScrollButtonPosition = (int)((1920 / 2) - (this->sellingWindowScrollLine.getGlobalBounds().width / 2));
	this->wasCursorPressed = false;
	this->quantityConvertedToSell = this->sellingWindowScrollButtonPosition;
	this->priceAccepted = RESET;

}

SellingWindow::~SellingWindow()
{

}

void SellingWindow::UpdateQuantityConvertedToSell(Purchasers *_purchasers, const int &_quantityStocked)
{
	// If the cursor is at the end of the line
	if (_quantityStocked >= _purchasers->GetUnitQuantityRessourceScope().y
		&& (this->sellingWindowScrollButtonPosition - ((1920 / 2) - (this->sellingWindowScrollLine.getGlobalBounds().width / 2))) >= this->sellingWindowScrollLine.getGlobalBounds().width - 2)
	{
		this->quantityConvertedToSell = _purchasers->GetUnitQuantityRessourceScope().y;
		this->priceAccepted = _purchasers->GetUnitPriceScope().x;
	}
	// If the cursor is at the begin of the line
	else if ((this->sellingWindowScrollButtonPosition - ((1920 / 2) - (this->sellingWindowScrollLine.getGlobalBounds().width / 2))) == 0)
	{
		this->quantityConvertedToSell = _purchasers->GetUnitQuantityRessourceScope().x;
		this->priceAccepted = _purchasers->GetUnitPriceScope().y;
	}
	else
	{
		int percentage = (int)(((this->sellingWindowScrollButtonPosition - ((1920 / 2) - (this->sellingWindowScrollLine.getGlobalBounds().width / 2))) * 100) / (((1920 / 2) + (this->sellingWindowScrollLine.getGlobalBounds().width / 2)) - ((1920 / 2) - (this->sellingWindowScrollLine.getGlobalBounds().width / 2))));
		
		int temporaryQuantityConverted = (int)((percentage * (_purchasers->GetUnitQuantityRessourceScope().y - _purchasers->GetUnitQuantityRessourceScope().x)) / 100) + _purchasers->GetUnitQuantityRessourceScope().x;

		if (_quantityStocked < _purchasers->GetUnitQuantityRessourceScope().y)
		{
			// Bloquer le curseur à la quantité stockée

			if (temporaryQuantityConverted >= _quantityStocked)
			{
				this->quantityConvertedToSell = _quantityStocked;
				//this->priceAccepted = (int)(((100 - percentage) * (_purchasers->GetUnitPriceScope().y - _purchasers->GetUnitPriceScope().x)) / 100) + _purchasers->GetUnitPriceScope().x;
			}
			else
			{
				this->quantityConvertedToSell = temporaryQuantityConverted;
				this->priceAccepted = (int)(((100 - percentage) * (_purchasers->GetUnitPriceScope().y - _purchasers->GetUnitPriceScope().x)) / 100) + _purchasers->GetUnitPriceScope().x;

			}
			
		}
		this->quantityConvertedToSell = 


	}

	//std::cout << "Quantity : " << (this->sellingWindowScrollButtonPosition - ((1920 / 2) - (this->sellingWindowScrollButton.getGlobalBounds().width / 2))) << std::endl;
	//std::cout << "Quantity : " << this->quantityConvertedToSell << std::endl;
}

void SellingWindow::UpdateSellingWindowTexts(Purchasers *_purchasers)
{
	UpdateDynamicsTexts(&this->sellingWindowPrice[0], _purchasers->GetUnitPriceScope().x);
	UpdateDynamicsTexts(&this->sellingWindowPrice[1], _purchasers->GetUnitPriceScope().y);
	UpdateDynamicsTexts(&this->sellingWindowPrice[2], this->priceAccepted);

	UpdateDynamicsTexts(&this->sellingWindowRessourceQuantity[0], _purchasers->GetUnitQuantityRessourceScope().x);
	UpdateDynamicsTexts(&this->sellingWindowRessourceQuantity[1], _purchasers->GetUnitQuantityRessourceScope().y);
	UpdateDynamicsTexts(&this->sellingWindowRessourceQuantity[3], this->quantityConvertedToSell);
}

void SellingWindow::DisplaySellingWindow(sf::RenderWindow &_window)
{
	// Display of the background
	BlitSprite(this->sellingWindowBackground, 1920 / 2, 1080 / 2, 0, _window);

	BlitSprite(this->sellingWindowAcceptButton, 1920 / 2 + 200, 1080 / 2 + 125, 0, _window);
	BlitSprite(this->sellingWindowRejectButton, 1920 / 2 - 200, 1080 / 2 + 125, 0, _window);

	BlitSprite(this->sellingWindowScrollLine, 1920 / 2, 1080 / 2, 0, _window);
	BlitSprite(this->sellingWindowScrollButton, this->sellingWindowScrollButtonPosition, 1080 / 2, 0, _window);
	
	BlitString(this->sellingWindowProvenance, _window);


	// Display of the prices texts
	BlitString(this->sellingWindowPrice[0], _window);
	BlitString(this->sellingWindowPrice[1], _window);
	BlitString(this->sellingWindowPrice[2], _window);

	// Display of the ressources quantities texts
	BlitString(this->sellingWindowRessourceQuantity[0], _window);
	BlitString(this->sellingWindowRessourceQuantity[1], _window);
	BlitString(this->sellingWindowRessourceQuantity[2], _window);
	BlitString(this->sellingWindowRessourceQuantity[3], _window);
}


void SellingWindow::InputSellingWindow(bool *_isOfferAccepted, enum GameState *_state, Stalls *_stall, sf::RenderWindow &_window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);

	// Button Accept
	if (mousePosition.x > 1920 / 2 + 200 - (this->sellingWindowAcceptButton.getGlobalBounds().width / 2)
		&& mousePosition.x < 1920 / 2 + 200 + (this->sellingWindowAcceptButton.getGlobalBounds().width / 2)
		&& mousePosition.y > 1080 / 2 + 125 - (this->sellingWindowAcceptButton.getGlobalBounds().height / 2)
		&& mousePosition.y < 1080 / 2 + 125 + (this->sellingWindowAcceptButton.getGlobalBounds().height / 2))
	{
		std::cout << "Marchant offer accepted !\n\n";

		*(_isOfferAccepted) = true;
		_stall->PickUpPriceAccepted(this->priceAccepted);

		*(_state) = NORMAL_MODE;
		_stall->SetStatus(STALL_OFFER_HANDLED);
	}

	// Button Refuse	
	if (mousePosition.x > 1920 / 2 - 200 - (this->sellingWindowRejectButton.getGlobalBounds().width / 2)
		&& mousePosition.x < 1920 / 2 - 200 + (this->sellingWindowRejectButton.getGlobalBounds().width / 2)
		&& mousePosition.y > 1080 / 2 + 125 - (this->sellingWindowRejectButton.getGlobalBounds().height / 2)
		&& mousePosition.y < 1080 / 2 + 125 + (this->sellingWindowRejectButton.getGlobalBounds().height / 2))
	{
		std::cout << "Marchant offer refused !\n\n";

		*(_isOfferAccepted) = false;
		_stall->PickUpPriceAccepted(this->priceAccepted);

		*(_state) = NORMAL_MODE;
		_stall->SetStatus(STALL_OFFER_HANDLED);
	}


	// Click and Drag
	// Click à un endroit de la ligne sur la hauteur du curseur

	// Cursor movement
	if (mousePosition.x > this->sellingWindowScrollButtonPosition - (this->sellingWindowScrollButton.getGlobalBounds().width / 2)
		&& mousePosition.x < this->sellingWindowScrollButtonPosition + (this->sellingWindowScrollButton.getGlobalBounds().width / 2)
		&& mousePosition.x > 1920 / 2 - this->sellingWindowScrollLine.getGlobalBounds().width / 2
		&& mousePosition.x < 1920 / 2 + this->sellingWindowScrollLine.getGlobalBounds().width / 2
		&& mousePosition.y > 1080 / 2 - (this->sellingWindowScrollButton.getGlobalBounds().height / 2)
		&& mousePosition.y < 1080 / 2 + (this->sellingWindowScrollButton.getGlobalBounds().height / 2))
	{
		if (this->wasCursorPressed == true)
		{
			this->sellingWindowScrollButtonPosition = mousePosition.x;
		}

		this->wasCursorPressed = true;


		std::cout << "Drag cursor\n";
	}
	else if (mousePosition.x > 1920 / 2 - this->sellingWindowScrollLine.getGlobalBounds().width / 2
		&& mousePosition.x < 1920 / 2 + this->sellingWindowScrollLine.getGlobalBounds().width / 2
		&& mousePosition.y > 1080 / 2 - (this->sellingWindowScrollButton.getGlobalBounds().height / 2) - 10
		&& mousePosition.y < 1080 / 2 + (this->sellingWindowScrollButton.getGlobalBounds().height / 2) + 10)
	{
		if (this->wasCursorPressed == true)
		{
			this->sellingWindowScrollButtonPosition = mousePosition.x;
		}

		this->wasCursorPressed = true;


		std::cout << "Drag cursor\n";
	}
	else
	{
		std::cout << "Dont drag cursor\n";
		this->wasCursorPressed = false;
	}

}
