#include <ultra64.h>
#include "functions.h"
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

void func_151DDBA0(void) {
    D_800D2E40 = 0;
    func_1501C730(6, 0x1D, 0, 0, 1);
    D_800E0B94 = 3;
    D_8008FDA4 = 0;
    D_800BEAC1 = 0;
    func_151E557C();
    func_1000F1A8();
    func_1000E934();
    D_8008FD8C = 1;
    D_8008FD90 = 1;
}
void func_151DE6D4(void);

extern s32 D_800E0A90;
extern u8 D_800E0B97;
extern u8 D_800E0B98;
extern u8 D_800E0A8C;
extern u8 D_8008FE28;
void func_151DE85C(void);
void func_151DE7D4(void);

extern s8 D_8008FE30;
extern s8 D_8008FD74;
extern s8 D_800E0B96;
void func_151DE81C(void);

extern u8 D_800D2E40;
extern u8 D_800E0B94;
extern u8 D_8008FD80;
extern u8 D_8008FDA4;
extern struct Struct151E5034 *D_8008FDD4;
void func_1501C730(s32, s32, s32, s32, s32);
void func_151DE85C(void);
void func_151DE8E8(void);

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
