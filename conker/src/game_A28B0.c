#include <ultra64.h>

// matching: func_150ADA20 returns a u32 random number, but this file was built against a u8-returning
// prototype (callers use the low byte without masking it), so it keeps that view.
#define func_150ADA20 func_150ADA20_functions_h
#include "functions.h"
#undef func_150ADA20
u8 func_150ADA20(void);
#include "variables.h"

/*
 * Object script interpreter commands.
 *
 * Each object runs a byte-code script (Actor.unk218 points at its next command). A command
 * is five bytes: an opcode (< 247, an index into D_80086730) and four argument bytes, which the
 * interpreter copies to D_800D1890..D_800D1893 before calling the handler. Bytes >= 247 are
 * one-byte jump labels.
 * The handlers below act on the current object, D_800D154C (index dc_glbl_CharNum in movement_obj);
 * their comments call the four argument bytes arg0..arg3. An object's "target" is the object
 * movement_obj[unk222], its current waypoint is unk21E of its path D_800D2104[unk13F].
 */



/* Jumps the script: skips `target` commands, or for target >= 247 skips forward to label `target`. */
void func_15075400(s32 target) {
    u8 label;
    u8 i;

    if (target < 247) {
        D_800D154C->scriptCmd += target;
        return;
    }

    for (i = 0; i < 100; i++) {
        if (D_800D154C->scriptCmd->op < 247) {
            D_800D154C->scriptCmd++;
        } else {
            label = D_800D154C->scriptCmd->op;
            D_800D154C->scriptCmd = (ObjScriptCmd *)((u8 *)D_800D154C->scriptCmd + 1);
            if (target == label) {
                // the interpreter steps past the current command after this returns
                D_800D154C->scriptCmd--;
                return;
            }
        }
    }
}

/* Applies the common movement flags of a move command. */
void func_15075498(void) {
    s32 value = D_800D1893 & 0x7F;

    if (value != 0x7F) {
        D_800D154C->intel_animseq = value;
    }
    D_800D154C->mvmt_control &= ~0x143E;
    if (D_800D1890 == 0xFA) {
        D_800D154C->mvmt_control |= 0x22;
    } else if (D_800D1890 == 0xFB) {
        D_800D154C->mvmt_control |= 4;
    }
    if (D_800D1893 & 0x80) {
        D_800D154C->mvmt_control |= 0x10;
    }
}

/* Starts a move: mode 0, duration arg0 * 100, speed arg2 (0xFF keeps the current speed). */
void func_15075548(void) {
    f32 speed;
    u8 speedArg;

    D_800D154C->unk223 = 0;
    D_800D154C->unk21C = D_800D1890 * 100;
    speedArg = D_800D1892;
    if (speedArg != 0xFF) {
        speed = speedArg;
        D_800D154C->stickspeed = speed;
        if (D_800D154C->stickspeed == 1.0f) {
            D_800D154C->stickspeed = 0.5f;
        }
        if (D_800BE616 != 0) {
            if ((D_800D154C->character_type == 40) && (D_800D154C->mv->unk128 & 1)) {
                D_800D154C->stickspeed *= 1.4f;
            }
        }
    }
    func_15075498();
}

/* Picks the next waypoint: random (arg1 0xFF), random but not the previous one (0xFE), or arg1. */
void func_15075650(void) {
    u8 range;
    u8 index;
    u8 previous;
    Actor *obj;

    index = D_800D2108[D_800D154C->listpos] - 1;
    if (D_800D154C->unk21F != 0) {
        index = D_800D154C->unk21F;
    }
    range = index - D_800D154C->unk220;
    if (D_800D1891 == 0xFF) {
        if (D_800D154C->unk223 != 13) {
            D_800D154C->unk21E = (func_150ADA20() % (u32)range) + D_800D154C->unk220;
        }
    } else if (D_800D1891 == 0xFE) {
        if (D_800D154C->unk223 != 13) {
            previous = D_800D154C->unk21E - 1;
            if (D_800D154C->unk21E <= D_800D154C->unk220) {
                previous = index - 1;
            }
            index = (func_150ADA20() % (u32)range) + D_800D154C->unk220;
            if (index != previous) {
                D_800D154C->unk21E = index;
                // matching: the copy, the `* 0` and the jump steer IDO to duplicate this store into v1
                obj = D_800D154C;
                obj->unk223 = D_800D154C->unk220 * 0;
                goto set_timer;
            }
        }
    } else if (D_800D1891 == 0xFF) {
        D_800D154C->unk21E = index - 1;
    } else {
        D_800D154C->unk21E = D_800D1891;
    }
    D_800D154C->unk223 = 0;
set_timer:
    D_800D154C->unk21C = D_800D1890 * 100;

    if (D_800D1892 != 0xFF) {
        D_800D154C->stickspeed = D_800D1892;
        if (D_800D154C->stickspeed == 1.0f) {
            D_800D154C->stickspeed = 0.5f;
        }
    }
    if (D_800D154C->unk21E < D_800D154C->unk220) {
        D_800D154C->unk21E = D_800D154C->unk220;
    }
    func_15075498();
}

/* Starts a move towards the current waypoint, arriving in arg1 ticks. */
void func_15075884(void) {
    Actor *obj;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 speed;

    func_15075548();
    obj = D_800D154C;
    dx = (f32)D_800D2104[obj->listpos][obj->unk21E + 1].x - obj->xpos;
    dz = (f32)D_800D2104[obj->listpos][obj->unk21E + 1].z - obj->zpos;
    dist = sqrtf((dx * dx) + (dz * dz));
    dx = (f32)(u32)D_800D1891;
    speed = dist / dx;
    obj->stickspeed = speed + speed;
}

/* Jumps to arg0 when waypoint arg1 compares (per arg2) with the waypoint unk221 steps back. */
void func_15075938(void) {
    s32 position;
    s32 desired;
    s32 matched;

    matched = 0;
    position = D_800D154C->unk21E - (s8)D_800D154C->unk221;
    if (position < 0) {
        position += D_800D2108[D_800D154C->listpos] - 1;
    } else if (position >= D_800D2108[D_800D154C->listpos] - 1) {
        position -= D_800D2108[D_800D154C->listpos] - 1;
    }
    desired = D_800D1891 == 0xFF ? D_800D2108[D_800D154C->listpos] - 2 : D_800D1891;
    if (D_800D1892 == 0) {
        if (desired == position) {
            matched = 1;
        }
    } else if (D_800D1892 == 1) {
        if (desired != position) {
            matched = 1;
        }
    } else if (D_800D1892 == 2) {
        if (desired < position) {
            matched = 1;
        }
    } else if (position < desired) {
        matched = 1;
    }
    if (matched) {
        func_15075400(D_800D1890);
    }
}

/* Jumps to arg0 when the current waypoint is (arg2 == 0) or is not (arg2 == 1) arg1. */
void func_15075A50(void) {
    u8 index = D_800D154C->unk21E;

    if (((D_800D1892 == 0) && (D_800D1891 == index)) ||
        ((D_800D1892 == 1) && (D_800D1891 != index))) {
        func_15075400(D_800D1890);
    }
}

/* Starts a move, stopping at once when already within 40 units of path record arg1. */
void func_15075AAC(void) {
    PathPoint *point;
    f32 dx;
    f32 dz;

    func_15075548();
    point = D_800D2104[D_800D154C->listpos] + D_800D1891;
    dx = point->x - D_800D154C->xpos;
    dz = point->z - D_800D154C->zpos;
    if (fabsf(dx) + fabsf(dz) < 40.0f) {
        D_800D154C->unk21C = 0;
        D_800D154C->speed = 0.0f;
    }
}

void func_15075B60(void) {
    func_15075548();
    D_800D154C->unk223 = 10;
}

void func_15075B8C(void) {
    func_15075650();
    D_800D154C->unk223 = 10;
}

void func_15075BB8(void) {
    func_15075548();
    D_800D154C->unk22C |= 2;
}

void func_15075BE8(void) {
    if (D_800D1893 != 0) {
        D_800D154C->unk22C |= 0x80;
    }
    D_800D154C->unk233 = D_800D1890;
}

/* Sets the vertical velocity to the s16 (arg2:arg0) unless arg3 == 1, and gravity to (s8)arg1 unless arg3 == 2. */
void func_15075C24(void) {
    if (D_800D1893 != 1) {
        D_800D154C->yspd = (s16)((D_800D1892 << 8) | D_800D1890);
    }
    if (D_800D1893 != 2) {
        D_800D154C->ygrav = (s16)(s8)D_800D1891;
    }
}

void func_15075CA0(void) {
    func_15075548();
    D_800D154C->unk223 = 1;
}

void func_15075CCC(void) {
    func_15075548();
    D_800D154C->unk223 = 16;
    D_800D154C->unk231 = D_800D1891;
}

void func_15075D0C(void) {
    func_15075548();
    D_800D154C->unk223 = 15;
}

void func_15075D38(void) {
    func_15075CA0();
    D_800D154C->unk223 = 14;
}

void func_15075D64(void) {
    func_15075548();
    D_800D154C->unk223 = 12;
    D_800D154C->unk222 = 0;
}

