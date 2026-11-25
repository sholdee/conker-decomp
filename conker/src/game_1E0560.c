#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f_1E0560;

typedef struct {
    /* 0x00 */ Vec3f_1E0560 pos;
    /* 0x0C */ Vec3f_1E0560 vel;
} HoltenRopePoint;

/* Rope effect: a 10-point chain hung between two endpoints and drawn as a textured strip. */
typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ u8 flags;           // 1: timed, 2: points need reset, 4: start lost, 8: end lost
    /* 0x011 */ u8 pad11;
    /* 0x012 */ s16 timer;
    /* 0x014 */ Vec3f_1E0560 start; // written by startCallback
    /* 0x020 */ Vec3f_1E0560 end;   // written by endCallback
    /* 0x02C */ s8 startCallback;   // index into D_8008FAF0, -1 for none
    /* 0x02D */ s8 endCallback;     // index into D_8008FAF0, -1 for none
    /* 0x02E */ u8 drawStyle;       // index into D_8008FB10
    /* 0x02F */ u8 pad2F;
    /* 0x030 */ f32 halfWidth;
    /* 0x034 */ s8 updateCallback;  // index into D_8008FAF8, -1 for none
    /* 0x035 */ u8 pad35[3];
    /* 0x038 */ f32 length;
    /* 0x03C */ f32 gain;
    /* 0x040 */ f32 stiffness;
    /* 0x044 */ u8 type;            // index into D_8008FB68 and D_8008FB70
    /* 0x045 */ u8 pad45[3];
    /* 0x048 */ HoltenRopePoint points[10];
    /* 0x138 */ f32 maxSpanSq;      // 2 * length^2 / pi^2
    /* 0x13C */ s32 texStep;        // texture t step per segment
    /* 0x140 */ u8 vtxBuffer[0x10];
    /* 0x150 */                     // type-specific data follows
} HoltenRopeEffect;

/* Template copied over HoltenRopeEffect 0x10-0x47 when a rope is created. */
typedef struct {
    u8 data[0x38];
} HoltenRopeDesc;

/* The actor a rope end is attached to. */
typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ u8 pad4[0x10];
    /* 0x014 */ Vec3f_1E0560 pos;
    /* 0x020 */ u8 pad20[0x1B];
    /* 0x03B */ u8 unk3B;
    /* 0x03C */ u8 pad3C[0x198];
    /* 0x1D4 */ Mtx *boneMtx;
} HoltenRopeTarget;

/* Endpoint data at 0x150 for ropes hung from an actor. */

typedef struct {
    /* 0x00 */ HoltenRopeTarget *target;
    /* 0x04 */ u8 targetId; // target->unk3B when the rope was attached
} HoltenRopeAnchor;

typedef struct {
    HoltenRopeEffect effect;
    HoltenRopeAnchor anchor;
} HoltenRopeAttached;

extern s32 (*D_8008FAF0[])(HoltenRopeEffect *, Vec3f_1E0560 *, s32);
extern s32 (*D_8008FAF8[])(HoltenRopeEffect *);
extern void (*D_8008FB70[])(HoltenRopeEffect *);
extern void *func_15167A68(s32, s32, s32, s32, u8, s32);
extern void func_151D5E30(void *);

/* Allocates a rope effect with `extraSize` bytes of type data and initializes it from `desc`. */
HoltenRopeEffect *func_151B30B0(HoltenRopeDesc *desc, f32 texScale, s32 extraSize, u8 arg3, s32 arg4) {
    HoltenRopeEffect *effect;
    HoltenRopeEffect *result;
    f32 length;
    f32 spanSq;

    effect = func_15167A68(0x33, arg4, extraSize + sizeof(HoltenRopeEffect), 1, arg3, 1);
    if (effect == NULL) {
        return NULL;
    }

    result = effect;
    memcpy(&effect->flags, desc, sizeof(HoltenRopeDesc));
    length = effect->length;
    effect->flags |= 0xE;
    spanSq = (length * length) / 9.869605064f;
    effect->maxSpanSq = spanSq + spanSq;
    effect->texStep = length * texScale * 4096.0f;
    bzero(effect->vtxBuffer, sizeof(effect->vtxBuffer));
    return result;
}

