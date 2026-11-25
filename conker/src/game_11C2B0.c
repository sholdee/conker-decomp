#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_800A1638[];
extern u8 D_800A163C[];
extern f32 D_800A15F0[][3];
extern s32 (*D_8008FD00)(s32, u8);
extern f32 D_800A1830;
extern f32 D_800A1834;
extern f32 D_800A1870;
extern f32 D_800A1874;
extern u8 D_800A1674;
extern u8 D_800A1680;
extern f32 sinf(f32);

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
} Struct150F0E48Src;

typedef struct {
    Struct150F0E48Src *unk0;
    u8 unk4;
    u8 pad5[3];
    f32 unk8;
    f32 unkC;
    f32 unk10;
} Struct150F0E48Sub;

typedef struct {
    u8 pad0[0x2B];
    u8 unk2B;
    u8 pad2C[0xC];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0x5C];
    Struct150F0E48Src *unkA8;
    u8 unkAC;
} Struct150F0E48Obj;

typedef struct {
    u8 pad_0[0x68];
    u8 field_0x68;
    u8 field_0x69;
    u8 pad_1[0x1A];
    u16 field_0x84;
    u8 pad_2[0x25E];
    s32 field_0x2E4;
} ActorStateFields;

void func_15143134(void *, f32 *, Mtx *);
void func_15143874(s32, f32, f32 *, f32 *);
void func_151C329C(void *, s32, s32);
void *func_151407D0(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32);
struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);
s32 func_15160A58(s32, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_150F0A24(f32 *);

/* Fires gun slot `slot` of `actor`: spawns the shot, a spark burst and a flash at the muzzle. */
struct Beam *func_150EEF80(struct127 *, u8, u8, s32);

void func_150EEE00(struct127 *actor, u8 slot) {
    f32 muzzle[3];
    Header header;
    Header2 origin;
    struct255 *bones;

    if (D_800A163C[slot] & actor->unk94) {
        return;
    }

    func_150EEF80(actor, slot, 0xFF, 1);
    bones = actor->unk1D4;
    if (bones != NULL) {
        func_15143134(D_800A15F0[slot], muzzle, (Mtx *)bones + D_800A1638[slot]);
        header.unk0 = 3;
        header.unk1 = -1;
        header.unk2 = (func_150ADA20() % 3U) + 4;
        header.unk4 = 0;
        origin.unk0 = muzzle[0];
        origin.unk4 = muzzle[1];
        origin.unk8 = muzzle[2];
        func_151602C0(&header, &origin, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 1);
        func_150F0A24(muzzle);
    }
}

/* Message payload: an actor reference (message types 0 and 0x43). */
typedef struct {
    struct127 *actor;
    u8 actorId;
    u8 slot;
} ActorRefMsg;

/* Message payload for type 0x2D: two actors exchange identities. */
typedef struct {
    struct127 *actorA;
    struct127 *actorB;
    u8 idA;
    u8 idB;
} ActorSwapMsg;

typedef union {
    ActorRefMsg ref;
    ActorSwapMsg swap;
} ActorMsg;

/* Sends message 0x43 for gun slot `slot` of `actor`. */
void func_150EEF40(struct127 *actor, u8 slot) {
    ActorRefMsg msg;

    msg.actor = actor;
    msg.actorId = actor->unique_id;
    msg.slot = slot;
    func_151403A8((s32)&msg, 0x43);
}

typedef struct { f32 x, y, z; } Vec3f;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 unk1C;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} BeamSpawn; /* 0x58 */

typedef struct {
    u8 pad0[9];
    u8 hidden;
    u8 padA[4];
    s16 position[3];
} FlashSprite;

/* A beam's muzzle flash (created by func_151602C0). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ FlashSprite *sprite;
    /* 0x18 */ struct Beam *beam;
} Flash;

/* State a gun beam object carries at +0x110. */
typedef struct {
    /* 0x00 */ struct127 *actor;       /* the shooter */
    /* 0x04 */ u8 actorId;             /* actor->unique_id at spawn */
    /* 0x08 */ struct127 *target;
    /* 0x0C */ u8 targetId;
    /* 0x0D */ u8 slot;                /* gun slot, 0 or 1 */
    /* 0x10 */ f32 widthPhase;
    /* 0x14 */ f32 alphaPhase;
    /* 0x18 */ u8 flags;               /* bit 0: the beam is aimed this frame */
    /* 0x1C */ struct Spark *sparks[2];
    /* 0x24 */ struct BeamEmitter *emitter;
    /* 0x28 */ struct Glow *glow;
    /* 0x2C */ Flash *flash;
    /* 0x30 */ Vec3f direction;
    /* 0x3C */ f32 distance;
    /* 0x40 */ f32 inverse;
} BeamLink; /* 0x44 */

typedef struct Beam {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ f32 width;
    /* 0x030 */ u8 pad30[4];
    /* 0x034 */ Vec3f start;
    /* 0x040 */ Vec3f end;
    /* 0x04C */ u8 pad4C[0x10];
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0xB3];
    /* 0x110 */ BeamLink link;
} Beam;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ s8 unk1D;
    /* 0x20 */ f32 unk20[10];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ u8 unk4C;
} SparkSpawn; /* 0x50 */

/* Back-reference a beam's spark carries (at spark->link). */
typedef struct {
    /* 0x00 */ struct127 *actor;
    /* 0x04 */ u8 actorId;
    /* 0x08 */ Beam *beam;
    /* 0x0C */ u8 index;               /* which beam->link.sparks[] entry */
} SparkLink; /* 0x10 */

/* Back-reference a beam's emitter carries at +0x28. */
typedef struct {
    /* 0x00 */ Beam *beam;
    /* 0x04 */ f32 accum;              /* particles owed, spawned one per 1.0 */
} EmitterLink; /* 0x8 */

/* A beam's emitter (created by func_15149130), carrying its EmitterLink at +0x28. */
typedef struct BeamEmitter {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ EmitterLink link;
} BeamEmitter;

typedef struct Spark {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u8 flags;               /* bit 1: drawn */
    /* 0x1D */ u8 pad1D[0x13];
    /* 0x30 */ Vec3f start;
    /* 0x3C */ Vec3f end;
    /* 0x48 */ Vec3f offset;
    /* 0x54 */ f32 distance;
    /* 0x58 */ f32 inverse;
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ SparkLink *link;
} Spark;

extern void *func_151A7950(void *, s32, u8, s32);
struct Glow *func_150EFEC8(struct127 *, u8, u8, u8, u8, Beam *, u8, s32);
Flash *func_150F0198(u8, u8, u8, u8, Beam *, u8, s32);

