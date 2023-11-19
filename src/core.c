#include "src/core.h"

int8_t OS = OS_WIN;

bool __has_remap = false;
uint16_t __remap_kc = CK_NO;
CustomKey __custom_keys[MATRIX_ROWS][MATRIX_COLS] = {};
uint8_t __mods_to_remove = 0;

uint8_t _get_full_mods(void) {
  uint8_t mods = get_mods();
  #ifndef NO_ACTION_ONESHOT
    return (uint8_t)(mods | get_weak_mods() | get_oneshot_mods());
  #else
    return (uint8_t)(mods | get_weak_mods());
  #endif
}

void _remove_mod_mask(uint8_t mask) {
  del_weak_mods(mask);
  #ifndef NO_ACTION_ONESHOT
    del_oneshot_mods(mask);
  #endif
  del_mods(mask);
}

ModState get_mod_state() {
  uint8_t mods = _get_full_mods();
  return (ModState) {
    .NONE = (mods == 0),
    .CTRL = (mods & MOD_MASK_CTRL),
    .CTRL_L = (mods & MOD_MASK_CTRL_L),
    .CTRL_R = (mods & MOD_MASK_CTRL_R),
    .ALT = (mods & MOD_MASK_ALT),
    .ALT_L = (mods & MOD_MASK_ALT_L),
    .ALT_R = (mods & MOD_MASK_ALT_R),
    .GUI = (mods & MOD_MASK_GUI),
    .GUI_L = (mods & MOD_MASK_GUI_L),
    .GUI_R = (mods & MOD_MASK_GUI_R),
    .SHIFT = (mods & MOD_MASK_SHIFT),
    .SHIFT_L = (mods & MOD_MASK_SHIFT_L),
    .SHIFT_R = (mods & MOD_MASK_SHIFT_R),
  };
}

bool handle_os_cycle(uint16_t kc, keyrecord_t *rec) {
  if (kc != CK_OS || !rec->event.pressed) return false;
  OS++;
  OS = OS % OS_LST;
  return true;
}

bool handle_macro(uint16_t kc, uint8_t mods, keyrecord_t *rec) {
  char* macro = process_macros(__remap_kc);
  if (macro == NULL || *macro == '\0' || !rec->event.pressed) return false;

  _remove_mod_mask(MOD_MASK_CSAG);

  unregister_code16(kc);
  unregister_code16(__remap_kc);

  send_string(macro);
  set_mods(mods);

  return true;
}

bool _is_mod_key(uint16_t kc) {
  if (kc == KC_LCTL) return true;
  if (kc == KC_RCTL) return true;
  if (kc == KC_LGUI) return true;
  if (kc == KC_RGUI) return true;
  if (kc == KC_LALT) return true;
  if (kc == KC_RALT) return true;
  if (kc == KC_LSFT) return true;
  if (kc == KC_RSFT) return true;
  return false;
}

bool handle_remap(uint16_t kc, uint8_t mods, keyrecord_t *rec) {
  bool pressed = rec->event.pressed;

  CustomKey *custom_key = &__custom_keys[rec->event.key.row][rec->event.key.col];

  if (pressed && !custom_key->pressed) {
    if (!__has_remap) return false;
    custom_key->pressed = true;
    custom_key->kc = __remap_kc;
    custom_key->mods = (_is_mod_key(__remap_kc)) ? MOD_BIT(__remap_kc) : 0;
    __mods_to_remove = MOD_BIT(kc);
    unregister_code16(kc);
    _remove_mod_mask(MOD_MASK_CSAG);
    register_code16(custom_key->kc);
    set_mods(mods);
    register_mods(custom_key->mods);
    _remove_mod_mask(__mods_to_remove);
    return true;
  }

  if (!pressed && custom_key->pressed) {
    custom_key->pressed = false;
    unregister_code16(custom_key->kc);
    unregister_mods(custom_key->mods);
    return true;
  }

  return false;
}

void handle_core_pre(uint16_t kc, keyrecord_t *rec) {
  _remove_mod_mask(__mods_to_remove);
  __mods_to_remove = 0;
}

bool handle_core(uint16_t kc, keyrecord_t *rec) {
  if (handle_os_cycle(kc, rec)) return false;
  if ((IS_QK_MOD_TAP(kc) || IS_QK_LAYER_TAP(kc)) && rec->tap.count == 0) return false;

  __has_remap = false;
  __remap_kc = kc;
  ModState ms = get_mod_state();

  uint16_t remap = process_remaps(__remap_kc, ms);
  if (remap != CK_NO) __remap_kc = remap;

  uint16_t os = process_os(__remap_kc, ms, OS);
  if (os != CK_NO) __remap_kc = os;

  __has_remap = (remap != CK_NO || os != CK_NO);
  if (__has_remap && __remap_kc == KC_NO) return true;

  uint8_t mods = _get_full_mods();
  if (handle_macro(kc, mods, rec)) return true;

  return handle_remap(kc, mods, rec);
}

uint16_t get_remapped_keycode(void) {
  return __remap_kc;
}
