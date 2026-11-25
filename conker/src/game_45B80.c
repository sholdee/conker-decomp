#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void* func_151674F8(void *arg0, s32 arg1, s16 arg2, s32 arg3);
Gfx *func_15174AA4(Gfx *arg0, s32 arg1, s32 arg2);
Gfx *func_1510B7B4(Gfx *arg0, s32 arg1);
Gfx *func_151135C4(Gfx *arg0, s32 arg1, s16 arg2);
Gfx *func_15186794(Gfx *arg0, s32 arg1);
Gfx *func_15188D00(Gfx *arg0, s32 arg1, s16 arg2);
Gfx *func_151D6778(Gfx *arg0);
Gfx *func_1502BAD0(Gfx *arg0, s32 arg1, s16 arg2);
Gfx *func_15035FE8(Gfx *arg0, s32 arg1);
extern u8 D_80089470;
s32 func_1501878C(void);
void func_15019F20(void);


void func_150186D0(void) {
    func_10001444();
    func_1501A39C();
    func_151E6BFC();

    D_800BE5E0 = 1;
    D_800D18A0 = 0;
    D_800D18A2 = 0;
    D_800D18A4 = 0;
    D_800D23A9 = 0;

    while (func_1501878C() != 0) {};

    D_8002AC60 = 1;
    D_8002AC64 = 3500;
    *D_800BE728 = func_1501BBB8();

    while (func_151DCFD8(1) != 0) {};
}

// something big and interesting
typedef struct Game45B80Command {
    u32 w0;
    u32 w1;
} Game45B80Command;

void func_1517E1AC(void);
void func_151412BC(void);
void func_10004074(s32);
void func_151E51EC(void);
void func_1510B3B0(void);
void func_1510B51C(void);
void func_1501905C(void);
s32 func_15019130(void);
Gfx *func_1501A680(Gfx *);
void func_1515D69C(void);
void func_15167310(void);
void func_15018F80(s16);
s32 func_15019464(Gfx *, s16);
s32 func_1515D6D0(s32, s32);
Gfx *func_150195A0(Gfx *, s16);
void func_15019414(void);
s32 func_150198FC(Gfx *);
s32 func_15019BB8(Gfx *, s32);
s32 func_15019E60(Gfx *);
Gfx *func_15019CC8(Gfx *, s32);
void *func_151E84B0(void *);
s32 func_15043384(s32);
void func_150347E8(void);
void func_1502E474(void);
void func_15189118(s32);
void func_15084A18(void);
void func_15125628(void);
void func_1510D404(void);
void func_15114A1C(void);
void func_1517CFC4(void);
void func_1501C880(s32, s32);
void func_1000E2F4(s32);
s32 func_151F2CDC(void);
void func_151F2BE8(void);
void func_151F2C4C(void);
extern s32 D_800DC9F0;
extern s32 D_800BE9C8[];
extern s32 D_800BE9CC;
extern u8 D_800BEAC2;
extern u8 D_800BE630[];
extern u8 D_800BEBA0;
extern s32 D_800BEB9C;
extern s32 D_800BEBA4;
extern u8 D_800C1060[];
extern u8 D_800D9B90[][0x20];

