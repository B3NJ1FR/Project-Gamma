#include "SellingWindow.h"
#include "GameDefinitions.h"


SellingWindow::SellingWindow(sf::Font *_font, const sf::Vector2i &_screenResolution)
{
	m_sellingWindowBackground = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_background.png", 1);

	m_sellingWindowAcceptButton = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_acceptButton.png", 1);
	m_sellingWindowRejectButton = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_refuseButton.png", 1);

	m_sellingWindowScrollButton = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_ScrollButton.png", 1);
	m_sellingWindowScrollLine = LoadSprite("Data/Assets/Sprites/Menu/SellingWindow/sellingWindow_ScrollLine.png", 1);


	LoadTextString(&m_sellingWindowProvenance, "", _font, 50, sf::Color::Black, sf::Vector2f(_screenResolution.x / 2, _screenResolution.y / 2 - 150));

	LoadTextString(&m_sellingWindowPrice[0], "", _font, 30, sf::Color::Black, sf::Vector2f(_screenResolution.x / 2 + 270, _screenResolution.y / 2 - 50));
	LoadTextString(&m_sellingWindowPrice[1], "", _font, 30, sf::Color::Black, sf::Vector2f(_screenResolution.x / 2 - 300, _screenResolution.y / 2 - 50));
	LoadTextString(&m_sellingWindowPrice[2], "", _font, 50, sf::Color::Black, sf::Vector2f(_screenResolution.x / 2, _screenResolution.y / 2 - 80));

	LoadTextString(&m_sellingWindowRessourceQuantity[0], "", _font, 30, sf::Color::Black, sf::Vector2f(_screenResolution.x / 2 - 300, _screenResolution.y / 2 - 20));
	LoadTextString(&m_sellingWindowRessourceQuantity[1], "", _font, 30, sf::Color::Black, sf::Vector2f(_screenResolution.x / 2 + 270, _screenResolution.y / 2 - 20));
	LoadTextString(&m_sellingWindowRessourceQuantity[2], "Quantity to sell :", _font, 30, sf::Color::Black, sf::Vector2f(1920 / 2 - 250, _screenResolution.y / 2 + 20));
	LoadTextString(&m_sellingWindowRessourceQuantity[3], "", _font, 30, sf::Color::Black, sf::Vector2f(_screenResolution.x / 2, _screenResolution.y / 2 + 20));


	m_sellingWindowScrollButtonPosition = (int)((_screenResolution.x / 2) - (m_sellingWindowScrollLine.getGlobalBounds().width / 2));
	m_sellingWindowScrollButtonMax = (int)((_screenResolution.x / 2) + (m_sellingWindowScrollLine.getGlobalBounds().width / 2));
	m_wasCursorPressed = false;
	m_quantityConvertedToSell = m_sellingWindowScrollButtonPosition;
	m_priceAccepted = RESET;

}

SellingWindow::~SellingWindow()
{

}


