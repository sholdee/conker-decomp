#include <ultra64.h>
#define func_15048A40 func_15048A40_void_unused
#define func_10010F88 func_10010F88_void_decl_in_functions_h
#include "functions.h"
#undef func_15048A40
#undef func_10010F88
s32 func_10010F88(s32 arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9);
#include "variables.h"
extern f32 func_15048A40(u8);
extern void func_15188010(s32, f32 *);
extern s32 func_151149AC(u32);
extern f32 D_800A2F8C;
extern void *allocate_memory(s32, s32, s32, s32);
extern f32 D_800A3210;
extern f32 D_800A3214;
extern f32 D_800A2FF4;
extern f32 D_800A2FF8;
extern f32 D_800A2FFC;
extern f32 D_800A2FB0;
extern f32 D_800A2FB4;

struct Struct1511F31CArg {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x28];
    u16 unk3E;
    u8 pad40[0x14];
    u16 unk54;
};

extern void func_1511F31C(struct Struct1511F31CArg *arg0);

struct Struct1511490C {
    char pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    char pad16[2];
    f32 unk18;
    char pad1C[0x10];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
};

extern void func_1511490C(f32 arg0[4][4], struct Struct1511490C *arg1);
extern void func_150A7960(f32 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);

struct Obj151151FC {
    f32 unk0;
    u8 pad4[0x38];
    s32 unk3C;
    u8 pad40[0x20];
    f32 unk60;
};

struct Obj151150BC {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x30];
    s32 unk3C;
    u8 pad40[0x28];
    f32 unk68;
};

struct Obj1511515C {
    u8 pad0[0x4];
    f32 unk4;
    u8 pad8[0x34];
    s32 unk3C;
    u8 pad40[0x24];
    f32 unk64;
};

struct Obj151152A8 {
    char pad_0x0[0x12];
    s16 field_0x12;
    char pad_0x14[0x28];
    s32 field_0x3C;
    char pad_0x40[0xF];
    u8 field_0x4F;
    char pad_0x50[0x23];
    u8 field_0x73;
    char pad_0x74[0x8];
    s32 field_0x7C;
};
void func_151150B0(s32 arg0);
void func_151150BC(struct Obj151150BC *arg0);
void func_1511515C(struct Obj1511515C *arg0);
void func_151151FC(struct Obj151151FC *arg0);
void func_1511529C(s32 arg0);
void func_151152A8(struct Obj151152A8 *arg0);
void func_151155C0(void *arg0);

struct Obj1511575C {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x3C - 0x16];
    s32 unk3C;
    u8 pad40[0xF];
    u8 unk4F;
    u8 pad50[0xA];
    s16 unk5A;
    s16 unk5C;
    s16 unk5E;
    u8 pad60[0x1C];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
};

extern s32 func_10010E78(u16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_1511575C(struct Obj1511575C *arg0);

extern f32 D_800A2FA0;
extern f32 D_800A2FA4;

struct S_15115EDC { u8 pad[0x7C]; f32 unk7C; f32 unk80; };
struct A_15115EDC { u8 pad[0x84]; u16 unk84; };

struct B_15115E0C {
    u8 pad0[0x4];
    f32 unk4;
    u8 pad8[0x8];
    s16 unk10;
    u8 pad12[0x2];
    s16 unk14;
    u8 pad16[0x39];
    u8 unk4F;
    u8 pad50[0x2C];
    f32 unk7C;
    f32 unk80;
};

struct C_15115E0C {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
};

struct Obj151163C0 {
    f32 unk0;
    u8 pad4[0xE];
    s16 unk12;
    u8 pad14[0x4];
    f32 unk18;
    u8 pad1C[0x20];
    s32 unk3C;
    u8 pad40[0x1C];
    s16 unk5C;
    u8 pad5E[0x2];
    f32 unk60;
    u8 pad64[0x18];
    s32 unk7C;
    s32 unk80;
};
void func_15115E0C(struct B_15115E0C *arg0, struct C_15115E0C *arg1);
void func_15115EDC(struct S_15115EDC *arg0, struct A_15115EDC *arg1);

struct Obj1511617C {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0x26];
    s32 vel;
    u8 pad40[0xF];
    u8 flags;
    u8 pad50[0xC];
    s16 dy;
    u8 pad5E[0x1E];
    f32 ground;
};

extern u16 D_800CC354;
extern f32 D_800A2FA8;
extern f32 D_800A2FAC;
extern f32 func_1510F648(f32, f32, f32);
void func_1511617C(struct Obj1511617C *arg0);
void func_151162D4(void *arg0);
void func_151163C0(struct Obj151163C0 *arg0);

struct Obj1511650C {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0xF];
    u8 unk4F;
    u8 pad50[0x14];
    f32 unk64;
    u8 pad68[0xC];
    u16 unk74;
    u8 pad76[0x6];
    s32 unk7C;
    s32 unk80;
};

struct Obj15116888;
s32 func_15116888(s32 arg0, s32 arg1, s32 arg2, struct Obj15116888 *arg3);
void func_1511650C(struct Obj1511650C *arg0, s32 arg1, s32 arg2, f32 arg3);

struct Obj15116888 {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
    u8 pad20[0x1C];
    f32 unk3C;
    u8 pad40[0x36];
    u16 unk76;
};

struct Vec15116888 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[0x4];
};

s32 func_150490A8(struct17 *arg0);
s32 func_15116888(s32 arg0, s32 arg1, s32 arg2, struct Obj15116888 *arg3);
void func_15116924(s32 arg0);
void func_15116930(u8 *arg0, u8 *arg1);

struct Obj15116BAC {
    u8 pad0[0x10];
    s16 unk10[3];
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0x1A];
    s16 unk5A[3];
    u8 pad60[0x14];
    u16 unk74;
    u8 pad76[0x6];
    s32 unk7C[3];
};

struct SoundEntry15116BAC {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
};

extern struct SoundEntry15116BAC D_80089260[];
extern struct SoundEntry15116BAC D_80089264[];
extern struct SoundEntry15116BAC D_80089268[];
extern void func_15114D24(struct Obj15116BAC *, s32, s32, s32, s32, s32);
extern void func_151169B4(struct Obj15116BAC *arg0);
extern void func_15116BAC(struct Obj15116BAC *arg0);
void func_15116984(u8 *arg0);
void func_151169B4(struct Obj15116BAC *arg0);
void func_15116BAC(struct Obj15116BAC *arg0);

struct Obj15116D7C {
    f32 unk0[3];
    u8 padC[0x30];
    s32 unk3C;
    u8 pad40[0x20];
    f32 unk60[3];
    u8 pad6C[0x10];
    f32 unk7C[3];
};
void func_15116D7C(struct Obj15116D7C *arg0);

typedef struct {
    u8 frames;
    u8 count;
} Anim15116EA4;

typedef struct {
    s32 start;
    s32 end;
} Rom15116EA4;

extern Anim15116EA4 D_80089320[];
extern Rom15116EA4 D_8002AAD0[];

