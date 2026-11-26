#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals (see init_50A0.c). */
u8 gScriptArg0;      // the argument bytes of the current command, arg0..arg3
u8 gScriptArg1;
u8 gScriptArg2;
u8 gScriptArg3;
f32 D_800D1898[2];

#include "functions.h"
#include "variables.h"

/*
 * Object script interpreter commands.
 *
 * Each object runs a byte-code script (Actor.unk218 points at its next command). A command
 * is five bytes: an opcode (< 247, an index into D_80086730) and four argument bytes, which the
 * interpreter copies to gScriptArg0..gScriptArg3 before calling the handler. Bytes >= 247 are
 * one-byte jump labels.
 * The handlers below act on the current object, gCurrentActor (index dc_glbl_CharNum in movement_obj);
 * their comments call the four argument bytes arg0..arg3. An object's "target" is the object
 * movement_obj[unk222], its current waypoint is unk21E of its path D_800D2104[unk13F].
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

    gCurrentActor->unk223 = 0;
    gCurrentActor->unk21C = gScriptArg0 * 100;
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

    index = D_800D2108[gCurrentActor->listpos] - 1;
    if (gCurrentActor->unk21F != 0) {
        index = gCurrentActor->unk21F;
    }
    range = index - gCurrentActor->unk220;
    if (gScriptArg1 == 0xFF) {
        if (gCurrentActor->unk223 != 13) {
            gCurrentActor->unk21E = (func_150ADA20_u8() % (u32)range) + gCurrentActor->unk220;
        }
    } else if (gScriptArg1 == 0xFE) {
        if (gCurrentActor->unk223 != 13) {
            previous = gCurrentActor->unk21E - 1;
            if (gCurrentActor->unk21E <= gCurrentActor->unk220) {
                previous = index - 1;
            }
            index = (func_150ADA20_u8() % (u32)range) + gCurrentActor->unk220;
            if (index != previous) {
                gCurrentActor->unk21E = index;
                // matching: the copy, the `* 0` and the jump steer IDO to duplicate this store into v1
                obj = gCurrentActor;
                obj->unk223 = gCurrentActor->unk220 * 0;
                goto set_timer;
            }
        }
    } else if (gScriptArg1 == 0xFF) {
        gCurrentActor->unk21E = index - 1;
    } else {
        gCurrentActor->unk21E = gScriptArg1;
    }
    gCurrentActor->unk223 = 0;
set_timer:
    gCurrentActor->unk21C = gScriptArg0 * 100;

    if (gScriptArg2 != 0xFF) {
        gCurrentActor->stickspeed = gScriptArg2;
        if (gCurrentActor->stickspeed == 1.0f) {
            gCurrentActor->stickspeed = 0.5f;
        }
    }
    if (gCurrentActor->unk21E < gCurrentActor->unk220) {
        gCurrentActor->unk21E = gCurrentActor->unk220;
    }
    script_applyMoveFlags();
}

/* Starts a move towards the current waypoint, arriving in arg1 ticks. */
void func_15075884(void) {
    Actor *obj;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 speed;

    script_startMove();
    obj = gCurrentActor;
    dx = D_800D2104[obj->listpos][obj->unk21E + 1].x - obj->xpos;
    dz = D_800D2104[obj->listpos][obj->unk21E + 1].z - obj->zpos;
    dist = sqrtf((dx * dx) + (dz * dz));
    dx = (f32)(u32)gScriptArg1;
    speed = dist / dx;
    obj->stickspeed = speed + speed;
}

/* Jumps to arg0 when waypoint arg1 compares (per arg2) with the waypoint unk221 steps back. */
void func_15075938(void) {
    s32 position;
    s32 desired;
    s32 matched;

    matched = 0;
    position = gCurrentActor->unk21E - (s8)gCurrentActor->unk221;
    if (position < 0) {
        position += D_800D2108[gCurrentActor->listpos] - 1;
    } else if (position >= D_800D2108[gCurrentActor->listpos] - 1) {
        position -= D_800D2108[gCurrentActor->listpos] - 1;
    }
    desired = gScriptArg1 == 0xFF ? D_800D2108[gCurrentActor->listpos] - 2 : gScriptArg1;
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
    u8 index = gCurrentActor->unk21E;

    if (((gScriptArg2 == 0) && (gScriptArg1 == index)) ||
        ((gScriptArg2 == 1) && (gScriptArg1 != index))) {
        script_jump(gScriptArg0);
    }
}

/* Starts a move, stopping at once when already within 40 units of path record arg1. */
void func_15075AAC(void) {
    PathPoint *point;
    f32 dx;
    f32 dz;

    script_startMove();
    point = D_800D2104[gCurrentActor->listpos] + gScriptArg1;
    dx = point->x - gCurrentActor->xpos;
    dz = point->z - gCurrentActor->zpos;
    if (fabsf(dx) + fabsf(dz) < 40.0f) {
        gCurrentActor->unk21C = 0;
        gCurrentActor->speed = 0.0f;
    }
}

void func_15075B60(void) {
    script_startMove();
    gCurrentActor->unk223 = 10;
}

void func_15075B8C(void) {
    script_pickWaypoint();
    gCurrentActor->unk223 = 10;
}

void func_15075BB8(void) {
    script_startMove();
    gCurrentActor->unk22C |= 2;
}

void func_15075BE8(void) {
    if (gScriptArg3 != 0) {
        gCurrentActor->unk22C |= 0x80;
    }
    gCurrentActor->unk233 = gScriptArg0;
}

/* Sets the vertical velocity to the s16 (arg2:arg0) unless arg3 == 1, and gravity to (s8)arg1 unless arg3 == 2. */
void func_15075C24(void) {
    if (gScriptArg3 != 1) {
        gCurrentActor->yspd = (s16)((gScriptArg2 << 8) | gScriptArg0);
    }
    if (gScriptArg3 != 2) {
        gCurrentActor->ygrav = (s8)gScriptArg1;
    }
}

void func_15075CA0(void) {
    script_startMove();
    gCurrentActor->unk223 = 1;
}

void func_15075CCC(void) {
    script_startMove();
    gCurrentActor->unk223 = 16;
    gCurrentActor->unk231 = gScriptArg1;
}

void func_15075D0C(void) {
    script_startMove();
    gCurrentActor->unk223 = 15;
}

void func_15075D38(void) {
    func_15075CA0();
    gCurrentActor->unk223 = 14;
}

void func_15075D64(void) {
    script_startMove();
    gCurrentActor->unk223 = 12;
    gCurrentActor->unk222 = 0;
}

void func_15075D9C(void) {
    script_startMove();
    gCurrentActor->unk223 = 9;
}

