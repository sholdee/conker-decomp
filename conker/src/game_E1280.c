#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#define TRAIL_STOPPED   1 /* state->flags: no more points are emitted */
#define TRAIL_FADE_TAIL 2 /* state->flags: ramp alpha up from the oldest point */
#define TRAIL_FADE_HEAD 4 /* state->flags: ramp alpha up from the newest point */

#define TRAIL_ATTACHED  2 /* trail->flags: the ribbon starts at the emitter position */

typedef struct {
    f32 x, y, z;
} Vec3f;

/* One point of a trail's ring buffer. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 width;
    /* 0x1C */ u16 alpha;
    /* 0x1E */ s16 life;
    /* 0x20 */ u8 texS;
    /* 0x21 */ u8 pad21[3];
} TrailPoint; // size = 0x24

/* Per-trail settings and emitter state, stored behind trail->state. */
typedef struct {
    /* 0x00 */ Vec3f baseVel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ u8 phaseJitter;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedSwing;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ f32 emitRate;
    /* 0x24 */ u8 shade;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ s16 alpha;
    /* 0x28 */ f32 widthGrowth;
    /* 0x2C */ f32 width;
    /* 0x30 */ u8 texStep;
    /* 0x31 */ u8 texStepJitter;
    /* 0x32 */ u8 fadeTailCount;
    /* 0x33 */ u8 fadeTailStep;
    /* 0x34 */ u8 fadeHeadCount;
    /* 0x35 */ u8 fadeHeadStep;
    /* 0x36 */ s16 life;
    /* 0x38 */ u8 burstPreset;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ f32 burstRate;
    /* 0x40 */ f32 speed;
    /* 0x44 */ f32 emitAccum;
    /* 0x48 */ u8 texS;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 speedTarget;
    /* 0x50 */ u8 phase;
    /* 0x51 */ u8 pad51[3];
    /* 0x54 */ f32 burstAccum;
} TrailState; // size = 0x58

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ u8 pad1C[2];
    /* 0x1E */ u16 flags;
    /* 0x20 */ u8 pad20[5];
    /* 0x25 */ u8 capacity; /* ring buffer size */
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail; /* oldest point */
    /* 0x2E */ s8 head; /* next free slot */
    /* 0x2F */ u8 pad2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 pad31[0x23];
    /* 0x54 */ Vec3f tailPos;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailState *state;
} Trail;

extern void func_15147D64(s32, s32);

void func_150B3DD0(void) {
    u8 sp24[4];

    func_15147D64(0, 5);
    sp24[0] = 0;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 2;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 4;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 1;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 3;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 5;
    func_151494E0((s32)&sp24[0], 0x18);
}

extern void func_1000FC18(s32, s16, s16, s16, s32);
extern void func_151478F4(void *);

void func_150B3E74(void *arg0) {
    func_1000FC18(0x221, (s16)(s32)*(f32 *)((u8 *)arg0 + 0x10),
                  (s16)(s32)*(f32 *)((u8 *)arg0 + 0x14),
                  (s16)(s32)*(f32 *)((u8 *)arg0 + 0x18), 0xFA0);
    func_151478F4(arg0);
}

extern void func_15147928(void *);

void func_150B3EE8(void *arg0) {
    func_1000FC18(0x221, (s16)(s32)*(f32 *)((u8 *)arg0 + 0x10),
                  (s16)(s32)*(f32 *)((u8 *)arg0 + 0x14),
                  (s16)(s32)*(f32 *)((u8 *)arg0 + 0x18), 0xFA0);
    func_15147928(arg0);
}

extern s32 func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 func_150B3F5C(void *arg0, void *arg1, s16 arg2) {
    struct vec150B3F5C {
        s32 unk0;
        s32 unk4;
        s32 unk8;
    };
    struct {
        struct vec150B3F5C unk0;
        s16 unkC;
        s16 unkE;
        s32 unk10;
        u8 unk14;
        u8 unk15;
        u8 pad16[2];
        s32 unk18;
    } sp9C;
    struct {
        u8 pad0[0x40];
        f32 unk40;
        f32 unk44;
        u8 unk48;
        u8 pad49[3];
        f32 unk4C;
        u8 unk50;
        u8 pad51[3];
        f32 unk54;
    } sp44;
    s32 temp_v0;
    s32 rnd;

    memcpy(&sp44, arg0, 0x40);
    sp44.unk44 = 0.0f;
    sp9C.unk15 = arg2;
    sp44.unk48 = 0;
    sp9C.unkE = 2;
    sp9C.unkC = 0x1F4;
    sp44.unk4C = *(f32 *)((u8 *)arg0 + 0x18) + *(f32 *)((u8 *)arg0 + 0x14);
    sp44.unk50 = 0;
    sp44.unk54 = 0.0f;
    sp44.unk40 = sp44.unk4C;
    sp9C.unk0 = *(struct vec150B3F5C *)arg1;
    sp9C.unk10 = 9;

    temp_v0 = func_15147A80(&sp9C, 0x58, 0x24, 7, 7, 7, 0, 0, 0, 0xFF, 1);
    if (temp_v0 != 0) {
        rnd = func_150ADA20();
        func_1000FA64(0x221, (s16)(s32)*(f32 *)((u8 *)arg1 + 0),
                      (s16)(s32)*(f32 *)((u8 *)arg1 + 4),
                      (s16)(s32)*(f32 *)((u8 *)arg1 + 8), 0x61A8, 0xFA0,
                      0x258, (s32)func_1000EF40, 0, 0, 8, rnd & 0x40);
        memcpy((void *)*(s32 *)((u8 *)temp_v0 + 0x98), &sp44, 0x58);
    }
    return temp_v0;
}

