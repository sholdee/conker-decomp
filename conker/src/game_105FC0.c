#include <ultra64.h>
// matching: functions.h declares these with other return types; the file needs the s32 forms
#define func_10010F88 func_10010F88_void
#include "functions.h"
#undef func_10010F88
#include "variables.h"
#include "effects.h"

/* Spawn description passed to func_15147A80. */
typedef struct {
    struct17 pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
} TrailSpawn;

/* One point of a trail's ring buffer (0x24 bytes). */
typedef struct {
    struct17 pos;
    struct17 vel;
    f32 len;        /* distance to the newer neighbour */
    u8 alpha;
    u8 pad1D[3];
    f32 texture;    /* texture s coordinate */
} TrailPoint;

/* Trail state block (0x50 bytes), copied into trail->data at spawn. */
typedef struct {
    Actor *owner;
    u8 ownerId;
    u8 pad5[3];
    struct17 unk8;
    struct17 lastPos;
    struct17 lastVel;
    f32 time;
    u16 sound;
    u8 pad32[2];
    f32 pending;        /* fractional points still to emit */
    f32 texScroll;
    f32 speedScale;
    f32 surfaceAccum;   /* hit effect accumulators, see func_150DA67C */
    f32 impactAccum;
    f32 sparkAccum;
    u8 flags;
    s8 timer;
    u8 pad4E[2];
} TrailData;

/* Trail object created by func_150D8B88. */
typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    struct17 pos;
    u16 unk1C;
    u16 flags;
    u8 pad20[5];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 tail;            /* oldest point */
    s8 head;            /* next point to write */
    u8 pad2F;
    u8 unk30;
    u8 pad31[0x23];
    struct17 unk54;
    u8 pad60[0x24];
    u8 unk84[0x10];
    TrailPoint *points;
    TrailData *data;
} Trail;