s32 func_1501878C(void) {
    s16 *message;
    s16 i;

    message = NULL;
    osRecvMesg(&D_800BEA10, (OSMesg *)&message, 1);
    D_8002AC60 = 0;
    D_8002AC64 = 0x320;
    switch (*message) {
    case 2:
        func_1517E1AC();
        if (D_800DC9F0 != 0) {
            func_151412BC();
        }
        (*(s16 *)&D_800BEAA8)--;
        if (D_800BE615 != 0 && *(s16 *)&D_800BEAA8 == 0) {
            func_10004074(D_800BE9C8[0]);
            func_10004074(D_800BE9CC);
            D_800BE9C8[0] = 0;
            return 0;
        }
        break;
    case 1:
        if (D_800BE615 == 0) {
            func_151E51EC();
            if (D_800BE615 == 0) {
                if (D_800BE614 != 0) {
                    func_1501A39C();
                    D_800BE614 = 0;
                }
                if (D_800D9AF0 != 0) {
                    func_1510B3B0();
                }
                if (D_800D9B18 != 0) {
                    func_1510B51C();
                }
                func_1501905C();
                if (D_800BEAC0 != 0 || func_15019130() != 0) {
                    D_800BE9D0 = (s32)func_1501A680((Gfx *)D_800BE9D0);
                    func_1515D69C();
                    func_15167310();
                    for (i = 0; i <= D_80082FA0; i++) {
                        D_80082FA4 = D_800BE630[i];
                        func_15018F80(D_80082FA4);
                        D_800BE9D0 = func_15019464((Gfx *)D_800BE9D0, D_80082FA4);
                    }
                    if (D_800BEBA0 == 0) {
                        { Game45B80Command *command = (Game45B80Command *)D_800BE9D0; D_800BE9D0 += 8; command->w0 = 0xE1000000; command->w1 = D_800BEB9C;
                            command = (Game45B80Command *)D_800BE9D0; D_800BE9D0 += 8; command->w0 = 0xDD0007FF; command->w1 = (u32)D_800C1060; }
                        { Game45B80Command *command = (Game45B80Command *)D_800BE9D0; D_800BE9D0 += 8; command->w0 = 0xD9C2F9FA; command->w1 = 0; }
                        { Game45B80Command *command = (Game45B80Command *)D_800BE9D0; D_800BE9D0 += 8; command->w0 = 0xD9FFFFFF; command->w1 = 0x220005; }
                        { Game45B80Command *command = (Game45B80Command *)D_800BE9D0; D_800BE9D0 += 8; command->w0 = 0xDA380003; command->w1 = (u32)&D_80089470; }
                        { Game45B80Command *command = (Game45B80Command *)D_800BE9D0; D_800BE9D0 += 8; command->w0 = 0xDC08000A; command->w1 = (u32)D_800D9B90[D_800BE9C0]; }
                        { Game45B80Command *command = (Game45B80Command *)D_800BE9D0; D_800BE9D0 += 8; command->w0 = 0xDC08060A; command->w1 = (u32)(D_800D9B90[D_800BE9C0] + 0x10); }
                    }
                    {
                        s32 saved;

                        for (i = 0; i <= D_80082FA0; i++) {
                            saved = D_800BE9D0;
                            D_80082FA4 = D_800BE630[i];
                            D_800BE9D0 = func_1515D6D0(D_800BE9D0, D_80082FA4);
                            D_800BE9D0 = (s32)func_150195A0((Gfx *)D_800BE9D0, D_80082FA4);
                            if ((D_800BEBA4 < (D_800BE9D0 - D_800BE9C8[D_800BE9C0]) >> 3) ? 1 : 0) {
                                D_800BE9D0 = saved;
                            }
                        }
                        D_80082FA4 = 0;
                        func_15019414();
                        D_800BE9D0 = func_150198FC((Gfx *)D_800BE9D0);
                        for (i = 0; i <= D_80082FA0; i++) {
                            D_80082FA4 = D_800BE630[i];
                            D_800BE9D0 = func_1515D6D0(D_800BE9D0, D_80082FA4);
                            D_800BE9D0 = (s32)func_15019CC8((Gfx *)D_800BE9D0, D_80082FA4);
                            if (D_800BEAC0 == 0 && D_80082FA0 > 0) {
                                D_800BE9D0 = func_15019BB8((Gfx *)D_800BE9D0, D_80082FA4);
                            }
                        }
                        D_800BE9D0 = func_15019E60((Gfx *)D_800BE9D0);
                        if (D_800BEAC0 == 0 && D_80082FA0 == 0) {
                            for (i = 0; i <= D_80082FA0; i++) {
                                D_80082FA4 = D_800BE630[i];
                                D_800BE9D0 = func_15019BB8((Gfx *)D_800BE9D0, D_80082FA4);
                            }
                        }
                        D_800BE9D0 = (s32)func_151E84B0((void *)D_800BE9D0);
                        D_800BE9D0 = func_15043384(D_800BE9D0);
                    }
                    func_150347E8();
                    func_1502E474();
                    func_15189118(0);
                    func_15084A18();
                    func_15125628();
                    { Game45B80Command *command = (Game45B80Command *)D_800BE9D0; D_800BE9D0 += 8; command->w0 = 0xE9000000; command->w1 = 0; }
                    { Game45B80Command *command = (Game45B80Command *)D_800BE9D0; D_800BE9D0 += 8; command->w0 = 0xDF000000; command->w1 = 0; }
                    func_10004674();
                    func_1510D404();
                    func_15114A1C();
                    func_1517CFC4();
                    func_1501C880(0, 1);
                    D_800BE9B4 = 0;
                    if (D_800BEAC0 != 0) {
                        D_800BEAC2 = 2;
                    } else if (D_800BEAC2 > 0) {
                        D_800BEAC2--;
                    }
                    if (D_800BEAC0 != D_800BEAC1) {
                        D_800BEAC0 = D_800BEAC1;
                        if (D_800BEAC0 != 0) {
                            func_1000E2F4(1);
                            func_10011E94(1);
                            if (func_151F2CDC() != 0) {
                                func_151F2BE8();
                            }
                        } else {
                            func_1000E2F4(0);
                            func_10011E94(0);
                            if (func_151F2CDC() != 0) {
                                func_151F2C4C();
                            }
                        }
                    }
                }
            }
        }
        break;
    case 4:
        while (func_151DCFD8(1) != 0) {
        }
        for (;;) {
        }
    }
    return 1;
}

