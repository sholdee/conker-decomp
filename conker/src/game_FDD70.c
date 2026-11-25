#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 D_80088890;
extern s32 D_80088894;
extern s32 D_80088898;
extern s32 D_8008889C;
extern s32 *D_800DBF94; /* per prop: bit i set while D_800CC2D0[i] stands on it */

extern struct131 *func_151149AC(u8 id); /* the prop whose id is `id` */
extern void func_151951E0(struct127 *);
extern void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, s32, s32);
extern void func_1507CD64(struct127 *, s32);

s32 func_1509BE40(s32, s32, s32, ...);
void func_1509BFB0(s32, s32, s32, ...);
/* Eases *value toward target, tracking its rate of change in *velocity. */
void func_150495B0(f32 *value, f32 target, f32 *velocity, f32 arg3, f32 arg4, f32 delta);

/* Current values, targets and easing velocities of three channels. */
extern f32 D_800888C0;
extern f32 D_800888C4;
extern f32 D_800888C8;
extern f32 D_800888CC;
extern f32 D_800888D0;
extern f32 D_800888D4;
extern f32 D_800888D8;
extern f32 D_800888DC;
extern f32 D_800888E0;

/* Picks targets for three channels (255, or one of them 64 per flags 0x4043-0x4045 while 0x403D is
 * set), eases the channels toward them, and writes them to ids 0x30F3-0x30F5 and 0x30F8-0x30FA. */
void func_150D08C0(struct108 *camera) {
    if (func_1509BE40(1, 0x403D, 6, 0x9000)) {
        camera->unk84 |= 0x80021000;
        if (func_1509BE40(1, 0x4043, 6, 0x9000)) {
            D_800888CC = 64.0f;
            D_800888D0 = 255.0f;
            D_800888D4 = 255.0f;
        } else if (func_1509BE40(1, 0x4044, 6, 0x9000)) {
            D_800888CC = 255.0f;
            D_800888D0 = 64.0f;
            D_800888D4 = 255.0f;
        } else if (func_1509BE40(1, 0x4045, 6, 0x9000)) {
            D_800888CC = 255.0f;
            D_800888D0 = 255.0f;
            D_800888D4 = 64.0f;
        } else {
            D_800888CC = 255.0f;
            D_800888D0 = 255.0f;
            D_800888D4 = 255.0f;
        }
    } else {
        D_800888CC = 255.0f;
        D_800888D0 = 255.0f;
        D_800888D4 = 255.0f;
        camera->unk84 &= ~0x80021000;
        camera->unk84 |= 8;
    }
    func_150495B0(&D_800888C0, D_800888CC, &D_800888D8, 4.0f, 6.0f, camera->unk7B4);
    func_150495B0(&D_800888C4, D_800888D0, &D_800888DC, 4.0f, 6.0f, camera->unk7B4);
    func_150495B0(&D_800888C8, D_800888D4, &D_800888E0, 4.0f, 6.0f, camera->unk7B4);
    func_1509BFB0(1, 0x30F5, 0x12, (u8)D_800888C0);
    func_1509BFB0(1, 0x30F4, 0x12, (u8)D_800888C4);
    func_1509BFB0(1, 0x30F3, 0x12, (u8)D_800888C8);
    func_1509BFB0(1, 0x30FA, 0x12, (u8)D_800888C0);
    func_1509BFB0(1, 0x30F9, 0x12, (u8)D_800888C4);
    func_1509BFB0(1, 0x30F8, 0x12, (u8)D_800888C8);
}

/* Steps four scroll counters by D_800BE9E4; then, unless D_800CC2D0[0]'s unk31C->unk120 is set, kills
 * D_800CC2D0[0] once per prop 0xFA-0xF8 it stands on, and if it stands on prop 0xFB plays sound 0x627 and
 * runs it through func_15136C3C .. func_1507CD64(6). */
