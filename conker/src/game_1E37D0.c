#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Spawn header that func_15147A80 copies to EffectObject+0x10. */
typedef struct EffectSpawn {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 lifetime;
    /* 0x0E */ u16 flags;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 capacity; /* number of points in the ring buffer */
    /* 0x16 */ u8 pad16[2];
} EffectSpawn;

/* Effect object created by func_15147A80: a ring buffer of points plus a kind-specific state block. */
typedef struct EffectObject {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ s16 lifetime;
    /* 0x1E */ u16 flags;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 capacity;
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail; /* oldest point */
    /* 0x2E */ s8 head; /* next point to write */
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ s8 unk30;
    /* 0x31 */ u8 pad31[0x23];
    /* 0x54 */ Vec3f tailPos;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 buffers[0x10];
    /* 0x94 */ void *points;
    /* 0x98 */ void *state;
} EffectObject;

/* Object a trail follows. */
typedef struct TrailTarget {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ Vec3f pos;
    /* 0x20 */ u8 pad20[0x1B];
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ f32 unk3C;
} TrailTarget;

/* A TrailTarget and its unk3B byte: argument of a ring effect (selector 0) and of message 0. */
typedef struct TrailTargetRef {
    /* 0x00 */ TrailTarget *target;
    /* 0x04 */ u8 targetUnk3B;
} TrailTargetRef;

/* State block of a trail effect (func_151B6320). */
typedef struct TrailState {
    /* 0x00 */ TrailTarget *target;
    /* 0x04 */ u8 targetUnk3B; /* target->unk3B when the trail was spawned */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ Vec3f lastPos;
    /* 0x14 */ f32 pending; /* points still to emit */
    /* 0x18 */ f32 elapsed; /* time since the last point was emitted */
    /* 0x1C */ f32 targetTexS; /* texture s at the target's position */
    /* 0x20 */ f32 texS; /* texture s at lastPos */
    /* 0x24 */ f32 distance; /* distance travelled at lastPos */
    /* 0x28 */ s32 unk28;
} TrailState;

/* One point of a trail effect. */
typedef struct TrailPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 life;
    /* 0x10 */ u8 alpha;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 texS;
    /* 0x18 */ f32 distance; /* distance travelled when emitted */
} TrailPoint;

extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern EffectObject *func_15147A80(EffectSpawn *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns a trail effect that follows target. */
void func_151B6320(TrailTarget *target, u8 arg1, s32 arg2) {
    s32 top_dummy; // matching: stack pad
    EffectSpawn spawn;
    s32 pad_dummy; // matching: stack pad
    TrailState state;
    EffectObject *obj;

    spawn.capacity = 10;
    spawn.pos.x = target->pos.x;
    spawn.pos.y = target->pos.y;
    spawn.pos.z = target->pos.z;
    spawn.lifetime = 300;
    spawn.flags = 6;
    state.target = target;
    state.targetUnk3B = target->unk3B;
    state.lastPos = spawn.pos;
    state.pending = 0.0f;
    state.elapsed = 0.0f;
    state.texS = -16384.0f;
    state.targetTexS = -16384.0f;
    state.distance = 0.0f;
    spawn.unk10 = 13;

    obj = func_15147A80(&spawn, 0x30, sizeof(TrailPoint), 11, 11, 11, 0, 0, 0, arg1, arg2);
    if (obj != NULL) {
        memcpy(obj->state, &state, sizeof(state));
    }
}

/* Ages a trail's points, drops expired ones and sets the alpha of the rest. Returns 0 once the trail has finished. */
s32 func_151B6420(EffectObject *obj) {
    TrailPoint *points;
    s32 i;
    f32 base;
    s32 alpha;
    f32 d;

    points = obj->points;
    if (obj->count < 2 && (obj->flags & 8)) {
        return 0;
    }
    i = obj->head;
    while (i != obj->tail) {
        i--;
        if (i < 0) {
            i = obj->capacity - 1;
        }
        (i + points)->life -= D_800BE9A4; // matching: IDO orders the addu operands as written
        if ((i + points)->life < 0.0f) {
            while (i != obj->tail) {
                obj->tail++;
                if (obj->tail == obj->capacity) {
                    obj->tail = 0;
                }
                obj->count--;
            }
        }
    }
    if (obj->count > 0) {
        i = obj->tail;
        base = points[obj->tail].distance;
        do {
            d = (i + points)->distance - base; // matching: IDO orders the addu operands as written
            alpha = d * 0.4940000176f;
            if (alpha >= 156) {
                (i + points)->alpha = 155;
            } else {
                (i + points)->alpha = alpha;
            }
            i++;
            if (i >= obj->capacity) {
                i = 0;
            }
        } while (i != obj->head);
    }
    if (obj->count > 0) {
        obj->tailPos = points[obj->tail].pos;
    } else {
        obj->tailPos.x = 0.0f;
        obj->tailPos.y = 0.0f;
        obj->tailPos.z = 0.0f;
    }
    return 1;
}

f32 func_15143E64(Vec3f *);

/* Follows the trail's target and emits points along the path it moved. Stops the trail once the target is gone. */
s32 func_151B65D4(EffectObject *obj) {
    TrailState *state;
    TrailPoint *points;
    TrailTarget *target;
    Vec3f delta;
    s32 pad0; // matching: stack pad
    s32 pad1; // matching: stack pad
    f32 dist;

    state = obj->state;
    points = obj->points;
    target = state->target;
    if (target->unk0 == 0 || target->unk3B != state->targetUnk3B || target->unk3C < 15.0f) {
        obj->unk30 = 0;
        obj->flags |= 8;
        return 1;
    }
    obj->pos.x = target->pos.x;
    obj->pos.y = target->pos.y;
    obj->pos.z = target->pos.z;
    delta.x = obj->pos.x - state->lastPos.x;
    delta.y = obj->pos.y - state->lastPos.y;
    delta.z = obj->pos.z - state->lastPos.z;
    dist = func_15143E64(&delta);
    state->pending += dist * 0.003721999936f * D_800BE9A4;
    state->targetTexS += dist * 4.409547806f;
    if (state->pending > 1.0f) {
        TrailPoint *point;
        Vec3f cur;
        f32 inv;
        f32 elapsed;
        f32 elapsedStep;
        f32 texS;
        f32 distance;
        f32 stepX;
        f32 stepY;
        f32 stepZ;
        f32 texRange;
        f32 texStep;

        inv = 1.0f / state->pending;
        elapsed = state->elapsed + D_800BE9A4;
        cur = state->lastPos;
        texS = state->texS;
        distance = state->distance;
        elapsedStep = -(elapsed * inv);
        stepX = delta.x * inv;
        stepY = delta.y * inv;
        stepZ = delta.z * inv;
        texRange = state->targetTexS - state->texS;
        texStep = texRange * inv;
        do {
            point = &points[obj->head];
            point->pos = cur;
            point->alpha = 155;
            point->texS = texS;
            point->life = 13.0f - elapsed;
            elapsed += elapsedStep;
            while (point->texS > 16384.0f) {
                point->texS -= 32768.0f;
            }
            point->distance = distance;
            obj->head++;
            if (obj->head == obj->capacity) {
                obj->head = 0;
            }
            obj->count++;
            if (obj->head == obj->tail) {
                obj->tail++;
                if (obj->tail == obj->capacity) {
                    obj->tail = 0;
                }
                obj->count--;
            }
            cur.x += stepX;
            cur.y += stepY;
            cur.z += stepZ;
            texS += texStep;
            distance += dist * inv;
            state->pending -= 1.0f;
        } while (state->pending > 1.0f);
        state->lastPos = cur;
        state->texS = texS;
        state->distance = distance;
        state->elapsed = elapsed;
    }
    return 1;
}

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern Vec3f *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090CD4[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

Gfx *func_151B6928(EffectObject *obj, Gfx *gfx, s16 view) {
    Vtx *vtx;
    TrailPoint *points;
    Vec3f *camera;
    u8 flag;
    Vec3f next;
    Vec3f cur;
    f32 toCamX;
    f32 toCamY;
    f32 toCamZ;
    f32 segX;
    f32 segY;
    f32 segZ;
    f32 sideX;
    f32 sideY;
    f32 sideZ;
    s32 index;
    s32 prev;
    f32 lengthSq;
    u8 curAlpha;
    f32 nextTexS;
    f32 curTexS;
    u8 nextAlpha;
    f32 scale;

    if (obj->count >= 2) {
        func_151D5D60(obj->buffers, view, (obj->capacity + 5) * 2 * sizeof(Vtx), (void **)&vtx, 0);
        if (vtx != NULL) {
            points = obj->points;
            camera = func_15144B34(view);
            flag = 1;
            gfx = func_15142E24(gfx, (s32)D_80090CD4, 0, 0, 0, 0, 0x1F, 0, 0, &flag, 3);
            gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &flag);
            gfx = func_1513F4E4(gfx, 0x4E, &flag);
            gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
            gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &flag);
            if (obj->flags & 2) {
                index = obj->head - 1;
                if (index < 0) {
                    index = obj->capacity - 1;
                }
                cur = obj->pos;
                curAlpha = 0x80;
                curTexS = 0.0f;
            } else {
                prev = obj->head - 1;
                if (prev < 0) {
                    prev = obj->capacity - 1;
                }
                index = prev - 1;
                if (index < 0) {
                    index = obj->capacity - 1;
                }
                cur = points[prev].pos;
                curAlpha = points[prev].alpha;
                curTexS = points[prev].texS;
            }
            next = points[index].pos;
            nextAlpha = points[index].alpha;
            nextTexS = points[index].texS;

            segX = cur.x - next.x;
            segY = cur.y - next.y;
            segZ = cur.z - next.z;
            toCamX = cur.x - camera->x;
            toCamY = cur.y - camera->y;
            toCamZ = cur.z - camera->z;
            sideX = (segY * toCamZ) - (toCamY * segZ);
            sideY = (segZ * toCamX) - (toCamZ * segX);
            sideZ = (segX * toCamY) - (toCamX * segY);
            lengthSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
            if (lengthSq == 0.0f) {
                sideZ = sideY = sideX = 0.0f;
            } else {
                scale = 10.0f / sqrtf(lengthSq);
                sideX = sideX * scale;
                sideY = sideY * scale;
                sideZ = sideZ * scale;
            }
            vtx->v.ob[0] = cur.x + sideX;
            vtx->v.ob[1] = cur.y + sideY;
            vtx->v.ob[2] = cur.z + sideZ;
            vtx->v.tc[0] = curTexS;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - sideX;
            vtx->v.ob[1] = cur.y - sideY;
            vtx->v.ob[2] = cur.z - sideZ;
            vtx->v.tc[0] = curTexS;
            vtx->v.tc[1] = 0x7FF;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                prev = index;
                segX = cur.x - next.x;
                segY = cur.y - next.y;
                segZ = cur.z - next.z;
                toCamX = next.x - camera->x;
                toCamY = next.y - camera->y;
                toCamZ = next.z - camera->z;
                sideX = (segY * toCamZ) - (toCamY * segZ);
                sideY = (segZ * toCamX) - (toCamZ * segX);
                sideZ = (segX * toCamY) - (toCamX * segY);
                lengthSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
                if (lengthSq == 0.0f) {
                    sideZ = sideY = sideX = 0.0f;
                } else {
                    scale = 10.0f / sqrtf(lengthSq);
                    sideX = sideX * scale;
                    sideY = sideY * scale;
                    sideZ = sideZ * scale;
                }
                vtx->v.ob[0] = next.x + sideX;
                vtx->v.ob[1] = next.y + sideY;
                vtx->v.ob[2] = next.z + sideZ;
                vtx->v.tc[0] = nextTexS;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = nextAlpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = next.x - sideX;
                vtx->v.ob[1] = next.y - sideY;
                vtx->v.ob[2] = next.z - sideZ;
                vtx->v.tc[0] = nextTexS;
                vtx->v.tc[1] = 0x7FF;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = nextAlpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                index--;
                if (index < 0) {
                    index = obj->capacity - 1;
                }
                if (prev != obj->tail) {
                    cur = next;
                    next = points[index].pos;
                    nextAlpha = points[index].alpha;
                    nextTexS = points[index].texS;
                }
            } while (prev != obj->tail);
        }
    }
    return gfx;
}