/* Per-frame update: counts down the lifetime, refreshes both endpoints and moves the points. */
void func_151B3184(HoltenRopeEffect *effect) {
    s32 callback;
    u8 finished;

    finished = FALSE;
    if (effect->flags & 1) {
        effect->timer -= D_800BE9E4;
        if (effect->timer < 0) {
            finished = TRUE;
        }
    }
    if (!finished) {
        callback = effect->startCallback;
        if (callback != -1) {
            if (!D_8008FAF0[callback](effect, &effect->start, TRUE)) {
                finished = TRUE;
            }
        }
        callback = effect->endCallback;
        if (callback != -1) {
            if (!D_8008FAF0[callback](effect, &effect->end, FALSE)) {
                finished = TRUE;
            }
        }
        if ((effect->flags & 4) || (effect->flags & 8)) {
            effect->flags |= 2;
        } else {
            callback = effect->updateCallback;
            if (callback != -1) {
                if (!D_8008FAF8[callback](effect)) {
                    finished = TRUE;
                }
            }
        }
    }
    if (finished) {
        func_1516972C((struct102 *)effect);
    }
}

/* Returns the draw settings for a rope's display list; nonzero if the rope should be drawn. */
typedef s32 (*HoltenRopeDrawStyle)(HoltenRopeEffect *effect, s32 *mode0, s32 *mode1,
                                   s32 *primR, s32 *primG, s32 *primB, s32 *primA,
                                   s32 *envR, s32 *envG, s32 *envB, s32 *envA,
                                   s32 *cycleType, u8 *renderMode, u8 *unk13);

