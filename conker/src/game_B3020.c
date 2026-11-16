#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s8 D_800D239A;
extern s8 D_800D2398;
extern s8 D_800D2399;
extern s8 *D_800D23B0;
s32 func_1508802C();

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
} B3020Struct;

extern s16 D_80087290;
extern s16 D_80087294;
extern B3020Struct *D_800D2350;
s16 *func_1502B5C8(s32 *, s32, s32, s32);
s32 func_15085BE8(void);

void func_15085B70(s32 arg0) {
    s16 *temp_v0;

    temp_v0 = func_1502B5C8(0, 2, 0x19, arg0);
    if (temp_v0 == 0) {
        D_80087290 = 0;
        D_80087294 = 0;
        D_800D2350 = 0;
    } else {
        D_80087290 = temp_v0[0];
        D_80087294 = temp_v0[1];
        D_800D2350 = (B3020Struct *)(temp_v0 + 2);
    }
    func_15085BE8();
}

extern u8 D_800D2358;
extern f32 D_800D2360[];
extern u8 D_800D237C[];

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
} B3020Struct2;

s32 func_15085BE8(void) {
    s32 i;
    s32 ret;
    s32 idx;
    f32 val;
    s32 a;
    s32 b;
    f32 min[6];
    f32 max[6];

    ret = 0;
    for (i = 0; i < 6; i++) {
        min[i] = 10000.0f;
        max[i] = -10000.0f;
    }
    for (i = 0; i < D_80087290; i++) {
        idx = ((B3020Struct2 *)D_800D2350)[i].unk6;
        val = ((B3020Struct2 *)D_800D2350)[i].unk2;
        if (val < min[idx]) {
            min[idx] = val;
        }
        if (max[idx] < val) {
            max[idx] = val;
        }
    }
    D_800D2358 = 0;
    for (i = 0; i < 6; i++) {
        if (min[i] <= max[i]) {
            D_800D237C[D_800D2358] = i;
            D_800D2358++;
        }
    }
    for (i = 0; i < D_800D2358 - 1; i++) {
        a = D_800D237C[i];
        b = D_800D237C[i + 1];
        D_800D2360[i] = (max[a] + min[b]) * 0.5f;
        if (max[a] > min[b]) {
            ret = 1;
        }
    }
    D_800D2360[i] = 100000.0f;
    return ret;
}

extern f32 D_800D2360[];
extern u8 D_800D237C[];
extern f32 D_8009D9CC;
extern u8 D_8008729C;
extern s32 D_800D2354;
extern f32 sqrtf(f32);
f32 func_15086D94(f32, f32, f32, f32, f32);

u8 func_15085DA8(f32 arg0) {
    s32 i = 0;

    while (D_800D2360[i] <= arg0) {
        i++;
    }
    return D_800D237C[i];
}

