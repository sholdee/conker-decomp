#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Ground trails (skid marks / tracks) left behind an actor. A TrailOwner keeps one trail per side
 * (side 6 and side 7). Each Trail is a ring buffer of TrailPoints that age, drift outwards and fade;
 * while the actor's slot has no trail, a TrailSpawner waits to create a new one.
 */

#define TRAIL_SIDE_6 6
#define TRAIL_SIDE_7 7

typedef struct TrailVec3 {
    f32 x;
    f32 y;
    f32 z;
} TrailVec3;

struct Trail;
struct TrailSpawner;

typedef struct TrailSide {
    /* 0x0 */ struct Trail *trail;
    /* 0x4 */ struct TrailSpawner *spawner;
} TrailSide;

typedef struct TrailSet {
    /* 0x0 */ TrailSide sides[2]; /* [0] = side 6, [1] = side 7 */
    /* 0x10 */ f32 sideOffset;    /* distance of each trail from the actor's centre line */
} TrailSet;

typedef struct TrailOwner {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ struct127 *actor;
    /* 0x1C */ u8 actorId; /* actor->unique_id this owner belongs to */
    /* 0x1D */ u8 pad1D[0x17];
    /* 0x34 */ u8 unk34[0x24];
    /* 0x58 */ TrailSet trails;
} TrailOwner;

/* Per-trail parameters, copied into the trail object when it is created (0x50 bytes). */
typedef struct TrailConfig {
    /* 0x00 */ TrailOwner *owner;
    /* 0x04 */ u8 flags;           /* 1: hidden below 2 points, 2: fade in at the tail, 4: fade out at the head */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 pointsPerUnit;   /* new points emitted per unit of distance moved */
    /* 0x0C */ f32 pendingPoints;
    /* 0x10 */ TrailVec3 emitPos;   /* where the last point was emitted */
    /* 0x1C */ f32 emitTime;
    /* 0x20 */ f32 actorX;
    /* 0x24 */ f32 actorZ;
    /* 0x28 */ s16 texS;            /* texture s of the next point */
    /* 0x2A */ u8 color[3];
    /* 0x2D */ u8 pad2D;
    /* 0x2E */ s16 pointLife;
    /* 0x30 */ s16 pointAlpha;
    /* 0x32 */ s16 pointFadeDelay;
    /* 0x34 */ s16 alphaDecay;      /* alpha lost per frame once the fade delay has run out */
    /* 0x36 */ u8 pad36[2];
    /* 0x38 */ f32 startWidth;
    /* 0x3C */ f32 widthGrowth;
    /* 0x40 */ u8 tailFadeCount;
    /* 0x41 */ u8 tailFadeStep;
    /* 0x42 */ u8 headFadeCount;
    /* 0x43 */ u8 headFadeStep;
    /* 0x44 */ u8 texSStep;
    /* 0x45 */ u8 texSStepRandom;
    /* 0x46 */ s16 texT;
    /* 0x48 */ s16 texTSpeed;
    /* 0x4A */ s16 texTSpan;
    /* 0x4C */ f32 driftSpeed;
} TrailConfig;

typedef struct TrailPoint {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 z;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 width;
    /* 0x10 */ s16 alpha;
    /* 0x12 */ s16 fadeDelay;
    /* 0x14 */ s16 fade; /* alpha scale out of 256 for the fades at either end */
    /* 0x16 */ s16 texS;
    /* 0x18 */ s16 life;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ f32 velX;
    /* 0x20 */ f32 velZ;
} TrailPoint; /* size 0x24 */

typedef struct Trail {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ TrailVec3 pos;
    /* 0x1C */ u8 pad1C[2];
    /* 0x1E */ u16 flags;
    /* 0x20 */ s32 side;
    /* 0x24 */ u8 pad24;
    /* 0x25 */ u8 capacity;
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail;  /* oldest point */
    /* 0x2E */ s8 head;  /* next free slot */
    /* 0x2F */ u8 pad2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 pad31[0x23];
    /* 0x54 */ TrailVec3 tailPos;
    /* 0x60 */ f32 groundY;
    /* 0x64 */ u8 pad64[0x20];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailConfig *config;
} Trail;

