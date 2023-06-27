#include QMK_KEYBOARD_H
#include "_layers.h"
#include "features/_remap.h"
#include "features/_rgb.h"
#include "features/_window_switch.h"

#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

void keyboard_post_init_user(void) {
    init_rgb();
    #ifdef CONSOLE_ENABLE
        debug_enable=true;
        // debug_keyboard=true;
    #endif
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (handle_rgb_mode(keycode, record)) return false;

    bool has_remap = false;
    uint16_t remapped_keycode = process_remap(keycode, record);
    if (remapped_keycode != KC_NO) {
        has_remap = true;
        keycode = remapped_keycode;
    }
    
    window_switch(keycode, record);
    return !has_remap;
}

// faster tapping term for space layer keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(RAISE, KC_SPC):
            return TAPPING_TERM_FAST;
        case LT(LOWER, KC_SPC): 
            return TAPPING_TERM_FAST;
        case LT(COMBO, KC_SPC): 
            return TAPPING_TERM_FAST;
        default:
            return TAPPING_TERM;
    }
}