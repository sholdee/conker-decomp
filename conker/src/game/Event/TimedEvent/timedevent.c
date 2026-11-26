#include <ultra64.h>

#include "structs.h"

/* This file's .bss (0x800D1580-0x800D188F), defined in address order before the headers declare it
   (see game/Actor/Movement/movement.c). */
s32 gEventParam; // parameter of the animation event being run (gEventParamF: the same bits as an f32)
ActorAnim *gModelAnimTables[187]; // per model id: animation table, after its AnimHeader (see model_loadAnimTable)
static f32 gEventParamF;
static f32 gEventResult;
static s32 gDeferredSound;
static s32 D_800D1880;

#include "functions.h"
#include "variables.h"

/* Defined in game/Particle/StyleParticle/styleparticle.c. Not in particle.h: game/Effects/ModelBurst/modelburst.c passes an s32 style. */
void particle_spawnQuadEdgeStyleParticles(f32 (*verts)[3], f32 x, f32 z, u8 style, f32 scale, s32 type, s32 countA, s32 countB, u8 arg8,
                   s32 arg9);


/* matching: four scalars, not s32[4] (an array gets an 8-aligned, larger frame) */
struct SoundChoices {
    s32 sound0;
    s32 sound1;
    s32 sound2;
    s32 sound3;
};

/* Zero-terminated list of the event ids that run when event_runTimedEvents is filtered. */
const u8 gFilteredEventIds[12] = { 0x69, 0x6A, 0x9D, 0x94, 0x7E, 0x7F, 0x7A, 0x29, 0 };

/* Turn-angle thresholds and animations per row (also read by game/Actor/ActorMotion/actormotion.c and game/Script/ObjScript/objscript.c). */
const u8 gTurnAnimTable[12][10] = {
    { 0x10, 0x00, 0x30, 0x04, 0x05, 0x02, 0x03, 0xFF, 0x02, 0x03 },
    { 0x20, 0x03, 0xFF, 0x06, 0x05, 0x06, 0x05, 0xFF, 0x06, 0x05 },
    { 0x20, 0x00, 0x40, 0x0A, 0x0B, 0x04, 0x03, 0x70, 0x08, 0x09 },
    { 0x0E, 0x00, 0x20, 0x04, 0x03, 0x02, 0x01, 0xFF, 0x02, 0x01 },
    { 0x20, 0xFF, 0x40, 0x05, 0x04, 0x05, 0x04, 0xFF, 0x05, 0x04 },
    { 0x15, 0x00, 0x30, 0x0C, 0x0D, 0x0C, 0x0D, 0xFF, 0x0C, 0x0D },
    { 0x28, 0xFF, 0x68, 0x05, 0x04, 0x0D, 0x10, 0xFF, 0x0D, 0x10 },
    { 0x40, 0x00, 0xFF, 0x05, 0x04, 0x05, 0x04, 0xFF, 0x05, 0x04 },
    { 0x10, 0x00, 0x15, 0x0B, 0x0C, 0x10, 0x0F, 0xFF, 0x10, 0x0F },
    { 0x16, 0x00, 0xFF, 0x05, 0x04, 0x05, 0x04, 0xFF, 0x05, 0x04 },
    { 0x20, 0x00, 0xFF, 0x09, 0x08, 0x09, 0x08, 0x50, 0x12, 0x12 },
    { 0x60, 0xFF, 0x60, 0x0D, 0x10, 0x0D, 0x10, 0xFF, 0x0D, 0x10 },
};

/* pissed_walk animations, one picked at random (gPissedWalkFirstAnims for the first pick). */
const u8 gPissedWalkAnims[8] = { 0xA6, 0xA7, 0xA6, 0xA6, 0xA6, 0xA7, 0xA6, 0xA8 };
const u8 gPissedWalkFirstAnims[8] = { 0xAC, 0xAD, 0xAC, 0xAC, 0xAC, 0xAD, 0xAC, 0xA9 };

/* Per projectile kind: hit value (also read by game/Collision/RayCast/raycast.c) and spawn offset x, y, z + bone. */
const u8 gProjectileHitValues[20] = {
    0x00, 0x00, 0x10, 0x10, 0x00, 0x13, 0x10, 0x12, 0x23, 0x24,
    0x25, 0x10, 0x10, 0x10, 0x10, 0x3C, 0x1A, 0x10, 0x10, 0x00,
};
const s16 gProjectileSpawnOffsets[19][4] = {
    { -2, -14, 60, 4 },  { -2, -14, 60, 4 },  { -1, -22, 77, 9 },  { 0, 0, 0, 2 },
    { 14, -28, 37, 9 },  { -4, 23, 201, 3 },  { 0, 0, 0, 2 },      { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },      { -4, 23, 201, 3 },  { -4, 23, 201, 3 },  { -1, -20, 12, 14 },
    { -1, -20, 12, 14 }, { 60, 0, 0, 23 },    { -40, 0, 0, 23 },   { 48, 148, 74, 0 },
    { 0, 0, 0, 0 },      { 0, 0, 0, 9 },      { -1, -22, 77, 9 },
};

/* Effect offsets (the words are f32 bit patterns). */
const Vec3f D_80099B70 = { 0.0f, 12.0f, 120.0f };
const Vec3f D_80099B7C = { 0.0f, 27.0f, 215.0f };
const Vec3f D_80099B88 = { 0.0f, 0.0f, 544.0f };
const Vec3f D_80099B94 = { 0.0f, 0.0f, 700.0f };
const Vec3f D_80099BA0 = { 0.0f, 0.0f, 278.0f };
const Vec3f D_80099BAC = { 0.0f, 0.0f, 556.0f };

/* Locals of the effect spawners func_15070D24 / func_15070F60, in stack order. */
typedef struct EffectLocals {
    Vec3i header2;
    LightEffectHeader header;
    u8 pad16[2];
    Vec3f worldPos;
    ParticleSize randoms;
    Vec3f vec78;
    Vec3f localPos;
} EffectLocals;

/* A floor probe result together with a point (func_15071A64, func_15074C00). */
typedef struct {
    /* 0x00 */ CollisionHit ground;
    /* 0x24 */ Vec3f pos;
} GroundPoint; // size 0x30

/* One entry of an object's timed event list. */
typedef struct TimedEvent {
    f32 time;
    u8 pad4[3];
    u8 id;
    s32 parameter;
} TimedEvent;

typedef void EventHandler(void);

/* Animation-command handlers, indexed by the command (see event_runAnimCommand). */
void (*gAnimCommandProcs[])(s32) = {
    func_1506EFC8, func_1506F004, NULL, func_1506F02C,
    func_1506F05C, NULL, func_1506F0C4, func_1506F0F0,
    event_sprayTrails, func_1506F1A8, event_stopFlamethrowerJets, func_1506F8C0,
    func_1506F54C, event_stopFlames, NULL, func_1506F8F0,
    func_1506F9C0, func_1506FA90, NULL, func_1506FB60,
    func_1506FB60, NULL, NULL, func_1506FBE8,
    event_cancelTrailPair, func_1506FC1C, NULL, func_1506F11C,
    func_1506F14C, func_1506FC9C, NULL, event_stopFlamethrowerJets,
    func_1506FD30, func_1506FCFC, event_queueTrailPair, func_1506FDF0,
    func_1506FE1C, func_1506FE48, func_1506FE74, func_1506FEA0,
    func_1506FECC, func_1506FEF8, func_1506FF24, func_1506FF50,
    func_1506FF78, func_1506FFAC, func_1506FFE0, func_15070014,
    func_1507003C, func_150700B4, func_15070084, func_1506FCC8,
    event_stopFlamethrowerJets, func_150700E4, func_15070114, func_15070144,
    func_150701C4, NULL, NULL, func_150701F4,
    func_15070224, func_15070300, func_15070690, func_15070690,
    func_150706C4, func_150706C4, func_150706F8, event_fireMuzzle0,
    event_fireMuzzle1, event_fireLauncher, func_150707F8, func_15070830,
    func_15070860, func_15070898, func_15070CDC, func_15070D00,
    func_15070D24, func_15071230, func_15071254, NULL,
    NULL, func_15071278, func_150712AC, func_15071360,
    func_15071434, event_spawnStripEffect, event_spawnStripEffect, event_notifyFollowersGone,
    func_15071544, func_1507158C, func_150715D4, func_1507161C,
    func_15071470, func_150714AC, func_150714E8, func_15071690,
    func_150716EC, func_15071764, func_150717E0, func_15071830,
    func_15071860, func_15071888, func_150718E4, func_15071998,
    func_150719CC, func_15071A00, func_15071A34, func_15071A64,
    func_1506F090, func_15071B18, func_15071D08,
};
/* Per-frame state handlers, indexed by the state number of an animation entry (see game/Actor/ActorMotion/actormotion.c). */
void (*gAnimStateProcs[])(void) = {
    func_1506D950, anim_updateIdle, func_1506AF74, func_1506AFE0,
    func_1506B020, func_1506B070, func_1506B078, func_1506D950,
    event_playNextAnim, func_1506B198, func_1506B1E8, func_1506B228,
    func_1506B268, func_1506B328, func_1506B2BC, func_1506B368,
    func_15072AF8, func_1506B370, NULL, func_1506B5A4,
    func_1506B5AC, func_1506B5B4, func_1506B5CC, NULL,
    func_1506B5E4, func_1506D950, func_1506D950, func_1506D950,
    NULL, func_1506B3B0, NULL, event_pickTurnAnim,
    func_1506B50C, func_1506B520, func_15071ED4, func_15071F14,
    func_1506B740, func_1506B82C, func_1506B7F4, func_15072918,
    func_15072968, func_150729D0, event_playFollowUpAnim, func_15072A14,
    func_1506B860, func_1506B88C, func_1506B8B4, func_1506B91C,
    func_1506B984, func_15073C28, func_15073C48, func_1506B944,
    func_1506B8F4, func_15073F54, func_1506B9AC, func_1506B9C0,
    func_15072A40, func_15072A7C, func_15072940, func_1506D898,
};
/* Timed event handlers, indexed by the event id (see event_runTimedEvents). */
static EventHandler *gTimedEventProcs[] = {
    func_1506D950, event_setYSpeed, event_jump, event_setYGravity,
    event_clearNoRun, event_setWantSpeed, event_setNoRun, func_1506BEDC,
    func_1506BF1C, event_playSoundCommand, event_playDeferredSound, event_fireProjectile,
    func_1506D584, event_resetBlink, func_1506EF5C, func_1506D950,
    func_1506D4EC, event_stopHeadEffect, func_1506D4F4, func_1506D538,
    func_15074870, func_1506D950, func_1506D74C, event_setYGravityTenths,
    func_1507490C, event_dropHeldObject, func_1506D934, func_1506D950,
    effect_rearmPlayerMarker, func_1506D958, func_150748F4, event_setImmune,
    func_1506DA94, event_startChasing, func_1506DB30, event_setBreakStun,
    event_setSpeed, event_setTaskInt2, event_setIntelHitRoute, func_1506DBA0,
    func_1506DC10, func_1506DBD4, func_15073D34, event_setTaskInt1,
    event_setEnergy, func_1506DCF8, func_15074A6C, event_scaleByPlayerDistance,
    event_addNoShove, func_1506DD6C, func_1506DDB8, func_1506DDC0,
    func_1506DE04, func_1506E5FC, event_throwHeldObject, func_1506EA18,
    func_1506E73C, func_15074B7C, NULL, NULL,
    func_1506EC50, func_1506ECD0, NULL, func_1506ECF0,
    NULL, NULL, func_1506ED0C, NULL,
    func_1506ED4C, func_1506ED68, func_1506ED90, func_1506B4EC,
    event_setMvmtControl, event_addTaskAttrib, event_clearTaskAttrib, event_clearMvmtControl,
    func_1506E7FC, func_1506E77C, func_1506DE84, event_setAfterimage,
    func_1506E63C, func_1506EED8, func_1506E8D8, func_1506E918,
    func_1506EA58, event_addMvmtControl, event_setIntelAccel, event_setIntelDecel,
    func_15071E58, func_15071F80, event_runAnimCommand, event_startHeadEffect,
    func_15071F54, func_15071FB0, event_launchPlayer, func_150722F0,
    func_1507266C, func_15071DC8, func_15071DF4, func_150727AC,
    func_150727F0, event_runChildAnimCommand, func_1506D6B4, event_setTaskInt0,
    func_15072DA0, func_15072DD8, func_15072E38, func_1506C418,
    func_1506C43C, func_1506E848, func_1506E6BC, func_1506E7BC,
    func_150721A4, func_15072E7C, func_15072E98, func_15072EC0,
    event_setStunned, func_15072F10, func_1506BDE8, event_spawnObjectAhead,
    func_1507304C, func_15073054, event_respawnObject, func_1507233C,
    func_15072360, func_15072388, func_150723AC, func_150723E0,
    func_15073078, func_150730A4, event_fireVolley, func_150730D0,
    event_releaseHeldObject, func_150739A4, func_150739C0, event_setStickSpeed,
    func_15073A50, func_1506EEAC, event_setRopeSwing, func_15073C50,
    func_15072740, func_1506E998, func_15073CB8, func_1506E9D8,
    func_1506E6FC, func_1506E67C, func_1506E958, func_15073CF4,
    func_15073D74, func_15073DA4, func_1506B82C, func_15073F5C,
    event_clearNoShove, func_15073F78, func_1506E898, event_spawnThrownItem,
    event_dealDamage, event_setWeaponStatus, func_15074664, event_setOwnerState,
    event_addShotsFired, func_1507488C, func_15073E2C, func_15073EA4,
    func_15073F1C,
};
s32 D_80086690 = 0x0FA10CDF; /* not referenced */
/* Hit-reaction handlers, indexed by AttackReaction.unk8 (see game/Actor/ActorMotion/actormotion.c). Declared without a
   prototype: most take (Actor *self, Actor *other, u8 *result), a few take other arguments. */
void (*gHitReactionProcs[])() = {
    func_15074BD8, func_15074BEC, func_15194320, func_15074C00,
    func_15074C80, func_15194394, func_15074DEC, func_15074E04,
    func_15194588, func_15074E80, func_151945CC, func_15194810,
    func_15074EE8, func_15074F30, func_15074F48, func_15074FD4,
    effect_hitReactNone, func_15075050, effect_hitReactKill, effect_hitReactSetOnFireDamaging,
    func_15194A68, effect_hitReactImpactDebris, func_15194B94, effect_hitReactFlashAndKill,
    func_15194F48, effect_hitReactLightBurst, effect_hitReactSetOnFire, effect_hitReactActorImpact,
    effect_hitReactSplash, func_150750A4, func_150750C4, func_150FFD2C,
    effect_hitReactJointImpact, effect_hitReactSmallSplash, func_15195698, func_15194DC8,
    effect_hitReactRumble,
};

/*
 * Runs every event of the object's event list whose time is <= `time`. Each event stores its
 * parameter in gEventParam (and as a float in gEventParamF) and calls handler gTimedEventProcs[id].
 * With `filtered` set, only ids in the zero-terminated list gFilteredEventIds run. A handler that
 * restarts the object's state (new token) rewinds `time` to the state's time. Returns
 * gEventResult, which handlers may set.
 */
f32 event_runTimedEvents(Actor *obj, f32 time, u8 filtered) {
    s32 token;
    u8 id;

    if (obj->drawtype == 4) {
        return 0.0f;
    }
    gEventResult = 0.0f;
    if (time < obj->lastEventTime) {
        obj->speccnt = 0;
    }
    D_800D1880 = 1;
    while (obj->events != NULL && obj->events[obj->speccnt].time <= time && D_800D1880 != 0) {
        if (obj->events[obj->speccnt].time < 900.0f) {
            gEventParam = obj->events[obj->speccnt].parameter;
            gEventParamF = *(f32 *)&gEventParam;
            token = obj->unk2D0->frameRom[0];
            if (gCutsceneStates[0] == 0 || D_800C365E[0] != 0) {
                id = obj->events[obj->speccnt].id;
                if (filtered) {
                    s32 i;
                    for (i = 0; filtered; i++) {
                        if (gFilteredEventIds[i] == 0) {
                            id = 0;
                        }
                        if (id == gFilteredEventIds[i]) {
                            break;
                        }
                    }
                }
                if (id != 0) {
                    gTimedEventProcs[id]();
                }
            }
            if (token != obj->unk2D0->frameRom[0]) {
                time = obj->unk2D0->frame[0];
            } else {
                obj->speccnt++;
            }
        }
    }
    return gEventResult;
}