/* Message 0x2D: objA and objB swap identities. */
typedef struct SwapMessage {
    /* 0x00 */ TrailTarget *objA;
    /* 0x04 */ TrailTarget *objB;
    /* 0x08 */ u8 objAUnk3B;
    /* 0x09 */ u8 objBUnk3B;
} SwapMessage;

/* Message handler of a trail: message 0 (msg is a TrailTargetRef) stops the trail when it names its target;
 * message 0x2D (a SwapMessage) moves the trail to the other object of the swap. */
void func_151B70B4(EffectObject *obj, void *msg, u8 msgType) {
    TrailState *state;
    TrailTarget *current;
    TrailTarget *objA;

    state = obj->state;
    current = state->target;
    if (msgType == 0) {
        if (current == ((TrailTargetRef *)msg)->target) {
            u16 *flags = &obj->flags; // matching: keeps the store out of the jr delay slot

            obj->unk30 = 0;
            *flags |= 8;
        }
    } else if (msgType == 0x2D) {
        SwapMessage *swap = msg;

        objA = swap->objA;
        if (objA == state->target) {
            state->target = swap->objB;
            state->targetUnk3B = swap->objBUnk3B;
            return;
        }
        if (state->target == swap->objB) {
            state->target = objA;
            state->targetUnk3B = swap->objAUnk3B;
        }
    }
}

