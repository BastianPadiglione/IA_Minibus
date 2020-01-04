#include "recuperationDeDonnees.h"
#include "main.h"
#include "gestionListeChaine.h"

void  initialisePartie(){
    
    scanf("%d %d", &nbJoueurs, &monIdJoueur);
    nbStations = NB_STATIONS_INITIAL;

    //pas de voyageur au début
    nbVoyageursActuel=0;
    initialiseListeVoyageur(); 

    for(int i = 0; i < nbStations; i++){
        scanf("%d %d %d %d", &listeStations[i].idStation, &listeStations[i].x, &listeStations[i].y, &listeStations[i].capacite);
    }
}

void initialiseTour(){
    initialiseJoueurs();
    initialiseStations();
    initialiseBus();
    initialiseVoyageurs();
}

void initialiseJoueurs(){
    
    for(int i = 0; i<nbJoueurs; i++){
        scanf("%d %d %d %d %d %d", &listeJoueurs[i].idJoueur, &listeJoueurs[i].argent, &listeJoueurs[i].nbAmeliorationSB, &listeJoueurs[i].nbAmeliorationSP, &listeJoueurs[i].nbAmeliorationCT, &listeJoueurs[i].etat);
    }
}

void initialiseStations(){

    int nouvelleStation;

    scanf("%d", &nouvelleStation);

    if(nouvelleStation){
        stationApparu = true;
        scanf("%d %d %d %d", &listeStations[nbStations].idStation, &listeStations[nbStations].x, &listeStations[nbStations].y, &listeStations[nbStations].capacite);
        nbStations++;
    }
    else{
        stationApparu = false;
    }
    
    
}

void initialiseBus(){
    
    scanf("%d",&nbBus);

    for(int i=0; i < nbBus; i++){
        scanf("%d %d %d %d %d %d", &listeBus[i].idBus, &listeBus[i].idJoueur, &listeBus[i].x, &listeBus[i].y, &listeBus[i].stationDeDirection, &listeBus[i].nbVoiture);

        //parametres implicites basés sur les améliorations des joueurs
        //1 car 1 voiture à la base
        listeBus[i].nbMaxVoiture = 1 + listeJoueurs[listeBus[i].idJoueur].nbAmeliorationSB;
        //1 car vitesse de base = 1
        listeBus[i].vitesse = 1 + listeJoueurs[listeBus[i].idJoueur].nbAmeliorationSP;
        listeBus[i].tarif = TARIF_BUS_BASE + (VALEUR_AUGM_TARIF_BUS * listeJoueurs[listeBus[i].idJoueur].nbAmeliorationCT);
        
        //si le bus est à sa station de destination
        if(listeBus[i].x == getStation(listeBus[i].stationDeDirection).x
            && listeBus[i].y == getStation(listeBus[i].stationDeDirection).y){

             listeBus[i].arrete = true;
        }
        else{
            listeBus[i].arrete = false;
        }  
    }
}

void initialiseVoyageurs(){

    scanf("%d %d %d", &nbNouveauxVoyageurs, &nbVoyageursMontes, &nbVoyageursDescendus);
    nbVoyageursActuel += (nbNouveauxVoyageurs - nbVoyageursDescendus);

    ajouteNouveauxVoyageurs();
    faisMonterVoyageurs();
    faisDecendreVoyageurs();
}

void ajouteNouveauxVoyageurs(){
    Voyageur * nouveau;
    for(int i = 0; i<nbNouveauxVoyageurs; i++){
        
        nouveau = (Voyageur *)malloc(sizeof(Voyageur));
       
        scanf("%d %d %d",&nouveau->idVoyageur, &nouveau->idStationDepart, &nouveau->idStationArrivee);

        nouveau->idBus = -1;
        ajouteVoyageur(nouveau);
    }
}

void faisMonterVoyageurs(){

    int idVoyageur, idBus;
    Voyageur * v;

    for(int i = 0; i<nbVoyageursMontes; i++){
        scanf("%d %d", &idVoyageur, &idBus);
        v=getVoyageur(idVoyageur);
        v->idBus = idBus;
    }
}

void faisDecendreVoyageurs(){

    int idVoyageurDescendu;
    for(int i = 0; i<nbVoyageursDescendus; i++){
        scanf("%d", &idVoyageurDescendu);
        supprimeVoyageur(idVoyageurDescendu);
    }
}

