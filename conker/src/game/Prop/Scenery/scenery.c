#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* not referenced: four points (x, y, z) with a radius */
const s16 D_800A00E0[4][4] = {
    { 0, 1035, 505, 200 },
    { 0, 1035, -400, 200 },
    { 479, 1035, 0, 200 },
    { -479, 1035, 0, 200 },
};

void func_150BEF70(s32 arg0) {
}

/* Starts two mirrored particle emitters at (0x1FDB, 0x25, +/-0x17C) whose timer is the low half of arg0. */
void prop_startMirroredEmitters(s32 arg0) {
    s32 i;
    s32 z;
    s32 timer;

    timer = arg0 & 0xFFFF;
    z = 0x17C;
    for (i = 0; i < 2; i++) {
        particle_newSpriteEmitter(0x1FDB, 0x25, z, 0xD,
            0, 0x50, 0x8F, 0, 0, 0, 0, 0,
            8, 0xC8, 0xA, 0, 0, 0, 0, 0,
            0x28, 0x28, 4, 0, 0, 0, 0, 0x555,
            0x555, 0x555, 0x555, timer, 0x32, 0, 0xFF, 0x14,
            0xFA0, 0x7D0, 1, 6, 0, 1, 0, 0,
            0, 0, 3, 0xFF, 0);
        z = -z;
    }
}

/* Steps the entry by D_800BE9E4 frames; returns 1 once it has faded out or shrunk to nothing. */
s32 func_150BF0F4(Struct150BF0F4 *entry) {
    s32 intensity;
    s32 limit;
    s32 delta;
    s32 size;
    s32 age;

    intensity = entry->level;
    limit = entry->maxLevel;
    if (entry->rising) {
        if (intensity != limit) {
            intensity += D_800BE9E4 * entry->riseRate;
            if (intensity > limit) {
                intensity = limit;
            }
            entry->level = intensity;
        }
    } else if (intensity != 0) {
        intensity -= D_800BE9E4 * entry->fallRate;
        if (intensity < 0) {
            intensity = 0;
        }
        entry->level = intensity;
    }

    if (!entry->rising && intensity == 0) {
        return 1;
    }

    delta = entry->unk14Rate;
    delta *= D_800BE9E4;
    entry->unk14 += delta;
    size = entry->unk14;
    delta = entry->unk16Rate;
    delta *= D_800BE9E4;
    entry->unk16 += delta;
    if (size <= 0 || size <= 0) { /* sic: unk16 is never tested */
        entry->unk16 = 0;
        entry->unk14 = entry->unk16;
        return 1;
    }

    age = entry->age;
    age += D_800BE9E4;
    if (age >= 0x80) {
        age = 0x7F;
    }
    entry->age = age;
    return 0;
}

/*
 * Behaviour scratch state (WorldObject.touch) of func_150BF21C: an object whose mesh is translated toward a
 * target height (param) and bobs (rotX-rotZ) while it is at rest.
 */
typedef struct {
    /* 0x7C */ f32 height;
    /* 0x80 */ f32 bobPhase;
    /* 0x84 */ f32 speed;
} BobWork;

#define BOB_WORK(obj) ((BobWork *)&(obj)->touch)

/* Moves the object toward its target height (param) (accelerating, with sounds), bobs it while at rest, then
 * writes its transformed mesh into the current vertex buffer. */
