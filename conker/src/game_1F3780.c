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
/* Argument of func_151D8868 (same layout as Game113D60Event in game_113D60.c). */
typedef struct {
    /* 0x0 */ s8 type;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 duration;
    /* 0x4 */ s8 unk4;
    /* 0x5 */ s8 cameraMask; /* 1 << camera->unk23D */
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 pad7;
} EffectEvent;

extern struct17 D_800AAB20[];
extern u8 D_800DCA20;
s32 func_151C229C(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, struct127 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);
void func_15145740(struct127 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 arg4);
s32 func_1514654C(struct127 *, s32, s32, struct17 **, struct17 **, s32);
void func_151D5174(struct127 *, struct17 *, struct17 *, struct17 *, struct17 *, struct17 *, struct17 *, struct17 **, struct17 **, struct17 *);
void *func_151D8868(EffectEvent *event, s32 arg1, s32 arg2, s32 arg3);
s32 func_15142A5C(struct127 *arg0);
void func_151D5148(struct127 *arg0);
s32 func_15145EA4(void *arg0, void *arg1, s32 arg2, s32 arg3);
void func_151D3F14(struct17 *arg0, u8 arg1, s32 arg2);
void func_151D4408(struct17 *, struct17 *, s32, struct127 *, f32, u8, s32);
void func_151C6D70(struct127 *, u8, struct17 *, s16, u8, s32);

s32 func_151C62D0(struct127 *arg0, u8 side, u8 arg2, s32 arg3, s16 arg4, u8 arg5, s32 arg6) {
    struct17 origin;
    struct17 aim;
    struct17 shotPos;
    struct17 sp130;
    struct17 sp124;
    struct17 sp118;
    struct17 start;
    struct17 dir;
    s32 matrix;
    struct255 *joints;
    struct17 *spF4;
    struct17 *spF0;
    struct17 spE4;
    struct17 *points[5];
    struct17 *results[5];
    f32 dz;
    f32 dx;
    f32 invLen;
    f32 offset;
    EffectEvent event;
    s32 mask;
    u32 i;

    if (arg0 == NULL) {
        return 0;
    }
    if (side >= 2) {
        return 0;
    }
    if (func_15142A5C(arg0) != 0) {
        return 0;
    }
    func_151D5148(arg0);
    if (!arg2) {
        func_15145740(arg0, &aim, &sp118, &spE4, 0.8110000491f);
    }
    joints = arg0->unk1D4;
    if (joints != NULL) {
        // matching: a block-scope declaration; its -g3 scope steers the schedule
        extern struct17 D_800AAB68[];

        matrix = (s32)joints + D_800AAB00[side];
        results[0] = &origin;
        results[1] = &start;
        results[2] = &dir;
        results[3] = &shotPos;
        results[4] = &aim;
        points[0] = &D_800AAB08[side];
        points[1] = &D_800AAB38[side];
        points[2] = &D_800AAB50[side];
        points[3] = &D_800AAB68[side];
        points[4] = &D_800AAB20[side];
        if (arg3 != 0) {
            if (func_1514654C(arg0, arg3, 0, points, results, arg2 ? 5 : 4) == 0) {
                return 0;
            }
        } else {
            func_15145EA4(points, results, matrix, arg2 ? 5 : 4);
        }
        dir.unk0 -= start.unk0;
        dir.unk4 -= start.unk4;
        dir.unk8 -= start.unk8;
        if (arg2) {
            aim.unk0 -= origin.unk0;
            aim.unk4 -= origin.unk4;
            aim.unk8 -= origin.unk8;
        }
    } else {
        if (arg2) {
            return 0;
        }
        /* No skeleton: start 34 units to the side of the aim direction (in the xz plane), 49 units up. */
        if ((fabsf(aim.unk0) > 9.999999747e-05f) || (fabsf(aim.unk8) > 9.999999747e-05f)) {
            // matching: a block-scope declaration; its -g3 scope steers the schedule
            f32 sqrtf(f32);

            dx = aim.unk0;
            dz = aim.unk8;
            invLen = 1.0f / sqrtf((dx * dx) + (dz * dz));
            dz = dz * invLen;
            dx = -dx * invLen;
        } else {
            dz = 1.0f;
            dx = 0.0f;
        }
        offset = side ? 34.0f : -34.0f;
        origin.unk0 = arg0->x_position + (offset * dx);
        origin.unk4 = arg0->y_position + 49.0f;
        origin.unk8 = arg0->z_position + (offset * dz);
        shotPos = origin;
    }
    func_151D5174(arg0, &origin, &aim, &sp118, &spE4, &sp130, &sp124, &spF4, &spF0, &shotPos);
    if (arg0->camera != NULL) {
        event.type = 1;
        event.duration = (func_150ADA20() % 11U) + 11;
        event.cameraMask = 1 << arg0->camera->unk23D;
        func_150ADA20();
        event.unk4 = 8;
        event.unk6 = -1;
        func_151D8868(&event, 0, 0xFF, 1);
    }
    if ((arg0->unk1D4 != NULL) && ((arg0->unk74 & 0xF) != 0xF)) {
        if (arg2 || ((D_80082FA0 < 2) && (func_150ADA20() & 1))) {
            func_151D3F14(&origin, arg5, arg6);
        }
        if ((arg2 || (D_80082FA0 < 2)) && (func_150ADA20() & 1)) {
            func_151D4408(&start, &dir, matrix, arg0, 1.0f, arg5, arg6);
        }
        mask = 1;
        for (i = D_800DCA20; i > 0; i--) {
            mask |= 1 << i;
        }
        if (arg2 || !(func_150ADA20() & mask)) {
            func_151C6D70(arg0, side, &origin, arg4, arg5, arg6);
        }
    }
    return func_151C229C(&shotPos, &aim, spF4, spF0, 1, 0, 300.0f, 0.003333333414f,
                         func_150ADA68() * 10.0f + 25.0f, func_150ADA68() * 150.0f + 400.0f, 50.0f,
                         (func_150ADA20() % 156U) + 100, arg0, arg2 ? 0 : 1, arg2 ? 0 : 1, arg2 ? 0 : 1, 8, arg2 ? 0 : 1, 1, 0, 0x1A, 0.0f, 0xFF,
                         -1, 0, arg5, arg6);
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

/* A 0x64-byte projectile description, passed to func_150F7470. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ struct17 pos;
    /* 0x14 */ u8 pad14[0x2C - 0x14];
    /* 0x2C */ struct17 end;
    /* 0x38 */ u8 pad38[0x59 - 0x38];
    /* 0x59 */ u8 count;
    /* 0x5A */ u8 pad5A[0x64 - 0x5A];
} ShotDesc;

