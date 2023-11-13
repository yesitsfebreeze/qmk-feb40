#pragma once
#include QMK_KEYBOARD_H

enum LAYERS {
  BASE,
  LOWER,
  RAISE,  
  COMBO,
};

extern int OS;
enum OS_TYPES {
  OS_WINDOWS,
  OS_REMOTE,
};

typedef union {
  struct {
    bool CTRL;
    bool ALT;
    bool GUI;
    bool SHIFT;
  };
} Mods;

extern bool is_in_alt_tab;

extern uint16_t __custom_kc;

#define KC_MB1      KC_MS_BTN1
#define KC_MB2      KC_MS_BTN2
#define KC_MB3      KC_MS_BTN3 // MWHEEL_CLICK

#define L_LWR       LT(LOWER, KC_SPC)
#define L_RSE       LT(RAISE, KC_SPC)
#define L_CMB       LT(COMBO, KC_SPC)

#define CK_OS       QK_KB_0
#define CK_RGB      QK_KB_1
#define CK_RGBI     QK_KB_2