void func_15075DC8(void) {
    func_15075CA0();
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

void func_15075E6C(void) {
    script_startMove();
    gCurrentActor->unk223 = 2;
}

void func_15075E98(void) {
    gCurrentActor->unk235 = gScriptArg0;
}

/* Jumps to arg0 with a chance of arg2 percent. */
void script_jumpRandom(void) {
    if ((func_150ADA20_u8() % 100U) < gScriptArg2) {
        script_jump(gScriptArg0);
    }
}

void func_15075F00(void) {
    gCurrentActor->unk234 = gScriptArg0;
    gCurrentActor->unk236 = gScriptArg1;
    gCurrentActor->unk237 = gScriptArg2;
}

void func_15075F40(void) {
    script_startMove();
    gCurrentActor->unk223 = 3;
}

/* Sets the vertical velocity (arg0, downwards if arg2), the gravity (arg1) and unk3A (arg3). */
void func_15075F6C(void) {
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
    gCurrentActor->unk3A = gScriptArg3;
}

/* Starts a jump towards waypoint arg1, timing the flight so that it lands there. */
void script_leapToWaypoint(void) {
    f32 dx;
    f32 dz;
    f32 time;

    script_startMove();
    gCurrentActor->unk21E = gScriptArg1;
    dx = D_800D2104[gCurrentActor->listpos][gScriptArg1 + 1].x - gCurrentActor->xpos;
    dz = D_800D2104[gCurrentActor->listpos][gScriptArg1 + 1].z - gCurrentActor->zpos;
    time = 2.0f * (sqrtf((dx * dx) + (dz * dz)) / gCurrentActor->stickspeed);
    if (time < 12.0f) {
        gCurrentActor->stickspeed *= time * 0.08333300054f;
        time = 12.0f;
    }
    gCurrentActor->unk21C = (u32)(time * 100.0f);
    gCurrentActor->yspd = gCurrentActor->ygrav * time * 0.5f;
    gCurrentActor->unk223 = 4;
}

void func_150761C8(void) {
    script_pickWaypoint();
    gCurrentActor->unk223 = 5;
}

void func_150761F4(void) {
    gCurrentActor->unk223 = 6;
    gCurrentActor->unk22C &= ~1;
}

void func_15076220(void) {
    func_150761F4();
    gCurrentActor->unk22C |= 1;
}

void func_15076250(void) {
    s32 id = (gScriptArg0 << 8) | gScriptArg1;

    actor_playPlayerSound(id, gCurrentActor, 32000, -100, 500, 2500, gScriptArg2);
}

void func_150762B0(void) {
    func_1000CBA8(gScriptArg0);
}

void func_150762D4(void) {
    func_151669A0(gCurrentActor->xpos, gCurrentActor->ypos + 100.0f,
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

void func_15076394(void) {
    gCurrentActor->unk236 = gScriptArg0;
}

void func_150763B0(void) {
    if (gScriptArg0 == 128) {
        gCurrentActor->unk1E5 = gCurrentActor->listpos_ptr->unkF;
    } else {
        gCurrentActor->unk1E5 = gScriptArg0;
    }
    gCurrentActor->unk1E7 = 0;

    if (gScriptArg2 != 0) {
        gCurrentActor->unk1E5 += func_150ADA20_u8() % (u32)gScriptArg2;
    }
    if (gScriptArg3 == 1) {
        gCurrentActor->unk1E6 = 0;
        gCurrentActor->unk1E8 = 0;
        return;
    }
    if (gScriptArg3 == 2) {
        gCurrentActor->unk1E6 = gScriptArg1;
        gCurrentActor->unk1E8 = 0;
        return;
    }
    if (gScriptArg1 == 0) {
        gCurrentActor->unk1E6 = gCurrentActor->unk1E5;
        gCurrentActor->unk1E8 = 0;
    }
}

void func_150764C8(void) {
    gCurrentActor->unk238 = gScriptArg0;
}

void script_setHitRoute(void) {
    gCurrentActor->intelhitroute = gScriptArg0;
}

void func_15076500(void) {
    gCurrentActor->unk22E = (gScriptArg0 << 8) | gScriptArg1;
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

void func_15076600(void) {
}

void func_15076608(void) {
    gCurrentActor->unk24A = gScriptArg0;
}

/* Sets the flags given by the four argument bytes in unkF8. */
void func_15076624(void) {
    s32 flags = (gScriptArg0 << 24) | (gScriptArg1 << 16) | (gScriptArg2 << 8) | gScriptArg3;

    gCurrentActor->noshove |= flags;
}

/* Clears the flags given by the four argument bytes in unkF8. */
void func_15076678(void) {
    s32 flags = (gScriptArg0 << 24) | (gScriptArg1 << 16) | (gScriptArg2 << 8) | gScriptArg3;

    gCurrentActor->noshove &= ~flags;
}

void func_150766D0(void) {
    if (((gCurrentActor->shad > 0.0f) && (gScriptArg0 == 0)) ||
        ((gScriptArg0 * 50 < gCurrentActor->shad) && (gScriptArg0 != 0))) {
        gCurrentActor->unk21C = 99;
        gCurrentActor->scriptCmd--;
    }
}

void func_15076760(void) {
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
        func_1504715C(&floor, gCurrentActor);
        func_1514B364(&position, &floor, 0xFF, 0);
        break;
    }
}

/* Jumps to arg0 when the direction from the object to its target is within arg1 of the target's facing. */
void func_150767F4(void) {
    Actor *target;
    u16 angle;

    target = &movement_obj[gCurrentActor->unk222];
    // matching: the (s32) cast before narrowing to u16 is needed
    angle = (s32)actor_getDirectionAngle(target->xpos - gCurrentActor->xpos,
                               gCurrentActor->zpos - target->zpos, 0);
    if ((((angle >> 8) - (movement_obj[gCurrentActor->unk222].langle >> 8) + gScriptArg1) & 0xFF) < gScriptArg1 * 2) {
        script_jump(gScriptArg0);
    }
}

/*
 * Picks the nearest object (flag 0x20 in unkF8) within arg0 * 8 units that lies inside the object's
 * view cone, storing its index | 0x80 in unkA8. If one is found, the script jumps to label arg3 and,
 * for a nonzero arg2, the found object starts script arg2 targeting this object.
 */
void func_150768DC(void) {
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
                    type = D_800D1C90[gCurrentActor->character_type];
                    viewAngle = type->viewAngle;
                    angle = actor_getDirectionAngle(obj->xpos - gCurrentActor->xpos,
                                          gCurrentActor->zpos - obj->zpos, 0) >> 8;
                    if (((((gCurrentActor->angle >> 8) + gCurrentActor->unk2CA) - angle + (viewAngle / 2)) & 0xFF) < viewAngle) {
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
            found->unk222 = self;
        }
    }
}

void func_15076B5C(void) {
    gCurrentActor->unk222 = gCurrentActor->unkA8 & 0x7F;
}

void func_15076B78(void) {
    gCurrentActor->drawtype = gScriptArg0;
}

/* Targets the last object below this one (all 25 if arg1) of type arg0 that is alive, and starts mode 9. */
void func_15076B94(void) {
    s32 i = gCurrentActor - movement_obj;

    if (gScriptArg1) {
        i = 25;
    }
    while (i != 0) {
        if ((gScriptArg0 == movement_obj[i - 1].character_type) && (movement_obj[i - 1].energy != 0)) {
            script_startMove();
            gCurrentActor->unk223 = 9;
            gCurrentActor->unk222 = i - 1;
            gCurrentActor->unk21C = 1000;
            return;
        }
        i--;
    }
}

void func_15076C7C(void) {
    actor_fadeCommand(gCurrentActor, 1, gScriptArg0, 0, 0);
}

void func_15076CB4(void) {
    gCurrentActor->angle = gCurrentActor->fangle;
}

void func_15076CCC(void) {
    gCurrentActor->unk21C = 10000;
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
    gCurrentActor->unk154 = gCurrentActor->scale_xz;
    gCurrentActor->unk158 = gCurrentActor->scale_y;
    actor_setScale(gCurrentActor, gCurrentActor->scale_xz, gCurrentActor->scale_y);
}

void script_setType(void) {
    gCurrentActor->type = gScriptArg0;
}

void func_15076E10(void) {
    gCurrentActor->unk1D0 = gScriptArg0;
    gCurrentActor->unk22C |= gScriptArg1;
}

void func_15076E48(void) {
}

/* Starts mode 7: a jump in a random direction with random speeds. */
void func_15076E50(void) {
    script_startMove();
    gCurrentActor->yspd = (func_150ADA20_u8() % 20U) + 55.0f;
    gCurrentActor->ygrav = 2.0f;
    gCurrentActor->fangle = func_150ADA20_u8() % 0xFFFFU;
    gCurrentActor->speed = (func_150ADA20_u8() % 20U) + 15;
    gCurrentActor->stickspeed = gCurrentActor->speed;
    gCurrentActor->unk223 = 7;
}

void func_15076F40(void) {
    gCurrentActor->unk21C = 1000;
    actor_remove(gCurrentActor, 1);
}

void func_15076F78(void) {
    s8 value;

    value = gScriptArg0;
    gCurrentActor->unkCC = value;
    value = gScriptArg1;
    gCurrentActor->unkCE = value;
}

/* arg3 == 0: moves the object by a random offset of up to +-arg0/2 (x) and +-arg1/2 (z), times 3;
 * otherwise raises its speed to at least arg3. */
void func_15076FA8(void) {
    if (gScriptArg3 == 0) {
        s16 offset;

        offset = (func_150ADA20_u8() % (u32)gScriptArg0) - (gScriptArg0 / 2);
        gCurrentActor->xpos += offset * 3.0f;
        offset = (func_150ADA20_u8() % (u32)gScriptArg1) - (gScriptArg1 / 2);
        gCurrentActor->zpos += offset * 3.0f;
    } else if (gCurrentActor->speed < gScriptArg3) {
        gCurrentActor->speed = gScriptArg3;
    }
}

/* Jumps to arg3 when the target's speed is below arg2. */
void script_jumpIfTargetSlow(void) {
    if (movement_obj[gCurrentActor->unk222].speed < gScriptArg2) {
        script_jump(gScriptArg3);
    }
}

void func_15077174(void) {
    gCurrentActor->unk10E = gScriptArg0;
}

void func_15077190(void) {
    s32 id = (gScriptArg0 << 8) | gScriptArg1;

    if (id != 0) {
        s32 param = gScriptArg2 << 7;

        sound_playActorUntracked(id, gCurrentActor, param, 500, 2500);
    }
}

void func_150771F0(void) {
    u8 mode;
    u8 param0;
    u8 param1;

    if (gScriptArg3 == 0) {
        param0 = gScriptArg0;
        param1 = gScriptArg1;
        if (gScriptArg2 != 0) {
            mode = 1;
        } else {
            mode = 2;
        }
        actor_fadeCommand(gCurrentActor, mode, param0, param1, 0);
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

void func_150772E8(void) {
    model_breakIntoParts(gCurrentActor, gScriptArg0, -1);
}

void func_15077318(void) {
    if (gScriptArg0 != 0) {
        gCurrentActor->unk101 |= 1;
    } else {
        gCurrentActor->unk101 &= ~1;
    }
}

void func_1507735C(void) {
}

void func_15077364(void) {
    if (gScriptArg3 != 0) {
        gScriptArg0 += func_150ADA20_u8() % (u32)gScriptArg3;
    }
    gCurrentActor->intel_animspd = gScriptArg0;
    gCurrentActor->unk249 = 0;
    gCurrentActor->intel_accel = gScriptArg1;
    gCurrentActor->intel_decel = gScriptArg2;
}

/*
 * arg3 != 0: adds (s16)(arg1:arg2) * D_800BE9E4 to the 13-bit counter kept in unk246 (high bits) and
 * unk249 (low byte), clamping at 0 and setting bit 7 of unk246; otherwise sets unk246 to arg0.
 */
void func_15077404(void) {
    if (gScriptArg3 != 0) {
        s16 value;
        s16 delta;

        value = ((gCurrentActor->intel_animspd & 0x1F) << 8) + gCurrentActor->unk249;
        delta = (gScriptArg1 << 8) + gScriptArg2;
        value += delta * D_800BE9E4;
        if (value < 0) {
            value = 0;
        }
        gCurrentActor->intel_animspd = (value >> 8) | 0x80;
        gCurrentActor->unk249 = value;
    } else {
        gCurrentActor->intel_animspd = gScriptArg0;
    }
}

/* Jumps to arg0 when the object has reached its destination node (or has none). */
void script_jumpIfAtDestination(void) {
    if ((gCurrentActor->unk20F == gCurrentActor->unk211) || (gCurrentActor->unk211 == 0xFF)) {
        gCurrentActor->task_attrib |= 0x800;
        script_jump(gScriptArg0);
    }
}

/* the s16 at 0x216 overlaps Actor's s32 unk214 */
#define UNK216(p) (*(s16 *)((u8 *)(p) + 0x216))

/*
 * Steps the object along the path-node graph: picks the next node (arg0 == 1: a random neighbour
 * other than the previous node, otherwise the next node towards the goal unk211) and stores a point
 * near it, offset sideways by up to twice the node's unk8, as waypoint 0 of the object's path.
 * Sets flag 0x800 in unk25C when the goal is reached or no node is left.
 */
void func_15077508(void) {
    s32 next;
    f32 x;
    f32 y;
    f32 z;
    s8 hasGoal;
    s8 hasGoal2;
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
        if (gCurrentActor->unk20F == gCurrentActor->unk211) {
            UNK216(gCurrentActor) = 0;
            gCurrentActor->task_attrib |= 0x800;
            return;
        }
        if (gCurrentActor->unk211 == 0xFF) {
            UNK216(gCurrentActor) = 0;
        }
    }
    gCurrentActor->task_attrib &= ~0x800;
    if (D_800D2108[gCurrentActor->listpos] - 1 > 0) {
        hasGoal2 = 0;
        hasGoal = 0;
        if (gCurrentActor->unk211 != 0xFF) {
            hasGoal = 1;
        }
        if (gCurrentActor->unk212[0] != 0xFF) {
            hasGoal2 = 1;
        }
        current = gCurrentActor->unk20F;
        if (current == 0xFF) {
            if (gScriptArg0 == 1) {
                hasGoal2 = 0;
                hasGoal = 0;
                next = gCurrentActor->unk210;
            }
            if (next == 0xFF) {
                next = func_15086098(gCurrentActor->xpos, gCurrentActor->ypos, gCurrentActor->zpos,
                                     hasGoal, hasGoal2, &gCurrentActor->unk211, gCurrentActor->unk212);
            }
        } else if (current != 0xFF) {
            if (gScriptArg0 == 1) {
                best = 0xFF;
                node = &D_800D2350[current];
                if (gCurrentActor->unk210 != 0xFF) {
                    // matching: dead code standing in for something the original compiled away; it
                    // keeps IDO's branch on pad210 and its register assignment
                    tmp = 0; for (i = 0, q = (u8 *)D_800D2350; i < 4; i++, q++) { tmp += q[9] == gCurrentActor->unk210; } tmp += (s32)node; tmp ^= (s32)node; tmp -= (s32)node; tmp |= (s32)node;
                }
                bestRoll = 10.0f;
                for (i = 0; i < 5; i++) {
                    if (node->links[i] != 0xFF && gCurrentActor->unk210 != node->links[i]) {
                        roll = func_150ADA68();
                        if (roll < bestRoll) {
                            bestRoll = roll;
                            best = node->links[i];
                        }
                    }
                }
                next = best;
                if (next == 0xFF) {
                    gCurrentActor->task_attrib |= 0x800;
                    next = gCurrentActor->unk210;
                }
            } else {
                next = func_15086364(current, hasGoal, hasGoal2, &gCurrentActor->unk211, gCurrentActor->unk212);
            }
        }
        if (next != 0xFF) {
            gCurrentActor->unk21E = 0;
            if (func_15086CBC(next, &x, &y, &z)) {
                UNK216(gCurrentActor) = D_800D2354;
                gCurrentActor->unk210 = gCurrentActor->unk20F;
                gCurrentActor->unk20F = next;
                {
                    f32 width;
                    f32 dx;
                    f32 dz;
                    f32 dist;

                    width = (f32)(u32)D_800D2350[next].unk8 + (f32)(u32)D_800D2350[next].unk8;
                    dx = x - gCurrentActor->xpos;
                    dz = z - gCurrentActor->zpos;
                    dist = sqrtf(dx * dx + dz * dz);
                    if (dist > 1.0f) {
                        dist = func_150ADA68() * width / dist;
                        x -= dz * dist;
                        z += dx * dist;
                    }
                }
                D_800D2104[gCurrentActor->listpos][1].x = x;
                D_800D2104[gCurrentActor->listpos][1].y = y;
                D_800D2104[gCurrentActor->listpos][1].z = z;
            }
        }
    }
}

/* Advances the waypoint index by unk221, wrapping around the path (or the first unk21F waypoints). */
void script_advanceWaypoint(void) {
    u8 count;

    count = D_800D2108[gCurrentActor->listpos] - 1;
    if (gCurrentActor->unk21F != 0) {
        count = gCurrentActor->unk21F;
    }
    gCurrentActor->unk21E += gCurrentActor->unk221;
    gCurrentActor->unk21E += count;
    gCurrentActor->unk21E %= count;
    if (gCurrentActor->unk21E < gCurrentActor->unk220) {
        gCurrentActor->unk21E = gCurrentActor->unk220;
    }
}

void func_150779A8(void) {
    script_pickWaypoint();
    gCurrentActor->unk223 = 11;
}

/* Jumps to arg0 when object 0 (or, with arg2, the target) is within arg3 * 8 units. */
void func_150779D4(void) {
    Actor *obj;
    u8 index;
    f32 range;

    index = 0;
    if (gScriptArg2 != 0) {
        index = gCurrentActor->unk222;
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

void func_15077AA0(void) {
    gCurrentActor->intelhitroute = gScriptArg0;
}

void func_15077ABC(void) {
    gCurrentActor->unk258 = gScriptArg0;
    gCurrentActor->unk257 = gScriptArg1;
    gCurrentActor->unk86 = func_150ADA20_u8() % 255U;
}

void func_15077B14(void) {
    gCurrentActor->unk24C = gScriptArg0;
    gCurrentActor->unk24D = gScriptArg1;
}

void func_15077B44(void) {
    actor_playSound((gScriptArg0 << 8) + gScriptArg1, gCurrentActor);
}

void func_15077B80(void) {
    s32 flags = (gScriptArg0 << 8) + gScriptArg1;

    gCurrentActor->task_attrib |= flags;
}

/* Restarts the object returned by actor_findByType(arg1) on script arg0. */
void func_15077BB4(void) {
    Actor *obj = actor_findByType(gScriptArg1);

    obj->scriptCmd = NULL;
    obj->intelwalk = gScriptArg0;
}

void func_15077BE4(void) {
    s32 mask = ~((gScriptArg0 << 8) + gScriptArg1);

    gCurrentActor->task_attrib &= mask;
}

void func_15077C1C(void) {
    gCurrentActor->unk23D = gScriptArg0;
}

/*
 * Targets an object that is alive, not immune (or 0xFF), unlinked (unless arg2) and either of
 * type arg0 or in state arg1 and not stunned: the first one found if arg3 == 0, else the nearest.
 */
void func_15077C38(void) {
    Actor *obj;
    f32 nearest;
    f32 dist;
    s32 i;

    nearest = 9999999.0f;
    gCurrentActor->unk222 = 0;

    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if (((obj->type != 0) && (obj->energy != 0)) &&
            ((obj->imune == 0) || (obj->imune == 0xFF)) &&
            ((obj->attach_char == 0) || (gScriptArg2 != 0)) &&
            ((obj->character_type == gScriptArg0) || ((obj->type == gScriptArg1) && (obj->stunned == 0)))) {
            if (gScriptArg3 == 0) {
                gCurrentActor->unk222 = i;
                return;
            }
            dist = actor_distance3D(gCurrentActor, obj);
            if (nearest > dist) {
                gCurrentActor->unk222 = i;
                nearest = dist;
            }
        }
    }
}

void func_15077DA0(void) {
    gCurrentActor->unk21E = gScriptArg0;
}

/* Moves the object onto waypoint arg0 (0xFA: the current waypoint). */
void script_warpToWaypoint(void) {
    if (gScriptArg0 != 0xFA) {
        gCurrentActor->unk21E = gScriptArg0;
    }
    gCurrentActor->xpos = D_800D2104[gCurrentActor->listpos][gCurrentActor->unk21E + 1].x;
    gCurrentActor->ypos = D_800D2104[gCurrentActor->listpos][gCurrentActor->unk21E + 1].y;
    gCurrentActor->zpos = D_800D2104[gCurrentActor->listpos][gCurrentActor->unk21E + 1].z;
}

void func_15077E9C(void) {
    s32 id = (gScriptArg0 << 8) + gScriptArg1;

    mp3_startTrackAt(id, gCurrentActor, 24000, 500, 2500);
}

void func_15077EEC(void) {
    gCurrentActor->unk254 = gScriptArg0;
}

void func_15077F08(void) {
    gCurrentActor->unk1EA = (gScriptArg0 << 8) | gScriptArg1;
}

void func_15077F34(void) {
    gCurrentActor->unk24F = gScriptArg0;
    gCurrentActor->unk250 = gScriptArg1;
}

/* Turns the object towards the current waypoint, starting turn animation arg0 - 1 for large turns. */
void script_turnToWaypoint(void) {
    f32 dx;
    f32 dz;
    u8 turn;
    u8 index;

    index = gScriptArg0 - 1;
    dx = D_800D2104[gCurrentActor->listpos][gCurrentActor->unk21E + 1].x - gCurrentActor->xpos;
    dz = gCurrentActor->zpos - D_800D2104[gCurrentActor->listpos][gCurrentActor->unk21E + 1].z;
    gCurrentActor->fangle = actor_getDirectionAngle(dx, dz, 0);
    turn = (gCurrentActor->fangle - gCurrentActor->angle) >> 8;
    if (turn & 0x80) {
        turn = -turn;
    }
    if (gTurnAnimTable[index][0] < turn) {
        gCurrentActor->unk250 = gScriptArg1;
        func_15056A00(gCurrentActor, turn, index);
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
    s32 index = gCurrentActor->unk222;

    if (movement_obj[index].type == 0) {
        script_jump(gScriptArg0);
    }
}

void func_150781A4(void) {
    gCurrentActor->unk23F = gScriptArg1;
    gCurrentActor->unk240 = gScriptArg2;
    gCurrentActor->unk241 = gScriptArg0;
    gCurrentActor->unk242 = gScriptArg3;
}

/* Jumps to arg0 when the object is closer (arg2 == 0) or farther (arg2 != 0) than arg3 * 8 from waypoint arg1. */
void script_jumpIfNearWaypoint(void) {
    PathPoint *point;
    f32 dx;
    f32 dz;
    f32 dist;
    s32 index;

    index = gScriptArg1 + 1;
    point = &D_800D2104[gCurrentActor->listpos][index];
    dx = point->x - gCurrentActor->xpos;
    dz = point->z - gCurrentActor->zpos;
    dist = sqrtf((dx * dx) + (dz * dz));
    dx = gScriptArg3 << 3;
    if (((gScriptArg2 == 0) && (dist < dx)) ||
        ((gScriptArg2 != 0) && (dist > dx))) {
        script_jump(gScriptArg0);
    }
}

void func_150782CC(void) {
    gCurrentActor->unk23E = gScriptArg0;
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
    s32 index = func_15083FB0(gScriptArg0);

    gCurrentActor->unk222 = index;
    if (index == -1) {
        gCurrentActor->unk222 = 0;
    }
}

/*
 * Jumps to arg0 when the distance to the target (arg2: 0 actor_distanceXZ, 1 actor_distance3D, else the
 * height difference) is below (arg1 == 0) or above (arg1 == 1) arg3 * 8 (0xFF: unk23D * 8).
 */
void script_jumpIfTargetDistance(void) {
    Actor *target;
    f32 dist;
    f32 threshold;

    if (gScriptArg2 == 0) {
        target = &movement_obj[gCurrentActor->unk222];
        dist = actor_distanceXZ(gCurrentActor, target);
    } else if (gScriptArg2 == 1) {
        target = &movement_obj[gCurrentActor->unk222];
        dist = actor_distance3D(gCurrentActor, target);
    } else {
        dist = fabsf(gCurrentActor->ypos - movement_obj[gCurrentActor->unk222].ypos);
    }

    threshold = gScriptArg3 * 8;
    if (gScriptArg3 == 0xFF) {
        threshold = gCurrentActor->unk23D * 8;
    }
    if (((gScriptArg1 == 0) && (dist < threshold)) ||
        ((gScriptArg1 == 1) && (threshold < dist))) {
        script_jump(gScriptArg0);
    }
}

void func_15078520(void) {
    script_jump(gScriptArg0);
}


/*
 * Places waypoint (arg0 & 0x3F) at a 12-bit signed offset (arg1:arg3 high nibble, arg2:arg3 low
 * nibble, times 8) from the target, rotated by the target's facing (mode arg0 >> 6 == 0), not at all
 * (mode 1) or by the direction from the target to this object (mode 2).
 */
void func_15078544(void) {
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
    x = movement_obj[gCurrentActor->unk222].xpos;
    z = movement_obj[gCurrentActor->unk222].zpos;
    y = movement_obj[gCurrentActor->unk222].ypos;
    offsetX = ((((s8)gScriptArg1 << 4) | (gScriptArg3 >> 4)) * 8);
    offsetZ = ((((s8)gScriptArg2 << 4) | (gScriptArg3 & 0xF)) * 8);
    if (mode == 0) {
        facing = ((movement_obj[gCurrentActor->unk222].langle >> 8) & 0xFF) - 0x40;
        sine = func_15048A40(facing);
        cosine = func_150489B0(facing);
    } else if (mode == 1) {
        sine = 0.0f;
        cosine = 1.0f;
    } else {
        angle = func_150484A0(gCurrentActor->xpos - x, gCurrentActor->zpos - z);
        sine = sinf(angle);
        cosine = cosf(angle);
    }
    x += offsetX * cosine + offsetZ * sine;
    z += offsetZ * cosine - offsetX * sine;
    D_800D2104[gCurrentActor->listpos][gScriptArg0 + 1].x = x;
    D_800D2104[gCurrentActor->listpos][gScriptArg0 + 1].y = y;
    D_800D2104[gCurrentActor->listpos][gScriptArg0 + 1].z = z;
}

/* Jumps to arg0 when the target's unk2D0->unk8 is below (arg2 == 0) or above (arg2 == 1) arg1. */
void func_1507879C(void) {
    AnimInstance *data;
    f32 value;
    s32 compare;

    data = movement_obj[gCurrentActor->unk222].unk2D0;
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
        if (gCurrentActor->unk222 == 0) {
            script_jump(gScriptArg0);
        }
    } else if (((gScriptArg2 == 0) && (gScriptArg1 == movement_obj[gCurrentActor->unk222].unk251)) ||
               ((gScriptArg2 == 1) && (gScriptArg1 != movement_obj[gCurrentActor->unk222].unk251))) {
        script_jump(gScriptArg0);
    }
}

/* Copies unk8 and unkC of the target's unk2D0 data into the object's own. */
void func_15078A08(void) {
    AnimInstance *dst;
    AnimInstance *src;

    dst = gCurrentActor->unk2D0;
    src = movement_obj[gCurrentActor->unk222].unk2D0;
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
        diff = (u8)(func_150484A0(movement_obj[index].xpos - gCurrentActor->xpos,
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
        value = movement_obj[gCurrentActor->unk222].character_type;
        break;
    case 16:
        value = movement_obj[index].energy;
        break;
    case 17:
        value = movement_obj[gCurrentActor->unk222].intelwalk;
        break;
    case 18:
        value = movement_obj[index].norun;
        break;
    }
    // matching: one line, as -g3 line numbers steer the scheduling of this test
    if (((gScriptArg1 == 0) && (value == gScriptArg3)) || ((gScriptArg1 == 1) && (value != gScriptArg3)) || ((gScriptArg1 == 2) && (value < gScriptArg3)) || ((gScriptArg1 == 3) && (value > gScriptArg3)) || ((gScriptArg1 == 4) && (value <= gScriptArg3)) || ((gScriptArg1 == 5) && (value >= gScriptArg3))) { return 1; } return 0;
}

void func_1507900C(void) {
    if (script_testProperty(0) != 0) {
        script_jump(gScriptArg0);
    }
}

void func_1507903C(void) {
    if (script_testProperty(0) != 0) {
        gCurrentActor->scriptCmd = script_getScript(0, gScriptArg0);
        gCurrentActor->scriptCmd--;
    }
}

void func_15079090(void) {
    if (script_testProperty(dc_glbl_CharNum) != 0) {
        script_jump(gScriptArg0);
    }
}

void func_150790C4(void) {
    if (script_testProperty(dc_glbl_CharNum) != 0) {
        gCurrentActor->scriptCmd = script_getScript(0, gScriptArg0);
        gCurrentActor->scriptCmd--;
    }
}

/* Jumps to arg0 when the y (arg3 & 2: x) of object 0 (arg3 & 1: this object) is below (s16)(arg1:arg2). */
void func_1507911C(void) {
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

void func_150791F0(void) {
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
    gCurrentActor->unk21C = 1;
}

void func_150792E0(void) {
    gCurrentActor->intelwalk = gScriptArg0;
}

void func_150792FC(void) {
    gCurrentActor->unk2F8 &= ~7;
    gCurrentActor->unk2F8 |= gScriptArg0;
}

/* Restarts the object found by func_150768DC on script arg0. */
void func_15079334(void) {
    Actor *found;

    if (gCurrentActor->unkA8 != 0) {
        found = &movement_obj[gCurrentActor->unkA8 & 0x7F];
        found->scriptCmd = NULL;
        found->intelwalk = gScriptArg0;
    }
}

void func_15079390(void) {
    u16 param0 = gScriptArg0;
    s32 param1 = gScriptArg1;
    s32 param2 = gScriptArg2;

    func_1514D3B0(gCurrentActor, param0, param1, param2);
}

/* Places waypoint arg0 at a random point up to 550 units from object 0, clamped to +-1200. */
void func_150793D8(void) {
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

    actor_splitVelocity(func_150ADA20_u8(), 550.0f, 0.0f, &offsetX, &offsetZ, &third);

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

    D_800D2104[gCurrentActor->listpos][gScriptArg0 + 1].x = x;
    D_800D2104[gCurrentActor->listpos][gScriptArg0 + 1].z = z;
}

/* Sets the speeds for a jump that lands on the target. */
void script_leapToTarget(void) {
    f32 dist;
    f32 time;

    dist = actor_distanceXZ(gCurrentActor, &movement_obj[gCurrentActor->unk222]);
    gCurrentActor->stickspeed = gCurrentActor->speed;
    time = 2.0f * (dist / gCurrentActor->stickspeed);
    gCurrentActor->yspd = (gCurrentActor->ygrav * time) * 0.5f;
    gCurrentActor->yspd += 2.0f * ((movement_obj[gCurrentActor->unk222].ypos - gCurrentActor->ypos) / time);
}

/* Restarts the object with unique id arg0 on script arg1. */
void func_1507965C(void) {
    s32 index;
    Actor *obj;

    index = func_15083FB0(gScriptArg0);
    if (index != -1) {
        obj = &movement_obj[index];
        obj->intelwalk = gScriptArg1;
        obj->scriptCmd = NULL;
        obj->unk23A = 0;
    }
}

/* arg2: sets `immune` of the object with unique id arg0 to arg1; else jumps to arg1 when that object is stunned. */
void func_150796CC(void) {
    s32 index = func_15083FB0(gScriptArg0);

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
        offsetX = (func_150ADA20_u8() % 500U) - 250;
        offsetZ = (func_150ADA20_u8() % 500U) - 250;
        gCurrentActor->xpos = D_800D2104[gCurrentActor->listpos][0].x + offsetX;
        gCurrentActor->zpos = D_800D2104[gCurrentActor->listpos][0].z + offsetZ;
    }
}

/* Starts a move with object 0's speed plus (s8)arg2, clamped to [arg1, 250]. */
void func_15079880(void) {
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
    gScriptArg1 = gCurrentActor->unk21E;
    script_jumpIfNearWaypoint();
}

void func_15079928(void) {
    u16 timer;

    timer = (gScriptArg1 << 16) + gScriptArg2;
    if (timer == 0) {
        timer = 0xFFFF;
    }
    func_1507E7E4(gCurrentActor, gScriptArg0, gScriptArg3, timer, 0);
}

void func_15079988(void) {
    func_1507EB4C(gCurrentActor, gScriptArg0);
}

void func_150799B4(void) {
    s32 id = (gScriptArg0 << 8) + gScriptArg1;

    if (gScriptArg2 != 0) {
        id += func_150ADA20_u8() % (u32)gScriptArg2;
    }
    actor_playSound(id, gCurrentActor);
}

void func_15079A28(void) {
    gCurrentActor->unk252 = gScriptArg0;
    gCurrentActor->unk253 = gScriptArg1;
}

void func_15079A58(void) {
    D_800D2110[gCurrentActor->listpos] = (gScriptArg0 << 8) + gScriptArg1;
}

/* Returns the direction from object `arg0` to the centre of the current object's path. */
u16 func_15079A98(s32 arg0) {
    PathPoint *path;
    Actor *obj;

    path = D_800D2104[gCurrentActor->listpos];
    obj = &movement_obj[arg0];
    return actor_getDirectionAngle(path[0].x - obj->xpos, obj->zpos - path[0].z, 0);
}

/*
 * Changes a property of an object: arg1 selects the object (0, 1: the current one, else unique id
 * arg1 - 2), arg2 the property and, for counters 1-3, arg3 the operation applied with value arg0.
 */
void func_15079B30(void) {
    s32 index;
    s32 *counter;
    Actor *savedObj;
    s32 savedIndex;
    u16 angle;
    s32 select;

    select = gScriptArg1;
    if (select == 0) {
        // matching: the jump into the switch keeps arg1 == 0 out of the jump table
        goto current;
    }
select_object: // matching: unused label, splits the block for the scheduler
    switch (gScriptArg1) {
    case 1:
    current:
        index = dc_glbl_CharNum;
        break;
    default:
        index = func_15083FB0(gScriptArg1 - 2);
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
        // matching: the block-scope redeclaration changes how IDO schedules the address of gCurrentActor
        extern Actor *gCurrentActor;

        // restart the object on script arg0, running as the current object meanwhile
        savedObj = gCurrentActor;
        savedIndex = dc_glbl_CharNum;
        gCurrentActor = &movement_obj[index];
        dc_glbl_CharNum = index;
        if (gCurrentActor->intelwalk != gScriptArg0) {
            gCurrentActor->intelwalk = gScriptArg0;
            gCurrentActor->scriptCmd = script_getScript(0, gCurrentActor->intelwalk);
            gCurrentActor->unk21C = 0;
        }
        dc_glbl_CharNum = savedIndex;
        gCurrentActor = savedObj;
        return;
    }
    case 6:
        movement_obj[index].unkC8 = gScriptArg0;
        return;
    case 7:
        movement_obj[index].vtilt = 0.0f;
        return;
    case 8:
        angle = func_15079A98(gCurrentActor->unk222);
        if ((func_15079A98(dc_glbl_CharNum) - angle) & 0x8000) {
            gCurrentActor->unk221 = gScriptArg0;
        }
        return;
    case 9: {
        u16 value = (gScriptArg3 << 8) | gScriptArg0;

        gCurrentActor->intel_animseq = value;
        return;
    }
    case 10:
        gCurrentActor->unk236 = gCurrentActor->unk23D + (s8)gScriptArg0;
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
        *counter += D_800BE9E4;
        break;
    case 3:
        (*counter)++;
        break;
    case 4:
        *counter -= D_800BE9E4;
        break;
    case 5:
        *counter -= D_800CC264;
        break;
    case 6:
        *counter += D_800CC264;
        break;
    }
}

void func_15079F24(void) {
    gCurrentActor->speedup = D_800D1898[gScriptArg0];
}

void func_15079F50(void) {
    gCurrentActor->unk23B = gScriptArg0;
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
void func_15079FBC(void) {
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
    func_15075F40();
    gCurrentActor->mvmt_control |= 0x40;
}

/* Sets coordinate arg1 (0 x, 1 y, 2 z) of waypoint arg0 to (s16)(arg2:arg3). */
void script_setWaypointCoord(void) {
    s32 value = ((s8)gScriptArg2 << 8) | gScriptArg3;

    ((s16 *)&(D_800D2104[gCurrentActor->listpos] + 1)[gScriptArg0])[gScriptArg1] = value; // matching: waypoints start one point past the centre
}

/* Sets the speed to object 0's speed plus (s8)arg2, clamped to [arg1, arg0]. */
void func_1507A164(void) {
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

void func_1507A210(void) {
    gCurrentActor->unk223 = 0;
    gCurrentActor->unk21C = gScriptArg0 * 100;
    gCurrentActor->unk22C &= 0xFD;
    script_applyMoveFlags();
}

void func_1507A270(void) {
    model_breakIntoParts(gCurrentActor, gScriptArg0, (s8)gScriptArg1);
}

void func_1507A2A4(void) {
    gScriptArg3 = (func_150ADA20_u8() % (u32)gScriptArg1) + gScriptArg3;
    func_15075CA0();
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
        i = func_150ADA20_u8();
        gCurrentActor->unkA8 = gCurrentActor->unk222;
        gCurrentActor->unk222 = candidates[i % count];
    }
}

void func_1507A3B4(void) {
    gCurrentActor->unk222 = gCurrentActor->unkA8;
}

void func_1507A3CC(void) {
    gCurrentActor->unk229 = gScriptArg0;
}

/* Returns the four argument bytes as one big-endian word. */
s32 script_getArgWord(void) {
    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    return ((gScriptArg0 & 0xFF) << 24) | ((gScriptArg1 & 0xFF) << 16) | ((gScriptArg2 & 0xFF) << 8) | (gScriptArg3 & 0xFF);
}

void func_1507A428(void) {
    s32 flags;

    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    flags = ((gScriptArg0 & 0xFF) << 24) | ((gScriptArg1 & 0xFF) << 16) | ((gScriptArg2 & 0xFF) << 8) | (gScriptArg3 & 0xFF);
    gCurrentActor->unk94 = ~(flags | 1);
}

void func_1507A47C(void) {
    s32 flags;

    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    flags = ((gScriptArg0 & 0xFF) << 24) | ((gScriptArg1 & 0xFF) << 16) | ((gScriptArg2 & 0xFF) << 8) | (gScriptArg3 & 0xFF);
    gCurrentActor->unk94 &= ~flags;
}

void func_1507A4D4(void) {
    s32 flags;

    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    flags = ((gScriptArg0 & 0xFF) << 24) | ((gScriptArg1 & 0xFF) << 16) | ((gScriptArg2 & 0xFF) << 8) | (gScriptArg3 & 0xFF);
    gCurrentActor->unk94 |= flags;
}

/*
 * Waypoint direction: arg0 0 sets the step unk221 to arg1, 1 reverses it, 2 reverses it and moves
 * the waypoint index by the new step plus arg3 further, wrapping at arg2 (0: the path's count).
 */
void func_1507A528(void) {
    s32 count;
    s32 index;

    if (gScriptArg0 == 0) {
        gCurrentActor->unk221 = gScriptArg1;
    } else if (gScriptArg0 == 1) {
        gCurrentActor->unk221 = -gCurrentActor->unk221;
    } else if (gScriptArg0 == 2) {
        if (gScriptArg2 != 0) {
            count = gScriptArg2;
        } else {
            count = D_800D2108[gCurrentActor->listpos] - 1;
        }
        gCurrentActor->unk221 = -gCurrentActor->unk221;
        index = gCurrentActor->unk21E + gCurrentActor->unk221;
        if (gCurrentActor->unk221 > 0) {
            index += gScriptArg3;
        } else {
            index -= gScriptArg3;
        }
        if (index >= count) {
            index -= count;
        } else if (index < 0) {
            index += count;
        }
        gCurrentActor->unk21E = index;
    }
}

/*
 * Jumps to arg0 when the angle (arg1: the object's unk78, else its facing relative to the
 * direction to the target), in 1/256 turns, lies in [arg2, arg3).
 */
void script_jumpIfAngleInRange(void) {
    u8 index;
    u16 angle;

    index = gCurrentActor->unk222;
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

s32 func_1507A6FC(s32 arg0) {
    return D_800BE748[arg0].button;
}

/* Jumps to arg3 when entry arg0 of D_800BE748 has all the bits of (arg1:arg2) set. */
void func_1507A71C(void) {
    u16 bits = func_1507A6FC(gScriptArg0);
    u16 mask = (gScriptArg1 << 8) | gScriptArg2;

    if ((bits & mask) == mask) {
        script_jump(gScriptArg3);
    }
}

void func_1507A774(void) {
    u16 value = (gScriptArg1 << 8) | gScriptArg2;

    if (level == value) {
        script_jump(gScriptArg0);
    }
}

void func_1507A7C0(void) {
    gCurrentActor->norun = gScriptArg0;
}

void func_1507A7DC(void) {
    func_150836CC(gCurrentActor, gScriptArg0);
}

void func_1507A808(void) {
    gCurrentActor->unk22C = (gCurrentActor->unk22C & gScriptArg0) | gScriptArg1;
}

void func_1507A838(void) {
    if (gCurrentActor->energy == gScriptArg1) {
        script_jump(gScriptArg0);
    }
}

void func_1507A878(void) {
    func_1512D748(0, gScriptArg0, gScriptArg1);
}

void func_1507A8A8(void) {
    if ((movement_obj[0].attach_char != 0) || (gIsMultiplayer != 0)) {
        script_jump(gScriptArg0);
    }
}

/* Jumps to arg0 when the target is (arg2 == 0) or is not (arg2 == 1) of type arg1. */
void script_jumpIfTargetType(void) {
    if (gScriptArg1 == movement_obj[gCurrentActor->unk222].character_type) {
        gScriptArg2 ^= 1;
    }
    if (gScriptArg2 != 0) {
        script_jump(gScriptArg0);
    }
}

/* Links the object to the one found by func_15083E90(arg0) (unk65 = its index + 1), or unlinks it (arg0 == 0). */
void script_attachToObject(void) {
    Actor *linked;

    if (gScriptArg0 != 0) {
        linked = func_15083E90(gScriptArg0);
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
void func_1507AA48(void) {
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
        path = D_800D2104[gCurrentActor->listpos];
        angle = (u16)(func_150484A0(path[gScriptArg1 + 1].x - path[gScriptArg0 + 1].x, path[gScriptArg1 + 1].z - path[gScriptArg0 + 1].z) * 10430.37793f) + (gScriptArg3 << 8);
        gCurrentActor->fangle = angle + 0x4000;
        break;
    case 5:
        gCurrentActor->fangle = func_150ADA20_u8();
        break;
    case 6:
        gCurrentActor->fangle = gCurrentActor->angle;
        break;
    case 7:
        gCurrentActor->angle = movement_obj[gCurrentActor->unk222].langle + value;
        break;
    case 8:
        gCurrentActor->angle = gCurrentActor->langle;
        break;
    }
}

void func_1507ACB0(void) {
    gCurrentActor->unk220 = gScriptArg0;
    gCurrentActor->unk21F = gScriptArg1;
}

void func_1507ACE0(void) {
    gCurrentActor->unk1E5 = gScriptArg0;
    gCurrentActor->unk1E6 = gScriptArg0;
    gCurrentActor->unk1E7 = gScriptArg1;
    gCurrentActor->unk1E8 = gScriptArg1;
}

void func_1507AD30(void) {
    gCurrentActor->unk255 = gScriptArg0;
    gCurrentActor->unk256 = gScriptArg1;
}

void func_1507AD60(void) {
    gCurrentActor->family = gScriptArg0;
}

void func_1507AD7C(void) {
    gCurrentActor->mvmt_control |= script_getArgWord();
}

void func_1507ADAC(void) {
    gCurrentActor->mvmt_control &= ~script_getArgWord();
}

/* Splits (angle between unk78 and the facing unk7A) / arg0 into unk1E6 (high byte) and unk1E8 (low byte). */
void func_1507ADE0(void) {
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
    gCurrentActor->unk1E6 = step >> 8;
    gCurrentActor->unk1E8 = step & 0xFF;
}

void func_1507AE78(void) {
    gCurrentActor->unk229 = gScriptArg0;
}

void func_1507AE94(void) {
    gCurrentActor->unkD2 = (gScriptArg0 << 8) | gScriptArg1;
    gCurrentActor->unkD4 = (gScriptArg2 << 8) | gScriptArg3;
    gCurrentActor->unkD2 *= gCurrentActor->scale_xz;
    gCurrentActor->unkD4 *= gCurrentActor->scale_y;
}

void func_1507AF3C(void) {
    gCurrentActor->unkD6 = (gScriptArg0 << 8) | gScriptArg1;
    gCurrentActor->unkD6 *= gCurrentActor->scale_y;
}

void func_1507AF98(void) {
    gCurrentActor->unkD8 = (gScriptArg0 << 8) | gScriptArg1;
    gCurrentActor->unkDA = (gScriptArg2 << 8) | gScriptArg3;
    gCurrentActor->unkD8 *= gCurrentActor->scale_xz;
    gCurrentActor->unkDA *= gCurrentActor->scale_xz;
}

void func_1507B040(void) {
    gCurrentActor->angle = gCurrentActor->langle;
}

/* Acts on the object returned by event_getOwnedObject: arg0 0 calls actor_fadeCommand, 1 sets and 2 restarts script arg1. */
void func_1507B058(void) {
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
            gCurrentActor->unk21C = 0;
        }
    }
    dc_glbl_CharNum = savedIndex;
    gCurrentActor = savedObj;
}

void func_1507B15C(void) {
    gCurrentActor->nojump = gScriptArg0;
}

void func_1507B178(void) {
    s16 value = (gScriptArg1 << 8) | gScriptArg0;

    if (gScriptArg3 == 2) {
        gCurrentActor->bodyOffsetY = value * gCurrentActor->scale_y;
    } else if (gScriptArg3 == 1) {
        actor_setBodyHeight(gCurrentActor, value);
    } else {
        actor_setBodyRadius(gCurrentActor, value);
    }
}

void func_1507B234(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(300, 1800);
    }
}

void func_1507B280(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(300, 3000);
    }
}

void func_1507B2CC(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(300, 1800);
    }
}

void func_1507B318(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(300, 3000);
    }
}

void func_1507B364(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(700, 4000);
    }
}

