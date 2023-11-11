#pragma once
#include QMK_KEYBOARD_H

#define CK(code) (QK_USER | ((code) & 0xFF))

uint8_t get_full_mods(void);

void remove_mod_mask(uint8_t mask);

bool is_shifted_kc(uint16_t kc);