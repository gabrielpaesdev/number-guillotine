#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/random.h>
#include "lang.h"
#include "engine.h"
#include "sfx.h"

extern GtkWidget *stack;
extern GtkWidget *label_info;
extern GtkWidget *label_tries;
extern GtkWidget *label_gameover;
extern GtkWidget *buttons[MAX_NUM+1];

static int secret_masked;
static int secret_mask;
int min = 1, max = MAX_NUM;
int tries_left = 0;
int active_count = MAX_NUM;
int current_difficulty_tries = 8;

static void set_secret(int value) {
    ssize_t r = getrandom(&secret_mask, sizeof(secret_mask), 0);
    (void)r;
    secret_masked = value ^ secret_mask;
}

static int get_secret(void) {
    return secret_masked ^ secret_mask;
}

gboolean delayed_victory(gpointer data) {
    play_victory();
    return FALSE;
}

gboolean delayed_defeat(gpointer data) {
    play_defeat();
    return FALSE;
}

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
        sprintf(buf, lang_get(STR_GAME_LOSE), get_secret());
        gtk_label_set_text(GTK_LABEL(label_gameover), buf);
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "gameover");
        g_timeout_add(350, delayed_defeat, NULL);
    }
}

void on_number_clicked(GtkWidget *widget, gpointer data) {
    int num = GPOINTER_TO_INT(data);
    tries_left--;
    int secret = get_secret();

    if (num == secret) {
        gtk_label_set_text(GTK_LABEL(label_gameover), lang_get(STR_GAME_WIN));
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "gameover");
        g_timeout_add(350, delayed_victory, NULL);
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
        g_timeout_add(350, delayed_defeat, NULL);
        return;
    }

    play_click();
    update_buttons();
}

void start_game(int tries) {
    current_difficulty_tries = tries;

    unsigned int seed;
    ssize_t rr = getrandom(&seed, sizeof(seed), 0);
    (void)rr;
    srand(seed);
    set_secret((rand() % MAX_NUM) + 1);
    min = 1;
    max = MAX_NUM;
    tries_left = tries;

    gtk_label_set_text(GTK_LABEL(label_info), lang_get(STR_GAME_INSTRUCT));
    update_buttons();
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "game");
    play_click();
}

void restart_game(GtkWidget *w, gpointer data) {
    start_game(current_difficulty_tries);
}

void start_easy(GtkWidget *w, gpointer data)   { start_game(8); }
void start_medium(GtkWidget *w, gpointer data) { start_game(6); }
void start_hard(GtkWidget *w, gpointer data)   { start_game(4); }
