#include QMK_KEYBOARD_H
#include "keymap.h"
#include "src/core.h"
#include "src/rgb.h"

typedef union {
  struct {
    bool enabled;
    bool ctl;
    bool alt;
    bool gui;
    bool any;
  };
} TabbingState;

TabbingState tabbing_state = {};

void handle_tabbing(uint16_t kc, keyrecord_t *rec) {
  bool pressed = rec->event.pressed;

  if (kc == KC_LCTL || kc == KC_RCTL) tabbing_state.ctl = pressed;
  if (kc == KC_LGUI || kc == KC_RGUI) tabbing_state.gui = pressed;
  if (kc == KC_LALT || kc == KC_LALT) tabbing_state.alt = pressed;    

  tabbing_state.any = tabbing_state.ctl || tabbing_state.gui || tabbing_state.alt;

  if (pressed) {
    if (kc != KC_TAB) return;
    if (tabbing_state.enabled) return;
    if (!tabbing_state.any) return;
    if (OS == OS_MAC) {
      if (tabbing_state.gui) {
        unregister_code16(KC_LGUI);
        register_code16(KC_LCTL);
      }
      if (tabbing_state.alt) {
        unregister_code16(KC_LALT);
        register_code16(KC_LGUI);
      }
    }
    layer_move(LOWER);
    tabbing_state.enabled = true;
  } else {
    if (!tabbing_state.enabled) return;
    if (tabbing_state.any) return;
    if (OS == OS_MAC) {
      unregister_code16(KC_LCTL);
      unregister_code16(KC_LGUI);
      unregister_code16(KC_LALT);
    }
    layer_move(BASE);
    tabbing_state.enabled = false;
    
  }
}

void keyboard_post_init_user(void) {
  #ifdef CONSOLE_ENABLE
    debug_enable=true;
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

void matrix_scan_user(void) {
  process_stats();
}

bool handle_hype(uint16_t kc, keyrecord_t *rec) {
  if (kc != KC_HYPE) return false;
  if (!rec->event.pressed) return false;
  if (rec->tap.count) {
    tap_code16(HYPR(KC_SPACE));
  } else {
    tap_code16(KC_LSFT);
  }
  return true;
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
  return TAPPING_TERM;
}