void func_15018DFC(void) {
    func_150A09D0(D_800BE9E4);
    func_150619A8();
    func_1501C870();
    if (D_800BEAC0 == 0) {
        if (D_800C35EA == 1) {
            func_1502378C();
        } else {
            func_1504A730();
        }
        func_15085ABC(D_800D2340);
    }
    func_1507C370();
    func_1507C8FC();
}

Gfx *func_15018E88(void *arg0, s16 arg1) {
    if (D_800BE616 == 0) {
        func_1517D7B0(&arg0, 1);
    }
    arg0 = func_151674F8(arg0, 0, arg1, 0);
    arg0 = func_151674F8(arg0, 0, arg1, 1);
    return func_15174AA4(arg0, D_800BE9F0, arg1);
}

void *func_15018F08(void *arg0, s16 arg1) {
    arg0 = func_15174B48(arg0, D_800BE9F0, arg1);
    arg0 = func_151674F8(arg0, 1, arg1, 0);
    arg0 = func_151674F8(arg0, 1, arg1, 1);

    func_1517D7B0(&arg0, 2);

    return arg0;
}

void func_15018F80(s16 arg0) {
    struct108 *sp24 = &D_800DBFF0[arg0];
    D_8003C8E0 = 0xA000000;
    if ((func_1517F40C(arg0) == 0) && (D_800BEAC0 == 0)) {
        func_1517F448(arg0);
    }
    func_1501B640(arg0);
    D_800D3674 = (sp24->unk388 < 0.0f) ? -1000 : 5000;
    func_15172D80(arg0);
    D_8003C8E0 = 0;
}

void func_1501905C(void) {
    D_800BE9D0 = D_800BE9D8[D_800BE9C0];
    *D_800BE728 = func_1501BBB8();
    func_15019F20();
    if ((D_800BEAC1 != 0) && (D_800BEAC0 == 0)) {
        func_15169040(0, 0x47);
    }
    func_1501C1B0();
    func_1501BB20();
    func_1502C380();
    if (0) {};
    func_1000D758(D_800DBFF0->unk2A4, D_800DBFF0->unk2A8, D_800DBFF0->unk2AC);
    func_10011BB8();
    if (D_800E0B94 == 2) {
        func_150ADACC(0x81280783); // 2166884227
    }
    func_10012020();
}

