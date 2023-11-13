#include QMK_KEYBOARD_H
#include "src/process.h"
#include "src/core.h"
#include "src/rgb.h"


// --------------------------------------------------------
// HELPERS
// --------------------------------------------------------


#ifdef CONSOLE_ENABLE
  #include "print.h"
#endif

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

void clear_full_mods(void) {
  clear_mods();
  del_weak_mods(MOD_MASK_CSAG);
  #ifndef NO_ACTION_ONESHOT
    del_oneshot_mods(MOD_MASK_CSAG);
  #endif
  del_mods(MOD_MASK_CSAG);
}

Mods get_mod_state(uint8_t mods) {
  return (Mods) {
    .CTRL = mods & MOD_MASK_CTRL,
    .ALT = mods & MOD_MASK_ALT,
    .GUI = mods & MOD_MASK_GUI,
    .SHIFT = mods & MOD_MASK_SHIFT,
  };
}

void handle_os_toggle(uint16_t kc, keyrecord_t *rec) {
  if (kc != CK_OS) return;
  if (!rec->event.pressed) return;
  OS++;
  if (OS > sizeof(enum OS_TYPES)) OS = 0;
}

void handle_alt_tab(uint16_t kc, keyrecord_t *rec) {
  uint8_t mods = get_mods();

  if (rec->event.pressed) {
    if (kc == KC_TAB) {
      bool alt_down = (mods & MOD_MASK_ALT);
      bool gui_down = (mods & MOD_MASK_GUI);
      bool ctrl_down = (mods & MOD_MASK_CTRL);

      if (alt_down || gui_down || ctrl_down) {
        layer_move(LOWER);
        is_in_alt_tab = true;
      }
    }
  } else if (is_in_alt_tab) {
    bool alt_up = kc == KC_LALT || kc == KC_RALT;
    bool gui_up = kc == KC_LGUI || kc == KC_RGUI;
    bool ctrl_up = kc == KC_LCTL || kc == KC_LCTL;

    if (alt_up || gui_up || ctrl_up) {
      layer_move(BASE);
      is_in_alt_tab = false;
    }
  }
} 

bool process_macro(uint16_t kc, uint16_t custom, uint8_t mods, keyrecord_t *rec) {
  if (!rec->event.pressed) return false;

  char* macro = process_macros(custom);
  if (!char_empty(macro)) {
      unregister_code16(kc);
      clear_full_mods();
      send_string(macro);
      set_mods(mods);
      return true;
  }

  return false;
}

bool process_custom_kc_press(uint16_t kc, uint16_t custom, uint8_t mods, keyrecord_t *rec) {
  if ((IS_QK_MOD_TAP(kc) || IS_QK_LAYER_TAP(kc)) && rec->tap.count == 0) return false;
  if (custom == KC_NO) return false;
  
  clear_full_mods();
  unregister_code16(kc);
  register_code16(custom);
  __custom_kc = custom;
  set_mods(mods);
  return true;
}

bool process_custom_kc_release(void) {
  if (__custom_kc == KC_NO) return false;
  unregister_code16(__custom_kc);
  __custom_kc = KC_NO;
  return true;
}


// --------------------------------------------------------
// IMPLEMENTATION
// --------------------------------------------------------


void keyboard_post_init_user(void) {
  #ifdef CONSOLE_ENABLE
    debug_enable=true;
  #endif
  init_rgb();
}

bool process_record_custom(uint16_t kc, keyrecord_t *rec) {
  bool pressed = rec->event.pressed;
  uint16_t custom = kc;
  uint8_t mods = get_full_mods();
  Mods mod_state = get_mod_state(mods);

  if (handle_rgb(kc, rec)) return true;
  handle_os_toggle(kc, rec);
  handle_alt_tab(kc, rec);  

  uint16_t remapped = process_remaps(kc, mod_state);
  if (remapped != 0) custom = remapped;
  uint16_t os = process_os(custom, mod_state, OS);
  if (os != 0) custom = os;
  // after remap and os keys, check if we ended up with a macro
  if (process_macro(kc, custom, mods, rec)) return true;

  // handle custom keycodes if any are present
  if ((remapped != 0 || os != 0) && pressed) {
    return process_custom_kc_press(kc, custom, mods, rec);
  }
  
  return process_custom_kc_release();
}

bool process_record_user(uint16_t kc, keyrecord_t *rec) {
  if (process_record_custom(kc, rec)) {
    uprintf("CUSTOM SHIFT(%d) CTRL(%d) ALT(%d) GUI(%d)\n", 
      (get_mods() & MOD_MASK_SHIFT) ? 1 : 0,
      (get_mods() & MOD_MASK_CTRL) ? 1 : 0,
      (get_mods() & MOD_MASK_ALT) ? 1 : 0,
      (get_mods() & MOD_MASK_GUI) ? 1 : 0
    );
    return false;
  }
  uprintf("DEFAULT SHIFT(%d) CTRL(%d) ALT(%d) GUI(%d)\n",
    (get_mods() & MOD_MASK_SHIFT) ? 1 : 0,
    (get_mods() & MOD_MASK_CTRL) ? 1 : 0,
    (get_mods() & MOD_MASK_ALT) ? 1 : 0,
    (get_mods() & MOD_MASK_GUI) ? 1 : 0
  );
  return true;
}

// faster tapping term for space layer keys
uint16_t get_tapping_term(uint16_t kc, keyrecord_t *rec) {
  switch (kc) {
    case L_RSE:
      return TAPPING_TERM_FAST;
    case L_LWR: 
      return TAPPING_TERM_FAST;
    case L_CMB: 
      return TAPPING_TERM_FAST;
    default:
      return TAPPING_TERM;
  }
}
