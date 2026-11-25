#include <ultra64.h>

#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
#define D_800D2104 D_800D2104_hdr
#define D_80099A3C D_80099A3C_hdr
#include "variables.h"
#undef D_800D2104
#undef D_80099A3C

/*
 * Object script interpreter commands.
 *
 * Each object runs a byte-code script (D_800D154C->unk218). A command is five bytes: an opcode
 * (< 247, an index into D_80086730) and four argument bytes, which the interpreter copies to
 * D_800D1890..D_800D1893 before calling the handler. Bytes >= 247 are one-byte jump labels.
 * The handlers below act on the current object, D_800D154C.
 */

/* One point of a path: s16 world coordinates. */
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u16 unk6;
} PathPoint;

/* A path, D_800D2104[n]: a centre point followed by D_800D2108[n] - 1 waypoints. */
typedef struct {
    PathPoint center;
    PathPoint points[1];
} Path;

extern Path **D_800D2104; // header declares it as s32 *

typedef struct {
    u8 unk0;
    u8 unk1[9];
} Row56A00;

extern Row56A00 D_80099A3C[]; // header declares it as s16[]

s32 func_1505D024(struct127 *arg0, s32 arg1, u16 arg2, s32 arg3);
void func_1504715C(f32 *arg0, struct127 *arg1);
void func_1514B364(f32 *arg0, f32 *arg1, s32 arg2, s32 arg3);
void func_15197A7C(struct127 *arg0);
u8 func_15056A00(struct127 *arg0, u8 arg1, u8 arg2);
struct216 *func_1507BB28(s32 arg0, s32 arg1);

/* Jumps the script: skips `arg0` commands, or for arg0 >= 247 skips forward to label arg0. */
void func_15075400(s32 arg0) {
    u8 label;
    u8 i;

    if (arg0 < 247) {
        D_800D154C->unk218 += arg0;
        return;
    }

    for (i = 0; i < 100; i++) {
        if (D_800D154C->unk218->unk0 < 247) {
            D_800D154C->unk218++;
        } else {
            label = D_800D154C->unk218->unk0;
            D_800D154C->unk218 = (struct216 *)((u8 *)D_800D154C->unk218 + 1);
            if (arg0 == label) {
                // the interpreter steps past the current command after this returns
                D_800D154C->unk218--;
                return;
            }
        }
    }
}