/* Spawns the beam of gun slot `slot` of `actor`, with its sparks, emitter, glow and flash. */
Beam *func_150EEF80(struct127 *actor, u8 slot, u8 arg2, s32 arg3) {
    Beam *beam;
    BeamSpawn spawn;
    BeamLink init;
    BeamLink *link;

    if (actor == NULL) {
        return NULL;
    }
    if (slot >= 2) {
        return NULL;
    }
    init.widthPhase = func_150ADA68() * 6.283185482f;
    init.alphaPhase = func_150ADA68() * 6.283185482f;
    init.actor = actor;
    init.actorId = actor->unique_id;
    init.target = func_15083E90(0x12);
    init.targetId = 0x12;
    init.slot = slot;
    init.direction = *(Vec3f *)&D_800A5480;
    init.flags = 0;
    spawn.unk0 = 0x94;
    spawn.unk1 = 7;
    spawn.unk2 = 0x2203;
    spawn.unk4 = 300;
    spawn.unk8 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0xFF;
    spawn.unk11 = 0xFF;
    spawn.unk12 = 0xFF;
    spawn.unk13 = 0xFF;
    init.inverse = 1.0f;
    spawn.unk18 = 1.0f;
    init.distance = 0.0f;
    spawn.unk34 = 0.0f;
    spawn.unk38 = 0.0f;
    spawn.unk3C = 0.0f;
    spawn.unk14 = 100.0f;
    spawn.unk1C = *(struct17 *)&D_800A5480;
    spawn.unk28 = *(struct17 *)&D_800A5480;
    spawn.unk40 = 0x046C0008;
    spawn.unk44 = 0xFF;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 7;
    spawn.unk48 = 0;
    spawn.unk4C = 0xFF;
    spawn.unk50 = 0;
    spawn.unk54 = 1;
    spawn.unk56 = 0xFF;
    beam = func_1513D2F0(&spawn, (s32)&D_800A4AA0, 0x1D, 0, 0, 0x1C, 0, 0, 0, 0x44, arg2, arg3);
    if (beam != NULL) {
        link = &beam->link;
        memcpy(link, &init, sizeof(init));
        {
        SparkSpawn part;
        SparkLink sparkLink;
        EmitterLink emitterLink;
        BeamEmitter *emitter;
        part.unk0 = 300;
        part.unk2 = 9;
        part.unk4 = 1.0f;
        part.unk8 = 1.0f;
        part.unkC = 0;
        part.unk14 = 0xFF;
        part.unk15 = 0xDC;
        part.unk16 = 0xA0;
        part.unk17 = 0xC8;
        part.unk1C = 0;
        part.unk1D = -1;
        part.unk20[0] = 0.0f;
        part.unk20[1] = 0.0f;
        part.unk20[2] = 0.0f;
        part.unk20[3] = 0.0f;
        part.unk20[4] = 0.0f;
        part.unk20[5] = 0.0f;
        part.unk20[6] = 0.0f;
        part.unk20[7] = 0.0f;
        part.unk20[8] = 0.0f;
        part.unk20[9] = 0.0f;
        part.unk48 = 1.0f;
        sparkLink.actor = actor;
        part.unk10 = 0.9499999881f;
        part.unk18 = 40.0f;
        sparkLink.actorId = actor->unique_id;
        sparkLink.beam = beam;
        part.unk4C = 1;
        for (sparkLink.index = 0; sparkLink.index < 2; sparkLink.index++) {
            link->sparks[sparkLink.index] = func_151A7950(&part, 0x10, arg2, arg3);
            if (link->sparks[sparkLink.index] != NULL) {
                memcpy(link->sparks[sparkLink.index]->link, &sparkLink, sizeof(sparkLink));
            }
        }
        emitterLink.beam = beam;
        emitterLink.accum = 0.0f;
        emitter = (BeamEmitter *)func_15149130(300, -1, 0x22, -1, 0, 0x20, (struct37 *)8, arg2, arg3);
        link->emitter = emitter;
        if (emitter != NULL) {
            memcpy(&emitter->link, &emitterLink, sizeof(emitterLink));
        }
        link->glow = func_150EFEC8(actor, slot, 0xFF, 0xD9, 0xA0, beam, arg2, arg3);
        link->flash = func_150F0198(0x46, 0xFF, 0x82, 0, beam, arg2, arg3);
        }
    }
    return beam;
}

/* Result record of the beam's hit test. */
typedef struct {
    void *object;
    u8 pad4[0x55];
    u8 count;
    u8 pad5A[0xA];
} BeamHit;

s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
void func_15081690(void *, f32, f32, f32, f32, f32, f32, void *, f32, s32, s32, s32, s32, s32, s32);
s32 func_1505D024(void *, s32, s32, s32);
void func_1502EA98(void *, s32, s32, s32, s32, s32, s32);

/* Per-frame update of a beam: aims it from the muzzle to the target, pulses it and applies its hit. */
s32 func_150EF38C(Beam *beam) {
    BeamLink *link;
    struct127 *actor;
    struct127 *target;
    s16 *position;
    BeamHit hit;
    f32 angle;
    void *object;

    link = &beam->link;
    actor = link->actor;
    target = link->target;
    if (actor->unique_id != link->actorId || actor->interaction_state == 0 || actor->id == 0xFF) {
        return 0;
    }
    if (target->unique_id != link->targetId || target->interaction_state == 0) {
        return 0;
    }
    if (actor->unk94 & D_800A163C[link->slot]) {
        return 0;
    }
    link->flags &= ~1;
    if (actor->unk1D4 != NULL) {
        func_15143134(D_800A15F0[link->slot], &beam->start.x, (Mtx *)actor->unk1D4 + D_800A1638[link->slot]);
        beam->end.x = target->x_position;
        beam->end.y = target->y_position;
        beam->end.z = target->z_position;
        link->direction.x = beam->end.x - beam->start.x;
        link->direction.y = beam->end.y - beam->start.y;
        link->direction.z = beam->end.z - beam->start.z;
        if (func_15145128(&link->direction.x, &link->direction.x, &link->distance, &link->inverse) != 0) {
            link->flags |= 1;
        }
    }
    link->widthPhase += 0.1600000113f * D_800BE9A4;
    angle = func_15144B68(link->widthPhase);
    link->widthPhase = angle;
    beam->width = sinf(angle) * 5.0f + 16.0f;
    link->alphaPhase += 0.1600000113f * D_800BE9A4;
    angle = func_15144B68(link->alphaPhase);
    link->alphaPhase = angle;
    beam->alpha = sinf(angle) * 25.0f + 230.0f;
    if (link->flags & 1) {
        if (link->flash != NULL) {
            link->flash->sprite->hidden = FALSE;
            position = link->flash->sprite->position;
            position[0] = beam->start.x;
            position[1] = beam->start.y;
            position[2] = beam->start.z;
        }
        func_15081690(actor, beam->start.x, beam->start.y, beam->start.z,
                      link->direction.x, link->direction.y, link->direction.z,
                      &hit, link->distance, 1, 0, 1, 4, 0, 0);
        if (hit.count >= 2) {
            object = hit.object;
            func_1502EA98(object, 0xFF, 0, 0, 0x90, 0, 1);
            func_1505D024(object, 0xA0038, 0, actor - D_800CC2D0);
        }
    } else if (link->flash != NULL) {
        link->flash->sprite->hidden = TRUE;
    }
    return 1;
}

s32 func_150EF784(struct127 *actor, s32 arg1, s32 arg2) {
    if (actor->id == 0x28) {
        return 1;
    }
    return 0;
}