struct Obj15116EA4 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0xA];
    Vtx *unk20[2];
    u8 pad28[0x14];
    s32 unk3C;
    u8 pad40[0x3C];
    s32 unk7C;
    s16 *unk80;
    s32 unk84;
};
void func_15116EA4(struct Obj15116EA4 *arg0);

typedef struct Motion151172D8 {
    u8 pad0[0x3C];
    s32 packedTarget;
    u8 pad40[0x3C];
    f32 damping;
    f32 acceleration;
    f32 velocity;
} Motion151172D8;

extern f32 D_800A2FD8;
f32 func_151172D8(Motion151172D8 *arg0, f32 angle);

struct Obj151174A0 {
    char pad0[0x8];
    f32 unk8;
};
void func_151174A0(struct Obj151174A0 *arg0);

struct Obj151174C8 {
    char pad0[0x4];
    f32 unk4;
};
void func_151174C8(struct Obj151174C8 *arg0);

struct Obj151174F0 {
    f32 unk0;
};
void func_151174F0(struct Obj151174F0 *arg0);
f32 func_15117518(struct Obj151174F0 *arg0, f32 current);

extern f32 func_15117518(struct Obj151174F0 *, f32);
void func_15117770(struct Obj151174F0 *arg0);

struct Obj15117798 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
};

struct Obj15117DA4 {
    f32 unk0;
    f32 unk4;
    u8 pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x28];
    u16 unk3E;
    u8 pad40[0x14];
    u16 unk54;
};
void func_15117798(struct Obj15117798 *arg0);

struct Obj151177C0 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x3B];
    u8 unk4F;
    u8 pad50[0x4];
    u16 unk54;
    u8 pad56[0x1D];
    u8 unk73;
    u8 pad74[0x8];
    s32 unk7C;
};
extern void func_15104A80(struct Obj151177C0 *);

void func_151177C0(struct Obj151177C0 *arg0) {
    s32 type;
    s32 offset;
    s32 step;
    s32 enable;
    s32 state;
    void (*cb)(struct Obj151177C0 *);
    s32 bit4;
    s16 dest;

    state = arg0->unk73 & 3;
    bit4 = arg0->unk73 & 4;
    type = arg0->unk54;
    offset = 0x28;
    step = 3;
    enable = 0;
    cb = NULL;
    if (type == 0x800B && D_800BE9F0 == 0xC) {
        offset = 0x22;
    } else if (D_800BE9F0 == 0x31) {
        offset = 0x14;
        enable = 1;
    } else if (D_800BE9F0 == 0xA) {
        offset = 0x14;
        enable = 1;
    } else if (D_800BE9F0 == 0x34) {
        offset = 0xA;
        enable = 1;
        step = 6;
        cb = func_15104A80;
    } else if (type == 4) {
        offset = 0xF;
    }
    if (arg0->unk7C == 0) {
        arg0->unk7C = arg0->unk12 | 0x80000000;
    }
    type = (s16)(arg0->unk7C & 0xFFFF);
    if (state == 3) {
        dest = type - offset;
        if (arg0->unk12 != dest) {
            arg0->unk12 = dest;
        }
        if (enable && !(arg0->unk4F & 4) && !bit4) {
            state = 1;
        }
    } else if (state == 0) {
        dest = type - offset;
        if (arg0->unk12 != dest) {
            arg0->unk12 = type;
        }
    } else if (state == 2) {
        dest = type - offset;
        arg0->unk12 -= step;
        if (dest >= arg0->unk12) {
            arg0->unk12 = dest;
            state = 3;
            if (cb != NULL) {
                cb(arg0);
            }
        }
    } else if (state == 1) {
        arg0->unk12 += 3;
        if (arg0->unk12 >= type) {
            arg0->unk12 = type;
            state = 0;
        }
    }
    arg0->unk73 &= ~3;
    arg0->unk73 |= state;
}

s32 func_150AD9A0(s32, s32, s32);
extern f32 D_800A2FDC;
extern f32 D_800A2FE0;
extern f32 D_800A2FE4;
extern f32 D_800A2FE8;
extern f32 D_800A2FEC;
extern f32 D_800A2FF0;
void func_151179BC(void *arg0) {
    s16 x;
    s16 y;
    s16 z;
    s16 speed;
    s32 cur;
    s32 dist;
    f32 fdist;
    f32 m;
    s32 state;
    s32 delay;
    f32 cx;
    f32 sx;
    f32 cy;
    f32 sy;
    f32 cz;
    f32 sz;

    state = *(u8 *)((u8 *)arg0 + 0x73) & 3;
    speed = (*(s32 *)((u8 *)arg0 + 0x3C) >> 16) & 0xFFFF;
    delay = *(s32 *)((u8 *)arg0 + 0x3C) & 0xFFFF;
    if (*(s32 *)((u8 *)arg0 + 0x80) == 0) {
        *(s32 *)((u8 *)arg0 + 0x7C) = (*(s16 *)((u8 *)arg0 + 0x12) << 16) | (*(s16 *)((u8 *)arg0 + 0x10) & 0xFFFF);
        *(s32 *)((u8 *)arg0 + 0x80) = *(s16 *)((u8 *)arg0 + 0x14) | 0x80000000;
    }
    x = *(s32 *)((u8 *)arg0 + 0x7C) & 0xFFFF;
    y = *(s32 *)((u8 *)arg0 + 0x7C) >> 16;
    z = *(s32 *)((u8 *)arg0 + 0x80) & 0xFFFF;
    if (state == 0) {
        *(s16 *)((u8 *)arg0 + 0x10) = x;
        *(s16 *)((u8 *)arg0 + 0x12) = y;
        *(s16 *)((u8 *)arg0 + 0x14) = z;
        return;
    }
    cur = func_150AD9A0(x - *(s16 *)((u8 *)arg0 + 0x10), y - *(s16 *)((u8 *)arg0 + 0x12), z - *(s16 *)((u8 *)arg0 + 0x14));
    if (state == 3) {
        if (speed < 0) {
            dist = -speed;
        } else {
            dist = speed;
        }
        if ((delay != 0) && !(*(u8 *)((u8 *)arg0 + 0x4F) & 4) && !(*(u8 *)((u8 *)arg0 + 0x73) & 4)) {
            if (D_800BE9E4 < *(s32 *)((u8 *)arg0 + 0x84)) {
                *(s32 *)((u8 *)arg0 + 0x84) -= D_800BE9E4;
            } else {
                state = 1;
            }
        } else {
            *(s32 *)((u8 *)arg0 + 0x84) = delay;
        }
    } else if (state == 2) {
        dist = cur + 6;
        if (dist >= ((speed < 0) ? -speed : speed)) {
            state = 3;
            if (speed < 0) {
                dist = -speed;
            } else {
                dist = speed;
            }
            *(s32 *)((u8 *)arg0 + 0x84) = delay;
        }
    } else if (state == 1) {
        if (cur < 3) {
            dist = 0;
            state = 0;
        } else {
            dist = cur - 3;
        }
    }
    if (cur != dist) {
        cx = cosf(*(f32 *)((u8 *)arg0 + 0) * D_800A2FDC);
        sx = sinf(*(f32 *)((u8 *)arg0 + 0) * D_800A2FE0);
        cy = cosf(*(f32 *)((u8 *)arg0 + 4) * D_800A2FE4);
        sy = sinf(*(f32 *)((u8 *)arg0 + 4) * D_800A2FE8);
        cz = cosf(*(f32 *)((u8 *)arg0 + 8) * D_800A2FEC);
        sz = sinf(*(f32 *)((u8 *)arg0 + 8) * D_800A2FF0);
        if (speed < 0) {
            dist = -dist;
        }
        fdist = dist;
        m = cx * sy;
        *(s16 *)((u8 *)arg0 + 0x10) = x + fdist * ((m * cz) + (sx * sz));
        *(s16 *)((u8 *)arg0 + 0x12) = y + fdist * ((m * sz) - (sx * cz));
        *(s16 *)((u8 *)arg0 + 0x14) = z + fdist * cx * cy;
    }
    *(u8 *)((u8 *)arg0 + 0x73) &= ~3;
    *(u8 *)((u8 *)arg0 + 0x73) |= state;
}