void func_1510D864(void);
void func_1509BA04(s32);
void func_1509BBA0(s32);
void func_1501E400(s32);
void func_15034F20(void);
void func_1510B690(void);
void func_1510F800(s32);
void func_15113180(void);
void func_15113E54(s32);
void func_15114188(void);
void func_15044A28(void);
void func_150242F8(s32, s32);
void func_1501EC38(s32);
void func_15020EC4(s32);
void func_1501E2F8(s32);
void func_15087CC0(void);
void func_1504ADD0(void);
void func_1510FC34(s32);
void func_15094EA0(s32);
void func_15112A80(s32);
void func_151749F8(s32, s32);
void func_15188B74(s32);
void func_1502BEE4(void);
void func_150A0D8C(void);
void func_15113218(void);
void func_151738C4(s32);
void func_1502C1A4(void);
void func_15113C88(void);
void func_150636F0(void);
void func_15183D28(void);
void func_151670C0(void);
void func_15177A94(void);
void func_151814FC(void);
void func_1517F75C(void);
void func_1517F7B4(void);
void func_15036148(void);
void func_1515D6C8(void);
void func_1516706C(void);
void func_151671E8(void);
extern u8 D_800DCD27;

// D_80043B40 is the 508-entry TLB slot table (see the handwritten miss handler at 0x10006138,
// which uses the same {page, age} layout); this pass ages every slot by one frame.
typedef struct {
    s32 page;
    s32 age;
} TlbSlot;

s32 func_15019130(void) {
    s32 i;
    u8 j;

    func_1510D864();
    func_1509BA04(0);
    func_1509BBA0(2);
    D_800DCD27 = 0;
    func_10004250();
    func_1501E400(0);
    func_15034F20();
    func_1510B690();
    func_1510F800(0);
    func_15113180();
    if (D_800BEAC0 == 0) {
        func_15113E54(1);
    }
    if (D_800BEAC0 == 0) {
        func_15114188();
    }
    func_15044A28();
    func_15018DFC();
    func_150242F8(1, 0);
    func_1501EC38(0);
    func_150242F8(0, 0);
    func_15020EC4(0);
    func_1501E2F8(0);
    if (*(s8 *)&D_800D23A9 != 0) {
        func_15087CC0();
    }
    func_15122AE0();
    func_1504ADD0();
    func_1510F800(0);
    for (i = 0; i <= D_80082FA0; i++) {
        func_1510FC34(i);
    }
    func_1510B690();
    for (j = 0; j <= D_80082FA0; j++) {
        func_15094EA0(j);
    }
    func_15112A80(0);
    func_151749F8(D_800BE9F0, 0);
    func_1501C860();
    func_1511FC20(D_800BE9C0);
    func_15188B74(0);
    if (D_800BEAC0 == 0) {
        func_1516706C();
        func_151671E8();
    }
    func_1502BEE4();
    if (D_800BE616 == 0) {
        func_150A0D8C();
    }
    func_15113218();
    func_15188B74(1);
    func_151738C4(D_800BE9F0);
    func_1502C1A4();
    func_15113C88();
    if (D_800CC2B0 != 0) {
        func_150636F0();
    }
    if (D_800BEAC0 == 0) {
        func_15183D28();
        func_151670C0();
        func_15177A94();
    }
    if (D_800BEAC0 == 0) {
        func_151814FC();
    }
    func_1517F75C();
    func_1517F7B4();
    func_15036148();
    func_1515D6C8();
    for (i = 0; i < 508; i++) {
        ((TlbSlot *)&D_80043B40)[i].age++;
    }
    return 1;
}

void func_15019414(void) {
    if (D_800BEAC0 == 0) {
        func_15167010();
    }
    func_1517ABB0();
    if (D_800BE616 == 0) {
        func_1508F0A4();
    }
}

