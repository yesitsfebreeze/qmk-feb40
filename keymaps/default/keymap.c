#include QMK_KEYBOARD_H
#include "features/_remap.h"
#define __ NO_LED
#include "print.h"
#ifdef RGB_MATRIX_ENABLE

#define RGB_MATRIX_MAX 41

led_config_t g_led_config = { {
    { 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30 },
    { 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, __, 19 },
    { 18, 17, 16, 15, 14, 13, 12, 11, 10, __, 9,  8  },
    { 7,  6,  5,  4,  __, __, 3,  __, __, 2,  1,  0  }
}, {
    {224,64 }, {204,64 }, {183,64 },                       {135,64 },                       { 84,64 }, { 48,64 }, { 25,64 }, {  3,64 },
    {224,43 }, {204,43 },            {181,43 }, {158,43 }, {137,43 }, {117,43 }, { 97,43 }, { 76,43 }, { 56,43 }, { 36,43 }, {  8,43 },
    {216,21 },            {188,21 }, {168,21 }, {148,21 }, {127,21 }, {107,21 }, { 87,21 }, { 66,21 }, { 46,21 }, { 25,21 }, {  3,21 },
    {224,0  }, {204,0  }, {183,0  }, {163,0  }, {143,0  }, {122,0  }, {102,0  }, { 81,0  }, { 61,0  }, { 41,0  }, { 20,0  }, {  0,0  }
}, {
    4, 4, 4,       4,       4, 4, 4, 4,
    4, 4,    4, 1, 1, 1, 1, 1, 1, 1, 4,
    4,    1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
    4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4
} };

#endif

enum LAYERS {
    BASE,
    LOWER,
    RAISE,
    COMBO
};

enum {
    RM_ESC,
    RM_ENT_BSPC,
    RM_QUOT_SQUOT,
    RM_UNDS,
    RM_EQUAL_DOLLAR,
    RM_LPAR_RPAR,
    RM_LBRAK_RBRAK,
    RM_LBCURL_RCURL,
    RM_LABR_RABR,
    RM_COMMA_QUES,
    RM_DOT_EXLM,
    RM_SLASH_BACKSLASH,
    RM_HASH_AT,
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
    [RM_EQUAL_DOLLAR] = {
        REMAP(KC_EQUAL),
        REMAP_MOD(KC_DLR, MOD_MASK_SHIFT),
    },
    [RM_LPAR_RPAR] = {
        REMAP(KC_LPRN),
        REMAP_MOD(KC_RPRN, MOD_MASK_SHIFT),
    },
    [RM_LBRAK_RBRAK] = {
        REMAP(KC_LBRC),
        REMAP_MOD(KC_RBRC, MOD_MASK_SHIFT),
    },
    [RM_LBCURL_RCURL] = {
        REMAP(KC_LCBR),
        REMAP_MOD(KC_RCBR, MOD_MASK_SHIFT),
    },
    [RM_LABR_RABR] = {
        REMAP(KC_LABK),
        REMAP_MOD(KC_RABK, MOD_MASK_SHIFT),
    },
    [RM_COMMA_QUES] = {
        REMAP(KC_COMMA),
        REMAP_MOD(KC_QUES, MOD_MASK_SHIFT),
    },
    [RM_DOT_EXLM] = {
        REMAP(KC_DOT),
        REMAP_MOD(KC_EXLM, MOD_MASK_SHIFT),
    },
    [RM_SLASH_BACKSLASH] = {
        REMAP(KC_SLASH),
        REMAP_MOD(KC_BACKSLASH, MOD_MASK_SHIFT),
    },
    [RM_HASH_AT] = {
        REMAP(KC_HASH),
        REMAP_MOD(KC_AT, MOD_MASK_SHIFT),
    }

};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        RM(RM_ESC), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, RM(RM_EQUAL_DOLLAR),
        KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, RM(RM_ENT_BSPC),
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, RM(RM_COMMA_QUES), RM(RM_DOT_EXLM), KC_DEL,
        KC_LCTL, KC_LGUI, KC_LALT, LT(LOWER, KC_SPC), LT(RAISE, KC_SPC), RM(RM_UNDS), RM(RM_HASH_AT), RM(RM_SLASH_BACKSLASH)
    ),
    [LOWER] = LAYOUT(
        KC_TRNS, KC_HOME, KC_UP, KC_END, KC_PGUP, KC_NO, KC_NO, KC_NO, RM(RM_LPAR_RPAR), RM(RM_LBCURL_RCURL), RM(RM_LBRAK_RBRAK), RM(RM_LABR_RABR),
        KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT, KC_PGDN, KC_NO, KC_NO, KC_NO, KC_SCLN, RM(RM_QUOT_SQUOT), KC_BSPC,
        KC_TRNS, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), KC_NO, KC_NO, KC_NO, KC_PMNS, KC_PPLS, KC_PAST,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LT(COMBO, KC_SPC), KC_AMPR, KC_PIPE, KC_PSLS
    ),
    [RAISE] = LAYOUT(
        KC_0, KC_1, KC_2, KC_3, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_EQUAL,
        KC_TRNS, KC_4, KC_5, KC_6, KC_NO, KC_NO, KC_NO, KC_NO, KC_SCLN, RM(RM_QUOT_SQUOT), KC_BSPC,
        KC_TRNS, KC_7, KC_8, KC_9, KC_NO, KC_NO, KC_NO, KC_NO, RM(RM_COMMA_QUES), RM(RM_DOT_EXLM), KC_DEL,
        KC_TRNS, KC_TRNS, KC_TRNS, LT(COMBO, KC_SPC), KC_TRNS, KC_NO, KC_NO, KC_NO
    ),
    [COMBO] = LAYOUT(
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT, KC_NO, KC_NO
    ),
};

