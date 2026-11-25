#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Same layout as SpawnRegion in game_131F30.c and game_16EE20.c. */
typedef struct SpawnRegion {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radiusX;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 radiusZ;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 rotation; /* degrees */
    /* 0x14 */ u8 unk14;     /* the effects set it to 1 while off */
    /* 0x15 */ u8 flags;     /* bits 0-1: shape */
} SpawnRegion;

/* Particle description passed to func_15130280 (see SparkSpawn in game_100810.c). */
typedef struct ParticleSpawn {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 kind;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ struct17 pos;
    /* 0x3C */ struct17 unk3C;
    /* 0x48 */ struct17 vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ f32 unk6C;
} ParticleSpawn; /* size 0x70 */

/* State at 0x110 of the region particle emitter (func_151A6600, func_151A6AB8). */
typedef struct EmitterState {
    /* 0x00 */ SpawnRegion *region;
    /* 0x04 */ f32 angle;
    /* 0x08 */ f32 angleSpeed;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ struct102 *unk10; /* released by func_151A6AB8 */
    /* 0x14 */ u8 phase;
    /* 0x15 */ u8 phaseSpeed;
    /* 0x16 */ u8 alpha;      /* oscillates in [129, 255] with phase */
    /* 0x17 */ u8 hasZone;    /* nonzero: register a zone around the object each frame */
    /* 0x18 */ f32 pending;   /* particles owed, accumulated per frame */
} EmitterState;

typedef struct EmitterObj {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x27];
    /* 0x034 */ f32 x;
    /* 0x038 */ f32 y;
    /* 0x03C */ f32 z;
    /* 0x040 */ u8 pad40[0x20];
    /* 0x060 */ s16 *unk60;
    /* 0x064 */ u8 pad64[0xAC];
    /* 0x110 */ EmitterState state;
} EmitterObj;

extern void *func_15130280(ParticleSpawn *, u8, s32, s32, u8, s32);
extern void func_15143794(s16 yaw, s16 pitch, f32 length, struct17 *out);
extern void func_15143874(s16 angle, f32 radius, f32 *x, f32 *z);
extern f32 func_15144528(f32 value, f32 hi, f32 lo);
extern s32 func_151464B8(s16 *);

/* Spins the emitter and, while func_151464B8 allows it, throws particles from random points of its region. */
s32 func_151A6600(EmitterObj *obj) {
    EmitterState *state;

    state = &obj->state;
    if (state->hasZone) {
        func_1508B20C(obj->x, obj->y, obj->z, 500.0f);
    }
    state->angle += state->angleSpeed * D_800BE9A4;
    state->angle = func_15144528(state->angle, 2048.0f, 0.0f);
    state->phase += state->phaseSpeed * D_800BE9E4;
    state->alpha = func_151423D8(state->phase - 0x40) * 63.0f + 192.0f;
    if (func_151464B8(obj->unk60) == 0) {
        state->pending += (0.3930000067f + func_150ADA68() * 0.4370000064f) * D_800BE9A4;
        if (state->pending > 1.0f) {
            ParticleSpawn spawn;

            spawn.kind = 40;
            spawn.unk8 = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0x1F0600;
            spawn.unk14 = 0xFF;
            spawn.unk15 = 0xFF;
            spawn.unk16 = 0xFF;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0xFF;
            spawn.unk19 = 0xFF;
            spawn.unk3C.unk0 = 0.0f;
            spawn.unk3C.unk4 = 0.0f;
            spawn.unk3C.unk8 = 0.0f;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk1A = 0xFF;
            spawn.unk1E = 12;
            spawn.unk20 = 21;
            spawn.unk22 = 1;
            spawn.unk24 = 1.0f;
            spawn.flags = 0xC207;
            spawn.unk60 = 6;
            spawn.unk61 = 8;
            spawn.unk62 = -1;
            spawn.unk63 = -1;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            do {
                func_15143874((u8)func_150ADA20(), state->region->radiusX * func_150ADA68(), &spawn.pos.unk0,
                              &spawn.pos.unk8);
                spawn.pos.unk0 += state->region->x;
                spawn.pos.unk4 = state->region->y;
                spawn.pos.unk8 += state->region->z;
                spawn.unkA = (func_150ADA20() & 0xF) + 25;
                spawn.unk1B = func_150ADA20() % 101U + 100;
                spawn.unk28 = spawn.unk2C = func_150ADA68() * 205.0f + 98.0f;
                func_15143794((u8)func_150ADA20(), func_150ADA20() % 35U - 64, func_150ADA68() * 10.0f + 10.0f,
                              &spawn.vel);
                // matching: `+ -x` (a subtraction emits sub.s instead of add.s)
                spawn.unk54 = func_150ADA68() * 0.2840000093f + -0.9100000262f;
                func_15130280(&spawn, 1, 0, 0, obj->unkC, obj->unk1);
                state->pending -= 1.0f;
            } while (state->pending > 1.0f);
        }
    }
    return 1;
}

extern void func_1000FD38(void *, void *, s32, void *);

/* Shuts the emitter down: marks its region off, calls func_1000FD38 with func_1000EF40 on it, and frees unk10. */
void func_151A6AB8(EmitterObj *obj) {
    obj->state.region->unk14 = 1;
    func_1000FD38(func_1000EF40, obj->state.region, 0, obj);
    if (obj->state.unk10 != NULL) {
        func_1516972C(obj->state.unk10);
    }
}
