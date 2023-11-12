#include QMK_KEYBOARD_H
#include "_layers.h"
#include "_custom_keys.h"
#include "features/_util.h"
#include "features/_macros.h"
#include "features/_remote_mode.h"
#include "features/_remap.h"
#include "features/_rgb.h"

uint16_t remap_list[][16] = {
  REMAP(CK_ESC, KC_ESC, REMAP_MOD(KC_TAB, MOD_MASK_ALT), REMAP_MOD(KC_GRV, MOD_MASK_SHIFT))
  REMAP(CK_ENT, KC_ENT, REMAP_MOD(KC_BSPC, MOD_MASK_SHIFT))
  REMAP(CK_QUOT, KC_DQUO, REMAP_MOD(KC_QUOTE, MOD_MASK_SHIFT))
  REMAP(CK_UNDS, KC_UNDS, REMAP_MOD(KC_UNDS, MOD_MASK_SHIFT))
  REMAP(CK_EQL, KC_EQUAL, REMAP_MOD(KC_EQUAL, MOD_MASK_SHIFT))
  REMAP(CK_LBRC, KC_LBRC, REMAP_MOD(KC_LABK, MOD_MASK_SHIFT))
  REMAP(CK_RBRC, KC_RBRC, REMAP_MOD(KC_RABK, MOD_MASK_SHIFT))
  REMAP(CK_LPRN, KC_LPRN, REMAP_MOD(KC_LCBR, MOD_MASK_SHIFT))
  REMAP(CK_RPRN, KC_RPRN, REMAP_MOD(KC_RCBR, MOD_MASK_SHIFT))
  REMAP(CK_AMPR, KC_AMPR, REMAP_MOD(KC_AMPR, MOD_MASK_SHIFT))
  REMAP(CK_PIPE, KC_PIPE, REMAP_MOD(KC_PIPE, MOD_MASK_SHIFT))
  REMAP(CK_COM, KC_COMMA, REMAP_MOD(KC_EXLM, MOD_MASK_SHIFT))
  REMAP(CK_DOT, KC_DOT, REMAP_MOD(KC_QUES, MOD_MASK_SHIFT))
  REMAP(CK_SLASH, KC_SLASH, REMAP_MOD(KC_BACKSLASH, MOD_MASK_SHIFT))
  REMAP(CK_PLMN, KC_PMNS, REMAP_MOD(KC_PPLS, MOD_MASK_SHIFT))
  REMAP(CK_HOME, KC_HOME, REMAP_MOD(KC_PGUP, MOD_MASK_CTRL))
  REMAP(CK_END, KC_END, REMAP_MOD(KC_PGDN, MOD_MASK_CTRL))
  REMAP(CK_9, KC_9, REMAP_MOD(KC_TILDE, MOD_MASK_SHIFT))
};

int remap_list_size = sizeof(remap_list)/sizeof(remap_list[0]);

MacroEntry macro_list[] = {
  {MA_OR, "||"},
  {MA_AND, "&&"},
  {MA_PTR, "->"},
};

int macro_list_size = sizeof(macro_list)/sizeof(macro_list[0]);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE]  = LAYOUT(
    CK_ESC,   KC_Q,     KC_W,     KC_E,     KC_R,    KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     CK_EQL,
    KC_TAB,   KC_A,     KC_S,     KC_D,     KC_F,    KC_G,      KC_H,     KC_J,     KC_K,     KC_L,               CK_ENT,
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,      KC_N,     KC_M,               CK_COM,   CK_DOT,   KC_DEL,
    KC_LCTL,  CK_META,  CK_ALT,             L_LOWER,            L_RAISE,                      CK_SLASH, CK_PLMN,  CK_UNDS
  ),
  [LOWER] = LAYOUT(
    _______,  CK_HOME,  KC_UP,    CK_END,   CK_LBRC,  CK_RBRC,  _______,  _______,  MA_AND,   MA_OR,    MA_PTR,   _______,
    _______,  KC_LEFT,  KC_DOWN,  KC_RIGHT, CK_LPRN,  CK_RPRN,  _______,  _______,  KC_SCLN,  CK_QUOT,            KC_BSPC,
    _______,  C(KC_Z),  C(KC_X),  C(KC_C),  C(KC_V),  _______,  _______,  _______,            _______,  _______,  _______,
    _______,  _______,  _______,            _______,            L_COMBO,                      _______,  _______,  _______
  ),
  [RAISE] = LAYOUT(
    _______,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     CK_9,     KC_0,     _______,
    KC_0,     KC_4,     KC_5,     KC_6,     _______,  _______,  _______,  _______,  KC_SCLN,  CK_QUOT,            KC_BSPC,
    _______,  KC_7,     KC_8,     CK_9,     _______,  _______,  _______,  _______,            _______,  _______,  _______,
    _______,  _______,  _______,            L_COMBO,            _______,                      _______,  _______,  _______
  ),
  [COMBO] = LAYOUT(
    CK_MODE,  KC_MB1,   KC_MS_U,  KC_MB2,   KC_F1,    KC_F2,    KC_F3,    _______,  _______,  _______,  _______,  CK_TEST,
    CK_RGB,   KC_MS_L,  KC_MS_D,  KC_MS_R,  KC_F4,    KC_F5,    KC_F6,    _______,  _______,  _______,            KC_ENTER,
    _______,  KC_WH_U,  KC_MB3,   KC_WH_D,  KC_F7,    KC_F8,    KC_F9,    KC_F10,             KC_F11,   KC_F12,   _______,
    _______,  _______,  _______,            _______,            _______,                      _______,  _______,  QK_BOOT
  ),
};