void func_150D0E90(s32 arg0) {
    struct131 *prop;
    s32 propIndex;
    s32 i;

    D_80088890 += D_800BE9E4 * 40;
    D_80088894 += D_800BE9E4 * -66;
    D_80088898 += D_800BE9E4 * -8;
    D_8008889C += D_800BE9E4 * 92;

    if (D_800CC2D0->unk31C->unk120 == 0) {
        for (i = 0; i < 3; i++) {
            prop = func_151149AC(0xFA - i);
            propIndex = prop - D_800DBEF4;
            if (D_800DBF94[propIndex] & 1) {
                func_151951E0(D_800CC2D0);
                func_10010154(0x627, D_800CC2D0, 0x7FFF, 200, 700);
            }
        }

        prop = func_151149AC(0xFB);
        propIndex = prop - D_800DBEF4;
        if (D_800DBF94[propIndex] & 1) {
            func_10010154(0x627, D_800CC2D0, 0x7FFF, 200, 700);
            func_15136C3C(D_800CC2D0, 1, 1, 1, 1, 0, 0xFF, 1);
            func_15145A50(D_800CC2D0);
            func_1507CD64(D_800CC2D0, 6);
        }
    }
}

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Spawn parameters of a blood strip (see func_15134DAC in game_161520.c). */
typedef struct {
    /* 0x00 */ u8 ownerId;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ struct127 *owner;
    /* 0x08 */ u8 bone;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ Vec3f offsetA;
    /* 0x18 */ Vec3f offsetB;
    /* 0x24 */ u8 flags;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ s16 duration;
    /* 0x28 */ s16 pause;
    /* 0x2A */ s16 lifetime;
    /* 0x2C */ u16 sound;
    /* 0x2E */ s8 sampleCallback;
    /* 0x2F */ s8 emitCallback;
    /* 0x30 */ s8 rateCallback;
    /* 0x31 */ u8 pad31[3];
    /* 0x34 */ f32 rate;
    /* 0x38 */ u8 mode;
    /* 0x39 */ s8 updateCallback;
    /* 0x3A */ u8 pad3A[2];
} BloodStripDesc;

/* A live blood strip (see game_161520.c); only the fields used here are named. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x74 - 0xD];
    /* 0x74 */ f32 power; /* set by the sample callback */
    /* 0x78 */ f32 time;
} BloodStrip;

BloodStrip *func_15134DAC(BloodStripDesc *desc, s32 arg1);

/* Spawns a blood strip on obj that uses sample callback 6 and emit callback 7 (below);
 * mirrored selects which way it is offset. */
void func_150D10E4(struct127 *obj, u8 mirrored) {
    BloodStripDesc desc;

    desc.ownerId = obj->unique_id;
    desc.owner = obj;
    desc.bone = 0;
    desc.offsetA.x = 0.0f;
    desc.offsetA.y = 0.0f;
    desc.offsetA.z = 0.0f;
    desc.offsetB.x = 0.0f;
    if (mirrored) {
        desc.offsetB.y = -30.0f;
    } else {
        desc.offsetB.y = 30.0f;
    }
    desc.offsetB.z = 0.0f;
    desc.flags = 2;
    desc.duration = 50;
    desc.pause = 22;
    desc.lifetime = 2000;
    desc.sound = 0;
    desc.sampleCallback = 6;
    desc.emitCallback = 7;
    desc.rateCallback = -1;
    desc.rate = 0.3508499861f;
    desc.mode = 0;
    desc.updateCallback = -1;
    func_15134DAC(&desc, 0);
}

/* BloodStrip sample callback 6: random power in [0.35, 0.5). */
s32 func_150D11B4(BloodStrip *strip) {
    strip->power = (func_150ADA68() * 150.0f + 350.0f) * 0.001f;
    return 1;
}

void func_151DB5D0(s32, Vec3f *, Vec3f *, f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32);

/* BloodStrip emit callback 7: moves both strip points `time` along their velocities and emits one
 * func_151DB5D0 particle at A, heading from A to B scaled by the strip's power. */
void func_150D1204(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip) {
    Vec3f start;
    f32 endX;
    f32 endY;
    f32 endZ;
    Vec3f velocity;
    s32 pad; // matching: unused stack slot
    s32 randomInt;
    f32 random1;
    f32 random0;

    start.x = pointA->x + velocityA->x * time;
    start.y = pointA->y + velocityA->y * time;
    start.z = pointA->z + velocityA->z * time;

    endX = pointB->x + velocityB->x * time;
    endY = pointB->y + velocityB->y * time;
    endZ = pointB->z + velocityB->z * time;

    velocity.x = (endX - start.x) * strip->power;
    velocity.y = (endY - start.y) * strip->power;
    velocity.z = (endZ - start.z) * strip->power;

    random0 = func_150ADA68();
    random1 = func_150ADA68();
    randomInt = func_150ADA20();

    func_151DB5D0(0, &start, &velocity, random0 * 61.0f + 60.0f, 1.03571403f, 0.9704080224f,
                  random1 * 0.1570000052f + -0.08000000566f, (randomInt & 0xF) + 35,
                  func_150ADA20() % 156U + 100, 30, 8, 0, strip->unkC, strip->unk1);
}