void func_1507B3B0(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(700, 4000);
    }
}

void func_1507B3FC(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(3000, 8000);
    }
}

void func_1507B448(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(190, 1300);
    }
}

void func_1507B494(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(190, 1300);
    }
}

void func_1507B4E0(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(100, 800);
    }
}

void func_1507B52C(void) {
    gEventParam = script_getArgWord();
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(100, 800);
    }
}

void func_1507B578(void) {
    gCurrentActor->doing_hit = gScriptArg0;
    gCurrentActor->unk114 = gScriptArg1;
}

/* Jumps to arg0 when mv->context_b is (arg2 == 0) or is not (arg2 == 1) arg1. */
void func_1507B5C4(void) {
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
    gCurrentActor->unk21C = 100;
}

void func_1507B6E0(void) {
    if ((gCurrentActor->norun != 0) || (gCurrentActor->shad > 5.0f)) {
        // repeat this command
        gCurrentActor->scriptCmd--;
        gCurrentActor->unk21C = 100;
    }
}

void func_1507B734(void) {
    u16 value = (gScriptArg0 << 8) | gScriptArg1;

    if (gScriptArg3 != 0) {
        gCurrentActor->mv->pressedButtons = value;
        gCurrentActor->mv->pressedButtonsTimer = gScriptArg2;
    } else {
        gCurrentActor->mv->buttons = value;
        gCurrentActor->mv->buttonsTimer = gScriptArg2;
    }
}

