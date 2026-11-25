#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
#undef func_15048A40
#undef func_150ADA20
#include "variables.h"


/* Whatever an object's unk7C points to in func_150C7968. */
typedef struct {
    /* 0x00 */ u8 pad00[0x13];
    /* 0x13 */ s8 unk13;
} LinkedObj;

/* The 0xA0-byte level objects (D_800DBEF4 entries) driven by the callbacks in this file. */
typedef struct {
    /* 0x00 */ f32 angleX;
    /* 0x04 */ f32 angleY;
    /* 0x08 */ f32 angleZ;
    /* 0x0C */ u8  pad0C[0x4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8  pad16[0x6];
    /* 0x1C */ s32 *unk1C; // func_150C7C90: list of words, ended by one whose top byte is -14
    /* 0x20 */ u8  pad20[0x1C];
    /* 0x3C */ union {
        s32 word;
        struct {
            s16 hi; // spin speed for func_151150BC
            u16 lo;
        } half;
    } params;
    /* 0x40 */ u8  pad40[0x33];
    /* 0x73 */ u8  flags;
    /* 0x74 */ u8  pad74[0x8];
    /* 0x7C */ union {
        s32 count;        // func_150C7C90
        LinkedObj *link;  // func_150C7968
    } unk7C;
    /* 0x80 */ u8  pad80[0x20];
} F4D20Obj; // size 0xA0

#define sObjs ((F4D20Obj *)D_800DBEF4)

extern void func_1511650C(F4D20Obj *obj, s32 arg1, s32 soundId, f32 pitchScale);
extern struct131 *func_151149AC(u8);
extern void func_151150BC(F4D20Obj *obj);
extern s32 func_15116110(F4D20Obj *obj);

/* Runs func_1511650C with sound 0x353 (or 0x43 at a lower pitch once object 0's flag bit 2 is set), unless
 * bit 3 of byte 0xA of the 27-byte flag record at D_800D2E4C is set. */
void func_150C7870(F4D20Obj *obj) {
    if (!(((u8 *)D_800D2E4C)[0xA] & 8)) { // struct102 has no field at byte 0xA (inside unk8)
        if (!(sObjs[0].flags & 4)) {
            func_1511650C(obj, 1, 0x353, 1000.0f);
        } else {
            func_1511650C(obj, 1, 0x43, 400.0f);
        }
    }
}

/* Spins the object at the opposite of object 3's spin speed. */
void func_150C78E0(F4D20Obj *obj) {
    F4D20Obj *obj3;

    if (!(obj->flags & 4)) {
        obj3 = &sObjs[3];
        obj->params.word = -(obj3->params.word & 0xFFFF0000) & 0xFFFF0000;
        func_151150BC(obj);
    }
}

/* Spins the object at object 3's spin speed. */
void func_150C7930(F4D20Obj *obj) {
    F4D20Obj *obj3;

    obj3 = &sObjs[3];
    obj->params.word = obj3->params.word & 0xFFFF0000;
    func_151150BC(obj);
}

/* After func_15116110, copies object 3's spin speed / 16 into the linked object's unk13. */
void func_150C7968(F4D20Obj *obj) {
    F4D20Obj *obj3;
    LinkedObj *link;
    s32 speed;

    func_15116110(obj);
    if (!(obj->flags & 4)) {
        obj3 = &sObjs[3];
        link = obj->unk7C.link;
        speed = obj3->params.half.hi >> 4;
        if (link != NULL) {
            link->unk13 = speed;
        }
    }
}

extern f32 func_15048A40(u8 arg0);
extern u32 func_150ADA20(void);
extern void func_1000D96C(s32, s32, s32);
extern void func_1000DE1C(s32, s32);
extern f32 func_150489B0(u8);
extern void func_15179FE0(u8, s16, s32, s16, s32, s32, s32, s32, s32, s32, s32);
extern s32 D_800887F4;

void func_150C79BC(s32 arg0) {
    s32 y;
    s32 dy;
    s32 dx;
    s32 dz;
    s32 r;
    u8 angle;

    dx = (s32)D_800DBFF0[arg0].unk2F8 + 2418;
    dy = (s32)D_800DBFF0[arg0].unk2FC - 2500;
    dz = (s32)D_800DBFF0[arg0].unk300 - 1775;
    if ((dy >= -199) && (dx * dx + dz * dz < 1440000)) {
        if (D_800887F4 == 0) {
            func_1000D96C(0x5F, 0xF, 6);
            func_1000DE1C(0x10, 4);
            D_800887F4 = 1;
        }
    } else if (D_800887F4 != 0) {
        func_1000D96C(0xF, 0x5F, 4);
        func_1000D96C(0x10, 0, 3);
        D_800887F4 = 0;
    }
    dy = (s32)D_800DBFF0[arg0].unk2FC - 1583;
    if ((dy < 0) && (dx * dx + dz * dz < 810000) && ((func_150ADA20() & 0xFFFF) < 0x2000)) {
        r = func_150ADA20() % 900U;
        angle = func_150ADA20();
        dx = func_150489B0(angle) * r + -2418.0f;
        dz = func_15048A40(angle) * r + 1775.0f;
        r = (f32)((func_150ADA20() & 0xFFFF) * 2) * (1.0f / 65536.0f);
        func_15179FE0(r, dx, 1583, dz, 7, 1200, 37, 10, 30, 3, 8);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
} Entry150C7C90;

typedef struct {
    u8 pad00[0x1C];
    Entry150C7C90 *unk1C;
    u8 pad20[0x1C];
    s32 unk3C;
    u8 pad40[0x3C];
    s32 unk7C;
} GameObj150C7C90;

void func_150C7C90(GameObj150C7C90 *arg0) {
    Entry150C7C90 *entry;
    Entry150C7C90 *selected;
    s32 idx;
    s32 count;
    s32 angle;
    s32 word;
    s32 newWord;

    idx = arg0->unk7C;
    if (idx == 0) {
        selected = arg0->unk1C;
        count = 0;
        if (*(s8 *)&selected->unk0 != -0xE) {
            do {
                count++;
            } while (*(s8 *)&selected[count].unk0 != -0xE);
        }
        arg0->unk7C = count;
        idx = count;
    }
    selected = &arg0->unk1C[idx];
    angle = -0x29D - func_151149AC(arg0->unk3C & 0xFF)->unk12;
    if (arg0->unk3C & 0x8000) {
        angle = 0x344 - angle;
    }
    while (angle < 0) {
        angle += 0x400;
    }
    while (angle >= 0x400) {
        angle -= 0x400;
    }
    selected->unk0 &= ~0xFFF;
    selected->unk0 |= angle;
}

typedef struct {
    u8 pad00[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
} GameObj150C7D7C;

void func_150C7D7C(GameObj150C7D7C *arg0) {
    struct127 *temp_v0;

    temp_v0 = func_15083E90(0xC);
    arg0->unk10 = (s16)(s32)(temp_v0->x_position - 30.0f);
    arg0->unk12 = (s16)(s32)(temp_v0->y_position + 50.0f);
    arg0->unk14 = (s16)(s32)(temp_v0->z_position + 30.0f);
}
