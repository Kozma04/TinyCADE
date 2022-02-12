#include "gamepad.h"

uint8_t gamepad_buffer = 0;
uint8_t gamepad_last_buffer = 0;

void gamepad_init() {
    PORTG.DIR = 0;
    PORTG.PINCONFIG = (1 << PORT_PULLUPEN_bp) | (1 << PORT_INVEN_bp);
    PORTG.PINCTRLUPD = 0xff;
}

void gamepad_poll() {
    gamepad_last_buffer = gamepad_buffer;
    gamepad_buffer = PORTG.IN;
}

bool gamepad_just_pressed(gamepad_button_t button) {
    uint8_t mask = 1 << button;
    return ((gamepad_buffer & mask) != 0) && ((gamepad_last_buffer & mask) == 0);
}

bool gamepad_pressed(gamepad_button_t button) {
    uint8_t mask = 1 << button;
    return (gamepad_buffer & mask) != 0;
}

bool gamepad_released(gamepad_button_t button) {
    uint8_t mask = 1 << button;
    return ((gamepad_buffer & mask) == 0) && ((gamepad_last_buffer & mask) != 0);
}