s32 func_1501A490(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_1510B958(s32 arg0);
s32 func_1510B9D0(s32 arg0, s16 arg1);
s32 func_1515D6D0(s32 arg0, s32 arg1);
s32 func_1517EFAC(s32 arg0);
extern s16 D_80084480;
extern s32 D_800BE9F0;
extern s32 D_800BE628;
extern u8 D_800BE9C0;
extern u8 D_800BEAC0;
extern u16 D_800D18A0;
s32 func_1510FEA0(s32 arg0, s32 arg1);
s32 func_15019464(Gfx *arg0, s16 arg1)
{
 func_1510B958(arg1); { Gfx *_g = (Gfx *) (arg0++); _g->words.w0 = ((((unsigned int) ((((unsigned int) 0xdc) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) (((sizeof(Vp)) - 1) / 8)) & ((0x01 << 5) - 1)) << 19))) | ((unsigned int) ((((unsigned int) (0 / 8)) & ((0x01 << 8) - 1)) << 8))) | ((unsigned int) ((((unsigned int) 8) & ((0x01 << 8) - 1)) << 0)); _g->words.w1 = (unsigned int) ((((D_800BE628 ^ 0) + (arg1 * 0x180)) + (D_800BE9C0 * 0x10)) + 0x40); } ;
  arg0 = (Gfx *) func_1501A490((s32) arg0, arg1, 0, 0, 0, 0);
  if (D_800BEAC0 != 0)
  {
    return (s32) arg0;
  }
  if (D_80084480 != 0)
  {
    return (s32) arg0;
  }
  arg0 = (Gfx *) func_1510FEA0((s32) arg0, D_800BE9F0);
  if ((func_1517EFAC(arg1) != 0) || ((D_800D18A0 & (1 << arg1)) != 0))
  {
    return (s32) arg0;
  }
  arg0 = (Gfx *) func_1515D6D0((s32) arg0, arg1);
  return func_1510B9D0((s32) arg0, arg1);
}

// NON-MATCHING: 70% of the way there... maybe.
// s32 func_15019464(Gfx *arg0, s16 arg1) {
//     s32 temp_s0;
//     s32 ret;
//
//     func_1510B958(arg1);
//     gSPViewport(arg0++, (D_800BE628 + (arg1 * 0x180) + (D_800BE9C0 * 0x10) + 0x40));
//     // arg0->unk0 = 0xDC080008;
//     // arg0->unk4 = (s32) (D_800BE628 + (temp_t6 * 0x180) + (D_800BE9C0 * 0x10) + 0x40);
//     temp_s0 = func_1501A490(arg0, arg1, 0, 0, 0, 0);
//     if ((D_800BEAC0 != 0) || (D_80084480 != 0)) {
//         return temp_s0;
//     }
//
//     temp_s0 = func_1510FEA0(temp_s0, D_800BE9F0);
//     if ((func_1517EFAC(arg1) != 0) || ((D_800D18A0 & (1 << arg1)) != 0)){
//         return temp_s0;
//     }
//
//     temp_s0 = func_1515D6D0(temp_s0, arg1);
//     temp_s0 = func_1510B9D0(temp_s0, arg1);
//     return temp_s0;
//
// }

Gfx *func_150195A0(Gfx *arg0, s16 arg1)
{
    struct108 *sp2C;
    Gfx *temp_v0;

    sp2C = &D_800DBFF0[arg1];
    if (D_800BEAC0 != 0) {
        return arg0;
    }
    if (D_80084480 != 0) {
        return arg0;
    }
    if ((func_1517EFAC(arg1) != 0) || ((D_800D18A0 & (1 << arg1)) != 0)) {
        return arg0;
    }
    if (D_80082FA0 > 0) {
        arg0 = func_1515D6D0((s32)func_1510B7B4(arg0, arg1), arg1);
        func_1510B958(arg1);
        gSPViewport(arg0++, (((D_800BE628 ^ 0) + (arg1 * 0x180)) + (D_800BE9C0 * 0x10)) + 0x40);
        arg0 = (Gfx *) func_1501A490((s32) arg0, arg1, 0, 0, 0, 0);
    }
    arg0 = func_151135C4(arg0, 1, arg1);
    if (D_800BE575 == 2) {
        arg0 = func_151D6778(arg0);
    }
    arg0 = func_151674F8(arg0, 6, arg1, 0);
    arg0 = func_151674F8(arg0, 6, arg1, 1);
    arg0 = func_15186794(arg0, arg1);
    arg0 = (Gfx *) func_1501A490((s32) arg0, arg1, 0, 0, 0, 0);
    arg0 = func_1502BAD0(arg0, 1, arg1);
    temp_v0 = func_15035FE8(arg0, arg1);
    arg0 = temp_v0;
    gSPMatrix(arg0++, &D_80089470, G_MTX_LOAD);
    if (sp2C->unk5F0 & 1) {
        arg0 = func_15018E88(arg0, arg1);
        arg0 = func_151135C4(arg0, 0, arg1);
        arg0 = func_15018F08(arg0, arg1);
    } else {
        arg0 = func_15018F08(arg0, arg1);
        arg0 = func_151135C4(arg0, 0, arg1);
        arg0 = func_15018E88(arg0, arg1);
    }
    arg0 = func_15188D00(arg0, 0, arg1);
    arg0 = func_15188D00(arg0, 1, arg1);
    arg0 = func_1502BAD0(arg0, 2, arg1);
    arg0 = func_151674F8(arg0, 2, arg1, 0);
    return func_151674F8(arg0, 2, arg1, 1);
}

