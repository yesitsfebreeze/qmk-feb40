#include "_util.h"

uint8_t get_full_mods(void) {
  uint8_t mods = get_mods();
# ifndef NO_ACTION_ONESHOT
    return (mods | get_weak_mods() | get_oneshot_mods()); 
# else
    return (mods | get_weak_mods()); 
# endif
}

void remove_mod_mask(uint8_t mask) {
  del_weak_mods(mask);
# ifndef NO_ACTION_ONESHOT
    del_oneshot_mods(mask);
# endif
  del_mods(mask);
}

bool is_shifted_kc(uint16_t kc) {
  if (((kc) >= KC_A && (kc) <= KC_Z)) return true;
  return IS_QK_MODS(kc) && (QK_MODS_GET_MODS(kc) & MOD_MASK_SHIFT) != 0;
}