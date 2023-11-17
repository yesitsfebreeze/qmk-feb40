#include "src/core.h"

int8_t OS = OS_WINDOWS;

bool __has_remap = false;
uint16_t __target_kc = 0;
CustomKey __custom_keys[MATRIX_ROWS][MATRIX_COLS] = {};

uint8_t get_full_mods(void) {
  uint8_t mods = get_mods();
  #ifndef NO_ACTION_ONESHOT
    return (uint8_t)(mods | get_weak_mods() | get_oneshot_mods());
  #else
    return (uint8_t)(mods | get_weak_mods());
  #endif
}

void remove_mod_mask(uint8_t mask) {
  del_weak_mods(mask);
  #ifndef NO_ACTION_ONESHOT
    del_oneshot_mods(mask);
  #endif
  del_mods(mask);
}

ModState get_mod_state(uint8_t mods) {
  return (ModState) {
    .NONE = mods == 0,
    .CTRL = mods & MOD_MASK_CTRL,
    .ALT = mods & MOD_MASK_ALT,
    .GUI = mods & MOD_MASK_GUI,
    .SHIFT = mods & MOD_MASK_SHIFT,
  };
}

bool handle_os_cycle(uint16_t kc, keyrecord_t *rec) {
  if (kc != CK_OS || !rec->event.pressed) return false;
  OS++;
  if (OS > sizeof(enum OS_TYPES)) OS = 0;
  return true;
}

bool handle_macro(uint16_t kc, uint8_t mods, keyrecord_t *rec) {
  if (__target_kc == 0) return false;

  char* macro = process_macros(__target_kc);
  if (macro == NULL || *macro == '\0' || !rec->event.pressed) return false;

  remove_mod_mask(MOD_MASK_CSAG);

  unregister_code16(kc);
  unregister_code16(__target_kc);

  send_string(macro);
  set_mods(mods);

  return true;
}

bool handle_remap(uint16_t kc, uint8_t mods, keyrecord_t *rec) {
  if (__target_kc == 0) return false;

  bool pressed = rec->event.pressed;

  CustomKey *custom_key = &__custom_keys[rec->event.key.row][rec->event.key.col];

  if (pressed && !custom_key->pressed) {
    if (!__has_remap) return false;
    custom_key->pressed = true;
    custom_key->kc = __target_kc;
    custom_key->mods = MOD_BIT(__target_kc);

    unregister_code16(kc);
    remove_mod_mask(MOD_MASK_CSAG);
    register_code16(custom_key->kc);
    set_mods(mods);
    register_mods(custom_key->mods)

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

bool handle_core(uint16_t kc, keyrecord_t *rec) {
  if (handle_os_cycle(kc, rec)) return false;
  if ((IS_QK_MOD_TAP(kc) || IS_QK_LAYER_TAP(kc)) && rec->tap.count == 0) return false;

  __has_remap = false;
  __target_kc = kc;
  uint8_t mods = get_full_mods();
  ModState ms = get_mod_state(mods);

  uint16_t remap = process_remaps(__target_kc, ms);
  if (remap != 0) __target_kc = remap;

  uint16_t os = process_os(__target_kc, ms, OS);
  if (os != 0) __target_kc = os; 

  __has_remap = (remap != 0 || os != 0);

  if (handle_macro(kc, mods, rec)) return true;

  return handle_remap(kc, mods, rec);
}
