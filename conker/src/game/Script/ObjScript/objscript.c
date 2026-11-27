#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals (see init/Sched/SchedInit/schedinit.c). */
u8 gScriptArg0;      // the argument bytes of the current command, arg0..arg3
u8 gScriptArg1;
u8 gScriptArg2;
u8 gScriptArg3;
f32 gScriptSpeedups[2];

#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/event.h"
#include "conker/face.h"
#include "conker/flamethrower.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/model.h"
#include "conker/mp.h"
#include "conker/mp3.h"
#include "conker/music.h"
#include "conker/particle.h"
#include "conker/path.h"
#include "conker/script.h"
#include "conker/sound.h"
#include "conker/zone.h"

/*
 * Object script interpreter commands.
 *
 * Each object runs a byte-code script (Actor.unk218 points at its next command). A command
 * is five bytes: an opcode (< 247, an index into gObjScriptHandlers) and four argument bytes, which the
 * interpreter copies to gScriptArg0..gScriptArg3 before calling the handler. Bytes >= 247 are
 * one-byte jump labels.
 * The handlers below act on the current object, gCurrentActor (index dc_glbl_CharNum in movement_obj);
 * their comments call the four argument bytes arg0..arg3. An object's "target" is the object
 * movement_obj[targetIndex], its current waypoint is waypoint of its path gSpawnPaths[unk13F].
 */



/* Jumps the script: skips `target` commands, or for target >= 247 skips forward to label `target`. */
void script_jump(s32 target) {
    u8 label;
    u8 i;

    if (target < 247) {
        gCurrentActor->scriptCmd += target;
        return;
    }

    for (i = 0; i < 100; i++) {
        if (gCurrentActor->scriptCmd->op < 247) {
            gCurrentActor->scriptCmd++;
        } else {
            label = gCurrentActor->scriptCmd->op;
            gCurrentActor->scriptCmd = (const ObjScriptCmd *)((const u8 *)gCurrentActor->scriptCmd + 1);
            if (target == label) {
                // the interpreter steps past the current command after this returns
                gCurrentActor->scriptCmd--;
                return;
            }
        }
    }
}

/* Applies the common movement flags of a move command. */
void script_applyMoveFlags(void) {
    s32 value = gScriptArg3 & 0x7F;

    if (value != 0x7F) {
        gCurrentActor->intel_animseq = value;
    }
    gCurrentActor->mvmt_control &= ~0x143E;
    if (gScriptArg0 == 0xFA) {
        gCurrentActor->mvmt_control |= 0x22;
    } else if (gScriptArg0 == 0xFB) {
        gCurrentActor->mvmt_control |= 4;
    }
    if (gScriptArg3 & 0x80) {
        gCurrentActor->mvmt_control |= 0x10;
    }
}

/* Starts a move: mode 0, duration arg0 * 100, speed arg2 (0xFF keeps the current speed). */
void script_startMove(void) {
    f32 speed;
    u8 speedArg;

    gCurrentActor->moveMode = 0;
    gCurrentActor->scriptTimer = gScriptArg0 * 100;
    speedArg = gScriptArg2;
    if (speedArg != 0xFF) {
        speed = speedArg;
        gCurrentActor->stickspeed = speed;
        if (gCurrentActor->stickspeed == 1.0f) {
            gCurrentActor->stickspeed = 0.5f;
        }
        if (gIsMultiplayer != 0) {
            if ((gCurrentActor->character_type == 40) && (gCurrentActor->mv->unk128 & 1)) {
                gCurrentActor->stickspeed *= 1.4f;
            }
        }
    }
    script_applyMoveFlags();
}

/* Picks the next waypoint: random (arg1 0xFF), random but not the previous one (0xFE), or arg1. */
void script_pickWaypoint(void) {
    u8 range;
    u8 index;
    u8 previous;
    Actor *obj;

    index = gSpawnPathCounts[gCurrentActor->listpos] - 1;
    if (gCurrentActor->waypointEnd != 0) {
        index = gCurrentActor->waypointEnd;
    }
    range = index - gCurrentActor->waypointStart;
    if (gScriptArg1 == 0xFF) {
        if (gCurrentActor->moveMode != 13) {
            gCurrentActor->waypoint = (math_randomU8() % (u32)range) + gCurrentActor->waypointStart;
        }
    } else if (gScriptArg1 == 0xFE) {
        if (gCurrentActor->moveMode != 13) {
            previous = gCurrentActor->waypoint - 1;
            if (gCurrentActor->waypoint <= gCurrentActor->waypointStart) {
                previous = index - 1;
            }
            index = (math_randomU8() % (u32)range) + gCurrentActor->waypointStart;
            if (index != previous) {
                gCurrentActor->waypoint = index;
                // matching: the copy, the `* 0` and the jump steer IDO to duplicate this store into v1
                obj = gCurrentActor;
                obj->moveMode = gCurrentActor->waypointStart * 0;
                goto set_timer;
            }
        }
    } else if (gScriptArg1 == 0xFF) {
        gCurrentActor->waypoint = index - 1;
    } else {
        gCurrentActor->waypoint = gScriptArg1;
    }
    gCurrentActor->moveMode = 0;
set_timer:
    gCurrentActor->scriptTimer = gScriptArg0 * 100;

    if (gScriptArg2 != 0xFF) {
        gCurrentActor->stickspeed = gScriptArg2;
        if (gCurrentActor->stickspeed == 1.0f) {
            gCurrentActor->stickspeed = 0.5f;
        }
    }
    if (gCurrentActor->waypoint < gCurrentActor->waypointStart) {
        gCurrentActor->waypoint = gCurrentActor->waypointStart;
    }
    script_applyMoveFlags();
}

/* Starts a move towards the current waypoint, arriving in arg1 ticks. */
void script_startTimedMove(void) {
    Actor *obj;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 speed;

    script_startMove();
    obj = gCurrentActor;
    dx = gSpawnPaths[obj->listpos][obj->waypoint + 1].x - obj->xpos;
    dz = gSpawnPaths[obj->listpos][obj->waypoint + 1].z - obj->zpos;
    dist = sqrtf((dx * dx) + (dz * dz));
    dx = (f32)(u32)gScriptArg1;
    speed = dist / dx;
    obj->stickspeed = speed + speed;
}

/* Jumps to arg0 when waypoint arg1 compares (per arg2) with the waypoint waypointStep steps back. */
void script_jumpIfWaypointCompare(void) {
    s32 position;
    s32 desired;
    s32 matched;

    matched = 0;
    position = gCurrentActor->waypoint - (s8)gCurrentActor->waypointStep;
    if (position < 0) {
        position += gSpawnPathCounts[gCurrentActor->listpos] - 1;
    } else if (position >= gSpawnPathCounts[gCurrentActor->listpos] - 1) {
        position -= gSpawnPathCounts[gCurrentActor->listpos] - 1;
    }
    desired = gScriptArg1 == 0xFF ? gSpawnPathCounts[gCurrentActor->listpos] - 2 : gScriptArg1;
    if (gScriptArg2 == 0) {
        if (desired == position) {
            matched = 1;
        }
    } else if (gScriptArg2 == 1) {
        if (desired != position) {
            matched = 1;
        }
    } else if (gScriptArg2 == 2) {
        if (desired < position) {
            matched = 1;
        }
    } else if (position < desired) {
        matched = 1;
    }
    if (matched) {
        script_jump(gScriptArg0);
    }
}

/* Jumps to arg0 when the current waypoint is (arg2 == 0) or is not (arg2 == 1) arg1. */
void script_jumpIfWaypoint(void) {
    u8 index = gCurrentActor->waypoint;

    if (((gScriptArg2 == 0) && (gScriptArg1 == index)) ||
        ((gScriptArg2 == 1) && (gScriptArg1 != index))) {
        script_jump(gScriptArg0);
    }
}

/* Starts a move, stopping at once when already within 40 units of path record arg1. */
void script_startMoveUnlessNear(void) {
    PathPoint *point;
    f32 dx;
    f32 dz;

    script_startMove();
    point = gSpawnPaths[gCurrentActor->listpos] + gScriptArg1;
    dx = point->x - gCurrentActor->xpos;
    dz = point->z - gCurrentActor->zpos;
    if (fabsf(dx) + fabsf(dz) < 40.0f) {
        gCurrentActor->scriptTimer = 0;
        gCurrentActor->speed = 0.0f;
    }
}

void script_startMoveWatchingTarget(void) {
    script_startMove();
    gCurrentActor->moveMode = 10;
}

void script_pickWaypointWatchingTarget(void) {
    script_pickWaypoint();
    gCurrentActor->moveMode = 10;
}

void func_15075BB8(void) {
    script_startMove();
    gCurrentActor->unk22C |= 2;
}

void script_setSpotScript(void) {
    if (gScriptArg3 != 0) {
        gCurrentActor->unk22C |= 0x80;
    }
    gCurrentActor->spotScript = gScriptArg0;
}

/* Sets the vertical velocity to the s16 (arg2:arg0) unless arg3 == 1, and gravity to (s8)arg1 unless arg3 == 2. */
void script_setYSpeedAndGravity(void) {
    if (gScriptArg3 != 1) {
        gCurrentActor->yspd = (s16)((gScriptArg2 << 8) | gScriptArg0);
    }
    if (gScriptArg3 != 2) {
        gCurrentActor->ygrav = (s8)gScriptArg1;
    }
}

void script_startChase(void) {
    script_startMove();
    gCurrentActor->moveMode = 1;
}

void script_startIntercept(void) {
    script_startMove();
    gCurrentActor->moveMode = 16;
    gCurrentActor->interceptSpeed = gScriptArg1;
}

void script_startChaseNoLeash(void) {
    script_startMove();
    gCurrentActor->moveMode = 15;
}

void script_startChase3D(void) {
    script_startChase();
    gCurrentActor->moveMode = 14;
}

void script_startChasePlayer(void) {
    script_startMove();
    gCurrentActor->moveMode = 12;
    gCurrentActor->targetIndex = 0;
}

void script_startMoveToTarget(void) {
    script_startMove();
    gCurrentActor->moveMode = 9;
}

void script_startChaseAlt(void) {
    script_startChase();
}

/* Switches to script arg1 (0: the object's current script id); a nonzero arg3 becomes the new id. */
void script_switchScript(void) {
    if (gScriptArg1 == 0) {
        gScriptArg1 = gCurrentActor->intelwalk;
    }
    if (gScriptArg3 != 0) {
        gCurrentActor->intelwalk = gScriptArg3;
    }
    gCurrentActor->scriptCmd = script_getScript(0, gScriptArg1);
    // the interpreter steps past the current command after this returns
    gCurrentActor->scriptCmd--;
}

void script_startFlee(void) {
    script_startMove();
    gCurrentActor->moveMode = 2;
}

void script_setSpotWideScript(void) {
    gCurrentActor->spotWideScript = gScriptArg0;
}

/* Jumps to arg0 with a chance of arg2 percent. */
void script_jumpRandom(void) {
    if ((math_randomU8() % 100U) < gScriptArg2) {
        script_jump(gScriptArg0);
    }
}

void script_setNearScript(void) {
    gCurrentActor->nearScript = gScriptArg0;
    gCurrentActor->nearRange = gScriptArg1;
    gCurrentActor->nearCone = gScriptArg2;
}

void script_startFollowTarget(void) {
    script_startMove();
    gCurrentActor->moveMode = 3;
}

