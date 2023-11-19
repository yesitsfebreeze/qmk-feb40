enum LAYERS {
  BASE,
  LOWER,
  RAISE,  
  COMBO,
};

enum OS_TYPES {
  OS_WIN,
  OS_REM,
  OS_LST
};

#define LT_L LT(1, KC_SPC)
#define LT_R LT(2, KC_SPC)
#define LT_C LT(3, KC_SPC)

#define CK_OS QK_USER_31
#define CK_NO QK_UNICODE_MAX
