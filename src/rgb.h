#pragma once
#include "src/core.h"
#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE

#define RGB_BASE 3, 255, 74
#define RGB_LOWER 136, 0, 255
#define RGB_RAISE 5, 59, 255
#define RGB_COMBO 255, 111, 0

#define RGB_LAYER_INDICATOR_KEY 24

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