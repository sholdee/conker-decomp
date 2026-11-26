#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Copies obj's position into pos (using unk180 as the height when set); with a hit buffer, also probes the ground below. */
s32 particle_getObjGroundPos(Vec3f *pos, Actor *obj, CollisionHit *hit, s32 arg3) {
    Vec3f probe;

    pos->x = obj->xpos;
    if (-9500.0f < obj->floorY) {
        pos->y = obj->floorY;
    } else {
        pos->y = obj->ypos;
    }
    pos->z = obj->zpos;
    if (hit == NULL) {
        return 1;
    }
    probe.x = pos->x;
    probe.y = pos->y + 100.0f;
    probe.z = pos->z;
    collision_initHitFromObject(hit, obj);
    return collision_probeFloorSurfABSet0Cached(&probe, 0, pos->y - 100.0f, hit);
}

/* Constant handler; always returns 9. */
s32 func_150BAA00(s32 arg0, s32 arg1) {
    return 9;
}

/* Spawns effects at obj; when the ground probe hits, also a burst aimed by camera_global_Cameras[gCurrentView].TrueCameraAngle (degrees). */
void particle_spawnObjEffects(Actor *obj, s32 arg1, s32 arg2) {
    Vec3f pos;
    CollisionHit hit;
    s32 angle;
    u8 onGround;
    s16 pad; // matching: unused slot moves IDO's spill of func_150ADA68()'s result to sp38

    if (obj->ob_matrix_ptr == NULL) {
        return;
    }

    onGround = particle_getObjGroundPos(&pos, obj, &hit, (u8)arg1);
    particle_startPlayerRumbles(&pos, 1307.0f, 2000.0f, 0.0005f, 0xC, 0xF, 0xFF, 0);
    func_15143E94(5, 0x4022);

    if (onGround) {
        angle = camera_global_Cameras[gCurrentView].TrueCameraAngle * 0.7111111283f;
        effect_spawnQuadEffect(-1, pos.x, pos.y + 6.0f, pos.z,
                      0x19, 0x12, 0, 0xFF, 1);
        particle_emitOnRandomCircle(pos.x, pos.y, pos.z,
                      (func_150ADA68() * 50.0f) + 40.0f, angle + 60, angle - 60,
                      (func_150ADA20() % 11U) + 30, 5, 0, 0.0f, 0, 0xFF);
    }
}
