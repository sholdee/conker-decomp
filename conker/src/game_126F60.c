#include <ultra64.h>
#define func_1513D2F0 func_1513D2F0_hdr
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"
#undef func_1513D2F0
void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, u8 argA, s32 argB);


extern s32 D_800BE628;
extern s32 D_80082FA4;
void func_15110360(s32 arg0, f32 arg1[4][4], f32 arg2, f32 arg3, f32 arg4);
Gfx *func_15110544(Gfx *gfx, s32 a, s32 b, s32 c, s32 d, u8 r, u8 g, u8 bl);
void func_150FB4C0(Gfx *gfx, f32 mtx[4][4]);

typedef struct {
    u8  pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} Struct150F9AB0;

void func_150F9AB0(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    f32 sp38[4][4];
    Struct150F9AB0 *temp;
    Gfx *gfx;

    func_15110360(D_80082FA4, sp38, arg4, arg5, arg6);
    temp = (Struct150F9AB0 *)((struct259 *)D_800BE628 + D_80082FA4);
    gfx = func_15110544(arg0, (s32)temp->unk2C, (s32)temp->unk24,
                        (s32)(temp->unk30 - 1.0f), (s32)temp->unk28, 0, 0, 0);
    gfx->words.w0 = 0xE7000000;
    gfx->words.w1 = 0;
    gfx++;
    gDPSetOtherMode(gfx++, 0x2C0F, 0x0F0A4004);
    gDPSetCombine(gfx++, 0x357E6A, 0xFFFFFFFF);
    func_150FB4C0(gfx, sp38);
}

typedef struct {
    /* 0x000 */ u8 pad0[0x3B];
    /* 0x03B */ u8 unk3B;
    /* 0x03C */ u8 pad3C[0x110];
    /* 0x14C */ f32 unk14C;
    /* 0x150 */ f32 unk150;
} Actor150F9BB0;

typedef struct {
    /* 0x00 */ Actor150F9BB0 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} Data150F9BB0; /* 0x18 */

typedef struct {
    /* 0x00 */ Actor150F9BB0 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ struct260 *unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s8 unk58;
} Ext150F9BB0; /* 0x5C */

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 unk1C;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} Spawn150F9BB0; /* 0x58 */

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ s8 unk59;
} Emit150F9BB0; /* 0x5C */

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ struct260 *unk8;
    /* 0x0C */ u8 unkC;
    /* 0x10 */ Actor150F9BB0 *unk10;
    /* 0x14 */ u8 unk14;
} Sub150F9BB0; /* 0x18 */

typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 duration;
    /* 0x04 */ u8 field4;
    /* 0x05 */ u8 field5;
    /* 0x06 */ s8 field6;
} Event150F9BB0;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ void *unk8[2];
    /* 0x10 */ void *unk10[2];
} Slots150F9BB0;

