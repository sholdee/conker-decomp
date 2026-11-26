#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-side (0, 1) tables: byte offset of the side's joint matrix in Actor.ob_matrix_ptr, and model-space points
 * that func_15145EA4 transforms by it. */
extern s32 D_800AAB00[];
extern Vec3f D_800AAB08[];
extern Vec3f D_800AAB20[];
extern Vec3f D_800AAB38[];
extern Vec3f D_800AAB50[];

/* Fires this side's shot from its joint points (or, without a skeleton, from beside the object): posts
 * a func_151D8868 event for its camera, side effects, then func_151C229C along `aim` (from the joints when arg2). */
s32 func_151C62D0(Actor *arg0, u8 side, u8 arg2, ModelAttachment *attachment, s16 arg4, u8 arg5, s32 arg6) {
    Vec3f origin;
    Vec3f aim;
    Vec3f shotPos;
    Vec3f sp130;
    Vec3f sp124;
    Vec3f sp118;
    Vec3f start;
    Vec3f dir;
    s32 matrix;
    MtxF *joints;
    Vec3f *spF4;
    Vec3f *spF0;
    Vec3f spE4;
    const Vec3f *points[5];
    Vec3f *results[5];
    f32 dz;
    f32 dx;
    f32 invLen;
    f32 offset;
    RumbleParams event;
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
    joints = arg0->ob_matrix_ptr;
    if (joints != NULL) {
        // matching: a block-scope declaration; its -g3 scope steers the schedule
        extern Vec3f D_800AAB68[];

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
        if (attachment != NULL) {
            if (func_1514654C(arg0, attachment, 0, points, results, arg2 ? 5 : 4) == 0) {
                return 0;
            }
        } else {
            func_15145EA4(points, results, matrix, arg2 ? 5 : 4);
        }
        dir.x -= start.x;
        dir.y -= start.y;
        dir.z -= start.z;
        if (arg2) {
            aim.x -= origin.x;
            aim.y -= origin.y;
            aim.z -= origin.z;
        }
    } else {
        if (arg2) {
            return 0;
        }
        /* No skeleton: start 34 units to the side of the aim direction (in the xz plane), 49 units up. */
        if ((fabsf(aim.x) > 9.999999747e-05f) || (fabsf(aim.z) > 9.999999747e-05f)) {
            // matching: a block-scope declaration; its -g3 scope steers the schedule
            f32 sqrtf(f32);

            dx = aim.x;
            dz = aim.z;
            invLen = 1.0f / sqrtf((dx * dx) + (dz * dz));
            dz = dz * invLen;
            dx = -dx * invLen;
        } else {
            dz = 1.0f;
            dx = 0.0f;
        }
        offset = side ? 34.0f : -34.0f;
        origin.x = arg0->xpos + (offset * dx);
        origin.y = arg0->ypos + 49.0f;
        origin.z = arg0->zpos + (offset * dz);
        shotPos = origin;
    }
    func_151D5174(arg0, &origin, &aim, &sp118, &spE4, &sp130, &sp124, &spF4, &spF0, &shotPos);
    if (arg0->CameraPtr != NULL) {
        event.flags = 1;
        event.duration = (func_150ADA20() % 11U) + 11;
        event.playerMask = 1 << arg0->CameraPtr->CNumber;
        func_150ADA20();
        event.strength = 8;
        event.updateKind = -1;
        func_151D8868(&event, 0, 0xFF, 1);
    }
    if ((arg0->ob_matrix_ptr != NULL) && ((arg0->unk74 & 0xF) != 0xF)) {
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

/* Handler-table entry: the side-0 point of func_151C69CC. */
void func_151C6974(Actor *arg0, s32 arg1, Vec3f *out) {
    func_151C69CC(arg0, out, 0);
}

/* Handler-table entry: the side-1 point of func_151C69CC. */
void func_151C69A0(Actor *arg0, s32 arg1, Vec3f *out) {
    func_151C69CC(arg0, out, 1);
}

/* Transforms this side's D_800AAB08 point by its joint matrix (D_800AAB00 bytes into arg0->ob_matrix_ptr) into *out. */
void func_151C69CC(Actor *arg0, Vec3f *out, u8 side) {
    s32 matrix = (s32)arg0->ob_matrix_ptr + D_800AAB00[side];

    func_15143134(&D_800AAB08[side], out, matrix);
}

/* A 0x64-byte projectile description, passed to func_150F7470. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ u8 pad14[0x2C - 0x14];
    /* 0x2C */ Vec3f end;
    /* 0x38 */ u8 pad38[0x59 - 0x38];
    /* 0x59 */ u8 count;
    /* 0x5A */ u8 pad5A[0x64 - 0x5A];
} ShotDesc;

/* The object whose shot description func_151C6A28 copies. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ ShotDesc shot;
} ShotHolder;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ShotHolder holder;
    /* 0x9C */ u8 flags;
} ShotSource;

