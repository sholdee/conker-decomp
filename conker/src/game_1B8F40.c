#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    s16 x, y, z;
} Vec3s;

/* Link fields shared by every object (see game_1944C0.c). */
typedef struct ObjLink {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 list;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ struct ObjLink *prev;
    /* 0x8 */ struct ObjLink *next;
    /* 0xC */ u8 owner;
} ObjLink;

/* Object lists: D_800DCE50[list][type] is the head of a linked list of objects of that type. */
extern ObjLink *D_800DCE50[2][104];

/* While a list is walked, the next node is saved in D_800DD198[depth] (depth D_800DD190). */
#define OBJ_ITER_NEXT ((ObjLink **)D_800DD198)

#define OBJ_TYPE_TRAIL_EMITTER 0x1F

/* Emitter settings, copied into the object from a template by func_1518BCD0. */
typedef struct {
    /* 0x00 */ f32 speedMin;
    /* 0x04 */ f32 speedRange;
    /* 0x08 */ f32 halfWidthMin;
    /* 0x0C */ f32 halfWidthRange;
    /* 0x10 */ s16 pointRange; /* each trail gets 3 + rand % (pointRange + 1) points */
    /* 0x12 */ s16 angleRange;
    /* 0x14 */ struct127 *owner;
    /* 0x18 */ u8 ownerId;     /* owner->unique_id when attached */
    /* 0x19 */ u8 texture;
    /* 0x1A */ u8 flags;
} TrailEmitterParams; /* size 0x1C */

/* Type 0x1F object: emits trails from two points of its owner actor. */
typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ TrailEmitterParams params;
    /* 0x2C */ s32 timers[2]; /* frames until the next trail from each point */
    /* 0x34 */ f32 prevX[2];  /* each point's position on the previous update */
    /* 0x3C */ f32 prevZ[2];
} TrailEmitter; /* size 0x44 */

void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);
TrailEmitter *func_1518BCD0(TrailEmitterParams *params, u8 owner, s32 list);

/* Returns the actor's trail emitter, creating one if its kind has emitter settings. */
TrailEmitter *func_1518BA90(struct127 *actor, u8 owner, s32 list) {
    ObjLink **heads;
    TrailEmitterParams params;
    TrailEmitter *emitter;
    s32 i;

    if (actor == NULL) {
        return NULL;
    }
    for (i = 0; i < 2; i++) {
        heads = D_800DCE50[i];
        for (emitter = (TrailEmitter *)heads[OBJ_TYPE_TRAIL_EMITTER]; emitter != NULL;
             emitter = (TrailEmitter *)emitter->link.next) {
            if (emitter->params.owner == actor || emitter->params.ownerId == actor->unique_id) {
                return emitter;
            }
        }
    }

    switch (actor->id + 1) {
    case 0: /* id 0xFF */
        return NULL;
    case 8:
        params.angleRange = 55;
        params.pointRange = 4;
        params.texture = 5;
        params.flags = 0x13;
        params.speedMin = 2.75999999f;
        params.speedRange = 5.960000038f;
        params.halfWidthMin = 3.210000038f;
        params.halfWidthRange = 5.0f;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        params.angleRange = 0;
        params.pointRange = 5;
        params.texture = 5;
        params.flags = 0x13;
        params.speedMin = 10.0f;
        params.speedRange = 10.0f;
        params.halfWidthMin = 10.0f;
        params.halfWidthRange = 10.0f;
        break;
    default:
        return NULL;
    }
    params.owner = actor;
    params.ownerId = actor->unique_id;
    return func_1518BCD0(&params, owner, list);
}

/* Detaches every trail emitter onto `actor` and destroys it. */
void func_1518BBF4(struct127 *actor) {
    s32 i;
    TrailEmitter *emitter;

    if (actor != NULL) {
        for (i = 0; i < 2; i++) {
            D_800DD190++;
            emitter = (TrailEmitter *)D_800DCE50[i][OBJ_TYPE_TRAIL_EMITTER];
            while (emitter != NULL) {
                OBJ_ITER_NEXT[D_800DD190] = emitter->link.next;
                emitter->params.owner = actor;
                if (actor != NULL || emitter->params.ownerId == actor->unique_id) {
                    func_1516972C((struct102 *)emitter);
                }
                emitter = (TrailEmitter *)OBJ_ITER_NEXT[D_800DD190];
            }
            {
                // matching: a slotless block-scope decl moves the -g3 line so the decrement fills the bnel slot
                extern s8 D_800DD190;

                D_800DD190--;
            }
        }
    }
}

