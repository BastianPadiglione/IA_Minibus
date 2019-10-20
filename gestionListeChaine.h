/************************************
 * Liste chainée de Voyageurs
*************************************/

#ifndef _GESTION_LISTE_CHAINE_

#define _GESTION_LISTE_CHAINE_


typedef struct VoyageurListeVoyageur VoyageurListeVoyageur;

struct VoyageurListeVoyageur{
    Voyageur * voyageur;
    VoyageurListeVoyageur * suivant;
};

typedef struct{
    VoyageurListeVoyageur * premier;
}ListeVoyageurs;

ListeVoyageurs listeVoyageurs;

void initialiseListeVoyageur();
void ajouteVoyageur(Voyageur * v);
void supprimeVoyageur(int idVoyageur);
Voyageur * getVoyageur(int idVoyageur);

#endif