// PERMUTER CANDIDATE (best 1470): logic verified correct (build OK; diff is ~all register-only
// 'r' marks). A uniform +1 temp-register offset begins in the viewport address expression
// (mine: mult=t2/D_800BE628=t3; target: t1/t2) despite identical preceding temps, and cascades.
// Needs decomp-permuter. Reconstruction (add file-local externs s8 D_80082F90; s32 D_8002AAEC;
// and prototypes func_151D5E90/1501A680/151D61B0/1517E28C/15172F70/15181EE0/1517F564/1508FD38/15096934):
// void func_150198FC(Gfx *arg0) {
//     if (D_800BE575 == 1) { arg0 = func_151D6778(arg0); }
//     if (D_80082FA0 != 0) { D_80082FA4 = D_80082FA0 + 1;
//         gSPViewport(arg0++, (D_80082FA4 * 0x180) + D_800BE628 + 64);
//     } else { gSPViewport(arg0++, D_800BE628 + 64); }
//     arg0 = (Gfx *)func_1501A490((s32)arg0, 0xFF, 0, 0, 0, 0);
//     if (D_800BEAC0 != 0) {
//         if (D_80082F90 == 0) {
//             gDPSetColorImage(arg0++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800BE620, D_800BE9C4);
//             gDPSetScissor(arg0++, G_SC_NON_INTERLACE, 0, 0, D_800BE620, D_800BE624);
//             arg0 = func_151D5E90(arg0, (&D_8002AAEC)[-D_800BE9C0], 0, 1);
//             arg0 = func_1501A680(arg0); D_80082F90 = 1;
//         } else if (D_80082F90 < 5) { D_80082F90 = D_80082F90 + 1; func_151D61B0(D_800BE9C4); }
//         arg0 = func_151D5E90(arg0, D_800BE9C4, 0, 1);
//     } else { D_80082F90 = 0; }
//     if (func_1517EFAC(0) == 0) { arg0 = func_1517E28C(arg0, D_800BE9F0);
//         arg0 = func_15172F70(arg0); arg0 = func_15181EE0(arg0); }
//     arg0 = func_1517F564(arg0);
//     if ((D_800BEAC0 == 0) && (D_800BEAC1 == 0)) { arg0 = func_1508FD38(arg0); }
//     arg0 = (Gfx *)func_151674F8(arg0, 3, 0, 0);
//     arg0 = (Gfx *)func_151674F8(arg0, 3, 0, 1);
//     func_15096934(arg0);
// }
Gfx *func_151D5E90(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_151D61B0(s32 arg0);
Gfx *func_1517E28C(Gfx *arg0, s32 arg1);
Gfx *func_15172F70(Gfx *arg0);
Gfx *func_15181EE0(Gfx *arg0);
Gfx *func_1517F564(Gfx *arg0);
Gfx *func_1508FD38(Gfx *arg0);
s32 func_15096934(Gfx *arg0);
s32 func_150198FC(Gfx *arg0) {
    static s8 D_80082F90;
    if (D_800BE575 == 1) {
        arg0 = func_151D6778(arg0);
    }
    if (D_80082FA0 != 0) {
        D_80082FA4 = D_80082FA0 + 1;
        gSPViewport(arg0++, (D_80082FA4 * 0x180) + D_800BE628 + 64);
    } else {
        gSPViewport(arg0++, D_800BE628 + 64);
    }
    arg0 = (Gfx *)func_1501A490((s32)arg0, 0xFF, 0, 0, 0, 0);
    if (D_800BEAC0 != 0) {
        if (D_80082F90 == 0) {
            gDPSetColorImage(arg0++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800BE620, D_800BE9C4);
            gDPSetScissor(arg0++, G_SC_NON_INTERLACE, 0, 0, D_800BE620, D_800BE624);
            arg0 = func_151D5E90(arg0, D_8002AAE8[1 - D_800BE9C0], 0, 1);
            arg0 = func_1501A680(arg0);
            D_80082F90 = 1;
        } else if (D_80082F90 < 5) {
            D_80082F90 = D_80082F90 + 1;
            func_151D61B0(D_800BE9C4);
        }
        arg0 = func_151D5E90(arg0, D_800BE9C4, 0, 1);
    } else {
        D_80082F90 = 0;
    }
    if (func_1517EFAC(0) == 0) {
        arg0 = func_1517E28C(arg0, D_800BE9F0);
        arg0 = func_15172F70(arg0);
        arg0 = func_15181EE0(arg0);
    }
    arg0 = func_1517F564(arg0);
    if ((D_800BEAC0 == 0) && (D_800BEAC1 == 0)) {
        arg0 = func_1508FD38(arg0);
    }
    arg0 = (Gfx *)func_151674F8(arg0, 3, 0, 0);
    arg0 = (Gfx *)func_151674F8(arg0, 3, 0, 1);
    return func_15096934(arg0);
}

s32 func_15019BB8(Gfx *arg0, s32 arg1) {
    gSPViewport(arg0++, (s32)((u8 *)D_800BE628 + arg1 * 0x180) + D_800BE9C0 * 0x10 + 64);
    arg0 = (Gfx *)func_1501A490((s32)arg0, arg1, 0, 0, 0, 0);
    arg0 = (Gfx *)func_1517F4D8((s32)arg0, arg1);
    arg0 = (Gfx *)func_1517F3A0((s32)arg0, arg1);
    arg0 = (Gfx *)func_15180580((s32)arg0, arg1);
    if ((D_800D18A0 & (1 << arg1)) || (D_800D18A2 & (1 << arg1))) {
        arg0 = (Gfx *)func_1507DB6C((s32)arg0, arg1);
    }
    arg0 = (Gfx *)func_151674F8(arg0, 4, arg1, 0);
    return (s32)func_151674F8(arg0, 4, arg1, 1);
}
// NON-MATCHING: need to figure out what is going on
// void func_15019BB8(struct14 *arg0, s32 arg1) {
//     s32 temp_v0;
//     s32 sp2A;
//     s16 sp28;
//
//     arg0->unk0 = 0xDC080008;
//     arg0->unk4 = &D_800BE628[arg1].unk40[D_800BE9C0];
//     sp28 = arg1;
//
//     temp_v0 = func_1501A490(&arg0->unk8, &sp28, 0, 0, 0, 0);
//     temp_v0 = func_1517F4D8(temp_v0, arg1);
//     temp_v0 = func_1517F3A0(temp_v0, arg1);
//     temp_v0 = func_15180580(temp_v0, arg1);
//     // temp_v1 = 1 << arg1;
//     if (((D_800D18A0 & (1 << arg1)) != 0) || ( ((D_800D18A2 & (1 << arg1)) != 0))) {
//         temp_v0 = func_1507DB6C(temp_v0, arg1);
//     }
//     temp_v0 = func_151674F8(temp_v0, 4, sp2A, 0);
//     func_151674F8(temp_v0, 4, sp2A, 1);
// }

typedef struct Cam15019CC8 {
    u8 pad0[0x40];
    Vp vp[2];
    u8 pad60[0x100 - 0x60];
    Mtx proj[2];
} Cam15019CC8;

Gfx *func_15019CC8(Gfx *gfx, s32 arg1) {
    gSPViewport(gfx++, &((Cam15019CC8 *)D_800BE628)[arg1].vp[D_800BE9C0]);
    gfx = (Gfx *)func_1501A490((s32)gfx, (s16)arg1, 0, 0, 0, 0);
    gSPMatrix(gfx++, &D_80089470, G_MTX_LOAD);
    gSPMatrix(gfx++, &((Cam15019CC8 *)D_800BE628)[arg1].proj[D_800BE9C0], G_MTX_PROJECTION | G_MTX_LOAD);
    if (D_800BE9F0 == 0x1D) {
        gfx = func_1502BAD0(gfx, 6, (s16)arg1);
    }
    gfx = (Gfx *)func_151674F8(gfx, 5, 0, 0);
    gfx = (Gfx *)func_151674F8(gfx, 5, 0, 1);
    gSPMatrix(gfx++, &((Cam15019CC8 *)D_800BE628)[arg1].proj[D_800BE9C0], G_MTX_PROJECTION | G_MTX_LOAD);
    gSPMatrix(gfx++, &((Mtx **)&D_800DC2A0)[D_800BE9C0][arg1], G_MTX_PROJECTION);
    return gfx;

}

s32 func_15019E60(Gfx *arg0) {
    if (D_80082FA0 != 0) {
        D_80082FA4 = D_80082FA0 + 1;
        gSPViewport(arg0++, (D_80082FA4 * 0x180) + D_800BE628 + 64);
    } else {
        gSPViewport(arg0++, D_800BE628 + 64);
    }
    return func_15043384(func_151E8620(func_1501A490(arg0, 0xFF, 0, 0, 0, 0)));
}

extern u32 D_800BE610;
extern u8 D_800BE619;
extern u8 D_800BE61A;
extern s32 D_800BE9AC;
extern s32 D_800BE9B0;
extern f32 D_800BE9B8;
extern f32 D_800BE9BC;
extern u8 D_800BEA0C;
f32 D_800BE5E8[11];
void func_150A019C(void);

void func_15019F20(void) {
    s32 ticks;
    u8 rate;
    f32 sum;
    u32 i;

    ticks = D_800BE9E4;
    D_800BEA08 = ticks;
    rate = D_800BEA0C;
    if (rate) {
        s32 pad;
        if (rate == 1) {
            D_800BE9E4 = 0;
            D_800BE5E0 = 0;
            ticks = 0;
            D_800BE9A4 = 0.0f;
        } else {
            i = ((u32*)D_800BE5E8)[10];
            i += ticks;
            D_800BE9A4 = (f32)ticks / (f32)(u32)rate;
            D_800BE9E4 = D_800BE9A4;
            D_800BE9A4 *= 0.5f;
            if (i >= rate) {
                D_800BE9E4 = i / rate;
                i -= D_800BE9E4 * i;
                if (1) {} if (1) {} if (1) {} if (1) {} if (1) {}
                D_800BE610 = i;
                ticks = D_800BE9E4;
            } else {
                D_800BE610 = i;
                D_800BE9E4 = ticks = 0;
            }
        }
    } else {
        D_800BE9A4 = (f32)ticks * 0.5f;
    }
    D_800BE5E0 &= 1;
    D_800BE5E0 += ticks;
    D_800BE9A0 = D_800BE5E0 >> 1;
    if (D_800BE9A4 != 0) {
        D_800BE9A8 = 1.0f / D_800BE9A4;
    } else {
        D_800BE9A4 = 0.0f;
        D_800BE9A8 = 0.0f;
        ;
    }
    D_800BE9AC += ticks;
    D_800BE9B0 = D_800BE9AC % 30;
    D_800BE5E8[D_800BE619] = D_800BE9A4;
    D_800BE619++;
    if (D_800BE619 == 10) {
        D_800BE619 = 0;
    }
    if (D_800BE61A < 10) {
        D_800BE61A++;
        D_800BE9B8 = D_800BE9A4;
        D_800BE9BC = D_800BE9A8;
    } else {
        sum = 0.0f;
        for (i = 0; i < 10; i++) {
            sum += D_800BE5E8[i];
        }
        D_800BE9B8 = sum * 0.1000000015f;
        if (D_800BE9B8 != 0) {
            D_800BE9BC = 1.0f / D_800BE9B8;
        } else {
            D_800BE9BC = 0.0f;
        }
    }
    func_150A019C();
}
