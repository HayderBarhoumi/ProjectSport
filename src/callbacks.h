#include <gtk/gtk.h>

/* Callbacks pour la page d'accueil */
void on_button1_clicked(GtkButton *button, gpointer user_data);
void on_button2_clicked(GtkButton *button, gpointer user_data);
void on_button3_clicked(GtkButton *button, gpointer user_data);
void on_button4_clicked(GtkButton *button, gpointer user_data);
void on_button5_clicked(GtkButton *button, gpointer user_data);

/* Callbacks pour la page d'ajout */
void on_button14_clicked(GtkButton *button, gpointer user_data);
void on_button15_clicked(GtkButton *button, gpointer user_data);

/* Callbacks pour la page de modification */
void on_button28_clicked(GtkButton *button, gpointer user_data);
void on_button29_clicked(GtkButton *button, gpointer user_data);

/* Callbacks pour la page de suppression */
void on_button19_clicked(GtkButton *button, gpointer user_data);
void on_button20_clicked(GtkButton *button, gpointer user_data);

/* Callbacks pour la page d'inscription */
void on_button1_inscription_clicked(GtkButton *button, gpointer user_data);
void on_button2_inscription_clicked(GtkButton *button, gpointer user_data);
void charger_liste_cours(GtkWidget *combo);

/* Fonctions utilitaires */
void afficher_erreur(GtkWidget *parent, const char *message);
void afficher_info(GtkWidget *parent, const char *message);
void actualiser_treeview(GtkWidget *treeview);
