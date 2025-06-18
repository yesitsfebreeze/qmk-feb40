#include QMK_KEYBOARD_H
#include "keymap.h"
#include "src/rgb.h"
#include "src/defines.h"
#include "src/process.h"
#ifdef CONSOLE_ENABLE
#include "print.h"
#endif


State state = {
  .layer = BASE,
  .pressed = false,
  .mods = {
    .NONE = true,
    .CTRL = false,
    .CTRL_L = false,
    .CTRL_R = false,
    .ALT = false,
    .ALT_L = false,
    .ALT_R = false,
    .GUI = false,
    .GUI_L = false,
    .GUI_R = false,
    .SHIFT = false,
    .SHIFT_L = false,
    .SHIFT_R = false
  },
  .tabbing = {false, false, false, false, false},
  .stats = {0, 250, 0, 0, false, false, false, false},
  .hype = {false, false, 0},
  .keys = {}
};

bool has_remap = false;
uint16_t current_kc = CK_NO;

ModState _get_current_mod_state(void) {
  uint8_t mods = get_mods();
  state.mods = (ModState){
    .NONE     = (mods == 0),
    .CTRL     = (mods & MOD_MASK_CTRL)  ? true : false,
    .CTRL_L   = (mods & MOD_MASK_CTRL_L) ? true : false,
    .CTRL_R   = (mods & MOD_MASK_CTRL_R) ? true : false,
    .ALT      = (mods & MOD_MASK_ALT)   ? true : false,
    .ALT_L    = (mods & MOD_MASK_ALT_L) ? true : false,
    .ALT_R    = (mods & MOD_MASK_ALT_R) ? true : false,
    .GUI      = (mods & MOD_MASK_GUI)   ? true : false,
    .GUI_L    = (mods & MOD_MASK_GUI_L) ? true : false,
    .GUI_R    = (mods & MOD_MASK_GUI_R) ? true : false,
    .SHIFT    = (mods & MOD_MASK_SHIFT) ? true : false,
    .SHIFT_L  = (mods & MOD_MASK_SHIFT_L) ? true : false,
    .SHIFT_R  = (mods & MOD_MASK_SHIFT_R) ? true : false,
  };

  return state.mods;
}

void _handle_custom_mod_mask(uint16_t kc, bool active) {
  switch (kc) {
    case KC_LCTL: state.mods.CTRL = state.mods.CTRL_L = active; break;
    case KC_RCTL: state.mods.CTRL = state.mods.CTRL_R = active; break;
    case KC_LGUI: state.mods.GUI  = state.mods.GUI_L  = active; break;
    case KC_RGUI: state.mods.GUI  = state.mods.GUI_R  = active; break;
    case KC_LALT: state.mods.ALT  = state.mods.ALT_L  = active; break;
    case KC_RALT: state.mods.ALT  = state.mods.ALT_R  = active; break;
    case KC_LSFT: state.mods.SHIFT = state.mods.SHIFT_L = active; break;
    case KC_RSFT: state.mods.SHIFT = state.mods.SHIFT_R = active; break;
  }
}

void _set_mod_state(bool state, uint8_t mask) {
  if (state) {
    add_mods(mask);
  } else {
    del_mods(mask);
  }
}

void _reset_mod_state(void) {
  _set_mod_state(state.mods.CTRL_L, MOD_MASK_CTRL_L);
  _set_mod_state(state.mods.CTRL_R, MOD_MASK_CTRL_R);

  _set_mod_state(state.mods.GUI_L, MOD_MASK_GUI_L);
  _set_mod_state(state.mods.GUI_R, MOD_MASK_GUI_R);

  _set_mod_state(state.mods.ALT_L, MOD_MASK_ALT_L);
  _set_mod_state(state.mods.ALT_R, MOD_MASK_ALT_R);

  _set_mod_state(state.mods.SHIFT_L, MOD_MASK_SHIFT_L);
  _set_mod_state(state.mods.SHIFT_R, MOD_MASK_SHIFT_R);
}

bool handle_macro(uint16_t kc, keyrecord_t *rec) {
  char* macro = process_macros(current_kc);
  if (macro == NULL || *macro == '\0' || !rec->event.pressed) return false;

  unregister_code16(kc);
  unregister_code16(current_kc);
  send_string(macro);

  return true;
}

bool handle_remap(uint16_t kc, keyrecord_t *rec) {
  bool pressed = rec->event.pressed;
  CustomKey *key = &state.keys[rec->event.key.row][rec->event.key.col];

  if (pressed && !key->pressed) {
    if (!has_remap) return false;
    key->pressed = true;
    key->kc = current_kc;
    _handle_custom_mod_mask(kc, false);
    _handle_custom_mod_mask(key->kc, true);
    unregister_code16(kc);
    register_code16(key->kc);

    return true;
  }

  if (!pressed && key->pressed) {
    key->pressed = false;
    unregister_code16(key->kc);
    _handle_custom_mod_mask(key->kc, false);

    return true;
  }

  return false;
}

