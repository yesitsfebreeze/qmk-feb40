// remaps any key to any other key in any mod state

#include "_util.h"
#include "_remap.h"

#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

uint16_t remap_press(uint16_t kc, keyrecord_t* record) {
  uint16_t ck = kc - CK(0);
  bool found = false;
  for (size_t i = 0; i < remap_list_size; ++i) {
    if (i == ck) found = true;
  }
  if (!found) return KC_NO;

  uint8_t mods = get_full_mods();
  uint16_t modded_kc = remap_list[ck][mods];

  // if we dont find any moddeed remap
  // the key should behave normally with manual mods
  if (modded_kc == KC_NO) {
    uint16_t resolved_kc = remap_list[ck][0];
    remapped_kc = resolved_kc;

    unregister_code16(kc);
    register_code16(remapped_kc);
    return remapped_kc;
  } 

  // if modded version was found, apply mods
  // then reset after registering
  remapped_kc = modded_kc;

  remove_mod_mask(MOD_MASK_CTRL);
  remove_mod_mask(MOD_MASK_ALT);
  remove_mod_mask(MOD_MASK_GUI);
  if (mods & MOD_MASK_SHIFT && !is_shifted_kc(remapped_kc)) {
    remove_mod_mask(MOD_MASK_SHIFT);
  }
  
  unregister_code16(kc);
  register_code16(remapped_kc);
  set_mods(mods);
  return remapped_kc;
}

uint16_t remap_release(void) {
  if (remapped_kc == KC_NO) return KC_NO;
  unregister_code16(remapped_kc);
  remapped_kc = KC_NO;
  return remapped_kc;
}

uint16_t process_remap(uint16_t kc, keyrecord_t* record) {
  // Continue default handling if this is a tap-hold key being held.
  if ((IS_QK_MOD_TAP(kc) || IS_QK_LAYER_TAP(kc)) && record->tap.count == 0) return KC_NO;
  if (kc < QK_USER || kc > QK_USER_MAX) return KC_NO;

  const bool pressed = record->event.pressed;
  if (pressed) return remap_press(kc, record);
  return remap_release();
}