s32 func_15085DF8(f32 arg0, f32 arg1, f32 arg2, s8 arg3, s8 arg4) {
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 temp_f2;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f20;
    f32 bestDist;
    s32 i;
    s32 useGate;
    s32 bestIdx;
    B3020Struct *entry;

    useGate = 0;
    bestDist = D_8009D9CC;
    if (arg3 == 0) {
        useGate = 1;
    }

    if (D_8008729C != 0xFF) {
        entry = &D_800D2350[D_8008729C];
        temp_f2 = (f32) entry->unk0 - arg0;
        temp_f16 = (f32) entry->unk2 - arg1;
        temp_f18 = (f32) entry->unk4 - arg2;
        if ((useGate == 0) || ((useGate != 0) && (sp70 = temp_f2, sp6C = temp_f16, sp68 = temp_f18, func_15086D94(arg0, arg1, arg2, temp_f2, temp_f18) < 0.0f))) {
            bestDist = (temp_f2 * temp_f2) + (temp_f16 * temp_f16) + (temp_f18 * temp_f18) + 10.0f;
        }
        D_8008729C = 0xFF;
    }

    bestIdx = 0xFF;
    i = 0;
    if (D_80087290 > 0) {
        do {
            entry = &D_800D2350[i];
            if (((arg4 == ((u8 *)entry)[6]) || (arg4 == -1)) && ((arg3 == ((u8 *)entry)[0xE]) || (arg3 == -1))) {
                temp_f2 = (f32) entry->unk0 - arg0;
                temp_f16 = (f32) entry->unk2 - arg1;
                temp_f18 = (f32) entry->unk4 - arg2;
                temp_f20 = (temp_f2 * temp_f2) + (temp_f16 * temp_f16) + (temp_f18 * temp_f18);
                if ((temp_f20 < bestDist) && ((useGate == 0) || ((useGate != 0) && (func_15086D94(arg0, arg1, arg2, temp_f2, temp_f18) < 0.0f)))) {
                    bestDist = temp_f20;
                    bestIdx = i;
                }
            }
            i++;
        } while (i < D_80087290);
    }

    D_800D2354 = (s32) sqrtf(bestDist);
    return bestIdx;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_B3020/func_15086098.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_B3020/func_15086364.s")

#pragma intrinsic(sqrtf)
f32 func_15086BD0(s32 arg0, s32 arg1);
void func_15086C68(void);

extern void func_150A3194(s32, s32, s32, s32, s32);

extern B3020Struct *D_800D2350;
void func_15086C70(s32 arg0);

extern s16 D_80087290;
s32 func_15086CBC(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3);
s32 func_15086D48(s32 arg0);

extern u8 *D_800872A0;
extern s8 D_800D23A8;
extern s32 D_800D2394;
extern void *allocate_memory(s32, s32, s32, s32);
u8 func_150888A8(u8, u8, s32);
void func_15088824();

typedef struct {
    u8 pad0[9];
    u8 unk9[5];
    u8 unkE;
    u8 unkF;
} B3020Bytes;
void func_150870D0(s32 arg0, s32 arg1, s32 arg2);

extern u8 *D_800872A0;
void func_150891E8(s8 *, struct127 *);
void func_150896EC();
void func_15087CC0(void);

extern u8 *D_800872A0;
u8 func_150888A8(u8, u8, s32);

struct Struct15087DCC {
    char pad0[0x2B];
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    s8 unk2F;
    char pad30[0x84 - 0x30];
};
void func_15087DCC(s32 arg0, s32 arg1);

struct Struct15087E54 {
    char pad0[0xC];
    f32 unkC;
    f32 unk10;
    char pad14[0x84 - 0x14];
};

struct Struct15087E54arg1 {
    char pad0[0x76];
    u16 unk76;
};
void func_15087E54(s32 arg0, struct Struct15087E54arg1 *arg1);

extern s8 D_800D23A8;
extern u8 *D_800872A0;

struct Struct15087EF0 {
    f32 unk0;
    char pad4[0xC - 4];
    f32 unkC;
    f32 unk10;
    char pad14[0x84 - 0x14];
};

struct Struct15087EF0arg1 {
    char pad0[0x76];
    s16 unk76;
};
void func_15087EF0(s32 arg0, struct Struct15087EF0arg1 *arg1);

struct Struct15087FC4 {
    char pad0[0x31];
    u8 unk31;
    char pad32[0x84 - 0x32];
};
void func_15087FC4(s32 arg0, s32 arg1);

extern u8 *D_800872A0;

struct Struct15087FEC {
    char pad0[4];
    f32 unk4;
    char pad8[0x84 - 8];
};
void func_15087FEC(s32 arg0, s32 arg1);
s32 func_1508802C(s8 *arg0, struct127 *arg1, s32 arg2);
s32 func_150880F8(s32 arg0, s32 arg1);

struct Struct150881CC {
    f32 unk0;
    char pad4[0x84 - 4];
};
s32 func_150881CC(s32 arg0);

struct Struct15088218 {
    char pad0[8];
    f32 unk8;
    char padC[0x24 - 0xC];
    s16 unk24;
    char pad26[0x84 - 0x26];
};
s32 func_15088218(s32 arg0);

struct Struct15088270 {
    char pad0[0x14];
    f32 unk14;
    char pad18[0x84 - 0x18];
};
s32 func_15088270(s32 arg0);

struct Struct150882B0 {
    char pad0[0x27];
    s8 unk27;
    char pad28[0x84 - 0x28];
};
s32 func_150882B0(s32 arg0);

struct Struct150882E4 {
    char pad0[8];
    f32 unk8;
    char padC[0x24 - 0xC];
    s16 unk24;
    char pad26[3];
    s8 unk29;
    char pad2A[0x84 - 0x2A];
};
s32 func_150882E4(s32 arg0, s32 arg1);

struct Struct1508855C {
    char pad0[0x31];
    s8 unk31;
    char pad32[0x84 - 0x32];
};
s32 func_1508855C(struct127 *arg0);

void bcopy(const void *, void *, int);
#define D_800872A0_S32 ((s32)D_800872A0)
void func_150885EC(s32 arg0, s32 arg1);
#undef D_800872A0_S32

extern s32 D_800D2394;
extern s8 D_800D2398;
extern s8 D_800D2399;
typedef struct Struct15088824 Struct15088824;
void func_15088824();
s32 func_1508868C(s32 arg0);
void func_15088780(void);

extern u8 *D_800872A0;
s32 func_150887F8(void);

struct Struct15088824 {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8 unk26;
    /* 0x27 */ u8 unk27;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 pad34[0x15];
    /* 0x49 */ u8 unk49;
};
void func_15088824(Struct15088824 *arg0);

struct Struct15088A08 {
    char pad0[0x8];
    f32 unk8;
    char padC[0xC];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s16 unk24;
    s8 unk26;
    s8 unk27;
    s8 unk28;
    char pad29[2];
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    s8 unk2F;
    s8 unk30;
    s8 unk31;
};

extern u8 D_800D2390;
extern s8 D_800E0BE7;
extern u8 D_800BE616;
extern s8 D_8008FD90;
extern s16 D_800E0C10[];
extern s32 D_800E0C18[];
s32 func_15088A08(struct Struct15088A08 *arg0, f32 arg1);

typedef struct Entry15088D58 {
    f32 value0;
    f32 value4;
    f32 phase8;
    f32 directionC;
    f32 direction10;
    u8 pad14[0x18];
    u8 node2C;
    u8 pad2D[2];
    s8 group2F;
    u8 pad30;
    s8 actor31;
    u8 pad32[0x52];
} Entry15088D58;

typedef struct Actor15088D58 {
    u8 pad0[0x14];
    f32 field14;
    f32 field18;
    f32 field1C;
    u8 pad20[0x20];
    f32 angle40;
    u8 pad44[0x32];
    s16 rotation76;
    s16 rotation78;
    s16 rotation7A;
    u8 pad7C[0xA8];
    u8 field124;
} Actor15088D58;

void func_15088F30();
f32 func_150497E0(f32 *, s32, f32);
f32 func_150498A4(f32 *, s32, f32, f32 *);
f32 func_15144BC8(f32);
extern f32 D_8009DA04;
void func_15088D58(Actor15088D58 *arg0);

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[3];
    u8 links[5];
    u8 type;
    u8 padF;
} Node15088F30;