typedef struct TrailLink {
    /* 0x0 */ TrailOwner *owner;
    /* 0x4 */ u8 side;
} TrailLink;

typedef struct TrailSpawner {
    /* 0x00 */ u8 pad0[0xD];
    /* 0x0D */ u8 flags;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ TrailLink link;
} TrailSpawner;

extern f32 D_800A8CE0;
extern f32 D_800A8CE4;
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

f32 fabsf(f32);
#pragma intrinsic(fabsf)

/* Computes where side `side`'s trail attaches to the owner's actor; returns 0 if the actor is gone. */
s32 func_1519EF90(config, side, out)
    TrailConfig *config;
    u8 side;
    TrailVec3 *out;
{
    struct127 *actor;
    TrailSet *trails;
    f32 dx;
    f32 dz;
    f32 dirX;
    f32 dirZ;
    f32 dist;

    trails = &config->owner->trails;
    actor = config->owner->actor;
    if (actor->unk1D4 == NULL) {
        return 0;
    }
    dx = actor->x_position - config->actorX;
    dz = actor->z_position - config->actorZ;
    if ((D_800A8CE0 < fabsf(dx)) || (D_800A8CE0 < fabsf(dz))) {
        f32 inv;

        inv = 1.0f / sqrtf((dx * dx) + (dz * dz));
        dirX = dz * inv;
        dirZ = dx * inv;
    } else {
        u8 angle;

        angle = func_15143E08(actor);
        dirX = func_151423D8(angle);
        dirZ = func_151423D8(angle - 0x40);
    }
    if (side == TRAIL_SIDE_6) {
        dist = trails->sideOffset;
    } else {
        dist = -trails->sideOffset;
    }
    out->x = actor->x_position + (dist * dirX);
    out->z = actor->z_position - (dist * dirZ);
    if (actor->unk118 < D_800A8CE4) {
        out->y = actor->y_position;
    } else {
        out->y = actor->unk118;
    }
    return 1;
}

void func_151478F4(Trail *);

void func_1519F108(Trail *trail) {
    TrailConfig *config = trail->config;
    TrailSide *sides;

    if (config->owner != NULL) {
        sides = config->owner->trails.sides;
        if (trail->side == TRAIL_SIDE_6) {
            sides[0].trail = NULL;
        }
        if (trail->side == TRAIL_SIDE_7) {
            sides[1].trail = NULL;
        }
    }
    func_151478F4(trail);
}

void func_15147928(Trail *);

void func_1519F168(Trail *trail) {
    TrailConfig *config = trail->config;
    TrailSide *sides;

    if (config->owner != NULL) {
        sides = config->owner->trails.sides;
        if (trail->side == TRAIL_SIDE_6) {
            sides[0].trail = NULL;
        }
        if (trail->side == TRAIL_SIDE_7) {
            sides[1].trail = NULL;
        }
    }
    func_15147928(trail);
}

/* Spawn descriptor for func_15147A80; it becomes the new trail's fields from 0x10 on. */
typedef struct TrailHeader {
    /* 0x00 */ TrailVec3 pos;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u16 flags;
    /* 0x10 */ s32 side;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 capacity;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 unk18;
} TrailHeader;

extern u8 D_800AB414[][3]; /* rgb per colour index */
u8 func_151D8E20(void);
Trail *func_15147A80(TrailHeader *, s32, s32, s32, s32, s32, s32, s32, void *, s32, s32);

