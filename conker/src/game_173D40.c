#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 (*D_8008FD1C)(void);
extern u8 (*D_8008FD30)(void);

/* Get the position of one of the actor's bones (0x13 for side 1, 0x17 for side 2; any other side
   fails with 0).  With a `hit` buffer, also look for the ground below that point and return
   func_15046C80's result. */
s32 func_15146890(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side) {
    Vec3f probe;
    s32 bone;

    if ((side != 1) && (side != 2)) {
        return 0;
    }

    if (side == 1) {
        bone = 0x13;
    } else {
        bone = 0x17;
    }

    func_15142314(actor->ob_matrix_ptr, bone, pos);
    if (hit == NULL) {
        return 1;
    }

    probe.x = pos->x;
    probe.y = pos->y + 10.0f;
    probe.z = pos->z;
    func_1504715C(hit, actor);
    return func_15046C80(&probe, 0, pos->y - 50.0f, hit);
}

/* Map an actor's damage/effect kind onto the particle-effect id used for it.  Most kinds are a
   fixed id; kind 1 depends on the current world (level) and, in world 4, on whether
   func_151420F8 accepts the actor; kind 6 first has to register a five-entry effect list against
   the actor's index in movement_obj, and reports -1 if that fails.  Anything from 9 up is unmapped. */
s32 func_15146970(s32 kind, Actor *actor) {
    switch (kind) {
    case 8: return 0x10;
    case 7: return 0xF;
    case 5: return 5;
    case 2: return 2;
    case 0: return 0;
    case 1:
        if (level == 4) {
            if (func_151420F8(actor) != 0) {
                return 6;
            }
            return 3;
        }
        return 1;
    case 4: return 4;
    case 3: return 3;
    case 6:
        {
            const s32 list[5] = { 0x401D, 0x401E, 0x401F, 0x4020, 0x4021 };

            if (func_150A2AEC(actor - movement_obj, 5, list, actor) == -1) {
                return -1;
            }
        }
        return 6;
    }
    return -1;
}

/* The effect callbacks below take an actor (Actor) and a bone selector `side` for
   func_15146890, plus a strength. */

// matching: `actor` is an int cast at each use; with a pointer parameter IDO merges the two
// loads of it that the original makes (lw t6 / lw a1 from the arg slot).

/* Burst of func_15150F90 pieces from the actor's bone on `side`. */
void func_15146A98(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    PieceSpray burst;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        func_15146890(&pos, (Actor *)actor, NULL, side);
        burst.countBase = 5;
        burst.countRange = 22;
        burst.pos = pos;
        burst.speedBase = 0.024999999f;
        burst.speedRange = 0.0199f;
        burst.angles.yawBase = 0;
        burst.angles.yawRange = 255;
        burst.angles.pitchBase = -20;
        burst.angles.pitchRange = 14;
        burst.directionBase = 6.0f;
        burst.directionRange = 3.0f;
        burst.jitter = 7.9760003f;
        burst.sizeBase = -0.40300003f;
        burst.sizeRange = 0.284f;
        burst.scale = 0.95355f;
        burst.kind = 3;
        burst.lifeBase = 25;
        burst.lifeRange = 25;
        func_15150F90(&burst, 0, 0xFF, 1);
    }
}

void func_15146BB8(s32 actor, s32 side, s32 strength) {
    func_15146BF8(actor, side, strength, 0);
}

void func_15146BD8(s32 actor, s32 side, s32 strength) {
    func_15146BF8(actor, side, strength, 1);
}

/* Effect 0xB (or 0xC when `alt`) at the ground below the actor's bone on `side`, plus effect 2
   there when strength is at least 115. */
void func_15146BF8(s32 actor, s32 side, s32 strength, u8 alt) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (func_15146890(&pos, (Actor *)actor, &hit, side) != 0) {
            func_15141F78(alt != 0 ? 0xC : 0xB, &hit, strength * (1.0f / 130.0f),
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
            if (strength >= 115) {
                func_15142180(2, &pos, hit.material, 1.0f, 1.0f);
            }
        }
    }
}

/* Effect 0xA at the ground below the actor's bone on `side`. */
void func_15146D60(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (func_15146890(&pos, (Actor *)actor, &hit, side) != 0) {
            func_15141F78(0xA, &hit, strength * (1.0f / 85.0f),
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
        }
    }
}

/* Effect func_151D8E20() at the ground below the actor's bone on `side` (only when strength is at
   least 71), and a randomly sized func_151D9B8C or func_151DAB58 particle at ground height. */
