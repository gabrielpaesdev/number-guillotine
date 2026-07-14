#ifndef ENGINE_H
#define ENGINE_H

#include <gtk/gtk.h>

#define MAX_NUM 100

void start_easy(GtkWidget *w, gpointer data);
void start_medium(GtkWidget *w, gpointer data);
void start_hard(GtkWidget *w, gpointer data);
void on_number_clicked(GtkWidget *widget, gpointer data);
void restart_game(GtkWidget *w, gpointer data);


void update_buttons();

#endif
