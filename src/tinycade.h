#pragma once

#define INIT_LED     (1 << 0)
#define INIT_OLED    (1 << 1)
#define INIT_GAMEPAD (1 << 2)
#define INIT_SYSTEM  (1 << 3)
#define INIT_AUDIO   (1 << 4)

#define INIT_ALL 0xff
#define INIT_SAFE (INIT_LED | INIT_OLED | INIT_GAMEPAD)


#include "system.h"
#include "audio.h"

#include "led.h"
#include "gamepad.h"
#include "oled.h"

#include "sprite.h"
#include "font.h"
#include "tilemap.h"
#include "mode7.h"

#include "font4x5.h"


extern void tinycade_init(uint8_t param);