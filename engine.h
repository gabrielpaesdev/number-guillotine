#ifndef ENGINE_H
#define ENGINE_H

#include <gtk/gtk.h>

#define MAX_NUM 100

/* Funções de lógica do jogo chamadas pela interface (main.c) */
void start_easy(GtkWidget *w, gpointer data);
void start_medium(GtkWidget *w, gpointer data);
void start_hard(GtkWidget *w, gpointer data);
void on_number_clicked(GtkWidget *widget, gpointer data);

/* Função auxiliar para atualizar botões (se necessário expor, mas geralmente interna) */
void update_buttons();

#endif