/* Moves and ages the trail's points, dropping expired ones from the tail. Returns 0 once a stopped trail is empty. */
s32 func_150B40E8(Trail *trail) {
    TrailState *state;
    TrailPoint *points;
    s32 i;
    s32 expired;

    state = trail->state;
    points = trail->points;

    if ((trail->count < 2) && (state->flags & TRAIL_STOPPED)) {
        return 0;
    }

    i = trail->head;
    if (i != trail->tail) {
        do {
            i--;
            expired = FALSE;
            if (i < 0) {
                i = trail->capacity - 1;
            }

            points[i].alpha = 0xFF;
            points[i].life -= D_800BE9E4;
            if (points[i].life < 0) {
                expired = TRUE;
            }

            points[i].vel.y += state->gravity * D_800BE9A4;
            points[i].pos.x += points[i].vel.x * D_800BE9A4;
            points[i].pos.y += points[i].vel.y * D_800BE9A4;
            points[i].pos.z += points[i].vel.z * D_800BE9A4;
            points[i].width += state->widthGrowth * D_800BE9A4;

            if (expired && i != trail->tail) {
                do {
                    trail->tail++;
                    if (trail->tail == trail->capacity) {
                        trail->tail = 0;
                    }
                    trail->count--;
                } while (i != trail->tail);
            }
        } while (i != trail->tail);
    }

    if (trail->count > 0) {
        trail->tailPos = points[trail->tail].pos;
    } else {
        trail->tailPos.x = 0.0f;
        trail->tailPos.y = 0.0f;
        trail->tailPos.z = 0.0f;
    }

    return 1;
}

void func_150B5088();

/* Emits new trail points for this frame, interpolating speed across the frame's step. */
s32 func_150B4294(Trail *trail) {
    f32 inv;
    f32 dt;
    f32 speed;
    TrailState *state;
    TrailPoint *points;
    Vec3f baseVel;
    f32 diff;
    f32 dtStep;
    f32 speedStep;

    state = trail->state;
    points = trail->points;
    state->phase += (func_150ADA20() % (u32)(state->phaseJitter + 1)) * D_800BE9E4;
    state->speedTarget = func_151423D8(state->phase - 0x40) * state->speedSwing + state->speedBase;
    state->emitAccum += state->emitRate * D_800BE9A4;
    if (state->emitAccum > 1.0f) {
        dt = D_800BE9A4;
        inv = 1.0f / state->emitAccum;
        baseVel = state->baseVel;
        dtStep = D_800BE9A4 * inv;
        speed = state->speed;
        diff = state->speedTarget - state->speed;
        speedStep = diff * inv;
        do {
            state->burstAccum += state->burstRate;
            if (state->burstAccum > 1.0f) {
                func_150B5088(trail);
            }
            points[trail->head].pos = trail->pos;
            points[trail->head].vel.x = baseVel.x * speed;
            points[trail->head].vel.y = baseVel.y * speed;
            points[trail->head].vel.z = baseVel.z * speed;
            points[trail->head].width = state->width;
            points[trail->head].alpha = 0xFF;
            points[trail->head].life = state->life;
            points[trail->head].texS = state->texS;
            state->texS += state->texStep + func_150ADA68() * state->texStepJitter;
            points[trail->head].vel.y += state->gravity * dt;
            points[trail->head].pos.x += points[trail->head].vel.x * dt;
            points[trail->head].pos.y += points[trail->head].vel.y * dt;
            points[trail->head].pos.z += points[trail->head].vel.z * dt;
            trail->head++;
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
            speed += speedStep;
            dt -= dtStep;
            state->emitAccum -= 1.0f;
        } while (state->emitAccum > 1.0f);
        state->speed = speed;
    }
    return 1;
}