/* Particle description passed to func_15130280. */
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
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f unk3C;
    /* 0x48 */ Vec3f unk48;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6C */ f32 unk6C;
} ParticleSpawn;

extern void *func_15130280(ParticleSpawn *, u8, s32, s32, u8, s32);

/* Spawns a particle at target's position. */
void func_151B7144(TrailTarget *target, u8 arg1, s32 arg2) {
    ParticleSpawn spawn;
    s32 flag80;
    s32 flag40;

    spawn.unk1D = 0x29;
    spawn.unk8 = 0xE03;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk1E = 18;
    spawn.unk20 = 14;
    if (func_150ADA20() & 1) {
        flag40 = 0x40;
    } else {
        flag40 = 0;
    }
    if (func_150ADA20() & 1) {
        flag80 = 0x80;
    } else {
        flag80 = 0;
    }
    spawn.flags = flag80 | 1 | flag40 | 0x4000 | 0x8000 | 0x200 | 0x400 | 0x800 | 0x10000;
    spawn.unk60 = 3;
    spawn.unk61 = 3;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk22 = 40;
    spawn.unk14 = 0xDD;
    spawn.unk15 = 0xD3;
    spawn.unk16 = 0xCD;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0x57;
    spawn.unk19 = 0x55;
    spawn.unk1A = 0x5A;
    spawn.unk1C = 0xFF;
    spawn.unk54 = 0.0f;
    spawn.unk24 = 1.05674696f;
    spawn.pos.x = target->pos.x;
    spawn.pos.y = target->pos.y;
    spawn.pos.z = target->pos.z;
    spawn.unk3C.x = 0.0f;
    spawn.unk3C.y = 0.0f;
    spawn.unk3C.z = 0.0f;
    spawn.unk1B = func_150ADA20() % 56U + 200;
    spawn.unkA = func_150ADA20() % 10U + 30;
    spawn.unk28 = spawn.unk2C = func_150ADA68() * 50.0f + 89.0f;
    spawn.unk48.x = 0.0f;
    spawn.unk48.y = 0.0f;
    spawn.unk48.z = 0.0f;
    func_15130280(&spawn, 1, 0, 0, arg1, arg2);
}