void func_150BF21C(WorldObject *obj) {
    Vtx *dst;
    Vtx *src;
    f32 diff;
    f32 x;
    f32 height;
    f32 z;
    f32 speed;
    s32 paused;
    f32 bob;
    MtxF translate;
    MtxF model;
    f32 rate;
    s32 i;
    f32 bobXZ;

    height = BOB_WORK(obj)->height;
    bob = 0.0f;
    rate = 0.05f;
    if (gCutsceneStates[0] == 0) {
        paused = FALSE;
    } else if (gCutsceneIds[0] == 1) {
        paused = FALSE;
    } else {
        paused = TRUE;
    }
    if (D_800BE9B4 != 0) {
        height = obj->moveData.word;
        BOB_WORK(obj)->height = height;
    }
    if (!paused) {
        diff = obj->moveData.word - height;
        if (diff == 0.0f) {
            BOB_WORK(obj)->speed = 0.0f;
            BOB_WORK(obj)->bobPhase += 0.05f;
            bob = ((height / 1500.0f) + 1.0f) * gFrameDelta;
            if (bob < 0.0f) {
                bob = 0.0f;
            }
            gfx_playObjectSound(obj, -1, 0x7D00, 500, 2000, 0);
        } else {
            rate = 0.2f;
            BOB_WORK(obj)->bobPhase = 0.0f;
            speed = BOB_WORK(obj)->speed;
            if (diff < 0.0f) {
                if (speed > -60.0f) {
                    speed -= 5.0f;
                }
            } else {
                if (speed < 6.0f) {
                    speed += 0.5f;
                }
                gfx_playObjectSound(obj, 0xB9, 0x7D00, 500, 2000, 1);
            }
            BOB_WORK(obj)->speed = speed;
            if (fabsf(speed) < fabsf(diff)) {
                height += speed * gFrameDelta;
                if (speed < -25.0f) {
                    BOB_WORK(&obj[1])->speed = -10.0f;
                } else {
                    BOB_WORK(&obj[1])->speed = speed * 0.4000000060f;
                }
                obj[1].moveData.word = 0x7D00;
                obj->task_flag &= ~TASK_FLAG_STATE;
                obj->task_flag |= TASK_FLAG_GOING_ON;
                if (obj->sound_id == 0) {
                    gfx_playObjectSound(obj, 0xB9, 0x3E80, 1000, 4000, 0);
                }
                gfx_startObjectSound(obj, 0x3E80, speed * 500.0f);
            } else {
                height = obj->moveData.word;
                if (speed < 0.0f) {
                    obj[1].moveData.word = 0x80;
                }
                obj->task_flag &= ~TASK_FLAG_STATE;
                obj->task_flag |= TASK_FLAG_ON;
                BOB_WORK(&obj[1])->speed = 0.0f;
                gfx_playObjectSound(obj, -1, 0, 0, 0, 0);
            }
            BOB_WORK(obj)->height = height;
            BOB_WORK(obj)->speed = speed;
        }
        bobXZ = bob * 3.0f;
        obj->t += ((bobXZ * sinf(BOB_WORK(obj)->bobPhase)) - obj->t) * rate;
        obj->u += (((bob * 4.0f) * sinf(BOB_WORK(obj)->bobPhase * 1.100000024f)) - obj->u) * rate;
        obj->v += ((bobXZ * cosf(BOB_WORK(obj)->bobPhase)) - obj->v) * rate;
    } else {
        height = 0.0f;
    }
    math_mtxTranslateF(translate, 0.0f, height, 0.0f);
    math_mtxFRotScaleTrans(model, obj->t, obj->u, obj->v, obj->xScale, obj->yScale, obj->zScale, obj->x, obj->y, obj->z);
    math_mtxMulF(translate, model, translate);
    dst = obj->vertices[gDrawBufferIndex];
    src = obj->srcVertices;
    for (i = 0; i < obj->numVertices; i++) {
        math_transformPoint(translate, src->v.ob[0], src->v.ob[1], src->v.ob[2], &x, &height, &z);
        dst->v.ob[0] = x;
        dst->v.ob[1] = height;
        dst->v.ob[2] = z;
        src++;
        dst++;
    }
}


/* Behaviour scratch state (WorldObject.touch) of the entry after func_150BF760's object. */
typedef struct {
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ void *texScroll; // from flamethrower_addObjectTexScroll
} ScrollWork;

