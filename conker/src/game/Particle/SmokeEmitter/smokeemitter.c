#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/beam.h"
#include "conker/collision.h"
#include "conker/colourfb.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "conker/trail.h"
#include "libc/string.h"

/* If object idA (else idB) has bit 0 of its gWorldObjectContacts flag word set, calls actor_hitScripted on
 * movement_obj[0] with 0x6E00 (else 0xEE00). Does nothing while movement_obj[0].stunned is set. */
void particle_hitPlayerOnObjectContact(s32 idA, s32 idB) {
    WorldObject *obj;
    s32 index;

    if (movement_obj[0].stunned != 0) {
        return;
    }

    obj = gfx_findObjectById(idA);
    index = obj - db_Object_array;
    if (gWorldObjectContacts[index] & 1) {
        actor_hitScripted(&movement_obj[0], 0x3F, 0x6E00, -1);
        return;
    }

    obj = gfx_findObjectById(idB);
    index = obj - db_Object_array;
    if (gWorldObjectContacts[index] & 1) {
        actor_hitScripted(&movement_obj[0], 0x3F, 0xEE00, -1);
    }
}

/* TriggerZone handler: trail_spawnNearCamera for obj, probing from the zone's top (y + ySize, unk2 + unk8) down to
 * its base (y, unk2). */
void particle_spawnZoneGroundFxNearCamera(void *obj, Range150AF2E0 *range) {
    trail_spawnNearCamera(obj, range->unk2 + range->unk8, range->unk2);
}

/* Emitter update: accumulates 0.1352..0.305 particles per unit of gFrameDelta and spawns one
 * randomly sized particle at the emitter's position for each whole one. */
void particle_updateSmokeEmitter(SmokeEmitter *emitter) {
    SmokeEmitterData *data;

    data = &emitter->data;
    data->spawnAccum += (1352.0f + math_randomFloat() * 1698.0f) * 0.0001f * gFrameDelta;
    if (data->spawnAccum > 1.0f) {
        ParticleDesc spawn;
        WobbleData wobble;
        SpriteParticle *particle;

        wobble.drag = 0.9704700112f;
        spawn.texture = 0x6C;
        spawn.renderMode = 0x5103;
        spawn.setGeometryMode = 0x200005;
        spawn.fadeStart = 70;
        spawn.fadeRate = 3;
        spawn.flags = 0x80DE07;
        spawn.primColorMode = 8;
        spawn.envColorMode = 6;
        spawn.updateFunc = 0x16;
        wobble.phase[0] = 0;
        wobble.phase[1] = 0;
        spawn.clearGeometryMode = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.scaleStart = 70;
        spawn.r = 0xC7;
        spawn.g = 0x78;
        spawn.b = 8;
        spawn.a = 0xFF;
        spawn.r2 = 0x30;
        spawn.g2 = 0xE;
        spawn.b2 = 0;
        spawn.brightness = 0xFF;
        spawn.scaleRate = 1.021028996f;
        spawn.pos = data->pos;
        spawn.offset.x = 0.0f;
        spawn.offset.y = 0.0f;
        spawn.offset.z = 0.0f;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        do {
            wobble.phaseStep[0] = math_random() % 5U + 4;
            wobble.phaseStep[1] = math_random() % 5U + 4;
            wobble.amplitude[0] = math_randomFloat() * 11.0f;
            wobble.amplitude[1] = math_randomFloat() * 11.0f;
            spawn.gravity = math_randomFloat() * 0.172f + 0.09800000489f;
            spawn.flags &= ~0xC0;
            spawn.flags |= ((math_random() & 1) ? 0x40 : 0) | ((math_random() & 1) ? 0x80 : 0);
            spawn.alpha = math_random() % 101U + 100;
            spawn.timer = math_random() % 51U + 80;
            spawn.width = spawn.height = math_randomFloat() * 71.0f + 80.0f;
            particle = particle_newSpriteParticleWithFloor(&spawn, 0, 0, 0x10, emitter->base.header.owner, emitter->base.header.kind);
            if (particle != NULL) {
                memcpy(&particle->data.wobble, &wobble, sizeof(wobble));
            }
            data->spawnAccum -= 1.0f;
        } while (data->spawnAccum > 1.0f);
    }
}

