#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* State block of a trail effect (trail_spawnFollowTrail). */
typedef struct TargetTrailState {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetUnk3B; /* target->unique_id when the trail was spawned */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ Vec3f lastPos;
    /* 0x14 */ f32 pending; /* points still to emit */
    /* 0x18 */ f32 elapsed; /* time since the last point was emitted */
    /* 0x1C */ f32 targetTexS; /* texture s at the target's position */
    /* 0x20 */ f32 texS; /* texture s at lastPos */
    /* 0x24 */ f32 distance; /* distance travelled at lastPos */
    /* 0x28 */ s32 unk28;
} TargetTrailState;

/* One point of a trail effect. */
typedef struct TargetTrailPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 life;
    /* 0x10 */ u8 alpha;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 texS;
    /* 0x18 */ f32 distance; /* distance travelled when emitted */
} TargetTrailPoint;

/* Spawns a trail effect that follows target. */
void trail_spawnFollowTrail(Actor *target, u8 arg1, s32 arg2) {
    EffectTemplate spawn;
    s32 pad_dummy; // matching: stack pad
    TargetTrailState state;
    EffectObj *obj;

    spawn.itemCount = 10;
    spawn.pos.x = target->xpos;
    spawn.pos.y = target->ypos;
    spawn.pos.z = target->zpos;
    spawn.life = 300;
    spawn.flags = 6;
    state.target = target;
    state.targetUnk3B = target->unique_id;
    state.lastPos = spawn.pos;
    state.pending = 0.0f;
    state.elapsed = 0.0f;
    state.texS = -16384.0f;
    state.targetTexS = -16384.0f;
    state.distance = 0.0f;
    spawn.kind = 13;

    obj = func_15147A80(&spawn, 0x30, sizeof(TargetTrailPoint), 11, 11, 11, 0, 0, 0, arg1, arg2);
    if (obj != NULL) {
        memcpy(obj->userData, &state, sizeof(state));
    }
}

/* Ages a trail's points, drops expired ones and sets the alpha of the rest. Returns 0 once the trail has finished. */
s32 trail_ageFollowTrail(EffectObj *obj) {
    TargetTrailPoint *points;
    s32 i;
    f32 base;
    s32 alpha;
    f32 d;

    points = obj->items;
    if (obj->head.count < 2 && (obj->head.tmpl.flags & 8)) {
        return 0;
    }
    i = obj->head.end;
    while (i != obj->head.start) {
        i--;
        if (i < 0) {
            i = obj->head.tmpl.itemCount - 1;
        }
        (i + points)->life -= gFrameDelta; // matching: IDO orders the addu operands as written
        if ((i + points)->life < 0.0f) {
            while (i != obj->head.start) {
                obj->head.start++;
                if (obj->head.start == obj->head.tmpl.itemCount) {
                    obj->head.start = 0;
                }
                obj->head.count--;
            }
        }
    }
    if (obj->head.count > 0) {
        i = obj->head.start;
        base = points[obj->head.start].distance;
        do {
            d = (i + points)->distance - base; // matching: IDO orders the addu operands as written
            alpha = d * 0.4940000176f;
            if (alpha >= 156) {
                (i + points)->alpha = 155;
            } else {
                (i + points)->alpha = alpha;
            }
            i++;
            if (i >= obj->head.tmpl.itemCount) {
                i = 0;
            }
        } while (i != obj->head.end);
    }
    if (obj->head.count > 0) {
        obj->head.modelPos = points[obj->head.start].pos;
    } else {
        obj->head.modelPos.x = 0.0f;
        obj->head.modelPos.y = 0.0f;
        obj->head.modelPos.z = 0.0f;
    }
    return 1;
}