/* Sets the vertical velocity (arg0, downwards if arg2), the gravity (arg1) and hopSpeed (arg3). */
void script_setYSpeedGravityAndHop(void) {
    if (gScriptArg0 != 0) {
        if (gScriptArg2 == 0) {
            gCurrentActor->yspd = gScriptArg0;
        } else {
            gCurrentActor->yspd = -gScriptArg0;
        }
    }
    if (gScriptArg1 != 0) {
        gCurrentActor->ygrav = gScriptArg1;
    }
    gCurrentActor->hopSpeed = gScriptArg3;
}

/* Starts a jump towards waypoint arg1, timing the flight so that it lands there. */
void script_leapToWaypoint(void) {
    f32 dx;
    f32 dz;
    f32 time;

    script_startMove();
    gCurrentActor->waypoint = gScriptArg1;
    dx = gSpawnPaths[gCurrentActor->listpos][gScriptArg1 + 1].x - gCurrentActor->xpos;
    dz = gSpawnPaths[gCurrentActor->listpos][gScriptArg1 + 1].z - gCurrentActor->zpos;
    time = 2.0f * (sqrtf((dx * dx) + (dz * dz)) / gCurrentActor->stickspeed);
    if (time < 12.0f) {
        gCurrentActor->stickspeed *= time * 0.08333300054f;
        time = 12.0f;
    }
    gCurrentActor->scriptTimer = (u32)(time * 100.0f);
    gCurrentActor->yspd = gCurrentActor->ygrav * time * 0.5f;
    gCurrentActor->moveMode = 4;
}

void script_startOrbitWaypoint(void) {
    script_pickWaypoint();
    gCurrentActor->moveMode = 5;
}

void func_150761F4(void) {
    gCurrentActor->moveMode = 6;
    gCurrentActor->unk22C &= ~1;
}

void script_startHoldTargetOffset(void) {
    func_150761F4();
    gCurrentActor->unk22C |= 1;
}

void script_playPlayerSound(void) {
    s32 id = (gScriptArg0 << 8) | gScriptArg1;

    actor_playPlayerSound(id, gCurrentActor, 32000, -100, 500, 2500, gScriptArg2);
}

void script_fadeFirstTwoChannels(void) {
    music_fadeFirstTwoChannels(gScriptArg0);
}

void script_spawnPuff(void) {
    particle_spawnPuffQuad(gCurrentActor->xpos, gCurrentActor->ypos + 100.0f,
                  gCurrentActor->zpos, 0.45f, 0xFF, 0);
}

void script_jumpIfNotHit(void) {
    if (gCurrentActor->ivehitit == 0) {
        script_jump(gScriptArg0);
    }
    if (gScriptArg1 != 0) {
        gCurrentActor->ivehitit = 0;
    }
}

void script_setNearRange(void) {
    gCurrentActor->nearRange = gScriptArg0;
}

void script_setTurnRates(void) {
    if (gScriptArg0 == 128) {
        gCurrentActor->moveTurnRate = gCurrentActor->listpos_ptr->turnRate;
    } else {
        gCurrentActor->moveTurnRate = gScriptArg0;
    }
    gCurrentActor->moveTurnFrac = 0;

    if (gScriptArg2 != 0) {
        gCurrentActor->moveTurnRate += math_randomU8() % (u32)gScriptArg2;
    }
    if (gScriptArg3 == 1) {
        gCurrentActor->faceTurnRate = 0;
        gCurrentActor->faceTurnFrac = 0;
        return;
    }
    if (gScriptArg3 == 2) {
        gCurrentActor->faceTurnRate = gScriptArg1;
        gCurrentActor->faceTurnFrac = 0;
        return;
    }
    if (gScriptArg1 == 0) {
        gCurrentActor->faceTurnRate = gCurrentActor->moveTurnRate;
        gCurrentActor->faceTurnFrac = 0;
    }
}

void script_setStunEndScript(void) {
    gCurrentActor->stunEndScript = gScriptArg0;
}

void script_setHitRoute(void) {
    gCurrentActor->intelhitroute = gScriptArg0;
}

void script_setAlertCooldown(void) {
    gCurrentActor->alertCooldown = (gScriptArg0 << 8) | gScriptArg1;
}

/* Sets `immune` to arg0 on the object's target (arg1 != 0) or on the object itself. */
void script_setImmune(void) {
    if (gScriptArg1 != 0) {
        movement_obj[gCurrentActor->owner].imune = gScriptArg0;
    } else {
        gCurrentActor->imune = gScriptArg0;
    }
}

void func_1507659C(void) {
    gCurrentActor->ypos = gCurrentActor->ypos + gScriptArg0 * 100;
    if (gScriptArg3 != 0) {
        gCurrentActor->ypos = 1800.0f;
    }
}

void script_emptyCommand61(void) {
}

void script_setTurnStopScript(void) {
    gCurrentActor->turnStopScript = gScriptArg0;
}

/* Sets the flags given by the four argument bytes in unkF8. */
void script_setNoshoveFlags(void) {
    s32 flags = (gScriptArg0 << 24) | (gScriptArg1 << 16) | (gScriptArg2 << 8) | gScriptArg3;

    gCurrentActor->noshove |= flags;
}

/* Clears the flags given by the four argument bytes in unkF8. */
void script_clearNoshoveFlags(void) {
    s32 flags = (gScriptArg0 << 24) | (gScriptArg1 << 16) | (gScriptArg2 << 8) | gScriptArg3;

    gCurrentActor->noshove &= ~flags;
}

void script_waitUntilNearGround(void) {
    if (((gCurrentActor->shad > 0.0f) && (gScriptArg0 == 0)) ||
        ((gScriptArg0 * 50 < gCurrentActor->shad) && (gScriptArg0 != 0))) {
        gCurrentActor->scriptTimer = 99;
        gCurrentActor->scriptCmd--;
    }
}

void script_emptyCommand70(void) {
}

void func_15076768(void) {
    Vec3f position;
    CollisionHit floor;

    switch (gScriptArg0) {
    case 0:
        flamethrower_stopJets(gCurrentActor);
        break;
    case 1:
        position.x = gCurrentActor->xpos;
        position.y = -390.0f;
        position.z = gCurrentActor->zpos;
        // matching: the jump ends the basic block, so as1 keeps the stores before the call
        goto setup_done;
    setup_done:
        collision_initHitFromObject(&floor, gCurrentActor);
        particle_spawnGroundCluster(&position, &floor, 0xFF, 0);
        break;
    }
}

/* Jumps to arg0 when the direction from the object to its target is within arg1 of the target's facing. */
void script_jumpIfBehindTarget(void) {
    Actor *target;
    u16 angle;

    target = &movement_obj[gCurrentActor->targetIndex];
    // matching: the (s32) cast before narrowing to u16 is needed
    angle = (s32)actor_getDirectionAngle(target->xpos - gCurrentActor->xpos,
                               gCurrentActor->zpos - target->zpos, 0);
    if ((((angle >> 8) - (movement_obj[gCurrentActor->targetIndex].langle >> 8) + gScriptArg1) & 0xFF) < gScriptArg1 * 2) {
        script_jump(gScriptArg0);
    }
}

/*
 * Picks the nearest object (flag 0x20 in unkF8) within arg0 * 8 units that lies inside the object's
 * view cone, storing its index | 0x80 in unkA8. If one is found, the script jumps to label arg3 and,
 * for a nonzero arg2, the found object starts script arg2 targeting this object.
 */
void script_findNearestInView(void) {
    s32 viewAngle;
    f32 dist;
    s32 self;
    Actor *found;
    Actor *obj;
    f32 nearest;
    s32 i;
    u8 script;
    ModelDef *type;
    u8 angle;
    u8 foundIndex;

    nearest = gScriptArg0 * 8;
    gCurrentActor->unkA8 = 0;
    self = gCurrentActor - movement_obj;

    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if ((obj->type != 0) && (obj->energy != 0) && (obj->noshove & 0x20) && (i != self)) {
            if ((gScriptArg1 != 0) || (obj->character_type != gCurrentActor->character_type)) {
                if (fabsf(gCurrentActor->ypos - obj->ypos) > 50.0f) {
                    continue;
                }
                dist = actor_distance3D(gCurrentActor, obj);
                if (dist < nearest) {
                    type = gModelDefs[gCurrentActor->character_type];
                    viewAngle = type->viewAngle;
                    angle = actor_getDirectionAngle(obj->xpos - gCurrentActor->xpos,
                                          gCurrentActor->zpos - obj->zpos, 0) >> 8;
                    if (((((gCurrentActor->angle >> 8) + gCurrentActor->lookYaw) - angle + (viewAngle / 2)) & 0xFF) < viewAngle) {
                        gCurrentActor->unkA8 = i + 0x80;
                        nearest = dist;
                    }
                }
            }
        }
    }

    if (gCurrentActor->unkA8 != 0) {
        gCurrentActor->scriptCmd = script_getScript(0, gScriptArg3);
        gCurrentActor->scriptCmd--;
        script = gScriptArg2;
        if (script != 0) {
            foundIndex = gCurrentActor->unkA8;
            found = &movement_obj[foundIndex & 0x7F];
            found->scriptCmd = NULL;
            found->intelwalk = script;
            found->targetIndex = self;
        }
    }
}

void script_targetFoundObject(void) {
    gCurrentActor->targetIndex = gCurrentActor->unkA8 & 0x7F;
}

void script_setDrawType(void) {
    gCurrentActor->drawtype = gScriptArg0;
}

/* Targets the last object below this one (all 25 if arg1) of type arg0 that is alive, and starts mode 9. */
void script_moveToLastOfType(void) {
    s32 i = gCurrentActor - movement_obj;

    if (gScriptArg1) {
        i = 25;
    }
    while (i != 0) {
        if ((gScriptArg0 == movement_obj[i - 1].character_type) && (movement_obj[i - 1].energy != 0)) {
            script_startMove();
            gCurrentActor->moveMode = 9;
            gCurrentActor->targetIndex = i - 1;
            gCurrentActor->scriptTimer = 1000;
            return;
        }
        i--;
    }
}

void script_setAlpha(void) {
    actor_fadeCommand(gCurrentActor, 1, gScriptArg0, 0, 0);
}

void script_setAngleFromFangle(void) {
    gCurrentActor->angle = gCurrentActor->fangle;
}

void script_removeSelf(void) {
    gCurrentActor->scriptTimer = 10000;
    actor_remove(gCurrentActor, 1);
}

void script_setSpeed(void) {
    gCurrentActor->speed = gScriptArg0;
}

/* Sets the object's scale from two s16 percentages, (arg1:arg0) horizontal and (arg3:arg2) vertical. */
void script_setScale(void) {
    f32 scale = 0.01f;
    s16 xz = gScriptArg0 | (gScriptArg1 << 8);
    s16 y = gScriptArg2 | (gScriptArg3 << 8);

    gCurrentActor->scale_xz = xz * scale;
    gCurrentActor->scale_y = y * scale;
    gCurrentActor->targetScaleXz = gCurrentActor->scale_xz;
    gCurrentActor->targetScaleY = gCurrentActor->scale_y;
    actor_setScale(gCurrentActor, gCurrentActor->scale_xz, gCurrentActor->scale_y);
}

void script_setType(void) {
    gCurrentActor->type = gScriptArg0;
}

