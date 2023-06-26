// handles rgb functionality

#pragma once
#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE

#define RGB_BASE 128, 0, 255
#define RGB_LOWER 128, 255, 64
#define RGB_RAISE 255, 64, 0
#define RGB_COMBO 255, 255, 0

#define RGB_LAYER_INDICATOR_KEY 24

#define KC_RGB_SWITCH RGB_TOG

extern int RGB_MODE;
extern int RGB_MODE_MAX;
extern uint8_t INDICATOR_R;
extern uint8_t INDICATOR_G;
extern uint8_t INDICATOR_B;

void init_rgb(void);
bool handle_rgb_mode(uint16_t keycode, keyrecord_t* record);

#endif