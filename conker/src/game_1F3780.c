#include <ultra64.h>
/* include/functions.h declares func_150ADA20 as returning u8, but the routine returns a full
 * word (its asm ends in dsra32 with no mask); func_151C62D0 copies the result with a plain
 * move, which a u8 declaration cannot produce. Rename the header's declaration out of the
 * way and declare the real return type file-locally. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

extern s32 D_800AAB00[];
extern struct17 D_800AAB08[];
extern struct17 D_800AAB38[];
extern struct17 D_800AAB50[];
typedef struct {
    s8 active;
    u8 pad1;
    s16 duration;
    s8 size;
    s8 enabled;
    s8 sentinel;
    u8 pad7;
} Func151C62D0Spawn;

extern struct17 D_800AAB20[];
extern struct17 D_800AAB68[];
extern u8 D_800DCA20;
s32 func_151C229C(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, struct127 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);
void func_15145740(struct127 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 arg4);
s32 func_1514654C(struct127 *, s32, s32, void *, void *, s32);
void func_151D5174(struct127 *, struct17 *, struct17 *, struct17 *, struct17 *, struct17 *, struct17 *, struct17 **, struct17 **, struct17 *);
void *func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_15142A5C(struct127 *arg0);
void func_151D5148(struct127 *arg0);
s32 func_15145EA4(void *arg0, void *arg1, s32 arg2, s32 arg3);
void func_151D3F14(struct17 *arg0, u8 arg1, s32 arg2);
void func_151D4408(struct17 *, struct17 *, s32, struct127 *, f32, u8, s32);
void func_151C6D70(struct127 *, u8, struct17 *, s16, u8, s32);

s32 func_151C62D0(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3, s16 arg4, s32 arg5, s32 arg6) {
    struct17 sp154;
    struct17 sp148;
    struct17 sp13C;
    struct17 sp130;
    struct17 sp124;
    struct17 sp118;
    struct17 sp10C;
    struct17 sp100;
    s32 spFC;
    struct255 *ptr;
    struct17 *spF4;
    struct17 *spF0;
    struct17 spE4;
    void *spD0[5];
    struct17 *spBC[5];
    f32 dz;
    f32 dx;
    f32 inv;
    f32 off;
    Func151C62D0Spawn spA4;
    s32 mask;
    u32 i;

    if (arg0 == NULL) {
        return 0;
    }
    if (arg1 >= 2) {
        return 0;
    }
    if (func_15142A5C(arg0) != 0) {
        return 0;
    }
    func_151D5148(arg0);
    if (!arg2) {
        func_15145740(arg0, &sp148, &sp118, &spE4, 0.8110000491f);
    }
    ptr = arg0->unk1D4;
    if (ptr != NULL) {
        typedef s32 Func151C62D0T;
        spFC = (s32)ptr + D_800AAB00[arg1];
        spBC[0] = &sp154;
        spBC[1] = &sp10C;
        spBC[2] = &sp100;
        spBC[3] = &sp13C;
        spBC[4] = &sp148;
        spD0[0] = &D_800AAB08[arg1];
        spD0[1] = &D_800AAB38[arg1];
        spD0[2] = &D_800AAB50[arg1];
        spD0[3] = &D_800AAB68[arg1];
        spD0[4] = &D_800AAB20[arg1];
        if (arg3 != 0) {
            if (func_1514654C(arg0, arg3, 0, spD0, spBC, arg2 ? 5 : 4) == 0) {
                return 0;
            }
        } else {
            func_15145EA4(spD0, spBC, spFC, arg2 ? 5 : 4);
        }
        sp100.unk0 -= sp10C.unk0;
        sp100.unk4 -= sp10C.unk4;
        sp100.unk8 -= sp10C.unk8;
        if (arg2) {
            sp148.unk0 -= sp154.unk0;
            sp148.unk4 -= sp154.unk4;
            sp148.unk8 -= sp154.unk8;
        }
    } else {
        if (arg2) {
            return 0;
        }
        if ((fabsf(sp148.unk0) > 9.999999747e-05f) || (fabsf(sp148.unk8) > 9.999999747e-05f)) {
            extern s32 D_80082FA0;
            dx = sp148.unk0;
            dz = sp148.unk8;
            inv = 1.0f / sqrtf((dx * dx) + (dz * dz));
            dz = dz * inv;
            dx = -dx * inv;
        } else {
            dz = 1.0f;
            dx = 0.0f;
        }
        off = arg1 ? 34.0f : -34.0f;
        sp154.unk0 = arg0->x_position + (off * dx);
        sp154.unk4 = arg0->y_position + 49.0f;
        sp154.unk8 = arg0->z_position + (off * dz);
        sp13C = sp154;
    }
    func_151D5174(arg0, &sp154, &sp148, &sp118, &spE4, &sp130, &sp124, &spF4, &spF0, &sp13C);
    if (arg0->camera != NULL) {
        spA4.active = 1;
        spA4.duration = (func_150ADA20() % 11U) + 11;
        spA4.enabled = 1 << arg0->camera->unk23D;
        func_150ADA20();
        spA4.size = 8;
        spA4.sentinel = -1;
        func_151D8868(&spA4, 0, 0xFF, 1);
    }
    if ((arg0->unk1D4 != NULL) && ((arg0->unk74 & 0xF) != 0xF)) {
        if (arg2 || ((D_80082FA0 < 2) && (func_150ADA20() & 1))) {
            func_151D3F14(&sp154, ((u8 *)&arg5)[3], arg6);
        }
        if ((arg2 || (D_80082FA0 < 2)) && (func_150ADA20() & 1)) {
            func_151D4408(&sp10C, &sp100, spFC, arg0, 1.0f, ((u8 *)&arg5)[3], arg6);
        }
        i = D_800DCA20;
        mask = 1;
        while (i > 0) {
            mask |= 1 << i;
            i--;
        }
        if (arg2 || !(func_150ADA20() & mask)) {
            func_151C6D70(arg0, arg1, &sp154, arg4, ((u8 *)&arg5)[3], arg6);
        }
    }
    return func_151C229C(&sp13C, &sp148, spF4, spF0, 1, 0, 300.0f, 0.003333333414f,
                         func_150ADA68() * 10.0f + 25.0f, func_150ADA68() * 150.0f + 400.0f, 50.0f,
                         (func_150ADA20() % 156U) + 100, arg0, arg2 ? 0 : 1, arg2 ? 0 : 1, arg2 ? 0 : 1, 8,
                         arg2 ? 0 : 1, 1, 0, 0x1A, 0.0f, 0xFF, -1, 0, ((u8 *)&arg5)[3], arg6);
}



void func_15143134(struct17 *point, struct17 *out, s32 matrix);
void func_151C69CC(struct127 *arg0, struct17 *out, u8 side);

/* Handler-table entry: the side-0 point of func_151C69CC. */
void func_151C6974(struct127 *arg0, s32 arg1, struct17 *out) {
    func_151C69CC(arg0, out, 0);
}

