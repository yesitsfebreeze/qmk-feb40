#include "_remap.h"
#include "_mods.h"

uint16_t _remap_resolve_key(uint16_t keycode) {
    _get_mods();

    for (int i = 0; i < _key_remap_list_size; i++) {
        _remap_key_t key = _key_remap_list[i];
        if (key.keycode != keycode) continue;
        if (key.mod && (_mods & key.mod) == 0) continue;
        if (key.new_keycode && key.new_keycode != KC_NO) {
            return key.new_keycode;
        }
    }

    return KC_NO;
}

uint16_t _remap_execute(uint16_t keycode) {
    uint16_t resolved_keycode = _remap_resolve_key(keycode);
    if (resolved_keycode == KC_NO) return KC_NO;
    
    _remap_original_keycode = keycode;
    _remap_current_keycode = resolved_keycode;
    _temp_unshift(resolved_keycode);
    unregister_code16(keycode);
    register_code16(resolved_keycode);
    set_mods(_mods);
    
    return resolved_keycode; 
}

bool _process_remap(uint16_t keycode, keyrecord_t* record) {
    if (!record->event.pressed) return false;
    
    // Continue default handling if this is a tap-hold key being held.
    if ((IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) && record->tap.count == 0) {
        return false;
    }

    uint16_t resolved_keycode = _remap_execute(keycode);
    if (resolved_keycode == KC_NO) return false;

    return true; 
}

bool _process_remap_release(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) return false;
    if (_remap_current_keycode == KC_NO) return false;

    unregister_code16(_remap_current_keycode);      
    _remap_current_keycode = KC_NO;
    _remap_original_keycode = KC_NO;
    set_mods(_mods);
    return true;
}