#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u16 unk16;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ Vtx *unk20[2];
    /* 0x28 */ Vtx *unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8 pad38[0x37];
    /* 0x6F */ u8 unk6F;
} Struct15177A94Obj;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ u8 unk4[0x14];
    /* 0x18 */ u8 unk18[8];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
} Struct15177A94Ent;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s8 unk16;
    /* 0x17 */ s8 unk17;
    /* 0x18 */ u8 unk18[6];
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ f32 *unk20;
    /* 0x24 */ s8 *unk24;
    /* 0x28 */ u8 *unk28;
} Struct15177410Ent;

extern Struct15177A94Obj *func_151149AC(u8);
void *allocate_memory(s32, s32, s32, s32);

void func_15177410(u8 arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4, f32 arg5, u16 arg6, f32 arg7, s8 arg8, s8 arg9, u8 argA, u8 argB, u8 argC, u8 argD, u8 argE, u8 argF) {
    Struct15177410Ent *ent;
    Struct15177A94Obj *obj;
    Struct15177410Ent *other;
    s32 i;
    s8 found;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 pos[3];
    f32 scale;
    s32 adist;
    s32 dist;
    f32 d;
    f32 c;
    f32 s;
    f32 ang;

    ent = (Struct15177410Ent *)allocate_memory(0x2C, 1, 0, 0);
    if (ent != NULL) {
        u8 b;

        ((Struct15177410Ent **)D_800DD410)[D_800DD418++] = ent;
        obj = func_151149AC(arg1);
        found = 0;
        ent->unk0 = arg1;
        ent->unk1 = arg0;
        for (i = 0; i < D_800DD418 - 1; i++) {
            other = ((Struct15177410Ent **)D_800DD410)[i];
            if (other->unk0 == arg1) {
                found = 1;
                break;
            }
        }
        ent->unk2 = found;
        pos[0] = (s16)arg2 - obj->unk10;
        pos[1] = (s16)arg3 - obj->unk12;
        pos[2] = (s16)arg4 - obj->unk14;
        ent->unkE = pos[0];
        ent->unk10 = pos[1];
        ent->unk12 = pos[2];
        ent->unkC = arg6;
        ent->unk8 = arg7;
        ent->unk14 = 0;
        ent->unk16 = arg8;
        ent->unk17 = arg9;
        if ((u16)arg6 != 0) {
            scale = 0.009999999776f;
            ent->unk4 = 1.0f / (((u16)arg6 * scale) * ((u16)arg6 * scale));
        } else {
            ent->unk4 = 0.0f;
            scale = 0.009999999776f;
        }
        if ((arg0 & 3) == 1) {
            ent->unk18[0] = argA;
            ent->unk18[1] = argB;
            ent->unk18[2] = argC;
            ent->unk18[3] = argD;
            ent->unk18[4] = argE;
            ent->unk18[5] = argF;
        }
        ent->unk24 = (s8 *)allocate_memory(obj->unk16, 1, 0, 0);
        if ((arg0 & 3) == 2) {
            ent->unk28 = (u8 *)allocate_memory(obj->unk16, 1, 0, 0);
        }
        if ((arg0 & 4) == 4) {
            ent->unk20 = (f32 *)allocate_memory(obj->unk16 * 4, 1, 0, 0);
        }
        if ((arg0 & 0x10) == 0x10) {
            ang = -arg5 * 0.0174532924f;
            s = sinf(ang);
            c = cosf(ang);
            b = arg5 * 0.7111111283f;
        }
        for (i = 0; i < obj->unk16; i++) {
            if ((arg0 & 0x10) == 0) {
                dx = obj->unk28[i].v.ob[0] - pos[0];
                dx = dx * dx;
                dy = obj->unk28[i].v.ob[1] - pos[1];
                dz = obj->unk28[i].v.ob[2] - pos[2];
                dy = dy * dy;
                dz = dz * dz;
                d = dx + dy + dz;
                if (d != 0) {
                    d = sqrtf(d);
                }
                adist = d * scale;
                dist = adist;
            } else if ((arg0 & 0x10) == 0x10) {
                dx = obj->unk28[i].v.ob[0] - pos[0];
                dz = obj->unk28[i].v.ob[2] - pos[2];
                dz = -dx * s + dz * c;
                adist = dz * scale;
                dist = adist;
                if (adist < 0) {
                    adist = -adist;
                }
            }
            ent->unk24[i] = (s8)arg9 * dist;
            if ((arg0 & 4) == 4) {
                adist = adist - (u16)arg6 * scale;
                if (adist >= 0) {
                    ent->unk20[i] = 0.0f;
                } else {
                    ent->unk20[i] = ent->unk4 * arg7 * (adist * adist);
                }
            }
            if ((arg0 & 3) == 2) {
                if ((arg0 & 0x10) == 0x10) {
                    ent->unk28[i] = b;
                } else {
                    u8 tmp;
                    tmp = func_150484A0(dx, dz) * 40.7436637878f;
                    ent->unk28[i] = tmp;
                }
            }
        }
    }
}