void func_1506AF74(void) {
    if ((gCurrentActor->unk100 & 8) != 0) {
        gCurrentActor->unk100 |= 4;
    }
    gCurrentActor->unk100 &= 0xF7;
    actor_playAnim(gCurrentActor, 0x38, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506AFE0(void) {
    actor_playAnim(gCurrentActor, 0x48, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506B020(void) {
    gCurrentActor->unk1CB = 1;
    actor_playAnim(gCurrentActor, 0x39, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506B070(void) {
}

void func_1506B078(void) {
    f32 speed = (gCurrentActor->ypos - (gCurrentActor->waterHeight - 150.0f)) * 0.005f;

    if (speed < 0.0f) {
        speed = 1.0f;
    } else {
        speed += 1.0f;
    }
    event_playAnim(209, speed, 4.0f);
}

/* actor_playAnim on the current object with the given id and two float arguments. */
void event_playAnim(s32 id, f32 arg1, f32 arg2) {
    actor_playAnim(gCurrentActor, id, arg1, arg2, 0.0f, 0.0f, 0);
}

void event_playNextAnim(void) {
    actor_playAnim(gCurrentActor, gCurrentActor->animseq.uh + 1, gCurrentActor->animsp, 3.0f, 0.0f, 0.0f, 0);
}

void func_1506B198(void) {
    actor_playAnim(gCurrentActor, gCurrentActor->animseq.uh + 1, gCurrentActor->animsp, 3.0f, 0.0f, 0.0f, 1);
}

void func_1506B1E8(void) {
    actor_playAnim(gCurrentActor, 0x3C, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_1506B228(void) {
    actor_playAnim(gCurrentActor, 0x54, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_1506B268(void) {
    gCurrentActor->count5 = 0;
    gCurrentActor->norun = 0;
    actor_playAnim(gCurrentActor, 0xF, 1.0f, 6.0f, 0.0f, 0.0f, 0);
}

void func_1506B2BC(void) {
    Actor *obj = event_getFirstOwnedObject(gCurrentActor);

    if (obj != NULL && (obj - movement_obj) + 1 == movement_obj[0].stand_on_char) {
        gCurrentActor->scriptCmd = NULL;
        gCurrentActor->intelwalk = 4;
    }
}

void func_1506B328(void) {
    actor_playAnim(gCurrentActor, 0x3E, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506B368(void) {
}

void func_1506B370(void) {
    actor_finishDeath(gCurrentActor - movement_obj);
}

void func_1506B3B0(void) {
}

/* Picks unk244 from the gTurnAnimTable table by the angle between the object's facing and the player. */
void event_pickTurnAnim(void) {
    s32 absDiff32;
    s16 angleDiff;
    u8 sel = 0;
    u8 absDiff;

    angleDiff = (gCurrentActor->langle + (gCurrentActor->unk1FD << 8)) -
                actor_getDirectionAngle(movement_obj[0].xpos - gCurrentActor->xpos,
                              gCurrentActor->zpos - movement_obj[0].zpos, 0);
    absDiff = angleDiff >> 8;
    if (absDiff & 0x80) {
        absDiff = -absDiff;
    }
    absDiff32 = absDiff;
    if (absDiff < gTurnAnimTable[0][sel]) { // matching: all three indexed by sel (still 0); constant indices schedule differently
        sel = 1;
    } else {
        if (gTurnAnimTable[0][sel + 7] < absDiff32) {
            sel = 5;
        } else if (gTurnAnimTable[0][sel + 2] < absDiff32) {
            sel = 2;
        }
        if (angleDiff < 0) {
            sel += 4;
        } else {
            sel += 3;
        }
    }
    gCurrentActor->speccnt = 0;
    gCurrentActor->intel_animseq = gTurnAnimTable[0][sel];
    gCurrentActor->unk21C = 20000;
}

void func_1506B4EC(void) {
    event_pickTurnAnim();
}

void func_1506B50C(void) {
    gCurrentActor->unk21C = 0;
}

void func_1506B520(void) {
    s32 id;

    if (gCurrentActor->energy != 0) {
        if (gCurrentActor->mv != NULL && gCurrentActor->mv->rt_context_b == 0x25) {
            id = 427;
        } else {
            id = 240;
        }
        actor_playAnim(gCurrentActor, id, 1.5f, 3.0f, 0.0f, 0.0f, 0);
    } else {
        actor_kill(gCurrentActor, 1);
    }
}

void func_1506B5A4(void) {
}

void func_1506B5AC(void) {
}

void func_1506B5B4(void) {
    gCurrentActor->unk1CB = 1;
}

void func_1506B5CC(void) {
    gCurrentActor->unk1CB = 1;
}

void func_1506B5E4(void) {
    actor_fadeCommand(gCurrentActor, 2, 7, 8, 0);
    gCurrentActor->animsp = 0.0f;
    gCurrentActor->unk2D0->speed[0] = 0.0f;
}

/* Picks a random entry (0..count-1) of gPissedWalkFirstAnims / gPissedWalkAnims for the object's mv->pissed_walk. */
void event_pickPissedWalk(s32 count) {
    u32 rand;
    u32 pick;
    u8 index;
    ActorCtl *ext;

    rand = math_random();
    pick = rand % (u8)count;
    index = pick;
    ext = gCurrentActor->mv;
    if (ext->pissed_hangover != 0) {
        if (level == 41) {
            if ((u8)pick == 7) {
                index = 0;
            }
        }
        ext->pissed_walk = gPissedWalkFirstAnims[index];
    } else {
        ext->pissed_walk = gPissedWalkAnims[(u8)pick];
    }

    if (gCurrentActor->mv->pissed_walk == 0xA7) {
        gEventParam = 0xFF020144;
        gCurrentActor->mv->pissed_stumble = (math_random() % 3U) + 2;
        func_1506E8D8();
        gCurrentActor->speed = gCurrentActor->speed * 0.2f;
    }
}

void func_1506B740(void) {
    u8 roll = math_random() & 0xFF;

    if (gCurrentActor->waterHeight != -10000.0f && gCurrentActor->ypos < gCurrentActor->waterHeight) {
        gCurrentActor->mv->pissed_walk = 720;
    } else {
        if (gCurrentActor->animseq.uh == 0xA7) {
            if (gCurrentActor->mv->pissed_stumble == 0) {
                roll = 0;
            } else {
                gCurrentActor->mv->pissed_stumble--;
                roll = 0xFF;
            }
        }
        if (roll < 0x80) {
            event_pickPissedWalk(8);
        }
    }
}

void func_1506B7F4(void) {
    gCurrentActor->norun = 0;
    gCurrentActor->count5 = 0;
    event_pickPissedWalk(2);
}

void func_1506B82C(void) {
    gCurrentActor->norun = 0;
    gCurrentActor->count5 = 0;
    anim_updateIdle();
}

void func_1506B860(void) {
    event_playAnim(181, 0.63f, 5.0f);
}

void func_1506B88C(void) {
    event_playAnim(182, 1.0f, 4.0f);
}

void func_1506B8B4(void) {
    gCurrentActor->mv->rt_context_b = 0;
    gCurrentActor->norun = 0;
    gCurrentActor->count5 = 0;
    anim_updateIdle();
}

void func_1506B8F4(void) {
    event_playAnim(222, 0.75f, 4.0f);
}

void func_1506B91C(void) {
    event_playAnim(191, 1.0f, 4.0f);
}

void func_1506B944(void) {
    gCurrentActor->norun = 0;
    gCurrentActor->count5 = 0;
    event_playAnim(197, 1.0f, 4.0f);
}

void func_1506B984(void) {
    event_playAnim(175, 1.0f, 4.0f);
}

void func_1506B9AC(void) {
    gCurrentActor->intel_animseq = 0;
}

void func_1506B9C0(void) {
    ActorCtl *ext;

    gPendingButtons[dc_glbl_CharNum] |= 0x4000;
    ext = gCurrentActor->mv;
    if (ext != NULL) {
        ext->rt_context_b = 0;
        gSeenTextBits[gContextTextId >> 3] |= 1 << (gContextTextId & 7);
    }
    anim_updateIdle();
}

/* Sound handlers: play sound gEventParam (its low half) at the current object (0 stops it). */
void func_1506BA4C(s32 minDistance, s32 maxDistance) {
    if (gEventParam == 0) {
        sound_stopActorSound2(gCurrentActor);
    } else {
        if (gCurrentActor->CameraPtr == 0) {
            sound_playActorSound2(gEventParam, gCurrentActor, 28000, minDistance, maxDistance);
        } else {
            sound_playActorSound2(gEventParam, gCurrentActor, 24000, 500, 2500);
        }
    }
}

void func_1506BAD8(s32 minDistance, s32 maxDistance) {
    if (gEventParam == 0) {
        sound_stopActorSound1(gCurrentActor);
    } else {
        if (gCurrentActor->CameraPtr == 0) {
            sound_playActorSound1(gEventParam, gCurrentActor, 28000, minDistance, maxDistance);
        } else {
            sound_playActorSound1(gEventParam, gCurrentActor, 24000, 500, 2500);
        }
    }
}

void func_1506BB64(s32 minDistance, s32 maxDistance) {
    mp3_startTrackAt(gEventParam, gCurrentActor, 28000, minDistance, maxDistance);
}

void func_1506BBA8(s32 minDistance, s32 maxDistance) {
    if (gCurrentActor->CameraPtr == 0) {
        sound_playActorSound2(gEventParam, gCurrentActor, 14000, minDistance, maxDistance);
    } else {
        sound_playActorSound2(gEventParam, gCurrentActor, 12000, 500, 2500);
    }
}

void func_1506BC24(s32 minDistance, s32 maxDistance) {
    if (gCurrentActor->CameraPtr == 0) {
        sound_playActorSound1(gEventParam, gCurrentActor, 14000, minDistance, maxDistance);
    } else {
        sound_playActorSound1(gEventParam, gCurrentActor, 12000, 500, 2500);
    }
}

void event_setYSpeed(void) {
    gCurrentActor->yspd = gEventParam;
}

/* Jump: sets yspd from gEventParam when the object is (nearly) at height unk180. */
void event_jump(void) {
    Actor *obj;
    f32 height;

    obj = gCurrentActor;
    if (obj->type == 0x16) {
        if (obj->ypos == obj->floorY) {
            gCurrentActor->yspd = (math_rand() % 8) + gEventParam;
            obj = gCurrentActor;
            if (obj->speed != 0.0f) {
                obj->yspd = obj->yspd * (obj->speed / 40.0f);
            } else {
                obj->yspd = 0.0f;
            }
        }
    } else {
        height = obj->ypos - obj->floorY;
        if (-5.0f < height && height < 5.0f) {
            obj->yspd = gEventParam;
        }
    }
}

void func_1506BDE8(void) {
    if (gCurrentActor->ypos < -1750.0f) {
        gCurrentActor->yspd = gEventParam;
    }
}

void event_setYGravity(void) {
    gCurrentActor->ygrav = gEventParam;
}

void event_setYGravityTenths(void) {
    gCurrentActor->ygrav = gEventParam * 0.1f;
}

void event_clearNoRun(void) {
    gCurrentActor->norun = 0;
}

void event_setWantSpeed(void) {
    gCurrentActor->wantspeed = gEventParam;
}

void event_setNoRun(void) {
    gCurrentActor->norun = gEventParam;
}

void func_1506BEDC(void) {
    gCurrentActor->doing_hit = gEventParam;
    gCurrentActor->unk114 = gEventParam >> 8;
}

void func_1506BF1C(void) {
    if (gCurrentActor->waterHeight <= gCurrentActor->ypos) {
        event_playSoundCommand();
    }
}

/*
 * Plays sound command gEventParam at the current object: bits 0-10 the sound id, bits 14/15 play
 * only for / except owner id (top byte), bit 16 defers to event_playDeferredSound, bits 11-13 the mode
 * (0x2000: surface-dependent sound from sound_pickEffectVariant, volume scaled by speed).
 */
void event_playSoundCommand(void) {
    u32 sound;
    u32 owner;
    u16 volume;
    f32 intensity;
    f32 max;
    u32 ownerFilter;
    s32 type;
    u8 id;

    sound = gEventParam & 0x7FF;
    id = gCurrentActor->character_type;
    ownerFilter = (u32)gEventParam >> 24;
    if (gModelDefs[id]->subModelCount != 0) {
        owner = *gSubModelLists[gModelDefs[id]->subModelList];
    } else {
        owner = id;
    }
    gDeferredSound = 0;
    switch (gEventParam & 0xC000) {
    case 0x4000:
        if (owner != ownerFilter) {
            return;
        }
        break;
    case 0x8000:
        if (owner == ownerFilter) {
            return;
        }
        break;
    }
    if (gEventParam & 0x10000) {
        gDeferredSound = gEventParam + 1;
        return;
    }
    volume = 32000;
    switch (gEventParam & 0x3800) {
    case 0x2000:
        if (gScreenLayout >= 2) {
            break;
        }
        type = collision_getSurfaceType(gCurrentActor->floorMaterial);
        if (type == 0xC && gCurrentActor->unkCC > -17.0f) {
            sound = sound_pickVariant(0x9C, 2);
        } else if (gCurrentActor->waterHeight != -10000.0f && gCurrentActor->ypos <= gCurrentActor->waterHeight) {
            sound = sound_pickEffectVariant(8, NULL, sound);
            type = 8;
        } else {
            sound = sound_pickEffectVariant(type, (s16 *)&volume, sound);
        }
        if (sound == 0) {
            break;
        }
        if (gCurrentActor->mv != NULL) {
            intensity = gCurrentActor->mv->actual_speed;
        } else {
            intensity = gCurrentActor->yspd;
        }
        if (type == 0xC) {
            max = 10.0f;
        } else {
            max = 30.0f;
        }
        if (max < intensity) {
            intensity = max;
        }
        if (intensity < 0.0f) {
            intensity = 0.0f;
        }
        intensity *= (f32)0.5 / max; // matching: 0.5f compiles differently
        intensity += 0.5f;
        volume = volume * intensity;
        if (type == 8) {
            sound_playActorUntracked(sound, gCurrentActor, volume, 500, 2500);
            break;
        }
        /* fallthrough */
    case 0:
        sound_playActorSound2(sound, gCurrentActor, volume, 500, 2500);
        break;
    case 0x1000:
        sound_playActorSound1(sound, gCurrentActor, 32000, 500, 2500);
        break;
    case 0x3800:
        if (sound_getPlayingSound(gCurrentActor->soundHandle2) != sound) {
            sound_playActorSound2(sound, gCurrentActor, volume, 500, 2500);
        }
        break;
    case 0x3000:
        sound_playActorUntracked(sound, gCurrentActor, 32000, 500, 2500);
        break;
    }
}

/*
 * Follow-up of event_playSoundCommand's deferred sound (gDeferredSound = parameter + 1): picks one of up to
 * four 11-bit sound ids (the deferred one plus three packed in gEventParam) and plays it.
 */
void event_playDeferredSound(void) {
    s32 cmd;
    struct SoundChoices sounds;
    s32 index;
    s32 sound;

    if (gDeferredSound != 0) {
        gDeferredSound--;
        cmd = gEventParam;
        sounds.sound0 = gDeferredSound & 0x7FF;
        sounds.sound1 = cmd & 0x7FF;
        sounds.sound2 = (cmd >> 11) & 0x7FF;
        sounds.sound3 = (cmd >> 22) & 0x7FF;

        if (sounds.sound3 != 0) {
            index = 4;
        } else if (sounds.sound2 != 0) {
            index = 3;
        } else if (sounds.sound1 != 0) {
            index = 2;
        } else {
            index = 0;
        }

        if (index != 0) {
            index = sound_pickVariant(sounds.sound0, index) - sounds.sound0;
        }

        sound = (&sounds.sound0)[index];
        if (sound != 0) {
            gDeferredSound &= 0xFFFEF800;
            gEventParam = sound | gDeferredSound;
            event_playSoundCommand();
        }
    }
}

void func_1506C418(void) {
    sound_stopActorSound1(gCurrentActor);
}

void func_1506C43C(void) {
    sound_stopActorSound2(gCurrentActor);
}


/*
 * Spawns a projectile object of the given kind (gProjectileSpawnOffsets / gProjectileHitValues rows) from the current
 * object, at `pos` (an f32[3]), at the bone from gProjectileSpawnOffsets, or just above the object. `flags`
 * aims it: 1 = no auto-aim, 2 = face the player, 4 = arc onto the player, 8 = random yaw spread,
 * 0x10 = lead the target unk222, 0x20 = remember unk222. Returns the new object.
 */
u8 *event_spawnProjectile(f32 yaw, f32 speed, f32 yVelocity, f32 gravity, s32 unk83, s32 kind, f32 minHeight,
                  f32 scale, f32 *pos, s32 flags, s32 arg10) {
    Actor *obj;
    u16 aimAngle;
    s16 angle;
    s32 slot;
    s32 anim;
    s32 count;
    s32 i;
    MtxF *mtx;
    f32 x;
    f32 y;
    f32 z;
    MtxF mf;

    count = 0;
    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type == 4) {
            count++;
        }
    }
    if (count >= 5) {
        obj = actor_findByState(4, &slot);
        actor_remove(obj, 0);
    } else {
        obj = actor_alloc();
        if ((u32)(obj - movement_obj) >= 25 && count > 0) {
            obj = actor_findByState(4, &slot);
            actor_remove(obj, 0);
        }
    }
    obj->unk136 = 11;
    obj->type = 4;
    obj->energy = 1;
    obj->langle = gCurrentActor->langle;
    if (pos != 0) {
        obj->xpos = pos[0];
        obj->ypos = pos[1];
        obj->zpos = pos[2];
    } else if (gCurrentActor->ob_matrix_ptr == NULL) {
        obj->xpos = gCurrentActor->xpos;
        obj->ypos = gCurrentActor->ypos + 50.0f;
        obj->zpos = gCurrentActor->zpos;
    } else {
        mtx = gCurrentActor->ob_matrix_ptr;
        if (gBoneMatricesFlushed) {
            guMtxL2F(mf, (Mtx *)&mtx[gProjectileSpawnOffsets[kind][3]]);
            mtx = &mf;
        } else {
            mtx = &mtx[gProjectileSpawnOffsets[kind][3]];
        }
        math_transformPoint(*mtx, gProjectileSpawnOffsets[kind][0], gProjectileSpawnOffsets[kind][1], gProjectileSpawnOffsets[kind][2], &x, &y, &z);
        obj->xpos = x;
        obj->ypos = y;
        obj->zpos = z;
    }
    if (minHeight != 0.0f) {
        if (obj->ypos < gCurrentActor->ypos + minHeight) {
            obj->ypos = gCurrentActor->ypos + minHeight;
        }
    }
    obj->shadow_size = 110;
    obj->unique_id = 0xFF - (obj - movement_obj);
    if (kind == 0 || kind == 1) {
        obj->drawtype = 2;
        obj->unk103 = 1;
    } else {
        obj->drawtype = 5;
        for (i = 0; i <= gScreenLayout; i++) {
            obj->unk304[i] = light_allocGfxLights(1);
        }
        if (obj->unk304[i - 1] != 0) {
            obj->unk301 = 1;
        }
        obj->unk314 = func_1515D440();
        switch (kind) {
        case 2:
            obj->vtilt = 260.0f;
            anim = 0x2A;
            obj->unk103 = 1;
            break;
        case 18:
            obj->vtilt = 260.0f;
            anim = 0x22;
            obj->unk103 = 1;
            break;
        case 11:
            anim = 0x6D;
            break;
        case 12:
            anim = 0x6B;
            break;
        case 3:
            anim = 0x39;
            break;
        case 6:
            anim = 0x51;
            break;
        case 15:
            anim = 0x93;
            break;
        case 17:
            anim = 0x39;
            break;
        default:
            anim = 0x50;
            break;
        }
        collision_createPartSet(obj, anim, 1.0f, 0);
    }
    angle = (s16)(yaw * 182.04445f) - 0x4000;
    if (gIsMultiplayer && !(flags & 1) && kind != 5 && kind != 9 && kind != 10 && kind != 4) {
        u8 targetIndex;
        f32 dist;
        u16 rawAngle;
        f32 targetY;

        aimAngle = rawAngle = actor_getDirectionAngle(speed, yVelocity, 0);
        targetIndex = actor_findTargetInCone(obj, &aimAngle, rawAngle >> 8, 0xFF, 20);
        if (targetIndex != 0xFF) {
            targetY = movement_obj[targetIndex].ypos;
            dist = actor_distanceXZ(gCurrentActor, &movement_obj[targetIndex]);
            angle = aimAngle;
            if (kind == 2 || kind == 18) {
                targetY -= 30.0f;
            }
            yVelocity = (targetY - gCurrentActor->ypos) / (dist / speed);
            gravity = 0.0f;
        }
    }
    if (flags & 2) {
        angle = actor_getDirectionAngle(movement_obj[0].xpos - obj->xpos, obj->zpos - movement_obj[0].zpos, 0);
    }
    if (flags & 4) {
        yVelocity = ((movement_obj[0].ypos + 60.0f) - obj->ypos) / (actor_distanceXZ(obj, movement_obj) / speed);
    }
    if (flags & 8) {
        angle += (u16)(math_random() % 0x1200U) - 0x900;
    }
    if (flags & 0x10) {
        f32 pad1; // matching: three unused stack slots
        f32 pad2;
        f32 pad3;
        f32 tx;
        f32 ty;
        f32 tz;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 offx;
        f32 offz;
        f32 offy;
        Actor *target;
        f32 hdist;
        f32 pitch;
        f32 dummy;

        target = &movement_obj[gCurrentActor->unk222];
        actor_splitVelocity(target->angle, target->speed * 4.0f, 0.0f, &offx, &offz, &offy);
        tx = target->xpos + offx;
        tz = target->zpos + offz;
        ty = (target->ypos + 30.0f) + target->yspd * 4.0f;
        dx = tx - obj->xpos;
        dy = ty - obj->ypos;
        dz = tz - obj->zpos;
        hdist = sqrtf(dx * dx + dz * dz);
        pitch = math_atan2(dy, hdist) * 10430.378f; /* radians to s16 angle units */
        angle = actor_getDirectionAngle(dx, -dz, 0);
        actor_splitVelocity((s16)pitch, speed, 0.0f, &speed, &yVelocity, &dummy);
        yVelocity = -yVelocity;
    }
    obj->langle = angle;
    obj->angle = angle;
    obj->theta = (s16)((u16)angle + 0x4000) * 0.0054931640625f;
    obj->ygrav = gravity;
    obj->yspd = yVelocity;
    obj->speed = speed;
    obj->count5 = *((u8 *)&unk83 + 3); // matching: lbu from the arg slot (a cast emits andi)
    obj->count3 = 0;
    obj->count1 = 0;
    obj->count2 = 0xFF;
    if (flags & 0x20) {
        obj->unk126 = obj->count2 = gCurrentActor->unk222;
    }
    obj->count4 = 0x26;
    if (arg10 == 0) {
        obj->doing_hit = gProjectileHitValues[kind] + 1;
    }
    obj->shad = 100.0f;
    obj->unk114 = 255.0f;
    obj->bodyRadius = 5;
    obj->bodyHeight = 5;
    obj->unkEC = 1.0f;
    obj->unkF0 = 1.0f;
    obj->unkDC = 1.0f;
    obj->unkE0 = 1.0f;
    obj->owner = dc_glbl_CharNum + 1;
    obj->unk122 = 0x11;
    obj->intelwalk = 0;
    obj->imune = obj->intelwalk * 0 + 0xFF; // matching: IDO reuses the zero of the unk232 store
    obj->noshove = 0x100E;
    if (arg10) {
        obj->noshove |= 0x8000;
        bzero(obj->mv = allocate_memory(0x1C0, 1, 0, 0), 0x1C0);
        obj->unk103 = 1;
    }
    obj->scale_xz = scale;
    obj->scale_y = scale;
    obj->unk66 = gCurrentActor->unk66 & 0xC;
    obj->headEffectState = kind;
    obj->unk284 = arg10;
    if (kind == 2 || kind == 18) {
        obj->headEffectTimer = 60;
    }
    switch (kind) {
    case 1:
        actor_runHitHandler(obj, 1, NULL);
        effect_attach(effect_newCharacterWatcher(obj, 1, 0), obj, 0x13);
        break;
    case 15:
        actor_runHitHandler(obj, 13, NULL);
        break;
    case 13:
        sound_playActorSound2(567, obj, 30000, 500, 2000);
        /* fallthrough */
    case 5:
    case 9:
    case 10:
        actor_runHitHandler(obj, 12, NULL);
        break;
    case 0:
        actor_runHitHandler(obj, 8, NULL);
        effect_attach(effect_newCharacterWatcher(obj, 1, 0), obj, 0x13);
        break;
    }
    return (u8 *)obj;
}

/* Fires projectile kind gEventParam from the current object, aimed by its facing and camera pitch. */
void event_fireProjectile(void) {
    s32 bone;
    s32 kind;
    f32 elevation;
    f32 heading;
    f32 dist;
    f32 gravity;
    f32 speed;
    f32 yVelocity;
    f32 scale;
    f32 unused;
    MtxF *mtxs;
    f32 x;
    f32 y;
    f32 z;
    f32 a;
    f32 b;
    f32 pad; // matching: unused stack slot
    s32 allocExt;
    s32 flags;
    f32 *pos;
    f32 vec[3];

    flags = 0;
    gravity = 0.0f;
    elevation = 45.0f;
    dist = 323.0f;
    scale = 0.5f;
    heading = (gCurrentActor->langle + 0x4000) * 0.0054931640625f;
    bone = -1;
    allocExt = 0;
    pos = NULL;
    kind = gEventParam;
    if (kind == 5 || kind == 9 || kind == 10) {
        bone = 3;
    }
    if (bone != -1) {
        if (gCurrentActor->ob_matrix_ptr != NULL) {
            mtxs = gCurrentActor->ob_matrix_ptr;
            math_transformPoint(mtxs[bone], 0.0f, 0.0f, 1.0f, &x, &y, &z);
            x -= mtxs[bone][3][0];
            y -= mtxs[bone][3][1];
            z -= mtxs[bone][3][2];
            a = math_atan2(x, z);
            b = math_atan2(y, sqrtf(x * x + z * z));
            a *= 57.295776f;
            b *= 57.295776f;
            if (a > 180.0f) {
                a -= 360.0f;
            }
            if (b > 180.0f) {
                b -= 360.0f;
            }
            heading = a;
            elevation = b;
        }
    } else if (gCurrentActor->CameraPtr != NULL && gCurrentActor->CameraPtr->Mode == 0x100) {
        elevation = gCurrentActor->CameraPtr->aim.signedPitch;
        if (elevation < 0.0f) {
            elevation = elevation * -0.5f + elevation * -0.2333f + 7.0f;
        } else {
            elevation = elevation * -0.5f + (60.0f - elevation) * 0.133f;
        }
        elevation *= 1.40625f;
    } else {
        elevation = -gCurrentActor->unk1D1;
    }

    switch (kind) {
    case 2:
    case 11:
    case 12:
    case 18:
        allocExt = 1;
        break;
    case 15:
        elevation *= 2.2f;
        elevation += 7.0f;
        gravity = 3.3f;
        break;
    case 1:
        gravity = 2.4f;
        break;
    case 3:
        dist = 500.0f;
        math_getHeadingElevation(&movement_obj[0].mv->rayHit.dir, &heading, &elevation);
        vec[0] = movement_obj[0].mv->rayHit.origin.x;
        vec[1] = movement_obj[0].mv->rayHit.origin.y;
        vec[2] = movement_obj[0].mv->rayHit.origin.z;
        allocExt = 1;
        pos = vec;
        elevation = -elevation;
        break;
    case 6:
        allocExt = 1;
        elevation = 0.0f;
        dist = 500.0f;
        break;
    case 17:
        flags = 0x30;
        elevation = 0.0f;
        dist = 150.0f;
        break;
    case 4:
        dist = 250.0f;
        break;
    case 5:
    case 9:
    case 10:
        allocExt = 1;
        gravity = 5.0f;
        dist = 593.0f;
        scale = 1.5f;
        break;
    }
    actor_splitVelocity((s16)(elevation * 182.04445f), dist, 0.0f, &speed, &yVelocity, &unused);
    yVelocity = -yVelocity;
    event_spawnProjectile(heading, speed, yVelocity, gravity, 100, kind, 0.0f, scale, pos, flags, allocExt);
}

/* Spawns object gEventParam & 0xFF ahead of the current object, with its marker 5000 units out. */
void event_spawnObjectAhead(void) {
    Actor *obj;
    f32 xOff;
    f32 zOff;
    f32 yOff;
    s32 type;
    s32 angle;
    f32 height;

    type = gEventParam & 0xFF;
    obj = actor_findOnPath(collision_findSpawnById(type));
    if (obj == NULL) {
        return;
    }

    height = 0.0f;
    trail_spawnActorBoneTrail(obj, 0xFF);
    if ((gEventParam & 0xFF00) != 0) {
        height = gCurrentActor->CameraPtr->aim.signedPitch - 15.0f;
    }

    angle = gCurrentActor->langle;
    obj->fangle = angle;
    obj->langle = angle;
    obj->angle = angle;
    actor_splitVelocity(angle, 5000.0f, height, &xOff, &zOff, &yOff);

    gSpawnPaths[obj->listpos][1].x = gCurrentActor->xpos + xOff;
    gSpawnPaths[obj->listpos][1].z = gCurrentActor->zpos + zOff;
    gSpawnPaths[obj->listpos][1].y = gCurrentActor->ypos + yOff;
    obj->attach_char = 0;
    obj->scriptCmd = NULL;

    if ((gEventParam & 0xFF00) != 0) {
        obj->ypos = gCurrentActor->ypos + 70.0f;
        obj->intelwalk = 4;
        obj->scale_xz = 0.25f;
        obj->scale_y = 0.25f;
    } else {
        obj->intelwalk = 2;
        obj->speed = -15.0f;
        obj->yspd = 34.0f;
        obj->ygrav = 1.1f;
        obj->scale_xz = 0.5f;
        obj->scale_y = 0.5f;
    }
}

void func_1506D4EC(void) {
}

void func_1506D4F4(void) {
    actor_playAnim(gCurrentActor, gCurrentActor->animseq.uh, 0.01f, 0.0f, 0.0f, 0.0f, 0);
}

void func_1506D538(void) {
    if (gCutsceneStates[0] != 1) {
        actor_finishDeath(dc_glbl_CharNum);
    }
}

void event_resetBlink(void) {
    gCurrentActor->blinkTimer = 0;
}

/*
 * face_requestExpression with the two bytes of gEventParam's high half and its low half (0xFFFF = none)
 * scaled by 1 / unk2D0->unk10.
 */
void func_1506D584(void) {
    Actor *obj = gCurrentActor;
    f32 divisor;
    u16 value;
    u8 hi;
    u8 lo;

    if (obj->defaultExpression != 0x2A) {
        hi = gEventParam >> 24;
        lo = (gEventParam >> 16) & 0xFF;
        value = gEventParam & 0xFFFF;
        divisor = obj->unk2D0->speed[0];
        if (divisor == 0.0f) {
            divisor = 0.01f;
        }
        if (value != 0xFFFF) {
            value = value / divisor;
        }
        face_requestExpression(obj, hi, lo, value, 0);
    }
}

void func_1506D6B4(void) {
    Actor *obj = gCurrentActor;
    s32 sel;

    if (obj->waterHeight == -10000.0f) {
        return;
    }
    if (obj->waterHeight < obj->aim.y) {
        return;
    }
    if (obj->energy >= 2) {
        sel = 0x2C;
    } else {
        sel = 0x29;
    }
    gEventParam = (gEventParam & 0xFFFF) | (sel = (sel << 10) << 14); // matching: two-step shift + store to sel
    func_1506D584();
}

void func_1506D74C(void) {
    f32 xOff;
    f32 zOff;
    f32 yOff;

    if (gCurrentActor->inwater == 0) {
        gCurrentActor->ygrav = 5.0f;
        gCurrentActor->yspd = 0.0f;
    }
    gCurrentActor->ypos += 70.0f;
    gCurrentActor->unk137 = 0;
    gCurrentActor->norun = 0;
    actor_splitVelocity(gCurrentActor->angle, gEventParam, 0, &xOff, &zOff, &yOff);
    actor_playAnim(gCurrentActor, 15, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    gCurrentActor->xpos += xOff;
    gCurrentActor->zpos += zOff;
    gCurrentActor->unk100 |= 2;
    gCurrentActor->shad = 0.0f;
    gCurrentActor->count5 = 20;
    gCurrentActor->mv->camjolt = 12;
    gCurrentActor->mv->side = 10;
}

void func_1506D898(void) {
    gCurrentActor->ypos -= 80.0f;
    actor_playAnim(gCurrentActor, 663, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    gCurrentActor->unk100 &= 0xFFDF;
    gCurrentActor->count5 = 0;
    gCurrentActor->norun = 0;
    gCurrentActor->mv->ladderFromTop = 0;
    gCurrentActor->mv->camjolt = 12;
}

void func_1506D934(void) {
    gCurrentActor->unk103 = gEventParam;
}

void func_1506D950(void) {
}

void func_1506D958(void) {
    s32 i;
    s32 id = gEventParam & 0xFF;
    s32 flag = (gEventParam >> 8) & 0xFF;
    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type != 0 && id == movement_obj[i].unkAB &&
            dc_glbl_CharNum + 1 == movement_obj[i].attach_char) {
            movement_obj[i].unk101 &= ~1;
            movement_obj[i].unk101 |= flag;
            return;
        }
    }
}

void event_setImmune(void) {
    gCurrentActor->imune = gEventParam;
}

void func_1506DA94(void) {
    if (gEventParam != 0) {
        if ((gCurrentActor->stickspeed > 20.0f) && (gCurrentActor->speed > 20.0f)) {
            gCurrentActor->count5 = 0;
        }
    }
    particle_spawnWakePuff(dc_glbl_CharNum, 0, 24, 0, 409, 4, 170, 0xFF, 0);
}

void func_1506DB30(void) {
    gScriptArg0 = gEventParam;
    func_15076760();
}

void event_setSpeed(void) {
    gCurrentActor->speed = gEventParam;
}

void event_setIntelHitRoute(void) {
    gCurrentActor->intelhitroute = gEventParam;
}

void func_1506DBA0(void) {
    gCurrentActor->unk1E5 = gEventParam & 0xFF;
    gCurrentActor->unk1E6 = (gEventParam >> 8) & 0xFF;
}

void func_1506DBD4(void) {
    Actor *obj = event_getOwnedObject(gCurrentActor, 0);

    if (obj != NULL) {
        func_15054A5C(obj, gCurrentActor);
    }
}

void func_1506DC10(void) {
    s32 id;
    s32 roll;

    if (gCurrentActor->waterHeight - 60.0f < gCurrentActor->ypos || gCurrentActor->waterHeight == -10000.0f) {
        roll = math_random() & 3;
        if (roll >= 2) {
            id = roll + 0x612;
        } else {
            id = roll + 0x8F;
        }
    } else {
        id = 9;
    }
    actor_playUntrackedSound(id, gCurrentActor);
}

void event_setTaskInt1(void) {
    gCurrentActor->taskint[1] = gEventParam;
}

void event_setTaskInt2(void) {
    gCurrentActor->taskint[2] = gEventParam;
}

void event_setEnergy(void) {
    gCurrentActor->energy = gEventParam;
}

void func_1506DCF8(void) {
}

/* Sets bits gEventParam in unkF8, or resets it to the spawn record default when 0. */
void event_addNoShove(void) {
    if (gEventParam == 0) {
        gCurrentActor->noshove = gCurrentActor->listpos_ptr->objFlags;
    } else {
        gCurrentActor->noshove |= gEventParam;
    }
}

void event_clearNoShove(void) {
    gCurrentActor->noshove &= ~gEventParam;
}

void func_1506DD6C(void) {
    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->matrix_physics = gEventParam & 0x7F;
    }
    if (gEventParam == 0) {
        gCurrentActor->angle = gCurrentActor->langle;
    }
}

void func_1506DDB8(void) {
}

void func_1506DDC0(void) {
    if (func_15178E50(level) != 0) {
        particle_spawnThrottledEmitter(dc_glbl_CharNum, gEventParam, 0xFF, 0);
    }
}

void func_1506DE04(void) {
    s32 i;
    for (i = 0; i < 2; i++) {
        particle_spawnWakePuff(dc_glbl_CharNum, 10, 18, 12, 409, 4, 170, 0xFF, 0);
    }
}


void func_1506DE84(void) {
    switch (gEventParam) {
    case 0:
        if (gCurrentActor->shad == 0.0f) {
            effect_spawnFadeBurst(gCurrentActor->xpos, gCurrentActor->ypos, gCurrentActor->zpos,
                          30, 30, 7, 0.1f, 0.1f);
        }
        return;
    case 1:
        if ((math_random() & 0xFF) < 0x40U) { // 1 in 4
            effect_spawnFadeBurst(gCurrentActor->xpos, gCurrentActor->ypos, gCurrentActor->zpos,
                          10, 20, 7, 0.03f, 0.02f);
            gEventParam = 262;
            func_1506BA4C(300, 1800);
        }
        return;
    case 2:
        func_1516F1C0(dc_glbl_CharNum, 16, 0xFF, 0);
        return;
    case 3:
        func_1516F1C0(dc_glbl_CharNum, 40, 0xFF, 0);
        return;
    case 4:
        func_1516F1C0(dc_glbl_CharNum, 40, 0xFF, 0);
        return;
    case 5:
        func_1516F2F8(dc_glbl_CharNum, 30, 20, 2, 0xFF, 0);
        return;
    case 6:
        func_1516F2F8(dc_glbl_CharNum, 18, 12, 2, 0xFF, 0);
        return;
    case 7:
        func_1516F2F8(dc_glbl_CharNum, 60, 120, 2, 0xFF, 0);
        return;
    case 8:
        func_1506E0EC();
        return;
    case 9:
        particle_fireActorBoneTracer(0);
        return;
    case 10:
        particle_fireActorBoneTracer(1);
        return;
    case 11:
        func_1506E2CC();
        return;
    case 12:
        func_1516F2F8(dc_glbl_CharNum, 18, 12, 0, 0xFF, 0);
        return;
    case 13:
        particle_freeCharSpriteEmitters();
        break;
    }
}

/* Passes the corners of a +-270 square in the object's root matrix, at height unk180, to particle_spawnQuadEdgeStyleParticles. */
void func_1506E0EC(void) {
    f32 x;
    f32 y;
    f32 z;
    f32 height;
    f32 width;
    f32 depth;
    s32 i;
    f32 vertices[4][3];
    f32 posX;
    f32 posZ;

    if (gCurrentActor->ob_matrix_ptr != NULL) {
        width = 270.0f;
        depth = width;
        x = width;
        z = depth;
        height = gCurrentActor->floorY;
        for (i = 0; i < 4; i++) {
            y = 0.0f;
            math_transformPoint(*gCurrentActor->ob_matrix_ptr, x, 0.0f, z, &x, &y, &z);
            vertices[i][0] = x;
            vertices[i][1] = height;
            vertices[i][2] = z;
            effect_spawnFadeBurst(x, y, z, 30, 20, 7, 0.13f, 0.1f);
            if (i == 0) {
                z = depth;
                x = -width;
            } else if (i == 1) {
                x = -width;
                z = -depth;
            } else if (i == 2) {
                x = width;
                z = -depth;
            }
        }
        posX = gCurrentActor->xpos;
        posZ = gCurrentActor->zpos;
        particle_spawnQuadEdgeStyleParticles(vertices, posX, posZ, 4, gCurrentActor->scale_xz, 0, 4, 4, 0xFF, 0);
    }
}

/* Same as func_1506E0EC with a +-100 quad at half scale. */
void func_1506E2CC(void) {
    f32 x;
    f32 y;
    f32 z;
    f32 halfX;
    f32 halfZ;
    f32 height;
    s32 i;
    f32 verts[4][3];
    f32 posX;
    f32 posZ;

    if (gCurrentActor->ob_matrix_ptr != NULL) {
        halfX = 100.0f;
        halfZ = 100.0f;
        height = gCurrentActor->floorY;
        x = halfX;
        z = halfX;
        for (i = 0; i < 4; i++) {
            y = 0.0f;
            math_transformPoint(*gCurrentActor->ob_matrix_ptr, x, y, z, &x, &y, &z);
            verts[i][0] = x;
            verts[i][1] = height;
            verts[i][2] = z;
            if (i == 0) {
                z = halfX;
                x = -halfX;
            } else if (i == 1) {
                x = -halfX;
                z = -halfZ;
            } else if (i == 2) {
                x = halfX;
                z = -halfX;
            }
        }
        posX = gCurrentActor->xpos;
        posZ = gCurrentActor->zpos;
        particle_spawnQuadEdgeStyleParticles(verts, posX, posZ, 5, gCurrentActor->scale_xz * 0.5f, 0, 4, 4, 0xFF, 0);
    }
}
/*
 * Resolves a sound command in place: bit 21 skips it while sound_isActorSound2Playing / sound_isActorSound1Playing /
 * func_151F2CDC (by `mode`) reports busy, ids >= 2000 come from the object's gSpawnModelData table,
 * the top byte is a play chance out of 256 and bits 16-20 a count of random variants.
 * Returns 0 if nothing should play.
 */
s32 event_resolveSoundCommand(Actor *obj, s32 *command, s32 mode) {
    s32 pad0; // matching: stack layout (pad0 / pad[3] / inner pad)
    u32 cmd;
    s32 pad[3];
    u32 id;
    u32 entry;
    u32 hi;

    cmd = *command;
    id = *command & 0x7FFF;
    if (cmd & 0x200000) {
        switch (mode) {
        case 0:
            if (sound_isActorSound2Playing(obj) != 0) {
                return 0;
            }
            break;
        case 1:
            if (sound_isActorSound1Playing(obj) != 0) {
                return 0;
            }
            break;
        case 2:
            if (func_151F2CDC() == 1) {
                return 0;
            }
            break;
        }
    }
    if (id >= 2000) {
        entry = gSpawnModelData[obj->unk288][id - 2000];
        if (entry == 0) {
            return 0;
        }
        cmd &= 0xFF200000;
        if (cmd != 0) {
            cmd |= entry & 0xFFFFFF;
        } else {
            cmd |= entry;
        }
    }
    id = cmd >> 16;
    if (id != 0) {
    lab: // matching: an unused label splits the block, which steers as1
        if ((cmd >> 24) >= (math_random() & 0xFF)) {
            id &= 0x1F;
            cmd &= 0x7FFF;
            if (id) {
                s32 pad;

                cmd = sound_pickVariant(cmd, id | 0); // matching: | 0 changes register allocation
            }
        } else {
            return 0;
        }
    }
    *command = cmd;
    return 1;
}

void func_1506E5FC(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(300, 1800);
    }
}

void func_1506E63C(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(100, 800);
    }
}

void func_1506E67C(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(190, 1300);
    }
}

void func_1506E6BC(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(300, 3000);
    }
}

void func_1506E6FC(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(700, 4000);
    }
}

void func_1506E73C(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BA4C(3000, 8000);
    }
}

void func_1506E77C(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BBA8(300, 1800);
    }
}

void func_1506E7BC(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        func_1506BBA8(300, 3000);
    }
}

void func_1506E7FC(void) {
    if (gCurrentActor->shad == 0.0f) {
        if (gCurrentActor->ivehitit == 0) {
            func_1506E5FC();
        }
    }
}

void func_1506E848(void) {
    if ((event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) && (sound_isSoundPlaying(gEventParam) == 0)) {
        func_1506BA4C(80, 800);
    }
}

void func_1506E898(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 2) != 0) {
        func_1506BB64(300, 1800);
    }
}

