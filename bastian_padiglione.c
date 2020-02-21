/**
 * 
 * Bastian Padiglione 
 * 
 * FIPA 1A
 * 
 * Au début de la partie, l'IA commence par récupérer les première info pour initialisé la partie
 * Ensuite à chaque tour, le programme va récupérer l'ensemble des données envoyées par le jeu
 * 
 * Après ça l'IA va appeler la fonction joue(). C'est cette fonction qui va décider quoi faire.
 * Elle va suivre la stratégie d'amelioration basique suivante : 
 * si le joueur peut acheter un bus il l'achete, si le jouer peut acheter une amelioration de vitesse il l'achete.
 * Enfin s'il a tous ses bus et toute ses amelioration de vitesse il achete les amelirations de tarif
 * Ensuite cette fonction va appeler ajusteDestinationBus() qui est le coeur de l'algorithme.
 * 
 * En effet ajusteDestinationBus() va optimiser l'ordre de passage des stations à chaque fois qu'une nouvelle station est apparu.
 * c'est également cette fonction qui va decider quand repartir d'une station, afin de rester uniquement le temps necessaire
 * 
 * La liste de voyageur est gérée de maniere dynamique avec une liste chainée, 
 * pour permettre une grande souplesse d'ajout et de suppression de voygaurs
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Valeurs générales
#define NB_JOUEURS_MAX 4
#define NB_STATION_MAX 10
#define NB_STATIONS_INITIAL 3
#define TARIF_BUS_BASE 10
#define NB_VOYAGEUR_MAX_VOITURE 5

//Prix des améliorations
#define PRIX_BUS 100
#define PRIX_VOITURE_BUS 50
#define PRIX_AUGM_TAILLE_BUS 100
#define PRIX_AUGM_VITESSE_BUS 200
#define PRIX_AUGM_TARIF_BUS 100

//Infos sur les amélioration (nottament les limitations)
#define NB_BUS_MAX_JOUEUR 4
#define NB_AUGM_SB_MAX 2
#define NB_AUGM_SP_MAX 2
#define NB_AUGM_CT_MAX 5
#define VALEUR_AUGM_TARIF_BUS 2

/*
* Structures générales représentants les différents acteurs du jeu
*/

typedef struct{
    int idJoueur, argent, nbAmeliorationSB, nbAmeliorationSP, nbAmeliorationCT, etat;
}Joueur;

typedef struct{
    int idStation, x, y, capacite;
}Station;

typedef struct{
    int idVoyageur, idStationDepart, idStationArrivee, idBus;
}Voyageur;

typedef struct{
    int idBus, idJoueur, x, y, stationDeDirection, nbVoiture, nbMaxVoiture, vitesse, tarif;
    bool arrete;
}Bus;


//Structures de gestion de la liste chainée
typedef struct VoyageurListeVoyageur VoyageurListeVoyageur;

struct VoyageurListeVoyageur{
    Voyageur * voyageur;
    VoyageurListeVoyageur * suivant;
};

typedef struct{ 
    VoyageurListeVoyageur * premier;
}ListeVoyageurs;


/*
*Variables globales du jeu
*/

int monIdJoueur;
int nbJoueurs;
int nbStations; 
int nbBus;
int nbNouveauxVoyageurs;
int nbVoyageursDescendus;
int nbVoyageursMontes;
int nbVoyageursActuel;
int tailleListeVoyageur;
bool stationApparu;
Joueur listeJoueurs[NB_JOUEURS_MAX];
Station listeStations[NB_STATION_MAX];
Bus listeBus[NB_JOUEURS_MAX * NB_BUS_MAX_JOUEUR];
ListeVoyageurs listeVoyageurs;





void joue();
void ajusteDestinationBus(char * commandes);

//gestion de l'ordonnacement des stations
void ordonneStations();
Station stationLaPlusProche(Station * station, int indice);
bool estPresent(Station * stations, Station stationCherchee, int taille);
int distance(Station s1, Station s2);
int idStationSuivante(int idSationActuelle);

//gestion du choix de changement de station
bool veutQuitterStation(Bus bus);
bool estVide(int idStation);
bool voyageurVeulentDescendre(int idBus, int idStation);