extern s32 func_151EF610(void);
extern void func_1000D96C(s32, s32, s32);
extern s32 func_10010F88(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern Trail *func_15147A80(TrailSpawn *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_15131828(void *, f32 *, void *, u8 *);
extern void func_15131958(void *, f32, void *);

/* Trail anchor point: 20 units above the actor. */
void func_150D8B10(Actor *actor, struct17 *out) {
    out->unk0 = actor->x_position;
    out->unk4 = actor->y_position + 20.0f;
    out->unk8 = actor->z_position;
}

extern u8 D_800BE616;
void func_15145740(Actor *actor, struct17 *target, struct17 *dir, struct17 *offsetDir, f32 pitchOffset);

/* Trail launch vector from the actor (func_15145740's offset direction). */
void func_150D8B3C(Actor *actor, struct17 *out) {
    struct17 target;
    struct17 dir;

    func_15145740(actor, &target, &dir, out, D_800BE616 ? 0.8400000334f : 1.15200007f);
}

/* Spawns a trail object that follows the actor. */
void func_150D8B88(Actor *actor) {
    TrailSpawn spawn;
    s32 pad; // matching: stack slot above data
    TrailData data;
    struct17 pos;
    Trail *trail;
    s32 variant;

    if (actor->unk31C != NULL) {
        if (D_800BE9F0 == 10) {
            func_1000D96C(10, 55, 0);
        }

        func_150D8B10(actor, &spawn.pos);
        data.timer = 0;
        data.flags = 1;
        data.owner = actor;
        data.ownerId = actor->unique_id;
        pos = spawn.pos;
        data.unk8 = pos;
        data.lastPos = pos;
        func_150D8B3C(actor, &data.lastVel);

        data.speedScale = 0.01724099927f;
        variant = func_151EF610() % 2;
        data.sound = func_10010F88(variant + 0x1B6, 0x7FFF, 0, 0, 0, 0, 0, 0, 500, 5000);
        data.pending = 0.0f;
        data.time = 0.0f;
        data.surfaceAccum = 0.0f;
        data.impactAccum = 0.0f;
        data.sparkAccum = 0.0f;
        data.texScroll = -16384.0f;

        if (actor->unk31C != NULL) {
            if (actor->unk31C->unk84 != 0) {
                data.flags &= ~1;
            }
        }

        spawn.unk15 = 50;
        spawn.unkC = 300;
        spawn.unkE = 54;
        spawn.unk10 = 2;
        spawn.unk14 = 5;
        spawn.unk18 = 0;

        trail = func_15147A80(&spawn, sizeof(TrailData), sizeof(TrailPoint), 0xE, 1, 0x11, 3, 0xFF, 0, 0xFF, 1);
        if (trail != NULL) {
            memcpy(trail->data, &data, sizeof(TrailData));
        }
    }
}

/* Advances pos by vel over dt under gravity. */
void func_150D8D84(struct17 *pos, struct17 *vel, f32 dt) {
    struct17 v0;

    v0 = *vel;
    vel->unk4 += -1.91200006f * dt;
    pos->unk0 += v0.unk0 * dt;
    pos->unk4 += (v0.unk4 * dt) + ((-0.95600003f * dt) * dt);
    pos->unk8 += v0.unk8 * dt;
}

void func_150D8E1C(Trail *trail) {
    trail->unk30 = 0;
    trail->flags &= ~2;
    trail->flags |= 8;
    trail->flags |= 1;
    trail->unk1C = 40;
}

/* Moves every live point under gravity and ticks the owner's counter. */
s32 func_150D8E4C(Trail *trail) {
    TrailData *data;
    TrailPoint *points;

    data = trail->data;
    points = trail->points;
    if ((trail->count < 2) && (trail->flags & 8)) {
        return 0;
    } else {
        s32 i = trail->head;

        while (i != trail->tail) {
            i--;
            if (i < 0) {
                i = trail->capacity - 1;
            }
            func_150D8D84(&points[i].pos, &points[i].vel, D_800BE9A4);
        }
    }

    if (trail->count > 0) {
        trail->unk54 = points[trail->tail].pos;
    } else {
        trail->unk54.unk0 = 0.0f;
        trail->unk54.unk4 = 0.0f;
        trail->unk54.unk8 = 0.0f;
    }

    data->timer += D_800BE9E4;
    if (data->timer >= 61) {
        if (data->owner != NULL && data->owner->unk31C != NULL) {
            data->owner->unk31C->unk1AA++;
        }
        data->timer = 0;
    }

    return 1;
}

/* Follows the owner and emits new trail points, interpolated over the elapsed time. */
s32 func_150D8FAC(Trail *trail) {
    TrailData *data;
    TrailPoint *points;
    Actor *actor;
    struct17 pos;
    struct17 vel;
    f32 time;
    f32 inv;
    u16 flags;
    f32 step;
    struct17 dpos;
    struct17 dvel;

    data = trail->data;
    points = trail->points;
    actor = data->owner;
    if ((actor->unk31C->unk78 != 3) || (actor->health <= 0)) {
        func_150D8E1C(trail);
    }
    if ((actor->interaction_state == 0) || (actor->unique_id != data->ownerId)) {
        return 0;
    }
    func_150D8B10(actor, &pos);
    func_150D8B3C(actor, &vel);
    trail->pos = pos;
    if (data->flags & 1) {
        flags = actor->unk31C->buttons;
        if (flags & 0x2000) {
            data->speedScale += 0.001379000023f * D_800BE9A4;
            if (data->speedScale > 0.04293100163f) {
                data->speedScale = 0.04293100163f;
            }
        } else {
            data->speedScale -= 0.001379000023f * D_800BE9A4;
            if (data->speedScale < 0.01724099927f) {
                data->speedScale = 0.01724099927f;
            }
        }
    }
    data->pending += 0.380000025f * D_800BE9A4;
    if (data->pending > 1.0f) {
        struct17 cur;
        struct17 curVel;
        f32 dpx;
        f32 dpy;
        f32 dpz;
        struct17 dvs;

        inv = 1.0f / data->pending;
        time = data->time + D_800BE9A4;
        cur = data->lastPos;
        curVel = data->lastVel;
        step = time * inv;
        dpos.unk0 = pos.unk0 - data->lastPos.unk0;
        dpos.unk4 = pos.unk4 - data->lastPos.unk4;
        dpos.unk8 = pos.unk8 - data->lastPos.unk8;
        dvel.unk0 = vel.unk0 - data->lastVel.unk0;
        dvel.unk4 = vel.unk4 - data->lastVel.unk4;
        dvel.unk8 = vel.unk8 - data->lastVel.unk8;
        dpx = dpos.unk0 * inv;
        dpy = dpos.unk4 * inv;
        dpz = dpos.unk8 * inv;
        dvs.unk0 = dvel.unk0 * inv;
        dvs.unk4 = dvel.unk4 * inv;
        dvs.unk8 = dvel.unk8 * inv;
        do {
            TrailPoint *point;

            point = &points[trail->head];
            point->pos = cur;
            point->vel.unk0 = data->speedScale * curVel.unk0;
            point->vel.unk4 = data->speedScale * curVel.unk4;
            point->vel.unk8 = data->speedScale * curVel.unk8;
            point->len = 0.0f;
            point->alpha = 255;
            point->texture = 0.0f;
            func_150D8D84(&point->pos, &point->vel, time);
            trail->head++;
            time -= step;
            if (trail->head == trail->capacity) {
                trail->head = 0;
            }
            trail->count++;
            if (trail->head == trail->tail) {
                /* buffer full: drop the oldest point */
                trail->tail++;
                if (trail->tail == trail->capacity) {
                    trail->tail = 0;
                }
                trail->count--;
            }
            cur.unk0 += dpx;
            cur.unk4 += dpy;
            cur.unk8 += dpz;
            curVel.unk0 += dvs.unk0;
            curVel.unk4 += dvs.unk4;
            curVel.unk8 += dvs.unk8;
            data->pending -= 1.0f;
        } while (data->pending > 1.0f);
        data->lastPos = cur;
        data->lastVel = curVel;
        data->time = time;
    }
    return 1;
}

/* Surface record of a ray hit (passed on to the particle spawners). */
typedef struct {
    u8 unk0[0x10];
    u16 unk10;
} RaySurface;

/* Result of the func_15081690 ray cast. */
typedef struct {
    Actor *obj;
    f32 dist;
    struct17 pos;
    u8 pad14[0x30];
    RaySurface surface;
    u8 pad56[3];
    u8 hit;             /* 0 none, 1 surface, 2+ object */
    u8 pad5A[0xE];
} RayHit;

f32 sqrtf(f32);

extern f32 func_150484A0(f32, f32);
extern f32 func_15143E64(f32 *);
extern f32 func_15144528(f32, f32, f32);
extern void func_15081690(Actor *, f32, f32, f32, f32, f32, f32, RayHit *, f32, s32, s32, s32, s32, s32, s32);
extern void func_1505A250(f32, f32, f32, f32 *, f32 *);
extern void func_1505D024(Actor *, s32, u16, s32);
void func_150DA67C(Trail *trail, RayHit *hit, f32 *dir);

/*
 * Walks the trail from newest to oldest point: clamps its total length, ray casts every
 * segment (hits trim the trail and affect the object hit), then scrolls the texture and
 * fades the oldest part.
 */
s32 func_150D942C(Trail *trail) {
    s32 count;
    s32 i;
    f32 totalLen;
    f32 pad; // matching: unused stack slot
    f32 maxLen;
    f32 excess;
    f32 delta[3];
    f32 len;
    f32 invLen;
    f32 dir[3];
    RayHit hit;
    TrailPoint *points;
    TrailPoint *prev;
    TrailData *data;
    Actor *owner;
    Actor *target;
    f32 dx;
    f32 dz;
    s32 ownerIndex;
    s32 j, k;
    f32 invFadeLen;
    f32 texPos;
    f32 fadeLen;

    count = trail->count;
    data = trail->data;
    points = trail->points;
    owner = data->owner;
    if (count >= 2) {
        totalLen = 0.0f;
        i = trail->head;
        if (trail->flags & 2) {
            prev = (TrailPoint *)&trail->pos;
        } else {
            i--;
            if (i < 0) {
                i = trail->capacity - 1;
            }
            prev = &points[i];
        }
        maxLen = 1195.0f;
        do {
            TrailPoint *cur;

            invLen = -9999.0f;
            i--;
            if (i < 0) {
                i = trail->capacity - 1;
            }
            cur = &points[i];
            delta[0] = cur->pos.unk0 - prev->pos.unk0;
            delta[1] = cur->pos.unk4 - prev->pos.unk4;
            delta[2] = cur->pos.unk8 - prev->pos.unk8;
            len = func_15143E64(delta);
            totalLen += len;
            cur->len = len;
            if (maxLen < totalLen) {
                /* too long: cut this segment and drop everything older */
                // matching: integer 0 here; 0.0f hoists the zero constant and grows the frame
                if (cur->len != 0) {
                    invLen = 1.0f / cur->len;
                    dir[0] = delta[0] * invLen;
                    dir[1] = delta[1] * invLen;
                    excess = (totalLen - maxLen) * invLen;
                    dir[2] = delta[2] * invLen;
                    cur->pos.unk0 -= delta[0] * excess;
                    cur->pos.unk4 -= delta[1] * excess;
                    cur->pos.unk8 -= delta[2] * excess;
                    cur->len *= 1.0f - excess;
                }
                totalLen = maxLen;
                while (i != trail->tail) {
                    trail->tail++;
                    if (trail->tail == trail->capacity) {
                        trail->tail = 0;
                    }
                    trail->count--;
                }
            }

            if (cur->len != 0.0f) {
                if (invLen == -9999.0f) {
                    invLen = 1.0f / cur->len;
                    dir[0] = delta[0] * invLen;
                    dir[1] = delta[1] * invLen;
                    dir[2] = delta[2] * invLen;
                }
                func_15081690(owner, prev->pos.unk0, prev->pos.unk4, prev->pos.unk8, dir[0], dir[1], dir[2], &hit, cur->len, 0, 0, 1, -1, 0, 0);
                if (hit.hit != 0) {
                    func_150DA67C(trail, &hit, dir);
                    if (hit.hit >= 2) {
                        target = hit.obj;
                        target->unk2EC = 1;
                        if (target->id == 0x3A) {
                            target->unk232 = 10;
                            target->unk218 = NULL;
                        } else if (target->id == 0x10 && target->unk251 == 1) {
                            func_1505A250(target->x_position - hit.pos.unk0, target->z_position - hit.pos.unk8,
                                          ((1.0f - (totalLen - 50.0f) * 0.001052631531f) * 0.4550000131f) + 0.2010000050f,
                                          &target->unk164, &target->unk168);
                            if (target->unk232 == 15) {
                                s32 pad; // matching: -g3 block scope
                                target->unk218 = NULL;
                            }
                        } else if (target->id == 0x10) {
                            dx = cur->pos.unk0 - owner->x_position;
                            dz = cur->pos.unk8 - owner->z_position;
                            if (target->unk232 == 16) {
                                target->xz_velocity *= 0.7200000286f;
                                excess = 1.0f / (sqrtf(dx * dx + dz * dz) * -0.07000000030f);
                                func_1505A250(dx * excess, dz * excess, 0.7f, &target->unk164, &target->unk168);
                            }
                            if (target->immune == 0) {
                                s32 pad; // matching: -g3 block scope
                                target->unk232 = 15;
                                target->unk218 = NULL;
                            }
                        } else if (target->id == 0x91) {
                            target->unk232 = 3;
                            target->unk218 = NULL;
                        } else if (target->id == 0x90) {
                            target->unk232 = 44;
                            target->unk218 = NULL;
                        } else {
                            ownerIndex = owner - D_800CC2D0;
                            func_1505D024(target, 0xE0036, ((u16)(func_150484A0(dir[0], dir[2]) * 10430.37793f) - 0x4000) | 1, ownerIndex);
                        }
                    }
                    /* the hit ends the trail here */
                    cur->pos = hit.pos;
                    totalLen -= cur->len;
                    cur->len = hit.dist;
                    totalLen += cur->len;
                    while (i != trail->tail) {
                        trail->tail++;
                        if (trail->tail == trail->capacity) {
                            trail->tail = 0;
                        }
                        trail->count--;
                    }
                    if (data->sound != 0) {
                        func_1000F91C(data->sound, 0x7FFF, 0, 0, 0, (s32)cur->pos.unk0, (s32)cur->pos.unk4, (s32)cur->pos.unk8, 500, 5000);
                    }
                }
            }
            prev = cur;
        } while (i != trail->tail);
        count = trail->count;
    }
    if (count >= 2) {
        data->texScroll += -395.0f * D_800BE9A4;
        data->texScroll = func_15144528(data->texScroll, 16383.0f, -16384.0f);
        texPos = 0.0f;
        j = trail->head;
        // matching: a do-while or plain break here changes the frame size (-g3)
        for (;;) {
            TrailPoint *cur2;

            j--;
            if (j < 0) {
                j = trail->capacity - 1;
            }
            cur2 = &points[j];
            texPos += cur2->len;
            cur2->texture = func_15144528(data->texScroll + texPos * 29.02999878f, 16383.0f, -16384.0f);
            if (j != trail->tail) {
                continue;
            }
            break;
        }
        count = trail->count;
    }
    if (count >= 2) {
        /* fade out the oldest 19.8% of the trail */
        len = 0.0f;
        dx = totalLen * 0.8020000458f;
        k = trail->head;
        fadeLen = totalLen - dx;
        invFadeLen = 1.0f / fadeLen;
        do {
            TrailPoint *cur3;

            k--;
            if (k < 0) {
                k = trail->capacity - 1;
            }
            cur3 = &points[k];
            len += cur3->len;
            if (dx < len) {
                s32 pad; // matching: -g3 block scope
                cur3->alpha = (u32)((fadeLen - (len - dx)) * invFadeLen * 155.0f);
            } else {
                cur3->alpha = 155;
            }
        } while (k != trail->tail);
    }
    return 1;
}

void func_151D5D60(void *, s16, s32, void **, u8 *);
struct17 *func_15144B34(s32);
Gfx *func_15142E24(Gfx *, u8 *, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_80091238[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;

/* Draws the trail as a camera-facing ribbon, one quad per segment. */
Gfx *func_150D9C7C(Trail *trail, Gfx *gfx, s16 view) {
    TrailPoint *points;
    struct17 next;
    struct17 cur;
    struct17 toCamera;
    struct17 delta;
    struct17 normal;
    TrailData *data;
    struct17 *camera;
    s32 index;
    s32 previous;
    f32 lengthSquared;
    u8 flag;
    u8 alpha;
    f32 nextTexture;
    f32 curTexture;
    Vtx_t *vtx;

    if (trail->count < 2) {
        return gfx;
    }
    data = trail->data;
    points = trail->points;
    func_151D5D60(trail->unk84, view, (trail->capacity * 2 + 10) * sizeof(Vtx_t), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    camera = func_15144B34(view);
    flag = 1;
    gfx = func_15142E24(gfx, D_80091238, 0, 0, 0, 0, 0x92, 0, 0, &flag, 3);
    gfx = func_15142B7C(gfx, 0x220005, 0x1D0600);
    gfx = func_1513F4E4(gfx, 0x4E, &flag);
    gfx = func_15142FBC(gfx, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[3][0] | D_800A4AC8[3][1], &flag);
    if (trail->flags & 2) {
        index = trail->head - 1;
        if (index < 0) {
            index = trail->capacity - 1;
        }
        cur = trail->pos;
        curTexture = data->texScroll;
    } else {
        previous = trail->head - 1;
        if (previous < 0) {
            previous = trail->capacity - 1;
        }
        index = previous - 1;
        if (index < 0) {
            index = trail->capacity - 1;
        }
        cur = points[previous].pos;
        curTexture = points[previous].texture;
    }
    next = points[index].pos;
    alpha = points[index].alpha;
    nextTexture = points[index].texture;
    delta.unk0 = cur.unk0 - next.unk0;
    delta.unk4 = cur.unk4 - next.unk4;
    delta.unk8 = cur.unk8 - next.unk8;
    toCamera.unk0 = cur.unk0 - camera->unk0;
    toCamera.unk4 = cur.unk4 - camera->unk4;
    toCamera.unk8 = cur.unk8 - camera->unk8;
    normal.unk0 = delta.unk4 * toCamera.unk8 - toCamera.unk4 * delta.unk8;
    normal.unk4 = delta.unk8 * toCamera.unk0 - toCamera.unk8 * delta.unk0;
    normal.unk8 = delta.unk0 * toCamera.unk4 - toCamera.unk0 * delta.unk4;
    lengthSquared = normal.unk0 * normal.unk0 + normal.unk4 * normal.unk4 + normal.unk8 * normal.unk8;
    if (lengthSquared != 0.0f) {
        f32 scale = 4.0f / sqrtf(lengthSquared);

        normal.unk0 *= scale;
        normal.unk4 *= scale;
        normal.unk8 *= scale;
    } else {
        normal.unk0 = 0.0f;
        normal.unk4 = 0.0f;
        normal.unk8 = 0.0f;
    }
    vtx->ob[0] = cur.unk0 + normal.unk0;
    vtx->ob[1] = cur.unk4 + normal.unk4;
    vtx->ob[2] = cur.unk8 + normal.unk8;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0x400;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = alpha;
    vtx->flag = 0;
    vtx++;
    vtx->ob[0] = cur.unk0 - normal.unk0;
    vtx->ob[1] = cur.unk4 - normal.unk4;
    vtx->ob[2] = cur.unk8 - normal.unk8;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = alpha;
    vtx->flag = 0;
    vtx++;
    do {
        delta.unk0 = cur.unk0 - next.unk0;
        delta.unk4 = cur.unk4 - next.unk4;
        delta.unk8 = cur.unk8 - next.unk8;
        toCamera.unk0 = next.unk0 - camera->unk0;
        toCamera.unk4 = next.unk4 - camera->unk4;
        toCamera.unk8 = next.unk8 - camera->unk8;
        normal.unk0 = delta.unk4 * toCamera.unk8 - toCamera.unk4 * delta.unk8;
        normal.unk4 = delta.unk8 * toCamera.unk0 - toCamera.unk8 * delta.unk0;
        normal.unk8 = delta.unk0 * toCamera.unk4 - toCamera.unk0 * delta.unk4;
        lengthSquared = normal.unk0 * normal.unk0 + normal.unk4 * normal.unk4 + normal.unk8 * normal.unk8;
        if (lengthSquared != 0.0f) {
            f32 scale = 4.0f / sqrtf(lengthSquared);

            normal.unk0 *= scale;
            normal.unk4 *= scale;
            normal.unk8 *= scale;
        } else {
            normal.unk0 = 0.0f;
            normal.unk4 = 0.0f;
            normal.unk8 = 0.0f;
        }
        vtx->ob[0] = next.unk0 + normal.unk0;
        vtx->ob[1] = next.unk4 + normal.unk4;
        vtx->ob[2] = next.unk8 + normal.unk8;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0x400;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = alpha;
        vtx->flag = 0;
        vtx++;
        vtx->ob[0] = next.unk0 - normal.unk0;
        vtx->ob[1] = next.unk4 - normal.unk4;
        vtx->ob[2] = next.unk8 - normal.unk8;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = alpha;
        vtx->flag = 0;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 1, 3, 2, 0);
        if (nextTexture < curTexture) {
            /* texture wrapped: repeat the last pair shifted back one period */
            memcpy(vtx, vtx - 2, 2 * sizeof(Vtx_t));
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
        }
        previous = index;
        index--;
        if (index < 0) {
            index = trail->capacity - 1;
        }
        cur = points[previous].pos;
        next = points[index].pos;
        alpha = points[index].alpha;
        curTexture = points[previous].texture;
        nextTexture = points[index].texture;
    } while (previous != trail->tail);
    return gfx;
}

/* Stops the trail's sound. */
void func_150DA484(Trail *trail) {
    TrailData *data = trail->data;

    if (data->sound != 0) {
        func_100111C8(data->sound);
    }
}

void func_151478F4(Trail *);

void func_150DA4B4(Trail *trail) {
    func_150DA484(trail);
    func_151478F4(trail);
}

void func_15147928(Trail *);

void func_150DA4E0(Trail *trail) {
    func_150DA484(trail);
    func_15147928(trail);
}

/* Message broadcast to trails by func_15147D64. */
typedef struct {
    Actor *owner;
    union {
        u8 ownerId;             /* types 0, 0x44 */
        Actor *other;       /* type 0x2D */
    } x4;
    union {
        f32 speedScale;         /* type 0x44 */
        struct {
            u8 ownerId;
            u8 otherId;
        } ids;                  /* type 0x2D */
    } x8;
} TrailMessage;

/* Trail message handler: 0x44 sets the speed scale, 0 removes the trail, 0x2D swaps owners. */
void func_150DA50C(Trail *trail, TrailMessage *msg, u8 type) {
    TrailData *data = trail->data;
    Actor *owner;

    if (type == 0x44) {
        if ((msg->owner == data->owner) || (msg->x4.ownerId == data->ownerId)) {
            data->speedScale = msg->x8.speedScale;
        }
    // matching: the label keeps a branch-to-epilogue after the store
    done:
        ;
    } else if (type == 0) {
        owner = msg->owner;
        if ((owner == data->owner) || (data->ownerId == msg->x4.ownerId)) {
            func_1516972C((struct102 *)trail);
        }
    } else if (type == 0x2D) {
        if (data->owner == msg->owner) {
            data->owner = msg->x4.other;
            data->ownerId = msg->x8.ids.otherId;
        } else if (data->owner == msg->x4.other) {
            data->owner = msg->owner;
            data->ownerId = msg->x8.ids.ownerId;
        }
    }
}

extern void func_15147D64(TrailMessage *, s32);

/* Sets the speed scale of every trail owned by the actor. */
void func_150DA5EC(Actor *actor, f32 speedScale) {
    TrailMessage msg;

    msg.owner = actor;
    msg.x4.ownerId = actor->unique_id;
    msg.x8.speedScale = speedScale;
    func_15147D64(&msg, 0x44);
}

/* Per-particle data stored in the sparks spawned by func_150DA67C. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} SparkData;

typedef struct {
    u8 pad0[0x58];
    u8 unk58[0x50];
    SparkData spark;
} Spark;

s32 func_150DA628(Spark *particle, s32 arg1) {
    SparkData *spark;

    spark = &particle->spark;
    func_15131828(particle, &spark->unk4, spark, &spark->unk2);
    func_15131958(particle->unk58, spark->unkC, spark);
    return 1;
}

typedef struct {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
    s32 unk40;
} ImpactEmitter; /* 0x44 */

typedef struct {
    u8 unk0[0x24];
} ImpactOrigin;

typedef struct {
    u8 unk0;
    struct17 dir;
    struct17 unk10;
    struct17 unk1C;
    f32 unk28;
} ImpactCone; /* 0x2C */

typedef struct {
    struct17 dir;
    RaySurface surface;
    f32 unk20;
} ImpactSpray; /* 0x24 */

extern Spark *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);
extern void func_150472C0(ImpactOrigin *, RayHit *);
extern void func_15153CCC(ImpactCone *, ImpactEmitter *, ImpactOrigin *, u8, s32);
extern void func_15153C84(ImpactSpray *, ImpactEmitter *, ImpactOrigin *, u8, s32);

/* Spawns the impact effects of a trail segment hitting something (hit from func_15081690). */
void func_150DA67C(Trail *trail, RayHit *hit, f32 *dir) {
    TrailData *data;
    Actor *target;

    data = trail->data;
    target = hit->obj;
    if (hit->hit != 0) {
        if (hit->hit >= 2 && (target->id == 0x33 || target->id == 0x3A)) {
            data->sparkAccum += (0.1990000159f + func_150ADA68() * 0.1540000141f) * D_800BE9A4;
            if (data->sparkAccum > 1.0f) {
                ParticleDesc spawn;
                SparkData sub;
                Spark *spark;

                sub.unkC = 0.9936270118f;
                spawn.texture = 0x6C;
                spawn.renderMode = 0x5103;
                spawn.unk0 = 0x200005;
                spawn.fadeStart = 0x2E;
                spawn.fadeRate = 5;
                spawn.flags = 0x90DE07;
                spawn.primColorMode = 8;
                spawn.envColorMode = 6;
                spawn.updateFunc = 0x24;
                spawn.deathFunc = -1;
                spawn.unk4 = 0;
                spawn.frame = 0;
                spawn.frameRate = 0;
                spawn.drawFunc = -1;
                spawn.handlerSet = 0;
                spawn.scaleStart = 0x2E;
                spawn.r = 0xDD;
                spawn.g = 0xD3;
                spawn.b = 0xCD;
                spawn.a = 0xFF;
                spawn.r2 = 0x57;
                spawn.g2 = 0x55;
                spawn.b2 = 0x5A;
                spawn.brightness = 0xFF;
                spawn.offset.x = 0.0f;
                spawn.offset.y = 0.0f;
                spawn.offset.z = 0.0f;
                spawn.scaleRate = 1.038234949f;
                spawn.pos = *(vertex *)&hit->pos; // TODO(vectors): struct17/vertex mix
                spawn.vel = *(vertex *)&D_800A5480;
                do {
                    sub.unk0 = func_150ADA20();
                    sub.unk1 = (s16)func_150ADA20(); // matching: the s16 cast gives IDO's extra move
                    sub.unk2 = (func_150ADA20() % 5U) + 4;
                    sub.unk3 = (func_150ADA20() % 5U) + 4;
                    sub.unk4 = func_150ADA68() * 10.0f;
                    sub.unk8 = func_150ADA68() * 10.0f;
                    spawn.gravity = func_150ADA68() * 0.0910000056f + 0.1010000035f;
                    spawn.flags &= ~0xC0;
                    spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
                    spawn.alpha = (func_150ADA20() % 101U) + 100;
                    spawn.timer = (func_150ADA20() % 11U) + 70;
                    spawn.width = spawn.height = func_150ADA68() * 116.0f + 131.0f;
                    spark = func_15130280(&spawn, 1, 0, 0x10, trail->unkC, trail->unk1);
                    if (spark != NULL) {
                        memcpy(&spark->spark, &sub, sizeof(sub));
                    }
                    data->sparkAccum -= 1.0f;
                } while (data->sparkAccum > 1.0f);
            }
        } else {
            data->impactAccum += (0.06000000238f + func_150ADA68() * 0.06000000238f) * D_800BE9A4;
            if (data->impactAccum > 1.0f) {
                ImpactEmitter emit;
                ImpactOrigin origin;

                func_150472C0(&origin, hit);
                emit.unk0 = hit->pos;
                emit.unkC = 3.480000019f;
                emit.unk24 = 1;
                emit.unk26 = 2;
                emit.unk28 = 3;
                emit.unk2A = 1;
                emit.unk2C = 20;
                emit.unk2E = 10;
                emit.unk30 = 200;
                emit.unk32 = 55;
                emit.unk3C = 16;
                emit.unk3E = 15;
                emit.unk40 = 0;
                emit.unk34 = 3;
                emit.unk10 = 2.659999847f;
                emit.unk14 = 1.33100009f;
                emit.unk18 = 1.160000086f;
                emit.unk1C = 0.01499999966f;
                emit.unk20 = 0.00699999975f;
                emit.unk38 = 0.200000003f;
                if (hit->hit >= 2) {
                    ImpactCone cone;

                    cone.unk0 = 1;
                    cone.dir.unk0 = -dir[0];
                    cone.dir.unk4 = -dir[1];
                    cone.dir.unk8 = -dir[2];
                    cone.unk10 = cone.unk1C = *(struct17 *)&D_800A5480;
                    cone.unk28 = 300.0f;
                    do {
                        func_15153CCC(&cone, &emit, &origin, trail->unkC, trail->unk1);
                        data->impactAccum -= 1.0f;
                    } while (data->impactAccum > 1.0f);
                } else {
                    ImpactSpray spray;

                    spray.dir.unk0 = -dir[0];
                    spray.dir.unk4 = -dir[1];
                    spray.dir.unk8 = -dir[2];
                    spray.surface = hit->surface;
                    spray.unk20 = 300.0f;
                    do {
                        func_15153C84(&spray, &emit, &origin, trail->unkC, trail->unk1);
                        data->impactAccum -= 1.0f;
                    } while (data->impactAccum > 1.0f);
                }
            }
        }
        if (hit->hit == 1) {
            data->surfaceAccum += (0.200000003f + func_150ADA68() * 0.1040000021f) * D_800BE9A4;
            while (data->surfaceAccum > 1.0f) {
                func_151D9B8C(3, func_150ADA68() * 30.0f + 15.0f, (func_150ADA20() % 101U) + 100, (s32)&hit->surface, &hit->pos,
                              (func_150ADA20() % 81U) + 60, 1, 1, 1, trail->unkC, trail->unk1);
                data->surfaceAccum -= 1.0f;
            }
        }
    }
}