void func_15075D9C(void) {
    func_15075548();
    D_800D154C->unk223 = 9;
}

void func_15075DC8(void) {
    func_15075CA0();
}

/* Switches to script arg1 (0: the object's current script id); a nonzero arg3 becomes the new id. */
void func_15075DE8(void) {
    if (D_800D1891 == 0) {
        D_800D1891 = D_800D154C->intelwalk;
    }
    if (D_800D1893 != 0) {
        D_800D154C->intelwalk = D_800D1893;
    }
    D_800D154C->scriptCmd = func_1507BB28(0, D_800D1891);
    // the interpreter steps past the current command after this returns
    D_800D154C->scriptCmd--;
}

void func_15075E6C(void) {
    func_15075548();
    D_800D154C->unk223 = 2;
}

void func_15075E98(void) {
    D_800D154C->unk235 = D_800D1890;
}

/* Jumps to arg0 with a chance of arg2 percent. */
void func_15075EB4(void) {
    if ((func_150ADA20() % 100U) < D_800D1892) {
        func_15075400(D_800D1890);
    }
}

void func_15075F00(void) {
    D_800D154C->unk234 = D_800D1890;
    D_800D154C->unk236 = D_800D1891;
    D_800D154C->unk237 = D_800D1892;
}

void func_15075F40(void) {
    func_15075548();
    D_800D154C->unk223 = 3;
}

/* Sets the vertical velocity (arg0, downwards if arg2), the gravity (arg1) and unk3A (arg3). */
void func_15075F6C(void) {
    if (D_800D1890 != 0) {
        if (D_800D1892 == 0) {
            D_800D154C->yspd = D_800D1890;
        } else {
            D_800D154C->yspd = -D_800D1890;
        }
    }
    if (D_800D1891 != 0) {
        D_800D154C->ygrav = D_800D1891;
    }
    D_800D154C->unk3A = D_800D1893;
}

/* Starts a jump towards waypoint arg1, timing the flight so that it lands there. */
void func_1507602C(void) {
    f32 dx;
    f32 dz;
    f32 time;

    func_15075548();
    D_800D154C->unk21E = D_800D1891;
    dx = (f32)D_800D2104[D_800D154C->listpos][D_800D1891 + 1].x - D_800D154C->xpos;
    dz = (f32)D_800D2104[D_800D154C->listpos][D_800D1891 + 1].z - D_800D154C->zpos;
    time = 2.0f * (sqrtf((dx * dx) + (dz * dz)) / D_800D154C->stickspeed);
    if (time < 12.0f) {
        D_800D154C->stickspeed *= time * 0.08333300054f;
        time = 12.0f;
    }
    D_800D154C->unk21C = (u32)(time * 100.0f);
    D_800D154C->yspd = D_800D154C->ygrav * time * 0.5f;
    D_800D154C->unk223 = 4;
}

void func_150761C8(void) {
    func_15075650();
    D_800D154C->unk223 = 5;
}

void func_150761F4(void) {
    D_800D154C->unk223 = 6;
    D_800D154C->unk22C &= ~1;
}

void func_15076220(void) {
    func_150761F4();
    D_800D154C->unk22C |= 1;
}

void func_15076250(void) {
    s32 id = (D_800D1890 << 8) | D_800D1891;

    func_15060778(id, D_800D154C, 32000, -100, 500, 2500, D_800D1892);
}

void func_150762B0(void) {
    func_1000CBA8(D_800D1890);
}

void func_150762D4(void) {
    func_151669A0((s32)D_800D154C->xpos, (s32)(D_800D154C->ypos + 100.0f),
                  (s32)D_800D154C->zpos, 0.45f, 0xFF, 0);
}

void func_15076340(void) {
    if (D_800D154C->ivehitit == 0) {
        func_15075400(D_800D1890);
    }
    if (D_800D1891 != 0) {
        D_800D154C->ivehitit = 0;
    }
}

void func_15076394(void) {
    D_800D154C->unk236 = D_800D1890;
}

void func_150763B0(void) {
    if (D_800D1890 == 128) {
        D_800D154C->unk1E5 = D_800D154C->listpos_ptr->unkF;
    } else {
        D_800D154C->unk1E5 = D_800D1890;
    }
    D_800D154C->unk1E7 = 0;

    if (D_800D1892 != 0) {
        D_800D154C->unk1E5 += func_150ADA20() % (u32)D_800D1892;
    }
    if (D_800D1893 == 1) {
        D_800D154C->unk1E6 = 0;
        D_800D154C->unk1E8 = 0;
        return;
    }
    if (D_800D1893 == 2) {
        D_800D154C->unk1E6 = D_800D1891;
        D_800D154C->unk1E8 = 0;
        return;
    }
    if (D_800D1891 == 0) {
        D_800D154C->unk1E6 = D_800D154C->unk1E5;
        D_800D154C->unk1E8 = 0;
    }
}

void func_150764C8(void) {
    D_800D154C->unk238 = D_800D1890;
}

void func_150764E4(void) {
    D_800D154C->intelhitroute = D_800D1890;
}

void func_15076500(void) {
    D_800D154C->unk22E = (D_800D1890 << 8) | D_800D1891;
}

/* Sets `immune` to arg0 on the object's target (arg1 != 0) or on the object itself. */
void func_1507652C(void) {
    if (D_800D1891 != 0) {
        movement_obj[D_800D154C->owner].imune = D_800D1890;
    } else {
        D_800D154C->imune = D_800D1890;
    }
}

void func_1507659C(void) {
    D_800D154C->ypos = D_800D154C->ypos + D_800D1890 * 100;
    if (D_800D1893 != 0) {
        D_800D154C->ypos = 1800.0f;
    }
}

void func_15076600(void) {
}

void func_15076608(void) {
    D_800D154C->unk24A = D_800D1890;
}

/* Sets the flags given by the four argument bytes in unkF8. */
void func_15076624(void) {
    s32 flags = (D_800D1890 << 24) | (D_800D1891 << 16) | (D_800D1892 << 8) | D_800D1893;

    D_800D154C->noshove |= flags;
}

/* Clears the flags given by the four argument bytes in unkF8. */
void func_15076678(void) {
    s32 flags = (D_800D1890 << 24) | (D_800D1891 << 16) | (D_800D1892 << 8) | D_800D1893;

    D_800D154C->noshove &= ~flags;
}

void func_150766D0(void) {
    if (((D_800D154C->shad > 0.0f) && (D_800D1890 == 0)) ||
        ((D_800D1890 * 50 < D_800D154C->shad) && (D_800D1890 != 0))) {
        D_800D154C->unk21C = 99;
        D_800D154C->scriptCmd--;
    }
}

void func_15076760(void) {
}

void func_15076768(void) {
    f32 position[3];
    f32 matrix[9];

    switch (D_800D1890) {
    case 0:
        func_15197A7C(D_800D154C);
        break;
    case 1:
        position[0] = D_800D154C->xpos;
        position[1] = -390.0f;
        position[2] = D_800D154C->zpos;
        // matching: the jump ends the basic block, so as1 keeps the stores before the call
        goto setup_done;
    setup_done:
        func_1504715C(matrix, D_800D154C);
        func_1514B364(position, matrix, 0xFF, 0);
        break;
    }
}

/* Jumps to arg0 when the direction from the object to its target is within arg1 of the target's facing. */
void func_150767F4(void) {
    Actor *target;
    u16 angle;

    target = &movement_obj[D_800D154C->unk222];
    // matching: the (s32) cast before narrowing to u16 is needed
    angle = (s32)func_1505A630(target->xpos - D_800D154C->xpos,
                               D_800D154C->zpos - target->zpos, 0);
    if ((((angle >> 8) - (movement_obj[D_800D154C->unk222].langle >> 8) + D_800D1891) & 0xFF) < D_800D1891 * 2) {
        func_15075400(D_800D1890);
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

    nearest = D_800D1890 * 8;
    D_800D154C->unkA8 = 0;
    self = D_800D154C - movement_obj;

    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if ((obj->type != 0) && (obj->energy != 0) && (obj->noshove & 0x20) && (i != self)) {
            if ((D_800D1891 != 0) || (obj->character_type != D_800D154C->character_type)) {
                if (fabsf(D_800D154C->ypos - obj->ypos) > 50.0f) {
                    continue;
                }
                dist = func_1505A72C(D_800D154C, obj);
                if (dist < nearest) {
                    type = D_800D1C90[D_800D154C->character_type];
                    viewAngle = type->viewAngle;
                    angle = func_1505A630(obj->xpos - D_800D154C->xpos,
                                          D_800D154C->zpos - obj->zpos, 0) >> 8;
                    if (((((D_800D154C->angle >> 8) + D_800D154C->unk2CA) - angle + (viewAngle / 2)) & 0xFF) < viewAngle) {
                        D_800D154C->unkA8 = i + 0x80;
                        nearest = dist;
                    }
                }
            }
        }
    }

    if (D_800D154C->unkA8 != 0) {
        D_800D154C->scriptCmd = func_1507BB28(0, D_800D1893);
        D_800D154C->scriptCmd--;
        script = D_800D1892;
        if (script != 0) {
            foundIndex = D_800D154C->unkA8;
            found = &movement_obj[foundIndex & 0x7F];
            found->scriptCmd = NULL;
            found->intelwalk = script;
            found->unk222 = self;
        }
    }
}