/* Follows the trail's target and emits points along the path it moved. Stops the trail once the target is gone. */
s32 trail_followTarget(EffectObj *obj) {
    TargetTrailState *state;
    TargetTrailPoint *points;
    Actor *target;
    Vec3f delta;
    s32 pad0; // matching: stack pad
    s32 pad1; // matching: stack pad
    f32 dist;

    state = obj->userData;
    points = obj->items;
    target = state->target;
    if (target->type == 0 || target->unique_id != state->targetUnk3B || target->speed < 15.0f) {
        obj->head.updateKind2 = 0;
        obj->head.tmpl.flags |= 8;
        return 1;
    }
    obj->head.tmpl.pos.x = target->xpos;
    obj->head.tmpl.pos.y = target->ypos;
    obj->head.tmpl.pos.z = target->zpos;
    delta.x = obj->head.tmpl.pos.x - state->lastPos.x;
    delta.y = obj->head.tmpl.pos.y - state->lastPos.y;
    delta.z = obj->head.tmpl.pos.z - state->lastPos.z;
    dist = func_15143E64(&delta);
    state->pending += dist * 0.003721999936f * gFrameDelta;
    state->targetTexS += dist * 4.409547806f;
    if (state->pending > 1.0f) {
        TargetTrailPoint *point;
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
        elapsed = state->elapsed + gFrameDelta;
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
            point = &points[obj->head.end];
            point->pos = cur;
            point->alpha = 155;
            point->texS = texS;
            point->life = 13.0f - elapsed;
            elapsed += elapsedStep;
            while (point->texS > 16384.0f) {
                point->texS -= 32768.0f;
            }
            point->distance = distance;
            obj->head.end++;
            if (obj->head.end == obj->head.tmpl.itemCount) {
                obj->head.end = 0;
            }
            obj->head.count++;
            if (obj->head.end == obj->head.start) {
                obj->head.start++;
                if (obj->head.start == obj->head.tmpl.itemCount) {
                    obj->head.start = 0;
                }
                obj->head.count--;
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

extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/* Draws the trail as a camera-facing strip 20 units wide, from the newest point to the oldest.
 * While flags & 2 is set the strip starts at the effect position itself. */
Gfx *trail_drawFollowTrail(EffectObj *obj, Gfx *gfx, s16 view) {
    Vtx *vtx;
    TargetTrailPoint *points;
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

    if (obj->head.count >= 2) {
        func_151D5D60(obj->buffers, view, (obj->head.tmpl.itemCount + 5) * 2 * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            points = obj->items;
            camera = func_15144B34(view);
            flag = 1;
            gfx = func_15142E24(gfx, &gEffectTextures[31], 0, 0, 0, 0, 0x1F, 0, 0, &flag, 3);
            gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &flag);
            gfx = func_1513F4E4(gfx, 0x4E, &flag);
            gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
            gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &flag);
            if (obj->head.tmpl.flags & 2) {
                index = obj->head.end - 1;
                if (index < 0) {
                    index = obj->head.tmpl.itemCount - 1;
                }
                cur = obj->head.tmpl.pos;
                curAlpha = 0x80;
                curTexS = 0.0f;
            } else {
                prev = obj->head.end - 1;
                if (prev < 0) {
                    prev = obj->head.tmpl.itemCount - 1;
                }
                index = prev - 1;
                if (index < 0) {
                    index = obj->head.tmpl.itemCount - 1;
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
                    index = obj->head.tmpl.itemCount - 1;
                }
                if (prev != obj->head.start) {
                    cur = next;
                    next = points[index].pos;
                    nextAlpha = points[index].alpha;
                    nextTexS = points[index].texS;
                }
            } while (prev != obj->head.start);
        }
    }
    return gfx;
}

/* Message 0x2D: objA and objB swap identities. */
typedef struct SwapMessage {
    /* 0x00 */ Actor *objA;
    /* 0x04 */ Actor *objB;
    /* 0x08 */ u8 objAUnk3B;
    /* 0x09 */ u8 objBUnk3B;
} SwapMessage;

/* Message handler of a trail: message 0 (msg is a TrailTargetRef) stops the trail when it names its target;
 * message 0x2D (a SwapMessage) moves the trail to the other object of the swap. */
void trail_onFollowTrailMessage(EffectObj *obj, void *msg, u8 msgType) {
    TargetTrailState *state;
    Actor *current;
    Actor *objA;

    state = obj->userData;
    current = state->target;
    if (msgType == 0) {
        if (current == ((TrailTargetRef *)msg)->target) {
            u16 *flags = &obj->head.tmpl.flags; // matching: keeps the store out of the jr delay slot

            obj->head.updateKind2 = 0;
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

/* Spawns a particle at target's position. */
void trail_spawnParticleAtTarget(Actor *target, u8 arg1, s32 arg2) {
    ParticleDesc spawn;
    s32 flag80;
    s32 flag40;

    spawn.texture = 0x29;
    spawn.renderMode = 0xE03;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.fadeStart = 18;
    spawn.fadeRate = 14;
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
    spawn.primColorMode = 3;
    spawn.envColorMode = 3;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 0;
    spawn.scaleStart = 40;
    spawn.r = 0xDD;
    spawn.g = 0xD3;
    spawn.b = 0xCD;
    spawn.a = 0xFF;
    spawn.r2 = 0x57;
    spawn.g2 = 0x55;
    spawn.b2 = 0x5A;
    spawn.brightness = 0xFF;
    spawn.gravity = 0.0f;
    spawn.scaleRate = 1.05674696f;
    spawn.pos.x = target->xpos;
    spawn.pos.y = target->ypos;
    spawn.pos.z = target->zpos;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    spawn.alpha = func_150ADA20() % 56U + 200;
    spawn.timer = func_150ADA20() % 10U + 30;
    spawn.width = spawn.height = func_150ADA68() * 50.0f + 89.0f;
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    func_15130280(&spawn, 1, 0, 0, arg1, arg2);
}

/* Particle a ribbon effect drags along (see trail_moveRibbon). */
typedef struct LinkedParticle {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ Vec3f pos;
} LinkedParticle;

/* State block of a ribbon effect (trail_spawnRibbon). A copy of its arguments follows at +0x10. */
typedef struct RibbonState {
    /* 0x00 */ LinkedParticle *linked;
    /* 0x04 */ void *args;
    /* 0x08 */ u8 selector; /* index into gRibbonSourceProcs and gRibbonMessageProcs */
    /* 0x09 */ u8 pad9[3];
} RibbonState;

/* One point of a ribbon effect. */
typedef struct RibbonEffectPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 distance; /* distance to the previous point */
    /* 0x10 */ u8 alpha;
    /* 0x11 */ u8 pad11[3];
} RibbonEffectPoint;

/* Returns 0 when the effect's source is gone, else stores the source position in pos. */
static s32 (*gRibbonSourceProcs[])(EffectObj *, Vec3f *) = { trail_getRibbonActorPos, trail_getRibbonPointBehind };

/* Spawns a ribbon effect that follows the position given by gRibbonSourceProcs[selector], with a copy of args
 * (argsSize bytes) and a linked particle. Returns NULL if nothing was spawned. */
EffectObj *trail_spawnRibbon(void *args, u8 selector, s32 argsSize, u8 arg3, s32 arg4) {
    EffectObj *obj;
    EffectTemplate18 spawn;
    s32 pad; // matching: stack pad
    RibbonState init;
    RibbonState *state;
    ParticleDesc particle;

    init.linked = NULL;
    init.args = NULL;
    init.selector = selector;
    spawn.itemCount = 20;
    spawn.pos = D_800A5480;
    spawn.life = 300;
    spawn.flags = 20;
    spawn.kind = 16;
    spawn.unk14 = 2;
    obj = func_15147A80(&spawn, argsSize + 0x10, sizeof(RibbonEffectPoint), 0, 14, 14, 0, 0, 0, arg3, arg4);
    if (obj != NULL) {
        state = obj->userData;
        memcpy(state, &init, sizeof(init));
        // matching: state->args = (u8 *)state + 0x10, stored and reloaded as an s32 (IDO allocates differently otherwise)
        ((s32 *)state)[1] = (s32)((s32 *)state + 4);
        memcpy((void *)((s32 *)state)[1], args, argsSize);
        if (gRibbonSourceProcs[selector](obj, &obj->head.tmpl.pos) == 0) {
            func_1516972C(&obj->head.header);
            return NULL;
        }
        {
        const s32 unk1DChoices[4] = { 0x60, 0x61, 0x62, 0x63 };
        s32 flag80;
        s32 flag40;
        particle.texture = unk1DChoices[func_150ADA20() & 3];
        particle.renderMode = 0x1303;
        particle.unk0 = 0x200005;
        particle.unk4 = 0;
        particle.timer = 300;
        particle.frame = 0;
        particle.frameRate = 0;
        particle.r = 0xFF;
        particle.g = 0xFF;
        particle.b = 0xFF;
        particle.a = 0xFF;
        particle.r2 = 0xFF;
        particle.g2 = 0xFF;
        particle.b2 = 0xFF;
        particle.alpha = 0xFF;
        particle.brightness = 0xFF;
        particle.width = particle.height = func_150ADA68() * 800.0f + 1700.0f;
        particle.pos = spawn.pos;
        particle.offset = D_800A5480;
        particle.vel = D_800A5480;
        particle.fadeStart = 1;
        particle.fadeRate = 0xFF;
        particle.scaleStart = 1;
        particle.gravity = 0.0f;
        particle.scaleRate = 1.0f;
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
        particle.primColorMode = 6;
        particle.envColorMode = 5;
        particle.updateFunc = -1;
        particle.deathFunc = -1;
        particle.drawFunc = -1;
        particle.handlerSet = 0;
        particle.viewMask = 0;
        particle.views = 0xFF;
        state->linked = func_15130280(&particle, 1, 0, 0, arg3, arg4);
        }
    }
    return obj;
}

/* Object a ribbon effect orbits (trail_getRibbonPointBehind). */
typedef struct RotatedTarget {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 scaleXZ;
    /* 0x1C */ f32 scaleY;
    /* 0x20 */ Vec3f rot;
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ Vec3f pos;
} RotatedTarget;

/* gRibbonSourceProcs callback: reads the followed actor's position; fails once the target is gone. */
s32 trail_getRibbonActorPos(EffectObj *obj, Vec3f *pos) {
    RibbonState *state;
    TrailTargetRef *ref;
    Actor *target;

    state = obj->userData;
    ref = state->args;
    target = ref->target;
    if (target->type == 0 || target->unique_id != ref->targetUnk3B) {
        return 0;
    }

    pos->x = target->xpos;
    pos->y = target->ypos;
    pos->z = target->zpos;
    return 1;
}

/* gRibbonSourceProcs callback: places pos 250 units along the target's local -z axis. */
s32 trail_getRibbonPointBehind(EffectObj *obj, Vec3f *pos) {
    RibbonState *state;
    RotatedTarget **ref;
    RotatedTarget *target;
    MtxF mtx;

    state = obj->userData;
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
    func_150A7960(mtx, 0.0f, 0.0f, -250.0f, &pos->x, &pos->y, &pos->z);
    return 1;
}

/* Moves the ribbon to its source position (gRibbonSourceProcs[selector]) and appends a point there. Returns 0 once the source is gone. */
s32 trail_moveRibbon(EffectObj *obj) {
    RibbonState *state;
    RibbonEffectPoint *points;
    Vec3f previous;
    Vec3f delta;
    f32 distance;

    state = obj->userData;
    points = obj->items;
    if (D_800BE9E4 > 0) {
        previous = obj->head.tmpl.pos;
        if (gRibbonSourceProcs[state->selector](obj, &obj->head.tmpl.pos) == 0) {
            return 0;
        }
        if (state->linked != NULL) {
            state->linked->pos = obj->head.tmpl.pos;
        }
        delta.x = obj->head.tmpl.pos.x - previous.x;
        delta.y = obj->head.tmpl.pos.y - previous.y;
        delta.z = obj->head.tmpl.pos.z - previous.z;
        distance = func_15143E64(&delta);
        {
            RibbonEffectPoint *point = &points[obj->head.end];

            point->pos = obj->head.tmpl.pos;
            point->distance = distance;
            point->alpha = 0xFF;
        }
        obj->head.end++;
        if (obj->head.end == obj->head.tmpl.itemCount) {
            obj->head.end = 0;
        }
        obj->head.count++;
        if (obj->head.end == obj->head.start) {
            obj->head.start++;
            if (obj->head.start == obj->head.tmpl.itemCount) {
                obj->head.start = 0;
            }
            obj->head.count--;
        }
    }
    return 1;
}

/* Trims the ribbon to at most 1198 units of length, then fades its points from the head (opaque) to the tail. */
s32 trail_shapeRibbon(EffectObj *obj) {
    RibbonEffectPoint *points;
    f32 total;
    f32 scale;
    s32 i;
    s32 prev;
    f32 t;
    RibbonEffectPoint *cur;
    RibbonEffectPoint *next;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 alphaScale;
    s32 j;

    points = obj->items;
    if (obj->head.count >= 2) {
        total = 0.0f;
        i = obj->head.end - 1;
        if (i < 0) {
            i = obj->head.tmpl.itemCount - 1;
        }
        while (i != obj->head.start) {
            prev = i;
            i--;
            if (i < 0) {
                i = obj->head.tmpl.itemCount - 1;
            }
            cur = &points[prev];
            total += cur->distance;
            scale = 1198.0f;
            if (total > scale) {
                next = &points[i];
                if (cur->distance != 0) {
                    t = (total - scale) / cur->distance;
                    dx = next->pos.x - cur->pos.x;
                    dy = next->pos.y - cur->pos.y;
                    dz = next->pos.z - cur->pos.z;
                    next->pos.x -= dx * t;
                    next->pos.y -= dy * t;
                    next->pos.z -= dz * t;
                    cur->distance *= 1.0f - t;
                }
                total = scale;
                while (i != obj->head.start) {
                    obj->head.start++;
                    if (obj->head.start == obj->head.tmpl.itemCount) {
                    wrap:; // matching: the label splits the basic block, which IDO needs here
                        obj->head.start = 0;
                    }
                    obj->head.count--;
                }
            }
        }

        if (total != 0) {
            scale = 1.0f / total;
        } else {
            scale = 0.0f;
        }
        ; // matching: an empty statement IDO needs for the scheduling
    }
    if (obj->head.count >= 2) {
        j = obj->head.end;
        dist = total;
        alphaScale = 255.0f * scale;
        do {
            j--;
            if (j < 0) {
                j = obj->head.tmpl.itemCount - 1;
            }
            cur = &points[j];
            cur->alpha = dist * alphaScale;
            dist -= cur->distance;
            next:; // matching: the label splits the basic block, which IDO needs here
        } while (j != obj->head.start);
    }
    return 1;
}

/* Draws the ribbon as a camera-facing strip 58 units wide, from the newest point to the oldest. */
Gfx *trail_drawRibbon(EffectObj *obj, Gfx *gfx, s16 view) {
    Vec3f next;
    Vec3f cur;
    s32 index;
    s32 previous;
    Vec3f *camera;
    RibbonEffectPoint *points;
    Vec3f toCamera;
    Vec3f delta;
    Vec3f normal;
    f32 lengthSquared;
    u8 flag;
    u8 curAlpha;
    u8 nextAlpha;
    Vtx_t *vtx;

    if (obj->head.count < 2) {
        return gfx;
    }
    points = obj->items;
    func_151D5D60(obj->buffers, view, (obj->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx_t), &vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    camera = func_15144B34(view);
    flag = 1;
    gfx = func_15142E24(gfx, &gEffectTextures[120], 0, 0, 0, 0, 0x78, 0, 0, &flag, 3);
    gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
    gfx = func_1513F4E4(gfx, 0x56, &flag);
    gfx = func_15142FBC(gfx, gSpriteTlutMode | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &flag);
    previous = obj->head.end - 1;
    if (previous < 0) {
        previous = obj->head.tmpl.itemCount - 1;
    }
    index = previous - 1;
    if (index < 0) {
        index = obj->head.tmpl.itemCount - 1;
    }
    cur = points[previous].pos;
    curAlpha = points[previous].alpha;
    next = points[index].pos;
    nextAlpha = points[index].alpha;
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
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP2Triangles(gfx++, 0, 1, 2, 0, 1, 3, 2, 0);
        previous = index;
        index--;
        if (index < 0) {
            index = obj->head.tmpl.itemCount - 1;
        }
        if (previous != obj->head.start) {
            cur = points[previous].pos;
            next = points[index].pos;
            nextAlpha = points[index].alpha;
        }
    } while (previous != obj->head.start);
    return gfx;
}

/* Message handlers of ribbon effects, indexed by RibbonState.selector; entries may be NULL. */
static void (*gRibbonMessageProcs[])(EffectObj *, void *, u8) = { trail_onActorRibbonMessage, NULL };

/* Message handler of a ribbon: forwards the message to gRibbonMessageProcs[selector], if there is one. */
void trail_onRibbonMessage(EffectObj *obj, void *msg, u8 msgType) {
    RibbonState *state = obj->userData;
    void (*handler)(EffectObj *, void *, u8) = gRibbonMessageProcs[state->selector];

    if (handler != NULL) {
        handler(obj, msg, msgType);
    }
}

/* gRibbonMessageProcs handler of a ribbon that follows an actor: message 0 (msg is a TrailTargetRef) naming the target
 * destroys the ribbon. */
void trail_onActorRibbonMessage(EffectObj *obj, void *msg, u8 msgType) {
    RibbonState *state = obj->userData;
    TrailTargetRef *ref = state->args;

    if (msgType == 0) {
        TrailTargetRef *named = msg;
        Actor *target = named->target;

        if (target == ref->target || named->targetUnk3B == ref->targetUnk3B) {
            func_1516972C(&obj->head.header);
        }
    }
}
