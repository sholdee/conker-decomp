#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern s32 func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
} Vec151B6320;

typedef struct {
    Vec151B6320 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} Header151B6320;

typedef struct {
    f32 *unk0;
    u8 unk4;
    u8 pad5[3];
    Vec151B6320 unk8;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28;
} Payload151B6320;

typedef struct {
    s32 unk0;
    u8 pad4[0x14 - 0x4];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    u8 pad2C[0x38 - 0x2C];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} struct151B76CCObj;

typedef struct {
    u8 pad0[4];
    struct151B76CCObj **unk4;
} struct151B76CCList;

typedef struct {
    u8 pad0[0x98];
    struct151B76CCList *unk98;
} struct151B76CCArg0;

void func_151B6320(f32 *arg0, u8 arg1, s32 arg2) {
    s32 top_dummy;
    Header151B6320 header;
    s32 pad_dummy;
    Payload151B6320 payload;
    s32 *temp_v0;

    header.unk15 = 0xA;
    header.unk0.unk0 = arg0[5];
    header.unk0.unk4 = arg0[6];
    header.unk0.unk8 = arg0[7];
    header.unkC = 0x12C;
    header.unkE = 6;
    payload.unk0 = arg0;
    payload.unk4 = *(u8 *)((u8 *)arg0 + 0x3B);
    payload.unk8 = header.unk0;
    payload.unk14 = 0.0f;
    payload.unk18 = 0.0f;
    payload.unk20 = -16384.0f;
    payload.unk1C = -16384.0f;
    payload.unk24 = 0.0f;
    header.unk10 = 0xD;

    temp_v0 = (s32 *)func_15147A80(&header, 0x30, 0x1C, 0xB, 0xB, 0xB, 0, 0, 0, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((void *)temp_v0[0x98 / 4], &payload, 0x2C);
    }
}

typedef struct { f32 x, y, z; } Vec151B65D4;

typedef struct {
    s32 unk0;
    u8 pad4[0x14 - 0x4];
    Vec151B65D4 pos;
    u8 pad20[0x3B - 0x20];
    u8 unk3B;
    f32 unk3C;
} Target151B65D4;

typedef struct {
    Target151B65D4 *target;
    u8 unk4;
    u8 pad5[3];
    Vec151B65D4 pos;
    f32 count;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} State151B65D4;

typedef struct {
    Vec151B65D4 pos;
    f32 unkC;
    u8 unk10;
    u8 pad11[3];
    f32 unk14;
    f32 unk18;
} Entry151B65D4;

typedef struct {
    u8 pad0[0x10];
    Vec151B65D4 pos;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 tail;
    s8 head;
    u8 pad2F;
    u8 unk30;
    u8 pad31[0x54 - 0x31];
    Vec151B65D4 unk54;
    u8 pad60[0x94 - 0x60];
    Entry151B65D4 *entries;
    State151B65D4 *state;
} Obj151B65D4;

s32 func_151B6420(Obj151B65D4 *arg0) {
    Entry151B65D4 *entries;
    s32 i;
    f32 base;
    s32 v;
    f32 d;

    entries = arg0->entries;
    if (arg0->count < 2 && (arg0->flags & 8)) {
        return 0;
    }
    i = arg0->head;
    while (i != arg0->tail) {
        i--;
        if (i < 0) {
            i = arg0->capacity - 1;
        }
        (i + entries)->unkC -= D_800BE9A4;
        if ((i + entries)->unkC < 0.0f) {
            while (i != arg0->tail) {
                arg0->tail++;
                if (arg0->tail == arg0->capacity) {
                    arg0->tail = 0;
                }
                arg0->count--;
            }
        }
    }
    if (arg0->count > 0) {
        i = arg0->tail;
        base = entries[arg0->tail].unk18;
        do {
            d = (i + entries)->unk18 - base;
            v = d * 0.4940000176f;
            if (v >= 0x9C) {
                (i + entries)->unk10 = 0x9B;
            } else {
                (i + entries)->unk10 = v;
            }
            i++;
            if (i >= arg0->capacity) {
                i = 0;
            }
        } while (i != arg0->head);
    }
    if (arg0->count > 0) {
        arg0->unk54 = entries[arg0->tail].pos;
    } else {
        arg0->unk54.x = 0.0f;
        arg0->unk54.y = 0.0f;
        arg0->unk54.z = 0.0f;
    }
    return 1;
}

f32 func_15143E64(void *);