extern HoltenRopeDrawStyle D_8008FB10[];
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090DE8[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/*
 * Draws the rope for view `view` as a camera-facing strip: each point gets a vertex
 * pair offset by `halfWidth` along the side vector (segment x eye-to-point).
 */
Gfx *func_151B32C8(Gfx *gfx, HoltenRopeEffect *effect, s16 view) {
    s32 i;
    Vtx *vtx;
    Vec3f_1E0560 a;
    Vec3f_1E0560 b;
    s32 texT;
    f32 *eye;
    f32 toEyeX;
    f32 toEyeY;
    f32 toEyeZ;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 sideX;
    f32 sideY;
    f32 sideZ;
    f32 lenSq;
    f32 pad[2]; // matching: stack slots

    if (effect->flags & 4) {
        return gfx;
    }
    if (effect->flags & 8) {
        return gfx;
    }
    if (effect->flags & 2) {
        return gfx;
    }
    func_151D5D60(effect->vtxBuffer, view, 25 * sizeof(Vtx), (void **)&vtx, NULL);
    if (vtx != NULL) {
        u8 needSync;
        s32 mode0;
        s32 mode1;
        s32 primR;
        s32 primG;
        s32 primB;
        s32 primA;
        s32 envR;
        s32 envG;
        s32 envB;
        s32 envA;
        s32 cycleType;
        u8 renderMode;
        u8 unk13;

        eye = &D_800DBFF0[view].unk2F8;
        needSync = TRUE;
        {
            HoltenRopePoint *points;

            if (!D_8008FB10[effect->drawStyle](effect, &mode0, &mode1, &primR, &primG, &primB, &primA, &envR, &envG, &envB, &envA, &cycleType, &renderMode, &unk13)) {
                return gfx;
            }
            points = effect->points;
            gfx = func_15142B7C(gfx, mode0, mode1);
            gfx = func_15142C10(gfx, envR, envG, envB, envA, &needSync);
            gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &needSync);
            gfx = func_1513F4E4(gfx, unk13, &needSync);
            gfx = func_15142E24(gfx, (s32)D_80090DE8, 0, 0, 0, 0, 0x36, 0, 0, &needSync, 3);
            gfx = func_15142FBC(gfx, cycleType | G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[renderMode].unk0 | D_800A4AC8[renderMode].unk4, &needSync);
            {
                f32 pad3; // matching: a local here opens a -g3 scope that IDO will not schedule across

                /* first pair: side vector of the first segment, placed at its start */
                a = points[0].pos;
                b = points[1].pos;
                dx = b.x - a.x;
                dy = b.y - a.y;
                dz = b.z - a.z;
                toEyeX = a.x - eye[0];
                toEyeY = a.y - eye[1];
                toEyeZ = a.z - eye[2];
                sideX = (dy * toEyeZ) - (toEyeY * dz);
                sideY = (dz * toEyeX) - (toEyeZ * dx);
                sideZ = (dx * toEyeY) - (toEyeX * dy);
                lenSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
                if (lenSq == 0.0f) {
                    sideX = 0.0f;
                    sideY = 0.0f;
                    sideZ = 0.0f;
                } else {
                    dz = effect->halfWidth / sqrtf(lenSq);
                    sideX *= dz;
                    sideY *= dz;
                    sideZ *= dz;
                }
                vtx->v.ob[0] = a.x + sideX;
                vtx->v.ob[1] = a.y + sideY;
                vtx->v.ob[2] = a.z + sideZ;
                vtx->v.tc[0] = 0;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 255;
                vtx->v.cn[1] = 100;
                vtx->v.cn[2] = 100;
                vtx->v.cn[3] = 255;
                vtx++;
                vtx->v.ob[0] = a.x - sideX;
                vtx->v.ob[1] = a.y - sideY;
                vtx->v.ob[2] = a.z - sideZ;
                vtx->v.tc[0] = 0x3C0;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 255;
                vtx->v.cn[1] = 100;
                vtx->v.cn[2] = 100;
                vtx->v.cn[3] = 255;
                vtx++;
                /* then one pair per point at the end of each segment, and two triangles joining it to the previous pair */
                texT = effect->texStep;
                for (i = 1; i < 10; i++) {
                    a = points[i - 1].pos;
                    b = points[i].pos;
                    dx = b.x - a.x;
                    dy = b.y - a.y;
                    dz = b.z - a.z;
                    toEyeX = b.x - eye[0];
                    toEyeY = b.y - eye[1];
                    toEyeZ = b.z - eye[2];
                    sideX = (dy * toEyeZ) - (toEyeY * dz);
                    sideY = (dz * toEyeX) - (toEyeZ * dx);
                    sideZ = (dx * toEyeY) - (toEyeX * dy);
                    lenSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
                    if (lenSq == 0.0f) {
                        sideX = 0.0f;
                        sideY = 0.0f;
                        sideZ = 0.0f;
                    } else {
                        dz = effect->halfWidth / sqrtf(lenSq);
                        sideX *= dz;
                        sideY *= dz;
                        sideZ *= dz;
                    }
                    vtx->v.ob[0] = b.x + sideX;
                    vtx->v.ob[1] = b.y + sideY;
                    vtx->v.ob[2] = b.z + sideZ;
                    vtx->v.tc[0] = 0;
                    vtx->v.tc[1] = texT;
                    vtx->v.cn[0] = 255;
                    vtx->v.cn[1] = 100;
                    vtx->v.cn[2] = 100;
                    vtx->v.cn[3] = 255;
                    vtx++;
                    vtx->v.ob[0] = b.x - sideX;
                    vtx->v.ob[1] = b.y - sideY;
                    vtx->v.ob[2] = b.z - sideZ;
                    vtx->v.tc[0] = 0x3C0;
                    vtx->v.tc[1] = texT;
                    vtx->v.cn[0] = 255;
                    vtx->v.cn[1] = 100;
                    vtx->v.cn[2] = 100;
                    vtx->v.cn[3] = 255;
                    vtx++;
                    texT += effect->texStep;
                    gSPVertex(gfx++, vtx - 4, 4, 0);
                    gSP1Triangle(gfx++, 0, 1, 2, 0);
                    gSP1Triangle(gfx++, 1, 3, 2, 0);
                }
            }
        }
    }
    return gfx;
}

extern void (*D_8008FB68[])(HoltenRopeEffect *, s32, u8);

/* Calls the rope type's handler from D_8008FB68, if it has one. */
void func_151B3A34(HoltenRopeEffect *effect, s32 arg1, u8 arg2) {
    void (*handler)(HoltenRopeEffect *, s32, u8) = D_8008FB68[effect->type];
    if (handler != NULL) {
        handler(effect, arg1, arg2);
    }
}

/* Update callback: lays the points out in a straight line from start to end, at rest. */
s32 func_151B3A7C(HoltenRopeEffect *effect) {
    s32 pad[4]; // matching: stack slots
    Vec3f_1E0560 pos;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 i;

    dx = effect->end.x - effect->start.x;
    dy = effect->end.y - effect->start.y;
    dz = effect->end.z - effect->start.z;
    pos = effect->start;
    for (i = 0; i < 10; i++) {
        HoltenRopePoint *point = &effect->points[i];
        point->pos = pos;
        point->vel.x = 0.0f;
        point->vel.y = 0.0f;
        point->vel.z = 0.0f;
        pos.x += dx * (1.0f / 9.0f);
        pos.y += dy * (1.0f / 9.0f);
        pos.z += dz * (1.0f / 9.0f);
    }
    effect->flags &= ~2;
    return TRUE;
}

