#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/*
 * Most configs below store random ranges as base/range pairs: an integer field is
 * (func_150ADA20() % (range + 1)) + base and a float field is func_150ADA68() * range + base
 * (a random float). Angles are 256-step binary angles; func_151423D8 is their cosine.
 */
/* Vector helpers (game_16EE20.c). */
extern s32 func_15144E80(s16 *, Vec3f *, Vec3f *, Vec3f *); /* triangle edges + normal */
extern s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);          /* normalize */
extern f32 func_15144A74(Vec3f *, Vec3f *);                        /* dot product */
extern s32 func_15146078(Vec3f *, Vec3f *, Vec3f *);       /* two axes perpendicular to a direction */
extern void func_15143794(s16, s16, f32, f32 *); /* (yaw, pitch, length) -> vector */
extern void func_15143874(s32, f32, f32 *, f32 *);

/* A random direction inside a cone around `direction`, built lazily (see func_1514F6E8). */
struct ConeBasis {
    u8 flags; /* 1: direction normalized and scaled, 2: axes built, 4: axes normalized */
    u8 pad1[3];
    Vec3f direction;
    Vec3f axis0;
    Vec3f axis1;
    f32 spread;
};

/* A cone emitter described by a surface triangle (three s16 vertices) and the side its normal faces. */
struct TriangleSource {
    Vec3f normal;
    s16 triangle[9];
    f32 spread;
};

/* Builds a cone basis from a triangle: the axes are two triangle edges, the direction is its normal,
 * flipped to face the same side as source->normal. */
void func_1514F640(struct TriangleSource *source, struct ConeBasis *cone) {
    cone->flags = 2;
    cone->spread = source->spread;
    if (func_15144E80(source->triangle, &cone->axis0, &cone->axis1, &cone->direction) != 0) {
        if (func_15144A74(&cone->direction, &source->normal) < 0.0f) {
            cone->direction.x = -cone->direction.x;
            cone->direction.y = -cone->direction.y;
            cone->direction.z = -cone->direction.z;
        }
    }
}

/* Finishes whatever parts of the cone basis are still missing; returns 0 if a vector is degenerate. */
s32 func_1514F6E8(struct ConeBasis *cone) {
    if (!(cone->flags & 1)) {
        if (func_15145128(&cone->direction, &cone->direction, NULL, NULL) == 0) {
            return 0;
        }
        cone->direction.x *= 1000.0f;
        cone->direction.y *= 1000.0f;
        cone->direction.z *= 1000.0f;
        cone->flags |= 1;
    }
    if (!(cone->flags & 2)) {
        if (func_15146078(&cone->direction, &cone->axis0, &cone->axis1) == 0) {
            return 0;
        }
        cone->flags |= 2 | 4;
    }
    if (!(cone->flags & 4)) {
        if (func_15145128(&cone->axis0, &cone->axis0, NULL, NULL) == 0) {
            return 0;
        }
        if (func_15145128(&cone->axis1, &cone->axis1, NULL, NULL) == 0) {
            return 0;
        }
        cone->flags |= 4;
    }
    return 1;
}

/* Picks a random direction inside the cone and scales it by `speed`. */
void func_1514F808(struct ConeBasis *cone, f32 speed, Vec3f *out) {
    f32 second;
    f32 first;

    func_15143874((s16)(func_150ADA20() & 0xFF), func_150ADA68() * cone->spread, &first, &second);
    out->x = (((cone->axis0.x * first) + (cone->axis1.x * second)) + cone->direction.x) * speed;
    out->y = (((cone->axis0.y * first) + (cone->axis1.y * second)) + cone->direction.y) * speed;
    out->z = (((cone->axis0.z * first) + (cone->axis1.z * second)) + cone->direction.z) * speed;
}

struct ConeSprayConfig {
    s32 countBase;
    s32 countRange;
    Vec3f position;
    f32 speedBase;
    f32 speedRange;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s8 unk50;
};

/* An emitter aimed along a direction (func_1514FB98). */
struct DirectionEmitter {
    Vec3f direction;
    f32 spread;
    struct ConeSprayConfig config;
};

/* An emitter aimed off a surface triangle (func_1514FBFC). */
struct TriangleEmitter {
    struct TriangleSource source;
    struct ConeSprayConfig config;
};

extern void func_151A2AD4(Vec3f *, Vec3f *, f32, f32, s32, f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s8, u8, s32);

/* Spawns a random number of particles from config->position, each moving in a random direction inside
 * the cone around `direction` (scaled by 1000 here) spanned by axis0/axis1. */
void func_1514F8F8(struct ConeSprayConfig *config, Vec3f *direction, Vec3f *axis0, Vec3f *axis1, f32 spread, u8 arg5, s32 arg6) {
    s32 count;
    Vec3f velocity;
    f32 second;
    f32 first;
    f32 speed;

    direction->x *= 1000.0f;
    direction->y *= 1000.0f;
    direction->z *= 1000.0f;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        func_15143874((s16)(func_150ADA20() & 0xFF), func_150ADA68() * spread, &first, &second);
        velocity.x = ((axis0->x * first) + (axis1->x * second) + direction->x) * speed;
        velocity.y = ((axis0->y * first) + (axis1->y * second) + direction->y) * speed;
        velocity.z = ((axis0->z * first) + (axis1->z * second) + direction->z) * speed;
        func_151A2AD4(&config->position, &velocity,
                      (func_150ADA68() * config->unk20) + config->unk1C,
                      (func_150ADA68() * config->unk28) + config->unk24,
                      (func_150ADA20() % (u32)(config->unk30 + 1)) + config->unk2C,
                      config->unk34, config->unk38, config->unk3C, config->unk40,
                      config->unk44, config->unk46, config->unk48, config->unk4A,
                      config->unk4C, config->unk4E, config->unk50, arg5, arg6);
        count--;
    }
}

void func_1514FB98(struct DirectionEmitter *emitter, u8 arg1, s32 arg2) {
    Vec3f axis0;
    Vec3f axis1;

    if (func_15146078(&emitter->direction, &axis0, &axis1) != 0) {
        func_1514F8F8(&emitter->config, &emitter->direction, &axis0, &axis1, emitter->spread, arg1, arg2);
    }
}

void func_1514FBFC(struct TriangleEmitter *emitter, u8 arg1, s32 arg2) {
    Vec3f axis0;
    f32 length0;
    f32 inverse0;
    Vec3f axis1;
    f32 length1;
    f32 inverse1;
    Vec3f normal;
    f32 normalLength;
    f32 normalInverse;

    if (func_15144E80(emitter->source.triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &emitter->source.normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        func_1514F8F8(&emitter->config, &normal, &axis0, &axis1, emitter->source.spread, arg1, arg2);
    }
}

struct Local1514FCE8 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s32 countBase;
    s32 countRange;
    Vec3f position;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    s32 unk34;
    s32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s8 unk58;
};

extern void func_151A2A14(Vec3f *, s16, s16, f32, f32, f32, s32, f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s8, u8, s32);

void func_1514FCE8(struct Local1514FCE8 *config, u8 arg1, s32 arg2) {
    s32 count;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        func_151A2A14(&config->position,
                      (s16)((func_150ADA20() % (u32)(config->unk2 + 1)) + config->unk0),
                      (s16)((func_150ADA20() % (u32)(config->unk6 + 1)) + config->unk4),
                      (func_150ADA68() * config->unk20) + config->unk1C,
                      (func_150ADA68() * config->unk28) + config->unk24,
                      (func_150ADA68() * config->unk30) + config->unk2C,
                      (func_150ADA20() % (u32)(config->unk38 + 1)) + config->unk34,
                      config->unk3C, config->unk40, config->unk44, config->unk48,
                      config->unk4C, config->unk4E, config->unk50,
                      config->unk52, config->unk54, config->unk56,
                      config->unk58, arg1, arg2);
        count--;
    }
}

struct Local1514FF44Arg {
    Vec3f position;
    s16 countBase;
    s16 countRange;
    f32 speedBase;
    f32 speedRange;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[2];
    f32 unk28;
    f32 unk2C;
    u8 pad30;
    u8 unk31;
    u8 pad32[2];
    f32 unk34; /* probability */
    u8 unk38;
    u8 pad39[3];
    f32 unk3C; /* probability */
};

extern void func_1514FF44(struct ConeBasis *, struct Local1514FF44Arg *, s32, u8, s32);
extern void func_151D9014(Vec3f *, Vec3f *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, u8, s32);

void func_1514FEFC(struct TriangleSource *source, struct Local1514FF44Arg *config, s32 arg2, u8 arg3, s32 arg4) {
    struct ConeBasis cone;

    func_1514F640(source, &cone);
    func_1514FF44(&cone, config, arg2, arg3, arg4);
}

