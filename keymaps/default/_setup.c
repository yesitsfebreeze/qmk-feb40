#include QMK_KEYBOARD_H
#include "_layers.h"
#include "features/_macros.h"
#include "features/_remote_mode.h"
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
    if (process_macros(keycode, record)) return false;
    if (handle_rgb_mode(keycode, record)) return false;

    process_remote_mode(keycode, record);

    bool has_change = false;

    uint16_t remote_keycode = process_remote_press(keycode, record);
    if (remote_keycode != KC_NO) {
        has_change = true;
        keycode = remote_keycode;
    }

    uint16_t remapped_keycode = process_remap(keycode, record);
    if (remapped_keycode != KC_NO) {
        has_change = true;
        keycode = remapped_keycode;
    }
    
    window_switch(keycode, record);
    return !has_change;
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