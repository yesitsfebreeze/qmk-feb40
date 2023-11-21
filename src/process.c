#include QMK_KEYBOARD_H
#include "keymap.h"
#include "src/core.h"
#include "src/rgb.h"

bool __ws_enabled = false;
bool __ws_ctrl_down = false;
bool __ws_gui_down = false;
bool __ws_alt_down = false;

typedef union {
  struct {
    bool enabled;
    bool ctl;
    bool alt;
    bool gui;
    bool any;
  };
} TabbingState;

TabbingState tabbing_state = {};

void get_tabbing_mod(uint16_t kc, keyrecord_t *rec) {
  bool pressed = rec->event.pressed;
  if (kc == KC_LCTL || kc == KC_RCTL) tabbing_state.ctl = pressed;
  if (kc == KC_LGUI || kc == KC_RGUI || (kc == CK_GUI && OS == OS_REM)) tabbing_state.gui = pressed;
  if (kc == KC_LALT || kc == KC_LALT || (kc == CK_ALT && OS == OS_REM)) tabbing_state.alt = pressed;
  
  tabbing_state.any = tabbing_state.ctl || tabbing_state.gui || tabbing_state.alt;
}

void handle_tabbing(uint16_t kc, keyrecord_t *rec) {
  bool pressed = rec->event.pressed;

  if (!tabbing_state.any && tabbing_state.enabled && !pressed) {
    layer_move(BASE);
    tabbing_state.enabled = false;
  }

  if (kc != KC_TAB) return;

  if (tabbing_state.any && !tabbing_state.enabled && pressed) {
    layer_move(LOWER);
    tabbing_state.enabled = true;
  }
}

void keyboard_post_init_user(void) {
  #ifdef CONSOLE_ENABLE
    debug_enable=true;
  #endif
  init_rgb();
}

bool is_core_kc(uint16_t kc) {
  if (kc == LT_L) return true;
  if (kc == LT_R) return true;
  if (kc == LT_C) return true;
  if (kc == QK_BOOT) return true;
  
  return false;
}

bool process_record_user(uint16_t kc, keyrecord_t *rec) {
  if (is_core_kc(kc)) return true;
  bool can_continue = true;
  if (handle_core(kc, rec)) can_continue = false;
  kc = get_current_keycode();

  if (handle_rgb(kc, rec)) return false;
  get_tabbing_mod(kc, rec);
  handle_tabbing(kc, rec);

  return can_continue;
}

// faster tapping term for space layer keys
uint16_t get_tapping_term(uint16_t kc, keyrecord_t *rec) {
  if (kc == LT_R || kc == LT_L || kc == LT_C) return TAPPING_TERM_FAST;
  return TAPPING_TERM;
}