#define SCROLL_WORK(obj) ((ScrollWork *)&(obj)->touch)

/* Drives object 0x14 according to the mode in obj->task_flag, spawning it first when a mode needs it,
 * and sets obj[1].rotZ to a sine of the object's animation progress. */
void func_150BF760(WorldObject *obj) {
    Actor *actor;
    AnimInstance *anim;
    s32 mode;
    s32 slot;
    f32 progress = 0.0f;

    if (gCutsceneStates[0] != 1 || gCutsceneIds[0] == 1) {
        func_1511B51C(obj);
        if (SCROLL_WORK(&obj[1])->texScroll == NULL) {
            SCROLL_WORK(&obj[1])->texScroll = flamethrower_addObjectTexScroll(obj, D_800902B8, 1, -1, 0, 0, -8);
        }
        actor = collision_findObjectById(0x14);
        mode = obj->task_flag & TASK_FLAG_STATE;
        if (actor != NULL) {
            anim = actor->unk2D0;
            progress = anim->frame[0] / (anim->numFrames[0] - 2.0f);
        }
        obj[1].v = 0.0f;
        if (mode == TASK_FLAG_GOING_ON) {
            if (progress <= 0.75f) {
                obj[1].v = -sinf(progress * 3.141592741f * 1.333333254f) * 10.0f;
            }
            if (actor == NULL) {
                slot = collision_findSpawnById(0x14);
                gSpawnRecords[slot].unk2 = 0;
                collision_trySpawnRecord(slot, 0);
                actor = collision_findObjectById(0x14);
            }
            if (actor->intelwalk != 2) {
                actor->intelwalk = 2;
                actor->scriptCmd = NULL;
            }
        } else if (mode == TASK_FLAG_ON) {
            if (actor != NULL && progress >= 1.0f) {
                actor_remove(actor, 0);
            }
        } else if (mode == TASK_FLAG_OFF) {
            if (progress >= 0.25f) {
                obj[1].v = sinf(progress * 4.188789368f - 4.188789368f) * 10.0f;
            }
            if (actor != NULL && progress >= 1.0f) {
                actor_remove(actor, 0);
            }
        } else if (mode == TASK_FLAG_GOING_OFF) {
            if (progress >= 0.25f) {
                obj[1].v = sinf(progress * 4.188789368f - 4.188789368f) * 10.0f;
            }
            if (actor == NULL) {
                slot = collision_findSpawnById(0x14);
                gSpawnRecords[slot].unk2 = 0;
                collision_trySpawnRecord(slot, 0);
                actor = collision_findObjectById(0x14);
            }
            if (actor == NULL && actor->intelwalk != 3) { /* sic: the original tests actor == NULL */
                actor->intelwalk = 3;
                actor->scriptCmd = NULL;
            }
        }
    }
}

/*
 * Behaviour scratch state (WorldObject.touch) of a platform that shakes (t/v) and sinks, one of a
 * pair whose partner entry follows it. moveData high half: kind; task_flag bits 0-1: state, bit 2: held down.
 */
typedef struct {
    /* 0x7C */ s32 home;  /* low half: rest height; bit 31: initialised */
    /* 0x80 */ s32 unk80; /* sinking timer; on the partner, its offset from the lowered height */
    /* 0x84 */ s32 unk84;
} PlatformWork;

#define WORK(obj) ((PlatformWork *)&(obj)->touch)

/* Platform update. State 0 rests at the home height, 2 shakes then sinks by `travel`, 3 stays down
 * and 1 rises back. Kind 1 platforms just follow their partner's offset. */