/* Update for particle_updateSmokeEmitter's particles: wobbles the offset and applies drag to the velocity. */
s32 particle_wobbleSmoke(SpriteParticle *particle, Vec3f *prevPos) {
    WobbleData *wobble;

    wobble = &particle->data.wobble;
    particle_wobbleSpriteOffset(&particle->head, wobble->amplitude, wobble->phase, wobble->phaseStep);
    particle_applyDrag(&particle->head.desc.vel, wobble->drag);
    return 1;
}

/* Starts a data-less effect (draw handler 2, no update) that ends after `lifetime` frames. */
void particle_startGreyscaleScreen(s16 lifetime, u8 owner, s32 list) {
    EffectDesc desc;

    desc.flags = 1;
    desc.updateIndex = -1;
    desc.drawIndex = 2;
    desc.lifetime = lifetime;
    desc.messageIndex = 0;
    colourfb_createScreenEffect(&desc, 0, owner, list);
}

void particle_drawGreyscaleScreen(void *effect, u8 *framebuffer) {
    colourfb_toGreyscale(framebuffer, framebuffer + 0x1ECC0);
}

typedef struct GameDC6B0SmokeMotion {
    f32 x, y;
    f32 phase; /* random in [0, 2pi) */
    f32 unkC;
} GameDC6B0SmokeMotion;

/* A sprite created by sprite_newScreenSprite; the puffs carry their motion at 0x70. */
typedef struct {
    u8 pad0[0x70];
    GameDC6B0SmokeMotion motion;
} GameDC6B0SmokeSprite;


/* Counts down the source's two timers and, when each expires, spawns a moving puff sprite or a
 * static spark sprite (lifetime capped at the source's maxLifetime) and restarts it randomly. */
void particle_updateSmokeSource(GameDC6B0SmokeSource *source) {
    GameDC6B0SmokeSprite *sprite;
    ScreenSpriteInfo puff;
    GameDC6B0SmokeMotion motion;
    GameDC6B0SmokeTimers *timers;
    ScreenSpriteInfo spark;

    timers = &source->timers;
    timers->puff -= gFrameTicks;
    if (timers->puff < 0) {
        motion.x = math_randomFloat() * 280.0f + -140.0f;
        motion.y = math_randomFloat() * 50.0f;
        motion.phase = 2.0f * (math_randomFloat() * 3.141592741f);
        motion.unkC = math_randomFloat() * 0.2680000067f;
        puff.x = 0.0f;
        puff.y = 0.0f;
        math_randomFloat(); /* a random value the original draws and discards */
        puff.texture = 0xFF;
        puff.halfWidth = 1.0f;
        puff.halfHeight = 115.0f;
        puff.timer = math_random() % 71U + 20;
        if (source->base.header.state < puff.timer) {
            puff.timer = source->base.header.state;
        }
        puff.flags = 0x21;
        puff.fadeStart = 1;
        puff.fadeRate = 0xFF;
        puff.kind = 0;
        puff.color1.red = 0xFF;
        puff.color1.green = 0xFF;
        puff.color1.blue = 0xFF;
        puff.color1.alpha = math_random() % 71U + 40;
        puff.color2.red = 0xFF;
        puff.color2.green = 0xFF;
        puff.color2.blue = 0xFF;
        puff.color2.alpha = 0xFF;
        puff.colorLevel = 0xFF;
        puff.render.otherModeH = 0;
        puff.render.setGeometryMode = 0x200004;
        puff.render.clearGeometryMode = 0x1F0601;
        puff.render.renderMode = 0x19;
        puff.render.combiner = 0x54;
        puff.render.colorDither = 0x80;
        puff.render.alphaDither = 0x20;
        puff.render.primMode = 0;
        puff.render.envMode = 0xA;
        puff.viewport = 0;
        sprite = sprite_newScreenSprite(&puff, 6, 0, 0, 0x10, source->base.header.owner, source->base.header.kind);
        if (sprite != NULL) {
            memcpy(&sprite->motion, &motion, sizeof(motion));
        }
        timers->puff = math_random() % 41U + 45;
    }
    timers->spark -= gFrameTicks;
    if (timers->spark < 0) {
        spark.x = math_randomFloat() * 280.0f + -140.0f;
        spark.y = math_randomFloat() * 200.0f + -100.0f;
        spark.halfWidth = spark.halfHeight = 2.0f * math_randomFloat() + 1.0f;
        spark.texture = particle_pickKind0Texture();
        spark.timer = math_random() % 7U + 5;
        if (source->base.header.state < spark.timer) {
            spark.timer = source->base.header.state;
        }
        spark.flags = 1 | (math_random() ? 2 : 0) | (math_random() ? 4 : 0) | 0x20;
        spark.fadeStart = 1;
        spark.fadeRate = 0xFF;
        spark.kind = 0;
        spark.color1.red = 0xFF;
        spark.color1.green = 0xFF;
        spark.color1.blue = 0xFF;
        spark.color1.alpha = math_random() % 121U + 80;
        spark.color2.red = 0xFF;
        spark.color2.green = 0xFF;
        spark.color2.blue = 0xFF;
        spark.color2.alpha = 0xFF;
        spark.colorLevel = 0xFF;
        spark.render.otherModeH = 0;
        spark.render.setGeometryMode = 0x200004;
        spark.render.clearGeometryMode = 0x1F0601;
        spark.render.renderMode = 0x19;
        spark.render.combiner = 0x55;
        spark.render.colorDither = 0x80;
        spark.render.alphaDither = 0x20;
        spark.render.primMode = 0;
        spark.render.envMode = 7;
        spark.viewport = 0;
        sprite_newScreenSprite(&spark, 0, 0, 0, 0, source->base.header.owner, source->base.header.kind);
        timers->spark = math_random() % 11U + 3;
    }
}

