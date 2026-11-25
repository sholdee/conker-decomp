#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct SoundEntry15116BAC {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
};

extern struct SoundEntry15116BAC D_80089260[];
extern void func_15114D24(void *, s32, s32, s32, s32, s32);

extern f32 sinf(f32);

struct Obj1511E780 {
    u8 pad0[4];
    f32 unk4;
    f32 unk8;
    u8 padC[0x30];
    s32 unk3C;
    u8 pad40[0x33];
    u8 unk73;
    u8 pad74[8];
    s32 unk7C;
    s32 unk80;
    f32 unk84;
};

void func_1511E780(struct Obj1511E780 *arg0) {
    f32 speed;
    s32 sfx;
    s32 snd;
    s32 state;
    f32 range;
    f32 rate;

    state = arg0->unk73 & 3;
    sfx = -1;
    if (arg0->unk80 == 1) {
        if (state == 0 || state == 1) {
            state = 2;
            sfx = 0;
        }
        speed = 1.0f;
    } else if (arg0->unk80 == 0) {
        if (state == 3 || state == 2) {
            state = 1;
            sfx = 1;
        }
        speed = -1.0f;
    } else {
        speed = 90.0f;
    }
    if (sfx != -1) {
        snd = arg0->unk3C & 0xFFFF;
        snd = ((s16 (*)[6])D_80089260)[snd][sfx];

        if (snd > 0) {
            func_15114D24(arg0, snd, 0x5DC0, 0xC8, 0x9C4, 0);
        }
    }
    speed *= D_800BE9E4;
    range = 70.0f;
    rate = 6.0f;
    switch (arg0->unk3C >> 16) {
    case 6:
        rate = 3.0f;
        range = 82.0f;
        break;
    case 2:
    case 4:
        range = 80.0f;
        break;
    }
    switch (arg0->unk3C >> 16) {
    case 0:
        if (arg0->unk7C == 0) {
            arg0->unk84 = arg0->unk8;
        }
        arg0->unk8 = arg0->unk8 - speed * rate;
        if (arg0->unk84 < arg0->unk8) {
            arg0->unk8 = arg0->unk84;
            state = 0;
        }
        if (arg0->unk8 < -110.0f) {
            arg0->unk8 = -110.0f;
            state = 3;
        }
        break;
    case 3:
        if (arg0->unk7C == 0) {
            arg0->unk84 = arg0->unk4;
        }
        arg0->unk4 = arg0->unk4 - speed * 3.0f;
        if (arg0->unk4 < 260.0f) {
            arg0->unk4 = 260.0f;
            state = 3;
        }
        if (arg0->unk84 < arg0->unk4) {
            arg0->unk4 = arg0->unk84;
            state = 0;
        }
        break;
    case 2:
    case 8:
        if (arg0->unk7C == 0) {
            arg0->unk84 = arg0->unk4;
        }
        arg0->unk4 = arg0->unk4 + speed * rate;
        if (arg0->unk84 + range < arg0->unk4) {
            arg0->unk4 = arg0->unk84 + range;
            state = 3;
        }
        if (arg0->unk4 < arg0->unk84) {
            arg0->unk4 = arg0->unk84;
            state = 0;
        }
        break;
    case 4:
    case 6:
    case 7:
        if (arg0->unk7C == 0) {
            arg0->unk84 = arg0->unk4;
        }
        arg0->unk4 = arg0->unk4 - speed * rate;
        if (arg0->unk84 < arg0->unk4) {
            arg0->unk4 = arg0->unk84;
            state = 0;
        }
        if (arg0->unk4 < arg0->unk84 - range) {
            arg0->unk4 = arg0->unk84 - range;
            state = 3;
        }
        break;
    case 10:
        arg0->unk84 += 0.00999999977648f;
    case 11:
        arg0->unk84 += 0.0399999991059f * D_800BE9E4;
        arg0->unk8 = sinf(arg0->unk84) * 10.0f;
        if (arg0->unk84 > 12.5663709641f) {
            arg0->unk84 -= 12.5663709641f * (f32)(u32)((func_150ADA20() & 4) + 1);
            func_15114D24(arg0, 0x503, 0x1F40, 0x96, 0x258, 0);
        }
        break;
    case 12:
        arg0->unk84 += 0.00999999977648f;
    case 13:
        arg0->unk84 += 0.0399999991059f * D_800BE9E4;
        arg0->unk8 = 180.0 - sinf(arg0->unk84) * 10.0f;
        break;
    }
    arg0->unk7C = 1;
    arg0->unk73 &= ~3;
    arg0->unk73 |= state;
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