void prop_updateSinkingPlatform(WorldObject *platform) {
    s16 lowered;
    s16 top;
    s16 bottom;
    s32 pad; // matching: unused stack slot
    s32 travel;
    s32 step;
    s32 diff;
    s32 state;
    s32 kind;
    RumbleParams fx;

    step = 0x10;
    kind = (platform->moveData.word >> 16) & 0xFFFF;
    state = platform->task_flag & TASK_FLAG_STATE;
    travel = 0;
    if (kind == 0) {
        travel = 0x82;
    } else if (kind == 2) {
        travel = 0x622;
        step = 0x30;
    }
    if (WORK(platform)->home == 0) {
        WORK(platform)->home = platform->y | 0x80000000;
    }
    top = (WORK(platform)->home & 0xFFFF);
    lowered = top - 0x50;
    bottom = lowered - travel;
    if (kind == 1) {
        diff = top - platform->y;
        if (diff != WORK(platform)->unk80) {
            platform->y = top + WORK(platform)->unk80;
        }
        return;
    }
    if ((platform->object_status & 4) && movement_obj[0].mv->unk57 == 1) {
        func_1518B6B0(movement_obj[0].xpos, movement_obj[0].ypos, movement_obj[0].zpos, 0xFF, 0);
        if (state == TASK_FLAG_OFF) {
            camera_startWobblePreset(3, D_800BE9E8, NULL, 0xFF, 0);
            fx.flags = 1;
            fx.duration = 60;
            fx.playerMask = 1;
            fx.strength = 8;
            fx.updateKind = -1;
            cont_spawnRumble(&fx, 0, 0xFF, 0);
        }
    }
    if (state == TASK_FLAG_ON) {
        if (lowered != platform->y) {
            platform->y = lowered;
        }
        if (!(platform->object_status & 4) && !(platform->task_flag & TASK_FLAG_LOCKED)) {
            state = TASK_FLAG_GOING_OFF;
        }
    } else if (state == TASK_FLAG_OFF) {
        if (top != platform->y) {
            platform->y = top;
        }
    } else if (state == TASK_FLAG_GOING_ON) {
        WORK(platform)->unk80 += D_800BE9E4;
        if (WORK(platform)->unk80 < 0) {
            platform->t = ((math_rand() % 5) - 2.0f) * 0.125f;
            platform->v = ((math_rand() % 5) - 2.0f) * 0.125f;
        } else {
            if (WORK(platform)->unk80 < 40) {
                platform->t = ((math_rand() % 5) - 2.0f) / 12.0f;
                platform->v = ((math_rand() % 5) - 2.0f) / 12.0f;
            } else {
                platform->v = 0.0f;
                platform->t = 0.0f;
            }
            if (platform->y < lowered) {
                if (kind == 0) {
                    step = ((platform->y - bottom + 0x10) * step) / travel;
                    if (WORK(platform)->unk84 == 0) {
                        particle_spawnPlacementRing(platform, 0xFF, 0);
                        WORK(platform)->unk84 = 1;
                    }
                }
                platform->y -= step;
                WORK(&platform[1])->unk80 = platform->y - lowered;
            } else {
                platform->y -= step;
                WORK(platform)->unk84 = 0;
            }
            if (platform->y <= bottom || step == 0) {
                platform->y = bottom;
                diff = bottom - lowered;
                top += diff;
                WORK(platform)->home += diff;
                WORK(platform)->home |= 0x80000000;
                WORK(platform)->unk80 = 0;
                WORK(&platform[1])->home += diff;
                WORK(&platform[1])->home |= 0x80000000;
                WORK(&platform[1])->unk80 = 0;
                if (kind == 4 && state != TASK_FLAG_ON) {
                    particle_spawnPlacementRing(platform, 0xFF, 0);
                }
                state = TASK_FLAG_ON;
                platform->v = 0.0f;
                platform->t = 0.0f;
                if (kind == 2) {
                    platform->inactive = 1;
                    platform[1].inactive = 1;
                    mp_fireLinkGroup(2, (platform - db_Object_array));
                    mp_fireLinkGroup(2, (&platform[1] - db_Object_array));
                }
            }
        }
    } else if (state == TASK_FLAG_GOING_OFF) {
        state = TASK_FLAG_GOING_OFF; // matching: redundant store steers register allocation
        platform->y += 3;
        if (platform->y >= top) {
            platform->y = top;
            state = TASK_FLAG_OFF;
        }
    }
    platform->task_flag &= ~TASK_FLAG_STATE;
    platform->task_flag |= state;
}

