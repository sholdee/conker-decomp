#include <ultra64.h>
#include "conker/actor.h"
#include "conker/beam.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/rope.h"
#include "conker/script.h"
#include "conker/sprite.h"
#include "conker/trail.h"
#include "libc/string.h"

/* Spawn callbacks by slot (see effect_spawnAttachments); slot 0 is empty. */
static void (*gAttachSpawnFuncs[])(Actor *) = {
    NULL,           effect_attachBone6GlowAndBone5Trail, effect_attachNarrowBone6GlowAndBone5Trail, effect_attachBone12GlowAndLight, effect_attachCharacterEffectMinus1, effect_attachCharacterEffect1,
    effect_attachCharacterEffect0,  effect_attachCharacterEffect2, effect_attachCharacterEffect3, func_1514E824, effect_attachWatcherAndRopeRider, func_1514D96C,
    effect_attachRedBoneGlowsAndLight,  func_1514DB58, func_1514DE50, effect_linkToObjectId9, effect_attachFixedPointStripToBone6, func_1514DA38,
    func_1514DAA4,  effect_attachCharacterWatcher, effect_spawnRopeTieInDinoArena, func_1514DE88, func_1514DB98,
};

/* Spawn callbacks by object id; entry 0 runs for every object first. */
static void (*gAttachSpawnFuncsById[])(Actor *) = {
    effect_attachActorTracker, func_1514DD2C, func_1514DD4C, func_1514DD6C, func_1514DD8C, effect_attachBone1RedLightAndGlow,
    func_1514F110, beam_attachActorLights, func_1514DDAC, func_1514DDCC, func_1514DDEC, func_1514DE38,
    func_1514DE44, effect_attachBone5LightBeam, func_1514DE0C, effect_hidePart1, effect_spawnActorEmitter, func_1514DE2C,
    effect_hideParts4And8To10, effect_attachObjBurst, effect_attachBone2Light, effect_attachFrontendFireImpEffect, level_spawnBoltEmitter, effect_attachTrailToBone1,
    effect_attachTrailToBone2, effect_attachTrailToBone3, effect_attachPulseEffect, func_1514F590, effect_attachPerimeterEmitter, effect_spawnActorPoolEmitter,
    particle_spawnBoneBurstEmitter, level_attachDripEmitter, effect_spawnFloorPointTracker, effect_attachTrailToBone4, func_1514D978,
};

/* Attachment type handlers (AttachmentType.destroyHandler, .messageHandler). Declared without a
   prototype: the handlers take the attachment's handle as what it is. */
static void (*gAttachDestroyFuncs[])() = { effect_releaseAttachedObject, effect_releaseAttachedEmitter, effect_releaseAttachedPointLight };
static s32 (*gAttachMessageFuncs[])() = {
    effect_handleToggleMessage, effect_handleGlowToggleMessage, effect_forwardGlowToggleMessage, effect_forwardToggleMessage,
    effect_handleEmitterTableMessage, effect_handleRopeRiderMessage, effect_handleLightMessage, effect_acceptAnyMessage,
};

/* Emitter offsets. */
static const Vec3f gBone2LightOffset = { 0.0f, 75.0f, 96.0f };
static const Vec3f D_800A58AC = { 0.0f, 66.0f, 50.0f };
static const Vec3f D_800A58B8 = { 0.0f, 66.0f, 274.0f };
static const Vec3f D_800A58C4 = { 0.0f, 31.0f, 73.0f };

/**
 * Finds which spawn-callback slots apply to `obj`: one chosen by its object id (*idSlot, flag 4),
 * one chosen by level (*globalSlot, flag 2) and, inside that, one chosen by its unique_id
 * (*uniqueSlot, flag 1). Returns the OR of the flags that were set.
 */
