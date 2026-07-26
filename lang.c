/*
 * Program: Number Guillotine
 * Description: A number-guessing puzzle game.
 *
 * Copyright (C) 2026 Gabriel Paes
 * Contact: <gabrielpaesdev@proton.me>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See <https://www.gnu.org/licenses/> for details.
 */
#include "lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static LanguageID current_lang = LANG_EN;

static const char *strings[STR_COUNT][LANG_MAX] = {
    {"Play", "Jogar", "Jouer", "Jugar"},
    {"Settings", "Configurações", "Paramètres", "Ajustes"},
    {"Credits", "Créditos", "Crédits", "Créditos"},
    {"Exit", "Sair", "Quitter", "Salir"},
    {"Choose Difficulty", "Escolha a Dificuldade", "Choisir a Difficulté", "Elegir Dificultad"},
    {"Easy (8 tries)", "Fácil (8 tentativas)", "Facile (8 tentatives)", "Fácil (8 intentos)"},
    {"Medium (6 tries)", "Médio (6 tentativas)", "Moyen (6 tentatives)", "Medio (6 intentos)"},
    {"Hard (4 tries)", "Difícil (4 tentativas)", "Difficile (4 tentatives)", "Difícil (4 intentos)"},
    {"Back", "Voltar", "Retour", "Volver"},
    {"Choose a number", "Escolha um número", "Choisissez un nombre", "Elige un número"},
    {"Tries left: %d", "Tentativas restantes: %d", "Tentatives restantes: %d", "Intentos restantes: %d"},
    {"The number is GREATER", "O número é MAIOR", "Le nombre est PLUS GRAND", "El número es MAYOR"},
    {"The number is SMALLER", "O número é MENOR", "Le nombre est PLUS PETIT", "El número es MENOR"},
    {"CONGRATS!\nYou Won!", "PARABÉNS!\nVocê venceu!", "FÉLICITATIONS!\nVous avez gagné!", "¡FELICIDADES!\n¡Ganaste!"},
    {"GAME OVER\nThe number was: %d", "GAME OVER\nO número certo era: %d", "GAME OVER\nLe nombre était: %d", "GAME OVER\nEl número era: %d"},
    {"Credits", "Créditos", "Crédits", "Créditos"},
    {
        "Developed by:\nGabriel Paes\n\nContact:\ngabrielpaesdev@proton.me\n\n\nBuild: %s",
        "Desenvolvido por:\nGabriel Paes\n\nContato:\ngabrielpaesdev@proton.me\n\n\nBuild: %s",
        "Développé por:\nGabriel Paes\n\nContact:\ngabrielpaesdev@proton.me\n\n\nBuild: %s",
        "Desarrollado por:\nGabriel Paes\n\nContacto:\ngabrielpaesdev@proton.me\n\n\nBuild: %s"
    },
    {"Settings", "Configurações", "Paramètres", "Ajustes"},
    {"Language", "Idioma", "Langue", "Idioma"},
    {"Dark Mode", "Modo Escuro", "Mode Sombre", "Modo Oscuro"},
    {"Animations", "Animações", "Animations", "Animaciones"},
    {"Volume", "Volume", "Volume", "Volumen"},
    {"Play Again", "Jogar Novamente", "Rejouer", "Jugar de Nuevo"},
    {"Score", "Pontuação", "Score", "Puntuación"}
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

const char* lang_get_title(void) {
    return "Number Guillotine";
}

const char* lang_get_copyright(void) {
    return "© Gabriel Paes 2026";
}

void lang_init_from_system(void) {
    const char *lang_env = getenv("LANG");
    if (lang_env != NULL) {
        if (strncmp(lang_env, "pt_BR", 5) == 0 || strncmp(lang_env, "pt", 2) == 0) {
            current_lang = LANG_PT;
        } else if (strncmp(lang_env, "fr", 2) == 0) {
            current_lang = LANG_FR;
        } else if (strncmp(lang_env, "es", 2) == 0) {
            current_lang = LANG_ES;
        } else {
            current_lang = LANG_EN;
        }
    } else {
        current_lang = LANG_EN;
    }
}
