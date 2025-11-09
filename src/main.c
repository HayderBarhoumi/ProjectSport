#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include "interface.h"
#include "support.h"
#include "callbacks.h"

/* Variable globale externe */
extern GtkWidget *window_acceuil;

int main(int argc, char *argv[]) {
    
    gtk_set_locale();
    gtk_init(&argc, &argv);

    /* Ajouter le répertoire courant pour les pixmaps */
    add_pixmap_directory(".");
    add_pixmap_directory("./pixmaps");
    add_pixmap_directory("../pixmaps");

    /* Créer et afficher uniquement la page d'accueil */
    window_acceuil = create_page_d_acceuil_oussema();
    
    /* Connecter les signaux manuellement */
    GtkWidget *button1 = lookup_widget(window_acceuil, "button1_oussema");
    g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(on_button1_clicked), NULL);
    
    GtkWidget *button2 = lookup_widget(window_acceuil, "button2_oussema");
    g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(on_button2_clicked), NULL);
    
    GtkWidget *button3 = lookup_widget(window_acceuil, "button3_oussema");
    g_signal_connect(G_OBJECT(button3), "clicked", G_CALLBACK(on_button3_clicked), NULL);
    
    GtkWidget *button4 = lookup_widget(window_acceuil, "button4_oussema");
    g_signal_connect(G_OBJECT(button4), "clicked", G_CALLBACK(on_button4_clicked), NULL);
    
    GtkWidget *button5 = lookup_widget(window_acceuil, "button5_oussema");
    g_signal_connect(G_OBJECT(button5), "clicked", G_CALLBACK(on_button5_clicked), NULL);
    
    /* Initialiser le TreeView */
    GtkWidget *treeview = lookup_widget(window_acceuil, "treeview2_oussema");
    actualiser_treeview(treeview);
    
    /* Connecter le signal de fermeture de fenêtre */
    g_signal_connect(G_OBJECT(window_acceuil), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_widget_show(window_acceuil);
    gtk_main();
    
    return 0;
}
