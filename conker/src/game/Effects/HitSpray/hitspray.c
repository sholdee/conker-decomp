#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Hit effects at pos plus a burst of debris particles. */
void effect_spawnRayHitDebris(Vec3f *pos, u8 fxOwner, s32 fxList) {
    JetSpraySpec fx;

    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);

    fx.angles.yawBase = 0;
    fx.angles.yawRange = 255;
    fx.angles.pitchBase = -64;
    fx.angles.pitchRange = 71;
    fx.spray.countBase = 6;
    fx.spray.countRange = 4;
    fx.spray.pos = *pos;
    fx.spray.speedBase = 23.0f;
    fx.spray.speedRange = 30.0f;
    fx.spray.sizeBase = 45.0f;
    fx.spray.sizeRange = 53.0f;
    fx.spray.sizeSlopeBase = 203.0f;
    fx.spray.sizeSlopeRange = 414.0f;
    fx.spray.particleCountBase = 7;
    fx.spray.particleCountRange = 3;
    fx.spray.spread = 15.0f;
    fx.spray.drag = 0.8720000386f;
    fx.spray.gravityBase = -0.3980000317f;
    fx.spray.gravityRange = 0.3640000224f;
    fx.spray.delayBase = 25;
    fx.spray.delayRange = 15;
    fx.spray.alphaBase = 100;
    fx.spray.alphaRange = 100;
    fx.spray.fadeStart = 12;
    fx.spray.fadeOutStart = 20;
    fx.spray.eventFunc = 0;

    particle_spawnJetSprayBursts(&fx, fxOwner, fxList);
}

/* Hit effects at pos plus impact particles thrown back along -dir, off surface if there is one. */
void effect_spawnRaySurfaceImpact(CollisionTri *surface, Vec3f *pos, Vec3f *dir, u8 fxOwner, s32 fxList) {
    SurfaceJetSpray surfaceFx;
    DirectedJetSpray fx;

    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);
    if (surface != NULL) {
        surfaceFx.source.normal.x = -dir->x;
        surfaceFx.source.normal.y = -dir->y;
        surfaceFx.source.normal.z = -dir->z;
        surfaceFx.source.triangle = *surface;
        surfaceFx.spray.pos = *pos;
        weapon_initImpactSpray(&surfaceFx.spray, &surfaceFx.source.spread);
        particle_spawnSurfaceJetSpray(&surfaceFx, fxOwner, fxList);
    } else {
        fx.direction.x = -dir->x;
        fx.direction.y = -dir->y;
        fx.direction.z = -dir->z;
        weapon_initImpactSpray(&fx.spray, &fx.spread);
        fx.spray.pos = *pos;
        particle_spawnDirectedJetSpray(&fx, fxOwner, fxList);
    }
}
