// implements key repeat on hold
// adds up to _4 keys per tapping

#pragma once
#include QMK_KEYBOARD_H

#define TD(kc) kc + SAFE_RANGE

#define TAP_DANCE_1(_kc, _def, _1) { .keycode = TD(_kc), .max_taps = 1, .keycodes = { _def, _1, KC_NO, KC_NO, KC_NO} } 
#define TAP_DANCE_2(_kc, _def, _1, _2) { .keycode = TD(_kc), .max_taps = 2, .keycodes = { _def, _1, _2, KC_NO, KC_NO } } 
#define TAP_DANCE_3(_kc, _def, _1, _2, _3) { .keycode = TD(_kc), .max_taps = 3, .keycodes = { _def, _1, _2, _3, KC_NO} } 
#define TAP_DANCE_4(_kc, _def, _1, _2, _3, _4) { .keycode = TD(_kc), .max_taps = 4, .keycodes = { _def, _1, _2, _3, _4} } 
#define TAP_DANCE_5(_kc, _def, _1, _2, _3, _4, _5) { .keycode = TD(_kc), .max_taps = 5, .keycodes = { _def, _1, _2, _3, _4, _5} } 

#define TAP_HOLD(_kc, _def, _1) { .keycode = TD(_kc), .max_taps = 1, .keycodes = { _def, _1, KC_NO, KC_NO, KC_NO, KC_NO}, .hold = true } 


typedef struct {
    uint16_t keycode;
    int max_taps;
    uint16_t keycodes[6];
    bool hold;
} _tap_dance;

extern _tap_dance _tap_dances[];
extern int _tap_dances_size; 

uint16_t _tap_dance_last_keycode;

bool _tap_dance_available;
bool _tap_dance_released;
uint16_t _tap_dance_active_keycode;
int _tap_dance_taps;
uint16_t _tap_dance_time;
int _tap_dance_max_taps;
uint16_t _tap_dance_keycodes[6];

void _tap_dance_assign(_tap_dance *td);
void _tap_dance_reset(void);
void _tap_dance_execute(void);
void _tap_dance_handle(_tap_dance *td);
bool _process_tap_dance(uint16_t keycode, keyrecord_t *record);
void _process_tap_dance_execute(void);