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

void func_15116930(u8 *arg0, u8 *arg1) {
    s32 temp;
    if (arg0[0x4F] & 0x4) {
        if (!(arg0[0x73] & 0x3) && !(arg0[0x73] & 0x4)) {
            if (((u8 **)arg1)[0x31C / 4][0x57] == 1) {
                arg0[0x73] &= 0xFFFC;
                arg0[0x73] |= 0x2;
            }
        }
    }
}

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

void func_15116984(u8 *arg0) {
    if (arg0[0x73] & 0x2) {
        func_151169B4((struct Obj15116BAC *)arg0);
    }
}

void func_151169B4(struct Obj15116BAC *arg0) {
    s32 step;
    s32 sound;
    s16 *rot;
    s16 *prev;
    s32 *tgt;
    s32 i;

    sound = arg0->unk3C >> 10;
    if ((arg0->unk7C[0] != arg0->unk10[0]) || (arg0->unk7C[1] != arg0->unk10[1]) || (arg0->unk7C[2] != arg0->unk10[2])) {
        if ((sound != 0) && (D_80089268[sound].unk0 != 0)) {
            func_15116BAC(arg0);
            return;
        }
        step = arg0->unk3C & ~0xFC00;
        if ((sound != 0) && (arg0->unk74 == 0) && (D_80089260[sound].unk0 != 0)) {
            func_15114D24(arg0, D_80089260[sound].unk0, 0x5DC0, 0x7D0, 0xFA0, 0);
        }
        step = (step * D_800BE9E4) >> 1;
        rot = arg0->unk10;
        prev = arg0->unk5A;
        tgt = arg0->unk7C;
        for (i = 0; i < 3; i++) {
            prev[i] = rot[i];
            if (rot[i] != tgt[i]) {
                if (tgt[i] < rot[i]) {
                    rot[i] = rot[i] - step;
                    if (rot[i] < tgt[i]) {
                        rot[i] = tgt[i];
                    }
                } else {
                    rot[i] = rot[i] + step;
                    if (tgt[i] < rot[i]) {
                        rot[i] = tgt[i];
                    }
                }
            }
            prev[i] = rot[i] - prev[i];
        }
    } else {
        arg0->unk5A[2] = 0;
        arg0->unk5A[1] = arg0->unk5A[2];
        arg0->unk5A[0] = arg0->unk5A[2];
        if (arg0->unk74 != 0) {
            if ((sound != 0) && (D_80089264[sound].unk0 != 0)) {
                func_15114D24(arg0, D_80089264[sound].unk0, 0x5DC0, 0x7D0, 0xFA0, 4);
            }
            arg0->unk74 = 0;
        }
    }
}

void func_15116BAC(struct Obj15116BAC *arg0) {
    s32 rate;
    s32 step;
    s32 sound;
    s32 started;
    s16 *rot;
    s16 *prev;
    s32 *tgt;
    s32 diff;
    s32 i;
    s32 maxDiff;

    sound = arg0->unk3C >> 10;
    started = 0;
    maxDiff = 0;
    rate = arg0->unk3C & ~0xFC00;
    if ((sound != 0) && (arg0->unk74 == 0) && (D_80089260[sound].unk0 != 0)) {
        func_15114D24(arg0, D_80089260[sound].unk0, 0x5DC0, 0x7D0, 0xFA0, 0);
        started = 1;
    }
    step = (rate * D_800BE9E4) >> 1;
    rot = arg0->unk10;
    prev = arg0->unk5A;
    tgt = arg0->unk7C;
    for (i = 0; i < 3; i++) {
        prev[i] = rot[i];
        if (rot[i] != tgt[i]) {
            if (tgt[i] < rot[i]) {
                rot[i] = rot[i] - step;
                diff = rot[i] - tgt[i];
                if (diff < 0) {
                    rot[i] = tgt[i];
                }
            } else {
                rot[i] = rot[i] + step;
                diff = tgt[i] - rot[i];
                if (diff < 0) {
                    rot[i] = tgt[i];
                }
            }
            if (maxDiff < diff) {
                maxDiff = diff;
            }
        }
        prev[i] = rot[i] - prev[i];
    }
    if ((sound != 0) && (D_80089260[sound].unk8 != 0)) {
        maxDiff -= (D_80089260[sound].unkA * rate) >> 1;
        if (maxDiff <= 0) {
            if ((maxDiff > -step) || started) {
                func_15114D24(arg0, D_80089260[sound].unk8, 0x5DC0, 0x7D0, 0xFA0, 0);
            }
        }
    }
}