s32 effect_findAttachSpawnSlots(Actor *obj, s32 *uniqueSlot, s32 *globalSlot, s32 *idSlot) {
    u8 found = 0;

    switch (obj->character_type) {
    case 94:
        *idSlot = 25;
        found = 4;
        break;
    case 154:
        *idSlot = 24;
        found = 4;
        break;
    case 16:
    case 46:
        *idSlot = 23;
        found = 4;
        break;
    case 127:
        *idSlot = 22;
        found = 4;
        break;
    case 153:
        *idSlot = 21;
        found = 4;
        break;
    case 137:
    case 186:
        *idSlot = 20;
        found = 4;
        break;
    case 173:
        *idSlot = 17;
        found = 4;
        break;
    case 134:
        *idSlot = 18;
        found = 4;
        break;
    case 82:
        *idSlot = 19;
        found = 4;
        break;
    case 95:
        *idSlot = 27;
        found = 4;
        break;
    case 107:
        *idSlot = 16;
        found = 4;
        break;
    case 148:
        *idSlot = 14;
        found = 4;
        break;
    case 37:
        *idSlot = 26;
        found = 4;
        break;
    case 146:
        *idSlot = 15;
        found = 4;
        break;
    case 159:
    case 160:
        *idSlot = 13;
        found = 4;
        break;
    case 147:
        *idSlot = 12;
        found = 4;
        break;
    case 141:
        *idSlot = 11;
        found = 4;
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 150:
        *idSlot = 0;
        found = 4;
        break;
    case 8:
        *idSlot = 2;
        found = 4;
        break;
    case 10:
        *idSlot = 3;
        found = 4;
        break;
    case 41:
    case 42:
        *idSlot = 4;
        found = 4;
        break;
    case 83:
        *idSlot = 5;
        found = 4;
        break;
    case 119:
        *idSlot = 6;
        found = 4;
        break;
    case 71:
        *idSlot = 7;
        found = 4;
        break;
    case 40:
        *idSlot = 8;
        found = 4;
        break;
    case 123:
        *idSlot = 9;
        found = 4;
        break;
    case 18:
        *idSlot = 10;
        found = 4;
        break;
    }

    switch (level) {
    case LEVEL_WMP_BEACH:
        *globalSlot = 21;
        found |= 2;
        if (obj->character_type == 156 || obj->character_type == 157) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_FRONTEND:
        *globalSlot = 20;
        found |= 2;
        if (obj->unique_id == 9) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W1_BULL_PADDOCK:
        *globalSlot = 16;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W1_POO_CABIN:
        *globalSlot = 17;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W2_WARBOSS:
        *globalSlot = 15;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W2_LAB:
        *globalSlot = 18;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W1_POO_BOSS:
        *globalSlot = 19;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W2_TANKTUNNEL:
        *globalSlot = 14;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W2_LIFTTUNNEL:
        *globalSlot = 10;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W2_FLAMETUNNEL:
        *globalSlot = 11;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W2_MACHINEGUNNEST:
        *globalSlot = 12;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W2_CANYON:
        *globalSlot = 13;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W2_TANK_FACTORY:
        *globalSlot = 7;
        found |= 2;
        switch (obj->unique_id) {
        case 8:
            *uniqueSlot = 0;
            found |= 1;
            break;
        case 1:
            *uniqueSlot = 1;
            found |= 1;
            break;
        }
        break;
    case LEVEL_W2_OUTSIDE_MOLEHOLE:
        *globalSlot = 6;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W1_DOG_FISH:
        *globalSlot = 5;
        found |= 2;
        if (obj->unique_id == 16) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W1_BARN_OUTSIDE:
        *globalSlot = 3;
        found |= 2;
        break;
    case LEVEL_W1_BRONTO_CAVERN:
        *globalSlot = 0;
        found |= 2;
        switch (obj->unique_id) {
        case 9:
            *uniqueSlot = 0;
            found |= 1;
            break;
        case 7:
            *uniqueSlot = 1;
            found |= 1;
            break;
        case 14:
        case 15:
        case 23:
        case 25:
            *uniqueSlot = 2;
            found |= 1;
            break;
        case 13:
            *uniqueSlot = 3;
            found |= 1;
            break;
        }
        break;
    case LEVEL_W2_BOILERROOM:
        *globalSlot = 1;
        found |= 2;
        switch (obj->unique_id) {
        case 6:
            *uniqueSlot = 0;
            found |= 1;
            break;
        case 7:
            *uniqueSlot = 1;
            found |= 1;
            break;
        case 8:
            *uniqueSlot = 2;
            found |= 1;
            break;
        case 9:
            *uniqueSlot = 3;
            found |= 1;
            break;
        case 10:
            *uniqueSlot = 4;
            found |= 1;
            break;
        case 11:
            *uniqueSlot = 5;
            found |= 1;
            break;
        case 12:
            *uniqueSlot = 6;
            found |= 1;
            break;
        case 13:
            *uniqueSlot = 7;
            found |= 1;
            break;
        case 15:
            *uniqueSlot = 8;
            found |= 1;
            break;
        }
        break;
    case LEVEL_THE_WINDMILL:
        *globalSlot = 2;
        found |= 2;
        break;
    case LEVEL_W1_BARN_ARENA:
        *globalSlot = 4;
        found |= 2;
        if (obj->unique_id == 6) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W1_DINO_ARENA:
        *globalSlot = 8;
        found |= 2;
        if (obj->unique_id == 16) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case LEVEL_W1_DOG_FISH_NURSERY_PART1:
        *globalSlot = 9;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    }
    return found;
}

/**
 * Runs the spawn callbacks for `obj` (see effect_findAttachSpawnSlots), after freeing whatever is still
 * queued for its slot. Does nothing if the object already has attachments.
 */
s32 effect_spawnAttachments(Actor *obj) {
    s32 uniqueSlot;
    s32 globalSlot;
    s32 idSlot;
    u8 flags;
    s32 found;
    s32 slot;
    s32 handler;
    PlayerMarker *queued;

    uniqueSlot = -1;
    globalSlot = -1;
    idSlot = -1;
    slot = obj - movement_obj;
    if (obj->attachments != NULL) {
        return 0;
    }

    do {
        queued = effect_findPlayerMarker(slot);
        if (queued != NULL) {
            effect_destroyObject(&queued->header);
        }
    } while (queued != NULL);

    if (slot == 0 && gIsMultiplayer == 0) {
        effect_newPlayerMarker(slot, 0xFF);
    }

    gAttachSpawnFuncsById[0](obj);
    // matching: the zero test must be on the s32 result, not on the u8 copy
    found = effect_findAttachSpawnSlots(obj, &uniqueSlot, &globalSlot, &idSlot);
    flags = found;
    if (found == 0) {
        return 0;
    }

    if (flags & 4) {
        handler = gAttachSpawnSlotsById[idSlot];
        if (handler != 0) {
            gAttachSpawnFuncs[handler](obj);
        }
    }

    if (flags & 2) {
        handler = gAttachSpawnSlotsByLevel[globalSlot];
        if (handler != 0) {
            gAttachSpawnFuncs[handler](obj);
        }
    }

    if ((flags & 2) && (flags & 1)) {
        if (gAttachSpawnFuncs[gAttachSpawnSlotLists[globalSlot][uniqueSlot]] != NULL) {
            gAttachSpawnFuncs[gAttachSpawnSlotLists[globalSlot][uniqueSlot]](obj);
        }
    }
    return 1;
}