void func_1506E8D8(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(300, 1800);
    }
}

void func_1506E918(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(100, 800);
    }
}

void func_1506E958(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(190, 1300);
    }
}

void func_1506E998(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(300, 3000);
    }
}

void func_1506E9D8(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(700, 4000);
    }
}

void func_1506EA18(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BAD8(3000, 8000);
    }
}

void func_1506EA58(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        func_1506BC24(300, 3000);
    }
}

/* Throws the object held in unk13C (object index + 100) along the current object's facing. */
void event_throwHeldObject(void) {
    u8 index;
    Actor *held;

    index = gCurrentActor->lifting - 100;
    if (gCurrentActor->lifting != 0) {
        held = &movement_obj[index];
        if (movement_obj[index].carried >= 100) {
            movement_obj[index].carried = 0;
            movement_obj[index].speed = 18.0f;
            movement_obj[index].yspd = 40.0f;
            movement_obj[index].ygrav = 6.0f;
            movement_obj[index].angle = gCurrentActor->langle;
            movement_obj[index].attach_char = 0;
            if (movement_obj[index].character_type == 0x20) {
                movement_obj[index].noshove |= 0x200;
                movement_obj[index].yspd = 0.0f;
                movement_obj[index].ygrav = 0.0f;
                movement_obj[index].speed = 0.0f;
            }
            movement_obj[index].stunned = 0xFE;
            movement_obj[index].stunnednum = 0;
            movement_obj[index].stunnedseq = actor_findReaction(12, held);
            movement_obj[index].animseq.uh = 0xFFFF;
            movement_obj[index].unk1CC = 1000000.0f;
            actor_advanceReaction(index, held);
            movement_obj[index].task_attrib |= 0x40;
        }
        gCurrentActor->lifting = 0;
    }
}

