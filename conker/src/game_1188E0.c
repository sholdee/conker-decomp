#include <ultra64.h>
#define func_150EB430 func_150EB430_header
#include "functions.h"
#undef func_150EB430
#define D_800D3098 D_800D3098_header
#include "variables.h"
#undef D_800D3098

s32 func_150EB484(void *arg0, struct17 *arg1, void *arg2);

void func_150EB430(void *arg0, void *arg1, void *arg2) {
    struct17 sp1C;

    sp1C.unk0 = ((struct17 *) arg0)->unk0 + ((struct17 *) arg1)->unk0;
    sp1C.unk4 = ((struct17 *) arg0)->unk4 + ((struct17 *) arg1)->unk4;
    sp1C.unk8 = ((struct17 *) arg0)->unk8 + ((struct17 *) arg1)->unk8;
    func_150EB484(arg0, &sp1C, arg2);
}

typedef struct Gate150EB484 {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1C[0x34 - 0x1C];
} Gate150EB484;

extern Gate150EB484 *D_800D3098;
extern s32 D_80088AA0[4];
extern s32 D_80088AB0[8];
extern s32 func_150A34B0(Gate150EB484 *, void *, struct17 *, void *);
extern void func_150A2864(s32, s32);
void func_150EBC80(void *arg0, u8 arg1, s32 arg2);

s32 func_150EB484(void *arg0, struct17 *arg1, void *arg2) {
    s32 i;
    s32 id;
    struct127 *obj;

    obj = func_15083E90(9);
    if (obj != NULL) {
        for (i = 0; i < 4; i++) {
            id = D_80088AA0[i];
            if (func_150A34B0(&D_800D3098[id], arg0, arg1, arg2)) {
                func_150A2864(id, 1);
                obj->unk2E8 = i + 1;
                func_150EBC80(&D_800D3098[id], 0xFF, 1);
                return 1;
            }
        }
        for (i = 0; i < 8; i++) {
            if (func_150A34B0(&D_800D3098[D_80088AB0[i]], arg0, arg1, arg2)) {
                return 1;
            }
        }
        return 0;
    }
    if (func_150A34B0(&D_800D3098[76], arg0, arg1, arg2)) {
        D_800D3098[76].unk18 = 1;
    }
    return 0;

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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1188E0/func_150EB8C4.s")

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

void func_150EBC80(void *arg0, u8 arg1, s32 arg2) {
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
