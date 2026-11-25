#include <ultra64.h>
#define func_150EB430 func_150EB430_header
#include "functions.h"
#undef func_150EB430
#define D_800D3098 D_800D3098_header
#include "variables.h"
#undef D_800D3098

/* An entry of the object table at D_800D3098 (struct178, a blob in structs.h): a collision
   cylinder at (x, y, z); see CollisionCylinder in game_D0960.c. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
    /* 0x0A */ u8 padA[0x18 - 0xA];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x34 - 0x1C];
} GateCylinder; /* size 0x34 */

/* variables.h declares D_800D3098 as an array; the code loads it as a pointer to the table. */
extern GateCylinder *D_800D3098;
extern s32 D_80088AA0[4];
extern s32 D_80088AB0[8];
extern s32 func_150A34B0(GateCylinder *cyl, vertex *start, vertex *end, vertex *hit);
extern void func_150A2864(s32, s32);
s32 func_150EB484(vertex *start, vertex *end, vertex *hit);
void func_150EBC80(GateCylinder *gate, u8 arg1, s32 arg2);

/* Tests the segment from start to start + delta against the gate cylinders. */
void func_150EB430(vertex *start, vertex *delta, vertex *hit) {
    vertex end;

    end.x = start->x + delta->x;
    end.y = start->y + delta->y;
    end.z = start->z + delta->z;
    func_150EB484(start, &end, hit);
}

/*
 * Tests the segment from start to end against the D_800D3098 cylinders. While object 9 exists,
 * a hit on one of the four D_80088AA0 cylinders disables it (func_150A2864(id, 1)), stores
 * which one (1-4) in the object's unk2E8 and plays func_150EBC80's effect at it; the eight
 * D_80088AB0 cylinders are only tested. Otherwise only cylinder 76 is tested, and a hit sets
 * its unk18. Returns TRUE when object 9 exists and the segment hit one of the twelve.
 */
s32 func_150EB484(vertex *start, vertex *end, vertex *hit) {
    s32 i;
    s32 id;
    struct127 *obj;

    obj = func_15083E90(9);
    if (obj != NULL) {
        for (i = 0; i < 4; i++) {
            id = D_80088AA0[i];
            if (func_150A34B0(&D_800D3098[id], start, end, hit)) {
                func_150A2864(id, 1);
                obj->unk2E8 = i + 1;
                func_150EBC80(&D_800D3098[id], 0xFF, 1);
                return TRUE;
            }
        }
        for (i = 0; i < 8; i++) {
            if (func_150A34B0(&D_800D3098[D_80088AB0[i]], start, end, hit)) {
                return TRUE;
            }
        }
        return FALSE;
    }
    if (func_150A34B0(&D_800D3098[76], start, end, hit)) {
        D_800D3098[76].unk18 = 1;
    }
    return FALSE;
}

typedef struct Obj150EB614 {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x3C - 0x16];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0x4F - 0x40];
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 pad50[0x73 - 0x50];
    /* 0x73 */ u8 unk73;
    /* 0x74 */ u8 pad74[0x7C - 0x74];
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
    /* 0x84 */ f32 unk84;
} Obj150EB614;

void func_15117770(Obj150EB614 *arg0);

void func_150EB614(Obj150EB614 *arg0) {
    f32 vel;
    f32 dz;
    f32 target;
    f32 dx;
    f32 damp;
    f32 accel;
    f32 dy;
    f32 angle;
    f32 prev;
    f32 dist;
    f32 diff;
    f32 absdiff;

    if ((arg0->unk73 & 3) != 3) {
    if ((arg0->unk73 & 3) != 2) {
    if (arg0->unk4F & 4) {
        if (D_800CC2D0->unk31C->unk57 != 0) {
            arg0->unk73 &= ~3;
            arg0->unk73 |= 2;
            arg0->unk3C = 0;
            arg0->unk7C = 0.5f;
            arg0->unk80 = 0.3000000119f;
            arg0->unk84 = 0.0f;
        }
        dz = arg0->unk14 - D_800CC2D0->z_position;
        dx = arg0->unk10 - D_800CC2D0->x_position;
        dy = arg0->unk12 - D_800CC2D0->y_position;
        dist = sqrtf(dz * dz + (dx * dx + dy * dy));
        if (dist > 740.0f) {
            dist = 740.0f;
        }
        target = dist * 0.01081081107f + -40.0f;
    } else {
        target = -40.0f;
    }
    angle = arg0->unk0;
    damp = arg0->unk7C;
    accel = arg0->unk80;
    vel = arg0->unk84;
    if (angle != target || vel != 0.0f) {
        angle += vel * D_800BE9E4;
        diff = func_15048A70(angle, target);
        absdiff = fabsf(diff);
        if (diff > 0.0f) {
            vel += absdiff * accel;
        } else {
            vel -= absdiff * accel;
        }
        vel *= damp;
        if (fabsf(diff) < 0.009999999776f && fabsf(vel) < 0.009999999776f) {
            vel = 0.0f;
            angle = target;
        } else if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        arg0->unk84 = vel;
    }
    arg0->unk0 = angle;
    } else {
        prev = arg0->unk0;
        func_15117770(arg0);
        if (prev == arg0->unk0) {
            arg0->unk73 &= ~3;
            arg0->unk73 |= 3;
            arg0->unk0 = 0.0f;
        }
    }
    }
}

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    struct17 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct17 unk28;
    struct17 unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    s8 unk69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    u8 pad76[2];
    s32 unk78;
} Func150EB8C4Spawn;

