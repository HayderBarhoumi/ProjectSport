#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "entraineur.h"

/* Variables globales pour stocker les fenêtres */
GtkWidget *window_acceuil = NULL;
GtkWidget *window_ajout = NULL;
GtkWidget *window_modif = NULL;
GtkWidget *window_suppression = NULL;
GtkWidget *window_inscription = NULL;

/* Variable pour stocker le CIN à supprimer */
char cin_a_supprimer[12] = "";

/* ========== FONCTIONS UTILITAIRES ========== */

void afficher_erreur(GtkWidget *parent, const char *message) {
    GtkWidget *dialog;
    GtkWidget *window = NULL;
    
    /* Essayer de trouver la fenêtre parente */
    if (parent != NULL) {
        window = parent;
        /* Remonter jusqu'à trouver une fenêtre */
        while (window && !GTK_IS_WINDOW(window)) {
            window = gtk_widget_get_parent(window);
        }
    }
    
    /* Si on n'a pas trouvé de fenêtre parente, utiliser NULL */
    if (window && !GTK_IS_WINDOW(window)) {
        window = NULL;
    }
    
    dialog = gtk_message_dialog_new(window ? GTK_WINDOW(window) : NULL,
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_ERROR,
                                     GTK_BUTTONS_OK,
                                     "%s", message);
    
    if (dialog) {
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void afficher_info(GtkWidget *parent, const char *message) {
    GtkWidget *dialog;
    GtkWidget *window = NULL;
    
    /* Essayer de trouver la fenêtre parente */
    if (parent != NULL) {
        window = parent;
        /* Remonter jusqu'à trouver une fenêtre */
        while (window && !GTK_IS_WINDOW(window)) {
            window = gtk_widget_get_parent(window);
        }
    }
    
    /* Si on n'a pas trouvé de fenêtre parente, utiliser NULL */
    if (window && !GTK_IS_WINDOW(window)) {
        window = NULL;
    }
    
    dialog = gtk_message_dialog_new(window ? GTK_WINDOW(window) : NULL,
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK,
                                     "%s", message);
    
    if (dialog) {
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}
void actualiser_treeview(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *f;
    Entraineur e;
    GList *columns;
    
    /* Créer le modèle avec les colonnes */
    store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
                               G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    
    /* Lire le fichier */
    f = fopen("entraineurs.txt", "r");
    if (f != NULL) {
        while (fscanf(f, "%11s %49s %49s %49s %19s %9s %d %d %d %49s",
                      e.cin, e.nom, e.prenom, e.specialite, e.tel,
                      e.sexe, &e.jour, &e.mois, &e.annee, e.mdp) == 10) {
            char date[20];
            snprintf(date, sizeof(date), "%02d/%02d/%d", e.jour, e.mois, e.annee);
            
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                             0, e.cin,
                             1, e.nom,
                             2, e.prenom,
                             3, e.specialite,
                             4, e.tel,
                             5, date,
                             -1);
        }
        fclose(f);
    }
    
    /* Appliquer le modèle */
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    /* Créer les colonnes si elles n'existent pas */
    columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    if (g_list_length(columns) == 0) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "CIN", 
                                                    renderer, "text", 0, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Nom", 
                                                    renderer, "text", 1, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Prénom", 
                                                    renderer, "text", 2, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Spécialité", 
                                                    renderer, "text", 3, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Téléphone", 
                                                    renderer, "text", 4, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Date Nais.", 
                                                    renderer, "text", 5, NULL);
    }
    if (columns) g_list_free(columns);
}

/* ========== CALLBACKS PAGE D'ACCUEIL ========== */

