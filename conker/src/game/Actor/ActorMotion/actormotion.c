#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* matching: frontend.c defines save_recordHit with an s32 cause; this file was compiled with a u8 cause (with the s32 prototype
   actor_applyHit's register allocation differs), so no header declares it */
void save_recordHit(Actor *source, Actor *victim, u8 cause);

/* Animation id lists (D_80099428 goes with the dive sound in actor_updatePlayerControls). */
const u16 D_80099420[4] = { 0x215, 0x10E, 0x108, 0x249 };
const u16 D_80099428[2] = { 0xE9, 0 };
/* Eye frame for eye codes 0..2 (actor_updateTypeAnims). */
const s32 D_8009942C[3] = { 0x12, 0x0B, 0x13 };


struct HitReaction;
struct HitReaction;



/* Keeps the actor inside its zone set: pushed out of obstacle circles, snapped to the nearest allowed circle. */
void actor_clampToZoneSet(Actor *actor, s32 zoneSetId) {
    s32 pad[5]; // matching: stack frame size
    f32 bestX;
    f32 bestZ;
    f32 dist;
    f32 invDist;
    f32 dx;
    f32 dz;
    f32 segZ;
    f32 radius;
    f32 bestDist;
    f32 y;
    s32 i;
    s32 outsideAll;
    s32 haveZone;
    u16 radiusFlags;
    s32 count;
    BoundCylinder *seg;
    BoundCylinder *segs;

    outsideAll = 1;
    haveZone = 0;
    if (actor->character_type == 0x3A) {
        if (actor->xpos > 1200.0f) {
            actor->xpos = 1200.0f;
        }
        if (actor->xpos < -1200.0f) {
            actor->xpos = -1200.0f;
        }
        if (actor->zpos > 1200.0f) {
            actor->zpos = 1200.0f;
        }
        if (actor->zpos < -1200.0f) {
            actor->zpos = -1200.0f;
        }
    }
    if ((zoneSetId != 0) && (gCylinderSets != NULL) && (actor->carried < 100)) {
        bestDist = 1000000000.0f;
        i = 0;
        while (zoneSetId != gCylinderSets[i].id) {
            if (gCylinderSets[i].cylinders == NULL) {
                return;
            }
            i++;
        }
        segs = gCylinderSets[i].cylinders;
        count = gCylinderSets[i].count;
        for (i = 0; i < count; i++) {
            seg = &segs[i];
            if (outsideAll || (seg->r & 0x8000)) {
                y = actor->ypos;
                if ((seg->y <= y) && (y <= (seg->h + seg->y))) {
                    radiusFlags = seg->r;
                    radius = radiusFlags & 0x7FFF;
                    dx = actor->xpos - seg->x;
                    segZ = seg->z;
                    dz = actor->zpos - segZ;
                    radiusFlags &= 0x8000;
                    if (radiusFlags) {
                        dist = (dx * dx) + (dz * dz) + 0.1f;
                        if (dist < (radius * radius)) {
                            invDist = 1.0f / sqrtf(dist);
                            actor->xpos = seg->x + (dx * invDist * radius);
                            actor->zpos = seg->z + (dz * invDist * radius);
                            actor->mvmt_control |= 0x2000;
                        }
                    } else {
                        haveZone = 1;
                        dist = (dx * dx) + (dz * dz) + 0.1f;
                        if (dist < (radius * radius)) {
                            outsideAll = 0;
                        } else {
                            if ((sqrtf(dist) - radius) < bestDist) {
                                invDist = 1.0f / sqrtf(dist);
                                bestDist = sqrtf(dist) - radius;
                                bestX = seg->x + (dx * invDist * radius);
                                bestZ = segZ;
                                bestZ += dz * invDist * radius; // matching: split statement orders the z add after x
                            }
                        }
                    }
                }
            }
        }
        if (outsideAll && haveZone) {
            actor->xpos = bestX;
            actor->zpos = bestZ;
            actor->mvmt_control |= 0x2000;
        }
    }
}

/* Keeps the actor within taskint[1] * 8 units of its path's second point. */
void actor_clampToPathRange(Actor *actor) {
    f32 dist;
    f32 centerX;
    f32 centerZ;
    f32 maxDist;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 centerY;
    PathPoint *path;

    if (actor->taskint[1] != 0) {
        path = gSpawnPaths[actor->listpos];
        centerX = path[1].x;
        centerZ = path[1].z;
        centerY = path[1].y;
        maxDist = actor->taskint[1];
        maxDist *= 8.0f; // matching: int->float convert must be its own statement
        dx = actor->xpos - centerX;
        dz = actor->zpos - centerZ;
        dy = actor->ypos - centerY;
        dist = sqrtf((dx * dx) + (dz * dz) + (dy * dy) + 0.1000000015f);
        dx *= 1.0f / dist;
        dz *= 1.0f / dist;
        dy *= 1.0f / dist;
        if (maxDist < dist) {
            actor->xpos = dx * maxDist + centerX;
            actor->zpos = dz * maxDist + centerZ;
            actor->ypos = dy * maxDist + centerY;
        }
    }
}

/* Moves the actor with the change in waterHeight since last frame, scaled by its height above it. */
void actor_followWaterLevel(Actor *actor) {
    f32 delta;
    f32 heightFactor;

    delta = actor->waterHeight - actor->prevWaterHeight;
    actor->prevWaterHeight = actor->waterHeight;
    heightFactor = 2.0f * (actor->ypos - ((actor->waterHeight - 60.0f) - 170.0f));
    if (!(heightFactor < 0.0f) && !(fabsf(delta) > 30.0f)) {
        if (heightFactor > 300.0f) {
            heightFactor = 300.0f;
        }
        actor->ypos += heightFactor * delta * 0.002997000003f;
    }
}
/* One step of an actor's reaction sequence. */
typedef struct {
    u16 anim;
    u8 speed; /* percent */
    u8 flags; /* bit 0: copied to unk10B; & 0xFE: stun time / 100 */
} Game83300AnimSub;

/* An actor type's reaction (0x18 bytes); ANIM_HEADER(gModelAnimTables[id])->reactions points to the table. */
typedef struct Game83300AnimEntry {
    u8 kind; /* matched by actor_findReaction */
    u8 pad1[3];
    s32 sound;
    u8 unk8;
    u8 pad9;
    Game83300AnimSub sub[3];
    u8 pad16[2];
} Game83300AnimEntry;

/* Moves the actor toward its host movement_obj[unk13D - 100] and picks the carried animation. */
void actor_moveToHost(Actor *actor) {
    ActorAnim *animTable;
    Game83300AnimEntry *anims;
    MtxS16 *hostBone;
    u32 animIdx;
    f32 heightOffset;
    f32 offsetX;
    f32 offsetZ;
    f32 offsetY;
    s32 timer;
    u8 hostIdx;
    f32 lerp;
    s32 variant;
    s32 attached;
    s32 animKind;
    f32 animArg;
    f32 tx;
    f32 tz;

    hostIdx = actor->carried - 100;
    attached = 0;
    animArg = 5.0f;
    actor->imune = 30;
    variant = 0;
    animKind = 13;
    actor->floorY = movement_obj[hostIdx].floorY;
    actor->unk1CC = actor->ypos;
    actor->ygrav = 4.5f;
    actor->yspd = 0.0;
    timer = actor->unk21C;
    if (timer < 30) {
        actor->unk21C = (u32)timer + gFrameSteps; // matching: u32 cast keeps addu operand order
    }
    if (actor->character_type == 0x57) {
        actor->basemtx = 9;
        if (actor->unk21C < 10) {
            return;
        }
        attached = 1;
        if (movement_obj[hostIdx].animseq.uh == 0x114) {
            animKind = 0x24;
        }
        if (movement_obj[hostIdx].animseq.uh == 0x116) {
            variant = 1;
        }
        if (movement_obj[hostIdx].animseq.uh == 0x117) {
            variant = 2;
        }
        actor_copyAnimPosition(hostIdx, dc_glbl_CharNum);
    } else if (actor->character_type == 0x8C) {
        actor->basemtx = 4;
        if (actor->unk21C < 10) {
            return;
        }
        attached = 1;
        if (movement_obj[hostIdx].animseq.uh == 0x1A6) {
            variant = 1;
        }
        if (movement_obj[hostIdx].animseq.uh == 0x1A7) {
            variant = 2;
        }
        actor_copyAnimPosition(hostIdx, dc_glbl_CharNum);
    } else if ((actor->character_type == 0xA8) || (actor->character_type == 0xA9)) {
        actor->basemtx = 9;
        attached = 1;
        if (movement_obj[hostIdx].animseq.uh == 0x1B0) {
            variant = 1;
        }
        if (movement_obj[hostIdx].animseq.uh == 0x1B1) {
            variant = 2;
        }
    } else if (actor->character_type == 0x5E) {
        actor->basemtx = 9;
        animArg = 16.0f;
        attached = 1;
        if (movement_obj[hostIdx].animseq.uh == 0x286) {
            variant = 2;
        }
        if (movement_obj[hostIdx].animseq.uh == 0x27E) {
            variant = 1;
        }
    }
    animIdx = actor_findReaction(animKind, actor);
    animTable = gModelAnimTables[actor->character_type];
    anims = ANIM_HEADER(animTable)->reactions;
    if ((movement_obj[hostIdx].type == 1) && (movement_obj[hostIdx].inwater != 0)) {
        actor->basemtx = 4;
        attached = 1; if (anims[animIdx].sub[1].anim != 0) { // matching: same line (as1 tie-break)
            variant = 1;
        }
    }
    if (actor->unk21C < 20) {
        timer = actor->unk21C;
        lerp = (u32)timer / 50.0f;
        if (gLevelLoadState != 0) {
            lerp = 1.0f;
        }
    } else {
        lerp = 0.8000000119f;
    }
    offsetX = actor->unk10F;
    actor_turnFacing(actor, 0x10, movement_obj[hostIdx].langle + (actor->unk13E << 8));
    if (movement_obj[hostIdx].ob_matrix_ptr != NULL) {
        heightOffset = actor->unk10C / 100;
        if (movement_obj[hostIdx].type != 0x15) {
            hostBone = (MtxS16 *)movement_obj[hostIdx].ob_matrix_ptr + 9;
            if (actor->unk21C >= 7) {
                actor->ypos = hostBone->intPart[3][1];
                if (actor->unk21C < 14) {
                    heightOffset = ((u32)(s32)actor->unk21C / 14.0f) * heightOffset;
                }
                actor->ypos += heightOffset;
            }
            if (actor->character_type == 0xE) {
                actor->lean_angle = 0.0;
            } else if (actor->character_type == 0x13) {
                offsetX = 204.0f;
                actor->lean_angle = 0.0;
            }
        }
    }
    if (movement_obj[hostIdx].type == 0x15) {
        animIdx = actor_findReaction(0x39, actor);
        actor->langle = movement_obj[hostIdx].langle;
        actor->ypos = movement_obj[hostIdx].ypos - 10.0f;
        if ((actor->shad > 100.0f) && (actor->unk21C >= 21)) {
            variant = 1;
        } else {
            animArg = 0.0;
        }
        offsetX = 0.0f;
        actor->lean_angle += (movement_obj[hostIdx].lean_angle - actor->lean_angle) * 0.1199999973f;
        actor->speed = 0.0f;
    }
    actor_splitVelocity(movement_obj[hostIdx].langle, offsetX, 0.0, &offsetX, &offsetZ, &offsetY);
    tx = movement_obj[hostIdx].xpos + offsetX;
    tz = movement_obj[hostIdx].zpos + offsetZ;
    actor->xpos += (tx - actor->xpos) * lerp;
    actor->zpos += (tz - actor->zpos) * lerp;
    {
        f32 rate;

        rate = anims[animIdx].sub[variant].speed;
    split: // matching: block boundary moves the u32->f32 convert's register (f16 instead of f0)
        rate = rate / 100.0f;
        actor_playAnim(actor, anims[animIdx].sub[variant].anim, rate, animArg, 0.0, 0, 0);
    }
    if (attached != 0) {
        actor->attach_char = hostIdx + 1;
        actor->unk101 |= 4;
    } else {
        actor->attach_char = 0;
    }
}

/* Whether actor movement_obj[targetIdx] is inside the circle (radius * 8) around the actor's first path point. */
s32 actor_isTargetInPathArea(Actor *actor, s32 targetIdx) {
    f32 radiusSq;
    f32 dz;
    f32 dx;
    PathPoint *path = gSpawnPaths[actor->listpos];

    if (path[0].param == 0) {
        return TRUE;
    }

    radiusSq = path[0].param * 8;
    radiusSq = radiusSq * radiusSq;
    dx = path[0].x - movement_obj[targetIdx].xpos;
    dz = path[0].z - movement_obj[targetIdx].zpos;

    if ((dx * dx) + (dz * dz) < radiusSq) {
        return TRUE;
    }
    return FALSE;
}

/* Neither caller reads a return value. */
void actor_startTurnAnim(Actor *actor, u8 turn, u8 row) {
    u8 col;
    u16 angle;
    const u8 *entry;

    col = 0;
    if (gTurnAnimTable[row][7] < turn) {
        col = 5;
    } else if (gTurnAnimTable[row][2] < turn) {
        col = 2;
    }
    angle = actor->angle;
    if ((actor->fangle - angle) & 0x8000) {
        col += 3;
    } else {
        col += 4;
    }
    entry = &gTurnAnimTable[row][col];
    if (*entry != 0xFF) {
        actor->scriptCmd--;
        actor->unk21C = 20000;
        actor->unk223 = 0xD;
        actor->stickspeed = 0.0f;
        actor->intel_animseq = *entry;
        actor->mvmt_control &= ~0xE;
        actor->mvmt_control |= 4;
        actor->fangle = angle;
        actor->langle = angle;
        actor->speccnt = 0;
    }
}

/*
 * Per-frame AI for a path/chase actor: picks the point to head for from its mode (unk223),
 * looks for targets to switch behaviour script (unk218), then turns and moves toward the point.
 */
