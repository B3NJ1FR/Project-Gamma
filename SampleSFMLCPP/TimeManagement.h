#ifndef TIME_MANAGEMENT__H
#define TIME_MANAGEMENT__H

#include "Common.hpp"



//	Ianuarius : (mars) 31 jours en l'honneur du dieu romain Janus
//	Februarius : (mars) 28 jours 
//	Martius : (mars) 31 jours en l'honneur du dieu romain Mars
//	Aprilis : (avril) 30 jours d�di�s � la d�esse grecque Aphrodite, et d�signant l'ouverture de l'ann�e
//	Ma�us : (mai) 31 jours en l'honneur des s�nateurs romains ou maiores
//	Iunius : (juin) 30 jours en l'honneur de la d�esse romaine Junon.
//	Quintilis : (juillet) 31 jours qui deviendra juillet en l'honneur Jules C�sar.
//	Sextlis : (ao�t) 31 jours qui deviendra ao�t en l'honneur de l'empereur Auguste.
//	September : (septembre) 30 jours
//	October : (octobre) 31 jours
//	November : (novembre) 30 jours
//	December : (d�cembre) 31 jours

class TimeManagement
{
private:
	sf::Clock gameClock;
	sf::Time frameTime;
	sf::Time timer;

public:
	TimeManagement();
	~TimeManagement();

	struct Timer
	{
		int a;
		int b;
		int c;
	};

	void UpdateFrameTime();
	void UpdateGeneralTime();

	float GetFrameTime();
	float GetGeneralTime();
};


#endif // !TIME_MANAGEMENT__H
