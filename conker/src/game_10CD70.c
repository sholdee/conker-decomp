#include <ultra64.h>
/* include/functions.h declares func_150ADA20 as returning u8.  The golden code for
 * func_150DFEFC feeds its result straight into `divu` with no zero-extension, which a
 * u8-returning declaration cannot produce, so the real return type is a word.  Per project
 * policy the shared header is left alone and the correct prototype is made file-local:
 * the header's declaration is renamed out of the way while functions.h is included. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
/* variables.h declares D_800D3098 as an array, but the code loads it as a pointer to the
 * table (func_150DF8C0 dereferences the symbol's first word), so it gets a local declaration. */
#define D_800D3098 D_800D3098_array_decl_in_variables_h
#include "variables.h"
#undef D_800D3098
extern struct178 *D_800D3098;

/* One display-list command, with the opcode byte of its first word addressable. */
typedef struct {
    union {
        s8 opcode;
        u32 word;
    } w0;
    u32 w1;
} GfxCmd;

/* matching: the scan loops address dl by (j << 3) bytes; dl[j] would be strength-reduced */
#define GFX_CMD(dl, j) ((GfxCmd *)(((j) << 3) + (u8 *)(dl)))

typedef struct {
    u8 values[3];
} Game10CD70Indices;

extern Game10CD70Indices D_80088984;

/* Returns whether the D_800D3098 record selected by D_80088984[index] is active. */
s32 func_150DF8C0(s32 index) {
    Game10CD70Indices indices = D_80088984;

    if (D_800D3098[indices.values[index]].unk6[0xE] != 0) {
        return 1;
    }
    return 0;
}

extern u8 D_800D9950[];
extern void func_150A2864(s32, s32);
extern void func_150A3444(s32, s16, s16, s16);
extern void func_151749A0(s32, s32);

/* Points slots 3 and 4 (func_150A2864/func_150A3444) 200 units above actors 6 and 7, or
 * disables them when the actor is missing, then scrolls the texture of each of the three
 * materials in the D_800B0E00 display list that loads its image from segment 2 + i. */
void func_150DF920(s32 arg0) {
    struct127 *obj;
    s32 i;
    s32 j;
    s32 id;
    s32 slot;
    f32 x;
    f32 y;
    f32 z;
    s32 active;
    GfxCmd *dl;
    s32 opcode;
    s32 uls;
    s32 ult;

    if (arg0 != 0) {
        return;
    }

    func_151749A0(5, 4);

    id = 6;
    slot = 3;
    for (i = 0; i < 2; i++) {
        obj = func_15083E90((u8)id); // takes a u8; functions.h says s32
        id = 7;
        if (obj != NULL) {
            func_150A2864(slot, 0);
            x = obj->x_position;
            y = obj->y_position + 200.0f;
            z = obj->z_position;
            func_150A3444(slot, x, y, z);
        } else {
            func_150A2864(slot, 1);
        }
        slot = 4;
    }

    dl = (GfxCmd *)D_800B0E00[0];

    for (i = 0; i < 3; i++) {
        active = func_150DF8C0(i);
        if (active) {
            if (D_800D9950[i] < 0x60) {
                D_800D9950[i] += D_800BE9E4;
                if (D_800D9950[i] > 0x60) {
                    D_800D9950[i] = 0x60;
                }
            }
        } else {
            D_800D9950[i] = 0;
        }

        /* Find the G_SETTIMG whose image address is segment 2 + i. */
        j = -1;
        do {
            j++;
            while (GFX_CMD(dl, j)->w0.opcode != (s8)G_SETTIMG &&
                   GFX_CMD(dl, j)->w0.opcode != (s8)G_ENDDL) {
                j++;
            }
            if (GFX_CMD(dl, j)->w0.opcode == (s8)G_ENDDL) {
                j = -1;
                break;
            }
        } while (dl[j].w1 != (i << 24) + 0x02000000); // segment (2 + i), offset 0

        if (j != -1) {
            /* Then the G_SETTILESIZE after it. */
            // matching: the opcode copy steers register allocation
            while ((opcode = GFX_CMD(dl, j)->w0.opcode) != (s8)G_SETTILESIZE) {
                j++;
            }

            if (j != -1) { // matching: always true here, but dropping it changes the branches
                if (active) {
                    uls = (dl[j].w0.word >> 12) & 0xFFF;
                    ult = dl[j].w0.word & 0xFFF;
                    ult += (D_800D9950[i] / 16) * D_800BE9E4;
                } else {
                    ult = 2;
                    uls = 2;
                }
                dl[j].w0.word = (G_SETTILESIZE << 24) | ((uls & 0xFFF) << 12) | (ult & 0xFFF);
            }
        }
    }
}

extern s32 D_80090324[];

