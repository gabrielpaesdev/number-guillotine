#ifndef CONFIG_H
#define CONFIG_H

#include "lang.h"

void config_load(LanguageID *language, int *dark_mode, int *anim_enabled, double *volume);
void config_save(LanguageID language, int dark_mode, int anim_enabled, double volume);

#endif