/* Handler-table entry: the side-1 point of func_151C69CC. */
void func_151C69A0(struct127 *arg0, s32 arg1, struct17 *out) {
    func_151C69CC(arg0, out, 1);
}

/* Transforms this side's D_800AAB08 point by its joint matrix (D_800AAB00 bytes into arg0->unk1D4) into *out. */
void func_151C69CC(struct127 *arg0, struct17 *out, u8 side) {
    s32 matrix = (s32)arg0->unk1D4 + D_800AAB00[side];

    func_15143134(&D_800AAB08[side], out, matrix);
}

typedef struct {
    u8 pad0[8];
    struct17 pos;
    u8 pad14[0x2C - 0x14];
    struct17 unk2C;
    u8 pad38[0x59 - 0x38];
    u8 unk59;
    u8 pad5A[0x64 - 0x5A];
} Func151C6A28Big;

extern struct17 D_800AAB38[];
extern struct17 D_800AAB50[];
s32 func_15142A5C(struct127 *arg0);
void func_151D5148(struct127 *arg0);
extern s32 func_15145EA4(void *arg0, void *arg1, s32 arg2, s32 arg3);
void func_151D3F14(struct17 *arg0, u8 arg1, s32 arg2);
void func_151D4408(struct17 *, struct17 *, s32, struct127 *, f32, u8, s32);
s32 func_1514ECE0(s32, s16, void *);
void func_150F7470(struct17 *, s32, s32, s32, s32, Func151C6A28Big *, f32, f32, f32, struct127 *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);

