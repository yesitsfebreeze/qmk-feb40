#include QMK_KEYBOARD_H
#include "src/process.h"
#include "src/core.h"
#include "src/rgb.h"

bool is_in_alt_tab = false;

void handle_alt_tab(uint16_t kc, keyrecord_t *rec) {
  uint8_t mods = get_mods();

  if (rec->event.pressed) {
    if (kc == KC_TAB) {
      bool alt_down = (mods & MOD_MASK_ALT);
      bool gui_down = (mods & MOD_MASK_GUI);
      bool ctrl_down = (mods & MOD_MASK_CTRL);

      if (alt_down || gui_down || ctrl_down) {
        layer_move(LOWER);
        is_in_alt_tab = true;
      }
    }
  } else if (is_in_alt_tab) {
    bool alt_up = kc == KC_LALT || kc == KC_RALT;
    bool gui_up = kc == KC_LGUI || kc == KC_RGUI;
    bool ctrl_up = kc == KC_LCTL || kc == KC_LCTL;

    if (alt_up || gui_up || ctrl_up) {
      layer_move(BASE);
      is_in_alt_tab = false;
    }
  }
}

void keyboard_post_init_user(void) {
  #ifdef CONSOLE_ENABLE
    debug_enable=true;
  #endif
  init_rgb();
}

bool process_record_user(uint16_t kc, keyrecord_t *rec) {
  handle_alt_tab(kc, rec);
  if (handle_rgb(kc, rec)) return false;
  if (handle_core(kc, rec)) return false;

  return true;
}

// faster tapping term for space layer keys
uint16_t get_tapping_term(uint16_t kc, keyrecord_t *rec) {
  switch (kc) {
    case LT_RAISE:
      return TAPPING_TERM_FAST;
    case LT_LOWER: 
      return TAPPING_TERM_FAST;
    case LT_COMBO: 
      return TAPPING_TERM_FAST;
    default:
      return TAPPING_TERM;
  }
}
