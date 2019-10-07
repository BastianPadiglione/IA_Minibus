
/************************************
 * Liste chainée de Voyageurs
*************************************/
#include "gestionListeChaine.h"

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