/* Creates the trail for `side` behind the owner's actor. */
Trail *func_1519F1C8(owner, side)
    TrailOwner *owner;
    u8 side;
{
    struct127 *actor;
    TrailHeader header;
    TrailConfig config;
    Trail *trail;
    u8 colorIndex;
    u32 sideWord; // matching: a widened copy of side, narrowed again for func_1519EF90 (andi)

    actor = owner->actor;
    colorIndex = func_151D8E20();
    config.owner = owner;
    header.flags = 0x42;
    header.unkC = 300;
    header.capacity = 10;
    config.pointLife = 200;
    config.flags = 2 | 4;
    config.tailFadeCount = 3;
    config.tailFadeStep = 0x55;
    config.headFadeCount = 3;
    config.headFadeStep = 0x55;
    config.texSStep = 0x88;
    config.texSStepRandom = 0xC4;
    config.texS = 0;
    config.texT = 0;
    config.pointAlpha = 255;
    config.pointFadeDelay = 40;
    config.alphaDecay = 25;
    config.texTSpeed = 6;
    config.texTSpan = 0x325;
    config.pointsPerUnit = 0.0150000006f;
    config.startWidth = 5.800000191f;
    config.widthGrowth = 1.0f;
    config.pendingPoints = 0.0f;
    config.emitTime = 0.0f;
    sideWord = side;
    header.side = sideWord;
    config.driftSpeed = 1.5f;
    config.actorX = actor->x_position;
    config.actorZ = actor->z_position;
    if (func_1519EF90(&config, (u8)sideWord, &header.pos) != 0) {
        config.emitPos = header.pos;
        header.flags |= 4;
    }
    config.color[0] = D_800AB414[colorIndex][0];
    config.color[1] = D_800AB414[colorIndex][1];
    config.color[2] = D_800AB414[colorIndex][2];
    trail = func_15147A80(&header, sizeof(TrailConfig), sizeof(TrailPoint), 5, 5, 5, 0, 0, owner->unk34, 0xFF, 1);
    if (trail != NULL) {
        memcpy(trail->config, &config, sizeof(TrailConfig));
    }
    return trail;
}

/* Gives the owner a new trail on both sides. */
void func_1519F3B8(TrailOwner *owner) {
    TrailSide *sides = owner->trails.sides;

    sides[0].trail = func_1519F1C8(owner, TRAIL_SIDE_6);
    sides[0].spawner = NULL;
    sides[1].trail = func_1519F1C8(owner, TRAIL_SIDE_7);
    sides[1].spawner = NULL;
}

void func_1519F48C(Trail *trail);
void func_151A0928(TrailSpawner *spawner);

/* Lets go of both trails and deletes any pending spawners. */
void func_1519F400(TrailOwner *owner) {
    TrailSide *sides = owner->trails.sides;

    if (sides[0].trail != NULL) {
        func_1519F48C(sides[0].trail);
    }
    if (sides[1].trail != NULL) {
        func_1519F48C(sides[1].trail);
    }
    if (sides[0].spawner != NULL) {
        func_151A0928(sides[0].spawner);
        func_1516972C((struct102 *)sides[0].spawner);
    }
    if (sides[1].spawner != NULL) {
        func_151A0928(sides[1].spawner);
        func_1516972C((struct102 *)sides[1].spawner);
    }
}

/* Detaches the trail from its owner; the points already laid stay and fade out. */
void func_1519F48C(Trail *trail) {
    TrailConfig *config = trail->config;
    TrailSide *sides;

    if (config->owner != NULL) {
        sides = config->owner->trails.sides;
        if (trail->side == TRAIL_SIDE_6) {
            sides[0].trail = NULL;
        }
        if (trail->side == TRAIL_SIDE_7) {
            sides[1].trail = NULL;
        }
        config->owner = NULL;
    }
    trail->unk30 = 0;
    trail->flags &= ~2;
    config->flags |= 1;
}

