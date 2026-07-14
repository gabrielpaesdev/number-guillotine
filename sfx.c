#include <stdio.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define PI_2 6.283185307179586

double master_volume = 1.0;

void sfx_set_volume(double vol) {
    master_volume = vol;
}

void play_tone(FILE *out, double freq, double duration) {
    int total_samples = (int)(SAMPLE_RATE * duration);
    double step = (freq * PI_2) / SAMPLE_RATE;

    for (int i = 0; i < total_samples; i++) {
        double wave = sin(i * step);
        double decay = 1.0 - ((double)i / total_samples);
        fputc((unsigned char)(wave * decay * (127.0 * master_volume) + 128), out);
    }
}

void play_sweep(FILE *out, double start_freq, double end_freq, double duration) {
    int total_samples = (int)(SAMPLE_RATE * duration);
    double phase = 0.0;

    for (int i = 0; i < total_samples; i++) {
        double progress = (double)i / total_samples;
        double freq = start_freq + (end_freq - start_freq) * progress;
        phase += (freq * PI_2) / SAMPLE_RATE;

        double wave = sin(phase);
        double decay = 1.0 - progress;
        fputc((unsigned char)(wave * decay * (127.0 * master_volume) + 128), out);
    }
}

void play_click() {
    if (master_volume <= 0.01) return;
    FILE *out = popen("aplay -q -c 1 -r 44100 -f U8 2>/dev/null", "w");
    if (out) {
        play_sweep(out, 1600.0, 200.0, 0.015);
        pclose(out);
    }
}

void play_victory() {
    if (master_volume <= 0.01) return;
    FILE *out = popen("aplay -q -c 1 -r 44100 -f U8 2>/dev/null", "w");
    if (out) {
        play_tone(out, 523.25, 0.08);
        play_tone(out, 659.25, 0.08);
        play_tone(out, 784.00, 0.08);
        play_tone(out, 1046.50, 0.40);
        pclose(out);
    }
}

void play_defeat() {
    if (master_volume <= 0.01) return;
    FILE *out = popen("aplay -q -c 1 -r 44100 -f U8 2>/dev/null", "w");
    if (out) {
        play_tone(out, 349.23, 0.17);
        play_tone(out, 329.63, 0.17);
        play_tone(out, 311.13, 0.17);
        play_tone(out, 293.66, 0.52);
        pclose(out);
    }
}
