#pragma once

#define FORCE_NKRO
#define TAPPING_TERM_PER_KEY
#define TAPPING_TERM 110
#define TAPPING_TERM_SLOW 180

#define USB_POLLING_INTERVAL_MS 1

#ifdef RGB_MATRIX_ENABLE
  #define WS2812_DI_PIN B3
  #define RGB_MATRIX_LED_COUNT 42

  #define RGB_MATRIX_ANI_SPEED 64

  #define RGBLIGHT_HUE_STEP 1
  #define RGBLIGHT_SAT_STEP 1
  #define RGBLIGHT_VAL_STEP 1

  #define RGB_MATRIX_KEYPRESSES
  #undef RGB_MATRIX_LED_PROCESS_LIMIT
  #undef RGB_MATRIX_LED_FLUSH_LIMIT
#endif