typedef struct Vec3f_1519F4F0 {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_1519F4F0;

typedef struct Config_1519F4F0 {
    void *owner;
    u8 type;
    u8 pad5[3];
    f32 field8;
    f32 fieldC;
    Vec3f_1519F4F0 position;
    f32 field1C;
    f32 field20;
    f32 field24;
    s16 field28;
    u8 color[3];
    u8 pad2D;
    s16 field2E;
    s16 field30;
    s16 field32;
    s16 field34;
    u8 pad36[2];
    f32 field38;
    f32 field3C;
    u8 field40;
    u8 field41;
    u8 field42;
    u8 field43;
    u8 field44;
    u8 field45;
    s16 field46;
    s16 field48;
    s16 field4A;
    f32 field4C;
} Config_1519F4F0;

typedef struct TrailPoint_1519F4F0 {
    f32 x, z, y, fieldC;
    s16 size, delay, alpha;
    u8 pad16[2];
    s16 life;
    u8 pad1A[2];
    f32 velocityX, velocityZ;
} TrailPoint_1519F4F0;

s32 func_15045800(Vec3f_1519F4F0 *, u16, f32, void *);
extern f32 D_800BE9A4;
extern s32 D_800BE9E4;

s32 func_1519F4F0(u8 *arg0) {
    TrailPoint_1519F4F0 *base;
    TrailPoint_1519F4F0 *point;
    Config_1519F4F0 *config;
    s32 index, expired;

    config = *(Config_1519F4F0 **)(arg0 + 0x98);
    base = *(TrailPoint_1519F4F0 **)(arg0 + 0x94);
    if (*(s8 *)(arg0 + 0x2C) < 2 && (config->type & 1)) return 0;
    index = *(s8 *)(arg0 + 0x2E);
    if (index != *(s8 *)(arg0 + 0x2D)) {
        do {
            index--;
            expired = 0;
            if (index < 0) index = arg0[0x25] - 1;
            point = base + index;
            point->life = (u32)point->life - (u32)D_800BE9E4;
            if (point->life < 0) expired = 1;
            point->alpha = 255;
            if (point->delay > 0) point->delay = (u32)point->delay - (u32)D_800BE9E4;
            else point->size = (u32)point->size - (u32)config->field34 * (u32)D_800BE9E4;
            point->fieldC += config->field3C * D_800BE9A4;
            point->x += point->velocityX * D_800BE9A4;
            point->z += point->velocityZ * D_800BE9A4;
            {
                Vec3f_1519F4F0 probe;
                probe.x = point->x;
                probe.y = point->y + 30.0f;
                probe.z = point->z;
                if (20000.0f < fabsf(probe.x) || 20000.0f < fabsf(probe.z)) expired = 1;
                else if (func_15045800(&probe, 0, point->y - 30.0f, arg0 + 0x60) != 0) {
                    point->y = *(f32 *)(arg0 + 0x60);
                } else expired = 1;
            }
            if (point->size < 0) expired = 1;
            if (expired != 0) {
                if (index != *(s8 *)(arg0 + 0x2D)) {
                    do {
                        *(s8 *)(arg0 + 0x2D) += 1;
                        if (arg0[0x25] == *(s8 *)(arg0 + 0x2D)) *(s8 *)(arg0 + 0x2D) = 0;
                        *(s8 *)(arg0 + 0x2C) -= 1;
                    } while (index != *(s8 *)(arg0 + 0x2D));
                }
                base[*(s8 *)(arg0 + 0x2D)].size = 0;
            }
        } while (index != *(s8 *)(arg0 + 0x2D));
    }
    config->field46 = (u32)config->field46 + (u32)config->field48 * (u32)D_800BE9E4;
    if (*(s8 *)(arg0 + 0x2C) > 0) {
        *(f32 *)(arg0 + 0x54) = base[*(s8 *)(arg0 + 0x2D)].x;
        *(f32 *)(arg0 + 0x58) = base[*(s8 *)(arg0 + 0x2D)].y;
        *(f32 *)(arg0 + 0x5C) = base[*(s8 *)(arg0 + 0x2D)].z;
    } else {
        *(f32 *)(arg0 + 0x54) = 0.0f;
        *(f32 *)(arg0 + 0x58) = 0.0f;
        *(f32 *)(arg0 + 0x5C) = 0.0f;
    }
    return 1;
}

typedef struct {
    f32 x, y, z;
} Vec3_1519F7F0;

typedef struct {
    f32 x, z;
} Vec2_1519F7F0;

struct Big1519F7F0 {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
    u8 pad3C[0xDC];
    f32 unk118;
    u8 pad11C[0xB8];
    s32 unk1D4;
};

struct Sub1519F7F0 {
    u8 pad0[0x18];
    struct Big1519F7F0 *unk18;
    u8 unk1C;
    u8 pad1D[0x3B];
    s32 unk58[4];
};

struct Ref1519F7F0 {
    struct Sub1519F7F0 *unk0;
    u8 unk4;
    f32 unk8;
    f32 unkC;
    Vec3_1519F7F0 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    s16 pad2A[2];
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 pad34[4];
    f32 unk38;
    f32 unk3C;
    u8 pad40[4];
    u8 unk44;
    u8 unk45;
    u8 pad46[6];
    f32 unk4C;
};

struct Elem1519F7F0 {
    f32 x;
    f32 z;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    Vec2_1519F7F0 vel;
};

struct Obj1519F7F0 {
    u8 pad0[0x10];
    Vec3_1519F7F0 unk10;
    u8 pad1C[2];
    u16 unk1E;
    s32 unk20;
    u8 pad24;
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x31];
    f32 unk60;
    u8 pad64[0x30];
    struct Elem1519F7F0 *unk94;
    struct Ref1519F7F0 *unk98;
};