extern s32 D_800A1CC0;
void *func_151407D0(void *, s32, void *, u8, u8, u8, u8, s8, u8, s32);
void func_151D8868(void *, s32, u8, s32);
s32 func_15160A58(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_150F9BB0(Actor150F9BB0 *arg0, u8 arg1, s32 arg2) {
    struct260 *obj;
    Data150F9BB0 data;
    Slots150F9BB0 *slots;

    data.unk0 = arg0;
    data.unk4 = arg0->unk3B;
    data.unk8 = 0;
    data.unkC = 0;
    data.unk10 = 0;
    data.unk14 = 0;
    obj = func_15149130(0x46, -1, 0x4D, -1, 1, 0x33, (struct37 *)0x18, arg1, arg2);
    if (obj != NULL) {
        slots = (Slots150F9BB0 *)((u8 *)obj + 0x28);
        memcpy(slots, &data, 0x18);
        {
            Ext150F9BB0 ext;
            Spawn150F9BB0 spawn;

        ext.unk58 = 0;
        ext.unk54 = (arg0->unk14C + arg0->unk150) * 0.5f;
        ext.unk0 = arg0;
        ext.unk4 = arg0->unk3B;
        ext.unk8 = 15.04527187f;
        ext.unk10 = 5.867655754f;
        ext.unk18 = 12.11144447f;
        ext.unkC = 0.3961740136f;
        ext.unk24 = 0.0f;
        ext.unk28 = 0.0f;
        ext.unk38 = 0.0f;
        spawn.unk14 = 0.0f;
        spawn.unk18 = 0.0f;
        ext.unk3C = 0.0f;
        ext.unk48 = 1.0f;
        ext.unk40 = 1.0f;
        ext.unk4C = 1.0f;
        ext.unk44 = 1.0f;
        ext.unk14 = 0.1042563021f;
        ext.unk20 = 0.5197371244f;
        ext.unk1C = 0.3440458775f;
        ext.unk2C = 160.0f;
        ext.unk30 = 95.0f;
        ext.unk34 = 207.5f;
        ext.unk50 = obj;
        spawn.unk0 = 0x5F;
        spawn.unk1 = 8;
        spawn.unk2 = 0x2203;
        spawn.unk4 = 0x12C;
        spawn.unk8 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0xFF;
        spawn.unk11 = 0xFF;
        spawn.unk12 = 0xFF;
        spawn.unk13 = 0xFF;
        spawn.unk1C = *(struct17 *)&D_800A5480;
        spawn.unk28 = *(struct17 *)&D_800A5480;
        spawn.unk34 = 1.0f;
        spawn.unk38 = 1.0f;
        spawn.unk3C = 1.0f;
        spawn.unk40 = 0x40EC0000;
        spawn.unk44 = 0;
        spawn.unk45 = 0xFF;
        spawn.unk46 = 0;
        spawn.unk47 = 7;
        spawn.unk48 = 0;
        for (ext.unk5 = 0; ext.unk5 < 2; ext.unk5++) {
            slots->unk8[ext.unk5] = func_1513D2F0(&spawn, &D_800A4AA0, 0, 0x25, 0, 0x1D, ((func_150ADA20() & 1) ? 2 : 0) + 1, 0, 0, 0x5C, arg1, arg2);
            if (slots->unk8[ext.unk5] != NULL) {
                memcpy((u8 *)slots->unk8[ext.unk5] + 0x110, &ext, 0x5C);
            }
        }
        }
        {
            Emit150F9BB0 emit;
            Spawn150F9BB0 spawn2;
            Sub150F9BB0 sub;

        sub.unk0 = 0;
        sub.unk4 = 1.0f;
        sub.unk10 = arg0;
        sub.unk8 = obj;
        sub.unk14 = arg0->unk3B;
        emit.unk1C = 0.6999999881f;
        emit.unk20 = 9.999999975e-07f;
        emit.unk24 = -1;
        emit.unk34 = -1;
        emit.unk28 = -1;
        emit.unk38 = -1;
        emit.unk2C = -1;
        emit.unk3C = -1;
        emit.unk30 = -1;
        emit.unk40 = -1;
        emit.unk0 = 50.29999924f;
        emit.unk4 = 50.29999924f;
        emit.unk8 = 30.89999962f;
        emit.unkC = 30.89999962f;
        emit.unk10 = 0.1000000015f;
        emit.unk14 = 0.1000000015f;
        emit.unk18 = 1.0f;
        emit.unk44 = 0;
        emit.unk48 = 1.0f;
        emit.unk4C = 0;
        emit.unk4E = 0;
        emit.unk50 = 0;
        emit.unk52 = 0;
        emit.unk54 = 0;
        emit.unk55 = 0;
        emit.unk56 = 0;
        emit.unk57 = 0;
        emit.unk58 = 0xF;
        emit.unk59 = -1;
        spawn2.unk0 = 0x60;
        spawn2.unk1 = 3;
        spawn2.unk2 = 0x2203;
        spawn2.unk4 = 0x12C;
        spawn2.unk8 = 0;
        spawn2.unkC = 0;
        spawn2.unk10 = 0xFF;
        spawn2.unk11 = 0xFF;
        spawn2.unk12 = 0xFF;
        spawn2.unk13 = 0xFF;
        spawn2.unk14 = 100.0f;
        spawn2.unk18 = 100.0f;
        spawn2.unk1C = *(struct17 *)&D_800A5480;
        spawn2.unk28 = *(struct17 *)&D_800A5480;
        spawn2.unk34 = 1.0f;
        spawn2.unk38 = 1.0f;
        spawn2.unk3C = 1.0f;
        spawn2.unk40 = 0xCD2002;
        spawn2.unk44 = 0xFF;
        spawn2.unk45 = 0xFF;
        spawn2.unk46 = 0;
        spawn2.unk47 = 7;
        spawn2.unk48 = 0;
        spawn2.unk4C = 0xFF;
        for (sub.unkC = 0; sub.unkC < 2; sub.unkC++) {
            slots->unk10[sub.unkC] = func_151407D0(&emit, 0x78, &spawn2, 0, 0x2A, 0, 0, -1, arg1, arg2);
            if (slots->unk10[sub.unkC] != NULL) {
                memcpy((u8 *)slots->unk10[sub.unkC] + 0x170, &sub, 0x18);
            }
        }
        }
    }
    {
        Event150F9BB0 event;

    event.type = 1;
    event.duration = 0x46;
    event.field5 = 1;
    event.field4 = 8;
    event.field6 = -1;
    func_151D8868(&event, 0, arg1, arg2);
    func_15160A58(arg0, 1, &D_800A1CC0, 3, 0x46, 0x28, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, arg1, arg2);
    }
}

extern s32 func_15145EA4(void *arg0, void *arg1, s32 arg2, s32 arg3);
extern struct17 D_800A1CCC;
extern struct17 D_800A1CD8;
extern struct17 D_800A1CE4;
extern struct17 D_800A1CF0;

typedef struct {
    u8 pad0[0x48];
    f32 unk48;
    f32 unk4C;
    u8 pad50[0x8];
    u8 unk58;
} SubA_150FA1B8;

typedef struct {
    u8 pad0[0x34];
    struct17 unk34;
    struct17 unk40;
    u8 pad4C[0x110 - 0x4C];
    SubA_150FA1B8 unk110;
} Struct150FA1B8_A;

typedef struct {
    u8 pad0[0x4];
    f32 unk4;
} SubB_150FA1B8;

typedef struct {
    u8 pad0[0x34];
    struct17 unk34;
    struct17 unk40;
    u8 pad4C[0xC];
    s32 unk58;
    u8 pad5C[0x170 - 0x5C];
    SubB_150FA1B8 unk170;
} Struct150FA1B8_B;

typedef struct {
    s32 unk0;
    u8 pad4[0x3B - 4];
    u8 unk3B;
    u8 pad3C[0x1D4 - 0x3C];
    s32 unk1D4;
} Struct150FA1B8_C;

typedef struct {
    Struct150FA1B8_C *unk0;
    u8 unk4;
    u8 pad5[3];
    Struct150FA1B8_A *unk8[2];
    Struct150FA1B8_B *unk10[2];
} Struct150FA1B8_D;

typedef struct {
    u8 pad0[0xE];
    s16 unkE;
    u8 pad10[0x18];
    Struct150FA1B8_D unk28;
} Struct150FA1B8;

void func_150FA1B8(Struct150FA1B8 *arg0) {
    Struct150FA1B8_C *c;
    Struct150FA1B8_D *d;

    c = arg0->unk28.unk0;
    d = &arg0->unk28;
    if ((c->unk0 == 0) || (c->unk3B != d->unk4)) {
        arg0->unkE = -1;
        return;
    }
    if (c->unk1D4 != 0) {
        struct17 sp88[2];
        struct17 sp70[2];
        s32 sp6C;
        struct17 *sp5C[4];
        struct17 *sp4C[4];
        Struct150FA1B8_A *a;
        Struct150FA1B8_B *b;
        u8 i;

        sp5C[0] = &D_800A1CCC;
        sp5C[1] = &D_800A1CD8;
        sp5C[2] = &D_800A1CE4;
        sp5C[3] = &D_800A1CF0;
        sp4C[0] = &sp88[0];
        sp4C[1] = &sp88[1];
        sp4C[2] = &sp70[0];
        sp4C[3] = &sp70[1];
        func_15145EA4(sp5C, sp4C, c->unk1D4 + 0x40, 4);
        for (i = 0; i < 2; i++) {
            if (d->unk8[i] != NULL) {
                a = d->unk8[i];
                a->unk110.unk58 |= 1;
                a->unk34 = sp88[i];
                a->unk40 = sp70[i];
            }
            if (d->unk10[i] != NULL) {
                b = d->unk10[i];
                b->unk58 |= 2;
                b->unk58 &= ~4;
                b->unk34 = sp88[i];
                b->unk40 = sp70[i];
            }
        }
    } else {
        SubA_150FA1B8 *sa;
        u8 j;

        for (j = 0; j < 2; j++) {
            if (d->unk8[j] != NULL) {
                sa = &d->unk8[j]->unk110;
                sa->unk58 &= ~1;
            }
            if (d->unk10[j] != NULL) {
                d->unk10[j]->unk58 &= ~2;
            }
        }
    }
    {
        u8 k;
        s32 pad;

        for (k = 0; k < 2; k++) {
            if (d->unk8[k] != NULL) {
                SubA_150FA1B8 *sa = &d->unk8[k]->unk110;

                if (arg0->unkE < 10) {
                    sa->unk48 = 0.0f;
                    sa->unk4C = 0.0f;
                } else {
                    sa->unk48 = 6.0f;
                    sa->unk4C = 15.0f;
                }
            }
            if (d->unk10[k] != NULL) {
                SubB_150FA1B8 *sb = &d->unk10[k]->unk170;

                if (arg0->unkE < 10) {
                    sb->unk4 = 0.0f;
                } else {
                    sb->unk4 = 1.306000113f;
                }
            }
        }
    }
}

void func_150FA468(struct102 *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 0x4C) {
        *(s8 *)((s32)arg0 + 0x11) = -1;
        func_1515D4D4(0, 0, 0, 0xFF);
    } else if (arg2 == 0x4D) {
        *(u8 *)((s32)arg0 + 0x11) = 0x1E;
    } else if (arg2 == 0x4E) {
        *(s8 *)((s32)arg0 + 0x11) = -1;
    } else if (arg2 == 0x4F) {
        *(s8 *)((s32)arg0 + 0x11) = -1;
        func_1515D4D4(0xFF, 0xFF, 0xFF, 0xFF);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_126F60/func_150FA520.s")

void func_150FAA40(u8 arg0, s32 arg1) {
    struct {
        s32 unk0;
        s32 unk4;
        f32 unk8;
    } sp3C;
    struct260 *temp_v0;

    if (D_80088B60 == 0) {
        sp3C.unk0 = *(s32 *)&D_800D3098 + 0x71C;
        sp3C.unk4 = *(s32 *)&D_800D3098 + 0x6E8;
        sp3C.unk8 = 0.0f;

        temp_v0 = func_15149130(0x12C, -1, 0x57, -1, 0, 0x46, (struct37 *)0xC, arg0, arg1);
        if (temp_v0 != NULL) {
            D_80088B60 = 1;
        }
        if (temp_v0 != NULL) {
            memcpy((void *)((s32)temp_v0 + 0x28), &sp3C, 0xC);
        }
    }
}

typedef struct {
    u8 count;
    s16 *ids;
    f32 *scales;
} Rec150FA;

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8;
} Emit150FA;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Emit150FA unk28;
} Emitter150FA;

