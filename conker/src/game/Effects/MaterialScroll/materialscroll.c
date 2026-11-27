#include <ultra64.h>
#include <math.h>
#include "conker/beam.h"
#include "conker/blood.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/texture.h"
#include "conker/zone.h"

/*
 * Placed among the LEVEL_W2_HARBOUR files: effect_levelUpdateMaterialScroll is level update hook 7, which only that
 * level's header selects, and that level's setup (level_setup15) clears gMaterialGlowLevels and gWarpModelSetupDone.
 * No table ties the other functions of the file to a level.
 */

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

/* The indices into zones of the three scrolling materials (a struct: effect_isMaterialZoneActive copies it). */
typedef struct {
    u8 values[3];
} MaterialZoneIds;

/* Set once the one-time setup of effect_setupWarpModelOnce has run (cleared by game/Level/LevelSetupReset/levelsetupreset.c and game/Level/LevelSetup02/levelsetup02.c). */
u8 gWarpModelSetupDone = 0;
static MaterialZoneIds gMaterialZoneIds = { { 0x3B, 0x3C, 0x3D } };

/* Per zone (gMaterialZoneIds): the glow level of its scrolling material, 0..0x60 (cleared by game/Level/LevelSetupReset/levelsetupreset.c and game/Level/LevelSetup02/levelsetup02.c). */
u8 gMaterialGlowLevels[3];

/* Returns whether the zones record selected by gMaterialZoneIds[index] is active. */
s32 effect_isMaterialZoneActive(s32 index) {
    MaterialZoneIds indices = gMaterialZoneIds;

    if (zones[indices.values[index]].inactive != 0) {
        return 1;
    }
    return 0;
}

/* Points slots 3 and 4 (zone_setDisabled/zone_setPosition) 200 units above actors 6 and 7, or
 * disables them when the actor is missing, then scrolls the texture of each of the three
 * materials in the gLevelDisplayLists display list that loads its image from segment 2 + i. */
void effect_levelUpdateMaterialScroll(s32 arg0) {
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

    effect_advanceLevelTexFrame(5, 4);

    id = 6;
    slot = 3;
    for (i = 0; i < 2; i++) {
        obj = collision_findObjectById(id);
        id = 7;
        if (obj != NULL) {
            zone_setDisabled(slot, 0);
            x = obj->xpos;
            y = obj->ypos + 200.0f;
            z = obj->zpos;
            zone_setPosition(slot, x, y, z);
        } else {
            zone_setDisabled(slot, 1);
        }
        slot = 4;
    }

    dl = (GfxCmd *)gLevelDisplayLists[0];

    for (i = 0; i < 3; i++) {
        active = effect_isMaterialZoneActive(i);
        if (active) {
            if (gMaterialGlowLevels[i] < 0x60) {
                gMaterialGlowLevels[i] += gFrameTicks;
                if (gMaterialGlowLevels[i] > 0x60) {
                    gMaterialGlowLevels[i] = 0x60;
                }
            }
        } else {
            gMaterialGlowLevels[i] = 0;
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
                    ult += (gMaterialGlowLevels[i] / 16) * gFrameTicks;
                } else {
                    ult = 2;
                    uls = 2;
                }
                dl[j].w0.word = (G_SETTILESIZE << 24) | ((uls & 0xFFF) << 12) | (ult & 0xFFF);
            }
        }
    }
}


/* Binds segments 2..4 to the textures of the three materials effect_levelUpdateMaterialScroll scrolls: an
 * active material gets entry 9 + gLevelTexFrame of D_80090324, an inactive one entry 8. */
Gfx *effect_bindMaterialTextures(Gfx *gdl) {
    s32 i;
    s32 segment;
    s32 addr;
    s32 unused;
    s32 asset;
    u8 variant;

    for (i = 0; i < 3; i++) {
        if (effect_isMaterialZoneActive(i) != 0) {
            variant = gLevelTexFrame;
            asset = D_80090324[variant + 9];
        } else {
            asset = D_80090324[8];
        }
        addr = texture_getAddress(asset, &unused, 3, 0);
        segment = i + 2;
        gSPSegment(gdl++, segment, addr);
    }
    return gdl;
}

