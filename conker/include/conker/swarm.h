#ifndef CONKER_SWARM_H
#define CONKER_SWARM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "swarm": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Swarm/SwarmReset/swarmreset.c
 *   game/Swarm/SwarmActor/swarmactor.c
 *   game/Swarm/SwarmCentre/swarmcentre.c
 *   game/Swarm/ModelSlots/modelslots.c
 */

/* Types that these files define; the declarations below use them. */
typedef struct SwarmActor SwarmActor;
typedef struct Swarm Swarm;
typedef struct SwarmMember SwarmMember;

/* A flying swarm member (bat/bird; object types 10, 0x13 and those swarm_spawnRing is given): spawned by
 * swarm_spawnAwayFromPlayer / swarm_spawnScattered / swarm_spawnRing, updated by swarm_updateFlight and friends, drawn by swarm_drawActor / swarm_drawFlying. The flamethrower jet
 * (flamethrower_burnSwarm) sets the ones of types 0x15-0x17 that it touches on fire. */
struct SwarmActor {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Mtx mtx[2];           /* double-buffered by gDrawBufferIndex */
    /* 0x90 */ u8 group;             /* index into gSwarmGroupsInUse / gSwarmGroupSoundTimers */
    /* 0x91 */ u8 state;
    /* 0x92 */ s16 flags;
    /* 0x94 */ Vtx *vertices;        /* bound to segment 1 when drawn */
    /* 0x98 */ f32 x, y, z;
    /* 0xA4 */ s16 speed;
    /* 0xA6 */ s16 speedStep;
    /* 0xA8 */ f32 pitch, yaw, roll;
    /* 0xB4 */ f32 pitchStep, yawStep, rollStep;
    /* 0xC0 */ f32 scale;
    /* 0xC4 */ u8 padC4[4];
    /* 0xC8 */ f32 groundHeight;
    /* 0xCC */ f32 perchPitchStep;   /* head-bob pitch rate while perched (swarm_updatePercher) */
    /* 0xD0 */ s16 originX, originY, originZ;
    /* 0xD6 */ s16 maxRadius, minRadius, heightRange;
    /* 0xDC */ s16 turnTimer, hoverTimer;
    /* 0xE0 */ f32 targetYaw;
    /* 0xE4 */ f32 targetY;
    /* 0xE8 */ u32 surfaceFlags;
    /* 0xEC */ u8 brightness;
    /* 0xED */ u8 modelIndex;        /* index into gEffectModelSlots / gSwarmFlights */
    /* 0xEE */ u16 sound;
    /* 0xF0 */ u8 alpha;
    /* 0xF1 */ u8 fadingOut;
};

/*
 * Per-model motion parameters of the effect model slots, indexed like gEffectModelSlots (gSwarmFlights). Every slot
 * uses the flight limits (swarm_updateFlight); the words from 0x18 on mean different things per slot.
 */
typedef struct SwarmFlight {
    /* 0x00 */ u8 maxSpeed;
    /* 0x01 */ u8 minSpeed;
    /* 0x02 */ u8 wanderTime;
    /* 0x03 */ u8 turnTime;
    /* 0x04 */ f32 maxRoll;
    /* 0x08 */ f32 maxPitch;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 maxYawStep;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ union {
        /* slots 0..6 (and gSwarmAltWobbles): wings wobbled between minAngle and maxAngle by effect_swingVertices */
        struct {
            /* 0x18 */ f32 angle;
            /* 0x1C */ f32 unk1C;
            /* 0x20 */ f32 unk20;
            /* 0x24 */ f32 velocity;
            /* 0x28 */ f32 maxAngle;
            /* 0x2C */ f32 minAngle;
            /* 0x30 */ f32 radius;
            /* 0x34 */ f32 unk34;
        } wobble;
        /* slots 7 and 10: wings flapped by swarm_flapWings */
        struct {
            /* 0x18 */ f32 angle;     /* amplitude * sin(phase) */
            /* 0x1C */ f32 rate;
            /* 0x20 */ f32 threshold;
            /* 0x24 */ f32 phase;
            /* 0x28 */ f32 amplitude;
            /* 0x2C */ f32 unk2C;
            /* 0x30 */ f32 innerWingLength;
            /* 0x34 */ f32 outerWingLength;
        } flap;
        /* slots 8 and 9: both vertex sets bobbed by swarm_bobWaveVertices */
        struct {
            /* 0x18 */ f32 rate[2];
            /* 0x20 */ f32 angle[2];
            /* 0x28 */ f32 amplitude;
            /* 0x2C */ f32 unk2C;
            /* 0x30 */ f32 unk30;
            /* 0x34 */ f32 unk34;
        } bob;
    } u;
} SwarmFlight; /* size 0x38 */

