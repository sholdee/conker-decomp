#include <ultra64.h>
#include "functions.h"
#include "variables.h"


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

#pragma GLOBAL_ASM("asm/nonmatchings/game_126F60/func_150F9BB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_126F60/func_150FA1B8.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/game_126F60/func_150FAAEC.s")

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