/* Advances the oscillator at 0x70 (math_wrapRadians wraps the phase) and writes its value to unk10. */
s32 particle_updateSineOscillator(Obj150AFBF4 *obj) {
    TimeOscillator *osc = &obj->osc;

    osc->phase = osc->phase + osc->rate * gFrameDelta;
    osc->phase = math_wrapRadians(osc->phase);
    obj->unk10 = sinf(osc->phase) * osc->amplitude + osc->base;
    return 1;
}

/* Spawns a particle_newSpriteEmitter particle emitter (texture 0xD, kind 4) on `object`'s `bone`. */
void particle_spawnBoneSpriteEmitter(s32 object, s32 bone, s32 timer, s32 radiusXZ, u8 owner, s32 list) {
    particle_newSpriteEmitter(1, 0, 0, 0xD,                 /* x, y, z, texture */
        0, 0, 0, 0, 0, 0, 0, 0,                 /* frame, rgba, env rgb */
        4, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0,        /* kind, velocity x/z high/low, unk32..34 */
        0xFF, 0, 1, 0, 0, 0, 0, 0xAA,           /* unk35..37, velocity randoms, scaleX */
        0xAA, 0xAA, 0xAA, timer, radiusXZ, 0,   /* scaleY, scale randoms, timer, radii */
        object, 0xF0, 0x50, 0x50, 1, 4, 0, 1,   /* object, lifetime, velocityY (+random), flags, interval, chance, burst */
        0, 0, 0, bone, 0, owner, list);          /* speed, startOffset, isAlt, bone, particleLink */
}

typedef struct Node150AFDB0 {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x2F];
    /* 0x3F */ u8 unk3F;
} Node150AFDB0;

/* Releases (effect_destroyObject) every node of object list 0 of type 17 whose unk3F is dc_glbl_CharNum. Each
 * node's successor is saved on the gObjectWalkNext iteration stack first, so releasing it is safe. */
void particle_freeCharSpriteEmitters(void) {
    Node150AFDB0 *node;

    gObjectWalkDepth++;
    for (node = (Node150AFDB0 *)gObjectLists[0][17]; node != NULL; node = gObjectWalkNext[gObjectWalkDepth]) {
        gObjectWalkNext[gObjectWalkDepth] = node->header.next;
        if (node->unk3F == dc_glbl_CharNum) {
            effect_destroyObject(&node->header);
        }
    }
    gObjectWalkDepth--;
}

/* Fires a tracer along the -z axis of bone 3 (bone 2 if `altBone`) of gCurrentActor, from 20 to
 * 150 units out, plus a tracer (beam_spawnBoneTracer) attached to the same bone. */