extern Rec150FA D_80088B20[];
void func_1514470C(s32, struct17 *);
void func_150F4570(struct17 *arg0, struct17 *arg1, u8 arg2, f32 arg3, f32 arg4, f32 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);

void func_150FAAEC(Emitter150FA *arg0) {
    Emit150FA *e;
    struct17 a;
    struct17 b;
    u8 i;
    f32 speed;
    u32 n;
    Rec150FA *rec;

    e = &arg0->unk28;
    n = 3;
    e->unk8 += (0.08000000566f + func_150ADA68() * 0.1800000072f) * D_800BE9A4;
    if (1.0f < e->unk8) {
        do {
            func_1514470C(e->unk0, &a);
            func_1514470C(e->unk4, &b);
            rec = &D_80088B20[1];
            i = func_150ADA20() % rec->count;
            speed = func_150ADA68() * 25.0f + 30.0f;
            func_150F4570(&a, &b, rec->ids[i], speed, 1.0f / speed,
                          rec->scales[i] * (func_150ADA68() * (f32)1 + (f32)1), 1, func_150ADA20() % n + 0x2EE,
                          arg0->unkC, arg0->unk1);
            e->unk8 -= 1.0f;
        } while (1.0f < e->unk8);
    }
}

void func_150FACE4(struct102 *arg0, s32 arg1, u8 arg2) {
    switch (arg2) {
        case 0x4E:
        case 0x4F:
            func_1516972C(arg0);
            break;
    }
}

