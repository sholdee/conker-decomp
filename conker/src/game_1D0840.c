#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    f32 x, y, z;
} Vec3f;

/*
 * Creation parameters of a trail (a ribbon of points that follows a point
 * on one of an actor's bones). The same 0x30 bytes start TrailState.
 */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;          /* owner->unique_id when created */
    /* 0x05 */ u8 bone;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ Vec3f offset;        /* attachment point in bone space */
    /* 0x14 */ f32 pointsPerUnit;   /* points emitted per unit travelled */
    /* 0x18 */ s16 intensity;       /* initial TrailPoint.intensity */
    /* 0x1A */ s16 holdTime;        /* initial TrailPoint.holdTime */
    /* 0x1C */ s16 fadeRate;        /* intensity lost per tick after holdTime */
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ f32 width;           /* width of a new point */
    /* 0x24 */ f32 widthGrowth;     /* width gained per second */
    /* 0x28 */ u8 texStep;          /* texture coordinate advance per point... */
    /* 0x29 */ u8 texStepRandom;    /* ...plus a random 0..texStepRandom */
    /* 0x2A */ s8 texScroll;        /* texture offset scroll per tick */
    /* 0x2B */ u8 tailFadeCount;
    /* 0x2C */ u8 tailFadeStep;
    /* 0x2D */ u8 headFadeCount;
    /* 0x2E */ u8 headFadeStep;
    /* 0x2F */ u8 pad2F;
} TrailParams;

#define TRAIL_DETACHED  1 /* owner gone: die once the trail has emptied */
#define TRAIL_FADE_TAIL 2
#define TRAIL_FADE_HEAD 4

/* Per-trail state, stored at TrailEffect.state. */
typedef struct {
    /* 0x00 */ TrailParams params;
    /* 0x30 */ u8 flags;
    /* 0x31 */ u8 pad31[3];
    /* 0x34 */ Vec3f lastPos;       /* head position when points were last emitted */
    /* 0x40 */ f32 pending;         /* fractional points not yet emitted */
    /* 0x44 */ u8 texCoord;
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ f32 age;
    /* 0x4C */ u8 texOffset;
    /* 0x4D */ u8 pad4D[3];
} TrailState;

/* Leading fields of a new effect object, passed to func_15147A80 (they land at its 0x10). */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u16 flags;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 unk18;
} EffectSpawnInfo;

struct260 *func_151A3504(TrailParams *params, u8 arg1);
void func_151A4590(struct127 *owner, u8 arg1);
void func_151A499C(struct127 *owner, u8 arg1);
void *func_1513418C(void *, s32, u8, s32);
struct260 *func_15130374(void *, u8, s32, u8, s32);
s32 func_15160A58(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_151A4E34(TrailParams *params, Vec3f *out);

void func_15143134(Vec3f *in, Vec3f *out, s32 matrix);

/* Spawns the effects attached to an actor: a trail and two more effects. */
void func_151A3390(struct127 *owner, u8 arg1) {
    TrailParams params;
    f32 origin[3];

    params.owner = owner;
    params.ownerId = owner->unique_id;
    params.bone = 1;
    params.offset.x = 0.0f;
    params.offset.y = 0.0f;
    params.offset.z = 0.0f;
    params.tailFadeCount = 1;
    params.tailFadeStep = 0xFF;
    params.headFadeCount = 8;
    params.headFadeStep = 0x1F;
    params.intensity = 170;
    params.holdTime = 40;
    params.fadeRate = 7;
    params.texStep = 2;
    params.texStepRandom = 4;
    params.texScroll = 1;
    params.pointsPerUnit = 0.01800000109f;
    params.width = 30.0f;
    params.widthGrowth = 0.9810000658f;
    func_151A3504(&params, arg1);
    func_151A4590(owner, arg1);
    func_151A499C(owner, arg1);
    func_10010154(0x1AA, owner, 0x55F0, 1000, 4000);
    origin[0] = 0.0f;
    origin[1] = 0.0f;
    origin[2] = 0.0f;
    func_15160A58(owner, 1, origin, 2, 300, 80, 0xFF, 0xFF, 0x75, 0xFF, 0, -1, 0, 0, arg1, 1);
}

/* Creates a trail effect object from params. Returns NULL if there is no owner. */
struct260 *func_151A3504(TrailParams *params, u8 arg1) {
    EffectSpawnInfo info;
    TrailState state;
    struct260 *effect;

    if (params->owner == NULL) {
        return NULL;
    }

    memcpy(&state, params, sizeof(TrailParams));
    state.texCoord = 0;
    info.unk15 = 50;
    info.flags = 2;
    info.unkC = 1000;
    state.flags = TRAIL_FADE_TAIL | TRAIL_FADE_HEAD;
    state.texOffset = 0;
    state.pending = 0.0f;
    state.age = 0.0f;

    if (func_151A4E34(&state.params, &info.pos) != 0) {
        state.lastPos = info.pos;
        info.flags |= 4;
    }

    info.unk10 = 8;
    effect = (struct260 *)func_15147A80(&info, sizeof(TrailState), 24, 6, 6, 6, 0, 0, 0, arg1, 0);
    if (effect != NULL) {
        memcpy((void *)*(s32 *)((u8 *)effect + 0x98), &state, sizeof(TrailState));
    }
    return effect;
}

/* One point of a trail. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 width;
    /* 0x10 */ s16 intensity;       /* alpha scale; the point dies when it drops below 0 */
    /* 0x12 */ s16 holdTime;        /* ticks before intensity starts to fall */
    /* 0x14 */ u8 alpha;
    /* 0x15 */ u8 texCoord;
    /* 0x16 */ u8 pad16[2];
} TrailPoint;