/* Binds segments 2..4 to the textures of the three materials func_150DF920 scrolls: an
 * active material gets entry 9 + D_800DD405 of D_80090324, an inactive one entry 8. */
Gfx *func_150DFBD0(Gfx *gdl) {
    s32 i;
    s32 segment;
    s32 addr;
    s32 unused;
    s32 asset;
    u8 variant;

    for (i = 0; i < 3; i++) {
        if (func_150DF8C0(i) != 0) {
            variant = D_800DD405;
            asset = D_80090324[variant + 9];
        } else {
            asset = D_80090324[8];
        }
        addr = func_1510D0EC(asset, &unused, 3, 0);
        segment = i + 2;
        gSPSegment(gdl++, segment, addr);
    }
    return gdl;
}

/* matching: 0.55f, kept in rodata; a literal folds the negation into a -0.55f constant */
extern f32 D_800A0FA4;

/* The object func_150DFCA8 animates (a handler in the 0x80088xxx object table). */
typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ f32 pos;
    /* 0x08 */ u8 pad8[0x5C];
    /* 0x64 */ f32 speed;
    /* 0x68 */ u8 pad68[0x14];
    /* 0x7C */ f32 targetSpeed;
} SwingObject;

/* Moves pos back and forth between 11 and 55: the target speed flips sign at either end and
 * the speed eases towards it by 0.05 per call. */
void func_150DFCA8(SwingObject *obj) {
    f32 pos;
    f32 target;

    // matching: the step shares target's register
    target = obj->speed * D_800BE9E4;
    obj->pos += target;
    pos = obj->pos;
    if (pos < 11.0f) {
        obj->targetSpeed = target = 0.55f;
    } else if (obj->pos > 55.0f) {
        pos = D_800A0FA4; // matching: the negated constant goes through pos's register
        obj->targetSpeed = target = -pos;
    } else {
        target = obj->targetSpeed;
    }
    if (obj->speed < target) {
        obj->speed += 0.05f;
        if (target < obj->speed) {
            obj->speed = target;
        }
    } else if (target < obj->speed) {
        obj->speed -= 0.05f;
        if (obj->speed < target) {
            obj->speed = target;
        }
    }
}

/* An actor's bone matrices: struct127.unk1D4 points at an array of these, 0x40 bytes each. */
typedef f32 MtxF[4][4];

/* Moves bone 1 of the actor to its x/z position at a fixed height of 2706. */
void func_150DFDA4(struct127 *actor) {
    f32 (*bone)[4] = ((MtxF *)actor->unk1D4)[1];

    bone[3][0] = actor->x_position;
    bone[3][1] = 2706.0f;
    bone[3][2] = actor->z_position;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x13];
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 pad14[0x10];
    /* 0x24 */ GfxCmd **dl;
} Game10CD70Object;

typedef struct {
    /* 0x000 */ u8 pad0[0x2D8];
    /* 0x2D8 */ f32 timer;
    /* 0x2DC */ f32 rate;
    /* 0x2E0 */ u8 pad2E0[0xC];
    /* 0x2EC */ s32 active;
} Game10CD70FadeState;

/* Counts the fade timer down (returning 1 once it expires) and rewrites the G_SETTILESIZE of
 * the object's display list: uls follows the fade, ult steps back by a third of the old uls. */
s32 func_150DFDD0(Game10CD70Object *obj, Game10CD70FadeState *state) {
    f32 fade;
    GfxCmd *dl;
    u32 w0;
    s32 j;
    s32 ult;

    if (state->active != 0) {
        obj->unk13 = 2;
        state->timer -= D_800BE9A4;
        if (state->timer < 0.0f) {
            state->active = 0;
            return 1;
        }
        fade = 1.0f - (state->rate * state->timer);
    } else {
        fade = 0.0f;
        obj->unk13 = 0;
    }

    dl = *obj->dl;
    if (dl != NULL) {
        j = 0;
        while (GFX_CMD(dl, j)->w0.opcode != (s8)G_SETTILESIZE) {
            j++;
        }
        w0 = dl[j].w0.word;
        ult = 2 - (s32)((w0 >> 12) & 0xFFF) / 3;
        while (ult < 0) {
            ult += 0x40;
        }
        dl[j].w0.word = (G_SETTILESIZE << 24) | (((s32)((500.0f * fade) + 2.0f) & 0xFFF) << 12) | (ult & 0xFFF);
    }

    return 0;
}

/* The 12-byte record at +0x28 that func_150E02C0 also hands to func_15149514 as
 * (arg0 + 0x28, arg0 + 0x2C): the source actor (with its unique_id, to detect a reused slot)
 * plus three independent countdowns. */
typedef struct {
    /* 0x00 */ struct127 *obj;
    /* 0x04 */ u8  id;
    /* 0x05 */ u8  pad5;
    /* 0x06 */ s16 flashTimer;
    /* 0x08 */ s16 boltTimer;
    /* 0x0A */ s16 timer2;
} BoltSource; /* size 0xC */

