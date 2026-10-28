#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 *D_800DBF94;
extern s32 func_151149AC(u32);
extern s32 func_1505D024(struct127 *, s32, u16, s32);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern void func_150E1AB0(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, u16, s32, s32, s16, s16, s32, u8, s16, s32, s32, u8, f32, f32, f32, f32, f32, f32);
extern void func_150E2DB4(struct127 *, u8, s16, s32, f32, f32, f32, f32, f32, f32, s16, s16, u16, u8);
extern void func_15179008(s32);
extern void func_15131828(void *, void *, void *, void *);
extern void func_15131958(void *, s32, void *);

typedef struct {
    char pad0[0xC];
    s32 unkC;
} struct150AF6E4;

void func_150AF200(s32 arg0, s32 arg1) {
    struct131 *temp;
    s32 idx;

    if (D_800CC3D4[0] != 0) {
        return;
    }

    temp = (struct131 *)func_151149AC(arg0 & 0xFF);
    idx = temp - D_800DBEF4;
    if (D_800DBF94[idx] & 1) {
        func_1505D024(&D_800CC2D0[0], 0x3F, 0x6E00, -1);
        return;
    }

    temp = (struct131 *)func_151149AC((u8)arg1);
    idx = temp - D_800DBEF4;
    if (D_800DBF94[idx] & 1) {
        func_1505D024(&D_800CC2D0[0], 0x3F, 0xEE00, -1);
    }
}

extern void func_151CF898(void *, f32, f32);

void func_150AF2E0(void *arg0, s16 *arg1) {
    func_151CF898(arg0, (f32)(arg1[4] + arg1[1]), (f32)arg1[1]);
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
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[9];
} Spawn15130280;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Extra150AF328;

typedef struct {
    struct17 pos;
    f32 accum;
} Sub150AF328;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Sub150AF328 unk28;
} Obj150AF328;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_150AF328(Obj150AF328 *arg0) {
    Sub150AF328 *sub;

    sub = &arg0->unk28;
    sub->accum += (1352.0f + func_150ADA68() * 1698.0f) * 0.0001f * D_800BE9A4;
    if (sub->accum > 1.0f) {
        Spawn15130280 spawn;
        Extra150AF328 extra;
        void *ret;

        extra.unkC = 0.9704700112f;
        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.unk1E = 0x46;
        spawn.unk20 = 3;
        spawn.unk58 = 0x80DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x16;
        extra.unk0 = 0;
        extra.unk1 = 0;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 0x46;
        spawn.unk14 = 0xC7;
        spawn.unk15 = 0x78;
        spawn.unk16 = 8;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0x30;
        spawn.unk19 = 0xE;
        spawn.unk1A = 0;
        spawn.unk1C = 0xFF;
        spawn.unk24 = 1.021028996f;
        spawn.unk30 = sub->pos;
        spawn.unk3C.unk0 = 0.0f;
        spawn.unk3C.unk4 = 0.0f;
        spawn.unk3C.unk8 = 0.0f;
        spawn.unk48.unk0 = 0.0f;
        spawn.unk48.unk4 = 0.0f;
        spawn.unk48.unk8 = 0.0f;
        do {
            extra.unk2 = func_150ADA20() % 5U + 4;
            extra.unk3 = func_150ADA20() % 5U + 4;
            extra.unk4 = func_150ADA68() * 11.0f;
            extra.unk8 = func_150ADA68() * 11.0f;
            spawn.unk54 = func_150ADA68() * 0.172f + 0.09800000489f;
            spawn.unk58 &= ~0xC0;
            spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.unk1B = func_150ADA20() % 101U + 100;
            spawn.unkA = func_150ADA20() % 51U + 80;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 71.0f + 80.0f;
            ret = func_15130280(&spawn, 0, 0, 0x10, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &extra, sizeof(extra));
            }
            sub->accum -= 1.0f;
        } while (sub->accum > 1.0f);
    }
}

s32 func_150AF6E4(u8 *arg0, s32 arg1) {
    struct150AF6E4 *temp_a2;

    temp_a2 = (struct150AF6E4 *)(arg0 + 0xA8);
    func_15131828(arg0, arg0 + 0xAC, temp_a2, arg0 + 0xAA);
    func_15131958(arg0 + 0x58, temp_a2->unkC, temp_a2);
    return 1;
}

extern void func_1515FF74(void *, s32, u8, s32);

struct sp18 {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 pad3;
    s16 unk4;
    s8 unk6;
};

void func_150AF738(s16 arg0, u8 arg1, s32 arg2) {
    struct sp18 sp18;

    sp18.unk0 = 1;
    sp18.unk1 = -1;
    sp18.unk2 = 2;
    sp18.unk4 = arg0;
    sp18.unk6 = 0;
    func_1515FF74(&sp18, 0, arg1, arg2);
}

extern void func_150B1DB0(void *, void *);

void func_150AF790(void *arg0, u8 *arg1) {
    func_150B1DB0(arg1, arg1 + 0x1ECC0);
}