void func_15076B5C(void) {
    D_800D154C->unk222 = D_800D154C->unkA8 & 0x7F;
}

void func_15076B78(void) {
    D_800D154C->drawtype = D_800D1890;
}

/* Targets the last object below this one (all 25 if arg1) of type arg0 that is alive, and starts mode 9. */
void func_15076B94(void) {
    s32 i = D_800D154C - movement_obj;

    if (D_800D1891) {
        i = 25;
    }
    while (i != 0) {
        if ((D_800D1890 == movement_obj[i - 1].character_type) && (movement_obj[i - 1].energy != 0)) {
            func_15075548();
            D_800D154C->unk223 = 9;
            D_800D154C->unk222 = i - 1;
            D_800D154C->unk21C = 1000;
            return;
        }
        i--;
    }
}

void func_15076C7C(void) {
    func_1506160C(D_800D154C, 1, D_800D1890, 0, 0);
}

void func_15076CB4(void) {
    D_800D154C->angle = D_800D154C->fangle;
}

void func_15076CCC(void) {
    D_800D154C->unk21C = 10000;
    func_15060F28(D_800D154C, 1);
}

void func_15076D04(void) {
    D_800D154C->speed = D_800D1890;
}

/* Sets the object's scale from two s16 percentages, (arg1:arg0) horizontal and (arg3:arg2) vertical. */
void func_15076D3C(void) {
    f32 scale = 0.01f;
    s16 xz = D_800D1890 | (D_800D1891 << 8);
    s16 y = D_800D1892 | (D_800D1893 << 8);

    D_800D154C->scale_xz = (f32)xz * scale;
    D_800D154C->scale_y = (f32)y * scale;
    D_800D154C->unk154 = D_800D154C->scale_xz;
    D_800D154C->unk158 = D_800D154C->scale_y;
    func_15062BDC(D_800D154C, D_800D154C->scale_xz, D_800D154C->scale_y);
}

void func_15076DF4(void) {
    D_800D154C->type = D_800D1890;
}

void func_15076E10(void) {
    D_800D154C->unk1D0 = D_800D1890;
    D_800D154C->unk22C |= D_800D1891;
}

void func_15076E48(void) {
}

/* Starts mode 7: a jump in a random direction with random speeds. */
void func_15076E50(void) {
    func_15075548();
    D_800D154C->yspd = (f32)(u32)(func_150ADA20() % 20U) + 55.0f;
    D_800D154C->ygrav = 2.0f;
    D_800D154C->fangle = func_150ADA20() % 0xFFFFU;
    D_800D154C->speed = (u32)((func_150ADA20() % 20U) + 15);
    D_800D154C->stickspeed = D_800D154C->speed;
    D_800D154C->unk223 = 7;
}

void func_15076F40(void) {
    D_800D154C->unk21C = 1000;
    func_15060F28(D_800D154C, 1);
}

void func_15076F78(void) {
    s8 value;

    value = D_800D1890;
    D_800D154C->unkCC = value;
    value = D_800D1891;
    D_800D154C->unkCE = value;
}

/* arg3 == 0: moves the object by a random offset of up to +-arg0/2 (x) and +-arg1/2 (z), times 3;
 * otherwise raises its speed to at least arg3. */
void func_15076FA8(void) {
    if (D_800D1893 == 0) {
        s16 offset;

        offset = (func_150ADA20() % (u32)D_800D1890) - (D_800D1890 / 2);
        D_800D154C->xpos += offset * 3.0f;
        offset = (func_150ADA20() % (u32)D_800D1891) - (D_800D1891 / 2);
        D_800D154C->zpos += offset * 3.0f;
    } else if (D_800D154C->speed < D_800D1893) {
        D_800D154C->speed = D_800D1893;
    }
}

/* Jumps to arg3 when the target's speed is below arg2. */
void func_150770E4(void) {
    if (movement_obj[D_800D154C->unk222].speed < D_800D1892) {
        func_15075400(D_800D1893);
    }
}

void func_15077174(void) {
    D_800D154C->unk10E = D_800D1890;
}

void func_15077190(void) {
    s32 id = (D_800D1890 << 8) | D_800D1891;

    if (id != 0) {
        s32 param = D_800D1892 << 7;

        func_10010630(id, D_800D154C, param, 500, 2500);
    }
}

void func_150771F0(void) {
    u8 mode;
    u8 param0;
    u8 param1;

    if (D_800D1893 == 0) {
        param0 = D_800D1890;
        param1 = D_800D1891;
        if (D_800D1892 != 0) {
            mode = 1;
        } else {
            mode = 2;
        }
        func_1506160C(D_800D154C, mode, param0, param1, 0);
    } else if (D_800D1892 == 0) {
        func_1502EA60(D_800D154C, D_800D1890);
    } else {
        func_1502EA7C(D_800D154C, D_800D1890);
    }
}

/* Sets the health to arg0 (arg1 == 0) or decrements it (arg1 == 1). */
void func_15077294(void) {
    switch (D_800D1891) {
    case 0:
        D_800D154C->energy = D_800D1890;
        break;
    case 1:
        D_800D154C->energy--;
        break;
    }
}

void func_150772E8(void) {
    func_1503DE70(D_800D154C, D_800D1890, -1);
}

void func_15077318(void) {
    if (D_800D1890 != 0) {
        D_800D154C->unk101 |= 1;
    } else {
        D_800D154C->unk101 &= ~1;
    }
}

void func_1507735C(void) {
}

void func_15077364(void) {
    if (D_800D1893 != 0) {
        D_800D1890 += func_150ADA20() % (u32)D_800D1893;
    }
    D_800D154C->intel_animspd = D_800D1890;
    D_800D154C->unk249 = 0;
    D_800D154C->intel_accel = D_800D1891;
    D_800D154C->intel_decel = D_800D1892;
}

/*
 * arg3 != 0: adds (s16)(arg1:arg2) * D_800BE9E4 to the 13-bit counter kept in unk246 (high bits) and
 * unk249 (low byte), clamping at 0 and setting bit 7 of unk246; otherwise sets unk246 to arg0.
 */
void func_15077404(void) {
    if (D_800D1893 != 0) {
        s16 value;
        s16 delta;

        value = ((D_800D154C->intel_animspd & 0x1F) << 8) + D_800D154C->unk249;
        delta = (D_800D1891 << 8) + D_800D1892;
        value += delta * D_800BE9E4;
        if (value < 0) {
            value = 0;
        }
        D_800D154C->intel_animspd = (value >> 8) | 0x80;
        D_800D154C->unk249 = value;
    } else {
        D_800D154C->intel_animspd = D_800D1890;
    }
}

