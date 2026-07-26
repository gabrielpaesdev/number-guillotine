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
extern int is_anim_enabled;

static int secret_masked;
static int secret_mask;
static time_t start_time;

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

static int calculate_score(int remaining_tries, int elapsed_sec) {
int score = (remaining_tries * 1000) + 5000 - (elapsed_sec * 50);
    if (score < 0) {
        score = 0;
    }
    return score;
}

gboolean delayed_victory(gpointer data) {
    play_victory();
    return FALSE;
}

gboolean delayed_defeat(gpointer data) {
    play_defeat();
    return FALSE;
}

/* ---- domino-style elimination animation ---- */

static void set_button_eliminated_instant(int i) {
    GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
    char buf[16];
    sprintf(buf, "<s>%d</s>", i);
    gtk_label_set_markup(GTK_LABEL(label), buf);
    gtk_widget_set_sensitive(buttons[i], FALSE);

    GtkStyleContext *ctx = gtk_widget_get_style_context(buttons[i]);
    gtk_style_context_add_class(ctx, "domino-out");
}

static void set_button_active_instant(int i) {
    GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
    char buf[8];
    sprintf(buf, "%d", i);
    gtk_label_set_text(GTK_LABEL(label), buf);
    gtk_widget_set_sensitive(buttons[i], TRUE);

    GtkStyleContext *ctx = gtk_widget_get_style_context(buttons[i]);
    gtk_style_context_remove_class(ctx, "domino-out");
}

static gboolean domino_out_cb(gpointer data) {
    int i = GPOINTER_TO_INT(data);
    if (i < 1 || i > MAX_NUM) return FALSE;

    GtkWidget *label = gtk_bin_get_child(GTK_BIN(buttons[i]));
    char buf[16];
    sprintf(buf, "<s>%d</s>", i);
    gtk_label_set_markup(GTK_LABEL(label), buf);
    gtk_widget_set_sensitive(buttons[i], FALSE);
    gtk_style_context_add_class(gtk_widget_get_style_context(buttons[i]), "domino-out");
    return FALSE;
}


static void animate_domino_range(int from, int to) {
    int dir = (to >= from) ? 1 : -1;
    int count = abs(to - from) + 1;

    int delay_step = 220 / count;
    if (delay_step < 4) delay_step = 4;
    if (delay_step > 18) delay_step = 18;

    int idx = 0;
    for (int i = from; (dir > 0) ? (i <= to) : (i >= to); i += dir) {
        g_timeout_add(idx * delay_step, domino_out_cb, GINT_TO_POINTER(i));
        idx++;
    }
}

void update_buttons() {
    active_count = 0;

    for (int i = 1; i <= MAX_NUM; i++) {
        if (i < min || i > max) {
            set_button_eliminated_instant(i);
        } else {
            set_button_active_instant(i);
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
        time_t end_time = time(NULL);
        int elapsed_sec = (int)(end_time - start_time);

        int score = calculate_score(tries_left, elapsed_sec);

        char buf[256];
        sprintf(buf, "%s\n%s: %d", lang_get(STR_GAME_WIN), lang_get(STR_GAME_SCORE), score);
        gtk_label_set_text(GTK_LABEL(label_gameover), buf);
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "gameover");
        g_timeout_add(350, delayed_victory, NULL);
        return;
    }

    int old_min = min, old_max = max;
    int went_lower;

    if (num < secret) {
        min = num + 1;
        went_lower = 1;
        gtk_label_set_text(GTK_LABEL(label_info), lang_get(STR_GAME_BIGGER));
    } else {
        max = num - 1;
        went_lower = 0;
        gtk_label_set_text(GTK_LABEL(label_info), lang_get(STR_GAME_SMALLER));
    }

    active_count = (max >= min) ? (max - min + 1) : 0;

    char tbuf[64];
    sprintf(tbuf, lang_get(STR_GAME_TRIES), tries_left);
    gtk_label_set_text(GTK_LABEL(label_tries), tbuf);

    if (is_anim_enabled) {
        if (went_lower) {
            animate_domino_range(num, old_min);
        } else {
            animate_domino_range(num, old_max);
        }
    } else {
        if (went_lower) {
            for (int i = old_min; i <= min - 1; i++) set_button_eliminated_instant(i);
        } else {
            for (int i = max + 1; i <= old_max; i++) set_button_eliminated_instant(i);
        }
    }

    if (tries_left <= 0 || active_count <= 1) {
        char buf[128];
        sprintf(buf, lang_get(STR_GAME_LOSE), secret);
        gtk_label_set_text(GTK_LABEL(label_gameover), buf);
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "gameover");
        g_timeout_add(350, delayed_defeat, NULL);
        return;
    }

    play_click();
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

    start_time = time(NULL);

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