extern Struct15177A94Obj *func_151149AC(u8);
extern void func_1511490C(f32 arg0[4][4], void *arg1);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern void func_150A6FA0(Vtx *, u16, void *, s32, s32, Vtx *);
extern void func_150A70C0(Vtx *, u16, void *, s32, Vtx *);
extern void func_150A71C8(Vtx *, u16, void *, void *, s32, Vtx *);
extern void func_150A7360(Vtx *, u16, void *, void *, s32, s32, Vtx *);
extern void func_150A751C(Vtx *, u16, void *, s32, s32, s32, Vtx *);

void func_15177A94(void) {
    s32 i;
    Struct15177A94Ent *ent;
    Struct15177A94Obj *obj;
    Vtx *src;
    f32 mtx[4][4];
    f32 x;
    f32 y;
    f32 z;
    s32 j;

    for (i = 0; i < D_800DD418; i++) {
        ent = ((Struct15177A94Ent **)D_800DD410)[i];
        obj = func_151149AC(ent->unk0);
        if (obj->unk6F & 0xF) {
            if (ent->unk2 == 0) {
                src = obj->unk28;
            } else {
                src = obj->unk20[D_800BE9C0];
            }
            if ((ent->unk1 & 3) == 1) {
                if ((ent->unk1 & 4) == 4) {
                    func_150A7360(obj->unk20[D_800BE9C0], obj->unk16, ent->unk4, ent->unk18, ent->unk24, ent->unk20, src);
                } else {
                    func_150A71C8(obj->unk20[D_800BE9C0], obj->unk16, ent->unk4, ent->unk18, ent->unk24, src);
                }
            } else if ((ent->unk1 & 3) == 0) {
                if ((ent->unk1 & 4) == 4) {
                    func_150A6FA0(obj->unk20[D_800BE9C0], obj->unk16, ent->unk4, ent->unk24, ent->unk20, src);
                } else {
                    func_150A70C0(obj->unk20[D_800BE9C0], obj->unk16, ent->unk4, ent->unk24, src);
                }
            } else if ((ent->unk1 & 3) == 2) {
                if ((ent->unk1 & 4) == 4) {
                    func_150A751C(obj->unk20[D_800BE9C0], obj->unk16, ent->unk4, ent->unk28, ent->unk24, ent->unk20, src);
                }
            }
        }
    }
    for (i = 0; i < D_800DD418; i++) {
        ent = ((Struct15177A94Ent **)D_800DD410)[i];
        if (func_151149AC(ent->unk0)->unk6F & 0xF) {
            j = 0;
            if (ent->unk2 == 0) {
                obj = func_151149AC(ent->unk0);
                if (obj->unk0 == 0.0f && obj->unk4 == 0.0f && obj->unk8 == 0.0f &&
                    obj->unk2C == 1.0f && obj->unk30 == 1.0f && obj->unk34 == 1.0f) {
                    for (j = 0; j < obj->unk16; j++) {
                        obj->unk20[D_800BE9C0][j].v.ob[0] = obj->unk28[j].v.ob[0] + obj->unk10;
                        obj->unk20[D_800BE9C0][j].v.ob[1] += obj->unk12;
                        obj->unk20[D_800BE9C0][j].v.ob[2] = obj->unk28[j].v.ob[2] + obj->unk14;
                    }
                } else {
                    func_1511490C(mtx, obj);
                    if (i) {}
                    for (j = 0; j < obj->unk16; j++) {
                        func_150A7960((f32 *)mtx, obj->unk28[j].v.ob[0], obj->unk20[D_800BE9C0][j].v.ob[1], obj->unk28[j].v.ob[2], &x, &y, &z);
                        obj->unk20[D_800BE9C0][j].v.ob[0] = x;
                        obj->unk20[D_800BE9C0][j].v.ob[1] = y;
                        obj->unk20[D_800BE9C0][j].v.ob[2] = z;
                    }
                }
            }
        }
    }
}

