#pragma once

#define AUDIO_BAUD 115200

#define WAVE_SQUARE_bp   0
#define WAVE_SAWTOOTH_bp 1
#define WAVE_TRIANGLE_bp 2
#define WAVE_NOISE_bp    3
#define WAVE_PCM_A_bp    4
#define WAVE_PCM_B_bp    5

#define WAVE_SQUARE_gc   (1 << WAVE_SQUARE_bp)
#define WAVE_SAWTOOTH_gc (1 << WAVE_SAWTOOTH_bp)
#define WAVE_TRIANGLE_gc (1 << WAVE_TRIANGLE_bp)
#define WAVE_NOISE_gc    (1 << WAVE_NOISE_bp)
#define WAVE_PCM_A_gc    (1 << WAVE_PCM_A_bp)
#define WAVE_PCM_B_gc    (1 << WAVE_PCM_B_bp)

#include <Arduino.h>


struct vibrato_t {
    uint8_t speed;
    uint16_t amplitude;
};

struct arpeggio_t {
    uint16_t multiply;
    uint8_t trigger;
};

struct slide_t {
    int16_t delta;
    uint16_t retrigger;
};

struct adsr_params_t {
    uint8_t a;
    uint8_t d;
    uint8_t s;
    uint8_t r;
};

struct instrument_t {
    uint8_t wave;
    uint8_t duty;

    vibrato_t vibrato;
    arpeggio_t arpeggio;
    slide_t slide;
    int16_t sweep;
    adsr_params_t adsr;
};

enum audio_dac_vref_t : uint8_t {
    DACREF_0V55 = 0,
    DACREF_1V5,
    DACREF_2V5,
    DACREF_4V34
};


extern void audio_begin();
extern void audio_reset();

extern void audio_set_dac_vref(audio_dac_vref_t vref);
extern void audio_mute(bool mute);
extern void audio_note_on(uint8_t channel, uint16_t freq);
extern void audio_note_off(uint8_t channel);
extern void audio_set_vol(uint8_t channel, uint8_t volume);
extern void audio_assign_instr(uint8_t channel, uint8_t instr_id);
extern void audio_modify_instr(uint8_t instr_id, instrument_t instr);
