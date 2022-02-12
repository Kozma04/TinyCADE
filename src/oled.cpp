#include "oled.h"
/*
const uint8_t SSD1325_bootloader_buf[] PROGMEM = {
    0xad, 0x02,                         // disable DC-DC converter
    0xae,                               // set display off
    0xa8, OLED_CMD_MULTIPLEX_RATIO,   // set multiplex ratio
    0xa1, OLED_CMD_START_LINE,        // set display start line
    0xa2, OLED_CMD_OFFSET,            // set display offset
    0xa0, OLED_CMD_REMAP,             // set re-map
    OLED_CMD_CURRENT_RANGE,           // current range
    SSD1325_SETGRAYTABLE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xbc, OLED_CMD_PRECHARGE_VOLTAGE, // set pre-charge voltage
    0xbe, OLED_CMD_VCOMH_VOLTAGE,     // set VCOMH voltage
    0x02, OLED_CMD_VSL_VOLTRAGE,      // set Segment Low Voltage (VSL)
    0xb1, OLED_CMD_PHASE_LEN,         // pre-charge/discharge
    0xb2, OLED_CMD_ROW_PERIOD,        // set row period
    0xb3, OLED_CMD_OSC_FREQ,          // set display clock divide ratio/oscillator frequency
};
*/

const uint8_t SSD1325_bootloader_buf[] PROGMEM = {0xad,
    0x02,
    0xae,
    0xa8,
    0x3f,
    0xa1,
    0x00,
    0xa2,
    0x00,
    0xa0,
    0x42, // 0x41
    0x86, // current range. 0x85 was prev.
    0x81,
    0x37,
    0xb1,
    0x11,
    0xb2,
    0x22,
    0xb3, // osc freq
    0xf3, // 0xc3 for video rec. (0xd5 most recent), 0xe5, 0xf3
    0xbc,
    0x00,
    0xbe, // set vcomH
    0x1d, // 0x1d, 0x11, 0x10
    0xbf, // set vsl
    0x02, // prev: 0x02

    0xb8,
    0x01,
    0x00,
    0x11,
    0x11,
    0x21,
    0x22,
    0x22,
    0x32
/*
    0xb8,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00*/
};

const oled_bootloader_t SSD1325_bootloader = {
    SSD1325_bootloader_buf,
    sizeof(SSD1325_bootloader_buf)
};

uint8_t oled_framebuffer[4096];

void oled_init_io() {
    PORTF.DIRSET = OLED_CS | OLED_DC | OLED_RW;
    PORTF.OUTCLR = OLED_RW | OLED_DC;
    PORTB.DIRSET = OLED_E;
    PORTE.DIRSET = 0xFF; // data
}

void oled_reset() {
    oled_enable_boost_conv(false);
    PORTF.OUTSET = OLED_CS;
    PORTF.DIRSET = OLED_RST;
    delay(150);
    PORTF.OUTCLR = OLED_RST;
    delay(150);
    PORTF.OUTCLR = OLED_CS;
    PORTF.OUTSET = OLED_RST;
    PORTF.OUTCLR = OLED_DC;
    oled_clear(0);
    oled_send_framebuffer();
}

void oled_enable_boost_conv(bool enable) {
    PORTC.DIRSET = OLED_BOOST_CONV;
    if(enable) PORTC.OUTSET = OLED_BOOST_CONV;
    else PORTC.OUTCLR = OLED_BOOST_CONV;
    delay(150);
}

void oled_byte_out_fast(uint8_t val) {
    PORTE.OUT = val;
    PORTB.OUTSET = OLED_E;
    PORTB.OUTCLR = OLED_E;
}

void oled_byte_out(uint8_t val) {
    PORTF.OUTCLR = OLED_CS;
    oled_byte_out_fast(val);
    PORTF.OUTSET = OLED_CS;
}

void oled_set_dc(bool data) {
    if(data) PORTF.OUTSET = OLED_DC;
    else PORTF.OUTCLR = OLED_DC;
}

void oled_comm_out_progmem(uint8_t *start, uint16_t size) {
    oled_set_dc(false);
    for(uint16_t i = 0; i < size; i++)
        oled_byte_out(pgm_read_byte_far(start++));
}

void oled_send_framebuffer() {
    oled_set_dc(false);
    oled_byte_out(0x15);
    oled_byte_out(0x00);
    oled_byte_out(0x3f); // col 0-127
    oled_byte_out(0x75);
    oled_byte_out(0x00);
    oled_byte_out(0x4f); // row 0-63
    oled_set_dc(true);
    PORTF.OUTCLR = OLED_CS;

    uint8_t *pStart = oled_framebuffer;
    uint8_t *pEnd = oled_framebuffer + sizeof(oled_framebuffer);
    while(pStart < pEnd) {
        PORTE.OUT = *(pStart++);
        PORTB.OUTSET = OLED_E;
        PORTB.OUTCLR = OLED_E;
    }
    PORTF.OUTSET = OLED_CS;
    oled_set_dc(true);
}

void oled_set_contrast(int8_t contrast) {
    if(contrast < 0) contrast = 0;
    oled_set_dc(false);
    oled_byte_out(0x81);     //set contrast
    oled_byte_out(contrast); //second byte 0x7f = max
}

void oled_enable_display(bool enable) {
    oled_set_dc(false);
    oled_byte_out(enable ? 0xaf : 0xae);
}

void oled_clear(uint8_t val) {
    val = (val & 0xf) | ((val & 0xf) << 4);
    for(uint8_t *p = oled_framebuffer; p < oled_framebuffer + sizeof(oled_framebuffer); p++)
        *p = val;
}
/*
void oled_set_pixel_fast(uint8_t x, uint8_t y, uint8_t val) {
    // r24 = x, r22 = y, r20 = val
    asm (
        "push r0 \n\t"

        "ldi r25, 128 \n\t"
        "mul r22, r25 \n\t"
        "add r0, r24 \n\t"
        "lsr r1 \n\t"
        "ror r0 \n\t"
        "mov XL, r0 \n\t"
        "mov XH, r1 \n\t"
        "subi XL, lo8(-(oled_framebuffer)) \n\t"
        "sbci XH, hi8(-(oled_framebuffer)) \n\t"
        "ld r25, X \n\t"
        "andi r24, 1 \n\t"
        "sbrs r24, 0 \n\t"
        "rjmp .+14 \n\t"
        // draw_pixel_unsafe_l
        "andi r25, 0xf0 \n\t"
        "or r25, r20 \n\t"
        "st X, r25 \n\t"
        "clr r1 \n\t"
        "pop r0 \n\t"
        "ret \n\t"
        // draw_pixel_unsafe_h
        "swap r20 \n\t"
        "andi r25, 0x0f \n\t"
        "or r25, r20 \n\t"
        "st X, r25 \n\t"
        "clr r1 \n\t"
        "pop r0 \n\t"
        "ret \n\t"
    );
}*/