void func_15076E10(void) {
    gCurrentActor->unk1D0 = gScriptArg0;
    gCurrentActor->unk22C |= gScriptArg1;
}

void script_emptyCommand113(void) {
}

/* Starts mode 7: a jump in a random direction with random speeds. */
void script_startRandomJump(void) {
    script_startMove();
    gCurrentActor->yspd = (math_randomU8() % 20U) + 55.0f;
    gCurrentActor->ygrav = 2.0f;
    gCurrentActor->fangle = math_randomU8() % 0xFFFFU;
    gCurrentActor->speed = (math_randomU8() % 20U) + 15;
    gCurrentActor->stickspeed = gCurrentActor->speed;
    gCurrentActor->moveMode = 7;
}

void script_removeSelfAlt(void) {
    gCurrentActor->scriptTimer = 1000;
    actor_remove(gCurrentActor, 1);
}

void script_setHeightOffset(void) {
    s8 value;

    value = gScriptArg0;
    gCurrentActor->heightOffset = value;
    value = gScriptArg1;
    gCurrentActor->targetHeightOffset = value;
}

/* arg3 == 0: moves the object by a random offset of up to +-arg0/2 (x) and +-arg1/2 (z), times 3;
 * otherwise raises its speed to at least arg3. */
void func_15076FA8(void) {
    if (gScriptArg3 == 0) {
        s16 offset;

        offset = (math_randomU8() % (u32)gScriptArg0) - (gScriptArg0 / 2);
        gCurrentActor->xpos += offset * 3.0f;
        offset = (math_randomU8() % (u32)gScriptArg1) - (gScriptArg1 / 2);
        gCurrentActor->zpos += offset * 3.0f;
    } else if (gCurrentActor->speed < gScriptArg3) {
        gCurrentActor->speed = gScriptArg3;
    }
}

/* Jumps to arg3 when the target's speed is below arg2. */
void script_jumpIfTargetSlow(void) {
    if (movement_obj[gCurrentActor->targetIndex].speed < gScriptArg2) {
        script_jump(gScriptArg3);
    }
}

void script_setDeathReaction(void) {
    gCurrentActor->deathReaction = gScriptArg0;
}

void script_playUntrackedSound(void) {
    s32 sound = (gScriptArg0 << 8) | gScriptArg1;

    if (sound != 0) {
        s32 volume = gScriptArg2 << 7;

        sound_playActorUntracked(sound, gCurrentActor, volume, 500, 2500);
    }
}

void script_runFadeCommand(void) {
    u8 fadeCmd;
    u8 value;
    u8 step;

    if (gScriptArg3 == 0) {
        value = gScriptArg0;
        step = gScriptArg1;
        if (gScriptArg2 != 0) {
            fadeCmd = 1;
        } else {
            fadeCmd = 2;
        }
        actor_fadeCommand(gCurrentActor, fadeCmd, value, step, 0);
    } else if (gScriptArg2 == 0) {
        actor_startFogFadeOut(gCurrentActor, gScriptArg0);
    } else {
        actor_startFogFadeIn(gCurrentActor, gScriptArg0);
    }
}

/* Sets the health to arg0 (arg1 == 0) or decrements it (arg1 == 1). */
void script_setEnergy(void) {
    switch (gScriptArg1) {
    case 0:
        gCurrentActor->energy = gScriptArg0;
        break;
    case 1:
        gCurrentActor->energy--;
        break;
    }
}

void script_breakIntoParts(void) {
    model_breakIntoParts(gCurrentActor, gScriptArg0, -1);
}

void script_setHidden(void) {
    if (gScriptArg0 != 0) {
        gCurrentActor->unk101 |= 1;
    } else {
        gCurrentActor->unk101 &= ~1;
    }
}

void script_doNothing(void) {
}

void script_setAnimSpeedAndAccel(void) {
    if (gScriptArg3 != 0) {
        gScriptArg0 += math_randomU8() % (u32)gScriptArg3;
    }
    gCurrentActor->intel_animspd = gScriptArg0;
    gCurrentActor->animSpeedFrac = 0;
    gCurrentActor->intel_accel = gScriptArg1;
    gCurrentActor->intel_decel = gScriptArg2;
}

/*
 * arg3 != 0: adds (s16)(arg1:arg2) * gFrameTicks to the 13-bit counter kept in unk246 (high bits) and
 * animSpeedFrac (low byte), clamping at 0 and setting bit 7 of unk246; otherwise sets unk246 to arg0.
 */
void func_15077404(void) {
    if (gScriptArg3 != 0) {
        s16 value;
        s16 delta;

        value = ((gCurrentActor->intel_animspd & 0x1F) << 8) + gCurrentActor->animSpeedFrac;
        delta = (gScriptArg1 << 8) + gScriptArg2;
        value += delta * gFrameTicks;
        if (value < 0) {
            value = 0;
        }
        gCurrentActor->intel_animspd = (value >> 8) | 0x80;
        gCurrentActor->animSpeedFrac = value;
    } else {
        gCurrentActor->intel_animspd = gScriptArg0;
    }
}

/* Jumps to arg0 when the object has reached its destination node (or has none). */
void script_jumpIfAtDestination(void) {
    if ((gCurrentActor->pathNode == gCurrentActor->pathGoal) || (gCurrentActor->pathGoal == 0xFF)) {
        gCurrentActor->task_attrib |= 0x800;
        script_jump(gScriptArg0);
    }
}

/*
 * Steps the object along the path-node graph: picks the next node (arg0 == 1: a random neighbour
 * other than the previous node, otherwise the next node towards the goal pathGoal) and stores a point
 * near it, offset sideways by up to twice the node's unk8, as waypoint 0 of the object's path.
 * Sets flag 0x800 in unk25C when the goal is reached or no node is left.
 */
void script_stepPathGraph(void) {
    s32 next;
    f32 x;
    f32 y;
    f32 z;
    s8 hasGoal;
    s8 hasAvoid;
    s32 i;
    s32 best;
    u8 *q;
    u8 current;
    s32 tmp;
    PathNode *node;
    f32 bestRoll;
    f32 roll;
    f32 pad; // matching: unused stack slot

    next = 0xFF;
    if (gScriptArg0 != 1) {
        if (gCurrentActor->pathNode == gCurrentActor->pathGoal) {
            gCurrentActor->pathDistance = 0;
            gCurrentActor->task_attrib |= 0x800;
            return;
        }
        if (gCurrentActor->pathGoal == 0xFF) {
            gCurrentActor->pathDistance = 0;
        }
    }
    gCurrentActor->task_attrib &= ~0x800;
    if (gSpawnPathCounts[gCurrentActor->listpos] - 1 > 0) {
        hasAvoid = 0;
        hasGoal = 0;
        if (gCurrentActor->pathGoal != 0xFF) {
            hasGoal = 1;
        }
        if (gCurrentActor->pathAvoid != 0xFF) {
            hasAvoid = 1;
        }
        current = gCurrentActor->pathNode;
        if (current == 0xFF) {
            if (gScriptArg0 == 1) {
                hasAvoid = 0;
                hasGoal = 0;
                next = gCurrentActor->prevPathNode;
            }
            if (next == 0xFF) {
                next = path_getNextNode(gCurrentActor->xpos, gCurrentActor->ypos, gCurrentActor->zpos,
                                     hasGoal, hasAvoid, &gCurrentActor->pathGoal, &gCurrentActor->pathAvoid);
            }
        } else if (current != 0xFF) {
            if (gScriptArg0 == 1) {
                best = 0xFF;
                node = &gPathNodes[current];
                if (gCurrentActor->prevPathNode != 0xFF) {
                    // matching: dead code standing in for something the original compiled away; it
                    // keeps IDO's branch on pad210 and its register assignment
                    tmp = 0; for (i = 0, q = (u8 *)gPathNodes; i < 4; i++, q++) { tmp += q[9] == gCurrentActor->prevPathNode; } tmp += (s32)node; tmp ^= (s32)node; tmp -= (s32)node; tmp |= (s32)node;
                }
                bestRoll = 10.0f;
                for (i = 0; i < 5; i++) {
                    if (node->links[i] != 0xFF && gCurrentActor->prevPathNode != node->links[i]) {
                        roll = math_randomFloat();
                        if (roll < bestRoll) {
                            bestRoll = roll;
                            best = node->links[i];
                        }
                    }
                }
                next = best;
                if (next == 0xFF) {
                    gCurrentActor->task_attrib |= 0x800;
                    next = gCurrentActor->prevPathNode;
                }
            } else {
                next = path_search(current, hasGoal, hasAvoid, &gCurrentActor->pathGoal, &gCurrentActor->pathAvoid);
            }
        }
        if (next != 0xFF) {
            gCurrentActor->waypoint = 0;
            if (mp_getNodePos(next, &x, &y, &z)) {
                gCurrentActor->pathDistance = gPathDistance;
                gCurrentActor->prevPathNode = gCurrentActor->pathNode;
                gCurrentActor->pathNode = next;
                {
                    f32 width;
                    f32 dx;
                    f32 dz;
                    f32 dist;

                    width = (f32)(u32)gPathNodes[next].unk8 + (f32)(u32)gPathNodes[next].unk8;
                    dx = x - gCurrentActor->xpos;
                    dz = z - gCurrentActor->zpos;
                    dist = sqrtf(dx * dx + dz * dz);
                    if (dist > 1.0f) {
                        dist = math_randomFloat() * width / dist;
                        x -= dz * dist;
                        z += dx * dist;
                    }
                }
                gSpawnPaths[gCurrentActor->listpos][1].x = x;
                gSpawnPaths[gCurrentActor->listpos][1].y = y;
                gSpawnPaths[gCurrentActor->listpos][1].z = z;
            }
        }
    }
}

/* Advances the waypoint index by waypointStep, wrapping around the path (or the first waypointEnd waypoints). */
void script_advanceWaypoint(void) {
    u8 count;

    count = gSpawnPathCounts[gCurrentActor->listpos] - 1;
    if (gCurrentActor->waypointEnd != 0) {
        count = gCurrentActor->waypointEnd;
    }
    gCurrentActor->waypoint += gCurrentActor->waypointStep;
    gCurrentActor->waypoint += count;
    gCurrentActor->waypoint %= count;
    if (gCurrentActor->waypoint < gCurrentActor->waypointStart) {
        gCurrentActor->waypoint = gCurrentActor->waypointStart;
    }
}

void script_pickWaypointNoAdvance(void) {
    script_pickWaypoint();
    gCurrentActor->moveMode = 11;
}

/* Jumps to arg0 when object 0 (or, with arg2, the target) is within arg3 * 8 units. */
void script_jumpIfWithinRange(void) {
    Actor *obj;
    u8 index;
    f32 range;

    index = 0;
    if (gScriptArg2 != 0) {
        index = gCurrentActor->targetIndex;
    }
    if (dc_glbl_CharNum != index) {
        obj = &movement_obj[index];
        if ((obj->type != 1) || (obj->attach_char == 0)) {
            // matching: the assignment inside the test makes IDO keep the bound in $f2
            if (actor_distanceXZ(gCurrentActor, obj) < (range = gScriptArg3 << 3)) {
                script_jump(gScriptArg0);
            }
        }
    }
}

void script_setHitRouteAlt(void) {
    gCurrentActor->intelhitroute = gScriptArg0;
}

