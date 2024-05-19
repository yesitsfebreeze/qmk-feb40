#pragma once
#include "src/core.h"
#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE

#define RGB_BASE 136, 0, 255
#define RGB_LOWER 3, 255, 74
#define RGB_RAISE 5, 59, 255
#define RGB_COMBO 255, 111, 0
#define RGB_GAME 255, 59,  5

#define RGB_LAYER_INDICATOR_KEY 18
#define RGB_LAYER_INDICATOR_W 3
#define RGB_LAYER_INDICATOR_A 14
#define RGB_LAYER_INDICATOR_S 15
#define RGB_LAYER_INDICATOR_D 16

extern bool RGB_INDICATOR;
extern bool RGB_MODE;
extern uint8_t INDICATOR_R;
extern uint8_t INDICATOR_G;
extern uint8_t INDICATOR_B;

#endif

#define CK_RGB  CK_INTERN_1
#define CK_RGBI CK_INTERN_2

void init_rgb(void);
bool handle_rgb(uint16_t kc, keyrecord_t *rec);