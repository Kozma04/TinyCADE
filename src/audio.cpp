#include "audio.h"

void audio_begin() {
    PORTC.DIRSET = 1 << 6; // audio reset pin
    Serial1.begin(AUDIO_BAUD);
}

void audio_reset() {
    Serial1.flush();
    PORTC.OUTSET = 1 << 6;
    while(!Serial1.available());
    PORTC.OUTCLR = 1 << 6;
    Serial1.read();
}

void audio_set_dac_vref(audio_dac_vref_t vref) {
    Serial1.write(0xf0);
    Serial1.write(vref);
}

void audio_mute(bool mute) {
    Serial1.write(0xf1);
    Serial1.write(mute);
}

void audio_note_on(uint8_t channel, uint16_t freq) {
    Serial1.write(0x10 | channel);
    Serial1.write(freq & 0xff);
    Serial1.write(freq >> 8);
}

void audio_note_off(uint8_t channel) {
    Serial1.write(0x20 | channel);
}

void audio_set_vol(uint8_t channel, uint8_t volume) {
    Serial1.write(0x40 | channel);
    Serial1.write(volume);
}

void audio_assign_instr(uint8_t channel, uint8_t instr_id) {
    Serial1.write(0x30 | channel);
    Serial1.write(instr_id);
}

void audio_modify_instr(uint8_t instr_id, instrument_t instr) {
    Serial1.flush();
    Serial1.write(0x50);
    Serial1.write(instr_id);
    Serial1.write(instr.wave);
    Serial1.write(instr.duty);
    Serial1.write(instr.vibrato.amplitude & 0xff);
    Serial1.write(instr.vibrato.amplitude >> 8);
    Serial1.write(instr.vibrato.speed);
    Serial1.write(instr.arpeggio.multiply & 0xff);
    Serial1.write(instr.arpeggio.multiply >> 8);
    Serial1.write(instr.arpeggio.trigger);
    Serial1.write(instr.slide.delta & 0xff);
    Serial1.write(instr.slide.delta >> 8);
    Serial1.write(instr.slide.retrigger & 0xff);
    Serial1.write(instr.slide.retrigger >> 8);
    Serial1.write(instr.sweep & 0xff);
    Serial1.write(instr.sweep >> 8);
    Serial1.write(instr.adsr.a);
    Serial1.write(instr.adsr.d);
    Serial1.write(instr.adsr.s);
    Serial1.write(instr.adsr.r);
    Serial1.flush();
}