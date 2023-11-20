#include "src/core.h"

int8_t OS = OS_WIN;

bool has_remap = false;
uint16_t current_kc = CK_NO;
ModState remap_mod_state = {}; 
CustomKey custom_keys[MATRIX_ROWS][MATRIX_COLS] = {};

ModState get_current_mod_state() {
  uint8_t mods = get_mods();
  return (ModState) {
    .NONE     = (mods == 0),
    .CTRL     = (mods & MOD_MASK_CTRL),
    .CTRL_L   = (mods & MOD_MASK_CTRL_L),
    .CTRL_R   = (mods & MOD_MASK_CTRL_R),
    .ALT      = (mods & MOD_MASK_ALT),
    .ALT_L    = (mods & MOD_MASK_ALT_L),
    .ALT_R    = (mods & MOD_MASK_ALT_R),
    .GUI      = (mods & MOD_MASK_GUI),
    .GUI_L    = (mods & MOD_MASK_GUI_L),
    .GUI_R    = (mods & MOD_MASK_GUI_R),
    .SHIFT    = (mods & MOD_MASK_SHIFT),
    .SHIFT_L  = (mods & MOD_MASK_SHIFT_L),
    .SHIFT_R  = (mods & MOD_MASK_SHIFT_R),
  };
}

void _handle_custom_mod_mask(uint16_t kc, bool state) {
  if (kc == KC_LCTL) remap_mod_state.CTRL   = remap_mod_state.CTRL_L  = state;
  if (kc == KC_RCTL) remap_mod_state.CTRL   = remap_mod_state.CTRL_R  = state;
  if (kc == KC_LGUI) remap_mod_state.GUI    = remap_mod_state.GUI_L   = state;
  if (kc == KC_RGUI) remap_mod_state.GUI    = remap_mod_state.GUI_R   = state;
  if (kc == KC_LALT) remap_mod_state.ALT    = remap_mod_state.ALT_L   = state;
  if (kc == KC_RALT) remap_mod_state.ALT    = remap_mod_state.ALT_R   = state;
  if (kc == KC_LSFT) remap_mod_state.SHIFT  = remap_mod_state.SHIFT_L = state;
  if (kc == KC_RSFT) remap_mod_state.SHIFT  = remap_mod_state.SHIFT_R = state;
}

void _set_mod_state(bool state, uint8_t mask) {
  if (state) return add_mods(mask);
  del_mods(mask);
}

void _set_remap_mod_state(void) {
  _set_mod_state(remap_mod_state.CTRL_L, MOD_MASK_CTRL_L);
  _set_mod_state(remap_mod_state.CTRL_R, MOD_MASK_CTRL_R);
  
  _set_mod_state(remap_mod_state.GUI_L, MOD_MASK_GUI_L);
  _set_mod_state(remap_mod_state.GUI_R, MOD_MASK_GUI_R);

  _set_mod_state(remap_mod_state.ALT_L, MOD_MASK_ALT_L);
  _set_mod_state(remap_mod_state.ALT_R, MOD_MASK_ALT_R);

  _set_mod_state(remap_mod_state.SHIFT_L, MOD_MASK_SHIFT_L);
  _set_mod_state(remap_mod_state.SHIFT_R, MOD_MASK_SHIFT_R);
}

bool handle_os_cycle(uint16_t kc, keyrecord_t *rec) {
  if (kc != CK_OS || !rec->event.pressed) return false;
  OS++;
  OS = OS % OS_LST;
  return true;
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
  CustomKey *custom_key = &custom_keys[rec->event.key.row][rec->event.key.col];

  if (pressed && !custom_key->pressed) {
    if (!has_remap) return false;
    custom_key->pressed = true;
    custom_key->kc = current_kc;
    _handle_custom_mod_mask(kc, false);
    _handle_custom_mod_mask(custom_key->kc, true);
    unregister_code16(kc);
    register_code16(custom_key->kc);

    return true;
  }
  
  if (!pressed && custom_key->pressed) {
    custom_key->pressed = false;
    unregister_code16(custom_key->kc);
    _handle_custom_mod_mask(custom_key->kc, false);

    return true;
  }

  return false;
}

bool handle_core(uint16_t kc, keyrecord_t *rec) {
  if (handle_os_cycle(kc, rec)) return false;
  if ((IS_QK_MOD_TAP(kc) || IS_QK_LAYER_TAP(kc)) && rec->tap.count == 0) return false;

  bool handled = false;
  has_remap = false;
  current_kc = kc;
  remap_mod_state = get_current_mod_state();
  del_mods(MOD_MASK_CSAG);

  if (rec->event.pressed) {
    _handle_custom_mod_mask(kc, true);
  } else {
    _handle_custom_mod_mask(kc, false);
  }

  uint16_t remap = process_remaps(current_kc, remap_mod_state);
  if (remap != CK_NO) current_kc = remap;

  uint16_t os = process_os(current_kc, remap_mod_state, OS);
  if (os != CK_NO) current_kc = os;

  has_remap = (remap != CK_NO || os != CK_NO);

  if (has_remap && current_kc == KC_NO)   handled = true;
  if (!handled && handle_macro(kc, rec))  handled = true;
  if (!handled && handle_remap(kc, rec))  handled = true;
    
  _set_remap_mod_state();
  return handled;
}

uint16_t get_current_keycode(void) {
  return current_kc;
}