s32 func_151B65D4(Obj151B65D4 *arg0) {
    State151B65D4 *state;
    Entry151B65D4 *entries;
    Target151B65D4 *target;
    Vec151B65D4 delta;
    s32 p0;
    s32 p1;
    f32 dist;

    state = arg0->state;
    entries = arg0->entries;
    target = state->target;
    if (target->unk0 == 0 || target->unk3B != state->unk4 || target->unk3C < 15.0f) {
        arg0->unk30 = 0;
        arg0->flags |= 8;
        return 1;
    }
    arg0->pos.x = target->pos.x;
    arg0->pos.y = target->pos.y;
    arg0->pos.z = target->pos.z;
    delta.x = arg0->pos.x - state->pos.x;
    delta.y = arg0->pos.y - state->pos.y;
    delta.z = arg0->pos.z - state->pos.z;
    dist = func_15143E64(&delta);
    state->count += dist * 0.003721999936f * D_800BE9A4;
    state->unk1C += dist * 4.409547806f;
    if (state->count > 1.0f) {
        Entry151B65D4 *e;
        Vec151B65D4 cur;
        f32 inv;
        f32 y;
        f32 dy;
        f32 a;
        f32 b;
        f32 dx;
        f32 dz;
        f32 dw;
        f32 diff;
        f32 da;
        inv = 1.0f / state->count;
        y = state->unk18 + D_800BE9A4;
        cur = state->pos;
        a = state->unk20;
        b = state->unk24;
        dy = -(y * inv);
        dx = delta.x * inv;
        dz = delta.y * inv;
        dw = delta.z * inv;
        diff = state->unk1C - state->unk20;
        da = diff * inv;
        do {
            e = &entries[arg0->head];
            e->pos = cur;
            e->unk10 = 0x9B;
            e->unk14 = a;
            e->unkC = 13.0f - y;
            y += dy;
            while (e->unk14 > 16384.0f) {
                e->unk14 -= 32768.0f;
            }
            e->unk18 = b;
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
            cur.x += dx;
            cur.y += dz;
            cur.z += dw;
            a += da;
            b += dist * inv;
            state->count -= 1.0f;
        } while (state->count > 1.0f);
        state->pos = cur;
        state->unk20 = a;
        state->unk24 = b;
        state->unk18 = y;
    }
    return 1;
}

typedef struct Trail151B6928 {
    struct17 pos;
    u8 pad0C[4];
    u8 alpha;
    u8 pad11[3];
    f32 rot;
    u8 pad18[4];
} Trail151B6928;

