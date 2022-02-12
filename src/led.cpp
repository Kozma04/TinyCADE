#include "led.h"

void led_init() {
    rgb_led_set(0, 0, 0);
    dbg_led_set(false);
    pinMode(PIN_PB4, OUTPUT);
    pinMode(PIN_PB5, OUTPUT);
    pinMode(PIN_PC5, OUTPUT);
    pinMode(PIN_PA7, OUTPUT);
}

void rgb_led_set_r(uint8_t r) {
    analogWrite(PIN_PB4, 255 - r);
}

void rgb_led_set_g(uint8_t g) {
    analogWrite(PIN_PB5, 255 - g);
}

void rgb_led_set_b(uint8_t b) {
    analogWrite(PIN_PC5, 255 - b);
}

void rgb_led_set(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(PIN_PB4, 255 - r);
    analogWrite(PIN_PB5, 255 - g);
    analogWrite(PIN_PC5, 255 - b);
}

void dbg_led_set(bool on) {
    digitalWrite(PIN_PA7, on);
}