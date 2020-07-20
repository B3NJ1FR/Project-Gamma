#ifndef VINES__H
#define VINES__H

#include "Ressources.h"
#include "Buildings.h"
#include "LinkedList.h"

#define VINE_LIFE_3Y_GROWTHING_TO_READY_TO_PRODUCE 12
#define VINE_LIFE_READY_TO_PRODUCE_TO_NOT_MAINTAINED


enum VineStateOfGeneralLife
{
	PLANTED,
	THREE_YEARS_GROWTHING,
	READY_TO_PRODUCE,
	NOT_MAINTAINED,
	TOO_OLD,
};

enum VineStateOfAnnualLife
{
	NEED_PRUNE, // Taille
	PRUNED,
	NEED_PLOUGHING, // Labourage
	PLOUGHED,
	NEED_WEEDING, // Désherbage
	WEEDED,
	NEED_CARE, // Entretien
	CARED,
	NEED_HARVEST, // Vendange
	HARVESTED,
	PUTTING_IN_WINE_STOREHOUSE, // Mise en Chai
};

// Calendrier des Vignes
//
//	-	Janvier - Taille de la vigne
//		Février - Préparer et bouturer
//	-	Mars - Labourer la terre
//		Avril - Planter et palisser
//	-	Mai - Désherber et Protéger du Mildiou et de l'oïdium 
//		Juin - Floraison
//		Juillet - Surveillance
//	-	Août - Entretien
//	-	Septembre - Vendanges
//	-	Octobre - Mise en Chai
//		Novembre - Préparer l'hiver
//		Décembre - Mise en repos de la vigne



class Vines : public LinkedListClass
{
public:

	struct VinesData
	{
		sf::Vector2f mapPosition;
		enum VineStateOfGeneralLife generalState;
		enum VineStateOfAnnualLife annualState;

		float lifeTime;
		float actualProductionTime;

		bool isChangingSprite;
		bool isProduced;
		bool isWorkerThere;
	};
	typedef struct VinesData sVines;

	Vines();
	~Vines();

	void InitialisationVines(Buildings *_vine);
	void ReadVineLinkedList();
	void AddNewVineToList(sf::Vector2f _mapPosition);
	void UpdateVineLife(const float &_frametime);
	void UpdateVineSprite(unsigned short ***_map);
	void UpdateVineProduction(Ressources *_ressource);
	bool CheckVinePresenceAtPosition(const sf::Vector2f &_mapPosition);

private:
	LinkedListClass::sLinkedList *list;
	Buildings *vineBuilding;
};

#endif // !VINES__H