/*
 * Update callback: lays the points on a half circle whose diameter is the start-end segment,
 * in the vertical plane through both ends.
 */
s32 func_151B3CF0(HoltenRopeEffect *effect) {
    Vec3f_1E0560 delta;
    Vec3f_1E0560 mid;
    Vec3f_1E0560 relStart;
    Vec3f_1E0560 relEnd;
    f32 invDist;
    f32 dirZ;
    f32 dirX;
    f32 across;
    f32 up;
    f32 heading;
    f32 angle;
    f32 radius;
    s32 i;

    delta.x = effect->end.x - effect->start.x;
    delta.y = effect->end.y - effect->start.y;
    delta.z = effect->end.z - effect->start.z;
    if ((0.0001f < fabsf(delta.x)) || (0.0001f < fabsf(delta.z))) {
        mid.x = effect->start.x + (delta.x * 0.5f);
        mid.y = effect->start.y + (delta.y * 0.5f);
        mid.z = effect->start.z + (delta.z * 0.5f);
        relStart.x = effect->start.x - mid.x;
        relStart.y = effect->start.y - mid.y;
        relStart.z = effect->start.z - mid.z;
        relEnd.x = effect->end.x - mid.x;
        relEnd.y = effect->end.y - mid.y;
        relEnd.z = effect->end.z - mid.z;
        invDist = 1.0f / sqrtf((delta.x * delta.x) + (delta.z * delta.z));
        dirZ = delta.z * invDist;
        dirX = delta.x * invDist;
        across = ((relEnd.z * dirZ) + (relEnd.x * dirX)) - ((relStart.z * dirZ) + (relStart.x * dirX));
        up = relEnd.y - relStart.y;
        heading = func_150484A0(up, across);
        radius = sqrtf((across * across) + (up * up)) * 0.5f;
        angle = heading - 3.1415927f;
        for (i = 0; i < 10; i++, angle += 0.34906587f) { // 20 degrees
            HoltenRopePoint *point = &effect->points[i];
            f32 s = sinf(angle);
            f32 c = cosf(angle);

            point->pos.x = (radius * c * dirX) + mid.x;
            point->pos.y = (radius * s) + mid.y;
            point->pos.z = (radius * c * dirZ) + mid.z;
        }
    }
    return TRUE;
}

/*
 * Endpoint callback for a rope hung from an actor: the start follows the actor
 * while it is still the one recorded at creation, the end hangs at a fixed point.
 */
s32 func_151B3F28(HoltenRopeAttached *rope, Vec3f_1E0560 *pos, u8 isStart) {
    s32 ret;

    ret = TRUE;
    if (isStart) {
        HoltenRopeAnchor *anchor = &rope->anchor;

        if (anchor->target->unk0 != 0 && anchor->target->unk3B == anchor->targetId) {
            pos->x = anchor->target->pos.x;
            pos->y = anchor->target->pos.y;
            pos->z = anchor->target->pos.z;
            rope->effect.flags &= ~4;
        } else {
            ret = FALSE;
            rope->effect.flags |= 0xC;
        }
    } else {
        pos->x = 0.0f;
        pos->y = 3000.0f;
        pos->z = 0.0f;
        rope->effect.flags &= ~8;
    }
    return ret;
}

