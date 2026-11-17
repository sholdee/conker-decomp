#include <ultra64.h>
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

struct Struct151E5034 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u16 unk20;
    char pad22[8];
    s8 unk2A;
    s8 unk2B;
    s8 unk2C;
    char pad2D[0x11];
    s8 unk3E;
    s8 unk3F;
    char pad40;
    s8 unk41;
    char pad42;
    s8 unk43;
    s8 unk44;
};

struct Struct151E4314 {
    s16 unk0;
    s8 unk2;
    s8 unk3;
};

extern s8 D_800AB692[];
extern struct Struct151E4314 D_800AB7A4[];
extern s8 D_800E0C00[];
extern s8 D_800E0BEB;
extern s16 D_800E0A80;
extern s16 D_8008FDCC;
extern u8 **D_800E0BD8;
extern void func_1501D348(s32, s32, s32, s32, s32);
void func_151E2834(void);
s32 func_1517EFDC(void);
extern s32 D_800E0A90;
extern u8 D_8008FE28;
extern s8 D_800E0B96;
extern u8 D_800D2E40;
extern u8 D_800E0B94;
extern u8 D_8008FDA4;
extern s16 D_800E0B9A;
extern u8 D_800E0B95;
extern u8 D_800E0BDC;
extern u16 D_800BE930;
extern s8 D_800AB690[];
extern s32 D_800AB754[];
extern char D_800AB850[];
extern void (*D_800E0A88)(void);
extern u8 D_800E0A95;
extern s8 D_800E0A96;
extern s8 D_8008FDE0;
extern void (*D_8008FDEC[])(void);
void func_1000E934(void);
void func_1000F1A8(void);
void func_1501C730(s32, s32, s32, s32, s32);
void func_15042D94(s32, s32, s32, char *, ...);
void func_151E2404(void);
void func_151E3344(s32, s32, s32, s32, s32);
void func_151E6964(s32);
void func_151E530C(void);
void func_151E55A8(void);
void func_151E557C(void);
void func_151E7EF8(void);
s32 func_151F2CDC(void);
void func_151F2D6C(s32, s32);
s32 func_1509CA30(s32);
s32 func_1509CA50(s32);
void func_1509CC94(s32);
void func_1509CDDC(s32);
void func_1509CF28(s32, s32 *);
void func_1509D054(void);
void func_1509D08C(s32, s32, s32 *, s32 *);
extern s8 D_8008FD70;
extern s16 D_8008FDAC;
extern u8 D_8008FDB4;
extern s8 D_8008FF00;
extern u8 D_8008FF04;
extern s32 D_800BE3E4;
extern s8 D_800D23A8;
extern s8 D_800E0B99;
extern s8 D_800E0BB1;
extern u8 D_800E0BD1;

extern s8 D_800E0BE0[];
extern s8 D_8008FE54[];
extern s8 D_8008FE6C[];
extern s8 D_8008FE84[];
void func_151DD9E4(void);
s32 func_151DDB94(s32 arg0);
void func_151DDBA0(void);

extern u8 D_8008FDB8;
extern u8 D_800C35E8;
extern s8 D_8008FEEC;
extern s8 D_8008FEF0;
extern s8 D_8008FD98;
extern u8 D_8008FDCD;
extern u8 D_800BEAC3;
extern u8 D_800E0A98;
extern s8 D_800E0A99;
extern s8 D_800E0A85;
extern s8 D_800E0BE3;
extern s32 D_800D2E48;
extern s32 D_8008FD78;
extern OSContPad D_800BE918[];
extern char D_800AB7D4[];
extern char D_800AB7DC[];
extern char D_800AB7E0[];
extern char D_800AB7E4[];
extern char D_800AB7E8[];
extern char D_800AB7EC[];
extern char D_800AB7F0[];
extern char D_800AB7F4[];
extern char D_800AB7F8[];
extern char D_800AB7FC[];
extern OSIoMesg D_800388E0;
extern s8 D_8008FE2C;
extern s8 D_8008FD74;
extern s8 D_8008FD9C;
extern struct Struct151E5034 *D_8008FDD4;
void func_15042E3C(char *, s32);

