#include QMK_KEYBOARD_H
#include "features/_mods.h"
#include "features/_tap_dance.h"
#include "features/_remap.h"

enum TAP_DANCE_KEYS {
    TD_Q_AT,
    TD_W_DQUOTE,
    TD_S_SQUOTE,
    TD_LBRKTS,
    TD_RBRKTS,
    TD_EQL,
    TD_A_ASTR,
    TD_CLN,
    TD_0,
    TD_1,
    TD_2,
    TD_3,
    TD_4,
    TD_5,
    TD_6,
    TD_7,
    TD_8,
    TD_9,
    TD_Z_CTRLZ,
    TD_X_CTRLX,
    TD_C_CTRLC,
    TD_V_CTRLV,
};

_tap_dance _tap_dances[] = {
  TAP_DANCE_1(TD_Q_AT, KC_Q, KC_AT),
  TAP_DANCE_1(TD_W_DQUOTE, KC_W, KC_DQUO),
  TAP_DANCE_1(TD_S_SQUOTE, KC_S, KC_QUOTE),
  TAP_DANCE_1(TD_LBRKTS, KC_F17, KC_LBRC),
  TAP_DANCE_1(TD_RBRKTS, KC_F18, KC_RBRC),
  TAP_DANCE_1(TD_EQL, KC_F13, KC_F15),
  TAP_DANCE_1(TD_A_ASTR, KC_A, KC_PAST),
  TAP_DANCE_1(TD_CLN, KC_SCLN, KC_COLN),
  TAP_DANCE_1(TD_0, KC_0, KC_EXCLAIM),
  TAP_DANCE_1(TD_1, KC_1, KC_AT),
  TAP_DANCE_1(TD_2, KC_2, KC_HASH),
  TAP_DANCE_1(TD_3, KC_3, KC_DOLLAR),
  TAP_DANCE_1(TD_4, KC_4, KC_PERCENT),
  TAP_DANCE_1(TD_5, KC_5, KC_CIRCUMFLEX),
  TAP_DANCE_1(TD_6, KC_6, KC_AMPERSAND),
  TAP_DANCE_1(TD_7, KC_7, KC_ASTERISK),
  TAP_DANCE_1(TD_8, KC_8, KC_LEFT_PAREN),
  TAP_DANCE_1(TD_9, KC_9, KC_RIGHT_PAREN),
  TAP_HOLD(TD_Z_CTRLZ, KC_Z, LCTL(KC_Z)),
  TAP_HOLD(TD_X_CTRLX, KC_X, LCTL(KC_X)),
  TAP_HOLD(TD_C_CTRLC, KC_C, KC_O ),
  TAP_HOLD(TD_V_CTRLV, KC_V, LCTL(KC_V)),
};

int _tap_dances_size = sizeof(_tap_dances) / sizeof(_tap_dances[0]);


enum CUSTOM_KEYS {
    CCK_NONE = SAFE_RANGE + 128, // leaves 128 tap dance mappings
    CCK_DEL_INS,
    CKC_PIPE_AMPR,
};


// order is important here, add modded remaps FIRST
_remap_key_t _key_remap_list[] = {
    
    REMAP_MOD(CCK_DEL_INS, KC_INS, MOD_MASK_SHIFT),
    REMAP(CCK_DEL_INS, KC_DEL),
    
    REMAP_MOD(CKC_PIPE_AMPR, KC_B, MOD_MASK_SHIFT),
    REMAP(CKC_PIPE_AMPR, KC_A),

    REMAP_MOD(KC_ENTER, KC_BSPC, MOD_MASK_SHIFT),
    REMAP_MOD(KC_DEL, KC_INS, MOD_MASK_SHIFT),
    REMAP_MOD(KC_F13, KC_QUES, MOD_MASK_SHIFT),
    REMAP(KC_F13, KC_UNDS),
    REMAP_MOD(KC_F14, KC_MINUS, MOD_MASK_SHIFT),
    REMAP(KC_F14, KC_KP_PLUS),
    REMAP_MOD(KC_F15, KC_EQUAL, MOD_MASK_SHIFT),
    REMAP(KC_F15, KC_EQUAL),
    REMAP_MOD(KC_F16, KC_BACKSLASH, MOD_MASK_SHIFT),
    REMAP(KC_F16, KC_SLASH),
    REMAP_MOD(KC_F17, KC_LBRC, MOD_MASK_SHIFT),
    REMAP(KC_F17, KC_LPRN),
    REMAP_MOD(KC_LBRC, KC_LABK, MOD_MASK_SHIFT),
    REMAP(KC_LBRC, KC_LCBR),
    REMAP_MOD(KC_F18, KC_RBRC, MOD_MASK_SHIFT),
    REMAP(KC_F18, KC_RPRN),
    REMAP_MOD(KC_RBRC, KC_RABK, MOD_MASK_SHIFT),
    REMAP(KC_RBRC, KC_RCBR),
    REMAP_MOD(KC_F19, KC_AMPR, MOD_MASK_SHIFT),
    REMAP(KC_F19, KC_PIPE),
    REMAP_MOD(KC_COMMA, KC_MINUS, MOD_MASK_SHIFT),
    REMAP_MOD(KC_DOT, KC_PLUS, MOD_MASK_SHIFT),
};

int _key_remap_list_size = sizeof(_key_remap_list) / sizeof(_key_remap_list[0]);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC, TD(TD_Q_AT), TD(TD_W_DQUOTE), KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, TD(TD_EQL),
        KC_TAB, TD(TD_A_ASTR), TD(TD_S_SQUOTE), KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_ENTER,
        KC_LSFT, TD(TD_Z_CTRLZ), TD(TD_X_CTRLX), TD(TD_C_CTRLC), TD(TD_V_CTRLV), KC_B, KC_N, KC_M, KC_COMMA, KC_DOT, KC_LSFT,
        KC_LCTL, KC_LGUI, KC_LALT, KC_NO, KC_NO, LT(1, KC_SPC), KC_NO, KC_NO, CKC_PIPE_AMPR, CCK_DEL_INS
    ),
    [1]  = LAYOUT(
        TD(TD_0), TD(TD_1), TD(TD_2), TD(TD_3), TD(TD_4), TD(TD_5), TD(TD_6), TD(TD_7), TD(TD_8), TD(TD_9), KC_F14, KC_TRNS,
        KC_TRNS, TD(TD_LBRKTS), KC_F16, TD(TD_RBRKTS), KC_HOME, KC_END, KC_LEFT, KC_DOWN , KC_UP, KC_RIGHT, KC_BSPC,
        KC_TRNS, KC_TRNS, KC_TRNS, TD(TD_CLN), KC_TRNS, KC_PGUP, KC_PGDN, KC_TRNS, KC_TRNS, KC_TRNS, KC_INS,
        QK_BOOTLOADER, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_LSFT, KC_TRNS, KC_NO, KC_TRNS, KC_TRNS
    ),
};


bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (_process_tap_dance(keycode, record)) return false;
    if (_process_remap_release(keycode, record)) return false;
    if (_process_remap(keycode, record)) return false;
    

    return true;
}

void matrix_scan_user(void) {
    _process_tap_dance_execute();
};
