#ifndef _MAIN_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define _MAIN_

//Valeurs générales
#define NB_JOUEURS_MAX 4
#define NB_STATION_MAX 10
#define NB_STATIONS_INITIAL 3
#define TARIF_BUS_BASE 10
#define NB_VOYAGEUR_MAX_VOITURE 5

//Prix des améliorations
#define PRIX_BUS 100
#define PRIX_VOITURE_BUS 50
#define PRIX_AUGM_TAILLE_BUS 100
#define PRIX_AUGM_VITESSE_BUS 200
#define PRIX_AUGM_TARIF_BUS 100

//Infos sur les amélioration (nottament les limitations)
#define NB_BUS_MAX_JOUEUR 4
#define TAILLE_BUS_MAX 3
#define VITESSE_BUS_MAX 3
#define NB_AUGM_TARIF_BUS_MAX 5
#define VALEUR_AUGM_TARIF_BUS 2


/*
* Structures générales représentants les différents acteurs du jeu
*/

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


/*
*Variables globales du jeu
*/

int monIdJoueur;
int nbJoueurs;
int nbStations; 
int nbBus;
int nbNouveauxVoyageurs;
int nbVoyageursDescendus;
int nbVoyageursMontes;
int nbVoyageursActuel;
int tailleListeVoyageur;
Joueur listeJoueurs[NB_JOUEURS_MAX];
Station listeStations[NB_STATION_MAX];
Bus listeBus[NB_JOUEURS_MAX * NB_BUS_MAX_JOUEUR];


void joue();


#endif