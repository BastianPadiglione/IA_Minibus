#include "IA_0.2.h"

int main()
{
    int tour = 1;
    initialisePartie();
   
    do{
        fprintf(stderr,"Tour : %d\n",tour);
        initialiseTour();
        fflush(stderr);
        joue();
        fflush(stdout);
        tour++;
    }while (listeJoueurs[idJoueur].etat == 0);
    
    return 0;
}

void  initialisePartie(){
    
    scanf("%d %d", &nbJoueurs, &idJoueur);
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



/************************************
 * Liste chainée de Voyageurs
*************************************/


void initialiseListeVoyageur(){
    listeVoyageurs.premier = NULL;
}

//TODO ajouter au début??? wtf à la fin
//Ajoute un nouveau voyageur v à la fin de la liste chainée
void ajouteVoyageur(Voyageur * v){

    //Creation du nouveau voyageur
    VoyageurListeVoyageur * nouveau = (VoyageurListeVoyageur *)malloc(sizeof(VoyageurListeVoyageur));
    nouveau->voyageur = v;
    nouveau->suivant = NULL;

    if(listeVoyageurs.premier==NULL){   
        listeVoyageurs.premier = nouveau;       
    }
    else{
       /* VoyageurListeVoyageur * premier = listeVoyageurs.premier;
        VoyageurListeVoyageur * deuxieme = premier->suivant;
        premier->suivant = nouveau;
        nouveau->suivant = deuxieme;*/
        
        VoyageurListeVoyageur * actuel = listeVoyageurs.premier;
        
        //on se place sur le dernier maillon de la chaine
        while(actuel->suivant != NULL){
            actuel = actuel->suivant;
        }
        //creation du maillon suivant
        actuel->suivant = nouveau;
    }
}

//supprime un voyageur d'id idVoyageur de la liste chainée
void supprimeVoyageur(int idVoyageur){

    VoyageurListeVoyageur * actuel = listeVoyageurs.premier;
    VoyageurListeVoyageur * aSupprimmer = NULL;
    int trouver = 0; //si on a trouver le bon voyageur
/*
    if(actuel->voyageur->idVoyageur == idVoyageur){
        trouver = 1;

        aSupprimmer = actuel;
        listeVoyageurs.premier->suivant = aSupprimmer->suivant;

        free(aSupprimmer->voyageur);
            free(aSupprimmer);
    }*/

    while(actuel->suivant != NULL && trouver == 0){

        if(actuel->suivant->voyageur->idVoyageur == idVoyageur){
            trouver = 1;
            
            aSupprimmer = actuel->suivant;
            actuel->suivant = aSupprimmer->suivant;//on recupere le maillon suivant celui à supprimer pour garder la suite de la liste
            
            free(aSupprimmer->voyageur);
            free(aSupprimmer);
        }
        actuel = actuel->suivant;
    }
}

//retourne une structure voyageur contenant l'id idVoyageur
Voyageur * getVoyageur(int idVoyageur){
    fprintf(stderr, "Début fonction getVoyageurs\n");
    VoyageurListeVoyageur * actuel = listeVoyageurs.premier;
    int trouver = 0; //si on a trouver le bon voyageur
    Voyageur * v=NULL;

    while(actuel != NULL && trouver == 0){

        fprintf(stderr, "Actuel = %d   Attendu = %d\n",actuel->voyageur->idVoyageur, idVoyageur);
        if(actuel->voyageur->idVoyageur == idVoyageur){
            fprintf(stderr, "Voyageur trouvé!\n");
            v = actuel->voyageur;
            trouver = 1;
        }
        else{
            fprintf(stderr,"Voyageur NON trouvé\n");
            actuel = actuel->suivant;
            if(actuel==NULL){
                fprintf(stderr, "Suivant est NULL\n");
            }
        }
        
        
    }
    fprintf(stderr, "Fin fonction getVoyageurs\n");
    return v;
    
}