/* Releases a beam's sparks, emitter, glow and flash (func_1516972C), then the beam itself. */
void func_150EF7B0(Beam *beam) {
    s32 i;
    void *part;
    BeamLink *link = &beam->link;

    for (i = 0; i < 2; i++) {
        part = link->sparks[i];
        if (part != NULL) {
            func_1516972C(part);
        }
    }
    part = link->emitter;
    if (part != NULL) {
        func_1516972C(part);
    }
    part = link->glow;
    if (part != NULL) {
        func_1516972C(part);
    }
    part = link->flash;
    if (part != NULL) {
        func_1516972C(part);
    }
    func_1513CA6C((struct210 *)beam);
}

/* Like func_150EF7B0, through the func_1516979C / func_1513CAA0 variants. */
void func_150EF860(Beam *beam) {
    s32 i;
    void *part;
    BeamLink *link = &beam->link;

    for (i = 0; i < 2; i++) {
        part = link->sparks[i];
        if (part != NULL) {
            func_1516979C(part);
        }
    }
    part = link->emitter;
    if (part != NULL) {
        func_1516979C(part);
    }
    part = link->glow;
    if (part != NULL) {
        func_1516979C(part);
    }
    part = link->flash;
    if (part != NULL) {
        func_1516979C(part);
    }
    func_1513CAA0((struct210 *)beam);
}

/* Message handler of a beam: releases it when its actor goes away (types 0 and 0x43)
 * and follows its actor and target through an identity swap (type 0x2D). */
void func_150EF910(Beam *beam, ActorMsg *msg, u8 type) {
    BeamLink *link = &beam->link;
    struct127 *actor;
    struct127 *msgActor;
    u8 actorId;
    u8 msgActorId;

    if (type == 0) {
        // matching: all four operands are loaded before the first compare
        actor = link->actor;
        msgActor = msg->ref.actor;
        actorId = link->actorId;
        msgActorId = msg->ref.actorId;
        if (actor == msgActor || actorId == msgActorId) {
            func_1516972C((struct102 *)beam);
        }
    } else if (type == 0x2D) {
        if (link->actor == msg->swap.actorA) {
            link->actor = msg->swap.actorB;
            link->actorId = msg->swap.idB;
        } else if (link->actor == msg->swap.actorB) {
            link->actor = msg->swap.actorA;
            link->actorId = msg->swap.idA;
        }
        if (link->target == msg->swap.actorA) {
            link->target = msg->swap.actorB;
            link->targetId = msg->swap.idB;
        } else {
            if (link->target == msg->swap.actorB) {
                link->target = msg->swap.actorA;
                link->targetId = msg->swap.idA;
            }
        // matching: the label ends a block, so IDO emits the else path's own branch to the epilogue
        swapped:;
        }
    } else if (type == 0x43) {
        if (link->actor == msg->ref.actor || link->actorId == msg->ref.actorId) {
            if (link->slot == msg->ref.slot) {
                func_1516972C((struct102 *)beam);
            }
        }
    }
}

extern f32 D_800A1620[][3];

/* Per-frame update of a beam spark: follows the beam while it is aimed, hides otherwise. */
s32 func_150EFA4C(Spark *spark) {
    Beam *beam;
    BeamLink *link;
    SparkLink *sparkLink;

    sparkLink = spark->link;
    beam = sparkLink->beam;
    link = &beam->link;
    if (link->flags & 1) {
        struct127 *actor;

        actor = link->actor;
        if (actor->unk1D4) {
            u8 slot;

            slot = link->slot;
            spark->start = beam->start;
            spark->end = beam->end;
            spark->distance = link->distance;
            spark->inverse = link->inverse;
            func_15143134(D_800A1620[slot], &spark->offset.x, (Mtx *)actor->unk1D4 + D_800A1638[slot]);
            spark->offset.x = spark->offset.x - spark->start.x;
            spark->offset.y = spark->offset.y - spark->start.y;
            spark->offset.z = spark->offset.z - spark->start.z;
            spark->flags |= 2;
        } else {
            spark->flags &= ~2;
        }
    } else {
        spark->flags &= ~2;
    }
    return 1;
}

typedef struct {
    struct17 pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    s8 unk14;
    u8 unk15;
} Spawn150EFB80;

typedef struct {
    f32 unk0;
    f32 vel[3];
    f32 unk10;
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
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    s32 unk24;
    s32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    u8 unk32;
    f32 unk34;
    s8 unk38;
    u8 unk39;
} Part150EFB80;

extern void func_15143794(s16, s16, f32, f32 *);
extern void func_1515C2F0(Spawn150EFB80 *, s32, Part150EFB80 *, s32, s32, s32);

/* Per-frame update of a beam's emitter: spawns about 0.36-0.66 particles per frame at the beam's end. */
void func_150EFB80(BeamEmitter *emitter) {
    EmitterLink *link;
    Beam *beam;
    u32 yaw;
    u32 pitch;
    Spawn150EFB80 spawn;
    Part150EFB80 part;
    f32 pad; // matching: unused stack slot

    link = &emitter->link;
    link->accum += (364.0f + func_150ADA68() * 300.0f) * 0.001000000047f;
    if (link->accum > 1.0f) {
        beam = link->beam;
        if (beam->link.flags & 1) {
            spawn.pos = *(struct17 *)&beam->end;
            spawn.unkE = 5;
            spawn.unk10 = 10;
            part.unk14 = 4;
            part.unk15 = 2;
            part.unk16 = 3;
            part.unk17 = 0x9D;
            part.unk18 = 0x72;
            part.unk19 = 0x2E;
            part.unk1B = 0;
            part.unk1C = 0;
            part.unk1D = 0;
            part.unk1E = 0;
            part.unk1F = 0xFF;
            part.unk20 = 3;
            part.unk21 = 0x24;
            part.unk24 = 0x200005;
            part.unk28 = 0x60600;
            part.unk2C = 12;
            part.unk2E = 21;
            part.unk30 = 1;
            part.unk32 = 0;
            part.unk34 = 1.0f;
            part.unk38 = -1;
            part.unk39 = 0;
            do {
                spawn.unk15 = func_150ADA20() % 3U + 3;
                spawn.unkC = (func_150ADA20() & 0xF) + 40;
                part.unk0 = func_150ADA68() * 10.0f + 22.0f;
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794(yaw & 0xFF, pitch % 31U - 55, func_150ADA68() * 15.0f + 25.0f, part.vel);
                part.unk10 = (func_150ADA68() * 740.0f + -1761.0f) * 0.001000000047f;
                part.unk1A = func_150ADA20() % 101U + 155;
                func_1515C2F0(&spawn, 0, &part, 0, emitter->unkC, emitter->unk1);
                link->accum -= 1.0f;
            } while (link->accum > 1.0f);
        } else {
            do {
                link->accum -= 1.0f;
            } while (link->accum > 1.0f);
        }
    }
}

/* Back-reference a beam's glow carries at +0x170. */
typedef struct {
    struct127 *actor;
    u8 actorId;
    u8 slot;
    Beam *beam;
} GlowLink;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    s8 unk59;
    u8 pad5A[2];
} Struct98_150EFEC8;

typedef struct {
    u8 unk00;
    u8 unk01;
    s16 unk02;
    s16 unk04;
    u8 pad06[2];
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    u8 pad4C[0xC];
} Struct40_150EFEC8;

