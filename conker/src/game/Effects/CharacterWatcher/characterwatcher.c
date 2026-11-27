#include <ultra64.h>
#include <math.h>
#include "conker/beam.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/trail.h"

/*
 * Split from game/Effects/EffectActor/effectactor.c at effect_newCharacterWatcher (16-aligned): game/Effects/EffectActor/effectactor's .rodata (the 1/255 literal at
 * 0x24AB30) ends padded where these vertices start, and its .data (the identity matrix of
 * effect_buildEffectActorScaleMtx) ends padded at 0x22F920, where these display lists start.
 */

/* Creates a watcher for chr; returns NULL when chr is NULL or has no watchable mode. */
CharacterWatcher *effect_newCharacterWatcher(Actor *chr, u8 enableActions, s32 extraSize) {
    CharacterWatcher *watcher;

    if (chr == NULL) {
        return NULL;
    }
    watcher = effect_newObject(0x2E, 0, extraSize + 0x58, 1, 0xFF, 1);
    if (watcher == NULL) {
        return NULL;
    }
    watcher->character = chr;
    watcher->characterId = chr->unique_id;
    watcher->state0 = 2;
    watcher->state1 = 2;
    watcher->state2 = 2;
    watcher->moveState = 3;
    watcher->surfaceState = 0;
    watcher->lastPosition.x = chr->xpos;
    watcher->lastPosition.y = chr->ypos;
    watcher->lastPosition.z = chr->zpos;
    watcher->flags = 0;
    if (enableActions != 0) {
        watcher->flags |= 1;
    }
    watcher->unk10 = 1;
    watcher->unk14 = 0;
    if (effect_pickWatcherMode(chr, &watcher->mode) == 0) {
        effect_freeObject(&watcher->header);
        return NULL;
    }
    watcher->hit.flags = 0;
    return watcher;
}

void effect_destroyCharacterWatcher(CharacterWatcher *watcher) {
    effect_detachHandle(watcher, watcher->character);
    effect_defaultDestroy(&watcher->header);
}

void effect_freeCharacterWatcher(CharacterWatcher *watcher) {
    effect_detachHandle(watcher, watcher->character);
    effect_defaultFree(&watcher->header);
}

/* A state-change rule: both masks are unions of STATE_BITS values. */
struct StateRule {
    s32 from;
    s32 to;
};

/* Packs the five state fields into one bitfield, three bits per field. */
#define STATE_BITS(a, b, c, d, e)                                              \
    ((1 << (a)) | (1 << ((b) + 3)) | (1 << ((c) + 6)) | (1 << ((d) + 9)) |     \
     (1 << ((e) + 13)))

/* Vertices of the effect display lists (D_8008AE60.., drawn through gEffectActorDisplayLists). */
static const Vtx D_800A6080[5] = {
    { { { 0, -127, -255 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0x00, 0xFF } } },
    { { { 0, 25, 0 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0x00, 0xFF } } },
    { { { -22, -12, 0 }, 0, { 0, 0 }, { 0xA0, 0x50, 0x14, 0xFF } } },
    { { { 22, -12, 0 }, 0, { 0, 0 }, { 0x64, 0x82, 0x12, 0xFF } } },
    { { { 0, 51, 255 }, 0, { 0, 0 }, { 0x80, 0x40, 0x00, 0xFF } } },
};
static const Vtx D_800A60D0[4] = {
    { { { 0, 25, -255 }, 0, { 0, 0 }, { 0xC8, 0xAA, 0x14, 0xFF } } },
    { { { -22, -12, -255 }, 0, { 0, 0 }, { 0x80, 0x20, 0x08, 0xFF } } },
    { { { 22, -12, -255 }, 0, { 0, 0 }, { 0x64, 0x64, 0x00, 0xFF } } },
    { { { 0, 0, 255 }, 0, { 0, 0 }, { 0x80, 0x40, 0x00, 0xFF } } },
};
static const Vtx D_800A6110[6] = {
    { { { 0, 255, 0 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x00, 0x40 } } },
    { { { 255, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x00, 0x40 } } },
    { { { 0, -255, 0 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x00, 0x40 } } },
    { { { -255, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x00, 0x40 } } },
    { { { 0, 0, 255 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x00, 0x40 } } },
    { { { 0, 0, -255 }, 0, { 0, 0 }, { 0xFF, 0x00, 0x00, 0x40 } } },
};
static const Vtx D_800A6170[5] = {
    { { { 0, -127, -255 }, 0, { 0, 0 }, { 0xFC, 0x52, 0x05, 0xFF } } },
    { { { 0, 25, 0 }, 0, { 0, 0 }, { 0xFE, 0x8D, 0x30, 0xFF } } },
    { { { -22, -12, 0 }, 0, { 0, 0 }, { 0xF0, 0x62, 0x1C, 0xFF } } },
    { { { 22, -12, 0 }, 0, { 0, 0 }, { 0xF9, 0x5B, 0x14, 0xFF } } },
    { { { 0, 51, 255 }, 0, { 0, 0 }, { 0x85, 0x29, 0x00, 0xFF } } },
};
static const Vtx D_800A61C0[4] = {
    { { { 0, 25, -255 }, 0, { 0, 0 }, { 0xF8, 0x58, 0x14, 0xFF } } },
    { { { -22, -12, -255 }, 0, { 0, 0 }, { 0xC6, 0x3E, 0x03, 0xFF } } },
    { { { 22, -12, -255 }, 0, { 0, 0 }, { 0xFE, 0x8F, 0x3D, 0xFF } } },
    { { { 0, 0, 255 }, 0, { 0, 0 }, { 0xFC, 0x52, 0x05, 0xFF } } },
};

