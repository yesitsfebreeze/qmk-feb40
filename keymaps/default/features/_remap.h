// remaps any key to any other key in any mod state

#pragma once
#include QMK_KEYBOARD_H

extern uint16_t remap_key_list[][16];

#define RM(code) (QK_USER | ((code)&0xFF))
#define REMAP(remap) [0] = remap
#define REMAP_MOD(remap, mod) [MOD_INDEX(mod)] = remap
 
uint16_t remapped_keycode;

uint16_t process_remap(uint16_t kc, keyrecord_t* rec);