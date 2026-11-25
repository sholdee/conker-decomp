#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


typedef struct {
    s32 unk00;
    s32 unk04;
    struct17 unk08;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 unk30;
    f32 unk34;
    f32 unk38;
} Arg150B06B0_2190;

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    struct17 unk08;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    s8 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s32 unk48;
} Arg150B06B0_3F18;

typedef struct {
    u8 pad[0x24];
} Floor150B06B0;

extern s32 func_15143E24(struct127 *);
extern void func_1504715C(Floor150B06B0 *, struct127 *);
extern void func_151C329C(void *, u8, s32);
extern void func_15152190(Arg150B06B0_2190 *, const s32 *, const f32 *, s32, f32, s32, u8, s32);
extern void func_15153F18(Arg150B06B0_3F18 *, struct17 *, Floor150B06B0 *, u8, s32);

extern const s32 D_8009F810[1];
extern const f32 D_8009F814[1];
extern const s32 D_8009F818[1];
extern const f32 D_8009F81C[1];

/*
 * Spawns effects at actor's position, aimed by func_15143E24(other): actor id 0xA6 gets func_151C329C
 * 50 units above it and one func_15152190 burst; any other actor gets a different burst plus two
 * func_15153F18 passes (unk3C = 0, then 1) using the func_1504715C info for actor.
 */
void func_150B06B0(struct127 *actor, struct127 *other, u8 arg2, s32 arg3) {
    struct17 pos;
    s8 angle;
    Floor150B06B0 floor;

    pos.unk0 = actor->x_position;
    pos.unk4 = actor->y_position;
    pos.unk8 = actor->z_position;
    angle = func_15143E24(other) + 0x40;
    func_1504715C(&floor, actor);
    if (actor->id == 0xA6) {
        struct17 raisedPos;
        Arg150B06B0_2190 burst;

        raisedPos.unk0 = pos.unk0;
        raisedPos.unk4 = pos.unk4 + 50.0f;
        raisedPos.unk8 = pos.unk8;
        func_151C329C(&raisedPos, arg2, arg3);
        burst.unk00 = 10;
        burst.unk04 = 10;
        burst.unk08 = pos;
        burst.unk1C = 7.0f;
        burst.unk20 = 17.0f;
        burst.unk14 = angle - 40;
        burst.unk16 = 80;
        burst.unk18 = -45;
        burst.unk1A = 26;
        burst.unk2C = 25;
        burst.unk2E = 25;
        burst.unk24 = -0.8000000119f;
        burst.unk28 = 0.3440000117f;
        burst.unk30 = 0.7070000172f;
        burst.unk34 = 0.5070000291f;
        burst.unk38 = 26.41100121f;
        func_15152190(&burst, D_8009F810, D_8009F814, 1, 0.0f, 0, arg2, arg3);
    } else {
        {
            Arg150B06B0_2190 burst;

            burst.unk00 = 10;
            burst.unk04 = 10;
            burst.unk08 = pos;
            burst.unk1C = 7.0f;
            burst.unk20 = 17.0f;
            burst.unk14 = angle - 40;
            burst.unk16 = 80;
            burst.unk18 = -21;
            burst.unk1A = 13;
            burst.unk2C = 25;
            burst.unk2E = 25;
            burst.unk24 = -0.8000000119f;
            burst.unk28 = 0.3440000117f;
            burst.unk30 = 0.04900000244f;
            burst.unk34 = 0.1020000055f;
            burst.unk38 = 26.41100121f;
            func_15152190(&burst, D_8009F818, D_8009F81C, 1, 0.0f, 0, arg2, arg3);
        }
        {
            Arg150B06B0_3F18 spray;

            spray.unk08 = pos;
            spray.unk14 = 3.049999952f;
            spray.unk2C = 7;
            spray.unk2E = 3;
            spray.unk00 = angle - 60;
            spray.unk02 = 120;
            spray.unk04 = -30;
            spray.unk06 = 16;
            spray.unk30 = 3;
            spray.unk32 = 2;
            spray.unk34 = 20;
            spray.unk36 = 20;
            spray.unk38 = 155;
            spray.unk3A = 100;
            spray.unk44 = 16;
            spray.unk46 = 15;
            spray.unk48 = 0;
            spray.unk3C = 0;
            spray.unk18 = 5.009999752f;
            spray.unk1C = 0.7000000477f;
            spray.unk20 = 0.5010000467f;
            spray.unk24 = 15.10000038f;
            spray.unk28 = 20.80000114f;
            spray.unk40 = 0.5f;
            func_15153F18(&spray, &spray.unk08, &floor, arg2, arg3);
            spray.unk3C = 1;
            func_15153F18(&spray, &spray.unk08, &floor, arg2, arg3);
        }
    }
}

/*
 * Picks a random spawn point on a 248-unit perimeter around owner: 122 units along the back edge
 * (-31.5 along forward), or 63 units along either side (61 out along right). Writes the point to outPos
 * (y taken from player->unk118) and a matching s16 yaw to outAngle.
 */
