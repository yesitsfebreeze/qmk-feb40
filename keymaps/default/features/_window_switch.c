// goes to lower level if window switch combo is pressed

#include "_window_switch.h"
#include "../_layers.h"

bool is_in_window_switch = false;

void window_switch(uint16_t keycode, keyrecord_t* record) {
    uint8_t mods = get_mods();

    if (record->event.pressed) {
        if (keycode == KC_TAB) {
            if ((mods & MOD_MASK_ALT) || (mods & MOD_MASK_GUI)) {
                layer_move(LOWER);
                is_in_window_switch = true;
            }
        }
    } else if (is_in_window_switch) {
        bool alt_down = keycode == KC_LALT || keycode == KC_RALT;
        bool gui_down = keycode == KC_LGUI || keycode == KC_RGUI;

        if (alt_down || gui_down) {
            layer_move(BASE);
            is_in_window_switch = false;
        }
    }
} 