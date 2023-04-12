#include "_mods.h"

void _get_mods(void) {
    const uint8_t mod_state = get_mods();
    uint8_t mods;
    #ifndef NO_ACTION_ONESHOT
        mods = (mod_state | get_weak_mods() | get_oneshot_mods()); 
    #else
        mods = (mod_state | get_weak_mods()); 
    #endif

    _mods = mods;
}

void _remove_shift_mod(void) {
    del_weak_mods(MOD_MASK_SHIFT);
#ifndef NO_ACTION_ONESHOT
    del_oneshot_mods(MOD_MASK_SHIFT);
#endif
    del_mods(MOD_MASK_SHIFT);
}

bool _is_shifted_keycode(uint16_t keycode) {
    return IS_QK_MODS(keycode) && (QK_MODS_GET_MODS(keycode) & MOD_LSFT) != 0;
}

void _temp_unshift(uint16_t keycode) {
    if ((keycode) >= KC_A && (keycode) <= KC_Z) {
        if (_mods && MOD_MASK_SHIFT) {
            set_mods(MOD_MASK_SHIFT);
        }
        return;   
    }

    if (_is_shifted_keycode(keycode)) {
        set_mods(MOD_MASK_SHIFT);
        return;
    }

    _remove_shift_mod(); 
}