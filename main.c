#include "main.h"
#include "recuperationDeDonnees.h"

//TODO fonction fini tour qui free les différents malloc

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
    }while (listeJoueurs[monIdJoueur].etat == 0);
    
    return 0;
}





/**************************
** joue
**************************/


int * getIdBusByIdJoueur(int idJoueur);


void joue(){

    printf("PASS\n");
}




//TODO tester fonctions de getion de bus
int getNbBusJoueur(int idJoueur){
    int nbBus = 0;

    for(int i = 0; i < nbBus; i++){
        if(listeBus[i].idJoueur == idJoueur){
            nbBus++;
        }
    }
    return nbBus;
}

int getMonNbBus(){
    return getNbBusJoueur(monIdJoueur);
}

int * getIdBusJoueur(int idJoueur){

    int nbBus = getNbBusJoueur(idJoueur);
    int * idBusJoueur = (int *)malloc(nbBus * sizeof(int));
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

bool peutAcheterBus(int idJoueur){
    int nbBus = getNbBusJoueur(idJoueur);

    return nbBus < NB_BUS_MAX_JOUEUR && listeJoueurs[idJoueur].argent >= PRIX_BUS;
}