void on_button1_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Ajouter */
    if (window_ajout == NULL) {
        window_ajout = create_page_d_ajout();
        
        /* Connecter les signaux des boutons de la fenêtre d'ajout */
        GtkWidget *btn_enreg = lookup_widget(window_ajout, "button14_oussema");
        g_signal_connect(G_OBJECT(btn_enreg), "clicked", G_CALLBACK(on_button14_clicked), NULL);
        
        GtkWidget *btn_annul = lookup_widget(window_ajout, "button15_oussema");
        g_signal_connect(G_OBJECT(btn_annul), "clicked", G_CALLBACK(on_button15_clicked), NULL);
    }
    gtk_widget_show_all(window_ajout);
    gtk_window_present(GTK_WINDOW(window_ajout));
}

void on_button2_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Modifier */
    GtkWidget *entry_cin = lookup_widget(GTK_WIDGET(button), "entry17_oussema");
    const char *cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));
    
    /* Vérifier si le champ est vide */
    if (cin == NULL || strlen(cin) == 0) {
        afficher_erreur(GTK_WIDGET(button), "Veuillez saisir un CIN !");
        return;
    }
    
    if (!valider_cin(cin)) {
        afficher_erreur(GTK_WIDGET(button), "CIN invalide ! Le CIN doit contenir exactement 8 chiffres.");
        return;
    }
    
    Entraineur e = chercher_entraineur(cin);
    if (strlen(e.cin) == 0) {
        afficher_erreur(GTK_WIDGET(button), "Entraîneur non trouvé !");
        return;
    }
    
    /* Créer la fenêtre de modification */
    if (window_modif == NULL) {
        window_modif = create_page_modification();
        
        /* IMPORTANT : Corriger la taille de fixed10 */
        GtkWidget *fixed10 = lookup_widget(window_modif, "fixed10_oussema");
        if (fixed10) {
            gtk_widget_set_size_request(fixed10, 800, 600);
        }
        
        /* Connecter les signaux */
        GtkWidget *btn_enreg = lookup_widget(window_modif, "button28_oussema");
        g_signal_connect(G_OBJECT(btn_enreg), "clicked", G_CALLBACK(on_button28_clicked), NULL);
        
        GtkWidget *btn_annul = lookup_widget(window_modif, "button29_oussema");
        g_signal_connect(G_OBJECT(btn_annul), "clicked", G_CALLBACK(on_button29_clicked), NULL);
    }
    
    /* Remplir les champs avec les données existantes */
    GtkWidget *label_cin = lookup_widget(window_modif, "label104_oussema");
    gtk_label_set_text(GTK_LABEL(label_cin), e.cin);
    
    GtkWidget *entry_nom = lookup_widget(window_modif, "entry21_oussema");
    gtk_entry_set_text(GTK_ENTRY(entry_nom), e.nom);
    
    GtkWidget *entry_prenom = lookup_widget(window_modif, "entry22_oussema");
    gtk_entry_set_text(GTK_ENTRY(entry_prenom), e.prenom);
    
    GtkWidget *entry_tel = lookup_widget(window_modif, "entry23_oussema");
    gtk_entry_set_text(GTK_ENTRY(entry_tel), e.tel);
    
    GtkWidget *entry_mdp = lookup_widget(window_modif, "entry24_oussema");
    gtk_entry_set_text(GTK_ENTRY(entry_mdp), e.mdp);
    
    GtkWidget *combo = lookup_widget(window_modif, "comboboxentry4_oussema");
    gtk_entry_set_text(GTK_ENTRY(GTK_BIN(combo)->child), e.specialite);
    
    GtkWidget *spin_jour = lookup_widget(window_modif, "spinbutton8_oussema");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_jour), e.jour);
    
    GtkWidget *spin_mois = lookup_widget(window_modif, "spinbutton9_oussema");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_mois), e.mois);
    
    GtkWidget *spin_annee = lookup_widget(window_modif, "spinbutton10_oussema");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_annee), e.annee);
    
    if (strcmp(e.sexe, "H") == 0 || strcmp(e.sexe, "h") == 0) {
        GtkWidget *radio_h = lookup_widget(window_modif, "radiobutton4_oussema");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_h), TRUE);
    } else {
        GtkWidget *radio_f = lookup_widget(window_modif, "radiobutton5_oussema");
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_f), TRUE);
    }
    
    gtk_widget_show_all(window_modif);
    gtk_window_present(GTK_WINDOW(window_modif));
}