void func_151C6A28(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct17 sp124;
    struct17 sp118;
    struct17 sp10C;
    s32 sp108;

    if ((arg0 != NULL) && (arg1 < 2) && (func_15142A5C(arg0) == 0)) {
        func_151D5148(arg0);
        if (*(s32 *)&arg0->unk1D4 != 0) {
            {
                struct17 *spFC[3];
                struct17 *spF0[3];

                sp108 = *(s32 *)&arg0->unk1D4 + D_800AAB00[arg1];
                spFC[0] = &D_800AAB08[arg1];
                spFC[1] = &D_800AAB38[arg1];
                spFC[2] = &D_800AAB50[arg1];
                spF0[0] = &sp124;
                spF0[1] = &sp118;
                spF0[2] = &sp10C;
                func_15145EA4(spFC, spF0, sp108, 3);
                if (0) { sp10C.unk0 = 0.0f; }
                if (0) { sp10C.unk0 = 1.0f; }
                if (0) { sp10C.unk0 = 2.0f; }
                if (0) { sp10C.unk0 = 3.0f; }
                if (0) { sp10C.unk0 = 4.0f; }
                if (0) { sp10C.unk0 = 5.0f; }
                if (0) { sp10C.unk0 = 6.0f; }
                if (0) { sp10C.unk0 = 7.0f; }
                if (0) { sp10C.unk0 = 8.0f; }
                if (0) { sp10C.unk0 = 9.0f; }
                if (0) { sp10C.unk0 = 10.0f; }
                if (0) { sp10C.unk0 = 11.0f; }
                sp10C.unk0 -= sp118.unk0;
                sp10C.unk4 -= sp118.unk4;
                sp10C.unk8 -= sp118.unk8;
            }
            func_151D3F14(&sp124, arg2, arg3);
            func_151D4408(&sp118, &sp10C, sp108, arg0, 1.0f, arg2, arg3);
            func_151C6D70(arg0, arg1, &sp124, -1, arg2, arg3);
            {
                u8 *temp;
                struct17 spE0;
                void *spDC;
                u8 *p28;
                Func151C6A28Big *src;

                if (func_1514ECE0(*(s32 *)((u8 *)arg0 + 0x2F4), 0x1B, &spDC) != 0) {
                    temp = *(u8 **)((u8 *)spDC + 0x10);
                    p28 = temp + 0x28;
                    if (temp[0x9C] & 1) {
                        src = (Func151C6A28Big *)(p28 + 0x10);
                        if (p28[0x69] != 0) {
                            if (src->unk59 >= 2) {
                                f32 dx;
                                f32 dy;
                                f32 dz;

                                dx = src->pos.unk0 - sp124.unk0;
                                dy = src->pos.unk4 - sp124.unk4;
                                dz = src->pos.unk8 - sp124.unk8;
                                spE0.unk0 = (dx * 1.100000024f) + sp124.unk0;
                                spE0.unk4 = (dy * 1.100000024f) + sp124.unk4;
                                spE0.unk8 = (dz * 1.100000024f) + sp124.unk8;
                            } else {
                                spE0 = src->pos;
                            }
                            {
                                Func151C6A28Big sp64;

                                sp64 = *src;
                                sp64.unk2C = spE0;
                                func_150F7470(&sp124, 0, 0, 0, 0, &sp64, 54.00000381f, 0.01851851679f, 500.0f, arg0, 0,
                                              (src->unk59 >= 2) ? 0 : 1, 1, 0, 0x1A, -1, 0, 1, arg2, arg3);
                            }
                        }
                    }
                }
            }
        }
    }
}

