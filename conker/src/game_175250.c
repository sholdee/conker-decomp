#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Spawn description passed to func_15147DA0 (first argument). */
typedef struct {
    Vec3f pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 pad14;
    u8 unk15;
} TrailSpawn;

/* One point of a trail; the points form a ring buffer (0x14 bytes each). */
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
    u8 texture; /* index into D_80091564 */
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    s16 unk1E;
} TrailMotion; /* size 0x20 */

typedef struct {
    TrailMotion motion;
    u8 state;      /* 0x20: handler index into D_8008A3E0 */
    u8 unk21;      /* handler index into D_8008A3F8 */
    u8 finishFn;   /* handler index into D_8008A42C */
    u8 hitFn;      /* handler index into D_8008A430 */
    u8 hitKind3Fn; /* handler index into D_8008A450 */
    u8 unk25;
    u8 pad26[2];
    TrailRender render; /* 0x28 */
} TrailParams; /* size 0x48 */

/* Result block filled by func_15046C80. */
typedef struct {
    f32 height;
    u8 data[0x19];
    u8 kind;
    u8 pad1E[6];
} TrailCollision; /* size 0x24 */

typedef struct {
    u8 pad0[0x10];
    Vec3f position;
    s16 unk1C;
    u8 pad1E[7];
    u8 capacity; /* 0x25: number of points in the ring buffer */
    u8 pad26[6];
    s8 count;    /* 0x2C */
    s8 start;    /* 0x2D: oldest point */
    s8 end;      /* 0x2E: one past the newest point */
    u8 pad2F[0x25];
    Vec3f unk54;
    TrailCollision collision; /* 0x60 */
    u8 buffers[0x10];         /* 0x84 */
    TrailPoint *points;       /* 0x94 */
    TrailParams *params;      /* 0x98 */
} Trail;