void actor_updateChaseAi(Actor *actor) {
    f32 tx;
    f32 tz;
    f32 ty;
    s32 state;
    f32 rangeSq;
    f32 dist;
    u8 targetIdx;
    u8 mode;
    f32 accel;
    f32 decel;
    u8 turnDiff;
    u16 cooldown;
    f32 speed;
    u16 moveTurnSpeed;
    u8 faceTurnRate;
    f32 targetDist;
    Actor *target;
    f32 arriveDist;
    u8 scanForTargets;
    ModelDef *info;
    f32 dx;
    f32 dz;
    f32 distSqXZ;

    scanForTargets = 1;
    if (actor->unk23A) {
        actor->scriptCmd = script_getScript(4, actor->unk23A);
        actor->unk21C = 0;
        actor->unk23A = 0;
    }
    if ((gCutsceneActorLock != 0) && (cutscene_isActorBound(actor) != 0)) {
        return;
    }
    if ((actor->mvmt_control & 0x1000) && (actor->shad < 0.200000003f)) {
        actor->unk21C = 0;
    }
    script_runActor(actor);
    mode = actor->unk223;
    if ((mode == 0xE) || (mode == 0xF) || (mode == 0x10)) {
        mode = 1;
    }
    if (mode == 0x11) {
        mode = 0;
    }
    if (actor->stunned != 0) {
        return;
    }
    moveTurnSpeed = (((actor->unk1E5 << 8) + actor->unk1E7) * gActorStepPercent) / 100;
    faceTurnRate = actor->unk1E6;
    if (gSpawnPathCounts[actor->listpos] == 0) {
        actor->speed = 0.0f;
        actor_playMoveAnim(actor, 0.0f);
        return;
    }
    state = mode;
    info = gModelDefs[actor->character_type];
    if (actor->unk23D == 0xFF) {
        rangeSq = 1000000000.0f;
    } else {
        rangeSq = actor->unk23D * 8;
        rangeSq *= rangeSq;
    }
    if (gSpawnPaths[actor->listpos]->param != 0) {
        tx = gSpawnPaths[actor->listpos]->x - actor->xpos;
        tz = gSpawnPaths[actor->listpos]->z - actor->zpos;
    }
    if ((actor->task_attrib & 4) && !(actor->task_attrib & 1) && ((state == 1) || (state == 3))) {
        actor->unk21C = 0;
    }
    if ((actor->task_attrib & 1) && ((state == 0) || (state == 5))) {
        if (actor_isTargetInPathArea(actor, actor->unk259[2]) != 0) {
            actor->scriptCmd = script_getScript(1, actor->unk233);
            actor->unk21C = 0;
            actor->unkA8 = actor->unk259[2] | 0x80;
            actor->unk222 = actor->unk259[2];
        }
    }
    if (actor->unk259[1] != 0) {
        actor->stickspeed = actor->unk259[1];
    }
    if ((state == 1) || (state == 0xC) || (state == 2) || (state == 3) || (state == 9) || (state == 0xA) ||
        ((state == 6) && !(actor->unk22C & 1))) {
        targetIdx = actor->unk222;
        target = &movement_obj[targetIdx];
        tx = target->xpos;
        tz = target->zpos;
        ty = target->ypos;
        if (actor->unk223 == 0x10) {
            f32 leadX;
            f32 leadZ;
            f32 leadY;

            leadZ = actor_distance3D(target, actor);
            leadX = target->speed * (leadZ / actor->unk231);
            actor_splitVelocity(target->angle, leadX, 0.0f, &leadX, &leadZ, &leadY);
            tx += leadX;
            tz += leadZ;
        }
        dx = tx - actor->xpos;
        dz = tz - actor->zpos;
        dist = sqrtf((dx * dx) + (dz * dz));
        if ((state == 1) && (actor->stickspeed > 1.0f) && (sqrtf(rangeSq) < (dist * 0.8000000119f))) {
            actor->unk21C = 0;
        }
        if (actor->character_type != 0x16) {
            if ((state == 1) && (actor->unk234 != 0) && ((dist < 370.0f) || (actor->character_type == 0x21)) && (targetIdx < 4) &&
                ((actor->speed > 15.0f) || (actor->character_type == 0x53))) {
                if (target->mv != NULL) {
                    target->mv->chased = 10;
                }
            }
            if ((state == 2) && (dist < 470.0f) && (targetIdx < 4) && (((actor->angle - target->angle) - 0x4000) & 0x8000)) {
                if (target->mv != NULL) {
                    target->mv->chasing = 6;
                }
            }
        }
        if (state != 9) {
            f32 range;
            s32 ok;
            u16 angle;

            if ((state != 0xA) && (state != 0xC)) {
                scanForTargets = 0;
            }
            if ((actor->unk22C & 0x80) &&
                (fabsf((actor->ypos - actor->shad) - (target->ypos - target->shad)) > 30.0f)) {
                actor->unk21C = 0;
            }
            if (actor->unk223 != 0xF) {
                if (actor_isTargetInPathArea(actor, actor->unk222) == 0) {
                    actor->unk21C = 0;
                }
            }
            if (actor->unk22E != 0) {
                range = 0.0f;
            } else if (!actor->unk236) {
                range = actor->unk23C * 8;
            } else {
                range = (actor->unk236 == 0xFF) ? 3000000.0f : actor->unk236 * 8;
            }
            if (((state == 1) || (state == 6) || (state == 0xA)) && (actor->unk234 != 0) && (dist < range)) {
                ok = 1;
                if ((actor->character_type == 8) && (range < fabsf(actor->ypos - target->ypos))) {
                    ok = 0;
                }
                if (actor->unk22A != 0) {
                    range = actor->unk22A * 8.0f;
                    if ((dc_glbl_CharNum != 0) && (fabsf(actor->ypos - movement_obj[0].ypos) < range)) {
                        ok = 0;
                    }
                }
                if (ok) {
                    angle = actor_getDirectionAngle(dx, -dz, 0);
                    if (actor->unk237 &&
                        (((((actor->angle - angle) >> 8) + (actor->unk237 / 2)) & 0xFF) < actor->unk237)) {
                        actor->scriptCmd = script_getScript(2, actor->unk234);
                        actor->unk21C = 0;
                    }
                }
            }
        }
    }
    if (scanForTargets != 0) {
        s32 alertKind;

        cooldown = actor->unk22E;
        if (actor->unk235 != 0) {
            alertKind = 0;
        } else {
            alertKind = 1;
        }
        if (cooldown != 0) {
            if (gFrameSteps >= cooldown) {
                actor->unk22E = 0;
            } else {
                actor->unk22E = cooldown - gFrameSteps;
            }
        }
        if (actor->unk241 != 0) {
            s32 i;
            s32 best;
            f32 bestDist;
            f32 ox;
            f32 oz;
            f32 d;
            s32 cone;
            s32 angHi;

            best = 0;
            bestDist = actor->unk23F * 8;
            for (i = 0; i < 25; i++) {
                target = &movement_obj[i];
                if ((target->type != 0) && (target->character_type == actor->unk240) && (target->activeFlags != 0)) {
                    ox = target->xpos - actor->xpos;
                    oz = actor->zpos - target->zpos;
                    d = sqrtf((ox * ox) + (oz * oz));
                    if (d < bestDist) {
                        cone = info->viewAngle * 3;
                        cone /= 2;
                        angHi = (u8)(actor_getDirectionAngle(ox, oz, 0) >> 8);
                        if ((((((actor->langle >> 8) - angHi) + (cone / 2)) & 0xFF) < cone)) {
                            best = i + 1;
                            bestDist = d;
                        }
                    }
                }
            }
            if (best != 0) {
                actor->scriptCmd = script_getScript(0, actor->unk241);
                actor->unk21C = 0;
                actor->unk222 = best - 1;
                actor->unkA8 = best + 0x7F;
                actor->unk22E = 5;
                if (actor->unk242 != 0) {
                    gCurrentActor = &movement_obj[best - 1];
                    movement_obj[best - 1].scriptCmd = script_getScript(0, actor->unk242);
                    movement_obj[best - 1].unk21C = 0;
                    movement_obj[best - 1].unk222 = dc_glbl_CharNum;
                    gCurrentActor = actor;
                }
            }
        }
        if ((actor->unk22E == 0) && (state != 4) && (state != 7) &&
            (((alertKind == 0) && (actor->unk235 != 0)) ||
             ((alertKind == 1) && (actor->unk233 != 0) && !(actor->task_attrib & 4)))) {
            u8 spotted;
            f32 px;
            f32 pz;
            f32 dsq;
            u8 pad; // matching: stack slot

            if (info->unk18 <= actor->speed) {
                if (actor_isTargetInPathArea(actor, 0) != 0) {
                    spotted = 0;
                    px = movement_obj[0].xpos - actor->aim.x;
                    pz = actor->aim.z - movement_obj[0].zpos;
                    if ((actor->character_type == 0x9E) && (actor->taskint[0] == 0)) {
                        rangeSq = 129600.0f;
                    }
                    dsq = (px * px) + (pz * pz);
                    if ((dsq < rangeSq) && (dsq < 100000000.0f)) {
                        if (actor_isTargetInPathArea(actor, 0) != 0) {
                            if (actor->unk229 == 0xFF) {
                                dsq = sqrtf(rangeSq);
                            } else {
                                dsq = actor->unk229 * 8.0f;
                            }
                            if ((dc_glbl_CharNum != 0) &&
                                (!actor->unk229 || (fabsf(actor->ypos - movement_obj[0].ypos) < dsq))) {
                                if (actor->character_type == 0x9E) {
                                    MtxF mtx;
                                    f32 lookX;
                                    f32 lookZ;
                                    f32 lookY;

                                    actor_splitVelocity(actor->langle, 9000.0f, actor->vtilt, &lookX, &lookZ, &lookY);
                                    lookX += actor->xpos;
                                    lookZ += actor->zpos;
                                    lookY += actor->ypos;
                                    guLookAtF(mtx, actor->xpos, actor->ypos, actor->zpos, lookX, lookY,
                                                  lookZ, 0.0f, 1.0f, 0);
                                    if (math_isBoxInView(mtx, movement_obj[0].xpos, movement_obj[0].ypos, movement_obj[0].zpos, 80.0f, 80.0f, 0.0f,
                                                      4000.0f, 320.0f, 320.0f) != 0) {
                                        spotted = 1;
                                    }
                                } else {
                                    s32 fov;

                                    fov = info->viewAngle;
                                    if (alertKind == 0) {
                                        fov *= 3;
                                    }
                                    if (((((((actor->langle >> 8) + actor->unk2CA) - (u8)(actor_getDirectionAngle(px, pz, 0) >> 8)) +
                                           (fov / 2)) & 0xFF) < fov)) {
                                        spotted = 1;
                                    }
                                }
                            }
                        }
                    }
                    if (spotted) {
                        if (alertKind == 0) {
                            actor->scriptCmd = script_getScript(3, actor->unk235);
                        } else {
                            actor->scriptCmd = script_getScript(1, actor->unk233);
                        }
                        actor->unk21C = 0;
                        actor->unk222 = spotted - 1;
                        actor->unkA8 = spotted + 0x7F;
                    }
                }
            }
        }
        if (state != 9) {
            if ((state == 6) || (state == 0xC)) {
                tx = movement_obj[actor->unk222].xpos;
                tz = movement_obj[actor->unk222].zpos;
                ty = movement_obj[actor->unk222].ypos;
            } else {
                tx = gSpawnPaths[actor->listpos][actor->unk21E + 1].x;
                tz = gSpawnPaths[actor->listpos][actor->unk21E + 1].z;
                ty = gSpawnPaths[actor->listpos][actor->unk21E + 1].y;
            }
            if ((state == 5) || (state == 6)) {
                f32 orbitX;
                f32 orbitZ;
                f32 orbitY;

                if ((state == 6) && (actor->unk22C & 1)) {
                    actor->unk86 = movement_obj[actor->unk222].langle >> 8;
                }
                actor->unk86 += actor->unk257;
                orbitX = actor->unk258 * 16.0f;
                actor_splitVelocity(actor->unk86 << 8, orbitX, 0.0f, &orbitX, &orbitZ, &orbitY);
                tx += orbitX;
                tz += orbitZ;
            }
        }
    }
    speed = actor->stickspeed;
    if ((state != 7) && (state != 8)) {
        tx = tx - actor->xpos;
        tz = actor->zpos - tz;
        ty = actor->ypos - ty;
        if ((state == 6) && (actor->unk22C & 1) && ((fabsf(tx) + fabsf(tz)) < 100.0f)) {
            speed = 0.0f;
            tx = movement_obj[actor->unk222].xpos - actor->xpos;
            tz = actor->zpos - movement_obj[actor->unk222].zpos;
            actor->fangle = actor_getDirectionAngle(tx, tz, 0);
        } else if ((actor->stickspeed >= 0.5f) && !(actor->mvmt_control & 0x40)) {
            actor->fangle = actor_getDirectionAngle(tx, tz, 0);
        }
    }
    accel = actor->intel_accel / 10.0f;
    distSqXZ = (tx * tx) + (tz * tz);
    targetDist = distSqXZ;
    if (actor->unk22C & 8) {
        targetDist += ty * ty;
    }
    targetDist = sqrtf(targetDist);
    if ((state != 4) && (state != 7) && (state != 8)) {
        u8 k;
        u8 n;
        u16 prevMoveAngle;
        u16 ang;
        s32 flags;

        prevMoveAngle = actor->angle;
        if (state == 2) {
            actor->fangle ^= 0x8000;
            if (!(actor->mvmt_control & 0x4000)) {
                actor->fangle += (((actor->unk21C / 100) & 0x20) - 0x10) << 8;
            }
        }
        if (actor->unk223 == 0x11) {
            actor->fangle += (((actor->unk21C / 100) & 0x10) - 8) << 9;
        }
        if ((actor->mvmt_control & 0x200) && (actor->unk1F4 < 3.0f)) {
            moveTurnSpeed = 0;
            faceTurnRate = 0;
        }
        arriveDist = actor->unk23E * 8;
        turnDiff = actor_turnMoving(actor, moveTurnSpeed);
        if (state != 0xD) {
            if (((u8)actor->unk24F != 0) && ((state != 0) || (arriveDist < targetDist))) {
                k = (u8)actor->unk24F - 1;
                if (gTurnAnimTable[k][0] < turnDiff) {
                    actor->angle = prevMoveAngle;
                    actor_startTurnAnim(actor, turnDiff, k);
                }
            }
        }
        if (state == 0xD) {
            moveTurnSpeed = 0;
            faceTurnRate = 0;
        }
        if ((info->unk19 < turnDiff) && (state != 2)) {
            speed = 0.0f;
            if (actor->unk24A && (actor->speed > 15.0f)) {
                actor->scriptCmd = script_getScript(6, actor->unk24A);
                actor->unk21C = 0;
                actor->unk222 = 0;
            }
        }
        if (((state == 0) || (state == 0xA)) && (targetDist < arriveDist)) {
            n = gSpawnPathCounts[actor->listpos] - 1;
            if (actor->unk21F != 0) {
                n = actor->unk21F;
            }
            if (actor->unk22C & 4) {
                actor->xpos = gSpawnPaths[actor->listpos][actor->unk21E + 1].x;
                actor->zpos = gSpawnPaths[actor->listpos][actor->unk21E + 1].z;
                actor->unk1FC |= 8;
                if (actor->unk22C & 8) {
                    actor->ypos = gSpawnPaths[actor->listpos][actor->unk21E + 1].y;
                }
            }
            if (!(actor->mvmt_control & 0x10)) {
                actor->unk21C = 0;
            }
            actor->unk21E += actor->unk221;
            actor->unk21E += n;
            actor->unk21E %= n;
            if (actor->unk21E < actor->unk220) {
                actor->unk21E = actor->unk220;
            }
        }
        ang = actor->fangle;
        if (actor->mvmt_control & 0x20000) {
            ang = actor_getDirectionAngle(movement_obj[actor->unk222].xpos - actor->xpos,
                                actor->zpos - movement_obj[actor->unk222].zpos, 0);
        }
        turnDiff = actor_turnFacing(actor, faceTurnRate, ang);
        decel = actor->intel_decel / 10.0f;
        if ((actor->unk3A != 0) && (actor->shad == 0.0f) && !(actor->unk3A & 3 & gActorFrameCount)) {
            actor->animseq.uh = 0xFF;
            actor->yspd = actor->unk3A;
        }
        flags = actor->mvmt_control;
        if (flags & 1) {
            speed = 0.0f;
        }
        if ((actor->unk22C & 0x10) && (actor->shad == 0.0f)) {
            speed = 0.0f;
            accel = 1.0f;
        }
        if (actor->type == 1) {
            actor->stickspeed = speed;
            return;
        }
        if (flags & 0x80000) {
            f32 h;

            h = ty + 70.0f;
            if (level == 0x26) {
                h -= 45.0f * actor->scale_y;
            }
            ang = actor_getDirectionAngle(sqrtf(distSqXZ), h, 0);
            actor_stepAimPitch(actor, (s16)ang / 200, 0);
        } else if (actor->aimPitch != 0) {
            actor_stepAimPitch(actor, 0, 0);
        }
        if (actor->noshove & 0x40000) {
            f32 wantVelX;
            f32 wantVelZ;
            f32 wantVelY;
            f32 r;

            speed = actor->stickspeed;
            actor_splitVelocity(actor->fangle, speed, 0.0f, &wantVelX, &wantVelZ, &wantVelY);
            if ((speed < 1.0f) || (((prevMoveAngle - actor->fangle) + 0x4000) & 0x8000)) {
                r = decel;
            } else {
                r = accel;
            }
            actor_stepVector2D(wantVelX, wantVelZ, r, &actor->unk174, &actor->unk178);
            actor->angle = actor_getDirectionAngle(actor->unk174, -actor->unk178, 0);
            r = sqrtf((actor->unk174 * actor->unk174) + (actor->unk178 * actor->unk178));
            actor->speed = r + r;
        } else {
            actor_accelerate(speed, actor, accel, decel, turnDiff);
        }
    } else {
        actor_turnFacing(actor, actor->unk1E6, actor->fangle);
        actor->angle = actor->fangle;
        actor->speed = speed;
    }
    actor_playMoveAnim(actor, accel);
    if ((actor->unk22C & 8) && !(actor->unk1FC & 8)) {
        f32 a;
        f32 lim;

        a = actor_getDirectionAngle(sqrtf(distSqXZ), ty, 0) * 0.005493164f;
        if (a > 180.0f) {
            a -= 360.0f;
        }
        a = (a - actor->vtilt) * 0.25f;
        lim = moveTurnSpeed * 0.005493164f;
        if (a > lim) {
            a = lim;
        }
        if (a < -lim) {
            a = -lim;
        }
        actor->vtilt = actor->vtilt + a;
    }
}
/* Plays the actor's movement animation (unk244) at a rate derived from its speed, or at `idleRate`. */
void actor_playMoveAnim(Actor *actor, f32 idleRate) {
    AnimInstance *anim;
    f32 speed;
    f32 fade;
    f32 rate;
    f32 scaledSpeed;
    s32 flags;

    flags = actor->intel_animspd;
    rate = (((flags & 0x1F) << 8) + actor->unk249) * 0.0003906250058f;
    if (!(flags & 0x80)) {
        speed = actor->speed;
        if ((speed <= 1.0f) || ((actor->unk22C & 0x10) && (actor->shad == 0.0f))) {
            rate = idleRate;
        } else {
            scaledSpeed = speed;
            scaledSpeed *= 0.5f / actor->scale_xz;
            rate *= 10.0f;
            rate = scaledSpeed / rate + 0.1000000015f;
            if (!(flags & 0x20)) {
                rate += 0.400000006f;
            }
        }
    }
    if (actor->unk223 == 0xD) {
        rate = (actor->unk250 & 0x7F) * 0.1000000015f;
    }
    if (flags == 0xFF) {
        rate = 0.0f;
    }
    actor_playAnim(actor, actor->intel_animseq, rate, actor->unk1D0, 0.0f, 0.0f, 0);
    if (actor->intel_animspd == 0xFF) {
        fade = gMusicEventLevels[gLastMarkerChannel][0];
        if (fade >= 0.0f) {
            anim = actor->unk2D0;
            anim->frame[0] = (anim->numFrames[0] * (32768.0f - fade)) / 32768.0f;
        }
    }
}

/* Per-frame update while the actor is stunned (stunned is the countdown; 255 = indefinitely). */
void actor_updateStunned(Actor *actor, f32 decel) {
    ActorCtl *child;
    f32 yVel;
    s32 water;
    s32 stun;
    s32 flags;
    s32 facing;
    u16 angle;
    s32 owner;

    decel = actor->unk109 * 0.1000000015f;
    actor_accelerate(0.0f, actor, 1.0f, decel, 0);
    if (actor->unk1CC < 100000.0f) {
        actor->unk1CC = actor->ypos;
    }
    water = actor->inwater;
    if (water != 0 && water < 10) {
        yVel = actor->yspd;
        actor->ygrav = 0.0f;
        if (yVel < 60.0f) {
            actor->yspd = yVel * 0.8999999762f;
        }
        if ((actor->waterHeight - 60.0f) + 40.0f < actor->ypos) {
            actor->count3 = 0;
            actor->count5 = 0;
            actor->inwater = 0;
            actor->vtilt = 0.0f;
            actor->ygrav = 4.0f;
        }
    }
    if (gIsMultiplayer == 0 || actor->type != 1) {
        actor->unk21C = 0;
    }
    stun = actor->stunned;
    if (stun != 255) {
        if (stun != 254 && (actor->energy != 0 || actor->type != 1)) {
            actor->stunned = stun - 1;
        }
        actor->unk10C -= gActorStepPercent;
        if (actor->shad < 0.200000003f && (actor->mvmt_control & 0x100)) {
            actor->unk10C = 0;
        }
        if (actor->unk10C <= 0) {
            child = actor->mv;
            if (child != NULL) {
                child->matrix_physics = 0;
            }
            actor->unk10C = 0;
            actor_advanceReaction(dc_glbl_CharNum, actor);
        }
        if (actor->stunned == 0) {
            facing = actor->langle;
            actor->speed = 0.0f;
            actor->count3 = 0;
            actor->fangle = facing;
            actor->angle = facing;
            if (actor->inwater != 0) {
                actor->yspd = 0.0f;
                if (actor->type == 1) {
                    func_1506B078();
                }
            } else if (actor->type == 1) {
                actor->ygrav = 4.0f;
                actor->noshove &= ~0x8000;
            }
            owner = actor->unk238;
            if (owner != 0) {
                actor->unk23A = owner;
            }
        } else {
            flags = actor->unk10B;
            if (!(flags & 2)) {
                angle = actor->angle;
                if (flags & 4) {
                    angle ^= 0x8000;
                }
                actor->count2 = 10;
                actor_turnFacing(actor, 12, angle);
            }
        }
    }
}

/* Updates the actor's floor height (unk180) and pushes it out of walls, reacting to a wall hit. */
void actor_updateFloorAndWalls(Actor *actor, f32 prevY) {
    f32 groundY;
    s32 hostIdx;
    s32 fastFloor;
    s32 flags;
    f32 *floorOut;

    actor->waterHeight = -10000.0f;
    cutscene_applyObjectTrack(actor, 0, -1);
    if (actor->noshove & 0x8000) {
        if (actor->drawtype == 7) {
            actor->shad = 0.0f;
        } else {
            actor->shad = 1.0f;
        }
        return;
    }
    if (actor->noshove & 0x200000) {
        actor->floorY = actor->unk224;
        return;
    }
    fastFloor = actor->noshove & 2;
    hostIdx = actor->lifting;
    flags = hostIdx - 100;
    if ((hostIdx >= 100) && (flags == 0)) {
        fastFloor = movement_obj[flags].noshove & 2;
    }
    if (!fastFloor ||
        !collision_getTriHeightAt(actor->xpos, actor->zpos, actor->floorTriVerts, actor->floorTri.v, &groundY)) {
        if (actor->noshove & 8) {
            floorOut = &actor->waterHeight;
        } else {
            floorOut = NULL;
        }
        collision_findFloorEx(&actor->floorTriVerts, actor->floorTri.v, &groundY, floorOut, &actor->floorMaterial, &actor->floorType,
                      actor->xpos, actor->ypos, actor->zpos, prevY, 0, actor, -10000.0f,
                      actor->ypos, actor->noshove & 0x04000000);
    } else if (actor->noshove & 8) {
        actor->waterHeight = collision_getPropSurfaceHeight(actor->xpos, actor->ypos, actor->zpos);
    }
    flags = actor->noshove;
    if ((actor->lifting != 0) && (actor->character_type != 0x9A)) {
        flags = movement_obj[actor->lifting - 100].noshove;
    }
    if ((flags & 4) && (actor->carried == 0) &&
        ((actor->playernumber != 0xFF) || (actor->xpos != actor->old_xpos) ||
         (groundY != actor->old_ypos) || (0.0f != actor->shad) ||
         (actor->zpos != actor->old_zpos))) {
        f32 x;
        f32 z;

        x = actor->xpos;
        z = actor->zpos;
        if ((actor->character_type == 0x1B) || (actor->character_type == 0x7E)) {
            D_800CBDD3 = 1;
        }
        collision_resolveActorWalls(actor->old_xpos, actor->old_ypos, actor->old_zpos, actor, 1, 0);
        if ((actor->character_type == 0x1B) || (actor->character_type == 0x7E)) {
            D_800CBDD3 = 0;
        }
        if ((x != actor->xpos) || (z != actor->zpos)) {
            actor->mvmt_control |= 0x10000;
            if (actor->unk24E != 0) {
                if (actor->character_type == 0xC) {
                    particle_spawnMoveHitBurst(actor, x, z, 0xFF, 0);
                }
                actor->intelwalk = actor->unk24E;
                actor->unk24E = 0;
                actor->scriptCmd = NULL;
                actor->fangle = (actor->langle = actor->angle);
            }
            if (actor->noshove & 0x1000) {
                /* bounce: reflect the moving angle about the push-out direction */
                u16 angle = actor_getDirectionAngle(actor->xpos - x, z - actor->zpos, 0);

                actor->angle = ((angle - actor->angle) + angle) + 0x8000;
                if (actor->type == 0x21) {
                    actor->speed *= 0.8999999762f;
                } else if (actor->type == 0x26) {
                    actor->speed *= 0.3000000119f;
                } else if ((actor->type != 4) || (actor->unk103 == 0)) {
                    actor->speed *= 0.3000000119f;
                } else {
                    u16 freeAngle;

                    actor->speed *= 0.8999999762f;
                    actor->langle = actor->angle;
                    if (actor_findTargetInCone(actor, &freeAngle, 0, actor->owner - 1, 20) != 0xFF) {
                        actor->angle = freeAngle;
                    }
                }
                if ((actor->unk2CC != 0) && (actor->speed > 5.0f)) {
                    gEventParam = actor->unk2CC;
                    event_playSound2Range1800();
                }
                if (actor->type == 4) {
                    actor->unk102 = 0;
                    actor->ygrav = 3.0f;
                }
            }
            if (!(actor->noshove & 2) ||
                !collision_getTriHeightAt(actor->xpos, actor->zpos, actor->floorTriVerts, actor->floorTri.v, &groundY)) {
                collision_findFloorEx(&actor->floorTriVerts, actor->floorTri.v, &groundY, &actor->waterHeight, &actor->floorMaterial,
                              &actor->floorType, actor->xpos, actor->ypos, actor->zpos, prevY, 0,
                              actor, -10000.0f, actor->ypos, actor->noshove & 0x04000000);
            }
        }
    }
    {
        u8 fallScript = actor->unk255;

        if (1) { // matching: the extra block makes IDO pick v0 for fallScript
            if ((fallScript != 0) && (actor->stunned == 0) &&
                (groundY < (actor->floorY - (actor->unk256 * 8)))) {
                actor->yspd = 0.0f;
                actor->ygrav = 0.0f;
                actor->speed = 0.0f;
                actor->scriptCmd = script_getScript(0, actor->unk255);
                actor->unk21C = 0;
                actor->unk255 = 0;
                return;
            }
        }
    }
    actor->floorY = groundY;
}

/* Picks gravity by height band and clamps the vertical velocity. */
void actor_setHoverGravity(Actor *actor, f32 maxY, f32 gravityAbove, f32 minY, f32 gravityBelow, f32 maxVelY, f32 minVelY) {
    if (maxY < actor->ypos) {
        actor->ygrav = gravityAbove;
    } else if (actor->ypos < minY) {
        actor->ygrav = gravityBelow;
    }
    if (maxVelY < actor->yspd) {
        actor->yspd = maxVelY;
    } else if (actor->yspd < minVelY) {
        actor->yspd = minVelY;
    }
}

/* Floats the actor on the water surface (waterHeight + 9); `depth` in [0, 1) sets how low it sits. */
void actor_floatOnWater(Actor *actor, f32 depth, f32 arg2) {
    f32 tmp; // matching: one variable for the damping, offset and sink limit (IDO allocates it as one)
    f32 surfaceY;
    f32 lift;

    tmp = depth;
    lift = 0.5f - depth;
    if (depth >= 0.5f) {
        depth -= 0.5f;
    }
    if (arg2) {} // matching: a use of the unused arg2 keeps it out of its stack slot
    surfaceY = actor->waterHeight + 9.0f;
    if ((actor->ypos < (actor->waterHeight + 9.0f)) || (actor->inwater >= 100)) {
        if (actor->inwater == 0) {
            actor->ypos = surfaceY;
            actor->inwater = 100;
            actor->ygrav = lift * -6.0f;
            actor->yspd *= tmp;
        } else {
            if ((surfaceY + 100.0f) < actor->ypos) {
                actor->inwater = 0;
            }
            if (lift < 0.0f) {
                actor->ygrav = lift * -6.0f;
            } else {
                tmp = actor->ypos - ((surfaceY + 10.0f) - (120.0f * depth));
                if ((fabsf(tmp) < 2.0f) && (fabsf(actor->yspd) < 2.0f)) {
                    actor->ypos -= tmp * 0.6000000238f;
                    actor->yspd = 0.0f;
                    actor->ygrav = 1.0f;
                } else if (tmp > 0.0f) {
                    if (actor->yspd > 0.0f) {
                        actor->ygrav = lift * 6.0f;
                        actor->yspd /= 2.0f;
                    }
                } else {
                    tmp = lift * 80.0f;
                    if (tmp < actor->yspd) {
                        actor->yspd = tmp;
                    }
                    actor->ygrav = lift * -6.0f;
                }
            }
        }
        tmp = -100.0f * depth;
        if (actor->yspd < tmp) {
            actor->yspd = tmp;
        }
    }
}

/* Per-frame movement/physics step for a non-player actor. */
void actor_updatePhysics(Actor *actor) {
    f32 stepRate;

    stepRate = 0.5f;
    gCollidedActors = 0;
    if ((actor->carried == 0) || (actor->unk21C < 16)) {
        if (actor->unk223 != 8) {
            actor_moveHorizontal(actor);
            actor_applyGravity(actor);
        }
    }
    actor_resolveCollisions(actor, dc_glbl_CharNum);
    actor_clampToZoneSet(actor, actor->family);
    if (actor->character_type == 8) {
        actor_clampToPathRange(actor);
    }
    if (actor->carried < 100) {
        actor_updateFloorAndWalls(actor, actor->old_ypos);
    }
    if (actor->mvmt_control & 0x40000) {
        if (((actor->mvmt_control & 0x12000) != 0) || (gCollidedActors != 0)) {
            actor->unk21C = 0;
        }
    }
    if (actor->unkB0 != 0) {
        actor_floatOnWater(actor, actor->unkB0 * 0.009999999776f, 1.0f);
    }
    if (actor->noshove & 0x20000) {
        actor_followWaterLevel(actor);
    }
    if (actor->inwater != 0) {
        actor_applyWaterCurrent(actor);
    }
    if (actor->character_type == 0xA) {
        actor->floorY = -5000.0f;
    }
    if ((actor->noshove & 0x180000) != 0) {
        actor_tiltToGround(actor);
    }
    actor_landNonPlayer(actor, actor->floorY);
    if (((actor->mvmt_control & 0x1000) != 0) && (actor->shad < 0.200000003f)) {
        actor_updateChaseAi(actor);
    }
    if (actor->noshove & 0x80000) {
        stepRate = 0.150000006f;
    }
    actor_stepVector2D(0.0f, 0.0f, stepRate, &actor->ob_slopex, &actor->ob_slopey);
    if (actor->doing_hit != 0) {
        actor_testAttack(actor, dc_glbl_CharNum);
    }
    if (actor->carried == 0) {
        if (gFrameSteps >= actor->unk10F) {
            actor->unk10F = 0;
        } else {
            actor->unk10F -= gFrameSteps;
        }
    }
    if (actor->ivehitit != 0) {
        actor->ivehitit -= 1;
    }
    if (actor->imune != 0xFF) {
        if (gFrameSteps >= actor->imune) {
            actor->imune = 0;
        } else {
            actor->imune -= gFrameSteps;
        }
    }
}

