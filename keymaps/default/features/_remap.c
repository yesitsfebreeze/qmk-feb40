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

bool is_shifted_kc(uint16_t kc) {
    if (((kc) >= KC_A && (kc) <= KC_Z)) return true;
    return IS_QK_MODS(kc) && (QK_MODS_GET_MODS(kc) & MOD_MASK_SHIFT) != 0;
}

uint16_t remap_press(uint16_t kc, keyrecord_t* record) {
    uint8_t mods = get_full_mods();
    uint16_t resoved_kc = remap_key_list[kc - RM(0)][mods];

    // if we dont find any remapped kc, the key should behave normally with its mods
    if (resoved_kc == KC_NO) {
        uint16_t resoved_kc = remap_key_list[kc - RM(0)][0];
        unregister_code16(kc);
        register_code16(resoved_kc);
        remapped_kc = resoved_kc;

        return remapped_kc;
    } 

    remove_mod_mask(MOD_MASK_CTRL);
    remove_mod_mask(MOD_MASK_ALT);
    remove_mod_mask(MOD_MASK_GUI);

    if (mods & MOD_MASK_SHIFT && !is_shifted_kc(resoved_kc)) {
        remove_mod_mask(MOD_MASK_SHIFT);
    }
    
    unregister_code16(kc);
    register_code16(resoved_kc);
    set_mods(mods);

    remapped_kc = resoved_kc;

    return remapped_kc;
}

uint16_t remap_release(void) {
    if (remapped_kc == KC_NO) return KC_NO;
    unregister_code16(remapped_kc);
    remapped_kc = KC_NO;
    return remapped_kc;
}

uint16_t process_remap(uint16_t kc, keyrecord_t* record) {
    // Continue default handling if this is a tap-hold key being held.
    if ((IS_QK_MOD_TAP(kc) || IS_QK_LAYER_TAP(kc)) && record->tap.count == 0) return KC_NO;
    if (kc < QK_USER || kc > QK_USER_MAX) return KC_NO;

    const bool pressed = record->event.pressed;
    if (pressed) return remap_press(kc, record);
    return remap_release();
}

