#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern s32 func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

struct Struct15147DA0_cpy { s32 unk0[8]; };

struct Struct15148AF4_entry {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    s16 unk12;
};

struct Struct15148AF4_data {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    u8 pad14[4];
    u8 unk18;
};

struct Struct15148AF4_arg0 {
    u8 pad0[0x25];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x65];
    struct Struct15148AF4_entry *unk94;
    struct Struct15148AF4_data *unk98;
};

s32 func_15147DA0(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 *arg11,
                  s32 arg12, u8 arg13, s32 arg14) {
    s32 v1;
    u8 *temp;

    *(s32 *)((u8 *)arg0 + 0x10) = 1;
    v1 = func_15147A80(arg0, (s32)arg2 + 0x48, 0x14, 1, 0, 1, arg9, arg10,
                       arg12, arg13, arg14);
    if (v1 == 0) {
        return 0;
    }
    temp = (u8 *)*(s32 *)((u8 *)v1 + 0x98);
    memcpy(temp, arg1, 0x20);
    temp[0x20] = arg3;
    temp[0x21] = arg4;
    temp[0x22] = arg5;
    temp[0x23] = arg6;
    temp[0x24] = arg7;
    temp[0x25] = arg8;
    *(struct Struct15147DA0_cpy *)(temp + 0x28) = *(struct Struct15147DA0_cpy *)arg11;
    return v1;
}


typedef struct {
    f32 x, y, z;
} Vec3f_15147EB8;

typedef struct { Vec3f_15147EB8 v; u8 pad[8]; } Ent15147EB8;

typedef struct {
    u8 pad0[0x18];
    u8 unk18;
    u8 pad19[2];
    u8 unk1B;
    s16 unk1C;
    s16 unk1E;
    u8 unk20;
    u8 unk21;
    u8 unk22;
} Data15147EB8;

typedef struct {
    u8 pad0[0x1C];
    s16 unk1C;
    u8 pad1E[0xE];
    s8 unk2C;
    s8 unk2D;
    u8 pad2E[0x26];
    Vec3f_15147EB8 unk54;
    u8 pad60[0x34];
    Ent15147EB8 *unk94;
    Data15147EB8 *unk98;
} Obj15147EB8;

typedef s32 (*Func15147EB8)();
extern Func15147EB8 D_8008A3E0[];
extern Func15147EB8 D_8008A3F8[];
extern Func15147EB8 D_8008A42C[];

s8 func_15147EB8(Obj15147EB8 *arg0) {
    Data15147EB8 *data;
    s8 done;
    Ent15147EB8 *ent;
    s16 val;
    s32 prod;
    s32 ret;

    data = arg0->unk98;
    done = 0;
    if (data->unk20 != 0) {
        if (D_8008A3E0[data->unk20]() == 0) {
            done = 1;
        }
    }
    if (data->unk21 != 0 && !done) {
        if (D_8008A3F8[data->unk21]() == 0) {
            done = 1;
        }
    }
    if ((data->unk18 & 0x40) && !done) {
        val = arg0->unk1C;
        if (val < data->unk1C) {
            prod = val * data->unk1E;
            if (prod < data->unk1B) {
                data->unk1B = prod;
            }
        }
    }
    ret = !done;
    if (done) {
        if (data->unk22 != 0) {
            done = D_8008A42C[data->unk22](arg0);
        }
    }
    ent = arg0->unk94;
    if (arg0->unk2C > 0) {
        arg0->unk54 = ent[arg0->unk2D].v;
    } else {
        arg0->unk54.x = 0.0f;
        arg0->unk54.y = 0.0f;
        arg0->unk54.z = 0.0f;
    }
    return ret;
}


typedef struct {
    struct17 pos;
    f32 unkC;
    u16 unk10;
    u8 pad12[2];
} Entry1514803C;

typedef struct {
    f32 unk0;
    u8 pad4[0x14];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[0xC];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[3];
    u8 unk3B;
    s32 unk3C;
    s32 unk40;
    s8 unk44;
    s8 unk45;
} Data1514803C;

typedef struct {
    u8 pad0[0x25];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x55];
    u8 buffers[0x10];
    Entry1514803C *entries;
    Data1514803C *data;
} Actor1514803C;