/* Spawns a trail emitter object from `params` with random initial timers. */
TrailEmitter *func_1518BCD0(TrailEmitterParams *params, u8 owner, s32 list) {
    TrailEmitter *emitter;

    emitter = func_15167A68(OBJ_TYPE_TRAIL_EMITTER, list, sizeof(TrailEmitter), 1, owner, 1);
    if (emitter == NULL) {
        return NULL;
    }
    memcpy(&emitter->params, params, sizeof(TrailEmitterParams));
    emitter->timers[0] = func_150ADA20() & 0x1F;
    emitter->timers[1] = func_150ADA20() & 0x1F;
    return emitter;
}

/*
 * struct127 bytes 0x1AC..0x1BA (padding in structs.h): the two points trails are emitted from
 * and the actor's facing direction in 1/256 units.
 */
typedef struct {
    /* 0x1AC */ Vec3s points[2];
    /* 0x1B8 */ s8 dirX;
    /* 0x1B9 */ u8 pad1B9;
    /* 0x1BA */ s8 dirZ;
} ActorTrailPoints;

#define ACTOR_TRAIL_POINTS(actor) ((ActorTrailPoints *)&(actor)->pad1AC)

/* Trail types (see game_175250.c). */

/* Spawn description passed to func_15147DA0 (first argument). */
typedef struct {
    Vec3f pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 pad14;
    u8 pointCount;
} TrailSpawn;

/* One point of a trail; the points form a ring buffer. */
typedef struct {
    Vec3f pos;
    f32 velocityY;
    u16 phase; /* texture coordinate along the trail */
    u8 pad12[2];
} TrailPoint;

/* Motion and appearance block that func_15147DA0 copies from its second argument. */
typedef struct {
    f32 halfWidth;
    Vec3f velocity;
    f32 gravity;
    u8 pad14[4];
    u8 flags;
    u8 texture;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    s16 unk1E;
} TrailMotion; /* size 0x20 */

/* Render settings that func_15147DA0 copies from its 12th argument. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 renderMode;
    union {
        s32 word;
        u8 bytes[4];
    } unk10;
    s32 unk14;
    s32 unk18;
    s8 unk1C;
    s8 unk1D;
} TrailRender; /* size 0x20 */

typedef struct {
    TrailMotion motion;
    u8 state;      /* 0x20: handler index */
    u8 unk21;
    u8 finishFn;
    u8 hitFn;
    u8 hitKind3Fn;
    u8 unk25;
    u8 pad26[2];
    TrailRender render; /* 0x28 */
} TrailParams; /* size 0x48 */

typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ Vec3f position;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u8 pad1E[7];
    /* 0x25 */ u8 capacity; /* number of points in the ring buffer */
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 start;    /* oldest point */
    /* 0x2E */ s8 end;      /* one past the newest point */
    /* 0x2F */ u8 pad2F[0x65];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailParams *params;
} Trail;

typedef struct {
    TrailEmitter *emitter;
    s8 point; /* which of the owner's two points the trail follows */
} TrailEmitterLink;

/* Params of a trail spawned by a TrailEmitter: the TrailParams, then emitter data. */
typedef struct {
    /* 0x00 */ TrailParams base;
    /* 0x48 */ TrailEmitterLink link;
    /* 0x50 */ Vec3f offsets[1]; /* per trail point, relative to the emit point */
} EmitterTrailParams;

extern Trail *func_15147DA0(TrailSpawn *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4,
                            s32 finishFn, s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10,
                            TrailRender *render, s32 arg12, s32 arg13, s32 arg14);

