#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>

void sfx_set_volume(double vol);
void play_tone(FILE *out, double freq, double duration);
void play_sweep(FILE *out, double start_freq, double end_freq, double duration);
void play_click(void);
void play_victory(void);
void play_defeat(void);

#endif