void on_button3_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Supprimer */
    GtkWidget *entry_cin = lookup_widget(GTK_WIDGET(button), "entry17_oussema");
    const char *cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));
    
    /* Vérifier si le champ est vide */
    if (cin == NULL || strlen(cin) == 0) {
        afficher_erreur(GTK_WIDGET(button), "Veuillez saisir un CIN !");
        return;
    }
    
    if (!valider_cin(cin)) {
        afficher_erreur(GTK_WIDGET(button), "CIN invalide ! Le CIN doit contenir exactement 8 chiffres.");
        return;
    }
    
    Entraineur e = chercher_entraineur(cin);
    if (strlen(e.cin) == 0) {
        afficher_erreur(GTK_WIDGET(button), "Entraîneur non trouvé !");
        return;
    }
    
    /* Sauvegarder le CIN */
    strcpy(cin_a_supprimer, cin);
    
    /* Afficher la fenêtre de confirmation */
    if (window_suppression == NULL) {
        window_suppression = create_page_de_suppression();
        
        /* Connecter les signaux */
        GtkWidget *btn_oui = lookup_widget(window_suppression, "button19_oussema");
        g_signal_connect(G_OBJECT(btn_oui), "clicked", G_CALLBACK(on_button19_clicked), NULL);
        
        GtkWidget *btn_annul = lookup_widget(window_suppression, "button20_oussema");
        g_signal_connect(G_OBJECT(btn_annul), "clicked", G_CALLBACK(on_button20_clicked), NULL);
    }
    gtk_widget_show_all(window_suppression);
    gtk_window_present(GTK_WINDOW(window_suppression));
}

void on_button4_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Chercher */
    GtkWidget *entry_cin = lookup_widget(GTK_WIDGET(button), "entry17_oussema");
    const char *cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));
    
    /* Vérifier si le champ est vide */
    if (cin == NULL || strlen(cin) == 0) {
        afficher_erreur(GTK_WIDGET(button), "Veuillez saisir un CIN !");
        return;
    }
    
    if (!valider_cin(cin)) {
        afficher_erreur(GTK_WIDGET(button), "CIN invalide ! Le CIN doit contenir exactement 8 chiffres.");
        return;
    }
    
    Entraineur e = chercher_entraineur(cin);
    if (strlen(e.cin) == 0) {
        afficher_erreur(GTK_WIDGET(button), "Entraîneur non trouvé !");
    } else {
        char message[500];
        snprintf(message, sizeof(message),
                 "CIN : %s\nNom : %s\nPrénom : %s\nSpécialité : %s\n"
                 "Téléphone : %s\nSexe : %s\nDate de naissance : %02d/%02d/%d",
                 e.cin, e.nom, e.prenom, e.specialite,
                 e.tel, e.sexe, e.jour, e.mois, e.annee);
        afficher_info(GTK_WIDGET(button), message);
    }
    
    /* Actualiser le treeview */
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "treeview2_oussema");
    actualiser_treeview(treeview);
}

void on_button5_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton S'inscrire */
    if (window_inscription == NULL) {
        window_inscription = create_page_d_inscription();
        
        /* Charger la liste des cours depuis le fichier */
        GtkWidget *combo_cours = lookup_widget(window_inscription, "combo1_oussema");
        if (combo_cours != NULL) {
            charger_liste_cours(combo_cours);
        }
        
        /* Connecter les signaux */
        GtkWidget *btn_valid = lookup_widget(window_inscription, "button2_oussema");
        g_signal_connect(G_OBJECT(btn_valid), "clicked", G_CALLBACK(on_button2_inscription_clicked), NULL);
        
        GtkWidget *btn_annul = lookup_widget(window_inscription, "button1_oussema");
        g_signal_connect(G_OBJECT(btn_annul), "clicked", G_CALLBACK(on_button1_inscription_clicked), NULL);
    }
    gtk_widget_show_all(window_inscription);
    gtk_window_present(GTK_WINDOW(window_inscription));
}

