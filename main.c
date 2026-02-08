#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include "lang.h"

#define MAX_NUM 100
#define BUILD_VERSION "v1.2.0"



GtkWidget *window;
GtkWidget *stack;


GtkWidget *lbl_menu_title;
GtkWidget *btn_menu_play;
GtkWidget *btn_menu_settings;
GtkWidget *btn_menu_credits;
GtkWidget *btn_menu_exit;


GtkWidget *lbl_diff_title;
GtkWidget *btn_diff_easy;
GtkWidget *btn_diff_med;
GtkWidget *btn_diff_hard;
GtkWidget *btn_diff_back;


GtkWidget *lbl_settings_title;
GtkWidget *lbl_settings_lang;
GtkWidget *btn_settings_back;


GtkWidget *lbl_credits_title;
GtkWidget *lbl_credits_info;
GtkWidget *btn_credits_back;


GtkWidget *buttons[MAX_NUM+1]; 
GtkWidget *label_info;         
GtkWidget *label_tries;
GtkWidget *label_gameover;     
GtkWidget *btn_gameover_back;
GtkWidget *game_box;           



/* ---------- ESTADO DO JOGO ---------- */
int secret;
int min = 1, max = MAX_NUM;
int tries_left = 0;
int active_count = MAX_NUM;

/* ---------- PROTÓTIPOS ---------- */
void update_interface_text();
void go_menu(GtkWidget *w, gpointer data);

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
        ".danger { font-size: 28px; color: #ff5555;}\n"
        ".copyright { font-size: 12px; color: #888888; }\n"
        ".active-lang { background: #0055aa; border: 1px solid #0088ff; }\n",
        -1, NULL);

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}

/* ---------- LÓGICA DE IDIOMA ---------- */

void update_interface_text() {
    
    gtk_label_set_text(GTK_LABEL(lbl_menu_title), lang_get(STR_TITLE));
    gtk_button_set_label(GTK_BUTTON(btn_menu_play), lang_get(STR_BTN_PLAY));
    gtk_button_set_label(GTK_BUTTON(btn_menu_settings), lang_get(STR_BTN_SETTINGS));
    gtk_button_set_label(GTK_BUTTON(btn_menu_credits), lang_get(STR_BTN_CREDITS));
    gtk_button_set_label(GTK_BUTTON(btn_menu_exit), lang_get(STR_BTN_EXIT));

    
    gtk_label_set_text(GTK_LABEL(lbl_diff_title), lang_get(STR_DIFF_TITLE));
    gtk_button_set_label(GTK_BUTTON(btn_diff_easy), lang_get(STR_DIFF_EASY));
    gtk_button_set_label(GTK_BUTTON(btn_diff_med), lang_get(STR_DIFF_MED));
    gtk_button_set_label(GTK_BUTTON(btn_diff_hard), lang_get(STR_DIFF_HARD));
    gtk_button_set_label(GTK_BUTTON(btn_diff_back), lang_get(STR_BTN_BACK));

    
    gtk_label_set_text(GTK_LABEL(lbl_settings_title), lang_get(STR_SETTINGS_TITLE));
    gtk_label_set_text(GTK_LABEL(lbl_settings_lang), lang_get(STR_SETTINGS_LANG_LABEL));
    gtk_button_set_label(GTK_BUTTON(btn_settings_back), lang_get(STR_BTN_BACK));

    
    gtk_label_set_text(GTK_LABEL(lbl_credits_title), lang_get(STR_CREDITS_TITLE));
    
    char buf_credits[256];
    sprintf(buf_credits, lang_get(STR_CREDITS_BODY), BUILD_VERSION);
    gtk_label_set_text(GTK_LABEL(lbl_credits_info), buf_credits);
    
    gtk_button_set_label(GTK_BUTTON(btn_credits_back), lang_get(STR_BTN_BACK));

    
    gtk_button_set_label(GTK_BUTTON(btn_gameover_back), lang_get(STR_BTN_BACK)); 

    
    gtk_label_set_text(GTK_LABEL(label_info), lang_get(STR_GAME_INSTRUCT));
}

void set_lang_en(GtkWidget *w, gpointer data) {
    lang_set(LANG_EN);
    update_interface_text();
}

void set_lang_pt(GtkWidget *w, gpointer data) {
    lang_set(LANG_PT);
    update_interface_text();
}

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

void go_settings(GtkWidget *w, gpointer data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "settings");
}

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

void start_easy(GtkWidget *w, gpointer data)  { start_game(8); }
void start_medium(GtkWidget *w, gpointer data){ start_game(6); }
void start_hard(GtkWidget *w, gpointer data)  { start_game(4); }

/* ---------- CONSTRUTORES DE TELA ---------- */

GtkWidget* create_box_with_copyright(GtkWidget *main_content) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), main_content, TRUE, TRUE, 0);

    GtkWidget *lbl_cpy = gtk_label_new(lang_get(STR_COPYRIGHT)); 
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_cpy), "copyright");
    gtk_widget_set_halign(lbl_cpy, GTK_ALIGN_CENTER);

    gtk_box_pack_start(GTK_BOX(vbox), lbl_cpy, FALSE, FALSE, 10);
    return vbox;
}