/* Trail emitter update: while the owner is alive, spawns a trail from each point when its timer runs out. */
void func_1518BD60(TrailEmitter *emitter) {
    u8 i;
    s32 point;
    Trail *trail;
    TrailSpawn spawn;
    TrailMotion motion;
    s32 pad; // matching: stack slot between motion and render
    TrailRender render;
    f32 speed;
    f32 sinA;
    f32 cosA;
    s32 angle;
    struct127 *owner;
    TrailEmitterLink *link;

    for (i = 0; i < 2; i++) {
        emitter->timers[i] -= D_800BE9E4;
        owner = emitter->params.owner;
        if (owner != NULL && owner->interaction_state != 0 && owner->unk1D4 != NULL &&
            owner->unique_id == emitter->params.ownerId) {
            if (emitter->timers[i] < 0) {
                point = i;
                if (point) {} // matching: a second use keeps the copy of i in its own register
                angle = func_150ADA20() % (u32)(emitter->params.angleRange + 1);
                sinA = func_151423D8(angle);
                cosA = func_151423D8(angle - 0x40);
                speed = func_150ADA68() * emitter->params.speedRange + emitter->params.speedMin;
                emitter->timers[i] = func_150ADA20() % 23U + 5;
                render.unk0 = 0;
                render.unk4 = 1;
                render.unk8 = 0x160600;
                render.renderMode = 3;
                render.unk10.word = 0x10;
                render.unk14 = 0x80;
                render.unk18 = 0x20;
                render.unk1C = 0;
                render.unk1D = 9;
                spawn.pointCount = func_150ADA20() % (u32)(emitter->params.pointRange + 1) + 3;
                spawn.unk10 = 1;
                spawn.unkE = 1;
                spawn.unkC = func_150ADA20() % 21U + 80;
                motion.texture = emitter->params.texture;
                motion.halfWidth = func_150ADA68() * emitter->params.halfWidthRange + emitter->params.halfWidthMin;
                motion.velocity.y = 0.0f;
                motion.gravity = 0.6539999843f;
                motion.velocity.x = speed * cosA;
                motion.velocity.z = speed * sinA;
                motion.flags = emitter->params.flags | 8;
                motion.unk1A = 0xFF - ((u8)((u32)emitter->params.owner->unk184 >> 5) << 6);
                motion.unk1B = 0xFF;
                trail = func_15147DA0(&spawn, &motion, spawn.pointCount * sizeof(Vec3f) + 8, 5, 1, 0, 2, 1, 0, 0,
                                      0, &render, 0, emitter->link.owner, emitter->link.list);
                if (trail != NULL) {
                    link = &((EmitterTrailParams *)trail->params)->link;
                    link->emitter = emitter;
                    link->point = point;
                }
                owner = emitter->params.owner;
            }
            emitter->prevX[i] = ACTOR_TRAIL_POINTS(owner)->points[i].x;
            emitter->prevZ[i] = ACTOR_TRAIL_POINTS(emitter->params.owner)->points[i].z;
        }
    }
}

/*
 * Trail update for trails spawned by a TrailEmitter: moves each point by its velocity and gravity
 * relative to the owner's emit point (rotated by the owner's facing), then either grows the trail by
 * one point or, once full, hands the owner's motion to the trail and lets it go.
 * Returns 0 when the owner is gone.
 */
