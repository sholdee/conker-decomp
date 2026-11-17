#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_151432BC(void *, f32 *, f32 *, f32 *, f32 *);
void *memcpy(void *, const void *, u32);
u8 func_151464B8(s16 *);

typedef struct Game1199D0Point {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Game1199D0Point;
typedef struct Game1199D0Spawner {
    void *shape;
    f32 rate;
    s16 *owner;
    f32 accumulator;
} Game1199D0Spawner;

void func_150EC520(u8 *arg0) {
    Game1199D0Spawner *state;
    Game1199D0Point point;
    f32 upper;

    if (func_151464B8(*(s16 **)(arg0 + 0x30)) == 0) {
        state = (Game1199D0Spawner *)(arg0 + 0x28);
        state->accumulator += (9.04e-08f + func_150ADA68() * 9.87e-08f) * state->rate * D_800BE9A4;
        if (state->accumulator > 1.0f) {
            point.w = 0.0f;
            do {
                void *spawned;
                func_151432BC(state->shape, &point.x, &point.z, &upper, &point.y);
                spawned = func_15149130(func_150ADA20() % 131U + 0x50, -1, 0x5A, -1, 1, 0, (struct37 *)0x10, arg0[0xC], arg0[1]);
                if (spawned != NULL) {
                    memcpy((u8 *)spawned + 0x28, &point, 0x10);
                }
                state->accumulator -= 1.0f;
            } while (state->accumulator > 1.0f);
        }
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[9];
} Spawn150EC6B0;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Extra150EC6B0;

typedef struct {
    struct17 pos;
    f32 accum;
} Sub150EC6B0;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Sub150EC6B0 unk28;
} Obj150EC6B0;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_150EC6B0(Obj150EC6B0 *arg0) {
    Sub150EC6B0 *sub;

    sub = &arg0->unk28;
    sub->accum += (1352.0f + func_150ADA68() * 1698.0f) * 0.0001f * D_800BE9A4;
    if (sub->accum > 1.0f) {
        Spawn150EC6B0 spawn;
        Extra150EC6B0 extra;
        void *ret;

        extra.unkC = 0.9704700112f;
        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.unk1E = 0x46;
        spawn.unk20 = 3;
        spawn.unk58 = 0x80DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        extra.unk0 = 0;
        extra.unk1 = 0;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk62 = 0x16;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 0x46;
        spawn.unk14 = 0x47;
        spawn.unk15 = 0xC2;
        spawn.unk16 = 0;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0xC0;
        spawn.unk19 = 0x49;
        spawn.unk1A = 0;
        spawn.unk1C = 0xFF;
        spawn.unk24 = 1.021028996f;
        spawn.unk30 = sub->pos;
        spawn.unk3C.unk0 = 0.0f;
        spawn.unk3C.unk4 = 0.0f;
        spawn.unk3C.unk8 = 0.0f;
        spawn.unk48.unk0 = 0.0f;
        spawn.unk48.unk4 = 0.0f;
        spawn.unk48.unk8 = 0.0f;
        do {
            extra.unk2 = func_150ADA20() % 5U + 4;
            extra.unk3 = func_150ADA20() % 5U + 4;
            extra.unk4 = func_150ADA68() * 11.0f;
            extra.unk8 = func_150ADA68() * 11.0f;
            spawn.unk54 = func_150ADA68() * 0.172f + 0.09800000489f;
            spawn.unk58 &= ~0xC0;
            spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.unk1B = func_150ADA20() % 101U + 100;
            spawn.unkA = func_150ADA20() % 51U + 80;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 80.0f + 92.0f;
            ret = func_15130280(&spawn, 0, 0, 0x10, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &extra, sizeof(extra));
            }
            sub->accum -= 1.0f;
        } while (sub->accum > 1.0f);
    }
}

void func_150ECA68(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s16 arg6, u8 arg7, s32 arg8) {
    struct {
        void *unk0;
        u8 unk4;
        u8 unk5;
        u8 unk6;
        u8 unk7;
        u8 unk8;
        u8 unk9;
    } sp3C;
    struct260 *temp_v0;

    sp3C.unk0 = arg0;
    sp3C.unk4 = *((u8 *)arg0 + 0x3B);
    sp3C.unk8 = arg4;
    sp3C.unk9 = arg5;
    sp3C.unk5 = arg1;
    sp3C.unk6 = arg2;
    sp3C.unk7 = arg3;

    temp_v0 = func_15149130((arg6 == -1) ? 0x12C : arg6, -1, 0x5D, -1, (arg6 == -1) ? 0 : 1, 0x45, (struct37 *)0xC, arg7, arg8);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp3C, 0xC);
    }
}