/* Applies the common movement flags of a move command. */
void func_15075498(void) {
    s32 value = D_800D1893 & 0x7F;

    if (value != 0x7F) {
        D_800D154C->unk244 = value;
    }
    D_800D154C->unkF4 &= ~0x143E;
    if (D_800D1890 == 0xFA) {
        D_800D154C->unkF4 |= 0x22;
    } else if (D_800D1890 == 0xFB) {
        D_800D154C->unkF4 |= 4;
    }
    if (D_800D1893 & 0x80) {
        D_800D154C->unkF4 |= 0x10;
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
        D_800D154C->unk44 = speed;
        if (D_800D154C->unk44 == 1.0f) {
            D_800D154C->unk44 = 0.5f;
        }
        if (D_800BE616 != 0) {
            if ((D_800D154C->id == 40) && (D_800D154C->unk31C->unk128 & 1)) {
                D_800D154C->unk44 *= 1.4f;
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
    struct127 *obj;

    index = D_800D2108[D_800D154C->unk13F] - 1;
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
        D_800D154C->unk44 = D_800D1892;
        if (D_800D154C->unk44 == 1.0f) {
            D_800D154C->unk44 = 0.5f;
        }
    }
    if (D_800D154C->unk21E < D_800D154C->unk220) {
        D_800D154C->unk21E = D_800D154C->unk220;
    }
    func_15075498();
}

/* Starts a move towards the current waypoint, arriving in arg1 ticks. */
void func_15075884(void) {
    struct127 *obj;
    f32 dx;
    f32 dz;
    f32 dist;
    f32 speed;

    func_15075548();
    obj = D_800D154C;
    dx = (f32)D_800D2104[obj->unk13F]->points[obj->unk21E].x - obj->x_position;
    dz = (f32)D_800D2104[obj->unk13F]->points[obj->unk21E].z - obj->z_position;
    dist = sqrtf((dx * dx) + (dz * dz));
    dx = (f32)(u32)D_800D1891;
    speed = dist / dx;
    obj->unk44 = speed + speed;
}

/* Jumps to arg0 when waypoint arg1 compares (per arg2) with the waypoint unk221 steps back. */
void func_15075938(void) {
    s32 position;
    s32 desired;
    s32 matched;

    matched = 0;
    position = D_800D154C->unk21E - (s8)D_800D154C->unk221;
    if (position < 0) {
        position += D_800D2108[D_800D154C->unk13F] - 1;
    } else if (position >= D_800D2108[D_800D154C->unk13F] - 1) {
        position -= D_800D2108[D_800D154C->unk13F] - 1;
    }
    desired = D_800D1891 == 0xFF ? D_800D2108[D_800D154C->unk13F] - 2 : D_800D1891;
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
    point = &D_800D2104[D_800D154C->unk13F]->center + D_800D1891;
    dx = point->x - D_800D154C->x_position;
    dz = point->z - D_800D154C->z_position;
    if (fabsf(dx) + fabsf(dz) < 40.0f) {
        D_800D154C->unk21C = 0;
        D_800D154C->xz_velocity = 0.0f;
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
        D_800D154C->y_velocity = (s16)((D_800D1892 << 8) | D_800D1890);
    }
    if (D_800D1893 != 2) {
        D_800D154C->gravity = (s16)(s8)D_800D1891;
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
        D_800D1891 = D_800D154C->unk232;
    }
    if (D_800D1893 != 0) {
        D_800D154C->unk232 = D_800D1893;
    }
    D_800D154C->unk218 = func_1507BB28(0, D_800D1891);
    // the interpreter steps past the current command after this returns
    D_800D154C->unk218--;
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
            D_800D154C->y_velocity = D_800D1890;
        } else {
            D_800D154C->y_velocity = -D_800D1890;
        }
    }
    if (D_800D1891 != 0) {
        D_800D154C->gravity = D_800D1891;
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
    dx = (f32)D_800D2104[D_800D154C->unk13F]->points[D_800D1891].x - D_800D154C->x_position;
    dz = (f32)D_800D2104[D_800D154C->unk13F]->points[D_800D1891].z - D_800D154C->z_position;
    time = 2.0f * (sqrtf((dx * dx) + (dz * dz)) / D_800D154C->unk44);
    if (time < 12.0f) {
        D_800D154C->unk44 *= time * D_8009A140;
        time = 12.0f;
    }
    D_800D154C->unk21C = (u32)(time * 100.0f);
    D_800D154C->y_velocity = D_800D154C->gravity * time * 0.5f;
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

extern void func_151669A0(s32 arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5);

void func_150762D4(void) {
    func_151669A0((s32)D_800D154C->x_position, (s32)(D_800D154C->y_position + 100.0f),
                  (s32)D_800D154C->z_position, 0.45f, 0xFF, 0);
}

void func_15076340(void) {
    if (D_800D154C->unk107 == 0) {
        func_15075400(D_800D1890);
    }
    if (D_800D1891 != 0) {
        D_800D154C->unk107 = 0;
    }
}

void func_15076394(void) {
    D_800D154C->unk236 = D_800D1890;
}

void func_150763B0(void) {
    if (D_800D1890 == 128) {
        D_800D154C->unk1E5 = D_800D154C->unk144->unkF;
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
    D_800D154C->unk239 = D_800D1890;
}

void func_15076500(void) {
    D_800D154C->unk22E = (D_800D1890 << 8) | D_800D1891;
}

/* Sets `immune` to arg0 on the object's target (arg1 != 0) or on the object itself. */
void func_1507652C(void) {
    if (D_800D1891 != 0) {
        D_800CC2D0[D_800D154C->unk124].immune = D_800D1890;
    } else {
        D_800D154C->immune = D_800D1890;
    }
}

void func_1507659C(void) {
    D_800D154C->y_position = D_800D154C->y_position + D_800D1890 * 100;
    if (D_800D1893 != 0) {
        D_800D154C->y_position = 1800.0f;
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

    D_800D154C->unkF8 |= flags;
}

/* Clears the flags given by the four argument bytes in unkF8. */
void func_15076678(void) {
    s32 flags = (D_800D1890 << 24) | (D_800D1891 << 16) | (D_800D1892 << 8) | D_800D1893;

    D_800D154C->unkF8 &= ~flags;
}

void func_150766D0(void) {
    if (((D_800D154C->unk28 > 0.0f) && (D_800D1890 == 0)) ||
        ((D_800D1890 * 50 < D_800D154C->unk28) && (D_800D1890 != 0))) {
        D_800D154C->unk21C = 99;
        D_800D154C->unk218--;
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
        position[0] = D_800D154C->x_position;
        position[1] = -390.0f;
        position[2] = D_800D154C->z_position;
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
    struct127 *target;
    u16 angle;

    target = &D_800CC2D0[D_800D154C->unk222];
    angle = (s32)func_1505A630(target->x_position - D_800D154C->x_position,
                               D_800D154C->z_position - target->z_position, 0);
    if ((((angle >> 8) - (D_800CC2D0[D_800D154C->unk222].unk7A >> 8) + D_800D1891) & 0xFF) < D_800D1891 * 2) {
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
    struct127 *found;
    struct127 *obj;
    f32 nearest;
    s32 i;
    u8 script;
    struct124 *type;
    u8 angle;
    u8 foundIndex;

    nearest = D_800D1890 * 8;
    D_800D154C->unkA8 = 0;
    self = ((s32)D_800D154C - (s32)D_800CC2D0) / (s32)sizeof(struct127);

    for (i = 0; i < 25; i++) {
        obj = &D_800CC2D0[i];
        if ((obj->interaction_state != 0) && (obj->health != 0) && (obj->unkF8 & 0x20) && (i != self)) {
            if ((D_800D1891 != 0) || (obj->id != D_800D154C->id)) {
                if (fabsf(D_800D154C->y_position - obj->y_position) > 50.0f) {
                    continue;
                }
                dist = func_1505A72C(D_800D154C, obj);
                if (dist < nearest) {
                    type = D_800D1C90[D_800D154C->id];
                    viewAngle = type->unk17;
                    angle = func_1505A630(obj->x_position - D_800D154C->x_position,
                                          D_800D154C->z_position - obj->z_position, 0) >> 8;
                    if (((((D_800D154C->unk76 >> 8) + D_800D154C->unk2CA) - angle + (viewAngle / 2)) & 0xFF) < viewAngle) {
                        D_800D154C->unkA8 = i + 0x80;
                        nearest = dist;
                    }
                }
            }
        }
    }

    if (D_800D154C->unkA8 != 0) {
        D_800D154C->unk218 = func_1507BB28(0, D_800D1893);
        D_800D154C->unk218--;
        script = D_800D1892;
        if (script != 0) {
            foundIndex = D_800D154C->unkA8;
            found = &D_800CC2D0[foundIndex & 0x7F];
            found->unk218 = NULL;
            found->unk232 = script;
            found->unk222 = self;
        }
    }
}

void func_15076B5C(void) {
    D_800D154C->unk222 = D_800D154C->unkA8 & 0x7F;
}

void func_15076B78(void) {
    D_800D154C->unk5 = D_800D1890;
}

/* Targets the last object below this one (all 25 if arg1) of type arg0 that is alive, and starts mode 9. */
void func_15076B94(void) {
    s32 i = ((s32)D_800D154C - (s32)D_800CC2D0) / (s32)sizeof(struct127);

    if (D_800D1891) {
        i = 25;
    }
    while (i != 0) {
        if ((D_800D1890 == D_800CC2D0[i - 1].id) && (D_800CC2D0[i - 1].health != 0)) {
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
    D_800D154C->unk76 = D_800D154C->unk78;
}

void func_15076CCC(void) {
    D_800D154C->unk21C = 10000;
    func_15060F28(D_800D154C, 1);
}

void func_15076D04(void) {
    D_800D154C->xz_velocity = D_800D1890;
}

/* Sets the object's scale from two s16 percentages, (arg1:arg0) horizontal and (arg3:arg2) vertical. */
void func_15076D3C(void) {
    f32 scale = 0.01f;
    s16 xz = D_800D1890 | (D_800D1891 << 8);
    s16 y = D_800D1892 | (D_800D1893 << 8);

    D_800D154C->xz_scale = (f32)xz * scale;
    D_800D154C->y_scale = (f32)y * scale;
    D_800D154C->unk154 = D_800D154C->xz_scale;
    D_800D154C->unk158 = D_800D154C->y_scale;
    func_15062BDC(D_800D154C, D_800D154C->xz_scale, D_800D154C->y_scale);
}

void func_15076DF4(void) {
    D_800D154C->interaction_state = D_800D1890;
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
    D_800D154C->y_velocity = (f32)(u32)(func_150ADA20() % 20U) + 55.0f;
    D_800D154C->gravity = 2.0f;
    D_800D154C->unk78 = func_150ADA20() % 0xFFFFU;
    D_800D154C->xz_velocity = (u32)((func_150ADA20() % 20U) + 15);
    D_800D154C->unk44 = D_800D154C->xz_velocity;
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
        D_800D154C->x_position += offset * 3.0f;
        offset = (func_150ADA20() % (u32)D_800D1891) - (D_800D1891 / 2);
        D_800D154C->z_position += offset * 3.0f;
    } else if (D_800D154C->xz_velocity < D_800D1893) {
        D_800D154C->xz_velocity = D_800D1893;
    }
}

/* Jumps to arg3 when the target's speed is below arg2. */
void func_150770E4(void) {
    if (D_800CC2D0[D_800D154C->unk222].xz_velocity < D_800D1892) {
        func_15075400(D_800D1893);
    }
}

void func_15077174(void) {
    D_800D154C->unk10E = D_800D1890;
}

void func_15077190(void) {
    s32 id = (D_800D1890 << 8) | D_800D1891;

    if (id != 0) {
        s32 volume = D_800D1892 << 7;

        func_10010630(id, D_800D154C, volume, 500, 2500);
    }
}

void func_150771F0(void) {
    s32 mode;
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
        D_800D154C->health = D_800D1890;
        break;
    case 1:
        D_800D154C->health--;
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
    D_800D154C->unk246 = D_800D1890;
    D_800D154C->unk249 = 0;
    D_800D154C->unk247 = D_800D1891;
    D_800D154C->unk248 = D_800D1892;
}

/*
 * arg3 != 0: adds (s16)(arg1:arg2) * D_800BE9E4 to the 13-bit counter kept in unk246 (high bits) and
 * unk249 (low byte), clamping at 0 and setting bit 7 of unk246; otherwise sets unk246 to arg0.
 */
void func_15077404(void) {
    if (D_800D1893 != 0) {
        s16 value;
        s16 delta;

        value = ((D_800D154C->unk246 & 0x1F) << 8) + D_800D154C->unk249;
        delta = (D_800D1891 << 8) + D_800D1892;
        value += delta * D_800BE9E4;
        if (value < 0) {
            value = 0;
        }
        D_800D154C->unk246 = (value >> 8) | 0x80;
        D_800D154C->unk249 = value;
    } else {
        D_800D154C->unk246 = D_800D1890;
    }
}

/* Jumps to arg0 when the object has reached its destination node (or has none). */
void func_150774B4(void) {
    if ((D_800D154C->unk20F == D_800D154C->unk211) || (D_800D154C->unk211 == 0xFF)) {
        D_800D154C->unk25C |= 0x800;
        func_15075400(D_800D1890);
    }
}

/* A node of the path-node graph D_800D2350 (same layout as in game_B4080.c). */
typedef struct PathNode {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 layer;
    /* 0x7 */ u8 unk7;
    /* 0x8 */ u8 unk8;
    /* 0x9 */ u8 links[5]; /* neighbouring nodes, 0xFF = none */
    /* 0xE */ u8 type;
    /* 0xF */ u8 param;
} PathNode;

extern PathNode *D_800D2350;
extern s32 D_800D2354;
s32 func_15086098(f32 x, f32 y, f32 z, s8 arg3, s8 arg4, u8 *arg5, u8 *arg6);
s32 func_15086364(s32 arg0, s8 arg1, s8 arg2, u8 *arg3, u8 *arg4);
s32 func_15086CBC(s32 index, f32 *x, f32 *y, f32 *z);

/* the s16 at 0x216 overlaps struct127's s32 unk214 */
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
            D_800D154C->unk25C |= 0x800;
            return;
        }
        if (D_800D154C->unk211 == 0xFF) {
            UNK216(D_800D154C) = 0;
        }
    }
    D_800D154C->unk25C &= ~0x800;
    if (D_800D2108[D_800D154C->unk13F] - 1 > 0) {
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
                next = D_800D154C->pad210;
            }
            if (next == 0xFF) {
                next = func_15086098(D_800D154C->x_position, D_800D154C->y_position, D_800D154C->z_position,
                                     hasGoal, hasGoal2, &D_800D154C->unk211, D_800D154C->unk212);
            }
        } else if (current != 0xFF) {
            if (D_800D1890 == 1) {
                best = 0xFF;
                node = &D_800D2350[current];
                if (D_800D154C->pad210 != 0xFF) {
                    // matching: dead code standing in for something the original compiled away; it
                    // keeps IDO's branch on pad210 and its register assignment
                    tmp = 0; for (i = 0, q = (u8 *)D_800D2350; i < 4; i++, q++) { tmp += q[9] == D_800D154C->pad210; } tmp += (s32)node; tmp ^= (s32)node; tmp -= (s32)node; tmp |= (s32)node;
                }
                bestRoll = 10.0f;
                for (i = 0; i < 5; i++) {
                    if (node->links[i] != 0xFF && D_800D154C->pad210 != node->links[i]) {
                        roll = func_150ADA68();
                        if (roll < bestRoll) {
                            bestRoll = roll;
                            best = node->links[i];
                        }
                    }
                }
                next = best;
                if (next == 0xFF) {
                    D_800D154C->unk25C |= 0x800;
                    next = D_800D154C->pad210;
                }
            } else {
                next = func_15086364(current, hasGoal, hasGoal2, &D_800D154C->unk211, D_800D154C->unk212);
            }
        }
        if (next != 0xFF) {
            D_800D154C->unk21E = 0;
            if (func_15086CBC(next, &x, &y, &z)) {
                UNK216(D_800D154C) = D_800D2354;
                D_800D154C->pad210 = D_800D154C->unk20F;
                D_800D154C->unk20F = next;
                {
                    f32 width;
                    f32 dx;
                    f32 dz;
                    f32 dist;

                    width = (f32)(u32)D_800D2350[next].unk8 + (f32)(u32)D_800D2350[next].unk8;
                    dx = x - D_800D154C->x_position;
                    dz = z - D_800D154C->z_position;
                    dist = sqrtf(dx * dx + dz * dz);
                    if (dist > 1.0f) {
                        dist = func_150ADA68() * width / dist;
                        x -= dz * dist;
                        z += dx * dist;
                    }
                }
                D_800D2104[D_800D154C->unk13F]->points[0].x = x;
                D_800D2104[D_800D154C->unk13F]->points[0].y = y;
                D_800D2104[D_800D154C->unk13F]->points[0].z = z;
            }
        }
    }
}

extern struct127 *D_800D154C;
extern u8 *D_800D2108;
void func_150778F0(void)
{
  u8 temp_t9;
  temp_t9 = (*((u8 **) (&D_800D2108)))[(*D_800D154C).unk13F] - 1;
  if (D_800D154C->unk21F != 0)
  {
    temp_t9 = D_800D154C->unk21F;
  }
  D_800D154C->unk21E += D_800D154C->unk221;
  D_800D154C->unk21E += temp_t9;
  D_800D154C->unk21E %= temp_t9;
  if (D_800D154C->unk21E < D_800D154C->unk220)
  {
    D_800D154C->unk21E = D_800D154C->unk220;
  }
}

// NON-MATCHING: close but not there yet
// void func_150778F0(void) {
//     u8 temp_t9;
//
//     temp_t9 = (u8)(D_800D2108 + D_800D154C->unk13F) - 1;
//     if (D_800D154C->unk21F != 0) {
//         temp_t9 = D_800D154C->unk21F;
//     }
//     D_800D154C->unk21E += D_800D154C->unk221;
//     D_800D154C->unk21E += temp_t9;
//     D_800D154C->unk21E %= temp_t9;
//     if (D_800D154C->unk21E < D_800D154C->unk220) {
//         D_800D154C->unk21E = D_800D154C->unk220;
//     }
// }

void func_150779A8(void) {
    func_15075650();
    D_800D154C->unk223 = 0xB;
}

void func_150779D4(void) {
    struct127 *tmp;
    u8 idx;
    f32 temp_f2;

    idx = 0;
    if (D_800D1892 != 0) {
        idx = D_800D154C->unk222;
    }
    if (D_800C3E78 != idx) {
        tmp = &D_800CC2D0[idx];
        if ((tmp->interaction_state != 1) || (tmp->unk65 == 0)) {
            if (func_1505A6F8(D_800D154C, tmp) < (temp_f2 = D_800D1893 << 3)) {
                func_15075400(D_800D1890);
            }
        }
    }
}
// NON-MATCHING: JUSTREG! using $f6 not $f2
// void func_150779D4(void) {
//     struct127 *tmp;
//     u8 idx = 0;
//
//     if (D_800D1892 != 0) {
//         idx = D_800D154C->unk222;
//     }
//     tmp = &D_800CC2D0[idx];
//     if ((D_800C3E78 != idx) && ((tmp->unk0 != 1) || (tmp->unk65 == 0))) {
//         if (func_1505A6F8(D_800D154C, tmp) < (D_800D1893 * 8)) {
//              func_15075400(D_800D1890);
//         }
//     }
// }

void func_15077AA0(void) {
    D_800D154C->unk239 = D_800D1890;
}

void func_15077ABC(void) {
    D_800D154C->unk258 = D_800D1890;
    D_800D154C->unk257 = D_800D1891;
    D_800D154C->unk86 = func_150ADA20() % 0xFFU;
}

void func_15077B14(void) {
    D_800D154C->unk24C = D_800D1890;
    D_800D154C->unk24D = D_800D1891;
}

void func_15077B44(void) {
    func_15060A30((D_800D1890 << 8) + D_800D1891, D_800D154C);
}

void func_15077B80(void) {
    s32 tmp = (D_800D1890 << 8) + D_800D1891;
    D_800D154C->unk25C |= tmp;
}

void func_15077BB4(void) {
    struct127 *tmp = func_1505F0AC(D_800D1891);
    tmp->unk218 = 0;
    tmp->unk232 = D_800D1890;
}

void func_15077BE4(void) {
    s32 tmp = ~((D_800D1890 << 8) + D_800D1891);
    D_800D154C->unk25C &= tmp;
}

void func_15077C1C(void) {
    D_800D154C->unk23D = D_800D1890;
}

void func_15077C38(void) {
    struct127 *actor;
    f32 tmp0;
    f32 distance;
    s32 i;

    tmp0 = D_8009A148;
    D_800D154C->unk222 = (u8)0;

    for (i = 0; i < 25; i++) {
        actor = &D_800CC2D0[i];
        if (((actor->interaction_state != 0) && (actor->health != 0)) &&
            ((actor->immune == 0) || (actor->immune == 0xFF)) &&
            ((actor->unk65 == 0) || (D_800D1892 != 0)) &&
            ((actor->id == D_800D1890) || ((actor->interaction_state == D_800D1891) &&
            (actor->stunned == 0)))) {
            if (D_800D1893 == 0) {
                D_800D154C->unk222 = i;
                return;
            }
            distance = func_1505A72C(D_800D154C, actor);
            if (tmp0 > distance) {
                D_800D154C->unk222 = i;
                tmp0 = distance;
            }
        }
    }
}

void func_15077DA0(void) {
    D_800D154C->unk21E = D_800D1890;
}

struct func_15077DBC_pt { s16 unk0; s16 unk2; s16 unk4; s16 unk6; };
void func_15077DBC(void) {
    if (D_800D1890 != 0xFA) {
        D_800D154C->unk21E = D_800D1890;
    }
    D_800D154C->x_position = (((struct func_15077DBC_pt **)D_800D2104)[D_800D154C->unk13F] + D_800D154C->unk21E)[1].unk0;
    D_800D154C->y_position = (((struct func_15077DBC_pt **)D_800D2104)[D_800D154C->unk13F] + D_800D154C->unk21E)[1].unk2;
    D_800D154C->z_position = (((struct func_15077DBC_pt **)D_800D2104)[D_800D154C->unk13F] + D_800D154C->unk21E)[1].unk4;
}

void func_15077E9C(void) {
    s32 tmp = ((D_800D1890 << 8) + D_800D1891);
    func_10012718(tmp, D_800D154C, 0x5DC0, 0x1F4, 0x9C4);
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

void func_15077F64(void) {
    f32 dx;
    f32 dz;
    u8 turn;
    u8 index;

    index = D_800D1890 - 1;
    dx = (f32)D_800D2104[D_800D154C->unk13F]->points[D_800D154C->unk21E].x - D_800D154C->x_position;
    dz = D_800D154C->z_position - (f32)D_800D2104[D_800D154C->unk13F]->points[D_800D154C->unk21E].z;
    D_800D154C->unk78 = func_1505A630(dx, dz, 0);
    turn = (D_800D154C->unk78 - D_800D154C->unk76) >> 8;
    if (turn & 0x80) {
        turn = -turn;
    }
    if (D_80099A3C[index].unk0 < turn) {
        D_800D154C->unk250 = D_800D1891;
        func_15056A00(D_800D154C, turn, index);
    }
}

void func_15078074(void) {
    f32 temp_f20;
    struct127 *tmp;
    s32 i;

    temp_f20 = 8 * D_800D1893;

    for (i = 0; i < 25; i++) {
        tmp = &D_800CC2D0[i];
        if ((tmp->interaction_state != 0) && (D_800C3E78 != i)) {
            if (func_1505A6F8(D_800D154C, tmp) < temp_f20) {
                func_15075400(D_800D1890);
                return;
            }
        }
    }
}

void func_1507813C(void) {
    s32 idx = D_800D154C->unk222;
    if (D_800CC2D0[idx].interaction_state == 0) {
        func_15075400(D_800D1890);
    }
}

void func_150781A4(void) {
    D_800D154C->unk23F = D_800D1891;
    D_800D154C->unk240 = D_800D1892;
    D_800D154C->unk241 = D_800D1890;
    D_800D154C->unk242 = D_800D1893;
}

struct func_150781F4_pt { s16 unk0; s16 unk2; s16 unk4; s16 unk6; };
void func_150781F4(void) {
    struct func_150781F4_pt *temp_v0;
    f32 dx;
    f32 dz;
    f32 dist;
    s32 idx;

    idx = D_800D1891 + 1;
    temp_v0 = &((struct func_150781F4_pt **)D_800D2104)[D_800D154C->unk13F][idx];
    dx = temp_v0->unk0 - D_800D154C->x_position;
    dz = temp_v0->unk4 - D_800D154C->z_position;
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

void func_150782E8(void) {
    if (((D_800D1892 == 0) && (D_800D154C->unique_id == D_800D1891)) ||
        ((D_800D1892 == 1) && (D_800D154C->unique_id != D_800D1891))) {
        func_15075400(D_800D1890);
    }
}

void func_15078358(void) {
    s32 tmp = func_15083FB0(D_800D1890);
    D_800D154C->unk222 = tmp;
    if (tmp == -1) {
        D_800D154C->unk222 = 0;
    }
}

void func_1507839C(void) {
    struct127 *tmp;
    f32 distance;
    f32 threshold;

    if (D_800D1892 == 0) {
        tmp = &D_800CC2D0[D_800D154C->unk222];
        distance = func_1505A6F8(D_800D154C, tmp);
    } else if (D_800D1892 == 1) {
        tmp = &D_800CC2D0[D_800D154C->unk222];
        distance = func_1505A72C(D_800D154C, tmp);
    } else {
        distance = fabsf(D_800D154C->y_position - *(f32 *)((s32)D_800CC2E8 + (D_800D154C->unk222 * 812)));
    }

    threshold = D_800D1893 * 8;
    if (D_800D1893 == 0xFF) {
        threshold = D_800D154C->unk23D * 8;
    }
    if (((D_800D1891 == 0) && (distance < threshold)) ||
        ((D_800D1891 == 1) && (threshold < distance))) {
        func_15075400(D_800D1890);
    }
}
// NON-MATCHING: JUSTREG
// void func_1507839C(void) {
//     struct127 *tmp;
//     f32 distance;
//     f32 threshold;
//
//     if (D_800D1892 == 0) {
//         tmp = &D_800CC2D0[D_800D154C->unk222];
//         distance = func_1505A6F8(D_800D154C, tmp);
//     } else if (D_800D1892 == 1) {
//         tmp = &D_800CC2D0[D_800D154C->unk222];
//         distance = func_1505A72C(D_800D154C, tmp);
//     } else {
//         distance = fabsf(D_800D154C->y_position - D_800CC2D0[D_800D154C->unk222].y_position);
//     }
//
//     threshold = D_800D1893 * 8;
//     if (D_800D1893 == 0xFF) {
//         threshold = D_800D154C->unk23D * 8;
//     }
//     if (((D_800D1891 == 0) && (distance > threshold)) ||
//         ((D_800D1891 == 1) && (distance > threshold))) {
//         func_15075400(D_800D1890);
//     }
// }

void func_15078520(void) {
    func_15075400(D_800D1890);
}

extern f32 func_15048A40(u8);
extern f32 func_150489B0(u8);
typedef struct {
    u8 pad[8];
    struct { s16 x, y, z, w; } p[1];
} Win78544;
extern f32 sinf(f32);
extern f32 cosf(f32);

void func_15078544(void) {
    struct127 *obj;
    f32 y;
    f32 x;
    f32 ox;
    f32 oz;
    f32 s;
    f32 c;
    f32 ang;
    u8 a;
    f32 z;
    s32 mode;
    u8 v;

    mode = D_800D1890 >> 6;
    D_800D1890 &= 0x3F;
    x = D_800CC2D0[D_800D154C->unk222].x_position;
    z = D_800CC2D0[D_800D154C->unk222].z_position;
    y = D_800CC2D0[D_800D154C->unk222].y_position;
    ox = ((((s8)D_800D1891 << 4) | (D_800D1893 >> 4)) * 8);
    oz = ((((s8)D_800D1892 << 4) | (D_800D1893 & 0xF)) * 8);
    if (mode == 0) {
        a = ((D_800CC2D0[D_800D154C->unk222].unk7A >> 8) & 0xFF) - 0x40;
        s = func_15048A40(a);
        c = func_150489B0(a);
    } else if (mode == 1) {
        s = 0.0f;
        c = 1.0f;
    } else {
        ang = func_150484A0(D_800D154C->x_position - x, D_800D154C->z_position - z);
        s = sinf(ang);
        c = cosf(ang);
    }
    x += ox * c + oz * s;
    z += oz * c - ox * s;
    ((Win78544 *)D_800D2104[D_800D154C->unk13F])->p[D_800D1890].x = x;
    ((Win78544 *)D_800D2104[D_800D154C->unk13F])->p[D_800D1890].y = y;
    ((Win78544 *)D_800D2104[D_800D154C->unk13F])->p[D_800D1890].z = z;
}
void func_1507879C(void) {
    struct197 *temp_v0;
    f32 temp_f0;
    s32 temp_v1;

    temp_v0 = (struct197 *)D_800CC5A0[D_800D154C->unk222].interaction_state;
    temp_f0 = temp_v0->unk8;
    temp_v1 = D_800D1892;
    if (((temp_v1 == 0) && (temp_f0 < (f32)((u32)D_800D1891))) ||
        ((temp_v1 == 1) && ((f32)((u32)D_800D1891) < temp_f0))) {
        func_15075400(D_800D1890);
    }
}
// NON-MATCHING: D_800CC5A0 ???
// void func_1507879C(void) {
//     f32 temp_f0 =  D_800CC5A0[D_800D154C->unk222].unk8;
//
//     if (((D_800D1892 == 0) && (temp_f0 < D_800D1891)) ||
//         ((D_800D1892 == 1) && (temp_f0 > D_800D1891))) {
//         func_15075400(D_800D1890);
//     }
// }

void func_15078874(void) {
    D_800D154C->unk251 = D_800D1890;
}

void func_15078890(void) {
    if (((D_800D1892 == 0) && (D_800D154C->unk251 == D_800D1891)) ||
        ((D_800D1892 == 1) && (D_800D154C->unk251 != D_800D1891))) {
        func_15075400(D_800D1890);
    }
}

void func_15078900(void) {
    if (D_800D1893 != 0) {
        if (D_800D154C->unk222 == 0) {
            func_15075400(D_800D1890);
        }
    } else {
        if (((D_800D1892 == 0) && (D_800D1891 == D_800CC521[D_800D154C->unk222 * 0x32C])) ||
            ((D_800D1892 == 1) && (D_800D1891 != D_800CC521[D_800D154C->unk222 * 0x32C]))) {
            func_15075400(D_800D1890);
        }
    }
}

void func_15078A08(void) {
    struct197 *temp_a0;
    struct197 *temp_v0;

    temp_v0 = D_800D154C->unk2D0;
    temp_a0 = D_800CC5A0[D_800D154C->unk222].interaction_state; // wtf
    temp_v0->unk8 = (f32) temp_a0->unk8;
    temp_v0->unkC = (f32) temp_a0->unkC;
}

// requires jump table
s32 func_15078A60(s32 arg0) {
    s32 value;
    s32 diff;
    s32 pad;
    u8 angle;

    switch (D_800D1892) {
    case 0:
        value = D_800CC2D0[arg0].xz_velocity;
        break;
    case 1:
        value = D_800CC2D0[arg0].unk2E4;
        break;
    case 2:
        value = D_800CC2D0[arg0].unk2E8;
        break;
    case 3:
        value = D_800CC2D0[arg0].unk2EC;
        break;
    case 4:
        value = D_800CC2D0[arg0].y_position;
        break;
    case 5:
        value = D_800CC2D0[arg0].unk44;
        break;
    case 6:
        angle = D_800CC2D0[arg0].unk7A >> 8;
        angle -= 0x40;
        diff = (u8)(func_150484A0(D_800CC2D0[arg0].x_position - D_800D154C->x_position, D_800CC2D0[arg0].z_position - D_800D154C->z_position) * 40.74366379f) - angle;
        value = (u8)diff;
        if (value > 0x80) {
            value = 0x100 - value;
        }
        break;
    case 7:
        value = D_800CC2D0[arg0].unk13C;
        break;
    case 8:
        value = D_800CC2D0[arg0].stunned;
        break;
    case 9:
        value = 0;
        if (((s16)D_800CC2D0[arg0].unk31C->unk8 != 0) && (D_800CC2D0[arg0].unk31C->unk16 != 0)) {
            value = 2;
        }
        break;
    case 10:
        value = D_800CC2D0[arg0].unk28;
        break;
    case 11:
        value = D_800CC2D0[0].unk31C->unk78;
        break;
    case 12:
        value = D_800CC2D0[0].unk31C->matrix_physics;
        break;
    case 13:
        value = D_800CC2D0[0].unk102;
        break;
    case 14:
        value = D_800BE9EC;
        break;
    case 15:
        value = D_800CC2D0[D_800D154C->unk222].id;
        break;
    case 16:
        value = D_800CC2D0[arg0].health;
        break;
    case 17:
        value = D_800CC2D0[D_800D154C->unk222].unk232;
        break;
    case 18:
        value = D_800CC2D0[arg0].disable_run;
        break;
    }
    if (((D_800D1891 == 0) && (value == D_800D1893)) || ((D_800D1891 == 1) && (value != D_800D1893)) || ((D_800D1891 == 2) && (value < D_800D1893)) || ((D_800D1891 == 3) && (value > D_800D1893)) || ((D_800D1891 == 4) && (value <= D_800D1893)) || ((D_800D1891 == 5) && (value >= D_800D1893))) { return 1; } return 0;
}

void func_1507900C(void) {
    if (func_15078A60(0) != 0) {
        func_15075400(D_800D1890);
    }
}

void func_1507903C(void) {
    if (func_15078A60(0) != 0) {
        D_800D154C->unk218 = func_1507BB28(0, D_800D1890);
        D_800D154C->unk218 -= 1;
    }
}

void func_15079090(void) {
    if (func_15078A60(D_800C3E78) != 0) {
        func_15075400(D_800D1890);
    }
}

void func_150790C4(void) {
    if (func_15078A60(D_800C3E78) != 0) {
        D_800D154C->unk218 = func_1507BB28(0, D_800D1890);
        D_800D154C->unk218 -= 1;
    }
}

u8 func_1507911C(void) {
    s16 temp_a2;
    s16 coord;
    u8 idx;

    temp_a2 = (D_800D1891 << 8) | D_800D1892;
    idx =  D_800D1893 & 1;
    if (idx != 0) {
        idx = D_800C3E78;
    }
    coord = D_800CC2D0[idx].y_position;
    if ((D_800D1893 & 2) != 0) {
        coord = D_800CC2D0[idx].x_position;
    }
    if (coord < temp_a2) {
        func_15075400(D_800D1890);
    }
}

void func_150791F0(void) {
    if ((s32) D_800D154C->unk2C9 < (s32) D_800D1890) {
        D_800D154C->unk1C9 = (u8)0xFFU;
    } else {
        D_800D154C->unk1C9 = D_800D1890;
    }
}

void func_15079228(void) {
    struct127 *temp_v1;
    struct178 *temp_v0;
    f32 temp_f0;
    f32 temp_f2;
    u16 temp_a2;

    temp_v0 = &(*(struct178 **)&D_800D3098)[D_800D154C->unk251];
    temp_v1 = D_800D154C;
    temp_f0 = temp_v0->unk0;
    temp_f2 = temp_v0->unk4;
    temp_a2 = func_1505A630(temp_v1->x_position - temp_f0, temp_f2 - temp_v1->z_position, 0);
    if (temp_a2 == 0) {
        temp_a2 = 1;
    }
    func_1505D024(D_800D154C, D_800D1890, temp_a2, -1);
    D_800D154C->unk218 -= 1;
    D_800D154C->unk21C = 1;
}

void func_150792E0(void) {
    D_800D154C->unk232 = D_800D1890;
}

void func_150792FC(void) {
    D_800D154C->unk2F8 &= 0xFFF8;
    D_800D154C->unk2F8 |= D_800D1890;
}

void func_15079334(void) {
    struct127 *tmp;

    if (D_800D154C->unkA8 != 0) {
        tmp = &D_800CC2D0[D_800D154C->unkA8 & 0x7F];
        tmp->unk218 = 0;
        tmp->unk232 = D_800D1890;
    }
}

void func_15079390(void) {
    u16 tmp0 = D_800D1890;
    u8 tmp1 = D_800D1891;
    u8 tmp2 = D_800D1892;
    func_1514D3B0(D_800D154C, tmp0, tmp1, tmp2);
}

void func_150793D8(void) {
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    s16 pad0;
    s16 pad1;
    s16 pad2;
    s16 x;
    s16 z;

    x = (s16)(s32)D_800CC2D0->x_position;
    z = (s16)(s32)D_800CC2D0->z_position;
    if ((x < -1180) || (x >= 1181) || (z < -1180) || (z >= 1181)) {
        x = 0;
        z = 0;
    }

    func_1505A184(func_150ADA20(), 550.0f, 0.0f, &sp34, &sp30, &sp2C);

    x = (s16)(s32)((f32)x + sp34);
    z = (s16)(s32)((f32)z + sp30);
    if (x < -1200) {
        x = -1200;
    }
    if (x >= 1201) {
        x = 1200;
    }
    if (z < -1200) {
        z = -1200;
    }
    if (z >= 1201) {
        z = 1200;
    }

    *(s16 *)((u8 *)D_800D2104[D_800D154C->unk13F] + (D_800D1890 * 8) + 8) = x;
    *(s16 *)((u8 *)D_800D2104[D_800D154C->unk13F] + (D_800D1890 * 8) + 0xC) = z;
}

void func_15079570(void) {
    f32 temp;
    f32 d2;

    temp = func_1505A6F8(D_800D154C, &D_800CC2D0[D_800D154C->unk222]);
    D_800D154C->unk44 = D_800D154C->xz_velocity;
    d2 = 2.0f * (temp / D_800D154C->unk44);
    D_800D154C->y_velocity = (D_800D154C->gravity * d2) * 0.5f;
    D_800D154C->y_velocity += 2.0f * ((*(f32*)((u8*)D_800CC2E8 + D_800D154C->unk222 * 812) - D_800D154C->y_position) / d2);
}
// void func_15079570(void) {
//     f32 temp_f2;
//
//     temp_f2 = func_1505A6F8(D_800D154C, &D_800CC2D0[ D_800D154C->unk222]);
//     D_800D154C->unk44 = D_800D154C->xz_velocity;
//     temp_f2 = 2.0f * (temp_f2 / D_800D154C->unk44);
//     D_800D154C->y_velocity = D_800D154C->gravity * temp_f2 * 0.5f;
//     D_800D154C->y_velocity += ((D_800CC2E8[D_800D154C->unk222 * 203] - D_800D154C->y_position) / temp_f2) * 2.0f;
// }

void func_1507965C(void) {
    s32 idx;
    struct127 *temp_v1;

    idx = func_15083FB0(D_800D1890);
    if (idx != -1) {
        temp_v1 = &D_800CC2D0[idx];
        temp_v1->unk232 = D_800D1891;
        temp_v1->unk218 = 0;
        temp_v1->unk23A = 0;
    }
}

void func_150796CC(void) {
    s32 idx = func_15083FB0(D_800D1890);
    if (D_800D1892 != 0) {
        if (idx != -1) {
            D_800CC3F5[idx * sizeof(struct127)] = D_800D1891;
        }
    } else if ((idx != -1) && (D_800CC3D4[idx * sizeof(struct127)] != 0)) {
        func_15075400(D_800D1891);
    }
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} CoordPair15079790;

void func_15079790(void) {
    s16 temp_t1;
    s16 temp_t0;

    if (D_800D1892 != 0) {
        D_800D154C->id = 0xFF;
    } else {
        D_800D154C->id = 0x3A;
        temp_t1 = (func_150ADA20() % 0x1F4U) - 0xFA;
        temp_t0 = (func_150ADA20() % 0x1F4U) - 0xFA;
        D_800D154C->x_position = ((CoordPair15079790 **)D_800D2104)[D_800D154C->unk13F]->unk0 + temp_t1;
        D_800D154C->z_position = ((CoordPair15079790 **)D_800D2104)[D_800D154C->unk13F]->unk4 + temp_t0;
    }
}

// seriously, D_800D2104?
// void func_15079790(void) {
//     s16 temp_t1;
//     s16 temp_t0;
//
//     if (D_800D1892 != 0) {
//         D_800D154C->id = 0xFF;
//     } else {
//         D_800D154C->id = 0x3A;
//         temp_t1 = (func_150ADA20() % 0x1F4U) - 0xFA;
//         temp_t0 = (func_150ADA20() % 0x1F4U) - 0xFA;
//         D_800D154C->x_position = (s32)&D_800D2104[D_800D154C->unk13F] + temp_t1;
//         D_800D154C->z_position = (s32)&D_800D2104[D_800D154C->unk13F + 1] + temp_t0;
//     }
// }

void func_15079880(void) {
    s32 tmp = D_800CC30C[0] + (s8)D_800D1892;

    if (tmp < D_800D1891) {
        tmp = D_800D1891;
    } else if (tmp >= 251) {
        tmp = 250;
    }
    D_800D1892 = tmp;
    func_15075548();
}

void func_150798F8(void) {
    D_800D1891 = D_800D154C->unk21E;
    func_150781F4();
}

void func_15079928(void) {
    s32 mask;

    mask = ((D_800D1891 << 0x10) + D_800D1892) & 0xFFFF;
    if (mask == 0) {
        mask = 0xFFFF;
    }
    func_1507E7E4(D_800D154C, D_800D1890, D_800D1893, mask, 0);
}

void func_15079988(void) {
    func_1507EB4C(D_800D154C, D_800D1890);
}

void func_150799B4(void) {
    s32 temp_a0 = (D_800D1890 << 8) + D_800D1891;
    if (D_800D1892 != 0) {
        temp_a0 += func_150ADA20() % (u32) D_800D1892;
    }
    func_15060A30(temp_a0, D_800D154C);
}

void func_15079A28(void) {
    D_800D154C->unk252 = D_800D1890;
    D_800D154C->unk253 = D_800D1891;
}

void func_15079A58(void) {
    (*(s16 **)&D_800D2110)[D_800D154C->unk13F] = (D_800D1890 << 8) + D_800D1891;
}
// void func_15079A58(void) {
//     s16 tmp = (D_800D1890 << 8) + D_800D1891;
//     D_800D2110[D_800D154C->unk13F] = tmp;
// }

u16 func_15079A98(s32 arg0) {
    struct169 *temp_v0;
    struct127 *temp_v1;

    temp_v0 = ((struct169**)D_800D2104)[D_800D154C->unk13F];
    temp_v1 = &D_800CC2D0[arg0];
    return func_1505A630(temp_v0->unk0 - temp_v1->x_position, temp_v1->z_position - temp_v0->unk4, 0);
}

void func_15079B30(void) {
    s32 idx;
    s32 *ptr;
    struct127 *saved_ptr;
    s32 saved_idx;
    u16 a;
    s32 sel;
    sel = D_800D1891;
    if (sel == 0) goto body;
lbl:
    switch (D_800D1891) {
    case 1:
    body:
        idx = D_800C3E78;
        break;
    default:
        idx = func_15083FB0((u8)(D_800D1891 - 2));
        if (idx == -1) {
            return;
        }
        break;
    }
    switch (D_800D1892) {
    case 1:
        ptr = &D_800CC2D0[idx].unk2E4;
        break;
    case 2:
        ptr = &D_800CC2D0[idx].unk2E8;
        break;
    case 3:
        ptr = &D_800CC2D0[idx].unk2EC;
        break;
    case 4:
    case 5:
    {
        extern struct127 *D_800D154C;
        saved_ptr = D_800D154C;
        saved_idx = D_800C3E78;
        D_800D154C = &D_800CC2D0[idx];
        D_800C3E78 = idx;
        if (D_800D154C->unk232 != D_800D1890) {
            D_800D154C->unk232 = D_800D1890;
            D_800D154C->unk218 = func_1507BB28(0, D_800D154C->unk232);
            D_800D154C->unk21C = 0;
        }
        D_800C3E78 = saved_idx;
        D_800D154C = saved_ptr;
        return;
    }
    case 6:
        *(f32 *)&D_800CC2D0[idx].padC8 = D_800D1890;
        return;
    case 7:
        D_800CC2D0[idx].unkB8 = 0.0f;
        return;
    case 8:
    {
        a = func_15079A98(D_800D154C->unk222);
        if ((func_15079A98(D_800C3E78) - a) & 0x8000) {
            D_800D154C->unk221 = D_800D1890;
        }
        return;
    }
    case 9:
    {
        u16 tmp = (D_800D1893 << 8) | D_800D1890;
        D_800D154C->unk244 = tmp;
        return;
    }
    case 10:
        D_800D154C->unk236 = D_800D154C->unk23D + (s8)D_800D1890;
        return;
    case 11:
        D_800D154C->pad22A = D_800D1890;
        return;
    }
    switch (D_800D1893) {
    case 0:
        *ptr = D_800D1890;
        break;
    case 1:
        *ptr -= 1;
        break;
    case 2:
        *ptr += D_800BE9E4;
        break;
    case 3:
        *ptr += 1;
        break;
    case 4:
        *ptr -= D_800BE9E4;
        break;
    case 5:
        *ptr -= D_800CC264;
        break;
    case 6:
        *ptr += D_800CC264;
        break;
    }
}

void func_15079F24(void) {
    D_800D154C->unk48 = D_800D1898[D_800D1890];
}

void func_15079F50(void) {
    D_800D154C->unk23B = D_800D1890;
}

void func_15079F6C(void) {
    u16 tmp0;
    u16 tmp1;
    tmp0 = D_800D1890 << 8;
    tmp1 = D_800D1891;
    D_800D154C->unk224 = (tmp0 | (((((((((tmp1 & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF) & 0xFF)) & 0xFFFFu;
    if (1) {
        D_800D154C->unk22B = D_800D1892;
    }
    D_800D154C->unk226 = D_800D1893;
    tmp0 += 0;
}
// NON-MATCHING: JUSTREG
// void func_15079F6C(void) {
//     u16 tmp0;
//     u16 tmp1;
//     tmp0 = D_800D1890 << 8;
//     tmp1 = D_800D1891;
//     D_800D154C->unk224 = tmp0 | tmp1;
//     D_800D154C->unk22B = D_800D1892;
//     D_800D154C->unk226 = D_800D1893;
// }

void func_15079FBC(void) {
    s16 speed;

    if ((D_800CC2D0->unk65 == 0) || (D_800CC2D0->unk44 < 5.0f)) {
        D_800D154C->unk232 = D_800D1891;
        D_800D154C->unk218 = func_1507BB28(0, D_800D154C->unk232);
        D_800D154C->unk218 -= 1;
        return;
    }

    D_800D154C->unk78 = D_800CC2D0->unk31C->unk4C;

    if (D_800CC2D0->unk31C->chased) {
        speed = D_800CC2D0->unk44;
    } else {
        speed = D_800CC2D0->unk44 * D_8009A1E4;
    }
    if (speed < 17) {
        D_800D1893 += 1;
        D_800D1890 = 8;
    }
    D_800D1892 = speed;
    func_15075F40();
    D_800D154C->unkF4 |= 0x40;
}

// D_800D2104!
s32 func_1507A100(void) {
    s32 val = ((s8)D_800D1892 << 8) | D_800D1893;
    *(s16 *)((u8 *)D_800D2104[D_800D154C->unk13F] + (D_800D1890 * 8) + (D_800D1891 * 2) + 8) = val;
    return val;
}

s32 func_1507A164(void) {
    s32 tmp = D_800CC30C[0] + (s8)D_800D1892;

    if (tmp < D_800D1891) {
        tmp = D_800D1891;
    } else if (tmp > D_800D1890) {
        tmp = D_800D1890;
    }
    D_800D154C->unk44 = tmp;
    if (D_800D154C->unk44 == 1.0f) {
        D_800D154C->unk44 = 0.5f;
    }
}

void func_1507A210(void) {
    D_800D154C->unk223 = 0;
    D_800D154C->unk21C = D_800D1890 * 0x64;
    D_800D154C->unk22C &= 0xFD;
    func_15075498();
}

void func_1507A270(void) {
    func_1503DE70(D_800D154C, D_800D1890, (s8)D_800D1891);
}

void func_1507A2A4(void) {
    D_800D1893 = (func_150ADA20() % (u32) D_800D1891) + D_800D1893;
    func_15075CA0();
}

void func_1507A2F8(void) {
    s32 i;
    u32 used;
    u8 sp1C[25];

    used = 0;
    for (i = 0; i < 25; i++) {
        if ((D_800CC2D0[i].interaction_state != 0) && (D_800CC2D0[i].id == D_800D1890)) {
            sp1C[used] = i;
            used++;
        }
    }
    if (used != 0) {
        i = func_150ADA20();
        D_800D154C->unkA8 = D_800D154C->unk222;
        D_800D154C->unk222 = sp1C[i % used];
    }
}

void func_1507A3B4(void) {
    D_800D154C->unk222 = D_800D154C->unkA8;
}

void func_1507A3CC(void) {
    D_800D154C->unk229 = D_800D1890;
}

//  what is up with these??
s32 func_1507A3E8(void) {
    return ((D_800D1890 & 0xFF) << 0x18) | ((D_800D1891 & 0xFF) << 0x10) | ((D_800D1892 & 0xFF) << 8) | (D_800D1893 & 0xFF);
}
// s32 func_1507A3E8(void) {
//     return (D_800D1890 << 0x18) | (D_800D1891 << 0x10) | (D_800D1892 << 8) | D_800D1893;
// }

void func_1507A428(void) {
    s32 temp;

    temp = ((D_800D1890 & 0xFF) << 0x18) | ((D_800D1891 & 0xFF) << 0x10) | ((D_800D1892 & 0xFF) << 8) | (D_800D1893 & 0xFF);
    D_800D154C->unk94 = ~(temp | 1);
}
void func_1507A47C(void) {
    s32 temp;

    temp = ((D_800D1890 & 0xFF) << 0x18) | ((D_800D1891 & 0xFF) << 0x10) | ((D_800D1892 & 0xFF) << 8) | (D_800D1893 & 0xFF);
    D_800D154C->unk94 &= ~temp;
}
void func_1507A4D4(void) {
    s32 temp;

    temp = ((D_800D1890 & 0xFF) << 0x18) | ((D_800D1891 & 0xFF) << 0x10) | ((D_800D1892 & 0xFF) << 8) | (D_800D1893 & 0xFF);
    D_800D154C->unk94 |= temp;
}

void func_1507A528(void) {
    s32 limit;
    s32 temp_a1;

    if (D_800D1890 == 0) {
        D_800D154C->unk221 = D_800D1891;
    } else if (D_800D1890 == 1) {
        D_800D154C->unk221 = -D_800D154C->unk221;
    } else if (D_800D1890 == 2) {
        if (D_800D1892 != 0) {
            limit = D_800D1892;
        } else {
            limit = (*(u8 **)&D_800D2108)[*(u8 *)((u8 *)D_800D154C + 0x13F)] - 1;
        }
        D_800D154C->unk221 = -D_800D154C->unk221;
        temp_a1 = D_800D154C->unk21E + D_800D154C->unk221;
        if (D_800D154C->unk221 > 0) {
            temp_a1 += D_800D1893;
        } else {
            temp_a1 -= D_800D1893;
        }
        if (temp_a1 >= limit) {
            temp_a1 = temp_a1 - limit;
        } else {
            if (temp_a1 < 0) {
                temp_a1 = temp_a1 + limit;
            }
        }
        D_800D154C->unk21E = temp_a1;
    }
}
// NON-MATCHING: 99% there..
// void func_1507A528(void) {
//     s32 limit;
//     s32 temp_a1;
//
//     if (D_800D1890 == 0) {
//         D_800D154C->unk221 = D_800D1891;
//     } else if (D_800D1890 == 1) {
//         D_800D154C->unk221 = -D_800D154C->unk221;
//     } else if (D_800D1890 == 2) {
//         if (D_800D1892 != 0) {
//             limit = D_800D1892;
//         } else {
//             limit = (u8)(D_800D2108 + D_800D154C->unk13F) - 1;
//         }
//         D_800D154C->unk221 = -D_800D154C->unk221;
//         temp_a1 = D_800D154C->unk21E + D_800D154C->unk221;
//         if (D_800D154C->unk221 > 0) {
//             temp_a1 += D_800D1893;
//         } else {
//             temp_a1 -= D_800D1893;
//         }
//         if (temp_a1 >= limit) {
//             temp_a1 = temp_a1 - limit;
//         } else {
//             if (temp_a1 < 0) {
//                 temp_a1 = temp_a1 + limit;
//             }
//         }
//         D_800D154C->unk21E = temp_a1;
//     }
// }

void func_1507A620(void) {
    u8 idx;
    u16 tmp;

    idx = D_800D154C->unk222;
    if (D_800D1891 != 0) {
        tmp = D_800D154C->unk78 >> 8;
    } else {
        tmp = (D_800D154C->unk7A - func_1505A630(D_800CC2D0[idx].x_position - D_800D154C->x_position, D_800D154C->z_position - D_800CC2D0[idx].z_position, 0)) >> 8;
    }
    if ((u8)(tmp - D_800D1892) < (u8)(D_800D1893 - D_800D1892)) {
        func_15075400(D_800D1890);
    }
}

s32 func_1507A6FC(s32 arg0) {
    // really??
    u32 tmp0 = (3 * arg0) & 0xFFFFFFFFFFFFFFFF;
    u32 tmp1 = tmp0 & 0xFFFFFFFFFFFFFFFF;
    return D_800BE748[tmp1 & 0xFFFFFFFFFFFFFFFF];
}

void func_1507A71C(void) {
    u16 tmp0 = func_1507A6FC(D_800D1890);
    u16 tmp1 = ((D_800D1891 << 8) | D_800D1892);

    if ((tmp0 & tmp1) == tmp1) {
        func_15075400(D_800D1893);
    }
}

void func_1507A774(void) {
    u16 temp_v0 = (D_800D1891 << 8) | D_800D1892;
    if (D_800BE9F0 == temp_v0) {
        func_15075400(D_800D1890);
    }
}

void func_1507A7C0(void) {
    D_800D154C->disable_run = (u8) D_800D1890;
}

void func_1507A7DC(void) {
    func_150836CC(D_800D154C, D_800D1890);
}

void func_1507A808(void) {
    D_800D154C->unk22C = (D_800D154C->unk22C & D_800D1890) | D_800D1891;
}

void func_1507A838(void) {
    if (D_800D154C->health == D_800D1891) {
        func_15075400(D_800D1890);
    }
}

void func_1507A878(void) {
    func_1512D748(0, D_800D1890, D_800D1891);
}

void func_1507A8A8(void) {
    if ((D_800CC335 != 0) || (D_800BE616 != 0)) {
        func_15075400(D_800D1890);
    }
}

void func_1507A8EC(void) {
    if (D_800D1891 == D_800CC2D4[D_800D154C->unk222 * sizeof(struct127)]) {
        D_800D1892 ^= 1;
    }
    if (D_800D1892 != 0) {
        func_15075400(D_800D1890);
    }
}

void func_1507A984(void) {
    s32 temp_v0;

    if (D_800D1890 != 0) {
        temp_v0 = func_15083E90(D_800D1890);
        if (temp_v0 != 0) {
            if (D_800D1892 == 0) {
                D_800D154C->unk101 |= 4;
            }
            if (D_800D1893 != 0) {
                D_800D154C->unk101 |= 0x40;
            }
            D_800D154C->unk65 = ((s32) (temp_v0 - (s32)&D_800CC2D0) / 0x32C) + 1;
            D_800D154C->unk5C = D_800D1891;
        }
    } else {
        D_800D154C->unk65 = 0;
    }
}

// requires jump table
typedef struct { s16 x, y, z, w; } E;
void func_1507AA48(void) {
    u16 val;
    E *tbl;
    s32 ang;

    val = (D_800D1890 << 8) | D_800D1891;
    switch (D_800D1892) {
    case 0:
        D_800D154C->unk78 = D_800D154C->unk76 + val;
        break;
    case 1:
        D_800D154C->unk78 = D_800D154C->unk76 - val;
        break;
    case 2:
        D_800D154C->unk78 = val;
        break;
    case 3:
        do {
            D_800D154C->unk76 = D_800D154C->unk78;
            D_800D154C->unk7A = D_800D154C->unk78;
        } while (0);
        break;
    case 4:
        tbl = (E *)D_800D2104[D_800D154C->unk13F];
        ang = (u16)(func_150484A0(tbl[D_800D1891 + 1].x - tbl[D_800D1890 + 1].x, tbl[D_800D1891 + 1].z - tbl[D_800D1890 + 1].z) * 10430.37793f) + (D_800D1893 << 8);
        D_800D154C->unk78 = ang + 0x4000;
        break;
    case 5:
        D_800D154C->unk78 = func_150ADA20();
        break;
    case 6:
        D_800D154C->unk78 = D_800D154C->unk76;
        break;
    case 7:
        D_800D154C->unk76 = D_800CC2D0[D_800D154C->unk222].unk7A + val;
        break;
    case 8:
        D_800D154C->unk76 = D_800D154C->unk7A;
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
    D_800D154C->unk1E4 = D_800D1890;
}

void func_1507AD7C(void) {
    D_800D154C->unkF4 |= func_1507A3E8();
}

void func_1507ADAC(void) {
    D_800D154C->unkF4 &= ~func_1507A3E8();
}

void func_1507ADE0(void) {
    s32 res;
    s32 temp_lo;
    s32 b = D_800D154C->unk7A;
    s32 a = D_800D154C->unk78;

    res = a - b;
    if (res < 0) {
        res = -res;
    }
    if (res >= 32769) {
        res = res + 0xFFFF0000;
    }
    if (res < 0) {
        res = -res;
    }
    temp_lo = res / (s32)D_800D1890;
    D_800D154C->unk1E6 = temp_lo >> 8;
    D_800D154C->unk1E8 = temp_lo & 0xFF;
}

void func_1507AE78(void) {
    D_800D154C->unk229 = D_800D1890;
}

void func_1507AE94(void) {
    D_800D154C->unkD2 = (D_800D1890 << 8) | D_800D1891;
    D_800D154C->unkD4 = (D_800D1892 << 8) | D_800D1893;
    D_800D154C->unkD2 *= D_800D154C->xz_scale;
    D_800D154C->unkD4 *= D_800D154C->y_scale;
}

void func_1507AF3C(void) {
    D_800D154C->unkD6 = (D_800D1890 << 8) | D_800D1891;
    D_800D154C->unkD6 *= D_800D154C->y_scale;
}

void func_1507AF98(void) {
    D_800D154C->unkD8 = (D_800D1890 << 8) | D_800D1891;
    D_800D154C->unkDA = (D_800D1892 << 8) | D_800D1893;
    D_800D154C->unkD8 *= D_800D154C->xz_scale;
    D_800D154C->unkDA *= D_800D154C->xz_scale;
}

void func_1507B040(void) {
    D_800D154C->unk76 = D_800D154C->unk7A;
}

void func_1507B058(void) {
    struct127 *sp24 = D_800D154C;
    s32 sp20 = D_800C3E78;

    D_800D154C = func_15072208(D_800D154C, 0);
    D_800C3E78 = ((s32)D_800D154C - (s32)&D_800CC2D0) / 0x32C;

    if (D_800D154C != 0) {
        switch (D_800D1890) {
            case 0:
                func_1506160C(D_800D154C, 1, D_800D1891, 0, 0);
                break;
            case 1:
                D_800D154C->unk232 = D_800D1891;
            case 2:
                D_800D154C->unk218 = func_1507BB28(0, D_800D1891);
                D_800D154C->unk21C = 0;
        }
    }
    D_800C3E78 = sp20;
    D_800D154C = sp24;
}

void func_1507B15C(void) {
    D_800D154C->disable_jump = D_800D1890;
}

void func_1507B178(void) {
    s16 tmp  = (D_800D1891 << 8) | D_800D1890;
    if (D_800D1893 == 2) {
        D_800D154C->unkE8 = (s16) (tmp * D_800D154C->y_scale);
    } else if (D_800D1893 == 1) {
        func_15062B50(D_800D154C, tmp);
    } else {
        func_15062B1C(D_800D154C, tmp);
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
        func_1506BA4C(300, 0xBB8);
    }
}

void func_1507B364(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(0x2BC, 0xFA0);
    }
}

void func_1507B3B0(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(0x2BC, 0xFA0);
    }
}

void func_1507B3FC(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(0xBB8, 0x1F40);
    }
}

void func_1507B448(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 1) != 0) {
        func_1506BAD8(0xBE, 0x514);
    }
}

void func_1507B494(void) {
    D_800D1580 = func_1507A3E8();
    if (func_1506E46C(D_800D154C, &D_800D1580, 0) != 0) {
        func_1506BA4C(0xBE, 0x514);
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
    D_800D154C->unkD0 = D_800D1890;
    D_800D154C->unk114 = D_800D1891;
}

void func_1507B5C4(void) {
    if (D_800D154C->unk31C->unk75 == D_800D1891) {
        D_800D1892 ^= 1;
    }
    if (D_800D1892 != 0) {
        func_15075400(D_800D1890);
    }
}

void func_1507B630(void) {
    if ((D_800D154C->disable_run == 0) && (D_800D154C->unk28 == 0.0f)) {
        if (D_800D154C->unk31C->unk78 != 0) {
            D_800D1890 ^= 1;
        }
        if (D_800D1890 != 0) {
            D_800D154C->unk31C->unk8C |= 0x4000;
            D_800D154C->unk31C->unk8F = 0;
        }
    } else {
        D_800D154C->unk218 -= 1;
    }
    D_800D154C->unk21C = 100;
}

void func_1507B6E0(void) {
    if ((D_800D154C->disable_run != 0) || (D_800D154C->unk28 > 5.0f)) {
        D_800D154C->unk218 -= 1;
        D_800D154C->unk21C = 0x64;
    }
}

void func_1507B734(void) {
    u16 tmp = ((D_800D1890 << 8) | D_800D1891);
    if (D_800D1893 != 0) {
        D_800D154C->unk31C->unk8C = tmp;
        D_800D154C->unk31C->unk8F = D_800D1892;
    } else {
        D_800D154C->unk31C->unk8A = tmp;
        D_800D154C->unk31C->unk8E = D_800D1892;
    }
}

void func_1507B7BC(void) {
    func_15075548();
    D_800D154C->unk223 = 17;
}

void func_1507B7E8(void) {
    struct127 *tmp = &D_800CC2D0[D_800D154C->unk222];
    if ((tmp->stunned != 0) || (tmp->health == 0)) {
        D_800D1891 ^= 1;
    }
    if (D_800D1891 != 0) {
        func_15075400(D_800D1890);
    }
}

u8 func_1507B884(void) {
    s32 tmp0;
    s32 tmp1;
    u8 temp_v0;
    void (*func)(s32);

    temp_v0 = D_800D1890;
    func = D_80086150[temp_v0];
    if (func != 0) {
        tmp0 = D_800D154C;
        tmp1 = D_800C3E78;
        func(temp_v0);
        D_800D154C = tmp0;
        D_800C3E78 = tmp1;
    }
}

void func_1507B8F4(void) {

    if (D_800BE616 != 0) {
        if ((D_800D1891 != 0) || ((D_800D154C->unk31C != 0) && (D_800D154C->unk31C->unk84 != 0))) {
            func_15075400(D_800D1890);
        }
    }
}

void func_1507B958(void) {
    D_800D154C->unkB0 = D_800D1890;
}

void func_1507B974(void) {
    struct127 *actor;

    actor = D_800D154C;
    if (D_800D154C->unk65 != 0) {
        actor = &D_800CC2D0[D_800D154C->unk65 - 1];
    }
    if (D_800D1891 == *(u16 *)((u8 *)D_800D2104[actor->unk13F] + (actor->unk21E * 8) + (D_800D1893 << 3) + 6)) {
        D_800D1892 ^= 1;
    }
    if (D_800D1892 != 0) {
        func_15075400(D_800D1890);
    }
}
// NON-MATCHING: dont think D_800D2104 is correct
// void func_1507B974(void) {
//     struct127 *actor;
//     struct169 *foo;
//
//     actor = D_800D154C;
//     if (D_800D154C->unk65 != 0) {
//         actor = &D_800CC2D0[D_800D154C->unk65 - 1];
//     }
//     foo = &D_800D2104[actor->unk13F + actor->unk21E + D_800D1893];
//     if (D_800D1891 == foo->unk0) {
//         D_800D1892 ^= 1;
//     }
//     if (D_800D1892 != 0) {
//         func_15075400(D_800D1890);
//     }
// }

void func_1507BA48(void) {
    u16 tmp = ((D_800D1892 << 8) | D_800D1893);
    if (D_800D1890 != 0) {
        tmp += func_150ADA20() % (u32) D_800D1890;
    }
    func_15060A9C(tmp, D_800D154C);
}

void func_1507BAD0(void) {
    D_800D154C->unkFC = func_1507A3E8();
}

void func_1507BAF8(void) {
    D_800D154C->unk2CC = func_1507A3E8();
}

void func_1507BB20(void) {
}

typedef struct {
    struct216 *script;
    u8 id;
} ScriptEntry;

extern ScriptEntry D_80086A88[]; // default scripts, ended by a NULL script
extern struct216 D_8009A120[];    // empty script

/* Returns script `arg1` of the current object's type, falling back to the default table. */
struct216 *func_1507BB28(s32 arg0, s32 arg1) {
    s32 typeData;
    ScriptEntry *table;
    s32 i;

    typeData = D_800D1588[D_800D154C->id];
    if (typeData == 0) {
        return D_8009A120;
    }
    table = *(ScriptEntry **)(typeData - 0x10);
    if (table == NULL) {
        return D_8009A120;
    }
    i = 0;
    while (arg1 != table[i].id) {
        if (table[i].script == NULL) {
            ScriptEntry *defaults = D_80086A88;

            i = 0;
            while (arg1 != defaults[i].id) {
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

void func_1507BC14(struct127 *arg0) {

    if (arg0->unk21C != 0) {
        if (arg0->unk218 != 0) {
            if (arg0->unk21C != 25500) {
                arg0->unk21C = arg0->unk21C - D_800CC264;
            }
            if ((arg0->unk21C >= 50000) || (arg0->unk21C == 0)) {
                arg0->unk21C = 0;
                if ((arg0->unkF4 & 0x400) != 0) {
                    arg0->unk21C = 20000;
                    arg0->unkF4 &= ~0x400;
                    arg0->unkF4 |= 4;
                }
            } else {
                return;
            }
        }
    }

    if (arg0->unk218 == 0) {
        arg0->unk218 = func_1507BB28(0, arg0->unk232);
        arg0->unk21C = 0;
    }

    while (arg0->unk21C == 0) {
        if (arg0->unk218->unk0 >= 0xF7) {
            arg0->unk218 = (u8 *)(arg0->unk218) + 1;
        } else {
            D_800D1890 = arg0->unk218->unk1;
            D_800D1891 = arg0->unk218->unk2;
            D_800D1892 = arg0->unk218->unk3;
            D_800D1893 = arg0->unk218->unk4;
            D_80086730[arg0->unk218->unk0]();
            arg0->unk218 = (u8 *)(arg0->unk218) + 5;
        }
    }
}