s32 func_1518C0B8(Trail *trail) {
    EmitterTrailParams *params;
    TrailPoint *points;
    s32 i;
    TrailEmitterLink *link;
    Vec3f *offsets;
    Vec3f pos;
    f32 dirX;
    f32 dirZ;
    f32 lenSq;
    f32 cosA;
    f32 sinA;
    f32 invLen;
    u16 phase;
    u16 phaseStep;
    f32 ownerVelX;
    f32 ownerVelZ;
    f32 invDt;
    f32 velX;
    f32 velZ;

    points = trail->points;
    params = (EmitterTrailParams *)trail->params;
    link = &params->link;
    offsets = params->offsets;
    i = trail->start;
    if (link->emitter->params.owner == NULL || link->emitter->params.owner->interaction_state == 0 ||
        link->emitter->params.owner->unk1D4 == NULL ||
        link->emitter->params.owner->unique_id != link->emitter->params.ownerId) {
        return 0;
    }
    pos.x = ACTOR_TRAIL_POINTS(link->emitter->params.owner)->points[link->point].x;
    pos.y = ACTOR_TRAIL_POINTS(link->emitter->params.owner)->points[link->point].y;
    pos.z = ACTOR_TRAIL_POINTS(link->emitter->params.owner)->points[link->point].z;
    dirX = ACTOR_TRAIL_POINTS(link->emitter->params.owner)->dirX * (1.0f / 256);
    dirZ = ACTOR_TRAIL_POINTS(link->emitter->params.owner)->dirZ * (1.0f / 256);
    lenSq = dirX * dirX + dirZ * dirZ;
    if (lenSq) {
        // matching: a slotless block-scope decl adds the -g3 line that gives the nop after the mul.s
        extern f32 sqrtf(f32);

        invLen = 1.0f / sqrtf(lenSq);
        cosA = dirX * invLen;
        sinA = dirZ * invLen;
    } else {
        sinA = cosA = 0.0f;
    }
    while (i != trail->end) {
        points[i].velocityY -= params->base.motion.gravity * D_800BE9A4;
        offsets[i].x += params->base.motion.velocity.x * D_800BE9A4;
        offsets[i].y += points[i].velocityY;
        offsets[i].z += params->base.motion.velocity.z * D_800BE9A4;
        points[i].pos.x = (pos.x + offsets[i].x * sinA) + offsets[i].z * cosA;
        points[i].pos.y = offsets[i].y + pos.y;
        points[i].pos.z = (pos.z + offsets[i].z * sinA) - offsets[i].x * cosA;
        i++;
        if (i == trail->capacity) {
            i = 0;
        }
    }
    if (trail->count < trail->capacity - 1) {
        phase = 0;
        phaseStep = 0;
        if (trail->count != 0) {
            phaseStep = 0x1000 / trail->count;
        }
        points[trail->end].pos = pos;
        points[trail->end].velocityY = 0.0f;
        offsets[trail->end].x = 0.0f;
        offsets[trail->end].y = 0.0f;
        offsets[trail->end].z = 0.0f;
        trail->count++;
        trail->end++;
        if (trail->end == trail->capacity) {
            trail->end = 0;
        }
        for (i = trail->start; i != trail->end;) {
            points[i].phase = phase;
            i++;
            phase += phaseStep;
            if (i == trail->capacity) {
                i = 0;
            }
        }
    } else {
        invDt = 1.0f / D_800BE9A4;
        velX = params->base.motion.velocity.x * sinA + params->base.motion.velocity.z * cosA;
        velZ = params->base.motion.velocity.z * sinA - params->base.motion.velocity.x * cosA;
        ownerVelX = (pos.x - link->emitter->prevX[link->point]) * invDt;
        ownerVelZ = (pos.z - link->emitter->prevZ[link->point]) * invDt;
        params->base.motion.velocity.x = velX + ownerVelX;
        params->base.motion.velocity.z = velZ + ownerVelZ;
        if (params->base.motion.flags & 0x17) {
            params->base.state = 3;
        } else {
            params->base.state = 2;
        }
    }
    return 1;
}

s32 func_1518C540(void* arg0) {
    s32 v1;
    void* p;

    p = *(void**)((u8*)arg0 + 0x98);
    v1 = *(s16*)((u8*)arg0 + 0x1C) << 3;
    if (v1 >= 0x100) {
        v1 = 0xFF;
    }
    *(s8*)((u8*)p + 0x1B) = v1;
    if ((v1 & 0xFF) < 0) {
        return 0;
    }
    return 1;
}

void func_1516865C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_15168800(s32 arg0, s32 arg1, s32 arg2);

typedef struct {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    s32 field_0xC;
    s32 field_0x10;
} EffectPoint;

typedef struct {
    u8 pad_0x0[0x20];
    s8 field_0x20;
} EffectState;

typedef struct {
    u8 pad_0x0[0x1];
    u8 field_0x1;
    u8 pad_0x2[0xA];
    u8 field_0xC;
    u8 pad_0xD[0x20];
    s8 field_0x2D;
    u8 pad_0x2E[0x66];
    EffectPoint *field_0x94;
    EffectState *field_0x98;
} EffectEmitter;