void script_setOrbit(void) {
    gCurrentActor->orbitRadius = gScriptArg0;
    gCurrentActor->orbitStep = gScriptArg1;
    gCurrentActor->unk86 = math_randomU8() % 255U;
}

void script_setStandOnScript(void) {
    gCurrentActor->standOnScript = gScriptArg0;
    gCurrentActor->unk24D = gScriptArg1;
}

void script_playSoundOnSelf(void) {
    actor_playSound((gScriptArg0 << 8) + gScriptArg1, gCurrentActor);
}

void script_setTaskAttrib(void) {
    s32 flags = (gScriptArg0 << 8) + gScriptArg1;

    gCurrentActor->task_attrib |= flags;
}

/* Restarts the object returned by actor_findByType(arg1) on script arg0. */
void script_restartObjectOfType(void) {
    Actor *obj = actor_findByType(gScriptArg1);

    obj->scriptCmd = NULL;
    obj->intelwalk = gScriptArg0;
}

void script_clearTaskAttrib(void) {
    s32 mask = ~((gScriptArg0 << 8) + gScriptArg1);

    gCurrentActor->task_attrib &= mask;
}

void script_setSightRange(void) {
    gCurrentActor->sightRange = gScriptArg0;
}

/*
 * Targets an object that is alive, not immune (or 0xFF), unlinked (unless arg2) and either of
 * type arg0 or in state arg1 and not stunned: the first one found if arg3 == 0, else the nearest.
 */
void script_findTarget(void) {
    Actor *obj;
    f32 nearest;
    f32 dist;
    s32 i;

    nearest = 9999999.0f;
    gCurrentActor->targetIndex = 0;

    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if (((obj->type != 0) && (obj->energy != 0)) &&
            ((obj->imune == 0) || (obj->imune == 0xFF)) &&
            ((obj->attach_char == 0) || (gScriptArg2 != 0)) &&
            ((obj->character_type == gScriptArg0) || ((obj->type == gScriptArg1) && (obj->stunned == 0)))) {
            if (gScriptArg3 == 0) {
                gCurrentActor->targetIndex = i;
                return;
            }
            dist = actor_distance3D(gCurrentActor, obj);
            if (nearest > dist) {
                gCurrentActor->targetIndex = i;
                nearest = dist;
            }
        }
    }
}

void script_setWaypoint(void) {
    gCurrentActor->waypoint = gScriptArg0;
}

/* Moves the object onto waypoint arg0 (0xFA: the current waypoint). */
void script_warpToWaypoint(void) {
    if (gScriptArg0 != 0xFA) {
        gCurrentActor->waypoint = gScriptArg0;
    }
    gCurrentActor->xpos = gSpawnPaths[gCurrentActor->listpos][gCurrentActor->waypoint + 1].x;
    gCurrentActor->ypos = gSpawnPaths[gCurrentActor->listpos][gCurrentActor->waypoint + 1].y;
    gCurrentActor->zpos = gSpawnPaths[gCurrentActor->listpos][gCurrentActor->waypoint + 1].z;
}

void script_startTrackAt(void) {
    s32 id = (gScriptArg0 << 8) + gScriptArg1;

    mp3_startTrackAt(id, gCurrentActor, 24000, 500, 2500);
}

void script_setHitPlayerScript(void) {
    gCurrentActor->hitPlayerScript = gScriptArg0;
}

void script_setTurnAccel(void) {
    gCurrentActor->turnAccel = (gScriptArg0 << 8) | gScriptArg1;
}

void script_setTurnAnim(void) {
    gCurrentActor->turnAnimRow = gScriptArg0;
    gCurrentActor->turnAnimSpeed = gScriptArg1;
}

/* Turns the object towards the current waypoint, starting turn animation arg0 - 1 for large turns. */
void script_turnToWaypoint(void) {
    f32 dx;
    f32 dz;
    u8 turn;
    u8 index;

    index = gScriptArg0 - 1;
    dx = gSpawnPaths[gCurrentActor->listpos][gCurrentActor->waypoint + 1].x - gCurrentActor->xpos;
    dz = gCurrentActor->zpos - gSpawnPaths[gCurrentActor->listpos][gCurrentActor->waypoint + 1].z;
    gCurrentActor->fangle = actor_getDirectionAngle(dx, dz, 0);
    turn = (gCurrentActor->fangle - gCurrentActor->angle) >> 8;
    if (turn & 0x80) {
        turn = -turn;
    }
    if (gTurnAnimTable[index][0] < turn) {
        gCurrentActor->turnAnimSpeed = gScriptArg1;
        actor_startTurnAnim(gCurrentActor, turn, index);
    }
}

/* Jumps to arg0 when any other active object is within arg3 * 8 units. */
void script_jumpIfAnyObjectNear(void) {
    f32 range;
    Actor *obj;
    s32 i;

    range = 8 * gScriptArg3;

    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if ((obj->type != 0) && (dc_glbl_CharNum != i)) {
            if (actor_distanceXZ(gCurrentActor, obj) < range) {
                script_jump(gScriptArg0);
                return;
            }
        }
    }
}

/* Jumps to arg0 when the target is inactive. */
void script_jumpIfTargetInactive(void) {
    s32 index = gCurrentActor->targetIndex;

    if (movement_obj[index].type == 0) {
        script_jump(gScriptArg0);
    }
}

void script_setTypeSpot(void) {
    gCurrentActor->typeSpotRange = gScriptArg1;
    gCurrentActor->typeSpotKind = gScriptArg2;
    gCurrentActor->typeSpotScript = gScriptArg0;
    gCurrentActor->typeSpotReplyScript = gScriptArg3;
}

/* Jumps to arg0 when the object is closer (arg2 == 0) or farther (arg2 != 0) than arg3 * 8 from waypoint arg1. */
void script_jumpIfNearWaypoint(void) {
    PathPoint *point;
    f32 dx;
    f32 dz;
    f32 dist;
    s32 index;

    index = gScriptArg1 + 1;
    point = &gSpawnPaths[gCurrentActor->listpos][index];
    dx = point->x - gCurrentActor->xpos;
    dz = point->z - gCurrentActor->zpos;
    dist = sqrtf((dx * dx) + (dz * dz));
    dx = gScriptArg3 << 3;
    if (((gScriptArg2 == 0) && (dist < dx)) ||
        ((gScriptArg2 != 0) && (dist > dx))) {
        script_jump(gScriptArg0);
    }
}

void script_setArriveRange(void) {
    gCurrentActor->arriveRange = gScriptArg0;
}

/* Jumps to arg0 when the object's unique id is (arg2 == 0) or is not (arg2 == 1) arg1. */
void script_jumpIfUniqueId(void) {
    if (((gScriptArg2 == 0) && (gCurrentActor->unique_id == gScriptArg1)) ||
        ((gScriptArg2 == 1) && (gCurrentActor->unique_id != gScriptArg1))) {
        script_jump(gScriptArg0);
    }
}

/* Targets the object with unique id arg0 (object 0 if there is none). */
void script_targetUniqueId(void) {
    s32 index = collision_findObjectSlotById(gScriptArg0);

    gCurrentActor->targetIndex = index;
    if (index == -1) {
        gCurrentActor->targetIndex = 0;
    }
}

/*
 * Jumps to arg0 when the distance to the target (arg2: 0 actor_distanceXZ, 1 actor_distance3D, else the
 * height difference) is below (arg1 == 0) or above (arg1 == 1) arg3 * 8 (0xFF: sightRange * 8).
 */
void script_jumpIfTargetDistance(void) {
    Actor *target;
    f32 dist;
    f32 threshold;

    if (gScriptArg2 == 0) {
        target = &movement_obj[gCurrentActor->targetIndex];
        dist = actor_distanceXZ(gCurrentActor, target);
    } else if (gScriptArg2 == 1) {
        target = &movement_obj[gCurrentActor->targetIndex];
        dist = actor_distance3D(gCurrentActor, target);
    } else {
        dist = fabsf(gCurrentActor->ypos - movement_obj[gCurrentActor->targetIndex].ypos);
    }

    threshold = gScriptArg3 * 8;
    if (gScriptArg3 == 0xFF) {
        threshold = gCurrentActor->sightRange * 8;
    }
    if (((gScriptArg1 == 0) && (dist < threshold)) ||
        ((gScriptArg1 == 1) && (threshold < dist))) {
        script_jump(gScriptArg0);
    }
}

void script_goto(void) {
    script_jump(gScriptArg0);
}


/*
 * Places waypoint (arg0 & 0x3F) at a 12-bit signed offset (arg1:arg3 high nibble, arg2:arg3 low
 * nibble, times 8) from the target, rotated by the target's facing (mode arg0 >> 6 == 0), not at all
 * (mode 1) or by the direction from the target to this object (mode 2).
 */
void script_placeWaypointNearTarget(void) {
    Actor *pad; // matching: unused stack slot
    f32 y;
    f32 x;
    f32 offsetX;
    f32 offsetZ;
    f32 sine;
    f32 cosine;
    f32 angle;
    u8 facing;
    f32 z;
    s32 mode;

    mode = gScriptArg0 >> 6;
    gScriptArg0 &= 0x3F;
    x = movement_obj[gCurrentActor->targetIndex].xpos;
    z = movement_obj[gCurrentActor->targetIndex].zpos;
    y = movement_obj[gCurrentActor->targetIndex].ypos;
    offsetX = ((((s8)gScriptArg1 << 4) | (gScriptArg3 >> 4)) * 8);
    offsetZ = ((((s8)gScriptArg2 << 4) | (gScriptArg3 & 0xF)) * 8);
    if (mode == 0) {
        facing = ((movement_obj[gCurrentActor->targetIndex].langle >> 8) & 0xFF) - 0x40;
        sine = math_sinBinAngle(facing);
        cosine = math_cosBinAngle(facing);
    } else if (mode == 1) {
        sine = 0.0f;
        cosine = 1.0f;
    } else {
        angle = math_atan2(gCurrentActor->xpos - x, gCurrentActor->zpos - z);
        sine = sinf(angle);
        cosine = cosf(angle);
    }
    x += offsetX * cosine + offsetZ * sine;
    z += offsetZ * cosine - offsetX * sine;
    gSpawnPaths[gCurrentActor->listpos][gScriptArg0 + 1].x = x;
    gSpawnPaths[gCurrentActor->listpos][gScriptArg0 + 1].y = y;
    gSpawnPaths[gCurrentActor->listpos][gScriptArg0 + 1].z = z;
}

/* Jumps to arg0 when the target's anim->unk8 is below (arg2 == 0) or above (arg2 == 1) arg1. */
void script_jumpIfTargetAnimFrame(void) {
    AnimInstance *data;
    f32 value;
    s32 compare;

    data = movement_obj[gCurrentActor->targetIndex].anim;
    value = data->frame[0];
    compare = gScriptArg2;
    if (((compare == 0) && (value < (f32)(u32)gScriptArg1)) ||
        ((compare == 1) && ((f32)(u32)gScriptArg1 < value))) {
        script_jump(gScriptArg0);
    }
}

void func_15078874(void) {
    gCurrentActor->unk251 = gScriptArg0;
}

/* Jumps to arg0 when unk251 is (arg2 == 0) or is not (arg2 == 1) arg1. */
void func_15078890(void) {
    if (((gScriptArg2 == 0) && (gCurrentActor->unk251 == gScriptArg1)) ||
        ((gScriptArg2 == 1) && (gCurrentActor->unk251 != gScriptArg1))) {
        script_jump(gScriptArg0);
    }
}

