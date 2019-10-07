#include "main.h"
#include "recuperationDeDonnees.h"



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

void joue(){}