/* Jumps to arg0 when the object has reached its destination node (or has none). */
void func_150774B4(void) {
    if ((D_800D154C->unk20F == D_800D154C->unk211) || (D_800D154C->unk211 == 0xFF)) {
        D_800D154C->task_attrib |= 0x800;
        func_15075400(D_800D1890);
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
    if (D_800D1890 != 1) {
        if (D_800D154C->unk20F == D_800D154C->unk211) {
            UNK216(D_800D154C) = 0;
            D_800D154C->task_attrib |= 0x800;
            return;
        }
        if (D_800D154C->unk211 == 0xFF) {
            UNK216(D_800D154C) = 0;
        }
    }
    D_800D154C->task_attrib &= ~0x800;
    if (D_800D2108[D_800D154C->listpos] - 1 > 0) {
        hasGoal2 = 0;
        hasGoal = 0;
        if (D_800D154C->unk211 != 0xFF) {
            hasGoal = 1;
        }
        if (D_800D154C->unk212[0] != 0xFF) {
            hasGoal2 = 1;
        }
        current = D_800D154C->unk20F;
        if (current == 0xFF) {
            if (D_800D1890 == 1) {
                hasGoal2 = 0;
                hasGoal = 0;
                next = D_800D154C->unk210;
            }
            if (next == 0xFF) {
                next = func_15086098(D_800D154C->xpos, D_800D154C->ypos, D_800D154C->zpos,
                                     hasGoal, hasGoal2, &D_800D154C->unk211, D_800D154C->unk212);
            }
        } else if (current != 0xFF) {
            if (D_800D1890 == 1) {
                best = 0xFF;
                node = &D_800D2350[current];
                if (D_800D154C->unk210 != 0xFF) {
                    // matching: dead code standing in for something the original compiled away; it
                    // keeps IDO's branch on pad210 and its register assignment
                    tmp = 0; for (i = 0, q = (u8 *)D_800D2350; i < 4; i++, q++) { tmp += q[9] == D_800D154C->unk210; } tmp += (s32)node; tmp ^= (s32)node; tmp -= (s32)node; tmp |= (s32)node;
                }
                bestRoll = 10.0f;
                for (i = 0; i < 5; i++) {
                    if (node->links[i] != 0xFF && D_800D154C->unk210 != node->links[i]) {
                        roll = func_150ADA68();
                        if (roll < bestRoll) {
                            bestRoll = roll;
                            best = node->links[i];
                        }
                    }
                }
                next = best;
                if (next == 0xFF) {
                    D_800D154C->task_attrib |= 0x800;
                    next = D_800D154C->unk210;
                }
            } else {
                next = func_15086364(current, hasGoal, hasGoal2, &D_800D154C->unk211, D_800D154C->unk212);
            }
        }
        if (next != 0xFF) {
            D_800D154C->unk21E = 0;
            if (func_15086CBC(next, &x, &y, &z)) {
                UNK216(D_800D154C) = D_800D2354;
                D_800D154C->unk210 = D_800D154C->unk20F;
                D_800D154C->unk20F = next;
                {
                    f32 width;
                    f32 dx;
                    f32 dz;
                    f32 dist;

                    width = (f32)(u32)D_800D2350[next].unk8 + (f32)(u32)D_800D2350[next].unk8;
                    dx = x - D_800D154C->xpos;
                    dz = z - D_800D154C->zpos;
                    dist = sqrtf(dx * dx + dz * dz);
                    if (dist > 1.0f) {
                        dist = func_150ADA68() * width / dist;
                        x -= dz * dist;
                        z += dx * dist;
                    }
                }
                D_800D2104[D_800D154C->listpos][1].x = x;
                D_800D2104[D_800D154C->listpos][1].y = y;
                D_800D2104[D_800D154C->listpos][1].z = z;
            }
        }
    }
}

/* Advances the waypoint index by unk221, wrapping around the path (or the first unk21F waypoints). */
void func_150778F0(void) {
    u8 count;

    count = D_800D2108[D_800D154C->listpos] - 1;
    if (D_800D154C->unk21F != 0) {
        count = D_800D154C->unk21F;
    }
    D_800D154C->unk21E += D_800D154C->unk221;
    D_800D154C->unk21E += count;
    D_800D154C->unk21E %= count;
    if (D_800D154C->unk21E < D_800D154C->unk220) {
        D_800D154C->unk21E = D_800D154C->unk220;
    }
}

void func_150779A8(void) {
    func_15075650();
    D_800D154C->unk223 = 11;
}

/* Jumps to arg0 when object 0 (or, with arg2, the target) is within arg3 * 8 units. */
void func_150779D4(void) {
    Actor *obj;
    u8 index;
    f32 range;

    index = 0;
    if (D_800D1892 != 0) {
        index = D_800D154C->unk222;
    }
    if (dc_glbl_CharNum != index) {
        obj = &movement_obj[index];
        if ((obj->type != 1) || (obj->attach_char == 0)) {
            // matching: the assignment inside the test makes IDO keep the bound in $f2
            if (func_1505A6F8(D_800D154C, obj) < (range = D_800D1893 << 3)) {
                func_15075400(D_800D1890);
            }
        }
    }
}

void func_15077AA0(void) {
    D_800D154C->intelhitroute = D_800D1890;
}

void func_15077ABC(void) {
    D_800D154C->unk258 = D_800D1890;
    D_800D154C->unk257 = D_800D1891;
    D_800D154C->unk86 = func_150ADA20() % 255U;
}

void func_15077B14(void) {
    D_800D154C->unk24C = D_800D1890;
    D_800D154C->unk24D = D_800D1891;
}

void func_15077B44(void) {
    func_15060A30((D_800D1890 << 8) + D_800D1891, D_800D154C);
}

void func_15077B80(void) {
    s32 flags = (D_800D1890 << 8) + D_800D1891;

    D_800D154C->task_attrib |= flags;
}

/* Restarts the object returned by func_1505F0AC(arg1) on script arg0. */
void func_15077BB4(void) {
    Actor *obj = func_1505F0AC(D_800D1891);

    obj->scriptCmd = NULL;
    obj->intelwalk = D_800D1890;
}

void func_15077BE4(void) {
    s32 mask = ~((D_800D1890 << 8) + D_800D1891);

    D_800D154C->task_attrib &= mask;
}

void func_15077C1C(void) {
    D_800D154C->unk23D = D_800D1890;
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
    D_800D154C->unk222 = 0;

    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if (((obj->type != 0) && (obj->energy != 0)) &&
            ((obj->imune == 0) || (obj->imune == 0xFF)) &&
            ((obj->attach_char == 0) || (D_800D1892 != 0)) &&
            ((obj->character_type == D_800D1890) || ((obj->type == D_800D1891) && (obj->stunned == 0)))) {
            if (D_800D1893 == 0) {
                D_800D154C->unk222 = i;
                return;
            }
            dist = func_1505A72C(D_800D154C, obj);
            if (nearest > dist) {
                D_800D154C->unk222 = i;
                nearest = dist;
            }
        }
    }
}

void func_15077DA0(void) {
    D_800D154C->unk21E = D_800D1890;
}

/* Moves the object onto waypoint arg0 (0xFA: the current waypoint). */
void func_15077DBC(void) {
    if (D_800D1890 != 0xFA) {
        D_800D154C->unk21E = D_800D1890;
    }
    D_800D154C->xpos = D_800D2104[D_800D154C->listpos][D_800D154C->unk21E + 1].x;
    D_800D154C->ypos = D_800D2104[D_800D154C->listpos][D_800D154C->unk21E + 1].y;
    D_800D154C->zpos = D_800D2104[D_800D154C->listpos][D_800D154C->unk21E + 1].z;
}

void func_15077E9C(void) {
    s32 id = (D_800D1890 << 8) + D_800D1891;

    func_10012718(id, D_800D154C, 24000, 500, 2500);
}

void func_15077EEC(void) {
    D_800D154C->unk254 = D_800D1890;
}

void func_15077F08(void) {
    D_800D154C->unk1EA = (D_800D1890 << 8) | D_800D1891;
}

void func_15077F34(void) {
    D_800D154C->unk24F = D_800D1890;
    D_800D154C->unk250 = D_800D1891;
}

/* Turns the object towards the current waypoint, starting turn animation arg0 - 1 for large turns. */
void func_15077F64(void) {
    f32 dx;
    f32 dz;
    u8 turn;
    u8 index;

    index = D_800D1890 - 1;
    dx = (f32)D_800D2104[D_800D154C->listpos][D_800D154C->unk21E + 1].x - D_800D154C->xpos;
    dz = D_800D154C->zpos - (f32)D_800D2104[D_800D154C->listpos][D_800D154C->unk21E + 1].z;
    D_800D154C->fangle = func_1505A630(dx, dz, 0);
    turn = (D_800D154C->fangle - D_800D154C->angle) >> 8;
    if (turn & 0x80) {
        turn = -turn;
    }
    if (D_80099A3C[index][0] < turn) {
        D_800D154C->unk250 = D_800D1891;
        func_15056A00(D_800D154C, turn, index);
    }
}

/* Jumps to arg0 when any other active object is within arg3 * 8 units. */
void func_15078074(void) {
    f32 range;
    Actor *obj;
    s32 i;

    range = 8 * D_800D1893;

    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if ((obj->type != 0) && (dc_glbl_CharNum != i)) {
            if (func_1505A6F8(D_800D154C, obj) < range) {
                func_15075400(D_800D1890);
                return;
            }
        }
    }
}

/* Jumps to arg0 when the target is inactive. */
void func_1507813C(void) {
    s32 index = D_800D154C->unk222;

    if (movement_obj[index].type == 0) {
        func_15075400(D_800D1890);
    }
}

void func_150781A4(void) {
    D_800D154C->unk23F = D_800D1891;
    D_800D154C->unk240 = D_800D1892;
    D_800D154C->unk241 = D_800D1890;
    D_800D154C->unk242 = D_800D1893;
}

/* Jumps to arg0 when the object is closer (arg2 == 0) or farther (arg2 != 0) than arg3 * 8 from waypoint arg1. */
void func_150781F4(void) {
    PathPoint *point;
    f32 dx;
    f32 dz;
    f32 dist;
    s32 index;

    index = D_800D1891 + 1;
    point = &D_800D2104[D_800D154C->listpos][index];
    dx = point->x - D_800D154C->xpos;
    dz = point->z - D_800D154C->zpos;
    dist = sqrtf((dx * dx) + (dz * dz));
    dx = (f32)(D_800D1893 << 3);
    if (((D_800D1892 == 0) && (dist < dx)) ||
        ((D_800D1892 != 0) && (dist > dx))) {
        func_15075400(D_800D1890);
    }
}

void func_150782CC(void) {
    D_800D154C->unk23E = D_800D1890;
}

/* Jumps to arg0 when the object's unique id is (arg2 == 0) or is not (arg2 == 1) arg1. */
void func_150782E8(void) {
    if (((D_800D1892 == 0) && (D_800D154C->unique_id == D_800D1891)) ||
        ((D_800D1892 == 1) && (D_800D154C->unique_id != D_800D1891))) {
        func_15075400(D_800D1890);
    }
}

/* Targets the object with unique id arg0 (object 0 if there is none). */
void func_15078358(void) {
    s32 index = func_15083FB0(D_800D1890);

    D_800D154C->unk222 = index;
    if (index == -1) {
        D_800D154C->unk222 = 0;
    }
}

