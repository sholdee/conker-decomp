#include <ultra64.h>
// matching: functions.h declares func_151D9B8C (as defined) with an s32 4th parameter; this file
// was compiled against a void * view, which schedules the call differently.
#define func_151D9B8C func_151D9B8C_s32_decl_in_functions_h
#include "functions.h"
#undef func_151D9B8C
void func_151D9B8C(u8, f32, u8, void *, f32 *, s16, u8, u8, u8, u8, s32);
#include "variables.h"

void func_151DAB58(u8, f32, u8, f32 *, s32, u8, s32);

/* Parameter block for func_15153F18; the s16 header (unk0..unk6) is passed separately. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ f32 position[3];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ u8 pad3D[3];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s32 unk48;
} BurstParams;

/* Ground hit filled by func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 unk4[0x19];
    /* 0x1D */ u8 surfaceType;
    /* 0x1E */ u8 pad1E[6];
} GroundHit;

void func_15153F18(s16 *, void *, GroundHit *, s32, s32);
void func_1504715C(GroundHit *, Actor *);
s32 func_15046C80(f32 *, s32, f32, GroundHit *);
void func_15143874(s16, f32, f32 *, f32 *);
void func_15136698(f32, f32, u8, u8, s32, void *, f32 *, s32, s32, s32, s32);

/* Spawns a ground burst effect around obj: one func_15153F18 burst, 7-12 scattered
 * func_151D9B8C / func_151DAB58 particles on the ground, and a func_15136698 effect under obj. */
void func_151AECA0(Actor *obj, u8 arg1, s32 arg2) {
    GroundHit ground;
    BurstParams burst;
    s32 count;
    f32 probe[3];
    f32 pos[3];
    f32 center[3];
    f32 centerProbe[3];
    f32 unused[15]; // matching: unused, pads the frame to the original 0x160 bytes
    u32 randA;
    u32 randB;
    f32 randF1;
    f32 randF2;
    volatile u32 angle; // matching: volatile forces the spill to sp+0x6C between the two calls

    if (obj == NULL) {
        return;
    }
    func_1504715C(&ground, obj);
// matching: the unused labels lbD/lbA/lbC/lbB/lbl split basic blocks; without them uopt
// register-allocates far more (callee-saved $f22-$f30, s5/s6) and the frame grows
lbD:
    burst.position[0] = obj->x_position;
    burst.position[1] = obj->y_position;
    burst.position[2] = obj->z_position;
    burst.unk14 = 5.079999924f;
    burst.unk2C = 10;
    burst.unk2E = 0;
    burst.unk2 = 255;
    burst.unk4 = -63;
    burst.unk0 = 0;
    burst.unk6 = 45;
    burst.unk30 = 3;
    burst.unk32 = 2;
    burst.unk34 = 40;
    burst.unk36 = 20;
    burst.unk38 = 155;
    burst.unk3A = 100;
    burst.unk44 = 16;
    burst.unk46 = 15;
    burst.unk48 = 0;
    burst.unk3C = 0;
    burst.unk18 = 3.019999981f;
    burst.unk1C = 0.807000041f;
    burst.unk20 = 0.7010000348f;
    burst.unk24 = 18.0f;
    burst.unk28 = 11.80000019f;
    burst.unk40 = 0.5f;
    func_15153F18(&burst.unk0, burst.position, &ground, 255, 1);
lbA:
    count = (func_150ADA20() % 6) + 7;
    probe[1] = obj->y_position + 100.0f;
    while (count != 0) {
        angle = func_150ADA20();
        func_15143874((u8)angle, (func_150ADA68() * 59.0f) + 90.0f, &pos[0], &pos[2]);
lbC:
        pos[0] += obj->x_position;
        pos[2] += obj->z_position;
        probe[0] = pos[0];
        probe[2] = pos[2];
        if (func_15046C80(probe, 0, obj->y_position - 500.0f, &ground) == 0 || ground.surfaceType == 3) {
            return;
        }
        pos[1] = ground.height;
        pos[1] += 10.0f;
        if (func_150ADA20() & 1) {
            func_151D9B8C(0, (func_150ADA68() * 4.5f) + 15.0f, (func_150ADA68() * 100.0f) + 155.0f,
                          ground.unk4, pos, 100, 0, 1, 0, arg1, arg2);
        } else {
            func_151DAB58(0, (func_150ADA68() * 5.900000095f) + 6.099999905f, (func_150ADA68() * 100) + 155, // matching: int 100/155 here, float literals change allocation
                          pos, 1, arg1, arg2);
        }
        count--;
    }
lbB:
    center[0] = obj->x_position;
    center[1] = obj->y_position;
    center[2] = obj->z_position;
    centerProbe[0] = center[0];
    centerProbe[1] = center[1] + 100.0f;
    centerProbe[2] = center[2];
    if (func_15046C80(centerProbe, 0, center[1] - 500.0f, &ground) == 0 || ground.surfaceType == 3) {
        return;
    }
    center[1] = ground.height;
    center[1] += 10.0f;
    randF1 = func_150ADA68();
    randF2 = func_150ADA68();
    randA = func_150ADA20();
    randB = func_150ADA20();
    func_15136698(((randF1 * 30.0f) + 90.0f) * 0.001000000047f,
                  ((randF2 * 103.0f) + 103.0f) * 9.999999747e-06f,
                  (randA % 101) + 155, (randB % 101) + 100,
                  (func_150ADA20() % 56) + 70, ground.unk4, center, 0, 1, arg1, arg2);
lbl:;
}