void func_15117D3C(struct Obj151152A8 *arg0, struct127 *arg1) {
    s32 temp = arg0->field_0x3C & 0xFFFF;
    if (arg1->interaction_state == 1) {
        if (arg0->field_0x4F & 0x4) {
            if (!(arg0->field_0x73 & 0x3) && !(arg0->field_0x73 & 0x4)) {
                if (temp == 0 || arg1->unk31C->unk57 == 1) {
                    arg0->field_0x73 &= 0xFFFC;
                    arg0->field_0x73 |= 0x2;
                }
            }
        }
    }
}

void func_15117DA4(struct Obj15117DA4 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4) {
    f32 angle;
    f32 s;
    f32 c;
    f32 temp_z;
    f32 temp_x;

    if (arg0->unk54 == 0x8006) {
        if (D_800BE9F0 == 0x35) {
            arg0 = (struct Obj15117DA4 *)func_151149AC(0xFD);
            func_1511F31C((struct Struct1511F31CArg *)arg0);
            arg2 = D_800A2FF4;
            arg1 = 0.0f;
            arg3 = 0.0f;
        }
    }

    angle = arg0->unk0 * D_800A2FF8;
    s = sinf(angle);
    c = cosf(angle);
    arg4[0] = arg1;
    arg4[1] = (arg2 * c) - (arg3 * s);
    arg4[2] = (arg2 * s) + (arg3 * c);

    angle = arg0->unk4 * D_800A2FFC;
    s = sinf(angle);
    c = cosf(angle);
    temp_x = arg4[0];
    temp_z = arg4[2];
    arg4[0] = (temp_z * s) + (temp_x * c);
    arg4[2] = (temp_z * c) + (-temp_x * s);

    arg4[0] = *(volatile f32 *)&arg4[0] + arg0->unk10;
    arg4[1] = *(volatile f32 *)&arg4[1] + arg0->unk12;
    arg4[2] = *(volatile f32 *)&arg4[2] + arg0->unk14;
}

struct F7Cw {
    u8 p0[0x17];
    u8 unk17;
    u8 p18[0xF];
    u8 unk27;
    u8 p28[0x43];
    u8 unk6B;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
};

