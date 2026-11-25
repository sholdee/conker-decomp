#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
#undef func_15048A40
#undef func_150ADA20
#define D_800D1550 D_800D1550_hdr
#define D_800D2104 D_800D2104_hdr
#include "variables.h"
#undef D_800D1550
#undef D_800D2104
extern f32 D_800D1550; // matching: as an f32[] (header type) the multiply schedules differently

/* One point of a path: s16 world coordinates (as in game_A28B0.c). */
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u16 unk6;
} PathPoint;

/* A path, D_800D2104[n]: a centre point followed by its waypoints. */
typedef struct {
    PathPoint center;
    PathPoint points[1];
} Path;

extern Path **D_800D2104; // header declares it as s32 *

/* Remembers the object's x/z, and drags a swimming player who comes within range down and sideways around it. */
void func_150B10F0(struct127 *obj) {
    f32 dist;
    f32 pushX;
    f32 pushZ;
    f32 unusedOut;
    f32 closeness;
    f32 distCopy; // matching: IDO keeps dist in a second FP register only through this copy
    f32 range;
    u16 angle;

    // 0x174/0x178 are f32 fields the shared header still declares as pads
    *(f32 *)&obj->pad174 = obj->x_position;
    *(f32 *)&obj->pad178 = obj->z_position;
    obj->immune = 100;
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
    if (func_1505A6F8(D_800CC2D0, obj) < 1100.0f) {
        dist = func_1505A6F8(obj, D_800CC2D0);
        distCopy = dist;
        range = obj->y_position - D_800CC2D0[0].y_position;
        if (range < 0.0f) {
            range = 1000.0f;
        } else {
            range = 1050.0f - range * 0.7f;
        }
        if (dist < range && D_800CC2D0[0].in_water == 1) {
            closeness = 1100.0f - distCopy;
            angle = func_1505A630(*(f32 *)&obj->pad174 - D_800CC2D0[0].x_position, D_800CC2D0[0].z_position - *(f32 *)&obj->pad178, 0);
            if (850.0f < closeness) {
                D_800CC2D0[0].y_position -= (closeness - 820.0f) * 0.09f * D_800D1550;
            }
            func_1505A184(angle - 0x3000, closeness * 0.045f + 5.0f, 0.0f, &pushX, &pushZ, &unusedOut);
            D_800CC2D0[0].unk164 = pushX;
            D_800CC2D0[0].unk168 = pushZ;
        }
    }
    func_1505E650(obj, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

void func_15056B08(struct127 *arg0);
s32  func_150535F4(struct127 *arg0);
void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2);

/* Holds the object at its path's centre height and eases unkC4 toward its turn rate (clamped to +-6). */
void func_150B12FC(struct127 *obj) {
    s16 turn;
    s16 prevAngle;
    f32 leanStep;

    prevAngle = obj->unk76;
    obj->unk80 = 10;
    {
        f32 height;

        height = D_800D2104[obj->unk13F]->center.y;
        func_15058EA4(obj, height, 1.0f, height, -0.7f, 12.0f, -16.0f);
    }
    func_15056B08(obj);
    prevAngle -= obj->unk76;
    turn = prevAngle * 6;
    leanStep = (f32)turn * 0.00390625f - obj->unkC4;
    if (leanStep > 6.0f) {
        leanStep = 6.0f;
    } else if (leanStep < -6.0f) {
        leanStep = -6.0f;
    }
    {
        f32 current = obj->unk148;

        obj->unk148 += (leanStep - current) * 0.05f;
        obj->unkC4 += obj->unk148;
    }
    obj->unk40 = (f32)(s16)(obj->unk7A + 0x4000) * 0.005493164f;
    func_15059140(obj);
    if (func_150535F4(obj) == 0) {
        func_1502178C(obj, 0, -1);
    }
}

void func_15056B08(struct127 *arg0);
void func_150585F0(struct127 *arg0, f32 arg1);
void func_15052760(struct127 *arg0, s16 arg1);
s32  func_150535F4(struct127 *arg0);
void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2);

/* D_800D20FC is a table of 0x30-byte spawn records. */
typedef struct {
    char pad_0[0x8];
    s16  y;
    char pad_A[0x26];
} SpawnRecord;

/*
 * Moves the object vertically toward a target height: its spawn record's y + 210 by default, or (unk223 == 14)
 * straight at its target object D_800CC2D0[unk222]. A nonzero unk224 overrides the height: 10000 means its
 * current path waypoint, anything else is unk224 * 8 (plus unk180 for most ids). Stunned objects only recover.
 */