#define TRAIL_EFFECT_ATTACHED 2     /* the head follows the owner's bone */
#define TRAIL_EFFECT_STARTED  4     /* state.lastPos is valid */

/* Effect object created by func_15147A80 for a trail. points[] is a ring buffer. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 pad0D[3];
    /* 0x10 */ Vec3f headPos;
    /* 0x1C */ u8 pad1C[2];
    /* 0x1E */ u16 flags;
    /* 0x20 */ u8 pad20[5];
    /* 0x25 */ u8 capacity;
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail;             /* oldest point */
    /* 0x2E */ s8 head;             /* slot the next point goes in */
    /* 0x2F */ u8 pad2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 pad31[0x23];
    /* 0x54 */ Vec3f tailPos;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailState *state;
} TrailEffect;

/*
 * Per-tick update of a trail's points: ages and fades them, drops dead ones from
 * the tail and publishes the tail position. Returns 0 to kill a detached trail.
 */
s32 func_151A361C(TrailEffect *effect) {
    TrailState *state = effect->state;
    TrailPoint *points = effect->points;
    TrailPoint *point;
    s32 i;

    if (effect->count < 2 && (state->flags & TRAIL_DETACHED)) {
        return 0;
    }
    state->texOffset += state->params.texScroll * D_800BE9E4;
    i = effect->head;
    if (i != effect->tail) {
        do {
            i--;
            if (i < 0) {
                i = effect->capacity - 1;
            }
            point = &points[i];
            if (point->holdTime > 0) {
                point->holdTime -= D_800BE9E4;
            } else {
                point->intensity -= D_800BE9E4 * state->params.fadeRate;
            }
            point->alpha = 0xFF;
            point->width += D_800BE9A4 * state->params.widthGrowth;
            if (point->intensity < 0) {
                state->flags &= ~TRAIL_FADE_TAIL;
                while (i != effect->tail) {
                    effect->tail++;
                    if (effect->tail == effect->capacity) {
                        effect->tail = 0;
                    }
                    effect->count--;
                }
                points[effect->tail].intensity = 0;
            }
        } while (i != effect->tail);
    }
    if (effect->count > 0) {
        effect->tailPos = points[effect->tail].pos;
    } else {
        effect->tailPos.x = 0.0f;
        effect->tailPos.y = 0.0f;
        effect->tailPos.z = 0.0f;
    }
    return 1;
}

void func_151A4E9C(void *arg0);

/*
 * Per-tick update of a trail's head: follows the owner's bone and emits points
 * along the path travelled. If the bone can no longer be found, the trail is
 * detached and a TrailRetryTask is started to re-create it later.
 */
