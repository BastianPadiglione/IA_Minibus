#include "recuperationDeDonnees.h"
#include "main.h"
#include "gestionListeChaine.h"

void  initialisePartie(){
    
    scanf("%d %d", &nbJoueurs, &monIdJoueur);
    nbStations = 3;
    nbVoyageursActuel=0;
    initialiseListeVoyageur(); 

    fprintf(stderr,"Debut ini Station\n");
    for(int i = 0; i < 3; i++){
        scanf("%d %d %d %d", &listeStations[i].idStation, &listeStations[i].x, &listeStations[i].y, &listeStations[i].capacite);
        fprintf(stderr,"%d %d %d %d\n", listeStations[i].idStation, listeStations[i].x, listeStations[i].y, listeStations[i].capacite);
    }
    fprintf(stderr,"Fin ini Station\n");
}

void initialiseTour(){
    initialiseJoueurs();
    initialiseStations();
    initialiseBus();
    initialiseVoyageurs();
}

void initialiseJoueurs(){
    fprintf(stderr,"Debut liste joueurs\n");
    
    fprintf(stderr,"Nombre joueurs = %d\n", nbJoueurs);
    for(int i = 0; i<nbJoueurs; i++){
        scanf("%d %d %d %d %d %d", &listeJoueurs[i].idJoueur, &listeJoueurs[i].argent, &listeJoueurs[i].nbAmeliorationSB, &listeJoueurs[i].nbAmeliorationSP, &listeJoueurs[i].nbAmeliorationCT, &listeJoueurs[i].etat);
        
        fprintf(stderr,"%d %d %d %d %d %d\n", listeJoueurs[i].idJoueur, listeJoueurs[i].argent, listeJoueurs[i].nbAmeliorationSB, listeJoueurs[i].nbAmeliorationSP, listeJoueurs[i].nbAmeliorationCT, listeJoueurs[i].etat);
    }
    fprintf(stderr,"Fin liste joueurs\n");
}

void initialiseStations(){

    int nouvelleStation;

    scanf("%d", &nouvelleStation);

    fprintf(stderr,"Nouvelle station : %d\n", nouvelleStation);

    if(nouvelleStation){
        scanf("%d %d %d %d", &listeStations[nbStations].idStation, &listeStations[nbStations].x, &listeStations[nbStations].y, &listeStations[nbStations].capacite);
       
        fprintf(stderr,"%d %d %d %d\n", listeStations[nbStations].idStation, listeStations[nbStations].x, listeStations[nbStations].y, listeStations[nbStations].capacite);
       
        nbStations++;
    }   
    
}

void initialiseBus(){
    
    scanf("%d",&nbBus);
    fprintf(stderr,"Nb bus : %d\nDébut liste bus\n", nbBus);

    for(int i=0; i < nbBus; i++){
        scanf("%d %d %d %d %d %d", &listeBus[i].idBus, &listeBus[i].idJoueur, &listeBus[i].x, &listeBus[i].y, &listeBus[i].stationDeDirection, &listeBus[i].nbVoiture);
        fprintf(stderr,"%d %d %d %d %d %d\n", listeBus[i].idBus, listeBus[i].idJoueur, listeBus[i].x, listeBus[i].y, listeBus[i].stationDeDirection, listeBus[i].nbVoiture);
        
        //parametres implicites basés sur les améiorations des joueurs
        listeBus[i].nbMaxVoiture = 1 + listeJoueurs[listeBus[i].idJoueur].nbAmeliorationSB;
        listeBus[i].vitesse = 1 + listeJoueurs[listeBus[i].idJoueur].nbAmeliorationSP;
        listeBus[i].tarif = 10 + (2 * listeJoueurs[listeBus[i].idJoueur].nbAmeliorationCT);
    }
    fprintf(stderr,"Fin liste bus\n");
}

void initialiseVoyageurs(){

    scanf("%d %d %d", &nbNouveauxVoyageurs, &nbVoyageursMontes, &nbVoyageursDescendus);
    fprintf(stderr,"NT : %d  BT : %d  DT : %d\n", nbNouveauxVoyageurs, nbVoyageursMontes, nbVoyageursDescendus);
    nbVoyageursActuel += (nbNouveauxVoyageurs - nbVoyageursDescendus);

    fprintf(stderr,"Voyageur actuels : %d\n", nbVoyageursActuel);

    ajouteNouveauxVoyageurs();
    faisMonterVoyageurs();
    faisDecendreVoyageurs();
}

void ajouteNouveauxVoyageurs(){
    fprintf(stderr,"Début ajout voyageur\n");
    Voyageur * nouveau;
    for(int i = 0; i<nbNouveauxVoyageurs; i++){
        
        nouveau = (Voyageur *)malloc(sizeof(Voyageur));
       
        scanf("%d %d %d",&nouveau->idVoyageur, &nouveau->idStationDepart, &nouveau->idStationArrivee);
        fprintf(stderr,"%d %d %d\n", nouveau->idVoyageur, nouveau->idStationDepart, nouveau->idStationArrivee);

        nouveau->idBus = -1;
        ajouteVoyageur(nouveau);
    }
    fprintf(stderr,"Fin ajout voyageur\n");

}

void faisMonterVoyageurs(){
    fprintf(stderr,"Début montée voyageur\n");

    int idVoyageur, idBus;
    Voyageur * v;

    for(int i = 0; i<nbVoyageursMontes; i++){
        scanf("%d %d", &idVoyageur, &idBus);
        fprintf(stderr, "%d %d\n", idVoyageur, idBus);
        v=getVoyageur(idVoyageur);
        v->idBus = idBus;
    }
    fprintf(stderr,"Fin montée voyageur\n");

}

void faisDecendreVoyageurs(){
    fprintf(stderr,"Début descente voyageur\n");

    int idVoyageurDescendu;
    for(int i = 0; i<nbVoyageursDescendus; i++){
        scanf("%d", &idVoyageurDescendu);
        fprintf(stderr,"%d\n", idVoyageurDescendu);
        supprimeVoyageur(idVoyageurDescendu);
    }
    fprintf(stderr,"Fin descente voyageur\n");
}