#define mode modeA[0]
void func_151DDC20(void) {
    static s8 D_8008FEF4;
    s8 sx;
    s8 sy;
    u8 skip;
    u8 modeA[1];
    s32 i;
    s32 y;
    s32 max;
    s32 sfx;
    s32 ypos;
    OSContPad *pad;
    s32 t;
    s32 x;
    u32 buf[8];
    u32 addr;

    s32 dummy;

    D_800E0B96 = 0;
    sfx = 0;
    skip = 0;
    mode = D_8008FDB8;
    if (D_800BE9F0 == 0x22) {
        mode = 1;
    }
    if ((D_800C35EA != 1) || (D_800BE9F0 != 0x18) || (D_800C35E8 != 1)) {
        if (func_1517EFDC() != 0) {
            skip = 1;
        }
    }
    if ((D_800C35EA == 1) && (D_800C3683 != 0)) {
        skip = 1;
    }
    if (D_8008FEEC > 0) {
        D_8008FEEC--;
    }
    if ((D_800BEAC0 == 0) && (D_800BEAC1 == 0) && (skip == 0) && (D_8008FEEC == 0)) {
        for (i = 0; (i <= D_80082FA0) && (D_800BEAC1 == 0); i++) {
            if (((&D_800BE930)[i] & 0x1000) && !(D_800D18A0 & (1 << i))) {
                D_800BEAC1 = 1;
                D_800BEAC3 = 1;
                D_800E0A98 = i;
                D_8008FE2C = 0;
                D_8008FEF4 = D_800BE918[i].stick_y;
                D_8008FEEC = 10;
            }
        }
    }
    if ((D_800BEAC0 == 0) && (D_800D2E48 != 0xC7)) {
        D_800E0A99 += D_800BE9E4;
        if (D_800E0A99 >= 60) {
            D_800E0A99 -= 60;
            D_800BE3E8++;
        }
    }
    if ((D_800BE616 != 0) && (D_800BEAC1 == 0) && (D_800E0BE3 >= 3)) {
        x = 0xC6;
        if (D_80082FA0 > 0) x = 0x64;
        if (D_8008FD78 > 0) {
            D_8008FD78 = D_8008FD78 - D_800BEA08;
        } else {
            D_8008FD78 = 0;
        }
        t = D_8008FD78 / 60;
        func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
        func_15042D94(0x87, x, 0x80, D_800AB7D4, t / 60, t % 60);
    }
    if (D_8008FEF0 > 0) {
        D_8008FEF0--;
        if (D_8008FEF0 < 2) {
            D_800BEAC1 = 0;
        }
    } else if (D_800BEAC0 != 0) {
        if (D_800BE616 != 0) {
            y = -0x24;
            max = 3;
        } else {
            y = 5;
            max = 2;
        }
        D_800E0B9A = (&D_800BE930)[D_800E0A98];
        if (D_8008FEEC != 0) {
            D_800E0B9A = 0;
        }
        pad = &D_800BE918[D_800E0A98];
        sx = pad->stick_x;
        sy = pad->stick_y;
        if (D_8008FE2C < max) {
            if (((D_8008FEF4 < 0x1F) && (sx >= 0x1F)) || ((D_8008FEF4 >= -0x1E) && (sx < -0x1E))) {
                if (D_8008FE2C == 2) {
                    D_8008FE2C = 0;
                } else {
                    D_8008FE2C = 1 - D_8008FE2C;
                }
                sfx = 0x62D;
            }
            if ((D_8008FE2C != 0) && (max == 3) && (((D_800E0A85 >= -0x1E) && (sy < -0x1E)) || ((D_800E0A85 < 0x1F) && (sy >= 0x1F)))) {
                sfx = 0x62D;
                if (D_8008FE2C == 2) {
                    D_8008FE2C = 1;
                } else {
                    D_8008FE2C = 2;
                }
            }
        } else if (((D_8008FEF4 >= -0x1E) && (sx < -0x1E)) || ((D_8008FEF4 < 0x1F) && (sx >= 0x1F))) {
            D_8008FD98 = 1 - D_8008FD98;
            sfx = 0x62D;
        }
        D_8008FEF4 = sx & 0xFF;
        D_800E0A85 = sy;
        func_1504332C(0xFF, 0xFF, 0xFF, D_8008FDCD);
        if (mode != 0) {
            ypos = y + 0x73;
            D_8008FE2C = 0;
            func_15042D94(0x94, ypos, 1, D_800AB7DC, 0xD);
        } else {
            ypos = y + 0x73;
            func_15042D94(0x94, ypos, 1, D_800AB7E0, 0x1A);
            if (D_8008FE2C < max) {
                func_15042D94(0xD5, ypos, 1, D_800AB7E4, (D_8008FE2C == 0) + 0xC);
            }
            func_150432FC(0x94, y + 0x55);
            if (max == D_8008FE2C) {
                func_15042E3C(D_800AB7E8, 0x32);
            } else if (max < D_8008FE2C) {
                func_15042E3C(D_800AB7EC, 0x33);
            }
        }
        func_150432FC(0x53, ypos);
        if (D_8008FE2C < max) {
            if (mode == 0) {
                i = 1;
                if (max == 3) {
                    func_150432FC(0x53, y + 0x63);
                    func_15042E3C(D_800AB7F0, (D_8008FE2C == 1) + 0x21);
                    i = 2;
                    func_150432FC(0x53, y + 0x82);
                }
                func_15042E3C(D_800AB7F4, (i == D_8008FE2C) + 0xE);
            }
            if (D_800E0B9A & 0x1000) {
                D_8008FEF0 = 5;
                D_8008FEEC = 10;
            } else if ((D_8008FE2C == 0) || (D_800E0B9A & 0x4000)) {
                if (D_800E0B9A & 0xC000) {
                    D_8008FEF0 = 5;
                    D_8008FEEC = 10;
                }
            } else if (D_800E0B9A & 0x8000) {
                D_8008FD98 = 0;
                D_8008FE2C = D_8008FE2C + max - 1;
                if (max == 2) {
                    D_8008FE2C++;
                }
                sfx = 0x500;
            }
        } else {
            func_15042D94(0x6C, ypos, 1, D_800AB7F8, D_8008FD98 + 0x1F);
            func_15042D94(0xBC, ypos, 1, D_800AB7FC, 0x1E - D_8008FD98);
            if (D_800E0B9A & 0x8000) {
                if (D_8008FD98 != 0) {
                    D_800E0B94 = 7;
                    D_800E0A88 = func_151DDBA0;
                    D_8008FD74 = 8;
                    if (D_800BE616 != 0) {
                        D_8008FDD4->unk3F = 4;
                        if (max == D_8008FE2C) {
                            D_8008FD9C = 1;
                            D_800E0A88 = func_151E2834;
                        }
                    } else if (D_8008FDA8 >= 0) {
                        func_15007718(D_8008FDA8);
                    }
                    D_8008FE2C = 0;
                    D_80087270[0] = 0;
                } else {
                    u32 *p;

                    p = buf;
                    while ((s32)p & 0xF) {
                        p++;
                    }
                    addr = func_151DDB94(-0xF81);
                    osWritebackDCache(p, 0x10);
                    osPiStartDma(&D_800388E0, 0, 0, addr, p, 0x10, gMessageQueue);
                    osRecvMesg(gMessageQueue, NULL, 1);
                    osInvalDCache(p, 0x10);
                    p[0] -= 0x3E0EF0F0;
                    p[1] -= 0x3E0EF0F0;
                    if ((p[0] != 0x3050A14A) || (p[1] != 0xBCFCF506)) {
                        osPiStartDma(&D_800388E0, 0, 0, func_150ADA20() & 0xFFFF, (void *)((func_150ADA20() & 0xFFFFE) + 0x80300000), 0x100, gMessageQueue);
                        osRecvMesg(gMessageQueue, NULL, 1);
                    }
                    D_8008FE2C -= 2;
                }
                sfx = 0x500;
            } else if (D_800E0B9A & 0x4000) {
                D_8008FE2C -= 2;
            }
        }
        if (sfx != 0) {
            func_10010F30(sfx, 0x5DC0, 0x40, 0, 0);
        }
    }
}