/* ========== CALLBACKS PAGE D'AJOUT ========== */

void on_button14_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Enregistrer (Ajout) */
    Entraineur e;
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    /* Récupérer les valeurs */
    GtkWidget *entry_cin = lookup_widget(window, "entry8_oussema");
    GtkWidget *entry_nom = lookup_widget(window, "entry9_oussema");
    GtkWidget *entry_prenom = lookup_widget(window, "entry10_oussema");
    GtkWidget *entry_tel = lookup_widget(window, "entry11_oussema");
    GtkWidget *entry_mdp = lookup_widget(window, "entry18_oussema");
    GtkWidget *combo = lookup_widget(window, "comboboxentry1_oussema");
    GtkWidget *spin_jour = lookup_widget(window, "spinbutton1_oussema");
    GtkWidget *spin_mois = lookup_widget(window, "spinbutton2_oussema");
    GtkWidget *spin_annee = lookup_widget(window, "spinbutton7_oussema");
    GtkWidget *radio_m = lookup_widget(window, "radiobutton1_oussema");
    
    const char *cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const char *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    const char *tel = gtk_entry_get_text(GTK_ENTRY(entry_tel));
    const char *mdp = gtk_entry_get_text(GTK_ENTRY(entry_mdp));
    const char *spec = gtk_entry_get_text(GTK_ENTRY(GTK_BIN(combo)->child));
    
    /* Validations */
    if (!valider_cin(cin)) {
        afficher_erreur(window, "CIN invalide ! Doit contenir exactement 8 chiffres.");
        return;
    }
    
    /* Vérifier si le CIN existe déjà */
    Entraineur existe = chercher_entraineur(cin);
    if (strlen(existe.cin) > 0) {
        afficher_erreur(window, "Un entraîneur avec ce CIN existe déjà !");
        return;
    }
    
    if (!valider_nom(nom)) {
        afficher_erreur(window, "Nom invalide ! Lettres uniquement.");
        return;
    }
    
    if (!valider_nom(prenom)) {
        afficher_erreur(window, "Prénom invalide ! Lettres uniquement.");
        return;
    }
    
    if (!valider_telephone(tel)) {
        afficher_erreur(window, "Téléphone invalide ! Doit contenir exactement 8 chiffres.");
        return;
    }
    
    if (strlen(spec) == 0) {
        afficher_erreur(window, "Veuillez sélectionner une spécialité !");
        return;
    }
    
    if (strlen(mdp) == 0) {
        afficher_erreur(window, "Veuillez saisir un mot de passe !");
        return;
    }
    
    /* Récupérer les valeurs */
    int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
    int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
    int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));
    
    if (!valider_date(jour, mois, annee)) {
        afficher_erreur(window, "Date invalide !");
        return;
    }
    
    /* Remplir la structure */
    strcpy(e.cin, cin);
    strcpy(e.nom, nom);
    strcpy(e.prenom, prenom);
    strcpy(e.specialite, spec);
    strcpy(e.tel, tel);
    strcpy(e.mdp, mdp);
    e.jour = jour;
    e.mois = mois;
    e.annee = annee;
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_m))) {
        strcpy(e.sexe, "H");
    } else {
        strcpy(e.sexe, "F");
    }
    
    /* Ajouter l'entraîneur */
    ajouter_entraineur(e);
    afficher_info(window, "Entraîneur ajouté avec succès !");
    
    /* Vider les champs */
    gtk_entry_set_text(GTK_ENTRY(entry_cin), "");
    gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
    gtk_entry_set_text(GTK_ENTRY(entry_prenom), "");
    gtk_entry_set_text(GTK_ENTRY(entry_tel), "");
    gtk_entry_set_text(GTK_ENTRY(entry_mdp), "");
    
    /* Actualiser le treeview de la page d'accueil si elle existe */
    if (window_acceuil != NULL) {
        GtkWidget *treeview = lookup_widget(window_acceuil, "treeview2_oussema");
        if (treeview != NULL) {
            actualiser_treeview(treeview);
        }
    }
    
    gtk_widget_hide(window);
}