/* Drops the object held in unk13C (object index + 100). */
void event_dropHeldObject(void) {
    u8 index = gCurrentActor->lifting - 100;

    if (gCurrentActor->lifting) {
        if (movement_obj[index].carried >= 100) {
            movement_obj[index].attach_char = 0;
            movement_obj[index].carried = 0;
            movement_obj[index].speed = 0.0f;
            movement_obj[index].yspd = 0.0f;
            movement_obj[index].ygrav = 6.0f;
            movement_obj[index].unk1CC = 1000000.0f;
        }
        gCurrentActor->lifting = 0;
    }
}

void func_1506EC50(void) {
    if (gCurrentActor->shad > 0.2f) {
        if ((gEventParam & 0x8000) == 0 || movement_obj[0].ypos < gCurrentActor->ypos) {
            gEventResult = gEventParam & 0xFF;
            D_800D1880 = 0;
            gCurrentActor->speccnt -= 1;
        }
    }
}

void func_1506ECD0(void) {
    gCurrentActor->unk2D0->anim[0] |= 0x8000;
}

void func_1506ECF0(void) {
    gCurrentActor->unk3A = gEventParam;
}

void func_1506ED0C(void) {
    gCurrentActor->count5 = gEventParam;
    gCurrentActor->unk100 |= 0x10;
    gCurrentActor->mv->prop = 1;
}

void func_1506ED4C(void) {
    gCurrentActor->unk100 = gEventParam;
}

void func_1506ED68(void) {
    gCurrentActor->unkA9 = 5;
    gCurrentActor->mv->prop = 0;
}

void func_1506ED90(void) {
    gCurrentActor->unk21C = gEventParam;
}

void event_setMvmtControl(void) {
    gCurrentActor->mvmt_control = gEventParam;
}

void event_clearMvmtControl(void) {
    gCurrentActor->mvmt_control &= ~gEventParam;
}

void event_addMvmtControl(void) {
    gCurrentActor->mvmt_control |= gEventParam;
}

void event_addTaskAttrib(void) {
    gCurrentActor->task_attrib |= gEventParam;
}

void event_clearTaskAttrib(void) {
    gCurrentActor->task_attrib &= ~gEventParam;
}

void event_setAfterimage(void) {
    s32 param = gEventParam;
    s32 low = gEventParam & 0xFFFF;

    if (param != 0) {
        trail_startActorAfterimage(gCurrentActor, low, gEventParam >> 16);
        return;
    }
    trail_fadeActorAfterimage(gCurrentActor);
}

void func_1506EEAC(void) {
    func_151898C0(gCurrentActor, gEventParam);
}

void func_1506EED8(void) {
    gCurrentActor->unk24E = gEventParam;
}

void event_startHeadEffect(void) {
    gCurrentActor->headEffect = (gEventParam >> 16) & 0xFFFF;
    gCurrentActor->headEffectState = (gEventParam >> 24) & 0xFF;
    gCurrentActor->headEffectTimer = gEventParam & 0xFFFF;
    gCurrentActor->unk284 = 0;
    gCurrentActor->unk285 = 0;
    gCurrentActor->unk286 = 0;
    gCurrentActor->unk287 = 0;
}

/* Stores the byte pair (bits 8-15, 0-7) in slot (bits 16-23) of the unk284 pair array. */
void func_1506EF5C(void) {
    s32 index;
    s16 first;
    s16 second;

    index = (gEventParam >> 16) & 0xFF;
    first = (gEventParam >> 8) & 0xFF;
    second = gEventParam & 0xFF;
    gCurrentActor->headEffectTimer = 0xFFFF;
    gCurrentActor->headEffect = 5;
    index *= 2;
    (&gCurrentActor->unk284)[index] = first;
    (&gCurrentActor->unk285)[index] = second;
}

void event_stopHeadEffect(void) {
    gCurrentActor->headEffectTimer = 0;
}

void func_1506EFC8(s32 cmd) {
    if (gCurrentActor->character_type != 0x8C) {
        func_150BB760(gCurrentActor);
    }
}

void func_1506F004(s32 cmd) {
    func_150BCBBC(gCurrentActor);
}

void func_1506F02C(s32 cmd) {
    particle_attachOwnerEmitter(gCurrentActor, 0xFF, 0);
}

void func_1506F05C(s32 cmd) {
    particle_startObjDustEmitter(gCurrentActor, 50, 0xFF, 1);
}

void func_1506F090(s32 cmd) {
    particle_startObjDustEmitter(gCurrentActor, -1, 0xFF, 1);
}

void func_1506F0C4(s32 cmd) {
    particle_spawnFootDust(gCurrentActor, 0);
}

void func_1506F0F0(s32 cmd) {
    particle_spawnFootDust(gCurrentActor, 1);
}

void func_1506F11C(s32 cmd) {
    effect_spawnJointStepEffects(gCurrentActor, 0, 0xFF);
}

void func_1506F14C(s32 cmd) {
    effect_spawnJointStepEffects(gCurrentActor, 1, 0xFF);
}

void event_sprayTrails(s32 cmd) {
    trail_sprayFromRing(gCurrentActor, 0xFF);
}

void func_1506F1A8(s32 cmd) {
    CharacterFlamethrowerSetup descriptor;
    Actor *obj;

    obj = gCurrentActor;
    descriptor.seed.owner = obj;
    descriptor.seed.ownerId = obj->unique_id;
    descriptor.seed.startOffset.z = 80.0f;
    descriptor.seed.endOffset.z = 102.0f;
    descriptor.seed.gravity = 0.1223f;
    descriptor.seed.emitRate = 0.6f;
    descriptor.seed.maxLength = 1200.0f;
    descriptor.seed.fadeStart = 0.656f;
    descriptor.seed.size = 26.0f;
    descriptor.seed.sizeRate = 4.8f;
    descriptor.seed.texSpeed = -187.0f;
    descriptor.seed.texScale = 4.64f;
    descriptor.line.rate = 0.00502f;
    descriptor.line.rateRange = 0.003f;
    descriptor.line.unkC = 0.606f;
    descriptor.line.unk14 = 0.418f;
    descriptor.line.unk10 = 0.23900001f;
    descriptor.line.unk18 = 0.20500001f;
    descriptor.line.speedMin = 1.177f;
    descriptor.line.speedRange = 0.572f;
    descriptor.line.gravityMin = 0.81900007f;
    descriptor.line.gravityRange = 0.846f;
    descriptor.line.dragA = 0.94086f;
    descriptor.line.dragB = 0.942411f;
    descriptor.line.position = 0.39200002f;
    descriptor.line.shrinkRate = 0.008f;
    descriptor.scatter.rate = 0.24900001f;
    descriptor.scatter.rateRange = 0.35200003f;
    descriptor.scatter.unk20 = 14.0f;
    descriptor.scatter.unk24 = 9.0f;
    descriptor.scatter.spread = 25.0f;
    descriptor.scatter.unk34 = 0.44000003f;
    descriptor.scatter.unk38 = 0.45100003f;
    descriptor.emit.rate = 0.30900002f;
    descriptor.seed.bone = 12;
    descriptor.seed.alpha = 200;
    descriptor.emit.rateRange = 0.312f;
    descriptor.seed.initFn = -1;
    descriptor.seed.attackFn = 2;
    descriptor.seed.destroyFn = -1;
    descriptor.seed.updateFn = -1;
    descriptor.kind = 15;
    descriptor.flags = 1;
    descriptor.red = 60;
    descriptor.green = 255;
    descriptor.blue = 235;
    descriptor.alpha = 82;
    descriptor.emit.unk1C = 402.0f;
    descriptor.line.lifeMin = 30;
    descriptor.line.lifeRange = 15;
    descriptor.line.fadeStart = 12;
    descriptor.line.alphaPerLife = 21;
    descriptor.line.unk3C = 80;
    descriptor.line.unk3D = 120;
    descriptor.scatter.unk14 = 4;
    descriptor.scatter.unk18 = 2;
    descriptor.scatter.unk1C = 30;
    descriptor.scatter.unk1E = 17;
    descriptor.seed.startOffset.x = 0.0f;
    descriptor.seed.startOffset.y = 0.0f;
    descriptor.seed.endOffset.x = 0.0f;
    descriptor.seed.endOffset.y = 0.0f;
    descriptor.seed.soundFn = 0;
    descriptor.seed.soundUpdateFn = 0;
    descriptor.seed.soundStopFn = 0;
    descriptor.scatter.target.x = 0.0f;
    descriptor.scatter.target.y = 5.0f;
    descriptor.scatter.target.z = 100.0f;
    descriptor.scatter.speedMin = 0.5f;
    descriptor.scatter.speedRange = 0.5f;
    descriptor.scatter.unk3C = 80;
    descriptor.scatter.unk3E = 120;
    descriptor.scatter.unk40 = 16;
    descriptor.scatter.unk42 = 15;
    descriptor.emit.target.x = 0.0f;
    descriptor.emit.target.y = 5.0f;
    descriptor.emit.target.z = 100.0f;
    descriptor.emit.unk14 = 40;
    descriptor.emit.unk16 = 15;
    descriptor.emit.unk18 = 80;
    descriptor.emit.unk1A = 80;
    descriptor.emit.unk38 = 20;
    descriptor.emit.unk3A = 12;
    descriptor.emit.unk20 = 303.0f;
    descriptor.emit.speedMin = 0.101f;
    descriptor.emit.speedRange = 0.19800001f;
    descriptor.emit.spread = 45.0f;
    descriptor.emit.unk30 = -0.65500003f;
    descriptor.emit.unk34 = 0.30800003f;
    flamethrower_spawnFlame(0, &descriptor, 0xFF, 0);
}