//verifications d'achat
bool peutAcheterBus(int idJoueur);
bool peutAjouterVoiture(int idJoueur, int idBus);
bool peutAcheterAmeliorationSB(int idJoueur);
bool peutAcheterAmeliorationSP(int idJoueur);
bool peutAcheterAmeliorationCT(int idJoueur);

//fonction d'achat d'ameliorations
void acheteBus(char * commandes);
void acheteAmeliorationSP(char * commande);
void acheteAmeliorationCT(char * commande);
void acheteAmeliorationSB(char * commande);

//recuperations d'informations
Station getStation(int idStation);
int getNbBusJoueur(int idJoueur);
int getMonNbBus();
int * getIdBusJoueur(int idJoueur);
int getNbVoyageurBus(int idBus);
int * getMesIdBus();

//gestion de la liste chainée de voyageurs
void initialiseListeVoyageur();
void ajouteVoyageur(Voyageur * v);
void supprimeVoyageur(int idVoyageur);
Voyageur * getVoyageur(int idVoyageur);

//recuperation des données
void initialisePartie();
void initialiseTour();
void initialiseJoueurs();
void initialiseStations();
void initialiseBus();
void initialiseVoyageurs();
void ajouteNouveauxVoyageurs();
void faisMonterVoyageurs();
void faisDecendreVoyageurs();











/******************************************************************
** Main
*******************************************************************/

int main()
{
    int tour = 1;
    initialisePartie();
   
    do{
       
        initialiseTour();
        
        joue();
        fflush(stderr);
        fflush(stdout);
        fflush(stdin);
        tour++;
    }while (listeJoueurs[monIdJoueur].etat == 0);
    
    return 0;
}


/******************************************************************
** joue
*******************************************************************/

void joue(){
    
    char * commandes = (char*)malloc(256 * sizeof(char));
    commandes[0] = '\0';

    if(peutAcheterBus(monIdJoueur)){
        acheteBus(commandes);
        listeJoueurs[monIdJoueur].argent -= PRIX_BUS;
    }

    if(peutAcheterAmeliorationSP(monIdJoueur)){
        acheteAmeliorationSP(commandes);
        listeJoueurs[monIdJoueur].argent -= PRIX_AUGM_VITESSE_BUS;
    }
    if(getMonNbBus() == NB_BUS_MAX_JOUEUR 
        && (listeJoueurs[monIdJoueur].nbAmeliorationSP) >=  NB_AUGM_SP_MAX 
        && peutAcheterAmeliorationCT(monIdJoueur)){

            acheteAmeliorationCT(commandes);
            listeJoueurs[monIdJoueur].argent -= PRIX_AUGM_TARIF_BUS;
        }

    ajusteDestinationBus(commandes);

    strcat(commandes, "PASS\n");
    printf("%s",commandes);

    free(commandes);
}

/**
** Si une station vient d'être ajoutée, alors les stations sont réordonnées
**
** Dans tout les cas vérifie pour chaque bus arreté s'il doit repartir.
** Si oui il ajoute dans @commandes la commande donnant la prochaine destination du bus
*/
void ajusteDestinationBus(char * commandes){

    int * mesIdBus = getMesIdBus();
    Bus bus;
    char * commandeDest = (char*)malloc(64*sizeof(char));

    
    if(stationApparu){
       ordonneStations();
    }
    

    for(int i = 0; i < getMonNbBus(); i++){

        bus = listeBus[mesIdBus[i]];
    
        if(bus.arrete){
            if(veutQuitterStation(bus)){
                fprintf(stderr,"debut if\n");
                sprintf(commandeDest, "DESTINATION %d %d ; ", bus.idBus, idStationSuivante(bus.stationDeDirection) );
                strcat(commandes,commandeDest);
                fprintf(stderr,"fin if\n");

            }
        }
    }
    free(mesIdBus);
    free(commandeDest);
}


/*****************
** gestion de l'ordonnacement des stations
******************/

/**
 * Optimise la distance entre les stations et met le nouvel ordre dans @listeStations
 */
void ordonneStations(){

    Station stationsOrdonnees[NB_STATION_MAX];
    stationsOrdonnees[0] = listeStations[0];

    for(int i = 1; i < nbStations; i++){
        stationsOrdonnees[i] = stationLaPlusProche(stationsOrdonnees, i - 1);
    }
     for(int i = 0; i<nbStations; i++){
        listeStations[i] = stationsOrdonnees[i];
    }
}