s32 func_151A37C0(TrailEffect *effect) {
    TrailState *state = effect->state;
    TrailPoint *points = effect->points;
    struct127 *owner;
    Vec3f delta;
    s32 started;
    TrailParams retry;

    owner = state->params.owner;
    if (owner->interaction_state == 0) {
        return 0;
    }
    if (owner->unique_id != state->params.ownerId) {
        return 0;
    }
    started = 0;
    if (!(effect->flags & TRAIL_EFFECT_STARTED)) {
        if (func_151A4E34(&state->params, &effect->headPos) != 0) {
            started = 1;
            state->lastPos = effect->headPos;
            effect->flags |= TRAIL_EFFECT_STARTED;
        } else {
            return 1;
        }
    }
    if (!started && func_151A4E34(&state->params, &effect->headPos) == 0) {
        func_151A4E9C(effect);
        memcpy(&retry, state, sizeof(TrailParams));
        {
            struct260 *task = func_151491F4(300, -1, 10, 0, 5, sizeof(TrailParams), effect->unkC, 0);
            if (task != NULL) {
                memcpy((u8 *)task + 0x28, &retry, sizeof(TrailParams));
            }
        }
        return 1;
    }
    delta.x = effect->headPos.x - state->lastPos.x;
    delta.y = effect->headPos.y - state->lastPos.y;
    delta.z = effect->headPos.z - state->lastPos.z;
    if (fabsf(delta.x) > 0.0001f || fabsf(delta.y) > 0.0001f || fabsf(delta.z) > 0.0001f) {
        state->pending += sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) * state->params.pointsPerUnit;
    }
    if (state->pending > 1.0f) {
        f32 invPending;
        f32 age;
        f32 ageStep;
        f32 width;
        Vec3f pos;
        f32 stepX;
        f32 stepY;
        f32 widthLeft;
        f32 stepZ;
        f32 widthStep;

        invPending = 1.0f / state->pending;
        age = state->age + D_800BE9A4;
        pos = state->lastPos;
        ageStep = age * invPending;
        width = state->params.width + state->params.widthGrowth * age;
        stepX = delta.x * invPending;
        stepY = delta.y * invPending;
        stepZ = delta.z * invPending;
        widthLeft = state->params.width - width;
        widthStep = widthLeft * invPending;
        do {
            TrailPoint *point = &points[effect->head];
            point->pos = pos;
            point->width = width;
            point->intensity = state->params.intensity;
            point->holdTime = state->params.holdTime;
            point->texCoord = state->texCoord;
            // matching: reusing "started" for the random value (on one line) sets the frame layout
            started = func_150ADA20(); state->texCoord += state->params.texStep + (u32)started % (u32)(state->params.texStepRandom + 1);
            point->alpha = 0xFF;
            effect->head++;
            if (effect->head == effect->capacity) {
                effect->head = 0;
            }
            effect->count++;
            if (effect->head == effect->tail) {
                effect->tail++;
                if (effect->tail == effect->capacity) {
                    effect->tail = 0;
                }
                effect->count--;
            }
            width += widthStep;
            pos.x += stepX;
            pos.y += stepY;
            pos.z += stepZ;
            age -= ageStep;
            state->pending -= 1.0f;
        } while (state->pending > 1.0f);
        state->lastPos = pos;
        state->age = age;
    }
    /*
     * matching: nine scopes holding a kept dead load each give as1 enough extra
     * basic blocks to stop it hoisting an addiu into the 'pending > 1.0f' branch
     * (ledger: ~/conker-ws/func_151A37C0/notes.md, pass6).
     */
    { typedef s32 T0; state = *(TrailState **)((u8 *)effect + 0x40); state = state; }
    { typedef s32 T1; state = *(TrailState **)((u8 *)effect + 0x44); state = state; }
    { typedef s32 T2; state = *(TrailState **)((u8 *)effect + 0x48); state = state; }
    { typedef s32 T3; state = *(TrailState **)((u8 *)effect + 0x4C); state = state; }
    { typedef s32 T4; state = *(TrailState **)((u8 *)effect + 0x50); state = state; }
    { typedef s32 T5; state = *(TrailState **)((u8 *)effect + 0x54); state = state; }
    { typedef s32 T6; state = *(TrailState **)((u8 *)effect + 0x58); state = state; }
    { typedef s32 T7; state = *(TrailState **)((u8 *)effect + 0x5C); state = state; }
    { typedef s32 T8; state = *(TrailState **)((u8 *)effect + 0x60); state = state; }
    return 1;
}

