#include QMK_KEYBOARD_H
#include "keymap.h"
#include "src/core.h"
#include "src/rgb.h"
#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

typedef struct {
  bool enabled : 1;
  bool ctl : 1;
  bool alt : 1;
  bool gui : 1;
  bool any : 1;
} TabbingState;

typedef struct {
  uint8_t state;
  uint16_t time;
  uint16_t timer;
  uint16_t last_timer;
  bool enabled : 1;
  bool running : 1;
  bool exec : 1;
  bool caps : 1;
} StatsState;

typedef struct {
  bool active : 1;
  int taps;
  uint16_t timer;
} HypeState;

typedef union {
  struct {
    uint8_t layer;
    bool pressed : 1;
    TabbingState tabbing;
    StatsState stats;
    uint16_t snap_tap[2];
    HypeState hype;
  };
} State;

State state = {
  .layer = BASE,
  .pressed = false,
  .tabbing = {false, false, false, false, false},
  .stats = {0, 250, 0, 0, false, false, false, false},
  .snap_tap = {KC_NO, KC_NO},
  .hype = {false, 0, 0}
};

void handle_tabbing(uint16_t kc) {
  if (state.layer == GAME) return;

  if (kc == KC_LCTL || kc == KC_RCTL) state.tabbing.ctl = state.pressed;
  if (kc == KC_LGUI || kc == KC_RGUI || (kc == KC_LCTL && OS == OS_MAC)) state.tabbing.gui = state.pressed;
  if (kc == KC_LALT || kc == KC_RALT || (kc == KC_LGUI && OS == OS_MAC)) state.tabbing.alt = state.pressed;

  state.tabbing.any = state.tabbing.ctl || state.tabbing.gui || state.tabbing.alt;

  if (state.pressed) {
    if (kc != KC_TAB) return;
    if (state.tabbing.enabled) return;
    if (!state.tabbing.any) return;
    layer_move(LOWER);
    state.tabbing.enabled = true;
  } else {
    if (!state.tabbing.enabled) return;
    if (state.tabbing.any) return;
    layer_move(BASE);
    state.tabbing.enabled = false;
  }
}

bool is_core_kc(uint16_t kc) {
  switch (kc) {
    case LT_L:
    case LT_R:
    case LT_C:
    case QK_BOOT:
      return true;
  }
  return false;
}

bool handle_stats(uint16_t kc) {
  if (kc != CK_STATS) return false;
  state.stats.enabled = state.pressed;
  if (!state.stats.enabled) return false;
  state.stats.exec = true;

  state.stats.last_timer = timer_elapsed(state.stats.timer);
  state.stats.timer = timer_read();
  state.stats.state = 0;

  if (state.stats.last_timer < state.stats.time) state.stats.state = 1;
  return true;
}

void process_stats(void) {
  if (!state.stats.exec) return;
  if (!state.stats.enabled) {
    if (!state.stats.running) return;
    state.stats.exec = false;
    unregister_code16(KC_TAB);
    unregister_code16(KC_CAPS);
    if (state.stats.caps) {
      tap_code16(KC_CAPS);
      state.stats.caps = false;
    }
    state.stats.running = false;
  } else {
    if (state.stats.running) return;
    if (state.stats.state == 0) {
      register_code16(KC_TAB);
      state.stats.caps = false;
    }
    if (state.stats.state == 1) {
      register_code16(KC_CAPS);
      state.stats.caps = true;
    }
    state.stats.running = true;
  }
}

bool handle_snaptap(uint16_t kc) {
  if (state.layer != GAME) return false;
  if (kc != KC_A && kc != KC_D) return false;

  uint16_t other_key = (kc == KC_A) ? KC_D : KC_A;

  if (state.pressed) {
    if (state.snap_tap[0] != kc) {
      if (state.snap_tap[0] != KC_NO) unregister_code16(state.snap_tap[0]);
      state.snap_tap[1] = state.snap_tap[0];
      state.snap_tap[0] = kc;
      register_code16(kc);
    }
  } else {
    if (kc == state.snap_tap[0]) {
      unregister_code16(kc);
      if (state.snap_tap[1] == other_key) register_code16(other_key);
      state.snap_tap[0] = state.snap_tap[1];
      state.snap_tap[1] = KC_NO;
    } else if (kc == state.snap_tap[1]) {
      state.snap_tap[1] = KC_NO;
    }
  }
  return true;
}

bool handle_hype(uint16_t kc) {
  if (kc != CK_HYPE) return false;
  state.hype.timer = timer_read();
  state.hype.active = state.pressed;
  if (state.hype.active) {
    state.hype.taps++;
  } else {
    unregister_code16(KC_LSFT);
  }
  return true;
}

void process_hype(void) {
  uint16_t elapsed = timer_elapsed(state.hype.timer);
  if (!state.hype.active && elapsed > TAPPING_TERM_SLOW) {
    if (state.hype.taps > 0) state.hype.taps--;
  }
  if (state.hype.active) {
    if (state.hype.taps == 2) {
      tap_code16(HYPR(KC_M));
      state.hype.taps = 0;
    } else {
      register_code16(KC_LSFT);
    }
  }
}


// QMK hooks

void keyboard_post_init_user(void) {
  #ifdef CONSOLE_ENABLE
    debug_enable = true;
  #endif
  init_rgb();
}

void matrix_scan_user(void) {
  process_stats();
  process_hype();
}

bool process_record_user(uint16_t kc, keyrecord_t *rec) {
  state.layer = get_highest_layer(layer_state);
  state.pressed = rec->event.pressed;

  if (is_core_kc(kc)) return true;
  bool can_continue = true;
  if (handle_core(kc, rec)) can_continue = false;
  kc = get_current_keycode();

  if (handle_snaptap(kc)) return false;
  if (handle_stats(kc)) return false;
  if (handle_hype(kc)) return false;
  if (handle_rgb(kc, rec)) return false;

  handle_tabbing(kc);

  return can_continue;
}

// Faster tapping term for space layer keys
uint16_t get_tapping_term(uint16_t kc, keyrecord_t *rec) {
  switch (kc) {
    case LT_L:
    case LT_R:
    case LT_C:
      return TAPPING_TERM;
    default:
      return TAPPING_TERM_SLOW;
  }
}