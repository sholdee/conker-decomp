#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"



/* Dust at one of actor's feet (foot 0 or 1: bone 13 or 17), on the ground height floorY: a particle_emitOnCircle
 * ring and a effect_spawnQuadEffect quad under the foot, then 5-12 particles thrown outwards, sized by the actor's
 * mean scale. */
void particle_spawnFootDust(Actor *actor, u8 foot) {
    Vec3f footPos;
    s32 bone;
    f32 size;
    ParticleSpawn spawn;
    f32 radius;
    f32 speed;
    f32 cosA;
    f32 sinA;
    f32 scale;
    u32 count;
    u8 angle;
    FloorParticle *particle;

    size = (actor->scale_xz + actor->scale_y) * 0.5f;
    if (actor != NULL && actor->ob_matrix_ptr != NULL) {
        switch (foot) {
        case 0:
            bone = 13;
            break;
        case 1:
            bone = 17;
            break;
        }
        // matching: an integer cast keeps ob_matrix_ptr in v1 (a pointer argument is loaded straight into a0)
        math_getBoneTranslation((s32)actor->ob_matrix_ptr, bone, &footPos);
        radius = size * 80.0f;
        particle_emitOnCircle(footPos.x, actor->floorY, footPos.z, radius, 0, 16, 16, 0, 0, radius * 4.0f, 0, 0xFF);
        effect_spawnQuadEffect(-1, footPos.x, (s32)actor->floorY + 2.0f, footPos.z, 11, 19, 0, 0xFF, 0);
        spawn.model = 5;
        spawn.flags = 0x29E8;
        spawn.variant = 0;
        spawn.unk5C = NULL;
        spawn.alpha = 0xFF;
        spawn.updateFn = 1;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 0;
        spawn.unk6A = 1;
        spawn.owner = actor;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.elasticity = 0.400000006f;
        spawn.rotVel.z = 0.0f;
        spawn.gravity = -0.8999999762f;
        spawn.ownerId = actor->unique_id;
        spawn.fadeStart = 16;
        spawn.fadeRate = 15;
        count = (math_randomU8() & 7) + 5;
        while (count--) {
            angle = math_randomU8() & 0xFF; // matching: the redundant mask on the u8 result is in the original
            speed = (math_randomFloat() * 0.8999999762f + 0.1000000015f) * 18.0f * size;
            cosA = math_cosBinAngleMasked(angle - 0x40);
            sinA = math_cosBinAngleMasked(angle);
            spawn.lifetime = (math_randomU8() & 0x1F) + 15;
            spawn.pos.x = (radius * cosA) + footPos.x;
            spawn.pos.y = actor->floorY + 2.0f;
            spawn.vel.y = speed;
            spawn.pos.z = (radius * sinA) + footPos.z;
            spawn.vel.x = speed * cosA;
            spawn.vel.z = speed * sinA;
            scale = (math_randomFloat() * 0.8999999762f + 0.1000000015f) * size * 2.230000019f;
            spawn.sizeXZ = scale;
            spawn.sizeY = scale;
            spawn.radius = scale;
            spawn.rot.x = math_randomFloat() * 360.0f;
            spawn.rot.y = math_randomFloat() * 360.0f;
            spawn.rot.z = math_randomFloat() * 360.0f;
            spawn.rotVel.x = 25.0f - math_randomFloat() * 50.0f;
            spawn.rotVel.y = 25.0f - math_randomFloat() * 50.0f;
            particle = particle_newModelParticle(&spawn, 3, 0xFF, 4, 0xFF, 0);
            if (particle != NULL) {
                particle->floorY = actor->floorY;
            }
        }
    }
}