extern void func_1515F170(s32 arg0, u8 arg1);

void func_150FAD28(void) {
    func_1515F170(8, 0);
    func_1515F170(0xB, 1);
    func_151494E0(*(s32 *)&D_800D3098 + 0x514, 0x30);
    func_151494E0(0, 0x4D);
}

void func_150FAD78(void) {
    func_1515F170(8, 1);
    func_1515F170(7, 0);
    func_151494E0(*(s32 *)&D_800D3098 + 0x514, 0x31);
    func_151494E0(0, 0x4C);
}

void func_150FADC8(s32 *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 0x53) {
        arg0[0x16] |= 2;
    } else if (arg2 == 0x54) {
        arg0[0x16] &= ~2;
    }
}

extern void *func_15157010(void *, s32, f32, s32, s32, s32, u8, s32);
extern void *func_1515548C(void *, s32, s32, s32, s32, u8, s32);

typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ s8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ s8 unk04;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ struct17 unk44;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} S150FAE18Main;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} S150FAE18Part;

void func_150FAE18(s16 arg0, u8 arg1, s32 arg2) {
    s16 pad;
    s16 id;

    id = arg0;
    {
    S150FAE18Main spC4;
    void *ret;

    spC4.unk38 = 0;
    spC4.unk00 = 0x61;
    spC4.unk01 = 3;
    spC4.unk02 = 4;
    spC4.unk03 = 2;
    spC4.unk04 = 0;
    spC4.unk06 = arg0;
    spC4.unk08 = 0xA5;
    spC4.unk0C = 0x17;
    spC4.unk10 = 0;
    spC4.unk14 = 0x200405;
    spC4.unk18 = 0x60200;
    spC4.unk2D = 8;
    spC4.unk1C = 1;
    spC4.unk20 = 0x38;
    spC4.unk2C = 0;
    spC4.unk24 = 0x80;
    spC4.unk28 = 0x20;
    spC4.unk30 = 0xFF;
    spC4.unk31 = 0xFF;
    spC4.unk32 = 0xFF;
    spC4.unk33 = 0xFF;
    spC4.unk34 = 0xFF;
    spC4.unk35 = 0xFF;
    spC4.unk36 = 0xFF;
    spC4.unk37 = 0xFF;
    spC4.unk3C = 0;
    spC4.unk40 = 0;
    spC4.unk41 = 1;
    spC4.unk44 = *(struct17 *)&D_800A5480;
    spC4.unk54 = 0xA;
    spC4.unk56 = 0x19;
    ret = func_15157010(&spC4, 0, 1.0, 0, 0, 2, arg1, arg2);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0x120, &id, 2);
    }
    {
    S150FAE18Part sp68;
    u8 i;
    const s16 tbl[8] = { 0xCE, 0xC5, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4 };

    sp68.unk04 = 62.0f;
    sp68.unk12 = arg0;
    sp68.unk14 = 0x29;
    sp68.unk16 = 0xA;
    sp68.unk18 = 0x19;
    sp68.unk08 = 16.0f;
    sp68.unk0C = 16.0f;
    sp68.unk1A = 0;
    sp68.unk1B = 0xFF;
    sp68.unk1C = 0xFF;
    sp68.unk1D = 0xFF;
    sp68.unk1E = 0xFF;
    sp68.unk1F = 0xFF;
    sp68.unk20 = 0xFF;
    sp68.unk21 = 0xFF;
    sp68.unk22 = 0xFF;
    sp68.unk23 = 0xFF;
    sp68.unk24 = 0;
    sp68.unk28 = 0x200405;
    sp68.unk2C = 0x60200;
    sp68.unk40 = 0;
    sp68.unk41 = 7;
    sp68.unk30 = 0x19;
    sp68.unk34 = 0x22;
    sp68.unk38 = 0x80;
    sp68.unk3C = 0x20;
    sp68.unk44 = 0;
    sp68.unk48 = 1.0f;
    sp68.unk4C = 1.0f;
    sp68.unk50 = 0.0f;
    sp68.unk54 = 0.0f;
    for (i = 0; i < 8; i++) {
        sp68.unk00 = i * 24.75f + -65.0f;
        sp68.unk10 = tbl[i];
        ret = func_1515548C(&sp68, 0xE, 0, 0, 2, arg1, arg2);
        if (ret != NULL) {
            memcpy((u8 *)ret + 0x70, &id, 2);
        }
    }
    }
    }
}

