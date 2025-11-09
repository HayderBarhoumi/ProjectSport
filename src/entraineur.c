#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "entraineur.h"

#define FICHIER "entraineurs.txt"


/* Validation du CIN : exactement 8 chiffres */
int valider_cin(const char *cin) {
    if (!cin) return 0;
    if (strlen(cin) != 8) return 0;
    for (size_t i = 0; i < strlen(cin); ++i)
        if (!isdigit((unsigned char)cin[i])) return 0;
    return 1;
}



/* Validation du téléphone : exactement 8 chiffres */
int valider_telephone(const char *tel) {
    if (!tel) return 0;
    if (strlen(tel) != 8) return 0;
    for (size_t i = 0; i < strlen(tel); ++i)
        if (!isdigit((unsigned char)tel[i])) return 0;
    return 1;
}



/* Validation du nom/prénom : uniquement des lettres */
int valider_nom(const char *nom) {
    if (!nom || strlen(nom) == 0) return 0;
    for (size_t i = 0; i < strlen(nom); ++i)
        if (!isalpha((unsigned char)nom[i])) return 0;
    return 1;
}



/* Validation de la date */
int valider_date(int j, int m, int a) {
    if (a < 1970 || a > 2005) return 0;
    if (m < 1 || m > 12) return 0;
    if (j < 1 || j > 31) return 0;
    if ((m == 4 || m == 6 || m == 9 || m == 11) && j > 30) return 0;
    if (m == 2) {
        int bissextile = (a % 4 == 0 && (a % 100 != 0 || a % 400 == 0));
        if (j > (bissextile ? 29 : 28)) return 0;
    }
    return 1;
}




int valider_sexe(const char *sexe) {
    if (!sexe) return 0;
    if (strlen(sexe) == 1) {
        char c = sexe[0];
        if (c == 'H' || c == 'h' || c == 'F' || c == 'f') return 1;
    } else {
        char s[16];
        size_t i;
        for (i = 0; i < sizeof(s)-1 && i < strlen(sexe); ++i) 
            s[i] = tolower((unsigned char)sexe[i]);
        s[i] = '\0';
        if (strcmp(s, "homme") == 0 || strcmp(s, "femme") == 0) return 1;
    }
    return 0;
}




void ajouter_entraineur(Entraineur e) {
    FILE *f = fopen(FICHIER, "a");
    if (f != NULL) {
        fprintf(f, "%s %s %s %s %s %s %d %d %d %s\n",
                e.cin, e.nom, e.prenom, e.specialite, e.tel,
                e.sexe, e.jour, e.mois, e.annee, e.mdp);
        fclose(f);
        printf("✅ Entraîneur ajouté avec succès.\n");
    } else {
        printf("❌ Erreur : impossible d'ouvrir le fichier.\n");
    }
}




void modifier_entraineur(Entraineur e) {
    FILE *f = fopen(FICHIER, "r");
    FILE *tmp = fopen("temp.txt", "w");
    Entraineur x;
    int trouve = 0;

    if (f && tmp) {
        while (fscanf(f, "%11s %49s %49s %49s %19s %9s %d %d %d %49s",
                      x.cin, x.nom, x.prenom, x.specialite, x.tel,
                      x.sexe, &x.jour, &x.mois, &x.annee, x.mdp) == 10) {
            if (strcmp(x.cin, e.cin) == 0) {
                fprintf(tmp, "%s %s %s %s %s %s %d %d %d %s\n",
                        e.cin, e.nom, e.prenom, e.specialite, e.tel,
                        e.sexe, e.jour, e.mois, e.annee, e.mdp);
                trouve = 1;
            } else {
                fprintf(tmp, "%s %s %s %s %s %s %d %d %d %s\n",
                        x.cin, x.nom, x.prenom, x.specialite, x.tel,
                        x.sexe, x.jour, x.mois, x.annee, x.mdp);
            }
        }
        fclose(f);
        fclose(tmp);
        remove(FICHIER);
        rename("temp.txt", FICHIER);
        
        if (trouve)
            printf("✅ Entraîneur modifié avec succès.\n");
        else
            printf("❌ Entraîneur non trouvé.\n");
    } else {
        printf("❌ Erreur : impossible d'ouvrir les fichiers.\n");
        if (f) fclose(f);
        if (tmp) fclose(tmp);
    }
}