/* The effect actor display lists (EffectActorTemplate.displayList, drawn by effect_drawEffectActor of game/Effects/EffectActor/effectactor). */
static Gfx D_8008AE60[] = {
    gsDPPipeSync(),
    gsSPVertex(D_800A6080, 5, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsSP1Triangle(0, 3, 1, 0),
    gsSP1Triangle(1, 4, 2, 0),
    gsSP1Triangle(2, 4, 3, 0),
    gsSP1Triangle(3, 4, 1, 0),
    gsSPEndDisplayList(),
};
static Gfx D_8008AEA8[] = {
    gsDPPipeSync(),
    gsSPVertex(D_800A60D0, 4, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 3, 1, 0),
    gsSP1Triangle(1, 3, 2, 0),
    gsSP1Triangle(2, 3, 0, 0),
    gsSPEndDisplayList(),
};
static Gfx D_8008AEE0[] = {
    gsDPPipeSync(),
    gsSPVertex(D_800A6110, 6, 0),
    gsSP1Triangle(0, 1, 5, 0),
    gsSP1Triangle(0, 5, 3, 0),
    gsSP1Triangle(0, 3, 4, 0),
    gsSP1Triangle(0, 4, 1, 0),
    gsSP1Triangle(5, 1, 2, 0),
    gsSP1Triangle(3, 5, 2, 0),
    gsSP1Triangle(4, 3, 2, 0),
    gsSP1Triangle(1, 4, 2, 0),
    gsSPEndDisplayList(),
};
static Gfx D_8008AF38[] = {
    gsDPPipeSync(),
    gsSPVertex(D_800A6170, 5, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsSP1Triangle(0, 3, 1, 0),
    gsSP1Triangle(1, 4, 2, 0),
    gsSP1Triangle(2, 4, 3, 0),
    gsSP1Triangle(3, 4, 1, 0),
    gsSPEndDisplayList(),
};
static Gfx D_8008AF80[] = {
    gsDPPipeSync(),
    gsSPVertex(D_800A61C0, 4, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 3, 1, 0),
    gsSP1Triangle(1, 3, 2, 0),
    gsSP1Triangle(2, 3, 0, 0),
    gsSPEndDisplayList(),
};
Gfx *gEffectActorDisplayLists[] = { D_8008AE60, D_8008AEA8, D_8008AF38, D_8008AF80, D_8008AEE0, NULL };

/* Transition rules of effect_updateCharacterWatcher, one per action slot of gWatcherModeActions. */
static const StateRule gWatcherStateRules[28] = {
    { 0x21FFA, 0x3FFC9 },
    { 0x3FFFF, 0x3FFC9 },
    { 0x21FFA, 0x3FFD1 },
    { 0x3FFFF, 0x3FFD1 },
    { 0x5FFA, 0x3FFC9 },
    { 0x5FFA, 0x3FFD1 },
    { 0x11FFA, 0x3FFC9 },
    { 0x11FFA, 0x3FFD1 },
    { 0x3FFC9, 0x3FFFA },
    { 0x3FFD1, 0x3FFFA },
    { 0x3FF7F, 0x3FEB9 },
    { 0x3FEB9, 0x3FF7F },
    { 0x3FFFF, 0x20DC9 },
    { 0x3FFFF, 0x20289 },
    { 0x3FFFF, 0x3E291 },
    { 0x3FFFF, 0x8C91 },
    { 0x3FFFF, 0x3E5C9 },
    { 0x3FFFF, 0x3E491 },
    { 0x3FFFF, 0x3E451 },
    { 0x3FFFF, 0x3FE51 },
    { 0x3FFFF, 0x3E851 },
    { 0x3FFFF, 0x3E251 },
    { 0x3ECB9, 0x3E2B9 },
    { 0x3E2B9, 0x3ECB9 },
    { 0x3FFFF, 0x3E2B9 },
    { 0x2BEB9, 0x3FE51 },
    { 0x3FFF9, 0x3FFFA },
    { 0x3FFD1, 0x3FFEF },
};

/* Per-mode action tables (gWatcherModeActions): an index into gWatcherActions per StateRule, -1 = none. */
static const s8 gWatcherMode0Actions[28] = {
    0, 8, 1, -1, 2, 3, 4, 5, 6, 7, 9, 10, -1, -1,
    -1, -1, -1, -1, -1, 13, -1, -1, 11, 12, 14, 15, 16, 21,
};
static const s8 gWatcherMode1Actions[28] = {
    22, -1, 22, -1, 22, 22, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};
static const s8 gWatcherMode2Actions[28] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};
static const s8 gWatcherMode3Actions[28] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, 18, -1, -1, -1, -1, 17, -1, -1, -1,
};
static const s8 gWatcherMode4Actions[28] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

/* Character-watcher actions (indices of the action tables). Declared without a prototype: each takes
   its own task view of the watcher. */
static void (*gWatcherActions[])() = {
    effect_watcherStepIntoShallowWater, effect_watcherStepIntoDeepWater, effect_watcherFallIntoShallowWater, effect_watcherFallIntoDeepWater, func_151A9DC0, func_151A9EC0,
    effect_watcherLeaveShallowWater, effect_watcherLeaveDeepWater, effect_watcherNoAction, effect_watcherStartSurfaceTrails1, effect_watcherReleaseSurfaceTrails1, effect_watcherReleaseSurfaceTrails2,
    effect_watcherStartSurfaceTrails2, effect_refreshTaskEffects, effect_keepTaskFlicker, effect_watcherSubmerge, effect_watcherStartFootSplashes, particle_keepFlickerSprite,
    particle_keepWatcherEmitter, effect_keepWatcherFloorSprite, effect_keepWatcherEmitter, effect_startPlayerDripEmitter, effect_watcherEnterWater,
};

/* The action table of each watcher mode. */
static const s8 *gWatcherModeActions[] = { gWatcherMode0Actions, gWatcherMode1Actions, gWatcherMode2Actions, gWatcherMode3Actions, gWatcherMode4Actions };