void event_stopFlamethrowerJets(s32 cmd) {
    flamethrower_stopJets(gCurrentActor);
}

void func_1506F54C(s32 cmd) {
    CharacterFlamethrowerSetup descriptor;

    descriptor.seed.owner = gCurrentActor;
    descriptor.seed.ownerId = gCurrentActor->unique_id;
    descriptor.seed.bone = 13;
    descriptor.seed.startOffset.x = 22.0f;
    descriptor.seed.startOffset.y = 22.0f;
    descriptor.seed.startOffset.z = 165.0f;
    descriptor.seed.endOffset.x = 22.0f;
    descriptor.seed.endOffset.y = 22.0f;
    descriptor.seed.endOffset.z = 193.0f;
    descriptor.seed.gravity = 1.138f;
    descriptor.seed.emitRate = 0.795f;
    descriptor.seed.maxLength = 370.0f;
    descriptor.seed.fadeStart = 0.615f;
    descriptor.seed.size = 17.800001f;
    descriptor.seed.sizeRate = 4.9f;
    descriptor.seed.alpha = 159;
    descriptor.seed.initFn = 0;
    descriptor.seed.attackFn = -1;
    descriptor.seed.destroyFn = 0;
    descriptor.seed.soundFn = 0;
    descriptor.seed.soundUpdateFn = 0;
    descriptor.seed.soundStopFn = 0;
    descriptor.seed.texSpeed = -191.0f;
    descriptor.seed.texScale = 7.85f;
    descriptor.seed.updateFn = -1;
    descriptor.kind = 15;
    descriptor.flags = 1;
    descriptor.red = 60;
    descriptor.green = 255;
    descriptor.blue = 235;
    descriptor.alpha = 82;
    descriptor.line.rate = 0.0029f;
    descriptor.line.rateRange = 0.00235f;
    descriptor.line.lifeMin = 35;
    descriptor.line.lifeRange = 10;
    descriptor.line.unkC = 0.80200005f;
    descriptor.line.unk14 = 0.367f;
    descriptor.line.unk10 = 0.58900005f;
    descriptor.line.unk18 = 0.307f;
    descriptor.line.speedMin = 0.614f;
    descriptor.line.speedRange = 0.29900003f;
    descriptor.line.gravityMin = 0.0f;
    descriptor.line.gravityRange = 0.73800003f;
    descriptor.line.dragA = 0.962646f;
    descriptor.line.dragB = 0.961868f;
    descriptor.line.fadeStart = 15;
    descriptor.line.alphaPerLife = 17;
    descriptor.line.position = 0.36900002f;
    descriptor.line.unk3C = 80;
    descriptor.line.unk3D = 80;
    descriptor.line.shrinkRate = 0.024f;
    descriptor.scatter.target.x = 22.0f;
    descriptor.scatter.target.y = 58.0f;
    descriptor.scatter.target.z = 212.0f;
    descriptor.scatter.rate = 0.2f;
    descriptor.scatter.rateRange = 0.105000004f;
    descriptor.scatter.unk14 = 3;
    descriptor.scatter.unk18 = 2;
    descriptor.scatter.unk1C = 25;
    descriptor.scatter.unk1E = 10;
    descriptor.scatter.unk20 = 6.0f;
    descriptor.scatter.unk24 = 10.0f;
    descriptor.scatter.speedMin = 0.3f;
    descriptor.scatter.speedRange = 0.2f;
    descriptor.scatter.spread = 37.0f;
    descriptor.scatter.unk34 = 0.75100005f;
    descriptor.scatter.unk38 = 0.32200003f;
    descriptor.scatter.unk3C = 80;
    descriptor.scatter.unk3E = 80;
    descriptor.scatter.unk40 = 15;
    descriptor.scatter.unk42 = 17;
    descriptor.emit.target.x = 22.0f;
    descriptor.emit.target.y = 130.0f;
    descriptor.emit.target.z = 306.0f;
    descriptor.emit.rate = 0.208f;
    descriptor.emit.rateRange = 0.193f;
    descriptor.emit.unk14 = 35;
    descriptor.emit.unk16 = 15;
    descriptor.emit.unk18 = 80;
    descriptor.emit.unk1A = 100;
    descriptor.emit.unk1C = 198.0f;
    descriptor.emit.unk20 = 102.0f;
    descriptor.emit.speedMin = 0.040000003f;
    descriptor.emit.speedRange = 0.040000003f;
    descriptor.emit.spread = 89.0f;
    descriptor.emit.unk30 = -0.49500004f;
    descriptor.emit.unk34 = 0.44300002f;
    descriptor.emit.unk38 = 15;
    descriptor.emit.unk3A = 17;
    flamethrower_spawnFlame(0, &descriptor, 0xFF, 0);
}

void func_1506F8C0(s32 cmd) {
    effect_emptyActorEventHook(gCurrentActor, 0, 0);
}

void func_1506F8F0(s32 cmd) {
    beam_spawnBoneTracerEx(gCurrentActor, gCurrentActor->unique_id, 0x14, -1, 482.0f, -127.0f, -45.0f, 659.0f, -211.0f, -114.0f, 3, 3, 5, 20.0f, (math_randomFloat() * 10.0f) + 40.0f, 0, 0.0f);
}

void func_1506F9C0(s32 cmd) {
    beam_spawnBoneTracerEx(gCurrentActor, gCurrentActor->unique_id, 0x14, -1, 241.0f, -127.0f, -45.0f, 418.0f, -211.0f, -114.0f, 3, 3, 5, 20.0f, (math_randomFloat() * 10.0f) + 40.0f, 0, 0.0f);
}

void func_1506FA90(s32 cmd) {
    beam_spawnBoneTracerEx(gCurrentActor, gCurrentActor->unique_id, 0x1A, -1, -181.0f, -218.0f, -1.0f, -681.0f, -584.0f, -2.0f, 3, 3, 5, 20.0f, (math_randomFloat() * 10.0f) + 40.0f, 0, 0.0f);
}

void func_1506FB60(s32 cmd) {
    if (func_151044F4() != 0) {
        func_151C6A28(gCurrentActor, cmd - 0x13, 0xFF, 0);
    } else {
        weapon_fireJointShot(gCurrentActor, cmd - 0x13, 0, 0, -1, 0xFF, 0);
    }
}

void func_1506FBE8(s32 cmd) {
    particle_startSparkEmitter(gCurrentActor, 0x46, 0xFF, 0);
}

void func_1506FC1C(s32 cmd) {
    particle_startSparkEmitter(gCurrentActor, 0x28, 0xFF, 0);
}

void event_cancelTrailPair(s32 cmd) {
    trail_cancelTrailPair();
}

void event_queueTrailPair(s32 cmd) {
    trail_queueTrailPair(gCurrentActor);
}

void func_1506FC9C(s32 cmd) {
    func_150B3AB0(gCurrentActor, 0xFF);
}

void func_1506FCC8(s32 cmd) {
    flamethrower_spawnJet(gCurrentActor, 6, 0xFF, 0);
}

void func_1506FCFC(s32 cmd) {
    flamethrower_spawnJet(gCurrentActor, 5, 0xFF, 0);
}

void func_1506FD30(s32 cmd) {
    beam_spawnBoneTracerEx(gCurrentActor, gCurrentActor->unique_id, 1, -1, 0.0f, 0, 345.0f, 0.0f, 0, 414.0f, 3, 3, 5, 20.0f, (math_randomFloat() * 10.0f) + 40.0f, 1, 50.0f);
}

void func_1506FDF0(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 0);
}

void func_1506FE1C(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 1);
}

void func_1506FE48(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 0);
}

void func_1506FE74(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 1);
}

void func_1506FEA0(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 2);
}

void func_1506FECC(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 3);
}

void func_1506FEF8(s32 cmd) {
    func_151AB920(gCurrentActor, 0);
}

void func_1506FF24(s32 cmd) {
    func_151AB920(gCurrentActor, 1);
}

void func_1506FF50(s32 cmd) {
    effect_spawnSparkEmitterTask(gCurrentActor);
}

void func_1506FF78(s32 cmd) {
    particle_spawnBoneGroundBurst(gCurrentActor, 0, 1, 0xFF);
}

void func_1506FFAC(s32 cmd) {
    particle_spawnBoneGroundBurst(gCurrentActor, 1, 1, 0xFF);
}

void func_1506FFE0(s32 cmd) {
    particle_spawnBoneGroundBurst(gCurrentActor, 2, 1, 0xFF);
}

void func_15070014(s32 cmd) {
    effect_spawnImpactRing(gCurrentActor);
}

void func_1507003C(s32 cmd) {
    effect_spawnJointImpact(gCurrentActor, 0);
    effect_startJointEmitterTask(gCurrentActor, 0, 0x3E8, 0xFF, 0);
}

void func_15070084(s32 cmd) {
    blood_spawnGroundBurst(gCurrentActor, 0xFF, 1);
}

void func_150700B4(s32 cmd) {
    effect_spawnLandingBurst(gCurrentActor, 0xFF, 1);
}

void func_150700E4(s32 cmd) {
    particle_spawnBoneModelBurst(gCurrentActor, 0xFF, 1);
}

void func_15070114(s32 cmd) {
    particle_attachBloodStrip(gCurrentActor, 0xFF, 1);
}

void func_15070144(s32 cmd) {
    IgnoreList info;
    s32 mode;

    info.count = 1;
    info.objs[1] = NULL;
    info.objs[2] = NULL;
    info.objs[0] = gCurrentActor;

    if (level == 0x2B) {
        mode = 3;
    } else {
        mode = 0;
    }
    weapon_fireBullet(gCurrentActor, NULL, &info, mode, 0xFF, 1);
    weapon_rumbleShooter(gCurrentActor);
}

void func_150701C4(s32 cmd) {
    weapon_fireSegmentShot(gCurrentActor, 0xFF, 1);
}

void func_150701F4(s32 cmd) {
    sight_explodeAtActor(gCurrentActor, 0xFF, 1);
}

void func_15070224(s32 cmd) {
    LightEffectHeader header;
    Vec3i header2;

    header.flags = 3;
    header.update = -1;
    header.lifetime = (math_random() % 5U) + 4;
    header.lightType = 0;

    header2.x = gCurrentActor->xpos;
    header2.y = gCurrentActor->ypos;
    header2.z = gCurrentActor->zpos;

    light_createWithEffect(&header, &header2, (math_random() % 3U) + 4, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 1);
}


/* Objects 0x77 / 0x28: fires (weapon_fireBullet, else event_fireProjectile), then spawns effect effect_spawnBoneBeamParticle at bone 4. */
void func_15070300(s32 cmd) {
    u8 flag;
    Actor *obj;
    u8 mode;
    Actor *other;
    u8 idx;
    IgnoreList info;
    Vec3f localPos;
    Vec3f vec74;
    ParticleSize randoms;
    Vec3f worldPos;
    const Vec3f *srcs[1];
    Vec3f *dsts[1];

    other = NULL;
    if (gCurrentActor->character_type == 0x77) {
        mode = 1;
        obj = gCurrentActor;
        idx = gCurrentActor - movement_obj;
        if (idx < 2 && D_800E0BCE[idx] != 0) {
            flag = 1;
            other = &movement_obj[D_800E0BCE[idx]];
            goto done; // matching: an else branch drops the dead flag = 0 below
        }
        flag = 0;
        if (obj->mv->SniperState != 0) {
            flag = 1;
        } else {
            flag = 0;
        }
    done:;
    } else if (gCurrentActor->character_type == 0x28) {
        obj = event_getFirstLiveOwnedObject(gCurrentActor);
        mode = 0;
        flag = 0;
        if (obj->mv->SniperState != 0) {
            flag = 1;
        } else {
            flag = 0;
        }
    } else {
        return;
    }
    weapon_rumbleShooter(obj);
    if (other != NULL) {
        weapon_rumbleShooter(other);
    }
    if (gIsMultiplayer != 0) {
        if (gCurrentActor->mv->unk128 & 0x10) {
            gEventParam = 10;
            mode = 2;
            gCurrentActor->mv->unk12C--;
        } else {
            gEventParam = 9;
        }
    } else {
        gEventParam = 5;
    }
    if (flag) {
        info.count = 3;
        info.objs[0] = obj;
        info.objs[1] = other;
        info.objs[2] = gCurrentActor;
        weapon_fireBullet(obj, other, &info, mode, 0xFF, 1);
    } else {
        event_fireProjectile();
    }
    if (gCurrentActor->ob_matrix_ptr == NULL) {
        return;
    }
    if ((gCurrentActor->unk74 & 0xF) == 0xF) {
        return;
    }
    localPos = D_80099B70;
    vec74 = D_80099B7C;
    randoms.width = (math_randomFloat() * 30.0f) + 40.0f;
    randoms.height = (math_randomFloat() * 2.512f) + 3.5900002f;
    srcs[0] = &localPos;
    dsts[0] = &worldPos;
    math_transformOffsets(srcs, dsts, &gCurrentActor->ob_matrix_ptr[4], 1);
    effect_spawnBoneBeamParticle(gCurrentActor, 4, &localPos, &vec74, &randoms,
                  (math_random() % 5U) + 5, (math_random() % 56U) + 200,
                  (math_randomFloat() * 510.0f) + 1296.0f,
                  &worldPos, 0xFF, 0, -1, 0xFF, 1);
    weapon_spawnShortLight(&worldPos, 0xFF, 1);
}

void func_15070690(s32 cmd) {
    beam_fireSlot(gCurrentActor, (cmd - 0x3E) & 0xFF, cmd);
}

void func_150706C4(s32 cmd) {
    beam_releaseSlot(gCurrentActor, (cmd - 0x3E) & 0xFF, cmd);
}

void func_150706F8(s32 cmd) {
    if ((gCurrentActor->unk94 & 0x10) == 0) {
        beam_fireSlotProjectile(gCurrentActor, 0, 0xFF, 1);
    }
    if ((gCurrentActor->unk94 & 8) == 0) {
        beam_fireSlotProjectile(gCurrentActor, 1, 0xFF, 1);
    }
}

void event_fireMuzzle0(s32 cmd) {
    weapon_fireTwinMuzzleShot(gCurrentActor, 0, 0xFF, 1);
}

void event_fireMuzzle1(s32 cmd) {
    weapon_fireTwinMuzzleShot(gCurrentActor, 1, 0xFF, 1);
}

void event_fireLauncher(s32 cmd) {
    weapon_fireLauncherProjectile(gCurrentActor, 0xFF, 1);
}

void func_150707F8(s32 cmd) {
    weapon_runLauncherMuzzle(gCurrentActor, 0xFF, 1, 0, 0);
}

void func_15070830(s32 cmd) {
    func_150FE320(gCurrentActor, 0xFF, 1);
}

void func_15070860(s32 cmd) {
    func_150FE49C(gCurrentActor, 0xFF, 1, 0, 0);
}

void func_15070898(s32 cmd) {
    CharacterFlamethrowerSetup descriptor;

    descriptor.seed.owner = gCurrentActor;
    descriptor.seed.ownerId = gCurrentActor->unique_id;
    descriptor.seed.bone = 9;
    descriptor.seed.startOffset.x = -4.734001f;
    descriptor.seed.startOffset.y = -111.957596f;
    descriptor.seed.startOffset.z = 33.473396f;
    descriptor.seed.endOffset.x = -8.837997f;
    descriptor.seed.endOffset.y = -159.75f;
    descriptor.seed.endOffset.z = 33.473396f;
    descriptor.seed.gravity = 0.278f;
    descriptor.seed.emitRate = 0.82800007f;
    descriptor.seed.maxLength = 312.0f;
    descriptor.seed.fadeStart = 0.325f;
    descriptor.seed.size = 7.1f;
    descriptor.seed.sizeRate = 5.0f;
    descriptor.seed.alpha = 180;
    descriptor.seed.initFn = -1;
    descriptor.seed.attackFn = 1;
    descriptor.seed.destroyFn = -1;
    descriptor.seed.soundFn = -1;
    descriptor.seed.soundUpdateFn = -1;
    descriptor.seed.soundStopFn = -1;
    descriptor.seed.texSpeed = -232.0f;
    descriptor.seed.texScale = 9.07f;
    descriptor.seed.updateFn = -1;
    descriptor.kind = 15;
    descriptor.flags = 17;
    descriptor.red = 60;
    descriptor.green = 255;
    descriptor.blue = 235;
    descriptor.alpha = 82;
    descriptor.line.rate = 0.00689f;
    descriptor.line.rateRange = 0.01038f;
    descriptor.line.lifeMin = 25;
    descriptor.line.lifeRange = 10;
    descriptor.line.unkC = 0.367f;
    descriptor.line.unk14 = 0.256f;
    descriptor.line.unk10 = 0.35000002f;
    descriptor.line.unk18 = 0.20500001f;
    descriptor.line.speedMin = 0.776f;
    descriptor.line.speedRange = 0.81100005f;
    descriptor.line.gravityMin = 0.125f;
    descriptor.line.gravityRange = 1.3110001f;
    descriptor.line.dragA = 0.935119f;
    descriptor.line.dragB = 0.943452f;
    descriptor.line.fadeStart = 12;
    descriptor.line.alphaPerLife = 21;
    descriptor.line.position = 0.307f;
    descriptor.line.unk3C = 80;
    descriptor.line.unk3D = 80;
    descriptor.line.shrinkRate = 0.002f;
    descriptor.scatter.target.x = -8.837997f;
    descriptor.scatter.target.y = -159.75f;
    descriptor.scatter.target.z = 33.473396f;
    descriptor.scatter.rate = 0.3f;
    descriptor.scatter.rateRange = 0.40100002f;
    descriptor.scatter.unk14 = 4;
    descriptor.scatter.unk18 = 2;
    descriptor.scatter.unk1C = 26;
    descriptor.scatter.unk1E = 16;
    descriptor.scatter.unk20 = 6.0f;
    descriptor.scatter.unk24 = 5;
    descriptor.scatter.speedMin = 0.3f;
    descriptor.scatter.speedRange = 0.3f;
    descriptor.scatter.spread = 50.0f;
    descriptor.scatter.unk34 = 0.0f;
    descriptor.scatter.unk38 = 0.712f;
    descriptor.scatter.unk3C = 80;
    descriptor.scatter.unk3E = 120;
    descriptor.scatter.unk40 = 15;
    descriptor.scatter.unk42 = 17;
    descriptor.emit.target.x = -8.837997f;
    descriptor.emit.target.y = -159.75f;
    descriptor.emit.target.z = 33.473396f;
    descriptor.emit.rate = 0.20400001f;
    descriptor.emit.rateRange = 0.504f;
    descriptor.emit.unk14 = 30;
    descriptor.emit.unk16 = 15;
    descriptor.emit.unk18 = 80;
    descriptor.emit.unk1A = 80;
    descriptor.emit.unk1C = 140.0f;
    descriptor.emit.unk20 = 182.0f;
    descriptor.emit.speedMin = 0.16000001f;
    descriptor.emit.speedRange = 0.113000005f;
    descriptor.emit.spread = 42.0f;
    descriptor.emit.unk30 = -0.518f;
    descriptor.emit.unk34 = 0.37600002f;
    descriptor.emit.unk38 = 10;
    descriptor.emit.unk3A = 25;
    flamethrower_spawnFlame(0, &descriptor, 0xFF, 0);
}

