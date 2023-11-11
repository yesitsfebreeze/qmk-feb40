#include "_macros.h"

// https://github.com/qmk/qmk_firmware/blob/master/docs/feature_send_string.md
bool process_macros(uint16_t kc, keyrecord_t* record) {
  
  for (size_t i = 0; i < macro_list_size; ++i) {
    if (macro_list[i].key == kc) {
      char *str = macro_list[i].value;
      if (str != NULL) {
        if (record->event.pressed) send_string(str);
      }
      return true;
    }
  }

  return false;
}