/* Update: recomputes the character's state and fires every rule matching the old -> new transition. */
void effect_updateCharacterWatcher(CharacterWatcher *watcher) {
    s32 oldBits;
    s8 action;
    u8 oldState0;
    u8 oldState1;
    u8 oldState2;
    u8 oldMoveState;
    u8 oldSurfaceState;
    s32 newBits;
    s32 i;

    oldState0 = watcher->state0;
    oldState1 = watcher->state1;
    oldState2 = watcher->state2;
    oldMoveState = watcher->moveState;
    oldSurfaceState = watcher->surfaceState;

    watcher->state0 = effect_watcherIsOutOfWater(watcher->character, watcher->mode);
    if (watcher->state0 == 0) {
        watcher->state1 = effect_watcherIsDeepWater(watcher->character, watcher->mode);
    } else {
        watcher->state1 = 2;
    }

    if (watcher->state0 == 0) {
        if (watcher->state1 == 0) {
            watcher->state2 = 1;
        } else {
            watcher->state2 = effect_watcherIsNearWaterSurface(watcher->character, watcher->mode);
        }
    } else {
        watcher->state2 = 0;
    }

    watcher->moveState = effect_getWatcherMoveState(watcher->character, &watcher->lastPosition, watcher->moveState);
    watcher->lastPosition.x = watcher->character->xpos;
    watcher->lastPosition.y = watcher->character->ypos;
    watcher->lastPosition.z = watcher->character->zpos;
    watcher->surfaceState = effect_getWatcherSurfaceState(watcher->character, watcher->mode);

    if (watcher->flags & 1) {
        newBits = STATE_BITS(watcher->state0, watcher->state1, watcher->state2, watcher->moveState,
                             watcher->surfaceState);
        for (i = 0; i < 28; i++) {
            action = gWatcherModeActions[watcher->mode][i];
            if (action != -1) {
                oldBits = STATE_BITS(oldState0, oldState1, oldState2, oldMoveState, oldSurfaceState);
                if ((gWatcherStateRules[i].from | oldBits) == gWatcherStateRules[i].from) {
                    if ((gWatcherStateRules[i].to | newBits) == gWatcherStateRules[i].to) {
                        gWatcherActions[action](watcher);
                    }
                }
            }
        }
    }
}

/* Message handler: frees the watcher when its character goes away (0), follows swaps (0x2D). */
void effect_characterWatcherMessage(void *self, ActorMsg *msg, u8 msgId) {
    CharacterWatcher *watcher = self;
    Actor *first;

    if (msgId == 0) {
        if ((msg->ref.actor == watcher->character) || (msg->ref.uniqueId == watcher->characterId)) {
            effect_destroyObject(self);
        }
    } else if (msgId == 0x2D) {
        first = msg->swap.actorA;
        if (watcher->character == first) {
            watcher->character = msg->swap.actorB;
            watcher->characterId = msg->swap.idB;
        } else {
            if (watcher->character == msg->swap.actorB) {
                watcher->character = first;
                watcher->characterId = msg->swap.idA;
            }
        dummy_label_15158FA4: ; // matching: the empty labelled statement splits IDO's basic block
        }
    } else if (msgId == 4) {
        if ((msg->ref.actor == watcher->character) || (msg->ref.uniqueId == watcher->characterId)) {
            trail_releaseSurfaceTrails(self);
        }
    }
}