void event_stopFlames(s32 cmd) {
    flamethrower_stopFlames(gCurrentActor);
}

void func_15070C40(u8 variant) {
    Vec3f pos;
    Actor *obj;
    Actor *owner;

    obj = gCurrentActor;
    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;

    owner = obj->owner ? &movement_obj[obj->owner - 1] : obj;
    effect_spawnImpactBlast(owner, obj, &pos, variant, 0, 0xFF, 0);
}

void func_15070CDC(s32 cmd) {
    func_15070C40(1);
}

void func_15070D00(s32 cmd) {
    func_15070C40(0);
}

void func_15070D24(s32 cmd) {
    EffectLocals fx;

    fx.localPos = D_80099B88;
    fx.vec78 = D_80099B94;

    if (gCurrentActor->ob_matrix_ptr == NULL) {
        return;
    }

    if ((gCurrentActor->unk74 & 0xF) == 0xF) {
        return;
    }

    fx.randoms.width = (math_randomFloat() * 101.0f) + 151.0f;
    fx.randoms.height = (math_randomFloat() * 4.0f) + 4.0f;

    math_transformOffset(&fx.localPos, &fx.worldPos, &gCurrentActor->ob_matrix_ptr[1]);

    effect_spawnBoneBeamParticle(gCurrentActor, 1, &fx.localPos, &fx.vec78, &fx.randoms,
                  (math_random() % 6U) + 5, (math_random() % 156U) + 100,
                  (math_randomFloat() * 2496.0f) + 3496.0f,
                  &fx.worldPos, 0xFF, 0, -1, 0xFF, 0);

    fx.header.flags = 3;
    fx.header.update = -1;
    fx.header.lifetime = (math_random() % 6U) + 0xA;
    fx.header.lightType = 0;

    fx.header2.x = fx.worldPos.x;
    fx.header2.y = fx.worldPos.y;
    fx.header2.z = fx.worldPos.z;

    light_createWithEffect(&fx.header, &fx.header2, (math_random() % 201U) + 55, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0);
}
void func_15070F60(cmd)
    u8 cmd;
{
    EffectLocals fx;

    fx.localPos = D_80099BA0;
    fx.vec78 = D_80099BAC;

    if (gCurrentActor->ob_matrix_ptr == NULL) {
        return;
    }

    if ((gCurrentActor->unk74 & 0xF) == 0xF) {
        return;
    }

    fx.randoms.width = (math_randomFloat() * 180.0f) + 150.0f;
    fx.randoms.height = math_randomFloat() * 2.0f + 2.5f;

    math_transformOffset(&fx.localPos, &fx.worldPos, &gCurrentActor->ob_matrix_ptr[cmd ? 1 : 2]);

    effect_spawnBoneBeamParticle(gCurrentActor, cmd ? 1 : 2, &fx.localPos, &fx.vec78, &fx.randoms,
                  (math_random() % 5U) + 5, (math_random() % 156U) + 100,
                  (math_randomFloat() * 4000.0f) + 4000.0f,
                  &fx.worldPos, 0xFF, 0, -1, 0xFF, 0);

    fx.header.flags = 3;
    fx.header.update = -1;
    fx.header.lifetime = (math_random() % 5U) + 3;
    fx.header.lightType = 0;

    fx.header2.x = fx.worldPos.x;
    fx.header2.y = fx.worldPos.y;
    fx.header2.z = fx.worldPos.z;

    light_createWithEffect(&fx.header, &fx.header2, (math_random() % 201U) + 55, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0);
    particle_startPlayerRumbles(&fx.worldPos, 506.0f, 1013.0f, 0.0009871669f, 0xF, 0x14, 0xFF, 0);
    particle_startPlayerCameraWobbles(&fx.worldPos, 506.0f, 1013.0f, 0.0009871669f, 5, 0xFF, 0);
}
void func_15071230(s32 cmd) {
    func_15070F60(0);
}

void func_15071254(s32 cmd) {
    func_15070F60(1);
}

void func_15071278(s32 cmd) {
    particle_spawnTwoSidedEmitter(gCurrentActor, 0, 1, gCurrentActor->animseq.ub[1]);
}

void func_150712AC(s32 cmd) {
    RumbleParams args;

    weapon_spawnForwardProjectile(gCurrentActor, 0xFF, 1);
    if (gCurrentActor->CameraPtr != NULL) {
        args.flags = 1;
        args.duration = (math_random() % 7U) + 10;
        args.playerMask = 1 << gCurrentActor->CameraPtr->CNumber;
        args.strength = (math_random() % 7U) + 2;
        args.updateKind = -1;
        cont_spawnRumble(&args, 0, 0xFF, 1);
    }
}

void func_15071360(s32 cmd) {
    RumbleParams args;

    if (func_151044F4() != 0) {
        func_150FF084(gCurrentActor, 0xFF, 1);
    } else {
        weapon_fireAttachedGun(gCurrentActor, 0xFF, 1);
    }
    if (gCurrentActor->CameraPtr != NULL) {
        args.flags = 1;
        args.duration = (math_random() % 5U) + 15;
        args.playerMask = 1 << gCurrentActor->CameraPtr->CNumber;
        args.strength = (math_random() % 4U) + 3;
        args.updateKind = -1;
        cont_spawnRumble(&args, 0, 0xFF, 1);
    }
}

void func_15071434(s32 cmd) {
    weapon_spawnShotEffects(gCurrentActor, 1, 0, 0xFF, 1);
}

void func_15071470(s32 cmd) {
    weapon_spawnShotEffects(gCurrentActor, 0, 0, 0xFF, 1);
}

void func_150714AC(s32 cmd) {
    weapon_spawnShotEffects(gCurrentActor, 0, 1, 0xFF, 1);
}

void func_150714E8(s32 cmd) {
    particle_spawnBoneSegmentParticle(gCurrentActor, &D_800A2148, &D_800A2154, gGunMuzzleBone, 1.0f, 0xFF, 1);
}

void func_15071544(s32 cmd) {
    particle_detachObjEmitters(gCurrentActor);
    particle_attachPointEmitter(gCurrentActor, 4, 0x78, 0xFF, 1);
}

void func_1507158C(s32 cmd) {
    particle_detachObjEmitters(gCurrentActor);
    particle_attachPointEmitter(gCurrentActor, 2, 0x78, 0xFF, 1);
}

void func_150715D4(s32 cmd) {
    particle_detachObjEmitters(gCurrentActor);
    particle_attachPointEmitter(gCurrentActor, 3, 0x78, 0xFF, 1);
}

void func_1507161C(s32 cmd) {

}

void event_spawnStripEffect(s32 cmd) {
    trail_spawnStripEffect(gCurrentActor, cmd - 0x55, 0xFF, 1);
}

void event_notifyFollowersGone(s32 cmd) {
    trail_notifyFollowersGone(gCurrentActor);
}

void func_15071690(s32 cmd) {
    Actor *obj;
    s32 pad0; // matching: unused stack slot
    s32 buf;

    obj = gCurrentActor;
    if (obj->ob_matrix_ptr != NULL && (obj->unk74 & 0xF) != 0xF) {
        effect_getBone5Point(obj, &buf);
        effect_spawnLargeObjBurstAt(&buf, 0xFF, 1);
    }
}

void func_150716EC(s32 cmd) {
    Vec3f pos;
    Actor *obj;

    obj = gCurrentActor;
    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;
    particle_startPlayerRumbles(&pos, 1502.0f, 3000.0f, 0.00033333333f, 12, 15, 0xFF, 0);
}

void func_15071764(s32 cmd) {
    Actor *obj = collision_findObjectById(18);
    if (obj != NULL) {
        beam_spawnFollowParticle(obj);
        beam_attachTrailEmitter(obj);
        light_createAtActor(obj, 1, 2, 300, 70, 255, 130, 0, 255, 1);
    }
}

void func_150717E0(s32 cmd) {
    ActorRef ref;
    Actor *obj;

    obj = collision_findObjectById(18);
    if (obj != NULL) {
        ref.actor = obj;
        ref.uniqueId = obj->unique_id;
        particle_broadcastToSprites((s32)&ref, 0x43);
        effect_sendEventToTimedEffects((s32)&ref, 0x43);
    }
}

void func_15071830(s32 cmd) {
    effect_spawnDelayedBurst(gCurrentActor, 0xFF, 1);
}

void func_15071860(s32 cmd) {
    func_1519072C(gCurrentActor);
}

void func_15071888(s32 cmd) {
    particle_spawnBoneSegmentParticle(gCurrentActor, &D_800A1FB0, &D_800A1FBC, D_80088B90, 0.7f, 0xFF, 1);
}

/* Spawns effect weapon_spawnImpactBurst at one of four random bones. */
void func_150718E4(s32 cmd) {
    Vec3f pos;
    const u8 bones[4] = { 13, 16, 24, 27 };

    if (gCurrentActor->ob_matrix_ptr != NULL || (gCurrentActor->unk74 & 0xF) == 0xF) {
        math_transformOffset(&gZeroVec3f, &pos, &gCurrentActor->ob_matrix_ptr[bones[math_random() & 3]]);
        pos.y = gCurrentActor->floorY + 20.0f;
        weapon_spawnImpactBurst(&pos, 0xFF, 1);
    }
}

const Vec3f D_80099BBC = { 0.0f, 17.0f, 7.0f };
const Vec3f D_80099BC8[3] = {
    { 0.0f, 2.0f, 145.0f },
    { 62.0f, 11.0f, 114.0f },
    { -62.0f, 11.0f, 114.0f },
};