void func_15146E84(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;
    u8 kind;
    Vec3f spot;
    s32 pad; // matching: an unused stack slot sits between spot and random
    s32 random;
    f32 size;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (func_15146890(&pos, (Actor *)actor, &hit, side) != 0) {
            kind = func_151D8E20();
            if (strength >= 71) {
                func_15142180(kind, &pos, hit.material, 1.0f, 1.0f);
            }

            spot.x = pos.x;
            spot.y = hit.height;
            spot.z = pos.z;

            if (func_150ADA20() & 1) {
                size = func_150ADA68();
                random = func_150ADA20();
                func_151D9B8C(kind, size * 10.0f + 10.0f, (random % 101U) + 100, &hit.tri,
                              &spot, (func_150ADA20() % 41U) + 80, 1, 1, 0, 0xFF, 1);
            } else {
                size = func_150ADA68();
                func_151DAB58(kind, size * 8.0f + 5.0f, (func_150ADA20() % 101U) + 100, &spot, 1,
                              0xFF, 1);
            }
        }
    }
}

/* func_1514C678 at the actor's bone on `side`, with a random radius below 11. */
void func_15147040(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    s16 roll;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        func_15146890(&pos, (Actor *)actor, NULL, side);
        roll = (func_150ADA20() % 11U) + 5;
        func_1514C678(pos.x, pos.y, pos.z, func_150ADA68() * 11.0f, 0, 0xFF, roll, 0x16, 0, 0.0f, 0,
                      0xFF);
    }
}

/* Spawn one func_15130374 particle at (x, y, z), flying off at a random pitch along `yaw`. */
s32 func_151470FC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw,
                  s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    ParticleDesc desc;
    TimedEffect *particle;
    f32 extra;
    f32 speed;
    u32 random;

    desc.texture = 0x29;
    desc.renderMode = 0xE03;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.timer = (func_150ADA20() & 0xF) + 20;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r2 = 0xB0;
    desc.g2 = 0xA0;
    desc.b2 = 0x2A;
    desc.r = 0x40;
    desc.g = 0xB;
    desc.b = 0x6A;
    desc.a = 0xFF;
    desc.alpha = (func_150ADA20() % 101U) + 100;
    desc.brightness = 0xFF;
    desc.primColorMode = 3;
    desc.envColorMode = 3;

    desc.width = desc.height = func_150ADA68() * 95.0f + 101.0f;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;
    random = func_150ADA20();
    speed = func_150ADA68();
    func_15143794(yaw, (random % 11U) - 11, (speed * 396.0f + 202.0f) * 0.01f, &desc.vel);
    desc.gravity = 0.0f;
    desc.flags = 0xE05;

    if (func_150ADA20() & 1) {
        desc.flags |= 0x40;
    }

    if (func_150ADA20() & 1) {
        desc.flags |= 0x80;
    }

    desc.updateFunc = 10;
    desc.scaleRate = 1.02f;
    extra = 0.94000006f;
    desc.deathFunc = -1;
    desc.fadeStart = 20;
    desc.fadeRate = 12;
    desc.scaleStart = 27;

    particle = func_15130374(&desc, 1, 4, argE, 1);
    if (particle != NULL) {
        memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
    }
    return 1;
}

/* Effect 0 at the ground below the actor's bone on `side` when strength is at least 115, then
   effect D_8008FD04[0]() there. */
void func_15147318(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (func_15146890(&pos, (Actor *)actor, &hit, side) != 0) {
            if (strength >= 115) {
                func_15142180(0, &pos, hit.material, 1.0f, 1.0f);
            }
            func_15141F78(D_8008FD04[0](), &hit, strength * (1.0f / 130.0f),
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
        }
    }
}

/* Effect D_8008FD1C() at the ground below the actor's bone on `side`, plus effect 6 there when
   strength is at least 115. */
void func_15147478(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (func_15146890(&pos, (Actor *)actor, &hit, side) != 0) {
            func_15141F78(D_8008FD1C(), &hit, strength * (1.0f / 130.0f),
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
            if (strength >= 115) {
                func_15142180(6, &pos, hit.material, 1.0f, 1.0f);
            }
        }
    }
}

/* Effect D_8008FD30() at the ground below the actor's bone on `side`, plus effect 0xB there when
   strength is at least 115. */
void func_151475D8(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (func_15146890(&pos, (Actor *)actor, &hit, side) != 0) {
            func_15141F78(D_8008FD30(), &hit, strength * (1.0f / 130.0f),
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
            if (strength >= 115) {
                func_15142180(0xB, &pos, hit.material, 1.0f, 1.0f);
            }
        }
    }
}