struct F7Tg {
    s32 p0;
    u8 p4[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 p20[0x45];
    u8 unk65;
    u8 p66[0x80];
    s16 unkE6;
    u8 pE8[0x1C];
    u8 unk104;
    u8 p105[0x216];
    struct F7Cw *w;
    u8 p320[0xC];
};

struct F7O {
    f32 unk0;
    u8 p4[0x28];
    f32 ox;
    f32 oy;
    f32 oz;
    u8 p38[0x17];
    u8 unk4F;
    u8 p50[0x23];
    u8 unk73;
    u16 unk74;
    u8 p76[6];
    s32 unk7C;
    f32 unk80;
    f32 unk84;
};

extern void func_15060A30(s32, void *);

void func_15117F3C(struct F7O *o) {
    f32 limit;
    s32 d1;
    f32 f84;
    f32 f80;
    struct F7Tg *p;
    struct F7Cw *w;
    s32 i;
    f32 dx;
    f32 dy;
    s32 flag8;
    s32 sound;
    f32 thr;
    s32 d2;
    struct F7Tg *target;
    s32 d3;
    f32 x;
    f32 y;
    f32 z;
    f32 pos[3];
    s32 d4;
    s32 mode;
    s32 t1;
    s32 flag4;
    s32 timer;
    f32 dz;
    f32 dist;

    limit = 40.0;
    thr = 40000.0f;
    sound = 0x4B8;
    mode = o->unk73 & 3;
    flag4 = o->unk73 & 4;
    flag8 = o->unk73 & 8;
    switch (D_800BE9F0) {
    case 1:
    case 41:
        x = -13.0f;
        y = 76.0f;
        z = 354.0f;
        thr = 10000.0f;
        break;
    case 11:
        x = 70.0f;
        y = 120.0f;
        z = 329.0f;
        sound = 0x4BA;
        flag8 = 8;
        break;
    case 23:
        x = 0.0f;
        y = 125.0f;
        z = 322.0f;
        break;
    case 40:
        limit = 55.0f;
        x = 0.0f;
        y = 99.0f;
        z = 168.0f;
        flag8 = 8;
        break;
    case 53:
        limit = 20.0f;
        x = 0.0f;
        y = -328.0f;
        z = 549.0f;
        break;
    case 47:
        limit = 90.0f;
        thr = 5625.0f;
        x = 0.0f;
        y = 61.0f;
        z = 104.0f;
        flag8 = 8;
        break;
    case 60:
        limit = 30;
        thr = 10000.0f;
        x = 22.0f;
        y = 13.0f;
        z = 419.0f;
        flag8 = 8;
        break;
    case 65:
        limit = 45.0f;
        if (flag4 == 4) {
            thr = -1.0f;
        }
        x = 0.0f;
        y = 25.0f;
        z = 336.0f;
        flag8 = 8;
        break;
    case 64:
        limit = 45.0f;
        x = 0.0f;
        y = 47.0f;
        z = 72.0f;
        flag8 = 8;
        thr = 10000.0f;
        break;
    default:
        return;
    }
    x *= o->ox;
    y *= o->oy;
    z *= o->oz;
    thr *= o->oz * o->oz;
    if (o->unk7C == 0) {
        o->unk7C = 2;
        if (mode == 3) {
            o->unk0 = limit;
        }
    }
    timer = (o->unk7C >> 8) & 0xFF;
    f80 = o->unk80;
    f84 = o->unk84;
    t1 = o->unk7C & 1;
    if (t1) {
    }
    func_15117DA4((struct Obj15117DA4 *)o, x, y, z, pos);
    target = 0;
    if (mode != 3 && mode != 1) {
        for (i = 0; i < D_8008FD8C; i++) {
            p = &((struct F7Tg *)D_800CC2D0)[i];
            if (p->unk65) {
                continue;
            }
            if (p->unk104) {
                continue;
            }
            w = p->w;
            if (w->unk17) {
                continue;
            }
            if (w->unk27) {
                continue;
            }
            if (w->unk6B >= 4) {
                continue;
            }
            if (p->y + 40.0f < pos[1]) {
                dy = (p->y + (f32)p->unkE6) - pos[1];
                dz = p->z - pos[2];
                dx = p->x - pos[0];
                dist = dz * dz + (dx * dx + dy * dy);
                if (dist) {
                }
                if (dist < thr) {
                    target = p;
                    break;
                }
            }
        }
    }
    if (target == 0 && mode == 2) {
        for (i = 0; i < D_8008FD8C; i++) {
            p = &((struct F7Tg *)D_800CC2D0)[i];
            if (p->w->unk6B != 0) {
                target = p;
                break;
            }
        }
    }
    if (target != 0 || mode == 2) {
        if (target != 0 && t1 == 0) {
            func_15060A30(0x16B, target);
        }
        if (flag4 == 0) {
            mode = 2;
            if (t1 == 0) {
                timer = 0x32;
                t1++;
                f80 = 0.0f;
            }
            if (target != 0) {
                if (timer != 0) {
                    target->w->unk6B = 1;
                } else {
                    target->w->unk6B = 2;
                }
            }
            if (timer != 0) {
                if (D_800BE9E4 < timer) {
                    timer -= D_800BE9E4;
                } else {
                    timer = 0;
                }
            } else {
                f84 += f80;
                if (30.0f < f84) {
                    f84 = 30.0f;
                }
                f80 += 0.02999999933f;
                if (0.200000003f < f80) {
                    if (o->unk74 == 0) {
                        func_15114D24((struct Obj15116BAC *)o, sound, 0x7D00, 0x5DC, 0xDAC, 0);
                    }
                }
                o->unk0 += f84;
                if (limit <= o->unk0) {
                    o->unk0 = limit;
                    f84 = 0.0f;
                    f80 = 0.0f;
                    mode = 3;
                    t1 = 0;
                    func_15114D24((struct Obj15116BAC *)o, 0x14, 0x7D00, 0x5DC, 0xDAC, 4);
                    if (target != 0) {
                        target->w->unk6B = 0;
                    }
                }
            }
            if (target != 0) {
                func_15117DA4((struct Obj15117DA4 *)o, x, y, z, pos);
                target->w->unk6C = (s16)pos[0];
                target->w->unk6E = (s16)pos[1];
                target->w->unk70 = (s16)pos[2];
            }
        } else {
            if (target != 0 && (w = target->w) != 0) {
                if (t1 == 0 && w->unk6B == 0) {
                    timer = 0x32;
                    t1++;
                }
                if (timer != 0) {
                    w->unk6B = 3;
                } else if (w->unk6B == 3) {
                    w->unk6B = 4;
                    t1 = 0;
                }
                if (timer != 0) {
                    if (D_800BE9E4 < timer) {
                        timer -= D_800BE9E4;
                    } else {
                        timer = 0;
                    }
                }
            }
        }
        if (target != 0) {
            if (target->w != 0) {
                func_15117DA4((struct Obj15117DA4 *)o, x, y, z, pos);
                target->w->unk6C = (s16)pos[0];
                target->w->unk6E = (s16)pos[1];
                target->w->unk70 = (s16)pos[2];
            }
        }
    } else if (flag8 == 0 || mode == 1) {
        if (0.0f < o->unk0) {
            if (t1 == 0) {
                f80 = 0.1000000015f;
                t1++;
            }
            mode = 1;
            f84 += 0.1000000015f;
            if (30.0f < f84) {
                f84 = 30.0f;
            }
            f80 += 0.05000000075f;
            o->unk0 = o->unk0 - f84;
            if (o->unk0 <= 0.0f) {
                o->unk0 = 0.0f;
                t1 = 0;
                mode = 0;
                f80 = 0.0f;
                f84 = 0.0f;
            }
        }
        if (target != 0) {
            target->w->unk6B = 0;
        }
    }
    o->unk73 &= 0xFFFC;
    o->unk73 |= mode;
    o->unk7C &= 0xFE;
    o->unk7C |= timer << 8;
    o->unk7C |= t1;
    o->unk80 = f80;
    o->unk84 = f84;
    o->unk4F &= 0xFF9F;
    if (mode == 2 || (mode == 0 && timer != 0)) {
        o->unk4F |= 0x20;
    } else {
        o->unk4F |= 0;
    }
}

s32 func_151189AC(void *arg0, s32 arg1);

s32 func_1511896C(void *arg0) {
    return func_151189AC(arg0, 0);
}

s32 func_1511898C(void *arg0) {
    return func_151189AC(arg0, 3);
}

extern void func_15173C60(s32, s32);
extern void func_15173C90(s32, s32, s32);
extern void func_1518804C(s32, f32);
extern void func_1511F990(void *, s32);
extern s32 func_15022B08(s32, s32);
extern struct SoundEntry15116BAC D_80089262[];
extern f32 D_800A3138;
extern f32 D_800A313C;
extern f32 D_800A3140;
extern f32 D_800A3144;

s32 func_151189AC(void *arg0, s32 arg1) {
    f32 speed;
    f32 target;
    f32 angle;
    f32 remaining;
    f32 damping;
    f32 accel;
    s32 mode;
    s32 index;
    s32 openAngle;
    s32 row;
    s32 sound;
    s32 slow;
    f32 scale;

    sound = 0;
    slow = 0;
    func_1511F990(arg0, arg1);
    row = (*(s32 *)(((u8 *)arg0) + 0x3C) & 0xFC00) >> 10;
    openAngle = (*(s32 *)(((u8 *)arg0) + 0x3C) << 22) >> 22;
    mode = ((u8 *)arg0)[0x73] & 3;
    target = openAngle;
    angle = *(f32 *)(((u8 *)arg0) + 4);
    speed = *(f32 *)(((u8 *)arg0) + 0x84);
    if (mode == 0 || mode == 1) {
        target = 0.0f;
        if (mode == 1 && (((u8 *)arg0)[0x73] & 4)) {
            slow = 1;
        }
    }
    if (mode == 0 || mode == 3) {
        speed = 0.0f;
        angle = target;
    }
    if (angle != target || speed != 0.0f) {
        if (*(f32 *)(((u8 *)arg0) + 0x7C) == 0.0f) {
            if (slow) {
                *(f32 *)(((u8 *)arg0) + 0x7C) = D_800A3138;
            } else {
                *(f32 *)(((u8 *)arg0) + 0x7C) = D_800A313C;
            }
        }
        damping = *(f32 *)(((u8 *)arg0) + 0x7C);
        if (*(f32 *)(((u8 *)arg0) + 0x80) == 0.0f) {
            if (slow) {
                *(f32 *)(((u8 *)arg0) + 0x80) = D_800A3140;
            } else {
                *(f32 *)(((u8 *)arg0) + 0x80) = D_800A3144;
            }
        }
        accel = *(f32 *)(((u8 *)arg0) + 0x80);
        if (speed == 0.0f) {
            if (target == 0.0f) {
                sound = D_80089262[row].unk0;
            } else {
                sound = D_80089260[row].unk0;
            }
        }
        angle += speed;
        remaining = func_15048A70(angle, target);
        if (fabsf(remaining) < fabsf(speed)) {
            speed = -speed * damping;
            if (fabsf(speed) < 1.0f) {
                speed = 0.0f;
                angle = target;
                if (target == 0.0f) {
                    mode = 0;
                    sound = D_80089260[row].unk6;
                } else {
                    mode = 3;
                    sound = D_80089260[row].unk4;
                }
            } else {
                accel *= 0.5f;
                *(f32 *)(((u8 *)arg0) + 0x80) = accel;
            }
        } else if (remaining > 0.0f) {
            speed += accel;
            if (speed > 5.0f) {
                speed = 5.0f;
            }
        } else {
            speed -= accel;
            if (speed < -5.0f) {
                speed = -5.0f;
            }
        }
        if ((remaining < 0 ? -remaining : remaining) > 30.0f && remaining * speed < 0.0f) {
            speed = 0.0f;
        }
        if (angle < -180.0f) {
            angle += 360.0f;
        } else if (angle >= 180.0f) {
            angle -= 360.0f;
        }
        ((u8 *)arg0)[0x73] &= ~3;
        ((u8 *)arg0)[0x73] |= mode;
        if (sound != 0) {
            func_15114D24((struct Obj15116BAC *)arg0, sound, 0x5DC0, 0x9C4, 0x1194, 0);
        }
    }
    *(f32 *)(((u8 *)arg0) + 0x84) = speed;
    if (D_800BE9B4 != 0 || angle != *(f32 *)(((u8 *)arg0) + 4)) {
        if (D_800C35EA == 1 && func_15022B08(((s32)arg0 - (s32)D_800DBEF4) / 160, 0) != 0) {
            angle = *(f32 *)(((u8 *)arg0) + 4);
        } else {
            *(f32 *)(((u8 *)arg0) + 4) = angle;
        }
        index = (*(s32 *)(((u8 *)arg0) + 0x3C) >> 24) & 0xFF;
        if (D_800BE9F0 == 2) {
            func_1000E75C(255.0f - fabsf(((angle - (f32)openAngle) * 255.0f) / (f32)openAngle));
            return;
        }
        if (index != 0) {
            scale = 1.0f - fabsf((fabsf(angle) - (openAngle < 0 ? -openAngle : openAngle)) / (f32)openAngle);
            func_1518804C(index - 1, scale);
            func_15173C60(scale * 255.0f, index - 1);
            if (D_800BE9F0 == 1 && index - 1 == 0) {
                func_15173C90(scale * 255.0f, 0, 0xF8);
            }
        }
    }
}

extern void func_1511F990(void *, s32);
extern void func_15173C60(s32, s32);
extern void func_1518804C(s32, f32);
extern void func_151669A0(s32, s32, s32, f32, u8, s32);
extern f32 D_800A3148;
extern f32 D_800A314C;
extern f32 D_800A3150;
extern f32 D_800A3154;
extern f32 D_800A3158;
void func_15118F24(u8 *arg0) {
    f32 speed;
    f32 target;
    f32 angle;
    f32 remaining;
    f32 damping;
    f32 accel;
    s32 mode;
    s32 index;
    s32 openAngle;
    f32 scale;

    func_1511F990(arg0, 1);
    mode = arg0[0x73] & 3;
    openAngle = *(s16 *)(arg0 + 0x3E);
    target = openAngle;
    angle = *(f32 *)(arg0 + 8);
    speed = *(f32 *)(arg0 + 0x84);
    if (mode == 0 || mode == 1) {
        target = 0.0f;
    }
    if (mode == 0 || mode == 3) {
        speed = 0.0f;
        angle = target;
    }
    if (angle != target || speed != 0.0f) {
        if (*(f32 *)(arg0 + 0x7C) == 0.0f) {
            *(f32 *)(arg0 + 0x7C) = D_800A3148;
        }
        damping = *(f32 *)(arg0 + 0x7C);
        if (*(f32 *)(arg0 + 0x80) == 0.0f) {
            *(f32 *)(arg0 + 0x80) = D_800A314C;
        }
        if (angle > 90.0f) {
            accel = *(f32 *)(arg0 + 0x80) * (180.0f - angle);
        } else {
            accel = *(f32 *)(arg0 + 0x80) * angle;
        }
        if (speed == 0.0f) {
            if (target == 0.0f) {
                accel += D_800A3150;
            } else {
                func_10010F88(0x4BA, 0x5DC0, 0, 0, 0, *(s16 *)(arg0 + 0x10), *(s16 *)(arg0 + 0x12),
                              *(s16 *)(arg0 + 0x14), 0xC8, 0x9C4);
                accel += D_800A3154;
            }
        }
        angle += speed;
        remaining = func_15048A70(angle, target);
        if (fabsf(remaining) < fabsf(speed)) {
            speed = -speed * damping;
            if (fabsf(speed) < *(f32 *)(arg0 + 0x80) * D_800A3158) {
                speed = 0.0f;
                angle = target;
                if (target == 0.0f) {
                    mode = 0;
                } else {
                    mode = 3;
                    func_10010F88(0x4BB, 0x5DC0, 0, 0, 0, *(s16 *)(arg0 + 0x10),
                                  *(s16 *)(arg0 + 0x12), *(s16 *)(arg0 + 0x14), 0xC8, 0x9C4);
                }
            } else {
                *(f32 *)(arg0 + 0x80) *= 0.5f;
            }
            func_151669A0(*(s16 *)(arg0 + 0x10), *(s16 *)(arg0 + 0x12), *(s16 *)(arg0 + 0x14), 0.15f,
                          0xFF, 0);
        } else if (remaining > 0.0f) {
            speed += accel;
            if (speed > 10.0f) {
                speed = 10.0f;
            }
        } else {
            speed -= accel;
            if (speed < -10.0f) {
                speed = -10.0f;
            }
        }
        if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        arg0[0x73] &= ~3;
        arg0[0x73] |= mode;
    }
    *(f32 *)(arg0 + 0x84) = speed;
    if (D_800BE9B4 != 0 || angle != *(f32 *)(arg0 + 8)) {
        *(f32 *)(arg0 + 8) = angle;
        index = (*(s32 *)(arg0 + 0x3C) >> 24) & 0xFF;
        if (index != 0 && openAngle != 0) {
            scale = 1.0f - fabsf((angle - (f32)openAngle) / (f32)openAngle);
            func_1518804C(index - 1, scale);
            func_15173C60(scale * 255.0f, index - 1);
        }
    }
}

struct Obj151193AC_0 {
    char pad0[0x73];
    u8 unk73;
};

struct Obj151193AC_1 {
    s32 unk0;
    char pad4[0x61];
    u8 unk65;
};

void func_151193AC(struct Obj151193AC_0 *arg0, struct Obj151193AC_1 *arg1) {
    if (arg1->unk0 == 1) {
        if (arg1->unk65 == 0) {
            if (!(arg0->unk73 & 0x3) && !(arg0->unk73 & 0x4)) {
                arg0->unk73 &= 0xFFFC;
                arg0->unk73 |= 0x2;
            }
        }
    }
}

struct Obj151193F4 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x28];
    s32 unk3C;
    u8 pad40[0x33];
    u8 unk73;
    u8 pad74[0x8];
    s32 unk7C;
    u32 unk80;
    s32 unk84;
};

