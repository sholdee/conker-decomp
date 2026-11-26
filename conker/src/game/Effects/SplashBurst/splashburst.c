#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// particle_spawnImpactMark (game/Particle/EmissionSource/emissionsource.c) is declared in each file that calls it: game/Effects/FollowEffects/followeffects.c and game/Sight/SightBeam/sightbeam.c were
// compiled with a different view of it.
void particle_spawnImpactMark(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind);


/* Spawns a burst of effects at obj: two sounds, a ground splash when there is floor below, and particles. */
void func_150E0870(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f origin;
    Vec3f impact;
    u8 hitGround;

    if (obj == NULL) {
        return;
    }
    if (gCutsceneStates[0] != 1) {
        sound_playActorUntracked(0x2BB, obj, 0x7FFF, 1000, 2000);
        sound_playActorUntracked(0x2BA, obj, 0x7FFF, 1000, 2000);
    }
    origin.x = obj->xpos;
    origin.y = obj->ypos + 20.0f;
    origin.z = obj->zpos;
    hitGround = FALSE;
    {
        Vec3f top;
        CollisionHit hit;

        top.x = origin.x;
        top.y = origin.y + 100.0f;
        top.z = origin.z;
        collision_initHitFromObject(&hit, obj);
        if (collision_probeFloorSurfABSet0Cached(&top, 0, origin.y - 200.0f, &hit) && (hit.flags & 1)) {
            impact.x = top.x;
            hitGround = TRUE;
            impact.y = hit.height;
            impact.z = top.z;
            particle_spawnImpactMark(math_randomFloat() * 125.0f + 204.0f, math_random() % 101U + 155, &hit.tri, &impact,
                          math_random() % 302U + 500, 0, 1, 0, 0, 0, arg1, 0);
        }
    }
    particle_startPlayerRumbles(&origin, 506.0f, 1013.0f, 0.0009871669f, 15, 20, arg1, arg2);
    particle_startPlayerCameraWobbles(&origin, 506.0f, 1013.0f, 0.0009871669f, 5, arg1, arg2);
    particle_spawnBlastSpray(&origin, arg1, arg2);
    particle_spawnLightFlash(&origin, arg1, arg2);
    particle_startRandomEmitter(hitGround ? &impact : &origin, math_random() % 62U + 120, arg1, arg2);
    particle_spawnGrowFade(&origin, math_randomFloat() * 4.0f + 12.0f, 1.6409999f, math_random() % 56U + 200, 0.0f, arg1,
                  arg2);
    blood_spawnHitEffects(obj, 1, 1, 1, 1, 0, arg1, arg2);
    particle_runAttackPairAt(&origin, 0, obj, obj, 0, 0x16, 0x15, 0);
}


/* Per-frame update for obj: steps its unk83 counter while unk87 is set, moves it, then stores its x/z displacement since last frame. */
void func_150E0BE0(Actor *obj) {
    s32 counter;

    obj->unkB0 = 15;
    obj->count2 = 10;

    if (obj->unk87 != 0) {
        counter = obj->count5;
        if (counter < 100) {
            counter = obj->count5 = counter + (u32)gFrameSteps;
            counter &= 0xFF; // matching: u32 add + explicit wrap orders the sb before the andi
        }
        if (counter >= 5) {
            counter = obj->unk251; // matching: reusing counter keeps both in v0
            if (counter != 3) {
                obj->fangle = movement_obj[0].angle;
                obj->scriptCmd = NULL;
                obj->intelwalk = 4;
                if (counter == 2) {
                    obj->intelwalk = 2;
                }
            }
        }
    } else {
        if (obj->count5 > 60) {
            gEventParam = 0xFF020133;
            gCurrentActor = movement_obj;
            func_1506E8D8();
            gCurrentActor = obj;
        }
        obj->count5 = 0;
    }

    if (obj->intelwalk == 4) {
        if (obj->count5 == 0) {
            obj->fangle = obj->angle;
        }
        actor_turnFacing(obj, 3, obj->fangle);
    }

    obj->unk87 = 0;
    if (!obj->stunned) {
        actor_updateChaseAi(obj);
    } else {
        actor_updateStunned(obj, 0.05f);
    }

    /* facing angle (0x10000 units per turn) rotated a quarter turn, in degrees */
    obj->theta = (s16)(obj->langle + 0x4000) * (360.0f / 0x10000);
    actor_updatePhysics(obj);
    obj->unkBC = obj->xpos - obj->old_xpos; // 0xBC is an f32 that structs.h still calls padBC
    obj->unk148 = obj->zpos - obj->old_zpos;
    if (actor_checkDeath(obj) == 0) {
        cutscene_applyObjectTrack(obj, 0, -1);
    }
}