typedef struct {
    f32 t;
    u8 pad4[0x27];
    u8 ids[4];
} Obj15088F30;
void func_15088F30(Obj15088F30 *arg0, f32 *outX, f32 *outY, f32 *outZ);
u8 func_1508907C(u8 node, u8 type, u8 exclude, u8 sub);


typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    f32 unkC;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
} CamParams150896EC;

typedef struct {
    u8 pad0[0x34];
    f32 t;
    f32 x;
    f32 y;
    f32 z;
    u8 nodes[4];
    u8 pad48;
    s8 state;
    u8 pad4A[2];
    CamParams150896EC cam;
} Obj150896EC;

#define obj ((Obj150891E8 *)arg0)
typedef struct {
    u8 pad0[0x30];
    s8 state;
    u8 pad31[0x19];
    u8 cur;
    u8 next;
    CamParams150896EC cam;
} Obj150891E8;

void func_1512D560();
void func_150891E8(s8 *arg0, struct127 *arg1);
#undef obj


void func_1512D560();
extern s32 D_800BE9E4;
void func_150896EC(Obj150896EC *arg0, struct127 *arg1, s32 arg2);
void func_15089BB0(void);

void *func_1509B570(s32);
void func_1508BC20(void);
extern f32 D_800872B0[], D_800872F8[];
extern s8 D_800872F0[], D_800E0BD0;
extern s32 D_800D2E48;
s32 func_15089BC0(void);
s32 func_15089F9C(s32 arg0);