/**
 * retourne la station la plus proche de stations[indiceStationActelle]
 * tel que celle ci ne soit pas déjà dans @stations
*/
Station stationLaPlusProche(Station * stations, int indiceStationActelle){
    int distancePlusCourte = __INT_MAX__;
    Station stationProche = listeStations[0];

    int dist;

    for(int i = 0; i < nbStations; i++){
        if(!estPresent(stations, listeStations[i], indiceStationActelle +1)){
            dist = distance(stations[indiceStationActelle], listeStations[i]);

            if(dist < distancePlusCourte){
                distancePlusCourte = dist;
                stationProche = listeStations[i];
            }
        }
    }
    return stationProche;
}

/**
 * vérifie si @stationCherchee est présent dans @stations, tel que la taille de stations = @taille
 */
bool estPresent(Station * stations, Station stationCherchee, int taille){

    for(int i = 0; i < taille; i++){
        if(stations[i].idStation == stationCherchee.idStation){
            return true;
        }
    }
    return false;
}

/**
 * Calcule et retourne la distance entre 2 stations
*/
int distance(Station s1, Station s2){
    int dist = 0;
    int diffX = abs(s1.x - s2.x);
    int diffY = abs(s1.y - s2.y);

    //se deplacer en diagonale vaut pour 1 déplacement
    while(diffX > 0 && diffY > 0){
        dist++;
        diffX--;
        diffY--;
    }
    //somme des deplacement diagonaux et verticaux ou horizontaux
    return dist + diffX + diffY;
}

/**
 * Retourne l'id de la station positionnée juste après la station d'id @idSationActuelle dans le tableau listeStations
*/
int idStationSuivante(int idStationActuelle){
    int positionStationActuelle;

    for (int i = 0; i< nbStations; i++){
        if(listeStations[i].idStation == idStationActuelle){
            positionStationActuelle = i;
        }
    }
    int positionStationSuivante = (positionStationActuelle + 1) % nbStations;

    return listeStations[positionStationSuivante].idStation;
}



/*****************
** gestion du choix de changement de station
******************/

bool veutQuitterStation(Bus bus){
    return (estVide(bus.stationDeDirection)                                     //si le bus ne peut pas prendre de voyageur (station vide ou bus plein)
                || getNbVoyageurBus(bus.idBus) == bus.nbVoiture * NB_VOYAGEUR_MAX_VOITURE)  //ET que personne ne veut descendre
                && !voyageurVeulentDescendre(bus.idBus, bus.stationDeDirection);
}

/**
 * Return true s'il n'y a aucun voyageur dans la station @idStation 
*/
bool estVide(int idStation){
    VoyageurListeVoyageur * actuel = listeVoyageurs.premier;
    bool estVide = true;

    while(actuel != NULL && estVide){
        if(actuel->voyageur->idBus == -1 
        && actuel->voyageur->idStationDepart == idStation){
            estVide = false;
        }
        actuel = actuel->suivant;
    }
    return estVide;
}
/**
 * Return true si des voyageurs du bus @idBus veulent descendre à la station @idStation
*/
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



/*****************
** verifications d'achat
******************/

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



/*****************
** fonction d'achat d'ameliorations
******************/


/**
 * Demande l'achat d'un bus à la station 0
*/
void acheteBus(char * commandes){
    char * commandeBus = "BUS 0 ; ";
    strcat(commandes, commandeBus);
}

void acheteAmeliorationSP(char * commande){
    strcat(commande, "UPDATESP ; ");
}

void acheteAmeliorationCT(char * commande){
    strcat(commande, "UPDATECT ; ");
}

void acheteAmeliorationSB(char * commande){
    strcat(commande, "UPDATESB ; ");
}



/*****************
** recuperations d'informations
******************/

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

Station getStation(int idStation){
    for(int i = 0; i < nbStations; i++){
        if(listeStations[i].idStation == idStation){
            return listeStations[i];
        }
    }
    return listeStations[0];
}