/* Eases the actor's push velocity (unk16C, unk170) toward `speed` along `angle`. */
void actor_easePushVelocity(Actor *actor, u16 angle, f32 speed, f32 rate) {
    f32 x;
    f32 z;
    f32 y;

    actor_splitVelocity(angle, speed, 0.0f, &x, &z, &y);
    actor->ob_overslopex += (x - actor->ob_overslopex) * rate;
    actor->ob_overslopey += (z - actor->ob_overslopey) * rate;
}

/* Applies the current map's (level) water current to a swimming actor. */
void actor_applyWaterCurrent(Actor *actor) {
    s32 pathIdx;
    s32 id;
    s32 tmp;

    pathIdx = -1;

    switch (level) {
        case 4:
            actor_easePushVelocity(actor, 0x4000, 50.0f, 0.05999999865889549f);
            break;
        case 6:
            pathIdx = collision_findSpawnById(28);
            break;
        case 41:
            if (zone_isActorOutsideId(dc_glbl_CharNum, 0x4028) == 0) {
                id = 12;
            } else if (zone_isActorOutsideId(dc_glbl_CharNum, 0x400E) == 0) {
                id = 10;
            } else if (zone_isActorOutsideId(dc_glbl_CharNum, 0x400D) == 0) {
                id = 9;
            } else {
                id = 6;
            }
            pathIdx = collision_findSpawnById(id);
        split: // matching: label steers IDO register allocation for id
            break;
        case 43:
            pathIdx = collision_findSpawnById(18);
            break;
        case 65:
            if (zone_isActorOutsideId(dc_glbl_CharNum, 0x401F) == 0) {
                id = 27;
            } else {
                id = 26;
            }
            pathIdx = collision_findSpawnById(id);
            break;
    }

    if (pathIdx != -1) {
        actor_driftOnCurrent(actor, pathIdx);
    }
}

/* Per carry variant (see actor_attachToHost): the host's animation, its speed and the carried actor's reaction. */
static s16 D_800860C0[] = { 0x00D5, 0x0114, 0x0017, 0x0288, 0x0034, 0x01FA };
static f32 D_800860CC[] = { 3.0f, 1.0f, 1.0f, 1.0f, 1.5f, 1.0f };
static u16 D_800860E4[] = { 0x000D, 0x000D, 0x0039, 0x000D, 0x000D, 0x000D };

/* Attaches the actor to movement_obj[hostIdx] (grabbed/carried) and starts both actors' animations. */
void actor_attachToHost(Actor *actor, s32 hostIdx) {
    Game83300AnimEntry *anims;
    ActorAnim *animTable;
    s32 variant;

    actor->noshove &= ~0x800000;
    actor->carried = hostIdx + 100;
    actor->unk21C = 0;
    actor->ygrav = 4.0f;
    actor->task_attrib &= ~8;

    movement_obj[hostIdx].count5 = 0xFF;
    movement_obj[hostIdx].norun = 0xFF;
    variant = 0;
    if (actor->character_type == 0x57) {
        variant = 1;
    }
    if (movement_obj[hostIdx].character_type == 0x9B) {
        variant = 2;
    }
    if (actor->character_type == 0x5E) {
        variant = 3;
    }
    if (actor->character_type == 0x3C) {
        variant = 4;
        movement_obj[hostIdx].lifting = 0;
        movement_obj[hostIdx].angle = actor_getDirectionAngle(actor->xpos - movement_obj[hostIdx].xpos, movement_obj[hostIdx].zpos - actor->zpos, 0);
    }
    if (actor->character_type == 0x89) {
        variant = 5;
    }
    actor_playAnim(&movement_obj[hostIdx], D_800860C0[variant], D_800860CC[variant], 0.0f, 0.0f, 0.0f, 0);
    actor->stunned = 0xFE;
    actor->stunnednum = 0;
    actor->stunnedseq = actor_findReaction(D_800860E4[variant], actor);
    actor->animseq.uh = 0xFFFF;
    actor_advanceReaction(dc_glbl_CharNum, actor);
    animTable = gModelAnimTables[actor->character_type];
    if (animTable != NULL) {
        anims = ANIM_HEADER(animTable)->reactions;
        if ((anims != NULL) && (anims[actor->stunnedseq].sub[2].flags & 2)) {
            actor->langle = movement_obj[hostIdx].langle;
        }
        if ((anims == NULL) || !(anims[actor->stunnedseq].sub[2].flags & 1)) {
            actor->unk13E = (actor->langle - movement_obj[hostIdx].langle) >> 8;
        } else {
            actor->unk13E = 0;
        }
    } else {
        actor->unk13E = (actor->langle - movement_obj[hostIdx].langle) >> 8;
    }
}

/* Lets a free player in gCollidedActors's mask grab the actor (see actor_attachToHost). */
void actor_checkGrab(Actor *actor) {
    s32 i;
    s32 mask;
    ActorCtl *movement;
    Actor *player;

    mask = gCollidedActors;
    for (i = 0; i < gPlayerCount; i++) {
        player = &movement_obj[i];
        if (((1 << i) & mask) && (player->lifting == 0) && (player->norun == 0) &&
            (player->type == 1) && (player->stunned == 0) && (player->playernumber != 0xFF)) {
            break;
        }
    }
    if (i != gPlayerCount) {
        player = &movement_obj[i];
        if (player->lifting == 0) {
            movement = player->mv;
            if ((movement->jump_on_char == 0) && (player->energy != 0) && (actor->carried == 0) &&
                ((actor->stunned != 0) || (actor->task_attrib & 0x1000) || (actor->character_type == 0x57)) &&
                (actor->shad == 0.0f) && (player->shad == 0.0f) && (actor->task_attrib & 8) &&
                (movement->GrenadeThrowState == 0) &&
                ((actor->character_type != 0xA9) || (player->team == 0)) &&
                ((actor->character_type != 0xA8) || (player->team != 0))) {
                player->lifting = dc_glbl_CharNum + 100;
                player->speed = 0.0f;
                movement->unk18 = 0;
                actor_attachToHost(actor, i);
            }
        }
    }
}

/* Turns the facing angle (unk7A) toward targetAngle, limited per frame; returns the remaining gap >> 8. */
u8 actor_turnFacing(Actor *actor, u8 turnRate, u16 targetAngle) {
    s16 delta;
    s16 maxDelta;
    s16 minDelta;
    s32 step;
    s32 range;
    s16 signedDelta;

    step = (((turnRate << 8) + actor->unk1E8) * gActorStepPercent) / 100;
    delta = targetAngle - actor->langle;
    if (actor->unk1EA != 0) {
        range = (actor->unk1EA * gActorStepPercent) / 100;
        maxDelta = actor->unk1EC + range;
        minDelta = actor->unk1EC - range;
        if (maxDelta < delta) {
            delta = maxDelta;
        }
        if (delta < minDelta) {
            delta = minDelta;
        }
    }

    signedDelta = delta;
    if (delta < 0) {
        delta ^= 0xFFFF;
    }
    if (delta < step) {
        step = delta;
    }
    if ((actor->mvmt_control & 1) == 0) {
        if (actor->count2 != 0) {
            if (signedDelta < 0) {
                actor->langle -= step;
            } else {
                actor->langle += step;
            }
        }
    }
    return delta >> 8;
}

/* Turns the moving angle (unk76) toward unk78 by at most maxStep; returns the gap before turning >> 8. */
u8 actor_turnMoving(Actor *actor, u16 maxStep) {
    s16 delta;
    s16 maxDelta;
    s16 minDelta;
    s32 range;
    s16 absDiff;
    s16 signedDelta;

    delta = actor->fangle - actor->angle;
    absDiff = delta;
    if (absDiff < 0) {
        absDiff ^= 0xFFFF;
    }
    if (actor->unk1EA != 0) {
        range = (actor->unk1EA * gActorStepPercent) / 100;
        maxDelta = actor->unk1EC + range;
        minDelta = actor->unk1EC - range;
        if (maxDelta < delta) {
            delta = maxDelta;
        }
        if (delta < minDelta) {
            delta = minDelta;
        }
    }

    signedDelta = delta;
    if (delta < 0) {
        delta ^= 0xFFFF;
    }
    if (delta < maxStep) {
        maxStep = delta;
    }
    if ((actor->mvmt_control & 1) == 0) {
        if (signedDelta < 0) {
            actor->angle -= maxStep;
            actor->unk1EC = -maxStep;
        } else {
            actor->angle += maxStep;
            actor->unk1EC = maxStep;
        }
    }
    return absDiff >> 8;
}


/* Applies one frame of horizontal movement (speed along unk76, plus push velocities), following the floor slope. */
void actor_moveHorizontal(Actor *actor) {
    f32 x;
    f32 z;
    f32 y;
    f32 vertical;
    u16 angle;
    Tri3f triangle;
    f32 speed;
    s32 i;
    f32 length;
    f32 scale;

    speed = actor->speed;
    angle = actor->angle;
    if (gActorFrameCount == 1) {
        return;
    }
    if (actor->unk1FC & 8) {
        actor->unk1FC &= ~8;
        return;
    }
    if (actor->playernumber != 0xFF) {
        if (actor->mv->bitten > 0) {
            angle += *(u16 *)&actor->mv->pissed_angle; // matching: read unsigned here (lhu); stored signed elsewhere
            actor->langle = angle;
        }
    }
    if (actor->speed < 1.0f && actor->speed >= 0.0f && !(actor->unk1FC & 1)) {
        if (actor->unk7E < 250) {
            actor->unk7E += 1;
        }
        if (actor->ob_slopex == 0.0f && actor->ob_slopey == 0.0f && actor->ob_overslopex == 0.0f && actor->ob_overslopey == 0.0f) {
            actor->unk1F4 = 0.0f;
            return;
        }
    } else {
        actor->unk7E = 0;
    }
    if ((actor->inwater != 0 && (actor->mv == NULL || actor->mv->unk21 == 0)) ||
        (actor->unk22C & 8)) {
        x = actor->vtilt;
        if (x > 90.0f) {
            x = 90.0f;
        } else if (x < -90.0f) {
            x = -90.0f;
        }
    } else {
        x = 0.0f;
    }
    if (actor->unk1FC & 1) {
        actor->unk1FC &= ~1;
        if (gActorTimeStep) {
            speed = 2.0f * (actor->unk1F4 * actor->scale_xz / gActorTimeStep); // matching: operand order
        }
        actor->unk1F4 = actor->speed;
        angle = actor->langle;
    }
    actor->unk1F4 = speed;
    actor_splitVelocity(angle, speed, x, &x, &z, &y);
    x += actor->ob_slopex + actor->ob_overslopex;
    z += actor->ob_slopey + actor->ob_overslopey;
    if (actor->inwater == 10) {
        y = 0.0f;
    }
    if ((actor->type != 1 || actor->mv->transport_surface == 0) && actor->shad <= 0.0f &&
        actor->inwater == 0 && !(actor->unk22C & 8)) {
        /* project the step onto the floor triangle */
        for (i = 0; i < 3; i++) {
            triangle.pts[i].x = actor->floorTri.v[i].x;
            triangle.pts[i].y = actor->floorTri.v[i].y;
            triangle.pts[i].z = actor->floorTri.v[i].z;
        }
        math_computeTriPlane(triangle);
        if (gTriPlaneNormalY != 0.0f) {
            vertical = (-gTriPlaneNormalX * x - gTriPlaneNormalZ * z) / gTriPlaneNormalY;
            if (vertical > 4.0f || actor->type != 1 || actor->mv->transport != 2) {
                length = sqrtf(x * x + z * z + vertical * vertical);
                if (length != 0.0f) {
                    scale = sqrtf(x * x + z * z);
                    x = x * scale / length;
                    y = vertical * scale / length;
                    z = z * scale / length;
                }
            }
        }
    }
    x *= gActorTimeStep;
    z *= gActorTimeStep;
    y *= gActorTimeStep;
    actor->xpos += x;
    actor->zpos += z;
    actor->ypos += y;
}
/* Splits `speed` (halved) along yaw `angle` and pitch `pitch` (degrees) into x/z/y components. */
void actor_splitVelocity(u16 angle, f32 speed, f32 pitch, f32 *outX, f32 *outZ, f32 *outY) {
    f32 sinYaw;
    f32 yaw;
    f32 cosYaw;

    speed = speed * 0.5f;
    pitch = pitch * 0.01745329238f;
    *outY = math_sin(pitch) * -speed;
    speed = math_cos(pitch) * speed;
    yaw = angle * 9.587380191e-05f;
    sinYaw = math_cos(yaw);
    cosYaw = math_sin(yaw);
    *outX = sinYaw * speed;
    *outZ = -cosYaw * speed;
}
/* Steps the 2D vector (*x, *z) toward (targetX, targetZ) by rate * dt, without overshooting. */
void actor_stepVector2D(f32 targetX, f32 targetZ, f32 rate, f32 *x, f32 *z) {
    f32 dx = targetX - *x, dz = targetZ - *z, mag, stepX, stepZ;

    if (dx == 0.0f) {
        if (dz == 0.0f) {
            return;
        }
    }
    mag = sqrtf((dx * dx) + (dz * dz));
    rate *= gActorTimeStep;
    stepX = fabsf((dx / mag) * rate);
    stepZ = fabsf((dz / mag) * rate);
    if (0.0f <= dx) {
        *x = *x + stepX;
        if (targetX < *x) {
            *x = targetX;
        }
    } else {
        *x = *x - stepX;
        if (*x < targetX) {
            *x = targetX;
        }
    }
    if (0.0f <= dz) {
        *z = *z + stepZ;
        if (targetZ < *z) {
            *z = targetZ;
        }
    } else {
        *z = *z - stepZ;
        if (*z < targetZ) {
            *z = targetZ;
        }
    }
}

/* Accelerates/decelerates the actor's speed toward targetSpeed; player rates are scaled in water and other states. */
void actor_accelerate(f32 targetSpeed, Actor *actor, f32 accel, f32 decel, u8 turnDiff) {
    accel *= gActorTimeStep;
    decel *= gActorTimeStep;
    if (gFloorMaterialClass != 0 && actor->shad < 5.0f && actor->type != 0x1E) {
        accel = 0.0f;
    }
    if (actor->type == 1) {
        if (actor->inwater != 0) {
            decel *= 0.400000006f;
            accel = 2.0f;
        }
        if (actor->unkA8 != 0) {
            decel *= 0.25f;
        }
        if (actor->count3 != 0) {
            if (actor->count3 & 0x40) {
                if (actor->count3 & 0x20) {
                    decel = 0.0f;
                } else {
                    decel = 1.299999952f;
                }
            } else {
                decel *= 0.349999994f;
                if (turnDiff >= 46) {
                    accel *= 0.5f;
                }
                if (turnDiff >= 91) {
                    targetSpeed *= 0.5f;
                }
            }
        }
        if (actor->unkAE != 0) {
            decel *= 0.200000003f;
        }
    }
    if (targetSpeed < 0.0f) {
        if (0.0f < actor->speed) {
            decel += accel;
        } else {
            decel = accel;
        }
    } else if (actor->speed < 0.0f) {
        accel += decel;
    }
    if (targetSpeed < actor->speed) {
        actor->speed -= decel;
        if (actor->speed < targetSpeed) {
            actor->speed = targetSpeed;
        }
    } else {
        if ((targetSpeed - 1.0f) < actor->speed) {
            accel *= 0.009999999776f;
        }
        actor->speed += accel;
        if (targetSpeed < actor->speed) {
            actor->speed = targetSpeed;
        }
    }
}

/* Analog stick deflection of a controller pad, scaled and capped at 35. */
f32 actor_getStickDeflection(OSContPad *pad) {
    f32 x, y;
    f32 mag;

    mag = pad->stick_x; // matching: staging x through mag steers IDO's register choice
    x = mag;
    y = pad->stick_y;

    x = sqrtf(x * x + y * y) * 0.6000000238f;
    mag = x;

    if (mag > 35.0f) {
        mag = 35.0f;
    }

    return mag;
}

/* Binary angle (0x10000 = full turn) of the direction (x, z); the third argument is unused. */
u16 actor_getDirectionAngle(f32 x, f32 z, s32 unused) {
    return (u16)(math_atan2(-x, z) * 10430.37793f) + 16384;
}

/* Horizontal (xz) distance between two actors. */
f32 actor_distanceXZ(Actor *a, Actor *b) {
    f32 x, z;

    x = a->xpos - b->xpos;
    x *= x;
    z = a->zpos - b->zpos;
    z *= z;
    return sqrtf(x + z);
}

/* 3D distance between two actors. */
f32 actor_distance3D(Actor *a, Actor *b) {
    f32 x, y, z;

    x = a->xpos - b->xpos;
    x *= x;
    z = a->zpos - b->zpos;
    z *= z;
    y = a->ypos - b->ypos;
    y *= y;
    return sqrtf(x + z + y);
}

/* Applies gravity: in the air it integrates in 0.1 steps; on the ground in a single step. Fall speed is capped at 500. */
void actor_applyGravity(Actor *actor) {
    f32 velocity;
    f32 decrement;
    s32 count;
    s32 i;

    if ((actor->shad != 0.0f) || (velocity = actor->yspd, velocity > 0.0f)) {
        decrement = actor->ygrav * 0.1f;
        count = (gActorTimeStep / 0.1f);
        for (i = 0; i < count; i++) {
            actor->yspd -= decrement;
            actor->ypos += actor->yspd * 0.05f;
        }
        if (actor->yspd < -500.0f) {
            actor->yspd = -500.0f;
        }
    } else {
        actor->yspd = velocity - (actor->ygrav * gActorTimeStep);
        velocity = actor->yspd; actor->ypos += velocity * gActorTimeStep * 0.5f; // matching: one line (as1 tie-break)
        if (velocity < -500.0f) {
            actor->yspd = -500.0f;
        }
    }
}
// a big one, but could be ok