s32 func_151B3FDC(HoltenRopeEffect *arg0) {
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f_1E0560 mid;
    Vec3f_1E0560 relStart;
    Vec3f_1E0560 relEnd;
    f32 len;
    f32 inv;
    f32 c;
    f32 s;
    f32 pad1;
    f32 u;
    f32 v;
    f32 pad2[7];
    f32 half;
    f32 dist;
    f32 d2;
    f32 ux;
    f32 uy;
    f32 w;
    f32 angle;
    f32 sn;
    f32 cs;
    f32 a;
    f32 b;
    f32 t;
    s32 i;
    HoltenRopePoint *node;

    dx = arg0->end.x - arg0->start.x;
    dy = arg0->end.y - arg0->start.y;
    dz = arg0->end.z - arg0->start.z;
    if ((fabsf(dx) > 9.999999747e-05f) || (fabsf(dz) > 9.999999747e-05f)) {
        f32 h;
        mid.x = arg0->start.x + (dx * 0.5f);
        mid.y = arg0->start.y + (dy * 0.5f);
        mid.z = arg0->start.z + (dz * 0.5f);
        relStart.x = arg0->start.x - mid.x;
        relStart.y = arg0->start.y - mid.y;
        relStart.z = arg0->start.z - mid.z;
        relEnd.x = arg0->end.x - mid.x;
        relEnd.y = arg0->end.y - mid.y;
        relEnd.z = arg0->end.z - mid.z;
        len = sqrtf((dx * dx) + (dz * dz));
        inv = 1.0f / len;
        c = dz * inv;
        s = dx * inv;
        u = ((relEnd.z * c) + (relEnd.x * s)) - ((relStart.z * c) + (relStart.x * s));
        v = relEnd.y - relStart.y;
        d2 = (u * u) + (v * v);
        dist = sqrtf(d2);
        inv = 1.0f / dist;
        half = dist * 0.5f;
        ux = u * inv;
        uy = v * inv;
        h = d2 * 0.25f;
        if (arg0->maxSpanSq < h) {
            return func_151B3A7C(arg0);
        }
        w = sqrtf(arg0->maxSpanSq - h);
        angle = 0.0f;
        node = arg0->points;
        for (i = 0; i < 10; i++, angle += 0.3490658700f) {
            HoltenRopePoint *n = &node[i];
            sn = sinf(angle);
            cs = cosf(angle);
            a = half * cs;
            b = w * sn;
            t = (a * ux) - (b * uy);
            n->pos.x = (t * s) + mid.x;
            n->pos.y = ((a * uy) - (b * ux)) + mid.y;
            n->pos.z = (t * c) + mid.z;
        }
    }
    return 1;
}

u8 func_151B42A4(HoltenRopeEffect *arg0) {
    u8 ret;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f_1E0560 mid;
    Vec3f_1E0560 relStart;
    Vec3f_1E0560 relEnd;
    f32 len;
    f32 inv;
    f32 pad3[5];
    f32 c;
    f32 s;
    f32 aZ;
    f32 ux;
    f32 uy;
    f32 u;
    f32 v;
    f32 half;
    f32 dist;
    f32 d2;
    f32 hpad;
    f32 pad4;
    f32 w;
    f32 angle2;
    f32 angleP;
    f32 sn;
    f32 cs;
    f32 sn2;
    f32 a;
    f32 b;
    f32 t;
    f32 k;
    Vec3f_1E0560 cur;
    Vec3f_1E0560 np;
    f32 dX;
    f32 dY;
    f32 dZ;
    f32 vx;
    f32 vy;
    f32 aX;
    f32 aY;
    s32 i;
    HoltenRopePoint *node;

    ret = 1;
    dx = -arg0->start.x + arg0->end.x;
    dy = -arg0->start.y + arg0->end.y;
    dz = arg0->end.z - arg0->start.z;
    if ((fabsf(dx) > 9.999999747e-05f) || (fabsf(dz) > 9.999999747e-05f)) {
        f32 h;
        mid.x = arg0->start.x + (dx * 0.5f);
        mid.y = arg0->start.y + (dy * 0.5f);
        mid.z = arg0->start.z + (dz * 0.5f);
        relStart.x = arg0->start.x - mid.x;
        relStart.y = arg0->start.y - mid.y;
        relStart.z = arg0->start.z - mid.z;
        relEnd.x = arg0->end.x - mid.x;
        relEnd.y = arg0->end.y - mid.y;
        relEnd.z = arg0->end.z - mid.z;
        len = sqrtf((dx * dx) + (dz * dz));
        inv = 1.0f / len;
        c = dz * inv;
        s = dx * inv;
        u = ((relEnd.z * c) + (relEnd.x * s)) - ((relStart.z * c) + (relStart.x * s));
        v = relEnd.y - relStart.y;
        d2 = (u * u) + (v * v);
        dist = sqrtf(d2);
        inv = 1.0f / dist;
        half = dist * 0.5f;
        ux = u * inv;
        uy = v * inv;
        h = d2 * 0.25f;
        if (arg0->maxSpanSq < h) {
            ret = func_151B3A7C(arg0);
        } else {
            f32 angle;
            w = sqrtf(arg0->maxSpanSq - h);
            node = arg0->points;
            for (i = 0, angle = 0.0f, angle2 = 0.0f; i < 10; i++, angle += 0.3490658700f, angle2 += 0.3490658700f) {
                HoltenRopePoint *n = &node[i];
                cur = n->pos;
                sn = sinf(angle);
                cs = cosf(angle);
                sn2 = sinf(angle2);
                a = half * cs;
                b = w * sn;
                t = (a * ux) - (b * uy);
                np.x = (t * s) + mid.x;
                np.y = ((a * uy) - (b * ux)) + mid.y;
                np.z = (t * c) + mid.z;
                if (arg0->flags & 2) {
                    n->pos = np;
                    n->vel.x = 0.0f;
                    n->vel.y = 0.0f;
                    n->vel.z = 0.0f;
                } else if (*(s32 *)D_800DBFF4 != 0) {
                    n->pos = np;
                    n->vel = *(Vec3f_1E0560 *)&D_800A5480;
                } else {
                    dX = (cur.x - np.x) * sn2 * arg0->stiffness;
                    dY = (cur.y - np.y) * sn2 * arg0->stiffness;
                    dZ = (cur.z - np.z) * sn2 * arg0->stiffness;
                    n->pos.x = np.x + dX;
                    n->pos.y = np.y + dY;
                    n->pos.z = np.z + dZ;
                    aX = arg0->gain * dX;
                    aY = arg0->gain * dY;
                    aZ = arg0->gain * dZ;
                    n->pos.x += (n->vel.x * D_800BE9A4) + (0.5f * aX * D_800BE9A4 * D_800BE9A4);
                    n->pos.y += (n->vel.y * D_800BE9A4) + (0.5f * aY * D_800BE9A4 * D_800BE9A4);
                    n->pos.z += (n->vel.z * D_800BE9A4) + (0.5f * aZ * D_800BE9A4 * D_800BE9A4);
                    n->vel.x = n->vel.x + (aX * D_800BE9A4);
                    n->vel.y = n->vel.y + (aY * D_800BE9A4);
                    n->vel.z = n->vel.z + (aZ * D_800BE9A4);
                }
            }
        }
    }
    arg0->flags &= ~2;
    return ret;
}