typedef struct {
    u8 pad_0x0[0x80];
    s32 field_0x80;
    u8 pad_0x84[0x6];
    s16 field_0x8A;
    s16 field_0x8C;
    s16 field_0x8E;
    s16 field_0x90;
    s16 field_0x92;
    s16 field_0x94;
    s16 field_0x96;
    s16 field_0x98;
    u8 pad_0x9A[0x6];
    s8 field_0xA0;
    s8 field_0xA1;
    s8 field_0xA2;
    u8 pad_0xA3[0x5];
} EffectSpawnPacket;

s32 func_1518C57C(EffectEmitter *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    EffectSpawnPacket sp38;
    EffectState *temp;
    EffectPoint *base;

    temp = arg0->field_0x98;
    base = arg0->field_0x94;
    sp38.field_0x80 = 0;
    sp38.field_0x8A = 0x100;
    sp38.field_0x8C = (s32)base[arg0->field_0x2D].field_0x0;
    sp38.field_0x8E = (s32)arg4;
    sp38.field_0x90 = (s32)base[arg0->field_0x2D].field_0x8;
    sp38.field_0x94 = (func_150ADA20() % 0x15U) + 0x23;
    sp38.field_0x92 = sp38.field_0x94;
    sp38.field_0xA2 = -1;
    sp38.field_0xA0 = 0x12;
    sp38.field_0x98 = 0xC;
    sp38.field_0xA1 = 0;
    sp38.field_0x96 = (s32)arg4;
    func_1516865C(&sp38, 0xFF, 0xFF, 0xFF, 0xFF);
    func_15168800((s32)&sp38, arg0->field_0xC, arg0->field_0x1);
    temp->field_0x20 = 4;
    return 1;
}

u8 func_151D8E20(void);

s32 func_1518C69C(EffectEmitter *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    EffectSpawnPacket sp50;
    EffectState *temp;
    EffectPoint *base;

    temp = arg0->field_0x98;
    base = arg0->field_0x94;
    sp50.field_0x80 = 0;
    sp50.field_0x8A = 0x100;
    sp50.field_0x8C = (s32)base[arg0->field_0x2D].field_0x0;
    sp50.field_0x8E = (s32)arg4;
    sp50.field_0x90 = (s32)base[arg0->field_0x2D].field_0x8;
    sp50.field_0x94 = (func_150ADA20() % 0x15U) + 0x23;
    sp50.field_0x92 = sp50.field_0x94;
    sp50.field_0xA2 = -1;
    sp50.field_0xA0 = 0x12;
    sp50.field_0x98 = 0xC;
    sp50.field_0xA1 = 0;
    sp50.field_0x96 = (s32)arg4;
    func_1516865C(&sp50, 0xFF, 0xFF, 0xFF, 0xFF);
    func_15168800((s32)&sp50, arg0->field_0xC, arg0->field_0x1);
    temp->field_0x20 = 4;
    {
    u8 id;
    f32 pos[3];
    pos[0] = base[arg0->field_0x2D].field_0x0;
    pos[1] = arg4;
    pos[2] = base[arg0->field_0x2D].field_0x8;
    id = func_151D8E20();
    func_151DBCBC(id, func_150ADA68() * 40.0f + 100.0f, 0xFF, arg5, pos, arg0->field_0xC, arg0->field_0x1);
    }
    return 1;
}

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
    u8 unk28;
} SubA1518C850;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB1518C850;

void func_1518C850(struct102 *arg0, s32 arg1, u8 arg2) {
    SubA1518C850 *dst = (SubA1518C850 *)arg0;
    ArgB1518C850 *b = (ArgB1518C850 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        b0 = b->unk0;
        if (b0 == dst->unk24) {
            dst->unk24 = b->u4.w4;
            dst->unk28 = b->unk9;
        } else {
            if (dst->unk24 == b->u4.w4) {
                dst->unk24 = b0;
                dst->unk28 = b->unk8;
            }
trailing_label_1518C850:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == dst->unk24) || (b->u4.b4 == dst->unk28)) {
            func_1516972C(arg0);
        }
    }
}