struct Obj15116D7C {
    f32 unk0[3];
    u8 padC[0x30];
    s32 unk3C;
    u8 pad40[0x20];
    f32 unk60[3];
    u8 pad6C[0x10];
    f32 unk7C[3];
};

void func_15116D7C(struct Obj15116D7C *arg0) {
    f32 step;
    f32 dt;
    f32 *rot;
    f32 *cur;
    f32 *tgt;
    s32 i;

    step = arg0->unk3C * 0.00390625f;
    dt = D_800BE9E4;
    rot = arg0->unk0;
    cur = arg0->unk60;
    tgt = arg0->unk7C;
    for (i = 0; i < 3; i++) {
        if (cur[i] != tgt[i]) {
            if (tgt[i] < cur[i]) {
                cur[i] -= step;
                if (cur[i] < tgt[i]) {
                    cur[i] = tgt[i];
                }
            } else {
                cur[i] += step;
                if (tgt[i] < cur[i]) {
                    cur[i] = tgt[i];
                }
            }
        }
        rot[i] += cur[i] * dt;
        if (rot[i] < 0.0f) {
            rot[i] += 360.0f;
        } else if (rot[i] >= 360.0f) {
            rot[i] -= 360.0f;
        }
    }
}

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

void func_15116EA4(struct Obj15116EA4 *arg0) {
    s32 size;
    f32 frac;
    u32 frameSize;
    s32 n;
    s32 i;
    s16 *cur;
    s16 *next;
    s32 idx;
    s32 pos;
    s32 frames;
    s32 count;
    s32 speed;
    s32 frame;

    idx = arg0->unk3C & 0xFFFF;
    frames = D_80089320[idx].frames;
    pos = arg0->unk84;
    count = D_80089320[idx].count;
    size = D_8002AAD0[frames].end - D_8002AAD0[frames].start;
    speed = arg0->unk3C >> 16;
    if (arg0->unk80 == NULL) {
        arg0->unk80 = allocate_memory(size, 1, 2, 0);
        func_10004514(D_8002AAD0[frames].start, arg0->unk80, size, 1);
    }
    frac = count * (pos * 1.525902189e-05f);
    frame = frac;
    frac = frac - frame;
    if (count == frame) {
        frac = 1.0f;
        frame--;
    }
    frameSize = (size / count) & ~1;
    n = frameSize / 6;
    cur = (s16 *)((u8 *)arg0->unk80 + frame * frameSize);
    if (frame + 1 != (u32)count) {
        next = (s16 *)((u8 *)arg0->unk80 + (frame + 1) * frameSize);
    } else {
        next = cur;
    }
    for (i = 0; i < n; i++) {
        arg0->unk20[D_800BE9C0][i].v.ob[0] = cur[i * 3 + 0] + (next[i * 3 + 0] - (f32)cur[i * 3 + 0]) * frac + arg0->unk10;
        arg0->unk20[D_800BE9C0][i].v.ob[2] = cur[i * 3 + 2] + (next[i * 3 + 2] - (f32)cur[i * 3 + 2]) * frac + arg0->unk12;
        arg0->unk20[D_800BE9C0][i].v.ob[1] = cur[i * 3 + 1] + (next[i * 3 + 1] - (f32)cur[i * 3 + 1]) * frac + arg0->unk14;
    }
    if (arg0->unk7C < pos) {
        pos -= speed;
        if (pos < arg0->unk7C) {
            pos = arg0->unk7C;
        }
    } else if (pos < arg0->unk7C) {
        pos += speed;
        if (arg0->unk7C < pos) {
            pos = arg0->unk7C;
        }
    }
    arg0->unk84 = pos;
}

