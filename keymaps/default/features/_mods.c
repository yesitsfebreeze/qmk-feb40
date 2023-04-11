

uint8_t _get_mods(void) {
    const uint8_t mod_state = get_mods();
    uint8_t mods;
    #ifndef NO_ACTION_ONESHOT
        mods = (mod_state | get_weak_mods() | get_oneshot_mods()); 
    #else
        mods = (mod_state | get_weak_mods()); 
    #endif

    _mods = mods;
}