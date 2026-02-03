#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUM 100

GtkWidget *window;
GtkWidget *stack;
GtkWidget *buttons[MAX_NUM+1];
GtkWidget *label_info;
GtkWidget *label_tries;
GtkWidget *label_gameover;

int secret;
int min = 1, max = MAX_NUM;
int tries_left = 0;
int active_count = MAX_NUM;

/* ---------- CSS ---------- */

void load_css() {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { background: #121212; }\n"
        "label { color: #eeeeee; font-size: 18px; }\n"
        "button { background: #2c2c2c; color: white; border-radius: 8px; padding: 10px; }\n"
        "button:hover { background: #444444; }\n"
        ".title { font-size: 32px; font-weight: bold; }\n"
        ".info { font-size: 20px; color: #00ffaa; }\n"
        ".danger { font-size: 28px; color: #ff5555; }\n",
        -1, NULL);

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}

/* ---------- PROTÓTIPOS ---------- */
GtkWidget* create_menu();
GtkWidget* create_difficulty();
GtkWidget* create_game();
GtkWidget* create_gameover();
GtkWidget* create_credits();

void start_easy(GtkWidget *w, gpointer data);
void start_medium(GtkWidget *w, gpointer data);
void start_hard(GtkWidget *w, gpointer data);

/* ---------- NAVEGAÇÃO ---------- */

void go_menu(GtkWidget *w, gpointer data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "menu");
}

void go_credits(GtkWidget *w, gpointer data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "credits");
}

void go_difficulty(GtkWidget *w, gpointer data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "difficulty");
}

/* ---------- LÓGICA ---------- */

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
    sprintf(tbuf, "Tentativas restantes: %d", tries_left);
    gtk_label_set_text(GTK_LABEL(label_tries), tbuf);

    if (active_count == 1) {
        char buf[128];
        sprintf(buf, "GAME OVER\nO número certo era: %d", secret);
        gtk_label_set_text(GTK_LABEL(label_gameover), buf);
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "gameover");
    }
}

void on_number_clicked(GtkWidget *widget, gpointer data) {
    int num = GPOINTER_TO_INT(data);
    tries_left--;

    if (num == secret) {
        gtk_label_set_text(GTK_LABEL(label_gameover),
            "PARABÉNS!\nVocê venceu!");
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "gameover");
        return;
    }

    if (num < secret) {
        min = num + 1;
        gtk_label_set_text(GTK_LABEL(label_info), "O número é MAIOR");
    } else {
        max = num - 1;
        gtk_label_set_text(GTK_LABEL(label_info), "O número é MENOR");
    }

    if (tries_left <= 0) {
        char buf[128];
        sprintf(buf, "GAME OVER\nO número certo era: %d", secret);
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

    gtk_label_set_text(GTK_LABEL(label_info), "Escolha um número");
    update_buttons();
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "game");
}

void start_easy(GtkWidget *w, gpointer data)  { start_game(8); }
void start_medium(GtkWidget *w, gpointer data){ start_game(6); }
void start_hard(GtkWidget *w, gpointer data)  { start_game(4); }

/* ---------- TELAS ---------- */

GtkWidget* create_menu() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    GtkWidget *title = gtk_label_new("Number Guillotine");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "title");

    GtkWidget *btn_play = gtk_button_new_with_label("Jogar");
    GtkWidget *btn_credits = gtk_button_new_with_label("Créditos");
    GtkWidget *btn_exit = gtk_button_new_with_label("Sair");

    g_signal_connect(btn_play, "clicked", G_CALLBACK(go_difficulty), NULL);
    g_signal_connect(btn_credits, "clicked", G_CALLBACK(go_credits), NULL);
    g_signal_connect(btn_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(box), btn_play, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_credits, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_exit, FALSE, FALSE, 0);

    return box;
}

GtkWidget* create_difficulty() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    GtkWidget *lbl = gtk_label_new("Escolha a dificuldade");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl), "title");

    GtkWidget *btn_easy = gtk_button_new_with_label("Fácil (8 tentativas)");
    GtkWidget *btn_med  = gtk_button_new_with_label("Médio (6 tentativas)");
    GtkWidget *btn_hard = gtk_button_new_with_label("Difícil (4 tentativas)");

    g_signal_connect(btn_easy, "clicked", G_CALLBACK(start_easy), NULL);
    g_signal_connect(btn_med,  "clicked", G_CALLBACK(start_medium), NULL);
    g_signal_connect(btn_hard, "clicked", G_CALLBACK(start_hard), NULL);

    gtk_box_pack_start(GTK_BOX(box), lbl, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_easy, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_med, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_hard, FALSE, FALSE, 0);

    return box;
}

GtkWidget* create_game() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    label_info = gtk_label_new("Escolha um número");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_info), "info");

    label_tries = gtk_label_new("");

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    int cols = 10;
    for (int i = 1; i <= MAX_NUM; i++) {
        char buf[8];
        sprintf(buf, "%d", i);
        GtkWidget *btn = gtk_button_new_with_label(buf);
        buttons[i] = btn;

        gtk_widget_set_size_request(btn, 50, 40);

        g_signal_connect(btn, "clicked", G_CALLBACK(on_number_clicked),
                         GINT_TO_POINTER(i));

        gtk_grid_attach(GTK_GRID(grid), btn, (i-1)%cols, (i-1)/cols, 1, 1);
    }

    gtk_box_pack_start(GTK_BOX(box), label_info, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label_tries, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);

    return box;
}

GtkWidget* create_gameover() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    label_gameover = gtk_label_new("");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_gameover), "danger");

    GtkWidget *btn = gtk_button_new_with_label("Voltar ao Menu");
    g_signal_connect(btn, "clicked", G_CALLBACK(go_menu), NULL);

    gtk_box_pack_start(GTK_BOX(box), label_gameover, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn, FALSE, FALSE, 0);

    return box;
}

GtkWidget* create_credits() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(box, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    GtkWidget *lbl = gtk_label_new(
        "Gabriel Paes 2026\n\n"
        "gabriel.paesbarreto@ufrpe.br"
    );

    GtkWidget *btn = gtk_button_new_with_label("Voltar");
    g_signal_connect(btn, "clicked", G_CALLBACK(go_menu), NULL);

    gtk_box_pack_start(GTK_BOX(box), lbl, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn, FALSE, FALSE, 0);

    return box;
}

/* ---------- MAIN ---------- */

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    load_css();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Number Guillotine v1.1.0");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 700);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    stack = gtk_stack_new();
    gtk_container_add(GTK_CONTAINER(window), stack);

    gtk_stack_add_named(GTK_STACK(stack), create_menu(), "menu");
    gtk_stack_add_named(GTK_STACK(stack), create_difficulty(), "difficulty");
    gtk_stack_add_named(GTK_STACK(stack), create_game(), "game");
    gtk_stack_add_named(GTK_STACK(stack), create_gameover(), "gameover");
    gtk_stack_add_named(GTK_STACK(stack), create_credits(), "credits");

    gtk_stack_set_visible_child_name(GTK_STACK(stack), "menu");

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}