extern s32 D_800D2C9C;
extern u8 D_80090CD4[];
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

/*
 * Draws a trail as a camera-facing ribbon: one quad per pair of neighbouring
 * points, from the head back to the tail. Needs at least two points.
 */
Gfx *func_151A3BE4(TrailEffect *effect, Gfx *gfx, s16 view) {
    Vtx *vtx;
    f32 camX, camY, camZ, segX, segY, segZ;
    f32 sideX, sideY, sideZ, lenSq, scale;
    u8 pad; // matching: unused local keeps the frame layout
    s32 idx;
    Vec3f next;
    Vec3f cur;
    s32 nextIdx;
    s32 curIdx;
    u8 nextTex;
    u8 curTex;
    u8 nextAlpha;
    u8 curAlpha;
    u8 stateFlag;
    f32 *camPos;
    TrailPoint *points;
    TrailState *state;

    if (effect->count >= 2) {
        func_151D5D60(effect->unk84, view, (effect->capacity + 5) * 2 * sizeof(Vtx), (void **)&vtx, 0);
        if (vtx != NULL) {
            state = effect->state;
            camPos = &D_800DBFF0[view].unk2F8;
            points = effect->points;
            stateFlag = 1;
        fence:; // matching: a label here is a uopt barrier before the fade loops
            if (state->flags & TRAIL_FADE_TAIL) {
                u8 fade;
                s16 remaining;

                fade = 0;
                remaining = state->params.tailFadeCount;
                idx = effect->tail;
                do {
                    points[idx].alpha = fade;
                    idx++;
                    fade += state->params.tailFadeStep;
                    if (idx == effect->capacity) {
                        idx = 0;
                    }
                    remaining--;
                } while (remaining != 0 && idx != effect->head);
            }
            if (state->flags & TRAIL_FADE_HEAD) {
                u8 fade;
                s16 remaining;

                fade = 0;
                remaining = state->params.headFadeCount;
                idx = effect->head - 1;
                if (idx < 0) {
                    idx = effect->capacity - 1;
                }
                do {
                    points[idx].alpha = (points[idx].alpha * fade) >> 8;
                    idx--;
                    fade += state->params.headFadeStep;
                    remaining--;
                    if (idx < 0) {
                        idx = effect->capacity - 1;
                    }
                } while (remaining != 0 && idx != effect->head);
            }
            gfx = func_15142E24(gfx, D_80090CD4, 0, 0, 0, 0, 0x1F, 0, 0, &stateFlag, 3);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &stateFlag);
            gfx = func_15142CF0(gfx, 0, 0, 0, 0, 0, 0, &stateFlag);
            gfx = func_1513F4E4(gfx, 6, &stateFlag);
            gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &stateFlag);
            if (effect->flags & TRAIL_EFFECT_ATTACHED) {
                nextIdx = effect->head - 1;
                if (nextIdx < 0) {
                    nextIdx = effect->capacity - 1;
                }
                cur = effect->headPos;
                curTex = points[nextIdx].texCoord + state->texOffset;
                curAlpha = (points[nextIdx].intensity * points[nextIdx].alpha) >> 8;
            } else {
                curIdx = effect->head - 1;
                if (curIdx < 0) {
                    curIdx = effect->capacity - 1;
                }
                nextIdx = curIdx - 1;
                if (nextIdx < 0) {
                    nextIdx = effect->capacity - 1;
                }
                cur = points[curIdx].pos;
                curTex = points[curIdx].texCoord + state->texOffset;
                curAlpha = (points[curIdx].intensity * points[curIdx].alpha) >> 8;
            }
            next = points[nextIdx].pos;
            nextTex = points[nextIdx].texCoord + state->texOffset;
            nextAlpha = (points[nextIdx].intensity * points[nextIdx].alpha) >> 8;
            segX = cur.x - next.x;
            segY = cur.y - next.y;
            segZ = cur.z - next.z;
            camX = cur.x - camPos[0];
            camY = cur.y - camPos[1];
            camZ = cur.z - camPos[2];
            sideX = segY * camZ - camY * segZ;
            sideY = segZ * camX - camZ * segX;
            sideZ = segX * camY - camX * segY;
            lenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
            if (lenSq == 0.0f) {
                sideZ = sideY = sideX = 0.0f;
            } else {
                scale = points[nextIdx].width / sqrtf(lenSq);
                sideX *= scale;
                sideY *= scale;
                sideZ *= scale;
            }
            vtx->v.ob[0] = cur.x + sideX;
            vtx->v.ob[1] = cur.y + sideY;
            vtx->v.ob[2] = cur.z + sideZ;
            vtx->v.tc[0] = curTex << 6;
            vtx->v.tc[1] = 0x7C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - sideX;
            vtx->v.ob[1] = cur.y - sideY;
            vtx->v.ob[2] = cur.z - sideZ;
            vtx->v.tc[0] = curTex << 6;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                segX = cur.x - next.x;
                segY = cur.y - next.y;
                segZ = cur.z - next.z;
                camX = next.x - camPos[0];
                camY = next.y - camPos[1];
                camZ = next.z - camPos[2];
                sideX = segY * camZ - camY * segZ;
                sideY = segZ * camX - camZ * segX;
                sideZ = segX * camY - camX * segY;
                lenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
                if (lenSq == 0.0f) {
                    sideZ = sideY = sideX = 0.0f;
                } else {
                    scale = points[nextIdx].width / sqrtf(lenSq);
                    sideX *= scale;
                    sideY *= scale;
                    sideZ *= scale;
                }
                vtx->v.ob[0] = next.x + sideX;
                vtx->v.ob[1] = next.y + sideY;
                vtx->v.ob[2] = next.z + sideZ;
                vtx->v.tc[0] = nextTex << 6;
                vtx->v.tc[1] = 0x7C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = nextAlpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = next.x - sideX;
                vtx->v.ob[1] = next.y - sideY;
                vtx->v.ob[2] = next.z - sideZ;
                vtx->v.tc[0] = nextTex << 6;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = nextAlpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (curTex < nextTex) {
                    memcpy(vtx, vtx - 2, 2 * sizeof(Vtx));
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                }
                curIdx = nextIdx;
                nextIdx--;
                if (nextIdx < 0) {
                    nextIdx = effect->capacity - 1;
                }
                if (curIdx != effect->tail) {
                    curTex = nextTex;
                    cur = next;
                    next = points[nextIdx].pos;
                    nextTex = points[nextIdx].texCoord + state->texOffset;
                    nextAlpha = (points[nextIdx].intensity * points[nextIdx].alpha) >> 8;
                }
            } while (curIdx != effect->tail);
        }
    }
    return gfx;
}

