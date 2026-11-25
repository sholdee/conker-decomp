#include <ultra64.h>
// matching: functions.h declares these with other return types; the file needs the s32 forms
#define func_10010F88 func_10010F88_void
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_10010F88
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

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
    struct127 *owner;
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
    f32 sparkAccum;
    f32 puffAccum;
    f32 particleAccum;
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

extern f32 D_800A0B48;
extern f32 D_800A0B4C;
extern f32 D_800A0B50;
extern s32 func_151EF610(void);
extern void func_1000D96C(s32, s32, s32);
extern s32 func_10010F88(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern Trail *func_15147A80(TrailSpawn *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_15131828(void *, void *, void *, void *);
extern void func_15131958(void *, s32, void *);

/* Trail anchor point: 20 units above the actor. */
void func_150D8B10(struct127 *actor, struct17 *out) {
    out->unk0 = actor->x_position;
    out->unk4 = actor->y_position + 20.0f;
    out->unk8 = actor->z_position;
}

extern u8 D_800BE616;
extern f32 D_800A0B40;
extern f32 D_800A0B44;
void func_15145740(void *, void *, void *, void *, f32);

void func_150D8B3C(struct127 *actor, struct17 *out) {
    f32 unused1[3];
    f32 unused2[3];

    func_15145740(actor, unused1, unused2, out, D_800BE616 ? D_800A0B40 : D_800A0B44);
}

/* Spawns a trail object that follows the actor. */
void func_150D8B88(struct127 *actor) {
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

        data.speedScale = D_800A0B48;
        variant = func_151EF610() % 2;
        data.sound = func_10010F88(variant + 0x1B6, 0x7FFF, 0, 0, 0, 0, 0, 0, 500, 5000);
        data.pending = 0.0f;
        data.time = 0.0f;
        data.sparkAccum = 0.0f;
        data.puffAccum = 0.0f;
        data.particleAccum = 0.0f;
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

        trail = func_15147A80(&spawn, 0x50, 0x24, 0xE, 1, 0x11, 3, 0xFF, 0, 0xFF, 1);
        if (trail != NULL) {
            memcpy(trail->data, &data, sizeof(TrailData));
        }
    }
}

/* Advances pos by vel over dt under gravity. */
void func_150D8D84(struct17 *pos, struct17 *vel, f32 dt) {
    struct17 v0;

    v0 = *vel;
    vel->unk4 += D_800A0B4C * dt;
    pos->unk0 += v0.unk0 * dt;
    pos->unk4 += (v0.unk4 * dt) + ((D_800A0B50 * dt) * dt);
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
    struct127 *actor;
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
        flags = actor->unk31C->unk8A;
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
    struct127 *obj;
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
extern void func_15081690(struct127 *, f32, f32, f32, f32, f32, f32, RayHit *, f32, s32, s32, s32, s32, s32, s32);
extern void func_1505A250(f32, f32, f32, f32 *, f32 *);
extern void func_1505D024(struct127 *, s32, u16, s32);
extern void func_150DA67C();

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
    struct127 *owner;
    struct127 *target;
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
            i -= 1;
            if (i < 0) {
                i = trail->capacity - 1;
            }
            prev = &points[i];
        }
        maxLen = 1195.0f;
        do {
            TrailPoint *cur;

            invLen = -9999.0f;
            i -= 1;
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
                if (cur->len != 0) {
                    invLen = 1.0f / cur->len;
                    dir[0] = delta[0] * invLen;
                    dir[1] = delta[1] * invLen;
                    excess = (totalLen - maxLen) * invLen;
                    dir[2] = delta[2] * invLen;
                    cur->pos.unk0 = cur->pos.unk0 - delta[0] * excess;
                    cur->pos.unk4 = cur->pos.unk4 - delta[1] * excess;
                    cur->pos.unk8 = cur->pos.unk8 - delta[2] * excess;
                    cur->len = cur->len * (1.0f - excess);
                }
                totalLen = maxLen;
                while (i != trail->tail) {
                    trail->tail += 1;
                    if (trail->tail == trail->capacity) {
                        trail->tail = 0;
                    }
                    trail->count -= 1;
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
                                target->xz_velocity = target->xz_velocity * 0.7200000286f;
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
                    totalLen = totalLen - cur->len;
                    cur->len = hit.dist;
                    totalLen += cur->len;
                    while (i != trail->tail) {
                        trail->tail += 1;
                        if (trail->tail == trail->capacity) {
                            trail->tail = 0;
                        }
                        trail->count -= 1;
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
        data->texScroll = data->texScroll + -395.0f * D_800BE9A4;
        data->texScroll = func_15144528(data->texScroll, 16383.0f, -16384.0f);
        texPos = 0.0f;
        j = trail->head;
        for (;;) {
            TrailPoint *cur2;

            j -= 1;
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

            k -= 1;
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

typedef struct Game105FC0Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Game105FC0Vec3f;
typedef struct Game105FC0TrailRecord {
    Game105FC0Vec3f position;
    u8 pad0C[0x10];
    u8 alpha;
    u8 pad1D[3];
    f32 texture;
} Game105FC0TrailRecord;

void func_151D5D60(void *, s16, s32, void **, u8 *);
s32 func_15144B34(s32);
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_80091238[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;

Gfx *func_150D9C7C(u8 *arg0, Gfx *arg1, s16 arg2) {
    Game105FC0TrailRecord *records;
    Game105FC0Vec3f next;
    Game105FC0Vec3f cur;
    Game105FC0Vec3f toCamera;
    Game105FC0Vec3f delta;
    Game105FC0Vec3f normal;
    u8 *state;
    Game105FC0Vec3f *camera;
    s32 index;
    s32 previous;
    f32 lengthSquared;
    u8 flag;
    u8 alpha;
    f32 nextTexture;
    f32 curTexture;
    Vtx_t *vtx;

    if (*(s8 *)(arg0 + 0x2C) < 2) {
        return arg1;
    }
    state = *(u8 **)(arg0 + 0x98);
    records = *(Game105FC0TrailRecord **)(arg0 + 0x94);
    func_151D5D60(arg0 + 0x84, arg2, (arg0[0x25] * 2 + 10) * sizeof(Vtx_t), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return arg1;
    }
    camera = (Game105FC0Vec3f *)func_15144B34(arg2);
    flag = 1;
    arg1 = (Gfx *)func_15142E24((s32)arg1, (s32)D_80091238, 0, 0, 0, 0, 0x92, 0, 0, &flag, 3);
    arg1 = func_15142B7C(arg1, 0x220005, 0x1D0600);
    arg1 = func_1513F4E4(arg1, 0x4E, &flag);
    arg1 = func_15142FBC(arg1, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[3][0] | D_800A4AC8[3][1], &flag);
    if (*(u16 *)(arg0 + 0x1E) & 2) {
        index = *(s8 *)(arg0 + 0x2E) - 1;
        if (index < 0) {
            index = arg0[0x25] - 1;
        }
        cur = *(Game105FC0Vec3f *)(arg0 + 0x10);
        curTexture = *(f32 *)(state + 0x38);
    } else {
        previous = *(s8 *)(arg0 + 0x2E) - 1;
        if (previous < 0) {
            previous = arg0[0x25] - 1;
        }
        index = previous - 1;
        if (index < 0) {
            index = arg0[0x25] - 1;
        }
        cur = records[previous].position;
        curTexture = records[previous].texture;
    }
    next = records[index].position;
    alpha = records[index].alpha;
    nextTexture = records[index].texture;
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
        f32 scale = 4.0f / sqrtf(lengthSquared);

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
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0x400;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = alpha;
    vtx->flag = 0;
    vtx++;
    vtx->ob[0] = cur.x - normal.x;
    vtx->ob[1] = cur.y - normal.y;
    vtx->ob[2] = cur.z - normal.z;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = alpha;
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
            f32 scale = 4.0f / sqrtf(lengthSquared);

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
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0x400;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = alpha;
        vtx->flag = 0;
        vtx++;
        vtx->ob[0] = next.x - normal.x;
        vtx->ob[1] = next.y - normal.y;
        vtx->ob[2] = next.z - normal.z;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = alpha;
        vtx->flag = 0;
        vtx++;
        gSPVertex(arg1++, vtx - 4, 4, 0);
        gSP1Triangle(arg1++, 0, 1, 2, 0);
        gSP1Triangle(arg1++, 1, 3, 2, 0);
        if (nextTexture < curTexture) {
            memcpy(vtx, vtx - 2, 0x20);
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
        }
        previous = index;
        index--;
        if (index < 0) {
            index = arg0[0x25] - 1;
        }
        cur = records[previous].position;
        next = records[index].position;
        alpha = records[index].alpha;
        curTexture = records[previous].texture;
        nextTexture = records[index].texture;
    } while (previous != *(s8 *)(arg0 + 0x2D));
    return arg1;
}

void func_150DA484(void *arg0) {
    u16 *temp = (u16 *)*(u32 *)((u8 *)arg0 + 0x98);
    if (temp[0x18] != 0) {
        func_100111C8(temp[0x18]);
    }
}

void func_150DA484(void *);
void func_151478F4(void *);

void func_150DA4B4(void *arg0) {
    func_150DA484(arg0);
    func_151478F4(arg0);
}

void func_15147928(void *);

void func_150DA4E0(void *arg0) {
    func_150DA484(arg0);
    func_15147928(arg0);
}

typedef struct {
    s32 unk0;
    u8 unk4;
    char pad5[0x3C - 0x5];
    f32 unk3C;
} SubA_150DA50C;

typedef struct {
    s32 unk0;
    union {
        u8 b4;
        s32 w4;
    } u4;
    union {
        f32 f8;
        struct {
            u8 b8;
            u8 b9;
        } s;
    } u8;
} ArgB_150DA50C;

void func_150DA50C(void *arg0, s32 arg1, u8 arg2) {
    SubA_150DA50C *temp_v1 = *(SubA_150DA50C **)((u8 *)arg0 + 0x98);
    ArgB_150DA50C *b = (ArgB_150DA50C *)arg1;
    s32 temp;

    if (arg2 == 0x44) {
        if ((b->unk0 == temp_v1->unk0) || (b->u4.b4 == temp_v1->unk4)) {
            temp_v1->unk3C = b->u8.f8;
        }
trailing_label_150DA50C:
        ;
    } else if (arg2 == 0) {
        temp = b->unk0;
        if ((temp == temp_v1->unk0) || (temp_v1->unk4 == b->u4.b4)) {
            func_1516972C(arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v1->unk0 == b->unk0) {
            temp_v1->unk0 = b->u4.w4;
            temp_v1->unk4 = b->u8.s.b9;
        } else if (temp_v1->unk0 == b->u4.w4) {
            temp_v1->unk0 = b->unk0;
            temp_v1->unk4 = b->u8.s.b8;
        }
    }
}

struct s_150DA5EC {
    s32 unk0;
    u8 unk4;
    f32 unk8;
};

extern void func_15147D64(struct s_150DA5EC *, s32);

void func_150DA5EC(u8 *arg0, f32 arg1) {
    struct s_150DA5EC sp1C;

    sp1C.unk0 = (s32) arg0;
    sp1C.unk4 = arg0[0x3B];
    sp1C.unk8 = arg1;
    func_15147D64(&sp1C, 0x44);
}

typedef struct {
    char pad_0x00[0xC];
    s32 field_0x0C;
} ActorA8State;

s32 func_150DA628(u8 *arg0, s32 arg1) {
    ActorA8State *temp_a2;

    temp_a2 = (ActorA8State *)(arg0 + 0xA8);
    func_15131828(arg0, arg0 + 0xAC, temp_a2, arg0 + 0xAA);
    func_15131958(arg0 + 0x58, temp_a2->field_0x0C, temp_a2);
    return 1;
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
} Particle150DA67C; /* 0x70 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Sub150DA67C;

typedef struct {
    u8 unk0[0x10];
    u16 unk10;
} Bytes150DA67C;

typedef struct {
    u8 pad0[4];
    u8 unk4;
} Kind150DA67C;

typedef struct {
    Kind150DA67C *unk0;
    u8 pad4[4];
    struct17 unk8;
    u8 pad14[0x30];
    Bytes150DA67C unk44;
    u8 pad56[3];
    u8 unk59;
} Obj150DA67C;

typedef struct {
    u8 pad0[0x40];
    f32 unk40;
    f32 unk44;
    f32 unk48;
} Data150DA67C;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x8B];
    Data150DA67C *unk98;
} Self150DA67C;

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
} Emit150DA67C; /* 0x44 */

typedef struct {
    u8 unk0[0x24];
} Dst150DA67C;

typedef struct {
    u8 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    struct17 unk10;
    struct17 unk1C;
    f32 unk28;
} ShapeA150DA67C; /* 0x2C */

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    Bytes150DA67C unkC;
    f32 unk20;
} ShapeB150DA67C; /* 0x24 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_150472C0(Dst150DA67C *, Obj150DA67C *);
extern void func_15153CCC(ShapeA150DA67C *, Emit150DA67C *, Dst150DA67C *, u8, s32);
extern void func_15153C84(ShapeB150DA67C *, Emit150DA67C *, Dst150DA67C *, u8, s32);

void func_150DA67C(Self150DA67C *arg0, Obj150DA67C *arg1, f32 *arg2) {
    Data150DA67C *data;
    Kind150DA67C *kind;

    data = arg0->unk98;
    kind = arg1->unk0;
    if (arg1->unk59 != 0) {
        if (arg1->unk59 >= 2 && (kind->unk4 == 0x33 || kind->unk4 == 0x3A)) {
            data->unk48 += (0.1990000159f + func_150ADA68() * 0.1540000141f) * D_800BE9A4;
            if (data->unk48 > 1.0f) {
                Particle150DA67C spawn;
                Sub150DA67C sub;
                void *ret;

                sub.unkC = 0.9936270118f;
                spawn.unk1D = 0x6C;
                spawn.unk8 = 0x5103;
                spawn.unk0 = 0x200005;
                spawn.unk1E = 0x2E;
                spawn.unk20 = 5;
                spawn.unk58 = 0x90DE07;
                spawn.unk60 = 8;
                spawn.unk61 = 6;
                spawn.unk62 = 0x24;
                spawn.unk63 = -1;
                spawn.unk4 = 0;
                spawn.unkC = 0;
                spawn.unk10 = 0;
                spawn.unk64 = -1;
                spawn.unk65 = 0;
                spawn.unk22 = 0x2E;
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
                spawn.unk24 = 1.038234949f;
                spawn.pos = arg1->unk8;
                spawn.vel = *(struct17 *)&D_800A5480;
                do {
                    sub.unk0 = func_150ADA20();
                    sub.unk1 = (s16)func_150ADA20();
                    sub.unk2 = (func_150ADA20() % 5U) + 4;
                    sub.unk3 = (func_150ADA20() % 5U) + 4;
                    sub.unk4 = func_150ADA68() * 10.0f;
                    sub.unk8 = func_150ADA68() * 10.0f;
                    spawn.unk54 = func_150ADA68() * 0.0910000056f + 0.1010000035f;
                    spawn.unk58 &= ~0xC0;
                    spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
                    spawn.unk1B = (func_150ADA20() % 101U) + 100;
                    spawn.unkA = (func_150ADA20() % 11U) + 70;
                    spawn.unk28 = spawn.unk2C = func_150ADA68() * 116.0f + 131.0f;
                    ret = func_15130280(&spawn, 1, 0, 0x10, arg0->unkC, arg0->unk1);
                    if (ret != NULL) {
                        memcpy((u8 *)ret + 0xA8, &sub, sizeof(sub));
                    }
                    data->unk48 -= 1.0f;
                } while (data->unk48 > 1.0f);
            }
        } else {
            data->unk44 += (0.06000000238f + func_150ADA68() * 0.06000000238f) * D_800BE9A4;
            if (data->unk44 > 1.0f) {
                Emit150DA67C emit;
                Dst150DA67C dst;

                func_150472C0(&dst, arg1);
                emit.unk0 = arg1->unk8;
                emit.unkC = 3.480000019f;
                emit.unk24 = 1;
                emit.unk26 = 2;
                emit.unk28 = 3;
                emit.unk2A = 1;
                emit.unk2C = 0x14;
                emit.unk2E = 0xA;
                emit.unk30 = 0xC8;
                emit.unk32 = 0x37;
                emit.unk3C = 0x10;
                emit.unk3E = 0xF;
                emit.unk40 = 0;
                emit.unk34 = 3;
                emit.unk10 = 2.659999847f;
                emit.unk14 = 1.33100009f;
                emit.unk18 = 1.160000086f;
                emit.unk1C = 0.01499999966f;
                emit.unk20 = 0.00699999975f;
                emit.unk38 = 0.200000003f;
                if (arg1->unk59 >= 2) {
                    ShapeA150DA67C a;

                    a.unk0 = 1;
                    a.unk4 = -arg2[0];
                    a.unk8 = -arg2[1];
                    a.unkC = -arg2[2];
                    a.unk10 = a.unk1C = *(struct17 *)&D_800A5480;
                    a.unk28 = 300.0f;
                    do {
                        func_15153CCC(&a, &emit, &dst, arg0->unkC, arg0->unk1);
                        data->unk44 -= 1.0f;
                    } while (data->unk44 > 1.0f);
                } else {
                    ShapeB150DA67C b;

                    b.unk0 = -arg2[0];
                    b.unk4 = -arg2[1];
                    b.unk8 = -arg2[2];
                    b.unkC = arg1->unk44;
                    b.unk20 = 300.0f;
                    do {
                        func_15153C84(&b, &emit, &dst, arg0->unkC, arg0->unk1);
                        data->unk44 -= 1.0f;
                    } while (data->unk44 > 1.0f);
                }
            }
        }
        if (arg1->unk59 == 1) {
            data->unk40 += (0.200000003f + func_150ADA68() * 0.1040000021f) * D_800BE9A4;
            if (data->unk40 > 1.0f) {
                do {
                    func_151D9B8C(3, func_150ADA68() * 30.0f + 15.0f, (func_150ADA20() % 101U) + 100, (s32)&arg1->unk44, &arg1->unk8,
                                  (func_150ADA20() % 81U) + 60, 1, 1, 1, arg0->unkC, arg0->unk1);
                    data->unk40 -= 1.0f;
                } while (data->unk40 > 1.0f);
            }
        }
    }
}

