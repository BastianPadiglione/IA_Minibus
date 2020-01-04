#include "main.h"
#include "verifications.h"
#include "recuperationDeDonnees.h"
#include "gestionListeChaine.h"

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


/**************************
** joue
**************************/

void ordonneStations();
Station stationLaPlusProche(Station * station, int indice);
bool estPresent(Station * stations, Station stationCherchee, int taille);
int distance(Station s1, Station s2);
int idStationSuivante(int idSationActuelle);

int getNbVoyageurBus(int idBus);
void acheteAmeliorationSP(char * commande);
void acheteAmeliorationCT(char * commande);
void acheteAmeliorationSB(char * commande);

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

    if(stationApparu){
       ordonneStations();
    }

    for(int i = 0; i < getMonNbBus(); i++){

        bus = listeBus[mesIdBus[i]];
    
        if(bus.arrete){
            if((estVide(bus.stationDeDirection)                                     //si le bus ne peut pas prendre de voyageur (station vide ou bus plein)
                || getNbVoyageurBus(bus.idBus) == bus.nbVoiture * NB_VOYAGEUR_MAX_VOITURE)  //ET que personne ne veut descendre
                && !voyageurVeulentDescendre(bus.idBus, bus.stationDeDirection)){

                sprintf(commandeDest, "DESTINATION %d %d ; ", bus.idBus, idStationSuivante(bus.stationDeDirection) );
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

void acheteAmeliorationCT(char * commande){
    strcat(commande, "UPDATECT ; ");
}

void acheteAmeliorationSB(char * commande){
    fprintf(stderr,"achete SB");
    strcat(commande, "UPDATESB ; ");
}

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

bool estPresent(Station * stations, Station stationCherchee, int taille){

    for(int i = 0; i < taille; i++){
        if(stations[i].idStation == stationCherchee.idStation){
            return true;
        }
    }
    return false;
}

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

Station getStation(int idStation){
    for(int i = 0; i < nbStations; i++){
        if(listeStations[i].idStation == idStation){
            return listeStations[i];
        }
    }
    return listeStations[0];
}