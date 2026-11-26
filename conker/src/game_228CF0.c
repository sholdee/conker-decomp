#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A data-only unit between game_5D2C0 and game_61490 in the link: its .rodata (0x23C630) and its .data
 * (0x228CF0) each start on a 16-byte unit boundary, and the 12 zero bytes after the last list are the
 * unit's own padding (front-end data and the jump table of game_61490 would be adjacent in one unit).
 *
 * Per-model bone remap lists: (bone, value) byte pairs terminated by 0xFF. actor_computeBones (game_58F80)
 * copies the list of a model's remap set (1-based: 0 is none) into its per-bone remap buffer.
 */
static const u8 D_80097B70[] = { 5, 2, 0xFF };
static const u8 D_80097B74[] = { 2, 2, 0xFF };
static const u8 D_80097B78[] = { 1, 2, 16, 2, 0xFF };
static const u8 D_80097B80[] = { 12, 2, 13, 2, 0xFF };
static const u8 D_80097B88[] = { 4, 2, 0xFF };
static const u8 D_80097B8C[] = { 4, 2, 0xFF };
static const u8 D_80097B90[] = { 1, 4, 4, 5, 14, 4, 18, 4, 22, 4, 0xFF };
static const u8 D_80097B9C[] = { 7, 1, 12, 1, 0xFF };
static const u8 D_80097BA4[] = { 12, 1, 0xFF };
static const u8 D_80097BA8[] = { 1, 3, 0xFF };
static const u8 D_80097BAC[] = { 5, 2, 0xFF };
static const u8 D_80097BB0[] = { 1, 2, 0xFF };
static const u8 D_80097BB4[] = { 4, 2, 0xFF };
static const u8 D_80097BB8[] = { 5, 2, 0xFF };
static const u8 D_80097BBC[] = { 4, 2, 0xFF };
static const u8 D_80097BC0[] = { 2, 2, 3, 2, 0xFF };
static const u8 D_80097BC8[] = { 3, 2, 0xFF };
static const u8 D_80097BCC[] = { 1, 2, 0xFF };
static const u8 D_80097BD0[] = { 43, 2, 44, 2, 0xFF };
static const u8 D_80097BD8[] = { 1, 2, 0xFF };
static const u8 D_80097BDC[] = { 1, 2, 0xFF };
static const u8 D_80097BE0[] = { 13, 2, 0xFF };
static const u8 D_80097BE4[] = { 5, 2, 0xFF };
static const u8 D_80097BE8[] = { 1, 2, 3, 2, 5, 2, 0xFF };
static const u8 D_80097BF0[] = { 29, 2, 0xFF };
static const u8 D_80097BF4[] = { 2, 2, 0xFF };
static const u8 D_80097BF8[] = { 4, 2, 0xFF };
static const u8 D_80097BFC[] = { 3, 2, 0xFF };
static const u8 D_80097C00[] = { 4, 2, 0xFF };
static const u8 D_80097C04[] = { 17, 2, 0xFF };
static const u8 D_80097C08[] = { 4, 2, 0xFF };
static const u8 D_80097C0C[] = { 4, 2, 0xFF };
static const u8 D_80097C10[] = { 4, 2, 0xFF };
static const u8 D_80097C14[] = { 3, 2, 5, 2, 0xFF };
static const u8 D_80097C1C[] = { 4, 2, 0xFF };
static const u8 D_80097C20[] = { 27, 2, 0xFF };
static const u8 D_80097C24[] = { 25, 2, 0xFF };
static const u8 D_80097C28[] = { 3, 2, 22, 2, 26, 2, 0xFF };
static const u8 D_80097C30[] = { 1, 2, 0xFF };
static const u8 D_80097C34[] = { 4, 2, 5, 2, 0xFF };
static const u8 D_80097C3C[] = { 4, 2, 5, 2, 0xFF };
static const u8 D_80097C44[] = { 4, 2, 5, 2, 0xFF };
static const u8 D_80097C4C[] = { 4, 2, 0xFF };
static const u8 D_80097C50[] = { 33, 2, 0xFF };
static const u8 D_80097C54[] = { 5, 2, 0xFF };
static const u8 D_80097C58[] = { 4, 2, 0xFF };
static const u8 D_80097C5C[] = { 4, 2, 0xFF };
static const u8 D_80097C60[] = { 12, 2, 0xFF };
static const u8 D_80097C64[] = { 4, 2, 5, 2, 0xFF };
static const u8 D_80097C6C[] = { 4, 2, 0xFF };
static const u8 D_80097C70[] = { 4, 2, 0xFF };

const u8 *D_80084230[] = {
    D_80097BAC,
    D_80097B78,
    D_80097B74,
    D_80097B80,
    D_80097B88,
    D_80097B9C,
    D_80097BA8,
    D_80097B8C,
    D_80097B90,
    D_80097BB0,
    D_80097BB4,
    D_80097BB8,
    D_80097BBC,
    D_80097B70,
    D_80097BC0,
    D_80097BC8,
    D_80097BCC,
    D_80097BD0,
    D_80097BD8,
    D_80097BDC,
    D_80097BE0,
    D_80097BE4,
    D_80097BE8,
    D_80097BF0,
    D_80097BF4,
    D_80097BF8,
    D_80097BFC,
    D_80097C00,
    D_80097BA4,
    D_80097C04,
    D_80097C08,
    D_80097C0C,
    D_80097C10,
    D_80097C14,
    D_80097C20,
    D_80097C24,
    D_80097C28,
    D_80097C30,
    D_80097C34,
    D_80097C3C,
    D_80097C44,
    D_80097C4C,
    D_80097C50,
    D_80097C1C,
    D_80097C54,
    D_80097C58,
    D_80097C5C,
    D_80097C60,
    D_80097C64,
    D_80097C6C,
    D_80097C70,
};