void func_151DE6CC(void) {
}
#undef mode

void func_151DE6D4(void) {
    s32 timer;

    func_151E530C();
    D_800CC2D0[0].unk25C |= 0x200;
    D_800CC2D0[0].y_position = 1000.0f;

    if (D_80000300 == 0) {
        D_800E0A90 = 0;
    }

    timer = D_800E0A90;
    if (timer >= 0x14B) {
        if (D_800E0B9A & 0x8000) {
            if (timer >= 0x1E0) {
            } else {
                timer = D_800E0A90 = 0x1E0;
            }
            if (timer < 0x259) {
            } else {
                if (timer >= 0x2D0) {
                } else {
                    timer = D_800E0A90 = 0x2D0;
                }
            }
        }
    }

    if (timer >= 0x349) {
        D_8008FE28 = 2;
        D_800E0B94 = 1;
        D_800D2E40 = 0;
        func_1501C730(6, 0x21, 0, 0, 1);
        *(u8 *)&D_800E0B96 = 0xFF;
    }
}

extern s32 D_800E0A90;
extern u8 D_800E0B97;
extern u8 D_800E0B98;
extern u8 D_800E0A8C;
extern u8 D_8008FE28;
void func_151DE85C(void);

void func_151DE7D4(void) {
    D_800E0A90 = 0;
    D_800E0B97 = 0;
    D_800E0B98 = 0;
    D_800E0A8C = 0;
    D_8008FE28 = 2;
    func_151DE85C();
}