void func_151193F4(struct Obj151193F4 *arg0) {
    s32 speed;
    s32 target;
    s32 diff;
    s32 scale;
    s32 base;
    u32 mul;

    if (arg0->unk84 == 0) {
        arg0->unk84 = arg0->unk12 | 0x80000000;
    }
    speed = arg0->unk7C;
    if (speed != 0) {
        mul = arg0->unk80;
        base = (s16)(arg0->unk84 & 0xFFFF);
        scale = (u16)(arg0->unk3C >> 16);
        target = (s32)(scale * mul) / 256 + base;
        arg0->unk73 &= ~3;
        if (target != arg0->unk12) {
            diff = arg0->unk12 - target;
            if (diff < 0) {
                if (-diff < speed) {
                    arg0->unk12 = target;
                } else {
                    arg0->unk12 = arg0->unk12 + speed;
                }
            } else {
                if (diff < speed) {
                    arg0->unk12 = target;
                } else {
                    arg0->unk12 = arg0->unk12 - speed;
                }
            }
            /* state bits = 0 (moving): the original keeps this RMW, golden has lbu/sb here */
            arg0->unk73 |= 0;
        } else {
            arg0->unk73 |= 3;
        }
    }
}


struct Obj151194D4Node {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
    char pad8[8];
};

