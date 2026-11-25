#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A8D60;

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

typedef struct TimedVec151A361C {
    f32 x;
    f32 y;
    f32 z;
} TimedVec151A361C;

typedef struct TimedEntry151A361C {
    TimedVec151A361C value0;
    f32 fieldC;
    s16 field10;
    s16 field12;
    u8 field14;
    u8 field15;
    u8 pad16[2];
} TimedEntry151A361C;

typedef struct TimedState151A361C {
    u8 *owner;
    u8 identity;
    u8 transformIndex;
    u8 pad06[2];
    TimedVec151A361C input08;
    f32 field14;
    s16 field18;
    s16 field1A;
    s16 field1C;
    u8 pad1E[2];
    f32 field20;
    f32 field24;
    u8 field28;
    u8 field29;
    s8 field2A;
    u8 pad2B[5];
    u8 flags30;
    u8 pad31[3];
    TimedVec151A361C previous34;
    f32 field40;
    u8 field44;
    u8 pad45[3];
    f32 field48;
    u8 field4C;
} TimedState151A361C;

typedef struct TimedObject151A361C {
    u8 pad0[0xC];
    u8 fieldC;
    u8 pad0D[3];
    TimedVec151A361C position10;
    u8 pad1C[2];
    u16 flags1E;
    u8 pad20[5];
    u8 field25;
    u8 pad26[6];
    s8 field2C;
    s8 field2D;
    s8 field2E;
    u8 pad2F[0x25];
    TimedVec151A361C output54;
    u8 pad60[0x34];
    TimedEntry151A361C *entries94;
    TimedState151A361C *state98;
} TimedObject151A361C;

s32 func_151A361C(TimedObject151A361C *arg0) {
    TimedState151A361C *state = arg0->state98;
    TimedEntry151A361C *entries = arg0->entries94;
    TimedEntry151A361C *entry;
    s32 index;

    if (arg0->field2C < 2 && (state->flags30 & 1)) {
        return 0;
    }
    state->field4C = (u32)state->field4C + (u32)(s32)state->field2A * (u32)D_800BE9E4;
    index = arg0->field2E;
    if (index != arg0->field2D) {
        do {
            index--;
            if (index < 0) {
                index = arg0->field25 - 1;
            }
            entry = &entries[index];
            if (entry->field12 > 0) {
                entry->field12 = (u32)(s32)entry->field12 - (u32)D_800BE9E4;
            } else {
                entry->field10 = (u32)(s32)entry->field10 - (u32)D_800BE9E4 * (u32)(s32)state->field1C;
            }
            entry->field14 = 0xFF;
            entry->fieldC += D_800BE9A4 * state->field24;
            if (entry->field10 < 0) {
                state->flags30 &= ~2;
                while (index != arg0->field2D) {
                    arg0->field2D++;
                    if (arg0->field2D == arg0->field25) {
                        arg0->field2D = 0;
                    }
                    arg0->field2C--;
                }
                entries[arg0->field2D].field10 = 0;
            }
        } while (index != arg0->field2D);
    }
    if (arg0->field2C > 0) {
        arg0->output54 = entries[arg0->field2D].value0;
    } else {
        arg0->output54.x = 0.0f;
        arg0->output54.y = 0.0f;
        arg0->output54.z = 0.0f;
    }
    return 1;
}

void func_151A4E9C(void *arg0);

