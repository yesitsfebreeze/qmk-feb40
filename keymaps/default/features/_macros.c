#include "_macros.h"

bool process_macros(uint16_t kc, keyrecord_t* record) {
    bool prsd = record->event.pressed;
    switch (kc) {
        case MC_OR:
            if (prsd) SEND_STRING("||");
            return true;
        case MC_AND:
            if (prsd) SEND_STRING("&&");
            return true;
        case MC_PTR:
            if (prsd) SEND_STRING("->");
            return true;
    }

    return false;
}