void on_button15_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Annuler (Ajout) */
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_hide(window);
}

/* ========== CALLBACKS PAGE DE MODIFICATION ========== */

void on_button28_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Enregistrer (Modification) */
    Entraineur e;
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    /* Récupérer le CIN (non modifiable) */
    GtkWidget *label_cin = lookup_widget(window, "label104_oussema");
    const char *cin = gtk_label_get_text(GTK_LABEL(label_cin));
    
    if (strlen(cin) == 0 || strcmp(cin, "*****") == 0) {
        afficher_erreur(window, "Erreur : CIN non trouvé !");
        return;
    }
    
    /* Récupérer les autres champs */
    GtkWidget *entry_nom = lookup_widget(window, "entry21_oussema");
    GtkWidget *entry_prenom = lookup_widget(window, "entry22_oussema");
    GtkWidget *entry_tel = lookup_widget(window, "entry23_oussema");
    GtkWidget *entry_mdp = lookup_widget(window, "entry24_oussema");
    GtkWidget *combo = lookup_widget(window, "comboboxentry4_oussema");
    GtkWidget *spin_jour = lookup_widget(window, "spinbutton8_oussema");
    GtkWidget *spin_mois = lookup_widget(window, "spinbutton9_oussema");
    GtkWidget *spin_annee = lookup_widget(window, "spinbutton10_oussema");
    GtkWidget *radio_m = lookup_widget(window, "radiobutton4_oussema");
    
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const char *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    const char *tel = gtk_entry_get_text(GTK_ENTRY(entry_tel));
    const char *mdp = gtk_entry_get_text(GTK_ENTRY(entry_mdp));
    const char *spec = gtk_entry_get_text(GTK_ENTRY(GTK_BIN(combo)->child));
    
    /* Validations */
    if (!valider_nom(nom)) {
        afficher_erreur(window, "Nom invalide !");
        return;
    }
    
    if (!valider_nom(prenom)) {
        afficher_erreur(window, "Prénom invalide !");
        return;
    }
    
    if (!valider_telephone(tel)) {
        afficher_erreur(window, "Téléphone invalide !");
        return;
    }
    
    int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
    int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
    int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));
    
    if (!valider_date(jour, mois, annee)) {
        afficher_erreur(window, "Date invalide !");
        return;
    }
    
    /* Remplir la structure */
    strcpy(e.cin, cin);
    strcpy(e.nom, nom);
    strcpy(e.prenom, prenom);
    strcpy(e.specialite, spec);
    strcpy(e.tel, tel);
    strcpy(e.mdp, mdp);
    e.jour = jour;
    e.mois = mois;
    e.annee = annee;
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_m))) {
        strcpy(e.sexe, "H");
    } else {
        strcpy(e.sexe, "F");
    }
    
    /* Modifier l'entraîneur */
    modifier_entraineur(e);
    afficher_info(window, "Entraîneur modifié avec succès !");
    
    /* Actualiser le treeview */
    if (window_acceuil != NULL) {
        GtkWidget *treeview = lookup_widget(window_acceuil, "treeview2_oussema");
        if (treeview != NULL) {
            actualiser_treeview(treeview);
        }
    }
    
    gtk_widget_hide(window);
}

void on_button29_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Annuler (Modification) */
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_hide(window);
}

/* ========== CALLBACKS PAGE DE SUPPRESSION ========== */

void on_button19_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Oui (Suppression) */
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    if (strlen(cin_a_supprimer) > 0) {
        supprimer_entraineur(cin_a_supprimer);
        afficher_info(window, "Entraîneur supprimé avec succès !");
        
        /* Actualiser le treeview */
        if (window_acceuil != NULL) {
            GtkWidget *treeview = lookup_widget(window_acceuil, "treeview2_oussema");
            if (treeview != NULL) {
                actualiser_treeview(treeview);
            }
            
            /* Vider le champ CIN */
            GtkWidget *entry_cin = lookup_widget(window_acceuil, "entry17_oussema");
            if (entry_cin != NULL) {
                gtk_entry_set_text(GTK_ENTRY(entry_cin), "");
            }
        }
        
        cin_a_supprimer[0] = '\0';
    }
    
    gtk_widget_hide(window);
}

