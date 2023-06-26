// goes to lower level if window switch combo is pressed

#pragma once
#include QMK_KEYBOARD_H

extern bool is_in_window_switch;

void window_switch(uint16_t keycode, keyrecord_t* record);