void func_151D5D60(void *, s16, s32, void **, u8 *);
extern struct17 *func_15144B34(s32);
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

Gfx *func_1514803C(Actor1514803C *arg0, Gfx *gfx, s16 arg2) {
    Data1514803C *data;
    Vtx *vtx;
    u8 update;
    struct17 b;
    struct17 a;
    s32 i;
    s32 j;
    struct17 *cam;
    Entry1514803C *entries;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 ex;
    f32 ey;
    f32 ez;
    f32 cx;
    f32 cy;
    f32 cz;
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

    data = arg0->data;
    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->buffers, arg2, (arg0->unk25 - 1) * 2 * sizeof(Vtx), (void **)&vtx, 0);
        if (vtx == NULL) {
            return gfx;
        }
        if (vtx != NULL) {
            if (D_800BE9C0 != 0) {
                vtx += (arg0->unk25 - 1) * 2;
            }
            update = 1;
            cam = func_15144B34(arg2);
            entries = arg0->entries;
            tex = func_1514306C(0, data->unk19, 0, 2);
            if (tex != D_800DD1B0) {
                D_800915A4 = D_80091564[data->unk19];
                gfx = func_150950D4(gfx, &D_800915A4, 0, 0, 0, 0, 2, 0x100, 0x100, (data->unk18 & 0x80) ? 0x3E : 3);
                D_800DD1B0 = tex;
            }
            func_151441A4(&envR, &envG, &envB, &envA, 0, 0, 0, 0, 0, 0, 0, data->unk1B, data->unk1A, data->unk44);
            func_151442FC(&primR, &primG, &primB, &primA, 0, 0, 0, 0, 0, 0, 0, data->unk1B, data->unk1A, data->unk45);
            gfx = func_15142B7C(gfx, data->unk2C, data->unk30);
            gfx = func_15142C10(gfx, primR, primG, primB, primA, &update);
            gfx = func_15142CF0(gfx, 0, 0, envR, envG, envB, envA, &update);
            gfx = func_1513F4E4(gfx, data->unk3B, &update);
            gfx = func_15142FBC(gfx, data->unk28 | G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | data->unk3C | data->unk40, D_800A4AC8[data->unk34].unk0 | D_800A4AC8[data->unk34].unk4, &update);
            i = arg0->unk2E - 1;
            if (i < 0) {
                i = arg0->unk25 - 1;
            }
            j = i - 1;
            if (j < 0) {
                j = arg0->unk25 - 1;
            }
            a = entries[i].pos;
            b = entries[j].pos;
            ex = a.unk0 - b.unk0;
            ey = a.unk4 - b.unk4;
            ez = a.unk8 - b.unk8;
            dx = a.unk0 - cam->unk0;
            dy = a.unk4 - cam->unk4;
            dz = a.unk8 - cam->unk8;
            cx = (ey * dz) - (dy * ez);
            cy = (ez * dx) - (dz * ex);
            cz = (ex * dy) - (dx * ey);
            len = sqrtf((cx * cx) + (cy * cy) + (cz * cz));
            if (len == 0.0f) {
                cx = 0.0f;
                cy = 0.0f;
                cz = 0.0f;
            } else {
                f32 scale;
                scale = data->unk0 / len;
                cx = cx * scale;
                cy = cy * scale;
                cz = cz * scale;
            }
            vtx->v.ob[0] = a.unk0 + cx;
            vtx->v.ob[1] = a.unk4 + cy;
            vtx->v.ob[2] = a.unk8 + cz;
            vtx->v.tc[1] = entries[i].unk10 + 0x4000;
            vtx->v.tc[0] = 0x43C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0xFF;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = a.unk0 - cx;
            vtx->v.ob[1] = a.unk4 - cy;
            vtx->v.ob[2] = a.unk8 - cz;
            vtx->v.tc[1] = entries[i].unk10 + 0x4000;
            vtx->v.tc[0] = 0x4000;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0xFF;
            vtx->v.flag = 0;
            vtx++;
            do {
                ex = a.unk0 - b.unk0;
                ey = a.unk4 - b.unk4;
                ez = a.unk8 - b.unk8;
                dx = b.unk0 - cam->unk0;
                dy = b.unk4 - cam->unk4;
                dz = b.unk8 - cam->unk8;
                cx = (ey * dz) - (dy * ez);
                cy = (ez * dx) - (dz * ex);
                cz = (ex * dy) - (dx * ey);
                len = sqrtf((cx * cx) + (cy * cy) + (cz * cz));
                if (len == 0.0f) {
                    cx = 0.0f;
                    cy = 0.0f;
                    cz = 0.0f;
                } else {
                    f32 scale;
                    scale = data->unk0 / len;
                    cx = cx * scale;
                    cy = cy * scale;
                    cz = cz * scale;
                }
                vtx->v.ob[0] = b.unk0 + cx;
                vtx->v.ob[1] = b.unk4 + cy;
                vtx->v.ob[2] = b.unk8 + cz;
                vtx->v.tc[1] = entries[j].unk10 + 0x4000;
                vtx->v.tc[0] = 0x43C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0xFF;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = b.unk0 - cx;
                vtx->v.ob[1] = b.unk4 - cy;
                vtx->v.ob[2] = b.unk8 - cz;
                vtx->v.tc[1] = entries[j].unk10 + 0x4000;
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
                    j = arg0->unk25 - 1;
                }
                a = entries[i].pos;
                b = entries[j].pos;
            } while (i != arg0->unk2D);
        }
    }
    return gfx;
}

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_151488C4;