void *func_15102B38(struct127 *, u8, struct17 *, struct17 *, f32 *, s16, u8, f32, struct17 *, u8, s32, s16, u8,
                    s32);

/* Spawns a func_15102B38 effect at `pos` between this side's D_800AAB08/D_800AAB20 points, with random
 * size, count and angle. */
void func_151C6D70(struct127 *arg0, u8 side, struct17 *pos, s16 arg3, u8 arg4, s32 arg5) {
    f32 values[2];
    f32 angle;
    u32 random1;
    u32 random0;

    values[1] = (func_150ADA68() * 1.001f) + 0.6040000319f;
    values[0] = (func_150ADA68() * 5.9f) + 14.1f;
    random0 = func_150ADA20();
    random1 = func_150ADA20();
    angle = func_150ADA68();
    func_15102B38(arg0, (u32)D_800AAB00[side] >> 6, &D_800AAB08[side], &D_800AAB20[side], values,
                  (random0 % 4U) + 6, (random1 % 1U) + 255, (angle * 270.0f) + 359.0f, pos, 255, 0, arg3, arg4,
                  arg5);
}

/* Spawn descriptor for func_1513264C (0x7C bytes); same layout as EffectSpawn in game_124920.c. */
typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ struct17 unk10;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ struct17 pos;
    /* 0x34 */ struct17 vel;
    /* 0x40 */ struct17 unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ s8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId; /* owner->unique_id */
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ s32 sound;
} EffectSpawn;

/* An object spawned by func_1513264C (only the field used here). */
typedef struct {
    /* 0x000 */ u8 pad0[0x170];
    /* 0x170 */ u8 unk170; /* start of the per-type state block */
} SpawnedEffect;

SpawnedEffect *func_1513264C(EffectSpawn *spawn, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);

/* Spawns an effect object owned by `owner` and initialises its one-byte state block. */
void func_151C6EA0(struct127 *owner, u8 arg1, s32 arg2) {
    EffectSpawn spawn;
    struct {
        u8 unk0;
    } state;
    struct17 *origin = (struct17 *)&D_800A5480;
    SpawnedEffect *effect;

    state.unk0 = 0;
    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    spawn.unk0C = 1.0f;
    spawn.unk08 = 1.0f;
    spawn.unk10 = *origin;
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.pos = *origin;
    spawn.vel = *origin;
    spawn.unk40 = *origin;
    spawn.unk50 = 0x11900;
    spawn.unk54 = 300;
    spawn.unk56 = 15;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 100;
    spawn.unk61 = 23;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 5;
    spawn.unk69 = -1;
    spawn.unk6A = 1;
    spawn.unk4C = 0.0f;
    spawn.owner = owner;
    spawn.ownerId = owner->unique_id;
    spawn.unk72 = 1;
    spawn.unk74 = 0xFF;
    spawn.sound = 0;

    effect = func_1513264C(&spawn, 3, 0xFF, 0, 1, arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->unk170, &state, sizeof(state));
    }
}

typedef struct {
    u8 pad0[0x60];
    s32 unk60;
    u8 pad64[0x70 - 0x64];
    u8 unk70;
    u8 pad71[0x7C - 0x71];
    struct127 *unk7C;
} Func151C7038Arg;

/* Mirrors the owner object's state (type 0xE9 only): flag 0x20000 and its animation speed scaled to 0-255. */
s32 func_151C7038(Func151C7038Arg *arg0) {
    struct127 *owner;
    f32 speed;

    owner = arg0->unk7C;
    if (owner->unk84.uh != 0xE9) {
        return 0;
    }
    if (func_15142A5C(owner) != 0) {
        arg0->unk60 |= 0x20000;
    } else {
        arg0->unk60 &= ~0x20000;
    }
    speed = owner->animation_speed * 35.0f;
    if (speed > 255.0f) {
        speed = 255; // matching: 255.0f schedules the clamp differently
    } else if (speed < 0.0f) {
        speed = 0.0f;
    }
    {
        // matching: the -g3 block scope keeps the float->u8 conversion after the clamp
        u8 clamped = speed;

        arg0->unk70 = clamped;
    }
    return 1;
}
