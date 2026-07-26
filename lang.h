#ifndef LANG_H
#define LANG_H

typedef enum {
    LANG_EN = 0,
    LANG_PT,
    LANG_FR,
    LANG_ES,
    LANG_MAX
} LanguageID;

typedef enum {
    STR_BTN_PLAY = 0,
    STR_BTN_SETTINGS,
    STR_BTN_CREDITS,
    STR_BTN_EXIT,
    STR_DIFF_TITLE,
    STR_DIFF_EASY,
    STR_DIFF_MED,
    STR_DIFF_HARD,
    STR_BTN_BACK,
    STR_GAME_INSTRUCT,
    STR_GAME_TRIES,
    STR_GAME_BIGGER,
    STR_GAME_SMALLER,
    STR_GAME_WIN,
    STR_GAME_LOSE,
    STR_CREDITS_TITLE,
    STR_CREDITS_BODY,
    STR_SETTINGS_TITLE,
    STR_SETTINGS_LANG_LABEL,
    STR_SETTINGS_THEME,
    STR_SETTINGS_ANIM,
    STR_SETTINGS_VOL,
    STR_BTN_PLAY_AGAIN,
    STR_GAME_SCORE,
    STR_COUNT
} StringID;

void lang_set(LanguageID id);
LanguageID lang_get_current(void);
const char* lang_get(StringID id);
const char* lang_get_title(void);
const char* lang_get_copyright(void);
void lang_init_from_system(void);

#endif