/* Lands the player on floorY: water entry, fall damage/death, landing animations and bounce. */
void actor_landPlayer(Actor *actor, f32 floorY) {
    f32 dist;
    f32 deathFall;
    u8 bounce;
    u8 keepLandFlags;
    s32 health;
    s32 floorType;
    u8 landState;
    s32 killed;
    s32 anim;
    s32 flags;
    f32 speed;
    s32 hurt;
    u8 found;
    u8 turnGap;

    bounce = 0;
    keepLandFlags = 1;
    dist = actor->ypos - floorY;
    if (actor->mv->transport_surface != 0) {
        if (dist < 0.0f) {
            actor->unk1CC = floorY;
            dist = 0.0f;
        }
        actor->shad = dist;
        dist = actor->ypos - (actor->waterHeight - 34.0f);
        if (dist < 70.0f && actor->yspd < 18.0f) {
            if (actor->yspd > -25.0f) {
                actor->mv->transport_surface = 2;
            }
            if (dist < 0.0f) {
                if (actor->yspd < -12.0f) {
                    actor->count3 = 0;
                    if (actor->yspd < -25.0f) {
                        actor->speed *= 0.400000006f;
                        actor->yspd = -30.0f;
                    }
                    actor->yspd *= 0.8500000238f;
                }
                actor->ygrav = -1.899999976f;
            } else {
                if ((actor->yspd > 10.0f) && (actor->yspd < 25.0f)) {
                    actor->yspd = 10.0f;
                }
                actor->ygrav = 2.200000048f;
            }
        }
    } else if (actor->attach_char != 0) {
        if (dist < 0.0f) {
            actor->unk1CC = floorY;
            dist = 0.0f;
            actor->ypos = floorY;
        }
        actor->shad = dist;
    } else {
        if (dist < 0.0f) {
            f32 hurtFall;
            
            landState = 0;
            if (actor->inwater == 0) {
                actor->unk137 = 0;
                actor->ygrav = 5.0f;
            }
            if (actor->inwater == 0) {
                actor->mv->save_done = 4;
            }
            if (actor->shad > 0.0f) {
                actor->mv->land_floor = 1;
                actor->mv->prop = 0;
            }
            actor->unk100 &= 0xFD;
            if (((actor->floorMaterial >> 12) & 0xF) == 1) {
                if (level == 0x14) {
                    blood_spawnDeathEffects(actor, 0xFF, 1);
                    actor_kill(actor, 8);
                    return;
                }
                actor_startOverEdge(actor);
                return;
            }
            hurtFall = 600.0f;
            if (level == 0x43) {
                deathFall = 1000.0f;
            } else {
                deathFall = hurtFall * 3.0f;
            }
            if ((actor->unk1CC - actor->ypos) > hurtFall && actor->mv->death_State == 0 &&
                actor->mv->rt_context_b != 1) {
                speed = 1.299999952f;
                do { actor->mv->ladder = 0; } while (0); // matching: the wrappers steer FP register choice
                do { actor->mv->land_floor = 2; } while (0);
                killed = 0;
                flags = 0;
                hurt = 0;
                anim = 0x30;
                if (level != 0x29) {
                    floorType = actor->floorMaterial;
                    if (((((u32)floorType >> 12) & 0xF) == 2) || ((floorType == 0x13) & 0x1F)) {
                        anim = 0xC8;
                        func_15060B70(0x1B8, actor);
                        speed = 1.0f;
                    } else {
                        health = actor->energy;
                        if (health != 0) {
                            if (actor->mv->rt_context_b == 0x21) {
                                flags = 1;
                            }
                            if (actor->character_type == 0x75) {
                                flags |= 2;
                            }
                        }
                        if ((actor->unk1CC < 90000.0f) && (deathFall < (actor->unk1CC - actor->ypos))) {
                            if (!(flags & 2)) {
                                actor->energy = 0;
                                if ((floorType & 0x1F) != 0xE) {
                                    blood_spawnDeathEffects(actor, 0xFF, 1);
                                    killed = 1;
                                }
                            } else {
                                hurt = 1;
                                if (health != 0) {
                                    actor->energy = health - 1;
                                }
                            }
                        }
                        if (flags == 0) {
                            health = actor->energy;
                            actor->stunned = 8;
                            if (health != 0) {
                                actor->energy = health - 1;
                            }
                        }
                        if (flags & 1) {
                            speed = 1.0f;
                            anim = 0x19D;
                        } else if (flags & 2) {
                            if (hurt) {
                                anim = 0x27;
                            } else {
                                anim = 9;
                            }
                        } else if (actor->energy != 0) {
                            anim = 0x27;
                        }
                        if ((anim == 0x27) && (actor->mv->rt_context_b == 0x25)) {
                            anim = 0x1AA;
                        }
                    }
                }
                if (killed == 0) {
                    actor_playAnim(actor, anim, speed, 2.0f, 6.0f, 0.0f, 0);
                }
                actor->stunned = 0;
                actor->count5 = 0xFF;
                actor->norun = 0xFE;
                if (flags != 0) {
                    actor->unk228 = 10;
                } else {
                    actor->unk228 = 30;
                }
                actor->shad = 0.0f;
                actor->angle = actor->langle;
                actor->unk100 &= 0x8B;
                actor->ypos = floorY;
                actor->unkA9 = 0;
                actor->yspd = -4.0f;
                actor->unk1CC = floorY;
                if (killed == 0) {
                    particle_spawnWakePuff(dc_glbl_CharNum, 0, 0x1F, 0, 0x199, 4, 0xAA, 0xFF, 0);
                } else {
                    actor_kill(actor, 8);
                }
                return;
            }
            actor->unk1CC = floorY;
            if (actor->inwater != 0) {
                actor->ypos = floorY;
                actor->shad = 0.0f;
                return;
            }
            if (actor->stunned == 0) {
                if ((actor->count1 == 0) && (actor->speed < 9.0f)) {
                    actor->angle = actor->langle;
                }
                turnGap = (actor->angle - actor->langle) >> 8;
                if (turnGap > 0x80) {
                    turnGap = -turnGap;
                }
                if (!actor->count1 && (turnGap < 0x50)) {
                    actor->count1 = 10;
                }
                if (actor->unkA9 == 2) {
                    Actor *other;
                    found = 0;
                    if (actor->stand_on_char != 0) {
                        other = &movement_obj[actor->stand_on_char - 1];
                        if (other->unk24C != 0) {
                            found = 1;
                            other->scriptCmd = 0;
                            other->intelwalk = other->unk24C;
                        }
                    }
                    actor->speed = 0.0f;
                    actor->count5 = 0xFF;
                    landState = 0xFF;
                    actor_playAnim(actor, 0x4B, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                    if (dc_glbl_CharNum < 4) {
                        actor->mv->unk57 = 1;
                    }
                    if (found == 0) {
                        actor_playUntrackedSound(0x19, actor);
                    }
                } else {
                    s32 landAnim;
                    landAnim = 0;
                    if ((s32)actor->shad > 0) {
                        ActorCtl *movement = actor->mv;
                        if ((movement->weapon_status != 1) && (movement->weapon_status != 3) && (movement->transport == 0)) {
                            if ((actor->unk100 & 0x10) || (!(actor->unk100 & 8) && (((actor->stickspeed == 0.0f) && (actor->unkA8 == 0)) || (actor->unk100 & 4)))) {
                                if (actor->unk100 & 0x10) {
                                    actor->count5 = 8;
                                    if (!(actor->unk100 & 0x40)) {
                                        actor->speed = 0.0f;
                                        actor->count5 = 0xD;
                                        landState = 5;
                                    }
                                } else if (actor->unk100 & 4) {
                                    actor->speed *= 0.6999999881f;
                                    actor->count5 = 0x14;
                                } else {
                                    actor->speed = 0.0f;
                                    actor->count5 = 0x10;
                                }
                                if (actor->mv->rt_context_b == 0x21) {
                                    landAnim = 0x19D;
                                } else if (actor->mv->rt_context_b == 0x25) {
                                    landAnim = 0xD8;
                                } else {
                                    landAnim = 0x36;
                                }
                            }
                        }
                    }
                    if ((actor->animseq.uh == 0x3B) || (actor->animseq.uh == 0x3C)) {
                        u32 surface;
                        surface = actor->floorMaterial & 0x1F;
                        landState = 0xFE;
                        keepLandFlags = 0;
                        actor->count5 = 0xFF;
                        if ((surface == 0xA) || (surface == 0xB) || (surface == 0xC) || (surface == 0xF) || (surface == 0x13)) {
                            landAnim = 0xC8;
                            func_15060B70(0x1B8, actor);
                        } else if ((surface == 1) || (surface == 2) || (surface == 7) || (surface == 0x11)) {
                            landAnim = 0xC8;
                            func_15060B70(0xA7, actor);
                        } else {
                            landAnim = 0x27;
                            func_15060B70(0xA7, actor);
                        }
                    }
                    if ((landAnim != 0) && !(actor->unk100 & 0x20)) {
                        actor_playAnim(actor, landAnim, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                    }
                    if (!(actor->unk100 & 0x80)) {
                        actor->unk100 = 1;
                    }
                    if (keepLandFlags == 0) {
                        actor->unk100 = 0x90;
                    }
                }
                if ((actor->shad > 0.0f) && !(actor->unk100 & 8)) {
                    if (gFloorMaterialClass != 0) {
                        if (actor->speed > 20.0f) {
                            actor->speed = 20.0f;
                        }
                    } else if (actor->unk110 == 0) {
                        actor->ob_slopex = 0.0f;
                        actor->ob_slopey = 0.0f;
                    }
                    actor->norun = landState;
                    if ((actor->mv->transport == 1) && (actor->yspd < -10.0f)) {
                        bounce = 2;
                    }
                }
                actor->unkA9 = 0;
            }
            if (!(actor->unk100 & 8)) {
                dist = 0.0f;
            } else {
                dist = 0.1000000015f;
            }
            actor->ypos = floorY;
            if ((actor->stunned == 0) && !(bounce & 2)) {
                actor->yspd = -4.0f;
            } else {
                if (actor->mvmt_control & 0x80) {
                    actor->yspd = -20.0f;
                } else if (actor->yspd > -10.0f) {
                    actor->yspd = 0.0f;
                } else {
                    actor->yspd = actor->yspd * -0.5f;
                    if (actor->yspd > 28.0f) {
                        actor->yspd = 28.0f;
                    }
                }
                if (actor->unk109 != 0) {
                    actor->speed *= 0.6000000238f;
                }
            }
            actor->count3 = 0;
            if (actor->count1 < 0xFA) {
                actor->count1++;
            }
            actor->unk100 &= 0x9F;
        } else if ((dist < 1.0f) && (actor->shad > 0.0f)) {
            dist = 1.0f;
        }
        if (dist > 20.0f) {
            actor->count1 = 0;
        }
        if (actor->inwater != 0) {
            actor->unk1CC = actor->ypos;
        }
        actor->shad = dist;
    }
}

/* Lands a non-player actor on floorY: fall damage, landing bounce/sound, and the fall-off reaction (unk252). */
void actor_landNonPlayer(Actor *actor, f32 floorY) {
    f32 prevHeight;
    f32 y;
    f32 factor;
    f32 gravity;
    s32 animKind;
    s32 health;
    s32 step;
    s32 sound;
    SpawnRecord *owner;

    prevHeight = actor->shad;
    if (!(actor->noshove & 0x8000)) {
        if (actor->unk252 != 0 && (actor->unk253 * 8) < actor->unk1CC - actor->ypos &&
            actor->stunned == 0) {
            animKind = actor->unk252;
            actor->stunned = 0xFE;
            actor->stunnednum = 0;
            actor->stunnedseq = actor_findReaction(animKind, actor);
            actor->animseq.uh = 0xFF;
            actor_advanceReaction(dc_glbl_CharNum, actor);
            actor->ypos = actor->old_ypos;
        }
        y = actor->ypos;
        actor->shad = y - floorY;
        if (y <= floorY) {
            actor->mvmt_control |= 0x8000;
            step = D_800B0DF0->unk13;
            if (step != 0 && prevHeight != 0.0f) {
                particle_spawnWakePuff(dc_glbl_CharNum, 0, 0x1000 / step, 0, 0x199, 4, 0xAA, 0xFF, 0);
            }
            actor->ypos = floorY;
            actor->shad = 0.0f;
            if (actor->stunned != 0 || !(actor->noshove & 0x10000)) {
                if (actor->type == 1) {
                    if (actor->yspd < -6.0f) {
                        actor->yspd = -6.0f;
                    }
                } else {
                    if (actor->unk1CC - actor->ypos > 450.0f) {
                        health = actor->energy;
                        if (health > 0) {
                            owner = actor->listpos_ptr;
                            if (owner != NULL && !(owner->unkE & 0x80)) {
                                actor->energy = health - 1;
                            }
                        }
                    }
                    actor->unk1CC = floorY;
                    if (actor->stunned == 0 && (gravity = actor->ygrav) > 0.5f) {
                        if ((actor->noshove & 0x800) && prevHeight != 0.0f &&
                            actor->yspd < -4.0f * gravity * gActorTimeStep) {
                            sound = actor->unk2CC;
                            factor = actor->unk2CB * 0.009999999776f;
                            actor->yspd = (0.0f - actor->yspd) * factor;
                            if (sound != 0 && actor->yspd > 5.0f) {
                                gEventParam = sound;
                                event_playSound2Range1800();
                            }
                        } else if (actor->type != 7) {
                            actor->yspd = -4.0f * gActorTimeStep;
                        }
                    } else {
                        if (actor->mvmt_control & 0x80) {
                            actor->yspd = -4.0f;
                        } else if (prevHeight == 0.0f ||
                                   actor->yspd > -1.5f * actor->ygrav * gActorTimeStep) {
                            actor->yspd = 0.0f;
                        } else {
                            factor = actor->unk2CB * 0.009999999776f;
                            actor->yspd = (0.0f - actor->yspd) * factor;
                        }
                        actor->speed *= 0.8999999762f;
                    }
                }
            }
        }
    }
}

/*
 * Applies a hit to `victim`: base[index] holds the attack's defaults and hit[index] the reaction chosen
 * for this victim. Sets knockback, damage and the reaction animation, and releases anything it carried.
 */
void actor_applyHit(Actor *attacker, Actor *victim, const HitReaction *base, const HitReaction *hit, u8 index,
                   s32 victimSlot, u8 damageMask) {
    u8 reaction;
    u8 tmp;
    s32 slot;
    Actor *savedActor;
    s32 savedIdx;
    s32 v;

    if (hit[index].unk0 & 0x80) {
        reaction = hit[index].unk0 & 0x7F;
    } else {
        reaction = base[index].unk0 & 0x7F;
    }
    victim->unk10B = 0;
    if ((base[index].unk24 & 1) && (victim->stunned != 0)) {
        damageMask = 0;
    }
    if (!(hit[index].unkF & 0x40)) {
        victim->angle = attacker->angle + (hit[index].unk10 << 8);
        if (base[index].unk24 & 0x10) {
            victim->angle = actor_getDirectionAngle(victim->xpos - attacker->xpos,
                                          attacker->zpos - victim->zpos, 0);
        }
        if (attacker->character_type == 0x44) {
            victim->angle = 0;
        }
    }
    if ((base[index].unkF | hit[index].unkF) & 0x20) {
        victim->unk10B |= 2;
    }
    victim->unk3A = 0;
    victim->unk109 = hit[index].unk20;
    victim->yspd = hit[index].yVelocity;
    victim->speed = hit[index].xzVelocity;
    victim->ygrav = hit[index].gravity;
    victim->unk100 &= 0xD6;
    v = hit[index].unkF & damageMask;
    if (v < victim->energy) {
        victim->energy -= v;
    } else {
        victim->energy = 0;
    }
    if (victim->type == 1) {
        {
            ActorCtl *movement;

            savedActor = gCurrentActor;
            savedIdx = dc_glbl_CharNum;
            gCurrentActor = victim;
            dc_glbl_CharNum = (victim - movement_obj);
            movement = movement_obj[dc_glbl_CharNum].mv;
            if (movement->transport != 2) {
                movement->transport = 0;
            }
        }
        event_runTimedEvents(victim, 800.0f, 1);
        gCurrentActor = savedActor;
        dc_glbl_CharNum = savedIdx;
        victim->noshove &= 0xFEFF7FFF;
        victim->mv->ladder = 0;
        victim->mv->unk6B = 0;
        if (victim->mv->rope != 0) {
            rope_release(victim);
        }
        victim->mv->side = 0;
        tmp = victim->attach_char;
        victim->unk137 = 0;
        if (tmp != 0) {
            Actor *other;

            v = tmp - 1;
            other = &movement_obj[(u8)v];

            if (((0x8A != other->character_type) && (0x23 != other->character_type)) || (victim->energy == 0) ||
                ((gIsMultiplayer != 0) && (victim->yspd > 30.0f))) {
                if (v) {} // matching: extra use of v before it is reused
                v = 0;
                if (0x8A == other->character_type) {
                    v = 1;
                }
                if (other->character_type == 0x23) {
                    v = 0xA;
                }
                if (other->character_type == 0xC) {
                    v = 0xF;
                }
                if (v != 0) {
                    other->intelwalk = v;
                    other->scriptCmd = NULL;
                }
                other->forcedCullMask = 0;
                other->lifting = 0;
                victim->intelhitroute = 0;
                victim->attach_char = 0;
            }
        }
    }
    v = hit[index].unk8;
    if (v == 0) {
        v = base[index].unk8;
    }
    if (reaction != 0x27) {
        gHitReactionProcs[v](attacker, victim, &reaction);
    }
    if (gIsMultiplayer != 0) {
        Actor *other;

        tmp = reaction;
        v = attacker->owner;
        other = v != 0 ? &movement_obj[v - 1] : attacker;
        save_recordHit(other, victim, tmp);
    }
    {
        struct S { void *a; u8 b; } msg;

        msg.a = attacker;
        msg.b = attacker->unique_id;
        effect_sendEventToTypeRange(0, 0x68, (s32)&msg, 2);
        msg.a = victim;
        msg.b = victim->unique_id;
        effect_sendEventToTypeRange(0, 0x68, (s32)&msg, 2);
    }
    if (base[index].unk4 != 0) {
        gEventParam = base[index].unk4;
        if (event_resolveSoundCommand(victim, &gEventParam, 0) != 0) {
            sound_playActorUntracked(gEventParam, victim, 0x6D60, 0x12C, 0x708);
        }
    } else {
        if (hit[index].unk4 != 0) {
            gEventParam = hit[index].unk4;
            if (event_resolveSoundCommand(victim, &gEventParam, 0) != 0) {
                sound_playActorUntracked(gEventParam, victim, 0x6D60, 0x12C, 0x708);
            }
        }
    }
    attacker->ivehitit = 60;
    if ((gIsMultiplayer == 0) || (victim->type != 1)) {
        victim->unk21C = 0;
        if (base[index].unk24 & 0x100) {
            victim->scriptCmd = NULL;
        }
    }
    slot = victimSlot - 1;
    victim->doing_hit = 0;
    v = attacker->owner;
    if (v) {
        victim->unk10A = v + 0x7F;
    } else {
        victim->unk10A = dc_glbl_CharNum | 0x80;
    }
    attacker->unk108 = slot | 0x80;
    if (reaction != 0x27) {
        if ((base[index].unk24 & 0x40) && (victim->type == 1)) {
            movement_obj[slot].mv->bitten = 8000;
        } else {
            v = victim->unk23B;
            if (v != 0) {
                reaction = v;
            }
            victim->stunned = 0xFE;
            victim->stunnednum = 0;
            victim->stunnedseq = actor_findReaction(reaction, victim);
            victim->animseq.uh = 0xFFFF;
            actor_advanceReaction(slot, victim);
            if (base[index].unk24 & 0x200) {
                victim->unk10B |= 0x20;
            }
            if (victim->yspd > 0.0f) {
                victim->shad += 0.1000000015f;
            }
            if (base[index].unk24 & 8) {
                victim->unk10B |= 0x40;
                gHitAttackerX = attacker->xpos;
                gHitAttackerZ = attacker->zpos;
                gHitAttackerY = attacker->ypos;
            }
        }
        if (base[index].unk24 & 2) {
            victim->unk10B |= 0x80;
        }
    }
    if ((base[index].unk24 & 0x400) || (hit[index].unk24 & 0x400)) {
        victim->unk10B |= 4;
    }
    v = base[index].unk22;
    if (v & 0x80) {
        victim->imune = v & 0x3F;
    }
}

/* How far (binary angle, absolute) the attacker is from the victim's facing direction. */
s16 actor_getAttackAngle(Actor *attacker, Actor *victim) {
    s16 ret = actor_getDirectionAngle(attacker->xpos - victim->xpos, victim->zpos - attacker->zpos, 0);
    ret -= victim->langle;

    if (ret < 0) {
        ret = -ret;
    }
    return ret;
}

/* The actor type's hit-reaction table (stored before its animation table), or the default one. */
const HitReaction *actor_getHitReactions(Actor *actor) {
    const HitReaction **slot;
    const HitReaction *ret;

    if ((actor->character_type != 0xFF) && (gModelAnimTables[actor->character_type] != NULL)) {
        slot = &ANIM_HEADER(gModelAnimTables[actor->character_type])->hits; // matching: the address is formed before the load
        ret = *slot;
    } else {
        ret = gDefaultHitReactions;
    }
    return ret;
}

/*
 * Resolves a hit on `victim` through the victim's reaction rules (unk239): may redirect to another
 * table entry, cancel the hit or handle it specially; otherwise applies it with actor_applyHit.
 * Returns hitMask with the victim's bit set when the hit landed.
 */
s32 actor_resolveHit(Actor *attacker, Actor *victim, const HitReaction *table, s32 index, s32 victimSlot, s32 hitMask, s32 damageMask) {
    s32 i;
    u8 done;
    u8 kind;
    u8 count;
    u8 flags;
    const HitReaction *entry;
    const HitRule *rules;
    s32 angle;
    u8 type;
    u8 idx;

    done = FALSE;
    kind = table[index].unk0 & 0x7F;
    entry = table;
    if (((u8)victim->intelhitroute & 0x7F) != 0) {
        count = gHitRuleCounts[((u8)victim->intelhitroute & 0x7F)];
        rules = gHitRoutes[((u8)victim->intelhitroute & 0x7F)];
        for (i = 0; i < count; i++) {
            type = rules[i].unk2;
            if (((type & 0x80) == 0 && kind == rules[i].unk1) || ((type & 0x80) != 0 && kind != rules[i].unk1)) {
                type &= 0x7F;
                if (rules[i].unk0 == 1) {
                    if (actor_getAttackAngle(attacker, victim) > 0x4000) {
                        type = 0xFF;
                    }
                }
                if (rules[i].unk0 == 2) {
                    if (actor_getAttackAngle(attacker, victim) < 0x4000) {
                        type = 0xFF;
                    }
                }
                if (rules[i].unk0 == 3 && victim->stunned == 0) {
                    type = 0xFF;
                }
                if (rules[i].unk0 == 4 && victim->stunned != 0) {
                    type = 0xFF;
                }
                if (type < 2) {
                    angle = actor_getDirectionAngle(attacker->xpos - victim->xpos, victim->zpos - attacker->zpos, 0) - victim->langle + 0x4000;
                    if ((u16)(angle >> 15) == type) {
                        index = rules[i].unk3;
                    }
                    break;
                }
                if (type == 2) {
                    index = rules[i].unk3;
                    break;
                }
                if (type == 3) {
                    done = TRUE;
                    break;
                }
                if (type == 4) {
                    if (rules[i].unk3 != 0xFF) {
                        index = rules[i].unk3;
                    }
                    damageMask = 0;
                    break;
                }
                if (type == 5) {
                    u8 next;
                    next = rules[i].unk3;
                    entry = &gKnockbackReactions[next - index];
                    break;
                }
                if (type == 6) {
                    u8 next;
                    next = rules[i].unk3;
                    damageMask = 0;
                    entry = &gKnockbackReactions[next - index];
                    break;
                }
                if (type == 7) {
                    if ((table[index].unkF & damageMask) >= victim->energy) {
                        idx = rules[i].unk3;
                        entry = &gKnockbackReactions[idx - index];
                        victim->energy = 0;
                        break;
                    }
                } else if (type == 8) {
                    done = TRUE;
                    victim->mvmt_control &= ~0x40040;
                    hitMask |= 1 << (victimSlot + 0x1F);
                    attacker->unk108 = victimSlot + 0x7F;
                    attacker->ivehitit = 0x3C;
                    victim->events = NULL;
                    victim->intelwalk = rules[i].unk3;
                    victim->scriptCmd = NULL;
                    victim->norun = 1;
                    victim->doing_hit = 0;
                    gEventParam = table[index].unk4;
                    if (gEventParam != 0) {
                        event_playSound2Range1800();
                    }
                    if (victim->type == 1) {
                        if ((victim->mv->transport == 2) && (victim->mv->bot == 0)) {
                            victim->mv->unk50 = 30;
                            victim->count3 = 0;
                            victim->yspd = 50.0f;
                            victim->ygrav = 2.0f;
                            break;
                        }
                    }
                    break;
                } else if (type == 9) {
                    break;
                } else if (type == 0xA) {
                    if (victim->animseq.uh == 0x283) {
                        victim->angle = attacker->langle;
                        victim->speed = 25.0f;
                        gEventParam = 0xFF0102E2;
                        event_playSound2Range1800();
                        attacker->doing_hit = 0;
                        attacker->intelwalk = 0xD;
                        attacker->scriptCmd = NULL;
                        done = TRUE;
                    }
                    break;
                } else if (type == 0xB) {
                    if (victim->shad > 8.0f) {
                        done = TRUE;
                    }
                    break;
                } else if (type == 0xC) {
                    if (victim->shad == 0.0f) {
                        attacker->taskint[2] = 100;
                        attacker->doing_hit = 0;
                    }
                    done = TRUE;
                    break;
                } else if (type == 0xD) {
                    s32 reactionBuf;
                    u8 handler;
                    s32 sound;
                    handler = table[index].unk8;
                    gEventParam = table[index].unk4;
                    if (gEventParam != 0) {
                        event_playSound2Range1800();
                    }
                    gHitReactionProcs[handler](attacker, victim, (u8 *)&reactionBuf);
                    if (victim->unkFC != 0) {
                        sound = victim->unkFC;
                        if (event_resolveSoundCommand(attacker, &sound, 1) != 0) {
                            actor_playUntrackedSoundRate(sound, victim, 0x5DC0);
                        }
                    }
                    hitMask |= 1 << (victimSlot + 0x1F);
                    done = TRUE;
                    break;
                }
            }
        }
    }
    if (done) {
        return hitMask;
    }
    i = victimSlot;
    actor_applyHit(attacker, victim, table, entry, index, i, damageMask);
    hitMask |= 1 << (i + 0x1F);
    return hitMask;
}

/* Tests the attacker's active attack (unkD0) against every other actor and applies the hits; returns the hit mask. */
s32 actor_testAttack(Actor *attacker, s32 selfSlot) {
    s32 i;
    s32 slotEnd;
    s32 attackIdx;
    s32 ret;
    Actor *victim;
    const HitReaction *hits;
    f32 dz2;
    const HitReaction *hit;
    f32 x;
    f32 y;
    f32 z;
    f32 radius;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 dist;
    f32 maxDist;
    f32 s;
    f32 c;
    f32 rx;
    f32 rz;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 h;
    f32 r;
    f32 t;
    f32 offZ;
    f32 height;
    f32 pad1; // matching: pad1..pad5 and padBC..padCC reproduce the stack layout
    f32 pad2;
    f32 pad3;
    f32 offZ2;
    f32 pad4;
    f32 pad5;
    Vec3f pos;
    f32 hitZ;
    f32 hitY;
    f32 hitX;
    Vec3f ray;
    f32 padCC;
    f32 hitDist;
    f32 padC4;
    f32 padC0;
    f32 padBC;
    s16 halfHeightRaw;

    selfSlot = (selfSlot & 0x7F) + 1;
    ret = 0;
    if (attacker->unk114 < 254.0f) {
        attacker->unk114 -= gActorTimeStep;
    }
    if (attacker->unk114 < 0.0f) {
        attacker->doing_hit = 0;
        return 0;
    }
    if (gCutsceneStates[0] == 1) {
        return 0;
    }
    attackIdx = attacker->doing_hit - 1;
    hits = actor_getHitReactions(attacker);
    if (hits[attackIdx].unk24 & 0x80) {
        // matching: byte-offset form (not &hits[attackIdx]) keeps IDO's addu operand order
        hit = (const HitReaction *)((const u8 *)hits + attackIdx * sizeof(HitReaction));
        y = attacker->ypos;
    } else {
        hit = (const HitReaction *)((const u8 *)hits + attackIdx * sizeof(HitReaction));
        dist = hit->unkA;
        dist *= attacker->scale_xz / 0.125f;
        actor_splitVelocity((hit->unk9 << 8) + attacker->langle, dist, 0.0f, &x, &z, &y);
        x += attacker->xpos;
        y += attacker->ypos;
        z += attacker->zpos;
        radius = hit->unkE;
        radius = attacker->scale_xz * 4.0f * radius;
    }
    dist = hit->unkC;
    y += dist * (attacker->scale_y / 0.25f);
    if (attacker->noshove & 0x100) {
        slotEnd = 4;
    } else {
        slotEnd = 26;
    }
    for (i = 1; i < slotEnd; i++) {
        victim = &movement_obj[i - 1];
        if (victim->type == 0) {
            continue;
        }
        if (i == selfSlot || victim->energy == 0 || victim->imune != 0) {
            continue;
        }
        hit = (const HitReaction *)((const u8 *)hits + attackIdx * sizeof(HitReaction));
        if (i == attacker->owner && !(hit->unk24 & 0x1000)) {
            continue;
        }
        if (hit->unk24 & 0x80) {
            s16 sideOffset;

            dist = 2.0f;
            maxDist = 1.0f;
            dx = victim->xpos - attacker->xpos;
            dz = victim->zpos - attacker->zpos;
            if (victim->unkD8 != 0 || victim->unkDA != 0) {
                dx += actor_rotateOffset(victim->theta, victim->unkDA, victim->unkD8, 1.0f, &offZ);
                dz += offZ;
            }
            rx = (attacker->theta - 90.0f) * 0.01745329238f;
            s = math_cos(-rx);
            c = math_sin(-rx);
            rx = dx * s + dz * c;
            dz = -dx * c + dz * s;
            hx = hit->unk9 * 5;
            dz2 = hit->unkA * 5;
            sideOffset = (s8)hit->unkE * 5;
            dy = sideOffset * attacker->scale_xz;
            dx = rx - dy;
            hx *= attacker->scale_xz;
            dz2 *= attacker->scale_xz;
            dy = y - (victim->ypos + victim->unkD6);
            hx += victim->unkD2;
            dz2 += victim->unkD2;
            hy = victim->unkD4;
            hy += 25.0f;
            if (-hy < dy && dy < hy && -hx < dx && dx < hx && -dz2 < dz && dz < dz2) {
                dist = 0.0f;
            }
        } else {
            dx = x - victim->xpos;
            dz2 = z - victim->zpos;
            if (victim->unkD8 != 0 || victim->unkDA != 0) {
                dx -= actor_rotateOffset(victim->theta, victim->unkDA, victim->unkD8, 1.0f, &offZ2);
                dz2 -= offZ2;
            }
            {
                typedef s32 T1505C7D8; // matching: block-scope declaration steers IDO's scheduling

                dy = y - (victim->ypos + victim->unkD6);
                r = victim->unkD2;
                h = victim->unkD4;
                if (h == 0.0f) {
                    continue;
                }
            }
            t = h / (radius + h);
            maxDist = (r + radius) * (r + radius);
            dy *= (r / h) * t + (1.0f - t);
            dist = dx * dx + dy * dy + dz2 * dz2;
        }
        if (dist < maxDist) {
            if ((hit->unk24 & 0x2000) && i != attacker->carried) {
                if (attacker->attachments != NULL) {
                    pos = *(Vec3f *)attacker->attachments; // the first attachment's first 12 bytes
                } else {
                    actor_computeSize(attacker, &halfHeightRaw, 0, 0);
                    pos.x = attacker->xpos;
                    pos.y = attacker->ypos + (halfHeightRaw >> 1);
                    pos.z = attacker->zpos;
                }
                rx = victim->xpos;
                if (victim->mv != NULL) {
                    height = victim->mv->height * 0.5f;
                } else {
                    height = 20.0f;
                }
                t = height;
                t += victim->ypos;
                rz = victim->zpos;
                ray.x = rx - pos.x;
                ray.y = t - pos.y;
                ray.z = rz - pos.z;
                if (collision_castRayWorld(pos.x, pos.y, pos.z, ray.x, ray.y, ray.z, NULL, NULL, &hitX, &hitY, &hitZ, &hitDist, 0, 0, 0.0f) != 0 &&
                    hitDist < math_vec3Length(&ray)) {
                    continue;
                }
            }
            gHitPointX = x;
            gHitPointY = y;
            gHitPointZ = z;
            ret = actor_resolveHit(attacker, victim, hits, attackIdx, i, ret, 0xF);
            attackIdx = attacker->doing_hit - 1;
        }
    }
    if (ret != 0) {
        hit = (const HitReaction *)((const u8 *)hits + attackIdx * sizeof(HitReaction));
        /* unk108 holds victim slot | 0x80 */
        if (movement_obj[attacker->unk108 - 0x80].type == 1) {
            if (attacker->unk254 != 0) {
                attacker->intelwalk = attacker->unk254;
                attacker->scriptCmd = NULL;
            }
        }
        if (!(hit->unk24 & 4)) {
            attacker->unk21C = 0;
        }
        if (!(hit->unk24 & 0x800)) {
            attacker->doing_hit = 0;
        }
    }
    return ret;
}

/* Hits `victim` with a scripted attack from the dummy attacker movement_obj[25]; flags & 0xFF picks the entry. */
s32 actor_hitScripted(Actor *victim, s32 flags, u16 angle, s32 sourceSlot) {
    Actor *dummy;
    s32 ret;
    s32 slot;
    const HitReaction *table;

    dummy = &movement_obj[25];
    table = gDefaultHitReactions;
    if (gCutsceneStates[0] == 1) {
        return 0;
    }
    if ((flags & 0x20000) && (victim->imune != 0)) {
        return 0;
    }
    if ((flags & 0x40000) && (victim->stunned != 0)) {
        return 0;
    }
    if (!(flags & 0x100000) && (victim->energy == 0)) {
        return 0;
    }
    if (sourceSlot == -1) {
        dummy->owner = 0;
    } else {
        dummy->owner = sourceSlot + 1;
    }
    dummy->angle = angle;
    dummy->langle = angle;
    if (flags & 0x10000) {
        table = actor_getHitReactions(gCurrentActor);
    }

    slot = victim - movement_obj;
    ret = 1 << slot;
    if (flags & 0x80000) {
        ret = actor_resolveHit(dummy, victim, table, flags & 0xFF, slot + 1, 0, 7);
    } else {
        actor_applyHit(dummy, victim, table, table, flags & 0xFF, slot + 1, 7);
    }
    return ret;
}

/*
 * Runs attack attackId from point (x, y, z) through the dummy attacker movement_obj[25]: sets its position,
 * owner, carried (the slot the line of sight skips), angle, attachments (the line-of-sight origin), unk114
 * (timer), doing_hit (attack + 1) and the xz/y scales.
 * Declared int but returns no value, in the pre-ANSI way: collision_castRayAtObjects uses what actor_testAttack leaves in v0.
 */
s32 actor_runAttackAt(f32 x, f32 y, f32 z, s32 attackId, s32 sourceSlot, u16 angle, s32 losSkipSlot, s32 losOrigin) {
    Actor *dummy;
    s32 savedIdx;

    savedIdx = dc_glbl_CharNum;
    dummy = &movement_obj[25];
    actor_clearSlot(dummy);
    dummy->xpos = x;
    dummy->ypos = y;
    dummy->zpos = z;
    dummy->owner = sourceSlot + 1;
    dummy->carried = losSkipSlot;
    dummy->angle = angle;
    dc_glbl_CharNum = 25;
    dummy->attachments = (Attachment *)losOrigin;
    dummy->unk114 = 100.0f;
    dummy->doing_hit = (attackId & 0xFFFF) + 1;
    dummy->scale_xz = 0.5f;
    dummy->scale_y = 0.5f;
    actor_testAttack(dummy, 25);
    dc_glbl_CharNum = savedIdx;
}

/* Starts the knockback reaction gKnockbackReactions[entryIdx] on the actor. */
void actor_startKnockback(Actor *actor, u8 entryIdx) {
    const HitReaction *entry;

    entry = &gKnockbackReactions[entryIdx];
    actor->yspd = entry->yVelocity;
    actor->speed = entry->xzVelocity;
    actor->ygrav = entry->gravity;
    actor->stunned = 0xFE;
    actor->stunnednum = 0;
    actor->stunnedseq = actor_findReaction(actor->unk10E & 0x7F, actor);
    actor->unk10E = 0xFF;
    actor_advanceReaction(dc_glbl_CharNum, actor);
}

/* Rotates the offset (a, b) by angleDeg - 90 degrees (after scaling); returns x, writes z. */
f32 actor_rotateOffset(f32 angleDeg, f32 a, f32 b, f32 scale, f32 *outZ) {
    f32 sinA;
    f32 cosA;
    f32 rad;

    if (scale != 1.0f) {
        a = a * scale;
        b = b * scale;
    }
    rad = (angleDeg - 90.0f) * 0.01745329238f;
    sinA = math_cos(rad);
    cosA = math_sin(rad);
    *outZ = (-a * cosA) + (b * sinA);
    return (a * sinA) + b * cosA;
}


/* Pushes the actor out of `other` along (nx, ny, nz) unless the other one is lighter. */
void actor_pushOut(Actor *actor, Actor *other, f32 nx, f32 ny, f32 nz, f32 distSq, f32 minDist, s32 otherSlot) {
    f32 dist;
    f32 invDist;
    f32 pushFactor;
    s32 temp_a2;
    s32 temp_t0;
    u8 actorWeight;
    u8 otherWeight;

    pushFactor = 0.400000006f;
    actorWeight = 0;
    otherWeight = 0;
    if (distSq < 1.0f) {
        distSq = 1.200000048f;
    }
    if ((nx == 0.0f) && (nz == 0.0f)) {
        nx = 1.0f;
    }
    gCollidedActors |= 1 << otherSlot;
    if (other->noshove & 0x10) {
        D_800CC26C = otherSlot;
        pushFactor = 0.8000000119f;
        if ((other->character_type == 0x61) && (actor->type == 1)) {
            goto end;
        }
    }
    if (actor->noshove & 1) {
        actorWeight = 1;
    }
    if (actor->noshove & 0x200) {
        actorWeight |= 2;
    }
    if (other->noshove & 1) {
        otherWeight = 1;
    }
    if (other->noshove & 0x200) {
        otherWeight |= 2;
    }
    if (otherWeight >= actorWeight) {
        if ((other->noshove & 0x400) && (other->shad == 0.0f)) {
            D_800CC26D = otherSlot + 100;
        }
        dist = sqrtf(distSq);
        invDist = 1.0f / dist;
        nx *= invDist;
        ny *= invDist;
        nz *= invDist;
        minDist -= dist;
        minDist *= pushFactor;
        actor->xpos += minDist * nx;
        actor->zpos += minDist * nz;
        actor->ypos += minDist * ny;
    }
end: // matching: a jump to the shared epilogue, not an early return
    ;
}

/* Pushes the actor out of an extra collision cylinder attached to `other` (offset in other's frame). */
void actor_pushOutCylinder(Actor *actor, Actor *other, f32 x, f32 y, f32 z, f32 ratio, f32 radius, s32 otherSlot,
                   f32 extraRadius, f32 extraHeight, f32 extraY, f32 offsetA, f32 offsetB) {
    f32 offsetX;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distSq;
    f32 sumRadius;

    offsetX = actor_rotateOffset(other->theta, offsetA, offsetB, other->scale_xz, &offsetB);
    extraRadius *= other->scale_xz;
    extraHeight *= other->scale_y;
    dx = x - (other->xpos + offsetX);
    dz = z - (other->zpos + offsetB);
    dy = y - (other->ypos + (extraY * other->scale_y));
    dy *= (ratio + (extraRadius / extraHeight)) * 0.5f;
    sumRadius = radius + extraRadius;
    distSq = ((dx * dx) + (dy * dy)) + (dz * dz);
    if (distSq < (sumRadius * sumRadius)) {
        actor_pushOut(actor, other, dx, dy, dz, distSq, sumRadius, otherSlot);
    }
}
/* Resolves collisions between the actor's body cylinder (bodyRadius/bodyOffsetY/unkEC) and every other actor's. */
void actor_resolveCollisions(Actor *actor, s32 selfIdx) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distSq;
    f32 sumRadius;
    f32 radius;
    f32 otherRadius;
    f32 height;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    s32 count;
    Actor *other;
    ActorCtl *movement;

    gCollidedActors = 0;
    D_800CC26C = 0;
    D_800CC26D = 0;
    count = 25;
    if (((actor->type != 1) ||
         ((movement_obj[selfIdx].mv->transport == 0) && (movement_obj[selfIdx].mv->jump_on_char == 0))) &&
        (actor->carried < 100) && (actor->bodyRadius != 0) && (actor->attach_char == 0) && !(actor->noshove & 0x01000000) &&
        !(actor->noshove & 0x4000)) {
        radius = actor->bodyRadius;
        height = actor->unkEC;
        x = actor->xpos;
        y = actor->ypos + actor->bodyOffsetY;
        z = actor->zpos;
        if (actor->noshove & 0x02000000) {
            count = 1;
        }
        for (i = 0; i < count; i++) {
            other = &movement_obj[i];
            if (other->type == 0) {
                continue;
            }
            if (other->bodyRadius < 2) {
                continue;
            }
            if ((other->noshove & 0x02000000) && (actor->playernumber == 0xFF)) {
                continue;
            }
            if (other->noshove & 0x01000000) {
                continue;
            }
            if (i == selfIdx) {
                continue;
            }
            if (other->attach_char == (selfIdx + 1)) {
                continue;
            }
            if (other->carried >= 100) {
                continue;
            }
            if (other->activeFlags == 0) {
                continue;
            }
            otherRadius = other->bodyRadius;
            sumRadius = radius + otherRadius;
            dx = x - other->xpos;
            dz = z - other->zpos;
            dy = y - (other->ypos + other->bodyOffsetY);
            dy *= (height + other->unkEC) * 0.5f;
            if ((gIsMultiplayer != 0) && (((dx * dx) + (dy * dy) + (dz * dz)) < 30000.0f) &&
                (actor->type == 1) && (other->type == 1) &&
                (actor->team != other->team)) {
                movement = actor->mv;
                if ((movement->rt_context_b == 0) && (other->mv->rt_context_b != 0)) {
                    movement->chased = 20;
                }
            }
            distSq = (dx * dx) + (dy * dy) + (dz * dz);
            if (distSq < (sumRadius * sumRadius)) {
                actor_pushOut(actor, other, dx, dy, dz, distSq, sumRadius, i);
            }
            if (other->character_type == 0x34) {
                if (other->unk251 == 2) {
                    actor_pushOutCylinder(actor, other, x, y, z, height, radius, i, 120.0f, 250.0f, 120.0f, -130.0f, 0.0f);
                } else {
                    actor_pushOutCylinder(actor, other, x, y, z, height, radius, i, 130.0f, 140.0f, 250.0f, -50.0f, 0.0f);
                }
            }
        }
    }
}

/*
 * Picks the nearest visible living actor inside a cone around the actor's facing (halfCone wide,
 * pitch +-0x10 around `pitch`), skipping skipIdx. Returns its index (0xFF if none), angle in *outAngle.
 */
u8 actor_findTargetInCone(Actor *actor, u16 *outAngle, u8 pitch, u8 skipIdx, u8 halfCone) {
    u8 bestStunned;
    u8 i;
    u16 angle;
    Actor *other;
    f32 best;
    f32 dist;
    f32 distXZ;
    f32 dy;
    s32 hitA[2];
    CollisionTri hitB;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 hitDist;
    u8 bestIdx;

    best = 10000000.0f;
    bestStunned = 0;
    bestIdx = 0xFF;
    if (skipIdx == 0xFE) {
        best = 40000.0f;
    }
    for (i = 0; i < 25; i++) {
        other = &movement_obj[i];
        if ((other->type != 0) && (other->energy != 0) && (i != (u8)(actor - movement_obj)) &&
            (other->noshove & 0x40) && (i != skipIdx) && (other->activeFlags != 0)) {
            dist = actor_getDistSqTo(actor, i, &angle, &distXZ, &dy, 0);
            if (((dist < best) || ((other->stunned == 0) && (bestStunned != 0))) &&
                ((u8)(((angle - actor->langle) >> 8) + halfCone) < (halfCone * 2)) &&
                ((u8)(((u8)(actor_getDirectionAngle(sqrtf(distXZ), -dy, 0) >> 8) - pitch) + 0x10) < 0x20)) {
                if ((collision_castRayWorld(actor->xpos, actor->ypos + 80.0f, actor->zpos,
                                   movement_obj[i].xpos - actor->xpos,
                                   movement_obj[i].ypos - actor->ypos,
                                   movement_obj[i].zpos - actor->zpos, hitA, &hitB, &hitX, &hitY, &hitZ,
                                   &hitDist, 0, 0, 0.0f) == 0) ||
                    !((hitDist = hitDist * hitDist) < dist)) {
                    bestStunned = other->stunned;
                    bestIdx = i;
                    best = dist;
                    *outAngle = angle;
                }
            }
        split: // matching: label splits the basic block like the original
            ;
        }
    }

    return bestIdx;
}

/* The first live actor that the actor would reach within `time` at its speed, inside a cone of its move angle; -1 if none. */
s8 actor_findPlayerAhead(Actor *actor, s32 unused, u16 cone, f32 time) {
    u8 i;
    u16 angle;
    f32 distXZ;
    f32 dy;
    f32 range;

    for (i = 0; i <= gScreenLayout; i++) {
        if (movement_obj[i].type == 0) {
            continue;
        }
        range = actor->speed * time * 0.5f;
        if (actor_getDistSqTo(actor, i, &angle, &distXZ, &dy, 0) < range * range) {
            if ((u16)(actor->angle - angle + cone / 2) < cone) {
                return i;
            }
        }
    }
    return -1;
}
/* Both callers pass a 6th argument (always 0) that the body never reads. */
f32 actor_getDistSqTo(Actor *actor, u8 otherIdx, u16 *outAngle, f32 *outDistSqXZ, f32 *outDy, s32 unused) {
    Actor *other;
    f32 dx;
    f32 dy;
    f32 dz;

    other = &movement_obj[otherIdx];
    dx = other->xpos - actor->xpos;
    dy = actor->ypos - other->ypos;
    dz = actor->zpos - other->zpos;
    *outDy = dy;
    *outAngle = actor_getDirectionAngle(dx, dz, 0);
    dx = dx * dx;
    dy = dy * dy;
    dz = dz * dz;
    *outDistSqXZ = dx + dz;
    return (dx + dy) + dz;
}

/* Stops the actor's animation and clears its playback state. */
void actor_stopAnim(Actor *actor) {
    s32 id;
    AnimInstance *state;
    u16 *p;

    actor->animseq.uh = 0xFFFF;
    state = actor->unk2D0;
    if (state != NULL) {
        id = actor->character_type;
        state->frameRom[0] = 0;
        bzero(state->header, sizeof(state->header));
        p = &gModelBoneCounts[id];
        state->header[0][1] = *p + 1;
        state->header[1][1] = *p + 1;
        state->frameBuf[0] = NULL;
        state->frameBuf[1] = NULL;
    }
}

/* Saves the current animation as the previous one, to blend from. */
void actor_saveBlendAnim(AnimInstance *state) {
    state->anim[1] = state->anim[0];
    state->frame[1] = state->frame[0];
    state->speed[1] = state->speed[0];
    state->start[1] = state->start[0];
    state->numFrames[1] = state->numFrames[0];
    state->nextFrameOffset[1] = state->nextFrameOffset[0];
    state->frameRom[1] = state->frameRom[0];
    bcopy(state->header[0], state->header[1], sizeof(state->header[0]));
}


/*
 * Starts animation `anim` (entry `animEntry`) on a playback state, or only updates its speed when it is
 * already playing. `actor` may be NULL for non-actor users; blendFrames >= 1 blends from the old one.
 */
void actor_startAnimState(Actor *actor, u16 *animEntry, AnimInstance *state, s32 arg3, s32 restart, s32 anim,
                   s32 bank, f32 speed, f32 blendFrames, f32 arg9, f32 arg10, s32 flag8000) {
    s32 index;
    s32 changed;
    s32 assetId;
    s32 kind;
    s32 dataSize;
    s32 romAddr;
    s32 *info;
    s16 blend;
    s32 mask;
    u8 buf[0x2C];

    changed = FALSE;
    if (restart != 0) {
        changed = TRUE;
        blendFrames = 0.0f;
    } else if (anim != (state->anim[0] & 0xFFFF7FFF)) {
        changed = TRUE;
    } else if (actor != NULL && (anim != actor->animseq.uh || actor->animseq.uh == 0xFFFF || actor->unk103 != 0)) {
        changed = TRUE;
    }
    if (changed != FALSE) {
        if (actor != NULL) {
            index = actor - movement_obj;
            kind = 2;
            bank = actor->unk6;
        } else {
            kind = 10;
        }
        if (animEntry != NULL) {
            assetId = animEntry[0];
        } else {
            assetId = anim * 2;
        }
        if (actor != NULL) {
            actor->animseq.uh = anim;
            actor->animsp = speed;
            actor->events = *(struct TimedEvent **)(animEntry + 2);
            actor->speccnt = 0;
            actor->unk103 = 0;
            actor->lastEventTime = 0.0f;
            if (index < gPlayerCount) {
                Actor *player = &movement_obj[index];

                player->mv->idle.active = 0;
                player->mv->idle.timer = 0;
                player->mv->idle.holdFrames = 0;
            }
        }
        if (state->frameRom[0] != 0) {
            blend = state->unk3C;
            if (blendFrames >= 1.0f) {
                state->unk3A = 500 / (s16)blendFrames;
                if (state->unk3C > 0 && anim == (state->anim[1] & 0xFFFF7FFF)) {
                    state->unk3C = 0x3FF - state->unk3C;
                    blend = 0;
                } else if (blend < 600) {
                    state->unk3C = 0x3FF;
                }
            } else {
                state->unk3C = 0;
            }
            if (blend < 600) {
                state->anim[1] = state->anim[0];
                state->frame[1] = state->frame[0];
                state->speed[1] = state->speed[0];
                state->start[1] = state->start[0];
                state->numFrames[1] = state->numFrames[0];
                state->nextFrameOffset[1] = state->nextFrameOffset[0];
                state->frameRom[1] = state->frameRom[0];
                bcopy(state->header[0], state->header[1], sizeof(state->header[0]));
            }
        }
        state->anim[0] = anim;
        if (flag8000 != 0) {
            state->anim[0] |= 0x8000;
        }
        state->frame[0] = arg9;
        state->speed[0] = speed;
        state->start[0] = arg10;
        if (animEntry != NULL) {
            state->callback = gAnimStateProcs[animEntry[1]];
        } else {
            state->callback = 0;
        }
        if (actor != NULL && (u8 *)actor->unk58 != NULL) {
            info = (s32 *)rom_loadTableEntries((u8 *)actor->unk58, buf, assetId, 2);
        } else {
            info = (s32 *)rom_loadSubTable(NULL, 2, buf, 3, kind, bank, assetId);
        }
        if (info != NULL) {
            dataSize = info[1];
            romAddr = info[0];
            state->frameRom[0] = info[2];
        }
        {
            s32 size = (dataSize + 0xF) & ~0xF;

            if (dataSize == 0) {
                actor_stopAnim(actor);
                return;
            }
            if (size < 200) {
                rom_copyUncached(romAddr, state->header[0], size);
            } else {
                rom_read(romAddr, state->header[0], size, 1);
            }
        }
        state->numFrames[0] = state->header[0][3];
        if (state->header[0][3] == 0xFF && actor != NULL) {
            switch (actor->character_type) {
            case 0x92:
                state->numFrames[0] = D_80099420[actor->animseq.uh - 20];
                break;
            case 0xB7:
            case 0xB8:
            case 0xB9:
                state->numFrames[0] = 641.0f;
                break;
            }
        }
        mask = 0x303;
        if (actor != NULL) {
            actor->unk24E = 0;
            actor->unk1FC |= 2;
            actor->unk1FC &= ~1;
            if (actor->stunned == 0) {
                mask = 0x30B;
            }
            actor->mvmt_control &= ~mask;
            if (actor->mvmt_control & 0x20) {
                actor->mvmt_control |= 2;
                actor->mvmt_control &= ~0x20;
            }
            if (state->header[0][7] & 0x20) {
                actor->unk1FD = state->header[0][0xB];
            } else {
                actor->unk1FD = 0;
            }
        }
        if (state->header[0][5] == 0) {
            state->frameRom[0] = 1;
        }
    } else {
        if (actor != NULL) {
            actor->animsp = speed;
        }
        state->speed[0] = speed;
        if (flag8000 != 0) {
            state->anim[0] |= 0x8000;
        } else {
            state->anim[0] &= 0xFFFF7FFF;
        }
        if (gCutsceneStates[0] == 1 && (state->anim[0] & 0x8000)) {
            state->frame[0] = arg9;
        }
    }
    if (actor != NULL && changed != FALSE) {
        actor_onAnimChanged(actor);
    }
}
/* Starts animation `anim` of the actor's type (see actor_startAnimState); ignored while a cutscene holds the actor. */
void actor_playAnim(Actor *actor, u16 anim, f32 speed, f32 blend, f32 arg4, f32 arg5, s32 flag8000) {
    ActorAnim *animTable;
    s32 idx;
    u16 *entry;
    AnimInstance *data;

    idx = actor->character_type;
    if ((gCutsceneActorLock != 0) && ((gCutsceneUpdateUnboundActors[0] == 0) || (cutscene_isActorBound(actor) != 0))) {
        return;
    }
    if (actor->character_type == 0xFF) {
        return;
    }
    if (gModelAnimCounts[actor->character_type] == 0) {
        return;
    }
    data = actor->unk2D0;
    if (data == NULL) {
        return;
    }
    if (gModelAnimCounts[actor->character_type] != 0) {
        if (anim >= gModelAnimCounts[actor->character_type]) {
            return;
        }
    } else {
        return;
    }
    animTable = gModelAnimTables[idx];
    if (animTable == NULL) {
        actor_stopAnim(actor);
        return;
    }
    if (anim < gModelAnimCounts[idx]) {
        if (animTable[0].unk0 != 999) {
            entry = ((u16 (*)[4])animTable)[anim]; // matching: indexing a u16[4] row view puts the base first in the addu
            animTable = NULL; // matching: dead store and do-while(0) shape the block like the original
            do {
                if (entry[0] != 999) {
                    if (entry[0] >= 30000) {
                        actor_stopAnim(actor);
                        return;
                    }
                    if (actor->unk2FF == 0) {
                        blend = 0.0f;
                    }
                    actor_startAnimState(actor, entry, data, 0, 0, anim, 0, speed, blend, arg4, arg5, flag8000);
                }
            } while (0);
        }
    }
}
/* Index of the actor type's reaction entry of the given kind (0 if there is none). */
u32 actor_findReaction(s32 kind, Actor *actor) {
    s32 id;
    ActorAnim *animTable;
    u32 count;
    Game83300AnimEntry *table;
    Game83300AnimEntry *entry;
    u32 i;

    id = actor->character_type;
    if (id == 0xFF) {
        return 0;
    }
    animTable = gModelAnimTables[id];
    if (animTable == NULL) {
        return 0;
    }
    count = ANIM_HEADER(animTable)->reactionsSize;
    if (count == 0) {
        return 0;
    }
    count = count / sizeof(Game83300AnimEntry);
    table = ANIM_HEADER(animTable)->reactions;
    if (table == NULL) {
        return 0;
    }
    if (count != 0) {
        /* matching: a pointer-stepped do-while; a for loop gets unrolled by IDO */
        i = 0;
        entry = table;
        do {
            if (kind == entry->kind) {
                return i;
            }
            i++;
            entry++;
        } while (i < count);
    }
    return 0;
}

/*
 * Advances the actor's reaction sequence (entry unk106, step unk105): plays the next step's animation
 * and stun time, starts the death animation when it ends with no health left, or ends the reaction.
 */
void actor_advanceReaction(s32 sourceIdx, Actor *actor) {
    ActorAnim *ptr;
    Game83300AnimEntry *table;
    f32 animSpeed;
    f32 blend;
    s32 i;
    u16 anim;
    u16 pad; // matching: stack slot

    blend = 6.0f;
    if (actor->character_type == 0xFF) {
        return;
    }
    anim = actor->stunnedseq;
    ptr = gModelAnimTables[actor->character_type];
    table = ANIM_HEADER(ptr)->reactions;
    if (table == NULL) {
        return;
    }
    if (actor->stunnednum == 0) {
        actor->unk10F = table[anim].unk8;
        actor->nojump = 0;
        if (actor->mv != NULL) {
            actor->mv->break_stun = 0;
        }
        if ((actor->character_type == 0x5E) && (actor->carried != 0)) {
            blend = 0.0f;
        }
        if (table[anim].sound != 0) {
            s32 sound;
            sound = table[anim].sound;
            if (event_resolveSoundCommand(actor, &sound, 1) != 0) {
                if (sound != 0x33) {
                    actor_playSound(sound, actor);
                } else {
                    actor_playUntrackedSoundRate(sound, actor, 0x5DC0);
                }
            }
        }
    }
    actor->mvmt_control &= 0xFFF3FEB1;
    if (actor->stunnednum < 3) {
        u8 stunTime;
        stunTime = table[anim].sub[actor->stunnednum].flags & 0xFE;
        actor->unk10B &= ~1;
        actor->unk10B |= table[anim].sub[actor->stunnednum].flags & 1;
        if ((actor->playernumber != 0xFF) && (actor->stunnednum == 0) && (actor->mv != NULL) &&
            (actor->CameraPtr != NULL) && (actor->mv->SniperState != 0)) {
            screen_startViewFlash(0xFF, 0, 0, 0xB4, 0x14, actor->CameraPtr->CNumber);
        }
        if ((actor->playernumber != 0xFF) && (actor->energy == 0)) {
            i = actor->stunnednum;
            while (table[anim].sub[i].anim != 0) {
                i++;
            }
            if (i < 2) {
                i = 2;
            }
            if (i == actor->stunnednum + 1) {
                s32 type;
                s32 deathKind;
                type = table[anim].kind;
                if (actor->character_type == 0x9A) {
                    deathKind = 0xE;
                } else if (actor->character_type == 0x53) {
                    deathKind = 0xA;
                } else if (actor->inwater != 0) {
                    deathKind = 4;
                } else if ((((actor->floorMaterial & 0x1F) == 0xE) || ((actor->floorMaterial & 0x1F) == 0x13)) &&
                           (level != 0x34)) {
                    deathKind = 0xD;
                } else {
                    switch (type) {
                        case 0x20:
                        case 0x21:
                        case 0x25:
                        case 0x26:
                        case 0x33:
                        case 0x34:
                        case 0x48:
                            deathKind = 0;
                            break;
                        case 0x2D:
                        case 0x2E:
                        case 0x2F:
                            deathKind = 7;
                            break;
                        case 0x2C:
                            deathKind = 8;
                            break;
                        case 0x42:
                            deathKind = 0xC;
                            break;
                        case 0x43:
                            deathKind = 0xB;
                            break;
                        case 0xE:
                        case 0xF:
                            deathKind = 0x10;
                            break;
                        case 0x10:
                            actor->mv->on_fire = 1;
                            gEventParam = 0xFF010434;
                            deathKind = 0;
                            event_playSound2Range1800();
                            gEventParam = 0xFF070169;
                            event_playSound1Range1800();
                            break;
                        default:
                            deathKind = 1;
                            break;
                    }
                }
                actor_kill(actor, deathKind);
                actor->stunnednum = 3;
                return;
            }
        }
        if (stunTime == 0xC8) {
            actor->mvmt_control |= 8;
            actor->unk10B |= 1;
        }
        actor->unk10C = stunTime * 100;
        animSpeed = table[anim].sub[actor->stunnednum].speed;
        animSpeed = animSpeed / 100.0f;
        anim = table[anim].sub[actor->stunnednum].anim;
        if (anim != 0) {
            actor->stunnednum++;
        }
        {
            s32 pad;

            if (animSpeed == 0.0f) {
                animSpeed = 1.0f;
            }
        }
    } else {
        actor->unk10C = 20000;
        animSpeed = 1.0f;
        anim = 0;
    }
    if (anim == 0) {
        actor->mvmt_control &= ~8;
        actor->stunned = 0;
        actor->unk10B = 0;
        actor->unk10F = 0;
        if (actor->type == 1) {
            actor->mv->idle.active = 0;
            actor->noshove &= 0xFEFFFDFF;
        }
    } else {
        actor_playAnim(actor, anim, animSpeed, blend, 0.0f, 0.0f, 0);
    }
}
/* Allocates an actor slot: a free one, else one in state 0x27 is released and reused. Returns it cleared. */
Actor *actor_alloc(void) {
    s32 i;
    Actor *ptr;

    for (i = 0, ptr = movement_obj; (i < 25) && (ptr->type != 0); i++, ptr++) {
    }

    if (i == 25) {
        for (ptr = movement_obj, i = 0; (i < 25) && ((0x27 != ptr->type) || (ptr->ob_matrix_ptr != NULL)); i++, ptr++) {
        }

        if (i != 25) {
            if (ptr->type != 0) {
                actor_remove(ptr, 0);
            }
        }
    }

    if (i == 25) {
        for (ptr = movement_obj, i = 0; (i < 25) && (0x27 != ptr->type); i++, ptr++) {
        }

        if (i != 25) {
            if (ptr->type != 0) {
                actor_remove(ptr, 0);
            }
        }
    }

    if (i == 25) {
        if (ptr->type != 0) {
            actor_remove(ptr, 0);
        }
    }

    actor_clearSlot(ptr);
    return ptr;
}

/* The first actor in `state` (or past the end); its index goes to *outIdx. */
Actor *actor_findByState(s32 state, s32 *outIdx) {
    Actor *ptr;
    s32 i;

    for (ptr = movement_obj, i = 0; (i < 25) && (state != ptr->type); i++, ptr++) {
    }

    *outIdx = i;
    return ptr;
}

/* The first live actor following path `pathIdx` (unk13F), or NULL. */
Actor *actor_findOnPath(s32 pathIdx) {
    Actor *actor = movement_obj;
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((actor->type != 0) && (pathIdx == actor->listpos)) {
            return actor;
        }
        actor++;
    }
    return NULL;
}