/* Debris model ids and their scales (particle_spawnDebrisBurst). */
const s32 gDebrisModelIds[6] = { 0x39, 0x11, 0x12, 0x13, 0x14, 0x15 };
const f32 gDebrisModelScales[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

void func_15071998(s32 cmd) {
    effect_spawnBonePointImpact(gCurrentActor, 0, 0xFF, 1);
}

void func_150719CC(s32 cmd) {
    effect_spawnBonePointImpact(gCurrentActor, 1, 0xFF, 1);
}

void func_15071A00(s32 cmd) {
    effect_spawnBonePointImpact(gCurrentActor, 2, 0xFF, 1);
}

void func_15071A34(s32 cmd) {
    trail_emitLineBursts(gCurrentActor, 0xFF, 1);
}

void func_15071A64(s32 cmd) {
    GroundPoint hit;

    if (!(math_random() & 1) && gCurrentActor->ob_matrix_ptr != NULL && (gCurrentActor->unk74 & 0xF) != 0xF &&
        movement_obj[0].stunned != 0 && movement_obj[0].energy > 0) {
        collision_initHitFromObject(&hit.ground, gCurrentActor);
        math_transformOffset(&D_80099BBC, &hit.pos, &gCurrentActor->ob_matrix_ptr[15]);
        particle_spawnSmallSplashAndDrops(&hit.pos.x, &hit.ground, 0, 0xFF, 1);
    }
}

void func_15071B18(s32 cmd) {
    Vec3f point;
    CollisionHit collision;

    if (gCurrentActor->ob_matrix_ptr != NULL && (gCurrentActor->unk74 & 0xF) != 0xF) {
        math_transformOffset(&D_80099BC8[math_random() % 3U], &point, &gCurrentActor->ob_matrix_ptr[7]);
        collision_initHitFromObject(&collision, gCurrentActor);
        if (math_random() & 1) {
            if (math_random() & 1) {
                particle_spawnSplashAndDrops(&point.x, &collision, 0, 0xFF, 1);
            } else {
                particle_spawnSmallSplashAndDrops(&point.x, &collision, 0, 0xFF, 1);
            }
        }
        if (math_random() & 1) {
            DebrisBurst config;

            config.countBase = 8;
            config.countRange = 5;
            config.pos = point;
            config.angles.yawBase = 0;
            config.angles.yawRange = 0xFF;
            config.angles.pitchBase = -64;
            config.angles.pitchRange = 60;
            config.speedBase = 10.0f;
            config.speedRange = 10.0f;
            config.lifeBase = 25;
            config.lifeRange = 20;
            config.gravityBase = -0.97800004f;
            config.gravityRange = 0.263f;
            config.scaleBase = 0.096f;
            config.scaleRange = 0.07f;
            config.spin = 21.892f;
            particle_spawnDebrisBurst(&config, gDebrisModelIds, gDebrisModelScales,
                          6, 25.0f, 0, 0xFF, 1);
        }
    }
}

void func_15071D08(s32 cmd) {
    func_150F2230(gCurrentActor, 0xFF, 1);
}

/* Runs handler gAnimCommandProcs[gEventParam], passing it the command. */
void event_runAnimCommand(void) {
    void (*func)(s32);

    func = gAnimCommandProcs[gEventParam];
    if (func != NULL) {
        func(gEventParam);
    }
}

/* Runs the gAnimCommandProcs handler on the object event_getFirstOwnedObject returns. */
void event_runChildAnimCommand(void) {
    Actor *target;
    Actor *saved;

    target = event_getFirstOwnedObject(gCurrentActor);
    if (target != NULL) {
        saved = gCurrentActor;
        gCurrentActor = target;
        event_runAnimCommand();
        gCurrentActor = saved;
    }
}

void func_15071DC8(void) {
    effect_runActorEffects(gCurrentActor, gEventParam);
}

void func_15071DF4(void) {
    effect_runSurfaceHandler(gCurrentActor, gEventParam);
}

void event_setIntelAccel(void) {
    gCurrentActor->intel_accel = gEventParam;
}

void event_setIntelDecel(void) {
    gCurrentActor->intel_decel = gEventParam;
}

void func_15071E58(void) {
    gEventResult = gEventParam & 0xFF;
    gCurrentActor->intel_animseq = (gEventParam >> 8) & 0xFF;
    actor_playAnim(gCurrentActor, gCurrentActor->intel_animseq, 1.2f, 3.0f, gEventResult, 0.0f, 0);
}

void func_15071ED4(void) {
    actor_playAnim(gCurrentActor, 89, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_15071F14(void) {
    actor_playAnim(gCurrentActor, 36, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_15071F54(void) {
    actor_startDeathCountdown(gCurrentActor, gEventParam);
}

void func_15071F80(void) {
    if (gCutsceneStates[0] == 1) {
        gCurrentActor->speccnt++;
    }
}

void func_15071FB0(void) {
    if (gCurrentActor->shad == 0.0f) {
        gCurrentActor->unk10C = 0;
    }
}

/*
 * Launches the player (movement_obj[0]) with the current object's stored velocity unk2D8[0..2]
 * and an animation. With gEventParam == 0, only if player->stand_on_char names the object owned by the
 * current player (unk65), and the player is turned around.
 */
void event_launchPlayer(void) {
    Actor *player;
    Actor *entry;
    s32 found;
    s32 anim;
    s32 visible;
    s32 i;

    visible = 1;
    anim = 404;
    player = movement_obj;
    if (gEventParam == 0) {
        found = -1;
        entry = player;
        for (i = 0; i < 25; i++) {
            if (entry->attach_char == dc_glbl_CharNum + 1) {
                found = i;
                break;
            }
            entry++;
        }
        if (found + 1 != player->stand_on_char) {
            return;
        }
        player->angle = gCurrentActor->angle + 0x8000;
        player->unk1CC = -5000.0f;
    } else {
        if (gCurrentActor->taskint[1] >= 113) {
            anim = 741;
            gCurrentActor->unk2D8[2] = 0.0f;
        } else {
            anim = 50;
            player->angle = gCurrentActor->angle;
            if (gCurrentActor->unk251 != 0) {
                player->unk1CC = movement_obj[0].ypos + 800.0f;
            }
            visible = 0;
        }
        gCurrentActor->taskint[0] = 50;
    }
    player->yspd = gCurrentActor->unk2D8[0];
    player->ygrav = gCurrentActor->unk2D8[1];
    player->speed = gCurrentActor->unk2D8[2];
    player->count3 = 1;
    actor_playAnim(player, anim, 1.0f, 2.0f, 6.0f, 0.0f, 0);
    movement_obj[0].mv->prop = visible;
    if (player->CameraPtr != NULL) {
        player->CameraPtr->SpecialFlags |= 0x400;
    }
}

void func_150721A4(void) {
    u16 lo = gEventParam;
    u16 mid = gEventParam >> 8;
    u16 hi = gEventParam >> 16;

    actor_fadeCommand(gCurrentActor, hi, lo, mid, 0);
}

Actor *event_getFirstOwnedObject(Actor *owner) {
    return event_getOwnedObject(owner, 0);
}

/* Returns the n-th object owned by `owner` (unk65 = owner index + 1), or NULL. */
Actor *event_getOwnedObject(Actor *owner, s32 n) {
    s32 ownerId;
    s32 count;
    s32 i;

    ownerId = (owner - movement_obj) + 1;
    count = 0;
    for (i = 0; i < 25; i++) {
        if (ownerId == movement_obj[i].attach_char) {
            if (count == n) {
                return &movement_obj[i];
            }
            count++;
        }
    }
    return NULL;
}

void func_150722F0(void) {
    Actor *child = event_getFirstOwnedObject(gCurrentActor);

    if (child != NULL) {
        s32 hi = gEventParam >> 8;
        u16 lo = gEventParam;

        actor_fadeCommand(child, 2, hi, lo, 0);
    }
}

void func_1507233C(void) {
    gCurrentActor->unk94 = ~(gEventParam | 1);
}

void func_15072360(void) {
    gCurrentActor->unk94 &= ~gEventParam;
}

void func_15072388(void) {
    gCurrentActor->unk94 |= gEventParam;
}

void func_150723AC(void) {
    Actor *obj = collision_findObjectById(gEventParam);

    if (obj != NULL) {
        actor_remove(obj, 0);
    }
}

void func_150723E0(void) {
    Actor *obj = collision_findObjectById(gEventParam);

    if (obj != NULL && obj->attach_char != 0) {
        actor_remove(obj, 0);
    }
}

/* Respawns the object whose spawn record (gSpawnRecords, 0x30-byte SpawnRecord entries) gEventParam names. */
void event_respawnObject(void) {
    Actor *obj;
    s32 spawnFlags;
    s32 angle;
    s32 slot;
    s32 id;
    s32 noKill;

    angle = (gEventParam >> 8) & 0x7F;
    noKill = (gEventParam >> 15) & 1;
    spawnFlags = (gEventParam >> 16) & 0xFF;
    id = collision_findSpawnById(gEventParam);
    if (id == -1) {
        return;
    }
    gSpawnInUse[id] = 0;
    if (!(spawnFlags & 0x80)) {
        obj = actor_findOnPath(id);
        if (obj != NULL) {
            actor_remove(obj, 0);
            actor_clearSlot(obj);
        }
    }
    gSpawnRecords[id].unk2 = 0;
    slot = collision_spawnObject((SpawnRecord *)(id * 0x30 + (s32)gSpawnRecords), id, 0, 0, 0); // matching: integer address math
    if (slot == 0) {
        return;
    }
    obj = &movement_obj[slot - 1];
    if (obj == NULL) {
        return;
    }
    obj->attach_char = (gCurrentActor - movement_obj) + 1;
    obj->basemtx = angle;
    if (noKill) {
        obj->unk101 = 0;
    } else {
        obj->unk101 = 4;
    }
    obj->intelwalk = spawnFlags & 0x7F;
    obj->owner = dc_glbl_CharNum + 1;
    obj->xpos = gCurrentActor->xpos;
    obj->ypos = gCurrentActor->ypos;
    obj->zpos = gCurrentActor->zpos;
    if (gIsMultiplayer != 0) {
        if (gCurrentActor->character_type == 0x53) {
            gCurrentActor->unk222 = obj - movement_obj;
        }
    }
    if (level == 0x3F && obj->character_type == 0x16) {
        obj->unk94 |= 2;
    }
}

void func_1507266C(void) {
    u16 packed;
    s32 angle;
    Actor *obj;

    if (gIsMultiplayer == 0) {
        if (gCurrentActor->unk222 == 0) {
            return;
        }
    }
    packed = gEventParam;
    angle = gEventParam >> 16;
    obj = &movement_obj[gCurrentActor->unk222];
    obj->attach_char = dc_glbl_CharNum + 1;
    obj->basemtx = angle;
    obj->unk101 = 4;
    obj->stunned = 0;
    obj->imune = 0xFF;
    obj->scriptCmd = NULL;
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
    obj->intelwalk = packed;
    obj->unkEC = 1.0f;
    obj->unkF0 = 1.0f;
    obj->unkDC = 1.0f;
    obj->unkE0 = 1.0f;
    func_151B5BA0(&movement_obj[dc_glbl_CharNum], obj, angle);
}

void func_15072740(void) {
    s32 angle;
    Actor *obj;

    angle = gEventParam >> 16;
    obj = &movement_obj[gCurrentActor->unk222];
    obj->attach_char = dc_glbl_CharNum + 1;
    obj->basemtx = angle;
    obj->unk101 = 4;
}

void func_150727AC(void) {
    gCurrentActor->animsp = gEventParam * 0.1f;
    gCurrentActor->unk2D0->speed[0] = gCurrentActor->animsp;
}

void func_150727F0(void) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type != 0 && dc_glbl_CharNum + 1 == movement_obj[i].attach_char) {
            movement_obj[i].scriptCmd = NULL;
            movement_obj[i].intelwalk = gEventParam;
        }
    }
}

void func_15072918(void) {
    actor_remove(gCurrentActor, 0);
}

void func_15072940(void) {
    actor_remove(gCurrentActor, 1);
}

void func_15072968(void) {
    actor_playAnim(gCurrentActor, 44, 1.5f, 12.0f, 0.0f, 0.0f, 0);
    gCurrentActor->norun = 25;
}

void event_setTaskInt0(void) {
    gCurrentActor->taskint[0] = gEventParam;
}

void func_150729D0(void) {
    gCurrentActor->mv->pissed_up = 1;
    face_setDefaultExpression(gCurrentActor, 21);
    func_1506B82C();
}

void func_15072A14(void) {
    gCurrentActor->mv->idle.delay = 0;
    anim_updateIdle();
}

void func_15072A40(void) {
    gCurrentActor->mv->GrenadeThrowState = 0;
    gCurrentActor->mv->rt_context_b = 0;
    anim_updateIdle();
}

void func_15072A7C(void) {
    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->unk1A9++;
        if ((math_random() & 7) < gCurrentActor->mv->unk1A9) {
            actor_playAnim(gCurrentActor, 394, 1.0f, 4.0f, 0.0f, 0.0f, 0);
        }
    }
}

void func_15072AF8(void) {
    actor_playAnim(gCurrentActor, gCurrentActor->animseq.uh + 1, 1.0f, 6.0f, 0.0f, 0.0f, 0);
}

/* Picks the follow-up animation for the current animation unk84 and plays it. */
void event_playFollowUpAnim(void) {
    s32 state;
    s32 anim = -1;

    state = gCurrentActor->animseq.uh;
    switch (state) {
    case 0x26F:
    case 0x270:
    case 0x271:
    case 0x272:
    case 0x273:
        anim = state + 1;
        break;
    case 0x274:
        anim = 0x26F;
        break;
    case 0x1D6:
        anim = 0x1D7;
        break;
    case 0x1D7:
        anim = 0x1D8;
        break;
    case 0x175:
        anim = 0x177;
        break;
    case 0x176:
        anim = 0x177;
        break;
    case 0x35:
        anim = 0x3A;
        break;
    case 0x3F:
        anim = 0x3A;
        break;
    case 0x1CD:
        anim = 0x1CE;
        break;
    case 0x1CF:
        anim = 0x1CE;
        break;
    case 0xFD:
        anim = 0xFE;
        break;
    case 0x10A:
        anim = 0x10A;
        break;
    case 0x12C:
        anim = 0x12D;
        break;
    case 0x142:
        anim = 0x12D;
        break;
    case 0x138:
        anim = 0x3E;
        break;
    case 0x10C:
        if (gCurrentActor->mv->SniperState) {
            anim = 0x10A;
        } else {
            anim_updateIdle();
        }
        break;
    case 0x156:
        if (gCurrentActor->mv->SniperState) {
            anim = 0x157;
        } else {
            anim_updateIdle();
        }
        break;
    case 0x167:
        if (gCurrentActor->mv->SniperState) {
            anim = 0x165;
        } else {
            anim_updateIdle();
        }
        break;
    default:
        if (state == 0xBE || (state == 0xC1 && !(gCurrentActor->mv->old_keys & 0x2000))) {
            anim = 0xBF;
        }
        break;
    }
    if (anim != -1) {
        actor_playAnim(gCurrentActor, anim, 1.0f, 4.0f, 0.0f, 0.0f, 0);
    }
}

void func_15072DA0(void) {
    gCurrentActor->unk2F8 &= 0xFFF8;
    gCurrentActor->unk2F8 |= gEventParam;
}

void func_15072DD8(void) {
    collision_createPartSet(gCurrentActor, gEventParam, 1.0f, 0);

    if (gCurrentActor->mv != NULL && gCurrentActor->mv->weapon_status == 1) {
        gCurrentActor->mv->weapon_status = 2;
    }
}

void func_15072E38(void) {
    collision_removePartSet(gCurrentActor, gEventParam);

    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->weapon_status = 0;
    }
}

void func_15072E7C(void) {
    gCurrentActor->unk10C = gEventParam;
}

void func_15072E98(void) {
    gCurrentActor->unk2D0->speed[0] *= gEventParamF;
}

void func_15072EC0(void) {
    AnimInstance *anim;

    anim = gCurrentActor->unk2D0;
    if (anim->speed[0] < gEventParamF) {
        gCurrentActor->unk10C = 0;
    }
}

void event_setStunned(void) {
    gCurrentActor->stunned = gEventParam;
}

void func_15072F10(void) {
    f32 targetX;
    f32 targetZ;
    f32 targetY;
    s16 pitch;

    pitch = gCurrentActor->unk1D1 * 200;
    gCurrentActor->mv->gun_before_reload -= 1;
    actor_splitVelocity(gCurrentActor->langle - gCurrentActor->mv->pssd_waist_tilt, 2000.0f, pitch * 0.0054931640625f,
                  &targetX, &targetZ, &targetY);
    targetX = targetX * 5000.0f + gCurrentActor->xpos;
    targetZ = targetZ * 5000.0f + gCurrentActor->zpos;
    targetY = targetY * 5000.0f + gCurrentActor->ypos;
    collision_castRayAtObjects(gCurrentActor, gCurrentActor->xpos, gCurrentActor->ypos + gCurrentActor->unkD6,
                  gCurrentActor->zpos, targetX, targetY, targetZ, dc_glbl_CharNum, 18);
}

void func_1507304C(void) {
}

void func_15073054(void) {
    gCurrentActor->unk22E = gEventParam;
}

void func_15073070(void) {

}

void func_15073078(void) {
    camera_startEnvelope(0, gEventParam, 1);
}

void func_150730A4(void) {
    gCurrentActor->levelMode = 3;
    gCurrentActor->levelTimer = gEventParam;
}

void func_150730D0(void) {
    if (gEventParam == 1) {
        gCurrentActor->unk2F8 |= 0x100;
    } else {
        gCurrentActor->unk2F8 &= 0xFEFF;
    }
}

/*
 * Spawns a projectile (state 0x21) playing animation `anim`, at bone `bone` of `parent` offset by
 * (offX, offY, offZ), or at the parent's position. Returns the new object.
 */
Actor *event_spawnAnimProjectile(Actor *parent, s32 bone, s32 anim, f32 offX, f32 offY, f32 offZ, f32 yawOffset, f32 speed,
                     f32 yVelocity, f32 gravity, s32 unk83) {
    MtxF *mtx;
    MtxF mf;
    Actor *obj;
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    obj = actor_alloc();
    obj->ygrav = gravity;
    obj->yspd = yVelocity;
    obj->speed = speed;
    obj->count5 = unk83;
    if (parent->ob_matrix_ptr != NULL && bone != -1) {
        mtx = &parent->ob_matrix_ptr[bone];
        if (gBoneMatricesFlushed != 0) {
            guMtxL2F(mf, (Mtx *)mtx);
            mtx = &mf;
        }
        math_transformPoint(*mtx, offX, offY, offZ, &x, &y, &z);
    } else {
        x = parent->xpos;
        y = parent->ypos;
        z = parent->zpos;
    }
    obj->xpos = x;
    obj->ypos = y;
    obj->zpos = z;
    obj->angle = obj->langle = parent->langle + yawOffset * 182.04445f;
    obj->type = 0x21;
    obj->imune = 0xFF;
    obj->noshove = 2;
    if (anim != 0x42) {
        obj->noshove = 0x100E;
    }
    obj->unk66 = parent->unk66 & 0xC;
    obj->bodyRadius = 10;
    obj->bodyHeight = 10;
    obj->unkEC = 1.0f;
    obj->unkF0 = 1.0f;
    obj->unkDC = 1.0f;
    obj->unkE0 = 1.0f;
    obj->scale_xz = 0.5f;
    obj->scale_y = 0.5f;
    if (anim == 0x1F) {
        obj->unkC8 = 10.0f;
    }
    obj->headEffectState = anim;
    obj->drawtype = 5;
    obj->unique_id = 255U - (obj - movement_obj);
    collision_createPartSet(obj, anim, 1.0f, 0);
    for (i = 0; i <= gScreenLayout; i++) {
        obj->unk304[i] = light_allocGfxLights(1);
    }
    if (obj->unk304[i - 1] != 0) {
        obj->unk301 = 1;
    }
    obj->unk314 = func_1515D440();
    effect_attach(effect_newCharacterWatcher(obj, 1, 0), obj, 0x13);
    return obj;
}

/* Fires projectiles from the current object; gEventParam picks the volley (0: aimed at the camera target). */
void event_fireVolley(void) {
    Actor *obj;
    s32 rot;
    f32 y;
    f32 x;
    f32 pad[7]; // matching: unused stack space

    if (gEventParam == 0) {
        f32 dist;
        Camera *target;
        s32 hit;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 angle;
        f32 yaw;

        hit = 0;
        target = gCurrentActor->CameraPtr;
        if (aim_isPointInCone(gCurrentActor - movement_obj, 23, 23, target->eye.x, target->eye.y, target->eye.z,
                          &dist, 30.0f, 0, 0) != 0 &&
            math_random() % 1000 < 200) {
            hit = 1;
        }
        obj = event_spawnAnimProjectile(gCurrentActor, 23, 0x1F, 0.0f, 0.0f, 16.0f, 0.0f, 42.8f, 49.9f, 2.9f, 100);
        obj->noshove |= 0x01000000;
        if (hit != 0) {
            dx = target->eye.x - obj->xpos;
            dy = target->eye.y - obj->ypos;
            dz = target->eye.z - obj->zpos;
            angle = math_atan2(dx, dz) * 57.295776f;
            yaw = angle;
            if (angle > 180.0f) {
                yaw = angle - 360.0f;
            }
            angle = obj->speed * dy / sqrtf(dx * dx + dz * dz);
            obj->yspd = angle;
            obj->ygrav = 0.0f;
            obj->taskint[0] = 1;
            rot = (s16)(yaw * 182.04445f) - 0x4000;
            obj->langle = rot;
            obj->angle = rot;
        } else {
            obj->taskint[0] = 0;
        }
    } else if (gEventParam == 1) {
        f32 t;
        f32 spread;

        spread = gCurrentActor->taskint[0] * 0.01f;
        t = (s32)(math_random() % 40 - 20) * spread;
        y = 58.4f + t;
        t = (s32)(math_random() % 40 - 20) * spread;
        x = 42.8f + t;
        event_spawnAnimProjectile(gCurrentActor, 14, 0x42, 0.0f, 0.0f, 16.0f, (s32)(math_random() % 40 - 20) * spread, x, y, 2.9f, 100);
    } else if (gEventParam == 2) {
        obj = event_spawnAnimProjectile(gCurrentActor, 9, 0x35, 0.0f, 27.3f, 0.0f, 180.0f, 23.4f, 45.2f, 3.5f, 100);
        obj->vtilt = 180.0f;
        obj->noshove |= 0x01000000;
    } else if (gEventParam == 3) {
        f32 t;

        t = (s32)(math_random() % 40 - 20) * 10.0f;
        y = 58.4f + t;
        t = (s32)(math_random() % 40 - 20) * 10.0f;
        x = 42.8f + t;
        event_spawnAnimProjectile(gCurrentActor, 14, 0x88, 0.0f, 0.0f, 16.0f, (s32)(math_random() % 40 - 20) * 10.0f, x, y, 2.9f, 100);
    }
}

/* Releases the object held in unk13C (object index + 100). */
void event_releaseHeldObject(void) {
    u8 index = gCurrentActor->lifting - 100;

    if (gCurrentActor->lifting != 0) {
        gCurrentActor->lifting = 0;
        movement_obj[index].noshove = 0x8200;
        movement_obj[index].carried = 0;
        movement_obj[index].angle = gCurrentActor->langle;
        movement_obj[index].attach_char = 0;
        movement_obj[index].intelwalk = 6;
        movement_obj[index].scriptCmd = NULL;
        movement_obj[index].stunned = 0;
        movement_obj[index].langle = 0xE000;
        actor_playAnim(gCurrentActor, 151, 1.0f, 5.0f, 0.0f, 0.0f, 0);
    }
}

void func_150739A4(void) {
    gCurrentActor->unk64 = gEventParam;
}

void func_150739C0(void) {
    gCurrentActor->unk64 -= gFrameSteps;

    if (gCurrentActor->unk64 >= 0) {
        D_800D1880 = 0;
        gCurrentActor->speccnt--;
        gEventResult = gEventParam;
    }
}

void event_setStickSpeed(void) {
    gCurrentActor->stickspeed = gEventParam;
}

/* Applies actor_hitScripted (gEventParam & 0xFF00FF) to object unk124; bit 30 sets its unk1CC to our height. */
void func_15073A50(void) {
    Actor *parent;
    s32 flags;
    s32 index;

    parent = gCurrentActor;
    index = parent->owner;
    if (movement_obj[index].attach_char != 0) {
        flags = gEventParam;
        parent->lifting = 0;
        movement_obj[index].imune = 0;
        actor_hitScripted(&movement_obj[index], gEventParam & 0xFF00FF, 0, dc_glbl_CharNum);
        movement_obj[index].unk1CC = 100000.0f;
        if (flags & 0x40000000) {
            movement_obj[index].unk1CC = gCurrentActor->ypos;
        }
        movement_obj[index].imune = 20;
        movement_obj[index].angle = gCurrentActor->langle + flags;
    }
}

