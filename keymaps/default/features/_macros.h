#pragma once
#include QMK_KEYBOARD_H

typedef struct {
  uint16_t key;
  char *value;
} MacroEntry;

extern MacroEntry macro_list[];
extern int macro_list_size;

bool process_macros(uint16_t kc, keyrecord_t* rec);