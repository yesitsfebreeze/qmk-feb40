// !!! DO NOT ADD #pragma once

RGB_MATRIX_EFFECT(REACTIVE)
RGB_MATRIX_EFFECT(INVERSED_REACTIVE)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static float ease_in_circ(float t) {
  return 1 - sqrt(1 - pow(t, 2));
}

static HSV reactive_math(HSV hsv, uint16_t offset) {
  float percent = ease_in_circ((float)offset / 255.0);
  hsv.v = scale8(255 - 255 * percent, hsv.v);
  return hsv;
}

static HSV reactive_math_inversed(HSV hsv, uint16_t offset) {
  hsv = reactive_math(hsv, offset);
  hsv.v = 255 - hsv.v;
  return hsv;
}

static bool REACTIVE(effect_params_t* params) {
  return effect_runner_reactive(params, &reactive_math);
}

static bool INVERSED_REACTIVE(effect_params_t* params) {
  return effect_runner_reactive(params, &reactive_math_inversed);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