typedef struct {
    struct Sub1519F7F0 *sub;
    s8 type;
} FxData1519F7F0;

s32 func_1519F7F0(struct Obj1519F7F0 *arg0) {
    struct Ref1519F7F0 *ref;
    struct Elem1519F7F0 *elems;
    struct Big1519F7F0 *big;
    s32 padE;
    s32 found;
    f32 stepH;
    s32 i;
    s32 j;
    f32 invDist;
    FxData1519F7F0 data;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 speedStep;
    f32 inv;
    Vec3_1519F7F0 cur;
    f32 speed;
    f32 y;
    f32 dyy;
    s32 *slots;
    struct260 *fx;
    f32 stepX;
    Vec2_1519F7F0 vel;
    f32 stepY;
    f32 stepZ;

    ref = arg0->unk98;
    elems = arg0->unk94;
    big = ref->unk0->unk18;
    if (big->unk0 == 0) {
        return 0;
    }
    if (big->unk1D4 == 0) {
        return 0;
    }
    if (big->unk3B != ref->unk0->unk1C) {
        return 0;
    }
    found = 0;
    if (!(arg0->unk1E & 4)) {
        if (func_1519EF90(ref, (u8)arg0->unk20, &arg0->unk10, elems) != 0) {
            found = 1;
            ref->unk10 = arg0->unk10;
            arg0->unk1E |= 4;
        } else {
            return 1;
        }
    }
    if (!found && func_1519EF90(ref, (u8)arg0->unk20, &arg0->unk10, elems) == 0) {
        func_1519F48C(arg0);
        slots = ref->unk0->unk58;
        data.sub = ref->unk0;
        data.type = arg0->unk20;
        fx = func_151491F4(0x12C, -1, 0xB, 0, 6, 8, 0xFF, 0);
        if (fx != NULL) {
            memcpy((u8 *)fx + 0x28, &data, 8);
        }
        if (arg0->unk20 == 6) {
            slots[1] = (s32)fx;
        } else {
            slots[3] = (s32)fx;
        }
        return 1;
    }
    ref->unk20 = big->unk14;
    ref->unk24 = big->unk1C;
    dx = arg0->unk10.x - ref->unk10.x;
    dy = arg0->unk10.y - ref->unk10.y;
    dz = arg0->unk10.z - ref->unk10.z;
    if (9.999999747e-05f < fabsf(dx) || 9.999999747e-05f < fabsf(dy) || 9.999999747e-05f < fabsf(dz)) {
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
        ref->unkC += dist * ref->unk8;
        invDist = 1.0f / dist;
    }
    if (ref->unkC > 1.0f) {
        inv = 1.0f / ref->unkC;
        speed = ref->unk1C + D_800BE9A4;
        speedStep = speed * inv;
        cur = ref->unk10;
        y = ref->unk38 + ref->unk3C * speed;
        dyy = ref->unk38 - y;
        if (arg0->unk20 == 7) {
            vel.x = -dz * invDist * ref->unk4C;
            vel.z = dx * invDist * ref->unk4C;
        } else {
            vel.x = dz * invDist * ref->unk4C;
            vel.z = -dx * invDist * ref->unk4C;
        }
        i = arg0->unk2E - 1;
        if (i < 0) {
            i = arg0->unk25 - 1;
        }
        stepX = dx * inv;
        stepY = dy * inv;
        stepZ = dz * inv;
        stepH = dyy * inv;
        if (i != arg0->unk2D && arg0->unk2C != 0) {
            j = i - 1;
            if (j < 0) {
                j = arg0->unk25 - 1;
            }
            (elems + j)->vel.x = ((elems + j)->vel.x + vel.x) * 0.5f;
            (elems + j)->vel.z = ((elems + j)->vel.z + vel.z) * 0.5f;
        }
        do {
            s32 ok;
            struct Elem1519F7F0 *e;

            ok = 1;
            e = &elems[arg0->unk2E];
            e->vel = vel;
            e->x = cur.x;
            e->z = cur.z;
            e->unk18 = ref->unk2E;
            e->unkC = y;
            e->unk10 = ref->unk30;
            e->unk12 = ref->unk32;
            e->unk14 = 0xFF;
            e->unk16 = ref->unk28;
            ref->unk28 += ref->unk44 + (u32)func_150ADA20() % (ref->unk45 + 1);
            e->x += e->vel.x * speed;
            e->z += e->vel.z * speed;
            {
            f32 probe[3];

            probe[0] = e->x;
            if (big->unk118 < -9500.0f) {
                probe[1] = big->unk18 + 100.0f;
            } else {
                probe[1] = big->unk118 + 100.0f;
            }
            probe[2] = e->z;
            if (fabsf(probe[0]) > 20000.0f || fabsf(probe[2]) > 20000.0f) {
                ok = 0;
            } else if (func_15045800(probe, 0, probe[1] - 200.0f, &arg0->unk60) != 0) {
                e->unk8 = arg0->unk60;
            } else {
                ok = 0;
            }
            }
            if (ok) {
                arg0->unk2E++;
                if (arg0->unk2E == arg0->unk25) {
                    arg0->unk2E = 0;
                }
                arg0->unk2C++;
                if (arg0->unk2E == arg0->unk2D) {
                    arg0->unk2D++;
                    if (arg0->unk2D == arg0->unk25) {
                        arg0->unk2D = 0;
                    }
                    arg0->unk2C--;
                }
            }
            cur.x += stepX;
            cur.y += stepY;
            cur.z += stepZ;
            y += stepH;
            speed -= speedStep;
            ref->unkC -= 1.0f;
        } while (ref->unkC > 1.0f);
        ref->unk10 = cur;
        ref->unk1C = speed;
    }
    return 1;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_1519FE6C;

typedef struct {
    f32 x;
    f32 z;
    f32 y;
    f32 width;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8 pad18[0xC];
} Elem1519FE6C;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x25];
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 pad2D[0xB];
    f32 unk38;
    u8 pad3C[4];
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 unk43;
    u8 pad44[2];
    s16 unk46;
    u8 pad48[2];
    s16 unk4A;
} Params1519FE6C;

