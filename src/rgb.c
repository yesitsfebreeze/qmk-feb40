#include "src/rgb.h"
#include "src/core.h"


#ifdef RGB_MATRIX_ENABLE

bool RGB_INDICATOR = true;
bool RGB_MODE = false;
uint8_t INDICATOR_R = 0;
uint8_t INDICATOR_G = 0;
uint8_t INDICATOR_B = 0;

uint8_t last_layer = BASE;

#define RGB_MATRIX_MAX 41

led_config_t g_led_config = { {
  { 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30 },
  { 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, NO_LED, 19 },
  { 18, 17, 16, 15, 14, 13, 12, 11, 10, NO_LED, 9,  8  },
  { 7,  6,  5,  4,  NO_LED, NO_LED, 3,  NO_LED, NO_LED, 2,  1,  0  }
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
  INDICATOR_R = r;
  INDICATOR_G = g;
  INDICATOR_B = b;
  HSV hsv = rgb_to_hsv(r, g, b);
  rgb_matrix_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
}

void set_rgb_mode(void) {
  rgb_matrix_set_speed(RGB_MATRIX_ANI_SPEED);
  rgb_matrix_disable();
  rgb_matrix_enable();

  if (RGB_MODE) {
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_INVERSED_REACTIVE);
  } else {
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_REACTIVE);
  }

  uint8_t tmp_layer = last_layer - 1;
  if (tmp_layer < 0) { 
    tmp_layer = last_layer + 1;
  }

  layer_move(BASE);
  layer_move(COMBO);
}

bool rgb_matrix_indicators_user(void) {
  if (!RGB_INDICATOR) return true;
  uint8_t current_layer = get_highest_layer(layer_state);
  if (current_layer == GAME) {
    rgb_matrix_set_color(RGB_LAYER_INDICATOR_KEY, 0, 0, 0);
    rgb_matrix_set_color(RGB_LAYER_INDICATOR_W - 2, INDICATOR_R, INDICATOR_G, INDICATOR_B);
    rgb_matrix_set_color(RGB_LAYER_INDICATOR_A - 2, INDICATOR_R, INDICATOR_G, INDICATOR_B);
    rgb_matrix_set_color(RGB_LAYER_INDICATOR_S - 2, INDICATOR_R, INDICATOR_G, INDICATOR_B);
    rgb_matrix_set_color(RGB_LAYER_INDICATOR_D - 2, INDICATOR_R, INDICATOR_G, INDICATOR_B);
  } else {
    if (RGB_MODE) {
      rgb_matrix_set_color(RGB_LAYER_INDICATOR_KEY, 0, 0, 0);
    } else {
      rgb_matrix_set_color(RGB_LAYER_INDICATOR_KEY, INDICATOR_R, INDICATOR_G, INDICATOR_B);
    }
  }
  return true;
}

void layer_color_change(uint8_t layer) {
  switch (layer) {
    case GAME:
      set_color(RGB_GAME);
      break;
    case COMBO:
      set_color(RGB_COMBO);
      break;
    case LOWER:
      set_color(RGB_LOWER);
      break;
    case RAISE:
      set_color(RGB_RAISE);
      break;
    default:
      if (OS == OS_MAC) {
        set_color(RGB_MAC);
        break;
      } else {
        set_color(RGB_BASE);
      }
      
      break;
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
  uint8_t layer = get_highest_layer(state);
  if (layer == last_layer) return state;
  layer_color_change(layer);
  last_layer = layer;
  return state;
}

#endif

void init_rgb() {
  #ifdef RGB_MATRIX_ENABLE
    layer_color_change(BASE);
    set_rgb_mode();
    layer_move(BASE);
  #endif
}

bool handle_rgb(uint16_t kc, keyrecord_t *rec) {
  #ifdef RGB_MATRIX_ENABLE
    if (!rec->event.pressed) return false;
    if (kc != CK_RGB && kc != CK_RGBI) return false;

    if (kc == CK_RGB) RGB_MODE = !RGB_MODE;
    if (kc == CK_RGBI) RGB_INDICATOR = !RGB_INDICATOR;
    
    set_rgb_mode();
    
    return true;
  #endif
  return false;
}