s32 func_151A37C0(TimedObject151A361C *arg0) {
    TimedState151A361C *state = arg0->state98;
    TimedEntry151A361C *entries = arg0->entries94;
    u8 *owner;
    TimedVec151A361C delta;
    s32 found;
    u8 buf[0x30];

    owner = state->owner;
    if (*(s32 *)owner == 0) {
        return 0;
    }
    if (state->identity != owner[0x3B]) {
        return 0;
    }
    found = 0;
    if (!(arg0->flags1E & 4)) {
        if (func_151A4E34((TrailParams *)state, (Vec3f *)&arg0->position10) != 0) {
            found = 1;
            state->previous34 = arg0->position10;
            arg0->flags1E |= 4;
        } else {
            return 1;
        }
    }
    if (!found && func_151A4E34((TrailParams *)state, (Vec3f *)&arg0->position10) == 0) {
        func_151A4E9C(arg0);
        memcpy(buf, state, 0x30);
        {
            struct260 *ret = func_151491F4(0x12C, -1, 0xA, 0, 5, 0x30, arg0->fieldC, 0);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0x28, buf, 0x30);
            }
        }
        return 1;
    }
    delta.x = arg0->position10.x - state->previous34.x;
    delta.y = arg0->position10.y - state->previous34.y;
    delta.z = arg0->position10.z - state->previous34.z;
    if (fabsf(delta.x) > 0.0001f || fabsf(delta.y) > 0.0001f || fabsf(delta.z) > 0.0001f) {
        state->field40 += sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) * state->field14;
    }
    if (state->field40 > 1.0f) {
        f32 inv;
        f32 val48;
        f32 step48;
        f32 path;
        TimedVec151A361C cur;
        f32 sx;
        f32 sy;
        f32 diff;
        f32 sz;
        f32 pathStep;

        inv = 1.0f / state->field40;
        val48 = state->field48 + D_800BE9A4;
        cur = state->previous34;
        step48 = val48 * inv;
        path = state->field20 + state->field24 * val48;
        sx = delta.x * inv;
        sy = delta.y * inv;
        sz = delta.z * inv;
        diff = state->field20 - path;
        pathStep = diff * inv;
        do {
            TimedEntry151A361C *entry = &entries[arg0->field2E];
            entry->value0 = cur;
            entry->fieldC = path;
            entry->field10 = state->field18;
            entry->field12 = state->field1A;
            entry->field15 = state->field44;
            found = func_150ADA20(); state->field44 += state->field28 + (u32)found % (u32)(state->field29 + 1);
            entry->field14 = 0xFF;
            arg0->field2E++;
            if (arg0->field2E == arg0->field25) {
                arg0->field2E = 0;
            }
            arg0->field2C++;
            if (arg0->field2E == arg0->field2D) {
                arg0->field2D++;
                if (arg0->field2D == arg0->field25) {
                    arg0->field2D = 0;
                }
                arg0->field2C--;
            }
            path += pathStep;
            cur.x += sx;
            cur.y += sy;
            cur.z += sz;
            val48 -= step48;
            state->field40 -= 1.0f;
        } while (state->field40 > 1.0f);
        state->previous34 = cur;
        state->field48 = val48;
    }
    {
        typedef s32 T0;
        state = *(TimedState151A361C **)((u8 *)arg0 + 0x40); state = state;
    }
    {
        typedef s32 T1;
        state = *(TimedState151A361C **)((u8 *)arg0 + 0x44); state = state;
    }
    {
        typedef s32 T2;
        state = *(TimedState151A361C **)((u8 *)arg0 + 0x48); state = state;
    }
    {
        typedef s32 T3;
        state = *(TimedState151A361C **)((u8 *)arg0 + 0x4C); state = state;
    }
    {
        typedef s32 T4;
        state = *(TimedState151A361C **)((u8 *)arg0 + 0x50); state = state;
    }
    {
        typedef s32 T5;
        state = *(TimedState151A361C **)((u8 *)arg0 + 0x54); state = state;
    }
    {
        typedef s32 T6;
        state = *(TimedState151A361C **)((u8 *)arg0 + 0x58); state = state;
    }
    {
        typedef s32 T7;
        state = *(TimedState151A361C **)((u8 *)arg0 + 0x5C); state = state;
    }
    {
        typedef s32 T8;
        state = *(TimedState151A361C **)((u8 *)arg0 + 0x60); state = state;
    }
    return 1;
}

typedef struct {
    f32 x, y, z;
} Vec3f_151A3BE4;

typedef struct {
    Vec3f_151A3BE4 pos;
    f32 width;
    s16 unk10;
    u8 pad12[2];
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} Point151A3BE4;

typedef struct {
    u8 pad0[0x2B];
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 pad2F;
    u8 unk30;
    u8 pad31[0x4C - 0x31];
    u8 unk4C;
} Style151A3BE4;

typedef struct {
    u8 pad0[0x10];
    Vec3f_151A3BE4 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x84 - 0x2F];
    u8 unk84[0x10];
    Point151A3BE4 *points;
    Style151A3BE4 *style;
} Owner151A3BE4;

extern s32 D_800D2C9C;
extern u8 D_80090CD4[];
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