extern u8 D_80091238[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

Gfx *func_150B4710(Trail *trail, Gfx *gfx, s16 viewport) {
    Vtx *vtx;
    TrailState *state;
    Vec3f b;
    Vec3f a;
    s32 j;
    s32 i;
    TrailPoint *points;
    f32 *cam;
    f32 cx;
    f32 cy;
    f32 cz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 len;
    u8 level;
    u8 prev;
    s16 alpha;
    s16 prevAlpha;
    u8 update;
    u8 fade;
    s16 count;
    s32 k;
    f32 scale;
    f32 scaleB;

    if (trail->count >= 2) {
        func_151D5D60(trail->unk84, viewport, (trail->capacity * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
        if (vtx != NULL) {
            cam = &D_800DBFF0[viewport].unk2F8;
            state = trail->state;
            points = trail->points;
            update = 1;
// matching: the label starts a new basic block, keeping the state loads below it
lbl1:
            if (state->flags & TRAIL_FADE_TAIL) {
                u8 fade1;
                s16 count1;

                fade1 = 0;
                count1 = state->fadeTailCount;
                k = trail->tail;
                do {
                    points[k].alpha = fade1;
                    fade1 += state->fadeTailStep;
                    if (++k == trail->capacity) k = 0;
                } while (--count1 != 0 && k != trail->head);
            }
            if (state->flags & TRAIL_FADE_HEAD) {
                fade = 0;
                count = state->fadeHeadCount;
                k = trail->head - 1;
                if (k < 0) {
                    k = trail->capacity - 1;
                }
                do {
                    points[k].alpha = fade;
                    fade += state->fadeHeadStep;
                    count--;
                    k--;
                    if (k < 0) {
                        k = trail->capacity - 1;
                    }
                } while (count != 0 && k != trail->head);
            }
            gfx = func_15142E24(gfx, (s32)D_80091238, 0, 0, 0, 0, 0x92, 0, 0, &update, 3);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_1513F4E4(gfx, 0x4D, &update);
            gfx = func_15142C10(gfx, state->shade, state->shade, state->shade, 0xFF, &update);
            gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &update);
            if (trail->flags & TRAIL_ATTACHED) {
                j = trail->head - 1;
                if (j < 0) {
                    j = trail->capacity - 1;
                }
                a = trail->pos;
                prev = points[j].texS;
                prevAlpha = (points[j].alpha * state->alpha) >> 8;
            } else {
                i = trail->head - 1;
                if (i < 0) {
                    i = trail->capacity - 1;
                }
                j = i - 1;
                if (j < 0) {
                    j = trail->capacity - 1;
                }
                a = points[i].pos;
                prev = points[i].texS;
                prevAlpha = (points[i].alpha * state->alpha) >> 8;
            }
            b = points[j].pos;
            level = points[j].texS;
            alpha = (points[j].alpha * state->alpha) >> 8;
            dx = a.x - b.x;
            dy = a.y - b.y;
            dz = a.z - b.z;
            cx = a.x - cam[0];
            cy = a.y - cam[1];
            cz = a.z - cam[2];
            nx = dy * cz - cy * dz;
            ny = dz * cx - cz * dx;
            nz = dx * cy - cx * dy;
            len = nx * nx + ny * ny + nz * nz;
            if (len == 0.0f) {
                nx = 0.0f;
                ny = 0.0f;
                nz = 0.0f;
            } else {
                scale = points[j].width / sqrtf(len);
                nx = nx * scale;
                ny = ny * scale;
                nz = nz * scale;
            }
            vtx->v.ob[0] = (s16)(a.x + nx);
            vtx->v.ob[1] = (s16)(a.y + ny);
            vtx->v.ob[2] = (s16)(a.z + nz);
            vtx->v.tc[0] = points[j].texS << 6;
            vtx->v.tc[1] = 0x43C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = (s16)(a.x - nx);
            vtx->v.ob[1] = (s16)(a.y - ny);
            vtx->v.ob[2] = (s16)(a.z - nz);
            vtx->v.tc[0] = points[j].texS << 6;
            vtx->v.tc[1] = 0x4000;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                dx = a.x - b.x;
                dy = a.y - b.y;
                dz = a.z - b.z;
                cx = b.x - cam[0];
                cy = b.y - cam[1];
                cz = b.z - cam[2];
                nx = dy * cz - cy * dz;
                ny = dz * cx - cz * dx;
                nz = dx * cy - cx * dy;
                len = nx * nx + ny * ny + nz * nz;
                if (len == 0.0f) {
                    nx = 0.0f;
                    ny = 0.0f;
                    nz = 0.0f;
                } else {
                    scaleB = points[j].width / sqrtf(len);
                    nx = nx * scaleB;
                    ny = ny * scaleB;
                    nz = nz * scaleB;
                }
                vtx->v.ob[0] = (s16)(b.x + nx);
                vtx->v.ob[1] = (s16)(b.y + ny);
                vtx->v.ob[2] = (s16)(b.z + nz);
                vtx->v.tc[0] = points[j].texS << 6;
                vtx->v.tc[1] = 0x43C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = (s16)(b.x - nx);
                vtx->v.ob[1] = (s16)(b.y - ny);
                vtx->v.ob[2] = (s16)(b.z - nz);
                vtx->v.tc[0] = points[j].texS << 6;
                vtx->v.tc[1] = 0x4000;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (prev < level) {
                    memcpy(vtx, vtx - 2, 2 * sizeof(Vtx));
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                }
                i = j;
                j--;
                if (j < 0) {
                    j = trail->capacity - 1;
                }
                prev = level;
                a = points[i].pos;
                b = points[j].pos;
                alpha = (points[j].alpha * state->alpha) >> 8;
                level = points[j].texS;
            } while (i != trail->tail);
        }
    }
    return gfx;
}

void func_150B5060(void *arg0) {
    void *temp_v0 = *(void **)((u8 *)arg0 + 0x98);

    *(u8 *)((u8 *)arg0 + 0x30) = 0;
    *(u16 *)((u8 *)arg0 + 0x1E) = *(u16 *)((u8 *)arg0 + 0x1E) & 0xFFFD;
    *(u8 *)((u8 *)temp_v0 + 0x1C) = *(u8 *)((u8 *)temp_v0 + 0x1C) | 1;
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} SplatParams;

typedef struct {
    struct17 unk0;
    u8 padC[0x18];
    u8 unk24;
    u8 pad25;
    s16 unk26;
    u8 pad28[0x10];
    u8 unk38;
    u8 pad39[0x1B];
    f32 unk54;
} SplatOwner;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    struct17 unk10;
    u8 pad1C[0x7C];
    SplatOwner *unk98;
} SplatActor;

