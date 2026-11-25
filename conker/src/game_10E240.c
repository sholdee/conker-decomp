#include <ultra64.h>
/* matching: func_1509BFB0 is void, but it ends by tail-dispatching through D_800884D4 via jalr and
 * leaves that result in v0; this file was built against an s32-returning prototype (caller regalloc). */
#define func_1509BFB0 func_1509BFB0_hdr
#include "functions.h"
#undef func_1509BFB0
s32 func_1509BFB0(s32 num, s32, s32, ...);
#include "variables.h"
#include "effects.h"

/* What func_1510FD20 returns for an x/z position. */
typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 unk2; // bitmask, tested against the low D_80082FA0 + 1 bits
} Struct1510FD20;

/* Data of the effect func_150E1060 spawns and func_150E114C updates. */
typedef struct {
    /* 0x00 */ u8 index; // 0 or 1: row of D_800A0FE0, D_800A0FE4 and D_800A0FFC
    /* 0x04 */ WorldObject *prop;
    /* 0x08 */ Struct1510FD20 *lookup; // func_1510FD20 at the prop's x/z
    /* 0x0C */ f32 accum; // particles owed; one is emitted per whole unit
} Emitter;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ Emitter emitter;
} EmitterEffect;

extern u8 D_800A0FE0[];
extern WorldObject *func_151149AC(u8);
extern void func_1510F800(s32);

s32 func_1509BE40(s32, s32, s32, s32);

/* Sets camera distances when camera->zone is 9 (else when unk2C is 8), and flag bits in unk84
 * from func_1509BE40 queries for the levels D_800BE9F0 == 0x1E, 0x1B and 0x2F. */
void func_150E0D90(Camera *camera) {
    if (camera->zone == 9) {
        if (D_800BE9F0 == 0x1E) {
            func_1509BFB0(0, 0x4044, 1);
        }
        if ((camera->mode != 0x100) && (camera->cmdRunning == 0)) {
            if (func_15123934(camera, 8, 0, 0, 0) != 0) {
                camera->flags |= 0x300000;
                camera->flags &= ~4;
                camera->preset = 1;
                camera->presetMask = 3;
                func_15124B18(camera);
            }
            camera->height = 125.0f;
            camera->unk34C = 125.0f;
            camera->targetDistance = 220.0f;
            camera->liftTarget = 30.0f;
        } else {
            camera->liftTarget = 0.0f;
        }
    } else {
        if ((camera->mode == 8) && (camera->cmdRunning == 0) && (func_151239CC(camera, 0) != 0)) {
            camera->unk34C = 110.0f;
            camera->height = 110.0f;
            camera->targetDistance = 300.0f;
        }
        if (D_800BE9F0 == 0x1E) {
            func_1509BFB0(0, 0x4044, 0);
        }
    }
    if (D_800BE9F0 == 0x2F) {
        camera->flags &= 0x7FFFFFFF;
    } else if (D_800BE9F0 == 0x1E) {
        if (func_1509BE40(1, 0x4042, 6, 0x9000) != 0) {
            camera->flags &= 0x7FFFFFFF;
        } else {
            camera->flags |= 0x80000000;
        }
        if (func_1509BE40(1, 0x4044, 6, 0x2000) != 0) {
            camera->flags |= 0x20001010;
            camera->flags &= ~8;
        } else {
            camera->flags &= 0xDFFFEFEF;
            camera->flags |= 8;
        }
    }
    if (D_800BE9F0 == 0x1B) {
        if (func_1509BE40(1, 0x406E, 6, 0x9000) != 0) {
            camera->flags |= 0x200;
        } else {
            camera->flags &= ~0x200;
        }
        if (func_1509BE40(1, 0x406F, 6, 0x9000) != 0) {
            camera->flags |= 0x80000000;
        } else {
            camera->flags &= 0x7FFFFFFF;
        }
    }
}

