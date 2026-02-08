#ifndef LANG_H
#define LANG_H

// IDs para todas as strings do jogo
typedef enum {
    STR_TITLE,
    STR_BTN_PLAY,
    STR_BTN_SETTINGS,
    STR_BTN_CREDITS,
    STR_BTN_EXIT,
    
    STR_DIFF_TITLE,
    STR_DIFF_EASY,
    STR_DIFF_MED,
    STR_DIFF_HARD,
    STR_BTN_BACK,
    
    STR_GAME_INSTRUCT,
    STR_GAME_TRIES,     // Format string com %d
    STR_GAME_BIGGER,
    STR_GAME_SMALLER,
    STR_GAME_WIN,
    STR_GAME_LOSE,      // Format string com %d
    
    STR_CREDITS_TITLE,
    STR_CREDITS_BODY,   // String longa com format %s para a versão
    
    STR_SETTINGS_TITLE,
    STR_SETTINGS_LANG_LABEL,
    STR_COPYRIGHT,
    
    STR_COUNT // Sempre o último
} StringID;

// IDs dos Idiomas
typedef enum {
    LANG_EN,
    LANG_PT,
    LANG_MAX
} LanguageID;

void lang_set(LanguageID id);
LanguageID lang_get_current();
const char* lang_get(StringID id);

#endif