typedef struct Glow {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ Vec3f start;
    /* 0x040 */ Vec3f end;
    /* 0x04C */ u8 pad4C[0xC];
    /* 0x058 */ s32 flags;             /* bits 1-2: drawn */
    /* 0x05C */ u8 pad5C[0x114];
    /* 0x170 */ GlowLink link;
} Glow;

/* Spawns the glow sprite of `beam` (gun slot `slot` of `actor`), tinted (r, g, b). */
Glow *func_150EFEC8(struct127 *actor, u8 slot, u8 r, u8 g, u8 b, Beam *beam, u8 arg6, s32 arg7) {
    Glow *glow;
    Struct98_150EFEC8 sp98;
    Struct40_150EFEC8 sp40;
    GlowLink link;

    link.actor = actor;
    link.actorId = actor->unique_id;
    link.slot = slot;
    link.beam = beam;

    sp40.unk00 = D_8008FD00((s32)actor, slot);
    sp40.unk01 = 3;
    sp40.unk02 = 0x2203;
    sp40.unk04 = 300;
    sp40.unk08 = 0;
    sp40.unk0C = 0;
    sp40.unk10 = r;
    sp40.unk11 = g;
    sp40.unk12 = b;
    sp40.unk13 = 0xFF;
    sp40.unk18 = 100.0f;
    sp40.unk14 = 100.0f;
    sp40.unk1C = 0.0f;
    sp40.unk20 = 0.0f;
    sp40.unk24 = 0.0f;
    sp40.unk28 = 0.0f;
    sp40.unk2C = 0.0f;
    sp40.unk30 = 0.0f;
    sp40.unk34 = 1.0f;
    sp40.unk38 = 1.0f;
    sp40.unk3C = 1.0f;
    sp40.unk40 = 0xCD2002;
    sp40.unk44 = 0xFF;
    sp40.unk45 = 0xFF;
    sp40.unk46 = 0;
    sp40.unk47 = 6;
    sp40.unk48 = 0;

    sp98.unk04 = 160.0f;
    sp98.unk00 = 160.0f;
    sp98.unk0C = 80.0f;
    sp98.unk08 = 80.0f;
    sp98.unk10 = 0.5f;
    sp98.unk14 = 0.5f;
    sp98.unk18 = 1.0f;
    sp98.unk1C = D_800A1830;
    sp98.unk20 = D_800A1834;
    sp98.unk24 = -1;
    sp98.unk34 = -1;
    sp98.unk28 = -1;
    sp98.unk38 = -1;
    sp98.unk2C = -1;
    sp98.unk3C = -1;
    sp98.unk30 = -1;
    sp98.unk40 = -1;
    sp98.unk44 = 0;
    sp98.unk48 = 1.0f;
    sp98.unk4C = 0;
    sp98.unk4E = 0;
    sp98.unk50 = 0;
    sp98.unk52 = 0;
    sp98.unk54 = 0xFF;
    sp98.unk55 = 0xFF;
    sp98.unk56 = 0xFF;
    sp98.unk57 = 0xFF;
    sp98.unk58 = 10;
    sp98.unk59 = -1;

    glow = func_151407D0(&sp98, 0x6C, &sp40, 0x1E, 0, 0, 0, -1, arg6, arg7);
    if (glow != NULL) {
        memcpy(&glow->link, &link, sizeof(link));
    }
    return glow;
}

/* Per-frame update of a beam's glow: spans 500 units from the beam's start along its direction. */
s32 func_150F00EC(Glow *glow) {
    Beam *beam;
    BeamLink *link;

    beam = glow->link.beam;
    link = &beam->link;
    if (beam->link.flags & 1) {
        glow->start = beam->start;
        glow->end.x = beam->start.x + (link->direction.x * 500.0f);
        glow->end.y = beam->start.y + (link->direction.y * 500.0f);
        glow->end.z = beam->start.z + (link->direction.z * 500.0f);
        glow->flags |= 6;
    } else {
        glow->flags &= ~4;
        glow->flags &= ~2;
    }
    return 1;
}

/* Spawns the muzzle flash of `beam`, colored (r, g, b, a). */
Flash *func_150F0198(u8 r, u8 g, u8 b, u8 a, Beam *beam, u8 arg5, s32 arg6) {
    Flash *flash;
    Header header;
    Header2 origin;
    Beam *link;

    link = beam; // matching: the payload is copied from a stack local, not the arg slot
    header.unk0 = 2;
    header.unk1 = -1;
    header.unk2 = 300;
    header.unk4 = 0x21;
    origin.unk0 = 0;
    origin.unk4 = 0;
    origin.unk8 = 0;

    flash = (Flash *)func_151602C0(&header, &origin, r, g, b, a, 0xFF, 0, 4, arg5, arg6);
    if (flash != NULL) {
        memcpy(&flash->beam, &link, sizeof(link));
    }
    return flash;
}

void func_150F02A0(Spark *spark);

void func_150F0260(Spark *spark) {
    func_150F02A0(spark);
}

void func_150F0280(Spark *spark) {
    func_150F02A0(spark);
}

/* Clears the beam's reference to this spark. */
void func_150F02A0(Spark *spark) {
    SparkLink *link = spark->link;
    Beam *beam = link->beam;

    beam->link.sparks[link->index] = NULL;
}

void func_150F0318(BeamEmitter *emitter);

void func_150F02C0(BeamEmitter *emitter) {
    func_150F0318(emitter);
    func_1514933C((struct260 *)emitter);
}

void func_150F02EC(BeamEmitter *emitter) {
    func_150F0318(emitter);
    func_15149368((struct260 *)emitter);
}

/* Clears the beam's reference to this emitter. */
void func_150F0318(BeamEmitter *emitter) {
    Beam *beam = emitter->link.beam;

    beam->link.emitter = NULL;
}

void func_150F0380(Glow *glow);

void func_150F0328(Glow *glow) {
    func_150F0380(glow);
    func_151411A4((struct210 *)glow);
}

void func_151411C4(struct210 *arg0);

void func_150F0354(Glow *glow) {
    func_150F0380(glow);
    func_151411C4((struct210 *)glow);
}

/* Clears the beam's reference to this glow. */
void func_150F0380(Glow *glow) {
    Beam *beam = glow->link.beam;

    beam->link.glow = NULL;
}

void func_150F03E8(Flash *flash);
void func_151617C4(Flash *flash);

void func_150F0390(Flash *flash) {
    func_150F03E8(flash);
    func_151617C4(flash);
}

void func_151617E4(Flash *flash);

void func_150F03BC(Flash *flash) {
    func_150F03E8(flash);
    func_151617E4(flash);
}

/* Clears the beam's reference to this flash. */
void func_150F03E8(Flash *flash) {
    Beam *beam = flash->beam;

    beam->link.flash = NULL;
}

extern f32 D_800A1640[][3];
extern f32 D_800A1658[][3];
extern u8 D_800A1670[];
extern s32 func_15145EA4(f32 **src, f32 **dst, Mtx *mtx, s32 count);
extern void func_15102B38(struct127 *, u8, f32 *, f32 *, f32 *, s32, s32, f32, struct17 *, s32, s32, s32, u8, s32);
s32 func_151C229C(struct17 *arg0, struct17 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, struct127 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);

/* Fires a shot from gun slot `slot` of `actor` (only once actor->unk2EC reaches 90),
 * with a spark burst and a trail at the muzzle. */