/* Particle a ring effect drags along (see func_151B77F4). */
typedef struct LinkedParticle {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ Vec3f pos;
} LinkedParticle;

/* State block of a ring effect (func_151B7328). A copy of its arguments follows at +0x10. */
typedef struct RingState {
    /* 0x00 */ LinkedParticle *linked;
    /* 0x04 */ void *args;
    /* 0x08 */ u8 selector; /* index into D_8008FB90 and D_8008FB98 */
    /* 0x09 */ u8 pad9[3];
} RingState;

/* One point of a ring effect. */
typedef struct RingPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 distance; /* distance to the previous point */
    /* 0x10 */ u8 alpha;
    /* 0x11 */ u8 pad11[3];
} RingPoint;

/* Returns 0 when the effect's source is gone, else stores the source position in pos. */
extern s32 (*D_8008FB90[])(EffectObject *, Vec3f *);

/* Spawns a ring effect that follows the position given by D_8008FB90[selector], with a copy of args
 * (argsSize bytes) and a linked particle. Returns NULL if nothing was spawned. */
EffectObject *func_151B7328(void *args, u8 selector, s32 argsSize, u8 arg3, s32 arg4) {
    EffectObject *obj;
    EffectSpawn spawn;
    s32 pad; // matching: stack pad
    RingState init;
    RingState *state;
    ParticleSpawn particle;

    init.linked = NULL;
    init.args = NULL;
    init.selector = selector;
    spawn.capacity = 20;
    spawn.pos = *(Vec3f *)&D_800A5480;
    spawn.lifetime = 300;
    spawn.flags = 20;
    spawn.unk10 = 16;
    spawn.unk14 = 2;
    obj = func_15147A80(&spawn, argsSize + 0x10, sizeof(RingPoint), 0, 14, 14, 0, 0, 0, arg3, arg4);
    if (obj != NULL) {
        state = obj->state;
        memcpy(state, &init, sizeof(init));
        // matching: state->args = (u8 *)state + 0x10, stored and reloaded as an s32 (IDO allocates differently otherwise)
        ((s32 *)state)[1] = (s32)((s32 *)state + 4);
        memcpy((void *)((s32 *)state)[1], args, argsSize);
        if (D_8008FB90[selector](obj, &obj->pos) == 0) {
            func_1516972C((struct102 *)obj);
            return NULL;
        }
        {
        const s32 unk1DChoices[4] = { 0x60, 0x61, 0x62, 0x63 };
        s32 flag80;
        s32 flag40;
        particle.unk1D = unk1DChoices[func_150ADA20() & 3];
        particle.unk8 = 0x1303;
        particle.unk0 = 0x200005;
        particle.unk4 = 0;
        particle.unkA = 300;
        particle.unkC = 0;
        particle.unk10 = 0;
        particle.unk14 = 0xFF;
        particle.unk15 = 0xFF;
        particle.unk16 = 0xFF;
        particle.unk17 = 0xFF;
        particle.unk18 = 0xFF;
        particle.unk19 = 0xFF;
        particle.unk1A = 0xFF;
        particle.unk1B = 0xFF;
        particle.unk1C = 0xFF;
        particle.unk28 = particle.unk2C = func_150ADA68() * 800.0f + 1700.0f;
        particle.pos = spawn.pos;
        particle.unk3C = *(Vec3f *)&D_800A5480;
        particle.unk48 = *(Vec3f *)&D_800A5480;
        particle.unk1E = 1;
        particle.unk20 = 0xFF;
        particle.unk22 = 1;
        particle.unk54 = 0.0f;
        particle.unk24 = 1.0f;
        if (func_150ADA20() & 1) {
            flag40 = 0x40;
        } else {
            flag40 = 0;
        }
        if (func_150ADA20() & 1) {
            flag80 = 0x80;
        } else {
            flag80 = 0;
        }
        particle.flags = flag80 | flag40 | 0x4000 | 0x8000 | 0x40000;
        particle.unk60 = 6;
        particle.unk61 = 5;
        particle.unk62 = -1;
        particle.unk63 = -1;
        particle.unk64 = -1;
        particle.unk65 = 0;
        particle.unk5C = 0;
        particle.unk66 = 0xFF;
        state->linked = func_15130280(&particle, 1, 0, 0, arg3, arg4);
        }
    }
    return obj;
}

