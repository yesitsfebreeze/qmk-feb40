// goes to lower level if window switch combo is pressed

#include "_window_switch.h"
#include "../_layers.h"

bool is_in_window_switch = false;

uint16_t remote_alt = KC_F19;
bool remote_alt_down = false;

uint16_t remote_gui = KC_F18;
bool remote_gui_down = false;


void window_switch(uint16_t keycode, keyrecord_t* record) {
    uint8_t mods = get_mods();

    if (record->event.pressed) {
        if (keycode == remote_alt) remote_alt_down = true;
        if (keycode == remote_gui) remote_gui_down = true;
    } else {
        if (keycode == remote_alt) remote_alt_down = false;
        if (keycode == remote_gui) remote_gui_down = false;
    }

    if (record->event.pressed) {
        if (keycode == KC_TAB) {
            bool alt_down = (mods & MOD_MASK_ALT) || remote_alt_down;
            bool gui_down = (mods & MOD_MASK_GUI) || remote_gui_down;
            bool ctrl_down = (mods & MOD_MASK_CTRL);

            if (alt_down || gui_down || ctrl_down) {
                layer_move(LOWER);
                is_in_window_switch = true;
            }
        }
    } else if (is_in_window_switch) {
        bool alt_up = keycode == KC_LALT || keycode == KC_RALT || keycode == remote_alt;
        bool gui_up = keycode == KC_LGUI || keycode == KC_RGUI || keycode == remote_gui;
        bool ctrl_up = keycode == KC_LCTL || keycode == KC_LCTL;

        if (alt_up || gui_up || ctrl_up) {
            layer_move(BASE);
            is_in_window_switch = false;
        }
    }
} 