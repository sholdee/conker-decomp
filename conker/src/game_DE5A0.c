#include <ultra64.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/math.h"



/* Remembers the object's x/z, and drags a swimming player who comes within range down and sideways around it. */
void func_150B10F0(Actor *actor) {
    f32 dist;
    f32 pushX;
    f32 pushZ;
    f32 unusedOut;
    f32 closeness;
    f32 distCopy; // matching: IDO keeps dist in a second FP register only through this copy
    f32 range;
    u16 angle;

    // 0x174/0x178 are f32 fields the shared header still declares as pads
    actor->unk174 = actor->xpos;
    actor->unk178 = actor->zpos;
    actor->imune = 100;
    actor->theta = (s16)(actor->langle + 0x4000) * 0.005493164f;
    if (actor_distanceXZ(movement_obj, actor) < 1100.0f) {
        dist = actor_distanceXZ(actor, movement_obj);
        distCopy = dist;
        range = actor->ypos - movement_obj[0].ypos; // matching: height above the player reuses `range`
        if (range < 0.0f) {
            range = 1000.0f;
        } else {
            range = 1050.0f - range * 0.7f;
        }
        if (dist < range && movement_obj[0].inwater == 1) {
            closeness = 1100.0f - distCopy;
            angle = actor_getDirectionAngle(actor->unk174 - movement_obj[0].xpos, movement_obj[0].zpos - actor->unk178, 0);
            if (850.0f < closeness) {
                movement_obj[0].ypos -= (closeness - 820.0f) * 0.09f * gActorTimeStep;
            }
            actor_splitVelocity(angle - 0x3000, closeness * 0.045f + 5.0f, 0.0f, &pushX, &pushZ, &unusedOut);
            movement_obj[0].ob_slopex = pushX;
            movement_obj[0].ob_slopey = pushZ;
        }
    }
    actor_playAnim(actor, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

/* Holds the object at its path's centre height and eases unkC4 toward its turn rate (clamped to +-6). */
void func_150B12FC(Actor *actor) {
    s16 turn;
    s16 prevAngle;
    f32 leanStep;

    prevAngle = actor->angle;
    actor->count2 = 10;
    {
        f32 height;

        height = gSpawnPaths[actor->listpos][0].y;
        actor_setHoverGravity(actor, height, 1.0f, height, -0.7f, 12.0f, -16.0f);
    }
    actor_updateChaseAi(actor);
    prevAngle -= actor->angle;
    turn = prevAngle * 6;
    leanStep = turn * 0.00390625f - actor->lean_angle;
    if (leanStep > 6.0f) {
        leanStep = 6.0f;
    } else if (leanStep < -6.0f) {
        leanStep = -6.0f;
    }
    {
        f32 current = actor->unk148;

        actor->unk148 += (leanStep - current) * 0.05f;
        actor->lean_angle += actor->unk148;
    }
    actor->theta = (s16)(actor->langle + 0x4000) * 0.005493164f;
    actor_updatePhysics(actor);
    if (actor_checkDeath(actor) == 0) {
        cutscene_applyObjectTrack(actor, 0, -1);
    }
}

/*
 * Moves the object vertically toward a target height: its spawn record's y + 210 by default, or (unk223 == 14)
 * straight at its target object movement_obj[unk222]. A nonzero unk224 overrides the height: 10000 means its
 * current path waypoint, anything else is unk224 * 8 (plus unk180 for most ids). Stunned objects only recover.
 */
void func_150B1484(Actor *actor) {
    s16 prevAngle;

    prevAngle = actor->angle;
    actor->count2 = 10;
    if (actor->stunned == 0) {
        f32 targetY;

        targetY = gSpawnRecords[actor->listpos].y + 210.0f;
        if (!(actor->unk22C & 8)) {
            if (actor->unk223 != 3) {
                actor->vtilt = actor->yspd * -0.5f;
            } else {
                actor->vtilt = 0.0f;
            }
        }
        if (actor->unk223 == 14) {
            f32 time;

            time = actor_distanceXZ(&movement_obj[actor->unk222], actor) / actor->speed;
            actor->yspd = (movement_obj[actor->unk222].ypos + actor->unk224 - actor->ypos) / time;
            actor->ygrav = 0.0f;
        } else {
            s16 heightCode;

            if (actor->unk223 == 1) {
                targetY = movement_obj[0].ypos + 210.0f;
            }
            heightCode = actor->unk224;
            if (heightCode != 0) {
                if (heightCode == 10000) {
                    targetY = gSpawnPaths[actor->listpos][actor->unk21E + 1].y;
                } else if ((actor->character_type == 0x50) || (actor->character_type == 5) || (actor->character_type == 0xAD) || (actor->character_type == 0xAE) || (actor->character_type == 0xAF)) {
                    targetY = heightCode * 8;
                } else {
                    targetY = heightCode * 8 + actor->floorY;
                }
            }
            if (!(actor->unk100 & 0x40)) {
                f32 gravity;
                f32 maxSpeed;

                if (actor->character_type == 0x44) {
                    targetY = gSpawnPaths[actor->listpos][actor->unk21E + 1].y;
                    gravity = 3.0f;
                    maxSpeed = 12.0f;
                } else {
                    gravity = 5.0f;
                    maxSpeed = 35.0f;
                }
                actor_setHoverGravity(actor, targetY, gravity, targetY, -gravity, maxSpeed, -maxSpeed);
            }
        }
        actor_updateChaseAi(actor);
    } else {
        actor_updateStunned(actor, 0.05f);
    }
    actor->theta = (s16)(actor->langle + 0x4000) * 0.0054931640625f;
    actor_updatePhysics(actor);
    if ((actor->character_type == 5) || (actor->character_type == 0xAD) || (actor->character_type == 0xAE) || (actor->character_type == 0xAF) || (actor->character_type == 0x44)) {
        actor_leanIntoTurn(actor, prevAngle);
    }
    if (actor_checkDeath(actor) == 0) {
        cutscene_applyObjectTrack(actor, 0, -1);
    }
}

/*
 * Pairs the object with the first actor in state 10 (`other`). With unk1E5 == 2 the object sits on top of
 * `other`; otherwise `other` hangs 170 below the object, swaying (unkB8) with the phase unk7E and spinning
 * with the frame step gFrameSteps. Either way the object then faces the player.
 */
void func_150B17DC(Actor *actor) {
    s32 otherIdx;
    f32 phase;
    f32 offsetX;
    f32 offsetZ;
    f32 unusedOut;
    Actor *other;

    other = actor_findByState(10, &otherIdx);
    actor_updateChaseAi(actor);
    if (actor->unk1E5 == 2) {
        actor->xpos = other->xpos;
        actor->zpos = other->zpos;
        actor->ypos = other->ypos + (actor->unk21C / 5 + 160.0f);
        actor->yspd = 0.0f;
        actor->ygrav = -0.35f;
        actor->unk7E = 0;
    } else {
        phase = actor->unk7E;
        phase *= 0.2f;
        other->vtilt = math_sin(phase) * 6.0f;
        actor_applyGravity(actor);
        other->ypos = actor->ypos - 170.0f;
        other->lifting = dc_glbl_CharNum + 100;
        actor_splitVelocity(other->langle, other->vtilt * -7.0f, 0.0f, &offsetX, &offsetZ, &unusedOut);
        other->xpos = actor->xpos + offsetX;
        other->zpos = actor->zpos + offsetZ;
        other->langle += gFrameSteps << 8;
        actor->unk7E += gFrameSteps;
        other->inwater = 0;
        other->speed = 0.0f;
        other->ob_overslopex = 0.0f;
        other->ob_overslopey = 0.0f;
    }
    actor->langle = actor_getDirectionAngle(movement_obj[0].xpos - actor->xpos, actor->zpos - movement_obj[0].zpos, 0);
    actor->theta = (s16)(actor->langle + 0x4000) * 0.0054931640625f;
}

/*
 * Keeps the object stunned with fixed state flags, eases unkB8 toward 0 (or -20 when unk13D is set, unk21C >= 9
 * and its host (movement_obj[carried - 100]) not stunned or gActorClock bit 2 set), and
 * now and then emits a particle at a random point around it that drifts with the player's motion.
 */
void func_150B19E0(Actor *actor) {
    f32 target;
    f32 radius;
    f32 speed;
    f32 sine;
    f32 cosine;
    BillboardTemplate particle;
    u32 random; // reused for both random draws
    u8 angle;

    target = 0.0f;
    actor->unkB0 = 25;
    actor->noshove &= ~4;
    actor->lifting = 0;
    actor->noshove |= 0x80;
    if (actor->speed > 1.0f) {
        actor->noshove |= 4;
    }
    actor->stunned = 10;
    actor->intelhitroute = 3;
    actor->unkFC = 0xA7;
    actor->count2 = 1;
    if (actor->task_attrib & 0x40) {
        actor->task_attrib &= ~0x40;
        actor->old_xpos = movement_obj[0].xpos;
        actor->old_zpos = movement_obj[0].zpos;
    }
    if (actor->carried != 0) {
        actor_moveToHost(actor);
        if ((movement_obj[actor->carried - 100].stunned == 0 || (gActorClock & 4)) && actor->unk21C >= 9) {
            target = -20.0f;
        }
    }
    actor->vtilt += (target - actor->vtilt) * 0.2f;
    actor->theta = (s16)(actor->langle + 0x4000) * 0.0054931640625f;
    actor_updatePhysics(actor);
    actor_checkGrab(actor);
    actor_playAnim(actor, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    cutscene_applyObjectTrack(actor, 0, -1);
    if (actor->carried != 0 && (u8)math_random() < 40U) {
        random = math_random() % 40U;
        radius = actor->scale_xz * random;
        angle = math_random();
        sine = math_cosBinAngle(angle);
        cosine = math_sinBinAngle(angle);
        particle.frame = 0;
        particle.frameStep = 0;
        particle.anim = gEffectAnimTextures[37];
        particle.x = actor->xpos + sine * radius;
        particle.y = actor->ypos;
        particle.z = actor->zpos + cosine * radius;
        collision_selectMap(0);
        particle.unk4 = collision_findMeshFloorNear(particle.x, particle.y, particle.z, 0);
        angle = movement_obj[0].angle >> 8;
        speed = movement_obj[0].speed * 64.0f;
        particle.velX = math_cosBinAngle(angle) * speed;
        particle.velZ = math_sinBinAngle(angle) * -speed;
        particle.unk1A = -30;
        particle.callback = 7;
        particle.velY = 0;
        particle.accelY = -80;
        random = (u8)math_random();
        particle.unk2B = random;
        particle.width = (s32)(random << 12) / 5100 + 204;
        particle.height = (s32)(random << 12) / 12750 + 81;
        particle.life = 400;
        particle.fadeStep = 0;
        particle.primR = 0xFF;
        particle.primG = 0xFF;
        particle.primB = 0;
        particle.primA = 0xFF;
        particle.flags = 0;
        effect_spawnBillboard(&particle, 0, 0, -1, 0xFF, 0);
    }
}