/* Object a ring effect orbits (func_151B76CC). */
typedef struct RotatedTarget {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 scaleXZ;
    /* 0x1C */ f32 scaleY;
    /* 0x20 */ Vec3f rot;
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ Vec3f pos;
} RotatedTarget;

/* D_8008FB90 callback: reads the followed TrailTarget's position; fails once the target is gone. */
s32 func_151B7678(EffectObject *obj, Vec3f *pos) {
    RingState *state;
    TrailTargetRef *ref;
    TrailTarget *target;

    state = obj->state;
    ref = state->args;
    target = ref->target;
    if (target->unk0 == 0 || target->unk3B != ref->targetUnk3B) {
        return 0;
    }

    pos->x = target->pos.x;
    pos->y = target->pos.y;
    pos->z = target->pos.z;
    return 1;
}

/* D_8008FB90 callback: places pos 250 units along the target's local -z axis. */
s32 func_151B76CC(EffectObject *obj, Vec3f *pos) {
    RingState *state;
    RotatedTarget **ref;
    RotatedTarget *target;
    f32 mtx[4][4];

    state = obj->state;
    ref = state->args;
    target = *ref;
    func_150A8050(mtx, target->rot.x, target->rot.y, target->rot.z);
    mtx[3][0] = target->pos.x;
    mtx[3][1] = target->pos.y;
    mtx[3][2] = target->pos.z;
    mtx[0][0] *= target->scaleXZ;
    mtx[0][1] *= target->scaleXZ;
    mtx[0][2] *= target->scaleXZ;
    mtx[1][0] *= target->scaleY;
    mtx[1][1] *= target->scaleY;
    mtx[1][2] *= target->scaleY;
    mtx[2][0] *= target->scaleXZ;
    mtx[2][1] *= target->scaleXZ;
    mtx[2][2] *= target->scaleXZ;
    func_150A7960(mtx[0], 0.0f, 0.0f, -250.0f, &pos->x, &pos->y, &pos->z);
    return 1;
}

s32 func_151B77F4(EffectObject *arg0) {
    RingState *state;
    RingPoint *entries;
    Vec3f previous;
    Vec3f delta;
    f32 distance;

    state = arg0->state;
    entries = arg0->points;
    if (D_800BE9E4 > 0) {
        previous = arg0->pos;
        if (D_8008FB90[state->selector](arg0, &arg0->pos) == 0) {
            return 0;
        }
        if (state->linked != NULL) {
            state->linked->pos = arg0->pos;
        }
        delta.x = arg0->pos.x - previous.x;
        delta.y = arg0->pos.y - previous.y;
        delta.z = arg0->pos.z - previous.z;
        distance = func_15143E64(&delta);
        {
            RingPoint *entry = &entries[arg0->head];

            entry->pos = arg0->pos;
            entry->distance = distance;
            entry->alpha = 0xFF;
        }
        arg0->head++;
        if (arg0->head == arg0->capacity) {
            arg0->head = 0;
        }
        arg0->count++;
        if (arg0->head == arg0->tail) {
            arg0->tail++;
            if (arg0->tail == arg0->capacity) {
                arg0->tail = 0;
            }
            arg0->count--;
        }
    }
    return 1;
}