typedef struct {
    f32 x, y, z;
    f32 velocity;
    u16 phase;
    u8 pad12[2];
} Game175250Point;

typedef struct {
    f32 field0;
    f32 speedX;
    f32 speedY;
    f32 speedZ;
    f32 gravity;
    u8 pad14[4];
    u8 flags;
    u8 pad19[7];
    s8 state;
    u8 pad21[2];
    u8 callback23;
    u8 callback24;
    u8 pad25[3];
} Game175250Motion;

typedef struct {
    f32 height;
    u8 data[0x19];
    u8 kind;
    u8 pad1E[0x16];
} Game175250Collision;

typedef struct {
    u8 pad0[0x10];
    Vec3i_151488C4 position;
    u8 pad1C[9];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 start;
    s8 end;
    u8 pad2F[0x31];
    Game175250Collision collision;
    Game175250Point *points;
    Game175250Motion *motion;
} Game175250Trail;

s32 func_151488C4(Game175250Trail *arg0) {
    Game175250Point *points;
    Game175250Motion *motion;
    s32 phase;
    struct {
        u16 step;
        u8 pad2[2];
    } interval;
    s32 index;
    s32 count;
    s32 phaseIndex;
    Game175250Point *point;

    points = arg0->points;
    motion = arg0->motion;
    index = arg0->start;
    if (index != arg0->end) {
        do {
            point = (Game175250Point *)((u32)points + index * 0x14);
            index++;
            point->velocity -= motion->gravity * D_800BE9A4;
            point->x += motion->speedX * D_800BE9A4;
            point->y += point->velocity * D_800BE9A4;
            point->z += motion->speedZ * D_800BE9A4;
            if (index == arg0->capacity) {
                index = 0;
            }
        } while (index != arg0->end);
    }
    count = arg0->count;
    if (count < arg0->capacity - 1) {
        phase = (motion->flags & 0x20) ? 0x1000 : 0;
        if (count != 0) {
            interval.step = 0x1000 / count;
        }
        arg0->count = count + 1;
        *(Vec3i_151488C4 *)&points[arg0->end] = arg0->position;
        points[arg0->end].velocity = motion->speedY;
        arg0->end++;
        if (arg0->end == arg0->capacity) {
            arg0->end = 0;
        }
        phaseIndex = arg0->start;
        if (phaseIndex != arg0->end) {
            do {
                points[phaseIndex].phase = phase;
                if (motion->flags & 0x20) {
                    phase = (phase - interval.step) & 0xFFFF;
                } else {
                    phase += interval.step;
                    phase &= 0xFFFF;
                }
                phaseIndex++;
                if (phaseIndex == arg0->capacity) {
                    phaseIndex = 0;
                }
            } while (phaseIndex != arg0->end);
        }
    } else if (motion->flags & 0x17) {
        motion->state = 3;
    } else {
        motion->state = 2;
    }
    return 1;
}