struct Obj151194D4Cmd {
    s8 unk0;
    char pad1[3];
    u32 unk4;
};

struct Obj151194D4 {
    char pad0[0x16];
    u16 unk16;
    char pad18[4];
    struct Obj151194D4Cmd *unk1C;
    char pad20[8];
    struct Obj151194D4Node *unk28;
    char pad2C[0x28];
    u16 unk54;
    char pad56[0x2E];
    s32 unk84;
};

void func_151194D4(struct Obj151194D4 *arg0, struct Obj151194D4Node *arg1, s32 arg2, s32 arg3) {
    struct Obj151194D4Cmd *cmd;
    struct Obj151194D4Node *node;
    s32 base;
    s32 found;
    u32 i;
    u32 word;
    u32 bits;

    cmd = arg0->unk1C;
    while (cmd->unk0 != -0x21) {
        switch (cmd->unk0) {
            case 1:
                base = (cmd->unk4 & 0xFFFFFF) >> 4;
                break;
            case 5:
                word = cmd->unk4;
                found = 0;
                bits = word;
                for (i = 0; i < 3; i++) {
                    node = &arg0->unk28[(bits & 0xFF) / 10 + base];
                    if ((arg1->unk0 == node->unk0) && (arg1->unk2 == node->unk2) && (arg1->unk4 == node->unk4)) {
                        found = 1;
                        node->unk6 = arg3;
                    } else {
                        bits >>= 8;
                    }
                }
                if (found) {
                    bits = word;
                    for (i = 0; i < 3; i++) {
                        node = &arg0->unk28[(bits & 0xFF) / 10 + base];
                        if (node->unk6 == 0) {
                            if (arg2 < ((node->unk4 * node->unk4) + (node->unk0 * node->unk0))) {
                                node->unk6 = arg3;
                                func_151194D4(arg0, node, arg2, arg3);
                            }
                        }
                        bits >>= 8;
                    }
                }
                break;
        }
        cmd++;
    }
}

extern f32 func_150489B0(u8);
extern f32 D_800A315C;

void func_151196D4(struct Obj151194D4 *arg0) {
    struct Obj151194D4Node *nodes;
    struct Obj151194D4Node *node;
    f32 sn;
    f32 cs;
    f32 scale;
    s32 range;
    s32 count;
    s32 i;
    s32 sumX;
    s32 sumZ;
    s32 angle;
    s32 temp;
    u8 back;

    node = arg0->unk28;
    range = 0x64;
    if ((arg0->unk54 == 0x21) || (arg0->unk54 == 0x22)) {
        range = 0x190;
    }
    arg0->unk84 = 1;
    count = 0;
    for (i = 0; i < arg0->unk16; i++) {
        if (node->unk6 == 0) {
            temp = node->unk0;
            temp = (node->unk4 * node->unk4) + (temp * temp);
            if (range < temp) {
                count++;
                node->unk6 = count;
                func_151194D4(arg0, node, range, count);
            }
        }
        node++;
    }

    if (count != 0) {
        scale = D_800A315C;
        do {
            nodes = arg0->unk28;
            node = nodes;
            sumX = 0;
            sumZ = 0;
            for (i = 0; i < arg0->unk16; i++) {
                if (count == node->unk6) {
                    sumX += node->unk0;
                    sumZ += node->unk4;
                }
                node++;
            }
            if ((sumX != 0) || (sumZ != 0)) {
                node = nodes;
                angle = (u8)(s32)(func_150484A0((f32)sumX, (f32)sumZ) * scale);
                if (count < angle) {
                    back = -angle;
                    cs = func_15048A40(back);
                    sn = func_150489B0(back);
                    for (i = 0; i < arg0->unk16; i++) {
                        if (count == node->unk6) {
                            temp = node->unk0;
                            node->unk6 = (((count - 1) & 3) << 8) | angle;
                            node->unk0 = (node->unk4 * cs) + (temp * sn);
                            node->unk4 = (node->unk4 * sn) - (temp * cs);
                        }
                        node++;
                    }
                }
            }
            count--;
        } while (count != 0);
    }
}

typedef struct {
    s16 x, y, z;
    u16 group;
    u8 pad8[8];
} Game144C70Vertex;

typedef struct {
    u8 pad0[4];
    f32 unk4;
    u8 pad8[8];
    s16 x, y, z;
    u16 count;
    u8 pad18[8];
    Game144C70Vertex *outputs[2];
    Game144C70Vertex *vertices;
    f32 scaleX, scaleY, scaleZ;
    u8 pad38[0x1C];
    u16 kind;
    u8 pad56[0x26];
    s32 timer;
    f32 phase;
    s32 active;
} Game144C70VertexState;

extern s32 func_151EF610(void);
struct127 *func_1505EFD0(s32);
extern f32 D_800A3160;
extern f32 D_800A3164;
extern f32 D_800A3168;
extern f32 D_800A316C;

