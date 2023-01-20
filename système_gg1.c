#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generer_inter_arrivee(){
    return rand()%5 + 1;
}

int generer_tmps_service(){
    return rand()%5 + 1;
}

void evenement_arrive(int *q, double *delta, double *t, double *t_arr, double *t_cum, double *t_occ, int *etat_serveur, double *t_dep, int *long_file, int *p){
    //incrémentation du compteur des arrivées
    (*q)++;
    //calcul du delta, le temps entre les 2 evenements successifs
    (*delta) = (*t_arr) - (*t);
    //mise a jour de l'horloge
    (*t) = (*t_arr);
    //programme l'arrive suivante dans le système
    (*t_arr) = (*t) + generer_inter_arrivee();
    //mise a jour du temps total de séjour des pièces dans le système
    (*t_cum) = (*t_cum) + (*delta) * ((*long_file) + (*etat_serveur));
    //mise a jour du temps d'occupation du serveur
    (*t_occ) = (*t_occ) + (*delta) * (*etat_serveur);

    //si le serveur est libre
    if((*etat_serveur) == 0){
        //changement d'état du serveur, il devient occupé
        (*etat_serveur) = 1;
        //programmation de fin de serice de la piece en cours de traitement
        (*t_dep) = (*t) + generer_tmps_service();
    }else{ //si le serveur est déjà occuper
        //incrémentation du nombre de pièces (en attente de traitement) dans la file
        (*long_file)++;
    }
    printf("ARRIVE => t : %lf | long_file : %d | etat_serveur : %d | q : %d | p : %d\n", *t, *long_file, *etat_serveur, *q, *p);

}

void evenement_depart(int *q, double *delta, double *t, double *t_arr, double *t_cum, double *t_occ, int *etat_serveur, double *t_dep, int *long_file, int *p){
    //incrémentation du nombre de pièces produites (fin du traitement effectif)
    (*p)++;
    //calcul du delta, le temps entre les 2 evenements successifs
    (*delta) = (*t_dep) - (*t);
    //mise a jour de l'horloge
    (*t) = (*t_dep);
    //mise a jour du temps total de séjour des pièces dans le système
    (*t_cum) = (*t_cum) + (*delta) * ((*long_file) + (*etat_serveur));
    //mise a jour du temps d'occupation du serveur
    (*t_occ) = (*t_occ) + (*delta) * (*etat_serveur);

    if((*long_file) > 0){ //s'il y a envore des pièces en attente dans la file
        //décrémentation du compteur du nombre de pièce en file
        (*long_file)--;
        //fin de service de la nouvelle pièce en traitement
        (*t_dep) = (*t) + generer_tmps_service();
    }else{ //si pas de pièce en file
        //changement d'état du serveur -> il devient libre
        (*etat_serveur) = 0;
        //représente une date de fin infinie --> correspond au fait que le serveur soit libre
        (*t_dep) = INT_MAX;
    }
    printf("DEPART => t : %lf | long_file : %d | etat_serveur : %d | q : %d | p : %d\n", *t, *long_file, *etat_serveur, *q, *p);
}

int main(){
    //initialisation des variables globales
    double t = 0; //horloge
    double t_max = 20; //t_simulation
    double t_arr = 0; //t arrivé suivant
    double t_dep = INT_MAX; //le prochain départ
    double delta = 0; //temps entre deux évènements successifs
    double t_occ = 0; //le temps cum d'occupation du serveur
    double t_cum = 0; //temps cumulé de séjour dans le systeme
    int etat_serveur = 0;
    int q = 0; //nombre d'arrivé
    int p = 0; //nombre de clients servi (passé dans le serveur)
    int long_file = 0; //nombre de client en file
    double taux_occ;
    double t_moy_sejour;
    double t_moy_attente;
    double nb_moy_clients;
    srand(time(NULL));

    //boucle de simulation
    while(t <= t_max){
        //printf("Temps de la simulation : %lf\n", t);
        //printf("temps arrive : %lf\ntemps depart : %lf\n\n", t_arr, t_dep);
        if(t_arr < t_dep){
            evenement_arrive(&q, &delta, &t, &t_arr, &t_cum, &t_occ, &etat_serveur, &t_dep, &long_file, &p);
        }else{
            evenement_depart(&q, &delta, &t, &t_arr, &t_cum, &t_occ, &etat_serveur, &t_dep, &long_file, &p);
        }
    }

    printf("\nCALCUL : \n");
    t_moy_attente = t_cum / p;
    printf("moyenne d'attente dans la file : %lf", t_moy_attente);

    return 0;
}