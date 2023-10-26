#pragma once
#include QMK_KEYBOARD_H

#define OS(code) (QK_USER | ((code)&0xFF))

#define KC_REMOTE_SWITCH RGB_MODE_SNAKE

#define KC_OS_META KC_F18
#define KC_OS_ALT KC_F19

extern bool REMOTE_ENABLED;

void process_remote_mode(uint16_t kc, keyrecord_t* rec);
uint16_t process_remote_press(uint16_t kc, keyrecord_t* rec);