void on_button20_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Annuler (Suppression) */
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    cin_a_supprimer[0] = '\0';
    gtk_widget_hide(window);
}

/* ========== CALLBACKS PAGE D'INSCRIPTION ========== */

void on_button2_inscription_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Valider (Inscription) */
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    /* Récupérer le CIN depuis entry1_oussema */
    GtkWidget *entry_cin = lookup_widget(window, "entry1_oussema");
    const char *cin = gtk_entry_get_text(GTK_ENTRY(entry_cin));
    
    if (cin == NULL || strlen(cin) == 0) {
        afficher_erreur(window, "Veuillez saisir un CIN d'entraîneur !");
        return;
    }
    
    if (!valider_cin(cin)) {
        afficher_erreur(window, "CIN invalide ! Le CIN doit contenir exactement 8 chiffres.");
        return;
    }
    
    /* Vérifier si l'entraîneur existe */
    Entraineur e = chercher_entraineur(cin);
    if (strlen(e.cin) == 0) {
        afficher_erreur(window, "Aucun entraîneur trouvé avec ce CIN !");
        return;
    }
    
    /* Afficher le nom dans entry20_oussema */
    GtkWidget *entry_nom = lookup_widget(window, "entry20_oussema");
    char nom_complet[100];
    snprintf(nom_complet, sizeof(nom_complet), "%s %s", e.nom, e.prenom);
    gtk_entry_set_text(GTK_ENTRY(entry_nom), nom_complet);
    
    /* Récupérer le cours depuis combo1_oussema */
    GtkWidget *combo_cours = lookup_widget(window, "combo1_oussema");
    const char *cours = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo_cours)->entry));
    
    if (strlen(cours) == 0) {
        afficher_erreur(window, "Veuillez sélectionner un cours !");
        return;
    }
    
    /* Récupérer la date depuis calendar1_oussema */
    GtkWidget *calendar = lookup_widget(window, "calendar1_oussema");
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
    month++; /* GTK calendar months are 0-indexed */
    
    /* Enregistrer l'inscription */
    FILE *f = fopen("inscriptions.txt", "a");
    if (f == NULL) {
        afficher_erreur(window, "Erreur d'ouverture du fichier d'inscriptions !");
        return;
    }
    
    fprintf(f, "%s %s %02d %02d %d\n", cin, cours, day, month, year);
    fclose(f);
    
    char message[200];
    snprintf(message, sizeof(message), 
             "%s %s a été inscrit au cours \"%s\" le %02d/%02d/%d",
             e.nom, e.prenom, cours, day, month, year);
    afficher_info(window, message);
    
    /* Vider les champs */
    gtk_entry_set_text(GTK_ENTRY(entry_cin), "");
    gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
    
    gtk_widget_hide(window);
}

void charger_liste_cours(GtkWidget *combo) {
    FILE *f = fopen("liste_cours.txt", "r");
    if (f == NULL) {
        return;
    }
    
    char cours[50];
    GList *items = NULL;
    
    while (fgets(cours, sizeof(cours), f) != NULL) {
        /* Enlever le saut de ligne */
        cours[strcspn(cours, "\n")] = 0;
        if (strlen(cours) > 0) {
            items = g_list_append(items, g_strdup(cours));
        }
    }
    fclose(f);
    
    if (items != NULL) {
        gtk_combo_set_popdown_strings(GTK_COMBO(combo), items);
        g_list_free(items);
    }
}
void on_button1_inscription_clicked(GtkButton *button, gpointer user_data) {
    /* Bouton Annuler (Inscription) */
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_hide(window);
}