bool is_in_window_switch = false;
void window_switch(uint16_t keycode, keyrecord_t* record) {
    uint8_t mods = get_mods();

    if (record->event.pressed) {
        if ((mods & MOD_MASK_ALT) && keycode == KC_TAB) {
        layer_move(LOWER);
        is_in_window_switch = true;
        }
    } else if (is_in_window_switch && ( keycode == KC_LALT || keycode == KC_RALT)) {
        layer_move(BASE);
        is_in_window_switch = false;
    }
} 

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    uint16_t remapped_keycode = process_remap(keycode, record); 
    if (remapped_keycode != KC_NO) {
        window_switch(remapped_keycode, record);
        return false;
    }
    
    window_switch(keycode, record);
    return true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(RAISE, KC_SPC):
            return TAPPING_TERM_FAST;
        case LT(LOWER, KC_SPC): 
            return TAPPING_TERM_FAST;
        case LT(COMBO, KC_SPC): 
            return TAPPING_TERM_FAST;
        default:
            return TAPPING_TERM;
    }
}

uint8_t INDICATOR_R = 0;
uint8_t INDICATOR_G = 0;
uint8_t INDICATOR_B = 0;

HSV rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b) {
    HSV hsv;
    float nr = r / 255.0;
    float ng = g / 255.0;
    float nb = b / 255.0;

    float max = fmax(nr, fmax(ng, nb));
    float min = fmin(nr, fmin(ng, nb));

    float d = max - min;

    float hue = 0;
    if (d != 0) {
        if (max == nr) {
            hue = fmodf((ng - nb) / d, 6.0);
        } else if (max == ng) {
            hue = ((nb - nr) / d) + 2.0;
        } else if (max == nb) {
            hue = ((nr - ng) / d) + 4.0;
        }
        hue *= 60;
    }

    float s = (max != 0) ? d / max : 0;

    // Convert HSV values to the range [0, 255]
    hsv.h = (hue * 255) / 360;
    hsv.s = s * 255;
    hsv.v = max * 255;
    return hsv;
}

void set_color(uint8_t r, uint8_t g, uint8_t b) {
    HSV hsv = rgb_to_hsv(r, g, b);
    rgb_matrix_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
    RGB rgb = hsv_to_rgb(hsv);
    INDICATOR_R = rgb.r;
    INDICATOR_G = rgb.g;
    INDICATOR_B = rgb.b;
}

bool rgb_matrix_indicators_user(void) {
    // rgb_matrix_set_color(23, INDICATOR_R, INDICATOR_G, INDICATOR_B);
    rgb_matrix_set_color(24, INDICATOR_R, INDICATOR_G, INDICATOR_B);
    // rgb_matrix_set_color(25, INDICATOR_R, INDICATOR_G, INDICATOR_B);
    return true;
}  

void layer_change(uint8_t layer) {
    switch (layer) {
        case COMBO:
            set_color(255, 128, 0);
            break;
        case LOWER:
            set_color(128, 255, 64);
            break;
        case RAISE:
            set_color(0, 128, 255);
            break;
        default:
            set_color(128, 0, 255);
            break;
    }
}

void matrix_scan_user(void) {
    uint8_t layer = get_highest_layer(layer_state);

    bool lspc = matrix_is_on(3, 3);
    bool rspc = matrix_is_on(3, 6);

    if (!lspc && !rspc && !is_in_window_switch) {
        layer_move(BASE);
        return;
    }

    if (layer == BASE) return;

    if (lspc && !rspc) {
        layer_move(LOWER);
    } else if (!lspc && rspc) {
        layer_move(RAISE);
    } else if (lspc && rspc) {
        layer_move(COMBO);
    }
} 

uint8_t last_layer = BASE;
layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);
    if (layer == last_layer) return state;
    layer_change(layer);
    last_layer = layer;
    return state;
}

void keyboard_post_init_user(void) {
    layer_change(BASE);
    rgb_matrix_set_speed(64);
    rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);

    // debug_enable=true;
    // debug_keyboard=true;
    // debug_matrix=true;
}