/* blood_spawnHitEffects on the object, then a burst of particles at its x/z, unk180 + 105 high. */
void func_150BFFE0(Actor *obj) {
    Vec3f pos;
    SpriteBurst spawn;

    pos.x = obj->xpos;
    pos.y = obj->floorY + 70.0f;
    pos.z = obj->zpos;
    blood_spawnHitEffects(obj, 0, 0, 1, 0, 0, 0xFF, 1);
    spawn.countBase = 0x11;
    spawn.countRange = 7;
    spawn.texture = 0x6C;
    spawn.renderMode = 0x5103;
    spawn.unk8 = 0x200005;
    spawn.unkC = 0;
    spawn.timerBase = 0x28;
    spawn.timerRange = 0x14;
    spawn.frameRateBase = 0;
    spawn.frameRateRange = 0;
    spawn.a = 0xFF;
    spawn.r = 0x4E;
    spawn.g = 0x54;
    spawn.b = 0x7B;
    spawn.r2 = 0xA4;
    spawn.g2 = 0xA1;
    spawn.b2 = 0xC8;
    spawn.alphaBase = 0x9B;
    spawn.alphaRange = 0x64;
    spawn.brightness = 0xFF;
    spawn.fadeStart = 0x1E;
    spawn.fadeRate = 8;
    spawn.scaleStart = 0x1E;
    spawn.scaleRate = 1.019999981f;
    spawn.sizeBase = 451.0f;
    spawn.sizeRange = 501.0f;
    spawn.pos.x = pos.x;
    spawn.pos.y = pos.y + 35.0f;
    spawn.pos.z = pos.z;
    spawn.yawBase = 0;
    spawn.pitchBase = -0x14;
    spawn.yawRange = 0xFF;
    spawn.pitchRange = 0x1E;
    spawn.speedBase = 15.0f;
    spawn.speedRange = 39.0f;
    spawn.gravityBase = 0.3540000021f;
    spawn.gravityRange = 0.6260000467f;
    spawn.flags = 0x40040E07;
    spawn.updateFunc = 0x10;
    spawn.deathFunc = -1;
    spawn.primColorMode = 8;
    spawn.envColorMode = 6;
    spawn.unk64 = 1;
    spawn.extra = 0.8775510192f;
    particle_spawnSpriteBurst(&spawn, 0xFF, 0xFF, 1);
}