extern s8 D_8008FE30;
extern s8 D_8008FD74;
extern s8 D_800E0B96;

void func_151DE81C(void) {
    D_8008FD74 = 4;
    D_800E0B96 = 0;
    if (D_8008FE30 == 0) {
        func_1500764C();
    }
}

extern u8 D_800D2E40;
extern u8 D_800E0B94;
extern u8 D_8008FD80;
extern u8 D_8008FDA4;
extern struct Struct151E5034 *D_8008FDD4;
void func_1501C730(s32, s32, s32, s32, s32);

void func_151DE85C(void) {
    D_800D2E40 = 0;
    func_1501C730(6, 0x1D, 0, 0, 1);
    D_800E0B94 = 3;
    D_8008FD80 = 1;
    D_8008FE28 = 2;
    D_8008FDA4 = 0;
    D_8008FDD4->unk3E = 0;
    D_8008FDD4->unk2B = 5;
    D_8008FDD4->unk2C = D_8008FDD4->unk2B;
}

void func_151DE8E8(void) {
}

extern u8 D_800E0A94;
extern s8 D_800E0BE9;
extern s8 D_8008FEF8;
extern u8 D_8008FE34;
extern f32 D_8008FDE8;
extern u8 D_800E0A97;
extern char D_800AB800[];
void func_1001263C(s32 arg0, s32 arg1, s32 arg2);
struct216 *func_1507BB28(s32, s32);
void func_1000DE1C(s32, s32);
void func_151E7E9C(void);
void func_151DF1BC(void);

struct Struct151DFF38 {
    u32 unk0;
    s32 unk4;
    s32 unk8;
    s8 unkC;
    s8 padD;
    s8 unkE;
    s8 padF;
};