/* The object whose shot description func_151C6A28 copies. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ ShotDesc shot;
} ShotHolder;

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ ShotHolder holder;
    /* 0x9C */ u8 flags;
} ShotSource;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ ShotSource *source;
} ShotSourceEntry;

s32 func_1514ECE0(s32, s16, ShotSourceEntry **);
void func_150F7470(struct17 *, s32, s32, s32, s32, ShotDesc *, f32, f32, f32, struct127 *, s32, s32, s32, s32, s32, s32,
                   s32, s32, u8, s32);

/* Emits this side's effects (func_151D3F14, func_151D4408, func_151C6D70) from its joint points, then fires a
 * copy of the shot described by the owner's 0x1B entry, its end pushed 10% beyond its start. */
void func_151C6A28(struct127 *arg0, u8 side, u8 arg2, s32 arg3) {
    struct17 origin;
    struct17 start;
    struct17 dir;
    s32 matrix;

    if ((arg0 != NULL) && (side < 2) && (func_15142A5C(arg0) == 0)) {
        func_151D5148(arg0);
        if (arg0->unk1D4 != NULL) {
            {
                struct17 *points[3];
                struct17 *results[3];

                matrix = (s32)arg0->unk1D4 + D_800AAB00[side];
                points[0] = &D_800AAB08[side];
                points[1] = &D_800AAB38[side];
                points[2] = &D_800AAB50[side];
                results[0] = &origin;
                results[1] = &start;
                results[2] = &dir;
                func_15145EA4(points, results, matrix, 3);
                // matching: six dead ifs (fewer fail) change uopt's register choice: arg0 stays on the stack
                if (0) {} if (0) {} if (0) {} if (0) {} if (0) {} if (0) {}
                dir.unk0 -= start.unk0;
                dir.unk4 -= start.unk4;
                dir.unk8 -= start.unk8;
            }
            func_151D3F14(&origin, arg2, arg3);
            func_151D4408(&start, &dir, matrix, arg0, 1.0f, arg2, arg3);
            func_151C6D70(arg0, side, &origin, -1, arg2, arg3);
            {
                ShotSource *source;
                struct17 end;
                ShotSourceEntry *entry;
                ShotHolder *holder;
                ShotDesc *src;

                /* 0x2F4 lies in struct127's pad2F0 */
                if (func_1514ECE0(*(s32 *)((u8 *)arg0 + 0x2F4), 0x1B, &entry) != 0) {
                    source = entry->source;
                    holder = &source->holder;
                    if (source->flags & 1) {
                        src = &holder->shot;
                        if (src->count != 0) {
                            if (src->count >= 2) {
                                f32 dx;
                                f32 dy;
                                f32 dz;

                                dx = src->pos.unk0 - origin.unk0;
                                dy = src->pos.unk4 - origin.unk4;
                                dz = src->pos.unk8 - origin.unk8;
                                end.unk0 = (dx * 1.100000024f) + origin.unk0;
                                end.unk4 = (dy * 1.100000024f) + origin.unk4;
                                end.unk8 = (dz * 1.100000024f) + origin.unk8;
                            } else {
                                end = src->pos;
                            }
                            {
                                ShotDesc shot;

                                shot = *src;
                                shot.end = end;
                                func_150F7470(&origin, 0, 0, 0, 0, &shot, 54.00000381f, 0.01851851679f, 500.0f, arg0, 0,
                                              (src->count >= 2) ? 0 : 1, 1, 0, 0x1A, -1, 0, 1, arg2, arg3);
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