/* arg3: jumps to arg0 when the target is object 0; else when the target's unk251 is (arg2 == 0) or is not (arg2 == 1) arg1. */
void func_15078900(void) {
    if (gScriptArg3 != 0) {
        if (gCurrentActor->targetIndex == 0) {
            script_jump(gScriptArg0);
        }
    } else if (((gScriptArg2 == 0) && (gScriptArg1 == movement_obj[gCurrentActor->targetIndex].unk251)) ||
               ((gScriptArg2 == 1) && (gScriptArg1 != movement_obj[gCurrentActor->targetIndex].unk251))) {
        script_jump(gScriptArg0);
    }
}

/* Copies unk8 and unkC of the target's anim data into the object's own. */
void script_copyTargetAnimFrame(void) {
    AnimInstance *dst;
    AnimInstance *src;

    dst = gCurrentActor->anim;
    src = movement_obj[gCurrentActor->targetIndex].anim;
    dst->frame[0] = src->frame[0];
    dst->frame[1] = src->frame[1];
}

/*
 * Script condition: reads property arg2 of object `index` (or of object 0 / the target for some)
 * and compares it with arg3 using operator arg1 (0 ==, 1 !=, 2 <, 3 >, 4 <=, 5 >=).
 */
s32 script_testProperty(s32 index) {
    s32 value;
    s32 diff;
    s32 pad; // matching: unused stack slot
    u8 facing;

    switch (gScriptArg2) {
    case 0:
        value = movement_obj[index].speed;
        break;
    case 1:
        value = movement_obj[index].taskint[0];
        break;
    case 2:
        value = movement_obj[index].taskint[1];
        break;
    case 3:
        value = movement_obj[index].taskint[2];
        break;
    case 4:
        value = movement_obj[index].ypos;
        break;
    case 5:
        value = movement_obj[index].stickspeed;
        break;
    case 6:
        // angle between the object's facing and the direction to it from the current object
        facing = movement_obj[index].langle >> 8;
        facing -= 0x40;
        diff = (u8)(math_atan2(movement_obj[index].xpos - gCurrentActor->xpos,
                                  movement_obj[index].zpos - gCurrentActor->zpos) * 40.74366379f) - facing;
        value = (u8)diff;
        if (value > 0x80) {
            value = 0x100 - value;
        }
        break;
    case 7:
        value = movement_obj[index].lifting;
        break;
    case 8:
        value = movement_obj[index].stunned;
        break;
    case 9:
        value = 0;
        if ((movement_obj[index].mv->pissed_up != 0) && (movement_obj[index].mv->pissed_hangover != 0)) {
            value = 2;
        }
        break;
    case 10:
        value = movement_obj[index].shad;
        break;
    case 11:
        value = movement_obj[0].mv->rt_context_b;
        break;
    case 12:
        value = movement_obj[0].mv->matrix_physics;
        break;
    case 13:
        value = movement_obj[0].unk102;
        break;
    case 14:
        value = D_800BE9EC;
        break;
    case 15:
        value = movement_obj[gCurrentActor->targetIndex].character_type;
        break;
    case 16:
        value = movement_obj[index].energy;
        break;
    case 17:
        value = movement_obj[gCurrentActor->targetIndex].intelwalk;
        break;
    case 18:
        value = movement_obj[index].norun;
        break;
    }
    // matching: one line, as -g3 line numbers steer the scheduling of this test
    if (((gScriptArg1 == 0) && (value == gScriptArg3)) || ((gScriptArg1 == 1) && (value != gScriptArg3)) || ((gScriptArg1 == 2) && (value < gScriptArg3)) || ((gScriptArg1 == 3) && (value > gScriptArg3)) || ((gScriptArg1 == 4) && (value <= gScriptArg3)) || ((gScriptArg1 == 5) && (value >= gScriptArg3))) { return 1; } return 0;
}

void script_jumpIfPlayerProperty(void) {
    if (script_testProperty(0) != 0) {
        script_jump(gScriptArg0);
    }
}

void script_switchIfPlayerProperty(void) {
    if (script_testProperty(0) != 0) {
        gCurrentActor->scriptCmd = script_getScript(0, gScriptArg0);
        gCurrentActor->scriptCmd--;
    }
}

void script_jumpIfProperty(void) {
    if (script_testProperty(dc_glbl_CharNum) != 0) {
        script_jump(gScriptArg0);
    }
}

void script_switchIfProperty(void) {
    if (script_testProperty(dc_glbl_CharNum) != 0) {
        gCurrentActor->scriptCmd = script_getScript(0, gScriptArg0);
        gCurrentActor->scriptCmd--;
    }
}

/* Jumps to arg0 when the y (arg3 & 2: x) of object 0 (arg3 & 1: this object) is below (s16)(arg1:arg2). */
void script_jumpIfCoordBelow(void) {
    s16 limit;
    s16 coord;
    u8 index;

    limit = (gScriptArg1 << 8) | gScriptArg2;
    index = gScriptArg3 & 1;
    if (index != 0) {
        index = dc_glbl_CharNum;
    }
    coord = movement_obj[index].ypos;
    if (gScriptArg3 & 2) {
        coord = movement_obj[index].xpos;
    }
    if (coord < limit) {
        script_jump(gScriptArg0);
    }
}

void script_setForcedLod(void) {
    if (gCurrentActor->modelCount < gScriptArg0) {
        gCurrentActor->forced_lod = 0xFF;
    } else {
        gCurrentActor->forced_lod = gScriptArg0;
    }
}

/* Calls actor_hitScripted with the direction from gate unk251 to the object (never 0) and holds for a tick. */
void func_15079228(void) {
    Actor *obj;
    TriggerZone *gate;
    f32 x;
    f32 z;
    u16 angle;

    // matching: the (no-op) load through the variable's address orders the address computation as the original
    gate = &(*(TriggerZone **)&zones)[gCurrentActor->unk251];
    obj = gCurrentActor;
    x = gate->x;
    z = gate->z;
    angle = actor_getDirectionAngle(obj->xpos - x, z - obj->zpos, 0);
    if (angle == 0) {
        angle = 1;
    }
    actor_hitScripted(gCurrentActor, gScriptArg0, angle, -1);
    gCurrentActor->scriptCmd--;
    gCurrentActor->scriptTimer = 1;
}

void script_setIntelWalk(void) {
    gCurrentActor->intelwalk = gScriptArg0;
}

void script_setAimTargetMode(void) {
    gCurrentActor->unk2F8 &= ~7;
    gCurrentActor->unk2F8 |= gScriptArg0;
}

/* Restarts the object found by script_findNearestInView on script arg0. */
void script_restartFoundObject(void) {
    Actor *found;

    if (gCurrentActor->unkA8 != 0) {
        found = &movement_obj[gCurrentActor->unkA8 & 0x7F];
        found->scriptCmd = NULL;
        found->intelwalk = gScriptArg0;
    }
}

void script_sendAttachmentMessage(void) {
    u16 type = gScriptArg0;
    s32 msg = gScriptArg1;
    s32 arg = gScriptArg2;

    effect_sendAttachmentMessage(gCurrentActor, type, msg, arg);
}

/* Places waypoint arg0 at a random point up to 550 units from object 0, clamped to +-1200. */
void script_placeWaypointNearPlayer(void) {
    f32 offsetX;
    f32 offsetZ;
    f32 third;
    s16 pad0; // matching: unused stack slots
    s16 pad1;
    s16 pad2;
    s16 x;
    s16 z;

    x = movement_obj[0].xpos;
    z = movement_obj[0].zpos;
    if ((x < -1180) || (x > 1180) || (z < -1180) || (z > 1180)) {
        x = 0;
        z = 0;
    }

    actor_splitVelocity(math_randomU8(), 550.0f, 0.0f, &offsetX, &offsetZ, &third);

    x = x + offsetX;
    z = z + offsetZ;
    if (x < -1200) {
        x = -1200;
    }
    if (x > 1200) {
        x = 1200;
    }
    if (z < -1200) {
        z = -1200;
    }
    if (z > 1200) {
        z = 1200;
    }

    gSpawnPaths[gCurrentActor->listpos][gScriptArg0 + 1].x = x;
    gSpawnPaths[gCurrentActor->listpos][gScriptArg0 + 1].z = z;
}

/* Sets the speeds for a jump that lands on the target. */
void script_leapToTarget(void) {
    f32 dist;
    f32 time;

    dist = actor_distanceXZ(gCurrentActor, &movement_obj[gCurrentActor->targetIndex]);
    gCurrentActor->stickspeed = gCurrentActor->speed;
    time = 2.0f * (dist / gCurrentActor->stickspeed);
    gCurrentActor->yspd = (gCurrentActor->ygrav * time) * 0.5f;
    gCurrentActor->yspd += 2.0f * ((movement_obj[gCurrentActor->targetIndex].ypos - gCurrentActor->ypos) / time);
}

/* Restarts the object with unique id arg0 on script arg1. */
void script_restartById(void) {
    s32 index;
    Actor *obj;

    index = collision_findObjectSlotById(gScriptArg0);
    if (index != -1) {
        obj = &movement_obj[index];
        obj->intelwalk = gScriptArg1;
        obj->scriptCmd = NULL;
        obj->pendingScript = 0;
    }
}

/* arg2: sets `immune` of the object with unique id arg0 to arg1; else jumps to arg1 when that object is stunned. */
void func_150796CC(void) {
    s32 index = collision_findObjectSlotById(gScriptArg0);

    if (gScriptArg2 != 0) {
        if (index != -1) {
            movement_obj[index].imune = gScriptArg1;
        }
    } else if ((index != -1) && (movement_obj[index].stunned != 0)) {
        script_jump(gScriptArg1);
    }
}

/* arg2: removes the object (type 0xFF); else makes it type 0x3A placed randomly within 250 of its path's centre. */
void func_15079790(void) {
    s16 offsetX;
    s16 offsetZ;

    if (gScriptArg2 != 0) {
        gCurrentActor->character_type = 0xFF;
    } else {
        gCurrentActor->character_type = 0x3A;
        offsetX = (math_randomU8() % 500U) - 250;
        offsetZ = (math_randomU8() % 500U) - 250;
        gCurrentActor->xpos = gSpawnPaths[gCurrentActor->listpos][0].x + offsetX;
        gCurrentActor->zpos = gSpawnPaths[gCurrentActor->listpos][0].z + offsetZ;
    }
}

/* Starts a move with object 0's speed plus (s8)arg2, clamped to [arg1, 250]. */
void script_startMoveAtPlayerSpeed(void) {
    s32 speed = movement_obj[0].speed + (s8)gScriptArg2;

    if (speed < gScriptArg1) {
        speed = gScriptArg1;
    } else if (speed > 250) {
        speed = 250;
    }
    gScriptArg2 = speed;
    script_startMove();
}

/* Jumps to arg0 when the object is near its current waypoint (see script_jumpIfNearWaypoint). */
void script_jumpIfAtWaypoint(void) {
    gScriptArg1 = gCurrentActor->waypoint;
    script_jumpIfNearWaypoint();
}

void script_requestExpression(void) {
    u16 timer;

    timer = (gScriptArg1 << 16) + gScriptArg2;
    if (timer == 0) {
        timer = 0xFFFF;
    }
    face_requestExpression(gCurrentActor, gScriptArg0, gScriptArg3, timer, 0);
}