extern Trail *func_15147A80(TrailSpawn *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Creates a trail object from a spawn description, motion block and render settings. */
Trail *func_15147DA0(TrailSpawn *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4, s32 finishFn,
                     s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10, TrailRender *render,
                     s32 arg12, u8 arg13, s32 arg14) {
    Trail *trail;
    TrailParams *params;

    spawn->unk10 = 1;
    trail = func_15147A80(spawn, extraSize + sizeof(TrailParams), sizeof(TrailPoint), 1, 0, 1, arg9, arg10,
                          arg12, arg13, arg14);
    if (trail == NULL) {
        return NULL;
    }
    params = trail->params;
    memcpy(&params->motion, motion, sizeof(TrailMotion));
    params->state = state;
    params->unk21 = arg4;
    params->finishFn = finishFn;
    params->hitFn = hitFn;
    params->hitKind3Fn = hitKind3Fn;
    params->unk25 = arg8;
    params->render = *render;
    return trail;
}

typedef s32 (*TrailFunc)();
extern TrailFunc D_8008A3E0[];
extern TrailFunc D_8008A3F8[];
extern TrailFunc D_8008A42C[];

/* Runs the trail's update handlers and fades; returns whether it is still alive. */
s8 func_15147EB8(Trail *trail) {
    TrailParams *params;
    s8 done;
    TrailPoint *points;
    s16 unk1C;
    s32 fade;
    s32 ret;

    params = trail->params;
    done = 0;
    /* The two update handlers are called without arguments. */
    if (params->state != 0) {
        if (D_8008A3E0[params->state]() == 0) {
            done = 1;
        }
    }
    if (params->unk21 != 0 && !done) {
        if (D_8008A3F8[params->unk21]() == 0) {
            done = 1;
        }
    }
    if ((params->motion.flags & 0x40) && !done) {
        unk1C = trail->unk1C;
        if (unk1C < params->motion.unk1C) {
            fade = unk1C * params->motion.unk1E;
            if (fade < params->motion.unk1B) {
                params->motion.unk1B = fade;
            }
        }
    }
    ret = !done;
    if (done) {
        if (params->finishFn != 0) {
            done = D_8008A42C[params->finishFn](trail); // matching: dead store of the result
        }
    }
    points = trail->points;
    if (trail->count > 0) {
        trail->unk54 = points[trail->start].pos;
    } else {
        trail->unk54.x = 0.0f;
        trail->unk54.y = 0.0f;
        trail->unk54.z = 0.0f;
    }
    return ret;
}

void func_151D5D60(void *, s16, s32, void **, u8 *);
extern Vec3f *func_15144B34(s32);
s32 func_1514306C(s32, s32, s32, s32);
extern Gfx *func_150950D4(Gfx *, void *, s32, s32, s32, s32, s32, s32, s32, s32);
void func_151441A4(s16 *, s16 *, s16 *, s16 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_151442FC(s16 *, s16 *, s16 *, s16 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern s32 D_80091564[];
extern s32 D_800915A4;
extern s32 D_800DD1B0;
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/* Draws the trail as a camera-facing ribbon of quads, newest point first. */
Gfx *func_1514803C(Trail *trail, Gfx *gfx, s16 arg2) {
    TrailParams *params;
    Vtx *vtx;
    u8 update;
    Vec3f next;
    Vec3f cur;
    s32 i;
    s32 j;
    Vec3f *cam;
    TrailPoint *points;
    f32 toCamX;
    f32 toCamY;
    f32 toCamZ;
    f32 segX;
    f32 segY;
    f32 segZ;
    f32 sideX;
    f32 sideY;
    f32 sideZ;
    f32 len;
    s16 envR;
    s16 envG;
    s16 envB;
    s16 envA;
    s16 primR;
    s16 primG;
    s16 primB;
    s16 primA;
    s32 tex;

    params = trail->params;
    if (trail->count >= 2) {
        func_151D5D60(trail->buffers, arg2, (trail->capacity - 1) * 2 * sizeof(Vtx), (void **)&vtx, 0);
        if (vtx == NULL) {
            return gfx;
        }
        if (vtx != NULL) { // matching: redundant re-test of vtx
            if (D_800BE9C0 != 0) {
                vtx += (trail->capacity - 1) * 2;
            }
            update = 1;
            cam = func_15144B34(arg2);
            points = trail->points;
            tex = func_1514306C(0, params->motion.texture, 0, 2);
            if (tex != D_800DD1B0) {
                D_800915A4 = D_80091564[params->motion.texture];
                gfx = func_150950D4(gfx, &D_800915A4, 0, 0, 0, 0, 2, 0x100, 0x100, (params->motion.flags & 0x80) ? 0x3E : 3);
                D_800DD1B0 = tex;
            }
            func_151441A4(&envR, &envG, &envB, &envA, 0, 0, 0, 0, 0, 0, 0, params->motion.unk1B, params->motion.unk1A, params->render.primMode);
            func_151442FC(&primR, &primG, &primB, &primA, 0, 0, 0, 0, 0, 0, 0, params->motion.unk1B, params->motion.unk1A, params->render.envMode);
            gfx = func_15142B7C(gfx, params->render.setGeometryMode, params->render.clearGeometryMode);
            gfx = func_15142C10(gfx, primR, primG, primB, primA, &update);
            gfx = func_15142CF0(gfx, 0, 0, envR, envG, envB, envA, &update);
            gfx = func_1513F4E4(gfx, ((u8 *)&params->render.unk10)[3] /* matching: reads the low byte */, &update);
            gfx = func_15142FBC(gfx, params->render.unk0 | G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | params->render.unk14 | params->render.unk18, D_800A4AC8[params->render.renderMode].unk0 | D_800A4AC8[params->render.renderMode].unk4, &update);
            i = trail->end - 1;
            if (i < 0) {
                i = trail->capacity - 1;
            }
            j = i - 1;
            if (j < 0) {
                j = trail->capacity - 1;
            }
            cur = points[i].pos;
            next = points[j].pos;
            segX = cur.x - next.x;
            segY = cur.y - next.y;
            segZ = cur.z - next.z;
            toCamX = cur.x - cam->x;
            toCamY = cur.y - cam->y;
            toCamZ = cur.z - cam->z;
            sideX = (segY * toCamZ) - (toCamY * segZ);
            sideY = (segZ * toCamX) - (toCamZ * segX);
            sideZ = (segX * toCamY) - (toCamX * segY);
            len = sqrtf((sideX * sideX) + (sideY * sideY) + (sideZ * sideZ));
            if (len == 0.0f) {
                sideX = 0.0f;
                sideY = 0.0f;
                sideZ = 0.0f;
            } else {
                f32 scale;
                scale = params->motion.halfWidth / len;
                sideX *= scale;
                sideY *= scale;
                sideZ *= scale;
            }
            vtx->v.ob[0] = cur.x + sideX;
            vtx->v.ob[1] = cur.y + sideY;
            vtx->v.ob[2] = cur.z + sideZ;
            vtx->v.tc[1] = points[i].phase + 0x4000;
            vtx->v.tc[0] = 0x43C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0xFF;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - sideX;
            vtx->v.ob[1] = cur.y - sideY;
            vtx->v.ob[2] = cur.z - sideZ;
            vtx->v.tc[1] = points[i].phase + 0x4000;
            vtx->v.tc[0] = 0x4000;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0xFF;
            vtx->v.flag = 0;
            vtx++;
            do {
                segX = cur.x - next.x;
                segY = cur.y - next.y;
                segZ = cur.z - next.z;
                toCamX = next.x - cam->x;
                toCamY = next.y - cam->y;
                toCamZ = next.z - cam->z;
                sideX = (segY * toCamZ) - (toCamY * segZ);
                sideY = (segZ * toCamX) - (toCamZ * segX);
                sideZ = (segX * toCamY) - (toCamX * segY);
                len = sqrtf((sideX * sideX) + (sideY * sideY) + (sideZ * sideZ));
                if (len == 0.0f) {
                    sideX = 0.0f;
                    sideY = 0.0f;
                    sideZ = 0.0f;
                } else {
                    f32 scale;
                    scale = params->motion.halfWidth / len;
                    sideX *= scale;
                    sideY *= scale;
                    sideZ *= scale;
                }
                vtx->v.ob[0] = next.x + sideX;
                vtx->v.ob[1] = next.y + sideY;
                vtx->v.ob[2] = next.z + sideZ;
                vtx->v.tc[1] = points[j].phase + 0x4000;
                vtx->v.tc[0] = 0x43C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0xFF;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = next.x - sideX;
                vtx->v.ob[1] = next.y - sideY;
                vtx->v.ob[2] = next.z - sideZ;
                vtx->v.tc[1] = points[j].phase + 0x4000;
                vtx->v.tc[0] = 0x4000;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0xFF;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                i = j;
                j--;
                if (j < 0) {
                    j = trail->capacity - 1;
                }
                cur = points[i].pos;
                next = points[j].pos;
            } while (i != trail->start);
        }
    }
    return gfx;
}

/* Moves every point by the trail's velocity and gravity, then appends the current position. */
s32 func_151488C4(Trail *trail) {
    TrailPoint *points;
    TrailParams *params;
    s32 phase;
    struct {
        u16 step;
    } interval; // matching: a struct puts step at offset 0 of its stack slot (a bare u16 sits at 2)
    s32 index;
    s32 count;
    s32 phaseIndex;

    points = trail->points;
    params = trail->params;
    index = trail->start;
    while (index != trail->end) {
        points[index].velocityY -= params->motion.gravity * D_800BE9A4;
        points[index].pos.x += params->motion.velocity.x * D_800BE9A4;
        points[index].pos.y += points[index].velocityY * D_800BE9A4;
        points[index].pos.z += params->motion.velocity.z * D_800BE9A4;
        index++;
        if (index == trail->capacity) {
            index = 0;
        }
    }
    count = trail->count;
    if (count < trail->capacity - 1) {
        phase = (params->motion.flags & 0x20) ? 0x1000 : 0;
        if (count != 0) {
            interval.step = 0x1000 / count;
        }
        trail->count = count + 1;
        points[trail->end].pos = trail->position;
        points[trail->end].velocityY = params->motion.velocity.y;
        trail->end++;
        if (trail->end == trail->capacity) {
            trail->end = 0;
        }
        phaseIndex = trail->start;
        while (phaseIndex != trail->end) {
            points[phaseIndex].phase = phase;
            if (params->motion.flags & 0x20) {
                phase = (phase - interval.step) & 0xFFFF;
            } else {
                phase += interval.step;
                phase &= 0xFFFF;
            }
            phaseIndex++;
            if (phaseIndex == trail->capacity) {
                phaseIndex = 0;
            }
        }
    } else if (params->motion.flags & 0x17) {
        params->state = 3;
    } else {
        params->state = 2;
    }
    return 1;
}

/* Moves every point by the trail's velocity and gravity, newest to oldest. */
s32 func_15148AF4(Trail *trail) {
    s32 idx;
    TrailPoint *points;
    TrailParams *params;

    points = trail->points;
    params = trail->params;
    idx = trail->end;
    do {
        idx--;
        if (idx < 0) {
            idx = trail->capacity - 1;
        }
        points[idx].velocityY -= params->motion.gravity * D_800BE9A4;
        points[idx].pos.x += params->motion.velocity.x * D_800BE9A4;
        points[idx].pos.y += points[idx].velocityY * D_800BE9A4;
        points[idx].pos.z += params->motion.velocity.z * D_800BE9A4;
    } while (idx != trail->start);
    return 1;
}

typedef s32 (*TrailHitFunc)(Trail *, f32, f32, f32, f32, void *);
extern TrailHitFunc D_8008A430[];
extern TrailHitFunc D_8008A450[];
s32 func_15046C80(f32 *, u16, f32, TrailCollision *);

/* Like func_15148AF4, then tests the oldest point's fall against the ground and runs a hit handler. */
s32 func_15148BA4(Trail *trail) {
    s32 index;
    TrailParams *params;
    TrailPoint *points;
    Vec3f oldPosition;
    Vec3f position;
    u8 callback;

    params = trail->params;
    points = trail->points;
    index = trail->end;
    if (params->motion.flags & 7) {
        oldPosition = points[trail->start].pos;
    }
    do {
        index--;
        if (index < 0) {
            index = trail->capacity - 1;
        }
        points[index].velocityY -= params->motion.gravity * D_800BE9A4;
        points[index].pos.x += params->motion.velocity.x * D_800BE9A4;
        points[index].pos.y += points[index].velocityY * D_800BE9A4;
        points[index].pos.z += params->motion.velocity.z * D_800BE9A4;
    } while (index != trail->start);
    if (params->motion.flags & 0x17) {
        if (points[trail->start].pos.y < oldPosition.y) {
            position.x = points[trail->start].pos.x;
            position.y = oldPosition.y;
            position.z = points[trail->start].pos.z;
            if (func_15046C80(&position.x, 0, points[trail->start].pos.y,
                              &trail->collision) != 0) {
                if (trail->collision.kind == 3) {
                    callback = params->hitKind3Fn;
                    if (callback != 0 &&
                        D_8008A450[callback](trail, oldPosition.x, oldPosition.y,
                                             oldPosition.z, trail->collision.height,
                                             trail->collision.data) == 0) {
                        return 0;
                    }
                } else {
                    callback = params->hitFn;
                    if (callback != 0 &&
                        D_8008A430[callback](trail, oldPosition.x, oldPosition.y,
                                             oldPosition.z, trail->collision.height,
                                             trail->collision.data) == 0) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

/* Drops the newest point and respaces the remaining points' phases; fails below three points. */
s32 func_15148DE0(Trail *trail) {
    s32 idx;
    u16 step;
    u16 phase;
    TrailParams *params;
    TrailPoint *points;

    if (trail->count >= 3) {
        trail->count--;
        points = trail->points;
        params = trail->params;
        idx = trail->start;
        step = 0x1000 / trail->count;
        phase = (params->motion.flags & 0x20) ? 0x1000 : 0;
        trail->end--;
        if (trail->end < 0) {
            trail->end = trail->capacity - 1;
        }
        while (idx != trail->end) {
            points[idx].phase = phase;
            if (params->motion.flags & 0x20) {
                phase -= step;
            } else {
                phase += step;
            }
            idx++;
            if (idx == trail->capacity) {
                idx = 0;
            }
        }
        return 1;
    }
    return 0;
}

/* Handler: switches the trail to state 4. */
s32 func_15148EF8(Trail *trail, s32 arg1, s32 arg2, s32 arg3) {
    TrailParams *params = trail->params;

    params->state = 4;
    return 1;
}

/* Spawns a trail at (x, y, z), launched at speed along the direction given by the yaw and pitch angles. */
void func_15148F1C(u8 texture, f32 x, f32 y, f32 z, s32 arg4, s32 yaw, s32 pitch, f32 speed, s16 arg8,
                   f32 halfWidth, f32 gravity, u8 arg11) {
    f32 yawWaveQ;
    TrailSpawn spawn;
    f32 horizontal;
    struct {
        f32 halfWidth;
        Vec3f velocity;
        f32 gravity;
        u8 pad14[4];
        u8 flags;
        u8 texture;
        u8 unk1A;
        u8 unk1B;
    } motion; /* TrailMotion without unk1C/unk1E (func_15147DA0 copies past its end) */
    f32 pitchWave;
    f32 pitchWaveQ;
    f32 yawWave;
    u8 tex;
    TrailRender render;

    /* func_151423D8 is a 256-step periodic lookup; "Q" values are a quarter turn (0x40) back. */
    pitchWave = func_151423D8((u8)pitch);
    pitchWaveQ = func_151423D8((u8)(pitch - 0x40));
    yawWave = func_151423D8((u8)yaw);
    yawWaveQ = func_151423D8((u8)(yaw - 0x40));

    spawn.unk10 = 1;
    spawn.unkE = 1;
    motion.unk1A = 0xFF;
    motion.flags = 8;
    tex = texture; // matching: the u8 copy owns the stack slot above render
    motion.texture = tex;
    if (tex == 10) {
        motion.flags = 0x28;
    }

    horizontal = speed * pitchWave;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    motion.halfWidth = halfWidth;
    motion.gravity = gravity;
    motion.velocity.x = horizontal * yawWaveQ;
    motion.velocity.y = -speed * pitchWaveQ;
    motion.velocity.z = horizontal * yawWave;
    motion.unk1B = 0xFF;
    spawn.unk15 = arg4 + 3;
    spawn.unkC = arg8;

    render.unk0 = 0;
    render.setGeometryMode = 1;
    render.clearGeometryMode = 0x160600;
    render.renderMode = 3;
    render.unk10 = 0x10;
    render.unk14 = 0x80;
    render.unk18 = 0x20;
    render.primMode = 0;
    render.envMode = 9;
    func_15147DA0(&spawn, (TrailMotion *)&motion, 0, 1, 0, 0, 0, 0, 0, 0, 0, &render, 0, arg11, 1);
}

/* Fades the trail in: alpha = 8 per unit of unk1C, capped at 255. */
s32 func_151490C8(Trail *trail) {
    TrailParams *params = trail->params;
    s32 alpha = trail->unk1C * 8;

    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    params->motion.unk1B = alpha;
    if ((u8)alpha < 0) { // matching: never true, but IDO still emits the test
        return 0;
    }
    return 1;
}

void func_151478F4(void);

void func_15149104(void) {
    func_151478F4();
}