/** Frees every attachment of `owner`, letting each attachment type release its handle first. */
s32 effect_freeAttachments(Actor *owner) {
    Attachment *node;
    Attachment *next;

    node = owner->attachments;
    while (node != NULL) {
        next = node->next;
        if (gAttachmentTypes[node->type]->destroyHandler != -1) {
            gAttachDestroyFuncs[gAttachmentTypes[node->type]->destroyHandler](node->handle);
        }
        effect_destroyObject(&node->header);
        node = next;
    }
    return 1;
}

/**
 * Sends message `msg` (with `arg`) to every attachment of `owner` whose type is `type` or one
 * of its subtypes. Returns the result of the last handler called, as 0 or 1.
 */
s32 effect_sendAttachmentMessage(Actor *owner, s16 type, s32 msg, s32 arg) {
    Attachment *node;
    s32 result;

    result = 0;
    for (node = owner->attachments; node != NULL; node = node->next) {
        if (type == node->type || effect_isAttachTypeInTree(gAttachmentTypes[type]->firstChild, node->type)) {
            if (gAttachmentTypes[node->type]->messageHandler != -1) {
                if (gAttachMessageFuncs[gAttachmentTypes[node->type]->messageHandler](node->handle, msg, arg)) {
                    result = 1;
                } else {
                    result = 0;
                }
            }
        }
    }
    return result;
}

/** Returns whether attachment type `type` occurs in the type subtree rooted at `root` or its siblings. */
s32 effect_isAttachTypeInTree(s16 root, s16 type) {
    if (root == -1) {
        return 0;
    }
    if (root == type) {
        return 1;
    }
    if (effect_isAttachTypeInTree(gAttachmentTypes[root]->nextSibling, type)) {
        return 1;
    }
    if (effect_isAttachTypeInTree(gAttachmentTypes[root]->firstChild, type)) {
        return 1;
    }
    return 0;
}

/** Spawns a beam_newStrip object running from a fixed point to `obj` and attaches it (type 0x15). */
void effect_attachFixedPointStripToBone6(Actor *obj) {
    StripDesc desc;
    StripObj *spawned;
    StripTarget payload;

    payload.obj = obj;
    payload.uniqueId = obj->unique_id;
    payload.offset.x = 0.0f;
    payload.offset.y = -180.0f;
    payload.offset.z = 77.0f;
    desc.p0.x = -477.0f;
    desc.p0.y = -2015.0f;
    desc.p0.z = 2244.0f;
    payload.bone = 6;
    desc.updateIdx = 1;
    desc.p1.x = obj->xpos;
    desc.p1.y = obj->ypos;
    desc.p1.z = obj->zpos;
    desc.kind = 1;
    desc.texScale = 0.3199999928f;
    spawned = beam_newStrip(&desc, sizeof(payload));
    if (spawned != NULL) {
        memcpy(&spawned->target, &payload, sizeof(payload));
        effect_attach(spawned, obj, 0x15);
    }
}

/** Attaches two emitters to `obj` (bones 0x2B and 5, types 9 and 10) plus a light_createAttached effect. */
void effect_attachRedBoneGlowsAndLight(Actor *obj) {
    GlowShape emitter;
    ParticleDef spawn;
    EmitterAttach attach;

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x12012;
    spawn.lifetime = 0;
    spawn.colorLevel = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0;
    spawn.rgba[2] = 0;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.width1 = 28.0f;
    emitter.height1 = 28.0f;
    emitter.offset1 = 0.5f;
    emitter.offset0 = 0.5f;
    emitter.width0 = 4.0f;
    emitter.height0 = 17.0f;
    emitter.alpha1 = 0.75f;
    emitter.alpha0 = 0.25f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.target = obj;
    attach.p0.x = 0.0f;
    attach.p0.y = 0.0f;
    attach.p1.x = 0.0f;
    attach.p1.y = 0.0f;
    attach.bone = 0x2B;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = 1;

    attach.p0.z = 25.0f;
    attach.p1.z = 75.0f;
    emitter.scale = 1.0f;

    effect_attach(particle_spawnBoneGlow(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 9);

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x12012;
    spawn.lifetime = 0;
    spawn.colorLevel = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0;
    spawn.rgba[2] = 0;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;

    emitter.width1 = 30.0f;
    emitter.height1 = 30.0f;
    emitter.width0 = 10.0f;
    emitter.offset1 = 0.8000000119f;
    emitter.offset0 = 0.8000000119f;

    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.alpha1 = 0.6000000238f;
    emitter.alpha0 = 0.6000000238f;
    emitter.height0 = 16.0f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.target = obj;
    attach.bone = 5;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = 0;
    attach.p0.y = 10.0f;
    attach.p0.x = 0.0f;
    attach.p1.x = 0.0f;
    attach.p0.z = -70.0f;
    attach.p1.y = 20.0f;
    attach.p1.z = -140.0f;
    emitter.scale = 1.0f;

    effect_attach(particle_spawnBoneGlow(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 0xA);
    light_createAttached(obj, 5, &attach.p0, 2, 300, 40, 0xFF, 0, 0, 0xFF, 0, 1, 0, 0, 0xFF, 1);
}

void func_1514D96C(Actor *obj) {
}

/** Attaches a effect_newCharacterWatcher object (type 0x13) with a zeroed 0x20-byte payload. */
void func_1514D978(Actor *obj) {
    struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
        f32 unk10;
        s32 unk14;
        s32 unk18;
        s32 unk1C;
    } payload;
    void *handle;

    payload.unk0 = 0;
    payload.unk4 = 0;
    payload.unk8 = 0;
    payload.unkC = 0;
    payload.unk14 = 0;
    payload.unk18 = 0;
    payload.unk1C = 0;
    payload.unk10 = 12.0f;

    handle = effect_newCharacterWatcher(obj, 1, sizeof(payload));
    if (handle != NULL) {
        memcpy((u8 *)handle + 0x58, &payload, sizeof(payload));
        effect_attach(handle, obj, 0x13);
    }
}

void effect_attachWatcherAndRopeRider(Actor *obj) {
    void *handle;

    func_1514D978(obj);
    handle = rope_createRider(obj, 20.0f, 0);
    effect_attach(handle, obj, 0x14);
}

/** Attaches a effect_newCharacterWatcher object (type 0x13) with a zeroed 0x1C-byte payload. */
void func_1514DA38(Actor *obj) {
    void *handle;
    struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
        s32 unk10;
        s32 unk14;
        s32 unk18;
    } payload;

    payload.unk14 = 0;
    payload.unk18 = 0;
    payload.unk0 = 0;
    payload.unk4 = 0;
    payload.unk8 = 0;
    payload.unkC = 0;
    handle = effect_newCharacterWatcher(obj, 1, sizeof(payload));
    if (handle != NULL) {
        memcpy((u8 *)handle + 0x58, &payload, sizeof(payload));
        effect_attach(handle, obj, 0x13);
    }
}

