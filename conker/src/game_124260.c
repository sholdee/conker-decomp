#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Message 0x3E payload: "this actor is gone"; beams owned by it destroy themselves (func_150F7310). */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id */
} OwnerGoneMsg;

/* Broadcasts message 0x3E for `owner`, so the beams it owns go away. */
void func_150F6DB0(struct127 *owner) {
    OwnerGoneMsg msg;

    msg.owner = owner;
    msg.ownerId = owner->unique_id;
    func_151494E0((s32)&msg, 0x3E);
}

/* Spawn descriptor for func_15130280 (a beam end sprite). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 life;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 color0[4];
    /* 0x18 */ u8 color1[4];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
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
} BeamEndSpawn;

/* A sprite spawned by func_15130280 that marks one end of the beam. */
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ struct17 pos;
    /* 0x4C */ u8 pad4C[0x74 - 0x4C];
    /* 0x74 */ s8 mode; /* 3 or -1 (see func_150F706C) */
} BeamEnd;

/* Result of the func_15081690 ray cast. */
typedef struct {
    /* 0x00 */ u8 *hitObj;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct17 pos;
    /* 0x14 */ u8 pad14[0x59 - 0x14];
    /* 0x59 */ u8 hit; /* 0: nothing, 1: ?, other: hitObj */
    /* 0x5A */ u8 pad5A[0x64 - 0x5A];
} RayHit;

/* Per-object state of a beam (0x78 bytes, copied in by func_150F6DE4). */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id */
    /* 0x08 */ BeamEnd *ends[2];
    /* 0x10 */ RayHit ray;
    /* 0x74 */ u8 flags;
} BeamState;

/* A beam object allocated by func_15149130. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE; /* -1: destroy */
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ BeamState beam;
} BeamObj;

extern BeamEnd *func_15130280(BeamEndSpawn *, u8, s32, s32, u8, s32);

/* Spawns a beam cast from `owner` along its facing, with two end sprites. */
BeamObj *func_150F6DE4(struct127 *owner) {
    BeamState state;
    u8 i;
    BeamObj *obj;

    state.owner = owner;
    state.ownerId = owner->unique_id;
    for (i = 0; i < 2; i++) {
        state.ends[i] = NULL;
    }
    state.flags = 0;
    obj = (BeamObj *)func_15149130(300, -1, 0x43, -1, 0, 0x37, (struct37 *)0x78, 0xFF, 1);
    if (obj != NULL) {
        BeamState *beam;

        beam = &obj->beam;
        memcpy(beam, &state, sizeof(state));
        {
            BeamEndSpawn spawn;

            spawn.unk8 = 0x4417;
            spawn.unk0 = 0x200004;
            spawn.unk4 = 0;
            spawn.life = 300;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.color0[0] = 0xFF;
            spawn.color0[1] = 0xFF;
            spawn.color0[2] = 0xFF;
            spawn.color0[3] = 0xFF;
            spawn.unk1C = 0xFF;
            spawn.unk28 = spawn.unk2C = 1511.0f;
            spawn.pos = *(struct17 *)&D_800A5480;
            spawn.unk3C = *(struct17 *)&D_800A5480;
            spawn.vel = *(struct17 *)&D_800A5480;
            spawn.unk1E = 1;
            spawn.unk20 = 0xFF;
            spawn.unk22 = 1;
            spawn.flags = 0x64C000;
            spawn.unk60 = 8;
            spawn.unk61 = 6;
            spawn.unk62 = -1;
            spawn.unk63 = -1;
            spawn.unk64 = 3;
            spawn.unk65 = 0;
            spawn.unk5C = 0;
            spawn.unk66 = 0xFF;
            spawn.unk68 = 10;
            spawn.unk1D = 100;
            spawn.color1[0] = 0;
            spawn.color1[1] = 0;
            spawn.color1[2] = 0xFF;
            spawn.color1[3] = 0xFF;
            spawn.unk54 = 0.0f;
            spawn.unk24 = 1.0f;
            spawn.unk6C = 20.0f;
            beam->ends[0] = func_15130280(&spawn, 1, 0, 0, obj->unkC, obj->unk1);
            spawn.color1[0] = 0xFF;
            spawn.color1[1] = 0;
            spawn.color1[2] = 0;
            spawn.color1[3] = 0xFF;
            beam->ends[1] = func_15130280(&spawn, 1, 0, 0, obj->unkC, obj->unk1);
        }
    }
    return obj;
}