Gfx *func_151A3BE4(Owner151A3BE4 *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    f32 dx, dy, dz, ex, ey, ez;
    f32 nx, ny, nz, len, sc;
    u8 k;
    s32 idx;
    Vec3f_151A3BE4 b;
    Vec3f_151A3BE4 a;
    s32 j;
    s32 i;
    u8 prev;
    u8 lvl;
    u8 alphaPrev;
    u8 alpha;
    u8 update;
    f32 *cam;
    Point151A3BE4 *points;
    Style151A3BE4 *style;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->unk84, arg2, (arg0->unk25 + 5) * 2 * sizeof(Vtx), (void **)&vtx, 0);
        if (vtx != NULL) {
            style = arg0->style;
            cam = &D_800DBFF0[arg2].unk2F8;
            points = arg0->points;
            update = 1;
        fence:;
            if (style->unk30 & 2) {
                u8 k;
                s16 n;

                k = 0;
                n = style->unk2B;
                idx = arg0->unk2D;
                do {
                    points[idx].unk14 = k;
                    idx++;
                    k += style->unk2C;
                    if (idx == arg0->unk25) {
                        idx = 0;
                    }
                    n--;
                } while (n != 0 && idx != arg0->unk2E);
            }
            if (style->unk30 & 4) {
                u8 m;
                s16 n2;

                m = 0;
                n2 = style->unk2D;
                idx = arg0->unk2E - 1;
                if (idx < 0) {
                    idx = arg0->unk25 - 1;
                }
                do {
                    points[idx].unk14 = (points[idx].unk14 * m) >> 8;
                    idx--;
                    m += style->unk2E;
                    n2 = n2 - 1;
                    if (idx < 0) {
                        idx = arg0->unk25 - 1;
                    }
                } while (n2 != 0 && idx != arg0->unk2E);
            }
            gfx = func_15142E24(gfx, (s32)D_80090CD4, 0, 0, 0, 0, 0x1F, 0, 0, &update, 3);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &update);
            gfx = func_15142CF0(gfx, 0, 0, 0, 0, 0, 0, &update);
            gfx = func_1513F4E4(gfx, 6, &update);
            gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &update);
            if (arg0->unk1E & 2) {
                j = arg0->unk2E - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                a = arg0->pos;
                lvl = (points[j].unk15 + style->unk4C);
                alpha = (points[j].unk10 * points[j].unk14) >> 8;
            } else {
                i = arg0->unk2E - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                j = i - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                a = points[i].pos;
                lvl = (points[i].unk15 + style->unk4C);
                alpha = (points[i].unk10 * points[i].unk14) >> 8;
            }
            b = points[j].pos;
            prev = (points[j].unk15 + style->unk4C);
            alphaPrev = (points[j].unk10 * points[j].unk14) >> 8;
            ex = a.x - b.x;
            ey = a.y - b.y;
            ez = a.z - b.z;
            dx = a.x - cam[0];
            dy = a.y - cam[1];
            dz = a.z - cam[2];
            nx = ey * dz - dy * ez;
            ny = ez * dx - dz * ex;
            nz = ex * dy - dx * ey;
            len = nx * nx + ny * ny + nz * nz;
            if (len == 0.0f) {
                nz = ny = nx = 0.0f;
            } else {
                sc = points[j].width / sqrtf(len);
                nx = nx * sc;
                ny = ny * sc;
                nz = nz * sc;
            }
            *(s16 *)((u8 *)vtx + 0) = (s32)(a.x + nx);
            *(s16 *)((u8 *)vtx + 2) = (s32)(a.y + ny);
            *(s16 *)((u8 *)vtx + 4) = (s32)(a.z + nz);
            *(s16 *)((u8 *)vtx + 8) = lvl << 6;
            *(s16 *)((u8 *)vtx + 0xA) = 0x7C0;
            *((u8 *)vtx + 0xC) = 0xFF;
            *((u8 *)vtx + 0xD) = 0xFF;
            *((u8 *)vtx + 0xE) = 0xFF;
            *((u8 *)vtx + 0xF) = alpha;
            *(s16 *)((u8 *)vtx + 6) = 0;
            vtx++;
            *(s16 *)((u8 *)vtx + 0) = (s32)(a.x - nx);
            *(s16 *)((u8 *)vtx + 2) = (s32)(a.y - ny);
            *(s16 *)((u8 *)vtx + 4) = (s32)(a.z - nz);
            *(s16 *)((u8 *)vtx + 8) = lvl << 6;
            *(s16 *)((u8 *)vtx + 0xA) = 0;
            *((u8 *)vtx + 0xC) = 0xFF;
            *((u8 *)vtx + 0xD) = 0xFF;
            *((u8 *)vtx + 0xE) = 0xFF;
            *((u8 *)vtx + 0xF) = alpha;
            *(s16 *)((u8 *)vtx + 6) = 0;
            vtx++;
            do {
                ex = a.x - b.x;
                ey = a.y - b.y;
                ez = a.z - b.z;
                dx = b.x - cam[0];
                dy = b.y - cam[1];
                dz = b.z - cam[2];
                nx = ey * dz - dy * ez;
                ny = ez * dx - dz * ex;
                nz = ex * dy - dx * ey;
                len = nx * nx + ny * ny + nz * nz;
                if (len == 0.0f) {
                    nz = ny = nx = 0.0f;
                } else {
                    sc = points[j].width / sqrtf(len);
                    nx = nx * sc;
                    ny = ny * sc;
                    nz = nz * sc;
                }
                *(s16 *)((u8 *)vtx + 0) = (s32)(b.x + nx);
                *(s16 *)((u8 *)vtx + 2) = (s32)(b.y + ny);
                *(s16 *)((u8 *)vtx + 4) = (s32)(b.z + nz);
                *(s16 *)((u8 *)vtx + 8) = prev << 6;
                *(s16 *)((u8 *)vtx + 0xA) = 0x7C0;
                *((u8 *)vtx + 0xC) = 0xFF;
                *((u8 *)vtx + 0xD) = 0xFF;
                *((u8 *)vtx + 0xE) = 0xFF;
                *((u8 *)vtx + 0xF) = alphaPrev;
                *(s16 *)((u8 *)vtx + 6) = 0;
                vtx++;
                *(s16 *)((u8 *)vtx + 0) = (s32)(b.x - nx);
                *(s16 *)((u8 *)vtx + 2) = (s32)(b.y - ny);
                *(s16 *)((u8 *)vtx + 4) = (s32)(b.z - nz);
                *(s16 *)((u8 *)vtx + 8) = prev << 6;
                *(s16 *)((u8 *)vtx + 0xA) = 0;
                *((u8 *)vtx + 0xC) = 0xFF;
                *((u8 *)vtx + 0xD) = 0xFF;
                *((u8 *)vtx + 0xE) = 0xFF;
                *((u8 *)vtx + 0xF) = alphaPrev;
                *(s16 *)((u8 *)vtx + 6) = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (lvl < prev) {
                    memcpy(vtx, vtx - 2, 0x20);
                    *(s16 *)((u8 *)vtx - 0x18) -= 0x4000;
                    vtx++;
                    *(s16 *)((u8 *)vtx - 0x18) -= 0x4000;
                    vtx++;
                }
                i = j;
                j--;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                if (i != arg0->unk2D) {
                    lvl = prev;
                    a = b;
                    b = points[j].pos;
                    prev = (points[j].unk15 + style->unk4C);
                    alphaPrev = (points[j].unk10 * points[j].unk14) >> 8;
                }
            } while (i != arg0->unk2D);
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