void func_150B1484(struct127 *obj) {
    s16 prevAngle;

    prevAngle = obj->unk76;
    obj->unk80 = 10;
    if (obj->stunned == 0) {
        f32 targetY;

        targetY = (f32)((SpawnRecord *)D_800D20FC)[obj->unk13F].y + 210.0f;
        if (!(obj->unk22C & 8)) {
            if (obj->unk223 != 3) {
                obj->unkB8 = obj->y_velocity * -0.5f;
            } else {
                obj->unkB8 = 0.0f;
            }
        }
        if (obj->unk223 == 14) {
            f32 time;

            time = func_1505A6F8(&D_800CC2D0[obj->unk222], obj) / obj->xz_velocity;
            obj->y_velocity = (D_800CC2D0[obj->unk222].y_position + (s16)obj->unk224 - obj->y_position) / time;
            obj->gravity = 0.0f;
        } else {
            s16 heightCode;

            if (obj->unk223 == 1) {
                targetY = D_800CC2D0[0].y_position + 210.0f;
            }
            heightCode = (s16)obj->unk224;
            if (heightCode != 0) {
                if (heightCode == 10000) {
                    targetY = D_800D2104[obj->unk13F]->points[obj->unk21E].y;
                } else if ((obj->id == 0x50) || (obj->id == 5) || (obj->id == 0xAD) || (obj->id == 0xAE) || (obj->id == 0xAF)) {
                    targetY = heightCode * 8;
                } else {
                    targetY = heightCode * 8 + obj->unk180;
                }
            }
            if (!(obj->unk100 & 0x40)) {
                f32 gravity;
                f32 maxSpeed;

                if (obj->id == 0x44) {
                    targetY = D_800D2104[obj->unk13F]->points[obj->unk21E].y;
                    gravity = 3.0f;
                    maxSpeed = 12.0f;
                } else {
                    gravity = 5.0f;
                    maxSpeed = 35.0f;
                }
                func_15058EA4(obj, targetY, gravity, targetY, -gravity, maxSpeed, -maxSpeed);
            }
        }
        func_15056B08(obj);
    } else {
        func_150585F0(obj, 0.05f);
    }
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.0054931640625f;
    func_15059140(obj);
    if ((obj->id == 5) || (obj->id == 0xAD) || (obj->id == 0xAE) || (obj->id == 0xAF) || (obj->id == 0x44)) {
        func_15052760(obj, prevAngle);
    }
    if (func_150535F4(obj) == 0) {
        func_1502178C(obj, 0, -1);
    }
}

struct127 *func_1505EEB0(s32 state, s32 *outIdx);
void func_15056B08(struct127 *arg0);
void func_1505A770(struct127 *arg0);

/*
 * Pairs the object with the first actor in state 10 (`other`). With unk1E5 == 2 the object sits on top of
 * `other`; otherwise `other` hangs 170 below the object, swaying (unkB8) with the phase unk7E and spinning
 * with the frame step D_800BE9A0. Either way the object then faces the player.
 */
void func_150B17DC(struct127 *obj) {
    s32 otherIdx;
    f32 phase;
    f32 offsetX;
    f32 offsetZ;
    f32 unusedOut;
    struct127 *other;

    other = func_1505EEB0(10, &otherIdx);
    func_15056B08(obj);
    if (obj->unk1E5 == 2) {
        obj->x_position = other->x_position;
        obj->z_position = other->z_position;
        obj->y_position = other->y_position + (obj->unk21C / 5 + 160.0f);
        obj->y_velocity = 0.0f;
        obj->gravity = -0.35f;
        obj->unk7E = 0;
    } else {
        phase = obj->unk7E;
        phase *= 0.2f;
        other->unkB8 = func_150AD78C(phase) * 6.0f;
        func_1505A770(obj);
        other->y_position = obj->y_position - 170.0f;
        other->unk13C = D_800C3E78 + 100;
        func_1505A184(other->unk7A, other->unkB8 * -7.0f, 0.0f, &offsetX, &offsetZ, &unusedOut);
        other->x_position = obj->x_position + offsetX;
        other->z_position = obj->z_position + offsetZ;
        other->unk7A += D_800BE9A0 << 8;
        obj->unk7E += D_800BE9A0;
        other->in_water = 0;
        other->xz_velocity = 0.0f;
        other->unk16C = 0.0f;
        other->unk170 = 0.0f;
    }
    obj->unk7A = func_1505A630(D_800CC2D0[0].x_position - obj->x_position, obj->z_position - D_800CC2D0[0].z_position, 0);
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.0054931640625f;
}