void particle_fireActorBoneTracer(s32 altBone) {
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 endX;
    f32 endY;
    f32 endZ;
    s32 bone;
    f32 (*mtx)[4];

    if (gCurrentActor->ob_matrix_ptr != NULL) {
        if (altBone == 0) {
            bone = 3;
        } else {
            bone = 2;
        }

        mtx = gCurrentActor->ob_matrix_ptr[bone];
        startX = 0.0f;
        startY = 0.0f;
        startZ = -20.0f;
        math_transformPoint(mtx, startX, startY, startZ, &startX, &startY, &startZ);
        endX = 0.0f;
        endY = 0.0f;
        endZ = -150.0f;
        math_transformPoint(mtx, endX, endY, endZ, &endX, &endY, &endZ);
        /* speed 40, stopDist 0, halfWidth 2, length 120, lifetime 60, texture 35 */
        beam_spawnTracer(0, startX, startY, startZ, endX, endY, endZ, 40.0f, 0.0f, 2.0f, 120.0f, 60, 35,
                      0, 0, 0, NULL, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        /* from (0, 0, -39) to (0, 0, -150) in bone space; on 3 frames, off 255, lifetime 4 */
        beam_spawnBoneTracer(gCurrentActor, gCurrentActor->unique_id, bone, -1, 0.0f, 0.0f, -39.0f, 0.0f, 0.0f, -150.0f,
                      3, 0xFF, 4, 0);
    }
}

/* Runs particle_hitPlayerOnObjectContact on objects 0xE2/0xE1 (after effect_updateRain(0)) when level is 6, else on 0xDF/0xDE. */
void func_150B003C(s32 unused) {
    if (level == LEVEL_THE_WINDMILL) {
        effect_updateRain(0);
        particle_hitPlayerOnObjectContact(0xE2, 0xE1);
        return;
    }
    particle_hitPlayerOnObjectContact(0xDF, 0xDE);
}

/* Arguments the projectile's ribbon trail keeps (trail_spawnRibbon selector 1). */
typedef struct GameDC6B0TrailArgs {
    GameDC6B0Projectile *source;
    s32 unk4;
} GameDC6B0TrailArgs;

/* Fires a projectile from `from` towards `to` at speed 80, with a ribbon trail. Does nothing if
 * the two points coincide. */
void particle_fireTrailProjectile(Vec3f *from, Vec3f *to, u8 owner, s32 list) {
    ParticleSpawn packet;
    Vec3f dir;
    f32 length;
    f32 invLength;
    GameDC6B0Projectile *projectile;
    GameDC6B0TrailArgs trailArgs;
    EffectObj **trail;

    trailArgs.unk4 = 0;
    dir.x = to->x - from->x;
    dir.y = to->y - from->y;
    dir.z = to->z - from->z;
    if (math_vec3Normalize(&dir, &dir, &length, &invLength) != 0) {
        packet.elasticity = packet.radius = 1.0f;
        packet.sizeXZ = packet.sizeY = 0.7f;
        packet.pos = *from;
        packet.vel.x = dir.x * 80.0f;
        packet.vel.y = dir.y * 80.0f;
        packet.vel.z = dir.z * 80.0f;
        math_getHeadingElevation(&packet.vel, &packet.rot.y, &packet.rot.x);
        packet.scale.z = packet.scale.y = packet.scale.x = 1.0f;
        packet.rot.z = 0.0f;
        packet.rotVel = gZeroVec3f;
        packet.flags = 0x19A0;
        packet.lifetime = 300;
        packet.model = 210;
        packet.variant = 7;
        packet.zone = 0;
        packet.alpha = 0xFF;
        packet.updateFn = 8;
        packet.update2Fn = 0;
        packet.floorHitFn = 0;
        packet.floorType3Fn = 0;
        packet.wallHitFn = 0;
        packet.destroyFn = 0;
        packet.squashFn = 0;
        packet.matrixFn = 2;
        packet.lightMode = 0;
        packet.owner = 0;
        packet.ownerId = 0;
        packet.fadeStart = 1;
        packet.fadeRate = 0xFF;
        packet.sound = 0;
        packet.gravity = 0.0f;
        projectile = particle_newModelParticleWithFloor(&packet, 3, 0xFF, 0, 4, owner, list);
        if (projectile != NULL) {
            trail = &projectile->trail;
            memcpy(trail, &trailArgs.unk4, sizeof(trailArgs.unk4));
            trailArgs.source = projectile;
            *trail = trail_spawnRibbon(&trailArgs, 1, sizeof(trailArgs), owner, list);
        }
    }
}

/* Releases the projectile's ribbon trail, if any. */
void particle_freeProjectileTrail(GameDC6B0Projectile *projectile) {
    EffectObj **trail = &projectile->trail;

    if (*trail != NULL) {
        effect_destroyObject(*trail);
    }
}

/* Releases the trail, then particle_destroyModelParticleDefault. */
void particle_destroyTrailProjectile(GameDC6B0Projectile *projectile) {
    particle_freeProjectileTrail(projectile);
    particle_destroyModelParticleDefault(&projectile->head);
}

/* Releases the trail, then particle_freeModelParticleDefault. */
void particle_freeTrailProjectile(GameDC6B0Projectile *projectile) {
    particle_freeProjectileTrail(projectile);
    particle_freeModelParticleDefault(&projectile->head);
}

/* The debris variants of particle_spawnObjDebrisBurst's burst and their scales. */
static const s32 gObjDebrisVariants[] = { 6, 6, 6, 82, 82 };
static const f32 gObjDebrisScales[] = { 0.2f, 0.2f, 0.2f, 0.2f, 0.2f };

/* Spawns a particle burst and a burst of debris (5 variants) 30 units above obj. */
void particle_spawnObjDebrisBurst(Actor *obj, u8 owner, s32 list) {
    Vec3f pos;

    pos.x = obj->xpos;
    pos.y = obj->ypos + 30.0f;
    pos.z = obj->zpos;
    {
        SpriteBurst burst;

        burst.countBase = 10;
        burst.countRange = 7;
        burst.texture = 0x6C;
        burst.renderMode = 0x5103;
        burst.setGeometryMode = 0x200005;
        burst.clearGeometryMode = 0;
        burst.timerBase = 30;
        burst.timerRange = 15;
        burst.frameRateBase = 0;
        burst.frameRateRange = 0;
        burst.a = 0xFF;
        burst.r = 0x7F;
        burst.g = 0x4B;
        burst.b = 6;
        burst.r2 = 0x80;
        burst.g2 = 0x53;
        burst.b2 = 0;
        burst.alphaBase = 100;
        burst.alphaRange = 100;
        burst.brightness = 0xFF;
        burst.fadeStart = 32;
        burst.fadeRate = 7;
        burst.scaleStart = 32;
        burst.scaleRate = 1.025728941f;
        burst.sizeBase = 351.0f;
        burst.sizeRange = 200.0f;
        burst.pos = pos;
        burst.yawBase = 0;
        burst.pitchBase = -25;
        burst.yawRange = 0xFF;
        burst.pitchRange = 21;
        burst.speedBase = 5.0f;
        burst.speedRange = 25.0f;
        burst.gravityBase = 0.008f;
        burst.gravityRange = 0.266f;
        burst.flags = 0x40E07;
        burst.updateFunc = 16;
        burst.deathFunc = -1;
        burst.primColorMode = 8;
        burst.envColorMode = 6;
        burst.unk64 = 1;
        burst.extra = 0.938608f;
        particle_spawnSpriteBurst(&burst, 0xFF, owner, list);
    }
    {
        DebrisBurst debris;

        debris.countBase = 36;
        debris.countRange = 10;
        debris.pos = pos;
        debris.angles.yawBase = 0;
        debris.angles.yawRange = 0xFF;
        debris.angles.pitchBase = -40;
        debris.angles.pitchRange = 20;
        debris.speedBase = 11.0f;
        debris.speedRange = 8.0f;
        debris.gravityBase = -1.09f;
        debris.gravityRange = 0.3710000217f;
        debris.lifeBase = 34;
        debris.lifeRange = 15;
        debris.scaleBase = 0.302000016f;
        debris.scaleRange = 0.3180000186f;
        debris.spin = 9.687f;
        particle_spawnDebrisBurst(&debris, gObjDebrisVariants, gObjDebrisScales, 5, 65.0f, 0, owner, list);
    }
}

/* Fills out a LightSwing on object `id`: the object, offset -150, scale 4.5 and the object's position as the pivot.
 * Returns 0 if there is no such object. */
s32 particle_initLightSwingOnObject(id, out)
    u8 id;
    struct Obj150B060C *out;
{
    out->object = gfx_findObjectById(id);
    if (out->object == NULL) {
        return 0;
    }
    out->unk0 = -150.0f;
    out->unk4 = 4.5f;
    out->x = out->object->x;
    out->y = out->object->y;
    out->z = out->object->z;
    return 1;
}
