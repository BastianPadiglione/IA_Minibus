#include "main.h"
#include "verifications.h"
#include "recuperationDeDonnees.h"
#include "gestionListeChaine.h"

int main()
{
    int tour = 1;
    initialisePartie();
   
    do{
       // fprintf(stderr,"Tour : %d\n",tour);
        initialiseTour();
        
        joue();
        fflush(stderr);
        fflush(stdout);
        tour++;
    }while (listeJoueurs[monIdJoueur].etat == 0);
    
    return 0;
}


/**************************
** joue
**************************/



int getNbVoyageurBus(int idBus);
void acheteAmeliorationSP(char * commande);


void joue(){
    
    char * commandes = (char*)malloc(256 * sizeof(char));
    commandes[0] = '\0';

    if(peutAcheterBus(monIdJoueur)){
        acheteBus(commandes);
    }

    if(peutAcheterAmeliorationSP(monIdJoueur)){
        acheteAmeliorationSP(commandes);
    }

    ajusteDestinationBus(commandes);

    strcat(commandes, "PASS\n");
    printf("%s",commandes);

    free(commandes);
}




//Renvoie le nombre de bus possédés par un joueur d'ID idJoueur
int getNbBusJoueur(int idJoueur){
    int nbBusJoueur = 0;

    for(int i = 0; i < nbBus; i++){
        if(listeBus[i].idJoueur == idJoueur){
            nbBusJoueur++;
        }
    }
    //fprintf(stderr, "J %d nbBus = %d\n",idJoueur, nbBusJoueur);

    return nbBusJoueur; 
}

int getMonNbBus(){
    return getNbBusJoueur(monIdJoueur);
}


//renvoie un pointeur sur un tableau contenant les IDs des bus d'un joueur idJoueur
//Attention à bien free le pointeur lors de l'utilisation de la fonction
int * getIdBusJoueur(int idJoueur){
    int nbBusJoueur = getNbBusJoueur(idJoueur);
    
    int * idBusJoueur = (int *)malloc(nbBusJoueur * sizeof(int));
    int indiceRetour=0;

    for(int i = 0; i < nbBus; i++){
        if(listeBus[i].idJoueur == idJoueur){

            idBusJoueur[indiceRetour]=listeBus[i].idBus;
            indiceRetour++;
        }
    }
    return idBusJoueur;
}

int * getMesIdBus(){
    return getIdBusJoueur(monIdJoueur);
}


bool estVide(int idStation){
    VoyageurListeVoyageur * actuel = listeVoyageurs.premier;
    bool estVide = true;

    //Mettre l'idBus d'un voygeur à -1 ou verifier que ça le soit par default au début
    while(actuel != NULL && estVide){
        if(actuel->voyageur->idBus == -1 
        && actuel->voyageur->idStationDepart == idStation){
            estVide = false;
        }
        actuel = actuel->suivant;
    }
    return estVide;
}


int getNbVoyageurBus(int idBus){

    VoyageurListeVoyageur * actuel = listeVoyageurs.premier;

    int nb = 0;
    
    while(actuel != NULL){
        if(actuel->voyageur->idBus == idBus){
            nb++;
        }
        actuel = actuel->suivant;
    }

    return nb;
}

bool voyageurVeulentDescendre(int idBus, int idStation){

    VoyageurListeVoyageur * actuel = listeVoyageurs.premier;
    bool veutDesc = false;

    while(actuel != NULL && !veutDesc){
        if(actuel->voyageur->idBus == idBus 
        && actuel->voyageur->idStationArrivee == idStation){
            veutDesc = true;
        }
        actuel = actuel->suivant;
    }
    return veutDesc;
}




/*
**Ajoute commandes
*/
void acheteBus(char * commandes){
    char * commandeBus = "BUS 0 ; ";
    strcat(commandes, commandeBus);
}

//TOFIX parfois le bus reste bloquer à la meme station
void ajusteDestinationBus(char * commandes){
    int * mesIdBus = getMesIdBus();
    Bus bus;
    char * commandeDest = (char*)malloc(32*sizeof(char));

    for(int i = 0; i < getMonNbBus(); i++){

        bus = listeBus[mesIdBus[i]];
    
        if(bus.arrete){
            
            if((estVide(bus.stationDeDirection)                                     //si le bus ne peut pas prendre de voyageur (station vide ou bus plein)
                || getNbVoyageurBus(bus.idBus) == bus.nbVoiture * NB_VOYAGEUR_MAX_VOITURE)  //ET que personne ne veut descendre
                && !voyageurVeulentDescendre(bus.idBus, bus.stationDeDirection)){
               
                sprintf(commandeDest, "DESTINATION %d %d ; ", bus.idBus, (bus.stationDeDirection+1)%nbStations);
                strcat(commandes,commandeDest);
            }
        }
    }
    free(mesIdBus);
    free(commandeDest);
}

void acheteAmeliorationSP(char * commande){
    strcat(commande, "UPDATESP ; ");
}