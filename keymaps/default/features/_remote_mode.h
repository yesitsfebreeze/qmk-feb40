#pragma once
#include QMK_KEYBOARD_H

extern bool REMOTE_ENABLED;

void process_remote_mode(uint16_t kc, keyrecord_t* rec);
uint16_t process_remote_press(uint16_t kc, keyrecord_t* rec);