/* The live actor whose playernumber is `key`, or NULL. */
Actor *actor_findByPlayerNumber(s32 key) {
    Actor *actor = movement_obj;
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((actor->type != 0) && (key == actor->playernumber)) {
            return actor;
        }
        actor++;
    }
    return NULL;
}

/* The first live actor of type `id`, or NULL. */
Actor *actor_findByType(s32 id) {
    Actor *actor = movement_obj;
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((actor->type != 0) && (id == actor->character_type)) {
            return actor;
        }
        actor++;
    }
    return NULL;
}
/* Clears an actor slot and sets its default fields. */
void actor_clearSlot(Actor *actor) {
    s32 *p;
    f32 one;
    f32 initial;

    for (p = (s32 *)actor; p < (s32 *)(actor + 1); p++) {
        *p = 0;
    }
    one = 1.0f;
    initial = -10000.0f;
    actor->unk2FD = 2;
    actor->unk38 = -10000;
    actor->scale_xz = one;
    actor->scale_y = one;
    actor->waterHeight = initial;
    actor->floorY = initial;
    actor->ygrav = 3.3f;
    actor->unk1DC = 0xFF;
    actor->playernumber = 0xFF;
    actor->animseq.uh = 0xFFFF;
    actor->listpos = 0xFF;
    actor->modelIds = &actor->character_type;
    actor->lodCount = 1;
    actor->modelCount = 1;
    actor->character_type = 0xFF;
    actor->unk2CB = 50;
    actor->speedup = one;
    actor->blinkTimer = (math_random() % 50U) + 50;
    actor_resetFade(actor);
    actor->unk1DD[0] = 0xFF;
    actor->unk1DD[1] = 0xFF;
    actor->unk1DD[2] = 0xFF;
    actor->floorTri.v[0].x = 0;
    actor->floorTri.v[0].y = 0;
    actor->floorTri.v[0].z = 0;
    actor->floorTri.v[1].x = 0;
    actor->floorTri.v[1].y = 0;
    actor->floorTri.v[1].z = 10;
    actor->floorTri.v[2].x = 10;
    actor->floorTri.v[2].y = 0;
    actor->floorTri.v[2].z = 0;
}
/* Player controls for jumping, swimming and the hover (anim 0xE9), from the controller pad. */
void actor_updatePlayerControls(Actor *actor, OSContPad *pad) {
    s32 anim;
    s32 sound;
    f32 speed;
    f32 animArg4;
    f32 rate;
    s32 id;
    u8 prevJumpState;
    f32 height;
    s32 neverSet;
    s32 buttons;
    s32 flag;
    s32 jumpType;
    s32 idx;
    u8 jumped;
    u8 jumpState;
    f32 savedSpeed;

    prevJumpState = actor->mv->prop;
    sound = 0;
    speed = 1.0f;
    rate = 0.0f;
    neverSet = 0;
    if ((actor->stunned != 0) || (actor->energy == 0)) {
        actor->wantspeed = 0.0f;
        return;
    }
    if ((actor->mv->bitten > 0) || (actor->mv->over_edge >= 0x64) || (actor->mv->unk6B != 0)) {
        return;
    }
    height = actor->shad;
    if (actor->mv->transport == 2) {
        height = actor->shad - 150.0f;
    } else if (actor->mv->transport_surface == 2) {
        height = 0.0f;
    } else if (actor->yspd > 0.0f) {
        height += 50.0f;
    }
    if (actor->unk110 != 0) {
        buttons = pad->button & ~0x2000;
    } else {
        buttons = pad->button;
    }
    if ((actor->inwater != 0) && (actor->mv->ladder == 0) && (actor->animseq.uh != 0x262)) {
        if (actor->inwater == 1) {
            rate = 2.0f;
            anim = 0;
            if ((gActorPressedButtons & 0x8000) && ((level != 0x29) || (actor->mv->pissed_up <= 0)) &&
                (((actor->waterHeight - 60.0f) - 30.0f) < actor->ypos)) {
                actor_leaveWater(actor);
                actor->mv->prop = 0;
                rate = 0.0f;
                if ((actor->mv->inst_context != 7) && (actor->mv->inst_context != 8)) {
                    actor->mv->prop = 1;
                }
                actor->angle = actor->langle = actor->fangle = actor->mv->press_angle;
                if ((actor->mv->unk68 != 0) && ((actor->angle - actor->mv->unk68 + 0x4000) & 0x8000)) {
                    actor->speed = 0.0f;
                    actor->norun = 0xFF;
                }
                actor->unkAE = 0xC;
                actor->yspd = 46.0f;
                actor->ygrav = 3.299999952f;
                actor->count3 = 2;
                actor->count5 = 0;
                anim = 0x32;
            }
            actor->wantspeed = 0.0f;
            if ((gActorPressedButtons & 0x4000) && !(pad->button & 0x10)) {
                if (actor->mv->unk22 < 0x69) {
                    actor->mv->unk22 = 0xF;
                }
            }
            if (((actor->speed > 2.0f) && (actor->mv->unk21 != 0)) ||
                ((actor->mv->unk21 == 0) && ((actor->mv->unk22 > 0) || (!(pad->button & 0x10) && (pad->button & 0x4000))))) {
                if (anim != 0x32) {
                    rate = 9.0f;
                    actor->count5 = 0;
                    if ((actor->waterHeight - 60.0f) < actor->ypos) {
                        speed = 1.200000048f;
                        anim = 0x22;
                        actor->wantspeed = 15.0f;
                    } else {
                        speed = 1.200000048f;
                        anim = 0xCE;
                        actor->wantspeed = 23.0f;
                    }
                }
            }
        }
        if (anim == 0) {
            rate = 10.0f;
            speed = 1.200000048f;
            anim = 0xD1;
            if ((actor->waterHeight - actor->ypos) < 70.0f) {
                anim = 0xCC;
                speed = 0.8999999762f;
            }
            actor->wantspeed -= 0.200000003f;
            if (actor->wantspeed < 0.0f) {
                actor->wantspeed = 0.0f;
            }
        }
        if ((actor->inwater != 0) && (actor->lifting != 0)) {
            rate = 7.0f;
            anim = 0xCB;
        }
        if ((actor->mv->weapon_status != 0) || ((actor->mv->rt_context_b != 0) && (actor->mv->rt_context_b != 0x27))) {
            actor_runPlayerAction(actor, 0, 0, height, 1);
        }
        if (anim != 0) {
            actor_playAnim(actor, anim, speed, rate, 0.0f, 0.0f, 0);
        }
        return;
    }
    if (actor->unk137 == 1) {
        anim = 0;
        if (actor->mv->hang_ledge == 0) {
            if (pad->stick_y >= 0x33) {
                speed = 2.200000048f;
                actor->mv->side = 0xC;
                anim = 0xEA;
                actor->unk137 = 0;
                actor->norun = 0x10;
                actor->count3 = 1;
                actor->ygrav = 5.0f;
                actor->yspd = 48.0f;
            } else if (pad->stick_y < -0x32) {
                anim = 0x38;
                actor->unk137 = 0;
                actor->ygrav = 3.5f;
            }
        }
        if ((gActorPressedButtons & 0x8000) || (actor->mv->bot != 0)) {
            actor->unk137 = 2;
            anim = 0x40;
            speed = 3.0f;
        }
        if (anim != 0) {
            actor_playAnim(actor, anim, speed, 4.0f, 0.0f, 0.0f, 0);
            return;
        }
    }
    idx = actor->attach_char;
    jumpType = 0;
    flag = 0;
    anim = 0;
    if (idx != 0) {
        id = movement_obj[idx - 1].character_type;
        if (((id == 0x28) && (level != 0x23)) || (id == 0x8A) || (id == 0x23)) {
            jumpType = id + 1;
            flag = 1;
        }
    }
    if ((jumpType != 0) || (height <= 0.0f) || (actor->mv->save_done != 0)) {
        actor->unk137 = 0;
        jumped = 0;
        animArg4 = 0.0f;
        if (gActorPressedButtons & 0x8000) {
            if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3) &&
                ((jumpType != 0) || ((actor->lifting < 0x64) && (actor->nojump == 0) && (actor->norun != 0xFF)))) {
                jumped = 1;
                animArg4 = 15.0f;
                actor->unk100 &= 0xFE;
                if (flag != 0) {
                    actor_fadeCommand(actor, 1, 0xFF, 0, 0);
                }
                if (actor->mv->env_suit != 0) {
                    animArg4 = 0.0f;
                    anim = 0x279;
                    speed = 1.25f;
                    actor->animseq.uh = 0;
                    actor->nojump = 0xA;
                    actor->norun = 0xFF;
                    actor->count5 = 0xFF;
                } else if (actor->mv->pissed_up != 0) {
                    actor->count5 = 0xFF;
                    actor->norun = 0xFF;
                    jumped = 0;
                    actor_playAnim(actor, 0x43, 1.299999952f, 2.0f, 0.0f, 0.0f, 0);
                } else {
                    if (actor->mv->injured != 0) {
                        anim = 0x32;
                        animArg4 = 0.0f;
                        actor->norun = 0xFF;
                        actor->count3 = 1;
                        actor->yspd = 47.0f;
                        actor->ygrav = 6.0f;
                    } else if (!(buttons & 0x2000) || (actor->mv->inst_context == 7) || (actor->mv->inst_context == 8)) {
                        if (actor->norun != 0) {
                            actor->norun = 0;
                            actor->langle = actor->angle = actor->mv->press_angle;
                        }
                        anim = 0x32;
                        if (actor->speed < 9.0f) {
                            animArg4 = 0.0f;
                            actor->mv->prop = 1;
                        } else {
                            speed = 0.8000000119f;
                            actor->mv->prop = 2;
                            anim = 0x37;
                            if (actor->mv->rt_context_b == 0x21) {
                                anim = 0x19E;
                                speed = 1.399999976f;
                            }
                        }
                        if ((actor->mv->inst_context == 7) || (actor->mv->inst_context == 8)) {
                            actor->mv->prop = 0;
                        }
                        if (actor->mv->transport == 1) {
                            actor->yspd = 40.0f;
                        } else {
                            actor->yspd = 59.0f;
                        }
                        if (actor->mv->transport == 2) {
                            actor->yspd = 80.0f;
                            sound = 0x7E2;
                        }
                        actor->ygrav = 4.0f;
                        if ((actor->floorMaterial & 0x1F) == 0xC) {
                            actor->yspd *= 0.6999999881f;
                        }
                        if ((actor->floorMaterial & 0x1F) == 0x13) {
                            anim = 0x32;
                            actor->yspd *= 0.5500000119f;
                        }
                        if (gFloorMaterialClass) {
                            actor->count3 = 0x82;
                            /* never taken: neverSet is always 0 (the compiler folds this away) */
                            if (neverSet) {
                                actor->count5 = 0x82;
                            }
                        } else {
                            actor->count3 = 2;
                        }
                    } else {
                        savedSpeed = actor->speed;
                        if (gFloorMaterialClass == 0) {
                            actor->count3 = 1;
                        } else {
                            actor->count3 = 0x81;
                        }
                        animArg4 = 0.0f;
                        speed = 1.080000043f;
                        actor->norun = 0x17;
                        actor->count5 = 0x64;
                        anim = 0xEB;
                        if (gFloorMaterialClass != 0) {
                            actor->speed = savedSpeed;
                            actor->yspd *= 1.399999976f;
                            actor->ygrav = 3.799999952f;
                        }
                    }
                }
            }
        }
        if (jumped != 0) {
            if (sound != 0) {
                if (event_resolveSoundCommand(actor, &sound, 1) != 0) {
                    actor_playSound(sound, actor);
                }
            }
            prevJumpState = 0;
            if (jumpType != 0) {
                actor->speed = 20.0f;
                if (jumpType == 0x29) {
                    actor_setIntelWalk(actor->attach_char - 1, 0xE);
                } else if (jumpType == 0x8B) {
                    actor_setIntelWalk(actor->attach_char - 1, 5);
                    actor->speed = 30.0f;
                    if (gIsMultiplayer != 0) {
                        actor->langle = 0x3000;
                        if (actor->zpos > 0.0f) {
                            actor->langle = 0xB000;
                        }
                    }
                } else if (jumpType == 0x24) {
                    actor_setIntelWalk(actor->attach_char - 1, 0xE);
                    actor->speed = 27.0f;
                }
                actor->mv->gpoint = 0;
                actor->imune = 0x14;
                actor->angle = actor->langle + 0x1200;
                actor->attach_char = 0;
                actor->norun = 0xFF;
                actor->yspd = 30.0f;
                actor->mv->prop = 0;
            }
            sound = 0;
            actor->mv->save_done = 0;
            if (actor->mv->transport != 0) {
                actor->unkA9 = 5;
            }
            if ((anim != 0xEB) && (anim != 0x279)) {
                actor->count5 = 0;
                actor->shad = 80.0f;
            }
            if (actor->mv->transport != 2) {
                actor_playAnim(actor, anim, speed, 2.0f, animArg4, 0.0f, 0);
            }
            height = 25.0f;
            actor->vtilt = 0.0f;
            actor->unk110 = 0;
            actor->unk100 &= 0x53;
        }
        anim = 0;
    }
    actor_runPlayerAction(actor, buttons, gActorPressedButtons, height, 0);
    {
    s32 pad;
    rate = 2.0f;
    if (((actor->mv->rope != 0) || (actor->mv->ladder != 0)) && (actor->nojump == 0) && (gActorPressedButtons & 0x8000)) {
        actor->angle = actor->langle = actor->mv->press_angle;
        actor->mv->prop = 1;
        actor->count2 = 0xA;
        actor->count5 = 0;
        actor->unk100 |= 8;
        actor->norun = 0;
        actor->unk100 &= 0xDE;
        anim = 0x37;
        speed = 1.299999952f;
        if (actor->inwater == 0) {
            actor->yspd = 40.0f;
            actor->ygrav = 3.200000048f;
        }
        actor->speed = (actor->stickspeed * 0.6000000238f) + 15.0f;
        if (actor->stickspeed < 4.0f) {
            actor->angle = actor->langle = actor->angle + 0x8000;
        }
        actor->mv->ladder = 0;
        actor->mv->ladderFromTop = 0;
        if (actor->mv->rope != 0) {
            rope_release(actor);
        }
        actor->mv->side = 0;
    } else if ((gActorPressedButtons & 0x8000) && (actor->mv->transport == 0)) {
        if ((actor->animseq.uh != 0x3B) && (actor->animseq.uh != 0x3C) && (actor->unk137 == 0) && (actor->lifting == 0) &&
            (actor->mv->ladder == 0) && (actor->mv->prop == 0) && (height > 55.0f)) {
            if ((actor->waterHeight != -10000.0f) && ((actor->waterHeight - actor->floorY) > 100.0f)) {
                actor->ygrav = 6.0f;
                actor->count3 = 1;
                actor->norun = 0x78;
                actor->unk137 = 0;
                anim = 0x3B;
                actor->unk1CC = actor->ypos;
            }
        }
    }
    }
    if ((gIsMultiplayer == 0) && (prevJumpState != 0) && (actor->inwater == 0) && (actor->lifting < 0x65) && (actor->character_type != 0x96)) {
        jumpState = actor->mv->prop;
        if ((jumpState != 0) && (actor->mv->transport == 0)) {
            if (actor->mv->prop < 3) {
                if ((gActorPressedButtons & 0x8000) && (actor->shad > 1.0f)) {
                    actor->speed *= 0.3000000119f;
                    actor->mv->prop = 3;
                    rate = 6.0f;
                    anim = 0xE9;
                    actor->yspd = 0.0f;
                    actor->count3 = 1;
                    actor->unk1CC = actor->ypos;
                    actor->ygrav = 1.0f;
                    if (actor->animseq.uh != 0xE9) {
                        sound_stopActorSound2(actor);
                        sound_addDynamicSound(0x61E, actor->xpos, actor->ypos, actor->zpos,
                                      0x7FFF, 0x1F4, 0xC8, sound_followOwnerAnimsCallback, actor, (s32)D_80099428, 0, 0);
                        func_151C6EA0(actor, 0xFF, 1);
                    }
                }
            } else {
                actor->mv->prop += gFrameSteps;
                if (actor->mv->prop < 0xD) {
                    actor->yspd = 8.0f;
                }
                if (actor->mv->prop < 0x11) {
                    actor->animsp += 0.25f;
                } else {
                    actor->animsp += (0.6000000238f - actor->animsp) * 0.1000000015f;
                }
                anim = 0xE9;
                speed = actor->animsp;
                actor->mv->turn = 6;
                if ((!(pad->button & 0x8000) && (actor->mv->prop >= 0x13)) || (actor->mv->prop >= 0x3E)) {
                    actor->norun = 0xFF;
                    actor->unk100 = 0x55;
                    actor->mv->prop = 0;
                    actor->ygrav = 4.5f;
                    func_1506AF74();
                    anim = 0;
                } else {
                    actor->count2 = 1;
                    actor->unkAA = 0x4B;
                    actor->count5 = 1;
                    if (actor->yspd < -8.0f) {
                        actor->yspd = -8.0f;
                    }
                }
            }
        }
    }
    if (anim != 0) {
        actor_playAnim(actor, anim, speed, rate, 0.0f, 0.0f, 0);
    }
    if (sound != 0) {
        actor_playSound(sound, actor);
    }
    id = actor->count3;
    if (((id & 7) == 2) && !(pad->button & 0x8000)) {
        actor->count3 = id - 1;
        actor->ygrav = 6.199999809f;
    }
}
/* Runs the player's current action/item (mv->context_b, overridden by context) for this frame's input. */
void actor_runPlayerAction(Actor *actor, s32 buttons, s32 pressed, f32 height, s32 nested) {
    s32 stunned;
    s32 state;
    s32 expired;
    s32 held;
    s32 result;
    s32 savedUnk75;
    s32 idx;

    result = 0;
    if (gCutsceneStates[0] == 1) {
        return;
    }
    held = 0;
    stunned = 0;
    expired = 0;

    if ((actor->mv->context_exclude_time != 0) && (nested == 0)) {
        if (gFrameTicks < actor->mv->context_exclude_time) {
            actor->mv->context_exclude_time -= gFrameTicks;
        } else {
            expired = 1;
            actor->mv->context_exclude_time = 0;
        }
    }

    state = actor->mv->context_b & ~0x80;
    if ((actor->mv->unk7C != 0) && (state == 0)) {
        state = actor->mv->unk7C;
    }

    if ((level != 0x29) && (level != 0x2E) && (actor->mv->rt_context_b == 0) &&
        (gCurrentPad->button & 0x20) && (text_isAvailable(state) != 0)) {
        state = 0x1A;
    }

    if (actor->mv->rt_context_b == 0x1A) {
        state = 0x1A;
    }
    if (actor->mv->rt_context_b == 0x2A) {
        state = 0x2A;
    }

    if ((actor->lifting != 0) || (actor->unk137 != 0)) {
        if (nested == 0) {
            state = 0;
        } else {
            held = 1;
        }
    }

    idx = actor->attach_char;
    if ((idx != 0) && (nested == 0)) {
        switch (movement_obj[idx - 1].character_type) {
        case 0x23:
        case 0x8A:
            state = 0x1B;
            break;
        case 0xC:
            state = 8;
            break;
        case 0x28:
            state = 0;
            break;
        default:
            state = 0xE;
            break;
        }
    }

    if (actor->mv->env_suit != 0) {
        state = 0x3C;
    }

    if ((nested == 0) && actor->mv->rt_context_b && (state != (actor->mv->rt_context_b & ~0x80))) {
        savedUnk75 = actor->mv->context_b;
        actor->mv->context_b = actor->mv->rt_context_b;
        actor_runPlayerAction(actor, buttons, pressed, height, 1);
        actor->mv->context_b = savedUnk75;
        actor->mv->rt_context_b = 0;
    }

    if (actor->stunned != 0) {
        stunned = 1;
        pressed = 0;
        buttons = 0;
    }

    if ((state != 0) && (actor->mv->transport == 0)) {
        result = func_15065A5C(actor, buttons, pressed, height, nested, state, stunned, expired, held);
    }

    if ((actor->mv->inst_context != 0) || (pressed & 0x4000)) {
        func_150642AC(actor, pressed, height, held, result);
    }
}
/*
 * Plays sound `sound` on an actor that has a camera (a player). flags & 3 picks the handle slot it
 * reuses (1: soundHandle1, 2/3: soundHandle2; 3 only retunes it if the same sound is still playing), flags & 4 makes
 * it positional, flags & 8 forces channel 0.
 */
