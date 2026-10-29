#include <ultra64.h>
/* func_150ADA20 returns a full 32-bit value; the shared header narrows it to u8 */
#define func_150ADA20 func_150ADA20_u8
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec151A6F00;

typedef struct {
    /* 0x00 */ s16 count;
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ s16 unk10;
    /* 0x14 */ f32 speed;
    /* 0x18 */ s16 yawBase;
    /* 0x1A */ s16 yawRange;
    /* 0x1C */ s16 pitchBase;
    /* 0x1E */ s16 pitchRange;
    /* 0x20 */ f32 velBase;
    /* 0x24 */ f32 velRange;
    /* 0x28 */ f32 velYBase;
    /* 0x2C */ f32 velYRange;
    /* 0x30 */ Vec151A6F00 vel;
    /* 0x3C */ f32 scaleBase;
    /* 0x40 */ f32 scaleRange;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
} Params151A6F00;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ Vec151A6F00 vel;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    s32 pad78;
} Particle151A6F00;

typedef struct {
    f32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    s16 unkA;
    s16 unkC;
} Extra151A6F00;

typedef struct {
    f32 *unk0;
    f32 *unk4;
    f32 *unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    u8 unk16;
    u8 unk17;
} Spawn151A6F00;

typedef struct {
    s32 unk0;
    void *unk4;
} Link151A6F00;

extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void *func_15134908(void *, s32, u8, s32);

void func_151A6F00(Params151A6F00 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    s16 i;
    Particle151A6F00 sp;
    s32 pad;
    Extra151A6F00 extra;
    f32 scale;
    u8 yaw;
    u8 pitch;
    f32 sinYaw;
    f32 cosYaw;
    f32 sinPitch;
    f32 cosPitch;
    f32 dist;
    u8 *v;

    i = arg0->count;
    sp.unk1C = 1.0f;
    sp.unk20 = 1.0f;
    sp.unk24 = 1.0f;
    sp.unk50 = 0x21E9;
    sp.unk58 = 0;
    sp.unk54 = arg0->unk10;
    sp.unk56 = 0x1B;
    sp.unk10 = 0.0f;
    sp.unk14 = 0.0f;
    sp.unk18 = 0.0f;
    sp.unk44 = 0.0f;
    sp.unk04 = 0.200000003f;
    extra.unk0 = -18.0f;
    extra.unk8 = arg0->unk54;
    extra.unk9 = arg0->unk55;
    extra.unkA = arg0->unk56;
    extra.unkC = arg0->unk58;
    sp.unk5C = 0;
    sp.unk60 = 0xFF;
    sp.unk61 = 1;
    sp.unk62 = 5;
    sp.unk63 = 4;
    sp.unk64 = 0;
    sp.unk65 = 0;
    sp.unk66 = 1;
    sp.unk67 = 0;
    sp.unk68 = 0;
    sp.unk6A = 0;
    sp.unk6C = 0;
    sp.unk70 = 0;
    sp.unk72 = 0x20;
    sp.unk74 = 7;
    while (i--) {
        yaw = (func_150ADA20() % (arg0->yawRange + 1)) + arg0->yawBase;
        pitch = (func_150ADA20() % (arg0->pitchRange + 1)) + arg0->pitchBase;
        sinYaw = func_151423D8((u8)(yaw - 0x40));
        cosYaw = func_151423D8(yaw);
        sinPitch = func_151423D8((u8)(pitch - 0x40));
        cosPitch = func_151423D8(pitch);
        dist = func_150ADA68() * arg0->speed;
        sp.unk28 = arg0->x + dist * cosPitch * sinYaw;
        sp.unk2C = arg0->y - dist * sinPitch;
        sp.unk30 = arg0->z + dist * cosPitch * cosYaw;
        switch (arg1) {
        case 0: {
            f32 speed;
            f32 vspeed;
            speed = (func_150ADA68() * arg0->velRange) + arg0->velBase;
            vspeed = (func_150ADA68() * arg0->velYRange) + arg0->velYBase;
            sp.vel.x = speed * sinYaw;
            sp.vel.y = vspeed;
            sp.vel.z = speed * cosYaw;
        }
            break;
        case 1: {
            f32 speed;
            speed = (func_150ADA68() * arg0->velRange) + arg0->velBase;
            sp.vel.x = speed * cosPitch * sinYaw;
            sp.vel.y = -speed * sinPitch;
            sp.vel.z = speed * cosPitch * cosYaw;
        }
            break;
        case 2:
            sp.vel = arg0->vel;
            break;
        case 3: {
            f32 speed;
            speed = (func_150ADA68() * arg0->velRange) + arg0->velBase;
            sp.vel.x = arg0->vel.x * speed;
            sp.vel.y = arg0->vel.y * speed;
            sp.vel.z = arg0->vel.z * speed;
            break;
        }
        }
        scale = sp.unk08 = sp.unk0C = (func_150ADA68() * arg0->scaleRange) + arg0->scaleBase;
        sp.unk00 = scale * 30.0f;
        sp.unk4C = (func_150ADA68() * arg0->unk48) + arg0->unk44;
        sp.unk40 = (func_150ADA68() * arg0->unk50) + arg0->unk4C;
        sp.unk48 = (func_150ADA68() * arg0->unk50) + arg0->unk4C;
        v = func_15132A4C(&sp, 0, 0, 0x10, arg2, arg3);
        if (v != NULL) {
            Spawn151A6F00 spawn;
            Link151A6F00 *link;
            void *ret;
            link = (Link151A6F00 *)(v + 0x170);
            memcpy(link, &extra, 0x10);
            spawn.unk0 = (f32 *)(v + 0x38);
            spawn.unk4 = (f32 *)(v + 0x3C);
            spawn.unk8 = (f32 *)(v + 0x40);
            spawn.unkC = 15.0f * sp.unk0C;
            spawn.unk14 = 0x12C;
            spawn.unk16 = 0;
            spawn.unk17 = 1;
            spawn.unk10 = 0.3580000103f;
            link->unk4 = NULL;
            ret = func_15134908(&spawn, 4, arg2, arg3);
            link->unk4 = ret;
            if (ret != NULL) {
                memcpy((u8 *)ret + 0x40, &scale, 4);
            }
        }
    }
}

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
