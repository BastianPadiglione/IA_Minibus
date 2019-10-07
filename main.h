#ifndef _MAIN_

#define _MAIN_
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


void joue();


#endif