typedef struct Actor151B6928 {
    u8 pad0[0x10];
    struct17 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x55];
    u8 buffers[0x10];
    Trail151B6928 *entries;
} Actor151B6928;

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern struct17 *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090CD4[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

Gfx *func_151B6928(Actor151B6928 *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    Trail151B6928 *entries;
    struct17 *cam;
    u8 update;
    struct17 b;
    struct17 a;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 ex;
    f32 ey;
    f32 ez;
    f32 cx;
    f32 cy;
    f32 cz;
    s32 i;
    s32 j;
    f32 len[1];
    u8 alphaA;
    f32 rotB;
    f32 rotA;
    u8 alphaB;
    f32 scale;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->buffers, arg2, (arg0->unk25 + 5) * 2 * sizeof(Vtx), (void **)&vtx, 0);
        if (vtx != NULL) {
            entries = arg0->entries;
            cam = func_15144B34(arg2);
            update = 1;
            gfx = func_15142E24(gfx, (s32)D_80090CD4, 0, 0, 0, 0, 0x1F, 0, 0, &update, 3);
            gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &update);
            gfx = func_1513F4E4(gfx, 0x4E, &update);
            gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
            gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &update);
            if (arg0->unk1E & 2) {
                i = arg0->unk2E - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                a = arg0->pos;
                alphaA = 0x80;
                rotA = 0.0f;
            } else {
                j = arg0->unk2E - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                i = j - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                a = entries[j].pos;
                alphaA = entries[j].alpha;
                rotA = entries[j].rot;
            }
            b = entries[i].pos;
            alphaB = entries[i].alpha;
            rotB = entries[i].rot;

            ex = a.unk0 - b.unk0;
            ey = a.unk4 - b.unk4;
            ez = a.unk8 - b.unk8;
            dx = a.unk0 - cam->unk0;
            dy = a.unk4 - cam->unk4;
            dz = a.unk8 - cam->unk8;
            cx = (ey * dz) - (dy * ez);
            cy = (ez * dx) - (dz * ex);
            cz = (ex * dy) - (dx * ey);
            len[0] = (cx * cx) + (cy * cy) + (cz * cz);
            if ((cx * cx) + (cy * cy) + (cz * cz) == 0) {
                cz = cy = cx = 0.0f;
            } else {
                scale = 10.0f / sqrtf(len[0]);
                cx = cx * scale;
                cy = cy * scale;
                cz = cz * scale;
            }
            vtx->v.ob[0] = a.unk0 + cx;
            vtx->v.ob[1] = a.unk4 + cy;
            vtx->v.ob[2] = a.unk8 + cz;
            vtx->v.tc[0] = rotA;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = alphaA;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = a.unk0 - cx;
            vtx->v.ob[1] = a.unk4 - cy;
            vtx->v.ob[2] = a.unk8 - cz;
            vtx->v.tc[0] = rotA;
            vtx->v.tc[1] = 0x7FF;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = alphaA;
            vtx->v.flag = 0;
            vtx++;
            do {
                j = i;
                ex = a.unk0 - b.unk0;
                ey = a.unk4 - b.unk4;
                ez = a.unk8 - b.unk8;
                dx = b.unk0 - cam->unk0;
                dy = b.unk4 - cam->unk4;
                dz = b.unk8 - cam->unk8;
                cx = (ey * dz) - (dy * ez);
                cy = (ez * dx) - (dz * ex);
                cz = (ex * dy) - (dx * ey);
                len[0] = (cx * cx) + (cy * cy) + (cz * cz);
                if ((cx * cx) + (cy * cy) + (cz * cz) == 0) {
                    cz = cy = cx = 0.0f;
                } else {
                    scale = 10.0f / sqrtf(len[0]);
                    cx = cx * scale;
                    cy = cy * scale;
                    cz = cz * scale;
                }
                vtx->v.ob[0] = b.unk0 + cx;
                vtx->v.ob[1] = b.unk4 + cy;
                vtx->v.ob[2] = b.unk8 + cz;
                vtx->v.tc[0] = rotB;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alphaB;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = b.unk0 - cx;
                vtx->v.ob[1] = b.unk4 - cy;
                vtx->v.ob[2] = b.unk8 - cz;
                vtx->v.tc[0] = rotB;
                vtx->v.tc[1] = 0x7FF;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alphaB;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                i--;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                if (j != arg0->unk2D) {
                    a = b;
                    b = entries[i].pos;
                    alphaB = entries[i].alpha;
                    rotB = entries[i].rot;
                }
            } while (j != arg0->unk2D);
        }
    }
    return gfx;
}


void func_151B70B4(void *arg0, void *arg1, u8 arg2) {
    void *target;
    s32 current;
    s32 value;

    target = *(void **)((u8 *)arg0 + 0x98);
    current = *(s32 *)target;
    if (arg2 == 0) {
        if (current == *(s32 *)arg1) {
            u16 *flags = (u16 *)((u8 *)arg0 + 0x1E);

            *(s8 *)((u8 *)arg0 + 0x30) = 0;
            *flags |= 8;
        }
    } else if (arg2 == 0x2D) {
        value = *(s32 *)arg1;
        if (value == *(s32 *)target) {
            *(s32 *)target = *(s32 *)((u8 *)arg1 + 4);
            *(u8 *)((u8 *)target + 4) = *(u8 *)((u8 *)arg1 + 9);
            return;
        }
        if (*(s32 *)((u8 *)arg1 + 4) == *(s32 *)target) {
            *(s32 *)target = value;
            *(u8 *)((u8 *)target + 4) = *(u8 *)((u8 *)arg1 + 8);
        }
    }
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
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    s16 unk68;
    f32 unk6C;
} Spawn151B7144;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151B7144(f32 *arg0, u8 arg1, s32 arg2) {
    Spawn151B7144 spawn;
    s32 temp_v0;
    s32 temp_v1;

    spawn.unk1D = 0x29;
    spawn.unk8 = 0xE03;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk1E = 0x12;
    spawn.unk20 = 0xE;
    if (func_150ADA20() & 1) {
        temp_v1 = 0x40;
    } else {
        temp_v1 = 0;
    }
    if (func_150ADA20() & 1) {
        temp_v0 = 0x80;
    } else {
        temp_v0 = 0;
    }
    spawn.unk58 = temp_v0 | 1 | temp_v1 | 0x4000 | 0x8000 | 0x200 | 0x400 | 0x800 | 0x10000;
    spawn.unk60 = 3;
    spawn.unk61 = 3;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk22 = 0x28;
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
    spawn.unk30.unk0 = arg0[5];
    spawn.unk30.unk4 = arg0[6];
    spawn.unk30.unk8 = arg0[7];
    spawn.unk3C.unk0 = 0.0f;
    spawn.unk3C.unk4 = 0.0f;
    spawn.unk3C.unk8 = 0.0f;
    spawn.unk1B = func_150ADA20() % 56U + 0xC8;
    spawn.unkA = func_150ADA20() % 10U + 0x1E;
    spawn.unk28 = spawn.unk2C = func_150ADA68() * 50.0f + 89.0f;
    spawn.unk48.unk0 = 0.0f;
    spawn.unk48.unk4 = 0.0f;
    spawn.unk48.unk8 = 0.0f;
    func_15130280(&spawn, 1, 0, 0, arg1, arg2);
}

