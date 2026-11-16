#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_61D10/func_15034860.s")

extern s16 D_800C3EF0;
extern f32 D_80097D60;

struct Entry15034EB4 {
    u8 pad[0x34];
    f32 unk34;
    u8 pad2[0x8];
};

struct Obj15034EB4 {
    u8 pad[0x14C];
    f32 unk14C;
    u8 pad2[0x84];
    struct Entry15034EB4 *unk1D4;
};

void func_15034EB4(struct Obj15034EB4 *obj, s32 idx, s32 idx2) {
    f32 amt;
    struct Entry15034EB4 *e;
    struct Entry15034EB4 *e2;
    struct Entry15034EB4 *base;

    if (D_800C3EF0 != 0) {
        base = obj->unk1D4;
        amt = ((f32)D_800C3EF0 * D_80097D60) * obj->unk14C;
        e = &base[idx];
        e->unk34 -= amt;
        if (idx2 != -1) {
            e2 = &base[idx2];
            e2->unk34 -= amt;
        }
    }
}

extern u8 D_800C3F00;
void func_15034F20(void);

void func_150A7A48(void *, void *, void *);
void guMtxF2L2(void *, void *);
void func_15034728(void *);
s32 func_150AD960(s32, s32, s32, s32);
void func_15034F30(u8 *arg0, u8 *arg1, s32 arg2, f32 (*arg3)[4][4], f32 (*arg4)[4][4], s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, f32 arg11, f32 arg12, f32 arg13, f32 arg14, f32 arg15, f32 arg16, s32 arg17, s32 arg18);

struct Entry150356C8 {
    u32 unk0;
    u8 unk4;
    u8 pad5[4];
    u8 unk9;
    u8 unkA;
    u8 unkB;
};

extern struct Entry150356C8 D_800C3F08[];
struct Entry150356C8 *func_150356C8(void);


s32 func_150A6360(void *, void *, f32, f32, f32, f32, f32, f32);
extern f32 D_80097D70;
extern u8 D_800D9C10;
s32 func_15035714(s32 arg0, u8 *arg1, u8 *arg2, f32 arg3);

extern u8 D_80082FC0[];
extern u8 D_80083140[];

struct Obj15035D6C {
    u8 unk0;
    u8 pad1[2];
    u8 unk3;
    u8 pad4[2];
    u8 unk6;
    u8 pad7[0xC];
    u8 unk13;
    u8 unk14;
    u8 pad15[0xF];
    Gfx **unk24;
};
Gfx *func_15035D6C(Gfx *gfx, struct Obj15035D6C *obj, s32 red, s32 green, s32 blue);

extern Gfx *func_1502CCFC(Gfx *, s32, s32, u32, s32, s32 *, s32, s32);
Gfx *func_15035FE8(Gfx *gfx, s32 arg1);

void func_1507C3E0(struct127 *arg0, s16 *arg1, s16 *arg2, s16 *arg3);
void func_15035808(s32, s32, f32, f32, f32, f32, f32, f32, f32, f32);
void func_15036148(void);
