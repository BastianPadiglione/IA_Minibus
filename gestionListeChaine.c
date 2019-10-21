
/************************************
 * Liste chainée de Voyageurs
*************************************/
#include "main.h"
#include "gestionListeChaine.h"


void initialiseListeVoyageur(){
    listeVoyageurs.premier = NULL;
}

//Ajoute un nouveau voyageur v au début de la liste chainée
void ajouteVoyageur(Voyageur * v){

    //Creation du nouveau voyageur
    VoyageurListeVoyageur * nouveau = (VoyageurListeVoyageur *)malloc(sizeof(VoyageurListeVoyageur));
    nouveau->voyageur = v;
    
    nouveau->suivant = listeVoyageurs.premier;
    listeVoyageurs.premier = nouveau;
 
}

//supprime un voyageur d'id idVoyageur de la liste chainée
void supprimeVoyageur(int idVoyageur){

    VoyageurListeVoyageur * actuel = listeVoyageurs.premier;
    VoyageurListeVoyageur * aSupprimmer = NULL;
    bool trouver = false;

    //On gere separement le cas ou le voyageur à supprimer est le premier de la liste
    if(actuel->voyageur->idVoyageur == idVoyageur){
        trouver = true;
        aSupprimmer = actuel;
        actuel = aSupprimmer->suivant;
    }


    while(actuel->suivant != NULL && !trouver){

        if(actuel->suivant->voyageur->idVoyageur == idVoyageur){
            trouver = 1;
            
            aSupprimmer = actuel->suivant;
            actuel->suivant = aSupprimmer->suivant;//on recupere le maillon suivant celui à supprimer pour garder la suite de la liste
        }
        else{//clause else car dans le cas où on supprime le dernier maillon actuel = NULL
            actuel = actuel->suivant;
        }   
    }

    if(trouver){
        free(aSupprimmer->voyageur);
        free(aSupprimmer);
    }
}

//retourne une structure voyageur contenant l'id idVoyageur
Voyageur * getVoyageur(int idVoyageur){
    VoyageurListeVoyageur * actuel = listeVoyageurs.premier;
    int trouver = 0; //si on a trouver le bon voyageur
    Voyageur * v=NULL;

    while(actuel != NULL && trouver == 0){

        if(actuel->voyageur->idVoyageur == idVoyageur){
            v = actuel->voyageur;
            trouver = 1;
        }

        actuel = actuel->suivant;
    }
    return v;
}