#pragma once
#include QMK_KEYBOARD_H

#define MC_AND KC_F20 
#define MC_OR KC_F21
#define MC_PTR KC_F22
 
bool process_macros(uint16_t kc, keyrecord_t* rec);