/* The object effect_updateSwingObject animates (a handler in the 0x80088xxx object table). */
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
void effect_updateSwingObject(SwingObject *obj) {
    f32 pos;
    f32 target;
    f32 maxSpeed = 0.55f; // matching: -0.55f as a literal folds; IDO negates the variable at run time

    // matching: the step shares target's register
    target = obj->speed * gFrameTicks;
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

/* Model attachment hook 12 (gAttachHooks[12]). While the owner's fade is active (taskint[2]), counts
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

/* Spawns flashes (light_createWithEffect), light-blue bolts (beam_spawnBoundsBolt) and beam_spawnBoundsImpact effects
 * around the emitter's source actor, each on its own random timer,
 * and ends the emitter (unkE = -1) once that actor is gone or its slot was reused. */
void effect_updateBoltEmitter(BoltEmitter *emitter) {
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
    src->flashTimer -= gFrameTicks;
    if (src->flashTimer < 0) {
        pos.x = obj->xpos;
        pos.y = obj->ypos;
        pos.z = obj->zpos;
        header.flags = 3;
        header.update = -1;
        header.lifetime = math_random() % 11U + 5;
        header.lightType = 0;
        light_createWithEffect(&header, &pos, math_random() % 121U + 50, 255, 255, 255, 255, 0, 0,
                      emitter->base.header.owner, emitter->base.header.kind);
        src->flashTimer = math_random() % 81U + 69;
    }
    src->boltTimer -= gFrameTicks;
    if (src->boltTimer < 0) {
        u8 colour[4];

        /* 2-4 bolts */
        count = math_random() % 3U + 2;
        do {
            colour[0] = 174;
            colour[1] = 210;
            colour[2] = 255;
            colour[3] = math_random() % 61U + 150;
            beam_spawnBoundsBolt(obj, math_random() & 0xFF, math_random() % 129U - 63,
                          math_random() % 21U + 10, 4, 40.0f, math_randomFloat() * 20.0f + 25.0f, 0, 2,
                          colour, emitter->base.header.owner, emitter->base.header.kind);
            count--;
        } while (count > 0);
        src->boltTimer = math_random() % 141U + 25;
    }
    src->timer2 -= gFrameTicks;
    if (src->timer2 < 0) {
        beam_spawnBoundsImpact(obj, math_random() & 0xFF, (math_random() & 0x7F) - 64,
                      emitter->base.header.owner, emitter->base.header.kind);
        src->timer2 = math_random() % 26U + 15;
    }
}

/* Passes the emitter's source record (actor and unique_id) to effect_handleTimedEffectOwnerEvent. */
void effect_boltEmitterMessage(BoltEmitter *emitter, s32 arg1, u8 arg2) {
    effect_handleTimedEffectOwnerEvent(arg1, arg2, &emitter->src.obj, &emitter->src.id, emitter);
}

/* One-time setup, guarded by the gWarpModelSetupDone flag. */
void effect_setupWarpModelOnce(void) {
    if (gWarpModelSetupDone == 0) {
        light_setGroupDisabled(6, 0);
        effect_spawnWarpModel();
        gWarpModelSetupDone = 1;
    }
}

/* Attaches a blood drip to the actor's bone 0. */
void effect_attachBloodDrip(Actor *actor, u8 arg1, s32 arg2) {
    BloodDripDesc desc;

    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.bone = 0;
    desc.owner = actor;
    desc.offset = gZeroVec3f;
    desc.lifetime = 300;
    desc.flags = 0x1B; /* FOLLOW_OWNER | ON_BONE | NO_PREVIOUS | STATIC */
    desc.emitter = 11; /* gBloodDripEmitters[11] = effect_spawnDripDrop */
    desc.callback = -1; /* none */
    desc.handlerSet = 0;
    desc.radius = 10.90000057f;
    desc.rate = 0.2312099934f;
    blood_spawnDrip(&desc, 0, arg1, arg2);
}

/* Blood-drip emitter callback (gBloodDripEmitters): spawns one drop at (x, y, z), flung in a random
 * direction and pushed against the drip's movement (dx, dy, dz). */
void effect_spawnDripDrop(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect) {
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
    random_a = math_random();
    random_b = math_random();
    alpha_random = math_randomFloat();
    math_dirFromBinAngles(random_a & 0xFF, random_b % 65U - 32, alpha_random * 5.87f, &velocity);
    scale = math_randomFloat() * 0.16900001f + 0.186f;
    acceleration.x = 0.0f;
    acceleration.y = 0.0f;
    acceleration.z = 0.0f;
    velocity.x += -dx * gFrameDeltaInv * scale;
    velocity.y += -dy * gFrameDeltaInv * scale;
    velocity.z += -dz * gFrameDeltaInv * scale;
    alpha_random = math_randomFloat();
    size_random = math_randomFloat();
    random_a = math_random();
    random_b = math_random();
    particle_spawnDragFadeSprite(&position, &acceleration, &velocity, 0.986891f, alpha_random * 0.23f + -0.337f,
                  size_random * 200.0f + 150.0f, random_a % 26U + 40, random_b % 201U + 55, 30, 20, 0, -1,
                  52, 53, 52, effect->owner, effect->kind);
}

/* Sets alpha from the distance to camera_global_Cameras's unk2F8 point: opaque within 2570, fading out
 * linearly to transparent at 3850. */
void effect_fadeByCameraDistance(WorldObject *obj) {
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

static u8 gSlotBoneBeamBone = 2;
/* Effect start and end points (bone space) of the two slots. */
static const Vec3f gSlotBoneBeamStarts[2] = { { 17.0f, 0.0f, 90.0f }, { -17.0f, 0.0f, 90.0f } };
static const Vec3f gSlotBoneBeamEnds[2] = { { 17.0f, 0.0f, 150.0f }, { -17.0f, 0.0f, 150.0f } };

/* Spawns a effect_spawnBoneBeamParticle effect between points gSlotBoneBeamStarts[slot] and gSlotBoneBeamEnds[slot] of the
 * actor's bone gSlotBoneBeamBone, starting at the first point in world space. */
void effect_spawnSlotBoneBeam(Actor *actor, u8 slot, u8 arg2, s32 arg3) {
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
    src[0] = &gSlotBoneBeamStarts[slot];
    dst[0] = &start;
    math_transformOffsets(src, dst, actor->ob_matrix_ptr + gSlotBoneBeamBone, 1);
    scale = 1.0f; // matching: IDO keeps the multiply by 1.0 only through a variable
    params.height = math_randomFloat() * scale + 2.0f;
    params.width = math_randomFloat() * 14.0f + 28.0f;
    randA = math_random();
    randB = math_random();
    rand0 = math_randomFloat() * 300.0f + 400.0f;
    effect_spawnBoneBeamParticle(actor, gSlotBoneBeamBone, &gSlotBoneBeamStarts[slot], &gSlotBoneBeamEnds[slot], &params, randA % 3U + 4,
                  randB % 156U + 100, rand0, &start, 255, 0, -1, arg2, arg3);
}