/* Parameters for func_1513418C, laid out like the other bone-attached effects here. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 ownerId;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ struct127 *owner;
    /* 0x10 */ u8 bone;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ Vec3f offset;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ u8 unk2D;
} BoneEffectParams151A4590;

void func_151A4590(struct127 *owner, u8 arg1) {
    BoneEffectParams151A4590 params;

    // matching: an integer test makes IDO reload owner from its arg slot
    if ((s32)owner != 0) {
        params.unk0 = 0;
        params.unk4 = 0;
        params.ownerId = owner->unique_id;
        params.bone = 1;
        params.offset.x = 0.0f;
        params.offset.y = 0.0f;
        params.offset.z = 0.0f;
        params.unk20 = 14.69999981f;
        params.unk24 = 3.0f;
        params.unk28 = 100;
        params.unk2A = 10;
        params.unk2B = 1;
        params.unk2C = -1;
        params.unk2D = 0;
        params.owner = owner;
        func_1513418C(&params, 0, arg1, 0);
    }
}

/* Leading fields shared by the effect and task objects in this file. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ s16 unkE;
} EffectHeader;

/* Particle emitter description passed to func_15130374. */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s32 unk0C;
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
    /* 0x3C */ u8 pad3C[0xC];
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 flags;
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ u8 pad64[0xC];
} ParticleSpawn151A4638;