/*******************************************************************
 * Gestion de la liste chainée de Voyageurs
*******************************************************************/



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
            trouver = true;
            
            aSupprimmer = actuel->suivant;
            actuel->suivant = aSupprimmer->suivant;//on recupere le maillon suivant celui à supprimer pour garder la suite de la liste
        }
        else{//clause else car dans le cas où on supprime le dernier maillon: actuel == NULL
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


/******************************************************************
** Recupération des données
*******************************************************************/

void  initialisePartie(){
    
    scanf("%d %d", &nbJoueurs, &monIdJoueur);
    nbStations = NB_STATIONS_INITIAL;

    //pas de voyageur au début
    nbVoyageursActuel=0;
    initialiseListeVoyageur(); 

    for(int i = 0; i < nbStations; i++){
        scanf("%d %d %d %d", &listeStations[i].idStation, &listeStations[i].x, &listeStations[i].y, &listeStations[i].capacite);
    }
}

void initialiseTour(){
    initialiseJoueurs();
    initialiseStations();
    initialiseBus();
    initialiseVoyageurs();
}

void initialiseJoueurs(){
    
    for(int i = 0; i<nbJoueurs; i++){
        scanf("%d %d %d %d %d %d", &listeJoueurs[i].idJoueur, &listeJoueurs[i].argent, &listeJoueurs[i].nbAmeliorationSB, &listeJoueurs[i].nbAmeliorationSP, &listeJoueurs[i].nbAmeliorationCT, &listeJoueurs[i].etat);
    }
}

void initialiseStations(){

    int nouvelleStation;

    scanf("%d", &nouvelleStation);

    if(nouvelleStation){
        stationApparu = true;
        scanf("%d %d %d %d", &listeStations[nbStations].idStation, &listeStations[nbStations].x, &listeStations[nbStations].y, &listeStations[nbStations].capacite);
        nbStations++;
    }
    else{
        stationApparu = false;
    }    
}

void initialiseBus(){
    
    scanf("%d",&nbBus);

    for(int i=0; i < nbBus; i++){
        scanf("%d %d %d %d %d %d", &listeBus[i].idBus, &listeBus[i].idJoueur, &listeBus[i].x, &listeBus[i].y, &listeBus[i].stationDeDirection, &listeBus[i].nbVoiture);

        //parametres implicites basés sur les améliorations des joueurs
        //1 car 1 voiture à la base
        listeBus[i].nbMaxVoiture = 1 + listeJoueurs[listeBus[i].idJoueur].nbAmeliorationSB;
        //1 car vitesse de base = 1
        listeBus[i].vitesse = 1 + listeJoueurs[listeBus[i].idJoueur].nbAmeliorationSP;
        listeBus[i].tarif = TARIF_BUS_BASE + (VALEUR_AUGM_TARIF_BUS * listeJoueurs[listeBus[i].idJoueur].nbAmeliorationCT);
        
        //si le bus est à sa station de destination
        if(listeBus[i].x == getStation(listeBus[i].stationDeDirection).x
            && listeBus[i].y == getStation(listeBus[i].stationDeDirection).y){

             listeBus[i].arrete = true;
        }
        else{
            listeBus[i].arrete = false;
        }  
    }
}

void initialiseVoyageurs(){

    scanf("%d %d %d", &nbNouveauxVoyageurs, &nbVoyageursMontes, &nbVoyageursDescendus);
    nbVoyageursActuel += (nbNouveauxVoyageurs - nbVoyageursDescendus);

    ajouteNouveauxVoyageurs();
    faisMonterVoyageurs();
    faisDecendreVoyageurs();
}

void ajouteNouveauxVoyageurs(){
    Voyageur * nouveau;
    for(int i = 0; i<nbNouveauxVoyageurs; i++){
        
        nouveau = (Voyageur *)malloc(sizeof(Voyageur));
       
        scanf("%d %d %d",&nouveau->idVoyageur, &nouveau->idStationDepart, &nouveau->idStationArrivee);

        nouveau->idBus = -1;
        ajouteVoyageur(nouveau);
    }
}

void faisMonterVoyageurs(){

    int idVoyageur, idBus;
    Voyageur * v;

    for(int i = 0; i<nbVoyageursMontes; i++){
        scanf("%d %d", &idVoyageur, &idBus);
        v=getVoyageur(idVoyageur);
        v->idBus = idBus;
    }
}

void faisDecendreVoyageurs(){

    int idVoyageurDescendu;
    for(int i = 0; i<nbVoyageursDescendus; i++){
        scanf("%d", &idVoyageurDescendu);
        supprimeVoyageur(idVoyageurDescendu);
    }
}