void func_151A4638(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, void *arg6) {
    struct260 *ret;
    struct {
        s16 sp28[6];
        u8 pad34[4];
        struct {
            s32 unk00;
            s32 unk04;
            s16 unk08;
            s16 unk0A;
            s32 unk0C;
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
            f32 unk30;
            f32 unk34;
            f32 unk38;
            u8 pad3C[0xC];
            f32 unk48;
            f32 unk4C;
            f32 unk50;
            f32 unk54;
            s32 unk58;
            u8 pad5C[4];
            u8 unk60;
            u8 unk61;
            u8 unk62;
            s8 unk63;
        } sp38;
        u8 pad9C[8];
    } stack;

    stack.sp38.unk1D = 0x27;
    stack.sp38.unk08 = 0x1401;
    stack.sp38.unk00 = 0x200005;
    stack.sp38.unk04 = 0;
    stack.sp38.unk0A = (func_150ADA20() % 5U) + 0xF;
    stack.sp38.unk0C = 0;
    stack.sp38.unk10 = 0;
    stack.sp38.unk14 = 0x8A;
    stack.sp38.unk15 = 0;
    stack.sp38.unk16 = 0;
    stack.sp38.unk17 = 0xFF;
    stack.sp38.unk18 = 0;
    stack.sp38.unk19 = 0;
    stack.sp38.unk1A = 0;
    stack.sp38.unk1E = 1;
    stack.sp38.unk20 = 0xFF;
    stack.sp38.unk22 = 1;
    stack.sp38.unk24 = 1.0f;

    stack.sp38.unk28 = stack.sp38.unk2C = (func_150ADA68() * 50.0f) + 500.0f;
    stack.sp38.unk30 = arg0;
    stack.sp38.unk34 = arg1;
    stack.sp38.unk38 = arg2;
    stack.sp38.unk48 = -arg3 * (D_800A8D60 * D_800BE9A8);
    stack.sp38.unk4C = -arg4 * (D_800A8D60 * D_800BE9A8);
    stack.sp38.unk50 = -arg5 * (D_800A8D60 * D_800BE9A8);
    stack.sp38.unk58 = 0xD;
    stack.sp38.unk60 = 1;
    stack.sp38.unk61 = 1;
    stack.sp38.unk54 = 0.0f;

    if (func_150ADA20() & 1) {
        stack.sp38.unk58 |= 0x40;
    }

    if (func_150ADA20() & 1) {
        stack.sp38.unk58 |= 0x80;
    }

    stack.sp38.unk1B = 0xFF;
    stack.sp38.unk1C = 0xFF;
    stack.sp38.unk62 = 0;
    stack.sp38.unk63 = -1;
    stack.sp28[0] = 0x10;
    stack.sp28[1] = 0xF;
    stack.sp28[2] = 0xD;
    stack.sp28[3] = 0x13;
    stack.sp28[4] = 0x11;
    stack.sp28[5] = -0x18;

    ret = func_15130374(&stack.sp38, 0, 0xC, *((u8 *)arg6 + 0xC), 1);
    if (ret != 0) {
        memcpy((u8 *)ret + 0xA8, stack.sp28, 0xC);
    }
}