typedef struct Func150FB188State {
    u8 pad0[0x54];
    f32 field54;
    f32 field58;
    f32 field5C;
} Func150FB188State;

typedef struct Func150FB188Transform {
    f32 field0;
    f32 field4;
    f32 field8;
    f32 fieldC;
    f32 field10;
} Func150FB188Transform;

void func_15157DEC(u8 *, u8 *);

s32 func_150FB188(Func150FB188State *arg0) {
    Func150FB188Transform transform;

    arg0->field5C = 0.0f;
    arg0->field54 = -95.0f;
    arg0->field58 = -80.0f;
    transform.field0 = 0.0f;
    transform.field4 = 0.0f;
    transform.field8 = 0.0f;
    transform.fieldC = 0.3f;
    transform.field10 = 0.3f;
    func_15157DEC((u8 *)arg0, (u8 *)&transform);
    return 1;
}

extern s32 func_151D710C(s32, s32, s32, s32, s32);
extern void func_15157F80(s32, s32, s32, s32, s32);

void func_150FB1E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_15157F80(func_151D710C(arg0, arg1, arg2, arg3, arg4), arg1, arg2, arg3, arg4);
}

void func_150FB240(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    if ((arg2 - arg3) < arg1) {
        *arg0 = (arg2 - arg1) * arg4;
    } else {
        *arg0 = 0xFF;
    }
}

