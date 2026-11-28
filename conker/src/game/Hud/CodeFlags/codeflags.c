#include <ultra64.h>
#include "conker/boot.h"
#include "conker/hud.h"

/* Clears the code flags set by hud_updateCodeEntry. */
void hud_clearCodeFlags(void) {
    D_800E9D00 = 0;
}

/* 0x40 of inter-TU padding at 0x151EF000..0x151EF040.  Not code: it is the
   tail of this TU's conker.us.yaml range (0x21C4A0..0x21C4F0) and must be
   stated explicitly, or everything downstream slides. */
GLOBAL_ASM(
glabel pad_151EF000
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
)

