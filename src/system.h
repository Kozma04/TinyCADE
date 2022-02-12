#pragma once

#include <Arduino.h>
#include <stdint.h>

#define SYS_CMD_TIMEOUT 1100

#define SYS_CMD_OK      0x0
#define SYS_CMD_FAIL    0x1
#define SYS_CMD_UNKNOWN 0x2
#define SYS_CMD_WAITING 0xff

extern uint8_t sys_last_status;
extern uint8_t sys_rx_buf[16];
extern uint8_t sys_rx_len;
extern void (*sys_shutdown_callback)();

extern void sys_begin_usart();

extern bool sys_wait_for_response(uint8_t size, uint16_t delay);

extern float sys_read_battery();
extern void sys_set_master_led(bool on);
extern uint32_t sys_read_rtc();
extern void sys_req_shutdown();
extern uint32_t sys_read_uptime();
extern void sys_write_rtc(uint32_t timestamp);
extern void sys_write_user_text(char *str);
extern void sys_read_user_text(char *str, uint8_t &len);