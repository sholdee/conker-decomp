#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A placed level prop: one 0xA0-byte entry of the D_800DBEF4 table (same layout as in
 * game_14C3F0.c). The words at 0x7C-0x87 are scratch state owned by the behaviour function.
 */
typedef struct Prop {
    /* 0x00 */ f32 rotX; /* degrees */
    /* 0x04 */ f32 rotY;
    /* 0x08 */ f32 rotZ;
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x26];
    /* 0x3C */ s32 params; /* high half: behaviour variant; low half: sound row */
    /* 0x40 */ u8 pad40[0x2E];
    /* 0x6E */ u8 finished; /* set to 1 when the behaviour has run its course */
    /* 0x6F */ u8 pad6F[3];
    /* 0x72 */ u8 id;
    /* 0x73 */ u8 state; /* bits 0-1: motion state */
    /* 0x74 */ u8 pad74[8];
    /* 0x7C */ union {
        struct {
            s32 started;   /* set after the first update */
            s32 triggered; /* 1: move away from home, 0: return home */
            f32 home;      /* rotation or coordinate saved on the first update */
        } slide;
        struct {
            s32 started;
            s32 unk80;
            f32 phase; /* radians */
        } wobble;
    } work;
    /* 0x88 */ u8 pad88[0x18];
} Prop; /* size 0xA0 */

/* Motion states kept in bits 0-1 of Prop.state. */
#define PROP_REST 0
#define PROP_RETURNING 1
#define PROP_MOVING 2
#define PROP_ARRIVED 3

/* One row of D_80089260, selected by the low half of Prop.params. */
typedef struct PropSoundRow {
    s16 sounds[6]; /* [0] start opening, [1] start closing, [2] opened, [3] closed */
} PropSoundRow;

#define PROP_SOUND_OPEN 0
#define PROP_SOUND_CLOSE 1

extern PropSoundRow D_80089260[];
extern void func_15114D24(Prop *prop, s32 sound, s32, s32, s32, s32);

extern f32 sinf(f32);

/*
 * Swings a hinged prop (rotY or rotZ, by variant) between its home rotation and a limit while
 * triggered, playing the row's open/close sounds; variants 10-13 rock rotZ on a sine instead.
 */
void func_1511E780(Prop *prop) {
    f32 speed;
    s32 soundSlot;
    s32 sound;
    s32 motion;
    f32 range;
    f32 rate;

    motion = prop->state & 3;
    soundSlot = -1;
    if (prop->work.slide.triggered == 1) {
        if (motion == PROP_REST || motion == PROP_RETURNING) {
            motion = PROP_MOVING;
            soundSlot = PROP_SOUND_OPEN;
        }
        speed = 1.0f;
    } else if (prop->work.slide.triggered == 0) {
        if (motion == PROP_ARRIVED || motion == PROP_MOVING) {
            motion = PROP_RETURNING;
            soundSlot = PROP_SOUND_CLOSE;
        }
        speed = -1.0f;
    } else {
        speed = 90.0f;
    }
    if (soundSlot != -1) {
        sound = prop->params & 0xFFFF; // matching: the row index must share sound's register
        sound = D_80089260[sound].sounds[soundSlot];
        if (sound > 0) {
            func_15114D24(prop, sound, 24000, 200, 2500, 0);
        }
    }
    speed *= D_800BE9E4;
    range = 70.0f;
    rate = 6.0f;
    switch (prop->params >> 16) {
    case 6:
        rate = 3.0f;
        range = 82.0f;
        break;
    case 2:
    case 4:
        range = 80.0f;
        break;
    }
    switch (prop->params >> 16) {
    case 0:
        if (prop->work.slide.started == 0) {
            prop->work.slide.home = prop->rotZ;
        }
        prop->rotZ -= speed * rate;
        if (prop->work.slide.home < prop->rotZ) {
            prop->rotZ = prop->work.slide.home;
            motion = PROP_REST;
        }
        if (prop->rotZ < -110.0f) {
            prop->rotZ = -110.0f;
            motion = PROP_ARRIVED;
        }
        break;
    case 3:
        if (prop->work.slide.started == 0) {
            prop->work.slide.home = prop->rotY;
        }
        prop->rotY -= speed * 3.0f;
        if (prop->rotY < 260.0f) {
            prop->rotY = 260.0f;
            motion = PROP_ARRIVED;
        }
        if (prop->work.slide.home < prop->rotY) {
            prop->rotY = prop->work.slide.home;
            motion = PROP_REST;
        }
        break;
    case 2:
    case 8:
        if (prop->work.slide.started == 0) {
            prop->work.slide.home = prop->rotY;
        }
        prop->rotY += speed * rate;
        if (prop->work.slide.home + range < prop->rotY) {
            prop->rotY = prop->work.slide.home + range;
            motion = PROP_ARRIVED;
        }
        if (prop->rotY < prop->work.slide.home) {
            prop->rotY = prop->work.slide.home;
            motion = PROP_REST;
        }
        break;
    case 4:
    case 6:
    case 7:
        if (prop->work.slide.started == 0) {
            prop->work.slide.home = prop->rotY;
        }
        prop->rotY -= speed * rate;
        if (prop->work.slide.home < prop->rotY) {
            prop->rotY = prop->work.slide.home;
            motion = PROP_REST;
        }
        if (prop->rotY < prop->work.slide.home - range) {
            prop->rotY = prop->work.slide.home - range;
            motion = PROP_ARRIVED;
        }
        break;
    case 10:
        prop->work.wobble.phase += 0.00999999977648f;
        /* fallthrough */
    case 11:
        prop->work.wobble.phase += 0.0399999991059f * D_800BE9E4;
        prop->rotZ = sinf(prop->work.wobble.phase) * 10.0f;
        if (prop->work.wobble.phase > 12.5663709641f) { /* 4 pi */
            // matching: the multiplier converts as unsigned
            prop->work.wobble.phase -= 12.5663709641f * (u32)((func_150ADA20() & 4) + 1);
            func_15114D24(prop, 0x503, 8000, 150, 600, 0);
        }
        break;
    case 12:
        prop->work.wobble.phase += 0.00999999977648f;
        /* fallthrough */
    case 13:
        prop->work.wobble.phase += 0.0399999991059f * D_800BE9E4;
        // matching: computed in double precision (180.0, not 180.0f)
        prop->rotZ = 180.0 - sinf(prop->work.wobble.phase) * 10.0f;
        break;
    }
    prop->work.slide.started = TRUE;
    prop->state &= ~3;
    prop->state |= motion;
}
struct Obj1511EC50 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x28];
    s32 unk3C;
    u8 pad40[0x2E];
    s8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    f32 unk84;
};

