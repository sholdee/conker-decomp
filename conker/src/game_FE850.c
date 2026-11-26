#include <ultra64.h>
#include "functions.h"
#include "variables.h"



extern f32 D_800A08B0;
extern f32 D_800A08B4;
extern f32 D_800A08B8;

struct Obj150D13A0 {
    u8 pad0[0x3C];
    f32 unk3C;
    u8 pad40[0xB8 - 0x40];
    f32 unkB8;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    u8 padC8[0x148 - 0xC8];
    f32 unk148;
};

void func_150D13A0(Actor *arg0) {
    struct Obj150D13A0 *temp = (struct Obj150D13A0 *)arg0;

    temp->unkB8 = temp->unkB8 + temp->unkBC;
    temp->unkBC = temp->unkBC * D_800A08B0;
    temp->unk3C = temp->unk3C * D_800A08B4;
    temp->unkC4 = temp->unkC4 + temp->unk148;
    temp->unk148 = temp->unk148 * D_800A08B8;
    func_15059C84(arg0);
}

void func_150D1410(Actor *arg0) {
    WorldObject *temp;

    temp = func_151149AC(0xF9);
    if (temp != 0) {
        if ((arg0 - movement_obj) == 0) {
            temp->inactive = 1;
        } else {
            temp->inactive = 0;
        }
    }
}

void func_150D146C(Actor *arg0) {
    WorldObject *temp;

    temp = func_151149AC(0xF9);
    if (temp != 0) {
        temp->inactive = 1;
    }
}

extern f32 D_800A08C0;

struct Obj150D149C {
    TimedEffect base;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
};

void func_150D149C(Obj150D149C *arg0) {
    f32 *p = &arg0->unk28;

    func_151467A4(&arg0->unk30, 10.0f, &arg0->unk2C, 50.0f, 100.0f, 123.0f, D_800A08C0, p);
    func_1515D4D4((s32)*p, D_800DCD20[1], D_800DCD20[2], 0);
}
