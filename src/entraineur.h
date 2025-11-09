#ifndef ENTRAINEUR_H
#define ENTRAINEUR_H

typedef struct {
    char cin[12];
    char nom[50];
    char prenom[50];
    char specialite[50];
    char tel[20];
    char sexe[10];
    int jour;
    int mois;
    int annee;
    char mdp[50];
} Entraineur;


void ajouter_entraineur(Entraineur e);
void modifier_entraineur(Entraineur e);
void supprimer_entraineur(const char *cin);
Entraineur chercher_entraineur(const char *cin);

void inscrire_cours();

int valider_cin(const char *cin);
int valider_telephone(const char *tel);
int valider_nom(const char *nom);
int valider_date(int j, int m, int a);
int valider_sexe(const char *sexe);

#endif
