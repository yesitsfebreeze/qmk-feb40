#pragma once // remaps any key to any other key in any mod state

#include QMK_KEYBOARD_H
#include "_util.h"

extern uint16_t remap_list[][16];
extern int remap_list_size;

#ifndef MOD_INDEX
  #define MOD_INDEX(code) code & 0x07
#endif

#define REMAP_MOD(remap, mod) [MOD_INDEX(mod)] = remap

#define REMAP(key, default, ...) \
  [key - CK(0)] = { \
    [0] = default, \
    __VA_ARGS__ \
  },
    
uint16_t remapped_kc;

uint16_t process_remap(uint16_t kc, keyrecord_t* rec);
