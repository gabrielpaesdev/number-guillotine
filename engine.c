#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "lang.h"
#include "engine.h"


extern GtkWidget *stack;
extern GtkWidget *label_info;
extern GtkWidget *label_tries;
extern GtkWidget *label_gameover;
extern GtkWidget *buttons[MAX_NUM+1];


int secret;
int min = 1, max = MAX_NUM;
int tries_left = 0;
int active_count = MAX_NUM;

/* ---------- LÓGICA DO JOGO ---------- */

void update_buttons() {
    active_count = 0;

    for (int i = 1; i <= MAX_NUM; i++) {
        GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
        char buf[64];

        if (i < min || i > max) {
            sprintf(buf, "<s>%d</s>", i);
            gtk_label_set_markup(GTK_LABEL(label), buf);
            gtk_widget_set_sensitive(buttons[i], FALSE);
        } else {
            sprintf(buf, "%d", i);
            gtk_label_set_text(GTK_LABEL(label), buf);
            gtk_widget_set_sensitive(buttons[i], TRUE);
            active_count++;
        }
    }

    char tbuf[64];
    sprintf(tbuf, lang_get(STR_GAME_TRIES), tries_left);
    gtk_label_set_text(GTK_LABEL(label_tries), tbuf);

    if (active_count == 1) {
        char buf[128];
        sprintf(buf, lang_get(STR_GAME_LOSE), secret);
        gtk_label_set_text(GTK_LABEL(label_gameover), buf);
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "gameover");
    }
}

void on_number_clicked(GtkWidget *widget, gpointer data) {
    int num = GPOINTER_TO_INT(data);
    tries_left--;

    if (num == secret) {
        gtk_label_set_text(GTK_LABEL(label_gameover), lang_get(STR_GAME_WIN));
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "gameover");
        return;
    }

    if (num < secret) {
        min = num + 1;
        gtk_label_set_text(GTK_LABEL(label_info), lang_get(STR_GAME_BIGGER));
    } else {
        max = num - 1;
        gtk_label_set_text(GTK_LABEL(label_info), lang_get(STR_GAME_SMALLER));
    }

    if (tries_left <= 0) {
        char buf[128];
        sprintf(buf, lang_get(STR_GAME_LOSE), secret);
        gtk_label_set_text(GTK_LABEL(label_gameover), buf);
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "gameover");
        return;
    }

    update_buttons();
}

void start_game(int tries) {
    srand(time(NULL));
    secret = (rand() % MAX_NUM) + 1;
    min = 1;
    max = MAX_NUM;
    tries_left = tries;

    gtk_label_set_text(GTK_LABEL(label_info), lang_get(STR_GAME_INSTRUCT));
    update_buttons();
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "game");
}

void start_easy(GtkWidget *w, gpointer data)   { start_game(8); }
void start_medium(GtkWidget *w, gpointer data) { start_game(6); }
void start_hard(GtkWidget *w, gpointer data)   { start_game(4); }
