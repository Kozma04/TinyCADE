#include "tinycade.h"

void tinycade_init(uint8_t param) {
    if(param & INIT_OLED) {
        oled_init_io();
        oled_reset();
        oled_comm_out_progmem(SSD1325_bootloader.buf, SSD1325_bootloader.size);
        oled_enable_boost_conv(true);
        oled_enable_display(true);
        oled_set_contrast(127);
    }
    if(param & INIT_LED) {
        led_init();
    }
    if(param & INIT_SYSTEM) {
        sys_begin_usart();
    }
    if(param & INIT_GAMEPAD) {
        gamepad_init();
    }
    if(param & INIT_AUDIO) {
        audio_begin();
        audio_reset();
    }
}