/* Piece handler: two particle bursts 20 units above `y` at the piece's position. */
s32 func_150C01DC(Piece *piece, s32 arg1, s32 arg2, s32 arg3, f32 y) {
    Vec3f pos;
    DebrisBurst burst;

    pos.x = piece->position.x;
    pos.y = y + 20.0f;
    pos.z = piece->position.z;
    {
    const s32 ids[1] = { 5 };
    const f32 scales[1] = { 1.0f };

    burst.countBase = 5;
    burst.countRange = 4;
    burst.pos = pos;
    burst.angles.yawBase = 0;
    burst.angles.yawRange = 0xFF;
    burst.angles.pitchBase = -0x40;
    burst.angles.pitchRange = 0x22;
    burst.speedBase = 12.0f;
    burst.speedRange = 8.0f;
    burst.gravityBase = -1.697000027f;
    burst.gravityRange = 0.4990000129f;
    burst.lifeBase = 0x1E;
    burst.lifeRange = 0xF;
    burst.scaleBase = 0.8000000119f;
    burst.scaleRange = 1.200000048f;
    burst.spin = 15.29900074f;
    particle_spawnDebrisBurst(&burst, ids, scales, 1, 0.0f, 1, piece->unkC, piece->unk1);
    }
    {
    SpriteBurst spawn;

    spawn.countBase = 5;
    spawn.countRange = 8;
    spawn.texture = 0x6C;
    spawn.renderMode = 0x5103;
    spawn.unk8 = 0x200005;
    spawn.unkC = 0;
    spawn.timerBase = 0x1E;
    spawn.timerRange = 0xF;
    spawn.frameRateBase = 0;
    spawn.frameRateRange = 0;
    spawn.a = 0xFF;
    spawn.r = 0x4E;
    spawn.g = 0x54;
    spawn.b = 0x7B;
    spawn.r2 = 0xA4;
    spawn.g2 = 0xA1;
    spawn.b2 = 0xC8;
    spawn.alphaBase = 0x64;
    spawn.alphaRange = 0x9B;
    spawn.brightness = 0xFF;
    spawn.fadeStart = 0xE;
    spawn.fadeRate = 0x12;
    spawn.scaleStart = 0xE;
    spawn.scaleRate = 1.046154022f;
    spawn.sizeBase = 218.0f;
    spawn.sizeRange = 305.0f;
    spawn.pos = pos;
    spawn.yawBase = 0;
    spawn.pitchBase = -0x28;
    spawn.yawRange = 0xFF;
    spawn.pitchRange = 0x28;
    spawn.speedBase = 19.0f;
    spawn.speedRange = 8.0f;
    spawn.gravityBase = 0.2620000243f;
    spawn.gravityRange = 0.2620000243f;
    spawn.flags = 0x840E07;
    spawn.updateFunc = 0x10;
    spawn.deathFunc = -1;
    spawn.primColorMode = 8;
    spawn.envColorMode = 6;
    spawn.unk64 = 1;
    spawn.extra = 0.8977670074f;
    particle_spawnSpriteBurst(&spawn, 0xFF, piece->unkC, piece->unk1);
    }
    return 0;
}

/* Scatters 9-13 pieces of variant `id` from origin; each piece carries a 0.65 burst chance. */
void prop_scatterPieces(Vec3f *origin, void *arg1, s32 id, u8 arg3, u8 arg4, s32 arg5) {
    DebrisScatter config;
    AngleRange angles;
    s32 ids[1];
    const f32 scales[1] = { 1.0f };
    f32 burstChance;
    u8 flags;

    ids[0] = id;
    config.countBase = 9;
    config.countRange = 4;
    burstChance = 0.6500000358f;
    config.origin = *origin;
    config.speedBase = 4.0f;
    config.speedRange = 7.0f;
    config.gravityBase = -1.280000091f;
    config.gravityRange = 0.3360000253f;
    config.scaleBase = 0.6000000238f;
    config.scaleRange = 0.4500000179f;
    config.lifetimeBase = 0x50;
    config.lifetimeRange = 0x3C;
    config.floor = arg1;
    config.variantIds = ids;
    config.variantScales = scales;
    config.variantCount = 1;
    config.spin = 17.07700157f;
    config.floorHitChance = 0.7520000339f;
    config.elasticityBase = 0.2040000111f;
    config.elasticityRange = 0.1640000045f;
    config.radiusScale = 10.0f;
    config.radius = 78.0f;
    if (arg3 != 0) {
        flags = 1;
    } else {
        flags = 0;
    }
    config.flags = flags;
    config.floorHitFn = 0xE;
    config.extraSize = 4;
    config.extraData = &burstChance;
    angles.yawBase = 0;
    angles.yawRange = 0xFF;
    angles.pitchBase = -0x32;
    angles.pitchRange = 0x23;
    particle_spawnDebrisScatter(&config, &angles, arg4, arg5);
}

/* Floor handler: with probability burstChance, bursts into smaller pieces and particles 20 units above
 * the floor; otherwise falls back to particle_floorBounceSettle. */