typedef struct {
    /* 0x00 */ u8  pad0;
    /* 0x01 */ u8  unk1;
    /* 0x02 */ u8  pad2[0xA];
    /* 0x0C */ u8  unkC;
    /* 0x0D */ u8  padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8  pad10[0x18];
    /* 0x28 */ BoltSource src;
} BoltEmitter;

extern struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
extern void func_15107700(struct127 *, s16, s16, s16, s32, f32, f32, s16, s16, u8 *, u8, s32);
extern void func_15107B78(struct127 *, s32, s32, s32, s32);

/* Spawns flashes (func_151602C0), light-blue bolts (func_15107700) and func_15107B78 effects
 * around the emitter's source actor, each on its own random timer,
 * and ends the emitter (unkE = -1) once that actor is gone or its slot was reused. */
void func_150DFEFC(BoltEmitter *emitter) {
    BoltSource *src;
    struct127 *obj;
    Header header;
    Header2 pos;
    s8 count;

    obj = emitter->src.obj;
    src = &emitter->src;
    if (obj->interaction_state == 0 || obj->unique_id != src->id) {
        emitter->unkE = -1;
        return;
    }
    src->flashTimer -= D_800BE9E4;
    if (src->flashTimer < 0) {
        pos.unk0 = obj->x_position;
        pos.unk4 = obj->y_position;
        pos.unk8 = obj->z_position;
        header.unk0 = 3;
        header.unk1 = -1;
        header.unk2 = func_150ADA20() % 11U + 5;
        header.unk4 = 0;
        func_151602C0(&header, &pos, func_150ADA20() % 121U + 50, 255, 255, 255, 255, 0, 0,
                      emitter->unkC, emitter->unk1);
        src->flashTimer = func_150ADA20() % 81U + 69;
    }
    src->boltTimer -= D_800BE9E4;
    if (src->boltTimer < 0) {
        u8 colour[4];

        /* 2-4 bolts */
        count = func_150ADA20() % 3U + 2;
        do {
            colour[0] = 174;
            colour[1] = 210;
            colour[2] = 255;
            colour[3] = func_150ADA20() % 61U + 150;
            func_15107700(obj, func_150ADA20() & 0xFF, func_150ADA20() % 129U - 63,
                          func_150ADA20() % 21U + 10, 4, 40.0f, func_150ADA68() * 20.0f + 25.0f, 0, 2,
                          colour, emitter->unkC, emitter->unk1);
            count--;
        } while (count > 0);
        src->boltTimer = func_150ADA20() % 141U + 25;
    }
    src->timer2 -= D_800BE9E4;
    if (src->timer2 < 0) {
        func_15107B78(obj, (s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() & 0x7F) - 64),
                      emitter->unkC, emitter->unk1);
        src->timer2 = func_150ADA20() % 26U + 15;
    }
}

/* Passes the emitter's source record (actor and unique_id) to func_15149514. */
void func_150E02C0(BoltEmitter *emitter, s32 arg1, u8 arg2) {
    func_15149514(arg1, arg2, (s32)&emitter->src.obj, (s32)&emitter->src.id, (s32)emitter);
}

void func_1513BAE8(void);

/* One-time setup, guarded by the D_80088980 flag. */
void func_150E0300(void) {
    if (D_80088980 == 0) {
        func_1515F170(6, 0);
        func_1513BAE8();
        D_80088980 = 1;
    }
}

/* Template for func_1513418C's blood-drip effect: copied to the effect's +0x10 (BloodDrip in
 * game_161520.c). */
typedef struct {
    /* 0x00 */ s32 target[2];
    /* 0x08 */ u8 ownerId;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ struct127 *owner;
    /* 0x10 */ u8 bone;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ struct17 offset;
    /* 0x20 */ f32 radius;
    /* 0x24 */ f32 rate;
    /* 0x28 */ s16 lifetime;
    /* 0x2A */ u8 flags;
    /* 0x2B */ u8 emitter;
    /* 0x2C */ s8 callback;
    /* 0x2D */ u8 unk2D;
} BloodDripDesc;

extern void *func_1513418C(BloodDripDesc *, s32, u8, s32);

/* Attaches a blood drip to the actor's bone 0. */
void func_150E0348(struct127 *actor, u8 arg1, s32 arg2) {
    BloodDripDesc desc;

    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.bone = 0;
    desc.owner = actor;
    desc.offset = *(struct17 *)&D_800A5480; /* a Vec3; variables.h declares it as one f32 */
    desc.lifetime = 300;
    desc.flags = 0x1B; /* FOLLOW_OWNER | ON_BONE | NO_PREVIOUS | STATIC */
    desc.emitter = 11; /* D_80089A70[11] = func_150E03F8 */
    desc.callback = -1; /* none */
    desc.unk2D = 0;
    desc.radius = 10.90000057f;
    desc.rate = 0.2312099934f;
    func_1513418C(&desc, 0, arg1, arg2);
}