typedef struct {
    struct17 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} Header151B7328;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 pad9[3];
} Payload151B7328;

typedef struct {
    s32 unk0;
    void *unk4;
} Data151B7328;

typedef struct {
    u8 pad0[0x98];
    Data151B7328 *unk98;
} Obj151B7328;

extern s32 (*D_8008FB90[])();

void *func_151B7328(void *arg0, u8 arg1, s32 arg2, u8 arg3, s32 arg4) {
    Obj151B7328 *obj;
    Header151B7328 header;
    s32 pad;
    Payload151B7328 payload;
    s32 *data;
    Spawn151B7144 spawn;

    payload.unk0 = 0;
    payload.unk4 = 0;
    payload.unk8 = arg1;
    header.unk15 = 0x14;
    header.unk0 = *(struct17 *)&D_800A5480;
    header.unkC = 0x12C;
    header.unkE = 0x14;
    header.unk10 = 0x10;
    header.unk14 = 2;
    obj = (Obj151B7328 *)func_15147A80(&header, arg2 + 0x10, 0x14, 0, 0xE, 0xE, 0, 0, 0, arg3, arg4);
    if (obj != NULL) {
        data = (s32 *)obj->unk98;
        memcpy(data, &payload, 0xC);
        data[1] = (s32)(data + 4);
        memcpy((void *)data[1], arg0, arg2);
        if (D_8008FB90[arg1](obj, (u8 *)obj + 0x10) == 0) {
            func_1516972C((struct102 *)obj);
            return NULL;
        }
        {
        const s32 colors[4] = { 0x60, 0x61, 0x62, 0x63 };
        s32 temp_v0;
        s32 temp_v1;
        spawn.unk1D = colors[func_150ADA20() & 3];
        spawn.unk8 = 0x1303;
        spawn.unk0 = 0x200005;
        spawn.unk4 = 0;
        spawn.unkA = 0x12C;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk14 = 0xFF;
        spawn.unk15 = 0xFF;
        spawn.unk16 = 0xFF;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0xFF;
        spawn.unk19 = 0xFF;
        spawn.unk1A = 0xFF;
        spawn.unk1B = 0xFF;
        spawn.unk1C = 0xFF;
        spawn.unk28 = spawn.unk2C = func_150ADA68() * 800.0f + 1700.0f;
        spawn.unk30 = header.unk0;
        spawn.unk3C = *(struct17 *)&D_800A5480;
        spawn.unk48 = *(struct17 *)&D_800A5480;
        spawn.unk1E = 1;
        spawn.unk20 = 0xFF;
        spawn.unk22 = 1;
        spawn.unk54 = 0.0f;
        spawn.unk24 = 1.0f;
        if (func_150ADA20() & 1) {
            temp_v1 = 0x40;
        } else {
            temp_v1 = 0;
        }
        if (func_150ADA20() & 1) {
            temp_v0 = 0x80;
        } else {
            temp_v0 = 0;
        }
        spawn.unk58 = temp_v0 | temp_v1 | 0x4000 | 0x8000 | 0x40000;
        spawn.unk60 = 6;
        spawn.unk61 = 5;
        spawn.unk62 = -1;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk5C = 0;
        spawn.unk66 = 0xFF;
        data[0] = (s32)func_15130280(&spawn, 1, 0, 0, arg3, arg4);
        }
    }
    return obj;
}

s32 func_151B7678(struct151B76CCArg0 *arg0, f32 *arg1) {
    struct151B76CCList *list;
    struct151B76CCObj **entry;
    struct151B76CCObj *obj;

    list = arg0->unk98;
    entry = list->unk4;
    obj = *entry;
    if ((obj->unk0 == 0) || (*((u8 *) entry + 4) != *((u8 *) obj + 0x3B))) {
        return 0;
    }

    arg1[0] = obj->unk14;
    arg1[1] = obj->unk18;
    arg1[2] = obj->unk1C;
    return 1;
}