s32 func_150F03F8(actor, slot, arg2, arg3)
    struct127 *actor;
    u8 slot;
    u8 arg2;
    s32 arg3;
{
    s32 ret;
    struct17 muzzle;
    struct17 dir;
    f32 *src[2];
    f32 *dst[2];
    Header hdr;
    Header2 origin;
    f32 params[2];
    s32 randB;
    f32 rand1;
    f32 rand0;
    s32 randA;

    if (actor == NULL) {
        return 0;
    }
    if (slot >= 2) {
        return 0;
    }
    if (actor->unk1D4 == NULL) {
        return 0;
    }
    if (actor->unk2EC < 90) {
        return 0;
    }
    src[0] = D_800A1640[slot];
    src[1] = D_800A1658[slot];
    dst[0] = &muzzle.unk0;
    dst[1] = &dir.unk0;
    func_15145EA4(src, dst, (Mtx *)actor->unk1D4 + D_800A1670[slot], 2);
    dir.unk0 -= muzzle.unk0;
    dir.unk4 -= muzzle.unk4;
    dir.unk8 -= muzzle.unk8;
    {
        f32 func_150ADA68(void); // matching: this block's extra line steers as1 scheduling

        rand0 = func_150ADA68();
        rand1 = func_150ADA68();
        func_150ADA20();
        ret = func_151C229C(&muzzle, &dir, 0, 0, 0, 0, 150.0f, 0.006666666828f, rand0 * 19.0f + 38.0f,
                            rand1 * 363.0f + 149.0f, 50.0f, 0xFF, actor, 1, 1, 0, 8, 0, 1, 0, 0xA0038, 0.0f,
                            0xFF, -1, 0, arg2, arg3);
    }
    hdr.unk0 = 3;
    hdr.unk1 = -1;
    hdr.unk2 = (func_150ADA20() % 5U) + 4;
    hdr.unk4 = 0;
    origin.unk0 = muzzle.unk0;
    origin.unk4 = muzzle.unk4;
    origin.unk8 = muzzle.unk8;
    randA = (func_150ADA20() % 51U) + 80;
    func_151602C0(&hdr, &origin, randA, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg2, arg3);
    params[1] = (func_150ADA68() * 1906.0f + 969.0f) * 0.001000000047f;
    params[0] = (func_150ADA68() * 911.0f + 628.0f) * 0.1000000015f;
    randA = func_150ADA20();
    randB = func_150ADA20();
    rand0 = func_150ADA68() * 2000.0f + 2000.0f;
    func_15102B38(actor, D_800A1670[slot], D_800A1640[slot], D_800A1658[slot], params,
                  (randA % 3U) + 8, (randB % 1U) + 0xFF, rand0, &muzzle, 0xFF, 0, -1, arg2, arg3);
    return ret;
}

/* Message handler of a beam spark: releases it when its actor goes away (type 0)
 * and follows its actor through an identity swap (type 0x2D). */
void func_150F07E4(Spark *spark, s32 data, u8 type) {
    SparkLink *link = spark->link;
    ActorMsg *msg = (ActorMsg *)data;
    struct127 *msgActor;

    if (type == 0) {
        msgActor = msg->ref.actor;
        if ((msgActor == link->actor) || (msg->ref.actorId == link->actorId)) {
            func_1516972C((struct102 *)spark);
        }
    } else if (type == 0x2D) {
        if (link->actor == msg->swap.actorA) {
            link->actor = msg->swap.actorB;
            link->actorId = msg->swap.idB;
        } else if (link->actor == msg->swap.actorB) {
            link->actor = msg->swap.actorA;
            link->actorId = msg->swap.idA;
        }
    }
}

/* Message handler of a beam glow; same as func_150F07E4. */
void func_150F088C(Glow *glow, ActorMsg *msg, u8 type) {
    GlowLink *link = &glow->link;
    struct127 *msgActor;

    if (type == 0) {
        msgActor = msg->ref.actor;
        if ((msgActor == link->actor) || (msg->ref.actorId == link->actorId)) {
            func_1516972C((struct102 *)glow);
        }
    } else if (type == 0x2D) {
        if (link->actor == msg->swap.actorA) {
            link->actor = msg->swap.actorB;
            link->actorId = msg->swap.idB;
        } else if (link->actor == msg->swap.actorB) {
            link->actor = msg->swap.actorA;
            link->actorId = msg->swap.idA;
        }
    }
}

void func_150F0938(s32 arg0) {
    func_15160A58(arg0, 0x25, &D_800A1674, 2, 0x12C, 4, 0, 0xFF, 0, 0xFF, 0, -1, 0, 0, 0xFF, 1);
    func_15160A58(arg0, 2, &D_800A1680, 2, 0x12C, 0xD, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, 0xFF, 1);
}

typedef struct {
    s32 field_0;
    s32 field_4;
    Vec3f position;
    f32 field_14;
    f32 field_18;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    s16 field_2C;
    s16 field_2E;
    s16 field_30;
    s16 field_32;
    s32 field_34;
    s32 field_38;
    s16 field_3C;
    s16 field_3E;
    s16 field_40;
    u8 bytes_42[23];
    u8 pad59[3];
    s32 field_5C;
    s32 field_60;
    s16 field_64;
    s16 field_66;
    s16 field_68;
    s8 field_6A;
    u8 pad6B;
    f32 field_6C;
    s8 field_70;
    s8 field_71;
    s8 field_72;
    s8 field_73;
} EffectPacket;

void func_15152B38(void *, s32, s32, void *);