/* Spawns a random number of particles, each moving in a random direction inside the cone. */
void func_1514FF44(struct ConeBasis *cone, struct Local1514FF44Arg *config, s32 arg2, u8 arg3, s32 arg4) {
    s32 count;
    Vec3f velocity;

    if (func_1514F6E8(cone) != 0) {
        count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
        while (count != 0) {
            func_1514F808(cone, (func_150ADA68() * config->speedRange) + config->speedBase, &velocity);
            func_151D9014(&config->position, &velocity, config->unk31,
                          (func_150ADA68() * config->unk20) + config->unk1C,
                          (func_150ADA20() % (u32)(config->unk1A + 1)) + config->unk18,
                          (func_150ADA20() % (u32)(config->unk25 + 1)) + config->unk24,
                          (func_150ADA68() * config->unk2C) + config->unk28,
                          func_150ADA68() < config->unk34, 1.4f, 1.4f, 1, arg2, config->unk38,
                          func_150ADA68() < config->unk3C, arg3, arg4);
            count--;
        }
    }
}

/* Like func_1514FF44, but each direction is a random yaw/pitch from `angles`. */
void func_15150178(AngleRange *angles, struct Local1514FF44Arg *config, s32 arg2, u8 arg3, s32 arg4) {
    s32 count;
    Vec3f velocity;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        func_15143794((s16)((func_150ADA20() % (u32)(angles->yawRange + 1)) + angles->yawBase),
                      (s16)((func_150ADA20() % (u32)(angles->pitchRange + 1)) + angles->pitchBase),
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &velocity.x);
        func_151D9014(&config->position, &velocity, config->unk31,
                      (func_150ADA68() * config->unk20) + config->unk1C,
                      (func_150ADA20() % (u32)(config->unk1A + 1)) + config->unk18,
                      (func_150ADA20() % (u32)(config->unk25 + 1)) + config->unk24,
                      (func_150ADA68() * config->unk2C) + config->unk28,
                      func_150ADA68() < config->unk34, 1.4f, 1.4f, 1, arg2, config->unk38,
                      func_150ADA68() < config->unk3C, arg3, arg4);
        count--;
    }
}

struct Arg15150400 {
    s32 countBase;
    s32 countRange;
    Vec3f origin;
    f32 speedBase;
    f32 speedRange;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    f32 scaleBase;
    f32 scaleRange;
    f32 spin; /* spawn.spin.x/z are random in [-spin, spin] */
    s32 unk34;
    f32 unk38; /* probability */
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    s32 *variantIds;
    f32 *variantScales;
    u32 variantCount;
    f32 radius;
    u8 flags;
    u8 unk59;
    u8 pad5A[2];
    s32 extraSize;
    void *extraData;
};

/* Spawn descriptor for func_1513264C. */
typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    Vec3f rotation; /* degrees */
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    Vec3f position;
    Vec3f velocity;
    Vec3f spin;
    f32 unk4C;
    s32 flags;
    s16 unk54;
    s16 variantId;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    s8 unk69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    u8 pad76[2];
    s32 unk78;
} Spawn1513264C;

extern void *func_1513264C(Spawn1513264C *spawn, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);

/* Spawns a random number of pieces in random directions around config->origin; each picks a random
 * entry of the variant tables and gets a copy of config->extraData. */
void func_15150400(struct Arg15150400 *config, AngleRange *angles, u8 arg2, s32 arg3) {
    s32 count;
    Spawn1513264C spawn;
    s16 yaw;
    s16 pitch;

    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.spin.y = 0.0f;
    spawn.flags = 0x1 | 0x8 | 0x20 | 0x40 | 0x80 | 0x100 | ((config->flags & 1) ? 0x800 : 0) | 0x2000 | 0x10000;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk62 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 2;
    spawn.unk69 = -1;
    spawn.unk6A = 0;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 12;
    spawn.unk74 = 21;
    spawn.unk78 = 0;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (u32)(angles->yawRange + 1)) + angles->yawBase;
        pitch = (func_150ADA20() % (u32)(angles->pitchRange + 1)) + angles->pitchBase;
        if (func_150ADA68() < config->unk38) {
            spawn.unk61 = 13;
            spawn.unk63 = config->unk59;
        } else {
            spawn.unk61 = 8;
            spawn.unk63 = 0;
        }
        spawn.unk04 = (func_150ADA68() * config->unk40) + config->unk3C;
        spawn.rotation.x = func_150ADA68() * 360.0f;
        spawn.rotation.y = func_150ADA68() * 360.0f;
        spawn.rotation.z = func_150ADA68() * 360.0f;
        func_15143794(yaw, pitch, (func_150ADA68() * config->speedRange) + config->speedBase, &spawn.velocity.x);
        spawn.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.unk4C = (func_150ADA68() * config->unk20) + config->unk1C;
        spawn.unk54 = (func_150ADA20() % (u32)(config->unk26 + 1)) + config->unk24;
        func_15143794(yaw, pitch, config->radius, &spawn.position.x);
        spawn.position.x += config->origin.x;
        spawn.position.y += config->origin.y;
        spawn.position.z += config->origin.z;
        {
        u32 variant;
        void *piece;
        variant = func_150ADA20() % config->variantCount;
        spawn.variantId = config->variantIds[variant];
        spawn.unk0C = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * config->variantScales[variant];
        spawn.unk08 = spawn.unk0C;
        spawn.unk00 = config->unk44 * spawn.unk08;
        piece = func_1513264C(&spawn, (config->flags & 1) ? 3 : 0, 0xFF, config->unk34, config->extraSize, arg2, arg3);
        if (config->extraSize > 0 && piece != NULL) {
            memcpy((u8 *)piece + 0x170, config->extraData, config->extraSize);
        }
        }
        count--;
    }
}

struct Arg1515080C {
    s32 countBase;
    s32 countRange;
    Vec3f origin;
    AngleRange angles;
    f32 speedBase;
    f32 speedRange;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 scaleBase;
    f32 scaleRange;
    f32 spin;
    f32 unk3C; /* probability */
    u8 unk40;
};