/*
 * Jumps to arg0 when the distance to the target (arg2: 0 func_1505A6F8, 1 func_1505A72C, else the
 * height difference) is below (arg1 == 0) or above (arg1 == 1) arg3 * 8 (0xFF: unk23D * 8).
 */
void func_1507839C(void) {
    Actor *target;
    f32 dist;
    f32 threshold;

    if (D_800D1892 == 0) {
        target = &movement_obj[D_800D154C->unk222];
        dist = func_1505A6F8(D_800D154C, target);
    } else if (D_800D1892 == 1) {
        target = &movement_obj[D_800D154C->unk222];
        dist = func_1505A72C(D_800D154C, target);
    } else {
        dist = fabsf(D_800D154C->ypos - movement_obj[D_800D154C->unk222].ypos);
    }

    threshold = D_800D1893 * 8;
    if (D_800D1893 == 0xFF) {
        threshold = D_800D154C->unk23D * 8;
    }
    if (((D_800D1891 == 0) && (dist < threshold)) ||
        ((D_800D1891 == 1) && (threshold < dist))) {
        func_15075400(D_800D1890);
    }
}

void func_15078520(void) {
    func_15075400(D_800D1890);
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

    mode = D_800D1890 >> 6;
    D_800D1890 &= 0x3F;
    x = movement_obj[D_800D154C->unk222].xpos;
    z = movement_obj[D_800D154C->unk222].zpos;
    y = movement_obj[D_800D154C->unk222].ypos;
    offsetX = ((((s8)D_800D1891 << 4) | (D_800D1893 >> 4)) * 8);
    offsetZ = ((((s8)D_800D1892 << 4) | (D_800D1893 & 0xF)) * 8);
    if (mode == 0) {
        facing = ((movement_obj[D_800D154C->unk222].langle >> 8) & 0xFF) - 0x40;
        sine = func_15048A40(facing);
        cosine = func_150489B0(facing);
    } else if (mode == 1) {
        sine = 0.0f;
        cosine = 1.0f;
    } else {
        angle = func_150484A0(D_800D154C->xpos - x, D_800D154C->zpos - z);
        sine = sinf(angle);
        cosine = cosf(angle);
    }
    x += offsetX * cosine + offsetZ * sine;
    z += offsetZ * cosine - offsetX * sine;
    D_800D2104[D_800D154C->listpos][D_800D1890 + 1].x = x;
    D_800D2104[D_800D154C->listpos][D_800D1890 + 1].y = y;
    D_800D2104[D_800D154C->listpos][D_800D1890 + 1].z = z;
}

/* Jumps to arg0 when the target's unk2D0->unk8 is below (arg2 == 0) or above (arg2 == 1) arg1. */
void func_1507879C(void) {
    AnimInstance *data;
    f32 value;
    s32 compare;

    data = movement_obj[D_800D154C->unk222].unk2D0;
    value = data->frame;
    compare = D_800D1892;
    if (((compare == 0) && (value < (f32)(u32)D_800D1891)) ||
        ((compare == 1) && ((f32)(u32)D_800D1891 < value))) {
        func_15075400(D_800D1890);
    }
}

void func_15078874(void) {
    D_800D154C->unk251 = D_800D1890;
}

/* Jumps to arg0 when unk251 is (arg2 == 0) or is not (arg2 == 1) arg1. */
void func_15078890(void) {
    if (((D_800D1892 == 0) && (D_800D154C->unk251 == D_800D1891)) ||
        ((D_800D1892 == 1) && (D_800D154C->unk251 != D_800D1891))) {
        func_15075400(D_800D1890);
    }
}

/* arg3: jumps to arg0 when the target is object 0; else when the target's unk251 is (arg2 == 0) or is not (arg2 == 1) arg1. */
void func_15078900(void) {
    if (D_800D1893 != 0) {
        if (D_800D154C->unk222 == 0) {
            func_15075400(D_800D1890);
        }
    } else if (((D_800D1892 == 0) && (D_800D1891 == movement_obj[D_800D154C->unk222].unk251)) ||
               ((D_800D1892 == 1) && (D_800D1891 != movement_obj[D_800D154C->unk222].unk251))) {
        func_15075400(D_800D1890);
    }
}

/* Copies unk8 and unkC of the target's unk2D0 data into the object's own. */
void func_15078A08(void) {
    AnimInstance *dst;
    AnimInstance *src;

    dst = D_800D154C->unk2D0;
    src = movement_obj[D_800D154C->unk222].unk2D0;
    dst->frame = src->frame;
    dst->prevFrame = src->prevFrame;
}

/*
 * Script condition: reads property arg2 of object `index` (or of object 0 / the target for some)
 * and compares it with arg3 using operator arg1 (0 ==, 1 !=, 2 <, 3 >, 4 <=, 5 >=).
 */
s32 func_15078A60(s32 index) {
    s32 value;
    s32 diff;
    s32 pad; // matching: unused stack slot
    u8 facing;

    switch (D_800D1892) {
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
        diff = (u8)(func_150484A0(movement_obj[index].xpos - D_800D154C->xpos,
                                  movement_obj[index].zpos - D_800D154C->zpos) * 40.74366379f) - facing;
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
        value = movement_obj[D_800D154C->unk222].character_type;
        break;
    case 16:
        value = movement_obj[index].energy;
        break;
    case 17:
        value = movement_obj[D_800D154C->unk222].intelwalk;
        break;
    case 18:
        value = movement_obj[index].norun;
        break;
    }
    // matching: one line, as -g3 line numbers steer the scheduling of this test
    if (((D_800D1891 == 0) && (value == D_800D1893)) || ((D_800D1891 == 1) && (value != D_800D1893)) || ((D_800D1891 == 2) && (value < D_800D1893)) || ((D_800D1891 == 3) && (value > D_800D1893)) || ((D_800D1891 == 4) && (value <= D_800D1893)) || ((D_800D1891 == 5) && (value >= D_800D1893))) { return 1; } return 0;
}

void func_1507900C(void) {
    if (func_15078A60(0) != 0) {
        func_15075400(D_800D1890);
    }
}

void func_1507903C(void) {
    if (func_15078A60(0) != 0) {
        D_800D154C->scriptCmd = func_1507BB28(0, D_800D1890);
        D_800D154C->scriptCmd--;
    }
}

void func_15079090(void) {
    if (func_15078A60(dc_glbl_CharNum) != 0) {
        func_15075400(D_800D1890);
    }
}

void func_150790C4(void) {
    if (func_15078A60(dc_glbl_CharNum) != 0) {
        D_800D154C->scriptCmd = func_1507BB28(0, D_800D1890);
        D_800D154C->scriptCmd--;
    }
}

/* Jumps to arg0 when the y (arg3 & 2: x) of object 0 (arg3 & 1: this object) is below (s16)(arg1:arg2). */
void func_1507911C(void) {
    s16 limit;
    s16 coord;
    u8 index;

    limit = (D_800D1891 << 8) | D_800D1892;
    index = D_800D1893 & 1;
    if (index != 0) {
        index = dc_glbl_CharNum;
    }
    coord = movement_obj[index].ypos;
    if (D_800D1893 & 2) {
        coord = movement_obj[index].xpos;
    }
    if (coord < limit) {
        func_15075400(D_800D1890);
    }
}

void func_150791F0(void) {
    if (D_800D154C->modelCount < D_800D1890) {
        D_800D154C->forced_lod = 0xFF;
    } else {
        D_800D154C->forced_lod = D_800D1890;
    }
}

/* Calls func_1505D024 with the direction from gate unk251 to the object (never 0) and holds for a tick. */
void func_15079228(void) {
    Actor *obj;
    TriggerZone *gate;
    f32 x;
    f32 z;
    u16 angle;

    // matching: the (no-op) load through the variable's address orders the address computation as the original
    gate = &(*(TriggerZone **)&zones)[D_800D154C->unk251];
    obj = D_800D154C;
    x = gate->x;
    z = gate->z;
    angle = func_1505A630(obj->xpos - x, z - obj->zpos, 0);
    if (angle == 0) {
        angle = 1;
    }
    func_1505D024(D_800D154C, D_800D1890, angle, -1);
    D_800D154C->scriptCmd--;
    D_800D154C->unk21C = 1;
}

void func_150792E0(void) {
    D_800D154C->intelwalk = D_800D1890;
}

void func_150792FC(void) {
    D_800D154C->unk2F8 &= ~7;
    D_800D154C->unk2F8 |= D_800D1890;
}

/* Restarts the object found by func_150768DC on script arg0. */
void func_15079334(void) {
    Actor *found;

    if (D_800D154C->unkA8 != 0) {
        found = &movement_obj[D_800D154C->unkA8 & 0x7F];
        found->scriptCmd = NULL;
        found->intelwalk = D_800D1890;
    }
}

