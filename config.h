#pragma once

#define FORCE_NKRO
#define TAPPING_TERM_PER_KEY
#define TAPPING_TERM_FAST 120
#define TAPPING_TERM 200

#define USB_POLLING_INTERVAL_MS  1

#ifdef RGB_MATRIX_ENABLE
    #define RGB_DI_PIN B3
    #define RGB_MATRIX_LED_COUNT 42

    #define RGB_DISABLE_WHEN_USB_SUSPENDED     // turn off effects when suspended
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 255  // limits maximum brightness of LEDs to x out of 255. If not defined maximum brightness is set to 255

    #define RGBLIGHT_HUE_STEP 1
    #define RGBLIGHT_SAT_STEP 1
    #define RGBLIGHT_VAL_STEP 1

    #define RGB_MATRIX_KEYPRESSES              // enable key press effects

    #define ENABLE_RGB_MATRIX_SOLID_COLOR
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#endif