s32 func_151B7998(EffectObject *arg0)
{
  RingPoint *entries;
  f32 total;
  f32 scale;
  s32 i;
  s32 prev;
  f32 t;
  RingPoint *cur;
  RingPoint *next;
  f32 dx;
  f32 dy;
  f32 dz;
  f32 dist;
  f32 inv;
  f32 k;
  s32 j;
  entries = arg0->points;
  if (arg0->count >= 2)
  {
    total = 0.0f;
    i = arg0->head - 1;
    if (i < 0)
    {
      i = arg0->capacity - 1;
    }
    while (i != arg0->tail)
    {
      prev = i;
      i--;
      if (i < 0)
      {
        i = arg0->capacity - 1;
      }
      cur = &entries[prev];
      total += cur->distance;
      if ((scale = 1198.0f) < total)
      {
        next = &entries[i];
        if (cur->distance != 0)
        {
          t = (total - scale) / cur->distance;
          dx = next->pos.x - cur->pos.x;
          dy = next->pos.y - cur->pos.y;
          dz = next->pos.z - cur->pos.z;
          next->pos.x = next->pos.x - (dx * t);
          next->pos.y = next->pos.y - (dy * t);
          next->pos.z = next->pos.z - (dz * t);
          cur->distance = cur->distance * (1.0f - t);
        }
        total = scale;
        while (i != arg0->tail)
        {
          arg0->tail++;
          if (arg0->tail == arg0->capacity)
          {
 goto dummy_label_317127; dummy_label_317127: ;
            arg0->tail = 0;
          }
          arg0->count--;
        }

      }
    }

    if (total != 0)
    {
      scale = 1.0f / total;
    }
    else
    {
      scale = 0.0f;
    }
    { }
  }
  if (arg0->count >= 2)
  {
    j = arg0->head;
    dist = total;
    k = 255.0f * scale;
    do
    {
      j--;
      if (j < 0)
      {
        j = arg0->capacity - 1;
      }
      cur = &entries[j];
      cur->alpha = dist * k;
      dist -= cur->distance;
      next_iter:
      ;

      ;
    }
    while (j != arg0->tail);
  }
  return 1;
}

extern u8 D_80091100[];