void func_15079390(void) {
    u16 param0 = D_800D1890;
    s32 param1 = D_800D1891;
    s32 param2 = D_800D1892;

    func_1514D3B0(D_800D154C, param0, param1, param2);
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

    x = (s16)(s32)movement_obj[0].xpos;
    z = (s16)(s32)movement_obj[0].zpos;
    if ((x < -1180) || (x > 1180) || (z < -1180) || (z > 1180)) {
        x = 0;
        z = 0;
    }

    func_1505A184(func_150ADA20(), 550.0f, 0.0f, &offsetX, &offsetZ, &third);

    x = (s16)(s32)((f32)x + offsetX);
    z = (s16)(s32)((f32)z + offsetZ);
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

    D_800D2104[D_800D154C->listpos][D_800D1890 + 1].x = x;
    D_800D2104[D_800D154C->listpos][D_800D1890 + 1].z = z;
}

/* Sets the speeds for a jump that lands on the target. */
void func_15079570(void) {
    f32 dist;
    f32 time;

    dist = func_1505A6F8(D_800D154C, &movement_obj[D_800D154C->unk222]);
    D_800D154C->stickspeed = D_800D154C->speed;
    time = 2.0f * (dist / D_800D154C->stickspeed);
    D_800D154C->yspd = (D_800D154C->ygrav * time) * 0.5f;
    D_800D154C->yspd += 2.0f * ((movement_obj[D_800D154C->unk222].ypos - D_800D154C->ypos) / time);
}

/* Restarts the object with unique id arg0 on script arg1. */
void func_1507965C(void) {
    s32 index;
    Actor *obj;

    index = func_15083FB0(D_800D1890);
    if (index != -1) {
        obj = &movement_obj[index];
        obj->intelwalk = D_800D1891;
        obj->scriptCmd = NULL;
        obj->unk23A = 0;
    }
}

/* arg2: sets `immune` of the object with unique id arg0 to arg1; else jumps to arg1 when that object is stunned. */
void func_150796CC(void) {
    s32 index = func_15083FB0(D_800D1890);

    if (D_800D1892 != 0) {
        if (index != -1) {
            movement_obj[index].imune = D_800D1891;
        }
    } else if ((index != -1) && (movement_obj[index].stunned != 0)) {
        func_15075400(D_800D1891);
    }
}

/* arg2: removes the object (type 0xFF); else makes it type 0x3A placed randomly within 250 of its path's centre. */
void func_15079790(void) {
    s16 offsetX;
    s16 offsetZ;

    if (D_800D1892 != 0) {
        D_800D154C->character_type = 0xFF;
    } else {
        D_800D154C->character_type = 0x3A;
        offsetX = (func_150ADA20() % 500U) - 250;
        offsetZ = (func_150ADA20() % 500U) - 250;
        D_800D154C->xpos = D_800D2104[D_800D154C->listpos][0].x + offsetX;
        D_800D154C->zpos = D_800D2104[D_800D154C->listpos][0].z + offsetZ;
    }
}

/* Starts a move with object 0's speed plus (s8)arg2, clamped to [arg1, 250]. */
void func_15079880(void) {
    s32 speed = movement_obj[0].speed + (s8)D_800D1892;

    if (speed < D_800D1891) {
        speed = D_800D1891;
    } else if (speed > 250) {
        speed = 250;
    }
    D_800D1892 = speed;
    func_15075548();
}

/* Jumps to arg0 when the object is near its current waypoint (see func_150781F4). */
void func_150798F8(void) {
    D_800D1891 = D_800D154C->unk21E;
    func_150781F4();
}

void func_15079928(void) {
    u16 timer;

    timer = (D_800D1891 << 16) + D_800D1892;
    if (timer == 0) {
        timer = 0xFFFF;
    }
    func_1507E7E4(D_800D154C, D_800D1890, D_800D1893, timer, 0);
}

void func_15079988(void) {
    func_1507EB4C(D_800D154C, D_800D1890);
}

void func_150799B4(void) {
    s32 id = (D_800D1890 << 8) + D_800D1891;

    if (D_800D1892 != 0) {
        id += func_150ADA20() % (u32)D_800D1892;
    }
    func_15060A30(id, D_800D154C);
}

void func_15079A28(void) {
    D_800D154C->unk252 = D_800D1890;
    D_800D154C->unk253 = D_800D1891;
}

void func_15079A58(void) {
    D_800D2110[D_800D154C->listpos] = (D_800D1890 << 8) + D_800D1891;
}

/* Returns the direction from object `arg0` to the centre of the current object's path. */
u16 func_15079A98(s32 arg0) {
    PathPoint *path;
    Actor *obj;

    path = D_800D2104[D_800D154C->listpos];
    obj = &movement_obj[arg0];
    return func_1505A630(path[0].x - obj->xpos, obj->zpos - path[0].z, 0);
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

    select = D_800D1891;
    if (select == 0) {
        // matching: the jump into the switch keeps arg1 == 0 out of the jump table
        goto current;
    }
select_object: // matching: unused label, splits the block for the scheduler
    switch (D_800D1891) {
    case 1:
    current:
        index = dc_glbl_CharNum;
        break;
    default:
        index = func_15083FB0((u8)(D_800D1891 - 2));
        if (index == -1) {
            return;
        }
        break;
    }
    switch (D_800D1892) {
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
        // matching: the block-scope redeclaration changes how IDO schedules the address of D_800D154C
        extern Actor *D_800D154C;

        // restart the object on script arg0, running as the current object meanwhile
        savedObj = D_800D154C;
        savedIndex = dc_glbl_CharNum;
        D_800D154C = &movement_obj[index];
        dc_glbl_CharNum = index;
        if (D_800D154C->intelwalk != D_800D1890) {
            D_800D154C->intelwalk = D_800D1890;
            D_800D154C->scriptCmd = func_1507BB28(0, D_800D154C->intelwalk);
            D_800D154C->unk21C = 0;
        }
        dc_glbl_CharNum = savedIndex;
        D_800D154C = savedObj;
        return;
    }
    case 6:
        movement_obj[index].unkC8 = D_800D1890;
        return;
    case 7:
        movement_obj[index].vtilt = 0.0f;
        return;
    case 8:
        angle = func_15079A98(D_800D154C->unk222);
        if ((func_15079A98(dc_glbl_CharNum) - angle) & 0x8000) {
            D_800D154C->unk221 = D_800D1890;
        }
        return;
    case 9: {
        u16 value = (D_800D1893 << 8) | D_800D1890;

        D_800D154C->intel_animseq = value;
        return;
    }
    case 10:
        D_800D154C->unk236 = D_800D154C->unk23D + (s8)D_800D1890;
        return;
    case 11:
        D_800D154C->unk22A = D_800D1890;
        return;
    }
    switch (D_800D1893) {
    case 0:
        *counter = D_800D1890;
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
    D_800D154C->speedup = D_800D1898[D_800D1890];
}

void func_15079F50(void) {
    D_800D154C->unk23B = D_800D1890;
}

