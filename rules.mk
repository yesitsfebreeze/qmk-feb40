# CONSOLE_ENABLE = yes

BOOTMAGIC_ENABLE = yes
MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
NKRO_ENABLE = yes
SEND_STRING_ENABLE = yes
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = ws2812
SRC += features/_remote_mode.c
SRC += features/_macros.c
SRC += features/_remap.c
SRC += features/_rgb.c
SRC += features/_window_switch.c
SRC += _setup.c