void script_setDefaultExpression(void) {
    face_setDefaultExpression(gCurrentActor, gScriptArg0);
}

void script_playRandomSoundOnSelf(void) {
    s32 id = (gScriptArg0 << 8) + gScriptArg1;

    if (gScriptArg2 != 0) {
        id += math_randomU8() % (u32)gScriptArg2;
    }
    actor_playSound(id, gCurrentActor);
}

void script_setFallReaction(void) {
    gCurrentActor->fallReaction = gScriptArg0;
    gCurrentActor->fallReactionHeight = gScriptArg1;
}

void script_setSpawnDelay(void) {
    gSpawnDelays[gCurrentActor->listpos] = (gScriptArg0 << 8) + gScriptArg1;
}

/* Returns the direction from object `index` to the centre of the current object's path. */
u16 script_getAngleToPathStart(s32 index) {
    PathPoint *path;
    Actor *obj;

    path = gSpawnPaths[gCurrentActor->listpos];
    obj = &movement_obj[index];
    return actor_getDirectionAngle(path[0].x - obj->xpos, obj->zpos - path[0].z, 0);
}

/*
 * Changes a property of an object: arg1 selects the object (0, 1: the current one, else unique id
 * arg1 - 2), arg2 the property and, for counters 1-3, arg3 the operation applied with value arg0.
 */
void script_changeObjectProperty(void) {
    s32 index;
    s32 *counter;

    switch (gScriptArg1) {
    case 0:
    case 1:
        index = dc_glbl_CharNum;
        break;
    default:
        index = collision_findObjectSlotById(gScriptArg1 - 2);
        if (index == -1) {
            return;
        }
        break;
    }
    switch (gScriptArg2) {
    case 1:
        counter = &movement_obj[index].taskint[0];
        break;
    case 2:
        counter = &movement_obj[index].taskint[1];
        break;
    case 3:
        counter = &movement_obj[index].taskint[2];
        break;
    case 4:
    case 5: {
        Actor *savedObj;
        s32 savedIndex;

        // restart the object on script arg0, running as the current object meanwhile
        savedObj = gCurrentActor;
        savedIndex = dc_glbl_CharNum;
        gCurrentActor = &movement_obj[index];
        dc_glbl_CharNum = index;
        if (gCurrentActor->intelwalk != gScriptArg0) {
            gCurrentActor->intelwalk = gScriptArg0;
            gCurrentActor->scriptCmd = script_getScript(0, gCurrentActor->intelwalk);
            gCurrentActor->scriptTimer = 0;
        }
        dc_glbl_CharNum = savedIndex;
        gCurrentActor = savedObj;
        return;
    }
    case 6:
        movement_obj[index].rootOffsetY = gScriptArg0;
        return;
    case 7:
        movement_obj[index].vtilt = 0.0f;
        return;
    case 8: {
        u16 angle;

        angle = script_getAngleToPathStart(gCurrentActor->targetIndex);
        if ((script_getAngleToPathStart(dc_glbl_CharNum) - angle) & 0x8000) {
            gCurrentActor->waypointStep = gScriptArg0;
        }
        return;
    }
    case 9: {
        u16 value = (gScriptArg3 << 8) | gScriptArg0;

        gCurrentActor->intel_animseq = value;
        return;
    }
    case 10:
        gCurrentActor->nearRange = gCurrentActor->sightRange + (s8)gScriptArg0;
        return;
    case 11:
        gCurrentActor->unk22A = gScriptArg0;
        return;
    }
    switch (gScriptArg3) {
    case 0:
        *counter = gScriptArg0;
        break;
    case 1:
        (*counter)--;
        break;
    case 2:
        *counter += gFrameTicks;
        break;
    case 3:
        (*counter)++;
        break;
    case 4:
        *counter -= gFrameTicks;
        break;
    case 5:
        *counter -= gActorStepPercent;
        break;
    case 6:
        *counter += gActorStepPercent;
        break;
    }
}

void script_setSpeedup(void) {
    gCurrentActor->speedup = gScriptSpeedups[gScriptArg0];
}

void script_setHitReaction(void) {
    gCurrentActor->hitReactionOverride = gScriptArg0;
}

