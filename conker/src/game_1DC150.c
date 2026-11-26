#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Spawns a ground burst effect around obj: one func_15153F18 burst, 7-12 scattered
 * func_151D9B8C / func_151DAB58 particles on the ground, and a func_15136698 effect under obj. */
void func_151AECA0(Actor *obj, u8 arg1, s32 arg2) {
    CollisionHit ground;
    SplashSpraySpec burst;
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
    burst.spray.pos.x = obj->xpos;
    burst.spray.pos.y = obj->ypos;
    burst.spray.pos.z = obj->zpos;
    burst.spray.scaleBase = 5.079999924f;
    burst.spray.countBase = 10;
    burst.spray.countRange = 0;
    burst.angles.yawRange = 255;
    burst.angles.pitchBase = -63;
    burst.angles.yawBase = 0;
    burst.angles.pitchRange = 45;
    burst.spray.itemCountBase = 3;
    burst.spray.itemCountRange = 2;
    burst.spray.lifeBase = 40;
    burst.spray.lifeRange = 20;
    burst.spray.unk30 = 155;
    burst.spray.unk32 = 100;
    burst.spray.unk3C = 16;
    burst.spray.unk3E = 15;
    burst.spray.unk40 = 0;
    burst.spray.kind = 0;
    burst.spray.scaleRange = 3.019999981f;
    burst.spray.sizeBase = 0.807000041f;
    burst.spray.sizeRange = 0.7010000348f;
    burst.spray.speedBase = 18.0f;
    burst.spray.speedRange = 11.80000019f;
    burst.spray.unk38 = 0.5f;
    func_15153F18(&burst.angles, &burst.spray, &ground, 255, 1);
lbA:
    count = (func_150ADA20() % 6) + 7;
    probe[1] = obj->ypos + 100.0f;
    while (count != 0) {
        angle = func_150ADA20();
        func_15143874((u8)angle, (func_150ADA68() * 59.0f) + 90.0f, &pos[0], &pos[2]);
lbC:
        pos[0] += obj->xpos;
        pos[2] += obj->zpos;
        probe[0] = pos[0];
        probe[2] = pos[2];
        if (func_15046C80(probe, 0, obj->ypos - 500.0f, &ground) == 0 || ground.source == 3) {
            return;
        }
        pos[1] = ground.height;
        pos[1] += 10.0f;
        if (func_150ADA20() & 1) {
            func_151D9B8C(0, (func_150ADA68() * 4.5f) + 15.0f, (func_150ADA68() * 100.0f) + 155.0f,
                          &ground.tri, pos, 100, 0, 1, 0, arg1, arg2);
        } else {
            func_151DAB58(0, (func_150ADA68() * 5.900000095f) + 6.099999905f, (func_150ADA68() * 100) + 155, // matching: int 100/155 here, float literals change allocation
                          pos, 1, arg1, arg2);
        }
        count--;
    }
lbB:
    center[0] = obj->xpos;
    center[1] = obj->ypos;
    center[2] = obj->zpos;
    centerProbe[0] = center[0];
    centerProbe[1] = center[1] + 100.0f;
    centerProbe[2] = center[2];
    if (func_15046C80(centerProbe, 0, center[1] - 500.0f, &ground) == 0 || ground.source == 3) {
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
                  (func_150ADA20() % 56) + 70, &ground.tri, center, 0, 1, arg1, arg2);
lbl:;
}