void func_15079F6C(void) {
    u16 high;
    u16 low;

    high = D_800D1890 << 8;
    low = D_800D1891;
    // matching: the stacked no-op masks raise the expression weight so IDO keeps `low` out of v1
    D_800D154C->unk224 = (high | (((((((((low & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF)) & 0xFFFFu;
    D_800D154C->unk22B = D_800D1892;
    D_800D154C->unk226 = D_800D1893;
}

/*
 * Follows object 0: when it is unlinked (unk65 == 0) or slower than 5, switches to script arg1;
 * otherwise starts a mode-3 move in its unk4C direction at its speed (times 0.7 unless chased).
 */
void func_15079FBC(void) {
    s16 speed;

    if ((movement_obj[0].attach_char == 0) || (movement_obj[0].stickspeed < 5.0f)) {
        D_800D154C->intelwalk = D_800D1891;
        D_800D154C->scriptCmd = func_1507BB28(0, D_800D154C->intelwalk);
        D_800D154C->scriptCmd--;
        return;
    }

    D_800D154C->fangle = movement_obj[0].mv->press_angle;

    if (movement_obj[0].mv->chased) {
        speed = movement_obj[0].stickspeed;
    } else {
        speed = movement_obj[0].stickspeed * 0.7f;
    }
    if (speed < 17) {
        D_800D1893 += 1;
        D_800D1890 = 8;
    }
    D_800D1892 = speed;
    func_15075F40();
    D_800D154C->mvmt_control |= 0x40;
}

/* Sets coordinate arg1 (0 x, 1 y, 2 z) of waypoint arg0 to (s16)(arg2:arg3). */
s32 func_1507A100(void) {
    s32 value = ((s8)D_800D1892 << 8) | D_800D1893;

    ((s16 *)&(D_800D2104[D_800D154C->listpos] + 1)[D_800D1890])[D_800D1891] = value; // matching: waypoints start one point past the centre
    return value;
}

/* Sets the speed to object 0's speed plus (s8)arg2, clamped to [arg1, arg0]. */
void func_1507A164(void) {
    s32 speed = movement_obj[0].speed + (s8)D_800D1892;

    if (speed < D_800D1891) {
        speed = D_800D1891;
    } else if (speed > D_800D1890) {
        speed = D_800D1890;
    }
    D_800D154C->stickspeed = speed;
    if (D_800D154C->stickspeed == 1.0f) {
        D_800D154C->stickspeed = 0.5f;
    }
}

void func_1507A210(void) {
    D_800D154C->unk223 = 0;
    D_800D154C->unk21C = D_800D1890 * 100;
    D_800D154C->unk22C &= 0xFD;
    func_15075498();
}

void func_1507A270(void) {
    func_1503DE70(D_800D154C, D_800D1890, (s8)D_800D1891);
}

void func_1507A2A4(void) {
    D_800D1893 = (func_150ADA20() % (u32)D_800D1891) + D_800D1893;
    func_15075CA0();
}

/* Targets a random active object of type arg0, saving the previous target in unkA8. */
void func_1507A2F8(void) {
    s32 i;
    u32 count;
    u8 candidates[25];

    count = 0;
    for (i = 0; i < 25; i++) {
        if ((movement_obj[i].type != 0) && (movement_obj[i].character_type == D_800D1890)) {
            candidates[count] = i;
            count++;
        }
    }
    if (count != 0) {
        i = func_150ADA20();
        D_800D154C->unkA8 = D_800D154C->unk222;
        D_800D154C->unk222 = candidates[i % count];
    }
}

void func_1507A3B4(void) {
    D_800D154C->unk222 = D_800D154C->unkA8;
}

void func_1507A3CC(void) {
    D_800D154C->unk229 = D_800D1890;
}

/* Returns the four argument bytes as one big-endian word. */
s32 func_1507A3E8(void) {
    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    return ((D_800D1890 & 0xFF) << 24) | ((D_800D1891 & 0xFF) << 16) | ((D_800D1892 & 0xFF) << 8) | (D_800D1893 & 0xFF);
}

void func_1507A428(void) {
    s32 flags;

    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    flags = ((D_800D1890 & 0xFF) << 24) | ((D_800D1891 & 0xFF) << 16) | ((D_800D1892 & 0xFF) << 8) | (D_800D1893 & 0xFF);
    D_800D154C->unk94 = ~(flags | 1);
}

void func_1507A47C(void) {
    s32 flags;

    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    flags = ((D_800D1890 & 0xFF) << 24) | ((D_800D1891 & 0xFF) << 16) | ((D_800D1892 & 0xFF) << 8) | (D_800D1893 & 0xFF);
    D_800D154C->unk94 &= ~flags;
}

void func_1507A4D4(void) {
    s32 flags;

    // matching: the `& 0xFF` masks (no-ops on u8) steer IDO's register allocation
    flags = ((D_800D1890 & 0xFF) << 24) | ((D_800D1891 & 0xFF) << 16) | ((D_800D1892 & 0xFF) << 8) | (D_800D1893 & 0xFF);
    D_800D154C->unk94 |= flags;
}

/*
 * Waypoint direction: arg0 0 sets the step unk221 to arg1, 1 reverses it, 2 reverses it and moves
 * the waypoint index by the new step plus arg3 further, wrapping at arg2 (0: the path's count).
 */
void func_1507A528(void) {
    s32 count;
    s32 index;

    if (D_800D1890 == 0) {
        D_800D154C->unk221 = D_800D1891;
    } else if (D_800D1890 == 1) {
        D_800D154C->unk221 = -D_800D154C->unk221;
    } else if (D_800D1890 == 2) {
        if (D_800D1892 != 0) {
            count = D_800D1892;
        } else {
            count = D_800D2108[D_800D154C->listpos] - 1;
        }
        D_800D154C->unk221 = -D_800D154C->unk221;
        index = D_800D154C->unk21E + D_800D154C->unk221;
        if (D_800D154C->unk221 > 0) {
            index += D_800D1893;
        } else {
            index -= D_800D1893;
        }
        if (index >= count) {
            index -= count;
        } else if (index < 0) {
            index += count;
        }
        D_800D154C->unk21E = index;
    }
}

/*
 * Jumps to arg0 when the angle (arg1: the object's unk78, else its facing relative to the
 * direction to the target), in 1/256 turns, lies in [arg2, arg3).
 */
void func_1507A620(void) {
    u8 index;
    u16 angle;

    index = D_800D154C->unk222;
    if (D_800D1891 != 0) {
        angle = D_800D154C->fangle >> 8;
    } else {
        angle = (D_800D154C->langle - func_1505A630(movement_obj[index].xpos - D_800D154C->xpos,
                                                   D_800D154C->zpos - movement_obj[index].zpos, 0)) >> 8;
    }
    if ((u8)(angle - D_800D1892) < (u8)(D_800D1893 - D_800D1892)) {
        func_15075400(D_800D1890);
    }
}

s32 func_1507A6FC(s32 arg0) {
    return D_800BE748[arg0].button;
}

/* Jumps to arg3 when entry arg0 of D_800BE748 has all the bits of (arg1:arg2) set. */
void func_1507A71C(void) {
    u16 bits = func_1507A6FC(D_800D1890);
    u16 mask = (D_800D1891 << 8) | D_800D1892;

    if ((bits & mask) == mask) {
        func_15075400(D_800D1893);
    }
}

void func_1507A774(void) {
    u16 value = (D_800D1891 << 8) | D_800D1892;

    if (level == value) {
        func_15075400(D_800D1890);
    }
}

void func_1507A7C0(void) {
    D_800D154C->norun = D_800D1890;
}

void func_1507A7DC(void) {
    func_150836CC(D_800D154C, D_800D1890);
}

void func_1507A808(void) {
    D_800D154C->unk22C = (D_800D154C->unk22C & D_800D1890) | D_800D1891;
}

void func_1507A838(void) {
    if (D_800D154C->energy == D_800D1891) {
        func_15075400(D_800D1890);
    }
}

void func_1507A878(void) {
    func_1512D748(0, D_800D1890, D_800D1891);
}

void func_1507A8A8(void) {
    if ((movement_obj[0].attach_char != 0) || (D_800BE616 != 0)) {
        func_15075400(D_800D1890);
    }
}

/* Jumps to arg0 when the target is (arg2 == 0) or is not (arg2 == 1) of type arg1. */
void func_1507A8EC(void) {
    if (D_800D1891 == movement_obj[D_800D154C->unk222].character_type) {
        D_800D1892 ^= 1;
    }
    if (D_800D1892 != 0) {
        func_15075400(D_800D1890);
    }
}

/* Links the object to the one found by func_15083E90(arg0) (unk65 = its index + 1), or unlinks it (arg0 == 0). */
void func_1507A984(void) {
    Actor *linked;

    if (D_800D1890 != 0) {
        linked = func_15083E90(D_800D1890);
        if (linked != NULL) {
            if (D_800D1892 == 0) {
                D_800D154C->unk101 |= 4;
            }
            if (D_800D1893 != 0) {
                D_800D154C->unk101 |= 0x40;
            }
            D_800D154C->attach_char = (linked - movement_obj) + 1;
            D_800D154C->basemtx = D_800D1891;
        }
    } else {
        D_800D154C->attach_char = 0;
    }
}

/* Sets an angle: arg2 selects the operation, (arg0:arg1) is the operand. */
void func_1507AA48(void) {
    u16 value;
    PathPoint *path;
    s32 angle;

    value = (D_800D1890 << 8) | D_800D1891;
    switch (D_800D1892) {
    case 0:
        D_800D154C->fangle = D_800D154C->angle + value;
        break;
    case 1:
        D_800D154C->fangle = D_800D154C->angle - value;
        break;
    case 2:
        D_800D154C->fangle = value;
        break;
    case 3:
        // matching: the do-while wrapper changes how IDO schedules case 4 below
        do {
            D_800D154C->angle = D_800D154C->fangle;
            D_800D154C->langle = D_800D154C->fangle;
        } while (0);
        break;
    case 4:
        // direction from waypoint arg0 to waypoint arg1, plus arg3 / 256 turns
        path = D_800D2104[D_800D154C->listpos];
        angle = (u16)(func_150484A0(path[D_800D1891 + 1].x - path[D_800D1890 + 1].x, path[D_800D1891 + 1].z - path[D_800D1890 + 1].z) * 10430.37793f) + (D_800D1893 << 8);
        D_800D154C->fangle = angle + 0x4000;
        break;
    case 5:
        D_800D154C->fangle = func_150ADA20();
        break;
    case 6:
        D_800D154C->fangle = D_800D154C->angle;
        break;
    case 7:
        D_800D154C->angle = movement_obj[D_800D154C->unk222].langle + value;
        break;
    case 8:
        D_800D154C->angle = D_800D154C->langle;
        break;
    }
}

void func_1507ACB0(void) {
    D_800D154C->unk220 = D_800D1890;
    D_800D154C->unk21F = D_800D1891;
}

void func_1507ACE0(void) {
    D_800D154C->unk1E5 = D_800D1890;
    D_800D154C->unk1E6 = D_800D1890;
    D_800D154C->unk1E7 = D_800D1891;
    D_800D154C->unk1E8 = D_800D1891;
}

void func_1507AD30(void) {
    D_800D154C->unk255 = D_800D1890;
    D_800D154C->unk256 = D_800D1891;
}

void func_1507AD60(void) {
    D_800D154C->family = D_800D1890;
}

void func_1507AD7C(void) {
    D_800D154C->mvmt_control |= func_1507A3E8();
}

void func_1507ADAC(void) {
    D_800D154C->mvmt_control &= ~func_1507A3E8();
}

/* Splits (angle between unk78 and the facing unk7A) / arg0 into unk1E6 (high byte) and unk1E8 (low byte). */
void func_1507ADE0(void) {
    s32 diff;
    s32 step;
    s32 facing = D_800D154C->langle;
    s32 target = D_800D154C->fangle;

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
    step = diff / (s32)D_800D1890;
    D_800D154C->unk1E6 = step >> 8;
    D_800D154C->unk1E8 = step & 0xFF;
}

void func_1507AE78(void) {
    D_800D154C->unk229 = D_800D1890;
}

void func_1507AE94(void) {
    D_800D154C->unkD2 = (D_800D1890 << 8) | D_800D1891;
    D_800D154C->unkD4 = (D_800D1892 << 8) | D_800D1893;
    D_800D154C->unkD2 *= D_800D154C->scale_xz;
    D_800D154C->unkD4 *= D_800D154C->scale_y;
}

void func_1507AF3C(void) {
    D_800D154C->unkD6 = (D_800D1890 << 8) | D_800D1891;
    D_800D154C->unkD6 *= D_800D154C->scale_y;
}

void func_1507AF98(void) {
    D_800D154C->unkD8 = (D_800D1890 << 8) | D_800D1891;
    D_800D154C->unkDA = (D_800D1892 << 8) | D_800D1893;
    D_800D154C->unkD8 *= D_800D154C->scale_xz;
    D_800D154C->unkDA *= D_800D154C->scale_xz;
}

void func_1507B040(void) {
    D_800D154C->angle = D_800D154C->langle;
}

/* Acts on the object returned by func_15072208: arg0 0 calls func_1506160C, 1 sets and 2 restarts script arg1. */
void func_1507B058(void) {
    Actor *savedObj = D_800D154C;
    s32 savedIndex = dc_glbl_CharNum;

    D_800D154C = func_15072208(D_800D154C, 0);
    dc_glbl_CharNum = D_800D154C - movement_obj;

    if (D_800D154C != NULL) {
        switch (D_800D1890) {
        case 0:
            func_1506160C(D_800D154C, 1, D_800D1891, 0, 0);
            break;
        case 1:
            D_800D154C->intelwalk = D_800D1891;
            /* fallthrough */
        case 2:
            D_800D154C->scriptCmd = func_1507BB28(0, D_800D1891);
            D_800D154C->unk21C = 0;
        }
    }
    dc_glbl_CharNum = savedIndex;
    D_800D154C = savedObj;
}

void func_1507B15C(void) {
    D_800D154C->nojump = D_800D1890;
}

void func_1507B178(void) {
    s16 value = (D_800D1891 << 8) | D_800D1890;

    if (D_800D1893 == 2) {
        D_800D154C->bodyOffsetY = (s16)(value * D_800D154C->scale_y);
    } else if (D_800D1893 == 1) {
        func_15062B50(D_800D154C, value);
    } else {
        func_15062B1C(D_800D154C, value);
    }
}

void func_1507B234(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(300, 1800);
    }
}

void func_1507B280(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(300, 3000);
    }
}

void func_1507B2CC(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(300, 1800);
    }
}

void func_1507B318(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(300, 3000);
    }
}

void func_1507B364(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(700, 4000);
    }
}

void func_1507B3B0(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(700, 4000);
    }
}

void func_1507B3FC(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(3000, 8000);
    }
}

void func_1507B448(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(190, 1300);
    }
}