struct Obj1511ED84 {
    u8 pad0[0x14];
    s16 unk14;
    u8 pad16[0x5C];
    u8 unk72;
    u8 pad73[0x9];
    s32 unk7C;
    s32 unk80;
    f32 unk84;
};

void func_1511EC50(struct Obj1511EC50 *arg0) {
    f32 step;
    f32 limit;

    if (arg0->unk7C == 0) {
        arg0->unk84 = (f32)arg0->unk12;
    }

    step = (f32)D_800BE9E4 * 4.0f;
    if (arg0->unk3C != 0) {
        step = step * 0.25f;
    }

    if (arg0->unk80 != 0) {
        arg0->unk12 = (s32)((f32)arg0->unk12 + (step * 5.0f));
        limit = arg0->unk84 + 500.0f;
        if (limit < (f32)arg0->unk12) {
            arg0->unk12 = (s32)limit;
            arg0->unk6E = 1;
        }
    } else {
        limit = arg0->unk84;
        arg0->unk12 = (s32)((f32)arg0->unk12 - step);
        if ((f32)arg0->unk12 < limit) {
            arg0->unk12 = (s32)limit;
        }
    }
    arg0->unk7C = 1;
}

void func_1511ED84(struct Obj1511ED84 *arg0) {
    f32 step;
    f32 limit;
    s32 direction;

    if (arg0->unk7C == 0) {
        arg0->unk84 = (f32)arg0->unk14;
    }

    step = (f32)D_800BE9E4 * 4.0f;
    direction = 1 - (arg0->unk72 & 1);

    if (arg0->unk80 != 0) {
        if (direction & 1) {
            arg0->unk14 = (s32)((f32)arg0->unk14 + step);
            limit = arg0->unk84;
            if (limit < (f32)arg0->unk14) {
                arg0->unk14 = (s32)limit;
            }
        } else {
            limit = arg0->unk84;
            arg0->unk14 = (s32)((f32)arg0->unk14 - step);
            if ((f32)arg0->unk14 < limit) {
                arg0->unk14 = (s32)limit;
            }
        }
    } else {
        if (direction & 1) {
            arg0->unk14 = (s32)((f32)arg0->unk14 - step);
            limit = arg0->unk84 - 350.0f;
            if ((f32)arg0->unk14 < limit) {
                arg0->unk14 = (s32)limit;
            }
        } else {
            arg0->unk14 = (s32)((f32)arg0->unk14 + step);
            limit = arg0->unk84 + 350.0f;
            if (limit < (f32)arg0->unk14) {
                arg0->unk14 = (s32)limit;
            }
        }
    }
    arg0->unk7C = 1;
}
