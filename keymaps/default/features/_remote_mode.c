#include "_remote_mode.h"
#include "../_custom_keys.h"

bool REMOTE_ENABLED = false;

void process_remote_mode(uint16_t kc, keyrecord_t* record) {
  if (kc != CK_MODE) return;
  if (!record->event.pressed) return;
  REMOTE_ENABLED = !REMOTE_ENABLED;
}

uint16_t process_remote_press(uint16_t kc, keyrecord_t* record) {
  uint16_t os_key_meta = KC_LGUI;
  uint16_t os_key_alt = KC_LALT;

  bool pressed = record->event.pressed;

  if (REMOTE_ENABLED) {
    os_key_meta = KC_F18;
    os_key_alt = KC_F19;
  }

  if (kc == CK_META) { (pressed) ? register_code16(os_key_meta) : unregister_code16(os_key_meta) ; return os_key_meta; }
  if (kc == CK_ALT) { (pressed) ? register_code16(os_key_alt) : unregister_code16(os_key_alt) ; return os_key_alt; }

  return KC_NO;
}