extern s8 D_8008FE2C;
extern u8 D_800E0A94;
extern char D_800AB814[];
extern char D_800AB818[];
extern char D_800AB828[];
extern char D_800AB82C[];
extern char D_800AB830[];
extern char D_800AB834[];
extern char D_800AB838[];
extern char D_800AB83C[];
extern char D_800AB840[];
void func_15042E3C(char *, s32);
void func_151E327C(void);
void func_151DFF38(void);
void func_151E09DC(void);

extern s8 D_8008FE40;
extern s8 D_8008FE44[];
extern u8 D_800E0B8C;
extern s8 D_800E0B90[];
extern s8 D_800AB68C[];
extern struct127 *D_800E0BA0[];
extern u32 D_8008FEA4[];
extern s8 D_8008FEC4[];
extern char D_800AB854[];
extern s8 D_8008FE2C;
void func_151E7F60(s32 arg0, s32 arg1);
void func_151E0B70(void);

void func_151E4314(void);
void func_15042E3C(char *, s32);
extern s8 D_8008FDA0;
extern s8 D_8008FE40;
extern s8 D_8008FE44[];
extern s8 D_8008FEE3[];
extern s8 D_800AB74C[][4];
extern char D_800AB858[];
extern char D_800AB85C[];
extern char D_800AB860[];
void func_151E1214(void);

extern s8 D_8008FDC4;
typedef struct { s8 v[4]; } Offs4;
extern Offs4 D_8008FF34;
extern u8 D_800AB62C[];
extern s8 D_800AB710[][12];
extern char D_800AB864[];
extern char D_800AB868[];
extern char D_800AB86C[];
extern char D_800AB870[];
extern char D_800AB874[];
extern char D_800AB878[];
extern char D_800AB87C[];
extern char D_800AB880[];
extern char D_800AB884[];
extern char D_800AB88C[];
extern char D_800AB890[];
extern char D_800AB894[];
extern char D_800AB898[];
extern s8 D_800E0BB0;
extern s8 D_800E0BC8[];
s32 func_151E24F0(s32 *arg0);
void func_151E1744(void);

extern u8 D_8008FD80;
void func_151E530C(void);
void func_151E43DC(void);
void func_151E2284(void);

extern s8 *D_80082BBC;
void func_151E2404(void);

extern s8 D_800AB694[];
extern s8 D_8008FE44[];
extern s8 D_800E0BCE[];
extern s8 D_800E0BEE;
extern u8 D_800BE740;
s32 func_151E24F0(s32 *arg0);

typedef struct {
    f32 unk0;
    s8 unk4;
    u8 unk5;
    u8 unk6;
    u8 pad7;
} Entry151E7F60;

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s8 unk4;
    s8 unk5;
    s8 unk6;
    s8 unk7;
    s8 unk8;
    s8 unk9;
} Entry151E2834;

extern Entry151E7F60 D_800AB57C[];
extern s16 D_800AB77C[];
extern s16 D_800AB790[];
extern s8 D_8008FD9C;
extern s32 D_8008FD78;
extern s8 D_8008FD7C;
extern s8 D_8008FD88;
extern s8 D_8008FF37[];
extern s8 D_800E0AB0[];
extern s8 D_800E0AC0[];
extern s8 D_800E0AD0[];
extern u8 D_800E0AA8;
extern s8 D_800E0AA9;
extern s8 D_800E0AAA;
extern s16 D_800E0AA0[];
extern u8 D_800E0B8C;
extern s8 D_800E0B90[];
extern s8 D_800E0BB8[];
extern s8 D_800E0BD0;
extern s16 D_800E0C10[];
extern s32 D_800E0C18[];
extern s8 D_800D2130;
extern s8 D_80087268;
extern u8 D_80084060[];
extern s8 D_8008FDC8;
extern s8 D_800AB68C[];
extern u16 D_8008FDC0;
void func_151E2834(void);
void func_151E30C4(void);
void func_151E327C(void);
void func_151E4264(void);
void func_151E4314(void);