/* Like func_15150400, but the variant tables are passed in and each variant is used at most once. */
void func_1515080C(struct Arg1515080C *config, s32 *variantIds, f32 *variantScales, s32 variantCount, void *extraData, s32 extraSize, u8 arg6, u8 arg7, s8 arg8, s32 arg9, f32 radius, u8 argB, u8 argC, u8 argD, s32 argE) {
    u8 *used;
    Spawn1513264C spawn;
    s32 remaining;
    s32 count;
    s32 slot;
    s32 done;
    s16 yaw;
    s16 pitch;

    used = allocate_memory(variantCount, 2, 2, 1);
    if (used != NULL) {
        {
        s32 i;
        for (i = 0; i < variantCount; i++) {
            used[i] = FALSE;
        }
        }
        remaining = variantCount;
        spawn.unk00 = 1.0f;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.unk04 = 0.5610000491f;
        spawn.spin.y = 0.0f;
        spawn.flags = 0x8 | 0x20 | 0x40 | (arg7 ? 0x80 : 0) | 0x100 | 0x800 | 0x1000 | 0x2000 | 0x10000 | (argB ? 0x4000 : 0) | (argC ? 0x100000 : 0);
        count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
        if (variantCount < count) {
            count = variantCount;
        }
        spawn.unk58 = arg6;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk62 = 0;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 2;
        spawn.unk6A = 0;
        spawn.unk6C = 0;
        spawn.unk70 = 0;
        spawn.unk72 = 12;
        spawn.unk74 = 21;
        spawn.unk69 = arg8;
        while (count != 0) {
            yaw = (func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase;
            pitch = (func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase;
            func_15143794(yaw, pitch, (func_150ADA68() * config->speedRange) + config->speedBase, &spawn.velocity.x);
            func_15143794(yaw, pitch, radius, &spawn.position.x);
            spawn.position.x += config->origin.x;
            spawn.position.y += config->origin.y;
            spawn.position.z += config->origin.z;
            spawn.rotation.x = func_150ADA68() * 360.0f;
            spawn.rotation.y = func_150ADA68() * 360.0f;
            spawn.rotation.z = func_150ADA68() * 360.0f;
            spawn.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.unk4C = (func_150ADA68() * config->unk28) + config->unk24;
            spawn.unk54 = (func_150ADA20() % (u32)(config->unk2E + 1)) + config->unk2C;
            {
            /* pick the skip-th unused variant, wrapping around */
            u32 skip;
            skip = func_150ADA20() % (u32)remaining;
            slot = 0;
            done = FALSE;
            do {
                while (used[slot]) {
                    slot++;
                }
                if (skip != 0) {
                    skip--;
                    slot++;
                    if (slot >= variantCount) {
                        slot = 0;
                    }
                } else {
                    done = TRUE;
                }
            } while (!done);
            }
            spawn.variantId = variantIds[slot];
            spawn.unk0C = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * variantScales[slot];
            remaining--;
            spawn.unk08 = spawn.unk0C;
            used[slot] = TRUE;
            {
            void *piece;
            if (func_150ADA68() < config->unk3C) {
                spawn.flags |= 1;
                spawn.unk63 = config->unk40;
                spawn.unk61 = 13;
            } else {
                spawn.flags &= ~1;
                spawn.unk63 = 0;
                spawn.unk61 = 8;
            }
            piece = func_1513264C(&spawn, 3, 0xFF, arg9, extraSize, argD, argE);
            if (extraData != NULL && piece != NULL) {
                memcpy((u8 *)piece + 0x170, extraData, extraSize);
            }
            }

            count--;
        }
    }
}
struct Local15150D1CArg {
    s32 countBase;
    s32 countRange;
    Vec3f origin;
    AngleRange angles;
    f32 maxDistance;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    s16 unk2A;
    u8 unk2C;
};

extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *, f32 *, f32 *, f32 *, f32 *, s32 *, s32, f32);
extern s32 func_15145C90(s32);

/* Casts a random number of rays from config->origin and spawns a decal (func_151D9B8C) on each
 * suitable surface hit closer than config->maxDistance. */
void func_15150D1C(struct Local15150D1CArg *config, u8 arg1, s32 arg2) {
    s16 *hitTriangle;
    Vec3f direction;
    s16 triangle[9];
    Vec3f hit;
    f32 distance;
    s32 surface;
    s32 count;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    hitTriangle = triangle; // matching: IDO keeps the buffer address in a saved register only via a variable
    while (count != 0) {
        func_15143794((s16)((func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase),
                      (s16)((func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase),
                      100.0f, &direction.x);
        if (func_150AC9C0(config->origin.x, config->origin.y, config->origin.z,
                          direction.x, direction.y, direction.z, NULL, hitTriangle,
                          &hit.x, &hit.y, &hit.z, &distance, &surface, 0, 0.0f) != 0
            && func_15145C90(surface) != 0 && distance < config->maxDistance) {
            func_151D9B8C(config->unk2C, (func_150ADA68() * config->unk24) + config->unk20,
                          (func_150ADA20() % 156U) + 100, (s32)hitTriangle, &hit,
                          (func_150ADA20() % (u32)(config->unk2A + 1)) + config->unk28,
                          1, 1, 1, arg1, arg2);
        }
        count--;
    }
}

typedef struct {
    s32 countBase, countRange;
    Vec3f position;
    f32 speedBase, speedRange;
    AngleRange angles;
    f32 directionBase, directionRange, jitter;
    f32 sizeBase, sizeRange, scale;
    u8 kind, pad3D;
    s16 lifeBase, lifeRange;
} Game150F90Config;

void *func_1518A3C0(Vec3f *, Vec3f *, f32, Vec3f *, Vec3f *, f32, f32, u8, s16, u8, s32, u8, s32);

/* Spawns a random number of randomly rotated pieces (func_1518A3C0) flying out of config->position. */
void func_15150F90(Game150F90Config *config, u8 arg1, u8 arg2, s32 arg3) {
    Vec3f jitter;
    f32 width;
    s32 count;
    Vec3f rotation;
    Vec3f direction;

    width = config->jitter + config->jitter;
    jitter.y = 0.0f;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        rotation.x = func_150ADA68() * 360.0f;
        rotation.y = func_150ADA68() * 360.0f;
        rotation.z = func_150ADA68() * 360.0f;
        func_15143794((s16)((func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase),
                      (s16)((func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase),
                      func_150ADA68() * config->directionRange + config->directionBase,
                      &direction.x);
        jitter.x = func_150ADA68() * width - config->jitter;
        jitter.z = func_150ADA68() * width - config->jitter;
        func_1518A3C0(&config->position, &rotation,
                      func_150ADA68() * config->speedRange + config->speedBase,
                      &direction, &jitter,
                      func_150ADA68() * config->sizeRange + config->sizeBase,
                      config->scale, config->kind,
                      (func_150ADA20() % (u32)(config->lifeRange + 1)) + config->lifeBase,
                      arg1, 0, arg2, arg3);
        count--;
    }
}

typedef struct {
    s16 countBase;
    s16 countRange;
    Vec3f position;
    Vec3f normal;
    s16 triangle[9];
    u8 pad2E[2];
    f32 spread;
    u8 unk34;
    u8 unk35;
    u16 unk36;
    s16 unk38;
    s16 unk3A;
    f32 scaleBase;
    f32 scaleRange;
    s32 flags;
    u8 unk48;
    u8 unk49;
    u8 pad4A[2];
    f32 unk4C;
    f32 unk50;
    f32 speedBase;
    f32 speedRange;
    u8 unk5C;
    u8 unk5D;
    u8 pad5E[2];
    s32 unk60;
    s32 unk64;
    s16 unk68;
    s16 unk6A;
    f32 unk6C;
    f32 spin; /* extra.spin.x/z are random in [-spin, spin] */
} Local151511FCArg;

/* Spawn descriptor for func_1513D2F0. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    Vec3f position;
    Vec3f rotation; /* degrees */
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 flags;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    s32 pad4C;
    s32 pad50;
    s16 unk54;
    s16 unk56;
} Local151511FCDesc;

/* Copied into the spawned object at +0x110. */
typedef struct {
    Vec3f velocity;
    f32 unkC;
    f32 unk10;
    Vec3f spin;
} Local151511FCExtra;

/* Spawns a random number of objects off a surface triangle, each moving in a random direction inside
 * the cone around the triangle's normal. */
void func_151511FC(Local151511FCArg *config, u8 arg1, s32 arg2) {
    s32 count;
    Vec3f axis0;
    f32 length0;
    f32 inverse0;
    Vec3f axis1;
    f32 length1;
    f32 inverse1;
    Vec3f normal;
    f32 normalLength;
    f32 normalInverse;
    Local151511FCDesc desc;
    Local151511FCExtra extra;
    f32 speed;
    f32 second;
    f32 first;
    void *object;

    desc.unk54 = config->unk68;
    desc.unk56 = config->unk6A;
    desc.unk0 = config->unk34;
    desc.unk1 = config->unk35;
    desc.unk2 = config->unk36;
    desc.unk8 = 0;
    desc.unkC = 0;
    desc.unk10 = 0xFF;
    desc.unk11 = 0xFF;
    desc.unk12 = 0xFF;
    desc.unk13 = 0xFF;
    desc.position = config->position;
    desc.unk34 = 1.0f;
    desc.unk38 = 1.0f;
    desc.unk3C = 1.0f;
    desc.flags = config->flags | 0x08000000;
    desc.unk45 = 0xFF;
    desc.unk46 = config->unk5C;
    desc.unk47 = config->unk5D;
    extra.unk10 = config->unk6C;
    if (func_15144E80(config->triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &config->normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        extra.spin.y = 0.0f;
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
        while (count != 0) {
            func_15143874((s16)(func_150ADA20() & 0xFF), func_150ADA68() * config->spread, &first, &second);
            speed = (func_150ADA68() * config->speedRange) + config->speedBase;
            desc.unk4 = (func_150ADA68() * config->unk3A) + config->unk38;
            desc.unk14 = desc.unk18 = (func_150ADA68() * config->scaleRange) + config->scaleBase;
            desc.rotation.x = func_150ADA68() * 360.0f;
            desc.rotation.y = func_150ADA68() * 360.0f;
            desc.rotation.z = func_150ADA68() * 360.0f;
            desc.unk44 = (func_150ADA20() % (u32)(config->unk49 + 1)) + config->unk48;
            extra.velocity.x = ((axis0.x * first) + (axis1.x * second) + normal.x) * speed;
            extra.velocity.y = ((axis0.y * first) + (axis1.y * second) + normal.y) * speed;
            extra.velocity.z = ((axis0.z * first) + (axis1.z * second) + normal.z) * speed;
            extra.unkC = (func_150ADA68() * config->unk50) + config->unk4C;
            extra.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            extra.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            // matching: the comma expression makes IDO load &D_800A4AA0 inside the loop's register plan
            object = func_1513D2F0(&desc, (0, &D_800A4AA0), 25, 0, 0, 23, ((func_150ADA20() & 1) * 2) + (func_150ADA20() & 1),
                                   config->unk60, config->unk64, 32, arg1, arg2);
            if (object != NULL) {
                memcpy((u8 *)object + 0x110, &extra, sizeof(extra));
            }
            count--;
        }
    }
}

/* First two arguments of func_15147DA0. */
typedef struct {
    Vec3f position;
    s16 life;
    s16 enabled;
    s32 active;
    u8 pad14;
    u8 fade;
    u8 pad16[2];
    s32 unk18;
} Spawn15147DA0;

typedef struct {
    f32 size;
    Vec3f velocity;
    f32 growth;
    u8 pad14[4];
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    s16 unk1C;
    s16 unk1E;
} Motion15147DA0;

/* The block func_15147DA0 takes by pointer (its 12th argument). */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
} Params15147DA0;

extern s32 func_15147DA0(Spawn15147DA0 *, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32, u8, s32);

typedef struct {
    Vec3f position;
    Vec3f normal;
    s16 triangle[9];
    u8 pad2A[2];
    f32 spread;
    f32 sizeBase, sizeRange, growthBase, growthRange, speedBase, speedRange;
    s16 countBase, countRange, fadeBase, fadeRange, lifeBase, lifeRange;
    u8 green, enabled, red, pad57;
    s32 unk58[6];
    u8 blue, alphaBase, alphaRange, pad73;
    s32 unk74, unk78;
    Params15147DA0 params;
    s16 unk9C, unk9E;
    s32 unkA0;
} Game151670Config;

/* Spawns a random number of particles off a surface triangle, each moving in a random direction
 * inside the cone around the triangle's normal. */
void func_15151670(Game151670Config *config, u8 arg1, s32 arg2) {
    Spawn15147DA0 spawn;
    Motion15147DA0 motion;
    u8 kind;
    Vec3f axis0;
    f32 length0, inverse0;
    Vec3f axis1;
    f32 length1, inverse1;
    Vec3f normal;
    f32 normalLength, normalInverse;
    f32 speed;
    f32 second;
    f32 first;
    s32 count;
    u32 rand;

    kind = arg1; // matching: IDO keeps arg1 in a saved register only through a copy
    if (func_15144E80(config->triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &config->normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        spawn.active = 1;
        spawn.enabled = config->enabled;
        motion.green = config->green;
        motion.blue = config->blue;
        motion.red = config->red;
        motion.unk1C = config->unk9C;
        motion.unk1E = config->unk9E;
        spawn.position = config->position;
        spawn.unk18 = config->unkA0;
        count = func_150ADA20() % (u32)(config->countRange + 1) + config->countBase;
        while (count != 0) {
            rand = func_150ADA20();
            func_15143874((s16)(rand & 0xFF), func_150ADA68() * config->spread, &first, &second);
            speed = func_150ADA68() * config->speedRange + config->speedBase;
            motion.alpha = func_150ADA20() % (u32)(config->alphaRange + 1) + config->alphaBase;
            spawn.fade = func_150ADA20() % (u32)(config->fadeRange + 1) + config->fadeBase;
            spawn.life = func_150ADA20() % (u32)(config->lifeRange + 1) + config->lifeBase;
            motion.size = func_150ADA68() * config->sizeRange + config->sizeBase;
            motion.growth = func_150ADA68() * config->growthRange + config->growthBase;
            motion.velocity.x = (axis0.x * first + axis1.x * second + normal.x) * speed;
            motion.velocity.y = (axis0.y * first + axis1.y * second + normal.y) * speed;
            motion.velocity.z = (axis0.z * first + axis1.z * second + normal.z) * speed;
            func_15147DA0(&spawn, &motion, 0, config->unk58[0], config->unk58[1],
                          config->unk58[2], config->unk58[3], config->unk58[4],
                          config->unk58[5], config->unk74, config->unk78,
                          (s32 *)&config->params, 0, kind, arg2);
            count--;
        }
    }
}

struct Local15151A38Arg {
    Vec3f position;
    f32 sizeBase;
    f32 sizeRange;
    f32 growthBase;
    f32 growthRange;
    f32 speedBase;
    f32 speedRange;
    s16 countBase;
    s16 countRange;
    s16 yawBase;
    s16 yawRange;
    s16 pitchBase;
    s16 pitchRange;
    s16 fadeBase;
    s16 fadeRange;
    s16 lifeBase;
    s16 lifeRange;
    u8 green;
    u8 enabled;
    u8 red;
    u8 pad3B;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 blue;
    u8 alphaBase;
    u8 alphaRange;
    u8 pad57;
    s32 unk58;
    s32 unk5C;
    Params15147DA0 params;
    s16 unk80;
    s16 unk82;
    s32 unk84;
};

/* Spawns a random number of particles at config->position, each moving at a random yaw (around y)
 * and pitch; func_151423D8 is the cosine of a 256-step angle. */
void func_15151A38(struct Local15151A38Arg *config, u8 arg1, s32 arg2) {
    Spawn15147DA0 spawn;
    Motion15147DA0 motion;
    s32 count;
    s16 yaw;
    s16 pitch;
    f32 speed;
    f32 sinPitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 horizontal;

    spawn.active = 1;
    spawn.enabled = config->enabled;
    motion.green = config->green;
    motion.blue = config->blue;
    motion.red = config->red;
    spawn.position = config->position;
    motion.unk1C = config->unk80;
    motion.unk1E = config->unk82;
    spawn.unk18 = config->unk84;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (u32)(config->yawRange + 1)) + config->yawBase;
        pitch = (func_150ADA20() % (u32)(config->pitchRange + 1)) + config->pitchBase;
        cosPitch = func_151423D8((u8)pitch);
        sinPitch = func_151423D8((u8)(pitch - 0x40));
        cosYaw = func_151423D8((u8)yaw);
        sinYaw = func_151423D8((u8)(yaw - 0x40));
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        motion.alpha = (func_150ADA20() % (u32)(config->alphaRange + 1)) + config->alphaBase;
        spawn.fade = (func_150ADA20() % (u32)(config->fadeRange + 1)) + config->fadeBase;
        spawn.life = (func_150ADA20() % (u32)(config->lifeRange + 1)) + config->lifeBase;
        motion.size = (func_150ADA68() * config->sizeRange) + config->sizeBase;
        motion.growth = (func_150ADA68() * config->growthRange) + config->growthBase;
        horizontal = speed * cosPitch;
        motion.velocity.x = horizontal * sinYaw;
        motion.velocity.y = -speed * sinPitch;
        motion.velocity.z = horizontal * cosYaw;
        func_15147DA0(&spawn, &motion, 0, config->unk3C, config->unk40, config->unk44, config->unk48,
                      config->unk4C, config->unk50, config->unk58, config->unk5C, (s32 *)&config->params, 0, arg1, arg2);
        count--;
    }
}

struct Arg15151D6C {
    s32 countBase;
    s32 countRange;
    Vec3f position;
    Vec3f normal;
    s16 triangle[9];
    f32 spread;
    f32 speedBase;
    f32 speedRange;
    f32 unk40;
    f32 unk44;
    s16 unk48;
    s16 unk4A;
    f32 scaleBase;
    f32 scaleRange;
    f32 spin; /* spawn.spin.x/z are random in [-spin, spin] */
};

/* Spawn descriptor for func_15132A4C: the first 0x78 bytes of Spawn1513264C. */
typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    Vec3f rotation; /* degrees */
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    Vec3f position;
    Vec3f velocity;
    Vec3f spin;
    f32 unk4C;
    s32 flags;
    s16 unk54;
    s16 variantId;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    s16 pad76;
} Spawn15132A4C;

extern void *func_15132A4C(Spawn15132A4C *, s32, s32, s32, u8, s32);

/* Spawns a random number of pieces off a surface triangle, each moving in a random direction inside
 * the cone around the triangle's normal and using a random entry of the variant tables. */
void func_15151D6C(struct Arg15151D6C *config, s32 *variantIds, f32 *variantScales, u32 variantCount, u8 arg4, s32 arg5) {
    s32 count;
    s32 variant;
    Spawn15132A4C spawn;
    Vec3f axis0;
    f32 length0;
    f32 inverse0;
    Vec3f axis1;
    f32 length1;
    f32 inverse1;
    Vec3f normal;
    f32 normalLength;
    f32 normalInverse;
    f32 second;
    f32 first;
    f32 speed;
    f32 pad[2]; // matching: unused stack slots in the original frame

    if (func_15144E80(config->triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &config->normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        spawn.unk00 = 1.0f;
        spawn.unk04 = 1.0f;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.position = config->position;
        spawn.spin.y = 0.0f;
        spawn.flags = 0x29E8;
        count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
        spawn.unk58 = 0;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk61 = 8;
        spawn.unk62 = 0;
        spawn.unk63 = 0;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 2;
        spawn.unk6A = 0;
        spawn.unk6C = 0;
        spawn.unk70 = 0;
        spawn.unk72 = 12;
        spawn.unk74 = 21;
        while (count != 0) {
            speed = (func_150ADA68() * config->speedRange) + config->speedBase;
            func_15143874((s16)(func_150ADA20() & 0xFF), func_150ADA68() * config->spread, &first, &second);
            spawn.velocity.x = ((axis0.x * first) + (axis1.x * second) + normal.x) * speed;
            spawn.velocity.y = ((axis0.y * first) + (axis1.y * second) + normal.y) * speed;
            spawn.velocity.z = ((axis0.z * first) + (axis1.z * second) + normal.z) * speed;
            spawn.rotation.x = func_150ADA68() * 360.0f;
            spawn.rotation.y = func_150ADA68() * 360.0f;
            spawn.rotation.z = func_150ADA68() * 360.0f;
        // matching: the labels end basic blocks, which keeps as1 from filling the calls' delay slots
        pickVariant:
            variant = func_150ADA20() % variantCount;
            spawn.variantId = variantIds[variant];
            spawn.unk0C = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * variantScales[variant];
            spawn.unk08 = spawn.unk0C;
        randomizeSpin:
            spawn.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.unk4C = (func_150ADA68() * config->unk44) + config->unk40;
            spawn.unk54 = (func_150ADA20() % (u32)(config->unk4A + 1)) + config->unk48;
            func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            count--;
        }
    }
}

struct Arg15152190 {
    s32 countBase;
    s32 countRange;
    Vec3f origin;
    AngleRange angles;
    f32 speedBase;
    f32 speedRange;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 scaleBase;
    f32 scaleRange;
    f32 spin; /* spawn.spin.x/z are random in [-spin, spin] */
};

/* Spawns a random number of pieces in random directions around config->origin, each using a random
 * entry of the variant tables. */
void func_15152190(struct Arg15152190 *config, s32 *variantIds, f32 *variantScales, u32 variantCount, f32 radius, u8 arg5, u8 arg6, s32 arg7) {
    s32 count;
    s32 variant;
    Spawn15132A4C spawn;
    s16 yaw;
    s16 pitch;

    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.spin.y = 0.0f;
    spawn.flags = (arg5 ? 0x100000 : 0) | 0x29E8;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 8;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 2;
    spawn.unk6A = 0;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 12;
    spawn.unk74 = 21;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase;
        pitch = (func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase;
        spawn.rotation.x = func_150ADA68() * 360.0f;
        spawn.rotation.y = func_150ADA68() * 360.0f;
        spawn.rotation.z = func_150ADA68() * 360.0f;
        func_15143794(yaw, pitch, (func_150ADA68() * config->speedRange) + config->speedBase, &spawn.velocity.x);
        spawn.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.unk4C = (func_150ADA68() * config->unk28) + config->unk24;
        spawn.unk54 = (func_150ADA20() % (u32)(config->unk2E + 1)) + config->unk2C;
        func_15143794(yaw, pitch, radius, &spawn.position.x);
        spawn.position.x += config->origin.x;
        spawn.position.y += config->origin.y;
        spawn.position.z += config->origin.z;
    // matching: the label ends a basic block, which keeps as1 from filling a call's delay slot
    pickVariant:
        variant = func_150ADA20() % variantCount;
        spawn.variantId = variantIds[variant];
        spawn.unk0C = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * variantScales[variant];
        spawn.unk08 = spawn.unk0C;
        {
            void *piece; // matching: storing the unused result changes as1's ordering of the argument stores
            piece = func_15132A4C(&spawn, 3, 0xFF, 0, arg6, arg7);

        }
        count--;
    }
}

struct Local15152520Arg {
    s32 countBase;
    s32 countRange;
    Vec3f position;
    AngleRange angles;
    f32 speedBase;
    f32 speedRange;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    Vec3f unk50; /* base of motion.unk1C */
    Vec3f unk5C; /* range of motion.unk1C */
    Color8 unk68;
    Color8 unk6C;
    u8 unk70;
    u8 unk71;
    s16 unk72;
    s16 unk74;
};

/* Second argument of func_15157898. */
struct Local15152520Motion {
    Vec3f rotation; /* degrees */
    f32 unk0C;
    Vec3f velocity;
    Vec3f unk1C;
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    u8 pad31[3];
};

/* First argument of func_15157898. */
struct Local15152520Params {
    u8 flags;
    u8 unk01;
    u8 unk02;
    s8 unk03;
    s8 unk04;
    u8 pad05;
    s16 unk06;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8 unk2C;
    u8 unk2D;
    u8 pad2E[2];
    Color8 unk30;
    Color8 unk34;
    u8 unk38;
    u8 pad39[3];
    s32 unk3C;
    u8 unk40;
    u8 unk41;
    u8 pad42[2];
    Vec3f position;
    u8 pad50[4];
    s16 unk54;
    s16 unk56;
};

extern void func_15157898(struct Local15152520Params *, struct Local15152520Motion *, s32, f32, s32, s32, s32, u8, s32);

void func_15152520(struct Local15152520Arg *config, u8 arg1, s32 arg2) {
    s32 pad; // matching: unused stack slot
    struct Local15152520Params params;
    struct Local15152520Motion motion;
    s32 count;

    params.unk38 = 0;
    params.flags = config->unk70 | 2;
    params.unk01 = 0;
    params.unk02 = 1;
    params.unk03 = -1;
    params.unk04 = -1;
    params.unk08 = config->unk34;
    params.unk0C = config->unk38;
    params.unk24 = 0x80;
    params.unk28 = 0x20;
    params.unk10 = 0;
    params.unk14 = 0x220405;
    params.unk18 = 0x40200;
    params.unk2C = 0;
    params.unk2D = 8;
    params.unk1C = 1;
    params.unk20 = 0x38;
    params.unk3C = 0;
    params.unk40 = 0;
    params.unk41 = 2;
    params.position = config->position;
    motion.unk2C = config->unk24;
    params.unk30 = config->unk68;
    params.unk34 = config->unk6C;
    params.unk54 = config->unk72;
    params.unk56 = config->unk74;
    motion.unk30 = config->unk71;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        params.unk06 = (func_150ADA20() % (u32)(config->unk32 + 1)) + config->unk30;
        motion.rotation.x = func_150ADA68() * 360.0f;
        motion.rotation.y = func_150ADA68() * 360.0f;
        motion.rotation.z = func_150ADA68() * 360.0f;
        motion.unk0C = (func_150ADA68() * config->unk4C) + config->unk48;
        func_15143794((s16)((func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase),
                      (s16)((func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase),
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &motion.velocity.x);
        motion.unk1C.x = (func_150ADA68() * config->unk5C.x) + config->unk50.x;
        motion.unk1C.y = (func_150ADA68() * config->unk5C.y) + config->unk50.y;
        motion.unk1C.z = (func_150ADA68() * config->unk5C.z) + config->unk50.z;
        motion.unk28 = (func_150ADA68() * config->unk2C) + config->unk28;
        func_15157898(&params, &motion, config->unk3C, (func_150ADA68() * config->unk44) + config->unk40, 0, 0, 0, arg1, arg2);
        count--;
    }
}

struct Local15152874Arg {
    s32 countBase;
    s32 countRange;
    Vec3f position;
    AngleRange angles;
    f32 speedBase;
    f32 speedRange;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s8 colorFunc; /* index into D_8008AC60, or -1 for opaque white */
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u8 pad36[2];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
};

/* First argument of func_150CCEB0. */
struct Local15152874Spawn {
    Vec3f position;
    Vec3f velocity;
    f32 unk18;
    f32 unk1C;
    Color8 color;
    s32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    s32 pad34;
};

extern void func_150CCEB0(struct Local15152874Spawn *, u8, u8);
extern void (*D_8008AC60[])(Color8 *);

void func_15152874(struct Local15152874Arg *config, u8 arg1, s32 arg2) {
    struct Local15152874Spawn spawn;
    s32 count;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    spawn.position = config->position;
    spawn.unk18 = config->unk24;
    spawn.unk30 = config->unk48;
    while (count != 0) {
        func_15143794((s16)((func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase),
                      (s16)((func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase),
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &spawn.velocity.x);
        spawn.unk1C = (func_150ADA68() * config->unk2C) + config->unk28;
        spawn.unk24 = (func_150ADA20() % (u32)(config->unk34 + 1)) + config->unk32;
        spawn.unk28 = (func_150ADA68() * config->unk40) + config->unk38;
        spawn.unk2C = (func_150ADA68() * config->unk44) + config->unk3C;
        if (config->colorFunc != -1) {
            D_8008AC60[config->colorFunc](&spawn.color);
        } else {
            spawn.color.red = 0xFF;
            spawn.color.green = 0xFF;
            spawn.color.blue = 0xFF;
            spawn.color.alpha = 0xFF;
        }
        func_150CCEB0(&spawn, arg1, arg2);
        count--;
    }
}

/* Five RGB colors. */
extern u8 D_800A5FE0[][3];

/* A D_8008AC60 color function: a random color of D_800A5FE0 with a random alpha in [155, 255]. */
void func_15152ABC(Color8 *out) {
    u8 *pad; // matching: unused stack slot above rgb
    u8 *rgb;

    rgb = D_800A5FE0[(u8)(func_150ADA20() % 5U)];
    out->alpha = (func_150ADA20() % 101U) + 155;
    out->red = rgb[0];
    out->green = rgb[1];
    out->blue = rgb[2];
}

extern void func_1515C2F0(EffectTemplate *, s32, TrailBurstParams *, s32, u8, s32);

void func_15152B38(TrailBurst *config, u8 arg1, s32 arg2) {
    EffectTemplate param;
    TrailBurstParams spawn;
    s32 count;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    param.pos = config->pos;
    param.flags = config->headerFlags;
    param.kind = 10;
    spawn.flags = config->flags;
    spawn.primMode = config->primMode;
    spawn.envMode = config->envMode;
    spawn.renderMode = config->renderMode;
    spawn.unk21 = config->unk21;
    spawn.setGeometryMode = config->setGeometryMode;
    spawn.clearGeometryMode = config->clearGeometryMode;
    spawn.fadeStart = config->fadeStart;
    spawn.fadeRate = config->fadeRate;
    spawn.unk38 = config->unk38;
    spawn.unk39 = config->unk39;
    spawn.shrinkStart = config->shrinkStart;
    spawn.shrinkPaused = config->shrinkPaused;
    spawn.shrinkFactor = config->shrinkFactor;
    while (count != 0) {
        param.itemCount = (func_150ADA20() % (u32)(config->capacityRange + 1)) + config->capacityBase;
        param.life = (func_150ADA20() % (u32)(config->lifeRange + 1)) + config->lifeBase;
        spawn.width = (func_150ADA68() * config->sizeRange) + config->sizeBase;
        spawn.gravity = (func_150ADA68() * config->gravityRange) + config->gravityBase;
        func_15143794((s16)((func_150ADA20() % (u32)(config->angles.yawRange + 1)) + config->angles.yawBase),
                      (s16)((func_150ADA20() % (u32)(config->angles.pitchRange + 1)) + config->angles.pitchBase),
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &spawn.vel.x);
        spawn.color.red = (func_150ADA20() % (u32)(config->colorRange.red + 1)) + config->colorBase.red;
        spawn.color.green = (func_150ADA20() % (u32)(config->colorRange.green + 1)) + config->colorBase.green;
        spawn.color.blue = (func_150ADA20() % (u32)(config->colorRange.blue + 1)) + config->colorBase.blue;
        spawn.color.alpha = (func_150ADA20() % (u32)(config->colorRange.alpha + 1)) + config->colorBase.alpha;
        spawn.color2.red = (func_150ADA20() % (u32)(config->color2Range.red + 1)) + config->color2Base.red;
        spawn.color2.green = (func_150ADA20() % (u32)(config->color2Range.green + 1)) + config->color2Base.green;
        spawn.color2.blue = (func_150ADA20() % (u32)(config->color2Range.blue + 1)) + config->color2Base.blue;
        spawn.color2.alpha = (func_150ADA20() % (u32)(config->color2Range.alpha + 1)) + config->color2Base.alpha;
        spawn.colorLevel = (func_150ADA20() % (u32)(config->colorLevelRange + 1)) + config->colorLevelBase;
        func_1515C2F0(&param, 0, &spawn, 0, arg1, arg2);
        count--;
    }
}

struct Local15152F70Arg {
    Vec3f position;
    f32 sizeBase;
    f32 sizeRange;
    f32 growthBase;
    f32 growthRange;
    f32 speedBase;
    f32 speedRange;
    s16 countBase;
    s16 countRange;
    s16 yawBase;
    s16 yawRange;
    s16 pitchBase;
    s16 pitchRange;
    s16 fadeBase;
    s16 fadeRange;
    s16 lifeBase;
    s16 lifeRange;
    u8 green;
    u8 red;
    u8 pad3A[2];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 blue;
    u8 alpha;
    u8 pad56[2];
};

/* Motion15147DA0 without the trailing unk1C/unk1E, which this caller never sets. */
typedef struct {
    f32 size;
    Vec3f velocity;
    f32 growth;
    u8 pad14[4];
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} MotionShort15147DA0;

/* Like func_15151A38, with a fixed color and fixed func_15147DA0 parameters. */
void func_15152F70(struct Local15152F70Arg *config, u8 arg1) {
    Spawn15147DA0 spawn;
    s32 count;
    MotionShort15147DA0 motion;
    s16 yaw;
    Params15147DA0 params;
    s16 pitch;
    f32 speed;
    f32 cosPitch;
    f32 sinPitch;
    f32 cosYaw;
    f32 sinYaw;

    spawn.active = 1;
    spawn.enabled = 1;
    motion.green = config->green;
    motion.blue = config->blue;
    motion.red = config->red;
    motion.alpha = config->alpha;
    spawn.position = config->position;
    params.unk0 = 0;
    params.unk4 = 1;
    params.unk8 = 0x160600;
    params.unkC = 3;
    params.unk10 = 0x10;
    params.unk14 = 0x80;
    params.unk18 = 0x20;
    params.unk1C = 0;
    params.unk1D = 9;
    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (u32)(config->yawRange + 1)) + config->yawBase;
        pitch = (func_150ADA20() % (u32)(config->pitchRange + 1)) + config->pitchBase;
        cosPitch = func_151423D8((u8)pitch);
        sinPitch = func_151423D8((u8)(pitch - 0x40));
        cosYaw = func_151423D8((u8)yaw);
        sinYaw = func_151423D8((u8)(yaw - 0x40));
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        spawn.fade = (func_150ADA20() % (u32)(config->fadeRange + 1)) + config->fadeBase;
        spawn.life = (func_150ADA20() % (u32)(config->lifeRange + 1)) + config->lifeBase;
        motion.size = (func_150ADA68() * config->sizeRange) + config->sizeBase;
        motion.growth = (func_150ADA68() * config->growthRange) + config->growthBase;
        motion.velocity.x = (speed * cosPitch) * sinYaw;
        motion.velocity.y = -speed * sinPitch;
        motion.velocity.z = (speed * cosPitch) * cosYaw;
        func_15147DA0(&spawn, &motion, 0, config->unk3C, config->unk40, config->unk44, config->unk48,
                      config->unk4C, config->unk50, 0, 0, (s32 *)&params, 0, arg1, 1);
        count--;
    }
}

typedef struct {
    Vec3f position;
    s16 countBase, countRange, yawBase, yawRange, pitchBase, pitchRange;
    f32 speedBase, speedRange;
    s16 lifeBase, lifeRange, sizeBase, sizeRange;
    s8 mode;
    u8 kind;
    s16 fadeBase, fadeRange, unk2E;
    s16 unk30, unk32, unk34, unk36;
    s32 flags;
    u8 color, alpha;
} Game153298Config;

extern void *func_15167D84(void *, s32, s32, s8, u8, s32);

/* Spawns a random number of particles (func_15167D84) at config->position, each moving at a random
 * yaw and pitch. Positions are integer units plus a 1/256 fraction. */
void func_15153298(Game153298Config *config, u8 arg1, s32 arg2) {
    BillboardTemplate packet;
    f32 speed;
    f32 sinPitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 horizontal;
    s16 yaw;
    s16 pitch;
    s32 count;

    packet.x = (s32)config->position.x;
    packet.y = (s32)config->position.y;
    packet.z = (s32)config->position.z;
    packet.subX = (u16)(s32)(config->position.x * 256.0f);
    packet.subZ = (u16)(s32)(config->position.y * 256.0f);
    packet.subY = (u16)(s32)(config->position.z * 256.0f);
    packet.primR = config->color;
    packet.primG = config->color;
    packet.primB = config->color;
    packet.envR = 0;
    packet.envG = 0;
    packet.envB = 0;
    packet.primA = config->alpha;
    packet.callback = config->mode;
    packet.flags = config->kind;
    packet.frame = 0;
    packet.frameStep = config->unk2E;
    packet.anim = config->flags;
    count = func_150ADA20() % (u32)(config->countRange + 1) + config->countBase;
    while (count != 0) {
        yaw = func_150ADA20() % (u32)(config->yawRange + 1) + config->yawBase;
        pitch = func_150ADA20() % (u32)(config->pitchRange + 1) + config->pitchBase;
        cosPitch = func_151423D8(pitch);
        sinPitch = func_151423D8(pitch - 64);
        cosYaw = func_151423D8(yaw);
        sinYaw = func_151423D8(yaw - 64);
        speed = func_150ADA68() * config->speedRange + config->speedBase;
        packet.life = func_150ADA20() % (u32)(config->lifeRange + 1) + config->lifeBase;
        horizontal = speed * cosPitch;
        packet.velX = (s32)(horizontal * sinYaw);
        packet.velY = (s32)(-speed * sinPitch);
        packet.velZ = (s32)(horizontal * cosYaw);
        packet.accelY = func_150ADA20() % (u32)(config->sizeRange + 1) + config->sizeBase;
        packet.fadeStep = func_150ADA20() % (u32)(config->fadeRange + 1) + config->fadeBase;
        packet.width = func_150ADA20() % (u32)(config->unk32 + 1) + config->unk30;
        packet.height = func_150ADA20() % (u32)(config->unk36 + 1) + config->unk34;
        func_15167D84(&packet, 0, 0, -1, arg1, arg2);
        count--;
    }
}

struct Local15153634Arg {
    s16 countBase;
    s16 countRange;
    u8 unk4;
    u8 pad5;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    Vec3f position;
    s16 yawBase;
    s16 pitchBase;
    s16 yawRange;
    s16 pitchRange;
    f32 speedBase;
    f32 speedRange;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    s8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 extra; /* copied into the spawned object at +0xA8 */
};

/* First argument of func_15130280. */
struct Local15153634Spawn {
    s32 unk00;
    s32 unk04;
    u16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    Vec3f position;
    u8 pad3C[0xC];
    Vec3f velocity;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    u8 pad64[2];
    u8 unk66;
    u8 pad67[9];
};

extern void *func_15130280(struct Local15153634Spawn *, u8, s32, s32, u8, s32);

/* Spawns a random number of particles (func_15130280) at config->position, each moving at a random
 * yaw and pitch, and copies config->extra into each. */
void func_15153634(struct Local15153634Arg *config, u8 arg1, u8 arg2, s32 arg3) {
    struct Local15153634Spawn spawn;
    f32 extra;
    s32 count;
    s16 yaw;
    s16 pitch;
    f32 speed;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;
    f32 cosYaw;
    f32 horizontal;
    void *object;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    extra = config->extra;
    spawn.unk1D = config->unk4;
    spawn.unk08 = config->unk6;
    spawn.unk04 = config->unkC;
    spawn.unk00 = config->unk8;
    spawn.unk0C = 0;
    spawn.unk14 = config->unk1C;
    spawn.unk15 = config->unk1D;
    spawn.unk16 = config->unk1E;
    spawn.unk17 = config->unk1F;
    spawn.unk18 = config->unk20;
    spawn.unk19 = config->unk21;
    spawn.unk1A = config->unk22;
    spawn.position = config->position;
    spawn.unk58 = config->unk5C;
    spawn.unk1C = config->unk25;
    spawn.unk62 = config->unk60;
    spawn.unk63 = config->unk61;
    spawn.unk1E = config->unk26;
    spawn.unk20 = config->unk28;
    spawn.unk60 = config->unk62;
    spawn.unk61 = config->unk63;
    spawn.unk22 = config->unk2A;
    spawn.unk24 = config->unk2C;
    spawn.unk66 = arg1;
    while (count != 0) {
        yaw = (func_150ADA20() % (u32)(config->yawRange + 1)) + config->yawBase;
        pitch = (func_150ADA20() % (u32)(config->pitchRange + 1)) + config->pitchBase;
        cosPitch = func_151423D8((u8)pitch);
        sinPitch = func_151423D8((u8)(pitch - 0x40));
        cosYaw = func_151423D8((u8)yaw);
        sinYaw = func_151423D8((u8)(yaw - 0x40));
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        spawn.unk0A = (func_150ADA20() % (u32)(config->unk12 + 1)) + config->unk10;
        spawn.unk10 = (func_150ADA20() % (u32)(config->unk18 + 1)) + config->unk14;
        horizontal = speed * cosPitch;
        spawn.unk28 = spawn.unk2C = (func_150ADA68() * config->unk34) + config->unk30;
        spawn.velocity.x = horizontal * sinYaw;
        spawn.velocity.y = -speed * sinPitch;
        spawn.velocity.z = horizontal * cosYaw;
        spawn.unk54 = (func_150ADA68() * config->unk58) + config->unk54;
        spawn.unk1B = (func_150ADA20() % (u32)(config->unk24 + 1)) + config->unk23;
        object = func_15130280(&spawn, config->unk64, 0, 4, arg2, arg3);
        if (object != NULL) {
            memcpy((u8 *)object + 0xA8, &extra, sizeof(extra));
        }
        count--;
    }
}

struct Local151539B4Arg {
    s16 countBase;
    s16 countRange;
    Vec3f position;
    s16 yawBase;
    s16 pitchBase;
    s16 yawRange;
    s16 pitchRange;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    Color8 color0;
    Color8 color1;
    u8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u16 unk36;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    s16 unk42;
    s16 unk44;
    u8 unk46;
};

/* First argument of func_15156190. */
struct Local151539B4Spawn {
    Vec3f position;
    Vec3f direction;
    f32 unk18;
    f32 unk1C;
    Color8 color0;
    Color8 color1;
    u8 unk28;
    u8 pad29;
    s16 unk2A;
    u16 unk2C;
    u8 pad2E[2];
    f32 unk30;
    u8 unk34;
    u8 pad35;
    s16 unk36;
    s16 unk38;
    u8 pad3A[2];
};

extern void func_15156190(struct Local151539B4Spawn *, u8, s32, u8, s32);

/* Spawns a random number of particles (func_15156190) at config->position, each pointing at a random
 * yaw and pitch (a direction of length 10). */
void func_151539B4(struct Local151539B4Arg *config, u8 arg1) {
    struct Local151539B4Spawn spawn;
    s32 count;
    s16 yaw;
    s16 pitch;
    f32 cosPitch;
    f32 sinPitch;
    f32 cosYaw;
    f32 horizontal;
    f32 sinYaw;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    spawn.position = config->position;
    spawn.color0.red = config->color0.red;
    spawn.color0.green = config->color0.green;
    spawn.color0.blue = config->color0.blue;
    spawn.color0.alpha = config->color0.alpha;
    spawn.color1.red = config->color1.red;
    spawn.color1.green = config->color1.green;
    spawn.color1.blue = config->color1.blue;
    spawn.color1.alpha = config->color1.alpha;
    spawn.unk28 = config->unk30;
    spawn.unk2C = config->unk36;
    spawn.unk36 = config->unk42;
    spawn.unk38 = config->unk44;
    while (count != 0) {
        yaw = (func_150ADA20() % (u32)(config->yawRange + 1)) + config->yawBase;
        pitch = (func_150ADA20() % (u32)(config->pitchRange + 1)) + config->pitchBase;
        cosPitch = func_151423D8((u8)pitch);
        sinPitch = func_151423D8((u8)(pitch - 0x40));
        cosYaw = func_151423D8((u8)yaw);
        horizontal = 10.0f * cosPitch;
        sinYaw = func_151423D8((u8)(yaw - 0x40));
        spawn.direction.x = horizontal * sinYaw;
        spawn.direction.y = -10.0f * sinPitch;
        spawn.direction.z = horizontal * cosYaw;
        spawn.unk18 = (func_150ADA68() * config->unk1C) + config->unk18;
        spawn.unk1C = (func_150ADA68() * config->unk24) + config->unk20;
        spawn.unk2A = (func_150ADA20() % (u32)(config->unk34 + 1)) + config->unk32;
        spawn.unk30 = (func_150ADA68() * config->unk3C) + config->unk38;
        spawn.unk34 = (func_150ADA20() % (u32)(config->unk41 + 1)) + config->unk40;
        func_15156190(&spawn, config->unk46, 0, arg1, 0);
        count--;
    }
}

struct Local15153CCCArg {
    Vec3f position;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 speedBase;
    f32 speedRange;
    s16 countBase;
    s16 countRange;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 unk34;
    u8 pad35[3];
    f32 unk38; /* probability */
    s16 unk3C;
    s16 unk3E;
    s32 unk40;
};

extern void func_151DA6F8(Vec3f *, Vec3f *, f32, s16, s32, f32, s32, s32, f32, f32, s32, u8, s32, s16, s16, s32, u8, s32);
extern void func_15153CCC(struct ConeBasis *, struct Local15153CCCArg *, s32, u8, s32);

void func_15153C84(struct TriangleSource *source, struct Local15153CCCArg *config, s32 arg2, u8 arg3, s32 arg4) {
    struct ConeBasis cone;

    func_1514F640(source, &cone);
    func_15153CCC(&cone, config, arg2, arg3, arg4);
}

/* Spawns a random number of particles (func_151DA6F8), each moving in a random direction inside the
 * cone. */
void func_15153CCC(struct ConeBasis *cone, struct Local15153CCCArg *config, s32 arg2, u8 arg3, s32 arg4) {
    s32 count;
    Vec3f velocity;

    if (func_1514F6E8(cone) != 0) {
        count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
        while (count != 0) {
            func_1514F808(cone, (func_150ADA68() * config->speedRange) + config->speedBase, &velocity);
            func_151DA6F8(&config->position, &velocity,
                          (func_150ADA68() * config->unk18) + config->unk14,
                          (s16)((func_150ADA20() % (u32)(config->unk2E + 1)) + config->unk2C),
                          (func_150ADA20() % (u32)(config->unk32 + 1)) + config->unk30,
                          (func_150ADA68() * config->unk10) + config->unkC,
                          (func_150ADA20() % (u32)(config->unk2A + 1)) + config->unk28,
                          func_150ADA68() < config->unk38, 1.7f, 1.7f, 1, config->unk34, arg2,
                          config->unk3C, config->unk3E, config->unk40, arg3, arg4);
            count--;
        }
    }
}

/* Like func_15153CCC, but each direction is a random yaw/pitch from `angles`. */
void func_15153F18(AngleRange *angles, struct Local15153CCCArg *config, s32 arg2, u8 arg3, s32 arg4) {
    s32 count;
    Vec3f velocity;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count != 0) {
        func_15143794((s16)((func_150ADA20() % (u32)(angles->yawRange + 1)) + angles->yawBase),
                      (s16)((func_150ADA20() % (u32)(angles->pitchRange + 1)) + angles->pitchBase),
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &velocity.x);
        func_151DA6F8(&config->position, &velocity,
                      (func_150ADA68() * config->unk18) + config->unk14,
                      (s16)((func_150ADA20() % (u32)(config->unk2E + 1)) + config->unk2C),
                      (func_150ADA20() % (u32)(config->unk32 + 1)) + config->unk30,
                      (func_150ADA68() * config->unk10) + config->unkC,
                      (func_150ADA20() % (u32)(config->unk2A + 1)) + config->unk28,
                      func_150ADA68() < config->unk38, 1.7f, 1.7f, 1, config->unk34, arg2,
                      config->unk3C, config->unk3E, config->unk40, arg3, arg4);
        count--;
    }
}

/* Copied into the object spawned by func_151541B8 at +0x170; advanced each frame by func_1515452C. */
typedef struct {
    f32 elapsed;
    f32 duration;
    f32 growth; /* size = growth * sqrt(elapsed) */
    f32 alpha;  /* alpha - fade * elapsed^2 */
    f32 fade;
    u8 growOneAxis; /* grow only the first size field */
} Extra1515452C;

/* Spawns a single object at `position` that grows and fades out over `duration`. With a nonzero arg4
 * it gets no random x/z rotation, spins only around y and grows along one axis. */
void *func_151541B8(Vec3f *position, f32 duration, f32 growth, f32 alpha, f32 arg4, u8 arg5, s32 arg6) {
    void *object;
    s32 oneAxis;
    Spawn15132A4C spawn;
    Extra1515452C extra;
    s32 pad[2]; // matching: unused stack slots

    if (duration <= 0.0f) {
        return NULL;
    }

    extra.elapsed = 0.0f;
    extra.duration = duration;
    extra.growth = growth;
    extra.alpha = alpha;
    extra.fade = alpha / (duration * duration);
    if (arg4 != 0) {
        oneAxis = TRUE;
    } else {
        oneAxis = FALSE;
    }
    extra.growOneAxis = oneAxis;

    spawn.unk08 = 0.0f;
    spawn.unk0C = arg4;
    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    if (arg4 != 0) {
        spawn.rotation.x = 0.0f;
    } else {
        spawn.rotation.x = func_150ADA68() * 360.0f;
    }
    spawn.rotation.y = func_150ADA68() * 360.0f;
    if (arg4 != 0) {
        spawn.rotation.z = 0.0f;
    } else {
        spawn.rotation.z = func_150ADA68() * 360.0f;
    }
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.position = *position;
    spawn.velocity.x = 0.0f;
    spawn.velocity.y = 0.0f;
    spawn.velocity.z = 0.0f;
    if (arg4 != 0) {
        spawn.spin.x = 0.0f;
    } else {
        spawn.spin.x = (func_150ADA68() * 8.512000084f) + -4.256000042f;
    }
    if (arg4 != 0) {
        spawn.spin.y = (func_150ADA68() * 8.512000084f) + -4.256000042f;
    } else {
        spawn.spin.y = 0.0f;
    }
    if (arg4 != 0) {
        spawn.spin.z = 0.0f;
    } else {
        spawn.spin.z = (func_150ADA68() * 8.512000084f) + -4.256000042f;
    }
    spawn.unk4C = 0.0f;
    spawn.flags = 0x100140;
    spawn.variantId = 84;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = alpha;
    spawn.unk61 = 15;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 2;
    spawn.unk6A = 0;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 1;
    spawn.unk74 = 0xFF;
    spawn.unk54 = 300;

    object = func_15132A4C(&spawn, 0, 0, sizeof(extra), arg5, arg6);
    if (object != NULL) {
        memcpy((u8 *)object + 0x170, &extra, sizeof(extra));
    }
    return object;
}

f32 sqrtf(f32);

/* Copied into the object spawned by func_15154884 at +0x170; advanced each frame by func_15154A88. */
typedef struct {
    f32 remaining;
    f32 size;
    f32 alpha;
    f32 phase; /* size and alpha are scaled by sinf(phase) */
    f32 phaseRate;
} Extra15154A88;

/* The fields of a func_15132A4C object that the update functions below use. */
typedef struct {
    u8 pad0[0x18];
    f32 unk18;
    f32 unk1C;
    Vec3f position;
    u8 pad2C[0x24];
    Vec3f velocity;
    u8 pad5C[0x14];
    u8 alpha;
    u8 pad71[0xFF];
    union {
        Extra1515452C grow;
        Extra15154A88 pulse;
    } extra;
} Object15132A4C;

/* Per-frame update for func_151541B8's objects: grow, fade, move; returns 0 once expired. */
s32 func_1515452C(Object15132A4C *object) {
    f32 size;
    Extra1515452C *extra;

    extra = &object->extra.grow;
    size = extra->growth * sqrtf(extra->elapsed);
    object->unk18 = size;
    if (!(extra->growOneAxis & 1)) {
        object->unk1C = size;
    }
    {
        f32 elapsed = extra->elapsed;
        object->alpha = (s8)(u32)(extra->alpha - (extra->fade * elapsed * elapsed));
        extra->elapsed += D_800BE9A4;
        if (extra->duration < extra->elapsed) {
            return 0;
        }
        object->position.x += object->velocity.x * D_800BE9A4;
        object->position.y += object->velocity.y * D_800BE9A4;
        object->position.z += object->velocity.z * D_800BE9A4;
        return 1;
    }
}

struct Local15154684Arg {
    s16 countBase;
    s16 countRange;
    s32 unk04;
    s16 unk08;
    s16 pitchBase;
    s16 unk0C;
    s16 pitchRange;
    f32 speedBase;
    f32 speedRange;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34; /* probability */
    s32 unk38;
};

extern void func_151C5F44(s32, f32 *, f32, f32, s32, s32, f32, s32, s32, u8, s32);

/* Spawns a random number of particles (func_151C5F44), each moving at a random yaw and a pitch from
 * config's range. */
void func_15154684(struct Local15154684Arg *config, u8 arg1, s32 arg2) {
    f32 pad; // matching: unused stack slot above velocity
    Vec3f velocity;
    s32 count;

    count = (func_150ADA20() % (u32)(config->countRange + 1)) + config->countBase;
    while (count > 0) {
        func_15143794((s16)(func_150ADA20() & 0xFF),
                      (s16)((func_150ADA20() % (u32)(config->pitchRange + 1)) + config->pitchBase),
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &velocity.x);
        func_151C5F44(config->unk04, &velocity.x,
                      (func_150ADA68() * config->unk1C) + config->unk18,
                      config->unk20,
                      (func_150ADA20() % (u32)(config->unk25 + 1)) + config->unk24,
                      (func_150ADA20() % (u32)(config->unk28 + 1)) + config->unk26,
                      (func_150ADA68() * config->unk30) + config->unk2C,
                      func_150ADA68() < config->unk34,
                      config->unk38, arg1, arg2);
        count--;
    }
}

/* Spawns a single object at `position` whose size and alpha swell and shrink along half a sine wave
 * over `duration`. */
void *func_15154884(Vec3f *position, f32 duration, f32 size, f32 alpha, u8 arg4, s32 arg5) {
    void *object;
    s32 pad; // matching: unused stack slot
    Spawn15132A4C spawn;
    Extra15154A88 extra;

    if (duration <= 0.0f) {
        return NULL;
    }

    extra.remaining = duration;
    extra.size = size;
    extra.phase = 0.0f;
    spawn.unk08 = 0.0f;
    spawn.unk0C = 0.0f;
    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    extra.alpha = alpha;
    extra.phaseRate = 3.141592741f / duration;
    spawn.rotation.x = func_150ADA68() * 360.0f;
    spawn.rotation.y = func_150ADA68() * 360.0f;
    spawn.rotation.z = func_150ADA68() * 360.0f;
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.position = *position;
    spawn.velocity.x = 0.0f;
    spawn.velocity.y = 0.0f;
    spawn.velocity.z = 0.0f;
    spawn.spin.x = (func_150ADA68() * 26.29600143f) + -13.14800072f;
    spawn.spin.y = 0.0f;
    spawn.spin.z = (func_150ADA68() * 26.29600143f) + -13.14800072f;
    spawn.unk4C = 0.0f;
    spawn.flags = 0x140;
    spawn.variantId = 85;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 18;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 2;
    spawn.unk6A = 0;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 1;
    spawn.unk74 = 0xFF;
    spawn.unk54 = 300;

    object = func_15132A4C(&spawn, 0, 0, sizeof(extra), arg4, arg5);
    if (object != NULL) {
        memcpy((u8 *)object + 0x170, &extra, sizeof(extra));
    }
    return object;
}

/* Per-frame update for func_15154884's objects: pulse size and alpha, move; returns 0 once expired. */
s32 func_15154A88(Object15132A4C *object) {
    Extra15154A88 *extra;
    f32 wave;
    f32 size;

    extra = &object->extra.pulse;
    wave = sinf(extra->phase);
    size = extra->size * wave;
    object->unk1C = size;
    object->unk18 = size;
    object->alpha = (f32)(u8)(u32)extra->alpha * wave;
    extra->remaining -= D_800BE9A4;
    if (extra->remaining <= 0.0f) {
        return 0;
    }

    extra->phase += extra->phaseRate * D_800BE9A4;
    object->position.x += object->velocity.x * D_800BE9A4;
    object->position.y += object->velocity.y * D_800BE9A4;
    object->position.z += object->velocity.z * D_800BE9A4;
    return 1;
}
