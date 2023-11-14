#include "src/core.h"

#ifdef CONSOLE_ENABLE
  #include "print.h"
#endif

int OS = OS_WINDOWS;
uint16_t __custom_kc = KC_NO;

bool char_empty(char* char_arr) {
  return char_arr == NULL || *char_arr == '\0';
}

uint8_t get_full_mods(void) {
  uint8_t mods = get_mods();
  #ifndef NO_ACTION_ONESHOT
    return (mods | get_weak_mods() | get_oneshot_mods()); 
  #else
    return (mods | get_weak_mods()); 
  #endif
}

void remove_mod_mask(uint8_t mask) {
  del_weak_mods(mask);
  #ifndef NO_ACTION_ONESHOT
    del_oneshot_mods(mask);
  #endif
  del_mods(mask);
}

bool is_shifted_keycode(uint16_t keycode) {
  if (((keycode) >= KC_A && (keycode) <= KC_Z)) return true;
  return IS_QK_MODS(keycode) && (QK_MODS_GET_MODS(keycode) & MOD_MASK_SHIFT) != 0;
}

Mods get_mod_state(uint8_t mods) {
  return (Mods) {
    .CTRL = mods & MOD_MASK_CTRL,
    .ALT = mods & MOD_MASK_ALT,
    .GUI = mods & MOD_MASK_GUI,
    .SHIFT = mods & MOD_MASK_SHIFT,
  };
}

bool handle_macro(uint16_t kc, uint16_t custom, uint8_t mods, keyrecord_t *rec) {
  if (!rec->event.pressed) return false;

  char* macro = process_macros(custom);
  if (!char_empty(macro)) {
    remove_mod_mask(MOD_MASK_CTRL);
    remove_mod_mask(MOD_MASK_ALT);
    remove_mod_mask(MOD_MASK_GUI);
    remove_mod_mask(MOD_MASK_SHIFT);
    unregister_code16(kc);
    send_string(macro);
    set_mods(mods);
    return true;
  }

  return false;
}

bool remap_press(uint16_t kc, uint16_t custom) {
  if (custom == 0) return false;
  uint8_t mods = get_full_mods();

  remove_mod_mask(MOD_MASK_CTRL);
  remove_mod_mask(MOD_MASK_ALT);
  remove_mod_mask(MOD_MASK_GUI);
  if (mods & MOD_MASK_SHIFT && !is_shifted_keycode(custom)) {
    remove_mod_mask(MOD_MASK_SHIFT);
  }

  unregister_code16(kc);
  register_code16(custom);
  set_mods(mods);

  __custom_kc = custom;

  return true;
}

bool remap_release(uint16_t kc, uint16_t custom) {
    if (__custom_kc == 0) return false;
    unregister_code16(__custom_kc);
    __custom_kc = 0;
    return true;
}

bool handle_os_cycle(uint16_t kc, keyrecord_t *rec) {
  if (kc != CK_OS) return false;
  if (!rec->event.pressed) return false;
  OS++;
  if (OS > sizeof(enum OS_TYPES)) OS = 0;
  return true;
}

bool handle_core(uint16_t kc, keyrecord_t *rec) {
  if (handle_os_cycle(kc, rec)) return false;

  if ((IS_QK_MOD_TAP(kc) || IS_QK_LAYER_TAP(kc)) && rec->tap.count == 0) return false;

  uint16_t custom = kc;
  uint8_t mods = get_full_mods();
  Mods mod_state = get_mod_state(mods);
  
  uint16_t remapped = process_remaps(kc, mod_state);
  if (remapped != 0) custom = remapped;
  uint16_t os = process_os(custom, mod_state, OS);
  if (os != 0) custom = os;

  // after remap and os keys, check if we ended up with a macro
  if (handle_macro(kc, custom, mods, rec)) return false;
  
  if ((remapped != 0 || os != 0)) {
    dprintf("custom kc 0x%04X\n", custom);
    const bool pressed = rec->event.pressed;
    if (pressed) return remap_press(kc, custom);
    return remap_release(kc, custom);
  }

  return false;
}