s32 effect_watcherIsOutOfWater(Actor *chr, u8 mode) {
    s32 ret;
    s32 flags;

    flags = chr->floorMaterial & 0x1F;
    if ((mode == 2) || (mode == 3)) {
        ret = 0;
    } else {
        if ((-10000.0f == chr->waterHeight) && ((flags & 0xA) == 0)) {
            ret = 1;
        } else if ((chr->waterHeight < chr->ypos) || (chr->unk137 != 0)) { /* byte 0x137 */
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return ret;
}

s32 effect_watcherIsDeepWater(Actor *chr, u8 mode) {
    s32 ret;

    if ((mode == 2) || (mode == 3)) {
        ret = 1;
    } else if (chr->floorY < (chr->waterHeight - 35.0f)) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

s32 effect_watcherIsNearWaterSurface(Actor *chr, u8 mode) {
    s32 ret;

    if ((mode == 2) || (mode == 3)) {
        if (gCutsceneStates[0] != 1) {
            ret = 0;
        } else if ((chr->waterHeight - 75.0f) < chr->ypos) {
            ret = 1;
        } else {
            ret = 0;
        }
    } else if ((chr->waterHeight - 75.0f) < chr->ypos) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

/* Returns 0 if chr has not moved since lastPosition, 1 if it started moving, 2 if it kept moving. */
s32 effect_getWatcherMoveState(Actor *chr, Vec3f *lastPosition, u8 prevState) {
    s32 ret;

    if ((chr->xpos != lastPosition->x) || (chr->ypos != lastPosition->y) ||
        (chr->zpos != lastPosition->z)) {
        if ((prevState == 1) || (prevState == 2)) {
            ret = 2;
        } else {
            ret = 1;
        }
    } else {
        ret = 0;
    }
    return ret;
}

/* Returns 3 on one of two animation ids (mode 0 only), 2 in water, 1 if unk28 is set, else 4. */
s32 effect_getWatcherSurfaceState(Actor *chr, u8 mode) {
    s32 i;
    s32 found;

    if (mode == 0) {
        u8 ids[2] = { 0x3B, 0x3C };

        found = 0;
        i = 1;
        do {
            if (chr->animseq.uh == ids[i--]) {
                found = 1;
            }
        } while ((found == 0) && (i >= 0));
    } else {
        found = 0;
    }
    if (found != 0) {
        i = 3;
    } else if (chr->inwater != 0) {
        i = 2;
    } else if (chr->shad != 0.0f) {
        i = 1;
    } else {
        i = 4;
    }
    return i;
}

/* Picks the watcher mode for chr; returns 0 if nothing applies. */
s32 effect_pickWatcherMode(Actor *chr, u8 *mode) {
    s32 ret;

    ret = 0;
    if (gIsMultiplayer != 0) {
        *mode = 0;
        ret = 1;
    }
    if (chr->unique_id == 1) {
        *mode = 0;
        ret = 1;
    }
    if ((chr->drawtype == 5) || (chr->drawtype == 2)) {
        *mode = 1;
        ret = 1;
    }
    switch (chr->character_type) {
    case 8:
        *mode = 2;
        ret = 1;
        break;
    case 10:
        *mode = 3;
        ret = 1;
        break;
    case 0x29:
    case 0x2A:
        *mode = 4;
        ret = 1;
        break;
    }
    return ret;
}


/*
 * Returns whether chr's watcher state matches the given field values (-1 matches anything).
 * chr's watcher is its type-0x13 attachment.
 */
u8 effect_matchWatcherState(Actor *chr, s16 state0, s16 state1, s16 state2, s16 moveState, s16 surfaceState) {
    CharacterWatcher *watcher;
    Attachment *node;
    u8 ret;
    s32 mask0;
    s32 mask1;
    s32 mask2;
    s32 maskMove;
    s32 maskSurface;
    s32 want;
    s32 have;

    ret = 0;
    if (effect_findAttachmentByType(chr->attachments, 0x13, &node) == 0) {
        return 0;
    }
    watcher = node->handle; // attachment type 0x13 holds the character's watcher
    if (moveState == -1) {
        maskMove = 0x1E00;
    } else {
        maskMove = 1 << (moveState + 9);
    }
    if (state2 == -1) {
        mask2 = 0x1C0;
    } else {
        mask2 = 1 << (state2 + 6);
    }
    if (state1 == -1) {
        mask1 = 0x38;
    } else {
        mask1 = 1 << (state1 + 3);
    }
    if (state0 == -1) {
        mask0 = 7;
    } else {
        mask0 = 1 << state0;
    }
    if (surfaceState == -1) {
        maskSurface = 0x3E000;
    } else {
        maskSurface = 1 << (surfaceState + 13);
    }
    have = STATE_BITS(watcher->state0, watcher->state1, watcher->state2, watcher->moveState,
                      watcher->surfaceState);
    want = maskSurface | mask0 | mask1 | mask2 | maskMove;
    if (want == (want | have)) {
        ret = 1;
    }
    return ret;
}

/* Particle data at 0xA8 for particles oscillating along a direction (effect_updateOscillateParticle). */
typedef struct {
    /* 0x00 */ f32 baseSize;
    /* 0x04 */ u8 widthPhase;
    /* 0x05 */ u8 heightPhase;
    /* 0x06 */ s8 widthPhaseStep;
    /* 0x07 */ s8 heightPhaseStep;
    /* 0x08 */ f32 widthWobble;
    /* 0x0C */ f32 heightWobble;
    /* 0x10 */ Vec3f direction;
    /* 0x1C */ f32 angle;
    /* 0x20 */ f32 angleSpeed;
} OscillateMotion;

/* Particle data at 0xA8 for drifting particles that may land (effect_updateDriftParticle, effect_updateLandingDriftParticle). */
typedef struct {
    /* 0x00 */ f32 baseSize;
    /* 0x04 */ u8 widthPhase;
    /* 0x05 */ u8 heightPhase;
    /* 0x06 */ s8 widthPhaseStep;
    /* 0x07 */ s8 heightPhaseStep;
    /* 0x08 */ f32 widthWobble;
    /* 0x0C */ f32 heightWobble;
    /* 0x10 */ u8 swayPhaseX;
    /* 0x11 */ u8 swayPhaseZ;
    /* 0x12 */ u8 swayPhaseStepX;
    /* 0x13 */ u8 swayPhaseStepZ;
    /* 0x14 */ f32 swayX;
    /* 0x18 */ f32 swayZ;
    /* 0x1C */ f32 damping;
    /* 0x20 */ f32 groundHeight; /* GROUND_UNKNOWN: query collision_probeCeilingSurfABSet0Cached each frame */
} ParticleDriftMotion;

#define GROUND_UNKNOWN -99999.8984375f

/* Particle object created by particle_newSpriteParticle / particle_newSpriteParticleWithFloor from a ParticleDesc6C. */
struct EffectMotionParticle {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ union {
        RiseMotion rise;
        OscillateMotion oscillate;
        ParticleDriftMotion drift;
    } motion;
};

/* Second argument of the particle update callbacks; only unk4 is read here. */
struct ParticleUpdateArg {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ f32 unk4;
};

/* Update callback: rises and sways; dies on reaching the ground hit above it (splashing when ground.source is 3). */
u8 effect_updateRiseParticleToCeiling(EffectMotionParticle *particle, ParticleUpdateArg *arg1) {
    u8 alive;
    s32 found;

    alive = 1;
    if (effect_updateRiseParticle(particle, arg1) != 0) {
        if (particle->head.desc.vel.y > 0.0f) {
            Vec3f pos;

            pos.x = particle->head.desc.pos.x + particle->head.desc.offset.x;
            pos.y = arg1->unk4;
            pos.z = particle->head.desc.pos.z + particle->head.desc.offset.z;
            if (math_isInWorldBounds(&pos) == 0) {
                return 0;
            }
            found = collision_probeCeilingSurfABSet0Cached(&pos, 0, particle->head.desc.pos.y, &particle->head.floor);
            if (found != 0) {
                pos.y = particle->head.floor.height;
                if (particle->head.floor.source == 3) {
                    particle_spawnKindMark(particle_getContextEffectKind(), particle->motion.rise.baseSize * 0.200000003f, 0xFF,
                                  &particle->head.floor.tri, &pos, particle->head.header.owner, particle->head.header.kind);
                }
                alive = 0;
            }
        }
    } else {
        alive = 0;
    }
    return alive;
}

/* Update callback: wobbles the size, caps the rise speed and sways with it; dies above the ceiling. */
s32 effect_updateRiseParticle(EffectMotionParticle *particle, ParticleUpdateArg *arg1) {
    RiseMotion *rise;
    f32 sin0;
    f32 sin1;
    u8 alive;
    f32 sway;

    rise = &particle->motion.rise;
    alive = 1;
    rise->widthPhase += rise->widthPhaseStep * gFrameTicks;
    rise->heightPhase += rise->heightPhaseStep * gFrameTicks;
    sin0 = math_cosBinAngleMasked(rise->widthPhase - 0x40);
    sin1 = math_cosBinAngleMasked(rise->heightPhase - 0x40);
    particle->head.desc.width = rise->baseSize + (rise->widthWobble * sin0);
    particle->head.desc.height = rise->baseSize + (rise->heightWobble * sin1);

    if (0.0f < particle->head.desc.vel.y) {
        if (rise->maxRiseSpeed < particle->head.desc.vel.y) {
            particle->head.desc.vel.y = rise->maxRiseSpeed;
        }
        sway = particle->head.desc.vel.y * rise->swayPerSpeed;
        rise->swayPhaseX += rise->swayPhaseStepX * gFrameTicks;
        rise->swayPhaseZ += rise->swayPhaseStepZ * gFrameTicks;
        sin0 = math_cosBinAngleMasked(rise->swayPhaseX - 0x40);
        sin1 = math_cosBinAngleMasked(rise->swayPhaseZ - 0x40);
        particle->head.desc.offset.x = (rise->swayX * sin0) * sway;
        particle->head.desc.offset.z = (rise->swayZ * sin1) * sway;
    }

    if (particle->head.desc.flags & 0x2000) {
        if (rise->ceiling < (particle->head.desc.offset.y + particle->head.desc.pos.y)) {
            alive = 0;
        }
    }
    return alive;
}

/* Spawns one rising particle (effect_updateRiseParticle family) at pos with the given velocity. */
EffectMotionParticle *effect_spawnRiseParticle(Vec3f *pos, Vec3f *velocity, s32 arg2, s32 arg3) {
    f32 size;
    ParticleDesc6C spawn;
    EffectMotionParticle *particle;
    RiseMotion rise;

    spawn.texture = 0x2F;
    spawn.renderMode = 0xC01;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.b2 = 0;
    spawn.g2 = 0;
    spawn.r2 = 0;
    spawn.a = 0;
    spawn.b = 0;
    spawn.g = 0;
    spawn.r = 0;
    spawn.frameRate = 0;
    spawn.frame = 0;
    spawn.brightness = 0xFF;
    spawn.scaleStart = 0;
    spawn.scaleRate = 0.0f;
    spawn.flags = 0x3207;
    spawn.primColorMode = 5;
    spawn.envColorMode = 5;
    spawn.updateFunc = 4;
    spawn.deathFunc = -1;
    spawn.pos = *pos;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    rise.widthPhase = 0;
    rise.heightPhase = 0;
    rise.swayPhaseX = 0;
    rise.swayPhaseZ = 0;
    rise.ceiling = 1000.0f;
    rise.maxRiseSpeed = 4.0f;
    rise.swayPerSpeed = 0.25f;
    spawn.vel = *velocity;

    if (math_random() & 1) {
        spawn.flags |= 0x40;
    }
    if (math_random() & 1) {
        spawn.flags |= 0x80;
    }

    spawn.timer = (math_random() % 25U) + 30;
    spawn.fadeStart = 30;
    spawn.fadeRate = 8;
    spawn.alpha = (math_random() % 101U) + 100;
    size = (math_randomFloat() * 20.0f) + 49.0f;
    spawn.width = size;
    rise.baseSize = size;
    spawn.height = size;
    rise.widthPhaseStep = (math_random() % 5U) + 4;
    rise.heightPhaseStep = (math_random() % 5U) + 4;
    rise.widthWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * rise.baseSize;
    rise.heightWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * rise.baseSize;
    rise.swayPhaseStepX = (math_random() % 5U) + 4;
    rise.swayPhaseStepZ = (math_random() % 5U) + 4;
    rise.swayX = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * rise.baseSize;
    rise.swayZ = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * rise.baseSize;
    spawn.gravity = ((math_randomFloat() * 254.0f) + 108.0f) * 0.001000000047f;

    particle = particle_newSpriteParticle(&spawn, 2, sizeof(RiseMotion));
    if (particle != NULL) {
        memcpy(&particle->motion, &rise, sizeof(RiseMotion));
    }
    return particle;
}

/* Spawns one stationary rising particle at (x, y, z). */
void effect_emitRiseParticle(f32 x, f32 y, f32 z, s32 arg3, s32 arg4, s32 arg5, EffectHeader *obj) {
    Vec3f pos;
    Vec3f velocity;

    pos.x = x;
    pos.y = y;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    velocity.z = 0.0f;
    pos.z = z;
    effect_spawnRiseParticle(&pos, &velocity, obj->owner, obj->kind);
}

/* Emitter data at 0x28 of the effect_updateDiskEmitter emitter. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 radius;
    /* 0x20 */ f32 angleX;
    /* 0x24 */ f32 angleY;
    /* 0x28 */ f32 pending; /* particles owed; one is spawned per whole unit */
    /* 0x2C */ f32 rate;
    /* 0x30 */ f32 rateRandom;
} DiskEmitter;

struct DiskEmitterObject {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ DiskEmitter emitter;
};

/* Emitter update: spawns oscillating particles at random points of a rotated disk. */
void effect_updateDiskEmitter(DiskEmitterObject *obj) {
    DiskEmitter *emitter;
    EffectMotionParticle *particle;

    emitter = &obj->emitter;
    if (math_isNearCamera(gLastPlayerIndex, 3000.0f, &emitter->position)) {
        emitter->pending += (emitter->rate + (math_randomFloat() * emitter->rateRandom)) * gFrameDelta;
        if (emitter->pending > 1.0f) {
            ParticleDesc6C spawn;
            OscillateMotion oscillate;
            f32 angle;
            f32 radius;
            f32 x;
            f32 z;
            f32 scale;
            f32 invScale;
            MtxF mtx;
            f32 dist;
            f32 fadeTime;
            f32 size;

            spawn.texture = 0x2F;
            spawn.renderMode = 0xC01;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.b2 = 0;
            spawn.g2 = 0;
            spawn.r2 = 0;
            spawn.a = 0;
            spawn.b = 0;
            spawn.g = 0;
            spawn.r = 0;
            spawn.frameRate = 0;
            spawn.frame = 0;
            spawn.brightness = 0xFF;
            spawn.scaleStart = 0;
            spawn.scaleRate = 0.0f;
            spawn.flags = 0x1205;
            spawn.primColorMode = 5;
            spawn.envColorMode = 5;
            spawn.updateFunc = 12;
            spawn.deathFunc = -1;
            spawn.pos = emitter->position;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            oscillate.widthPhase = 0;
            oscillate.heightPhase = 0;
            spawn.gravity = 0.0f;
            do {
                angle = math_randomFloat() * 6.283185482f;
                radius = math_randomFloat() * emitter->radius;
                x = sinf(angle) * radius;
                z = cosf(angle) * radius;
                math_mtxRotateRPYF(mtx, emitter->angleX, emitter->angleY, 0.0f);
                math_transformPoint(mtx, x, 0.0f, z, &oscillate.direction.x, &oscillate.direction.y,
                              &oscillate.direction.z);
                oscillate.angle = math_randomFloat() * 6.283185482f;
                oscillate.angleSpeed = ((math_randomFloat() * 320.0f) + -160.0f) * 0.001000000047f;
                {
                    s16 pad; // matching: IDO's frame has an unused slot here

                    scale = ((math_randomFloat() * 161.0f) + 446.0f) * 0.009999999776f;
                    invScale = 1.0f / scale;
                    dist = emitter->unk18 * invScale;
                    fadeTime = ((math_randomFloat() * 6596.0f) + 2402.0f) * 0.1000000015f * invScale;
                    spawn.vel.x = emitter->velocity.x * scale;
                    spawn.vel.y = emitter->velocity.y * scale;
                    spawn.vel.z = emitter->velocity.z * scale;
                    if (dist > 500.0f) {
                        spawn.timer = 500;
                    } else {
                        spawn.timer = (fadeTime * 0.5f) + dist;
                    }
                    spawn.fadeStart = fadeTime;
                    spawn.fadeRate = 0xFF / spawn.fadeStart;
                    spawn.flags &= ~0xC0;
                    if (math_random() & 1) {
                        spawn.flags |= 0x40;
                    }
                    if (math_random() & 1) {
                        spawn.flags |= 0x80;
                    }
                    spawn.alpha = (math_random() % 51U) + 124;
                    size = (math_randomFloat() * 100.0f) + 240.0f;
                    spawn.width = size;
                    oscillate.baseSize = size;
                    spawn.height = size;
                    oscillate.widthPhaseStep = (math_random() % 5U) + 4;
                    oscillate.heightPhaseStep = (math_random() % 5U) + 4;
                    oscillate.widthWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * oscillate.baseSize;
                    oscillate.heightWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * oscillate.baseSize;
                    particle = particle_newSpriteParticle(&spawn, 2, sizeof(OscillateMotion));
                    if (particle != NULL) {
                        memcpy(&particle->motion, &oscillate, sizeof(OscillateMotion));
                    }
                    emitter->pending -= 1.0f;
                }
            } while (emitter->pending > 1.0f);
        }
    }
}


/* Update callback: wobbles the size and moves the offset along direction by sin(angle). */
s32 effect_updateOscillateParticle(EffectMotionParticle *particle, ParticleUpdateArg *arg1) {
    OscillateMotion *osc;
    f32 sin0;
    f32 sin1;

    osc = &particle->motion.oscillate;
    osc->widthPhase += osc->widthPhaseStep * gFrameTicks;
    osc->heightPhase += osc->heightPhaseStep * gFrameTicks;
    sin0 = math_cosBinAngleMasked(osc->widthPhase - 0x40);
    sin1 = math_cosBinAngleMasked(osc->heightPhase - 0x40);
    particle->head.desc.width = osc->baseSize + (osc->widthWobble * sin0);
    particle->head.desc.height = osc->baseSize + (osc->heightWobble * sin1);
    if ((osc && osc) && osc) { // matching: these dead tests of osc steer IDO's schedule of the angle update
    }
    osc->angle += osc->angleSpeed * gFrameDelta;
    osc->angle = math_wrapRadians(osc->angle);
    sin0 = sinf(osc->angle);
    particle->head.desc.offset.x = osc->direction.x * sin0;
    particle->head.desc.offset.y = osc->direction.y * sin0;
    particle->head.desc.offset.z = osc->direction.z * sin0;
    return 1;
}

/* Spawns one drifting particle; with no horizontal velocity it lands on the ground below pos. */
EffectMotionParticle *effect_spawnDriftMotionParticle(Vec3f *pos, Vec3f *velocity, f32 arg2, f32 damping, CollisionHit *ground, f32 size,
                        s16 lifetime, u8 alpha, u8 arg8, u8 arg9, s32 argA) {
    EffectMotionParticle *particle;
    ParticleDesc spawn;
    ParticleDriftMotion drift;
    CollisionHit hit;
    s32 flipY;
    s32 flipX;

    spawn.texture = 0x2F;
    spawn.renderMode = 0x2203;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.b2 = 0;
    spawn.g2 = 0;
    spawn.r2 = 0;
    spawn.a = 0;
    spawn.b = 0;
    spawn.g = 0;
    spawn.r = 0;
    spawn.frameRate = 0;
    spawn.frame = 0;
    spawn.brightness = 0xFF;
    spawn.scaleStart = 0;
    spawn.scaleRate = 0.0f;
    flipX = (math_random() & 1) ? 0x40 : 0;
    flipY = (math_random() & 1) ? 0x80 : 0;
    spawn.flags = flipY | 0x1007 | flipX | 0x8000 | 0x4000;
    spawn.primColorMode = 6;
    spawn.envColorMode = 8;
    if (arg8) {
        spawn.updateFunc = 0x15;
    } else {
        spawn.updateFunc = 0x14;
    }
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.pos = *pos;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    drift.baseSize = size;
    spawn.width = size;
    spawn.height = size;
    drift.widthPhase = math_random() & 0xFF;
    drift.heightPhase = math_random() & 0xFF;
    drift.widthPhaseStep = (math_random() % 5U) + 4;
    drift.heightPhaseStep = (math_random() % 5U) + 4;
    drift.widthWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * drift.baseSize;
    drift.heightWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * drift.baseSize;
    drift.swayPhaseX = 0;
    drift.swayPhaseZ = 0;
    drift.swayPhaseStepX = (math_random() % 5U) + 4;
    drift.swayPhaseStepZ = (math_random() % 5U) + 4;
    drift.swayX = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * drift.baseSize;
    drift.swayZ = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * drift.baseSize;
    spawn.vel = *velocity;
    spawn.handlerSet = 0;
    spawn.timer = lifetime;
    spawn.alpha = alpha;
    spawn.gravity = arg2;
    drift.damping = damping;
    if (velocity->x != 0 || velocity->z != 0) {
        drift.groundHeight = GROUND_UNKNOWN;
    } else {
        if (ground != NULL) {
            hit = *ground;
        } else {
            hit.flags = 0;
        }
        if (math_isInWorldBounds(pos)) {
            if (collision_probeCeilingSurfABSet0Cached(pos, 0, 19500.0f, &hit)) {
                ground = &hit;
                drift.groundHeight = hit.height;
            } else {
                drift.groundHeight = 10000.0f;
            }
        } else {
            drift.groundHeight = 10000.0f;
        }
    }
    particle = particle_newSpriteParticleWithFloor(&spawn, 2, ground, sizeof(ParticleDriftMotion), arg9, argA);
    if (particle != NULL) {
        memcpy(&particle->motion, &drift, sizeof(ParticleDriftMotion));
    }
    return particle;
}

/* Update callback: wobbles the size, sways the offset and damps the velocity. */
s32 effect_updateDriftParticle(EffectMotionParticle *particle, ParticleUpdateArg *arg1) {
    ParticleDriftMotion *drift;
    s32 i;

    drift = &particle->motion.drift;
    particle->head.desc.width = (math_cosBinAngleMasked(drift->widthPhase - 0x40) * drift->widthWobble) + drift->baseSize;
    particle->head.desc.height = (math_cosBinAngleMasked(drift->heightPhase - 0x40) * drift->heightWobble) + drift->baseSize;
    particle->head.desc.offset.x = math_cosBinAngleMasked(drift->swayPhaseX - 0x40) * drift->swayX;
    particle->head.desc.offset.z = math_cosBinAngleMasked(drift->swayPhaseZ - 0x40) * drift->swayZ;

    drift->widthPhase += drift->widthPhaseStep * gFrameTicks;
    drift->heightPhase += drift->heightPhaseStep * gFrameTicks;
    drift->swayPhaseX += drift->swayPhaseStepX * gFrameTicks;
    drift->swayPhaseZ += drift->swayPhaseStepZ * gFrameTicks;

    for (i = gFrameTicks; i > 0; i--) {
        particle->head.desc.vel.x *= drift->damping;
        particle->head.desc.vel.y *= drift->damping;
        particle->head.desc.vel.z *= drift->damping;
    }
    return 1;
}

/* Update callback: drifts, then dies on reaching the ground (sometimes splashing on surface 3). */
u8 effect_updateLandingDriftParticle(EffectMotionParticle *particle, ParticleUpdateArg *arg1) {
    ParticleDriftMotion *drift;
    u8 alive;
    Vec3f pos;

    alive = 1;
    drift = &particle->motion.drift;
    effect_updateDriftParticle(particle, arg1);
    if (particle->head.desc.vel.y < 0.0f) {
        return 1;
    }
    pos.x = particle->head.desc.pos.x + particle->head.desc.offset.x;
    pos.y = arg1->unk4;
    pos.z = particle->head.desc.pos.z + particle->head.desc.offset.z;
    if (drift->groundHeight == GROUND_UNKNOWN) {
        if (math_isInWorldBounds(&pos) == 0) {
            return 0;
        }
        if (collision_probeCeilingSurfABSet0Cached(&pos, 0, particle->head.desc.pos.y, &particle->head.floor) != 0) {
            pos.y = particle->head.floor.height;
            alive = 0;
        }
    } else if (drift->groundHeight <= particle->head.desc.pos.y) {
        pos.y = drift->groundHeight;
        alive = 0;
    }
    if (alive == 0) {
        if (math_randomFloat() < 0.200000003f) {
            if (particle->head.floor.source == 3) {
                particle_spawnKindMark(particle_getContextEffectKind(), drift->baseSize * 0.200000003f, particle->head.desc.alpha,
                              &particle->head.floor.tri, &pos, particle->head.header.owner, particle->head.header.kind);
            }
        }
    }
    return alive;
}

/* Returns the address of chr's watcher floor hit through *out; 0 if chr has no watcher. */
s32 effect_getWatcherFloorHit(Actor *chr, CollisionHit **out) {
    Attachment *node;
    CharacterWatcher *watcher;

    if (effect_findAttachmentByType(chr->attachments, 0x13, &node) == 0) {
        return 0;
    }
    watcher = node->handle;
    *out = &watcher->hit;
    return 1;
}

/* Settings an emitter (effect_updateRiseEmitter) spawns its rising particles with. */
typedef struct {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRandom;
    /* 0x08 */ f32 sizeBase;
    /* 0x0C */ f32 sizeRandom;
    /* 0x10 */ f32 unk54Base;
    /* 0x14 */ f32 unk54Random;
    /* 0x18 */ f32 maxRiseSpeed;
    /* 0x1C */ f32 swayPerSpeed;
    /* 0x20 */ u8 alphaBase;
    /* 0x21 */ u8 alphaRange;
    /* 0x22 */ u8 color[3];
    /* 0x25 */ u8 pad25[3];
} EmitterSettings;

typedef struct {
    s16 base;
    s16 range;
} TypeRange;

/* Data at 0x80 of a spawner object: emitters get a random type in [types.base, types.base + types.range]. */
typedef struct {
    /* 0x00 */ TypeRange types;
    /* 0x04 */ EmitterSettings settings;
} SpawnerData;

/* Spawner object created by beam_newAreaEmitter (effect_spawnRiseZoneEmitter). */
struct Spawner {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EmitterState state;
    /* 0x80 */ SpawnerData data; /* Emitter.data */
};

struct SpawnerTemplate {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ SpawnerData data;
};

static const SpawnerTemplate gRiseSpawnerTemplates[1] = {
    { 3.7e-07f, 1.12e-06f,
      { { 20, 90 }, { 0.15f, 0.37f, 60.0f, 88.0f, 0.079f, 0.12900001f, 7.0f, 0.14285715f, 0x3C, 0x8C, { 0xA3, 0xD8, 0x00 } } } },
};

struct SpawnerSource {
    /* 0x00 */ u8 pad0[0x1F];
    /* 0x1F */ u8 variant; /* index into gRiseSpawnerTemplates */
};

/* Creates a spawner object for src from template gRiseSpawnerTemplates[src->variant] (variant 0 only). */
// matching: volatile makes IDO reload src from its stack slot
void effect_spawnRiseZoneEmitter(SpawnerSource *volatile src, s32 arg1) {
    EmitterDef info;
    s32 pad[2]; // matching: 8 unused frame bytes between data and info
    SpawnerData data;
    Spawner *spawner;
    s32 variant;

    variant = src->variant;
    if (variant <= 0) {
        info.flags = 0xF;
        info.anchor = (TriggerZone *)src;
        info.rate = gRiseSpawnerTemplates[variant].rate;
        info.rateRange = gRiseSpawnerTemplates[variant].rateRange;
        info.pos.x = 0.0f;
        info.pos.y = 0.0f;
        info.pos.z = 0.0f;
        info.bottom = 0.0f;
        info.radius = 0.0f;
        info.type = 0;
        info.update = -1;
        info.spawn = 2;
        data.types = gRiseSpawnerTemplates[variant].data.types;
        data.settings = gRiseSpawnerTemplates[variant].data.settings;
        spawner = (Spawner *)beam_newAreaEmitter(&info, -1, sizeof(SpawnerData), 0xFF, 0); /* an Emitter with SpawnerData */
        if (spawner != NULL) {
            memcpy(&spawner->data, &data, sizeof(SpawnerData));
        }
    }
}

/* Emitter data at 0x28 of the effect_updateRiseEmitter emitter. */
typedef struct {
    /* 0x00 */ f32 pending; /* particles owed; one is spawned per whole unit */
    /* 0x04 */ Vec3f position;
    /* 0x10 */ f32 ceiling;
    /* 0x14 */ EmitterSettings settings;
} RiseEmitter;

struct RiseEmitterObject {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ RiseEmitter emitter;
};

/* Spawner callback: creates an emitter of a random type at pos that spawns particles up to ceiling. */
void effect_spawnRiseEmitter(Spawner *spawner, Vec3f *pos, f32 ceiling, s32 arg3) {
    RiseEmitter emitter;
    SpawnerData *data;
    RiseEmitterObject *obj;

    data = &spawner->data;
    emitter.pending = 0.0f;
    emitter.position = *pos;
    emitter.ceiling = ceiling;
    emitter.settings = data->settings;
    obj = effect_newTimedEffect(math_random() % (data->types.range + 1) + data->types.base,
                                             -1, 0x3F, -1, 1, 0, sizeof(RiseEmitter), spawner->base.header.owner,
                                             spawner->base.header.kind);
    if (obj != NULL) {
        memcpy(&obj->emitter, &emitter, sizeof(RiseEmitter));
    }
}

/* Emitter update: spawns rising particles (effect_updateRiseParticle family) at the emitter's position. */
void effect_updateRiseEmitter(RiseEmitterObject *obj) {
    RiseEmitter *emitter;

    emitter = &obj->emitter;
    emitter->pending += (emitter->settings.rate + (math_randomFloat() * emitter->settings.rateRandom)) * gFrameDelta;
    if (emitter->pending > 1.0f) {
        ParticleDesc spawn;
        RiseMotion rise;
        EffectMotionParticle *particle;
        s32 flipY;
        s32 flipX;

        rise.widthPhase = 0;
        rise.heightPhase = 0;
        rise.swayPhaseX = 0;
        rise.swayPhaseZ = 0;
        rise.maxRiseSpeed = emitter->settings.maxRiseSpeed;
        rise.swayPerSpeed = emitter->settings.swayPerSpeed;
        rise.ceiling = emitter->ceiling;
        spawn.texture = 0x2F;
        spawn.renderMode = 0x4403;
        spawn.unk0 = 0x200005;
        spawn.unk4 = 0x9F0600;
        spawn.timer = 300;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.r2 = 0xFF;
        spawn.g2 = 0xFF;
        spawn.b2 = 0xFF;
        spawn.a = 0xFF;
        spawn.r = emitter->settings.color[0];
        spawn.g = emitter->settings.color[1];
        spawn.b = emitter->settings.color[2];
        spawn.brightness = 0xFF;
        spawn.pos = emitter->position;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;
        spawn.scaleStart = 1;
        spawn.scaleRate = 1.0f;
        spawn.primColorMode = 6;
        spawn.envColorMode = 9;
        spawn.updateFunc = 4;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.offset.x = 0.0f;
        spawn.offset.y = 0.0f;
        spawn.offset.z = 0.0f;
        do {
            rise.widthPhaseStep = (math_random() % 5U) + 4;
            rise.heightPhaseStep = (math_random() % 5U) + 4;
            rise.baseSize = spawn.width = spawn.height =
                (math_randomFloat() * emitter->settings.sizeRandom) + emitter->settings.sizeBase;
            rise.widthWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * rise.baseSize;
            rise.heightWobble = ((math_randomFloat() * 0.25f) + 0.150000006f) * rise.baseSize;
            rise.swayPhaseStepX = (math_random() % 5U) + 4;
            rise.swayPhaseStepZ = (math_random() % 5U) + 4;
            rise.swayX = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * rise.baseSize;
            rise.swayZ = ((math_randomFloat() * 0.1000000015f) + 0.07999999821f) * rise.baseSize;
            spawn.gravity = (math_randomFloat() * emitter->settings.unk54Random) + emitter->settings.unk54Base;
            spawn.alpha = (math_random() % (emitter->settings.alphaRange + 1)) + emitter->settings.alphaBase;
            flipX = (math_random() & 1) ? 0x40 : 0;
            flipY = (math_random() & 1) ? 0x80 : 0;
            spawn.flags = flipY | 6 | flipX | 0x8000 | 0x4000 | 0x2000 | 0x1000;
            particle = particle_newSpriteParticleWithFloor(&spawn, 2, NULL, sizeof(RiseMotion), obj->base.header.owner, obj->base.header.kind);
            if (particle != NULL) {
                memcpy(&particle->motion, &rise, sizeof(RiseMotion));
            }
            emitter->pending -= 1.0f;
        } while (emitter->pending > 1.0f);
    }
}
