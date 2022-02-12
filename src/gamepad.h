#pragma once

#include <avr/io.h>
#include <stdint.h>

enum gamepad_button_t : uint8_t {
    BTN_B = 0,
    BTN_A,
    BTN_MENU,
    BTN_START,
    BTN_RIGHT,
    BTN_LEFT,
    BTN_DOWN,
    BTN_UP
};

extern uint8_t gamepad_buffer;
extern uint8_t gamepad_last_buffer;

extern void gamepad_init();
extern void gamepad_poll();
extern bool gamepad_just_pressed(gamepad_button_t button);
extern bool gamepad_pressed(gamepad_button_t button);
extern bool gamepad_released(gamepad_button_t button);