void func_1507B494(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(190, 1300);
    }
}

void func_1507B4E0(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(100, 800);
    }
}

void func_1507B52C(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(100, 800);
    }
}

void func_1507B578(void) {
    D_800D154C->doing_hit = D_800D1890;
    D_800D154C->unk114 = D_800D1891;
}

/* Jumps to arg0 when mv->context_b is (arg2 == 0) or is not (arg2 == 1) arg1. */
void func_1507B5C4(void) {
    if (D_800D154C->mv->context_b == D_800D1891) {
        D_800D1892 ^= 1;
    }
    if (D_800D1892 != 0) {
        func_15075400(D_800D1890);
    }
}

void func_1507B630(void) {
    if ((D_800D154C->norun == 0) && (D_800D154C->shad == 0.0f)) {
        if (D_800D154C->mv->rt_context_b != 0) {
            D_800D1890 ^= 1;
        }
        if (D_800D1890 != 0) {
            D_800D154C->mv->pressedButtons |= 0x4000;
            D_800D154C->mv->pressedButtonsTimer = 0;
        }
    } else {
        // repeat this command
        D_800D154C->scriptCmd--;
    }
    D_800D154C->unk21C = 100;
}

void func_1507B6E0(void) {
    if ((D_800D154C->norun != 0) || (D_800D154C->shad > 5.0f)) {
        // repeat this command
        D_800D154C->scriptCmd--;
        D_800D154C->unk21C = 100;
    }
}

void func_1507B734(void) {
    u16 value = (D_800D1890 << 8) | D_800D1891;

    if (D_800D1893 != 0) {
        D_800D154C->mv->pressedButtons = value;
        D_800D154C->mv->pressedButtonsTimer = D_800D1892;
    } else {
        D_800D154C->mv->buttons = value;
        D_800D154C->mv->buttonsTimer = D_800D1892;
    }
}

void func_1507B7BC(void) {
    func_15075548();
    D_800D154C->unk223 = 17;
}

/* Jumps to arg0 when the target is (arg1 == 0) or is not (arg1 == 1) stunned or dead. */
void func_1507B7E8(void) {
    Actor *target = &movement_obj[D_800D154C->unk222];

    if ((target->stunned != 0) || (target->energy == 0)) {
        D_800D1891 ^= 1;
    }
    if (D_800D1891 != 0) {
        func_15075400(D_800D1890);
    }
}

/* Calls handler arg0 of D_80086150, preserving the current object. */
void func_1507B884(void) {
    Actor *savedObj;
    s32 savedIndex;
    u8 index;
    void (*handler)(s32);

    index = D_800D1890;
    handler = D_80086150[index];
    if (handler != NULL) {
        savedObj = D_800D154C;
        savedIndex = dc_glbl_CharNum;
        handler(index);
        D_800D154C = savedObj;
        dc_glbl_CharNum = savedIndex;
    }
}

void func_1507B8F4(void) {
    if (D_800BE616 != 0) {
        if ((D_800D1891 != 0) || ((D_800D154C->mv != NULL) && (D_800D154C->mv->bot != 0))) {
            func_15075400(D_800D1890);
        }
    }
}

void func_1507B958(void) {
    D_800D154C->unkB0 = D_800D1890;
}

/*
 * Jumps to arg0 when unk6 of the path record arg3 past the current waypoint (of the linked object,
 * if any) is (arg2 == 0) or is not (arg2 == 1) arg1.
 */
void func_1507B974(void) {
    Actor *obj;

    obj = D_800D154C;
    if (D_800D154C->attach_char != 0) {
        obj = &movement_obj[D_800D154C->attach_char - 1];
    }
    if (D_800D1891 == D_800D2104[obj->listpos][obj->unk21E + D_800D1893].param) {
        D_800D1892 ^= 1;
    }
    if (D_800D1892 != 0) {
        func_15075400(D_800D1890);
    }
}

void func_1507BA48(void) {
    u16 id = (D_800D1892 << 8) | D_800D1893;

    if (D_800D1890 != 0) {
        id += func_150ADA20() % (u32)D_800D1890;
    }
    func_15060A9C(id, D_800D154C);
}

void func_1507BAD0(void) {
    D_800D154C->unkFC = func_1507A3E8();
}

void func_1507BAF8(void) {
    D_800D154C->unk2CC = func_1507A3E8();
}

void func_1507BB20(void) {
}

typedef struct ScriptEntry {
    ObjScriptCmd *script;
    u8 id;
} ScriptEntry;

extern ScriptEntry D_80086A88[]; // default scripts, ended by a NULL script
extern ObjScriptCmd D_8009A120[];    // fallback script

/* Returns script `scriptId` of the current object's type, falling back to the default table. */
ObjScriptCmd *func_1507BB28(s32 arg0, s32 scriptId) {
    ActorAnim *typeData;
    ScriptEntry *table;
    s32 i;

    typeData = D_800D1588[D_800D154C->character_type];
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
void func_1507BC14(Actor *obj) {
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
        obj->scriptCmd = func_1507BB28(0, obj->intelwalk);
        obj->unk21C = 0;
    }

    while (obj->unk21C == 0) {
        if (obj->scriptCmd->op >= 247) {
            // a label: skip its single byte
            obj->scriptCmd = (ObjScriptCmd *)((u8 *)obj->scriptCmd + 1);
        } else {
            D_800D1890 = obj->scriptCmd->args[0];
            D_800D1891 = obj->scriptCmd->args[1];
            D_800D1892 = obj->scriptCmd->args[2];
            D_800D1893 = obj->scriptCmd->args[3];
            D_80086730[obj->scriptCmd->op]();
            obj->scriptCmd++;
        }
    }
}
