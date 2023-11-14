#include QMK_KEYBOARD_H
#include "src/core.h"
#include "src/rgb.h"

bool __ws_enabled = false;
bool __ws_kc = false;
bool __ws_ctrl_down = false;
bool __ws_gui_down = false;
bool __ws_alt_down = false;

void handle_window_switch(uint16_t kc, keyrecord_t *rec) {
  bool pressed = rec->event.pressed;
  if (kc == KC_LALT || kc == KC_LALT) __ws_alt_down = pressed;
  if (kc == KC_LGUI || kc == KC_RGUI) __ws_gui_down = pressed;
  if (kc == KC_LCTL || kc == KC_RCTL) __ws_ctrl_down = pressed;

  if (pressed) {
    bool any_down = __ws_alt_down || __ws_gui_down || __ws_ctrl_down;
    if (kc == KC_TAB && any_down) {
      layer_move(LOWER);
      __ws_enabled = true;
      
    }
    
    return;
  } else {
    bool all_up = !__ws_alt_down && !__ws_gui_down && !__ws_ctrl_down;
    if (__ws_enabled && all_up) {
      layer_move(BASE);
      __ws_enabled = false;
    }
    return;
  }
}

void keyboard_post_init_user(void) {
  #ifdef CONSOLE_ENABLE
    debug_enable=true;
  #endif
  init_rgb();
}

bool process_record_user(uint16_t kc, keyrecord_t *rec) {
  handle_window_switch(kc, rec);
  if (handle_rgb(kc, rec)) return false;
  if (handle_core(kc, rec)) return false;

  return true;
}

// faster tapping term for space layer keys
uint16_t get_tapping_term(uint16_t kc, keyrecord_t *rec) {
  if (kc == LT_RAISE || kc == LT_LOWER || kc == LT_COMBO) return TAPPING_TERM_FAST;
  return TAPPING_TERM;
}