/* Endpoint data at 0x150 for ropes tied between two actors' bones. */
typedef struct {
    /* 0x00 */ HoltenRopeTarget *target;
    /* 0x04 */ u8 targetId; // target->unk3B when the rope was attached
    /* 0x05 */ u8 bone;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ Vec3f_1E0560 offset; // in the bone's space
} HoltenRopeBoneAnchor;

typedef struct {
    HoltenRopeBoneAnchor start;
    HoltenRopeBoneAnchor end;
} HoltenRopeBoneAnchors;

void func_15143134(Vec3f_1E0560 *in, Vec3f_1E0560 *out, Mtx *mtx);

/*
 * Places one end of a rope tied between two bones (called through func_151B2FA0). Returns 0 once
 * either actor is gone; while either has no bone matrices yet, both ends are marked lost.
 */
s32 func_151B47D8(HoltenRopeEffect *effect, HoltenRopeBoneAnchors *anchors, Vec3f_1E0560 *pos, u8 isStart) {
    HoltenRopeTarget *startTarget = anchors->start.target;
    HoltenRopeTarget *endTarget = anchors->end.target;

    if (startTarget->boneMtx == NULL || endTarget->boneMtx == NULL) {
        effect->flags |= 0xC;
        return TRUE;
    }
    if (startTarget->unk0 == 0 || startTarget->unk3B != anchors->start.targetId || endTarget->unk0 == 0 || endTarget->unk3B != anchors->end.targetId) {
        return FALSE;
    }
    effect->flags &= ~(isStart ? 4 : 8);
    func_15143134(isStart ? &anchors->start.offset : &anchors->end.offset, pos,
                  isStart ? &startTarget->boneMtx[anchors->start.bone] : &endTarget->boneMtx[anchors->end.bone]);
    return TRUE;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 velocity[3];
} Struct151B48DCPoint;