s32 func_15148AF4(struct Struct15148AF4_arg0 *arg0) {
    s32 idx;
    struct Struct15148AF4_entry *entries;
    struct Struct15148AF4_data *data;
    struct Struct15148AF4_entry *entry;

    entries = arg0->unk94;
    data = arg0->unk98;
    idx = arg0->unk2E;
    do {
        idx -= 1;
        if (idx < 0) {
            idx = arg0->unk25 - 1;
        }
        entry = (struct Struct15148AF4_entry *)((u8 *)entries + (idx * 0x14));
        entry->unkC = entry->unkC - (data->unk10 * D_800BE9A4);
        entry->unk0 = entry->unk0 + (data->unk4 * D_800BE9A4);
        entry->unk4 = entry->unk4 + (entry->unkC * D_800BE9A4);
        entry->unk8 = entry->unk8 + (data->unkC * D_800BE9A4);
    } while (idx != arg0->unk2D);
    return 1;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_15148BA4;

typedef s32 (*Game175250HitCallback)(void *, f32, f32, f32, f32, void *);
extern Game175250HitCallback D_8008A430[];
extern Game175250HitCallback D_8008A450[];
s32 func_15046C80(f32 *, u16, f32, void *);

s32 func_15148BA4(Game175250Trail *arg0) {
    s32 index;
    Game175250Motion *motion;
    s32 points;
    Vec3f_15148BA4 oldPosition;
    Vec3f_15148BA4 position;
    u8 callback;

    motion = arg0->motion;
    points = (s32)arg0->points;
    index = arg0->end;
    if (motion->flags & 7) {
        oldPosition = *(Vec3f_15148BA4 *)((u32)points + arg0->start * 0x14);
    }
    do {
        index--;
        if (index < 0) {
            index = arg0->capacity - 1;
        }
        ((Game175250Point *)((u32)points + index * 0x14))->velocity -= motion->gravity * D_800BE9A4;
        ((Game175250Point *)((u32)points + index * 0x14))->x += motion->speedX * D_800BE9A4;
        ((Game175250Point *)((u32)points + index * 0x14))->y += ((Game175250Point *)((u32)points + index * 0x14))->velocity * D_800BE9A4;
        ((Game175250Point *)((u32)points + index * 0x14))->z += motion->speedZ * D_800BE9A4;
    } while (index != arg0->start);
    if (motion->flags & 0x17) {
        if (((Game175250Point *)((u32)points + arg0->start * 0x14))->y < oldPosition.y) {
            position.x = ((Game175250Point *)((u32)points + arg0->start * 0x14))->x;
            position.y = oldPosition.y;
            position.z = ((Game175250Point *)((u32)points + arg0->start * 0x14))->z;
            if (func_15046C80(&position.x, 0, ((Game175250Point *)((u32)points + arg0->start * 0x14))->y,
                              &arg0->collision) != 0) {
                if (arg0->collision.kind == 3) {
                    callback = motion->callback24;
                    if (callback != 0 &&
                        D_8008A450[callback](arg0, oldPosition.x, oldPosition.y,
                                             oldPosition.z, arg0->collision.height,
                                             arg0->collision.data) == 0) {
                        return 0;
                    }
                } else {
                    callback = motion->callback23;
                    if (callback != 0 &&
                        D_8008A430[callback](arg0, oldPosition.x, oldPosition.y,
                                             oldPosition.z, arg0->collision.height,
                                             arg0->collision.data) == 0) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

s32 func_15148DE0(void *arg0) {
    s32 idx;
    s32 step;
    s32 step2;
    s8 temp_v0;
    s32 phase;
    struct Struct15148AF4_data *data;
    struct Struct15148AF4_entry *entries;

    temp_v0 = *(s8 *)((u8 *)arg0 + 0x2C);
    if (temp_v0 >= 3) {
        *(s8 *)((u8 *)arg0 + 0x2C) = temp_v0 - 1;
        entries = *(struct Struct15148AF4_entry **)((u8 *)arg0 + 0x94);
        data = *(struct Struct15148AF4_data **)((u8 *)arg0 + 0x98);
        idx = *(s8 *)((u8 *)arg0 + 0x2D);
        step = (0x1000 / *(s8 *)((u8 *)arg0 + 0x2C)) & 0xFFFF;
        phase = (data->unk18 & 0x20) ? 0x1000 : 0;
        *(s8 *)((u8 *)arg0 + 0x2E) = *(s8 *)((u8 *)arg0 + 0x2E) - 1;
        if (*(s8 *)((u8 *)arg0 + 0x2E) < 0) {
            *(s8 *)((u8 *)arg0 + 0x2E) = *(u8 *)((u8 *)arg0 + 0x25) - 1;
        }
        if (idx != *(s8 *)((u8 *)arg0 + 0x2E)) {
            step2 = step;
            step = 0x14;
            do {
                ((struct Struct15148AF4_entry *)((u8 *)entries + (idx * step)))->unk10 = phase;
                if (data->unk18 & 0x20) {
                    phase = (phase - step2) & 0xFFFF;
                } else {
                    phase = (phase + step2) & 0xFFFF;
                }
                idx++;
                if (idx == *(u8 *)((u8 *)arg0 + 0x25)) {
                    idx = 0;
                }
            } while (idx != *(s8 *)((u8 *)arg0 + 0x2E));
        }
        return 1;
    }
    return 0;
}

s32 func_15148EF8(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *temp = (u8 *)*(s32 *)((u8 *)arg0 + 0x98);
    temp[0x20] = 4;
    return 1;
}

void func_15148F1C(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, f32 arg7, s16 arg8, f32 arg9, f32 arg10, u8 arg11) {
    f32 temp_f2;
    struct { f32 x; f32 y; f32 z; s16 unk0C; s16 unk0E; s32 unk10; u8 pad14; u8 unk15; } sp9C;
    f32 temp_f0;
    struct { f32 unk00; f32 unk04; f32 unk08; f32 unk0C; f32 unk10; u8 pad14[4]; u8 unk18; u8 unk19; u8 unk1A; u8 unk1B; } sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    u8 temp_v0;
    struct { s32 unk00; s32 unk04; s32 unk08; s32 unk0C; s32 unk10; s32 unk14; s32 unk18; u8 unk1C; u8 unk1D; } sp4C;

    sp78 = func_151423D8((u8)arg6);
    sp74 = func_151423D8((u8)(arg6 - 0x40));
    sp70 = func_151423D8((u8)arg5);
    temp_f2 = func_151423D8((u8)(arg5 - 0x40));

    sp9C.unk10 = 1;
    sp9C.unk0E = 1;
    sp7C.unk1A = 0xFF;
    sp7C.unk18 = 8;
    temp_v0 = *(((u8 *)&arg0) + 3);
    sp7C.unk19 = temp_v0;
    if (temp_v0 == 0xA) {
        sp7C.unk18 = 0x28;
    }

    temp_f0 = arg7 * sp78;
    sp9C.x = arg1;
    sp9C.y = arg2;
    sp9C.z = arg3;
    sp7C.unk00 = arg9;
    sp7C.unk10 = arg10;
    sp7C.unk04 = temp_f0 * temp_f2;
    sp7C.unk08 = -arg7 * sp74;
    sp7C.unk0C = temp_f0 * sp70;
    sp7C.unk1B = 0xFF;
    sp9C.unk15 = arg4 + 3;
    sp9C.unk0C = arg8;

    sp4C.unk00 = 0;
    sp4C.unk04 = 1;
    sp4C.unk08 = 0x160600;
    sp4C.unk0C = 3;
    sp4C.unk10 = 0x10;
    sp4C.unk14 = 0x80;
    sp4C.unk18 = 0x20;
    sp4C.unk1C = 0;
    sp4C.unk1D = 9;
    func_15147DA0(&sp9C, &sp7C, 0, 1, 0, 0, 0, 0, 0, 0, 0, (s32 *)&sp4C, 0, arg11, 1);
}

s32 func_151490C8(void *arg0) {
    u8 *v0 = (u8 *)*(s32 *)((u8 *)arg0 + 0x98);
    s32 v1 = *(s16 *)((u8 *)arg0 + 0x1C) << 3;

    if (v1 >= 0x100) {
        v1 = 0xFF;
    }
    v0[0x1B] = v1;
    if ((v1 & 0xFF) < 0) {
        return 0;
    }
    return 1;
}

void func_151478F4(void);

void func_15149104(void) {
    func_151478F4();
}
