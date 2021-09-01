#pragma once

#include "Common.hpp"

#define NUMBER_OF_CARACTERS_PER_LINE 38

class TutorialWindow
{
private:

	sf::Sprite m_backgroundMessage;

	std::string *m_messages;

	sf::Text m_text;
	sf::Text m_textPressSpace;

	short int m_counterMessages;
	short int m_actualMessages;

	bool m_isMessageHasChanged;
	bool m_isTutorielDisplayFinished;

public:
	// Constructor & Destructor
	TutorialWindow(sf::Font *_font);
	~TutorialWindow();

	// Getters
	inline bool GetIsTutorialFinished() const { return m_isTutorielDisplayFinished; };

	// Methods
	void InitTextOfTutorialFromFile();

	void InputTutorialWindow(enum CurrentGameState *_state, sf::RenderWindow &_window);
	void UpdateTutorialWindow(enum CurrentGameState* _state, sf::Font *_font);
	void DisplayTutorialWindow(sf::RenderWindow &_window);

	// Save and Load
	void SavingTutorialProgressionForFile(std::ofstream* _file);
	void LoadingTutorialProgressionForFile(std::ifstream* _file);
};