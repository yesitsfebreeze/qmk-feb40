#include QMK_KEYBOARD_H
#include "src/process.h"
#include "keymap.h"
#include "src/core.h"
#include "src/rgb.h"

#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
  debug_enable = true;
#endif
  init_rgb();
}

bool is_core_kc(uint16_t kc) {
  if (kc == LT_L) return true;
  if (kc == LT_R) return true;
  if (kc == LT_C) return true;
  if (kc == QK_BOOT) return true;
  
  return false;
}

TabState tab_state = {};

void arrow_remap(uint16_t kc, keyrecord_t *rec) {
  if (!tab_state.enabled) return;
  
  static const uint16_t remap_table[4][2] = {
    {KC_W, KC_UP},
    {KC_A, KC_LEFT},
    {KC_S, KC_DOWN},
    {KC_D, KC_RIGHT}
  };

  bool pressed = rec->event.pressed;
  for (int i = 0; i < 4; i++) {
    if (kc == remap_table[i][0]) {
      if (pressed) {
        unregister_code16(remap_table[i][0]);
        register_code16(remap_table[i][1]);
      } else {
        unregister_code16(remap_table[i][1]);
        register_code16(remap_table[i][0]);
      }
      break;
    }
  }
}

int last_os = -1;
void handle_tabbing(uint16_t kc, keyrecord_t *rec) {
  bool pressed = rec->event.pressed;
  
  if (kc == KC_LCTL) tab_state.ctl = pressed;
  if (kc == KC_LGUI) tab_state.gui = pressed;
  if (kc == KC_LALT) tab_state.alt = pressed;

  if (last_os != OS) {
    last_os = OS;
    tab_state.enabled = false;
    tab_state.ctl = false;
    tab_state.gui = false;
    tab_state.alt = false;
    tab_state.any = false;
  }

  tab_state.any = tab_state.ctl || tab_state.gui || tab_state.alt;

  if (OS == OS_MAC && !pressed && !tab_state.any) {
    if (kc == KC_LCTL || kc == KC_LGUI || kc == KC_LALT) {
     unregister_code16(KC_LCTL);
     unregister_code16(KC_LGUI);
     unregister_code16(KC_LALT);
    }
  }

  if (pressed) {
    if (!tab_state.any) return;
    if (tab_state.enabled) {
      arrow_remap(kc, rec);
       return;
    }
    if (kc != KC_TAB) return;
    tab_state.enabled = true;
    if (OS == OS_MAC) {
      if (tab_state.gui) {
        unregister_code16(KC_LGUI);
        register_code16(KC_LCTL);
      }

      if (tab_state.alt) {
        unregister_code16(KC_LALT);
        register_code16(KC_LGUI);
      }
    }
  } else {
    if (!tab_state.enabled) return;
    if (tab_state.any) return;
    tab_state.enabled = false;
  }
}

void handle_capslock(uint16_t kc, keyrecord_t *rec) {
  if (kc != KC_CAPS) return;
  bool pressed = rec->event.pressed;
  if (pressed) return;
  tap_code16(KC_CAPS);
}

uint16_t stats_time = 250;
uint16_t stats_timer;
uint16_t last_stats_timer;
uint16_t stats_state    = 0;
bool     stats_enabled  = false;
bool     stats_running  = false;
bool     stats_can_exec = false;
bool     stats_is_caps  = false;

bool handle_stats(uint16_t kc, keyrecord_t *rec) {
  if (kc != CK_STATS) return false;
  stats_enabled = rec->event.pressed;
  if (!stats_enabled) return false;
  stats_can_exec = true;

  last_stats_timer = timer_elapsed(stats_timer);
  stats_timer      = timer_read();
  stats_state      = 0;

  if (last_stats_timer < stats_time) stats_state = 1;
  return true;
}

void process_stats(void) {
  if (!stats_can_exec) return;
  if (!stats_enabled) {
    if (!stats_running) return;
    stats_can_exec = false;
    unregister_code16(KC_TAB);
    unregister_code16(KC_CAPS);
    if (stats_is_caps) {
      tap_code16(KC_CAPS);
      stats_is_caps = false;
    }
    stats_running = false;
  } else {
    if (stats_running) return;
    if (stats_state == 0) {
      register_code16(KC_TAB);
      stats_is_caps = false;
    }
    if (stats_state == 1) {
      register_code16(KC_CAPS);
      stats_is_caps = true;
    }
    stats_running = true;
  }
}

bool hyper = false;
int hyper_taps = 0;
uint16_t hype_timer;
bool handle_hype(uint16_t kc, keyrecord_t *rec) {
  if (kc != CK_HYPE) return false;
  hype_timer = timer_read();
  hyper = rec->event.pressed;
  if (hyper) hyper_taps++;
  return true;
}

void process_hype(void) {
  uint16_t elapsed = timer_elapsed(hype_timer);
  if (!hyper && elapsed > TAPPING_TERM_SLOW) {
    if (hyper_taps > 0) hyper_taps--;
  }
  if (hyper) {
    if (hyper_taps == 2) {
      tap_code16(HYPR(KC_SPACE));
      hyper_taps = 0;
    } else {
      register_code16(KC_LSFT);  
    }
  } else {
    unregister_code16(KC_LSFT);
  }
}

void matrix_scan_user(void) {
  process_stats();
  process_hype();
}

bool process_record_user(uint16_t kc, keyrecord_t *rec) {
  if (is_core_kc(kc)) return true;
  bool can_continue = true;
  if (handle_core(kc, rec)) can_continue = false;
  kc = get_current_keycode();

  if (handle_rgb(kc, rec)) return false;
  if (handle_stats(kc, rec)) return false;
  if (handle_hype(kc, rec)) return false;

  handle_tabbing(kc, rec);
  handle_capslock(kc, rec);

  return can_continue;
}

// faster tapping term for space layer keys
uint16_t get_tapping_term(uint16_t kc, keyrecord_t *rec) {
  if (kc == LT_L) return TAPPING_TERM;
  if (kc == LT_R) return TAPPING_TERM;
  if (kc == LT_C) return TAPPING_TERM;
  return TAPPING_TERM_SLOW;
}
