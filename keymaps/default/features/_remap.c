// remaps any key to any other key in any mod state

#include "_remap.h"

#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

uint8_t get_full_mods(void) {
    uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
    return (mods | get_weak_mods() | get_oneshot_mods()); 
#else
    return (mods | get_weak_mods()); 
#endif
}

void remove_mod_mask(uint8_t mask) {
    del_weak_mods(mask);
#ifndef NO_ACTION_ONESHOT
    del_oneshot_mods(mask);
#endif
    del_mods(mask);
}

bool is_shifted_keycode(uint16_t keycode) {
    if (((keycode) >= KC_A && (keycode) <= KC_Z)) return true;
    return IS_QK_MODS(keycode) && (QK_MODS_GET_MODS(keycode) & MOD_MASK_SHIFT) != 0;
}

uint16_t remap_press(uint16_t keycode, keyrecord_t* record) {
    uint8_t mods = get_full_mods();
    uint16_t resoved_keycode = remap_key_list[keycode - RM(0)][mods];

    // if we dont find any remapped keycode, the key should behave normally with its mods
    if (resoved_keycode == KC_NO) {
        uint16_t resoved_keycode = remap_key_list[keycode - RM(0)][0];
        unregister_code16(keycode);
        register_code16(resoved_keycode);
        remapped_keycode = resoved_keycode;

        return remapped_keycode;
    } 

    remove_mod_mask(MOD_MASK_CTRL);
    remove_mod_mask(MOD_MASK_ALT);
    remove_mod_mask(MOD_MASK_GUI);

    if (mods & MOD_MASK_SHIFT && !is_shifted_keycode(resoved_keycode)) {
        remove_mod_mask(MOD_MASK_SHIFT);
    }
    
    unregister_code16(keycode);
    register_code16(resoved_keycode);
    set_mods(mods);

    remapped_keycode = resoved_keycode;

    return remapped_keycode;
}

uint16_t remap_release(uint16_t keycode) {
    if (remapped_keycode == KC_NO) return KC_NO;
    unregister_code16(remapped_keycode);
    remapped_keycode = KC_NO;
    return remapped_keycode;
}

uint16_t process_remap(uint16_t keycode, keyrecord_t* record) {
    // Continue default handling if this is a tap-hold key being held.
    if ((IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) && record->tap.count == 0) return KC_NO;
    if (keycode < QK_USER || keycode > QK_USER_MAX) return KC_NO;

    const bool pressed = record->event.pressed;
    if (pressed) return remap_press(keycode, record);
    return remap_release(keycode);
}