/** Sets bit 1 of obj->hiddenParts and attaches a effect_newCharacterWatcher object (type 0x13) with an 8-byte payload. */
void func_1514DAA4(Actor *obj) {
    void *handle;

    obj->hiddenParts |= 2;
    {
        s32 payload[2];

        payload[0] = 0;
        payload[1] = 0;
        handle = effect_newCharacterWatcher(obj, 1, sizeof(payload));
        if (handle != NULL) {
            memcpy((u8 *)handle + 0x58, payload, sizeof(payload));
            effect_attach(handle, obj, 0x13);
        }
    }
}

/** Attaches a effect_newCharacterWatcher object (type 0x13) without payload. */
void effect_attachCharacterWatcher(Actor *obj) {
    void *handle = effect_newCharacterWatcher(obj, 1, 0);

    if (handle != NULL) {
        effect_attach(handle, obj, 0x13);
    }
}

void func_1514DB58(Actor *obj) {
}

void effect_spawnRopeTieInDinoArena(Actor *obj) {
    if (level == LEVEL_W1_DINO_ARENA) {
        rope_spawnTie(obj);
    }
}

void func_1514DB98(Actor *obj) {
    effect_attachTankEffects(obj);
}

void effect_attachBone2Light(Actor *obj) {
    light_createAttached(obj, 2, &gBone2LightOffset, 2, 300, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, 0xFF, 1);
}

void effect_attachObjBurst(Actor *obj) {
    level_spawnObjBurst(obj, 0xFF, 1);
}

void effect_spawnFloorPointTracker(Actor *obj) {
    trail_spawnTrackerEffect(obj, 0, 0xFF, 1);
}

void effect_hidePart1(Actor *obj) {
    obj->hiddenParts |= 2;
}

void effect_hideParts4And8To10(Actor *obj) {
    obj->hiddenParts |= 0x710;
}

/** Sets obj->hiddenBones to 0x6000 and spawns part sets 0x23 and 0x44 on `obj`. */
void effect_attachHeadPartSets(Actor *obj) {
    obj->hiddenBones = 0x6000;
    collision_createPartSet(obj, 0x23, 1.0f, 0);
    collision_createPartSet(obj, 0x44, 1.0f, 0);
}

void func_1514DCF4(Actor *obj) {
    if (obj->mv != NULL) {
        obj->mv->unk94 = 1;
    }
    collision_createPartSet(obj, 0x17, 1.0f, 0);
}

void func_1514DD2C(Actor *obj) {
    effect_attachHeadPartSets(obj);
}

void func_1514DD4C(Actor *obj) {
    effect_attachHeadPartSets(obj);
}

void func_1514DD6C(Actor *obj) {
    effect_attachHeadPartSets(obj);
}

void func_1514DD8C(Actor *obj) {
    effect_attachHeadPartSets(obj);
}

void func_1514DDAC(Actor *obj) {
    effect_attachHeadPartSets(obj);
}

void func_1514DDCC(Actor *obj) {
    effect_attachHeadPartSets(obj);
}

void func_1514DDEC(Actor *obj) {
    effect_attachHeadPartSets(obj);
}

void func_1514DE0C(Actor *obj) {
    effect_attachHeadPartSets(obj);
}

void func_1514DE2C(Actor *obj) {
}

void func_1514DE38(Actor *obj) {
}

void func_1514DE44(Actor *obj) {
}

void func_1514DE50(Actor *obj) {
    if (gTaskDoneBits[0x11] & 8) { // low byte of gTaskDoneBits[0x10]
        effect_attachHeadPartSets(obj);
    }
}

void func_1514DE88(Actor *obj) {
}