void SellingWindow::UpdateQuantityConvertedToSell(Purchasers *_purchasers, const int &_quantityStocked, const sf::Vector2i& _screenResolution)
{
	// If the cursor is at the end of the line and the quantity stocked is higher than it asked
	if (_quantityStocked >= _purchasers->GetUnitQuantityRessourceScope().y
		&& (m_sellingWindowScrollButtonPosition - ((_screenResolution.x / 2) - (m_sellingWindowScrollLine.getGlobalBounds().width / 2))) >= m_sellingWindowScrollLine.getGlobalBounds().width - 2)
	{
		m_quantityConvertedToSell = _purchasers->GetUnitQuantityRessourceScope().y;
		m_priceAccepted = _purchasers->GetUnitPriceScope().x;
	}
	// If the cursor is at the begin of the line and thus at the minimal quantity
	else if ((m_sellingWindowScrollButtonPosition - ((_screenResolution.x / 2) - (m_sellingWindowScrollLine.getGlobalBounds().width / 2))) == 0)
	{
		m_quantityConvertedToSell = _purchasers->GetUnitQuantityRessourceScope().x;
		m_priceAccepted = _purchasers->GetUnitPriceScope().y;
	}
	else
	{
		int percentage = (int)(((m_sellingWindowScrollButtonPosition - ((_screenResolution.x / 2) - (m_sellingWindowScrollLine.getGlobalBounds().width / 2))) * 100) / (((_screenResolution.x / 2) + (m_sellingWindowScrollLine.getGlobalBounds().width / 2)) - ((_screenResolution.x / 2) - (m_sellingWindowScrollLine.getGlobalBounds().width / 2))));
		
		int temporaryQuantityConverted = (int)((percentage * (_purchasers->GetUnitQuantityRessourceScope().y - _purchasers->GetUnitQuantityRessourceScope().x)) / 100) + _purchasers->GetUnitQuantityRessourceScope().x;

		// In the case if we've stocked less object than the purchaser can buy
		if (_quantityStocked < _purchasers->GetUnitQuantityRessourceScope().y)
		{
			if (m_sellingWindowScrollButtonMax == (int)((_screenResolution.x / 2) + (m_sellingWindowScrollLine.getGlobalBounds().width / 2)))
			{
				// We block the cursor at the stocked quantity and not higher
				float temporaryPercentage = 100 * (_quantityStocked - _purchasers->GetUnitQuantityRessourceScope().x) / (_purchasers->GetUnitQuantityRessourceScope().y - _purchasers->GetUnitQuantityRessourceScope().x);
				m_sellingWindowScrollButtonMax = (int)((temporaryPercentage * m_sellingWindowScrollLine.getGlobalBounds().width) / 100) + 1;
			}
			

			// If the cursor atteign the max quantity stocked
			if ((m_sellingWindowScrollButtonPosition - ((_screenResolution.x / 2) - (m_sellingWindowScrollLine.getGlobalBounds().width / 2))) >= m_sellingWindowScrollButtonMax - 1)
			{
				m_quantityConvertedToSell = _quantityStocked;
			}
			else
			{
				m_quantityConvertedToSell = temporaryQuantityConverted;
			}
			
			m_priceAccepted = (int)(((100 - percentage) * (_purchasers->GetUnitPriceScope().y - _purchasers->GetUnitPriceScope().x)) / 100) + _purchasers->GetUnitPriceScope().x;
		}
		else
		{
			m_quantityConvertedToSell = temporaryQuantityConverted;

			m_priceAccepted = (int)(((100 - percentage) * (_purchasers->GetUnitPriceScope().y - _purchasers->GetUnitPriceScope().x)) / 100) + _purchasers->GetUnitPriceScope().x;
		}
	}
}

void SellingWindow::UpdateSellingWindowTexts(Purchasers *_purchasers)
{
	// Update Purchaser's Provenance
	m_sellingWindowProvenance.setString(_purchasers->GetProvenanceName());

	// Update of the prices texts
	UpdateDynamicsTexts(&m_sellingWindowPrice[0], _purchasers->GetUnitPriceScope().x);
	UpdateDynamicsTexts(&m_sellingWindowPrice[1], _purchasers->GetUnitPriceScope().y);
	UpdateDynamicsTexts(&m_sellingWindowPrice[2], m_priceAccepted);

	// Update of the ressources quantities texts
	UpdateDynamicsTexts(&m_sellingWindowRessourceQuantity[0], _purchasers->GetUnitQuantityRessourceScope().x);
	UpdateDynamicsTexts(&m_sellingWindowRessourceQuantity[1], _purchasers->GetUnitQuantityRessourceScope().y);
	UpdateDynamicsTexts(&m_sellingWindowRessourceQuantity[3], m_quantityConvertedToSell);
}

void SellingWindow::DisplaySellingWindow(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution)
{
	// Display of the background
	BlitSprite(m_sellingWindowBackground, _screenResolution.x / 2, _screenResolution.y / 2, 0, _window);
	
	// Display of the accept and reject buttons
	BlitSprite(m_sellingWindowAcceptButton, _screenResolution.x / 2 + 200, _screenResolution.y / 2 + 125, 0, _window);
	BlitSprite(m_sellingWindowRejectButton, _screenResolution.x / 2 - 200, _screenResolution.y / 2 + 125, 0, _window);

	// Display of the scroll's cursor and line
	BlitSprite(m_sellingWindowScrollLine, _screenResolution.x / 2, _screenResolution.y / 2, 0, _window);
	BlitSprite(m_sellingWindowScrollButton, m_sellingWindowScrollButtonPosition, _screenResolution.y / 2, 0, _window);
	

	// Display of the merchant provenance
	BlitString(m_sellingWindowProvenance, _window);


	// Display of the prices texts
	BlitString(m_sellingWindowPrice[0], _window);
	BlitString(m_sellingWindowPrice[1], _window);
	BlitString(m_sellingWindowPrice[2], _window);

	// Display of the ressources quantities texts
	BlitString(m_sellingWindowRessourceQuantity[0], _window);
	BlitString(m_sellingWindowRessourceQuantity[1], _window);
	BlitString(m_sellingWindowRessourceQuantity[2], _window);
	BlitString(m_sellingWindowRessourceQuantity[3], _window);
}


