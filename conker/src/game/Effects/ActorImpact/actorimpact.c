#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/weapon.h"



/* The debris variant and scale of each of effect_spawnActorImpactDebris's bursts. */
static const s32 gActorImpactDebrisKindsA[] = { 0xE2 };
static const f32 gActorImpactDebrisScalesA[] = { 1.0f };
static const s32 gActorImpactDebrisKindsB[] = { 0x39 };
static const f32 gActorImpactDebrisScalesB[] = { 1.0f };

/*
 * Spawns effects at actor's position, aimed by math_getActorRelFacingBin(other): actor id 0xA6 gets weapon_spawnImpactBurst
 * 50 units above it and one particle_spawnDebrisBurst burst; any other actor gets a different burst plus two
 * particle_spawnSplashSprayAtAngles passes (unk3C = 0, then 1) using the collision_initHitFromObject info for actor.
 */
void effect_spawnActorImpactDebris(Actor *actor, Actor *other, u8 arg2, s32 arg3) {
    Vec3f pos;
    s8 angle;
    CollisionHit floor;

    pos.x = actor->xpos;
    pos.y = actor->ypos;
    pos.z = actor->zpos;
    angle = math_getActorRelFacingBin(other) + 0x40;
    collision_initHitFromObject(&floor, actor);
    if (actor->character_type == 0xA6) {
        Vec3f raisedPos;
        DebrisBurst burst;

        raisedPos.x = pos.x;
        raisedPos.y = pos.y + 50.0f;
        raisedPos.z = pos.z;
        weapon_spawnImpactBurst(&raisedPos, arg2, arg3);
        burst.countBase = 10;
        burst.countRange = 10;
        burst.pos = pos;
        burst.speedBase = 7.0f;
        burst.speedRange = 17.0f;
        burst.angles.yawBase = angle - 40;
        burst.angles.yawRange = 80;
        burst.angles.pitchBase = -45;
        burst.angles.pitchRange = 26;
        burst.lifeBase = 25;
        burst.lifeRange = 25;
        burst.gravityBase = -0.8000000119f;
        burst.gravityRange = 0.3440000117f;
        burst.scaleBase = 0.7070000172f;
        burst.scaleRange = 0.5070000291f;
        burst.spin = 26.41100121f;
        particle_spawnDebrisBurst(&burst, gActorImpactDebrisKindsA, gActorImpactDebrisScalesA, 1, 0.0f, 0, arg2, arg3);
    } else {
        {
            DebrisBurst burst;

            burst.countBase = 10;
            burst.countRange = 10;
            burst.pos = pos;
            burst.speedBase = 7.0f;
            burst.speedRange = 17.0f;
            burst.angles.yawBase = angle - 40;
            burst.angles.yawRange = 80;
            burst.angles.pitchBase = -21;
            burst.angles.pitchRange = 13;
            burst.lifeBase = 25;
            burst.lifeRange = 25;
            burst.gravityBase = -0.8000000119f;
            burst.gravityRange = 0.3440000117f;
            burst.scaleBase = 0.04900000244f;
            burst.scaleRange = 0.1020000055f;
            burst.spin = 26.41100121f;
            particle_spawnDebrisBurst(&burst, gActorImpactDebrisKindsB, gActorImpactDebrisScalesB, 1, 0.0f, 0, arg2, arg3);
        }
        {
            SplashSpraySpec spray;

            spray.spray.pos = pos;
            spray.spray.scaleBase = 3.049999952f;
            spray.spray.countBase = 7;
            spray.spray.countRange = 3;
            spray.angles.yawBase = angle - 60;
            spray.angles.yawRange = 120;
            spray.angles.pitchBase = -30;
            spray.angles.pitchRange = 16;
            spray.spray.itemCountBase = 3;
            spray.spray.itemCountRange = 2;
            spray.spray.lifeBase = 20;
            spray.spray.lifeRange = 20;
            spray.spray.alphaBase = 155;
            spray.spray.alphaRange = 100;
            spray.spray.fadeStart = 16;
            spray.spray.fadeRate = 15;
            spray.spray.material = 0;
            spray.spray.kind = 0;
            spray.spray.scaleRange = 5.009999752f;
            spray.spray.sizeBase = 0.7000000477f;
            spray.spray.sizeRange = 0.5010000467f;
            spray.spray.speedBase = 15.10000038f;
            spray.spray.speedRange = 20.80000114f;
            spray.spray.burstChance = 0.5f;
            particle_spawnSplashSprayAtAngles(&spray.angles, &spray.spray, &floor, arg2, arg3);
            spray.spray.kind = 1;
            particle_spawnSplashSprayAtAngles(&spray.angles, &spray.spray, &floor, arg2, arg3);
        }
    }
}
