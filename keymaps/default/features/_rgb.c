// handles rgb functionality

#include "_rgb.h"
#include "../_layers.h"


#ifdef RGB_MATRIX_ENABLE

int RGB_MODE = 2;
int RGB_MODE_MAX = 4;
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
  if (RGB_MODE == 0) {
    r = 0;
    g = 0;
    b = 0;
  }

  HSV hsv = rgb_to_hsv(r, g, b);
  rgb_matrix_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
  RGB rgb = hsv_to_rgb(hsv);
  INDICATOR_R = rgb.r;
  INDICATOR_G = rgb.g;
  INDICATOR_B = rgb.b;
}


void set_rgb_mode(void) {
  rgb_matrix_set_speed(RGB_MATRIX_ANI_SPEED);
  rgb_matrix_disable();

  switch (RGB_MODE) {
    case 0:
      rgb_matrix_mode(RGB_MODE_0);
      rgb_matrix_set_color_all(0, 0, 0);
      break;
    case 1:
      rgb_matrix_enable();
      rgb_matrix_mode(RGB_MODE_1);
      break;
    case 2:
      rgb_matrix_enable();
      rgb_matrix_mode(RGB_MODE_2);
      break;
    case 3:
      rgb_matrix_enable();
      rgb_matrix_mode(RGB_MODE_3);
      break;
    case 4:
      rgb_matrix_enable();
      rgb_matrix_mode(RGB_MODE_4);
      break;
  }

  uint8_t tmp_layer = last_layer - 1;
  if (tmp_layer < 0) { 
    tmp_layer = last_layer + 1;
  }

  layer_move(BASE);
  layer_move(COMBO);
}

bool handle_rgb_mode(uint16_t keycode, keyrecord_t* record) {
  if (keycode != KC_RGB_SWITCH) return false;
  // exit out early on release
  if (!record->event.pressed) return true;

  if (RGB_MODE == RGB_MODE_MAX) {
    RGB_MODE = 0;
  } else {
    RGB_MODE++;
  }
  
  set_rgb_mode();
  
  return true;
}

bool rgb_matrix_indicators_user(void) {
  rgb_matrix_set_color(RGB_LAYER_INDICATOR_KEY, INDICATOR_R, INDICATOR_G, INDICATOR_B);
  return true;
}

void layer_color_change(uint8_t layer) {
  switch (layer) {
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
      set_color(RGB_BASE);
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

void init_rgb() {
  layer_color_change(BASE);
  set_rgb_mode();
  layer_move(BASE);
}

#endif