/* Unpacks gEventParam into mv->ropeSwingForce (low half, as f32), ropeSwingAngle and ropeSwingTimer (top bytes). */
void event_setRopeSwing(void) {
    s32 *param;
    Actor **objp;
    f32 low;
    f32 mid;

    param = &gEventParam; // matching: both globals are accessed through pointer locals
    objp = &gCurrentActor;
    low = *param & 0xFFFF;
    (*objp)->mv->ropeSwingForce = low;
    mid = (*param >> 16) & 0xFF;
    (*objp)->mv->ropeSwingAngle = mid;
    (*objp)->mv->ropeSwingTimer = (*param >> 24) & 0xFF;
}

void func_15073C28(void) {
    anim_updateIdle();
}

void func_15073C48(void) {

}

void func_15073C50(void) {
    gCurrentActor->unk224 = movement_obj[gCurrentActor->unk222].ypos + gEventParam;
}

void func_15073CB8(void) {
    Actor *obj = actor_findByType(0x53);

    if (obj != NULL && (u16)obj->intel_animseq == 0x1F) {
        obj->unk21C = 0;
    }
}

void func_15073CF4(void) {
    actor_setBodyRadius(gCurrentActor, gEventParam);
}

void func_15073D34(void) {
    actor_setBodyHeight(gCurrentActor, gEventParam);
}

void func_15073D74(void) {
    Actor *obj;

    obj = collision_findObjectById(gEventParam);
    if (obj != NULL) {
        obj->attach_char = 0;
    }
}

void func_15073DA4(void) {
    s32 kind;

    if (gEventParam != 0) {
        kind = 18;
    } else {
        kind = 2;
    }
    event_spawnProjectile(gCurrentActor->theta, 150.0f, 0, 0, 100, kind, 60.0f, 0.5f, NULL, 0, 1);
}

void func_15073E2C(void) {
    event_spawnProjectile(gCurrentActor->theta, 80.0f, 0, 0, 100, 11, 40.0f, 0.5f, NULL, 14, 1);
}

void func_15073EA4(void) {
    event_spawnProjectile(gCurrentActor->theta, 60.0f, 0, 0, 100, 12, 40.0f, 0.5f, NULL, 14, 1);
}

void func_15073F1C(void) {
    ModelAttachment *att = attach_find(gCurrentActor, 60, 0);

    if (att != NULL) {
        att->hookState = 2400;
    }
}

void func_15073F54(void) {
}

void func_15073F5C(void) {
    gCurrentActor->unk2CB = gEventParam;
}

void func_15073F78(void) {
    gCurrentActor->unk10B &= ~gEventParam;
}

/*
 * Spawns the object gEventParam & 0xFFFF as a thrown/fired item of the current object, at a bone
 * or above it, then sets its launch velocity and state (unk232) per thrower id.
 */
void event_spawnThrownItem(void) {
    s32 state;
    s32 idx;
    s32 useDefault;
    s32 arg;
    f32 speed;
    Actor *obj;
    s32 bone;
    s32 slot;
    s32 cmd;
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 y2;
    s32 rnd;
    s16 angle;
    Actor *player;

    arg = gEventParam & 0xFFFF;
    slot = collision_findSpawnById(arg);
    if (slot != -1) {
        state = 1;
        gSpawnInUse[slot] = 0;
        idx = collision_spawnObject((SpawnRecord *)(slot * 0x30 + (s32)gSpawnRecords), slot, 0, 0, 0); // matching: integer address math
        if (idx != 0) {
            idx--;
            obj = &movement_obj[idx];
            obj->owner = dc_glbl_CharNum + 1;
            obj->unique_id = dc_glbl_CharNum + 0x96;
            player = gCurrentActor;
            speed = 40.0f;
            useDefault = 1;
            if (player->ob_matrix_ptr != NULL) {
                useDefault = 0;
                switch (player->character_type) {
                case 0x75:
                case 0x80:
                    bone = 4;
                    break;
                case 0x5A:
                    bone = 19;
                    break;
                case 0x7B:
                    if (arg == 0x10) {
                        bone = (gEventParam >> 16) & 0xFF;
                    } else {
                        useDefault = 1;
                    }
                    break;
                default:
                    useDefault = 1;
                    break;
                }
                if (!useDefault) {
                    slot = (s32)&player->ob_matrix_ptr[bone];
                    math_transformOffset(NULL, (Vec3f *)&obj->xpos, slot); // xpos, ypos, zpos as one Vec3f
                    {
                        typedef s32 padA; // matching: a scope with a declaration steers as1 (-g3)
                        player = gCurrentActor;
                    }
                }
            }
            if (useDefault) {
                obj->xpos = player->xpos;
                obj->ypos = player->ypos + 100.0f;
                obj->zpos = player->zpos;
            }
            if (gIsMultiplayer != 0) {
                obj->taskint[0] = player->mv->GrenadeTimer;
                state = 5;
                player->mv->GrenadeThrowState = 0;
                gCurrentActor->mv->rt_context_b = 0;
                cmd = (gEventParam >> 16) & 1;
                obj->ygrav = 4.0; // matching: double literal
                player = gCurrentActor;
                speed = ((0xF0 - player->mv->GrenadeTimer) * 60.0f) / 240.0f;
                speed += 40.0f;
                if (cmd) {
                    speed = 0.0f;
                }
                if (player->mv->AimState != 0) {
                    {
                        typedef s32 padT; // matching: as above
                        actor_splitVelocity((u16)(s32)((45.0f - player->mv->aimPitch) * 182.04445f) & 0xFFFF, speed * 4.0f, 0.0f, &x1, &y1, &z1);
                        obj->yspd = -y1;
                    }
                    player = gCurrentActor;
                } else {
                    obj->yspd = 50.0f;
                }
                if (player->character_type == 0x75) {
                    slot = (s32)attach_find(player, 0x64, 0); // matching: slot is reused for the attachment
                } else {
                    slot = (s32)attach_find(player, 0x2C, 0);
                }
                if (slot != 0) {
                    obj->soundHandle2 = ((ModelAttachment *)slot)->hookState2;
                    sound_replaceCallback((SoundRecordCallback)attach_soundCallback, (void *)slot, (s32)gCurrentActor, sound_followOwnerCallback, obj, obj->unique_id);
                }
                player = gCurrentActor;
            }
            angle = player->langle;
            if (player->mv != NULL) {
                angle -= player->mv->pssd_waist_tilt;
            }
            obj->langle = angle;
            obj->angle = angle;
            obj->speed = speed;
            obj->unk1CC = obj->ypos;
            if (player->character_type == 0x4A) {
                state = 5;
                obj->speed = 0.0f;
                trail_spawnActorBoneTrail(obj, 0xFF);
                gSpawnPaths[obj->listpos][1].x = movement_obj[0].xpos;
                gSpawnPaths[obj->listpos][1].z = movement_obj[0].zpos;
            } else if (player->character_type == 0x7B) {
                if (arg == 0x10) {
                    state = 7;
                    gSpawnPaths[obj->listpos][1].x = movement_obj[0].xpos;
                    gSpawnPaths[obj->listpos][1].y = 980;
                    gSpawnPaths[obj->listpos][1].z = movement_obj[0].zpos;
                    obj->unique_id = 0xFF - idx;
                    actor_runHitHandler(obj, 0xC, NULL);
                    beam_spawnMuzzleBurst(gCurrentActor, math_random() & 1, math_random() & 3, 0xFF, 1);
                } else if (arg == 0xA) {
                    state = 0xE;
                    obj->ypos = player->ypos;
                } else {
                    state = 4;
                    obj->unique_id = 0x12;
                    obj->ypos = 1040.0f;
                    obj->speed = 0.0f;
                }
            } else if (player->character_type == 0x9A) {
                rnd = (math_random() & 0xFFF) - 0x800;
                speed = (s16)(math_random() % 800U) * 0.01f;
                state = 7;
                actor_splitVelocity(gCurrentActor->angle + rnd, 30.0f - speed, 0.0f, &x2, &z2, &y2);
                obj->xpos -= x2;
                obj->zpos -= z2;
                player = gCurrentActor;
                obj->speed = (player->speed * 1.35f) + 6.0f;
                obj->yspd = player->yspd;
                obj->imune = 0xFF;
                obj->ypos = player->ypos - 40.0f;
                obj->unk222 = player->unk222;
            }
            obj->intelwalk = state;
            obj->energy |= 0x80;
        }
    }
}

void event_setWeaponStatus(void) {
    gCurrentActor->mv->weapon_status = gEventParam;
}

void func_15074664(void) {
    if (gCurrentActor->mv != NULL) {
        if (gEventParam == 1) {
            audio_setMixFallback(5);
        } else if (gCurrentActor->mv->unk94 == 1) {
            audio_setMixFallback(0);
        }
        gCurrentActor->mv->unk94 = gEventParam;
    }
}

/* Deals gEventParam damage to the current object (or, while it holds one, to the player). */
void event_dealDamage(void) {
    Actor *target;
    s32 damage;
    ActorCtl *ext;
    Camera *camera;

    target = NULL;
    if (gIsMultiplayer != 0) {
        target = gCurrentActor;
    } else if (gCurrentActor->lifting != 0) {
        target = movement_obj;
    }
    if (target != NULL) {
        damage = gEventParam;
        if (damage >= target->energy) {
            target->energy = 0;
            gCurrentActor->energy = 0;
            if (target->type == 1) {
                target->imune = 0xFF;
                actor_kill(target, 6);
            }
        } else {
            ext = target->mv;
            target->energy = target->energy - damage;
            if (ext != NULL) {
                camera = target->CameraPtr;
                if (camera != NULL) {
                    if (ext->SniperState != 0) {
                        screen_startViewFlash(0xFF, 0, 0, 180, 20, camera->CNumber);
                    }
                }
            }
        }
    }
}
/* Sets the state (unk232) of the current object's owner (unk65 = index + 1). */
void event_setOwnerState(void) {
    s32 i;
    s32 owner;

    owner = gCurrentActor->attach_char;
    i = owner;
    if (i != 0) {
        i = i - 1;
        movement_obj[i].intelwalk = gEventParam;
        movement_obj[i].scriptCmd = NULL;
    }
}

void event_addShotsFired(void) {
    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->shotsfired += gEventParam;
    }
}

void func_15074870(void) {
    gCurrentActor->unk24F = gEventParam;
}

/* Adds the top byte to unk138 when bit `mask` of word taskint[index] matches the low bit's sense. */
void func_1507488C(void) {
    u8 index;
    u8 mask;
    u8 enabled;

    index = gEventParam >> 16;
    mask = gEventParam >> 8;
    enabled = gEventParam & 1;
    if ((&gCurrentActor->taskint[0])[index] & mask) {
        enabled ^= 1;
    }
    if (enabled != 0) {
        gCurrentActor->speccnt += (u8)(gEventParam >> 24);
    }
}

void func_150748F4(void) {
    movement_obj[0].ivehitit = gEventParam;
}

void func_1507490C(void) {
    u8 index = gCurrentActor->lifting - 100;

    if (gCurrentActor->lifting != 0) {
        if (movement_obj[index].carried >= 100) {
            movement_obj[index].intelwalk = gEventParam;
            movement_obj[index].scriptCmd = NULL;
            movement_obj[index].stunned = 0;
        }
    }
}

/* Makes every object within gEventParam * 8 units start chasing (mv->chasing = 20). */
void event_startChasing(void) {
    s32 i;
    ActorCtl *ext;
    f32 dist = gEventParam * 8;

    for (i = 0; i < D_8008FD8C; i++) {
        if (actor_distance3D(gCurrentActor, &movement_obj[i]) < dist) {
            ext = movement_obj[i].mv;
            if (ext != NULL) {
                ext->chasing = 20;
            }
        }
    }
}

void event_setBreakStun(void) {
    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->break_stun = gEventParam;
    }
}

void func_15074A6C(void) {
    if (gCurrentActor->lifting == 0) {
        gCurrentActor->speccnt++;
    }
}

/* Scales the object by distance to the player: 0.3 within 200, unk148 beyond 1500, lerped between. */
void event_scaleByPlayerDistance(void) {
    f32 dist;
    f32 maxScale;
    f32 scale;
    f32 range;

    dist = actor_distance3D(movement_obj, gCurrentActor);
    if (gCurrentActor->unk148 < 0.2f) {
        gCurrentActor->unk148 = gCurrentActor->unk154;
    }
    if (dist < 200.0f) {
        scale = 0.3f;
    } else {
        maxScale = gCurrentActor->unk148;
        if (1500.0f < dist) {
            scale = maxScale;
        } else {
            range = maxScale - 0.3f;
            maxScale = (dist - 200.0f) / 1300.0f; // matching: reusing maxScale for the fraction gives IDO's register split
            scale = 0.3f + maxScale * range;
        }
    }
    gCurrentActor->unk158 = scale;
    gCurrentActor->unk154 = gCurrentActor->unk158;
    gCurrentActor->unk15C = 0.02f;
}

void func_15074B7C(void) {
    D_800D1880 = 0;
    gEventResult = gEventParam;

    if (gEventParam == 0) {
        gEventResult = 0.1f;
        gCurrentActor->unk1FC |= 4;
        gCurrentActor->speccnt = 0;
    }
}

void func_15074BD8(Actor *self, Actor *other, u8 *result) {
}

void func_15074BEC(Actor *self, Actor *other, u8 *result) {
}

/* Collision handlers (self, other, result): */

void func_15074C00(Actor *self, Actor *other, u8 *result) {
    GroundPoint hit;

    if (((u8)other->intelhitroute & 0x7F) == 5) {
        collision_initHitFromObject(&hit.ground, other);
        hit.pos.x = gHitPointX;
        hit.pos.y = gHitPointY;
        hit.pos.z = gHitPointZ;
        prop_scatterPieces(&hit.pos, &hit.ground, 22, 1, 0xFF, 0);
    }
}

void func_15074C80(Actor *self, Actor *other) {
    s32 anim;

    if (self->headEffectState == 17) {
        effect_hitReactSmallSplash(self, other);
    }

    if (self->headEffectState == 2 || self->headEffectState == 18 || self->headEffectState == 3 || self->headEffectState == 6) {
        if (other->energy == 0) {
            if (self->headEffectState == 2) {
                anim = 0x2A;
            } else if (self->headEffectState == 18) {
                anim = 0x2F;
            } else {
                if (self->headEffectState == 3) {
                    anim = 0x39;
                } else {
                    anim = 0x51;
                }
            }
            attach_attachInPlace(self, other, anim, 12, 1, 0);
        }
        self->energy = 0;
    }

    if (other->type == 0x19) {
        effect_spawnRotatedQuadEffect(256.0f - (math_atan2(self->yspd, self->speed) * 40.743664f),
                      0xC0 - (self->angle >> 8),
                      self->xpos,
                      self->ypos,
                      self->zpos,
                      16,
                      20,
                      0,
                      0xFF,
                      0);
    }
}

void func_15074DEC(Actor *self, Actor *other, u8 *result) {
    self->taskint[1] = 1;
}

void func_15074E04(Actor *self, Actor *other, u8 *result) {
    func_1516FE1C(other - movement_obj, 180, 0xFF, 0);
    particle_startActorEmitter(other, 11, 0, 1, 0xFF, 0, "mvmt_imp_set_up_smoke");
}

void func_15074E80(Actor *self, Actor *other, u8 *result) {
    u8 hitResult = 0;

    func_15194794(self, other, &hitResult);
    if (other->type == 1) {
        self->imune = 0xFF;
        other->imune = 200;
        gEventParam = 0x60000;
        func_15072740();
    }
}

void func_15074EE8(Actor *self, Actor *other, u8 *result) {
    if (other->type == 1) {
        self->imune = 0xFF;
        other->imune = 200;
        gEventParam = 0x70000;
        func_15072740();
    }
}

void func_15074F30(Actor *self, Actor *other, u8 *result) {
    self->intelwalk = other->unk109;
    self->scriptCmd = NULL;
}

/* Sends `other` towards (-2641, 3039). */
void func_15074F48(Actor *self, Actor *other, u8 *result) {
    f32 dx = -2641.0f - self->xpos;
    f32 dz = self->zpos - 3039.0f;

    other->angle = actor_getDirectionAngle(dx, dz, 0);
    other->speed = sqrtf(dx * dx + dz * dz) * 0.03f;
    func_15194408(self, other);
}

void func_15074FD4(Actor *self, Actor *other, u8 *result) {
    if (other->type == 1) {
        self->imune = 0xFF;
        other->imune = 200;
        gEventParam = 0x60000;
        if (self->character_type == 0x8E) {
            gEventParam = 0x140000;
        }
        func_15072740();
    }
    effect_hitReactSplash(self, other, result);
}

void func_15075050(Actor *self, Actor *other, u8 *result) {
    self->vtilt = 5.0f;
    if (self->speed > 0.0f) {
        self->vtilt = -5.0f;
    }
    self->ygrav = 5.0f;
    self->yspd = 12.0f;
}

void func_150750A4(Actor *self, Actor *other, u8 *result) {
    self->ygrav = 5.0f;
}

void func_150750C4(Actor *self, Actor *other, u8 *result) {
    if (other->character_type == 0x88 && self->ivehitit < 55) {
        self->speccnt += 3;
    }
    func_15194794(self, other, result);
    if (other->character_type != 0x53) {
        func_15145A50(other);
        other->energy = 0;
        actor_kill(other, 8);
        *result = 39;
    }
}

/* Returns the first live object owned by `owner` (unk65 = owner index + 1), or NULL. */
Actor *event_getFirstLiveOwnedObject(Actor *owner) {
    Actor *obj;
    s32 i;

    obj = movement_obj;
    for (i = 0; i < 26; i++) {
        if (obj->type != 0 && obj->character_type != 0xFF && obj->attach_char == (owner - movement_obj) + 1) {
            return obj;
        }
        obj++;
    }
    return NULL;
}

