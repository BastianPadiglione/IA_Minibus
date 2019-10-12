#include "main.h"
#include "recuperationDeDonnees.h"

//TODO fonction fini tour qui free les différents malloc

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


int getNbBusJoueur(int idJoueur);
bool peutAcheterBus(int idJoueur);
int * getIdBusJoueur(int idJoueur);
bool peutAjouterVoiture(int idJoueur, int idBus);

void joue(){

    for(int i = 0; i< nbJoueurs; i++){
        for(int j = 0; j< nbBus; j++){
            fprintf(stderr, "J%d B%d=%d\n", listeJoueurs[i].idJoueur, listeBus[j].idBus, peutAjouterVoiture(i,j));
        }
    }

    printf("PASS\n");
}


//Renvoie le nombre de bus possédés par un joueur d'ID idJoueur
int getNbBusJoueur(int idJoueur){
    int nbBusJoueur = 0;

    for(int i = 0; i < nbBus; i++){
        if(listeBus[i].idJoueur == idJoueur){
            nbBusJoueur++;
        }
    }
    return nbBusJoueur; 
}

int getMonNbBus(){
    return getNbBusJoueur(monIdJoueur);
}


//Utile ? il faudrait pouvoir free facilement la liste d'id fournie par la fonction ...
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

//Verifie si un joueur d'ID idJoueur peut acheter un nouveau bus
bool peutAcheterBus(int idJoueur){
    int nbBus = getNbBusJoueur(idJoueur);

    return nbBus < NB_BUS_MAX_JOUEUR && listeJoueurs[idJoueur].argent >= PRIX_BUS;
}

//Verifie si un joueur d'ID idJoueur peut acheter une voiture à un bus d'ID idBus
bool peutAjouterVoiture(int idJoueur, int idBus){

    if(idJoueur == listeBus[idBus].idJoueur //si le bus appartient bien au joueur
    && listeBus[idBus].nbVoiture < listeBus[idBus].nbMaxVoiture //ET que le bus peut avoir d'autres voitures
    && listeJoueurs[idJoueur].argent >= PRIX_VOITURE_BUS //ET que le joueur a l'argent nécessaire
    ){
        return true;
    }
    else {
        return false;
    }
}