extern s8 D_800E0BD0;
extern u8 D_800E0BB8[];
extern s8 D_800872D0[];
extern s16 D_800872C4[];
extern s8 D_800872E0[];
extern s8 D_800872E8[];
extern f32 D_800872F8[];
void func_15042D94(s32, s32, s32, char *, ...);
s32 func_1508CAD8(s32, s32, s32, s32, s32, s32);
void func_1508EC5C(s32, s32);
void func_1508EBF8(s32, s32);
s32 func_1508DAEC(s32, s32);
void func_1508B3F8(void);

#define MGR_W(off) ((s32 *)(D_800D23B0 + (off)))
#define MGR_B(off) ((s8 *)(D_800D23B0 + (off)))
#define MGR_F(off) ((f32 *)(D_800D23B0 + (off)))
s32 func_1508A6FC(void);

extern s8 D_8008FD90;
extern s16 *D_8008FDD4;
s32 func_1508B194(s32 arg0);
void func_1508B1D4(s32 arg0);

typedef struct {
    f32 radiusSq;
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} Entry1508B20C;

typedef struct {
    u8 pad0[0x1745];
    s8 count;
    u8 pad1746[2];
    Entry1508B20C entries[8];
} Mgr1508B20C;

#define MGR ((Mgr1508B20C *)D_800D23B0)
void func_1508B20C(f32 arg0, f32 arg1, f32 arg2, f32 arg3);

#undef MGR

typedef struct {
    f32 x;
    f32 z;
    f32 radiusSq;
    f32 dist[8];
    s16 count;
    u8 ids[8];
    u8 visited[1];
} Search1508B2A8;
void func_1508B2A8(u8 id, Search1508B2A8 *s);


typedef struct {
    f32 radiusSq;
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} Entry1508B3F8;

typedef struct {
    f32 x;
    f32 z;
    f32 radiusSq;
    f32 dist[8];
    s16 count;
    u8 ids[8];
    u8 visited[0x20];
} Search1508B3F8;
void func_1508B3F8(void);

s32 func_1508C5B8(s32, s32);
void func_1508A1BC(void);

typedef struct B9BCActor {
    u8 pad0[0x14];
    f32 field14;
    f32 field18;
    f32 field1C;
} B9BCActor;
s32 func_1508B9BC(void);
s32 func_1508BF14(void);
s32 func_1508C194(s32 arg0);

typedef struct GameB4080SightInner {
    u8 pad0[0x75];
    u8 kind75;
    u8 pad76[0xE];
    u8 flag84;
    u8 pad85[0x8F];
    s16 height114;
} GameB4080SightInner;

typedef struct GameB4080SightActor {
    s32 type;
    u8 pad4[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0x184];
    s16 pointX;
    s16 pointY;
    s16 pointZ;
    u8 pad1AA[0x172];
    GameB4080SightInner *inner;
} GameB4080SightActor;

void *func_1505EEF4(s32);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *,
                  f32 *, f32 *, f32 *, f32 *, s32 *, void *, f32);
extern s16 D_800872C4[];
extern s8 D_800E0BD0;
void func_1508C1A4(s32 arg0, s32 arg1);
s32 func_1508C5B8(s32 arg0, s32 arg1);
s32 func_1508C9CC(void);
s32 func_1508CA88(void);

extern struct127 D_800CC2D0[26];
extern s8 *D_800D23B0;
s32 func_1508DAEC(s32 arg0, s32 arg1);
void func_1508E6C8(void);
s32 func_1508E6D0(s32 arg0);
s32 func_1508E780(s32 arg0);

extern struct127 D_800CC40F[];
void func_1508EB90(s32 arg0, s32 arg1, s32 arg2);
void func_1508EBF8(s32 arg0, s32 arg1);
void func_1508EC5C(s32 arg0, s32 arg1);

typedef struct {
    u16 unk0;
    u16 count;
    s32 unk4;
    u16 entries[8];
} GameB4080Group;
s32 func_1508ECC0(u16 arg0, u16 arg1, u16 arg2);

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8[4];
} Struct1508EDBC;
void func_1508EDBC(s32 arg0);

void func_1503DDD0(s32);
void func_15114B94(u32);
void func_1508EE0C(u16 arg0, u16 arg1);
