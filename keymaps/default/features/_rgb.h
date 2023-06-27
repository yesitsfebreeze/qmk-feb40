// handles rgb functionality

#pragma once
#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE

#define RGB_BASE 3, 255, 74
#define RGB_LOWER 136, 0, 255
#define RGB_RAISE 255, 111, 0
#define RGB_COMBO 10, 247, 247

#define RGB_MODE_0 RGB_MATRIX_NONE
#define RGB_MODE_1 RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define RGB_MODE_2 RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#define RGB_MODE_3 RGB_MATRIX_SOLID_COLOR

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