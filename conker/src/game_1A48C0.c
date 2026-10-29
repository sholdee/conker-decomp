#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_1A48C0/func_15177410.s")

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