void func_1507B7BC(void) {
    script_startMove();
    gCurrentActor->unk223 = 17;
}

/* Jumps to arg0 when the target is (arg1 == 0) or is not (arg1 == 1) stunned or dead. */
void script_jumpIfTargetStunned(void) {
    Actor *target = &movement_obj[gCurrentActor->unk222];

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

void func_1507B958(void) {
    gCurrentActor->unkB0 = gScriptArg0;
}

/*
 * Jumps to arg0 when unk6 of the path record arg3 past the current waypoint (of the linked object,
 * if any) is (arg2 == 0) or is not (arg2 == 1) arg1.
 */
void func_1507B974(void) {
    Actor *obj;

    obj = gCurrentActor;
    if (gCurrentActor->attach_char != 0) {
        obj = &movement_obj[gCurrentActor->attach_char - 1];
    }
    if (gScriptArg1 == D_800D2104[obj->listpos][obj->unk21E + gScriptArg3].param) {
        gScriptArg2 ^= 1;
    }
    if (gScriptArg2 != 0) {
        script_jump(gScriptArg0);
    }
}

void func_1507BA48(void) {
    u16 id = (gScriptArg2 << 8) | gScriptArg3;

    if (gScriptArg0 != 0) {
        id += func_150ADA20_u8() % (u32)gScriptArg0;
    }
    actor_playUntrackedSound(id, gCurrentActor);
}

void func_1507BAD0(void) {
    gCurrentActor->unkFC = script_getArgWord();
}

void func_1507BAF8(void) {
    gCurrentActor->unk2CC = script_getArgWord();
}

void func_1507BB20(void) {
}

typedef struct ScriptEntry {
    const ObjScriptCmd *script;
    u8 id;
} ScriptEntry;

/* The fallback script. */
static const ObjScriptCmd D_8009A120[5] = {
    { 0x67, { 0x14, 0x0F, 0x19, 0x00 } },
    { 0x31, { 0x80, 0x00, 0x00, 0x00 } },
    { 0x33, { 0xFF, 0x00, 0x00, 0x00 } },
    { 0x01, { 0x14, 0x00, 0x00, 0x00 } },
    { 0x08, { 0x00, 0x00, 0x00, 0x00 } },
};

/* The command handlers, indexed by opcode. */
static void (*D_80086730[214])(void) = {
    func_1507735C, script_startMove, func_15075BE8, func_1507A774,
    func_1507A71C, func_1507A7C0, func_15075C24, func_15075CA0,
    script_switchScript, func_15075E6C, func_15075E98, script_jumpRandom,
    func_15075F00, func_15075F40, func_1507BA48, func_15075F6C,
    script_leapToWaypoint, func_150761C8, func_1507BAD0, func_150761F4,
    func_1507AE94, func_1507A7DC, func_1507A838, func_1507A878,
    func_15076250, func_150762B0, func_1507A8A8, func_1507AA48,
    func_1507ACB0, func_1507ACE0, script_jumpIfAngleInRange, func_1507AF3C,
    func_15076220, script_jumpIfNotHit, func_15077404, func_15075CCC,
    func_15075BB8, NULL, func_15075DC8, func_150762D4,
    func_1507AD30, func_1507AD60, script_jumpIfTargetType, func_1507ADE0,
    func_15076394, func_150764C8, func_1507AE78, script_setHitRoute,
    func_15076500, func_150763B0, func_1507AF98, script_setImmune,
    func_1507B040, func_1507B058, func_1507659C, func_1507911C,
    func_1507B15C, func_1507B234, func_1507B280, func_1507B2CC,
    func_1507B318, func_15076600, func_1507B364, func_1507B3B0,
    func_1507B3FC, func_15076624, func_1507B178, func_15076608,
    func_1507B974, func_150766D0, func_15076760, func_15076768,
    func_1507B578, func_1507BAF8, func_150767F4, func_150768DC,
    func_15076B5C, func_15076B78, script_startMove, func_15076C7C,
    func_15076CB4, func_15076B94, func_1507AD7C, func_1507ADAC,
    func_15076CCC, script_setSpeed, script_setScale, func_1507B7BC,
    script_setType, func_1507B448, func_1507B494, func_1507B4E0,
    script_startMove, func_1507B52C, func_15075B8C, script_jumpIfTargetStunned,
    script_startMove, func_1507B8F4, func_150771F0, func_150772E8,
    func_15076678, func_1507A808, script_pickWaypoint, func_15077364,
    script_advanceWaypoint, func_150779A8, func_150779D4, func_15075D64,
    func_1507BB20, func_15075AAC, func_15077190, func_15075B60,
    func_15076E10, func_15076E48, func_1507B5C4, func_1507B630,
    func_1507B6E0, func_15076E50, func_1507B734, func_15076F40,
    func_15076F78, func_15076FA8, func_15077BE4, script_jumpIfTargetSlow,
    func_15077174, func_1507B958, func_15075D9C, script_setEnergy,
    script_leapToTarget, func_1507965C, func_150796CC, func_1507735C,
    script_targetRandomOfType, func_1507A3B4, func_1507A528, func_1507735C,
    func_1507735C, func_1507735C, func_15077AA0, func_1507735C,
    func_15077ABC, func_15077B14, func_15077B44, func_15077C1C,
    func_15075938, func_15075884, func_15077C38, script_jumpIfWaypoint,
    func_15077B80, func_15077BB4, func_15077DA0, NULL,
    func_15077E9C, script_warpToWaypoint, func_15077EEC, func_15077F08,
    func_15077F34, script_turnToWaypoint, script_jumpIfAnyObjectNear, script_jumpIfTargetInactive,
    func_150781A4, script_jumpIfNearWaypoint, func_150782CC, NULL,
    script_jumpIfUniqueId, script_targetUniqueId, script_jumpIfTargetDistance, func_15078520,
    func_15078544, func_1507879C, func_15078874, func_15078890,
    func_15078900, func_15078A08, func_1507900C, func_15079228,
    func_150792E0, func_150792FC, func_15079390, func_1507903C,
    func_150793D8, func_15079880, script_jumpIfAtWaypoint, func_15079928,
    func_15079988, func_15079090, func_150799B4, func_15079790,
    func_15079334, func_150791F0, func_15079A28, func_15079A58,
    func_15079B30, func_15079F24, func_15079F50, func_15079F6C,
    func_15079FBC, script_setWaypointCoord, func_1507A164, func_1507A210,
    func_150790C4, func_15075D38, func_1507A270, func_15075D0C,
    func_1507A2A4, func_1507A3CC, func_1507A428, func_1507A47C,
    func_1507A4D4, script_attachToObject, func_15077508, script_jumpIfAtDestination,
    func_15077318, script_callHandler
};

/* The default scripts, ended by a NULL script. */
static ScriptEntry D_80086A88[3] = {
    { D_8009A120, 0x01 },
    { D_8009A120, 0xFA },
    { NULL, 0 },
};

/* Returns script `scriptId` of the current object's type, falling back to the default table. */
const ObjScriptCmd *script_getScript(s32 arg0, s32 scriptId) {
    ActorAnim *typeData;
    ScriptEntry *table;
    s32 i;

    typeData = gModelAnimTables[gCurrentActor->character_type];
    if (typeData == NULL) {
        return D_8009A120;
    }
    table = ANIM_HEADER(typeData)->scripts;
    if (table == NULL) {
        return D_8009A120;
    }
    i = 0;
    while (scriptId != table[i].id) {
        if (table[i].script == NULL) {
            ScriptEntry *defaults = D_80086A88;

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
 * Runs an object's script: counts down the wait timer unk21C (25500 waits forever) and, once it
 * expires, executes commands until one sets a new wait time.
 */
void script_runActor(Actor *obj) {
    if (obj->unk21C != 0) {
        if (obj->scriptCmd != NULL) {
            if (obj->unk21C != 25500) {
                obj->unk21C = obj->unk21C - D_800CC264;
            }
            if ((obj->unk21C >= 50000) || (obj->unk21C == 0)) {
                // expired (or wrapped below zero)
                obj->unk21C = 0;
                if (obj->mvmt_control & 0x400) {
                    obj->unk21C = 20000;
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
        obj->unk21C = 0;
    }

    while (obj->unk21C == 0) {
        if (obj->scriptCmd->op >= 247) {
            // a label: skip its single byte
            obj->scriptCmd = (const ObjScriptCmd *)((const u8 *)obj->scriptCmd + 1);
        } else {
            gScriptArg0 = obj->scriptCmd->args[0];
            gScriptArg1 = obj->scriptCmd->args[1];
            gScriptArg2 = obj->scriptCmd->args[2];
            gScriptArg3 = obj->scriptCmd->args[3];
            D_80086730[obj->scriptCmd->op]();
            obj->scriptCmd++;
        }
    }
}
