#pragma once

#include <Arduino.h>

#define SSD1325_SETCOLADDR 0x15  //!< Set column address
#define SSD1325_SETROWADDR 0x75  //!< Set row address
#define SSD1325_SETCONTRAST 0x81 //!< Set contrast current
#define SSD1325_SETCURRENT 0x84  //!< Set current range

#define SSD1325_SETREMAP 0xA0      //!< Set re-map
#define SSD1325_SETSTARTLINE 0xA1  //!< Set display start line
#define SSD1325_SETOFFSET 0xA2     //!< Set display offset
#define SSD1325_NORMALDISPLAY 0xA4 //!< Set display mode normal
#define SSD1325_DISPLAYALLON 0xA5  //!< Set entire display on
#define SSD1325_DISPLAYALLOFF 0xA6 //!< Set entire display off
#define SSD1325_INVERTDISPLAY 0xA7 //!< Invert display
#define SSD1325_SETMULTIPLEX 0xA8  //!< Set multiplex ratio
#define SSD1325_MASTERCONFIG 0xAD  //!< Set master configuration
#define SSD1325_DISPLAYOFF 0xAE    //!< Set display off
#define SSD1325_DISPLAYON 0xAF     //!< Set display on

#define SSD1325_SETPRECHARGECOMPENABLE                                         \
  0xB0                                   //!< Set pre-charge compensation enable
#define SSD1325_SETPHASELEN 0xB1         //!< Set phase length
#define SSD1325_SETROWPERIOD 0xB2        //!< Set row period
#define SSD1325_SETCLOCK 0xB3            //!< Set display clock divide ratio
#define SSD1325_SETPRECHARGECOMP 0xB4    //!< Set pre-charge
#define SSD1325_SETGRAYTABLE 0xB8        //!< Set gray scale table
#define SSD1325_SETPRECHARGEVOLTAGE 0xBC //!< Set precharge voltage
#define SSD1325_SETVCOMLEVEL 0xBE        //!< Set Vcomh Voltage
#define SSD1325_SETVSL 0xBF              //!< Set segment low voltage (VSL)

#define SSD1325_GFXACCEL 0x23 //!< Graphic acceleration command options
#define SSD1325_DRAWRECT 0x24 //!< Draw rectangle
#define SSD1325_COPY 0x25     //!< Copy

#define OLED_CMD_MULTIPLEX_RATIO   0x3f
#define OLED_CMD_START_LINE        0x00
#define OLED_CMD_OFFSET            0x00
#define OLED_CMD_REMAP             0x42
#define OLED_CMD_CURRENT_RANGE     0x86 // quarter current range
#define OLED_CMD_PRECHARGE_VOLTAGE 0x00
#define OLED_CMD_VCOMH_VOLTAGE     0x1e // 0x1f
#define OLED_CMD_VSL_VOLTRAGE      0x00 // 0x08
#define OLED_CMD_ROW_PERIOD        0x10
#define OLED_CMD_OSC_FREQ          0x01  // 0x27, 0x16, 0xca (old). new: 0xc2, 0x01, 0x34 (manually found)
#define OLED_CMD_PHASE_LEN         0x01 // 0x01

#define OLED_E          (1 << 7) // PB7
#define OLED_CS         (1 << 2) // PF2
#define OLED_DC         (1 << 3) // PF3
#define OLED_RW         (1 << 4) // PF4
#define OLED_RST        (1 << 5) // PF5
#define OLED_BOOST_CONV (1 << 2) // PC2


struct oled_bootloader_t {
    const uint8_t *buf;
    uint16_t size;
};

extern const oled_bootloader_t SSD1325_bootloader;

extern uint8_t oled_framebuffer[4096];

extern "C" {
    extern void oled_set_pixel_fast(uint8_t, uint8_t, uint8_t);
}

extern void oled_init_io();
extern void oled_reset();
extern void oled_enable_boost_conv(bool);
extern void oled_byte_out_fast(uint8_t) __attribute__((always_inline));
extern void oled_byte_out(uint8_t) __attribute__((always_inline));
extern void oled_set_dc(bool) __attribute__((always_inline));

extern void oled_comm_out_progmem(uint8_t*, uint16_t);

extern void oled_send_framebuffer();
extern void oled_set_contrast(int8_t);
extern void oled_enable_display(bool);

extern void oled_clear(uint8_t);
extern void oled_set_pixel(int16_t, int16_t, uint8_t);