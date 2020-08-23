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
//	-	Juillet - Entretien
//		Août - Surveillance
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
		float secondaryTime;

		bool isChangingSprite;
		bool isProduced;
		bool isWorkerThere;

		bool isPruned;
		bool isPloughed;
		bool isWeeded;
		bool isCared;
	};
	typedef struct VinesData sVines;

	Vines();
	~Vines();

	void InitialisationVines(Buildings *_vine);
	void ReadVineLinkedList();
	void AddNewVineToList(sf::Vector2f _mapPosition);
	void UpdateVineLife(const float &_frametime, const enum MonthsInOneYear &_actualMonth);
	void UpdateVineSprite(unsigned short ***_map);
	void UpdateVineProduction(Ressources *_ressource);

	bool ConfirmVinePresenceAtWorkerPosition(const sf::Vector2f &_mapPosition);
	bool CheckVineHasProducedRessource(const sf::Vector2f &_mapPosition);
	int VinesSendRessourceProducedToPresentWorker(const sf::Vector2f &_mapPosition, const float &_frametime);

	bool DestroyedBuildingSelected(const sf::Vector2f &_mapPosition);

	void SavingVinesListForFile(std::ofstream *_file);
	void LoadingVinesListFromFile(std::ifstream *_file);
private:
	LinkedListClass::sLinkedList *list;
	Buildings *vineBuilding;
};

#endif // !VINES__H