void supprimer_entraineur(const char *cin) {
    FILE *f = fopen(FICHIER, "r");
    FILE *tmp = fopen("temp.txt", "w");
    Entraineur x;
    int trouve = 0;
    
    if (f && tmp) {
        while (fscanf(f, "%11s %49s %49s %49s %19s %9s %d %d %d %49s",
                      x.cin, x.nom, x.prenom, x.specialite, x.tel,
                      x.sexe, &x.jour, &x.mois, &x.annee, x.mdp) == 10) {
            if (strcmp(x.cin, cin) != 0) {
                fprintf(tmp, "%s %s %s %s %s %s %d %d %d %s\n",
                        x.cin, x.nom, x.prenom, x.specialite, x.tel,
                        x.sexe, x.jour, x.mois, x.annee, x.mdp);
            } else {
                trouve = 1;
            }
        }
        fclose(f);
        fclose(tmp);
        remove(FICHIER);
        rename("temp.txt", FICHIER);
        
        if (trouve)
            printf("✅ Entraîneur supprimé avec succès.\n");
        else
            printf("❌ Entraîneur non trouvé.\n");
    } else {
        printf("❌ Erreur : impossible d'ouvrir les fichiers.\n");
        if (f) fclose(f);
        if (tmp) fclose(tmp);
    }
}

void inscrire_cours() {
    char cin[12];
    char cours[50];
    int jour, mois, annee;
    int c;

    printf("\n=== INSCRIPTION À UN COURS SPORTIF ===\n");

    
    do {
        printf("CIN de l'entraîneur : ");
        if (scanf("%11s", cin) != 1) cin[0] = '\0';
        while ((c = getchar()) != '\n' && c != EOF) {}
        if (!valider_cin(cin)) {
            printf(" CIN invalide.\n");
        }
    } while (!valider_cin(cin));


    Entraineur e = chercher_entraineur(cin);
    if (strlen(e.cin) == 0) {
        printf(" Aucun entraîneur trouvé avec ce CIN.\n");
        return;
    }

    
    do {
        printf("Nom du cours : ");
        if (scanf("%49s", cours) != 1) cours[0] = '\0';
        while ((c = getchar()) != '\n' && c != EOF) {}
        if (strlen(cours) == 0) {
            printf(" Nom du cours invalide.\n");
        }
    } while (strlen(cours) == 0);

    
    do {
        printf("Jour du cours : ");
        if (scanf("%d", &jour) != 1) jour = 0;
        while ((c = getchar()) != '\n' && c != EOF) {}

        printf("Mois du cours : ");
        if (scanf("%d", &mois) != 1) mois = 0;
        while ((c = getchar()) != '\n' && c != EOF) {}

        printf("Année du cours : ");
        if (scanf("%d", &annee) != 1) annee = 0;
        while ((c = getchar()) != '\n' && c != EOF) {}

        if (!valider_date(jour, mois, annee)) {
            printf(" Date invalide, réessayez.\n");
        }
    } while (!valider_date(jour, mois, annee));

 
    FILE *f = fopen("inscriptions.txt", "a");
    if (f == NULL) {
        printf(" Erreur d’ouverture du fichier d’inscriptions.\n");
        return;
    }

    fprintf(f, "%s %s %d %d %d\n", cin, cours, jour, mois, annee);
    fclose(f);

    printf(" %s %s a été inscrit au cours \"%s\" le %02d/%02d/%d.\n",
           e.nom, e.prenom, cours, jour, mois, annee);
}



Entraineur chercher_entraineur(const char *cin) {
    Entraineur x, vide = {"", "", "", "", "", "", 0, 0, 0, ""};
    FILE *f = fopen(FICHIER, "r");
    
    if (f) {
        while (fscanf(f, "%11s %49s %49s %49s %19s %9s %d %d %d %49s",
                      x.cin, x.nom, x.prenom, x.specialite, x.tel,
                      x.sexe, &x.jour, &x.mois, &x.annee, x.mdp) == 10) {
            if (strcmp(x.cin, cin) == 0) {
                fclose(f);
                return x;
            }
        }
        fclose(f);
    }
    return vide;
}