typedef struct {
    s32 flags;
    s32 unk4;
    u8 pad08[4];
    s16 unkC, unkE;
    s16 x, y, z, velocityX, velocityZ;
    s16 unk1A;
    s8 fractionX, fractionY, fractionZ, mode;
    s16 velocityY, size, unk24, unk26, life;
    s8 fade;
    u8 unk2B;
    u8 red, green, blue, alpha;
    u8 pad30[4];
    s16 kind;
} Packet150B19E0;

extern f32 func_15048A40(u8 arg0);
extern u32 func_150ADA20(void);
extern f32 func_150489B0(u8 arg0);
extern void func_150562FC(struct127 *arg0);
extern void func_150597FC(struct127 *arg0);
extern void func_1510F800(s32);
extern s32 func_1510F8D8(s16, s16, s16, s32);
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);
extern s32 D_8008CAE0;
extern f32 D_800CC2E4;
extern f32 D_800CC2EC;
extern u8 D_800B86A4[][0x32C];

void func_150B19E0(struct127 *arg0) {
    f32 target;
    f32 dist;
    f32 speed;
    f32 sine;
    f32 cosine;
    Packet150B19E0 packet;
    u32 r;
    u8 angle;

    target = 0.0f;
    arg0->unkB0 = 0x19;
    arg0->unkF8 &= ~4;
    arg0->unk13C = 0;
    arg0->unkF8 |= 0x80;
    if (arg0->xz_velocity > 1.0f) {
        arg0->unkF8 |= 4;
    }
    arg0->stunned = 0xA;
    arg0->unk239 = 3;
    arg0->unkFC = 0xA7;
    arg0->unk80 = 1;
    if (arg0->unk25C & 0x40) {
        arg0->unk25C &= ~0x40;
        arg0->old_x_position = D_800CC2E4;
        arg0->old_z_position = D_800CC2EC;
    }
    if (arg0->unk13D != 0) {
        func_150562FC(arg0);
        if ((D_800B86A4[arg0->unk13D][0] == 0 || (D_800CC250 & 4)) && arg0->unk21C >= 9) {
            target = -20.0f;
        }
    }
    arg0->unkB8 += (target - arg0->unkB8) * 0.200000003f;
    arg0->unk40 = (s16)(arg0->unk7A + 0x4000) * 0.0054931640625f;
    func_15059140(arg0);
    func_150597FC(arg0);
    func_1505E650(arg0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    func_1502178C(arg0, 0, -1);
    if (arg0->unk13D != 0 && (u8)func_150ADA20() < 40U) {
        r = func_150ADA20() % 40U;
        dist = arg0->xz_scale * r;
        angle = func_150ADA20();
        sine = func_150489B0(angle);
        cosine = func_15048A40(angle);
        packet.unkC = 0;
        packet.unkE = 0;
        packet.flags = D_8008CAE0;
        packet.x = arg0->x_position + sine * dist;
        packet.y = arg0->y_position;
        packet.z = arg0->z_position + cosine * dist;
        func_1510F800(0);
        packet.unk4 = func_1510F8D8(packet.x, packet.y, packet.z, 0);
        angle = D_800CC2D0[0].unk76 >> 8;
        speed = D_800CC2D0[0].xz_velocity * 64.0f;
        packet.velocityX = func_150489B0(angle) * speed;
        packet.velocityZ = func_15048A40(angle) * -speed;
        packet.unk1A = -30;
        packet.mode = 7;
        packet.velocityY = 0;
        packet.size = -80;
        r = (u8)func_150ADA20();
        packet.unk2B = r;
        packet.unk24 = (s32)(r << 12) / 5100 + 0xCC;
        packet.unk26 = (s32)(r << 12) / 12750 + 0x51;
        packet.life = 400;
        packet.fade = 0;
        packet.red = 0xFF;
        packet.green = 0xFF;
        packet.blue = 0;
        packet.alpha = 0xFF;
        packet.kind = 0;
        func_15167D84(&packet, 0, 0, -1, 0xFF, 0);
    }
}