typedef struct {
    u8 pad0[0x10];
    Vec3f_1519FE6C unk10;
    u8 pad1C[2];
    u16 unk1E;
    s32 unk20;
    u8 pad24;
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x55];
    u8 unk84[0x10];
    Elem1519FE6C *unk94;
    Params1519FE6C *unk98;
} Obj1519FE6C;

extern u8 D_80090E0C[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

#define EL(n) ((Elem1519FE6C *)((u8 *)elems + (n) * sizeof(Elem1519FE6C)))
Gfx *func_1519FE6C(Obj1519FE6C *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    Params1519FE6C *params;
    Elem1519FE6C *elems;
    Vec3f_1519FE6C cur;
    Vec3f_1519FE6C prev;
    s32 i;
    s32 j;
    f32 nx;
    s16 cnt1;
    s16 cnt2;
    s16 prevColor;
    s16 curColor;
    s32 prevAlpha;
    s32 curAlpha;
    f32 dx;
    f32 dz;
    f32 prevWidth;
    s32 a;
    s32 b;
    s16 tc[3];
    u8 alpha1;
    u8 alpha2;
    u8 update;
    f32 curWidth;
    f32 invlen;
    f32 nz;
    s32 k;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->unk84, arg2, (arg0->unk25 * 3 << 4) + 0x120, (void **)&vtx, 0);
        if (vtx != NULL) {
            params = arg0->unk98;
            elems = arg0->unk94;
            update = 1;
            if (params->unk4 & 2) {
                alpha1 = 0;
                cnt1 = params->unk40;
                j = arg0->unk2D;
                do {
                    elems[j].unk14 = alpha1;
                    j++;
                    alpha1 += params->unk41;
                    if (j == arg0->unk25) {
                        j = 0;
                    }
                    cnt1--;
                } while (cnt1 != 0 && j != arg0->unk2E);
                do { } while (0);
            }
            if (params->unk4 & 4) {
                alpha2 = 0;
                cnt2 = params->unk42;
                j = arg0->unk2E - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                do {
                    elems[j].unk14 = (elems[j].unk14 * alpha2) >> 8;
                    j--;
                    alpha2 += params->unk43;
                    cnt2--;
                    if (j < 0) {
                        j = arg0->unk25 - 1;
                    }
                } while (cnt2 != 0 && j != arg0->unk2E);
            }
            gfx = func_15142E24(gfx, (s32)D_80090E0C, 0, 0, 0, 0, 0x39, 0, 0, &update, 0x3E);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_15142C10(gfx, params->unk2A, params->unk2B, params->unk2C, 0xFF, &update);
            gfx = func_15142CF0(gfx, 0, 0, 0xFF, 0xFF, 0xFF, 0, &update);
            gfx = func_1513F4E4(gfx, 7, &update);
            gfx = func_15142FBC(gfx, G_TP_PERSP | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &update);
            if (arg0->unk1E & 2) {
                i = arg0->unk2E - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                prev = arg0->unk10;
                prevAlpha = (EL(i)->unk14 * EL(i)->unk10) >> 8;
                prevColor = EL(i)->unk16;
                prevWidth = params->unk38;
            } else {
                k = arg0->unk2E - 1;
                if (k < 0) {
                    k = arg0->unk25 - 1;
                }
                i = k - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                prev.x = EL(k)->x;
                prev.y = EL(k)->y;
                prev.z = EL(k)->z;
                prevAlpha = (EL(k)->unk14 * EL(k)->unk10) >> 8;
                prevWidth = EL(k)->width;
                prevColor = EL(k)->unk16;
            }
            cur.x = EL(i)->x;
            cur.y = EL(i)->y;
            cur.z = EL(i)->z;
            b = 2;
            curColor = EL(i)->unk16;
            curWidth = EL(i)->width;
            curAlpha = (EL(i)->unk14 * EL(i)->unk10) >> 8;
            dx = cur.x - prev.x;
            dz = cur.z - prev.z;
            if (0.0001f < fabsf(dx) || 0.0001f < fabsf(dz)) {
                invlen = 1.0f / sqrtf(dx * dx + dz * dz);
            } else {
                invlen = 1.0f;
            }
            tc[0] = params->unk46;
            tc[2] = params->unk46 + params->unk4A;
            if (tc[2] < tc[0]) {
                tc[0] = tc[0] % 1024;
                params->unk46 = tc[0];
                tc[2] = params->unk46 + params->unk4A;
            }
            tc[1] = (tc[0] + tc[2]) >> 1;
            if (arg0->unk20 == 6) {
                a = 0;
            } else if (arg0->unk20 == 7) {
                a = 2;
                b = 0;
            } else {
                b = 0;
                a = 2;
            }
            nx = -dz * invlen * prevWidth;
            nz = dx * invlen * prevWidth;
            vtx->v.ob[0] = prev.x + nx;
            vtx->v.ob[1] = prev.y;
            vtx->v.ob[2] = prev.z + nz;
            vtx->v.tc[0] = prevColor;
            vtx->v.tc[1] = tc[a];
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = prev.x;
            vtx->v.ob[1] = prev.y;
            vtx->v.ob[2] = prev.z;
            vtx->v.tc[0] = prevColor;
            vtx->v.tc[1] = tc[1];
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = prev.x - nx;
            vtx->v.ob[1] = prev.y;
            vtx->v.ob[2] = prev.z - nz;
            vtx->v.tc[0] = prevColor;
            vtx->v.tc[1] = tc[b];
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            do {
                nx = -dz * invlen * curWidth;
                nz = dx * invlen * curWidth;
                vtx->v.ob[0] = cur.x + nx;
                vtx->v.ob[1] = cur.y;
                vtx->v.ob[2] = cur.z + nz;
                vtx->v.tc[0] = curColor;
                vtx->v.tc[1] = tc[a];
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = cur.x;
                vtx->v.ob[1] = cur.y;
                vtx->v.ob[2] = cur.z;
                vtx->v.tc[0] = curColor;
                vtx->v.tc[1] = tc[1];
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = curAlpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = cur.x - nx;
                vtx->v.ob[1] = cur.y;
                vtx->v.ob[2] = cur.z - nz;
                vtx->v.tc[0] = curColor;
                vtx->v.tc[1] = tc[b];
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 6, 6, 0);
                gSP1Triangle(gfx++, 0, 3, 4, 0);
                gSP1Triangle(gfx++, 0, 4, 1, 0);
                gSP1Triangle(gfx++, 1, 4, 5, 0);
                gSP1Triangle(gfx++, 1, 5, 2, 0);
                if (prevColor != curColor) {
                }
                k = i--;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                if (k != arg0->unk2D) {
                    prev = cur;
                    cur.x = EL(i)->x;
                    cur.y = EL(i)->y;
                    cur.z = EL(i)->z;
                    dx = cur.x - prev.x;
                    dz = cur.z - prev.z;
                    prevColor = curColor;
                    curColor = EL(i)->unk16;
                    curAlpha = (EL(i)->unk14 * EL(i)->unk10) >> 8;
                    if (dx != 0.0f || dz != 0.0f) {
                        invlen = 1.0f / sqrtf(dx * dx + dz * dz);
                    } else {
                        invlen = 1.0f;
                    }
                    curWidth = EL(i)->width;
                }
            } while (k != arg0->unk2D);
        }
    }
    return gfx;
}