/* Spawns a particle effect (func_15152B38) at `pos`; func_150EEE00 uses it at the muzzle. */
void func_150F0A24(f32 *pos) {
    EffectPacket packet;

    packet.field_0 = 9;
    packet.field_4 = 6;
    packet.position = *(Vec3f *)pos;
    packet.field_14 = 14.88f;
    packet.field_18 = 14.88f;
    packet.field_1C = -1.353000045f;
    packet.field_20 = 0.7180000544f;
    packet.field_24 = 39.0f;
    packet.field_28 = 35.0f;
    packet.field_2C = 0;
    packet.field_2E = 0xFF;
    packet.field_30 = -31;
    packet.field_32 = 80;
    packet.field_34 = 3;
    packet.field_38 = 4;
    packet.field_3C = 20;
    packet.field_3E = 30;
    packet.field_40 = 1;
    packet.bytes_42[0] = 4;
    packet.bytes_42[1] = 2;
    packet.bytes_42[2] = 3;
    packet.bytes_42[3] = 0xFF;
    packet.bytes_42[4] = 0xFF;
    packet.bytes_42[5] = 0xFF;
    packet.bytes_42[6] = 0xFF;
    packet.bytes_42[7] = 0;
    packet.bytes_42[8] = 0;
    packet.bytes_42[9] = 0;
    packet.bytes_42[10] = 0;
    packet.bytes_42[11] = 0xFF;
    packet.bytes_42[12] = 0xFF;
    packet.bytes_42[13] = 0xFF;
    packet.bytes_42[14] = 0xFF;
    packet.bytes_42[15] = 0;
    packet.bytes_42[16] = 0;
    packet.bytes_42[17] = 0;
    packet.bytes_42[18] = 0;
    packet.bytes_42[19] = 0xFF;
    packet.bytes_42[20] = 0;
    packet.bytes_42[21] = 3;
    packet.bytes_42[22] = 0x24;
    packet.field_5C = 0x200005;
    packet.field_60 = 0x60600;
    packet.field_64 = 8;
    packet.field_66 = 31;
    packet.field_68 = 1;
    packet.field_6A = 0;
    packet.field_6C = 1.0f;
    packet.field_70 = -1;
    packet.field_71 = 0;
    packet.field_72 = -1;
    packet.field_73 = -1;
    func_15152B38(&packet, 0xFF, 1, pos);
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
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Struct150F0BECParticle; /* 0x70 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern f32 D_800A1860;
extern f32 D_800A1864;
extern f32 D_800A1868;
extern f32 D_800A186C;

void func_150F0BEC(Struct150F0E48Src *arg0) {
    Struct150F0BECParticle spawn;
    Struct150F0E48Sub sub;
    void *ret;
    s32 temp_v0;
    s32 temp_v1;

    spawn.unk1D = 0x2B;
    spawn.unk8 = 0x4403;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0x70600;
    spawn.unkA = 0x12C;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk14 = 0xFF;
    spawn.unk15 = 0xFF;
    spawn.unk16 = 0xFF;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0xD9;
    spawn.unk1A = 0xA0;
    spawn.unk1B = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk2C = 250.0f;
    spawn.unk28 = 250.0f;
    spawn.pos.unk0 = arg0->unk14;
    spawn.pos.unk4 = arg0->unk18;
    spawn.pos.unk8 = arg0->unk1C;
    spawn.unk3C = *(struct17 *)&D_800A5480;
    spawn.vel = *(struct17 *)&D_800A5480;
    spawn.unk1E = 1;
    spawn.unk20 = 0xFF;
    spawn.unk22 = 1;
    spawn.unk54 = 0.0f;
    spawn.unk24 = 1.0f;
    if (func_150ADA20() & 1) {
        temp_v1 = 0x40;
    } else {
        temp_v1 = 0;
    }
    if (func_150ADA20() & 1) {
        temp_v0 = 0x80;
    } else {
        temp_v0 = 0;
    }
    spawn.unk58 = (temp_v0 | temp_v1) | 0x8000 | 0x4000 | 0x40000;
    spawn.unk60 = 6;
    spawn.unk61 = 6;
    spawn.unk62 = 0x25;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0xA;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    spawn.unk68 = 0;
    sub.unk0 = arg0;
    spawn.unk6C = D_800A1860;
    sub.unk4 = arg0->unk3B;
    sub.unk8 = func_150ADA68() * 2.0f * D_800A1864;
    sub.unkC = func_150ADA68() * 2.0f * D_800A1868;
    sub.unk10 = func_150ADA68() * 2.0f * D_800A186C;
    ret = func_15130280(&spawn, 1, 0, 0x14, 0xFF, 1);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0xA8, &sub, sizeof(sub));
    }
}

s32 func_150F0E48(Struct150F0E48Obj *arg0, s32 arg1) {
    Struct150F0E48Sub *sub;
    Struct150F0E48Src *src;

    sub = (Struct150F0E48Sub *)((u8 *)arg0 + 0xA8);
    src = sub->unk0;
    if (src->unk3B != sub->unk4) {
        return 0;
    }

    arg0->unk40 = src->unk14;
    arg0->unk44 = src->unk18;
    arg0->unk48 = src->unk1C;
    sub->unk8 += D_800A1870 * D_800BE9A4;
    sub->unkC += D_800A1874 * D_800BE9A4;
    sub->unk10 += 0.25f * D_800BE9A4;
    sub->unk8 = func_15144B68(sub->unk8);
    sub->unkC = func_15144B68(sub->unkC);
    sub->unk10 = func_15144B68(sub->unk10);
    arg0->unk38 = (sinf(sub->unk8) * 243.0f) + 780.0f;
    arg0->unk3C = (sinf(sub->unkC) * 243.0f) + 780.0f;
    arg0->unk2B = (sinf(sub->unk10) * 50.0f) + 200.0f;
    return 1;
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA150F1020;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB150F1020;

void func_150F1020(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA150F1020 *temp_v0 = (SubA150F1020 *)((u8 *)arg0 + 0xA8);
    ArgB150F1020 *b = (ArgB150F1020 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                temp_v0->unk4 = b->unk8;
            }
        block_150F1020: ;
        }
    } else if (arg2 == 0 || arg2 == 0x43) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

void func_150F10D4(struct210 *arg0) {
    s8 sp40[0x18];
    struct260 *temp_v0;

    *(struct210 **)&sp40[0] = arg0;
    sp40[4] = *(u8 *)((s32)arg0 + 0x3B);
    *(f32 *)&sp40[8] = 0.0f;
    *(f32 *)&sp40[0xC] = *(f32 *)((s32)arg0 + 0x14);
    *(f32 *)&sp40[0x10] = *(f32 *)((s32)arg0 + 0x18);
    *(f32 *)&sp40[0x14] = *(f32 *)((s32)arg0 + 0x1C);

    temp_v0 = func_15149130(0x12C, -1, 0x4C, -1, 0, 0x3A, (struct37 *)0x18, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp40, 0x18);
    }
}

typedef struct {
    Struct150F0E48Src *unk0;
    u8 unk4;
    u8 pad5[3];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} Link150F1170;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    u8 pad10[0x18];
    Link150F1170 unk28;
} Obj150F1170;

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    s8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Sub150F1170;

void func_150F1170(Obj150F1170 *arg0) {
    Link150F1170 *sub;
    Struct150F0E48Src *src;
    struct17 pos;
    f32 dx;
    f32 dy;
    f32 dz;
    Struct150F0BECParticle spawn;
    Sub150F1170 ext;
    void *ret;
    s32 a;

    src = arg0->unk28.unk0;
    pos = *(struct17 *)&arg0->unk28.unkC;
    sub = &arg0->unk28;
    if ((*(s32 *)src == 0) || (src->unk3B != sub->unk4)) {
        arg0->unkE = -1;
        return;
    }
    sub->unkC = src->unk14;
    sub->unk10 = src->unk18;
    sub->unk14 = src->unk1C;
    dx = sub->unkC - pos.unk0;
    dy = sub->unk10 - pos.unk4;
    dz = sub->unk14 - pos.unk8;
    sub->unk8 += (0.1090000048f + func_150ADA68() * 0.2840000093f) * D_800BE9A4;
    if (sub->unk8 > 1.0f) {
        {
        s32 pad;
        }
        ext.unkC = 0.986371994f;
        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.unk1E = 0x23;
        spawn.unk20 = 7;
        spawn.unk58 = 0x90DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x20;
        spawn.unk63 = -1;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 0x23;
        spawn.unk14 = 0xDD;
        spawn.unk15 = 0xD3;
        spawn.unk16 = 0xCD;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0x57;
        spawn.unk19 = 0x55;
        spawn.unk1A = 0x5A;
        spawn.unk1C = 0xFF;
        spawn.unk3C.unk0 = 0.0f;
        spawn.unk3C.unk4 = 0.0f;
        spawn.unk3C.unk8 = 0.0f;
        spawn.unk24 = 1.029834032f;
        do {
            {
            f32 r;
            r = func_150ADA68();
            spawn.pos.unk0 = dx * r + pos.unk0;
            spawn.pos.unk4 = dy * r + pos.unk4;
            spawn.pos.unk8 = dz * r + pos.unk8;
            }
            spawn.vel.unk0 = dx * D_800BE9A8 * 0.3330000043f;
            spawn.vel.unk4 = dy * D_800BE9A8 * 0.3330000043f;
            spawn.vel.unk8 = dz * D_800BE9A8 * 0.3330000043f;
            ext.unk0 = func_150ADA20();
            ext.unk1 = (s16)func_150ADA20();
            ext.unk2 = (func_150ADA20() % 5U) + 4;
            ext.unk3 = (func_150ADA20() % 5U) + 4;
            ext.unk4 = func_150ADA68() * 39.0f;
            ext.unk8 = func_150ADA68() * 39.0f;
            spawn.unk54 = func_150ADA68() * 0.5139999986f + 0.6560000181f;
            spawn.unk58 &= ~0xC0;
            a = (func_150ADA20() & 1) ? 0x80 : 0;
            spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | a;
            spawn.unk1B = (func_150ADA20() % 71U) + 0x50;
            spawn.unkA = (func_150ADA20() % 31U) + 0x3C;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 514.0f + 404.0f;
            ret = func_15130280(&spawn, 1, 0, 0x10, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &ext, sizeof(ext));
            }
            sub->unk8 -= 1.0f;
        } while (sub->unk8 > 1.0f);
    }
}

void func_150F15F8(s32 arg0, s32 arg1, u8 arg2) {
    s32 *p = &arg0;
    s32 q;
    s32 b0;

    if (arg2 == 0x43) {
        b0 = *(s32 *)arg1;
        q = *p + 0x28;
        if ((b0 == *(s32 *)q) || (*(u8 *)(q + 4) == *(u8 *)(*(s32 *)&arg1 + 4))) {
            func_1516972C((struct102 *)*p);
        }
    } else {
        q = *p + 0x28;
        func_15149514(arg1, arg2, q, q + 4, *p);
    }
}

struct func150F1684_sub {
    s32 unk0;
    u8 unk4;
};

void func_150F1684(struct func150F1684_sub *arg0, struct func150F1684_sub *arg1, u8 arg2) {
    struct func150F1684_sub *temp = (struct func150F1684_sub *)((s32)arg0 + 0x18);
    s32 word;

    if (arg2 == 0x43) {
        word = arg1->unk0;
        if ((temp->unk0 == word) || (arg1->unk4 == temp->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x8F];
    s32 unk94;
    u8 pad98[0x13C];
    s32 unk1D4;
} Struct150F16DC;

extern f32 D_800A168C[][4][2][3];
extern f32 D_800A174C[][4][2][3];
extern u8 D_800A180C[][2];

void func_150F16DC(Struct150F16DC *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    struct17 pos;
    u8 idx;

    if (arg0 == NULL) {
        return;
    }
    if (((u8 *)&arg2)[3] >= 4) {
        return;
    }
    if (((u8 *)&arg1)[3] >= 2) {
        return;
    }
    if (arg0->unk4 == 0x23) {
        idx = 1;
    } else {
        const u8 masks[2] = { 4, 2 };
        if (masks[((u8 *)&arg1)[3]] & arg0->unk94) {
            return;
        }
        idx = 0;
    }
    {
        s32 srcs[1];
        s32 dsts[1];
        Header hdr;
        Header2 hdr2;
        f32 sp5C[2];
        s32 sp58;
        f32 rand0;
        s32 sp50;
        f32 (*sp4C)[3];
        u8 *sp48;

        if (arg0->unk1D4 == 0) {
            return;
        }
        srcs[0] = (s32)D_800A168C[idx][((u8 *)&arg2)[3]][((u8 *)&arg1)[3]];
        dsts[0] = (s32)&pos;
        sp50 = (D_800A180C[idx][((u8 *)&arg1)[3]] << 6) + arg0->unk1D4;
        sp48 = &D_800A180C[idx][((u8 *)&arg1)[3]];
        sp4C = D_800A168C[idx][((u8 *)&arg2)[3]];
        func_15145EA4(srcs, dsts, sp50, 1);
        hdr.unk0 = 3;
        hdr.unk1 = -1;
        hdr.unk2 = (func_150ADA20() % 5U) + 4;
        hdr.unk4 = 0;
        hdr2.unk0 = pos.unk0;
        hdr2.unk4 = pos.unk4;
        hdr2.unk8 = pos.unk8;
        sp50 = (func_150ADA20() % 51U) + 0x50;
        func_151602C0(&hdr, &hdr2, sp50, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,
                      ((u8 *)&arg3)[3], arg4);
        sp5C[1] = (func_150ADA68() * 701.0f + 1925.0f) * 0.001000000047f;
        sp5C[0] = (func_150ADA68() * 1155.0f + 1526.0f) * 0.1000000015f;
        sp50 = func_150ADA20();
        sp58 = func_150ADA20();
        rand0 = func_150ADA68() * 1877.0f + 2414.0f;
        func_15102B38((struct127 *)arg0, *sp48, sp4C[((u8 *)&arg1)[3]],
                      D_800A174C[idx][((u8 *)&arg2)[3]][((u8 *)&arg1)[3]], sp5C,
                      (sp50 % 6U) + 8, (sp58 % 1U) + 0xFF, rand0, &pos, 0xFF, 0, -1,
                      ((u8 *)&arg3)[3], arg4);
    }
}

typedef struct {
    u16 v[3];
} Rates150F1A00;

typedef struct {
    u8 pad0[0x14];
    s16 unk14;
} Timer150F1A00;

typedef struct {
    u8 pad0[0xF80];
    f32 m[4][4];
} Model150F1A00;

typedef struct {
    u8 pad0[0x1D4];
    Model150F1A00 *unk1D4;
    u8 pad1D8[0x10C];
    s32 unk2E4;
} Actor150F1A00;

extern Rates150F1A00 D_80088B00;
extern f32 D_800A18E8;
extern f32 D_800A18EC;
extern f32 D_800A18F0;
f32 cosf(f32);

void func_150F1A00(Actor150F1A00 *arg0) {
    Model150F1A00 *model;
    Rates150F1A00 rates;
    s32 idx;
    Timer150F1A00 *timer;
    u16 rate;
    f32 scale;
    s32 i;
    s32 j;
    f32 *row;

    rates = D_80088B00;
    idx = 0;
    if ((arg0->unk2E4 & 3) == 3) {
        idx = 1;
    }
    if ((arg0->unk2E4 & 0xC) == 0xC) {
        idx = 2;
    }
    rate = rates.v[idx];
    timer = (Timer150F1A00 *)&D_800CC298;
    timer->unk14 += rate * D_800BE9E4;
    scale = (((cosf(timer->unk14 * D_800A18E8) + 1.0f) * 0.5f) * D_800A18EC) + D_800A18F0;
    model = arg0->unk1D4;
    for (i = 0, row = model->m[0]; i < 3; i++, row += 4) {
        for (j = 0; j < 3; j++) {
            row[j] *= scale;
        }
    }
}

typedef struct {
    u8 pad0[0x12];
    s16 unk12;
} Tilt150F1B48;

/* D_800CC298 is typed s32 in variables.h; this function sees it as a tilt record. */
#define TILT ((Tilt150F1B48 *)&D_800CC298)

extern f32 D_800A18F4;
extern f32 D_800A18F8;

s16 *func_150F1B48(s16 *arg0, s32 arg1) {
    s32 down;
    s32 step;
    s32 v;
    s32 i;

    down = (D_800CC2D0[arg1].unk84.uh == 0x21) ? 1 : 0;
    step = D_800BE9E4 * 0x222;
    if (down) {
        v = TILT->unk12 - step;
        if (D_800A18F4 < v) {
            TILT->unk12 = v;
        } else {
            TILT->unk12 = -0x1999;
        }
    } else {
        v = TILT->unk12 + step;
        if (v < D_800A18F8) {
            TILT->unk12 = v;
        } else {
            TILT->unk12 = 0x1555;
        }
    }
    for (i = 0; i < 6; i++) {
        *arg0++ = 0xB2 + i * 6;
        *arg0++ = TILT->unk12;
    }
    return arg0;

}

void func_150F1CB0(ActorStateFields *arg0) {
    if (arg0->field_0x84 == 0x14) {
        arg0->field_0x68 = 0x1B;
    } else {
        arg0->field_0x68 = 0xC;
    }
    arg0->field_0x69 = 0x13;
    if ((arg0->field_0x2E4 & 0x3) == 0x3) {
        arg0->field_0x69 = 0x14;
    }
    if ((arg0->field_0x2E4 & 0xC) == 0xC) {
        arg0->field_0x69 = 0x17;
    }
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct17 pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u8 unk40;
} Cfg150F1D10A;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct17 pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
} Cfg150F1D10B;