/* Emits a particle burst at (x, y, z), moving against the given direction. */
void func_151A4638(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, EffectHeader *parent) {
    ParticleSpawn151A4638 spawn;
    f32 scale;
    s16 params[6];
    struct260 *particles;

    spawn.unk1D = 0x27;
    spawn.unk08 = 0x1401;
    spawn.unk00 = 0x200005;
    spawn.unk04 = 0;
    spawn.unk0A = (func_150ADA20() % 5U) + 15;
    spawn.unk0C = 0;
    spawn.unk10 = 0;
    spawn.unk14 = 0x8A;
    spawn.unk15 = 0;
    spawn.unk16 = 0;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0;
    spawn.unk19 = 0;
    spawn.unk1A = 0;
    spawn.unk1E = 1;
    spawn.unk20 = 0xFF;
    spawn.unk22 = 1;
    spawn.unk24 = 1.0f;

    spawn.unk28 = spawn.unk2C = func_150ADA68() * 50.0f + 500.0f;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    scale = 0.0869999975f * D_800BE9A8;
    spawn.vel.x = -dirX * scale;
    spawn.vel.y = -dirY * scale;
    spawn.vel.z = -dirZ * scale;
    spawn.flags = 0xD;
    spawn.unk60 = 1;
    spawn.unk61 = 1;
    spawn.unk54 = 0.0f;

    if (func_150ADA20() & 1) {
        spawn.flags |= 0x40;
    }

    if (func_150ADA20() & 1) {
        spawn.flags |= 0x80;
    }

    spawn.unk1B = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk62 = 0;
    spawn.unk63 = -1;
    params[0] = 16;
    params[1] = 15;
    params[2] = 13;
    params[3] = 19;
    params[4] = 17;
    params[5] = -24;

    particles = func_15130374(&spawn, 0, 12, parent->unkC, 1);
    if (particles != NULL) {
        memcpy((u8 *)particles + 0xA8, params, sizeof(params));
    }
}

/* Per-particle parameters copied in by func_151A4638: (age limit, factor) pairs. */
typedef struct {
    /* 0x0 */ s16 limit2C;          /* below this age, unk2C = age * mul2C */
    /* 0x2 */ s16 mul2C;
    /* 0x4 */ s16 limit2B;          /* below this age, unk2B = age * mul2B */
    /* 0x6 */ s16 mul2B;
    /* 0x8 */ s16 limitSize;        /* below this age, sizes change by sizeRate per tick */
    /* 0xA */ s16 sizeRate;
} ParticleAgeParams;

/* A particle created by func_15130374. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 age;             /* counts down */
    /* 0x1C */ u8 pad1C[0xF];
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 pad2D[0xB];
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u8 pad40[0x30];
    /* 0x70 */ u8 unk70;
    /* 0x71 */ u8 unk71;
    /* 0x72 */ u8 unk72;
    /* 0x73 */ u8 pad73[0x35];
    /* 0xA8 */ ParticleAgeParams ageParams;
} Particle;

/* Particle update for func_151A4638's bursts: fades and resizes by age. */
s32 func_151A483C(Particle *particle, u8 arg1) {
    s16 age;
    ParticleAgeParams *params;

    params = &particle->ageParams;
    age = particle->age;
    if (age < params->limit2B) {
        particle->unk2B = age * params->mul2B;
    }
    if (age < params->limitSize) {
        {
            f32 sizeStep = params->sizeRate * D_800BE9E4;

            particle->unk38 += sizeStep;
            particle->unk3C += sizeStep;
        }
        age = particle->age;
    }
    if (age < params->limit2C) {
        particle->unk72 = 1;
        particle->unk70 = 2;
        particle->unk71 = 2;
        particle->unk2C = age * params->mul2C;
        particle->unk18 = 0x5203;
    }
    return 1;
}

/* Like func_151A483C, but always sets unk2C from the age. */
s32 func_151A4900(Particle *particle, u8 arg1) {
    s16 age;
    ParticleAgeParams *params;

    params = &particle->ageParams;
    age = particle->age;
    if (age < params->limit2B) {
        particle->unk2B = age * params->mul2B;
    }
    if (age < params->limitSize) {
        {
            f32 sizeStep = params->sizeRate * D_800BE9E4;

            particle->unk38 += sizeStep;
            particle->unk3C += sizeStep;
        }
        age = particle->age;
    }
    particle->unk2C = age * params->mul2C;
    return 1;
}

