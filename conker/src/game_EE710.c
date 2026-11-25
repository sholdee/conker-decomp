#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Spawn template for func_15132A4C: the layout of Particle 0x10..0x8B (see game_15F680.c). */
typedef struct {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x5C */ s16 *unk5C;
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[6];
} ParticleSpawn; // size 0x7C

/* A live func_15132A4C particle (layout as Particle in game_15F680.c); only its extra data is used here. */
typedef struct {
    /* 0x000 */ u8 pad0[0x170];
    /* 0x170 */ f32 unk170; // extra data: the owner's unk180 at spawn time
} Particle;

void func_15142314(s32 bones, s32 bone, vertex *out); // bones: the owner's bone matrices (Mtx array)
void func_1514C2F0(f32 x, f32 y, f32 z, f32 radius, u8 startAngle, s8 angleStep, s16 count,
                   u8 kind, s32 arg8, f32 arg9, s32 arg10, u8 arg11);
void func_15165F80(s32 direction, s32 x, s32 y, s32 z, s32 growthRate, s32 life, s32 arg6, u8 arg7, s32 arg8);
Particle *func_15132A4C(ParticleSpawn *, s32, s32, s32, u8, s32);

/* Dust at one of actor's feet (foot 0 or 1: bone 13 or 17), on the ground height unk180: a func_1514C2F0
 * ring and a func_15165F80 quad under the foot, then 5-12 particles thrown outwards, sized by the actor's
 * mean scale. */
void func_150C1260(struct127 *actor, u8 foot) {
    vertex footPos;
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
    Particle *particle;

    size = (actor->xz_scale + actor->y_scale) * 0.5f;
    if (actor != NULL && actor->unk1D4 != NULL) {
        switch (foot) {
        case 0:
            bone = 13;
            break;
        case 1:
            bone = 17;
            break;
        }
        // matching: an integer cast keeps unk1D4 in v1 (a pointer argument is loaded straight into a0)
        func_15142314((s32)actor->unk1D4, bone, &footPos);
        radius = size * 80.0f;
        func_1514C2F0(footPos.x, actor->unk180, footPos.z, radius, 0, 16, 16, 0, 0, radius * 4.0f, 0, 0xFF);
        func_15165F80(-1, footPos.x, (s32)actor->unk180 + 2.0f, footPos.z, 11, 19, 0, 0xFF, 0);
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
        count = (func_150ADA20() & 7) + 5;
        while (count--) {
            angle = func_150ADA20() & 0xFF; // matching: the redundant mask on the u8 result is in the original
            speed = (func_150ADA68() * 0.8999999762f + 0.1000000015f) * 18.0f * size;
            cosA = func_151423D8(angle - 0x40);
            sinA = func_151423D8(angle);
            spawn.lifetime = (func_150ADA20() & 0x1F) + 15;
            spawn.pos.x = (radius * cosA) + footPos.x;
            spawn.pos.y = actor->unk180 + 2.0f;
            spawn.vel.y = speed;
            spawn.pos.z = (radius * sinA) + footPos.z;
            spawn.vel.x = speed * cosA;
            spawn.vel.z = speed * sinA;
            scale = (func_150ADA68() * 0.8999999762f + 0.1000000015f) * size * 2.230000019f;
            spawn.sizeXZ = scale;
            spawn.sizeY = scale;
            spawn.radius = scale;
            spawn.rot.x = func_150ADA68() * 360.0f;
            spawn.rot.y = func_150ADA68() * 360.0f;
            spawn.rot.z = func_150ADA68() * 360.0f;
            spawn.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
            spawn.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
            particle = func_15132A4C(&spawn, 3, 0xFF, 4, 0xFF, 0);
            if (particle != NULL) {
                particle->unk170 = actor->unk180;
            }
        }
    }
}