void func_1514DE94(Actor *obj) {
    ModelAttachment *part;

    light_createBeam(obj, 12, &D_800A58AC, &D_800A58B8, 0.008333000354f, 2, 300,
                  0xFF, 0xFF, 0xFF, 0xFF, 0, 1, 1, 0xFF, 1);
    part = collision_createPartSet(obj, 0x3C, 1.0f, 0);
    if (gLevelLoadState == 0) {
        part->hookState = 2400;
    }
    effect_attach(light_createAttached(obj, 0, &D_800A58C4, 2, 300, 11, 0xFF, 0xFF, 0xFF, 2, 0, -1, 0, 0, 0xFF, 1), obj, 0x19);
}

/** Links `obj` to the object whose unique_id is 9 (unk65 holds that object's slot + 1). */
void effect_linkToObjectId9(Actor *obj) {
    obj->attach_char = collision_findObjectSlotById(9) + 1;
    obj->unk101 |= 0x34;
}

void effect_attachBone6GlowAndBone5Trail(Actor *obj) {
    GlowShape emitter;
    ParticleDef spawn;
    EmitterAttach attach;

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x12002;
    spawn.lifetime = 0;
    spawn.colorLevel = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0x8E;
    spawn.rgba[2] = 0x19;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.width1 = 20.0f;
    emitter.height1 = 20.0f;
    emitter.width0 = 20.0f;
    emitter.height0 = 20.0f;
    emitter.offset1 = 0.0f;
    emitter.offset0 = 0.0f;
    emitter.alpha1 = 0.5f;
    emitter.alpha0 = 0.5f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.p0.x = 35.0f;
    attach.p0.y = 27.0f;
    attach.p1.x = 35.0f;
    attach.p1.y = 27.0f;
    attach.target = obj;
    attach.bone = 6;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = -1;

    attach.p0.z = 59.0f;
    attach.p1.z = 559.0f;
    emitter.scale = 1.0f;

    effect_attach(particle_spawnBoneGlow(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 6);
    trail_attachToBone(obj, 5, -1, 0xFF, 1);
}

void effect_attachNarrowBone6GlowAndBone5Trail(Actor *obj) {
    GlowShape emitter;
    ParticleDef spawn;
    EmitterAttach attach;

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x12002;
    spawn.lifetime = 0;
    spawn.colorLevel = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0x8E;
    spawn.rgba[2] = 0x19;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.width1 = 16.0f;
    emitter.height1 = 16.0f;
    emitter.width0 = 16.0f;
    emitter.height0 = 16.0f;
    emitter.offset1 = 0.0f;
    emitter.offset0 = 0.0f;
    emitter.alpha1 = 0.5f;
    emitter.alpha0 = 0.5f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.p0.x = 35.0f;
    attach.p0.y = 27.0f;
    attach.p1.x = 35.0f;
    attach.p1.y = 27.0f;
    attach.target = obj;
    attach.bone = 6;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = -1;

    attach.p0.z = 59.0f;
    attach.p1.z = 559.0f;
    emitter.scale = 1.0f;

    effect_attach(particle_spawnBoneGlow(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 6);
    trail_attachToBone(obj, 5, -1, 0xFF, 1);
}

void effect_attachBone12GlowAndLight(Actor *obj) {
    GlowShape emitter;
    ParticleDef spawn;
    EmitterAttach attach;

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x2012;
    spawn.lifetime = 0;
    spawn.colorLevel = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0xE6;
    spawn.rgba[2] = 0;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.width1 = 45.0f;
    emitter.height0 = 45.0f;
    emitter.offset1 = 0.5f;
    emitter.offset0 = 0.5f;
    emitter.alpha0 = 0.0f;
    emitter.height1 = 55.0f;
    emitter.width0 = 25.0f;
    emitter.alpha1 = 0.8999999762f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.p0.x = 0.0f;
    attach.p0.y = 0.0f;
    attach.p1.x = 0.0f;
    attach.p1.y = 0.0f;
    attach.target = obj;
    attach.bone = 0xC;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = 2;

    attach.p0.z = 100.0f;
    attach.p1.z = 600.0f;
    emitter.scale = 1.0f;

    effect_attach(particle_spawnBoneGlow(&spawn, &emitter, &attach, 2, 3, 0, 0), obj, 7);

    {
        Vec3f offset;

        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 150.0f;
        light_createAttached(obj, 12, &offset, 2, 300, 40, 0xFF, 0xB5, 0, 0xFF, 0, 2, 0, 0, 0xFF, 1);
    }
}

/** Attaches the "Charactereffect 0" effect (type 0xD) and a light_createPointLight object tracking obj's position (type 0x18). */
void effect_attachCharacterEffect0(Actor *obj) {
    void *handle;
    s32 pos;

    handle = particle_startActorEmitter(obj, 5, 0, 1, 0xFF, 1, "Charactereffect 0");
    effect_attach(handle, obj, 0xD);
    pos = (s32)&obj->xpos; // passed as two 16-bit halves
    handle = light_createPointLight(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    effect_attach(handle, obj, 0x18);
}

void effect_attachCharacterEffectMinus1(Actor *obj) {
    void *handle;
    s32 pos;

    handle = particle_startActorEmitter(obj, 5, 0, 1, 0xFF, 1, "Charactereffect -1");
    effect_attach(handle, obj, 0xD);
    pos = (s32)&obj->xpos; // passed as two 16-bit halves
    handle = light_createPointLight(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    effect_attach(handle, obj, 0x18);
}

void effect_attachCharacterEffect1(Actor *obj) {
    void *handle;
    s32 pos;

    handle = particle_startActorEmitter(obj, 5, 0, 1, 0xFF, 1, "Charactereffect 1");
    effect_attach(handle, obj, 0xD);
    pos = (s32)&obj->xpos; // passed as two 16-bit halves
    handle = light_createPointLight(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    effect_attach(handle, obj, 0x18);
}

void effect_attachCharacterEffect2(Actor *obj) {
    void *handle;
    s32 pos;

    handle = particle_startActorEmitter(obj, 5, 0, 1, 0xFF, 1, "Charactereffect 2");
    effect_attach(handle, obj, 0xE);
    pos = (s32)&obj->xpos; // passed as two 16-bit halves
    handle = light_createPointLight(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    effect_attach(handle, obj, 0x18);
}

void effect_attachCharacterEffect3(Actor *obj) {
    void *handle;

    handle = particle_startActorEmitter(obj, 7, 0, 1, 0xFF, 1, "Charactereffect 3");
    effect_attach(handle, obj, 0xF);
}

/* Emitter offsets of effect_attachBone1RedLightAndGlow and effect_attachTankEffects. */
static const Vec3f D_800A5934 = { 0.0f, 90.0f, 0.0f };
static const Vec3f D_800A5940 = { 0.0f, 70.0f, 0.0f };
static const Vec3f D_800A594C = { 0.0f, 120.0f, 0.0f };
static const Vec3f gTankFrontLightOffset = { 0.0f, 112.0f, 151.0f };
static const Vec3f gTankRearLightOffset = { 0.0f, 74.0f, -161.0f };

void func_1514E824(Actor *obj) {
}

/* Attachment destroy handlers (gAttachDestroyFuncs entries): release an attachment's handle. */
void effect_releaseAttachedObject(EffectHeader *handle) {
    effect_destroyObject(handle);
}

void effect_releaseAttachedEmitter(void *handle) {
    particle_freeActorEmitterParticles(handle);
    effect_destroyObject(handle);
}

void effect_releaseAttachedPointLight(void *handle) {
    light_freePointLight(handle);
}

/* Attachment message handlers (gAttachMessageFuncs entries). Each returns 1 if it handled `msg`. */

struct ToggleTarget {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ s32 flags;
    /* 0x014 */ u8 pad14[0xFC];
    /* 0x110 */ GlowShape emitter;    // for objects spawned by particle_spawnBoneGlow
};

/** Messages 0, 1 and 2 toggle, set and clear bit 0 of the handle's flags. */
s32 effect_handleToggleMessage(ToggleTarget *target, s32 msg, s32 arg) {
    s32 flags;

    switch (msg) {
    case 0:
        flags = target->flags;
        if (flags & 1) {
            target->flags = flags & ~1;
        } else {
            target->flags = flags | 1;
        }
        break;
    case 2:
        target->flags &= ~1;
        break;
    case 1:
        target->flags |= 1;
        break;
    default:
        return 0;
    }
    return 1;
}

/** Like effect_handleToggleMessage, but clearing bit 0 also hides the glow's attached lens flare. */
s32 effect_handleGlowToggleMessage(s32 handle, s32 msg, s32 arg) {
    // matching: with an s32 parameter IDO keeps &target->unk110 in its own register
    ToggleTarget *target = (ToggleTarget *)handle;
    GlowShape *emitter;
    s32 flags;
    LensFlare *flare;

    emitter = &target->emitter;
    switch (msg) {
    case 0:
        flags = target->flags;
        if (flags & 1) {
            target->flags = flags & ~1;
            flare = emitter->attach;
            if (flare != NULL) {
                flare->alpha = 0;
            }
        } else {
            target->flags = flags | 1;
        }
        break;
    case 2:
        target->flags &= ~1;
        flare = emitter->attach;
        if (flare != NULL) {
            flare->alpha = 0;
        }
        break;
    case 1:
        target->flags |= 1;
        break;
    default:
        return effect_handleToggleMessage(target, msg, arg);
    }
    return 1;
}

s32 effect_forwardGlowToggleMessage(s32 handle, s32 msg, s32 arg) {
    return effect_handleGlowToggleMessage(handle, msg, arg);
}

s32 effect_forwardToggleMessage(ToggleTarget *target, s32 msg, s32 arg) {
    return effect_handleToggleMessage(target, msg, arg);
}

/** Messages 3..12 switch the emitter between table 2 and tables 3, 5 and 6. */
s32 effect_handleEmitterTableMessage(ActorEmitterProcess *target, s32 msg, s32 arg) {
    switch (msg) {
    case 3:
        if (target->params.table == 2) {
            particle_setActorEmitterTable(target, 3);
        } else {
            particle_setActorEmitterTable(target, 2);
        }
        break;
    case 4:
        particle_setActorEmitterTable(target, 3);
        break;
    case 5:
        particle_setActorEmitterTable(target, 2);
        break;
    case 6:
        if (target->params.table == 2) {
            particle_setActorEmitterTable(target, 5);
        } else {
            particle_setActorEmitterTable(target, 2);
        }
        break;
    case 7:
        particle_setActorEmitterTable(target, 5);
        break;
    case 8:
        particle_setActorEmitterTable(target, 2);
        break;
    case 9:
        if (target->params.table == 2) {
            particle_setActorEmitterTable(target, 6);
        } else {
            particle_setActorEmitterTable(target, 2);
        }
        break;
    case 10:
        particle_setActorEmitterTable(target, 6);
        break;
    case 11:
        particle_setActorEmitterTable(target, 2);
        break;
    case 12:
        particle_setActorEmitterTable(target, 2);
        break;
    default:
        return effect_handleToggleMessage((ToggleTarget *)target, msg, arg);
    }
    return 1;
}

s32 effect_handleRopeRiderMessage(ToggleTarget *target, s32 msg, s32 arg) {
    return effect_handleToggleMessage(target, msg, arg);
}

s32 effect_acceptAnyMessage(s32 handle, s32 msg, s32 arg) {
    return 1;
}

/** Allocates an unlinked attachment node for `handle` with `extraSize` bytes of payload. */
Attachment *effect_newAttachment(void *handle, s16 type, s32 extraSize) {
    Attachment *node;

    node = effect_newObject(0x24, 1, extraSize + 0x20, 1, 0xFF, 1);
    if (node == NULL) {
        return NULL;
    }
    node->handle = handle;
    node->next = NULL;
    node->prev = NULL;
    node->type = type;
    return node;
}

/**
 * Attaches `handle` to `owner` as an attachment of `type` (pushed at the head of its list).
 * If no node can be allocated, the handle is released instead. Returns the node or NULL.
 */
Attachment *effect_attach(void *handle, Actor *owner, s16 type) {
    Attachment *result;
    Attachment *node;

    result = NULL;
    if (handle != NULL) {
        node = effect_newAttachment(handle, type, 0);
        result = node;
        if (node != NULL) {
            node->next = owner->attachments;
            owner->attachments = node;
            node->prev = NULL;
            if (node->next != NULL) {
                node->next->prev = node;
            }
        } else if (gAttachmentTypes[type]->destroyHandler != -1) {
            gAttachDestroyFuncs[gAttachmentTypes[type]->destroyHandler](handle);
        } else {
            effect_destroyObject(handle);
        }
    }
    return result;
}

/** Finds the first attachment of `type` in the list starting at `list`; stores it (or NULL) in *out. */
s32 effect_findAttachmentByType(Attachment *list, s16 type, Attachment **out) {
    s32 found;
    Attachment *node;
    Attachment *next;

    found = FALSE;
    node = list;
    if (node != NULL) {
        do {
            next = node->next;
            if (type == node->type) {
                found = TRUE;
            } else {
                node = next;
            }
        } while (node != NULL && !found);
    }

    if (out != NULL) {
        *out = node;
    }
    return found;
}

/** Finds the first attachment holding `handle` in the list starting at `list`; stores it (or NULL) in *out. */
s32 effect_findAttachmentByHandle(Attachment *list, void *handle, Attachment **out) {
    s32 found = FALSE;
    Attachment *node = list;
    Attachment *next;

    if (node != NULL) {
        do {
            next = node->next;
            if (handle == node->handle) {
                found = TRUE;
            } else {
                node = next;
            }
        } while (node != NULL && !found);
    }
    if (out != NULL) {
        *out = node;
    }
    return found;
}

/** Unlinks `node` from `owner`'s attachment list and frees it. Returns the node's handle. */
void *effect_removeAttachment(Attachment *node, Actor *owner) {
    void *handle;

    if (node == owner->attachments) {
        owner->attachments = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    handle = node->handle;
    effect_destroyObject(&node->header);
    return handle;
}

/** Removes (without releasing the handle) every attachment of `owner` that holds `handle`. */
void effect_detachHandle(void *handle, Actor *owner) {
    Attachment *node;
    s32 ret;
    Attachment *match;
    u8 found;

    node = owner->attachments;
    match = NULL;
    do {
        // matching: test the s32 result, loop on the u8 copy
        ret = effect_findAttachmentByHandle(node, handle, &match);
        found = ret;
        if (ret) {
            node = match->next;
            effect_removeAttachment(match, owner);
        }
    } while (found);
}

/** Attaches a sprite_newActorTracker object for `obj` (type 0x16). */
void effect_attachActorTracker(Actor *obj) {
    ActorTrackerDesc desc;
    ActorTracker *handle;

    desc.actor = obj;
    desc.actorId = obj->unique_id;
    desc.flags = 0;
    desc.lifetime = 300;
    handle = sprite_newActorTracker(&desc, 0, 0xFF, 1);
    effect_attach(handle, obj, 0x16);
}


void effect_attachBone1RedLightAndGlow(Actor *obj) {
    void *handle;

    handle = light_createAttached(obj, 1, &D_800A5934, 2, 300, 2, 0xFF, 0, 0, 0xFF, 0, 3, 0, 0, 0xFF, 1);
    effect_attach(handle, obj, 0x19);
    {
        ParticleDef spawn;
        GlowShape emitter;
        EmitterAttach attach;

        spawn.texture = 0x69;
        spawn.flags = 0x04ED200A;
        spawn.lifetime = 300;
        spawn.frame = 0;
        spawn.frameSpeed = 0;
        spawn.colorLevel = 0xFF;
        spawn.rgba[0] = 0xFF;
        spawn.rgba[1] = 0;
        spawn.rgba[2] = 0;
        spawn.rgba[3] = 0xFF;
        spawn.renderModes = 0x4403;
        spawn.pos.x = 0.0f;
        spawn.pos.y = 0.0f;
        spawn.pos.z = 0.0f;
        spawn.rot.x = 0.0f;
        spawn.rot.y = 0.0f;
        spawn.rot.z = 0.0f;
        spawn.type = 3;
        spawn.height = 100.0f;
        spawn.width = 100.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.alpha = 0xFF;
        spawn.primMode = 0;
        spawn.envMode = 6;
        spawn.zone = 0;
        spawn.viewportMask = 0xFF;
        spawn.material = 0;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;

        emitter.height1 = 95.0f;
        emitter.width1 = 95.0f;
        emitter.height0 = 65.0f;
        emitter.width0 = 65.0f;
        emitter.offset1 = 0.150000006f;
        emitter.offset0 = 0.150000006f;
        emitter.alpha1 = 0.6999999881f;
        emitter.alpha0 = 0.400000006f;
        emitter.fadeRate = 9.999999975e-07f;

        attach.targetId = obj->unique_id;
        attach.target = obj;
        attach.p0 = D_800A5940;
        attach.p1 = D_800A594C;
        attach.bone = 1;
        emitter.visibility[0] = 0;
        emitter.visibility[1] = 0;
        emitter.visibility[2] = 0;
        emitter.visibility[3] = 0;
        emitter.scale = 1.0f;
        attach.handler = 3;

        particle_spawnBoneGlow(&spawn, &emitter, &attach, 2, 0, 0, 0);
    }
}

void func_1514F110(Actor *obj) {
    effect_attachTankEffects(obj);
}

/* Attachment message handler for a light effect (e.g. light_createAttached's): 13 turns the light on, 14 off. */
s32 effect_handleLightMessage(LightEffect *target, s32 msg, s32 arg) {
    switch (msg) {
    case 13:
        target->light->disabled = 0;
        break;
    case 14:
        target->light->disabled = 1;
        break;
    default:
        return effect_handleToggleMessage((ToggleTarget *)target, msg, arg);
    }
    return 1;
}

void effect_attachTankEffects(Actor *obj) {
    void *handle;

    trail_spawnTrackerEffect(obj, 1, 0xFF, 1);
    trail_spawnTrackerEffect(obj, 2, 0xFF, 1);
    effect_startTankSound(obj);
    if (gIsMultiplayer == 0) {
        effect_attachTankHeadlightBeam(obj);
    }
    effect_spawnBloodStrips(obj, 0xFF, 1);
    effect_spawnOwnerModel(obj, 0xFF, 1);
    light_createAttached(obj, 1, &gTankFrontLightOffset, 2, 300, 12, 0xFF, 0xFF, 0xFF, 2, 0, -1, 0, 0, 0xFF, 1);
    handle = light_createAttached(obj, 1, &gTankRearLightOffset, 2, 300, 15, 0xFF, 0, 0, 0xFF, 1, -1, 0, 0, 0xFF, 1);
    if (handle != NULL) {
        effect_attach(handle, obj, 0x19);
    }
}

void effect_attachBone5LightBeam(Actor *obj) {
    const Vec3f pointA = { 52.0f, 0.0f, 80.0f };
    const Vec3f pointB = { 52.0f, 0.0f, 353.0f };

    light_createBeam(obj, 5, &pointA, &pointB, 0.02350199968f, 2, 300,
                  0xFF, 0xFF, 0xFF, 0xFF, 1, 0, 0, 0xFF, 1);
}

void effect_spawnActorEmitter(Actor *obj) {
    struct {
        Actor *obj;
        u8 uniqueId;
        f32 unk8;
    } payload;
    TimedEffect *spawned;

    payload.obj = obj;
    payload.uniqueId = obj->unique_id;
    payload.unk8 = 0.0f;

    // the seventh argument is the payload size
    spawned = effect_newTimedEffect(300, -1, 0x3A, -1, 0, 0x2B, sizeof(payload), 0xFF, 1);
    if (spawned != NULL) {
        memcpy((u8 *)spawned + 0x28, &payload, sizeof(payload));
    }
}

void effect_attachFrontendFireImpEffect(Actor *obj) {
    void *handle;

    handle = particle_startActorEmitter(obj, 5, 0, 1, 0xFF, 1, "Charactereffect frontend fire imp");
    effect_attach(handle, obj, 0xD);
}

void effect_attachTrailToBone1(Actor *obj) {
    trail_attachToBone(obj, 1, -1, 0xFF, 1);
}

void effect_attachTrailToBone2(Actor *obj) {
    trail_attachToBone(obj, 2, 1, 0xFF, 1);
}

void effect_attachTrailToBone3(Actor *obj) {
    trail_attachToBone(obj, 3, 0, 0xFF, 1);
}

void effect_attachTrailToBone4(Actor *obj) {
    trail_attachToBone(obj, 4, -1, 0xFF, 1);
}

void effect_attachPulseEffect(Actor *obj) {
    effect_spawnPulseEffect(obj, -1, 0xFF, 1);
}

void func_1514F590(Actor *obj) {
    level_spawnModelBeam(obj, 0xFF, 1);
    level_spawnFlashBurst(obj, 0xFF, 1);
}

void effect_spawnActorPoolEmitter(Actor *obj) {
    PoolEmitterDesc desc;

    desc.source = NULL;
    desc.target = obj;
    desc.unk4 = 0.0f;
    desc.targetId = obj->unique_id;
    desc.rate = 0.349999994f;
    desc.slotCount = 20;
    desc.timer = 300;
    desc.flags = 4;
    particle_newPoolEmitter(&desc, 0xFF, 1);
}
