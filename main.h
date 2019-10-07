#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    int idJoueur, argent, nbAmeliorationSB, nbAmeliorationSP, nbAmeliorationCT, etat;
}Joueur;

typedef struct{
    int idStation, x, y, capacite;
}Station;

typedef struct{
    int idVoyageur, idStationDepart, idStationArrivee, idBus;
}Voyageur;

typedef struct{
    int idBus, idJoueur, x, y, stationDeDirection, nbVoiture, nbMaxVoiture, vitesse, tarif;
    bool arrete;
}Bus;


int idJoueur;
int nbJoueurs;
int nbStations; 
int nbBus;
int nbNouveauxVoyageurs;
int nbVoyageursDescendus;
int nbVoyageursMontes;
int nbVoyageursActuel;
int tailleListeVoyageur;
Joueur listeJoueurs[4];
Station listeStations[10];
Bus listeBus[16];


void initialisePartie();
void initialiseTour();
void initialiseJoueurs();
void initialiseStations();
void initialiseBus();
void initialiseVoyageurs();
void ajouteNouveauxVoyageurs();
void faisMonterVoyageurs();
void faisDecendreVoyageurs();


/************************************
 * Liste chainée de Voyageurs
*************************************/
typedef struct VoyageurListeVoyageur VoyageurListeVoyageur;

struct VoyageurListeVoyageur{
    Voyageur * voyageur;
    VoyageurListeVoyageur * suivant;
};

typedef struct{
    VoyageurListeVoyageur * premier;
}ListeVoyageurs;

ListeVoyageurs listeVoyageurs;

void initialiseListeVoyageur();
void ajouteVoyageur(Voyageur * v);
void supprimeVoyageur(int idVoyageur);
Voyageur * getVoyageur(int idVoyageur);