void func_15145740(struct127 *actor, struct17 *out, struct17 *arg2, struct17 *arg3, f32 arg4);
void func_15081690(struct127 *actor, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, RayHit *hit, f32 maxDist, s32 arg9,
                   s32 argA, s32 argB, s32 argC, s32 argD, s32 argE);
s32 func_1506196C(u8 *obj, s32 arg1);
void func_1502EA98(u8 *obj, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

/* Per-frame update: re-casts the beam from the owner and moves the end sprites to where it hits. */
void func_150F706C(BeamObj *obj) {
    BeamState *beam;
    struct127 *owner;
    struct17 origin;
    struct17 hitPos;

    beam = &obj->beam;
    owner = beam->owner;

    if (owner->interaction_state == 0 || owner->unique_id != beam->ownerId) {
        obj->unkE = -1;
        return;
    }

    if (D_800C35EA == 1) {
        if (beam->ends[0] != NULL && beam->ends[1] != NULL) {
            beam->ends[0]->mode = 3;
            beam->ends[1]->mode = 3;
        }
        return;
    }

    origin.unk0 = owner->x_position;
    origin.unk4 = owner->y_position + 46.0f;
    origin.unk8 = owner->z_position;
    func_15145740(owner, (struct17 *)&D_800D9A50, NULL, NULL, 0.0f);

    ((struct17 *)&D_800D9A50)->unk0 *= 0.001f;
    ((struct17 *)&D_800D9A50)->unk4 *= 0.001f;
    ((struct17 *)&D_800D9A50)->unk8 *= 0.001f;

    func_15081690(owner, origin.unk0, origin.unk4, origin.unk8,
                  ((struct17 *)&D_800D9A50)->unk0, ((struct17 *)&D_800D9A50)->unk4, ((struct17 *)&D_800D9A50)->unk8,
                  &beam->ray, 5000.0f, 0, 0, 1, -1, 0, 0);

    beam->flags |= 1;

    if (beam->ends[0] != NULL && beam->ends[1] != NULL) {
        if (beam->ray.hit == 0) {
            beam->ends[0]->mode = beam->ends[1]->mode = 3;
        } else {
            hitPos = beam->ray.pos;
            beam->ends[1]->pos = hitPos;
            beam->ends[0]->pos = hitPos;

            if (beam->ray.hit == 1) {
                beam->ends[0]->mode = -1;
                beam->ends[1]->mode = 3;
            } else if (func_1506196C(beam->ray.hitObj, 0) < 0xFF) {
                beam->ends[0]->mode = -1;
                beam->ends[1]->mode = 3;
            } else {
                func_1502EA98(beam->ray.hitObj, 0xFF, 0, 0, 0x7F, 0, 0x10);
                beam->ends[0]->mode = 3;
                beam->ends[1]->mode = -1;
            }
        }
    }
}

void func_150F7310(s32 arg0, s32 arg1, u8 arg2) {
    s32 *p = &arg0;
    s32 q;
    if (arg2 == 0x3E) {
        q = *p + 0x28;
        if ((*(s32 *)q == *(s32 *)arg1) || (*(u8 *)(q + 4) == *(u8 *)(*(s32 *)&arg1 + 4))) {
            func_1516972C((struct102 *)*p);
        }
    } else {
        q = *p + 0x28;
        func_15149514(arg1, arg2, q, q + 4, *p);
    }
}

extern void func_1514EDF0(struct260 *, s32);

void func_150F739C(struct260 *arg0) {
    s32 i;
    s32 v;
    s32 *p = (s32 *)((u8 *)arg0 + 0x28);

    for (i = 0; i < 2; i = (u8)(i + 1)) {
        v = p[i + 2];
        if (v != 0) {
            func_1516972C((struct102 *)v);
        }
    }
    func_1514EDF0(arg0, p[0]);
}

void func_150F739C(struct260 *arg0);

void func_150F740C(struct260 *arg0) {
    func_150F739C(arg0);
    func_1514933C(arg0);
}

void func_15149368(struct260 *arg0);

void func_150F7438(struct260 *arg0) {
    func_150F739C(arg0);
    func_15149368(arg0);
}