/* game/Swarm/SwarmReset/swarmreset.c */
extern const ParticleTexInfo gParticleTexInfos[69];
void swarm_resetModelSlots(void);

/* game/Swarm/SwarmActor/swarmactor.c */
void swarm_spawnAwayFromPlayer(s32 count, s32 group, s32 x, s32 y, s32 z, s32 player, f32 minScale, f32 scaleRange, s32 flags);
void swarm_spawnScattered(s32 count, s32 group, s32 x, s32 y, s32 z, s32 spread, s32 player, s32 airborne, f32 minScale,
                   f32 scaleRange);
void swarm_spawnRing(s32 count, s32 group, s32 x, s32 y, s32 z, s32 maxRadius, s32 player, s32 heightRange, s32 minRadius,
                   s32 flags, s32 baseType, u32 typeCount, s32 baseModel, s32 modelMask, s32 state, f32 minScale,
                   f32 scaleRange, s32 spawnFlags);
void swarm_updateFlight(SwarmActor *actor);
void swarm_updatePercher(SwarmActor *actor);
void swarm_updateHoverer(SwarmActor *actor);
Gfx *swarm_drawFlying(Gfx *gdl, SwarmActor *actor, s16 camera);
Gfx *swarm_drawActor(Gfx *gdl, SwarmActor *actor, s16 camera);
void swarm_flapWings(u8 index);

/* game/Swarm/SwarmCentre/swarmcentre.c */
void swarm_createSwarm(u8 memberCount, f32 *targetPos, u8 tag, s32 x, s32 y, s32 z, s32 unused, s32 life, u8 owner, s32 playSfx,
                   s32 size);
void swarm_updateMember(SwarmMember *member);
Gfx *swarm_drawMember(Gfx *gfx, SwarmMember *member, s16 view);
void swarm_doNothing(void);
void swarm_updateSwarm(Swarm *swarm, s32 x, s32 y, s32 z);
Swarm *swarm_findByTag(u8 tag);
void swarm_setTarget(u8 tag, f32 *targetPos, s16 yOffset);
void swarm_setTargetPoint(u8 tag, s32 x, s32 y, s32 z, s16 yOffset);
s32 swarm_getDistanceToActor(u8 tag, s32 index);
void swarm_freeSwarm(Swarm *swarm);
void swarm_destroyByTag(u8 tag);

/* game/Swarm/ModelSlots/modelslots.c */
extern EffectModelSlot gEffectModelSlots[11]; /* one loaded model per effect model slot */
extern u8 gSwarmGroupsInUse[4];               /* per swarm group: in use */
extern u8 gSwarmGroupSoundTimers[4];               /* per swarm group: countdown */
extern s8 gEffectModelSlotStates[11];              /* per slot: use state (1 = used this frame, -2 = unused) */
extern Gfx *gEffectModelSecondDls[11];            /* per slot: the model's second display list */
extern SwarmFlight gSwarmFlights[11];
s32 swarm_spawnEffect(s32 x, s32 y, s32 z, s32 count, s32 type, s32 radius, s32 player, s32 maxRadius, s32 heightRange,
                  s32 minRadius, s32 spawnFlags);
void swarm_loadModelWithAltVerts(s32 slot);
void swarm_loadModelPair(s32 slot);
void swarm_loadModelWithExtra(s32 slot, s32 extraSlot);
void swarm_loadModel(s32 slot);
Gfx *swarm_callModelSecondDl(Gfx *gdl, s32 type, s32 list, s32 view);
void swarm_freeModel(s32 slot);
void swarm_updateModelSlots(void);
s32 func_15184118(Actor *obj);

#endif