void actor_playPlayerSound(s32 sound, Actor *actor, u16 rate, s32 volume, s32 minDist, s32 maxDist, s32 flags) {
    s32 channel;
    u32 handle;
    u16 *slot;

    handle = 0;
    slot = NULL;
    if (flags & 8) {
        channel = 0;
    } else {
        channel = (actor->floorMaterial >> 3) & 0x30;
    }
    switch (flags & 3) {
    case 1:
        slot = &actor->soundHandle1;
        handle = actor->soundHandle1;
        break;
    case 2:
    case 3:
        slot = &actor->soundHandle2;
        handle = actor->soundHandle2;
        if ((flags & 3) == 2) {
            break;
        }
        if ((sound_getPlayingSound(handle) == sound) & 0x7FFF) {
            if (flags & 4) {
                sound_setVoiceSpatial(handle, rate + channel * 50, actor->listpos * 10 + volume, channel, 0,
                              actor->xpos, actor->ypos, actor->zpos, minDist, maxDist);
            } else {
                sound_setVoiceParam(handle, 0x10, volume);
                sound_setVoiceParam(handle, 8, rate + channel * 50);
            }
            return;
        }
        sound |= 0x8000;
        break;
    }
    if (flags & 4) {
        handle = sound_startVoiceAt(handle, sound, rate + channel * 50, volume, channel, 0, actor->xpos,
                               actor->ypos, actor->zpos, minDist, maxDist);
    } else {
        handle = sound_startVoice(handle, sound, (rate + channel * 50), 0x40, volume, channel, 1);
    }
    if (slot != NULL) {
        *slot = handle;
    }
}
/* Plays an actor sound (players use handle slot 1). */
void actor_playSound(s32 sound, Actor *actor) {
    if (actor->CameraPtr == NULL) {
        sound_playActorSound1(sound, actor, 28000, 500, 2500);
    } else {
        actor_playPlayerSound(sound, actor, 24000, 0, 500, 2500, 1);
    }
}
/* Plays an actor sound without a handle slot. */
void actor_playUntrackedSound(s32 sound, Actor *actor) {
    if (actor->CameraPtr == NULL) {
        sound_playActorUntracked(sound, actor, 24000, 500, 2500);
    } else {
        actor_playPlayerSound(sound, actor, 24000, 0, 500, 2500, 0);
    }
}