GtkWidget* create_menu() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    lbl_menu_title = gtk_label_new("");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_menu_title), "title");

    btn_menu_play = gtk_button_new_with_label("");
    btn_menu_settings = gtk_button_new_with_label("");
    btn_menu_credits = gtk_button_new_with_label("");
    btn_menu_exit = gtk_button_new_with_label("");

    g_signal_connect(btn_menu_play, "clicked", G_CALLBACK(go_difficulty), NULL);
    g_signal_connect(btn_menu_settings, "clicked", G_CALLBACK(go_settings), NULL);
    g_signal_connect(btn_menu_credits, "clicked", G_CALLBACK(go_credits), NULL);
    g_signal_connect(btn_menu_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_box_pack_start(GTK_BOX(box), lbl_menu_title, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(box), btn_menu_play, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_menu_settings, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_menu_credits, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_menu_exit, FALSE, FALSE, 0);

    return create_box_with_copyright(box);
}

GtkWidget* create_difficulty() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    lbl_diff_title = gtk_label_new("");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_diff_title), "title");

    btn_diff_easy = gtk_button_new_with_label("");
    btn_diff_med  = gtk_button_new_with_label("");
    btn_diff_hard = gtk_button_new_with_label("");
    btn_diff_back = gtk_button_new_with_label("");

    g_signal_connect(btn_diff_easy, "clicked", G_CALLBACK(start_easy), NULL);
    g_signal_connect(btn_diff_med,  "clicked", G_CALLBACK(start_medium), NULL);
    g_signal_connect(btn_diff_hard, "clicked", G_CALLBACK(start_hard), NULL);
    g_signal_connect(btn_diff_back, "clicked", G_CALLBACK(go_menu), NULL);

    gtk_box_pack_start(GTK_BOX(box), lbl_diff_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_diff_easy, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_diff_med, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_diff_hard, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_diff_back, FALSE, FALSE, 20);

    return create_box_with_copyright(box);
}

GtkWidget* create_settings() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    lbl_settings_title = gtk_label_new("");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_settings_title), "title");

    lbl_settings_lang = gtk_label_new("");
    
    GtkWidget *box_lang = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(box_lang, GTK_ALIGN_CENTER);
    
    GtkWidget *btn_en = gtk_button_new_with_label("English");
    GtkWidget *btn_pt = gtk_button_new_with_label("Português");
    
    g_signal_connect(btn_en, "clicked", G_CALLBACK(set_lang_en), NULL);
    g_signal_connect(btn_pt, "clicked", G_CALLBACK(set_lang_pt), NULL);
    
    gtk_box_pack_start(GTK_BOX(box_lang), btn_en, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_lang), btn_pt, FALSE, FALSE, 0);

    btn_settings_back = gtk_button_new_with_label("");
    g_signal_connect(btn_settings_back, "clicked", G_CALLBACK(go_menu), NULL);

    gtk_box_pack_start(GTK_BOX(box), lbl_settings_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), lbl_settings_lang, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), box_lang, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(box), btn_settings_back, FALSE, FALSE, 20);

    return create_box_with_copyright(box);
}

GtkWidget* create_game() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    label_info = gtk_label_new("");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_info), "info");

    label_tries = gtk_label_new("");

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);

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

    return create_box_with_copyright(box);
}

GtkWidget* create_gameover() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    label_gameover = gtk_label_new("");
    gtk_label_set_justify(GTK_LABEL(label_gameover), GTK_JUSTIFY_CENTER);
    gtk_style_context_add_class(gtk_widget_get_style_context(label_gameover), "danger");

    btn_gameover_back = gtk_button_new_with_label("");
    g_signal_connect(btn_gameover_back, "clicked", G_CALLBACK(go_menu), NULL);

    gtk_box_pack_start(GTK_BOX(box), label_gameover, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_gameover_back, FALSE, FALSE, 0);

    return create_box_with_copyright(box);
}

GtkWidget* create_credits() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    lbl_credits_title = gtk_label_new("");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_credits_title), "title");

    lbl_credits_info = gtk_label_new("");
    gtk_style_context_add_class(gtk_widget_get_style_context(lbl_credits_info), "info");
    gtk_label_set_justify(GTK_LABEL(lbl_credits_info), GTK_JUSTIFY_CENTER);

    btn_credits_back = gtk_button_new_with_label("");
    g_signal_connect(btn_credits_back, "clicked", G_CALLBACK(go_menu), NULL);

    gtk_box_pack_start(GTK_BOX(box), lbl_credits_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), lbl_credits_info, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_credits_back, FALSE, FALSE, 0);

    return create_box_with_copyright(box);
}

/* ---------- MAIN ---------- */

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    load_css();

    
    lang_set(LANG_EN);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_title(GTK_WINDOW(window), "Number Guillotine " BUILD_VERSION);
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 700);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    stack = gtk_stack_new();
    gtk_container_add(GTK_CONTAINER(window), stack);

    
    gtk_stack_add_named(GTK_STACK(stack), create_menu(), "menu");
    gtk_stack_add_named(GTK_STACK(stack), create_settings(), "settings");
    gtk_stack_add_named(GTK_STACK(stack), create_difficulty(), "difficulty");
    gtk_stack_add_named(GTK_STACK(stack), create_game(), "game");
    gtk_stack_add_named(GTK_STACK(stack), create_gameover(), "gameover");
    gtk_stack_add_named(GTK_STACK(stack), create_credits(), "credits");

    
    update_interface_text();

    gtk_stack_set_visible_child_name(GTK_STACK(stack), "menu");

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
