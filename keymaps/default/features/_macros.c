#include "_macros.h"
#include "_util.h"

// https://github.com/qmk/qmk_firmware/blob/master/docs/feature_send_string.md
bool process_macros(uint16_t kc, keyrecord_t* record) {
  
  for (size_t i = 0; i < macro_list_size; ++i) {
    if (macro_list[i].key == kc) {
      char *str = macro_list[i].value;
      if (str != NULL) {
        if (record->event.pressed) {
          uint8_t mods = get_full_mods();
          remove_mod_mask(MOD_MASK_CTRL);
          remove_mod_mask(MOD_MASK_ALT);
          remove_mod_mask(MOD_MASK_GUI);
          remove_mod_mask(MOD_MASK_SHIFT);
          send_string(str);
          set_mods(mods);
        }
      }
      return true;
    }
  }

  return false;
}