void SellingWindow::InputSellingWindow(bool *_isOfferAccepted, enum CurrentGameState *_state, Stalls *_stall, sf::RenderWindow &_window, const sf::Vector2i& _screenResolution)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);

	// Button Accept
	if (mousePosition.x > _screenResolution.x / 2 + 200 - (m_sellingWindowAcceptButton.getGlobalBounds().width / 2)
		&& mousePosition.x < _screenResolution.x / 2 + 200 + (m_sellingWindowAcceptButton.getGlobalBounds().width / 2)
		&& mousePosition.y > _screenResolution.y / 2 + 125 - (m_sellingWindowAcceptButton.getGlobalBounds().height / 2)
		&& mousePosition.y < _screenResolution.y / 2 + 125 + (m_sellingWindowAcceptButton.getGlobalBounds().height / 2))
	{
		std::cout << "Marchant offer accepted !\n\n";

		*(_isOfferAccepted) = true;
		*(_state) = NORMAL_MODE;

		_stall->PickUpPriceAccepted(m_priceAccepted);
		_stall->SetRessourceQuantityToSell(m_quantityConvertedToSell);
		_stall->SetStatus(STALL_OFFER_HANDLED);


		// Reset of the data for the next offer
		m_sellingWindowScrollButtonPosition = (int)((_screenResolution.x / 2) - (m_sellingWindowScrollLine.getGlobalBounds().width / 2));
		m_sellingWindowScrollButtonMax = (int)((_screenResolution.x / 2) + (m_sellingWindowScrollLine.getGlobalBounds().width / 2));
	}

	// Button Refuse	
	if (mousePosition.x > _screenResolution.x / 2 - 200 - (m_sellingWindowRejectButton.getGlobalBounds().width / 2)
		&& mousePosition.x < _screenResolution.x / 2 - 200 + (m_sellingWindowRejectButton.getGlobalBounds().width / 2)
		&& mousePosition.y > _screenResolution.y / 2 + 125 - (m_sellingWindowRejectButton.getGlobalBounds().height / 2)
		&& mousePosition.y < _screenResolution.y / 2 + 125 + (m_sellingWindowRejectButton.getGlobalBounds().height / 2))
	{
		std::cout << "Marchant offer refused !\n\n";

		*(_isOfferAccepted) = false;
		*(_state) = NORMAL_MODE;

		_stall->SetStatus(STALL_OFFER_HANDLED);

		// Reset of the data for the next offer
		m_sellingWindowScrollButtonPosition = (int)((_screenResolution.x / 2) - (m_sellingWindowScrollLine.getGlobalBounds().width / 2));
		m_sellingWindowScrollButtonMax = (int)((_screenResolution.x / 2) + (m_sellingWindowScrollLine.getGlobalBounds().width / 2));
	}

	// Cursor movement with the mouse on the cursor
	if (mousePosition.x > m_sellingWindowScrollButtonPosition - (m_sellingWindowScrollButton.getGlobalBounds().width / 2)
		&& mousePosition.x < m_sellingWindowScrollButtonPosition + (m_sellingWindowScrollButton.getGlobalBounds().width / 2)
		&& mousePosition.x > _screenResolution.x / 2 - m_sellingWindowScrollLine.getGlobalBounds().width / 2
		&& mousePosition.x < _screenResolution.x / 2 + m_sellingWindowScrollLine.getGlobalBounds().width / 2
		&& mousePosition.x < _screenResolution.x / 2 - (m_sellingWindowScrollLine.getGlobalBounds().width / 2) + m_sellingWindowScrollButtonMax
		&& mousePosition.y > _screenResolution.y / 2 - (m_sellingWindowScrollButton.getGlobalBounds().height / 2)
		&& mousePosition.y < _screenResolution.y / 2 + (m_sellingWindowScrollButton.getGlobalBounds().height / 2))
	{
		if (m_wasCursorPressed == true)
		{
			m_sellingWindowScrollButtonPosition = mousePosition.x;
		}

		m_wasCursorPressed = true;
	}
	// Cursor movement without the mouse on the cursor
	else if (mousePosition.x > _screenResolution.x / 2 - m_sellingWindowScrollLine.getGlobalBounds().width / 2
		&& mousePosition.x < _screenResolution.x / 2 + m_sellingWindowScrollLine.getGlobalBounds().width / 2
		&& mousePosition.x < _screenResolution.x / 2 - (m_sellingWindowScrollLine.getGlobalBounds().width / 2) + m_sellingWindowScrollButtonMax
		&& mousePosition.y > _screenResolution.y / 2 - (m_sellingWindowScrollButton.getGlobalBounds().height / 2) - 10
		&& mousePosition.y < _screenResolution.y / 2 + (m_sellingWindowScrollButton.getGlobalBounds().height / 2) + 10)
	{
		if (m_wasCursorPressed == true)
		{
			m_sellingWindowScrollButtonPosition = mousePosition.x;
		}

		m_wasCursorPressed = true;
	}
	else
	{
		m_wasCursorPressed = false;
	}
}