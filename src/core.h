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

#define L_LOWER     LT(LOWER, KC_SPC)
#define L_RAISE     LT(RAISE, KC_SPC)
#define L_COMBO     LT(COMBO, KC_SPC)

// QK_KB_0 - QK_KB_31 and QK_USER_0 - QK_USER_31
#define CK_OS       QK_KB_0
#define CK_RGB      QK_KB_1
#define MA_OR       QK_KB_2
#define MA_AND      QK_KB_3
#define MA_PTR      QK_KB_4
#define CK_SFT_ENT  QK_KB_5