/* Spawner update: once the owner's actor is back, creates the trail for its side and removes itself. */
void func_151A084C(TrailSpawner *spawner) {
    Trail *trail;
    TrailSet *trails;
    struct127 *actor;
    u8 done;
    TrailLink *link;

    done = 0;
    link = &spawner->link;
    actor = link->owner->actor;
    trails = &link->owner->trails;
    if (actor->interaction_state == 0) {
        done = 1;
    }
    if (actor->unique_id != link->owner->actorId) {
        done = 1;
    }
    if (!done && actor->unk1D4 != NULL) {
        done = 1;
        // matching: the unused extra arguments pin link and spawner to a2/a3
        trail = func_1519F1C8(link->owner, link->side, link, spawner);
        if (link->side == TRAIL_SIDE_6) {
            trails->sides[0].trail = trail;
        } else {
            trails->sides[1].trail = trail;
        }
    }
    if (done) {
        func_151A0928(spawner);
        spawner->unkE = -1;
        spawner->flags |= 1;
    }
}

/* Clears the owner's reference to this spawner. */
void func_151A0928(TrailSpawner *spawner) {
    TrailSide *sides = spawner->link.owner->trails.sides;

    if (spawner->link.side == TRAIL_SIDE_6) {
        sides[0].spawner = NULL;
    } else {
        sides[1].spawner = NULL;
    }
}

typedef struct TrailEvent {
    /* 0x0 */ struct127 *actor;
    /* 0x4 */ u8 actorId;
} TrailEvent;

/* Event handler for a trail: event 10 about its owner's actor detaches the trail. */
void func_151A0950(Trail *trail, TrailEvent *event, u8 type) {
    TrailOwner *owner;
    TrailConfig *config = trail->config;
    struct127 *actor;

    if (type == 10) {
        owner = config->owner;
        if (owner != NULL) {
            actor = owner->actor;
            if (event->actor == actor || event->actorId == owner->actorId) {
                func_1519F48C(trail);
            }
        }
    }
}

/* Event handler for a spawner: event 0 about its owner's actor deletes the spawner. */
void func_151A09B4(TrailSpawner *spawner, TrailEvent *event, u8 type) {
    struct127 *actor = spawner->link.owner->actor;

    if (type == 0) {
        if (actor == event->actor || event->actorId == actor->unique_id) {
            func_151A0928(spawner);
            func_1516972C((struct102 *)spawner);
        }
    }
}
