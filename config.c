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
#include "config.h"
#include <glib.h>

#define CONFIG_GROUP "Settings"
#define CONFIG_APP_DIR "number-guillotine"
#define CONFIG_FILE_NAME "config.ini"


static gchar *config_get_path(void) {
    gchar *dir = g_build_filename(g_get_user_config_dir(), CONFIG_APP_DIR, NULL);
    g_mkdir_with_parents(dir, 0700);

    gchar *path = g_build_filename(dir, CONFIG_FILE_NAME, NULL);
    g_free(dir);
    return path;
}

void config_load(LanguageID *language, int *dark_mode, int *anim_enabled, double *volume) {
    gchar *path = config_get_path();
    GKeyFile *kf = g_key_file_new();

    if (g_key_file_load_from_file(kf, path, G_KEY_FILE_NONE, NULL)) {
        GError *err = NULL;

        gint lang = g_key_file_get_integer(kf, CONFIG_GROUP, "language", &err);
        if (!err && lang >= 0 && lang < LANG_MAX) *language = (LanguageID)lang;
        g_clear_error(&err);

        gboolean dark = g_key_file_get_boolean(kf, CONFIG_GROUP, "dark_mode", &err);
        if (!err) *dark_mode = dark;
        g_clear_error(&err);

        gboolean anim = g_key_file_get_boolean(kf, CONFIG_GROUP, "animations", &err);
        if (!err) *anim_enabled = anim;
        g_clear_error(&err);

        gdouble vol = g_key_file_get_double(kf, CONFIG_GROUP, "volume", &err);
        if (!err) *volume = vol;
        g_clear_error(&err);
    }


    g_key_file_free(kf);
    g_free(path);
}

void config_save(LanguageID language, int dark_mode, int anim_enabled, double volume) {
    gchar *path = config_get_path();
    GKeyFile *kf = g_key_file_new();

    g_key_file_set_integer(kf, CONFIG_GROUP, "language", language);
    g_key_file_set_boolean(kf, CONFIG_GROUP, "dark_mode", dark_mode);
    g_key_file_set_boolean(kf, CONFIG_GROUP, "animations", anim_enabled);
    g_key_file_set_double(kf, CONFIG_GROUP, "volume", volume);

    g_key_file_save_to_file(kf, path, NULL);

    g_key_file_free(kf);
    g_free(path);
}