void func_150ECB4C(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

typedef struct {
    /* 0x00 */ struct127 *field_0x00;
    /* 0x04 */ u8 field_0x04;
    /* 0x05 */ u8 field_0x05;
    /* 0x06 */ u8 field_0x06;
    /* 0x07 */ u8 field_0x07;
    /* 0x08 */ u8 field_0x08;
    /* 0x09 */ u8 field_0x09;
} ObjectColorCommandData;

typedef struct {
    /* 0x00 */ u8 pad_0[0xE];
    /* 0x0E */ s16 field_0x0E;
    /* 0x10 */ u8 pad_1[0x18];
    /* 0x28 */ ObjectColorCommandData field_0x28;
} ObjectColorCommandTask;

extern void func_1502EA98(struct127*, u8, u8, u8, u8, s32, u8);

void func_150ECB8C(ObjectColorCommandTask *arg0) {
    ObjectColorCommandData* p = &arg0->field_0x28;
    struct127* ptr = p->field_0x00;

    if ((ptr->interaction_state == 0) || (ptr->unique_id != p->field_0x04)) {
        arg0->field_0x0E = -1;
    } else {
        func_1502EA98(ptr, p->field_0x05, p->field_0x06, p->field_0x07, p->field_0x08, 0, p->field_0x09);
    }
}

extern void func_151C9AC0(s32, u8, s32);
extern void func_150ECA68(void *, u8, u8, u8, u8, u8, s16, u8, s32);

void func_150ECC00(s32 arg0, u8 arg1, s32 arg2) {
    func_151C9AC0(arg0, arg1, arg2);
    func_150ECA68(arg0, 0, 0xFF, 0, 0xFF, 4, -1, arg1, arg2);
}

typedef struct Game1199D0Camera {
    u8 pad00[0x84];
    s32 flags;
    u8 pad88[0xAC];
    s32 field134;
    u8 pad138[0x58];
    f32 field190;
    u8 pad194[0x620];
    f32 delta;
} Game1199D0Camera;

void func_150495B0(f32 *, f32, f32 *, f32, f32, f32);
s32 func_1509BE40(s32, s32, s32, ...);
extern f32 D_80088AD0;
extern f32 D_80088AD4;
extern f32 D_80088AD8;
extern f32 D_80088ADC;
extern f32 D_80088AE0;
extern f32 D_80088AE4;
extern f32 D_80088AE8;
extern f32 D_80088AEC;
extern f32 D_80088AF0;

void func_150ECC70(Game1199D0Camera *arg0) {
    s32 value;
    s32 width;
    s32 height;

    if (func_1509BE40(1, 0x403B, 6, 0x2000) != 0) {
        s32 mode;

        func_1509BFB0(0, 0x4031, 1);
        func_1509BFB0(0, 0x4032, 1);
        func_1509BFB0(0, 0x4033, 1);
        arg0->flags = (s32) (arg0->flags | 0x80021200);
        if (func_1509BE40(1, 0x4038, 6, 0x2000) != 0) {
            D_80088ADC = 128.0f;
            D_80088AE0 = 255.0f;
            D_80088AE4 = 255.0f;
        } else if (func_1509BE40(1, 0x4039, 6, 0x2000) != 0) {
            D_80088ADC = 255.0f;
            D_80088AE0 = 128.0f;
            D_80088AE4 = 255.0f;
        } else if (func_1509BE40(1, 0x403A, 6, 0x2000) != 0) {
            D_80088ADC = 255.0f;
            D_80088AE0 = 255.0f;
            D_80088AE4 = 128.0f;
        } else {
            D_80088ADC = 255.0f;
            D_80088AE0 = 255.0f;
            D_80088AE4 = 255.0f;
        }
        mode = func_1509BE40(0, 0x2007, 0xB7) | 0x2000;
        if (func_1509BE40(1, 0x4010, 6, 0x2000) == 0) {
            arg0->field134 = 0;
            arg0->field190 = 0.0f;
            return;
        }
        if (mode == 0) {
            return;
        }
        arg0->field134 = 1;
        value = func_1509BE40(1, mode, 0x9C, 0x2000);
        func_1509BFB0(1, 0x9000, 0x17, value);
        width = func_1509BE40(1, mode, 0x9A, 0x2000);
        height = width >> 1;
        if (width < 900) {
            width = 900;
        } else if (width > 1190) {
            width = 1190;
        }
        if (height < 180) {
            height = 180;
        } else if (height > 231) {
            height = 231;
        }
        func_1509BFB0(2, 0x9000, 8, width, height);
        arg0->field190 = 220.0f;
    } else {
        D_80088ADC = 255.0f;
        D_80088AE0 = 255.0f;
        D_80088AE4 = 255.0f;
        func_1509BFB0(0, 0x4031, 0);
        func_1509BFB0(0, 0x4032, 0);
        func_1509BFB0(0, 0x4033, 0);
        arg0->flags &= 0x7FFDEDFF;
        arg0->flags |= 8;
    }
    func_150495B0(&D_80088AD0, D_80088ADC, &D_80088AE8, 4.0f, 6.0f, arg0->delta);
    func_150495B0(&D_80088AD4, D_80088AE0, &D_80088AEC, 4.0f, 6.0f, arg0->delta);
    func_150495B0(&D_80088AD8, D_80088AE4, &D_80088AF0, 4.0f, 6.0f, arg0->delta);
    func_1509BFB0(1, 0x30F3, 0x12, (u32) D_80088AD0 & 0xFF);
    func_1509BFB0(1, 0x30F4, 0x12, (u32) D_80088AD4 & 0xFF);
    func_1509BFB0(1, 0x30F5, 0x12, (u32) D_80088AD8 & 0xFF);
}
