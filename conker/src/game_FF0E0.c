#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Vec150D1F6C;

typedef struct {
    Vec150D1F6C unk0;
    f32 unkC;
} Arg150D1F6C;

typedef struct {
    u8 pad0[0x18];
    Vec150D1F6C unk18;
    u8 pad24[0x18];
    f32 unk3C;
    s32 unk40;
    u8 unk44;
} Sub150D1F6C;

typedef struct {
    u8 pad0[0x40];
    Vec150D1F6C unk40;
} Obj150D1F6C;

typedef struct {
    u8 pad0[9];
    u8 unk9;
    u8 padA[4];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u8 pad14[0x1B];
    u8 unk2F;
} Sub150D1C30_14;

typedef struct {
    u8 pad0[0x14];
    Sub150D1C30_14 *unk14;
} Obj150D1C30;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    Obj150D1C30 *unk24[6];
    f32 unk3C;
} Data150D1C30;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f150D1C30;

extern Vec3f150D1C30 D_800A08F0[];
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, f32 *, f32 *, f32 *, f32 *, s32, s32, f32);

void func_150D1C30(struct260 *arg0) {
    Data150D1C30 *d;
    Obj150D1C30 *obj;
    f32 mtx[4][4];
    s32 i;

    d = (Data150D1C30 *)((u8 *)arg0 + 0x28);
    if (*((u8 *)arg0 + 0x6C) & 1) {
        d->unkC += d->unk18 * D_800BE9A4;
        d->unk10 += d->unk1C * D_800BE9A4;
        d->unk14 += d->unk20 * D_800BE9A4;
        func_150A8050(mtx, d->unkC, d->unk10, d->unk14);
        for (i = 0; i < 6; i++) {
            obj = d->unk24[i];
            if (obj != NULL) {
                f32 spA0;
                f32 sp9C;
                f32 sp98;
                f32 sp94;
                f32 sp90;
                f32 sp8C;
                f32 sp88;
                func_150A7960((f32 *)mtx, D_800A08F0[i].x, D_800A08F0[i].y, D_800A08F0[i].z, &sp98, &sp9C, &spA0);
                if (func_150AC9C0(d->unk0, d->unk4, d->unk8, sp98, sp9C, spA0, NULL, NULL, &sp8C, &sp90, &sp94, &sp88, 0, 0, 0.0f) != 0) {
                    obj->unk14->unk9 = 0;
                    obj->unk14->unkE = sp8C;
                    obj->unk14->unk10 = sp90;
                    obj->unk14->unk12 = sp94;
                    obj->unk14->unk2F = d->unk3C * sp88;
                } else {
                    obj->unk14->unk9 = 1;
                }
            }
        }
    } else {
        for (i = 0; i < 6; i++) {
            Obj150D1C30 *o = d->unk24[i];
            if (o != NULL) {
                o->unk14->unk9 = 1;
            }
        }
    }
}

void func_150D1F6C(struct260 *arg0, s32 arg1, u8 arg2) {
    Sub150D1F6C *sub;
    Arg150D1F6C *arg;

    sub = (Sub150D1F6C *)((s32)arg0 + 0x28);
    if (arg2 == 0x32) {
        arg = (Arg150D1F6C *)arg1;
        sub->unk18 = arg->unk0;
        sub->unk3C = arg->unkC;
    dummy_label_150D1F6C_1: ;
    } else {
        if ((arg2 == 0x30) || (arg2 == 0x31)) {
            if (arg1 == sub->unk40) {
                if (arg2 == 0x30) {
                    sub->unk44 |= 1;
                } else {
                    sub->unk44 &= ~1;
                dummy_label_150D1F6C_2: ;
                }
            }
        } else if (arg2 == 0x4E) {
            ((Obj150D1F6C *)arg0)->unk40 = *(Vec150D1F6C *)&D_800A5480;
        } else if (arg2 == 0x4F) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

void func_150D2054(struct260 *arg0) {
    s32 i;
    s32 *p;
    void *ptr;

    p = (s32 *)((u8 *)arg0 + 0x28);
    i = 0;
    do {
        ptr = (void *)p[i + 9];
        if (ptr != NULL) {
            func_1516972C((struct102 *)ptr);
        }
        i++;
        i = (u8)i;
    } while (i < 6);
}

extern void func_150D2054(struct260 *);

void func_150D20B0(struct260 *arg0) {
    func_150D2054(arg0);
    func_15149368(arg0);
}

void func_150D20DC(struct260 *arg0) {
    func_150D2054(arg0);
    func_1514933C(arg0);
}