void func_15119938(Game144C70VertexState *arg0) {
    f32 dist;
    Game144C70Vertex *vertex;
    Game144C70Vertex *out;
    s32 offset;
    s32 i;
    f32 *pair;
    struct127 *player;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 x;
    f32 y;
    f32 z;
    f32 sine;
    f32 rot[4][2];
    f32 rotCos;
    f32 rotSin;
    f32 cosine;
    f32 angle;

    vertex = arg0->vertices;
    out = arg0->outputs[D_800BE9C0];
    posX = arg0->x;
    posY = arg0->y;
    posZ = arg0->z;
    scaleX = arg0->scaleX;
    scaleY = arg0->scaleY;
    scaleZ = arg0->scaleZ;
    offset = arg0->unk4 * D_800A3160;
    if (arg0->active == 0) {
        func_151196D4((struct Obj151194D4 *)arg0);
    }
    if (arg0->timer >= 0x400) {
        arg0->phase += ((f32)(arg0->timer - 0x400) - arg0->phase) * D_800A3164 * D_800BE9A4;
        if (arg0->timer != 0x420) {
            arg0->timer--;
            if (arg0->timer == 0x400) {
                arg0->timer = 0;
                arg0->phase = 0.0f;
            }
        }
    } else {
        arg0->phase += (0.01953125f + 0.00390625f * (func_151EF610() % 6)) * D_800BE9A4;
        arg0->timer -= func_151EF610() % 3;
        if (arg0->timer < 0x14) {
            arg0->timer = 0xEC;
        }
    }
    for (i = 0; i < 4; i++) {
        if (arg0->timer >= 0x400) {
            rot[i][0] = func_15048A40(arg0->phase);
            rot[i][1] = func_150489B0(arg0->phase);
        } else {
            angle = arg0->phase * D_800A3168 - D_800A316C * i;
            if (angle <= 0.0f) {
                angle = 5.0f;
            } else {
                sine = func_15048A40(arg0->timer >> 1);
                angle = sinf(angle) * (2.0f * sine) + 5.0f;
            }
            rot[i][0] = func_15048A40(angle);
            rot[i][1] = func_150489B0(angle);
        }
    }
    if (arg0->timer != 0x420) {
        player = func_1505EFD0(0);
        if (player != NULL) {
            x = player->x_position - arg0->x;
            y = player->y_position - arg0->y;
            z = player->z_position - arg0->z;
            dist = x * x + y * y + z * z;
            if (dist < 3072.0f) {
                if (player->unk31C->unk57 == 1) {
                    arg0->timer = 0x420;
                } else {
                    arg0->timer = 0x41F;
                }
            }
        }
    }
    for (i = 0; i < arg0->count; i++) {
        if (vertex->group != 0) {
            sine = func_15048A40(vertex->group + offset);
            cosine = func_150489B0(vertex->group + offset);
            pair = rot[vertex->group >> 8];
            rotCos = pair[0];
            rotSin = pair[1];
            z = vertex->y * rotCos + vertex->z * rotSin;
            y = vertex->y * rotSin - vertex->z * rotCos;
            if (y < 1.0f) {
                z -= y;
                y = 1.0f;
            }
            x = vertex->x * cosine + z * sine;
            z = z * cosine - vertex->x * sine;
        } else {
            x = vertex->x;
            y = vertex->y;
            z = vertex->z;
        }
        out->x = x * scaleX + posX;
        out->y = y * scaleY + posY;
        out->z = z * scaleZ + posZ;
        out++;
        vertex++;
    }
}

typedef struct Obj15119FC0 {
    f32 x, y, z;
    u8 pad0C[4];
    s16 position[3];
    u8 pad16[0x26];
    s32 flags;
    u8 pad40[0xF];
    s8 state;
    u8 pad50[0x1E];
    s8 removed;
    u8 pad6F;
    u8 flags70;
    u8 pad71[0xB];
    f32 phase;
    s32 baseHeight;
    s32 timer;
    u8 pad88[2];
    u8 alpha;
} Obj15119FC0;


extern s32 func_15060BA4(void *, s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_1508EE0C(s32, s32);
extern void func_151D69B4(void *, void *);
void func_15119FC0(Obj15119FC0 *arg0);

typedef struct {
    s32 *unk0;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[3];
} Struct1511A494Entry;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[0x34];
    u16 unk54;
} Struct1511A494Arg;

extern Struct1511A494Entry D_80089324[];
extern Struct1511A494Entry D_80089444[];
extern s32 func_1511A410(s32, s32 *);
extern s32 func_1510D0EC(s32, s32 *, s32, s32);
extern void func_1510D874(void *, s32, s32, s32, s32);
void func_1511A494(Struct1511A494Arg *arg0, s32 *arg1, s32 *arg2);

extern void func_15116110(void *);
void func_1511A6FC(s32 *arg0);

extern u8 D_800C35EA;
extern struct131 *D_800DBEF4;
extern s32 func_15022B08(s32, s32);
extern void func_151162D4(void *);
void func_1511A738(struct131 *arg0);

struct S_1511A7C0 {
    u8 pad0[0x4];
    f32 *unk4;
    u8 pad8[0x8];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
};
struct A_1511A7C0 {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[0x68];
    struct S_1511A7C0 *unk80;
};
void func_1511A7C0(struct A_1511A7C0 *arg0);

typedef struct {
    u8 *bytes;
    f32 *values;
    f32 *rates;
    f32 *scales;
    f32 field_10;
    f32 field_14;
    f32 field_18;
    u8 field_1C;
    u8 field_1D;
    u8 field_1E;
    s32 field_20;
    s32 field_24;
} Game147C70Output;

typedef struct {
    u8 pad0[0x16];
    u16 value_count;
    u8 pad18[8];
    Vtx_t *lists[2];
    Vtx_t *vertices;
    u8 pad2C[0x10];
    s32 packed3C;
    u8 pad40[0x33];
    u8 flags73;
    u8 pad74[0xC];
    Game147C70Output *output;
    s32 packed84;
} Game147C70State;

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
void func_1511A838(Game147C70State *arg0);

struct Obj1511AF30 {
    f32 pos;
    u8 pad4[0x38];
    s32 level;
    u8 pad40[0x3C];
    f32 base;
    f32 wobble;
    f32 wobbleTarget;
};

extern f32 D_800A3188;
extern f32 D_800A318C;
extern f32 D_800A3190;
extern f32 D_800A3194;
void func_1511AF30(struct Obj1511AF30 *arg0);

struct Vtx1511B7D4 {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[0xA];
};

struct Obj1511B7D4 {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x4];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0xA];
    struct Vtx1511B7D4 *unk20[2];
    struct Vtx1511B7D4 *unk28;
    u8 pad2C[0x10];
    s32 unk3C;
    u8 pad40[0x3C];
    s32 unk7C;
    f32 unk80;
    f32 unk84;
    u8 pad88[0x18];
};