s32 func_151B76CC(struct151B76CCArg0 *arg0, f32 *arg1) {
    struct151B76CCList *list;
    struct151B76CCObj **objPtr;
    struct151B76CCObj *obj;
    f32 mtx[4][4];

    list = arg0->unk98;
    objPtr = list->unk4;
    obj = *objPtr;
    func_150A8050(mtx, obj->unk20, obj->unk24, obj->unk28);
    mtx[3][0] = obj->unk38;
    mtx[3][1] = obj->unk3C;
    mtx[3][2] = obj->unk40;
    mtx[0][0] *= obj->unk18;
    mtx[0][1] *= obj->unk18;
    mtx[0][2] *= obj->unk18;
    mtx[1][0] *= obj->unk1C;
    mtx[1][1] *= obj->unk1C;
    mtx[1][2] *= obj->unk1C;
    mtx[2][0] *= obj->unk18;
    mtx[2][1] *= obj->unk18;
    mtx[2][2] *= obj->unk18;
    func_150A7960(mtx[0], 0.0f, 0.0f, -250.0f, arg1, arg1 + 1, arg1 + 2);
    return 1;
}

typedef struct { f32 x, y, z; } RingVec3f;

typedef struct Game1E37D0RingEntry {
    RingVec3f position;
    f32 distance;
    u8 state;
    u8 pad11[3];
} Game1E37D0RingEntry;

typedef struct Game1E37D0RingState {
    u8 *linked;
    u8 pad4[4];
    u8 selector;
} Game1E37D0RingState;

typedef struct Game1E37D0RingObject {
    u8 pad0[0x10];
    RingVec3f position;
    u8 pad1C[9];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 tail;
    s8 head;
    u8 pad2F[0x65];
    Game1E37D0RingEntry *entries;
    Game1E37D0RingState *state;
} Game1E37D0RingObject;

extern s32 (*D_8008FB90[])(Game1E37D0RingObject *, RingVec3f *);
f32 func_15143E64(void *);

s32 func_151B77F4(Game1E37D0RingObject *arg0) {
    Game1E37D0RingState *state;
    Game1E37D0RingEntry *entries;
    RingVec3f previous;
    RingVec3f delta;
    f32 distance;

    state = arg0->state;
    entries = arg0->entries;
    if (D_800BE9E4 > 0) {
        previous = arg0->position;
        if (D_8008FB90[state->selector](arg0, &arg0->position) == 0) {
            return 0;
        }
        if (state->linked != 0) {
            *(RingVec3f *)(state->linked + 0x40) = arg0->position;
        }
        delta.x = arg0->position.x - previous.x;
        delta.y = arg0->position.y - previous.y;
        delta.z = arg0->position.z - previous.z;
        distance = func_15143E64(&delta);
        {
            Game1E37D0RingEntry *entry = &entries[arg0->head];

            entry->position = arg0->position;
            entry->distance = distance;
            entry->state = 0xFF;
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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B7998.s")

extern u8 D_80091100[];

Gfx *func_151B7C38(Game1E37D0RingObject *arg0, Gfx *arg1, s16 arg2) {
    RingVec3f next;
    RingVec3f cur;
    s32 index;
    s32 previous;
    RingVec3f *camera;
    Game1E37D0RingEntry *entries;
    RingVec3f toCamera;
    RingVec3f delta;
    RingVec3f normal;
    f32 lengthSquared;
    u8 flag;
    u8 curAlpha;
    u8 nextAlpha;
    Vtx_t *vtx;

    if (arg0->count < 2) {
        return arg1;
    }
    entries = arg0->entries;
    func_151D5D60((u8 *)arg0 + 0x84, arg2, (arg0->capacity * 2 + 10) * sizeof(Vtx_t), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return arg1;
    }
    camera = (RingVec3f *)func_15144B34(arg2);
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
    cur = entries[previous].position;
    curAlpha = entries[previous].state;
    next = entries[index].position;
    nextAlpha = entries[index].state;
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
            cur = entries[previous].position;
            next = entries[index].position;
            nextAlpha = entries[index].state;
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

void func_151B8370(void *arg0) {
    s32 v0 = *(s32 *)((u8 *)arg0 + 0x98);
    struct102 *v = *(struct102 **)v0;
    if (v != 0) {
        func_1516972C(v);
    }
}

extern void func_151B8370(void *);
extern void func_151478F4(void *);

void func_151B83A0(void *arg0) {
    func_151B8370(arg0);
    func_151478F4(arg0);
}

extern void func_15147928(void *);

void func_151B83CC(void *arg0) {
    func_151B8370(arg0);
    func_15147928(arg0);
}