/* Emits this side's effects (func_151D3F14, func_151D4408, func_151C6D70) from its joint points, then fires a
 * copy of the shot described by the owner's 0x1B entry, its end pushed 10% beyond its start. */
void func_151C6A28(Actor *arg0, u8 side, u8 arg2, s32 arg3) {
    Vec3f origin;
    Vec3f start;
    Vec3f dir;
    s32 matrix;

    if ((arg0 != NULL) && (side < 2) && (func_15142A5C(arg0) == 0)) {
        func_151D5148(arg0);
        if (arg0->ob_matrix_ptr != NULL) {
            {
                Vec3f *points[3];
                Vec3f *results[3];

                matrix = (s32)arg0->ob_matrix_ptr + D_800AAB00[side];
                points[0] = &D_800AAB08[side];
                points[1] = &D_800AAB38[side];
                points[2] = &D_800AAB50[side];
                results[0] = &origin;
                results[1] = &start;
                results[2] = &dir;
                func_15145EA4(points, results, matrix, 3);
                // matching: six dead ifs (fewer fail) change uopt's register choice: arg0 stays on the stack
                if (0) {} if (0) {} if (0) {} if (0) {} if (0) {} if (0) {}
                dir.x -= start.x;
                dir.y -= start.y;
                dir.z -= start.z;
            }
            func_151D3F14(&origin, arg2, arg3);
            func_151D4408(&start, &dir, matrix, arg0, 1.0f, arg2, arg3);
            func_151C6D70(arg0, side, &origin, -1, arg2, arg3);
            {
                ShotSource *source;
                Vec3f end;
                Attachment *entry;
                ShotHolder *holder;
                ShotDesc *src;

                if (func_1514ECE0(arg0->attachments, 0x1B, &entry) != 0) {
                    source = entry->handle;
                    holder = &source->holder;
                    if (source->flags & 1) {
                        src = &holder->shot;
                        if (src->count != 0) {
                            if (src->count >= 2) {
                                f32 dx;
                                f32 dy;
                                f32 dz;

                                dx = src->pos.x - origin.x;
                                dy = src->pos.y - origin.y;
                                dz = src->pos.z - origin.z;
                                end.x = (dx * 1.100000024f) + origin.x;
                                end.y = (dy * 1.100000024f) + origin.y;
                                end.z = (dz * 1.100000024f) + origin.z;
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

/* Spawns a func_15102B38 effect at `pos` between this side's D_800AAB08/D_800AAB20 points, with random
 * size, count and angle. */
void func_151C6D70(Actor *arg0, u8 side, Vec3f *pos, s16 arg3, u8 arg4, s32 arg5) {
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

/* An object spawned by func_1513264C (only the field used here). */
typedef struct {
    /* 0x000 */ u8 pad0[0x170];
    /* 0x170 */ u8 unk170; /* start of the per-type state block */
} SpawnedEffect;

/* Spawns an effect object owned by `owner` and initialises its one-byte state block. */
void func_151C6EA0(Actor *owner, u8 arg1, s32 arg2) {
    ParticleSpawn spawn;
    struct {
        u8 unk0;
    } state;
    Vec3f *origin = &D_800A5480;
    SpawnedEffect *effect;

    state.unk0 = 0;
    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    spawn.sizeY = 1.0f;
    spawn.sizeXZ = 1.0f;
    spawn.rot = *origin;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = *origin;
    spawn.vel = *origin;
    spawn.rotVel = *origin;
    spawn.flags = 0x11900;
    spawn.lifetime = 300;
    spawn.model = 15;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = 100;
    spawn.updateFn = 23;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 5;
    spawn.drawFn = -1;
    spawn.unk6A = 1;
    spawn.gravity = 0.0f;
    spawn.owner = owner;
    spawn.ownerId = owner->unique_id;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.sound = 0;

    effect = func_1513264C(&spawn, 3, 0xFF, 0, 1, arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->unk170, &state, sizeof(state));
    }
}

/* Mirrors the owner object's state (type 0xE9 only): flag 0x20000 and its animation speed scaled to 0-255. */
s32 func_151C7038(Func151C7038Arg *arg0) {
    Actor *owner;
    f32 speed;

    owner = arg0->unk7C;
    if (owner->animseq.uh != 0xE9) {
        return 0;
    }
    if (func_15142A5C(owner) != 0) {
        arg0->unk60 |= 0x20000;
    } else {
        arg0->unk60 &= ~0x20000;
    }
    speed = owner->animsp * 35.0f;
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
