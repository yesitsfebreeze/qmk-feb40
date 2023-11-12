#include "features/_util.h"

#define KC_MB1      KC_MS_BTN1
#define KC_MB2      KC_MS_BTN2
#define KC_MB3      KC_MS_BTN3 // MWHEEL_CLICK

#define L_LOWER     LT(LOWER, KC_SPC)
#define L_RAISE     LT(RAISE, KC_SPC)
#define L_COMBO     LT(COMBO, KC_SPC)

#define CK_ESC      CK(0)
#define CK_ENT      CK(1)
#define CK_QUOT     CK(2)
#define CK_UNDS     CK(3)
#define CK_EQL      CK(4)
#define CK_LBRC     CK(5)
#define CK_RBRC     CK(6)
#define CK_LPRN     CK(7)
#define CK_RPRN     CK(8)
#define CK_AMPR     CK(9)
#define CK_PIPE     CK(10)
#define CK_COM      CK(11)
#define CK_DOT      CK(12)
#define CK_SLASH    CK(13)
#define CK_PLMN     CK(14)
#define CK_HOME     CK(15)
#define CK_END      CK(16)
#define CK_9        CK(17)

#define CK_TEST     CK(18)

#define CK_RGB      CK(30)
#define CK_MODE     CK(31)
#define CK_META     CK(32)
#define CK_ALT      CK(33)

#define MA_OR       CK(40)
#define MA_AND      CK(41)
#define MA_PTR      CK(42)


extern void hook_key_change(uint16_t keycode, keyrecord_t* record);