Gfx *func_151B7C38(EffectObject *arg0, Gfx *arg1, s16 arg2) {
    Vec3f next;
    Vec3f cur;
    s32 index;
    s32 previous;
    Vec3f *camera;
    RingPoint *entries;
    Vec3f toCamera;
    Vec3f delta;
    Vec3f normal;
    f32 lengthSquared;
    u8 flag;
    u8 curAlpha;
    u8 nextAlpha;
    Vtx_t *vtx;

    if (arg0->count < 2) {
        return arg1;
    }
    entries = arg0->points;
    func_151D5D60((u8 *)arg0 + 0x84, arg2, (arg0->capacity * 2 + 10) * sizeof(Vtx_t), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return arg1;
    }
    camera = (Vec3f *)func_15144B34(arg2);
    flag = 1;
    arg1 = func_15142E24(arg1, (s32)D_80091100, 0, 0, 0, 0, 0x78, 0, 0, &flag, 3);
    arg1 = func_15142B7C(arg1, 0x200005, 0x1F0600);
    arg1 = func_1513F4E4(arg1, 0x56, &flag);
    arg1 = func_15142FBC(arg1, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &flag);
    previous = arg0->head - 1;
    if (previous < 0) {
        previous = arg0->capacity - 1;
    }
    index = previous - 1;
    if (index < 0) {
        index = arg0->capacity - 1;
    }
    cur = entries[previous].pos;
    curAlpha = entries[previous].alpha;
    next = entries[index].pos;
    nextAlpha = entries[index].alpha;
    delta.x = cur.x - next.x;
    delta.y = cur.y - next.y;
    delta.z = cur.z - next.z;
    toCamera.x = cur.x - camera->x;
    toCamera.y = cur.y - camera->y;
    toCamera.z = cur.z - camera->z;
    normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
    normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
    normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
    lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    if (lengthSquared != 0.0f) {
        f32 scale = 29.0f / sqrtf(lengthSquared);

        normal.x *= scale;
        normal.y *= scale;
        normal.z *= scale;
    } else {
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
    }
    vtx->ob[0] = cur.x + normal.x;
    vtx->ob[1] = cur.y + normal.y;
    vtx->ob[2] = cur.z + normal.z;
    vtx->tc[0] = 5;
    vtx->tc[1] = 0;
    vtx->cn[3] = curAlpha;
    vtx->flag = 0;
    vtx++;
    vtx->ob[0] = cur.x - normal.x;
    vtx->ob[1] = cur.y - normal.y;
    vtx->ob[2] = cur.z - normal.z;
    vtx->tc[0] = 0x1FA4;
    vtx->tc[1] = 0;
    vtx->cn[3] = curAlpha;
    vtx->flag = 0;
    vtx++;
    do {
        delta.x = cur.x - next.x;
        delta.y = cur.y - next.y;
        delta.z = cur.z - next.z;
        toCamera.x = next.x - camera->x;
        toCamera.y = next.y - camera->y;
        toCamera.z = next.z - camera->z;
        normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
        normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
        normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
        lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
        if (lengthSquared != 0.0f) {
            f32 scale = 29.0f / sqrtf(lengthSquared);

            normal.x *= scale;
            normal.y *= scale;
            normal.z *= scale;
        } else {
            normal.x = 0.0f;
            normal.y = 0.0f;
            normal.z = 0.0f;
        }
        vtx->ob[0] = next.x + normal.x;
        vtx->ob[1] = next.y + normal.y;
        vtx->ob[2] = next.z + normal.z;
        vtx->tc[0] = 5;
        vtx->tc[1] = 0;
        vtx->cn[3] = nextAlpha;
        vtx->flag = 0;
        vtx++;
        vtx->ob[0] = next.x - normal.x;
        vtx->ob[1] = next.y - normal.y;
        vtx->ob[2] = next.z - normal.z;
        vtx->tc[0] = 0x1FA4;
        vtx->tc[1] = 0;
        vtx->cn[3] = nextAlpha;
        vtx->flag = 0;
        vtx++;
        gSPVertex(arg1++, vtx - 4, 4, 0);
        gSP2Triangles(arg1++, 0, 1, 2, 0, 1, 3, 2, 0);
        previous = index;
        index--;
        if (index < 0) {
            index = arg0->capacity - 1;
        }
        if (previous != arg0->tail) {
            cur = entries[previous].pos;
            next = entries[index].pos;
            nextAlpha = entries[index].alpha;
        }
    } while (previous != arg0->tail);
    return arg1;
}

extern void (*D_8008FB98[])(void *, s32, u8);

void func_151B82CC(void *arg0, s32 arg1, u8 arg2) {
    void *v0 = *(void **)((u8 *)arg0 + 0x98);
    void (*f)(void *, s32, u8) = D_8008FB98[*(u8 *)((u8 *)v0 + 0x8)];
    if (f != 0) {
        f(arg0, arg1, arg2);
    }
}

struct S151B8318 {
    s32 unk0;
    u8 unk4;
};

struct T151B8318 {
    s32 unk0;
    struct S151B8318 *unk4;
};

void func_151B8318(struct102 *arg0, struct S151B8318 *arg1, u8 arg2) {
    int new_var;
    struct T151B8318 *v0 = *(struct T151B8318 **)((u8 *)arg0 + 0x98);
    struct S151B8318 *v1 = v0->unk4;

    if (arg2 == 0) {
        if ((v1->unk0 == (new_var = arg1->unk0)) || (arg1->unk4 == v1->unk4)) {
            func_1516972C(arg0);
        }
    }
}
void func_151B8370(void *arg0);

extern void func_151B8370(void *);
extern void func_151478F4(void *);
void func_151B83A0(void *arg0);

extern void func_15147928(void *);
void func_151B83CC(void *arg0);
