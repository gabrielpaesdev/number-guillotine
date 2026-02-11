#include "lang.h"
#include <stdio.h>

static LanguageID current_lang = LANG_EN; // Default: Inglês

// Tabela de Strings: [ID DA STRING][ID DO IDIOMA]
static const char *strings[STR_COUNT][LANG_MAX] = {
    // STR_TITLE
    {"Number Guillotine", "Number Guillotine"},
    // STR_BTN_PLAY
    {"Play", "Jogar"},
    // STR_BTN_SETTINGS
    {"Settings", "Configurações"},
    // STR_BTN_CREDITS
    {"Credits", "Créditos"},
    // STR_BTN_EXIT
    {"Exit", "Sair"},
    
    // STR_DIFF_TITLE
    {"Choose Difficulty", "Escolha a Dificuldade"},
    // STR_DIFF_EASY
    {"Easy (8 tries)", "Fácil (8 tentativas)"},
    // STR_DIFF_MED
    {"Medium (6 tries)", "Médio (6 tentativas)"},
    // STR_DIFF_HARD
    {"Hard (4 tries)", "Difícil (4 tentativas)"},
    // STR_BTN_BACK
    {"Back", "Voltar"},
    
    // STR_GAME_INSTRUCT
    {"Choose a number", "Escolha um número"},
    // STR_GAME_TRIES
    {"Tries left: %d", "Tentativas restantes: %d"},
    // STR_GAME_BIGGER
    {"The number is GREATER", "O número é MAIOR"},
    // STR_GAME_SMALLER
    {"The number is SMALLER", "O número é MENOR"},
    // STR_GAME_WIN
    {"CONGRATS!\nYou Won!", "PARABÉNS!\nVocê venceu!"},
    // STR_GAME_LOSE
    {"GAME OVER\nThe number was: %d", "GAME OVER\nO número certo era: %d"},
    
    // STR_CREDITS_TITLE
    {"Credits", "Créditos"},
    // STR_CREDITS_BODY
    {
        "Developed by:\nGabriel Paes\n\nContact:\ngabriel.paesbarreto@ufrpe.br\n\n\nBuild: %s",
        "Desenvolvido por:\nGabriel Paes\n\nContato:\ngabriel.paesbarreto@ufrpe.br\n\n\nBuild: %s"
    },
    
    // STR_SETTINGS_TITLE
    {"Settings", "Configurações"},
    // STR_SETTINGS_LANG_LABEL
    {"Language / Idioma", "Language / Idioma"},
    
    // STR_SETTINGS_THEME
    {"Dark Mode", "Modo Escuro"},

    // STR_SETTINGS_THEME
    {"Animations", "Animações"},
    
    // STR_COPYRIGHT
    {"© Gabriel Paes 2026", "© Gabriel Paes 2026"}
};

void lang_set(LanguageID id) {
    if (id >= 0 && id < LANG_MAX) {
        current_lang = id;
    }
}

LanguageID lang_get_current() {
    return current_lang;
}

const char* lang_get(StringID id) {
    if (id >= 0 && id < STR_COUNT) {
        return strings[id][current_lang];
    }
    return "???";
}