struct Sub151A483C {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
};

struct Obj151A483C {
    u8 pad0[0x18];
    s16 unk18;
    s16 unk1A;
    u8 pad1C[0x2B - 0x1C];
    u8 unk2B;
    u8 unk2C;
    u8 pad2D[0x38 - 0x2D];
    f32 unk38;
    f32 unk3C;
    u8 pad40[0x70 - 0x40];
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 pad73[0xA8 - 0x73];
    struct Sub151A483C unkA8;
};

s32 func_151A483C(struct Obj151A483C *arg0, u8 arg1) {
    s16 temp_v1;
    struct Sub151A483C *temp_v0;
    f32 temp_f0;

    temp_v0 = &arg0->unkA8;
    temp_v1 = arg0->unk1A;
    if (temp_v1 < temp_v0->unk4) {
        arg0->unk2B = (u32)temp_v1 * (u32)temp_v0->unk6;
    }
    if (temp_v1 < temp_v0->unk8) {
        temp_f0 = (f32)(temp_v0->unkA * D_800BE9E4);
        *(volatile f32 *)&arg0->unk38 = arg0->unk38 + temp_f0;
        *(volatile f32 *)&arg0->unk3C = arg0->unk3C + temp_f0;
        temp_v1 = *(volatile s16 *)&arg0->unk1A;
    }
    if (temp_v1 < temp_v0->unk0) {
        arg0->unk72 = 1;
        arg0->unk70 = 2;
        arg0->unk71 = 2;
        arg0->unk2C = (u32)temp_v1 * (u32)temp_v0->unk2;
        arg0->unk18 = 0x5203;
    }
    return 1;
}

s32 func_151A4900(struct Obj151A483C *arg0, u8 arg1) {
    s16 temp_v1;
    struct Sub151A483C *temp_v0;

    temp_v0 = &arg0->unkA8;
    temp_v1 = arg0->unk1A;
    if (temp_v1 < temp_v0->unk4) {
        arg0->unk2B = (u32)temp_v1 * (u32)temp_v0->unk6;
    }
    if (temp_v1 < temp_v0->unk8) {
        {
            f32 temp_f0 = (f32)(temp_v0->unkA * D_800BE9E4);

            arg0->unk38 = arg0->unk38 + temp_f0;
            arg0->unk3C = arg0->unk3C + temp_f0;
        }
        temp_v1 = arg0->unk1A;
    }
    arg0->unk2C = (u32)temp_v1 * (u32)temp_v0->unk2;
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