/* Spawns the emitter effect for prop D_800A0FE0[index] (index 0 or 1); returns it, or NULL. */
struct260 *func_150E1060(u8 index, u8 owner, s32 kind) {
    struct260 *effect;
    Emitter emitter;

    if (index < 0) { // matching: always false for a u8, but removing it changes the code
        return NULL;
    }

    if (index >= 2) {
        return NULL;
    }

    emitter.index = index;
    emitter.prop = func_151149AC(D_800A0FE0[index]);
    if (emitter.prop == NULL) {
        return NULL;
    }

    func_1510F800(0);
    emitter.lookup = (Struct1510FD20 *)func_1510FD20(emitter.prop->x, emitter.prop->z);
    emitter.accum = 0.0f;

    effect = func_15149130(300, -1, 0x1F, -1, 0, 0x1F, sizeof(emitter), owner, kind);
    if (effect != NULL) {
        memcpy(&((EmitterEffect *)effect)->emitter, &emitter, sizeof(emitter));
    }
    return effect;
}

/* Two points per emitter index, in the prop's space (start and end of the emission direction). */
extern Vec3f D_800A0FE4[];
extern Vec3f D_800A0FFC[];
extern struct260 *func_15130374(ParticleDesc68 *, u8, s32, u8, s32);
extern void func_15145CD0(WorldObject *, Vec3f **, f32 **, s32);

/* Emitter effect update: accumulates a random rate and, while the emitter's lookup has one of the
 * low D_80082FA0 + 1 bits set, emits one particle per whole unit along D_800A0FE4 -> D_800A0FFC. */
void func_150E114C(EmitterEffect *effect) {
    Emitter *emitter;
    s32 i;
    s16 mask;
    s32 pad[2]; // matching: two unused stack words above args
    ParticleDesc68 args;
    f32 extra;
    f32 dir[3];
    Vec3f *points[2];
    f32 *results[2];
    s32 flip80;
    s32 flip40;
    f32 speed;
    struct260 *particle;

    emitter = &effect->emitter;
    emitter->accum += (204.0f + (func_150ADA68() * 159.0f)) * 0.001f * D_800BE9A4;
    if (emitter->accum > 1.0f) {
        for (i = 0, mask = 0; i <= D_80082FA0; i++) {
            mask |= 1 << i;
        }
        if (!(emitter->lookup->unk2 & mask)) {
            do {
                emitter->accum -= 1.0f;
            } while (emitter->accum > 1.0f);
            return;
        }
        {
            f32 *d; // matching: the block-scoped pointer steers regalloc for the subtraction below
            args.texture = 0x29;
            args.renderMode = 0x4403;
            args.unk0 = 0x200005;
            args.fadeStart = 0x21;
            args.fadeRate = 7;
            args.flags = 0x1CE05;
            args.primColorMode = 6;
            args.envColorMode = 8;
            args.updateFunc = 0x10;
            args.deathFunc = -1;
            args.unk4 = 0;
            args.frame = 0;
            args.frameRate = 0;
            args.gravity = 0.0f;
            args.drawFunc = -1;
            args.handlerSet = 0;
            args.scaleStart = 0x27;
            args.r = 0xDD;
            args.g = 0xD3;
            args.b = 0xCD;
            args.a = 0xFF;
            args.r2 = 0x57;
            args.g2 = 0x55;
            args.b2 = 0x5A;
            args.brightness = 0xFF;
            extra = 0.962338984f;
            args.scaleRate = 1.011461973f;
            points[0] = &D_800A0FE4[emitter->index];
            points[1] = &D_800A0FFC[emitter->index];
            results[0] = &args.pos.x;
            results[1] = dir;
            func_15145CD0(emitter->prop, points, results, 2);
            d = dir;
            d[0] -= args.pos.x;
            d[1] -= args.pos.y;
            d[2] -= args.pos.z;
        }
        do {
            args.flags &= ~0xC0;
            flip80 = (func_150ADA20() & 1) ? 0x80 : 0;
            flip40 = (func_150ADA20() & 1) ? 0x40 : 0;
            args.flags |= flip40 | flip80;
            args.offset.x = 0.0f;
            args.offset.y = 0.0f;
            args.offset.z = 0.0f;
            args.alpha = 0xFF;
            args.timer = (func_150ADA20() % 17U) + 50;
            args.width = args.height = (func_150ADA68() * 165.0f) + 357.0f;
            speed = ((func_150ADA68() * 195.0f) + 356.0f) * 0.001f;
            args.vel.x = dir[0] * speed;
            args.vel.y = dir[1] * speed;
            args.vel.z = dir[2] * speed;
            particle = func_15130374(&args, 1, sizeof(extra), effect->owner, effect->kind);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
            }
            emitter->accum -= 1.0f;
        } while (emitter->accum > 1.0f);
    }
}