extern f32 D_800A8D64;

struct frame151A499C {
    void *unk0;
    u8 unk4;
    f32 unk8;
    f32 unkC;
    u8 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

void func_151A499C(struct127 *arg0, u8 arg1) {
    struct260 *ret;
    struct frame151A499C sp2C;

    sp2C.unk0 = arg0;
    sp2C.unk4 = *((u8 *)arg0 + 0x3B);
    sp2C.unkC = 0.0f;
    sp2C.unk10 = 1;
    sp2C.unk14 = 0.0f;
    sp2C.unk18 = 0.0f;
    sp2C.unk1C = 0.0f;
    sp2C.unk8 = D_800A8D64;
    ret = func_151491F4(0x12C, -1, 5, 0, 1, 0x20, arg1, 0);
    if (ret != 0) {
        memcpy((u8 *)ret + 0x28, &sp2C, 0x20);
    }
}

struct Obj151A4A38 {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x38];
    u8 unk74;
    u8 pad75[1];
    u16 unk76;
    u8 pad78[0x15C];
    s32 unk1D4;
};

struct Sub151A4A38 {
    struct Obj151A4A38 *unk0;
    u8 unk4;
    u8 pad5[3];
    f32 unk8;
    f32 unkC;
    u8 unk10;
    u8 pad11[3];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct Ent151A4A38 {
    u8 pad0[1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 unkD;
    s16 unkE;
    u8 pad10[0x18];
    struct Sub151A4A38 unk28;
};

struct Block151A4A38 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Arg151A4A38 {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    struct Block151A4A38 unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    s8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
};

void func_15153634(void *, s32, s32, s32);

void func_151A4A38(struct Ent151A4A38 *arg0) {
    struct Sub151A4A38 *sub;
    struct Block151A4A38 sp98;
    struct Arg151A4A38 sp2C;
    s16 sp2A;

    sub = &arg0->unk28;
    if ((arg0->unk28.unk0->unk0 == 0) || (sub->unk0->unk0 == 8) || (sub->unk0->unk3B != sub->unk4)) {
        arg0->unkE = -1;
        arg0->unkD |= 1;
        return;
    }

    if (sub->unk0->unk1D4 == 0) {
        return;
    }
    if ((sub->unk0->unk74 & 0xF) == 0xF) {
        return;
    }

    sub->unkC += sub->unk8 * D_800BE9A4;
    if (sub->unkC > 1.0f) {
        sp2A = (sub->unk0->unk76 >> 8) - 0x40;
        func_15143134((Vec3f *)&sub->unk14, (Vec3f *)&sp98, sub->unk0->unk1D4 + (sub->unk10 << 6));

        sp2C.unk0 = (s16)sub->unkC;
        sp2C.unk2 = 0;
        sub->unkC -= (f32)sp2C.unk0;
        sp2C.unk4 = 0x28;
        sp2C.unk6 = 0xC01;
        sp2C.unk8 = 0x200005;
        sp2C.unkC = 0;
        sp2C.unk10 = 0x17;
        sp2C.unk12 = 0xD;
        sp2C.unk14 = 0;
        sp2C.unk18 = 0;
        sp2C.unk1C = 0;
        sp2C.unk1D = 0;
        sp2C.unk1E = 0;
        sp2C.unk1F = 0xFF;
        sp2C.unk20 = 0;
        sp2C.unk21 = 0;
        sp2C.unk22 = 0;
        sp2C.unk30 = 300.0f;
        sp2C.unk34 = 400.0f;
        sp2C.unk38 = sp98;
        sp2C.unk26 = 1;
        sp2C.unk28 = 0;
        sp2C.unk2A = 1;
        sp2C.unk2C = 1.0f;
        sp2C.unk64 = 0;
        sp2C.unk44 = sp2A - 0x19;
        sp2C.unk46 = -0x2C;
        sp2C.unk48 = 0x32;
        sp2C.unk4A = 0x32;
        sp2C.unk4C = 0.0f;
        sp2C.unk50 = 15.0f;
        sp2C.unk54 = -0.5f;
        sp2C.unk58 = -0.5f;
        sp2C.unk5C = 7;
        sp2C.unk62 = 1;
        sp2C.unk63 = 0;

        if (func_150ADA20() & 1) {
            sp2C.unk5C |= 0x40;
        }
        if (func_150ADA20() & 1) {
            sp2C.unk5C |= 0x80;
        }

        sp2C.unk23 = 0xFF;
        sp2C.unk24 = 0;
        sp2C.unk25 = 0xFF;
        sp2C.unk60 = -1;
        sp2C.unk61 = -1;
        func_15153634(&sp2C, 0xFF, arg0->unkC, arg0->unk1);
    }
}

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA_151A4CE0;

typedef struct {
    s32 unk0;
    union {
        u8 b4;
        s32 w4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_151A4CE0;

void func_151A4E9C(void *arg0);

void func_151A4CE0(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_151A4CE0 *temp_v0 = *(SubA_151A4CE0 **)((u8 *)arg0 + 0x98);
    ArgB_151A4CE0 *b = (ArgB_151A4CE0 *)arg1;

    if (arg2 == 0) {
        if ((b->unk0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_151A4E9C(arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

void func_1516972C(struct102 *arg0);

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA_151A4D88;

typedef struct {
    s32 unk0;
    union {
        u8 b4;
        s32 w4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_151A4D88;

void func_151A4D88(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_151A4D88 *temp_v0 = (SubA_151A4D88 *)((u8 *)arg0 + 0x28);
    ArgB_151A4D88 *b = (ArgB_151A4D88 *)arg1;

    if (arg2 == 0) {
        if ((b->unk0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

/*
 * Transforms the trail's attachment offset by its owner's bone matrix into out.
 * Returns 0 if the owner has no bone matrices or is not being drawn.
 */
s32 func_151A4E34(TrailParams *params, Vec3f *out) {
    struct127 *owner = params->owner;
    u8 *bones = (u8 *)owner->unk1D4;

    if (bones == NULL) {
        return 0;
    }
    if ((owner->unk74 & 0xF) == 0xF) {
        return 0;
    }
    func_15143134(&params->offset, out, (s32)bones + params->bone * sizeof(Mtx));
    return 1;
}


void func_151A4E9C(void *arg0)
{
  u8 *p;
  u8 v;
  int mask;
  *((u8 *) (((u8 *) arg0) + 0x30)) = 0;
  *((u16 *) (((u8 *) arg0) + 0x1E)) &= 0xFFFD;
 mask = 0xFFu; do { p = *((u8 **) (((u8 *) arg0) + 0x98)); v = (p[0x30] & mask) | 1; *(p + 0x30) = v; *((volatile u8 *) (p + 0x30)) = v | 4; } while (0);
}


/* A func_151491F4 task that retries creating a trail until its owner can carry it. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ TrailParams trail;
} TrailRetryTask;

/*
 * Per-tick update of a TrailRetryTask: the task ends when the owner is gone or
 * once the trail has been re-created on the owner's bone.
 */
void func_151A4ECC(TrailRetryTask *task) {
    TrailParams *trail;
    u8 done;

    done = 0;
    trail = &task->trail;
    if (trail->owner->interaction_state == 0) {
        done = 1;
    }
    if (trail->owner->unique_id != trail->ownerId) {
        done = 1;
    }
    if (!done) {
        if (trail->owner->unk1D4 != NULL) {
            if ((trail->owner->unk74 & 0xF) != 0xF) {
                func_151A3504(trail, task->unkC);
                done = 1;
            }
        }
    }
    if (done) {
        task->unkE = -1;
        task->unkD = task->unkD | 1;
    }
}

void func_1516972C(struct102 *arg0);

struct struct151A4F7C {
    s32 unk0;
    u8 unk4;
};

void func_151A4F7C(struct102 *arg0, struct102 *arg1, u8 arg2)
{
    struct struct151A4F7C *p = (struct struct151A4F7C *)((u8 *)arg0 + 0x28);
    struct struct151A4F7C *q = (struct struct151A4F7C *)arg1;
    if (!arg2) {
        if (q->unk0 == p->unk0 || q->unk4 == p->unk4) {
            func_1516972C(arg0);
        }
    }
}
