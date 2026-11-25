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

f32 func_15048A40(u8 arg0); // header declares it as returning void
u32 func_150ADA20(void);    // header declares it as returning u8; callers here use the full word
f32 func_150489B0(u8 arg0);
void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2);
void func_15052760(struct127 *arg0, s16 arg1);
s32 func_150535F4(struct127 *arg0);
void func_150562FC(struct127 *arg0);
void func_15056B08(struct127 *arg0);
void func_150585F0(struct127 *arg0, f32 arg1);
void func_150597FC(struct127 *arg0);
void func_1505A770(struct127 *arg0);
struct127 *func_1505EEB0(s32 state, s32 *outIdx);
void func_1510F800(s32);
s32 func_1510F8D8(s16, s16, s16, s32);
s32 func_15167D84(void *, s32, s32, s32, s32, s32);

extern s32 D_8008CAE0;
extern u8 D_800B86A4[][0x32C];
extern f32 D_800CC2E4;
extern f32 D_800CC2EC;
extern f32 D_800D1550; // matching: as the header's f32[] the multiply in func_150B10F0 schedules differently

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
        range = obj->y_position - D_800CC2D0[0].y_position; // matching: height above the player reuses `range`
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

/* D_800D20FC is a table of 0x30-byte spawn records. */
typedef struct {
    u8 pad0[0x8];
    s16 y;
    u8 padA[0x26];
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

/* Particle spawn request copied by func_15167D84 (field names as in game_10A350.c's Game10A350Emission). */
typedef struct {
    /* 0x00 */ s32 texture;
    /* 0x04 */ s32 surface;
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ s16 frame;
    /* 0x0E */ s16 frame_rate;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 velocity_x;
    /* 0x18 */ s16 velocity_z;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s8 fraction_x;
    /* 0x1D */ s8 fraction_y;
    /* 0x1E */ s8 fraction_z;
    /* 0x1F */ s8 callback;
    /* 0x20 */ s16 velocity_y;
    /* 0x22 */ s16 acceleration;
    /* 0x24 */ s16 width;
    /* 0x26 */ s16 height;
    /* 0x28 */ s16 lifetime;
    /* 0x2A */ s8 fade;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 red;
    /* 0x2D */ u8 green;
    /* 0x2E */ u8 blue;
    /* 0x2F */ u8 alpha;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ s16 flags;
    /* 0x36 */ u8 pad36[2];
} ParticleSpawn;

/*
 * Keeps the object stunned with fixed state flags, eases unkB8 toward 0 (or -20 when unk13D is set, unk21C >= 9
 * and D_800B86A4's row clear or D_800CC250 bit 2 set), and
 * now and then emits a particle at a random point around it that drifts with the player's motion.
 */
void func_150B19E0(struct127 *obj) {
    f32 target;
    f32 radius;
    f32 speed;
    f32 sine;
    f32 cosine;
    ParticleSpawn particle;
    u32 random; // reused for both random draws
    u8 angle;

    target = 0.0f;
    obj->unkB0 = 25;
    obj->unkF8 &= ~4;
    obj->unk13C = 0;
    obj->unkF8 |= 0x80;
    if (obj->xz_velocity > 1.0f) {
        obj->unkF8 |= 4;
    }
    obj->stunned = 10;
    obj->unk239 = 3;
    obj->unkFC = 0xA7;
    obj->unk80 = 1;
    if (obj->unk25C & 0x40) {
        obj->unk25C &= ~0x40;
        obj->old_x_position = D_800CC2E4;
        obj->old_z_position = D_800CC2EC;
    }
    if (obj->unk13D != 0) {
        func_150562FC(obj);
        if ((D_800B86A4[obj->unk13D][0] == 0 || (D_800CC250 & 4)) && obj->unk21C >= 9) {
            target = -20.0f;
        }
    }
    obj->unkB8 += (target - obj->unkB8) * 0.2f;
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.0054931640625f;
    func_15059140(obj);
    func_150597FC(obj);
    func_1505E650(obj, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    func_1502178C(obj, 0, -1);
    if (obj->unk13D != 0 && (u8)func_150ADA20() < 40U) {
        random = func_150ADA20() % 40U;
        radius = obj->xz_scale * random;
        angle = func_150ADA20();
        sine = func_150489B0(angle);
        cosine = func_15048A40(angle);
        particle.frame = 0;
        particle.frame_rate = 0;
        particle.texture = D_8008CAE0;
        particle.x = obj->x_position + sine * radius;
        particle.y = obj->y_position;
        particle.z = obj->z_position + cosine * radius;
        func_1510F800(0);
        particle.surface = func_1510F8D8(particle.x, particle.y, particle.z, 0);
        angle = D_800CC2D0[0].unk76 >> 8;
        speed = D_800CC2D0[0].xz_velocity * 64.0f;
        particle.velocity_x = func_150489B0(angle) * speed;
        particle.velocity_z = func_15048A40(angle) * -speed;
        particle.unk1A = -30;
        particle.callback = 7;
        particle.velocity_y = 0;
        particle.acceleration = -80;
        random = (u8)func_150ADA20();
        particle.unk2B = random;
        particle.width = (s32)(random << 12) / 5100 + 204;
        particle.height = (s32)(random << 12) / 12750 + 81;
        particle.lifetime = 400;
        particle.fade = 0;
        particle.red = 0xFF;
        particle.green = 0xFF;
        particle.blue = 0;
        particle.alpha = 0xFF;
        particle.flags = 0;
        func_15167D84(&particle, 0, 0, -1, 0xFF, 0);
    }
}