void func_150B0A60(struct127 *owner, struct127 *player, f32 *forward, f32 *right, f32 *outPos, s16 *outAngle) {
    f32 along;
    f32 spread;
    f32 side;

    *outAngle = (s16)(owner->unk40 * 0.7111111283f - 128.0f);
    along = func_150ADA68() * 248.0f;
    spread = func_150ADA68() + -0.5f;
    if (along < 122.0f) {
        outPos[0] = forward[0] * -31.5f + (right[0] * spread) * 122.0f;
        outPos[1] = 0.0f;
        outPos[2] = forward[1] * -31.5f + (right[1] * spread) * 122.0f;
    } else {
        f32 sideLength = 63.0f; // matching: a named local, not the literal, keeps 63.0f in one register

        side = 61.0f;
        along = along - 122.0f;
        if (along < sideLength) {
            side = -side;
            *outAngle += 0x40;
        } else {
            *outAngle -= 0x40;
        }
        outPos[0] = forward[0] * sideLength * spread + right[0] * side;
        outPos[1] = 0.0f;
        outPos[2] = forward[1] * sideLength * spread + right[1] * side;
    }
    outPos[0] += owner->x_position;
    outPos[1] += player->unk118;
    outPos[2] += owner->z_position;
}

void func_150B0C58(void *, u8, s32);

s32 func_150B0C34(s32 arg0) {
    func_150B0C58((void *)arg0, 0xFF, 1);
}

void func_150B0C58(void *arg0, u8 arg1, s32 arg2) {
    struct {
        void *unk0;
        u8 unk4;
        u8 pad5;
        u8 pad6;
        u8 pad7;
        f32 unk8;
    } sp34;
    struct260 *temp_v0;

    sp34.unk0 = arg0;
    sp34.unk4 = *(u8 *)((s32)arg0 + 0x3B);
    sp34.unk8 = 0.0f;

    temp_v0 = func_15149130(0x12C, -1, 0x58, -1, 0, 0x43, (struct37 *)0xC, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0xC);
    }
}

void func_150B0CE0(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

typedef struct {
    s32 *obj;
    u8 id;
    u8 pad5[3];
    f32 timer;
} Sub28_DDB60;

extern f32 D_8009F860[];
extern f32 D_8009F868[];
s32 func_1515C0F8(s32 *, f32 **);
void func_1514373C(f32, f32, f32 *, f32 *);
void func_15143794(s16, s16, f32, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);

void func_150B0D20(u8 *arg0)
{
  Sub28_DDB60 *sub;
  s32 *obj;
  s32 flag;
  f32 dist;
  f32 lim;
  s32 idx;
  f32 dir[2];
  f32 perp[2];
  f32 *vel;
  f32 pos[3];
  f32 rot[3];
  s16 angle;
  obj = *((s32 **) (arg0 + 0x28));
  sub = (Sub28_DDB60 *) (arg0 + 0x28);
  if (((*obj) == 0) || (sub->id != (*(((u8 *) obj) + 0x3B))))
  {
    *((s16 *) (arg0 + 0xE)) = -1;
    return;
  }
  idx = *(((u8 *) obj) + 0x124);
  flag = 0;
  if ((*(((u8 *) D_800CC2D0[idx].unk31C) + 0x4F)) == 2)
  {
    dist = *((f32 *) (((u8 *) obj) + 0x3C));
    lim = (*((f32 *) (((u8 *) (&D_800CC2D0[idx])) + 0x44))) * 1.350000024f;
    if (dist < lim)
    {
      dist = lim;
    }
    if (dist > 26.0f)
    {
      if (1)
      {
        flag = 1;
      }
    }
  }
  if (flag)
  {
    sub->timer += (D_8009F860[flag] + (func_150ADA68() * D_8009F868[flag])) * D_800BE9A4;
    if (sub->timer > 1.0f)
    {
      if (func_1515C0F8(obj, &vel) == 0)
      {
        vel = &D_800A5480;
      }
      func_1514373C((*((f32 *) (((u8 *) obj) + 0x40))) * 0.01745329238f, 1.0f, &dir[0], &dir[1]);
      perp[0] = dir[1];
      perp[1] = -dir[0];
      do
      {
        func_150B0A60(obj, (s32 *) (&D_800CC2D0[idx]), dir, perp, pos, &angle);
        func_15143794(angle, (func_150ADA20() % 26) - 41, (func_150ADA68() * 3.769999981f) + 4.059999943f, rot);
        rot[0] -= vel[0] * 0.9250000715f;
        rot[2] -= vel[2] * 0.9250000715f;
        func_151D9014(pos, rot, 5, (func_150ADA68() * 0.1780000031f) + (-0.4420000315f), (func_150ADA20() % 21) + 20, (func_150ADA20() % 101) + 155, (func_150ADA68() * 50.0f) + 80.0f, func_150ADA68() < 0.04400000349f, 1.399999976f, 1.399999976f, 1, 0, 1, 0, arg0[0xC], arg0[1]);
        sub->timer -= 1.0f;
      }
      while (sub->timer > 1.0f);
    }
  }
}