extern u8 D_800BE9EB;
void func_150E83AC(void *, s16, u8, s32);
void *func_15164F0C(u8, u8, void *, u8, s32);
void func_151D3FF4(void *, u8, s32);
void func_151D5514(void *, u8, s32);
void *func_151541B8(void *, f32, f32, f32, f32, u8, s32);
extern struct17 D_800A1490;
extern struct17 D_800A149C;
extern struct17 D_800A14A8[4];
extern u8 D_800A14D8[4];
extern void func_15143794(s16, s16, f32, f32 *);
extern s32 func_1513264C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);

void func_150EB8C4(void)
{
  Func150EB8C4Spawn spawn;
  func_15164F0C(2, D_800BE9EB, 0, 0xFF, 1);
  func_150E83AC(&D_800A149C, (s16) ((func_150ADA20() % 62U) + 0x78), 0xFF, 1);
  func_151D5514(&D_800A1490, 0xFF, 1);
  func_151D3FF4(&D_800A1490, 0xFF, 1);
  func_151541B8(&D_800A1490, (func_150ADA68() * 4.0f) + 12.0f, 1.6409999f, (f32) ((func_150ADA20() % 56U) + 0xC8), 0.0f, 0xFF, 1);
  {
    u8 i;
    s32 r1;
    s32 r2;
    spawn.unk0C = 1.0f;
    spawn.unk08 = 1.0f;
    spawn.unk00 = 9.0f;
    spawn.unk04 = 0.4350000322f;
    spawn.unk10 = *((struct17 *) (&D_800A5480));
    spawn.unk44 = 0.0f;
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.unk50 = 0x29E9;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 0xD;
    spawn.unk62 = 0;
    spawn.unk63 = 0xC;
    if (r2)
    {
    }
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 2;
    spawn.unk6A = 2;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 0x19;
    spawn.unk74 = 0xA;
    spawn.unk78 = 0;
    for (i = 0; i < 4; i++)
    {
      spawn.unk28 = D_800A14A8[i];
      r1 = func_150ADA20();
      r2 = func_150ADA20();
      func_15143794((r1 % 26U) + 0x2D, (r2 & 0xF) - 0x19, (func_150ADA68() * 30.0f) + 15.0f, &spawn.unk34.unk0);
      spawn.unk40 = (func_150ADA68() * 15.10800076f) + (-7.554000378f);
      spawn.unk48 = (func_150ADA68() * 15.10800076f) + (-7.554000378f);
      spawn.unk4C = (func_150ADA68() * 0.6170000434f) + (-1.621000051f);
      spawn.unk54 = (func_150ADA20() % 51U) + 0x32;
      spawn.unk56 = D_800A14D8[i];
      func_1513264C(&spawn, 3, 0xFF, 0, 0, 0xFF, 1);
    }

  }
}

typedef struct Game1188E0Event {
    u8 type;
    u8 pad1;
    s16 duration;
    u8 field4;
    u8 field5;
    s8 field6;
    u8 pad7;
} Game1188E0Event;

typedef struct Game1188E0Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Game1188E0Vec3f;

void *func_151D8868(void *, s32, u8, s32);
void func_150E83AC(void *, s16, u8, s32);
void *func_15164F0C(u8, u8, void *, u8, s32);
void func_151D3FF4(void *, u8, s32);
void func_151D5514(void *, u8, s32);
void *func_151541B8(void *, f32, f32, f32, f32, u8, s32);

void func_150EBC80(GateCylinder *arg0, u8 arg1, s32 arg2) {
    Game1188E0Vec3f position;
    f32 random;
    f32 step;
    Game1188E0Event event;
    f32 scale;
    u8 index;

    position.x = ((s16 *)arg0)[0];
    position.y = ((s16 *)arg0)[1];
    position.z = ((s16 *)arg0)[2];
    step = ((s16 *)arg0)[4] * 0.5f;
    func_150E83AC(&position, (s16)((func_150ADA20() % 101U) + 0x12C),
                  arg1, arg2);
    event.type = 1;
    event.duration = (func_150ADA20() % 21U) + 0xF;
    event.field4 = 0;
    event.field6 = -1;
    event.field5 = 1;
    func_151D8868(&event, 0, arg1, arg2);
    func_15164F0C(0, 0, 0, arg1, arg2);
    scale = 1.6409999f;
    index = 0;
    do {
        func_151D3FF4(&position, arg1, arg2);
        func_151D5514(&position, arg1, arg2);
        random = func_150ADA68();
        func_151541B8(&position, random * 4.0f + 12.0f, scale,
                     (f32)((func_150ADA20() % 56U) + 0xC8), 0.0f, arg1, arg2);
        index++;
        position.y += step;
    } while (index < 2);
}