typedef struct GameDC6B0SmokeTimers {
    s16 puff;
    s16 spark;
} GameDC6B0SmokeTimers;

typedef struct GameDC6B0SmokeSource {
    u8 pad0;
    u8 owner;
    u8 pad2[0xA];
    u8 group;
    u8 padD;
    s16 maxLifetime;
    u8 pad10[0x18];
    GameDC6B0SmokeTimers timers;
} GameDC6B0SmokeSource;

typedef struct GameDC6B0SmokePacket {
    f32 x, y;
    f32 scaleX, scaleY;
    u8 selector;
    u8 pad11;
    s16 lifetime, flags, field16, field18;
    u8 color0, color1, color2, color3, color4;
    u8 color5, color6, color7, color8, color9;
    s32 field24, field28, field2C, field30, field34, field38, field3C;
    u8 field40, field41;
    u8 pad42[2];
    u8 field44;
    u8 pad45[0x13];
} GameDC6B0SmokePacket;

typedef struct GameDC6B0SmokeMotion {
    f32 x, y, speed, angle;
} GameDC6B0SmokeMotion;

u8 func_151D8E6C(void);
void *func_1515548C(void *, u8, s32, s32, s32, u8, s32);
extern f32 D_8009F7D0;
extern f32 D_8009F7D4;

void func_150AF7C4(GameDC6B0SmokeSource *arg0) {
    u8 *result;
    struct {
        GameDC6B0SmokeMotion motion;
        GameDC6B0SmokePacket packet;
    } puff;
    GameDC6B0SmokeTimers *timers;
    GameDC6B0SmokePacket spark;

    timers = &arg0->timers;
    timers->puff -= D_800BE9E4;
    if (timers->puff < 0) {
        puff.motion.x = func_150ADA68() * 280.0f + -140.0f;
        puff.motion.y = func_150ADA68() * 50.0f;
        puff.motion.speed = 2.0f * (func_150ADA68() * D_8009F7D0);
        puff.motion.angle = func_150ADA68() * D_8009F7D4;
        puff.packet.x = 0.0f;
        puff.packet.y = 0.0f;
        func_150ADA68();
        puff.packet.selector = 0xFF;
        puff.packet.scaleX = 1.0f;
        puff.packet.scaleY = 115.0f;
        puff.packet.lifetime = func_150ADA20() % 71U + 20;
        if (arg0->maxLifetime < puff.packet.lifetime) {
            puff.packet.lifetime = arg0->maxLifetime;
        }
        puff.packet.flags = 0x21;
        puff.packet.field16 = 1;
        puff.packet.field18 = 0xFF;
        puff.packet.color0 = 0;
        puff.packet.color1 = 0xFF;
        puff.packet.color2 = 0xFF;
        puff.packet.color3 = 0xFF;
        puff.packet.color4 = func_150ADA20() % 71U + 40;
        puff.packet.color5 = 0xFF;
        puff.packet.color6 = 0xFF;
        puff.packet.color7 = 0xFF;
        puff.packet.color8 = 0xFF;
        puff.packet.color9 = 0xFF;
        puff.packet.field24 = 0;
        puff.packet.field28 = 0x200004;
        puff.packet.field2C = 0x1F0601;
        puff.packet.field30 = 0x19;
        puff.packet.field34 = 0x54;
        puff.packet.field38 = 0x80;
        puff.packet.field3C = 0x20;
        puff.packet.field40 = 0;
        puff.packet.field41 = 0xA;
        puff.packet.field44 = 0;
        result = func_1515548C(&puff.packet, 6, 0, 0, 0x10, arg0->group, arg0->owner);
        if (result != NULL) {
            memcpy(result + 0x70, &puff.motion, sizeof(puff.motion));
        }
        timers->puff = func_150ADA20() % 41U + 45;
    }
    timers->spark -= D_800BE9E4;
    if (timers->spark < 0) {
        spark.x = func_150ADA68() * 280.0f + -140.0f;
        spark.y = func_150ADA68() * 200.0f + -100.0f;
        spark.scaleX = spark.scaleY = 2.0f * func_150ADA68() + 1.0f;
        spark.selector = func_151D8E6C();
        spark.lifetime = func_150ADA20() % 7U + 5;
        if (arg0->maxLifetime < spark.lifetime) {
            spark.lifetime = arg0->maxLifetime;
        }
        spark.flags = 1 | (func_150ADA20() ? 2 : 0) | (func_150ADA20() ? 4 : 0) | 0x20;
        spark.field16 = 1;
        spark.field18 = 0xFF;
        spark.color0 = 0;
        spark.color1 = 0xFF;
        spark.color2 = 0xFF;
        spark.color3 = 0xFF;
        spark.color4 = func_150ADA20() % 121U + 80;
        spark.color5 = 0xFF;
        spark.color6 = 0xFF;
        spark.color7 = 0xFF;
        spark.color8 = 0xFF;
        spark.color9 = 0xFF;
        spark.field24 = 0;
        spark.field28 = 0x200004;
        spark.field2C = 0x1F0601;
        spark.field30 = 0x19;
        spark.field34 = 0x55;
        spark.field38 = 0x80;
        spark.field3C = 0x20;
        spark.field40 = 0;
        spark.field41 = 7;
        spark.field44 = 0;
        func_1515548C(&spark, 0, 0, 0, 0, arg0->group, arg0->owner);
        timers->spark = func_150ADA20() % 11U + 3;
    }
}

