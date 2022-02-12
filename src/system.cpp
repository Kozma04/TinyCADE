#include "system.h"

uint8_t sys_last_status = SYS_CMD_WAITING;
uint8_t sys_rx_buf[16];
uint8_t sys_rx_len;
void (*sys_shutdown_callback)() = 0;

void sys_begin_usart() {
    delay(1000);
    Serial2.begin(4800);
    while(Serial2.available() == 0) {
        Serial2.write(1);
        Serial2.flush();
        delay(100);
    }
    while(Serial2.available()) Serial2.read();
    sys_set_master_led(false);
}

bool sys_wait_for_response(uint8_t size, uint16_t delay = SYS_CMD_TIMEOUT) {
    uint32_t time = millis();
    sys_rx_len = 0;
    while(Serial2.available()) Serial2.read();
    while(millis() - time < SYS_CMD_TIMEOUT) {
        if(Serial2.available()) {
            sys_rx_buf[sys_rx_len++] = Serial2.read();
        }
        if(sys_rx_len == size) {
            sys_last_status = sys_rx_buf[sys_rx_len - 1];
            return true;
        }
    }
    return false;
}

float sys_read_battery() {
    Serial2.write(0x0);
    Serial2.write(0x0);
    sys_wait_for_response(3);
    uint16_t raw = sys_rx_buf[0] | (sys_rx_buf[1] << 8);
    return 1125.3f / raw;
}

void sys_set_master_led(bool on) {
    Serial2.write(0x1);
    Serial2.write(on);
    Serial2.write(0x0);
    sys_wait_for_response(1);
}

uint32_t sys_read_rtc() {
    Serial2.write(0x2);
    Serial2.write(0x0);
    sys_wait_for_response(5);
    uint32_t timestamp = uint32_t(sys_rx_buf[0]) | (uint32_t(sys_rx_buf[1]) << 8) | (uint32_t(sys_rx_buf[2]) << 16) | (uint32_t(sys_rx_buf[3]) << 24);
    return timestamp;
}

void sys_req_shutdown() {
    Serial2.write(0x3);
    Serial2.write(0x0);
    sys_wait_for_response(1);
    if(sys_last_status == SYS_CMD_OK && sys_shutdown_callback)
        (*sys_shutdown_callback)();
}

uint32_t sys_read_uptime() {
    Serial2.write(0x4);
    Serial2.write(0x0);
    sys_wait_for_response(5);
    uint32_t uptime = uint32_t(sys_rx_buf[0]) | (uint32_t(sys_rx_buf[1]) << 8) | (uint32_t(sys_rx_buf[2]) << 16) | (uint32_t(sys_rx_buf[3]) << 24);
    return uptime;
}

void sys_write_rtc(uint32_t timestamp) {
    Serial2.write(0x5);
    Serial2.write((timestamp >> 24) & 0xff);
    Serial2.write((timestamp >> 16) & 0xff);
    Serial2.write((timestamp >> 8) & 0xff);
    Serial2.write(timestamp & 0xff);
    Serial2.write(0x0);
    sys_wait_for_response(1, 10000);
}

void sys_write_user_text(char *str) {
    Serial2.write(0x6);
    uint8_t i = 0;
    for(; i < 10 && *str != 0; i++)
        Serial2.write(*(str++));
    for(; i < 10; i++)
        Serial2.write(0x0);
    Serial2.write(0x0);
    sys_wait_for_response(1, 10000);
}

void sys_read_user_text(char *str, uint8_t &len) {
    Serial2.write(0x7);
    Serial2.write(0x0);
    sys_wait_for_response(11);
    uint8_t i;
    len = 10;
    for(i = 0; i < 10; i++) {
        *(str++) = sys_rx_buf[i];
        if(sys_rx_buf[i] == 0) len = i;
    }
    if(i == 10) *str = 0;
}