bool handle_core(uint16_t kc, keyrecord_t *rec) {
  if ((IS_QK_MOD_TAP(kc) || IS_QK_LAYER_TAP(kc)) && rec->tap.count == 0) return false;

  bool handled = false;
  has_remap = false;
  current_kc = kc;
  state.mods = _get_current_mod_state();
  del_mods(MOD_MASK_CSAG);
  _handle_custom_mod_mask(kc, rec->event.pressed);

  uint16_t remap = process_remaps(current_kc, state.mods);
  if (remap != CK_NO) current_kc = remap;

  has_remap = remap != CK_NO;

  if (has_remap && current_kc == KC_NO)   handled = true;
  if (!handled && handle_macro(kc, rec))  handled = true;
  if (!handled && handle_remap(kc, rec))  handled = true;

  _reset_mod_state();
  return handled;
}

uint16_t get_current_keycode(void) {
  return current_kc;
}

void handle_tabbing(uint16_t kc) {
  if (state.layer == GAME) return;

  if (kc == KC_LCTL) state.tabbing.ctl = state.pressed;
  if (kc == KC_LGUI) state.tabbing.gui = state.pressed;
  if (kc == KC_LALT) state.tabbing.alt = state.pressed;

  state.tabbing.any = state.tabbing.ctl || state.tabbing.gui || state.tabbing.alt;

  if (state.pressed) {
    if (kc != KC_TAB) return;
    if (state.tabbing.enabled) return;
    if (!state.tabbing.any) return;

    layer_move(LOWER);
    state.tabbing.enabled = true;
  } else {
    if (!state.tabbing.enabled) return;

    state.tabbing.any = state.tabbing.ctl || state.tabbing.gui || state.tabbing.alt;

    if (state.tabbing.any) return;

    layer_move(BASE);
    state.tabbing.enabled = false;
  }
}

bool is_core_kc(uint16_t kc) {
  switch (kc) {
    case LT_L:
    case LT_R:
    case LT_C:
    case QK_BOOT:
      return true;
  }
  return false;
}

bool handle_stats(uint16_t kc) {
  if (kc != CK_STATS) return false;
  state.stats.enabled = state.pressed;
  if (!state.stats.enabled) return false;
  state.stats.exec = true;

  state.stats.last_timer = timer_elapsed(state.stats.timer);
  state.stats.timer = timer_read();
  state.stats.state = 0;

  if (state.stats.last_timer < state.stats.time) state.stats.state = 1;
  return true;
}

void process_stats(void) {
  if (!state.stats.exec) return;
  if (!state.stats.enabled) {
    if (!state.stats.running) return;
    state.stats.exec = false;
    unregister_code16(KC_TAB);
    unregister_code16(KC_CAPS);
    if (state.stats.caps) {
      tap_code16(KC_CAPS);
      state.stats.caps = false;
    }
    state.stats.running = false;
  } else {
    if (state.stats.running) return;
    if (state.stats.state == 0) {
      register_code16(KC_TAB);
      state.stats.caps = false;
    }
    if (state.stats.state == 1) {
      register_code16(KC_CAPS);
      state.stats.caps = true;
    }
    state.stats.running = true;
  }
}

bool handle_hype(uint16_t kc) {
  if (kc != CK_HYPE) {
    state.hype.interrupt = true;
    return false;
  }
  state.hype.pressed = state.pressed;

  if (state.hype.pressed) {
    state.hype.interrupt = false;
    state.hype.timer = timer_read();
  } else {
    state.hype.active = false;
    unregister_code16(KC_LCTL);
  }

  return true;
}

void process_hype(void) {
  uint16_t elapsed = timer_elapsed(state.hype.timer);

  if (!state.hype.pressed) {
    if (state.hype.interrupt) {
      return;
    }
    if (!state.hype.active && elapsed < TAPPING_TERM_SLOW) {
      tap_code16(HYPR(KC_M));
      state.hype.active = true;
    }
  } else {
    register_code16(KC_LCTL);
  }
}

// QMK hooks

void keyboard_post_init_user(void) {
  #ifdef CONSOLE_ENABLE
    debug_enable = true;
  #endif
  #ifdef RGB_MATRIX_ENABLE
    init_rgb();
  #endif
}

void matrix_scan_user(void) {
  process_stats();
  process_hype();
}

bool process_record_user(uint16_t kc, keyrecord_t *rec) {
  state.layer = get_highest_layer(layer_state);
  state.pressed = rec->event.pressed;

  if (is_core_kc(kc)) return true;
  bool can_continue = true;
  if (handle_core(kc, rec)) can_continue = false;
  kc = get_current_keycode();

  if (handle_stats(kc)) return false;
  if (handle_hype(kc)) return false;
  #ifdef RGB_MATRIX_ENABLE
    if (handle_rgb(kc, rec)) return false;
  #endif

  handle_tabbing(kc);
  return can_continue;
}

// Faster tapping term for space layer keys
uint16_t get_tapping_term(uint16_t kc, keyrecord_t *rec) {
  switch (kc) {
    case LT_L:
    case LT_R:
    case LT_C:
      return TAPPING_TERM;
    default:
      return TAPPING_TERM_SLOW;
  }
}
