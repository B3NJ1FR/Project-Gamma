#pragma once

#include "Common.hpp"

#define TEMPORARY_TIME 60
#define OLD_TIME (2 * 3600)

enum MonthsInOneYear
{
	IANUARIUS,
	FEBRUARIUS,
	MARTIUS,
	APRILIS,
	MAïUS,
	IUNIUS,
	QUINTILIS,
	SEXTILIS,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER,
};

//	Ianuarius : (mars) 31 jours en l'honneur du dieu romain Janus
//	Februarius : (mars) 28 jours 
//	Martius : (mars) 31 jours en l'honneur du dieu romain Mars
//	Aprilis : (avril) 30 jours dédiés à la déesse grecque Aphrodite, et désignant l'ouverture de l'année
//	Maïus : (mai) 31 jours en l'honneur des sénateurs romains ou maiores
//	Iunius : (juin) 30 jours en l'honneur de la déesse romaine Junon.
//	Quintilis : (juillet) 31 jours qui deviendra juillet en l'honneur Jules César.
//	Sextilis : (août) 31 jours qui deviendra août en l'honneur de l'empereur Auguste.
//	September : (septembre) 30 jours
//	October : (octobre) 31 jours
//	November : (novembre) 30 jours
//	December : (décembre) 31 jours

enum class TypeOfTimeAcceleration
{
	GAME_PAUSE,
	GAME_NORMAL_SPEED,
	GAME_DOUBLE_SPEED,
	GAME_TRIPLE_SPEED,
};

class TimeManagement
{
private:
	sf::Clock m_gameClock;
	sf::Time m_frameTime;
	sf::Time m_timer;

	bool m_isMonthHasChanged;
	bool m_isYearHasChanged;

	enum MonthsInOneYear m_actualMonth;
	int m_numberOfYears;

	sf::Text m_monthText;
	sf::Text yearText;

	short int m_accelerator;
	enum TypeOfTimeAcceleration m_typeOfAcceleration;

	sf::Sprite m_timesSprite[4];
	sf::Sprite m_sundial;
	sf::Sprite m_sundialArrowIndicator;

	// Constructor
	TimeManagement();

public:
	// Destructor
	~TimeManagement();

	void Initialisation(sf::Font* _font, const sf::Vector2i& _screenResolution);
	static TimeManagement* GetSingleton();

	// Setters
	inline void SetAccelerator(const short int& _accelerator);
	void SetTypeOfAcceleration(const enum TypeOfTimeAcceleration& _typeOfAcceleration);

	// Getters
	inline short int GetAccelerator() const { return m_accelerator; };
	inline enum TypeOfTimeAcceleration GetTypeOfAcceleration() const { return m_typeOfAcceleration; };
	inline float GetFrameTime() const { return m_frameTime.asSeconds() * m_accelerator; };
	inline float GetContinuousFrameTime()const { return m_frameTime.asSeconds(); };
	inline float GetGeneralTime() const { return m_timer.asSeconds(); };
	inline enum MonthsInOneYear GetCurrentMonth() const { return m_actualMonth; };
	//inline int GetCurrentYear() const { return 7; };
	inline int GetCurrentYear() const { return m_numberOfYears; };

	// Methods
	void UpdateFrameTime();
	void UpdateGeneralTime();

	void InputTimeManagement(sf::RenderWindow &_window, const sf::Vector2i& _screenResolution);
	void UpdateMonthToDisplay();
	void DisplayTimeChanger(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);
	void DisplayCalendar(sf::RenderWindow& _window, const sf::Vector2i& _screenResolution);

	void SavingTimeFromFile(std::ofstream *_file);
	void LoadingTimeFromFile(std::ifstream *_file);
};