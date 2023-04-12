#pragma once
#include QMK_KEYBOARD_H

uint8_t _mods;

void _get_mods(void); 
void _remove_shift_mod(void);
bool _is_shifted_keycode(uint16_t keycode);
void _temp_unshift(uint16_t keycode);