#include "Purse.h"
#include "Fonts.h"


Purse::Purse()
{
	m_papyrusBackground = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/Purse/purse_background.png", 1);
	m_returnButton = LoadSprite("Data/Assets/Sprites/Menu/VillaManagement/return_button.png", 1, true);
	m_returnButton.setScale(sf::Vector2f(0.65f, 0.65f));

	m_font = Fonts::GetSingleton()->GetCharlemagneFont();

	sf::Color colorYellow = { 236, 150, 55, 255 };
	LoadTextString(&m_textMoneyName, "Sesterces : ", &m_font, 55, colorYellow, 1);
	LoadTextString(&m_textQtyOfMoney, "", &m_font, 55, colorYellow, 1);

	internalState = InternalState::STATE_INIT;
}

Purse::~Purse()
{
}

Purse* Purse::GetSingleton()
{
	static Purse uniqueInstance;
	return &uniqueInstance;
}

void Purse::Input(sf::Event _event, sf::RenderWindow& _window, const sf::Vector2i& _screenResolution)
{
	if (_event.type == sf::Event::KeyPressed
		&& _event.key.code == sf::Keyboard::Escape)
	{
		internalState = InternalState::STATE_EXIT;
	}

	if (_event.type == sf::Event::MouseButtonPressed
		&& _event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);
		sf::Vector2f screenCenter = sf::Vector2f(_screenResolution.x / 2, _screenResolution.y / 2);

		// Button to return to the villa
		if (mousePosition.x > screenCenter.x - (m_returnButton.getGlobalBounds().width / 2)
			&& mousePosition.x < screenCenter.x + (m_returnButton.getGlobalBounds().width / 2)
			&& mousePosition.y > screenCenter.y + (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) - 10.0f - (m_returnButton.getGlobalBounds().height / 2)
			&& mousePosition.y < screenCenter.y + (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) - 10.0f + (m_returnButton.getGlobalBounds().height / 2))
		{
			internalState = InternalState::STATE_EXIT;
		}
	}
}

void Purse::UpdateTextsContent()
{
	UpdateDynamicsTexts(&m_textQtyOfMoney, Money::GetSingleton()->GetMoneyQuantity());
	ChangeTextStringOrigin(&m_textQtyOfMoney, 4);
}

void Purse::Update(VillaManagementStateMachine* _internalStateMachine)
{
	switch (internalState)
	{
	case InternalState::STATE_INIT:
		UpdateTextsContent();

		internalState = InternalState::STATE_UPDATE;
		break;

	case InternalState::STATE_UPDATE:
		break;

	case InternalState::STATE_EXIT:
		internalState = InternalState::STATE_INIT;
		*_internalStateMachine = VillaManagementStateMachine::NORMAL_STATE;
		break;

	case InternalState::STATE_WAITING:
		break;

	default:
		break;
	}
}

void Purse::Display(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution)
{
	if (internalState != InternalState::STATE_UPDATE) return;

	sf::Vector2f screenCenter = sf::Vector2f(_screenResolution.x / 2, _screenResolution.y / 2);

	// Display of the background
	BlitSprite(m_papyrusBackground, screenCenter.x, screenCenter.y, 0, _window);

	
	// Display papyrus title & quantity
	BlitString(m_textMoneyName, screenCenter.x - 150.0f, screenCenter.y, _window);
	BlitString(m_textQtyOfMoney, screenCenter.x + 275.0f, screenCenter.y, _window);

	// Display the money at the end of the line
	sf::Sprite spriteSesterce = Money::GetSingleton()->GetSprite();
	spriteSesterce.setScale(sf::Vector2f(0.85f, 0.85f));
	BlitSprite(spriteSesterce, screenCenter.x + 325.0f, screenCenter.y + 10.0f, _window);

	// Display the return button
	BlitSprite(m_returnButton, screenCenter.x, screenCenter.y + (float)(m_papyrusBackground.getGlobalBounds().height / 2.0f) - 10.0f, _window);
}