extern u8 D_800A2F70;
extern u8 D_800A2F71[];
void func_1511B7D4(struct Obj1511B7D4 *arg0);


struct Obj1511BA24 {
    u8 pad0[0x3C];
    s32 unk3C;
    u8 pad40[0x4A];
    u8 unk8A;
};
void func_1511BA24(struct Obj1511BA24 *arg0);

typedef struct Game1483E0UvVertex {
    s16 x, y, z;
    u8 unknown06[2];
    s16 s, t;
    u32 unknown0C;
} Game1483E0UvVertex;

typedef struct Game1483E0UvPair {
    s16 s, t;
} Game1483E0UvPair;

typedef struct Game1483E0State {
    u8 unknown00[0x10];
    s16 x, y, z;
    u16 count;
    u8 unknown18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 unknown2C[0x13];
    u8 reference_id;
    u8 unknown40[0x3C];
    Game1483E0UvPair *coords;
    void *reference;
} Game1483E0State;

extern f32 D_800A31D0;
extern f32 D_800A31D4;
extern f32 D_800A31D8;
extern f32 D_800A31DC;
void func_1511BB04(Game1483E0State *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);

struct Obj1511BDF4 {
    u8  pad0[0x3F];
    u8  unk3F;
    u8  pad40[0x40];
    struct127 *unk80;
};

extern void func_1511BB04(Game1483E0State *, f32, f32, f32, f32);
void func_1511BDF4(struct Obj1511BDF4 *arg0);

struct Obj1511BE5C {
    u8  pad0[0x4];
    f32 unk4;
    u8  pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
};

extern f32 D_800A31E0;
void func_1511BE5C(struct Obj1511BE5C *arg0);
void func_1511C540(void);

struct Obj1511C548 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[0x74];
    f32 unk80;
    f32 unk84;
};

extern f32 D_800A31EC;
extern f32 D_800A31F0;
void func_1511C548(struct Obj1511C548 *arg0);


struct Obj1511C638 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0x2E];
    u8 unk6E;
    u8 pad6F[4];
    u8 unk73;
    u8 pad74[8];
    s32 unk7C;
    s32 unk80;
    u8 pad84[6];
    u8 unk8A;
    u8 pad8B[0x15];
};

extern void func_15170F4C(struct Obj1511C638 *, u8, s32);
extern void func_150B2740(u8, u8);
extern s32 func_1000EC24();
void func_1511C638(struct Obj1511C638 *arg0);

extern f32 D_800A31F4;
void func_1511CB2C(s32 arg0, f32 *arg1);

struct Wave1511CB44 {
    u8 *phases;
    u16 phase;
    u16 speed;
    f32 amp;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 angle;
    s16 (*uv)[2];
};

struct Obj1511CB44 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u16 unk16;
    u8 pad18[0x8];
    Vtx *unk20[2];
    Vtx *unk28;
    u8 pad2C[0x46];
    u8 unk72;
    u8 pad73[0x9];
    struct Wave1511CB44 *unk7C;
    u8 pad80[0xA];
    u8 unk8A;
};

extern s32 D_800902D8;
extern s32 func_15195FB0(void *, s32, s32, s32, s32, s32, s32);
extern f32 func_150489B0(u8);
extern void func_15094AB8(Vtx *arg0, s16 (*arg1)[2], s32 arg2, f32 arg3, s32 arg4, s32 arg5);
void func_1511CB44(struct Obj1511CB44 *arg0);

typedef struct Game1483E0WaveState {
    Game1483E0UvPair *coords;
    s32 field4;
    s32 field8;
} Game1483E0WaveState;

typedef struct Game1483E0WaveMesh {
    u8 pad0[0x16];
    u16 count;
    u8 pad18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 pad2C[0x10];
    s32 flags;
    u8 pad40[0x3C];
    Game1483E0WaveState *wave;
    s32 phase;
} Game1483E0WaveMesh;
void func_1511D7BC(Game1483E0WaveMesh *arg0);

struct Obj1511D9E4 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u16 unk16;
    u8 pad18[0x8];
    Vtx *unk20[2];
    Vtx *unk28;
    u8 pad2C[0x10];
    s32 unk3C;
    u8 pad40[0x3C];
    s16 (*unk7C)[2];
    f32 unk80;
};

extern void func_15094AB8(Vtx *arg0, s16 (*arg1)[2], s32 arg2, f32 arg3, s32 arg4, s32 arg5);
void func_1511D9E4(struct Obj1511D9E4 *arg0);

struct Obj1511DBC4Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
};

struct Obj1511DBC4 {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x67];
    u8 unk73;
    u8 pad74[0x8];
    struct Obj1511DBC4Sub *unk7C;
};
void func_1511DBC4(struct Obj1511DBC4 *arg0);

void func_10010558(u16, void *, s32, s32, s32, s32);
void func_1511DD98(u8 *arg0, u8 *arg1);

struct Obj1511DF6CSub {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    f32 unk8;
    f32 unkC;
};

struct Obj1511DF6C {
    f32 unk0;
    f32 unk4;
    u8 pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x2];
    f32 unk18;
    u8 pad1C[0x10];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    u8 pad38[0x1C];
    u16 unk54;
    u8 pad56[0x18];
    u8 unk6E;
    u8 pad6F[0xD];
    struct Obj1511DF6CSub *unk7C;
    s32 unk80;
    s32 unk84;
};

extern void *func_1509B570(s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_151D6970(struct127 *, struct Obj1511DF6C *);
void func_1511DF6C(struct Obj1511DF6C *arg0);

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
void func_1511EC50(struct Obj1511EC50 *arg0);
void func_1511ED84(struct Obj1511ED84 *arg0);

typedef struct {
    f32 x, y, z, radius, angle, dx, dz;
    s16 timer;
    u8 pad1E[6];
} Game11EF40Motion;

typedef struct {
    f32 x, y, z;
} Vec3f_1511EF40;

extern s32 func_15086CBC(s32, f32 *, f32 *, f32 *);
extern s32 func_15086D48(s32);
extern f32 D_800A3378;
void func_1511EF40(u8 *arg0);
void func_1511F31C(struct Struct1511F31CArg *arg0);

struct Obj1511F3E8 {
    u8 pad0[0x3C];
    s32 unk3C;
    u8 pad40[0x2E];
    u8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    u8 pad88[2];
    u8 unk8A;
};
void func_1511F3E8(struct Obj1511F3E8 *arg0);
void func_1511F768(s32 *arg0, s32 arg1);

struct Obj1511F788 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x4];
    f32 unk18;
    u8 pad1C[0x20];
    s32 unk3C;
    u8 pad40[0x1C];
    s16 unk5C;
    u8 pad5E[0x10];
    s8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
};

union Temp1511F788 {
    struct {
        f32 pad;
        s32 w;
    } s;
    f64 d;
};
void func_1511F788(struct Obj1511F788 *arg0);

extern s32 func_151149AC(u32);
void func_1511F92C(s16 *arg0);