/* actor_playUntrackedSound with an explicit rate. */
void actor_playUntrackedSoundRate(s32 sound, Actor *actor, s32 rate) {
    if (actor->CameraPtr == NULL) {
        sound_playActorUntracked(sound, actor, rate, 500, 2500);
    } else {
        actor_playPlayerSound(sound, actor, rate, 0, 500, 2500, 0);
    }
}

/* Plays a sound at the actor. */
void func_15060B70(s32 sound, void *actor) {
    sound_playActorSound2(sound, actor, 28000, 500, 2500);
}

/* Adds health up to the maximum of 6; returns FALSE when already full. */
s32 actor_addHealth(Actor *actor, s32 amount) {
    if (actor->energy == 6) {
        return FALSE;
    }
    actor->energy += amount;
    if (actor->energy >= 7) {
        actor->energy = 6;
    }
    return TRUE;
}

/* Stores the actor's head position (s16 at 0x1BC): headHeight along its head bone's up axis, or its origin. */
void actor_updateHeadPos(Actor *actor) {
    f32 pad0; // matching: pad0..pad3 reproduce the stack layout
    f32 pad1;
    f32 pad2;
    f32 pad3;
    f32 offZ;
    f32 len;
    f32 upX;
    f32 upY;
    f32 upZ;
    f32 height;
    f32 offY;
    f32 scale;
    f32 offX;
    s32 done;
    ModelDef *info;
    f32 (*bone)[4];

    done = FALSE;
    if (actor->character_type != 0xFF) {
        if ((actor->ob_matrix_ptr != 0) && (gBoneMatricesFlushed == 0)) {
            info = gModelDefs[actor->character_type];
            bone = actor->ob_matrix_ptr[info->headBone];
            height = info->headHeight * actor->scale_y;
            if (height) {
                upX = bone[1][0];
                upY = bone[1][1];
                upZ = bone[1][2];
                len = sqrtf((upX * upX) + (upY * upY) + (upZ * upZ));
                scale = len;
                if (len) {
                    scale = height / len;
                }
                offX = upX * scale;
                offY = upY * scale;
                offZ = upZ * scale;
            } else {
                offY = (offZ = 0.0f);
                offX = offY;
            }
            done = TRUE;
            actor->unk1BC[0] = (bone[3][0] + offX);
            actor->unk1BC[1] = (bone[3][1] + offY);
            actor->unk1BC[2] = (bone[3][2] + offZ);
        }
    }
    if (done == FALSE) {
        actor->unk1BC[0] = actor->xpos;
        actor->unk1BC[1] = actor->ypos;
        actor->unk1BC[2] = actor->zpos;
    }
}
/* Detaches every actor carried by (unk65 pointing at) this one. */
void actor_releaseCarried(Actor *actor) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type != 0) && ((actor - movement_obj) + 1 == movement_obj[i].attach_char)) {
            movement_obj[i].attach_char = 0;
        }
    }
}

/* Removes the actor: releases its resources and attachments, then clears its slot. */
void actor_remove(Actor *actor, s32 mode) {
    effect_freeAttachments(actor);
    effect_broadcastActorEvent(actor, 0);
    if ((actor->drawtype != 2) && (actor->drawtype != 3)) {
        collision_freeObjectModels(actor);
        actor_releaseCarriedNonPlayers(actor, mode);
    }
    if (mode == 1) {
        actor_updateAttachedObject(actor, 1, 0);
        model_stopParts(actor - movement_obj);
    }
    if (actor->listpos != 0xFF) {
        if (mode != 2) {
            /* matching: spawn is read as an s32 at each use; pointer reads let IDO keep it in a register */
            if ((*(s32 *)&actor->listpos_ptr) != 0) {
                if (((SpawnRecord *)*(s32 *)&actor->listpos_ptr)->flags & 0x20) {
                    ((SpawnRecord *)*(s32 *)&actor->listpos_ptr)->unk2 = 1;
                    gSpawnInUse[actor->listpos] = 0;
                } else if (((SpawnRecord *)*(s32 *)&actor->listpos_ptr)->flags & 1) {
                    gSpawnInUse[actor->listpos] = 0;
                } else {
                    s32 pad;
                    ((SpawnRecord *)*(s32 *)&actor->listpos_ptr)->unk2 = 1;
                    gSpawnInUse[actor->listpos] &= 0x80;
                }
            } else {
                gSpawnInUse[actor->listpos] &= 0x80;
            }
        } else {
            gSpawnInUse[actor->listpos] = 0;
        }
    }
    /* free the actor's allocations */
    if (actor->morph != NULL) {
        heap_free(actor->morph);
    }
    if (actor->hit_vtx_array != NULL) {
        heap_free(actor->hit_vtx_array);
    }
    if (actor->hit_box_array != NULL) {
        heap_free(actor->hit_box_array);
    }
    if (actor->hit_ylimit_array != NULL) {
        heap_free(actor->hit_ylimit_array);
    }
    if (actor->prevBones != NULL) {
        heap_free(actor->prevBones);
    }
    if (actor->blobShadow != NULL) {
        effect_destroyObject(actor->blobShadow);
    }
    if (actor->unk304[0] != 0) {
        s32 i;
        for (i = 0; i <= gScreenLayout; i = (u8)(i + 1)) {
            heap_setTag(actor->unk304[i], 4);
        }
    }
    // matching: unk314 is read as an s32 (one load feeds both the test and the call)
    if (*(s32 *)&actor->unk314 != 0) {
        heap_setTag((s32 *)*(s32 *)&actor->unk314, 4);
    }
    if (actor->unk2D0 != NULL) {
        heap_free(actor->unk2D0);
    }
    if (actor->camData != NULL) {
        heap_free(actor->camData);
    }
    if ((actor->mv != NULL) && (actor->mv->boneChain != NULL)) {
        heap_free(actor->mv->boneChain);
    }
    sound_stopActorSounds(actor);
    attach_removeAll(actor);
    actor_releaseCarried(actor);
    trail_removeActorAfterimages(actor);
    if (actor->mv != NULL) {
        heap_free((void *)(s32)actor->mv); // matching: passing actor->mv directly mismatches
    }
    actor_clearSlot(actor);
}

/* Drifts the actor along water-current path pathIdx, from the nearest of its points. */
void actor_driftOnCurrent(Actor *actor, s32 pathIdx) {
    s32 count;
    s32 closest;
    s32 i;
    s32 best;
    s16 x;
    s16 z;
    PathPoint *pts;
    f32 speed;
    f32 scale;
    s32 dx;
    s32 dz;
    u16 angle;
    s32 dist;

    count = gSpawnPathCounts[pathIdx];
    if (count < 3) {
        return;
    }
    x = actor->xpos;
    z = actor->zpos;
    best = 0x7FFFFFFF;
    for (i = 0; i < count; i++) {
        dx = x - gSpawnPaths[pathIdx][i].x;
        dz = z - gSpawnPaths[pathIdx][i].z;
        if (dx * dx + dz * dz < best) {
            closest = i;
            best = dx * dx + dz * dz;
        }
    }
    if (closest != 0 && closest != count - 1) {
        pts = gSpawnPaths[pathIdx];
        if (best > 28000) {
            dx = x - pts[closest + 1].x;
            dz = z - pts[closest + 1].z;
            dist = dx * dx + dz * dz;
            dx = x - pts[closest - 1].x;
            dz = z - pts[closest - 1].z;
            if (dx * dx + dz * dz < dist) {
                closest--;
            }
        }
        if (pts[closest + 1].param == 0) {
            speed = 8.0f;
        } else {
            speed = pts[closest + 1].param;
        }
        scale = speed * 0.0025f;
        if (scale > 1.0f) {
            scale = 1.0f;
        }
        angle = actor_getDirectionAngle(pts[closest + 1].x - pts[closest].x, pts[closest].z - pts[closest + 1].z, 0);
        if (actor->mv != NULL && speed > 20.0f) {
            actor->mv->unk68 = angle | 1;
        }
        actor_easePushVelocity(actor, angle, speed, scale);
    }
}

// TODO: is this really Actor?
/* Resets the actor's bytes 0x7..0xF. */
void actor_resetFade(Actor *actor) {
    actor->alpha = 0xFF;
    actor->targetAlpha = 0xFF;
    actor->fadeStep = 0;
    actor->fadeMode = 0;
    actor->playerFadeBits = 0;
    actor->playerAlphas[0] = actor->playerAlphas[1] = actor->playerAlphas[2] = actor->playerAlphas[3] = 0xFF;
}

/*
 * Actor fading. alpha is faded toward targetAlpha by fadeStep per frame in mode fadeMode (4: in, 5/6: out,
 * then removed). playerAlphas are per-player alphas, playerFadeBits holds per-player hide bits
 * (low nibble) and pending bits (high nibble). Commands 8 and up act on player `player` only.
 */
void actor_fadeCommand(Actor *actor, u8 cmd, u8 value, u8 step, s32 player) {
    s32 bit;
    s32 hi;

    if (cmd >= 8) {
        if (cmd != 10) {
            if (cmd == 8) {
                bit = 1 << (u8)player;
                actor->playerFadeBits |= bit;
                value = 0;
            } else {
                bit = 1 << (u8)player;
                actor->playerFadeBits &= ~bit;
                value = 0xFF;
            }
            hi = bit << 4;
            if (actor->playerFadeBits & hi) {
                actor->playerAlphas[(u8)player] = value;
            }
            actor->playerFadeBits &= ~hi;
        } else {
            actor->playerFadeBits |= (1 << (u8)player) << 4;
            value = 0;
        }
        if ((actor->unk2FD != 0) || (gCameraSnapTimers[(u8)player] != 0)) {
            actor->playerAlphas[(u8)player] = value;
        }
    } else if (cmd >= actor->fadeMode) {
        if (cmd == 4) {
            actor->fadeMode = cmd;
            actor->alpha = 0;
            actor->targetAlpha = 0xFF;
            actor->fadeStep = 0x20;
        } else if (cmd == 5) {
            actor->fadeMode = cmd;
            actor->targetAlpha = 0;
            actor->fadeStep = 0x20;
        } else if (cmd == 6) {
            actor->fadeMode = cmd;
            actor->targetAlpha = 0;
            actor->fadeStep = 8;
        } else if (cmd == 2) {
            actor->fadeMode = cmd;
            actor->targetAlpha = value;
            actor->fadeStep = step;
        } else if (cmd == 1) {
            actor->fadeMode = 0;
            actor->alpha = value;
            actor->targetAlpha = value;
        }
        if (*(s32 *)gCameraSnapTimers != 0) {
            actor->alpha = actor->targetAlpha;
        }
    }
}

/* Per-frame fade step (see actor_fadeCommand); removes the actor when a fade-out finishes. */
void actor_updateFade(Actor *actor) {
    s32 i;
    s32 mask;
    s32 value;

    if (actor->type != 0) {
        mask = 1;
        for (i = 0; i <= gScreenLayout; i++) {
            if (actor->playerFadeBits & mask) {
                if (actor->playerAlphas[i] > 0) {
                    if (gCameraSnapTimers[i] == 0) {
                        value = actor->playerAlphas[i] - 50;
                        if (value < 0) {
                            value = 0;
                        }
                    } else {
                        value = 0;
                    }
                    actor->playerAlphas[i] = value;
                }
            } else {
                if (actor->playerAlphas[i] < 0xFF) {
                    if (gCameraSnapTimers[i] == 0) {
                        value = actor->playerAlphas[i] + 50;
                        if (value > 0xFF) {
                            value = 0xFF;
                        }
                    } else {
                        value = 0xFF;
                    }
                    actor->playerAlphas[i] = value;
                }
            }
            mask <<= 1;
        }
        /* matching: byte indexing (7: alpha, 8: targetAlpha, 9: fadeStep, 0xA: fadeMode); field access loads in another order */
        if (((u8 *)actor)[0xA] != 0) {
            if (((u8 *)actor)[8] != ((u8 *)actor)[7]) {
                if (((u8 *)actor)[7] < ((u8 *)actor)[8]) {
                    value = ((u8 *)actor)[9] + ((u8 *)actor)[7];
                    if (((u8 *)actor)[8] < value) {
                        value = ((u8 *)actor)[8];
                    }
                } else {
                    value = ((u8 *)actor)[7] - ((u8 *)actor)[9];
                    if (value < ((u8 *)actor)[8]) {
                        value = ((u8 *)actor)[8];
                    }
                }
                ((u8 *)actor)[7] = value;
            }
            if (((u8 *)actor)[8] == ((u8 *)actor)[7]) {
                if (((u8 *)actor)[0xA] == 6) {
                    actor_remove(actor, 1);
                } else if (((u8 *)actor)[0xA] == 5) {
                    actor_remove(actor, 2);
                } else {
                    ((u8 *)actor)[0xA] = 0;
                }
            }
        }
    }
}

/* The actor's alpha as seen by `player` (0..255). */
s32 actor_getAlpha(Actor *actor, s32 player) {
    s32 alpha = actor->alpha * actor->playerAlphas[player];

    if (alpha == 255 * 255) {
        alpha = 255;
    } else {
        alpha = alpha >> 8;
    }
    return alpha;
}

