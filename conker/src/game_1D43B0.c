#include <ultra64.h>
/* func_150ADA20 returns a full 32-bit value; the shared header narrows it to u8 */
#define func_150ADA20 func_150ADA20_u8
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_1D43B0/func_151A6F00.s")

struct S151A73EC {
    s32 unk0;
    void *unk4;
};
s32 func_151A73EC(void *arg0) {
    struct S151A73EC *p = (struct S151A73EC *)((u8 *)arg0 + 0x170);
    if (*(s16 *)((u8 *)arg0 + 0x64) < 0x20) {
        if (p->unk4 != 0) {
            func_1516972C((struct102 *)p->unk4);
            p->unk4 = 0;
        }
    }
    return 1;
}

typedef struct {
    f32 field_0;
    void *field_4;
    u8 spawn;
    u8 alternate;
    s16 first;
    s16 last;
} Game1D4140Entry;

typedef struct {
    u8 pad_0[0x10];
    f32 base_height;
    f32 bounce;
    f32 scale;
    u8 pad1C[0x20];
    f32 height;
    u8 pad40[4];
    f32 velocity_x;
    f32 velocity_y;
    f32 velocity_z;
    f32 rotation_x;
    f32 rotation_y;
    f32 rotation_z;
    u8 pad5C[4];
    s32 flags;
    s16 field_64;
    u8 pad_66[0x10A];
    Game1D4140Entry field_170;
} Game1D4140State;

void func_1514AB5C(f32, f32, f32, f32, s32, f32, f32, f32, f32, s32, s32, s32);

s32 func_151A743C(Game1D4140State *arg0, f32 arg1, s32 arg2, f32 arg3, f32 arg4) {
    Game1D4140Entry *state;
    u8 alternate;

    state = &arg0->field_170;
    arg0->height = arg0->base_height + arg4;
    if (arg0->field_170.field_4 != 0) {
        func_1516972C(state->field_4);
        state->field_4 = 0;
    }
    if (state->field_0 < arg0->velocity_y) {
        arg0->flags &= ~7;
        arg0->flags &= ~8;
        arg0->flags &= ~0x40;
        arg0->flags &= ~0x20;
        arg0->velocity_x = 0.0f;
        arg0->velocity_y = 0.0f;
        arg0->velocity_z = 0.0f;
        if (arg0->field_64 >= 0x21) {
            arg0->field_64 = 0x20;
        }
        if (state->spawn != 0) {
            alternate = 0;
            if (state->alternate != 0) {
                alternate = 1;
            }
            func_1514AB5C(arg1, arg4, arg3, arg0->scale * 35.0f,
                         func_150ADA20() % 3U + 2,
                         arg0->scale, arg0->scale * 0.5f,
                         arg0->scale, arg0->scale * 0.5f,
                         state->first, state->last, alternate);
        }
    } else {
        arg0->velocity_x *= arg0->bounce;
        arg0->velocity_y = -arg0->velocity_y * arg0->bounce;
        arg0->velocity_z *= arg0->bounce;
        arg0->rotation_x *= arg0->bounce;
        arg0->rotation_y *= arg0->bounce;
        arg0->rotation_z *= arg0->bounce;
    }
    return 1;
}

f32 func_150ADA68(void);
extern f32 D_800BE9A8;
extern f32 D_800A8DC8;
extern f32 D_800A8DCC;
extern f32 D_800A8DD0;
void func_151A26EC(f32 *arg0, f32 *arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, s32 argE, s32 argF, s32 arg10);
void func_151A7610(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, u8 *arg6)
{
  f32 sp8C[3];
  f32 sp80[3];
  f32 sp74[3];
  f32 r;
  f32 sp6C;
  struct 
  {
    f32 a;
    f32 b;
    u32 c;
    u32 pad0;
    u32 pad1;
  } sp58;
  f32 dt;
  sp6C = *((f32 *) (arg6 + 0x40));
  sp8C[0] = arg0;
  sp8C[1] = arg1;
  sp8C[2] = arg2;
 do { } while (0);
  if (dt && dt)
  {
  }
  r = func_150ADA68();
  dt = D_800BE9A8;
  r = (r * 166.0f) + 160.0f;
  sp74[0] = 0.0f;
  sp74[1] = 0.0f;
  sp74[2] = 0.0f;
  r *= D_800A8DC8;
  sp80[0] = ((-arg3) * dt) * r;
  sp80[1] = ((-arg4) * dt) * r;
  sp80[2] = ((-arg5) * dt) * r;
  sp58.a = func_150ADA68();
  sp58.b = func_150ADA68();
  sp58.c = func_150ADA20();
  func_151A26EC(sp8C, sp74, sp80, 1.0f, ((sp58.a * D_800A8DCC) + (-600.0f)) * D_800A8DD0, ((sp58.b * 500.0f) + 400.0f) * sp6C, (sp58.c % 21U) + 15, (func_150ADA20() % 101U) + 100, 15, 20, 0, -1, 86, 39, 0, arg6[0xC], arg6[1]);
}


typedef struct Obj151A77C0 {
    char pad0[0x14];
    /* 0x14 */ s32 unk14;
    char pad18[0x2F - 0x18];
    /* 0x2F */ u8 unk2F;
    char pad30[0x34 - 0x30];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    char pad3A[0x3F - 0x3A];
    /* 0x3F */ u8 unk3F;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 unk42;
    char pad43;
    /* 0x44 */ u16 unk44;
    char pad46[0x50 - 0x46];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ s16 unk5A;
} Obj151A77C0;

typedef struct Sub151A77C0 {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
} Sub151A77C0;

void func_151A77C0(Obj151A77C0 *arg0) {
    Sub151A77C0 *s;

    s = (Sub151A77C0 *)&arg0->unk50;
    if (arg0->unk38 < arg0->unk54) {
        arg0->unk3F = arg0->unk38 * arg0->unk56;
    }
    if (arg0->unk38 < s->unk8) {
        s16 d = s->unkA * D_800BE9E4;
        arg0->unk34 += d;
        arg0->unk36 += d;
    }
    if (arg0->unk38 < s->unk0) {
        arg0->unk2F = 0x15;
        arg0->unk44 |= 0x201;
        arg0->unk40 = arg0->unk41 = arg0->unk42 = arg0->unk38 * s->unk2;
        arg0->unk14 = 0xC000F;
    }
}


typedef struct Obj151A787C {
    char pad0[0x34];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    char pad3A[0x3F - 0x3A];
    /* 0x3F */ u8 unk3F;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 unk42;
    char pad43[0x50 - 0x43];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ s16 unk5A;
} Obj151A787C;

typedef struct Sub151A787C {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
} Sub151A787C;

void func_151A787C(Obj151A787C *arg0) {
    Sub151A787C *s;

    s = (Sub151A787C *)&arg0->unk50;
    if (arg0->unk38 < arg0->unk54) {
        arg0->unk3F = arg0->unk38 * arg0->unk56;
    }
    if (arg0->unk38 < s->unk8) {
        s16 d = s->unkA * D_800BE9E4;
        arg0->unk34 += d;
        arg0->unk36 += d;
    }
    arg0->unk40 = arg0->unk41 = arg0->unk42 = arg0->unk38 * s->unk2;
}

struct S151A7908 {
    s32 unk0;
    void *unk4;
};
void func_151A7908(void *arg0) {
    struct S151A7908 *p = (struct S151A7908 *)((u8 *)arg0 + 0x170);
    if (p->unk4 != 0) {
        func_1516972C((struct102 *)p->unk4);
        p->unk4 = 0;
    }
}