typedef struct {
    struct17 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} SplatSpawn;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[4];
} SplatPart;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
} SplatStyle;

extern SplatParams D_8009FBF0[];
extern s32 func_15147DA0(SplatSpawn *, SplatPart *, s32, s32, s32, s32, s32, s32, s32, s32, s32, SplatStyle *, s32, s32, s32);

void func_150B5088(SplatActor *arg0) {
    SplatStyle sp100;
    SplatOwner *owner;
    SplatParams *par;
    SplatSpawn spE0;
    SplatPart spC0;
    f32 x;
    f32 y;
    f32 z;
    f32 speed;
    f32 cosA;
    f32 sinA;
    f32 cosB;
    f32 sinB;
    u8 angA;
    u8 angB;

    owner = arg0->unk98;
    par = &D_8009FBF0[owner->unk38];

    sp100.unk0 = 0;
    sp100.unk4 = 1;
    sp100.unk8 = 0x160600;
    sp100.unkC = 3;
    sp100.unk10 = 0x10;
    sp100.unk14 = 0x80;
    sp100.unk18 = 0x20;
    sp100.unk1C = 0;
    sp100.unk1D = 9;
    spE0.unkE = 1;
    spC0.unk19 = 6;
    spC0.unk1A = owner->unk24;
    spC0.unk18 = 8;
    spC0.unk1B = owner->unk26;
    spE0.unk0 = arg0->unk10;

    do {
        speed = func_150ADA68() * par->unk8 + par->unk4;
        angA = func_150ADA20() & 0xFF;
        angB = func_150ADA20() & 0xFF;
        cosA = func_151423D8(angA - 0x40);
        sinA = func_151423D8(angA);
        cosB = func_151423D8(angB - 0x40);
        sinB = func_151423D8(angB);
        x = owner->unk0.unk0 + par->unk0 * sinB * cosA;
        y = owner->unk0.unk4 - par->unk0 * cosB;
        z = owner->unk0.unk8 + par->unk0 * sinB * sinA;
        spE0.unk15 = ((u32)func_150ADA20() % (par->unk22 + 1)) + par->unk20;
        spE0.unkC = ((u32)func_150ADA20() % (par->unk1E + 1)) + par->unk1C;
        spC0.unk0 = func_150ADA68() * par->unk10 + par->unkC;
        spC0.unk10 = func_150ADA68() * par->unk18 + par->unk14;
        spC0.unk4 = speed * x;
        spC0.unk8 = speed * y;
        spC0.unkC = speed * z;
        func_15147DA0(&spE0, &spC0, 0, 1, 7, 0, 0, 0, 0, 0, 0, &sp100, 0, arg0->unkC, arg0->unk1);
        owner->unk54 -= 1.0f;
    } while (owner->unk54 > 1.0f);
}

void func_150B538C(void *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 5) {
        func_150B5060(arg0);
    }
}