void func_15143794(s16, s16, f32, void *);
void func_151A26EC(f32 *, f32 *, f32 *, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* The leading bytes of an effect (cf. struct102); only unk1 and unkC are read here. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
} EffectHeader;

/* Blood-drip emitter callback (D_80089A70): spawns one drop at (x, y, z), flung in a random
 * direction and pushed against the drip's movement (dx, dy, dz). */
void func_150E03F8(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect) {
    f32 position[3];
    f32 velocity[3];
    f32 acceleration[3];
    f32 scale;
    f32 size_random;
    u32 random_b;
    u32 random_a;
    f32 alpha_random;

    position[0] = x;
    position[1] = y;
    position[2] = z;
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    alpha_random = func_150ADA68();
    func_15143794(random_a & 0xFF, random_b % 65U - 32, alpha_random * 5.87f, velocity);
    scale = func_150ADA68() * 0.16900001f + 0.186f;
    acceleration[0] = 0.0f;
    acceleration[1] = 0.0f;
    acceleration[2] = 0.0f;
    velocity[0] += -dx * D_800BE9A8 * scale;
    velocity[1] += -dy * D_800BE9A8 * scale;
    velocity[2] += -dz * D_800BE9A8 * scale;
    alpha_random = func_150ADA68();
    size_random = func_150ADA68();
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    func_151A26EC(position, acceleration, velocity, 0.986891f, alpha_random * 0.23f + -0.337f,
                  size_random * 200.0f + 150.0f, random_a % 26U + 40, random_b % 201U + 55, 30, 20, 0, -1,
                  52, 53, 52, effect->unkC, effect->unk1);
}

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

/* The object func_150E05F8 fades (a handler in the 0x80089xxx object table; cf. struct131). */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x74];
    /* 0x8A */ u8 alpha;
} DistanceFadeObject;

/* matching: 3850.0f; as a literal IDO folds 1 / (3850 - 2570) to a constant */
extern f32 D_800A0FD4;

/* Sets alpha from the distance to D_800DBFF0's unk2F8 point: opaque within 2570, fading out
 * linearly to transparent at 3850 (D_800A0FD4). */
void func_150E05F8(DistanceFadeObject *obj) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    s32 alpha;

    dz = D_800DBFF0->unk300 - obj->z;
    dx = D_800DBFF0->unk2F8 - obj->x;
    dy = D_800DBFF0->unk2FC - obj->y;
    dist = sqrtf(dz * dz + (dx * dx + dy * dy));
    if (dist <= 2570.0f) {
        alpha = 255;
    } else if (dist >= D_800A0FD4) {
        alpha = 0;
    } else {
        alpha = 255 - (dist - 2570.0f) * (1 / (D_800A0FD4 - 2570.0f)) * 255.0f;
    }
    obj->alpha = alpha;
}

extern u8 D_80088988;
extern f32 D_800A0F70[][3];
extern f32 D_800A0F88[][3];
extern s32 func_15145EA4(f32 **src, f32 **dst, Mtx *mtx, s32 count);
extern void func_15102B38(struct127 *, u8, f32 *, f32 *, f32 *, s32, s32, f32, struct17 *, s32, s32, s32, u8,
                          s32);

/* Spawns a func_15102B38 effect between points D_800A0F70[slot] and D_800A0F88[slot] of the
 * actor's bone D_80088988, starting at the first point in world space. */
void func_150E06D8(struct127 *actor, u8 slot, u8 arg2, s32 arg3) {
    struct17 start;
    f32 scale;
    f32 params[2];
    f32 *src[1];
    f32 *dst[1];
    f32 rand0;
    s32 randB;
    s32 randA;

    if (actor->unk1D4 == NULL) {
        return;
    }
    src[0] = D_800A0F70[slot];
    dst[0] = &start.unk0;
    func_15145EA4(src, dst, (Mtx *)actor->unk1D4 + D_80088988, 1);
    scale = 1.0f; // matching: IDO keeps the multiply by 1.0 only through a variable
    params[1] = func_150ADA68() * scale + 2.0f;
    params[0] = func_150ADA68() * 14.0f + 28.0f;
    randA = func_150ADA20();
    randB = func_150ADA20();
    rand0 = func_150ADA68() * 300.0f + 400.0f;
    func_15102B38(actor, D_80088988, D_800A0F70[slot], D_800A0F88[slot], params, randA % 3U + 4,
                  randB % 156U + 100, rand0, &start, 255, 0, -1, arg2, arg3);
}