typedef struct {
    u8 pad0[0x10];
    u8 flags;
    u8 pad11;
    s16 timer;
    u8 primaryState[0xC];
    u8 secondaryState[0xC];
    s8 primaryCallback;
    s8 secondaryCallback;
    u8 pad2E[6];
    s8 updateCallback;
    u8 pad35[3];
    f32 radius;
    f32 gain;
    f32 stiffness;
    u8 pad44[4];
    Struct151B48DCPoint points[10];
} Struct151B48DC;

s32 func_151B48DC(Struct151B48DC *arg0) {
    f32 start;
    f32 end;
    f32 step;
    f32 x;
    s32 i;

    start = *(f32 *)arg0->primaryState;
    end = *(f32 *)arg0->secondaryState;
    step = -(start - end) * 0.1f;
    x = start;
    for (i = 0; i < 10; i++, x += step) {
        Struct151B48DCPoint *point = &arg0->points[i];

        point->x = x;
        point->y = 0.0f;
        point->z = 0.0f;
    }
    arg0->flags &= ~2;
    return 1;
}

s32 func_151B498C(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5,
                   s32 *arg6, s32 *arg7, s32 *arg8, s32 *arg9, s32 *arg10, s32 *arg11,
                   u8 *arg12, u8 *arg13) {
    *arg1 = 0x220005;
    *arg2 = 0x40600;
    *arg3 = 0xFF;
    *arg4 = 0xFF;
    *arg5 = 0xFF;
    *arg6 = 0xFF;
    *arg7 = 0xFF;
    *arg8 = 0xFF;
    *arg9 = 0xFF;
    *arg10 = 0xFF;
    *arg11 = 0;
    *arg12 = 5;
    *arg13 = 0x2B;
    return 1;
}

typedef struct {
    u8 pad0[0xA4];
    u8 unkA4;
} Model151B4A14;

typedef struct {
    u8 pad0[0x150];
    Model151B4A14 *unk150;
} Obj151B4A14;

extern void func_1502EC34(void *, u32 *, u32 *, u32 *, u32 *);

u8 func_151B4A14(Obj151B4A14 *arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5,
                 s32 *arg6, s32 *arg7, s32 *arg8, s32 *arg9, s32 *arg10, s32 *arg11,
                 u8 *arg12, u8 *arg13) {
    u8 ret;
    s32 mode1;
    s32 mode2;
    Model151B4A14 *model;
    u32 r;
    u32 g;
    u32 b;
    u32 a;
    s32 full;
    u32 alpha;

    ret = 1;
    full = 0xFF;
    mode1 = 0x200005;
    mode2 = 0x60600;
    model = arg0->unk150;
    func_1502EC34(model, &r, &g, &b, &a);
    if (model->unkA4 & 1) {
        *arg1 = mode1;
        *arg2 = mode2;
        *arg3 = r;
        *arg4 = g;
        *arg5 = b;
        *arg6 = full;
        *arg9 = alpha = a;
        *arg8 = alpha;
        *arg7 = alpha;
        *arg10 = full;
        *arg11 = 0x100000;
        *arg12 = 5;
        *arg13 = 0x2F;
    } else {
        ret = func_151B498C(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
    }
    return ret;

}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 velocity[3];
} HoltenRopePoint_151B4B78;

typedef struct {
    u8 pad0[0x10];
    u8 flags;
    u8 pad11[0x37];
    HoltenRopePoint_151B4B78 points[10];
} HoltenRopeEffect_151B4B78;

s32 func_151B4B78(HoltenRopeEffect_151B4B78 *arg0) {
    f32 x;
    s32 i;

    x = -1000.0f;
    for (i = 0; i < 10; i++, x += 222.22223f) {
        HoltenRopePoint_151B4B78 *point = &arg0->points[i];

        point->x = x;
        point->y = 0.0f;
        point->z = 0.0f;
    }
    arg0->flags &= ~2;
    return 1;
}

void func_151B4C1C(u8 *arg0) {
    void (*fn)(void *);

    func_151D5E30(arg0 + 0x140);
    fn = D_8008FB70[arg0[0x44]];
    if (fn != NULL) {
        fn(arg0);
    }
}

void func_151B4C1C(u8 *);

void func_151B4C6C(struct102 *arg0) {
    func_151B4C1C((u8 *)arg0);
    func_15169824(arg0);
}

void func_151B4C98(struct102 *arg0) {
    func_151B4C1C((u8 *)arg0);
    func_15169824(arg0);
}
