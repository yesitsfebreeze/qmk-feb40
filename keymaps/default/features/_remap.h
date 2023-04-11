#pragma once
#include QMK_KEYBOARD_H

typedef struct {
    uint16_t keycode;
    uint16_t new_keycode;
    uint8_t mod;
} _remap_key_t;

extern _remap_key_t _key_remap_list[];
extern int _key_remap_list_size;

uint16_t _remap_original_keycode;
uint16_t _remap_current_keycode;
uint8_t _remap_mods;

#define REMAP(_keycode, _new_keycode) { .keycode = _keycode, .new_keycode = _new_keycode }
#define REMAP_MOD(_keycode, _new_keycode, _mod) { .keycode = _keycode, .new_keycode = _new_keycode, .mod = _mod }

uint16_t _remap_resolve_key(uint16_t keycode);
uint16_t _remap_execute(uint16_t keycode);
bool _process_remap(uint16_t keycode, keyrecord_t* record);
bool _process_remap_release(uint16_t keycode, keyrecord_t* record);