typedef struct Motion151172D8 {
    u8 pad0[0x3C];
    s32 packedTarget;
    u8 pad40[0x3C];
    f32 damping;
    f32 acceleration;
    f32 velocity;
} Motion151172D8;

extern f32 D_800A2FD8;

f32 func_151172D8(Motion151172D8 *arg0, f32 angle) {
    f32 velocity;
    f32 delta;
    f32 target;
    f32 magnitude;
    f32 damping;
    f32 acceleration;

    damping = arg0->damping;
    acceleration = arg0->acceleration;
    velocity = arg0->velocity;
    target = (f32)((arg0->packedTarget << 22) >> 22);
    if (angle != target || velocity != 0.0f) {
        if (velocity == 0.0f && (arg0->packedTarget >> 10) != 0) {
            if (D_80089260[arg0->packedTarget >> 10].unk0 != 0) {
                func_15114D24((struct Obj15116BAC *)arg0, D_80089260[arg0->packedTarget >> 10].unk0, 0x5DC0, 0xC8, 0x9C4, 0);
            }
        }
        angle += velocity * (f32)D_800BE9E4;
        delta = func_15048A70(angle, target);
        magnitude = fabsf(delta);
        if (delta > 0.0f) {
            velocity += magnitude * acceleration;
        } else {
            velocity -= magnitude * acceleration;
        }
        velocity *= damping;
        if (fabsf(delta) < D_800A2FD8 && fabsf(velocity) < D_800A2FD8) {
            angle = target;
            velocity = 0.0f;
        } else if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        arg0->velocity = velocity;
    }
    return angle;
}

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

s32 func_150AD9A0(s32, s32, s32);
extern f32 D_800A2FDC;
extern f32 D_800A2FE0;
extern f32 D_800A2FE4;
extern f32 D_800A2FE8;
extern f32 D_800A2FEC;
extern f32 D_800A2FF0;
void func_151179BC(void *arg0);
void func_15117D3C(struct Obj151152A8 *arg0, struct127 *arg1);
void func_15117DA4(struct Obj15117DA4 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4);

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
void func_15117F3C(struct F7O *o);

s32 func_151189AC(void *arg0, s32 arg1);
s32 func_1511896C(void *arg0);
s32 func_1511898C(void *arg0);

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
s32 func_151189AC(void *arg0, s32 arg1);

extern void func_1511F990(void *, s32);
extern void func_15173C60(s32, s32);
extern void func_1518804C(s32, f32);
extern void func_151669A0(s32, s32, s32, f32, u8, s32);
extern f32 D_800A3148;
extern f32 D_800A314C;
extern f32 D_800A3150;
extern f32 D_800A3154;
extern f32 D_800A3158;
void func_15118F24(u8 *arg0);

struct Obj151193AC_0 {
    char pad0[0x73];
    u8 unk73;
};

struct Obj151193AC_1 {
    s32 unk0;
    char pad4[0x61];
    u8 unk65;
};
void func_151193AC(struct Obj151193AC_0 *arg0, struct Obj151193AC_1 *arg1);

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
void func_151193F4(struct Obj151193F4 *arg0);


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
void func_151194D4(struct Obj151194D4 *arg0, struct Obj151194D4Node *arg1, s32 arg2, s32 arg3);

extern f32 func_150489B0(u8);
extern f32 D_800A315C;
void func_151196D4(struct Obj151194D4 *arg0);

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
void func_15119938(Game144C70VertexState *arg0);

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
