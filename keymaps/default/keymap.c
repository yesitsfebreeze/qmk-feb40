#include QMK_KEYBOARD_H
#include "src/core.h"
#include "src/process.h"

// QK_KB_2 - QK_KB_31 and QK_USER_0 - QK_USER_31
#define MA_OR    QK_KB_2
#define MA_AND   QK_KB_3
#define MA_PTR   QK_KB_4
#define KC_SENT  QK_KB_5

uint16_t process_remaps(uint16_t kc, Mods mods) {
  if (kc == KC_ESC && mods.ALT) return KC_TAB;
  if (kc == KC_ESC && mods.SHIFT) return KC_GRV;
  if (kc == KC_ENT && mods.SHIFT) return KC_BSPC;
  if (kc == KC_QUOT && mods.SHIFT) return KC_QUOT;
  if (kc == KC_QUOT) return KC_DQUO;
  if (kc == KC_UNDS && mods.SHIFT) return KC_UNDS;
  if (kc == KC_EQL && mods.SHIFT) return KC_EQL;
  if (kc == KC_LBRC && mods.SHIFT) return KC_LABK;
  if (kc == KC_RBRC && mods.SHIFT) return KC_RABK;
  if (kc == KC_LPRN && mods.SHIFT) return KC_LCBR;
  if (kc == KC_RPRN && mods.SHIFT) return KC_RCBR;
  if (kc == KC_AMPR && mods.SHIFT) return MA_AND;
  if (kc == KC_PIPE && mods.SHIFT) return MA_OR;
  if (kc == KC_COMM && mods.SHIFT) return KC_EXLM;
  if (kc == KC_DOT && mods.SHIFT) return KC_QUES;
  if (kc == KC_SLASH && mods.SHIFT) return KC_BACKSLASH;
  if (kc == KC_HOME && mods.CTRL) return KC_PGUP;
  if (kc == KC_END && mods.CTRL) return KC_PGDN;
  if (kc == KC_9 && mods.SHIFT) return KC_TILDE;
  if (kc == KC_MINS && mods.SHIFT) return KC_PPLS;
  if (kc == KC_SENT) return RSFT(KC_ENT);

  return 0;
}

uint16_t process_os(uint16_t kc, Mods mods, int os) {
  if(os == OS_REMOTE) {
    if (kc == KC_LWIN) return KC_F18;
    if (kc == KC_LALT) return KC_F19;
  }

  return 0;
}

char* process_macros(uint16_t kc) {
  if (kc == MA_OR) return "||";
  if (kc == MA_AND) return "&&";
  if (kc == MA_PTR) return "->";

  return "";
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE]  = LAYOUT(
    KC_ESC,   KC_Q,     KC_W,     KC_E,     KC_R,    KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_EQL,
    KC_TAB,   KC_A,     KC_S,     KC_D,     KC_F,    KC_G,      KC_H,     KC_J,     KC_K,     KC_L,               KC_ENT,
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,      KC_N,     KC_M,               KC_COMM,  KC_DOT,   KC_DEL,
    KC_LCTL,  KC_LWIN,  KC_LALT,            L_LOWER,            L_RAISE,                      KC_SLASH, KC_MINS,  KC_UNDS
  ),
  [LOWER] = LAYOUT(
    _______,  KC_HOME,  KC_UP,    KC_END,   KC_LBRC,  KC_RBRC,  _______,  _______,  KC_AMPR,  KC_PIPE,  MA_PTR,   _______,
    _______,  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_LPRN,  KC_RPRN,  _______,  _______,  KC_SCLN,  KC_QUOT,            KC_BSPC,
    _______,  C(KC_Z),  C(KC_X),  C(KC_C),  C(KC_V),  _______,  _______,  _______,            _______,  _______,  _______,
    _______,  _______,  _______,            _______,            L_COMBO,                      _______,  _______,  _______
  ),
  [RAISE] = LAYOUT(
    _______,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     _______,
    KC_0,     KC_4,     KC_5,     KC_6,     _______,  _______,  _______,  _______,  KC_SCLN,  KC_QUOT,            KC_BSPC,
    _______,  KC_7,     KC_8,     KC_9,     _______,  _______,  _______,  _______,            _______,  _______,  _______,
    _______,  _______,  _______,            L_COMBO,            _______,                      _______,  _______,  _______
  ),
  [COMBO] = LAYOUT(
    CK_OS,    KC_MB1,   KC_MS_U,  KC_MB2,   KC_F1,    KC_F2,    KC_F3,    _______,  _______,  _______,  _______,  _______,
    CK_RGB,   KC_MS_L,  KC_MS_D,  KC_MS_R,  KC_F4,    KC_F5,    KC_F6,    _______,  _______,  _______,            KC_SENT,
    _______,  KC_WH_U,  KC_MB3,   KC_WH_D,  KC_F7,    KC_F8,    KC_F9,    KC_F10,             KC_F11,   KC_F12,   _______,
    _______,  _______,  _______,            _______,            _______,                      _______,  _______,  QK_BOOT
  ),
};