extern s32 D_800A18C0[];
extern f32 D_800A18CC[];
extern s32 D_800A18D8[];
extern f32 D_800A18E0[];

void func_151D5334(struct17 *, f32, f32, f32, s32, u8, s32);
void func_151D5514(struct17 *, u8, s32);
void func_151D3FF4(struct17 *, u8, s32);
void func_151541B8(struct17 *, f32, f32, f32, f32, u8, s32);
void func_1515080C(Cfg150F1D10A *, s32 *, f32 *, s32, s32, s32, s32, s32, s32, s32, f32, s32, s32, u8, s32);
void func_15152190(Cfg150F1D10B *, s32 *, f32 *, s32, f32, s32, u8, s32);

void func_150F1D10(struct127 *arg0, u8 arg1, s32 arg2) {
    struct17 pos;
    f32 scale;

    if (arg0 == NULL) {
        return;
    }
    pos.unk0 = arg0->x_position;
    pos.unk4 = arg0->y_position + 20.0f;
    pos.unk8 = arg0->z_position;
    scale = (arg0->xz_scale + arg0->y_scale) * 0.5f;
    func_151D5404(&pos, 506.0f, 1013.0f, 0.0009871669f, 0xF, 0x14, arg1, arg2);
    func_151D5334(&pos, 506.0f, 1013.0f, 0.0009871669f, 5, arg1, arg2);
    func_151D5514(&pos, arg1, arg2);
    func_10010F88((func_150ADA20() & 8) + 0x2B6, 0x7FFF, 0, 0, 0, pos.unk0, pos.unk4, pos.unk8, 0xFA0, 0x1770);
    func_151D3FF4(&pos, arg1, arg2);
    func_151541B8(&pos, func_150ADA68() * 4.0f + 12.0f, 1.6409999f,
                  (f32)(u32)(func_150ADA20() % 0x38U + 0xC8), 0.0f, arg1, arg2);
    {
        Cfg150F1D10A a;
        Cfg150F1D10B b;

    a.unk0 = 3;
    a.unk4 = 0;
    a.pos = pos;
    a.unk1C = 30.0f;
    a.unk20 = 20.0f;
    a.unk24 = -1.855000138f;
    a.unk28 = 0.6980000138f;
    a.unk14 = 0;
    a.unk16 = 0xFF;
    a.unk18 = -0x40;
    a.unk1A = 0x2C;
    a.unk2C = 0x3C;
    a.unk2E = 0x14;
    a.unk30 = 1 * scale;
    a.unk34 = 0 * scale;
    a.unk38 = 8.990000725f;
    a.unk3C = 0.0f;
    a.unk40 = 0xC;
    func_1515080C(&a, D_800A18C0, D_800A18CC, 3, 0, 0, 0, 1, -1, 0, 0.0f, 0, 0, arg1, arg2);
    b.unk0 = 5;
    b.unk4 = 3;
    b.pos = pos;
    b.unk1C = 25.0f;
    b.unk20 = 15.0f;
    b.unk14 = 0;
    b.unk16 = 0xFF;
    b.unk18 = -0x40;
    b.unk1A = 0x1E;
    b.unk2C = 0x46;
    b.unk2E = 0x14;
    b.unk24 = -1.239000082f;
    b.unk28 = 0.3760000169f;
    b.unk30 = 0.503000021f;
    b.unk34 = 0.2560000122f;
    b.unk38 = 13.93300056f;
    func_15152190(&b, D_800A18D8, D_800A18E0, 2, 0.0f, 0, arg1, arg2);
    }
}

