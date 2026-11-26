#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-impact-type effect selection: indices into the tables below, -1 for none. */
typedef struct {
    /* 0x00 */ s8 first;      /* D_800A9040 index */
    /* 0x01 */ s8 firstStyle; /* D_800A9068 index (read by func_151AC078) */
    /* 0x02 */ s8 second;     /* D_800A9158 index */
    /* 0x03 */ s8 unk3;
    /* 0x04 */ s8 impact;     /* D_800A925C index */
} FxImpactSet; // size 0x5

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ s16 base;
    /* 0x06 */ u16 range;
} FxSpawnRange; // size 0x8

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x08 */ f32 unk8;
} FxImpactEntry; // size 0xC

/* Passed through func_1514C678 to the spawn callbacks (e.g. func_151AC078). */
struct FxSpawnPayload {
    /* 0x00 */ CollisionHit *hit;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 kind;
};

extern FxImpactSet D_800A9020[];
extern FxSpawnRange D_800A9040[];
extern FxSpawnRange D_800A9158[];
extern FxImpactEntry D_800A925C[];


/* Spawns the effects listed for impact type `type` at `pos` on surface `hit`. */
void func_151ABE40(Vec3f *pos, CollisionHit *hit, s32 type, u8 arg3, s32 arg4) {
    FxImpactSet *set;
    u8 kind;

    kind = func_151D8E20();
    if (hit == NULL) {
        return;
    }
    set = &D_800A9020[type];
    if (set->first != -1) {
        FxSpawnPayload payload;

        payload.hit = hit;
        payload.unk4 = arg4;
        payload.kind = kind;
        {
            FxSpawnRange *spawn = &D_800A9040[set->first];

            func_1514C678(pos->x, pos->y, pos->z, spawn->unk0, 0, 255,
                          func_150ADA20() % (spawn->range + 1) + spawn->base, 11, type, 0.0f,
                          &payload, arg3);
        }
    }
    if (set->second != -1) {
        FxSpawnPayload payload;

        payload.hit = hit;
        payload.unk4 = arg4;
        payload.kind = kind;
        {
            FxSpawnRange *spawn = &D_800A9158[set->second];

            func_1514C678(pos->x, pos->y, pos->z, spawn->unk0, 0, 255,
                          func_150ADA20() % (spawn->range + 1) + spawn->base, 12, type, 0.0f,
                          &payload, arg3);
        }
    }
    if (set->impact != -1 && (hit->flags & 1)) {
        FxImpactEntry *impact = &D_800A925C[set->impact];

        func_151DBE80(kind, impact->unk0, impact->unk8, impact->unk4, pos, &hit->tri, 150, 0, arg3, arg4);
    }
}

/* Random ranges for one spray particle: each value is base + rand() % (range + 1) or
 * base + randf() * range. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 pitchBase;
    /* 0x04 */ u16 pitchRange;
    /* 0x08 */ f32 speedBase;
    /* 0x0C */ f32 speedRange;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 chance;
} FxSprayParams; // size 0x30

extern FxSprayParams D_800A9068[];

/* Spawn callback: launches one spray particle at (x, y, z) heading along `yaw`, with random
 * pitch and speed from the impact type's spray parameters. */
