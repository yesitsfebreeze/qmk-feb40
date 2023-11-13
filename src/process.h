#pragma once

#include QMK_KEYBOARD_H
#include "src/core.h"

extern uint16_t process_remaps(uint16_t kc, Mods mods);
extern uint16_t process_os(uint16_t kc, Mods mods, int os);
extern char* process_macros(uint16_t kc);
