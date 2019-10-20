#include "verifications.h"


//Verifie si un joueur d'ID idJoueur peut acheter un nouveau bus
bool peutAcheterBus(int idJoueur){
    int nbBus = getNbBusJoueur(idJoueur);

    return nbBus < NB_BUS_MAX_JOUEUR 
        && listeJoueurs[idJoueur].argent >= PRIX_BUS;
}

//Verifie si un joueur d'ID idJoueur peut acheter une amelioration SP
bool peutAcheterAmeliorationSP(int idJoueur){

    return listeJoueurs[idJoueur].argent >= PRIX_AUGM_VITESSE_BUS //si le joueur a l'argent nécessaire
        && (listeJoueurs[idJoueur].nbAmeliorationSP) <  NB_AUGM_SP_MAX; //ET que le joueur peut acheter d'autre améliorations SP
}

//Verifie si un joueur d'ID idJoueur peut acheter une amelioration SP
bool peutAcheterAmeliorationCT(int idJoueur){

    return listeJoueurs[idJoueur].argent >= PRIX_AUGM_TARIF_BUS
        && (listeJoueurs[idJoueur].nbAmeliorationCT) < NB_AUGM_CT_MAX;
}

//Verifie si un joueur d'ID idJoueur peut acheter une amelioration SB
bool peutAcheterAmeliorationSB(int idJoueur){

    return listeJoueurs[idJoueur].argent >= PRIX_AUGM_TAILLE_BUS  //Si le joueur a l'argent nécessaire
        && (listeJoueurs[idJoueur].nbAmeliorationSB) < NB_AUGM_SB_MAX; //ET que le joueur n'est pas au max du nombre d'amelioration de ce type
}

//Verifie si un joueur d'ID idJoueur peut acheter une voiture à un bus d'ID idBus
bool peutAjouterVoiture(int idJoueur, int idBus){

    return idJoueur == listeBus[idBus].idJoueur //si le bus appartient bien au joueur
    && listeBus[idBus].nbVoiture < listeBus[idBus].nbMaxVoiture //ET que le bus peut avoir d'autres voitures
    && listeJoueurs[idJoueur].argent >= PRIX_VOITURE_BUS; //ET que le joueur a l'argent nécessaire
}