/*
 * Per-frame fade and visibility update of all actors: activeFlags bit 0 is set when actor_isVisible says the
 * actor is visible, or when one of its linked actors (keys in unk20B) is.
 */
void actor_updateVisibility(void) {
    s32 i;
    s32 count;
    u8 list[25];
    s32 j;
    Actor *actor;
    Actor *other;
    s32 idx;

    count = 0;
    for (i = 0; i < 25; i++) {
        actor = &movement_obj[i];
        if (actor->type != 0) {
            if (actor->unk2FD != 0) {
                actor->unk2FD--;
            }
            actor_updateFade(actor);
            if (actor_isVisible(actor) != 0) {
                actor->activeFlags |= 1;
            } else {
                actor->activeFlags &= ~1;
                if (actor->unk20B[0] != 0 || actor->unk20B[1] != 0 || actor->unk20B[2] != 0 || actor->unk20B[3] != 0) {
                    list[count] = i;
                    count++;
                }
            }
        }
    }
    if (count != 0) {
        for (i = 0; i < count; i++) {
            idx = list[i];
            actor = &movement_obj[idx];
            for (j = 0; j < 4; j++) {
                if (actor->unk20B[j] != 0) {
                    other = collision_findObjectById(actor->unk20B[j]);
                    if (other != NULL && (other->activeFlags & 1)) {
                        actor->activeFlags |= 1;
                        break;
                    }
                }
            }
        }
    }
}


/* Per-frame texture/morph animations of actor types that have them (once per type for most). */
void actor_updateTypeAnims(void) {
    Actor *unused; // matching: stack slot
    s32 id;
    s32 i;
    u8 done[10];

    bzero(done, sizeof(done));
    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type == 0) || (movement_obj[i].activeFlags == 0)) {
            continue;
        }
        id = movement_obj[i].character_type;
        if ((id == 0x13) && (done[0] == 0)) {
            actor_scrollTypeTexture(id, 0x5E, 0, -1, 0, 0);
            actor_scrollTypeTexture(id, 0x60, 0, -1, 0, 0);
            actor_scrollTypeTexture(id, 0x62, 0, -1, 0, 0);
            actor_scrollTypeTexture(id, 0x64, 0, -1, 0, 0);
            done[0] = 1;
        } else if ((id == 0x23) && (done[1] == 0)) {
            actor_scrollTypeTexture(id, 0x1E9, 0, -1, 0, 0);
            actor_scrollTypeTexture(id, 0x1EB, 0, -1, 0, 0);
            actor_scrollTypeTexture(id, 0x1ED, 0, -1, 0, 0);
            actor_scrollTypeTexture(id, 0x1EF, 0, -1, 0, 0);
            done[1] = 1;
        } else if ((id == 0x24) && (done[2] == 0)) {
            if ((math_random() % 1000U) < 300) {
                D_800CC298.unk8 = (math_random() % 5U) - 2;
            }
            actor_scrollTypeTexture(id, 4, -4, D_800CC298.unk8, 0, 0);
            if (gModelVtxIndices[id] != NULL) {
                actor_rotateModelTexCoords(id, gModelVtxIndices[id][0], 0x37, 0, D_800CC298.unk4, 2, 1);
                actor_rotateModelTexCoords(id, gModelVtxIndices[id][1], 0x27, 0x37, D_800CC298.unk0, 1, 0);
            }
            D_800CC298.unk0 += -4.0f * gFrameTicks;
            if (D_800CC298.unk0 <= 0.0f) {
                D_800CC298.unk0 += 360.0f;
            }
            D_800CC298.unk4 += -0.5f * gFrameTicks;
            if (D_800CC298.unk4 <= 0.0f) {
                D_800CC298.unk4 += 360.0f;
            }
            done[2] = 1;
        } else if ((id == 0x2C) && (done[3] == 0)) {
            if (D_800CC298.unk9 >= 0x29) {
                D_800CC298.unkA = -1;
            } else if (D_800CC298.unk9 <= 0) {
                D_800CC298.unkA = 1;
            }
            D_800CC298.unk9 += D_800CC298.unkA;
            actor_scrollTypeTexture(id, 0xAA, D_800CC298.unk9, 0, 0, 1);
            done[3] = 1;
        } else if ((id == 0x3A) && (done[4] == 0)) {
            f32 weights[4];
            s32 j;
            u8 step[4] = { 4, 6, 4, 5 };

            if (movement_obj[i].ob_matrix_ptr != NULL) {
                for (j = 0; j < 4; j++) {
                    weights[j] = (math_cosBinAngle(D_800CC298.unkB[j]) + 1.0f) * 0.5f;
                    D_800CC298.unkB[j] += step[j];
                }
                model_rampVertexGroupColours(&movement_obj[i], weights, 1);
                done[4] = 1;
            }
        } else if ((id == 0x3F) && (done[5] == 0)) {
            actor_scrollTypeTexture(id, 4, 0, -4, 0, 0);
            done[5] = 1;
        } else if (id == 0x4B) {
            if (done[6] == 0) {
                done[6] = 1;
                actor_scrollTypeTexture(id, 0x173, -4, 0, 0, 0);
                actor_scrollTypeTexture(id, 0x175, -2, 0, 0, 0);
                actor_scrollTypeTexture(id, 0x177, -1, 0, 0, 0);
            }
            if (D_800CC298.unkF >= 4) {
                D_800CC298.unkF = 0;
                if (movement_obj[i].unk68[1] == 5) {
                    movement_obj[i].unk68[1] = 0;
                } else {
                    movement_obj[i].unk68[1]++;
                }
                if (movement_obj[i].unk68[1] >= 3) {
                    movement_obj[i].unk68[0] = 5 - movement_obj[i].unk68[1];
                } else {
                    movement_obj[i].unk68[0] = movement_obj[i].unk68[1];
                }
                movement_obj[i].unk68[0] += 0xF;
            } else {
                D_800CC298.unkF += gFrameTicks;
            }
        } else if (id == 0x45) {
            s32 val = movement_obj[i].eyeCode[1];

            if (val >= 10) {
                val -= 10;
            } else {
                val = val * 2 + 6;
            }
            movement_obj[i].unk68[0] = val;
        } else if (id == 0x5B) {
            s32 v;
            s32 k;

            // matching: eyeCode[k] / unk68[k] as fields change how IDO strength-reduces k
            for (k = 0; k < 2; k++) {
                v = ((u8 *)&movement_obj[i])[0x6C + k]; // eyeCode[k]
                if (v >= 10) {
                    v -= 10;
                } else {
                    v = D_8009942C[v];
                }
                ((u8 *)&movement_obj[i])[0x68 + k] = v; // unk68[k]
            }
        } else if (id == 0x54) {
            f32 scale;

            if (movement_obj[i].taskint[2] != 0) {
                if (D_800CC298.unk10 < 0xEB) {
                    D_800CC298.unk10 += 0x14;
                } else {
                    D_800CC298.unk10 = 0xFF;
                }
            } else if (D_800CC298.unk10 >= 0x15) {
                D_800CC298.unk10 -= 0x14;
            } else {
                D_800CC298.unk10 = 0;
            }
            scale = D_800CC298.unk10 * 0.003921568859f;
            model_rampVertexGroupColours(&movement_obj[i], &scale, 0);
        } else if (id == 0x8A) {
            s32 delta;
            s32 pad;

            if (level != 0x30) {
                if (movement_obj[i].animseq.uh == 1) {
                    actor_scrollTypeTexture(id, 0x253, 0, 0x10, 0, 0);
                    actor_scrollTypeTexture(id, 0x255, 0, 8, 0, 0);
                    actor_scrollTypeTexture(id, 0x257, 0, 4, 0, 0);
                }
            } else {
                if (movement_obj[i].animseq.uh == 1) {
                    delta = gFrameTicks * -40;
                } else {
                    delta = 0;
                }
                actor_scrollVertexTexCoords(&movement_obj[i], 0, 0, 0x1000, 0x1000, 0xC, 0, delta);
            }
        } else if (id == 0x42) {
            if ((movement_obj[i].animseq.uh == 2) || (movement_obj[i].animseq.uh == 1)) {
                if (movement_obj[i].unk68[0] == 0xD) {
                    movement_obj[i].unk68[0] = 0xC;
                }
                if ((math_random() & 3) == 0) {
                    if (movement_obj[i].unk68[0] == 0x15) {
                        movement_obj[i].unk68[0] = 0xC;
                    } else {
                        movement_obj[i].unk68[0] = 0x15;
                    }
                }
            } else {
                movement_obj[i].unk68[0] = 0xD;
            }
        } else if (id == 0x7B) {
            func_150F1CB0(&movement_obj[i]);
        } else if ((id == 0x92) && (done[9] == 0)) {
            actor_scrollTypeTexture(id, 4, 0, -2, 0, 0);
            actor_scrollTypeTexture(id, 0x13B, 0, -2, 0, 0);
            actor_scrollTypeTexture(id, 0x20D, 0, -2, 0, 0);
            done[9] = 1;
        } else if (id == 0x43) {
            actor_scrollTypeTexture(id, 4, 0x14, 8, 0, 0);
        }
    }
}

/*
 * Whether the actor should be drawn: TRUE when it has no cull radius (spawn->unk2A) or is a player, when
 * a player is inside its path area, or when it is inside some player's view; FALSE if fully faded.
 */
s32 actor_isVisible(Actor *actor) {
    f32 scale;
    s32 sum;
    s32 hidden;
    s32 i;
    u8 idx;
    f32 dx;
    f32 dz;
    f32 height;
    s32 id;
    u8 *p;

    scale = (actor->listpos_ptr != NULL) ? actor->listpos_ptr->unk2A : 0.0f;
    if (scale == 0.0f) {
        return 1;
    }
    if (actor->playernumber != 0xFF) {
        return 1;
    }
    if ((gCutsceneActorLock != 0) && (cutscene_isActorBound(actor) != 0)) {
        return 1;
    }
    sum = 0;
    p = (u8 *)actor;
    if (gScreenLayout >= 0) {
        do {
            sum += p[0xB];
            p++;
        } while (p <= (0, gScreenLayout) + (u8 *)actor); // matching: the comma expression shapes the loop test
    }
    if (sum == 0) {
        return 0;
    }
    if ((actor->cullMask & 0xF) != 0xF) {
        return 1;
    }
    idx = actor->listpos;
    if (gSpawnPathCounts[idx] != 0) {
        hidden = 1;
        for (i = 0; i < gPlayerCount; i++) {
            dx = gSpawnPaths[idx]->x - movement_obj[i].xpos;
            dz = gSpawnPaths[idx]->z - movement_obj[i].zpos;
            if ((dx * dx) + (dz * dz) < gSpawnPaths[idx]->param * gSpawnPaths[idx]->param) {
                hidden = 0;
                break;
            }
        }
        if (!hidden) {
            return 1;
        }
    }
    id = actor->character_type;
    height = gModelDefs[id]->bodyHeight * actor->scale_y;
    for (i = 0; i <= gScreenLayout; i++) {
        hidden = 1;
        if (view_testCylinderInView(&gViews[i], gViewMatrices[i], actor->xpos,
                          actor->ypos + height, actor->zpos, scale, scale, 20000.0f) != 0) {
            hidden = 0;
            break;
        }
    }
    if (!hidden) {
        return 1;
    }
    return 0;
}
/* Removes the non-player actors carried by this one. */
void actor_releaseCarriedNonPlayers(Actor *actor, s32 mode) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type != 0) &&
            (((actor - movement_obj) + 1) == movement_obj[i].attach_char) &&
            (movement_obj[i].playernumber == 0xFF)) {
            actor_remove(&movement_obj[i], mode);
        }
    }
}

/* Called after the actor's animation changed. */
void actor_onAnimChanged(Actor *actor) {
    actor->unk2FB = 0;
    bone_updateChainPoseFlags(actor);
    aim_updateLock(actor);
}


#define OBJ_INDEX(obj) ((u32)((obj) - movement_obj))

/* Swaps two actor slots (b becomes the player with a camera), fixing up indices and carry links. */
void actor_swapSlots(Actor *a, Actor *b) {
    s32 idx0; // matching: idx0, cam and pad are dead, but keep the frame layout
    Camera *cam;
    Actor swap;
    SpawnRecord *pad;
    s32 i;
    s32 u;

    math_broadcastActorSwap(a, b);
    bcopy(b, &swap, sizeof(Actor));
    bcopy(a, b, sizeof(Actor));
    bcopy(&swap, a, sizeof(Actor));
    idx0 = OBJ_INDEX(a);
    cam = &camera_global_Cameras[OBJ_INDEX(b)];
    b->playernumber = OBJ_INDEX(b);
    b->unique_id = OBJ_INDEX(b) + 1;
    b->listpos = OBJ_INDEX(b);
    b->CameraPtr = &camera_global_Cameras[OBJ_INDEX(b)];
    a->playernumber = OBJ_INDEX(a);
    a->unique_id = OBJ_INDEX(a) + 1;
    a->listpos = OBJ_INDEX(a);
    a->CameraPtr = NULL;
    u = (s32)a->listpos_ptr;
    a->listpos_ptr = b->listpos_ptr;
    b->listpos_ptr = (SpawnRecord *)u;
    camera_global_Cameras[OBJ_INDEX(b)].mv = b->mv;
    b->mv->SniperState = 0;
    attach_swapOwners(b->unique_id, a->unique_id);
    if (b->blobShadow != NULL) {
        effect_destroyObject(b->blobShadow);
        b->blobShadow = NULL;
    }
    if (a->blobShadow != NULL) {
        effect_destroyObject(a->blobShadow);
        a->blobShadow = NULL;
    }
    actor_resetFade(b);
    for (i = 0; i < 25; i++) {
        if (i == OBJ_INDEX(b) || i == OBJ_INDEX(a)) {
            continue;
        }
        if (movement_obj[i].type == 0) {
            continue;
        }
        u = movement_obj[i].attach_char;
        if (u != 0) {
            u--;
            if (u == OBJ_INDEX(b)) {
                movement_obj[i].attach_char = OBJ_INDEX(a) + 1;
            } else if (u == OBJ_INDEX(a)) {
                movement_obj[i].attach_char = OBJ_INDEX(b) + 1;
            }
        }
    }
    if (b->lifting >= 100) {
        u8 x = b->lifting - 100;
        movement_obj[x].carried = OBJ_INDEX(b) + 100;
    }
    actor_onAnimChanged(b);
}

/* Recomputes the body cylinder ratios: unkEC = radius / height, unkF0 = height / radius (radius bodyRadius, height bodyHeight). */
void actor_updateBodyRatios(Actor *actor) {
    if (actor->bodyHeight && actor->bodyRadius) {
        f32 radius = actor->bodyRadius;
        actor->unkEC = radius / actor->bodyHeight;
        actor->unkF0 = actor->bodyHeight / radius;
    } else {
        actor->unkF0 = actor->unkEC = 0.0f;
    }
}

/* Sets the body radius to scale_xz * radius. */
void actor_setBodyRadius(Actor *actor, f32 radius) {
    actor->bodyRadius = actor->scale_xz * radius;
    actor_updateBodyRatios(actor);
}

/* Sets the body height to scale_y * height. */
void actor_setBodyHeight(Actor *actor, f32 height) {
    actor->bodyHeight = actor->scale_y * height;
    actor_updateBodyRatios(actor);
}

/* actor_updateBodyRatios for the hit cylinder (radius unkD2, height unkD4). */
void actor_updateHitRatios(Actor *actor) {
    if (actor->unkD4 && actor->unkD2) {
        f32 radius = actor->unkD2;
        actor->unkDC = radius / actor->unkD4;
        actor->unkE0 = actor->unkD4 / radius;
    } else {
        actor->unkDC = 0.0f;
        actor->unkE0 = 0.0f;
    }
}

/* Sets the actor's scale and rescales its hit and body cylinders from the type info. */
void actor_setScale(Actor *actor, f32 xzScale, f32 yScale) {
    ModelDef *info;

    actor->scale_xz = xzScale;
    actor->scale_y = yScale;
    if (actor->character_type != 255) {
        info = gModelDefs[actor->character_type];
        actor->unkD2 =  (info->unk20 * actor->scale_xz);
        actor->unkD4 =  (info->unk22 * actor->scale_y);
        actor->unkD6 =  (info->unk24 * actor->scale_y);
        actor->bodyRadius =  (info->unk1A * actor->scale_xz);
        actor->bodyHeight =  (info->unk1C * actor->scale_y);
        actor->bodyOffsetY =  (info->unk1E * actor->scale_y);
        actor_updateBodyRatios(actor);
        actor_updateHitRatios(actor);
    }
}

/*
 * Scrolls texture tile `tile` of actor type `type` (set `set`) by (ds, dt), wrapping at the tile size;
 * with `absolute` set the coordinates are set instead. Coordinates are packed 12:12 in unk0.
 */
void actor_scrollTypeTexture(s32 type, s32 tile, s32 ds, s32 dt, s32 set, s32 absolute) {
    Gfx *base, *entry; /* G_SETTILESIZE-style words: w0 = flags in the top byte, s:t packed 12:12; w1 = tile size - 2 */
    s32 packed, wrapS, wrapT, newS, newT, tmp, flag;

    newS = 0;
    wrapS = 0;
    wrapT = 0;
    if (gModelData[type] == NULL) {
        return;
    }
    base = gModelDlParts[type][set];
    packed = ((Gfx *)((u8 *)base + tile * 8))->words.w1; // matching: byte offsets, not base[tile]
    wrapS = ((packed >> 12) & 0xFFF) + 2;
    wrapT = (packed & 0xFFF) + 2;
    entry = ((Gfx *)((u8 *)base + tile * 8));
    packed = entry->words.w0;
    flag = absolute;
    if (flag != 0) {
        newS = ds & 0xFFF;
    } else {
        tmp = ((packed >> 12) & 0xFFF) + ds;
        newS = tmp;
        if (wrapS < tmp) {
            newS = tmp - wrapS;
        } else if (tmp < 0) {
            newS = tmp + wrapS;
        }
    }
    if (flag != 0) {
        newT = dt & 0xFFF;
    } else {
        tmp = (packed & 0xFFF) + dt;
        newT = tmp;
        if (wrapT < tmp) {
            newT = tmp - wrapT;
        } else if (tmp < 0) {
            newT = tmp + wrapT;
        }
    }
    packed = (packed & 0xFF000000) | (newT & 0xFFF) | ((newS & 0xFFF) << 12);
    entry->words.w0 = packed;
}

/* Rewrites texture coordinates in the current vertex buffer of every visible actor of `type`. */
void actor_rotateModelTexCoords(s32 type, s32 firstVtx, s32 arg2, s32 arg3, s32 arg4, s32 set, s32 doubleSize) {
    s32 i;
    s32 w;
    s32 h;
    Vtx *vtx;

    if (gModelSavedTexCoords[type] != 0) {
        for (i = 0; i < 25; i++) {
            if (movement_obj[i].type == 0) continue;
            if (type != movement_obj[i].character_type) continue;
            if (movement_obj[i].activeFlags == 0) continue;
            w = gModelTextures[type][set].width;
            h = gModelTextures[type][set].height;
            if (doubleSize != 0) {
                w *= 2;
                h *= 2;
            }
            vtx = ((Vtx **)movement_obj[i].modelData)[gDrawBufferIndex];
            if (vtx != NULL) {
                vtx += firstVtx;
                sprite_rotateTexcoords(vtx, (s16 (*)[2])gModelSavedTexCoords[type] + arg3, arg2, arg4, h, w);
            }
        }
    }
}

/*
 * Scrolls the texture coordinates of `count` vertices (vertex group `group`, or from the start) in the
 * actor's current vertex buffer, copying from the other buffer and wrapping at (wrapS, wrapT).
 */
void actor_scrollVertexTexCoords(Actor *actor, s32 group, s32 arg2, s32 wrapS, s32 wrapT, s32 count, s32 ds, s32 dt) {
    u8 *channelBase;
    Vtx *source;
    Vtx *target;
    s32 offset;
    s32 i;

    /* two vertex buffers per channel at 0x28C (double-buffered by gDrawBufferIndex) */
    channelBase = (u8 *)actor + (actor->current_lod * 8);
    target = ((Vtx **)(channelBase + 0x28C))[gDrawBufferIndex];
    source = ((Vtx **)(channelBase + 0x28C))[!gDrawBufferIndex];
    if (target == NULL) {
        return;
    }
    if (group != -1) {
        offset = gModelVtxIndices[actor->character_type][group] * sizeof(Vtx);
        target = (Vtx *)((u8 *)target + offset);
        source = (Vtx *)((u8 *)source + offset);
    }
    if (ds != 0) {
        if (wrapS < target->v.tc[0]) {
            ds -= wrapS;
        } else if (target->v.tc[0] < 0) {
            ds += wrapS;
        }
    }
    if (dt != 0) {
        if (wrapT < target->v.tc[1]) {
            dt -= wrapT;
        } else if (target->v.tc[1] < 0) {
            dt += wrapT;
        }
    }
    for (i = 0; i < count; i++) {
        target[i].v.tc[0] = source[i].v.tc[0] + ds;
        target[i].v.tc[1] = source[i].v.tc[1] + dt;
    }
}

/* Calls effect_setOnFire for every other player touching the actor this frame (gCollidedActors mask). */
void actor_igniteTouching(Actor *actor) {
    s32 i;
    s32 self;
    ActorCtl *movement;

    self = actor - movement_obj;
    for (i = 0; i < gPlayerCount; i++) {
        if (i == self) {
            continue;
        }
        if (((1 << i) & gCollidedActors) == 0) {
            continue;
        }
        movement = movement_obj[i].mv;
        if (movement == NULL) {
            continue;
        }
        if (movement->on_fire != 0) {
            continue;
        }
        effect_setOnFire(actor, &movement_obj[i], 1);
    }
}

/* Respawns the actor in place: rebuilds its slot from its spawn entry (spawn), keeping position, angles and camera. */
void actor_respawnInPlace(Actor *actor, s32 type, s32 interaction_state, f32 scale) {
    SpawnRecord *spawn;
    s32 pathIdx;
    s32 index;
    Camera *camera;
    f32 x;
    f32 y;
    f32 z;
    f32 pitch;
    f32 unk40;
    f32 unkC4;
    s32 angle;

    pathIdx = actor->listpos;
    spawn = actor->listpos_ptr;
    spawn->type = type;
    spawn->interaction_state = interaction_state;
    spawn->xz_scale = scale;
    spawn->y_scale = scale;
    spawn->unk2 = 0;
    camera = actor->CameraPtr;
    x = actor->xpos;
    y = actor->ypos;
    z = actor->zpos;
    pitch = actor->vtilt;
    unk40 = actor->theta;
    unkC4 = actor->lean_angle;
    angle = actor->angle;
    index = actor - movement_obj;
    actor_remove(actor, 0);
    collision_spawnObject(spawn, pathIdx, 0, 0, index + 1);
    actor->CameraPtr = camera;
    actor->xpos = x;
    actor->ypos = y;
    actor->zpos = z;
    actor->vtilt = pitch;
    actor->theta = unk40;
    actor->lean_angle = unkC4;
    actor->angle = angle;
    actor->langle = angle;
    actor->old_xpos = x;
    actor->old_ypos = y;
    actor->old_zpos = actor->zpos;
    actor->unk1CC = actor->ypos;
    actor->CameraPtr->mv = actor->mv;
}