extern u8 D_800D98F0;
extern f32 D_800A191C;

void func_150F20F0(s32 arg0) {
    f32 x;
    f32 z;
    s32 vol;
    s32 sp48;
    s32 sp44;
    struct127 *obj;

    obj = &D_800CC2D0[arg0];
    x = obj->x_position;
    z = obj->z_position;
    x *= x;
    z *= z;
    /* squares the already-squared terms: compares x^4 + z^4, as golden does */
    x = (x * x) + (z * z);
    if (D_800D98F0 != 0) {
        if (D_800BE9E4 < D_800D98F0) {
            D_800D98F0 -= D_800BE9E4;
        } else {
            D_800D98F0 = 0;
        }
    } else if (x > D_800A191C) {
        func_100114D0(0, (s32)D_800CC2D0[arg0].y_position, 0, 0x7FFF, 0x4E20, 0x4A38, &vol, &sp48, &sp44);
        vol += 0x80;
        D_800D98F0 = (func_150ADA20() & 0x7F) + 0x80;
        func_10010F30(((u32)func_150ADA20() % 3) + 0x6C, 0x5DC0, vol, 0, 0);
    }
}

void func_150F2230(struct127 *arg0, s32 arg1, s32 arg2) {
    struct { f32 x; f32 y; f32 z; } sp2C;
    struct { f32 x; f32 y; f32 z; } sp20;

    if ((arg0->unk1D4 != NULL) && ((arg0->unk74 & 0xF) != 0xF)) {
        sp2C.x = 0.0f;
        sp2C.y = 0.0f;
        sp2C.z = 0.0f;
        func_15143874((s16)(func_150ADA20() & 0xFF), 100.0f, &sp2C.x, &sp2C.z);
        func_15143134(&sp2C.x, &sp20.x, (s32)arg0->unk1D4 + 0x4C0);
        func_151C329C(&sp20.x, (u8)arg1, arg2);
    }
}