s32 func_150FB29C(void *arg0) {
    func_150FB240((u8 *)arg0 + 0x43, *(s16 *)((u8 *)arg0 + 0x16), *(s16 *)((u8 *)arg0 + 0x120), *(s16 *)((u8 *)arg0 + 0x64), *(s16 *)((u8 *)arg0 + 0x66));
    return 1;
}

extern void func_150FB240(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);

s32 func_150FB2E0(void *arg0) {
    func_150FB240((u8 *)arg0 + 0x2E, *(s16 *)((u8 *)arg0 + 0x22), *(s16 *)((u8 *)arg0 + 0x70), *(s16 *)((u8 *)arg0 + 0x26), *(s16 *)((u8 *)arg0 + 0x28));
    return 1;
}

extern s32 func_15145EA4(void *arg0, void *arg1, s32 arg2, s32 arg3);
void func_150FEC28(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);
void func_151D4408(struct17 *, struct17 *, s32, struct127 *, f32, s32, s32);

void func_150FB324(struct127 *arg0, s32 arg1, s32 arg2) {
    struct17 sp94;
    struct17 sp88;
    struct17 sp7C;
    s32 sp78;
    s32 sp74;
    const struct17 sp68 = { -108.0f, -9.0f, 85.0f };
    const struct17 sp5C = { -150.0f, -9.0f, 111.0f };
    const struct17 sp50 = { -32.0f, 4.0f, 40.0f };
    const struct17 sp44 = { -21.0f, 15.0f, 40.0f };
    struct17 *sp38[3];
    struct17 *sp2C[3];

    if (arg0->unk1D4 == 0) {
        return;
    }
        sp38[0] = (struct17 *)&sp68;
        sp38[1] = (struct17 *)&sp50;
        sp38[2] = (struct17 *)&sp44;
        sp2C[0] = &sp94;
        sp2C[1] = &sp88;
        sp2C[2] = &sp7C;
        sp78 = (s32)arg0->unk1D4 + 0x380;
        func_15145EA4(sp38, sp2C, sp78, 3);
        sp7C.unk0 -= sp88.unk0;
        sp7C.unk4 -= sp88.unk4;
        sp7C.unk8 -= sp88.unk8;
        {
            void func_150FEC28(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);
            func_150FEC28((s32)arg0, 0xE, (s32)&sp68, (s32)&sp5C, (s32)&sp94, ((u8 *)&arg1)[3], arg2);
        }
        func_151D4408(&sp88, &sp7C, sp78, arg0, 1.0f, ((u8 *)&arg1)[3], arg2);
}