s32 func_151AC078(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9,
                  s32 argA, s32 type, s32 argC, FxSpawnPayload *payload, u8 argE) {
    FxSprayParams *params;
    EffectTemplate18 spawn;
    TrailMotion part;
    f32 roll;
    TrailRender style;
    f32 sinYaw;
    f32 cosYaw;
    f32 speed;
    f32 horizontal;
    f32 vertical;
    s16 pitch;
    f32 sinPitch;
    f32 cosPitch;
    u32 random;
    s32 firstFlag;
    s32 secondFlag;

    {
        s32 colorFlag;

        params = &D_800A9068[D_800A9020[type].firstStyle];
        sinYaw = func_151423D8(yaw);
        cosYaw = func_151423D8(yaw - 0x40);
        style.unk0 = 0;
        style.setGeometryMode = 1;
        style.clearGeometryMode = 0x160600;
        style.renderMode = 1;
        style.unk10 = 16;
        style.unk14 = 128;
        style.unk18 = 32;
        style.primMode = 0;
        style.envMode = 9;
        spawn.kind = 1;
        spawn.flags = 1;
        spawn.pos.y = y;
        part.unk14 = y;
        spawn.pos.x = x;
        spawn.pos.z = z;
        part.texture = D_800AB3F4[payload->kind];
        part.shade = 255;
        colorFlag = D_800AB404[payload->kind] ? 0x20 : 0;
        part.flags = colorFlag | 8;
    }
    roll = func_150ADA68();
    if (roll < params->chance) {
        part.flags |= 3;
        firstFlag = 6;
        secondFlag = 2;
    } else {
        firstFlag = 0;
        secondFlag = 0;
        part.flags &= ~3;
    }
    random = func_150ADA20();
    part.alpha = random % (params->unk1 + 1) + params->unk0;
    random = func_150ADA20();
    pitch = random % (params->pitchRange + 1) + params->pitchBase;
    sinPitch = func_151423D8(pitch);
    cosPitch = func_151423D8(pitch - 0x40);
    speed = func_150ADA68() * params->speedRange + params->speedBase;
    horizontal = speed * sinPitch;
    part.velocity.x = horizontal * cosYaw;
    vertical = -speed * cosPitch;
    part.velocity.y = vertical;
    part.velocity.z = horizontal * sinYaw;
    random = func_150ADA20();
    spawn.itemCount = random % (params->unk14 + 1) + params->unk10;
    random = func_150ADA20();
    spawn.life = random % (params->unk1A + 1) + params->unk18;
    part.halfWidth = func_150ADA68() * params->unk20 + params->unk1C;
    part.gravity = func_150ADA68() * params->unk28 + params->unk24;
    func_15147DA0(&spawn, &part, 0, 1, 8, 0, firstFlag, secondFlag, 0, 0, 0, &style, (s32)payload->hit, argE, payload->unk4);
    return 1;
}

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
} FxEmitterPoint; // size 0x14

typedef struct {
    /* 0x00 */ f32 scale;
    /* 0x04 */ u8 pad4[0x17];
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ u8 unk20;
} FxEmitterData;

/* Effect state passed to the callbacks in the D_8008A3F8 / D_8008A430 handler tables. */
struct FxEmitter {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ FxEmitterPoint *points;
    /* 0x98 */ FxEmitterData *data;
};

/* Lowers the effect's alpha to unk1C * 8 (capped at 255) once that is below it. */
s32 func_151AC3CC(FxEmitter *emitter) {
    FxEmitterData *data = emitter->data;
    s32 alpha = emitter->head.tmpl.life * 8;

    if (alpha >= 256) {
        alpha = 255;
    }
    if (alpha < data->alpha) {
        data->alpha = alpha;
    }
    return 1;
}

/* Spawns a burst at the emitter's current point (at the given height): randomly one of two
 * effects, scaled from the emitter's data. */
s32 func_151AC408(FxEmitter *emitter, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface) {
    FxEmitterData *data = emitter->data;
    FxEmitterPoint *points = emitter->points;
    Vec3f pos;
    u8 kind;
    s32 life;

    pos.x = points[emitter->head.start].x;
    pos.y = height;
    pos.z = points[emitter->head.start].z;
    kind = func_151D8E20();
    if (func_150ADA20() & 1) {
        life = func_150ADA20() % 41 + 80;
        func_151D9B8C(kind, data->scale * 6.0f, data->alpha, surface, &pos, life, 1, 1, 0, emitter->head.header.owner,
                      emitter->head.header.kind);
    } else {
        func_151DAB58(kind, data->scale * 1.5f, data->alpha, &pos, 1, emitter->head.header.owner, emitter->head.header.kind);
    }
    data->unk20 = 4;
    return 1;
}
