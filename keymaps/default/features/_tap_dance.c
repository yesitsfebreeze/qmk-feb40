#include "_tap_dance.h"
#include "_mods.h"
#include "_remap.h"

void _tap_dance_assign(_tap_dance *td) {
    _tap_dance_available = true;
    _tap_dance_active_keycode = td->keycode;
    _tap_dance_max_taps = td->max_taps;
    _tap_dance_hold = td->hold;
    for (int k = 0; k <= 5; k++) {
        _tap_dance_keycodes[k] = td->keycodes[k];
    }
}

void _tap_dance_reset() {
    _tap_dance_other_key = false;
    _tap_dance_available = false;
    _tap_dance_active_keycode = KC_NO;
    _tap_dance_taps = 0;
    _tap_dance_max_taps = 0;
    _tap_dance_hold = false;
    _tap_dance_time = 0;
    for (int k = 0; k <= 5; k++) {
        _tap_dance_keycodes[k] = KC_NO;   
    }
}


uint16_t _tap_dance_get_keycode(void) {
    uint16_t resolved_keycode = _remap_resolve_key(_tap_dance_keycodes[_tap_dance_taps]);
    if (resolved_keycode == KC_NO) {
        resolved_keycode = _tap_dance_keycodes[_tap_dance_taps];
    }
    _tap_dance_last_sent_keycode = resolved_keycode;

    _temp_unshift(resolved_keycode);

    return resolved_keycode;
} 


void _tap_dance_execute(void) {
    if (_tap_dance_released && _tap_dance_last_sent_keycode != KC_NO) {
        unregister_code16(_tap_dance_last_sent_keycode);
        _tap_dance_last_sent_keycode = KC_NO;
        set_mods(_mods);
    }

    if (!_tap_dance_available) return;

    uint16_t elapsed = timer_elapsed(_tap_dance_time);
    bool window_elapsed = elapsed >= TAP_DANCE_TIME;
    bool hold_window_elapsed = elapsed >= TAP_DANCE_HOLD_TIME;
   
    if (_tap_dance_other_key) {
        tap_code16(_tap_dance_get_keycode());
        _tap_dance_reset();
        return;
    }

    if (_tap_dance_hold) {
        if (hold_window_elapsed && !_tap_dance_released) {
            _tap_dance_taps = 1;
            register_code16(_tap_dance_get_keycode());
            _tap_dance_reset();
        }
        if (_tap_dance_released) {
            tap_code16(_tap_dance_get_keycode());
            _tap_dance_reset();
            set_mods(_mods);
        }
        return;
    }

    if (window_elapsed) {
        register_code16(_tap_dance_get_keycode());
        _tap_dance_reset();    
    }

}

void _tap_dance_handle(void) {
    if (_tap_dance_hold) {
        _tap_dance_time = timer_read();
        return;
    }

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
                // finish previous tap dance
                if (td->keycode != _tap_dance_active_keycode) {
                    _tap_dance_other_key = true;
                    _process_tap_dance_execute();
                }

                _tap_dance_released = false;
                _tap_dance_other_key = false;
                
                _tap_dance_assign(td);
                _tap_dance_handle();
                return true;
            }
        };
    }

    // other key was pressed
    // finish previous tap dance
    _tap_dance_other_key = true;
    _process_tap_dance_execute();
    return false;
}

void _process_tap_dance_execute(void) {
    _tap_dance_execute();
}

