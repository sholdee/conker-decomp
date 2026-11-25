#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Sprite descriptor passed to func_15169968 (same layout as struct_7484 in game_1228D0.c). */
typedef struct {
    /* 0x00 */ void *texture;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s32 fadeInHold;    /* fade-in frames << 16 | hold frames */
    /* 0x0C */ s32 fadeOutTotal;  /* fade-out frames << 16 | total frames */
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 timer;         /* frames left, counts down from the total */
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  pad1A[0x2];
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 scaleX;        /* 0x1000 = 1.0 */
    /* 0x2A */ s16 scaleY;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8  unk30;
    /* 0x31 */ u8  unk31;
    /* 0x32 */ u8  red;
    /* 0x33 */ u8  green;
    /* 0x34 */ u8  blue;
    /* 0x35 */ u8  alpha;
    /* 0x36 */ u8  unk36;
    /* 0x37 */ u8  unk37;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ u8  unk39;
} struct_7484;

struct102 *func_15169968(struct_7484 *);

/* Shows a sprite that fades in over `fadeIn` frames, holds for `hold` frames and
 * fades out over `fadeOut` frames. */
void func_1510A930(s32 fadeIn, s32 hold, s32 fadeOut) {
    struct_7484 desc;
    s32 total;

    desc.texture = &D_800917EC;
    desc.unk30 = 10;
    desc.unk2C = 64;
    desc.unk2E = 32;
    desc.scaleX = 0x18FC;
    desc.scaleY = 0x1A12;
    desc.unk36 = 7;
    total = fadeIn + hold + fadeOut;
    desc.unk39 = 0;
    desc.fadeInHold = (fadeIn << 16) | hold;
    desc.fadeOutTotal = (fadeOut << 16) | total;
    desc.unk10 = 10000;
    desc.timer = total;
    desc.unk18 = 0;
    desc.unk24 = 0;
    desc.unk26 = 0;
    desc.unk31 = 1;
    desc.red = 0xFF;
    desc.green = 0xFF;
    desc.blue = 0xFF;
    desc.unk37 = 0x11;
    desc.alpha = 0;
    desc.unk16 = 0;
    desc.unk1C = 228.0f;
    desc.unk20 = 252.59259033203125f;
    func_15169968(&desc);
}
