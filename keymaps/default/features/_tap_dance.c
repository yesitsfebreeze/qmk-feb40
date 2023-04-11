#include "_tap_dance.h"
#include "_mods.h"
#include "_remap.h"

void _tap_dance_assign(_tap_dance *td) {
    _tap_dance_available = true;
    _tap_dance_active_keycode = td->keycode;
    _tap_dance_max_taps = td->max_taps;
    for (int k = 0; k <= 5; k++) {
        _tap_dance_keycodes[k] = td->keycodes[k];
    }
}

void _tap_dance_reset() {    
    _tap_dance_available = false;
    _tap_dance_active_keycode = KC_NO;
    _tap_dance_taps = 0;
    _tap_dance_max_taps = 0;
    _tap_dance_time = 0;
    for (int k = 0; k <= 5; k++) {
        _tap_dance_keycodes[k] = KC_NO;   
    }
}

void _tap_dance_execute(void) {
    if (!_tap_dance_available) return;
    uint16_t resolved_keycode = _remap_resolve_key(_tap_dance_keycodes[_tap_dance_taps]);
    if (resolved_keycode == KC_NO) {
        resolved_keycode = _tap_dance_keycodes[_tap_dance_taps];
    }

    _tap_dance_last_keycode = resolved_keycode;
    del_mods(MOD_MASK_SHIFT);
    register_code16(_tap_dance_last_keycode);
    set_mods(_mods);

    _tap_dance_reset();
}

void _tap_dance_handle(_tap_dance *td) {
    // finish previous tap dance
    if (td->keycode != _tap_dance_active_keycode) _tap_dance_execute();
    
    _tap_dance_assign(td);
    
    bool in_time = timer_elapsed(_tap_dance_time) <= TAP_DANCE_TIME;
    if (!in_time) _tap_dance_taps = 0;
    if (in_time) _tap_dance_taps += 1;

    if (_tap_dance_taps > _tap_dance_max_taps) {
        _tap_dance_taps = _tap_dance_max_taps;
    }

    _tap_dance_time = timer_read();
}

bool _process_tap_dance(uint16_t keycode, keyrecord_t *record) {
    _tap_dance_released = true;
    for (int i = 0; i < _tap_dances_size; i++) {
        _tap_dance *td = &_tap_dances[i];
        if (td->keycode == keycode) {
            if (!record->event.pressed) {
                _tap_dance_released = true;
                return false;
            } else {
                _tap_dance_released = false;
                _tap_dance_handle(td);
                return true;
            }
        };
    }

    // finish previous tap dance
    _tap_dance_execute();
    return false;
}

void _process_tap_dance_execute(void) {
    if (_tap_dance_released && _tap_dance_last_keycode != KC_NO) {
        unregister_code(_tap_dance_last_keycode);
        _tap_dance_last_keycode = KC_NO;
    }

    if (_tap_dance_active_keycode == KC_NO) return;
    if (timer_elapsed(_tap_dance_time) >= TAP_DANCE_TIME) _tap_dance_execute();
}

