#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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

/* Set once the one-time setup of func_150E0300 has run (cleared by game_3BA70.c and game_3D9A0.c). */
u8 D_80088980 = 0;
static Game10CD70Indices D_80088984 = { { 0x3B, 0x3C, 0x3D } };

/* Per zone (D_80088984): the glow level of its scrolling material, 0..0x60 (cleared by game_3BA70.c and game_3D9A0.c). */
u8 D_800D9950[3];

/* Returns whether the zones record selected by D_80088984[index] is active. */
s32 func_150DF8C0(s32 index) {
    Game10CD70Indices indices = D_80088984;

    if (zones[indices.values[index]].inactive != 0) {
        return 1;
    }
    return 0;
}

/* Points slots 3 and 4 (func_150A2864/func_150A3444) 200 units above actors 6 and 7, or
 * disables them when the actor is missing, then scrolls the texture of each of the three
 * materials in the D_800B0E00 display list that loads its image from segment 2 + i. */
void func_150DF920(s32 arg0) {
    Actor *obj;
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
        obj = func_15083E90(id);
        id = 7;
        if (obj != NULL) {
            func_150A2864(slot, 0);
            x = obj->xpos;
            y = obj->ypos + 200.0f;
            z = obj->zpos;
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

/* The object func_150DFCA8 animates (a handler in the 0x80088xxx object table). */
struct SwingObject {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ f32 pos;
    /* 0x08 */ u8 pad8[0x5C];
    /* 0x64 */ f32 speed;
    /* 0x68 */ u8 pad68[0x14];
    /* 0x7C */ f32 targetSpeed;
};

/* Moves pos back and forth between 11 and 55: the target speed flips sign at either end and
 * the speed eases towards it by 0.05 per call. */
void func_150DFCA8(SwingObject *obj) {
    f32 pos;
    f32 target;
    f32 maxSpeed = 0.55f; // matching: -0.55f as a literal folds; IDO negates the variable at run time

    // matching: the step shares target's register
    target = obj->speed * D_800BE9E4;
    obj->pos += target;
    pos = obj->pos;
    if (pos < 11.0f) {
        obj->targetSpeed = target = 0.55f;
    } else if (obj->pos > 55.0f) {
        obj->targetSpeed = target = -maxSpeed;
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

/* Moves bone 1 of the actor to its x/z position at a fixed height of 2706. */
void func_150DFDA4(Actor *actor) {
    f32 (*bone)[4] = actor->ob_matrix_ptr[1];

    bone[3][0] = actor->xpos;
    bone[3][1] = 2706.0f;
    bone[3][2] = actor->zpos;
}

/* Model attachment hook 12 (D_800841C0[12]). While the owner's fade is active (taskint[2]), counts
 * its timer (unk2D8[0]) down, returning 1 once it expires, and rewrites the G_SETTILESIZE of the
 * attachment's display list: uls follows the fade, ult steps back by a third of the old uls. */
s32 func_150DFDD0(ModelAttachment *att, Actor *owner) {
    f32 fade;
    GfxCmd *dl;
    u32 w0;
    s32 j;
    s32 ult;

    if (owner->taskint[2] != 0) {
        att->hiddenDLists = 2;
        owner->unk2D8[0] -= gFrameDelta;
        if (owner->unk2D8[0] < 0.0f) {
            owner->taskint[2] = 0;
            return 1;
        }
        fade = 1.0f - (owner->unk2D8[0] * owner->unk2D8[1]);
    } else {
        fade = 0.0f;
        att->hiddenDLists = 0;
    }

    dl = (GfxCmd *)*att->dlists;
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


struct BoltEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoltSource src;
};

/* Spawns flashes (light_createWithEffect), light-blue bolts (func_15107700) and func_15107B78 effects
 * around the emitter's source actor, each on its own random timer,
 * and ends the emitter (unkE = -1) once that actor is gone or its slot was reused. */
void func_150DFEFC(BoltEmitter *emitter) {
    BoltSource *src;
    Actor *obj;
    LightEffectHeader header;
    Vec3i pos;
    s8 count;

    obj = emitter->src.obj;
    src = &emitter->src;
    if (obj->type == 0 || obj->unique_id != src->id) {
        emitter->base.header.state = -1;
        return;
    }
    src->flashTimer -= D_800BE9E4;
    if (src->flashTimer < 0) {
        pos.x = obj->xpos;
        pos.y = obj->ypos;
        pos.z = obj->zpos;
        header.flags = 3;
        header.update = -1;
        header.lifetime = func_150ADA20() % 11U + 5;
        header.lightType = 0;
        light_createWithEffect(&header, &pos, func_150ADA20() % 121U + 50, 255, 255, 255, 255, 0, 0,
                      emitter->base.header.owner, emitter->base.header.kind);
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
                          colour, emitter->base.header.owner, emitter->base.header.kind);
            count--;
        } while (count > 0);
        src->boltTimer = func_150ADA20() % 141U + 25;
    }
    src->timer2 -= D_800BE9E4;
    if (src->timer2 < 0) {
        func_15107B78(obj, func_150ADA20() & 0xFF, (func_150ADA20() & 0x7F) - 64,
                      emitter->base.header.owner, emitter->base.header.kind);
        src->timer2 = func_150ADA20() % 26U + 15;
    }
}

/* Passes the emitter's source record (actor and unique_id) to func_15149514. */
void func_150E02C0(BoltEmitter *emitter, s32 arg1, u8 arg2) {
    func_15149514(arg1, arg2, &emitter->src.obj, &emitter->src.id, emitter);
}

/* One-time setup, guarded by the D_80088980 flag. */
void func_150E0300(void) {
    if (D_80088980 == 0) {
        light_setGroupDisabled(6, 0);
        func_1513BAE8();
        D_80088980 = 1;
    }
}

/* Attaches a blood drip to the actor's bone 0. */
void func_150E0348(Actor *actor, u8 arg1, s32 arg2) {
    BloodDripDesc desc;

    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.bone = 0;
    desc.owner = actor;
    desc.offset = D_800A5480;
    desc.lifetime = 300;
    desc.flags = 0x1B; /* FOLLOW_OWNER | ON_BONE | NO_PREVIOUS | STATIC */
    desc.emitter = 11; /* D_80089A70[11] = func_150E03F8 */
    desc.callback = -1; /* none */
    desc.unk2D = 0;
    desc.radius = 10.90000057f;
    desc.rate = 0.2312099934f;
    func_1513418C(&desc, 0, arg1, arg2);
}

/* Blood-drip emitter callback (D_80089A70): spawns one drop at (x, y, z), flung in a random
 * direction and pushed against the drip's movement (dx, dy, dz). */
void func_150E03F8(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect) {
    Vec3f position;
    Vec3f velocity;
    Vec3f acceleration;
    f32 scale;
    f32 size_random;
    u32 random_b;
    u32 random_a;
    f32 alpha_random;

    position.x = x;
    position.y = y;
    position.z = z;
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    alpha_random = func_150ADA68();
    func_15143794(random_a & 0xFF, random_b % 65U - 32, alpha_random * 5.87f, &velocity);
    scale = func_150ADA68() * 0.16900001f + 0.186f;
    acceleration.x = 0.0f;
    acceleration.y = 0.0f;
    acceleration.z = 0.0f;
    velocity.x += -dx * gFrameDeltaInv * scale;
    velocity.y += -dy * gFrameDeltaInv * scale;
    velocity.z += -dz * gFrameDeltaInv * scale;
    alpha_random = func_150ADA68();
    size_random = func_150ADA68();
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    func_151A26EC(&position, &acceleration, &velocity, 0.986891f, alpha_random * 0.23f + -0.337f,
                  size_random * 200.0f + 150.0f, random_a % 26U + 40, random_b % 201U + 55, 30, 20, 0, -1,
                  52, 53, 52, effect->owner, effect->kind);
}

/* Sets alpha from the distance to camera_global_Cameras's unk2F8 point: opaque within 2570, fading out
 * linearly to transparent at 3850. */
void func_150E05F8(WorldObject *obj) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    s32 alpha;
    f32 far = 3850.0f; // matching: with the literal inline, IDO folds 1 / (3850 - 2570) to a constant

    dz = camera_global_Cameras->eye.z - obj->z;
    dx = camera_global_Cameras->eye.x - obj->x;
    dy = camera_global_Cameras->eye.y - obj->y;
    dist = sqrtf(dz * dz + (dx * dx + dy * dy));
    if (dist <= 2570.0f) {
        alpha = 255;
    } else if (dist >= far) {
        alpha = 0;
    } else {
        alpha = 255 - (dist - 2570.0f) * (1 / (far - 2570.0f)) * 255.0f;
    }
    obj->alpha = alpha;
}

static u8 D_80088988 = 2;
/* Effect start and end points (bone space) of the two slots. */
static const Vec3f D_800A0F70[2] = { { 17.0f, 0.0f, 90.0f }, { -17.0f, 0.0f, 90.0f } };
static const Vec3f D_800A0F88[2] = { { 17.0f, 0.0f, 150.0f }, { -17.0f, 0.0f, 150.0f } };

/* Spawns a func_15102B38 effect between points D_800A0F70[slot] and D_800A0F88[slot] of the
 * actor's bone D_80088988, starting at the first point in world space. */
void func_150E06D8(Actor *actor, u8 slot, u8 arg2, s32 arg3) {
    Vec3f start;
    f32 scale;
    ParticleSize params;
    const Vec3f *src[1];
    Vec3f *dst[1];
    f32 rand0;
    s32 randB;
    s32 randA;

    if (actor->ob_matrix_ptr == NULL) {
        return;
    }
    src[0] = &D_800A0F70[slot];
    dst[0] = &start;
    func_15145EA4(src, dst, actor->ob_matrix_ptr + D_80088988, 1);
    scale = 1.0f; // matching: IDO keeps the multiply by 1.0 only through a variable
    params.height = func_150ADA68() * scale + 2.0f;
    params.width = func_150ADA68() * 14.0f + 28.0f;
    randA = func_150ADA20();
    randB = func_150ADA20();
    rand0 = func_150ADA68() * 300.0f + 400.0f;
    func_15102B38(actor, D_80088988, &D_800A0F70[slot], &D_800A0F88[slot], &params, randA % 3U + 4,
                  randB % 156U + 100, rand0, &start, 255, 0, -1, arg2, arg3);
}