extern f32 sinf(f32);

struct Sub150AFBF4 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
};

struct Obj150AFBF4 {
    char pad0[0x10];
    f32 unk10;
    char pad14[0x5C];
    struct Sub150AFBF4 unk70;
};

s32 func_150AFBF4(struct Obj150AFBF4 *arg0) {
    struct Sub150AFBF4 *p = &arg0->unk70;
    p->unk8 = p->unk8 + p->unkC * D_800BE9A4;
    p->unk8 = func_15144B68(p->unk8);
    arg0->unk10 = sinf(p->unk8) * p->unk4 + p->unk0;
    return 1;
}

extern void func_1516D99C();

void func_150AFC68(s32 a0, s32 a1, s32 a2, s32 a3, u8 a4, s32 a5) {
    func_1516D99C(1, 0, 0, 0xD,
        0, 0, 0, 0, 0, 0, 0, 0,
        4, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0,
        0xFF, 0, 1, 0, 0, 0, 0, 0xAA,
        0xAA, 0xAA, 0xAA, a2, a3, 0, a0, 0xF0,
        0x50, 0x50, 1, 4, 0, 1, 0, 0,
        0, a1, 0, a4, a5);
}

extern void *D_800DCE94;

struct Node150AFDB0 {
    char pad0[0x8];
    struct Node150AFDB0 *unk8;
    char pad0C[0x33];
    u8 unk3F;
};

void func_150AFDB0(void) {
    struct Node150AFDB0 *node;

    D_800DD190++;
    node = D_800DCE94;
    while (node != 0) {
        ((struct Node150AFDB0 **)D_800DD198)[(s8)D_800DD190] = node->unk8;
        if (node->unk3F == D_800C3E78) {
            func_1516972C((struct102 *)node);
        }
        node = ((struct Node150AFDB0 **)D_800DD198)[(s8)D_800DD190];
    }
    D_800DD190--;
}

void func_150AFE64(s32 arg0) {
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    s32 idx;
    f32 *mtx;
    f32 zero;

    if (D_800D154C->unk1D4 != 0) {
        if (arg0 == 0) {
            idx = 3;
        } else {
            idx = 2;
        }

        zero = 0.0f;
        mtx = (f32 *)((u8 *)D_800D154C->unk1D4 + (idx << 6));
        spA4 = zero;
        spA0 = zero;
        sp9C = -20.0f;
        func_150A7960(mtx, spA4, spA0, sp9C, &spA4, &spA0, &sp9C);
        sp98 = zero;
        sp94 = zero;
        sp90 = -150.0f;
        func_150A7960(mtx, sp98, sp94, sp90, &sp98, &sp94, &sp90);
        func_150E1AB0(0, spA4, spA0, sp9C, sp98, sp94, sp90, 40.0f, zero, 2.0f, 120.0f, 0x3C, 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, zero, zero, zero, zero, zero, zero);
        func_150E2DB4(D_800D154C, D_800D154C->unique_id, (s16)idx, -1, zero, zero, -39.0f, zero, zero, -150.0f, 3, 0xFF, 4, 0);
    }
}

void func_150B003C(s32 arg0) {
    if (D_800BE9F0 == 6) {
        func_15179008(0);
        func_150AF200(0xE2, 0xE1);
        return;
    }
    func_150AF200(0xDF, 0xDE);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_DC6B0/func_150B0094.s")

void func_150B02C0(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x170) != 0) {
        func_1516972C(*(struct102 **)((u8 *)arg0 + 0x170));
    }
}

extern void func_150B02C0(void *);
extern void func_15132570(void *);

void func_150B02F0(void *arg0) {
    func_150B02C0(arg0);
    func_15132570(arg0);
}

extern void func_1513259C(void *);

void func_150B031C(void *arg0) {
    func_150B02C0(arg0);
    func_1513259C(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_DC6B0/func_150B0348.s")

extern s32 func_151149AC(u32);

struct Obj150B060C {
    f32 unk0;
    f32 unk4;
    s16 *unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
};

s32 func_150B060C(u8 arg0, struct Obj150B060C *arg1) {
    arg1->unk8 = (s16 *)func_151149AC(arg0);
    if (arg1->unk8 == 0) {
        return 0;
    }
    arg1->unk0 = -150.0f;
    arg1->unk4 = 4.5f;
    arg1->unkC = (f32)arg1->unk8[8];
    arg1->unk10 = (f32)arg1->unk8[9];
    arg1->unk14 = (f32)arg1->unk8[10];
    return 1;
}