extern s8 D_8008FE2C;
extern u16 D_8008FDC0;
extern s8 D_800AB68C[];
extern struct Struct151E4314 D_800AB7BC[];
extern s8 D_800E0A86;
extern s8 D_800E0AC0[];
extern s8 D_800E0AC1;
extern s8 D_800E0AC2;
extern s8 D_800E0AC3;
void func_151E4BD8(void);

extern u8 D_800E0B94;
void func_151E4DC4(void);

extern s16 D_800E0B9A;
void func_151E4DD8(void);
void func_151E4E00(void);
void func_151E4E64(void);

extern f32 D_8008FDD0;
extern f32 D_800ABA2C;
extern f32 D_800ABA30;
extern f32 D_800ABA34;
extern f32 D_800DDDC8;
extern f32 D_800DDDD8;
void func_151E4EE8(void);

extern struct Struct151E5034 *D_8008FDD4;
void func_151E5034(void);
void func_151E51EC(void);
void func_151E530C(void);
void func_151E53E8(s32 arg0);

extern u8 D_80084060[];
void func_151E557C(void);
void func_151E55A8(void);

extern u8 D_800E0A8C;
void func_151E562C(void);

extern s8 D_8008FDC8;
s8 func_151E564C(void);

extern u8 D_800BE616;
extern u8 D_800E0B94;
extern s8 D_800E0C00[];
s32 func_151E5F64(s32 arg0);
s8 func_151E5FAC(void);

typedef struct {
    /* 0x00 */ char pad0[0x46];
    /* 0x46 */ s16 unk46[4];
    /* 0x4E */ char pad4E[0x18];
    /* 0x66 */ struct {
        /* 0x00 */ s16 unk0;
        /* 0x02 */ s16 unk2;
        /* 0x04 */ s16 unk4;
        /* 0x06 */ s16 unk6;
        /* 0x08 */ s16 unk8;
        /* 0x0A */ s16 unkA;
    } rec[4];
} ScoreState151E5FF4;

s32 func_1000F568(s32 arg0, s32 arg1);
void func_10010558(u16 arg0, struct127 *arg1, s32 arg2, s16 arg3, u16 arg4, s32 arg5);

#define SCORE ((ScoreState151E5FF4 *)D_8008FDD4)
void func_151E5FF4(struct127 *arg0, struct127 *arg1, s32 arg2);

extern s8 D_800E0BEA;
extern s8 D_800E0BD3;
extern s32 D_800E0BD4;
extern s32 D_8008FDD8;
extern u8 **D_800E0B88;
extern void func_10004074(void *);
extern void *allocate_memory(s32, s32, s32, s32);
s32 func_1502B5C8();
void func_151E6964(s32 arg0);
void func_151E6BFC(void);

struct Struct151149AC {
    char pad0[0x80];
    s32 unk80;
};
extern struct Struct151E5034 *D_8008FDD4;
extern u8 D_8008FFB0[2][6];
extern f32 D_800ABA8C;
struct Struct151149AC *func_151149AC(s32);
void func_151E7DC0(void);

extern s8 D_800E0BE9;
void func_151E7E9C(void);

extern void func_151DDC20(void);
void func_151E7EF8(void);

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 padE[0x22];
} Spawn151E7F60;


typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 unk6;
    u8 pad7;
} Pos151E7F60;

extern struct127 *D_800E0BA0[];
extern Entry151E7F60 D_800AB57C[];
extern Pos151E7F60 D_800AB940[];
extern s32 func_15082A44(void *, s32, s32, s32, s32);
extern void func_15083384(struct127 *, u8);
void func_151E7F60(s32 arg0, s32 arg1);
void func_151E8214(void);
void func_151E82B8(void);
void func_151E83E8(void);
