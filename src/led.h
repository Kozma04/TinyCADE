#pragma once

#include <Arduino.h>

extern void led_init();

extern void rgb_led_set_r(uint8_t r);
extern void rgb_led_set_g(uint8_t g);
extern void rgb_led_set_b(uint8_t b);
extern void rgb_led_set(uint8_t r, uint8_t g, uint8_t b);

extern void dbg_led_set(bool on);