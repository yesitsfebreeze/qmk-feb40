#include QMK_KEYBOARD_H
#include "_layers.h"
#include "features/_remap.h"
#include "features/_rgb.h"

enum {
    RM_ESC,
    RM_ENT_BSPC,
    RM_QUOT_SQUOT,
    RM_UNDS,
    RM_EQUAL,
    RM_LPAR_LCURL,
    RM_RPAR_RCURL,
    RM_LBRAK_LABR,
    RM_RBRAK_RABR,
    RM_COMMA_EXLM,
    RM_DOT_QUES,
    RM_SLASH_BACKSLASH,
    RM_PLS_MNS,
    RM_DOT_COMMA,
    RM_Q,
    RM_HOME,
    RM_END,
};

uint16_t remap_key_list[][16] = {
    [RM_ESC] = {
        REMAP(KC_ESC),
        REMAP_MOD(KC_TAB, MOD_MASK_ALT),
        REMAP_MOD(KC_GRV, MOD_MASK_SHIFT),
    },
    [RM_ENT_BSPC] = {
        REMAP(KC_ENT),
        REMAP_MOD(KC_BSPC, MOD_MASK_SHIFT),
    },
    [RM_QUOT_SQUOT] = {
        REMAP(KC_DQUO),
        REMAP_MOD(KC_QUOTE, MOD_MASK_SHIFT),
    },
    [RM_UNDS] = {
        REMAP(KC_UNDS),
        REMAP_MOD(KC_UNDS, MOD_MASK_SHIFT),
    },
    [RM_EQUAL] = {
        REMAP(KC_EQUAL),
        REMAP_MOD(KC_EQUAL, MOD_MASK_SHIFT),
    },
    [RM_LPAR_LCURL] = {
        REMAP(KC_LPRN),
        REMAP_MOD(KC_LCBR, MOD_MASK_SHIFT),
    },
    [RM_RPAR_RCURL] = {
        REMAP(KC_RPRN),
        REMAP_MOD(KC_RCBR, MOD_MASK_SHIFT),
    },
    [RM_LBRAK_LABR] = {
        REMAP(KC_LBRC),
        REMAP_MOD(KC_LABK, MOD_MASK_SHIFT),
    },
    [RM_RBRAK_RABR] = {
        REMAP(KC_RBRC),
        REMAP_MOD(KC_RABK, MOD_MASK_SHIFT),
    },
    [RM_COMMA_EXLM] = {
        REMAP(KC_COMMA),
        REMAP_MOD(KC_EXLM, MOD_MASK_SHIFT),
    },
    [RM_DOT_QUES] = {
        REMAP(KC_DOT),
        REMAP_MOD(KC_QUES, MOD_MASK_SHIFT),
    },
    [RM_SLASH_BACKSLASH] = {
        REMAP(KC_SLASH),
        REMAP_MOD(KC_BACKSLASH, MOD_MASK_SHIFT),
    },
    [RM_PLS_MNS] = {
        REMAP(KC_PPLS),
        REMAP_MOD(KC_PMNS, MOD_MASK_SHIFT),
    },
    [RM_DOT_COMMA] = {
        REMAP(KC_DOT),
        REMAP_MOD(KC_COMMA, MOD_MASK_SHIFT),
    },
    [RM_Q] = {
        REMAP(KC_Q),
        REMAP_MOD(A(KC_F4), MOD_MASK_CTRL),
    },
    [RM_HOME] = {
        REMAP(KC_HOME),
        REMAP_MOD(KC_PGUP, MOD_MASK_CTRL),
    },
    [RM_END] = {
        REMAP(KC_END),
        REMAP_MOD(KC_PGDN, MOD_MASK_CTRL),
    },
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        RM(RM_ESC), RM(RM_Q), KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, RM(RM_EQUAL),
        KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, RM(RM_ENT_BSPC),
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, RM(RM_COMMA_EXLM), RM(RM_DOT_QUES), KC_DEL,
        KC_LCTL, KC_LGUI, KC_LALT, LT(LOWER, KC_SPC), LT(RAISE, KC_SPC), RM(RM_SLASH_BACKSLASH), RM(RM_PLS_MNS), RM(RM_UNDS)
    ),
    [LOWER] = LAYOUT(
        KC_TRNS, RM(RM_HOME), KC_UP, RM(RM_END), RM(RM_LBRAK_LABR), RM(RM_RBRAK_RABR), KC_PMNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT, RM(RM_LPAR_LCURL), RM(RM_RPAR_RCURL), KC_PPLS, KC_NO, KC_SCLN, RM(RM_QUOT_SQUOT), KC_BSPC,
        KC_TRNS, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), KC_PGDN, KC_PGUP, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LT(COMBO, KC_SPC), KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [RAISE] = LAYOUT(
        KC_0, KC_1, KC_2, KC_3, KC_PMNS, KC_PSLS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,  KC_EQUAL,
        RM(RM_DOT_COMMA), KC_4, KC_5, KC_6, KC_PPLS, KC_PAST, KC_NO, KC_NO, KC_SCLN, RM(RM_QUOT_SQUOT), KC_BSPC,
        KC_LSFT, KC_7, KC_8, KC_9, KC_PERC, KC_AMPR, KC_PIPE, KC_TILDE, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, LT(COMBO, KC_SPC), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [COMBO] = LAYOUT(
        KC_NO, KC_F1, KC_F2, KC_F3, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT,
        KC_RGB_SWITCH, KC_F4, KC_F5, KC_F6, KC_NO, LCAG(KC_1), LCAG(KC_2), LCAG(KC_3), LCAG(KC_4), LCAG(KC_5), KC_ENTER,
        KC_LSFT, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
};