void func_15079F6C(void) {
    u16 high;
    u16 low;

    high = gScriptArg0 << 8;
    low = gScriptArg1;
    // matching: the stacked no-op masks raise the expression weight so IDO keeps `low` out of v1
    gCurrentActor->unk224 = (high | (((((((((low & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF)) & 0xFFFFu;
    gCurrentActor->unk22B = gScriptArg2;
    gCurrentActor->unk226 = gScriptArg3;
}

/*
 * Follows object 0: when it is unlinked (unk65 == 0) or slower than 5, switches to script arg1;
 * otherwise starts a mode-3 move in its unk4C direction at its speed (times 0.7 unless chased).
 */
void script_steerByPlayerStick(void) {
    s16 speed;

    if ((movement_obj[0].attach_char == 0) || (movement_obj[0].stickspeed < 5.0f)) {
        gCurrentActor->intelwalk = gScriptArg1;
        gCurrentActor->scriptCmd = script_getScript(0, gCurrentActor->intelwalk);
        gCurrentActor->scriptCmd--;
        return;
    }

    gCurrentActor->fangle = movement_obj[0].mv->press_angle;

    if (movement_obj[0].mv->chased) {
        speed = movement_obj[0].stickspeed;
    } else {
        speed = movement_obj[0].stickspeed * 0.7f;
    }
    if (speed < 17) {
        gScriptArg3 += 1;
        gScriptArg0 = 8;
    }
    gScriptArg2 = speed;
    script_startFollowTarget();
    gCurrentActor->mvmt_control |= 0x40;
}

/* Sets coordinate arg1 (0 x, 1 y, 2 z) of waypoint arg0 to (s16)(arg2:arg3). */
void script_setWaypointCoord(void) {
    s32 value = ((s8)gScriptArg2 << 8) | gScriptArg3;

    ((s16 *)&(gSpawnPaths[gCurrentActor->listpos] + 1)[gScriptArg0])[gScriptArg1] = value; // matching: waypoints start one point past the centre
}

/* Sets the speed to object 0's speed plus (s8)arg2, clamped to [arg1, arg0]. */
void script_setSpeedFromPlayer(void) {
    s32 speed = movement_obj[0].speed + (s8)gScriptArg2;

    if (speed < gScriptArg1) {
        speed = gScriptArg1;
    } else if (speed > gScriptArg0) {
        speed = gScriptArg0;
    }
    gCurrentActor->stickspeed = speed;
    if (gCurrentActor->stickspeed == 1.0f) {
        gCurrentActor->stickspeed = 0.5f;
    }
}

void script_startMoveKeepSpeed(void) {
    gCurrentActor->moveMode = 0;
    gCurrentActor->scriptTimer = gScriptArg0 * 100;
    gCurrentActor->unk22C &= 0xFD;
    script_applyMoveFlags();
}

void script_breakIntoPartsVariant(void) {
    model_breakIntoParts(gCurrentActor, gScriptArg0, (s8)gScriptArg1);
}

void script_startChaseRandomAnim(void) {
    gScriptArg3 = (math_randomU8() % (u32)gScriptArg1) + gScriptArg3;
    script_startChase();
}

/* Targets a random active object of type arg0, saving the previous target in unkA8. */
void script_targetRandomOfType(void) {
    s32 i;
    u32 count;
    u8 candidates[25];

    count = 0;
    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type != 0) && (movement_obj[i].character_type == gScriptArg0)) {
            candidates[count] = i;
            count++;
        }
    }
    if (count != 0) {
        i = math_randomU8();
        gCurrentActor->unkA8 = gCurrentActor->targetIndex;
        gCurrentActor->targetIndex = candidates[i % count];
    }
}

void script_restoreTarget(void) {
    gCurrentActor->targetIndex = gCurrentActor->unkA8;
}

void script_setSpotHeight(void) {
    gCurrentActor->spotHeight = gScriptArg0;
}

/* Returns the four argument bytes as one big-endian word. */
s32 script_getArgWord(void) {
    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    return ((gScriptArg0 & 0xFF) << 24) | ((gScriptArg1 & 0xFF) << 16) | ((gScriptArg2 & 0xFF) << 8) | (gScriptArg3 & 0xFF);
}

void script_showOnlyParts(void) {
    s32 flags;

    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    flags = ((gScriptArg0 & 0xFF) << 24) | ((gScriptArg1 & 0xFF) << 16) | ((gScriptArg2 & 0xFF) << 8) | (gScriptArg3 & 0xFF);
    gCurrentActor->hiddenParts = ~(flags | 1);
}

void script_showParts(void) {
    s32 flags;

    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    flags = ((gScriptArg0 & 0xFF) << 24) | ((gScriptArg1 & 0xFF) << 16) | ((gScriptArg2 & 0xFF) << 8) | (gScriptArg3 & 0xFF);
    gCurrentActor->hiddenParts &= ~flags;
}

void script_hideParts(void) {
    s32 flags;

    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    flags = ((gScriptArg0 & 0xFF) << 24) | ((gScriptArg1 & 0xFF) << 16) | ((gScriptArg2 & 0xFF) << 8) | (gScriptArg3 & 0xFF);
    gCurrentActor->hiddenParts |= flags;
}

/*
 * Waypoint direction: arg0 0 sets the step waypointStep to arg1, 1 reverses it, 2 reverses it and moves
 * the waypoint index by the new step plus arg3 further, wrapping at arg2 (0: the path's count).
 */
void script_changeWaypointDirection(void) {
    s32 count;
    s32 index;

    if (gScriptArg0 == 0) {
        gCurrentActor->waypointStep = gScriptArg1;
    } else if (gScriptArg0 == 1) {
        gCurrentActor->waypointStep = -gCurrentActor->waypointStep;
    } else if (gScriptArg0 == 2) {
        if (gScriptArg2 != 0) {
            count = gScriptArg2;
        } else {
            count = gSpawnPathCounts[gCurrentActor->listpos] - 1;
        }
        gCurrentActor->waypointStep = -gCurrentActor->waypointStep;
        index = gCurrentActor->waypoint + gCurrentActor->waypointStep;
        if (gCurrentActor->waypointStep > 0) {
            index += gScriptArg3;
        } else {
            index -= gScriptArg3;
        }
        if (index >= count) {
            index -= count;
        } else if (index < 0) {
            index += count;
        }
        gCurrentActor->waypoint = index;
    }
}

/*
 * Jumps to arg0 when the angle (arg1: the object's unk78, else its facing relative to the
 * direction to the target), in 1/256 turns, lies in [arg2, arg3).
 */
void script_jumpIfAngleInRange(void) {
    u8 index;
    u16 angle;

    index = gCurrentActor->targetIndex;
    if (gScriptArg1 != 0) {
        angle = gCurrentActor->fangle >> 8;
    } else {
        angle = (gCurrentActor->langle - actor_getDirectionAngle(movement_obj[index].xpos - gCurrentActor->xpos,
                                                   gCurrentActor->zpos - movement_obj[index].zpos, 0)) >> 8;
    }
    if ((u8)(angle - gScriptArg2) < (u8)(gScriptArg3 - gScriptArg2)) {
        script_jump(gScriptArg0);
    }
}

s32 script_getPadButtons(s32 player) {
    return gPads[player].button;
}

/* Jumps to arg3 when entry arg0 of gPads has all the bits of (arg1:arg2) set. */
void script_jumpIfButtonsHeld(void) {
    u16 bits = script_getPadButtons(gScriptArg0);
    u16 mask = (gScriptArg1 << 8) | gScriptArg2;

    if ((bits & mask) == mask) {
        script_jump(gScriptArg3);
    }
}

void script_jumpIfLevel(void) {
    u16 value = (gScriptArg1 << 8) | gScriptArg2;

    if (level == value) {
        script_jump(gScriptArg0);
    }
}

void script_setNorun(void) {
    gCurrentActor->norun = gScriptArg0;
}

void script_removePartSet(void) {
    collision_removePartSet(gCurrentActor, gScriptArg0);
}

void func_1507A808(void) {
    gCurrentActor->unk22C = (gCurrentActor->unk22C & gScriptArg0) | gScriptArg1;
}

void script_jumpIfEnergy(void) {
    if (gCurrentActor->energy == gScriptArg1) {
        script_jump(gScriptArg0);
    }
}

void script_startViewEnvelope(void) {
    camera_startEnvelope(0, gScriptArg0, gScriptArg1);
}

void script_jumpIfPlayerAttachedOrMultiplayer(void) {
    if ((movement_obj[0].attach_char != 0) || (gIsMultiplayer != 0)) {
        script_jump(gScriptArg0);
    }
}

/* Jumps to arg0 when the target is (arg2 == 0) or is not (arg2 == 1) of type arg1. */
void script_jumpIfTargetType(void) {
    if (gScriptArg1 == movement_obj[gCurrentActor->targetIndex].character_type) {
        gScriptArg2 ^= 1;
    }
    if (gScriptArg2 != 0) {
        script_jump(gScriptArg0);
    }
}

/* Links the object to the one found by collision_findObjectById(arg0) (unk65 = its index + 1), or unlinks it (arg0 == 0). */
void script_attachToObject(void) {
    Actor *linked;

    if (gScriptArg0 != 0) {
        linked = collision_findObjectById(gScriptArg0);
        if (linked != NULL) {
            if (gScriptArg2 == 0) {
                gCurrentActor->unk101 |= 4;
            }
            if (gScriptArg3 != 0) {
                gCurrentActor->unk101 |= 0x40;
            }
            gCurrentActor->attach_char = (linked - movement_obj) + 1;
            gCurrentActor->basemtx = gScriptArg1;
        }
    } else {
        gCurrentActor->attach_char = 0;
    }
}

/* Sets an angle: arg2 selects the operation, (arg0:arg1) is the operand. */
void script_setFacing(void) {
    u16 value;
    PathPoint *path;
    s32 angle;

    value = (gScriptArg0 << 8) | gScriptArg1;
    switch (gScriptArg2) {
    case 0:
        gCurrentActor->fangle = gCurrentActor->angle + value;
        break;
    case 1:
        gCurrentActor->fangle = gCurrentActor->angle - value;
        break;
    case 2:
        gCurrentActor->fangle = value;
        break;
    case 3:
        // matching: the do-while wrapper changes how IDO schedules case 4 below
        do {
            gCurrentActor->angle = gCurrentActor->fangle;
            gCurrentActor->langle = gCurrentActor->fangle;
        } while (0);
        break;
    case 4:
        // direction from waypoint arg0 to waypoint arg1, plus arg3 / 256 turns
        path = gSpawnPaths[gCurrentActor->listpos];
        angle = (u16)(math_atan2(path[gScriptArg1 + 1].x - path[gScriptArg0 + 1].x, path[gScriptArg1 + 1].z - path[gScriptArg0 + 1].z) * 10430.37793f) + (gScriptArg3 << 8);
        gCurrentActor->fangle = angle + 0x4000;
        break;
    case 5:
        gCurrentActor->fangle = math_randomU8();
        break;
    case 6:
        gCurrentActor->fangle = gCurrentActor->angle;
        break;
    case 7:
        gCurrentActor->angle = movement_obj[gCurrentActor->targetIndex].langle + value;
        break;
    case 8:
        gCurrentActor->angle = gCurrentActor->langle;
        break;
    }
}

void script_setWaypointRange(void) {
    gCurrentActor->waypointStart = gScriptArg0;
    gCurrentActor->waypointEnd = gScriptArg1;
}

void script_setTurnRatesWithFrac(void) {
    gCurrentActor->moveTurnRate = gScriptArg0;
    gCurrentActor->faceTurnRate = gScriptArg0;
    gCurrentActor->moveTurnFrac = gScriptArg1;
    gCurrentActor->faceTurnFrac = gScriptArg1;
}

void script_setFallScript(void) {
    gCurrentActor->fallScript = gScriptArg0;
    gCurrentActor->fallScriptDepth = gScriptArg1;
}

void script_setFamily(void) {
    gCurrentActor->family = gScriptArg0;
}

void script_setMvmtControl(void) {
    gCurrentActor->mvmt_control |= script_getArgWord();
}

void script_clearMvmtControl(void) {
    gCurrentActor->mvmt_control &= ~script_getArgWord();
}

/* Splits (angle between unk78 and the facing unk7A) / arg0 into faceTurnRate (high byte) and faceTurnFrac (low byte). */
void script_setFaceTurnTime(void) {
    s32 diff;
    s32 step;
    s32 facing = gCurrentActor->langle;
    s32 target = gCurrentActor->fangle;

    diff = target - facing;
    if (diff < 0) {
        diff = -diff;
    }
    if (diff > 0x8000) {
        diff -= 0x10000;
    }
    if (diff < 0) {
        diff = -diff;
    }
    step = diff / gScriptArg0;
    gCurrentActor->faceTurnRate = step >> 8;
    gCurrentActor->faceTurnFrac = step & 0xFF;
}

void script_setSpotHeightAlt(void) {
    gCurrentActor->spotHeight = gScriptArg0;
}

void script_setHitCylinder(void) {
    gCurrentActor->hitRadius = (gScriptArg0 << 8) | gScriptArg1;
    gCurrentActor->hitHeight = (gScriptArg2 << 8) | gScriptArg3;
    gCurrentActor->hitRadius *= gCurrentActor->scale_xz;
    gCurrentActor->hitHeight *= gCurrentActor->scale_y;
}

void script_setHitOffsetY(void) {
    gCurrentActor->hitOffsetY = (gScriptArg0 << 8) | gScriptArg1;
    gCurrentActor->hitOffsetY *= gCurrentActor->scale_y;
}

void script_setHitOffsetXZ(void) {
    gCurrentActor->hitOffsetZ = (gScriptArg0 << 8) | gScriptArg1;
    gCurrentActor->hitOffsetX = (gScriptArg2 << 8) | gScriptArg3;
    gCurrentActor->hitOffsetZ *= gCurrentActor->scale_xz;
    gCurrentActor->hitOffsetX *= gCurrentActor->scale_xz;
}

void script_setAngleFromLangle(void) {
    gCurrentActor->angle = gCurrentActor->langle;
}

/* Acts on the object returned by event_getOwnedObject: arg0 0 calls actor_fadeCommand, 1 sets and 2 restarts script arg1. */
void script_commandOwnedObject(void) {
    Actor *savedObj = gCurrentActor;
    s32 savedIndex = dc_glbl_CharNum;

    gCurrentActor = event_getOwnedObject(gCurrentActor, 0);
    dc_glbl_CharNum = gCurrentActor - movement_obj;

    if (gCurrentActor != NULL) {
        switch (gScriptArg0) {
        case 0:
            actor_fadeCommand(gCurrentActor, 1, gScriptArg1, 0, 0);
            break;
        case 1:
            gCurrentActor->intelwalk = gScriptArg1;
            /* fallthrough */
        case 2:
            gCurrentActor->scriptCmd = script_getScript(0, gScriptArg1);
            gCurrentActor->scriptTimer = 0;
        }
    }
    dc_glbl_CharNum = savedIndex;
    gCurrentActor = savedObj;
}

void script_setNojump(void) {
    gCurrentActor->nojump = gScriptArg0;
}

void script_setBodySize(void) {
    s16 value = (gScriptArg1 << 8) | gScriptArg0;

    if (gScriptArg3 == 2) {
        gCurrentActor->bodyOffsetY = value * gCurrentActor->scale_y;
    } else if (gScriptArg3 == 1) {
        actor_setBodyHeight(gCurrentActor, value);
    } else {
        actor_setBodyRadius(gCurrentActor, value);
    }
}

void script_playSound1Range1800(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(300, 1800);
    }
}

void script_playSound1Range3000(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(300, 3000);
    }
}

void script_playSound2Range1800(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(300, 1800);
    }
}

void script_playSound2Range3000(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(300, 3000);
    }
}

void script_playSound1Range4000(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(700, 4000);
    }
}

void script_playSound2Range4000(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(700, 4000);
    }
}

void script_playSound1Range8000(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(3000, 8000);
    }
}

void script_playSound1Range1300(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(190, 1300);
    }
}

void script_playSound2Range1300(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(190, 1300);
    }
}

void script_playSound1Range800(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(100, 800);
    }
}

void script_playSound2Range800(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(100, 800);
    }
}

void script_setDoingHit(void) {
    gCurrentActor->doing_hit = gScriptArg0;
    gCurrentActor->doingHitTimer = gScriptArg1;
}

/* Jumps to arg0 when mv->context_b is (arg2 == 0) or is not (arg2 == 1) arg1. */
void script_jumpIfContext(void) {
    if (gCurrentActor->mv->context_b == gScriptArg1) {
        gScriptArg2 ^= 1;
    }
    if (gScriptArg2 != 0) {
        script_jump(gScriptArg0);
    }
}

void func_1507B630(void) {
    if ((gCurrentActor->norun == 0) && (gCurrentActor->shad == 0.0f)) {
        if (gCurrentActor->mv->rt_context_b != 0) {
            gScriptArg0 ^= 1;
        }
        if (gScriptArg0 != 0) {
            gCurrentActor->mv->pressedButtons |= 0x4000;
            gCurrentActor->mv->pressedButtonsTimer = 0;
        }
    } else {
        // repeat this command
        gCurrentActor->scriptCmd--;
    }
    gCurrentActor->scriptTimer = 100;
}

void script_waitUntilCanRun(void) {
    if ((gCurrentActor->norun != 0) || (gCurrentActor->shad > 5.0f)) {
        // repeat this command
        gCurrentActor->scriptCmd--;
        gCurrentActor->scriptTimer = 100;
    }
}

void script_pressButtons(void) {
    u16 value = (gScriptArg0 << 8) | gScriptArg1;

    if (gScriptArg3 != 0) {
        gCurrentActor->mv->pressedButtons = value;
        gCurrentActor->mv->pressedButtonsTimer = gScriptArg2;
    } else {
        gCurrentActor->mv->buttons = value;
        gCurrentActor->mv->buttonsTimer = gScriptArg2;
    }
}

void script_startWeavingMove(void) {
    script_startMove();
    gCurrentActor->moveMode = 17;
}

/* Jumps to arg0 when the target is (arg1 == 0) or is not (arg1 == 1) stunned or dead. */
void script_jumpIfTargetStunned(void) {
    Actor *target = &movement_obj[gCurrentActor->targetIndex];

    if ((target->stunned != 0) || (target->energy == 0)) {
        gScriptArg1 ^= 1;
    }
    if (gScriptArg1 != 0) {
        script_jump(gScriptArg0);
    }
}