s32 prop_onPieceFloorHit(Piece *piece, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
    u8 flags;

    if (math_randomFloat() < piece->burstChance) {
        {
            DebrisScatter config;
            AngleRange angles;
            s32 ids[1];
            const f32 scales[1] = { 1.0f };

            ids[0] = piece->unk66;
            config.countBase = 2;
            config.countRange = 0;
            config.origin.x = piece->position.x;
            config.origin.y = floorHeight + 20.0f;
            config.origin.z = piece->position.z;
            config.speedBase = 5.0f;
            config.speedRange = 4.0f;
            config.gravityBase = -1.210000038f;
            config.gravityRange = 0.4720000327f;
            config.lifetimeBase = 0x28;
            config.lifetimeRange = 0x1E;
            config.scaleBase = piece->unk18 * 0.3600000143f;
            config.scaleRange = piece->unk18 * 0.4130000174f;
            config.spin = 31.52700233f;
            config.floor = piece->unk110;
            config.floorHitChance = 1.0f;
            config.elasticityBase = 0; // matching: integer zeros here; 0.0f changes the code
            config.elasticityRange = 0;
            config.radiusScale = 10.0f;
            config.variantIds = ids;
            config.variantScales = scales;
            config.variantCount = 1;
            config.radius = 0;
            if (piece->unk60 & 0x800) {
                flags = 1;
            } else {
                flags = 0;
            }
            config.flags = flags;
            config.floorHitFn = 0xA;
            config.extraSize = 0;
            config.extraData = NULL;
            angles.yawBase = 0;
            angles.yawRange = 0xFF;
            angles.pitchBase = -0x2A;
            angles.pitchRange = 0x19;
            particle_spawnDebrisScatter(&config, &angles, piece->unkC, piece->unk1);
            {
            SpriteBurst spawn;
            f32 spawnY;

            spawnY = floorHeight + 20.0f;
            spawn.countBase = 1;
            spawn.countRange = 2;
            spawn.texture = 0x6C;
            spawn.renderMode = 0x5103;
            spawn.unk8 = 0x200005;
            spawn.unkC = 0;
            spawn.timerBase = 0x1E;
            spawn.timerRange = 0x10;
            spawn.frameRateBase = 0;
            spawn.frameRateRange = 0;
            spawn.a = 0xFF;
            spawn.r = 0x4E;
            spawn.g = 0x54;
            spawn.b = 0x7B;
            spawn.r2 = 0xA4;
            spawn.g2 = 0xA1;
            spawn.b2 = 0xC8;
            spawn.alphaBase = 0xC8;
            spawn.alphaRange = 0x37;
            spawn.brightness = 0xFF;
            spawn.fadeStart = 0x14;
            spawn.fadeRate = 0xC;
            spawn.scaleStart = 0x14;
            spawn.scaleRate = 1.024461985f;
            spawn.sizeBase = 178.0f;
            spawn.sizeRange = 91.0f;
            spawn.pos.x = piece->position.x;
            spawn.pos.y = spawnY;
            spawn.pos.z = piece->position.z;
            spawn.yawBase = 0;
            spawn.pitchBase = -0x12;
            spawn.yawRange = 0xFF;
            spawn.pitchRange = 0xB;
            spawn.speedBase = 4.0f;
            spawn.speedRange = 6.0f;
            spawn.gravityBase = 0.006000000052f;
            spawn.gravityRange = 0.0f;
            spawn.flags = 0x40040E07;
            spawn.updateFunc = 0x10;
            spawn.deathFunc = -1;
            spawn.primColorMode = 8;
            spawn.envColorMode = 6;
            spawn.unk64 = 1;
            spawn.extra = 0.9535999894f;
            particle_spawnSpriteBurst(&spawn, 0xFF, piece->unkC, piece->unk1);
            }
        }
        return 0;
    }
    return particle_floorBounceSettle((ModelParticle *)piece, x, y, z, floorHeight, surface);
}
