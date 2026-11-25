#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Sends message 0x3E (func_151494E0) for `owner`; beams it owns destroy themselves on it. */
void func_150F6DB0(Actor *owner) {
    ActorRef msg;

    msg.actor = owner;
    msg.uniqueId = owner->unique_id;
    func_151494E0((s32)&msg, 0x3E);
}

/* A sprite spawned by func_15130280 that marks one end of the beam. */
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ Vec3f pos;
    /* 0x4C */ u8 pad4C[0x74 - 0x4C];
    /* 0x74 */ s8 mode; /* 3 or -1 (see func_150F706C) */
} BeamEnd;

/* Result of the func_15081690 ray cast. */
typedef struct {
    /* 0x00 */ u8 *hitObj;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ u8 pad14[0x59 - 0x14];
    /* 0x59 */ u8 hit; /* 0: no hit */
    /* 0x5A */ u8 pad5A[0x64 - 0x5A];
} RayHit;

/* Per-object state of a beam (0x78 bytes, copied in by func_150F6DE4). */
typedef struct {
    /* 0x00 */ Actor *owner;
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

extern BeamEnd *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/* Spawns a beam object for `owner`, with two end sprites (blue-tinted, then red-tinted). */
BeamObj *func_150F6DE4(Actor *owner) {
    BeamState state;
    u8 i;
    BeamObj *obj;

    state.owner = owner;
    state.ownerId = owner->unique_id;
    for (i = 0; i < 2; i++) {
        state.ends[i] = NULL;
    }
    state.flags = 0;
    // functions.h types the 7th argument (the size of the object data) as struct37 *
    obj = (BeamObj *)func_15149130(300, -1, 0x43, -1, 0, 0x37, sizeof(BeamState), 0xFF, 1);
    if (obj != NULL) {
        BeamState *beam;

        beam = &obj->beam;
        memcpy(beam, &state, sizeof(state));
        {
            ParticleDesc spawn;

            spawn.renderMode = 0x4417;
            spawn.unk0 = 0x200004;
            spawn.unk4 = 0;
            spawn.timer = 300;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.a = 0xFF;
            spawn.brightness = 0xFF;
            spawn.width = spawn.height = 1511.0f;
            spawn.pos = D_800A5480;
            spawn.offset = D_800A5480;
            spawn.vel = D_800A5480;
            spawn.fadeStart = 1;
            spawn.fadeRate = 0xFF;
            spawn.scaleStart = 1;
            spawn.flags = 0x64C000;
            spawn.primColorMode = 8;
            spawn.envColorMode = 6;
            spawn.updateFunc = -1;
            spawn.deathFunc = -1;
            spawn.drawFunc = 3;
            spawn.handlerSet = 0;
            spawn.viewMask = 0;
            spawn.views = 0xFF;
            spawn.minSize = 10;
            spawn.texture = 100;
            spawn.r2 = 0;
            spawn.g2 = 0;
            spawn.b2 = 0xFF;
            spawn.alpha = 0xFF;
            spawn.gravity = 0.0f;
            spawn.scaleRate = 1.0f;
            spawn.maxSize = 20.0f;
            beam->ends[0] = func_15130280(&spawn, 1, 0, 0, obj->unkC, obj->unk1);
            spawn.r2 = 0xFF;
            spawn.g2 = 0;
            spawn.b2 = 0;
            spawn.alpha = 0xFF;
            beam->ends[1] = func_15130280(&spawn, 1, 0, 0, obj->unkC, obj->unk1);
        }
    }
    return obj;
}

void func_15145740(Actor *actor, Vec3f *out, Vec3f *arg2, Vec3f *arg3, f32 arg4);
void func_15081690(Actor *actor, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, RayHit *hit, f32 maxDist, s32 arg9,
                   s32 argA, s32 argB, s32 argC, s32 argD, s32 argE);
s32 func_1506196C(u8 *obj, s32 arg1);
void func_1502EA98(u8 *obj, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

/* Per-frame update: re-casts the beam from the owner and moves the end sprites to where it hits. */
void func_150F706C(BeamObj *obj) {
    BeamState *beam;
    Actor *owner;
    Vec3f origin;
    Vec3f hitPos;

    beam = &obj->beam;
    owner = beam->owner;

    if (owner->interaction_state == 0 || owner->unique_id != beam->ownerId) {
        obj->unkE = -1;
        return;
    }

    if (D_800C35EA[0] == 1) {
        if (beam->ends[0] != NULL && beam->ends[1] != NULL) {
            beam->ends[0]->mode = 3;
            beam->ends[1]->mode = 3;
        }
        return;
    }

    origin.x = owner->x_position;
    origin.y = owner->y_position + 46.0f;
    origin.z = owner->z_position;
    func_15145740(owner, &D_800D9A50, NULL, NULL, 0.0f);

    D_800D9A50.x *= 0.001f;
    D_800D9A50.y *= 0.001f;
    D_800D9A50.z *= 0.001f;

    func_15081690(owner, origin.x, origin.y, origin.z,
                  D_800D9A50.x, D_800D9A50.y, D_800D9A50.z,
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

/* Message handler: destroys the beam when its owner is reported gone (0x3E); other messages go to the default handler. */
void func_150F7310(BeamObj *obj, ActorRef *msg, u8 msgId) {
    // matching: taking &obj keeps it in its arg slot (IDO reloads it instead of saving it in s0)
    BeamObj **objp = &obj;
    BeamState *beam;

    if (msgId == 0x3E) {
        beam = &(*objp)->beam;
        // matching: the punned read of msg makes IDO load it a second time for the id compare
        if (msg->actor == beam->owner || beam->ownerId == ((ActorRef *)*(s32 *)&msg)->uniqueId) {
            func_1516972C((struct102 *)*objp);
        }
    } else {
        beam = &(*objp)->beam;
        func_15149514((s32)msg, msgId, &beam->owner, &beam->ownerId, *objp);
    }
}


/* Destroys the beam's end sprites and detaches the beam from its owner. */
void func_150F739C(BeamObj *obj) {
    BeamState *beam;
    u8 i;

    beam = &obj->beam;
    for (i = 0; i < 2; i++) {
        if (beam->ends[i] != NULL) {
            func_1516972C((struct102 *)beam->ends[i]);
        }
    }
    func_1514EDF0(obj, beam->owner);
}

/* Beam callback: tears the beam down (func_150F739C), then hands it to func_1514933C. */
void func_150F740C(BeamObj *obj) {
    func_150F739C(obj);
    func_1514933C((struct260 *)obj);
}

/* Beam callback: tears the beam down (func_150F739C), then hands it to func_15149368. */
void func_150F7438(BeamObj *obj) {
    func_150F739C(obj);
    func_15149368((struct260 *)obj);
}