/* Calls handler arg0 of gAnimCommandProcs, preserving the current object. */
void script_callHandler(void) {
    Actor *savedObj;
    s32 savedIndex;
    u8 index;
    void (*handler)(s32);

    index = gScriptArg0;
    handler = gAnimCommandProcs[index];
    if (handler != NULL) {
        savedObj = gCurrentActor;
        savedIndex = dc_glbl_CharNum;
        handler(index);
        gCurrentActor = savedObj;
        dc_glbl_CharNum = savedIndex;
    }
}

void func_1507B8F4(void) {
    if (gIsMultiplayer != 0) {
        if ((gScriptArg1 != 0) || ((gCurrentActor->mv != NULL) && (gCurrentActor->mv->bot != 0))) {
            script_jump(gScriptArg0);
        }
    }
}

void script_setFloatDepth(void) {
    gCurrentActor->floatDepth = gScriptArg0;
}

/*
 * Jumps to arg0 when unk6 of the path record arg3 past the current waypoint (of the linked object,
 * if any) is (arg2 == 0) or is not (arg2 == 1) arg1.
 */
void script_jumpIfWaypointParam(void) {
    Actor *obj;

    obj = gCurrentActor;
    if (gCurrentActor->attach_char != 0) {
        obj = &movement_obj[gCurrentActor->attach_char - 1];
    }
    if (gScriptArg1 == gSpawnPaths[obj->listpos][obj->waypoint + gScriptArg3].param) {
        gScriptArg2 ^= 1;
    }
    if (gScriptArg2 != 0) {
        script_jump(gScriptArg0);
    }
}

void script_playRandomUntrackedSound(void) {
    u16 id = (gScriptArg2 << 8) | gScriptArg3;

    if (gScriptArg0 != 0) {
        id += math_randomU8() % (u32)gScriptArg0;
    }
    actor_playUntrackedSound(id, gCurrentActor);
}

void script_setHitSound(void) {
    gCurrentActor->hitSound = script_getArgWord();
}

void script_setImpactSound(void) {
    gCurrentActor->impactSound = script_getArgWord();
}

void script_emptyCommand108(void) {
}

typedef struct ScriptEntry {
    const ObjScriptCmd *script;
    u8 id;
} ScriptEntry;

/* The fallback script. */
static const ObjScriptCmd gFallbackObjScript[5] = {
    { 0x67, { 0x14, 0x0F, 0x19, 0x00 } },
    { 0x31, { 0x80, 0x00, 0x00, 0x00 } },
    { 0x33, { 0xFF, 0x00, 0x00, 0x00 } },
    { 0x01, { 0x14, 0x00, 0x00, 0x00 } },
    { 0x08, { 0x00, 0x00, 0x00, 0x00 } },
};

/* The command handlers, indexed by opcode. */
static void (*gObjScriptHandlers[214])(void) = {
    script_doNothing, script_startMove, script_setSpotScript, script_jumpIfLevel,
    script_jumpIfButtonsHeld, script_setNorun, script_setYSpeedAndGravity, script_startChase,
    script_switchScript, script_startFlee, script_setSpotWideScript, script_jumpRandom,
    script_setNearScript, script_startFollowTarget, script_playRandomUntrackedSound, script_setYSpeedGravityAndHop,
    script_leapToWaypoint, script_startOrbitWaypoint, script_setHitSound, func_150761F4,
    script_setHitCylinder, script_removePartSet, script_jumpIfEnergy, script_startViewEnvelope,
    script_playPlayerSound, script_fadeFirstTwoChannels, script_jumpIfPlayerAttachedOrMultiplayer, script_setFacing,
    script_setWaypointRange, script_setTurnRatesWithFrac, script_jumpIfAngleInRange, script_setHitOffsetY,
    script_startHoldTargetOffset, script_jumpIfNotHit, func_15077404, script_startIntercept,
    func_15075BB8, NULL, script_startChaseAlt, script_spawnPuff,
    script_setFallScript, script_setFamily, script_jumpIfTargetType, script_setFaceTurnTime,
    script_setNearRange, script_setStunEndScript, script_setSpotHeightAlt, script_setHitRoute,
    script_setAlertCooldown, script_setTurnRates, script_setHitOffsetXZ, script_setImmune,
    script_setAngleFromLangle, script_commandOwnedObject, func_1507659C, script_jumpIfCoordBelow,
    script_setNojump, script_playSound1Range1800, script_playSound1Range3000, script_playSound2Range1800,
    script_playSound2Range3000, script_emptyCommand61, script_playSound1Range4000, script_playSound2Range4000,
    script_playSound1Range8000, script_setNoshoveFlags, script_setBodySize, script_setTurnStopScript,
    script_jumpIfWaypointParam, script_waitUntilNearGround, script_emptyCommand70, func_15076768,
    script_setDoingHit, script_setImpactSound, script_jumpIfBehindTarget, script_findNearestInView,
    script_targetFoundObject, script_setDrawType, script_startMove, script_setAlpha,
    script_setAngleFromFangle, script_moveToLastOfType, script_setMvmtControl, script_clearMvmtControl,
    script_removeSelf, script_setSpeed, script_setScale, script_startWeavingMove,
    script_setType, script_playSound1Range1300, script_playSound2Range1300, script_playSound1Range800,
    script_startMove, script_playSound2Range800, script_pickWaypointWatchingTarget, script_jumpIfTargetStunned,
    script_startMove, func_1507B8F4, script_runFadeCommand, script_breakIntoParts,
    script_clearNoshoveFlags, func_1507A808, script_pickWaypoint, script_setAnimSpeedAndAccel,
    script_advanceWaypoint, script_pickWaypointNoAdvance, script_jumpIfWithinRange, script_startChasePlayer,
    script_emptyCommand108, script_startMoveUnlessNear, script_playUntrackedSound, script_startMoveWatchingTarget,
    func_15076E10, script_emptyCommand113, script_jumpIfContext, func_1507B630,
    script_waitUntilCanRun, script_startRandomJump, script_pressButtons, script_removeSelfAlt,
    script_setHeightOffset, func_15076FA8, script_clearTaskAttrib, script_jumpIfTargetSlow,
    script_setDeathReaction, script_setFloatDepth, script_startMoveToTarget, script_setEnergy,
    script_leapToTarget, script_restartById, func_150796CC, script_doNothing,
    script_targetRandomOfType, script_restoreTarget, script_changeWaypointDirection, script_doNothing,
    script_doNothing, script_doNothing, script_setHitRouteAlt, script_doNothing,
    script_setOrbit, script_setStandOnScript, script_playSoundOnSelf, script_setSightRange,
    script_jumpIfWaypointCompare, script_startTimedMove, script_findTarget, script_jumpIfWaypoint,
    script_setTaskAttrib, script_restartObjectOfType, script_setWaypoint, NULL,
    script_startTrackAt, script_warpToWaypoint, script_setHitPlayerScript, script_setTurnAccel,
    script_setTurnAnim, script_turnToWaypoint, script_jumpIfAnyObjectNear, script_jumpIfTargetInactive,
    script_setTypeSpot, script_jumpIfNearWaypoint, script_setArriveRange, NULL,
    script_jumpIfUniqueId, script_targetUniqueId, script_jumpIfTargetDistance, script_goto,
    script_placeWaypointNearTarget, script_jumpIfTargetAnimFrame, func_15078874, func_15078890,
    func_15078900, script_copyTargetAnimFrame, script_jumpIfPlayerProperty, func_15079228,
    script_setIntelWalk, script_setAimTargetMode, script_sendAttachmentMessage, script_switchIfPlayerProperty,
    script_placeWaypointNearPlayer, script_startMoveAtPlayerSpeed, script_jumpIfAtWaypoint, script_requestExpression,
    script_setDefaultExpression, script_jumpIfProperty, script_playRandomSoundOnSelf, func_15079790,
    script_restartFoundObject, script_setForcedLod, script_setFallReaction, script_setSpawnDelay,
    script_changeObjectProperty, script_setSpeedup, script_setHitReaction, func_15079F6C,
    script_steerByPlayerStick, script_setWaypointCoord, script_setSpeedFromPlayer, script_startMoveKeepSpeed,
    script_switchIfProperty, script_startChase3D, script_breakIntoPartsVariant, script_startChaseNoLeash,
    script_startChaseRandomAnim, script_setSpotHeight, script_showOnlyParts, script_showParts,
    script_hideParts, script_attachToObject, script_stepPathGraph, script_jumpIfAtDestination,
    script_setHidden, script_callHandler
};

/* The default scripts, ended by a NULL script. */
static ScriptEntry gDefaultObjScripts[3] = {
    { gFallbackObjScript, 0x01 },
    { gFallbackObjScript, 0xFA },
    { NULL, 0 },
};

/* Returns script `scriptId` of the current object's type, falling back to the default table. */
const ObjScriptCmd *script_getScript(s32 arg0, s32 scriptId) {
    ActorAnim *typeData;
    ScriptEntry *table;
    s32 i;

    typeData = gModelAnimTables[gCurrentActor->character_type];
    if (typeData == NULL) {
        return gFallbackObjScript;
    }
    table = ANIM_HEADER(typeData)->scripts;
    if (table == NULL) {
        return gFallbackObjScript;
    }
    i = 0;
    while (scriptId != table[i].id) {
        if (table[i].script == NULL) {
            ScriptEntry *defaults = gDefaultObjScripts;

            i = 0;
            while (scriptId != defaults[i].id) {
                if (defaults[i].script == NULL) {
                    /* end of the default table without a match: the original game hangs here */
                    while (1) {
                    }
                }
                i++;
            }
            return defaults[i].script;
        }
        i++;
    }
    return table[i].script;
}

/*
 * Runs an object's script: counts down the wait timer scriptTimer (25500 waits forever) and, once it
 * expires, executes commands until one sets a new wait time.
 */
void script_runActor(Actor *obj) {
    if (obj->scriptTimer != 0) {
        if (obj->scriptCmd != NULL) {
            if (obj->scriptTimer != 25500) {
                obj->scriptTimer = obj->scriptTimer - gActorStepPercent;
            }
            if ((obj->scriptTimer >= 50000) || (obj->scriptTimer == 0)) {
                // expired (or wrapped below zero)
                obj->scriptTimer = 0;
                if (obj->mvmt_control & 0x400) {
                    obj->scriptTimer = 20000;
                    obj->mvmt_control &= ~0x400;
                    obj->mvmt_control |= 4;
                }
            } else {
                return;
            }
        }
    }

    if (obj->scriptCmd == NULL) {
        obj->scriptCmd = script_getScript(0, obj->intelwalk);
        obj->scriptTimer = 0;
    }

    while (obj->scriptTimer == 0) {
        if (obj->scriptCmd->op >= 247) {
            // a label: skip its single byte
            obj->scriptCmd = (const ObjScriptCmd *)((const u8 *)obj->scriptCmd + 1);
        } else {
            gScriptArg0 = obj->scriptCmd->args[0];
            gScriptArg1 = obj->scriptCmd->args[1];
            gScriptArg2 = obj->scriptCmd->args[2];
            gScriptArg3 = obj->scriptCmd->args[3];
            gObjScriptHandlers[obj->scriptCmd->op]();
            obj->scriptCmd++;
        }
    }
}
