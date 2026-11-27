#include <ultra64.h>
#include "structs.h"

/*
 * This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). Head/tail of the texture-scroll
 * reference list (TexScrollRef) and the texture-scroll list (TexScroll).
 */
static struct ListNode *gTexScrollRefHead;
static struct ListNode *gTexScrollRefTail;
static struct ListNode *gTexScrollHead;
static struct ListNode *gTexScrollTail;
/* Next automatic (negative) texture-scroll id. */
static s32 gNextAutoTexScrollId;
static s32 D_800E08F4;    /* not referenced */
static s32 D_800E08F8[2]; /* not referenced */
/* The trigger zones of the flamethrower characters, by slot (game/Level/ZoneSpawn/zonespawn.c registers them). */
TriggerZone *gFlamethrowerZones[6];

#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/flamethrower.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/mp.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "conker/sprite.h"
#include "conker/swarm.h"
#include "conker/texture.h"
#include "conker/trail.h"
#include "conker/view.h"
#include "libc/string.h"



/* Generic doubly linked list node; every list allocated by flamethrower_appendListNode starts with one. */
struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
};


/* The opcode byte of a display-list command. */
#define DL_OPCODE(cmd) (*(s8 *)&(cmd))

/* A G_SETTILESIZE run inside some display list that a TexScroll rewrites every frame. */
typedef struct TexScrollRef {
    /* 0x00 */ ListNode link;
    /* 0x08 */ Gfx *dl;
    /* 0x0C */ u16 cmdIndex;
    /* 0x0E */ u8 count;
    /* 0x0F */ u8 pad0F;
    /* 0x10 */ struct TexScrollRef *next;
} TexScrollRef;

/* A scrolling texture: patches the G_SETTILESIZE commands that follow its G_SETTIMG. */
struct TexScroll {
    /* 0x00 */ struct TexScroll *prev;
    /* 0x04 */ struct TexScroll *next;
    /* 0x08 */ s16 id;
    /* 0x0A */ s16 width;
    /* 0x0C */ s16 height;
    /* 0x0E */ u16 scrollS;
    /* 0x10 */ u16 scrollT;
    /* 0x12 */ s8 speedS;
    /* 0x13 */ s8 speedT;
    /* 0x14 */ u8 levelCount;
    /* 0x15 */ char pad15[0x3];
    /* 0x18 */ TexScrollRef *refs;
    /* 0x1C */ s16 cmdIndex[2][5]; /* per main display list (gLevelDisplayLists[0] / [1]), -1 = unused */
    /* 0x30 */ u8 cmdCount[2][5];
};

/* Allocates a `size`-byte list node and appends it to the list (*head, *tail). */
void *flamethrower_appendListNode(s32 size, ListNode **head, ListNode **tail) {
    ListNode *node;
    ListNode *last;

    node = allocate_memory(size, 1, 0, 0);
    if (node != NULL) {
        node->next = NULL;
        last = *tail;
        if (last != NULL) {
            node->prev = last;
            (*tail)->next = node;
            *tail = node;
        } else {
            node->prev = NULL;
            *tail = node;
            *head = node;
        }
    }
    return node;
}

void trailing_151957B0(void) {
}

/* Empties both texture-scroll lists. */
void flamethrower_clearTexScrolls(void) {
    gTexScrollRefHead = gTexScrollRefTail = NULL;
    gTexScrollHead = gTexScrollTail = NULL;
    gNextAutoTexScrollId = -2;
}

/*
 * Finds the `occurrence`-th G_SETTIMG in `dl` that loads texture gTextureAddrs[texture] (any texture when
 * `texture` is 0). Returns the index of the G_SETTILESIZE that follows it, or -1, and stores in *levels how
 * many (G_SETTILESIZE, G_SETTILE) pairs follow.
 */
s32 flamethrower_findTileSizeCmd(Gfx *dl, s32 texture, s32 occurrence, s32 *levels) {
    s32 i;
    s32 c;
    s8 op;
    s8 nextOp;

    *levels = 0;
    i = -1;
    for (;;) {
        i++;
        while (DL_OPCODE(dl[i]) != (s8)G_SETTIMG && DL_OPCODE(dl[i]) != (s8)G_ENDDL) {
            i++;
        }
        if (DL_OPCODE(dl[i]) == (s8)G_ENDDL) {
            return -1;
        }
        if ((dl[i].words.w1 == gTextureAddrs[texture] || texture == 0) && occurrence-- <= 0) {
            break;
        }
    }
    op = DL_OPCODE(dl[i]);
    while (op != (s8)G_SETTILESIZE) {
        i++;
        op = DL_OPCODE(dl[i]);
    }
    c = i;
    do {
        (*levels)++;
        op = DL_OPCODE(dl[++c]);
        nextOp = DL_OPCODE(dl[++c]);
    } while (op == (s8)G_SETTILE && nextOp == (s8)G_SETTILESIZE);
    return i;
}

/* Allocates a TexScroll for texture `id`; `size`/`pos` are the w1/w0 words of its G_SETTILESIZE. */
TexScroll *flamethrower_newTexScroll(s32 id, s32 size, s32 pos, s32 doubled) {
    TexScroll *scroll;
    s32 i;
    s32 width;
    s32 height;

    scroll = flamethrower_appendListNode(sizeof(TexScroll), &gTexScrollHead, &gTexScrollTail);
    if (scroll != NULL) {
        scroll->refs = NULL;
        for (i = 0; i < 5; i++) {
            scroll->cmdIndex[0][i] = -1;
        }
        for (i = 0; i < 5; i++) {
            scroll->cmdIndex[1][i] = -1;
        }
        scroll->id = id;
        width = ((size >> 12) & 0xFFF) + 2;
        height = (size & 0xFFF) + 2;
        if (doubled) {
            width <<= 1;
            height <<= 1;
        }
        scroll->scrollS = (pos >> 12) & 0xFFF;
        scroll->scrollT = pos & 0xFFF;
        scroll->width = width;
        scroll->height = height;
        scroll->scrollS = scroll->scrollS << 3;
        scroll->scrollT = scroll->scrollT << 3;
        scroll->levelCount = 1;
    }
    return scroll;
}

/* Returns `cmdIndex` unchanged. */
s16 flamethrower_returnCmdIndex(s16 cmdIndex, TexScroll *scroll, s32 size, s32 doubled) {
    return cmdIndex;
}

/*
 * Registers every (or the `occurrence`-th, when not -1) use of texture `texture` in main display list `which`
 * of `dl` with a TexScroll scrolling by (speedS, speedT).
 */
TexScroll *flamethrower_addTexScroll(Gfx *dl, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 which, s32 speedS,
                         s32 speedT) {
    s32 idx;
    s32 slot;
    s32 size;
    s32 levels;
    s32 pos;
    s32 i = occurrence;
    TexScroll *scroll = NULL;

    if (dl == NULL) {
        return NULL;
    }
    if (occurrence == -1) {
        i = 0;
    }
    do {
        idx = flamethrower_findTileSizeCmd(dl, texture, i, &levels);
        if (idx == -1) {
            occurrence = 0;
        } else {
            if (texture == 0) {
                texture = gNextAutoTexScrollId;
            }
            size = dl[idx].words.w1;
            pos = dl[idx].words.w0;
            scroll = (TexScroll *)gTexScrollHead;
            // matching: dead store left over from flamethrower_addTexScrollInDl's lookup slot; removing it moves branches
            if (isNew) {
                slot = -1;
            } else {
                slot = texture;
            }
            while (scroll != NULL && scroll->id != 0) {
                scroll = scroll->next;
            }
            if (scroll == NULL) {
                scroll = flamethrower_newTexScroll(texture, size, pos, doubled);
                if (scroll != NULL) {
                    isNew = FALSE;
                    scroll->speedS = speedS;
                    scroll->speedT = speedT;
                } else {
                    return NULL;
                }
            } else {
                idx = flamethrower_returnCmdIndex(idx, scroll, size, doubled);
                isNew = FALSE;
            }
            if (scroll->levelCount < levels) {
                scroll->levelCount = levels;
            }
            if (which == 0) {
                slot = 0;
                while (slot < 5 && scroll->cmdIndex[0][slot] != -1) {
                    slot++;
                }
                if (slot < 5) {
                    scroll->cmdIndex[0][slot] = idx;
                    scroll->cmdCount[0][slot] = levels;
                }
            } else {
                slot = 0;
                while (slot < 5 && scroll->cmdIndex[1][slot] != -1) {
                    slot++;
                }
                if (slot < 5) {
                    scroll->cmdIndex[1][slot] = idx;
                    scroll->cmdCount[1][slot] = levels;
                }
            }
            i++;
        }
    } while (occurrence == -1);
    if (texture == gNextAutoTexScrollId) {
        gNextAutoTexScrollId--;
    }
    return scroll;
}

/*
 * Adds a reference to the `count` tile-size commands at dl[cmdIndex] to `scroll`, unless already present
 * (the walk never compares the last reference).
 */
void flamethrower_addTexScrollRef(TexScroll *scroll, Gfx *dl, s32 cmdIndex, s32 count) {
    TexScrollRef *ref;
    TexScrollRef *next;

    next = scroll->refs;
    if (next != NULL) {
        ref = next;
        do {
            if (dl == ref->dl && cmdIndex == ref->cmdIndex) {
                return;
            }
            next = ref->next;
            if (next != NULL) {
                ref = next;
                next = next->next;
            }
        } while (next != NULL);
        next = flamethrower_appendListNode(sizeof(TexScrollRef), &gTexScrollRefHead, &gTexScrollRefTail);
        ref->next = next;
    } else {
        next = flamethrower_appendListNode(sizeof(TexScrollRef), &gTexScrollRefHead, &gTexScrollRefTail);
        scroll->refs = next;
    }
    next->dl = dl;
    next->cmdIndex = cmdIndex;
    next->count = count;
    next->next = NULL;
}

/* Like flamethrower_addTexScroll, but records the uses in an arbitrary display list `dl` (as TexScrollRefs). */
TexScroll *flamethrower_addTexScrollInDl(Gfx *dl, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS,
                         s32 speedT) {
    s32 idx;
    s32 key;
    s32 size;
    s32 levels;
    s32 pos;
    s32 i = occurrence;
    TexScroll *scroll;

    if (dl == NULL) {
        return NULL;
    }
    if (gPaused != 0) {
        return NULL;
    }
    if (occurrence == -1) {
        i = 0;
    }
    do {
        idx = flamethrower_findTileSizeCmd(dl, texture, i, &levels);
        if (idx == -1) {
            occurrence = 0;
        } else {
            if (texture == 0) {
                texture = gNextAutoTexScrollId;
            }
            if (isNew) {
                key = -1;
            } else {
                key = texture;
            }
            scroll = (TexScroll *)gTexScrollHead;
            while (scroll != NULL && key != scroll->id) {
                scroll = scroll->next;
            }
            size = dl[idx].words.w1;
            pos = dl[idx].words.w0;
            if (scroll == NULL) {
                scroll = flamethrower_newTexScroll(texture, size, pos, doubled);
                if (scroll != NULL) {
                    isNew = FALSE;
                    scroll->speedS = speedS;
                    scroll->speedT = speedT;
                } else {
                    return NULL;
                }
            } else {
                idx = flamethrower_returnCmdIndex(idx, scroll, size, doubled);
                isNew = FALSE;
            }
            if (scroll->levelCount < levels) {
                scroll->levelCount = levels;
            }
            flamethrower_addTexScrollRef(scroll, dl, idx, levels);
            i++;
        }
    } while (occurrence == -1);
    if (texture == gNextAutoTexScrollId) {
        gNextAutoTexScrollId--;
    }
    return scroll;
}

TexScroll *flamethrower_addObjectTexScroll(WorldObject *obj, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS,
                         s32 speedT) {
    return flamethrower_addTexScrollInDl(obj->displayList, texture, doubled, occurrence, isNew, speedS, speedT);
}

/* Advances every TexScroll and rewrites the G_SETTILESIZE commands it is registered in. */
void flamethrower_updateTexScrolls(Gfx *dl0, Gfx *dl1) {
    TexScroll *scroll;
    u32 tileSize[8];
    TexScrollRef *ref;
    s32 uls;
    s32 ult;
    s32 w;
    s32 h;
    s32 x;
    s32 y;
    s32 i;
    s32 k;
    s32 idx;

    scroll = (TexScroll *)gTexScrollHead;
    while (scroll != NULL) {
        w = scroll->width;
        h = scroll->height;
        x = scroll->scrollS;
        y = scroll->scrollT;
        uls = x / 8 + 2;
        ult = y / 8 + 2;
        if (uls >= w) {
            uls = uls - w;
        }
        if (ult >= h) {
            ult = ult - h;
        }
        for (i = 0; i < 8 && i < scroll->levelCount; i++) {
            tileSize[i] = (G_SETTILESIZE << 24) | ((uls & 0xFFF) << 12) | (ult & 0xFFF);
            uls /= 2;
            ult /= 2;
        }
        w *= 8;
        h *= 8;
        if (gPaused == 0) {
            x += scroll->speedS * gFrameTicks;
            y += scroll->speedT * gFrameTicks;
        }
        if (x < 0) {
            x += w;
        } else if (x >= w) {
            x -= w;
        }
        if (y < 0) {
            y += h;
        } else if (y >= h) {
            y -= h;
        }
        scroll->scrollS = x;
        scroll->scrollT = y;
        for (k = 0; k < 5 && scroll->cmdIndex[0][k] != -1; k++) {
            idx = scroll->cmdIndex[0][k];
            for (i = 0; i < scroll->cmdCount[0][k]; i++) {
                dl0[idx].words.w0 = tileSize[i];
                idx += 2;
            }
        }
        for (k = 0; k < 5 && scroll->cmdIndex[1][k] != -1; k++) {
            idx = scroll->cmdIndex[1][k];
            for (i = 0; i < scroll->cmdCount[1][k]; i++) {
                dl1[idx].words.w0 = tileSize[i];
                idx += 2;
            }
        }
        ref = scroll->refs;
        while (ref != NULL) {
            idx = ref->cmdIndex;
            for (i = 0; i < ref->count; i++, idx += 2) {
                ref->dl[idx].words.w0 = tileSize[i];
            }
            if (ref->next != NULL) {
                ref = ref->next;
            } else {
                ref = NULL;
            }
        }
        scroll = scroll->next;
    }
}

/* Sets the scroll speed of a TexScroll. */
void flamethrower_setTexScrollSpeed(TexScroll *scroll, s32 speedS, s32 speedT) {
    if (scroll != NULL) {
        scroll->speedS = speedS;
        scroll->speedT = speedT;
    }
}

/* One point of an effect's ring buffer (0x24 bytes). */
typedef struct JetPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 size;
    /* 0x1C */ s16 life;
    /* 0x1E */ s8 holdTimer;
    /* 0x1F */ u8 alpha;
    /* 0x20 */ u8 texS; /* texture s offset */
    /* 0x21 */ u8 spawnedChild;
    /* 0x22 */ u8 pad22[0x2];
} JetPoint;

/* Effect object created by effect_newEffectObj: a ring buffer of points plus a kind-specific state block. */
struct CharacterFlamethrowerActor {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 buffers[0x10];
    /* 0x94 */ void *points;
    /* 0x98 */ void *state;
};

/* Preset in gJetTemplates that flamethrower_spawnJet copies into a CharacterFlamethrowerJet. */
struct CharacterFlamethrowerJetTemplate {
    /* 0x00 */ s16 lifetime;
    /* 0x04 */ f32 gravity;
    /* 0x08 */ f32 emitRate;
    /* 0x0C */ u8 shade;
    /* 0x10 */ s32 id;
    /* 0x14 */ u8 bone;
    /* 0x18 */ Vec3f startOffset;
    /* 0x24 */ Vec3f endOffset;
    /* 0x30 */ s8 speedFn;
    /* 0x31 */ u8 fadeRate;
    /* 0x32 */ u8 life;
    /* 0x33 */ u8 holdTime;
    /* 0x34 */ f32 sizeRate;
    /* 0x38 */ f32 size;
    /* 0x3C */ s8 initFn;
    /* 0x3D */ s8 updateFn;
    /* 0x3E */ s8 destroyFn;
    /* 0x3F */ s8 soundFn;
    /* 0x40 */ s8 soundUpdateFn;
    /* 0x41 */ s8 soundStopFn;
    /* 0x42 */ s8 unk42;
    /* 0x44 */ u16 kind;
    /* 0x46 */ u8 childPreset;
    /* 0x47 */ u8 childLife;
    /* 0x48 */ f32 childRate;
    /* 0x4C */ u8 scatterPreset;
    /* 0x50 */ f32 scatterRate;
    /* 0x54 */ u8 spritePreset;
    /* 0x58 */ f32 spriteRate;
    /* 0x5C */ u8 headFadeCount;
    /* 0x5D */ u8 headFadeStep;
    /* 0x5E */ u8 tailFadeCount;
    /* 0x5F */ u8 tailFadeStep;
};

/* State block (0x90 bytes) of a jet effect: emits particles between two bone-relative points. */
typedef struct CharacterFlamethrowerJet {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id */
    /* 0x05 */ u8 bone;
    /* 0x06 */ u8 flags;
    /* 0x07 */ s8 speedFn; /* gJetSpeedProcs index, -1 = none */
    /* 0x08 */ Vec3f startOffset;
    /* 0x14 */ Vec3f endOffset;
    /* 0x20 */ Vec3f start;
    /* 0x2C */ Vec3f end;
    /* 0x38 */ f32 speed;
    /* 0x3C */ f32 timeCarry;
    /* 0x40 */ f32 gravity;
    /* 0x44 */ f32 targetSpeed;
    /* 0x48 */ f32 emitRate;
    /* 0x4C */ f32 emitAccum;
    /* 0x50 */ u8 texS; /* texture s offset given to the next point */
    /* 0x51 */ u8 shade; /* ribbon grey level */
    /* 0x52 */ u8 fadeRate;
    /* 0x53 */ u8 life;
    /* 0x54 */ u8 holdTime;
    /* 0x58 */ f32 sizeRate;
    /* 0x5C */ f32 size;
    /* 0x60 */ s8 initFn;    /* gJetInitProcs index */
    /* 0x61 */ s8 updateFn;  /* gJetUpdateProcs index */
    /* 0x62 */ s8 destroyFn; /* gJetDestroyProcs index */
    /* 0x63 */ s8 soundFn;   /* gJetSoundStartProcs index: starts the sound */
    /* 0x64 */ s8 soundUpdateFn; /* gJetSoundUpdateProcs index */
    /* 0x65 */ s8 soundStopFn;   /* gJetSoundStopProcs index */
    /* 0x66 */ u16 sound;
    /* 0x68 */ s8 unk68;     /* gFlamethrowerJetProcs index */
    /* 0x69 */ u8 childPreset; /* gJetChildPresets index */
    /* 0x6A */ u8 childLife; /* points whose life drops below this spawn a child (flamethrower_shedJetLines) */
    /* 0x6C */ f32 childRate;
    /* 0x70 */ f32 childAccum;
    /* 0x74 */ u8 scatterPreset; /* gJetScatterPresets index (flamethrower_scatterParticles) */
    /* 0x78 */ f32 scatterRate;
    /* 0x7C */ f32 scatterAccum;
    /* 0x80 */ u8 spritePreset; /* gJetSpritePresets index (flamethrower_emitSprites) */
    /* 0x84 */ f32 spriteRate;
    /* 0x88 */ f32 spriteAccum;
    /* 0x8C */ u8 headFadeCount;
    /* 0x8D */ u8 headFadeStep;
    /* 0x8E */ u8 tailFadeCount;
    /* 0x8F */ u8 tailFadeStep;
} CharacterFlamethrowerJet;


/* Jet callback tables, indexed by the CharacterFlamethrowerJet *Fn fields (-1 = none). */
f32 (*gJetSpeedProcs[])(void *, s32) = {
    (f32 (*)(void *, s32))flamethrower_getSlowJetSpeed,
    (f32 (*)(void *, s32))flamethrower_getPowerTunable,
    (f32 (*)(void *, s32))flamethrower_getFastJetSpeed,
};
void (*gJetInitProcs[])(void *) = { (void (*)(void *))flamethrower_activateJetZone };
s32 (*gJetUpdateProcs[])(void *) = {
    (s32 (*)(void *))flamethrower_hitPlayer,
    (s32 (*)(void *))flamethrower_burnSwarm,
    (s32 (*)(void *))flamethrower_hitObjects,
};
void (*gJetDestroyProcs[])(void *) = { (void (*)(void *))flamethrower_deactivateJetZone };
s16 (*gJetSoundStartProcs[])(void) = { (s16 (*)(void))flamethrower_startJetSound };
void (*gJetSoundUpdateProcs[])(void *) = { (void (*)(void *))flamethrower_moveJetSound };
void (*gJetSoundStopProcs[])(void *) = { (void (*)(void *))flamethrower_stopJetSound };
void (*gFlamethrowerJetProcs[])(void *) = { (void (*)(void *))flamethrower_emptyJetProc };

/* Runs a jet's destroy callbacks, then effect_defaultDestroyEffectObj. */
void flamethrower_destroyJet(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;

    jet = actor->state;
    if (jet->soundStopFn != -1) {
        gJetSoundStopProcs[jet->soundStopFn](actor);
    }
    if (jet->destroyFn != -1) {
        gJetDestroyProcs[jet->destroyFn](actor);
    }
    effect_defaultDestroyEffectObj(actor);
}

/* Runs a jet's destroy callbacks, then effect_defaultFreeEffectObj. */
void flamethrower_freeJet(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;

    jet = actor->state;
    if (jet->soundStopFn != -1) {
        gJetSoundStopProcs[jet->soundStopFn](actor);
    }
    if (jet->destroyFn != -1) {
        gJetDestroyProcs[jet->destroyFn](actor);
    }
    effect_defaultFreeEffectObj(actor);
}

/* Preset in gJetChildPresets for the child particles a jet point spawns (flamethrower_shedJetLines). */
struct JetChildPreset {
    /* 0x0 */ f32 sizeScale;
    /* 0x4 */ f32 unk4;
    /* 0x8 */ s16 lifeMin;
    /* 0xA */ s16 lifeRange;
};

/* Sprite preset in gJetSpritePresets (flamethrower_emitSprites). */
struct JetSpritePreset {
    f32 radius;
    f32 speed;
    f32 speed_range;
    s16 acceleration;
    s16 acceleration_range;
    s16 fade;
    s16 fade_range;
    s16 size;
    s16 size_range;
};

const JetChildPreset gJetChildPresets[] = {
    { 1.0f, 1.0f, 9, 12 },
    { 0.656f, 1.3f, 5, 12 },
};

const ScatterPreset gJetScatterPresets[] = {
    { 20.0f, 0.114f, 0.506f, 5.0f, 12.0f, 0.0f, 0.47f, 55, 10, 3, 7 },
    { 20.0f, 0.5f, 0.8f, 5.0f, 5.0f, 1.0f, 1.0f, 17, 10, 3, 2 },
};

const JetSpritePreset gJetSpritePresets[] = {
    { 45.19f, 40.0f, 25.0f, -314, 186, 8, 11, 385, 386 },
    { 40.0f, 30.0f, 20.0f, -100, 70, 10, 20, 200, 150 },
};

const CharacterFlamethrowerJetTemplate gJetTemplates[] = {
    { 600, 1.141f, 1.04f, 255, 20, 12, { 0.0f, 0.0f, 80.0f }, { 0.0f, -2.0f, 102.0f }, 0, 30, 155, 20, 4.8f, 17.0f, -1, 0, -1, -1, -1, -1, 0, 2, 0, 127, 0.726f, 0, 0.226f, 0, 0.135f, 6, 42, 6, 42 },
    { 30, 0.1f, 0.49f, 255, 24, 0, { 0.0f, 40.0f, 8.0f }, { 0.0f, 40.0f, 10.0f }, 2, 192, 255, 8, 1.2f, 0.9f, -1, -1, -1, -1, -1, -1, -1, 3, 0, 127, 0.726f, 0, 0.226f, 0, 0.135f, 6, 42, 6, 42 },
    { 30, 0.1f, 0.49f, 255, 24, 0, { 0.0f, 40.0f, 8.0f }, { 0.0f, 40.0f, 10.0f }, 2, 192, 255, 8, 1.2f, 0.9f, -1, -1, -1, -1, -1, -1, -1, 3, 0, 127, 0.726f, 0, 0.226f, 0, 0.135f, 6, 42, 6, 42 },
    { 600, 1.141f, 1.04f, 255, 20, 13, { 22.0f, 22.0f, 165.0f }, { 22.0f, 22.0f, 180.0f }, 0, 40, 255, 19, 4.8f, 17.0f, 0, -1, 0, 0, 0, 0, -1, 2, 0, 127, 0.726f, 0, 0.226f, 0, 0.135f, 6, 42, 6, 42 },
    { 600, 0.0f, 2.14f, 255, 20, 9, { -4.734001f, -111.957596f, 33.473396f }, { -8.837997f, -159.75f, 33.473396f }, 0, 12, 200, 0, 4.0f, 9.0f, -1, -1, -1, -1, -1, -1, 0, 2, 1, 100, 0.8f, 1, 0.15f, 1, 0.2f, 6, 42, 6, 42 },
    { 600, 0.0f, 2.14f, 255, 20, 9, { -4.734001f, -111.957596f, 33.473396f }, { -8.837997f, -159.75f, 33.473396f }, 0, 12, 200, 0, 4.0f, 9.0f, -1, 1, -1, -1, -1, -1, 0, 2, 1, 100, 0.8f, 1, 0.15f, 1, 0.2f, 6, 42, 6, 42 },
    { 600, 0.0f, 2.14f, 255, 20, 9, { 12.0f, -44.0f, 136.0f }, { 12.0f, -54.0f, 175.0f }, 0, 12, 200, 0, 4.0f, 9.0f, -1, 2, -1, 0, 0, 0, 0, 2, 1, 100, 0.8f, 1, 0.15f, 1, 0.2f, 6, 42, 6, 42 },
};

/* Spawns a jet effect from preset gJetTemplates[preset], attached to a bone of `owner`. */
CharacterFlamethrowerActor *flamethrower_spawnJet(Actor *owner, s32 preset, u8 arg2, s32 arg3) {
    EffectTemplate spawn;
    CharacterFlamethrowerJet jet;
    CharacterFlamethrowerActor *actor;
    MtxF *matrix;
    const CharacterFlamethrowerJetTemplate *entry;
    CharacterFlamethrowerJet *state;

    if (owner == NULL) {
        return NULL;
    }
    if (owner->ob_matrix_ptr == NULL) {
        return NULL;
    }
    entry = &gJetTemplates[preset];
    jet.owner = owner;
    jet.ownerId = owner->unique_id;
    jet.flags = 2;
    jet.texS = 0;
    jet.emitAccum = 0.0f;
    jet.childAccum = 0.0f;
    jet.scatterAccum = 0.0f;
    jet.spriteAccum = 0.0f;
    jet.initFn = entry->initFn;
    jet.updateFn = entry->updateFn;
    jet.destroyFn = entry->destroyFn;
    spawn.life = entry->lifetime;
    jet.gravity = entry->gravity;
    jet.emitRate = entry->emitRate;
    jet.shade = entry->shade;
    spawn.itemCount = entry->id;
    jet.bone = entry->bone;
    jet.startOffset = entry->startOffset;
    jet.endOffset = entry->endOffset;
    jet.speedFn = entry->speedFn;
    jet.fadeRate = entry->fadeRate;
    jet.life = entry->life;
    jet.holdTime = entry->holdTime;
    spawn.flags = entry->kind;
    jet.childLife = entry->childLife;
    jet.scatterPreset = entry->scatterPreset;
    jet.spritePreset = entry->spritePreset;
    jet.soundFn = entry->soundFn;
    jet.sizeRate = entry->sizeRate;
    jet.size = entry->size;
    jet.childRate = entry->childRate;
    jet.scatterRate = entry->scatterRate;
    jet.spriteRate = entry->spriteRate;
    jet.soundUpdateFn = entry->soundUpdateFn;
    jet.soundStopFn = entry->soundStopFn;
    jet.unk68 = entry->unk42;
    jet.childPreset = entry->childPreset;
    jet.headFadeCount = entry->headFadeCount;
    jet.headFadeStep = entry->headFadeStep;
    jet.tailFadeCount = entry->tailFadeCount;
    jet.tailFadeStep = entry->tailFadeStep;
    if (jet.soundFn != -1) {
        jet.sound = gJetSoundStartProcs[jet.soundFn]();
    } else {
        jet.sound = 0;
    }
    matrix = &jet.owner->ob_matrix_ptr[jet.bone];
    math_transformOffset(&jet.startOffset, &jet.start, matrix);
    math_transformOffset(&jet.endOffset, &jet.end, matrix);
    spawn.kind = 3;
    spawn.pos.x = jet.start.x;
    spawn.pos.y = jet.start.y;
    spawn.pos.z = jet.start.z;
    jet.timeCarry = 0.0f;
    actor = effect_newEffectObj(&spawn, sizeof(CharacterFlamethrowerJet), sizeof(JetPoint), 2, 2, 2, 0, 0, 0, arg2, arg3);
    if (actor != NULL) {
        state = actor->state;
        memcpy(actor->state, &jet, sizeof(CharacterFlamethrowerJet));
        if (state->speedFn != -1) {
            state->speed = gJetSpeedProcs[state->speedFn](actor, state->speedFn);
        } else {
            state->speed = 0.0f;
        }
        if (jet.initFn != -1) {
            gJetInitProcs[jet.initFn](actor);
        }
    }
    return actor;
}


/* Per-frame jet update: moves, fades and retires its particles, then runs its update callbacks. */
s32 flamethrower_updateJet(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    JetPoint *points;
    s32 i;
    s16 n;
    u8 scale;
    s8 hold;
    u8 jetFlags;
    u8 flags; /* doubles as the fade-in alpha */

    jet = actor->state;
    points = actor->points;
    if (actor->head.count < 2 && (jet->flags & 1)) {
        return 0;
    }
    i = actor->head.end;
    while (i != actor->head.start) {
        i--;
        if (i < 0) {
            i = actor->head.tmpl.itemCount - 1;
        }
        points[i].vel.y += jet->gravity * gFrameDelta;
        points[i].pos.x += points[i].vel.x * gFrameDelta;
        points[i].pos.y += points[i].vel.y * gFrameDelta;
        points[i].pos.z += points[i].vel.z * gFrameDelta;
        points[i].alpha = 255;
        hold = points[i].holdTimer;
        if (hold > 0) {
            points[i].holdTimer = hold - gFrameTicks;
        } else {
            points[i].life -= jet->fadeRate * gFrameTicks;
        }
        if (points[i].life < jet->childLife) {
            if (points[i].spawnedChild == 0) {
                flamethrower_shedJetLines(actor, i);
            }
        }
        points[i].size += jet->sizeRate * gFrameDelta;
        if (points[i].life < 0) {
            /* retire this point and every older one */
            jet->flags &= ~2;
            while (i != actor->head.start) {
                actor->head.start = actor->head.start + 1;
                if (actor->head.start == actor->head.tmpl.itemCount) {
                    actor->head.start = 0;
                }
                actor->head.count = actor->head.count - 1;
            }
            points[actor->head.start].life = 0;
        }
    }
    i = actor->head.start;
    // matching: the extra copy through jetFlags is needed
    jetFlags = jet->flags;
    flags = jetFlags;
    if (flags & 2) {
        /* fade in the oldest points */
        flags = 0;
        n = jet->headFadeCount;
        do { // matching: one-line body (-g3 line numbers steer as1's scheduling)
            points[i].alpha = flags; i++; flags += jet->headFadeStep; if (i == actor->head.tmpl.itemCount) { i = 0; } n--;
        } while (n != 0 && i != actor->head.end);
        flags = jet->flags;
    }
    if (flags & 4) {
        /* fade out the newest points */
        scale = 0;
        n = jet->tailFadeCount;
        i = actor->head.end - 1;
        if (i < 0) {
            i = actor->head.tmpl.itemCount - 1;
        }
        do { // matching: one-line body (-g3 line numbers steer as1's scheduling)
            points[i].alpha = (points[i].alpha * scale) >> 8; i--; scale += jet->tailFadeStep; if (i < 0) { i = actor->head.tmpl.itemCount - 1; } n--;
        } while (n != 0 && i != actor->head.end);
    }
    if (jet->soundUpdateFn != -1) {
        gJetSoundUpdateProcs[jet->soundUpdateFn](actor);
    }
    if (jet->unk68 != -1) {
        gFlamethrowerJetProcs[jet->unk68](actor);
    }
    if (jet->updateFn != -1) {
        if (gJetUpdateProcs[jet->updateFn](actor) == 0) {
            return 0;
        }
    }
    if (actor->head.count > 0) {
        actor->head.modelPos = points[actor->head.start].pos;
    } else {
        actor->head.modelPos.x = 0.0f;
        actor->head.modelPos.y = 0.0f;
        actor->head.modelPos.z = 0.0f;
    }
    return 1;
}



/*
 * Emits a jet's particles for this frame: tracks its bone-relative start/end points, then spawns
 * emitAccum particles interpolated between last frame's and this frame's points (and sparks/smoke).
 */
s32 flamethrower_emitJet(CharacterFlamethrowerActor *actor, s32 arg1) {
    CharacterFlamethrowerJet *jet;
    JetPoint *points;
    Vec3f end;
    s32 unused; // matching: unused locals (also pad4-pad7) keep the original frame size
    MtxF *matrix;
    f32 inv;
    f32 tstep;
    f32 dz;
    f32 dy;
    f32 dx;
    f32 dz2;
    f32 dy2;
    f32 dx2;
    f32 dsize;
    f32 dspeed;
    Vec3f cur;
    Vec3f cur2;
    f32 t;
    f32 speed;
    f32 size;
    f32 fq; /* scratch: first the time of the first point, later the size step */
    s32 head;
    s32 tail;
    f32 pad4;
    f32 pad5;
    f32 pad6;
    f32 pad7;

    jet = actor->state;
    points = actor->points;
    if (jet->owner->ob_matrix_ptr == NULL) {
        return 0;
    }
    if (jet->speedFn != -1) {
        jet->targetSpeed = gJetSpeedProcs[jet->speedFn](actor, arg1);
    }
    matrix = &jet->owner->ob_matrix_ptr[jet->bone];
    math_transformOffset(&jet->startOffset, &actor->head.tmpl.pos, matrix);
    math_transformOffset(&jet->endOffset, &end, matrix);
    jet->emitAccum += jet->emitRate * gFrameDelta;
    if (jet->emitAccum > 1.0f) {
        do { // matching: a plain block here reorders the float rodata
            inv = 1.0f / jet->emitAccum;
            t = jet->timeCarry + gFrameDelta;
            fq = t;
            cur = jet->start;
            cur2 = jet->end;
            tstep = t * inv;
            size = jet->size + (jet->sizeRate * t);
            speed = jet->speed;
            dx = actor->head.tmpl.pos.x - jet->start.x;
            dy = actor->head.tmpl.pos.y - jet->start.y;
            dz = actor->head.tmpl.pos.z - jet->start.z;
            dx2 = end.x - jet->end.x;
            dy2 = end.y - jet->end.y;
            dz2 = end.z - jet->end.z;
            dsize = jet->size - size;
            dspeed = jet->targetSpeed - speed;
        } while (0);
        t = fq;
        do {
            jet->scatterAccum += jet->scatterRate;
            if (jet->scatterAccum > 1.0f) {
                flamethrower_scatterParticles(actor, &end);
            }
            jet->spriteAccum += jet->spriteRate;
            if (jet->spriteAccum > 1.0f) {
                flamethrower_emitSprites(actor, &end);
            }
            points[actor->head.end].spawnedChild = 0;
            points[actor->head.end].pos.x = cur.x;
            points[actor->head.end].pos.y = cur.y;
            points[actor->head.end].pos.z = cur.z;
            points[actor->head.end].vel.x = (cur2.x - cur.x) * speed;
            points[actor->head.end].vel.y = (cur2.y - cur.y) * speed;
            points[actor->head.end].vel.z = (cur2.z - cur.z) * speed;
            points[actor->head.end].size = size;
            points[actor->head.end].life = jet->life;
            points[actor->head.end].holdTimer = jet->holdTime;
            points[actor->head.end].alpha = 255;
            points[actor->head.end].texS = jet->texS;
            jet->texS += (math_random() % 6) + 3;
            points[actor->head.end].vel.y += jet->gravity * t;
            points[actor->head.end].pos.x += points[actor->head.end].vel.x * t;
            points[actor->head.end].pos.y += points[actor->head.end].vel.y * t;
            points[actor->head.end].pos.z += points[actor->head.end].vel.z * t;
            t -= tstep;
            head = ++actor->head.end;
            if ((head & 0xFFFFFFFF) == actor->head.tmpl.itemCount) { // matching: no-op mask steers the compare
                head = (actor->head.end = 0);
            }
            actor->head.count++;
            tail = actor->head.start;
            if (tail == head) {
                actor->head.start = tail + 1;
                if (actor->head.start == actor->head.tmpl.itemCount) {
                    actor->head.start = 0;
                }
                actor->head.count--;
            }
            cur.x += dx * inv;
            cur.y += dy * inv;
            cur.z += dz * inv;
            cur2.x += dx2 * inv;
            cur2.y += dy2 * inv;
            cur2.z += dz2 * inv;
            fq = dsize * inv;
            speed += dspeed * inv;
            size += fq;
            jet->emitAccum -= 1.0f;
        } while (jet->emitAccum > 1.0f);
        jet->start = cur;
        jet->end = cur2;
        jet->speed = speed;
        jet->timeCarry = t;
    }
    return 1;
}


/* A TextureDesc whose frame table is the single asset id in front of it. */
typedef struct OneFrameTextureDesc {
    /* 0x0 */ u32 frame; // the texture's asset id
    /* 0x4 */ TextureDesc desc; // desc.addr points at frame
} OneFrameTextureDesc; /* size 0x10 */


/* Draws a jet as a camera-facing ribbon through its points, oldest last. */
Gfx *flamethrower_drawJet(CharacterFlamethrowerActor *actor, Gfx *gfx, s16 view) {
    Vtx_t *vtx;
    Vec3f *eye;
    Vec3f next;
    Vec3f cur;
    JetPoint *points;
    CharacterFlamethrowerJet *jet;
    s32 i;
    s32 j;
    Vec3f toCam;
    Vec3f delta;
    Vec3f n;
    f32 len;
    u8 curTexS;
    u8 nextTexS;
    u8 flag;
    u8 alpha;
    f32 width;
    s32 newStrip;

    if (actor->head.count >= 2) {
        particle_getViewBuffer(actor->buffers, view, (actor->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            eye = &camera_global_Cameras[view].eye;
            jet = actor->state;
            points = actor->points;
            flag = 1;
            {
                OneFrameTextureDesc tex;

                tex.frame = D_800915B0.addr;
                tex.desc.addr = (u32)&tex.frame;
                tex.desc.frameCount = D_800915B0.frames;
                tex.desc.width = D_800915B0.width;
                tex.desc.height = D_800915B0.height;
                tex.desc.fmt = D_800915B0.fmt;
                tex.desc.siz = D_800915B0.siz;
                gfx = math_gfxLoadTextureFrame(gfx, &tex.desc, 0, 0, 0x100, 0x100, 0, 4, 0, &flag, 3);
                gfx = math_gfxSetEnvColor(gfx, jet->shade, jet->shade, jet->shade, 0xFF, &flag);
                gfx = math_gfxUpdateGeometryMode(gfx, 1, 0x160600);
                gfx = particle_setCombineMode(gfx, 0xB, &flag);
                gfx = math_gfxSetOtherMode(gfx, gSpriteTlutMode | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, 0x5049D8, &flag);
                if (actor->head.tmpl.flags & 2) {
                    j = actor->head.end - 1;
                    if (j < 0) {
                        j = actor->head.tmpl.itemCount - 1;
                    }
                    cur = actor->head.tmpl.pos;
                    next = points[j].pos;
                    nextTexS = points[j].texS;
                    curTexS = points[j].texS;
                    width = jet->size;
                } else {
                    i = actor->head.end - 1;
                    if (i < 0) {
                        i = actor->head.tmpl.itemCount - 1;
                    }
                    j = i - 1;
                    if (j < 0) {
                        j = actor->head.tmpl.itemCount - 1;
                    }
                    cur = points[i].pos;
                    next = points[j].pos;
                    curTexS = points[i].texS;
                    nextTexS = points[j].texS;
                    width = points[j].size;
                }
                delta.x = cur.x - next.x;
                delta.y = cur.y - next.y;
                delta.z = cur.z - next.z;
                toCam.x = cur.x - eye->x;
                toCam.y = cur.y - eye->y;
                toCam.z = cur.z - eye->z;
                n.x = delta.y * toCam.z - toCam.y * delta.z;
                n.y = delta.z * toCam.x - toCam.z * delta.x;
                n.z = delta.x * toCam.y - toCam.x * delta.y;
                len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
                if (len == 0.0f) {
                    n.x = 0.0f;
                    n.y = 0.0f;
                    n.z = 0.0f;
                } else {
                    f32 scale;

                    scale = width / len;
                    n.x *= scale;
                    n.y *= scale;
                    n.z *= scale;
                }
                alpha = (points[j].life * points[j].alpha) >> 8;
                vtx->ob[0] = cur.x + n.x;
                vtx->ob[1] = cur.y + n.y;
                vtx->ob[2] = cur.z + n.z;
                vtx->tc[0] = (curTexS + 0x100) << 6;
                vtx->tc[1] = 0x47C0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = alpha;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = cur.x - n.x;
                vtx->ob[1] = cur.y - n.y;
                vtx->ob[2] = cur.z - n.z;
                vtx->tc[0] = (curTexS + 0x100) << 6;
                vtx->tc[1] = 0x4000;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = alpha;
                vtx->flag = 0;
                vtx++;
                do {
                    delta.x = cur.x - next.x;
                    delta.y = cur.y - next.y;
                    delta.z = cur.z - next.z;
                    toCam.x = next.x - eye->x;
                    toCam.y = next.y - eye->y;
                    toCam.z = next.z - eye->z;
                    n.x = delta.y * toCam.z - toCam.y * delta.z;
                    n.y = delta.z * toCam.x - toCam.z * delta.x;
                    n.z = delta.x * toCam.y - toCam.x * delta.y;
                    len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
                    if (len == 0.0f) {
                        n.x = 0.0f;
                        n.y = 0.0f;
                        n.z = 0.0f;
                    } else {
                        f32 scale;

                        scale = points[j].size / len;
                        n.x *= scale;
                        n.y *= scale;
                        n.z *= scale;
                    }
                    alpha = (points[j].life * points[j].alpha) >> 8;
                    vtx->ob[0] = next.x + n.x;
                    vtx->ob[1] = next.y + n.y;
                    vtx->ob[2] = next.z + n.z;
                    vtx->tc[0] = (nextTexS + 0x100) << 6;
                    vtx->tc[1] = 0x47C0;
                    vtx->cn[0] = 0xFF;
                    vtx->cn[1] = 0xFF;
                    vtx->cn[2] = 0xFF;
                    vtx->cn[3] = alpha;
                    vtx->flag = 0;
                    vtx++;
                    vtx->ob[0] = next.x - n.x;
                    vtx->ob[1] = next.y - n.y;
                    vtx->ob[2] = next.z - n.z;
                    vtx->tc[0] = (nextTexS + 0x100) << 6;
                    vtx->tc[1] = 0x4000;
                    vtx->cn[0] = 0xFF;
                    vtx->cn[1] = 0xFF;
                    vtx->cn[2] = 0xFF;
                    vtx->cn[3] = alpha;
                    vtx->flag = 0;
                    vtx++;
                    gSPVertex(gfx++, vtx - 4, 4, 0);
                    gSP1Triangle(gfx++, 0, 1, 2, 0);
                    gSP1Triangle(gfx++, 1, 3, 2, 0);
                    newStrip = curTexS < nextTexS;
                    curTexS = nextTexS;
                    if (newStrip) {
                        memcpy(vtx, vtx - 2, 0x20);
                        vtx[-2].tc[0] -= 0x4000;
                        vtx++;
                        vtx[-2].tc[0] -= 0x4000;
                        vtx++;
                    }
                    i = j;
                    j--;
                    if (j < 0) {
                        j = actor->head.tmpl.itemCount - 1;
                    }
                    cur = points[i].pos;
                    next = points[j].pos;
                    nextTexS = points[j].texS;
                } while (i != actor->head.start);
            }
        }
    }
    return gfx;
}


/* Jet speed callbacks (gJetSpeedProcs). */
f32 flamethrower_getSlowJetSpeed(CharacterFlamethrowerActor *actor) {
    return 1.32f;
}

/* Returns the "power" tunable (math_debugTunableRange) scaled by 0.01. */
f32 flamethrower_getPowerTunable(CharacterFlamethrowerActor *actor) {
    return math_debugTunableRange(10, "power", 1, 500, "../Effects/CharacterFlameThrower/characterflamethrower.c", 2252) * 0.01f;
}

f32 flamethrower_getFastJetSpeed(CharacterFlamethrowerActor *actor) {
    return 10.9f;
}

/* Sends message 8 for `owner`, which stops its jets (see flamethrower_onJetMessage). */
void flamethrower_stopJets(Actor *owner) {
    ActorRef ref;

    if (owner != NULL) {
        ref.actor = owner;
        ref.uniqueId = owner->unique_id;
        effect_sendEventToEffectObjs((s32)&ref, 8);
    }
}

/* User data of the line particles a jet spawns (flamethrower_shedJetLines), at SceneParticle 0x110. */
typedef struct JetChildData {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x30 */ Vec3f velA;
    /* 0x3C */ Vec3f velB;
    /* 0x48 */ f32 gravity;
    /* 0x4C */ f32 dragA;
    /* 0x50 */ f32 dragB;
    /* 0x54 */ f32 shrinkRate;
    /* 0x58 */ s16 fadeStart;
    /* 0x5A */ s16 alphaPerLife;
} JetChildData;

/* User data of the line particles a flame sheds (flamethrower_shapeFlame), at SceneParticle 0x110. */
typedef struct FlameLineData {
    /* 0x00 */ Vec3f velA;
    /* 0x0C */ Vec3f velB;
    /* 0x18 */ f32 gravity;
    /* 0x1C */ f32 dragA;
    /* 0x20 */ f32 dragB;
    /* 0x24 */ f32 shrinkRate;
    /* 0x28 */ s16 fadeStart;
    /* 0x2A */ s16 alphaPerLife;
} FlameLineData;

/*
 * A particle of the shared particle system (particle_newQuadParticle/particle_newQuadParticleWhite); only the fields used here.
 * The ones in this file are beams: a camera-facing quad from `start` towards `end`.
 */
struct SceneParticle {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ union {
        JetChildData jet;
        FlameLineData line;
    } data;
};

/* Particle callback: applies drag and gravity to both end points' velocities, then moves them. */
s32 flamethrower_moveJetLine(SceneParticle *p) {
    p->data.jet.velA.x *= p->data.jet.dragA;
    p->data.jet.velA.y += p->data.jet.gravity * gFrameDelta;
    p->data.jet.velA.z *= p->data.jet.dragA;
    p->data.jet.velB.x *= p->data.jet.dragB;
    p->data.jet.velB.y += p->data.jet.gravity * gFrameDelta;
    p->data.jet.velB.z *= p->data.jet.dragB;
    p->head.def.pos.x += p->data.jet.velA.x * gFrameDelta;
    p->head.def.pos.y += p->data.jet.velA.y * gFrameDelta;
    p->head.def.pos.z += p->data.jet.velA.z * gFrameDelta;
    p->head.def.rot.x += p->data.jet.velB.x * gFrameDelta;
    p->head.def.rot.y += p->data.jet.velB.y * gFrameDelta;
    p->head.def.rot.z += p->data.jet.velB.z * gFrameDelta;
    return 1;
}

/* Beam callback: shrinks width and progress by shrinkRate and fades alpha out over the last fadeStart frames. */
s32 flamethrower_fadeJetLine(SceneParticle *p) {
    s16 life;

    life = p->head.def.lifetime;
    p->head.def.height = p->head.def.height - (p->head.def.height * p->data.jet.shrinkRate);
    p->head.def.width = p->head.def.width - (p->head.def.width * p->data.jet.shrinkRate);
    if (life < p->data.jet.fadeStart) {
        p->head.def.unk44 = life * p->data.jet.alphaPerLife;
    }
    return 1;
}



/* Builds the beam quad (4 vertices) from start towards end, `progress` of the way, facing the camera. */
void *flamethrower_buildJetLineQuad(SceneParticle *beam, s16 view) {
    Vtx_t *vertices;
    void *result;
    f32 progress;
    Vec3f tip;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    u8 fresh;
    Vec3f *eye;
    f32 scale;

    particle_getViewBuffer(beam->head.vtxBuffers, view, 0x40, &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(beam->head.vtxBuffers[view], beam->head.vtxTemplate, 0x40);
            memcpy(beam->head.vtxBuffers[view] + 4, beam->head.vtxTemplate, 0x40);
        }
    } else {
        return NULL;
    }
    progress = beam->head.def.height;
    dx = beam->head.def.rot.x - beam->head.def.pos.x;
    dy = beam->head.def.rot.y - beam->head.def.pos.y;
    dz = beam->head.def.rot.z - beam->head.def.pos.z;
    tip.x = beam->head.def.pos.x + dx * progress;
    tip.y = beam->head.def.pos.y + dy * progress;
    tip.z = beam->head.def.pos.z + dz * progress;
    eye = &camera_global_Cameras[view].eye;
    dx = tip.x - beam->head.def.pos.x;
    dy = tip.y - beam->head.def.pos.y;
    dz = tip.z - beam->head.def.pos.z;
    vx = beam->head.def.pos.x - eye->x;
    vy = beam->head.def.pos.y - eye->y;
    vz = beam->head.def.pos.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = beam->head.def.width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->ob[0] = beam->head.def.pos.x + nx;
    vertices->ob[1] = beam->head.def.pos.y + ny;
    vertices->ob[2] = beam->head.def.pos.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->ob[0] = beam->head.def.pos.x - nx;
    vertices->ob[1] = beam->head.def.pos.y - ny;
    vertices->ob[2] = beam->head.def.pos.z - nz;
    vertices->flag = 0;
    vertices++;
    dx = -dx;
    dy = -dy;
    dz = -dz;
    vx = tip.x - eye->x;
    vy = tip.y - eye->y;
    vz = tip.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = beam->head.def.width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->ob[0] = tip.x + nx;
    vertices->ob[1] = tip.y + ny;
    vertices->ob[2] = tip.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->ob[0] = tip.x - nx;
    vertices->ob[1] = tip.y - ny;
    vertices->ob[2] = tip.z - nz;
    vertices->flag = 0;
    return result;
}


/* Message handler: dies when its owner is killed (0, 2, 8); follows an owner swap (0x2D). */
void flamethrower_onJetMessage(CharacterFlamethrowerActor *actor, ActorMsg *msg, u8 kind) {
    ActorRef *ref = actor->state;

    if (kind == 0 || kind == 2 || kind == 8) {
        if (msg->ref.actor == ref->actor || msg->ref.uniqueId == ref->uniqueId) {
            flamethrower_stopJet(actor);
        }
    } else if (kind == 0x2D) {
        if (ref->actor == msg->swap.actorA) {
            ref->actor = msg->swap.actorB;
            ref->uniqueId = msg->swap.idB;
        } else if (ref->actor == msg->swap.actorB) {
            ref->actor = msg->swap.actorA;
            ref->uniqueId = msg->swap.idA;
        }
    }
}


/*
 * Called when jet point `index` fades below childLife: spawns line particles (moved and faded by
 * flamethrower_moveJetLine/flamethrower_fadeJetLine) around the segment between it and the next newer point.
 */
void flamethrower_shedJetLines(CharacterFlamethrowerActor *actor, s32 index) {
    CharacterFlamethrowerJet *jet;
    JetPoint *points;
    JetPoint *cur;
    JetPoint *next;
    const JetChildPreset *preset;
    s32 i;
    SceneParticle *ret;
    u8 a;
    u8 b;
    ParticleDef48 spawn;
    JetChildData data;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;
    f32 rad;
    f32 rc;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 flag;

    jet = actor->state;
    points = actor->points;
    preset = &gJetChildPresets[jet->childPreset];
    cur = (JetPoint *)((u8 *)points + index * sizeof(JetPoint)); // matching: byte-offset form keeps the addu operand order
    jet->childAccum += jet->childRate;
    cur->spawnedChild = TRUE;
    if ((i = index + 1) == actor->head.tmpl.itemCount) {
        i = 0;
    }
    if (jet->childAccum > 1.0f && i != actor->head.end) {
        s32 pad[2]; // matching: unused stack slots

        spawn.frame = 0;
        data.unk20 = 25137.0f;
        data.unk24 = 6733.0f;
        data.unk2C = 0;
        spawn.frameSpeed = (data.unk20 + data.unk24) * 0.5f;
        data.gravity = jet->gravity;
        data.fadeStart = 8;
        data.alphaPerLife = 32;
        data.dragA = 0.8280000091f;
        data.dragB = 0.753000021f;
        data.shrinkRate = -0.0120000001f;
        data.unk0 = preset->sizeScale * 30.09054375f;
        data.unk4 = preset->sizeScale * 18.35523224f;
        data.unk1C = 0;
        spawn.width = (data.unk0 + data.unk4) * 0.5f;
        data.unk8 = spawn.width;
        data.unkC = preset->unk4 * 3.961740017f;
        data.unk10 = preset->unk4 * 2.919177055f;
        data.unk14 = preset->unk4 * 8.116548538f;
        data.unk1E = 0;
        spawn.scale.x = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.flags = 0x40000001;
        spawn.unk44 = 255;
        spawn.unk45 = 255;
        spawn.height = (data.unkC + data.unk10) * 0.5f;
        data.unk18 = spawn.height;
        spawn.scale.y = 0.0f;
        if (math_random() & 1) {
            spawn.texture = 0x13;
        } else {
            spawn.texture = 0x14;
        }
        spawn.rgba[0] = 0;
        spawn.rgba[1] = 0;
        spawn.rgba[2] = 0;
        spawn.rgba[3] = 0x68;
        spawn.renderModes = 0x301;
        next = (JetPoint *)((u8 *)points + i * sizeof(JetPoint));
        data.velA = next->vel;
        data.velB = cur->vel;
        do {
            a = math_random() & 0xFF;
            b = math_random() & 0xFF;
            sinA = math_cosBinAngleMasked(a - 0x40);
            cosA = math_cosBinAngleMasked(a);
            sinB = math_cosBinAngleMasked(b - 0x40);
            cosB = math_cosBinAngleMasked(b);
            rad = next->size * 0.8000000119f;
            spawn.lifetime = (math_random() % (preset->lifeRange + 1)) + preset->lifeMin;
            rc = rad * cosB;
            dx = rc * sinA;
            dy = -rad * sinB;
            dz = rc * cosA;
            spawn.pos.x = next->pos.x + dx;
            spawn.pos.y = next->pos.y + dy;
            spawn.pos.z = next->pos.z + dz;
            spawn.rot.x = cur->pos.x + dx;
            spawn.rot.y = cur->pos.y + dy;
            spawn.rot.z = cur->pos.z + dz;
            if (math_random() & 1) {
                flag = 1;
            } else {
                flag = 0;
            }
            ret = particle_newQuadParticleWhite(&spawn, 0xA, 0xB, 0, 9, flag | 2, 0x5C, 0xFF, 0);
            if (ret != NULL) {
                memcpy(&ret->data, &data, sizeof(data));
            }
            jet->childAccum -= 1.0f;
        } while (jet->childAccum > 1.0f);
    }
}


/* Jet update callback: a point inside object 0's (the player's) cylinder hits it and retires the older points. */
s32 flamethrower_hitPlayer(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    JetPoint *points;
    Actor *obj;
    s32 i;
    Vec3f pos;
    f32 r;
    f32 h;
    f32 d;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 p1; // matching: unused, keeps the original frame size

    jet = actor->state;
    points = actor->points;
    obj = &movement_obj[0];
    i = actor->head.end;
    while (i != actor->head.start) {
        i--;
        if (i < 0) {
            i = actor->head.tmpl.itemCount - 1;
        }
        pos = points[i].pos;
        r = obj->bodyRadius;
        h = obj->bodyHeight;
        dx = pos.x - obj->xpos;
        dz = pos.z - obj->zpos;
        dy = pos.y - (obj->ypos + obj->bodyOffsetY);
        dy *= (1.0f + r / h) * 0.5f;
        d = dx * dx + dy * dy + dz * dz;
        if (d < r * r) {
            actor_hitScripted(obj, 0x60006, obj->langle, jet->owner - movement_obj);
            particle_startActorEmitter(obj, 0xB, 0, 1, actor->head.header.owner, actor->head.header.kind, "Characterflamethrower");
            while (i != actor->head.start) {
                actor->head.start = actor->head.start + 1;
                if (actor->head.start == actor->head.tmpl.itemCount) {
                    actor->head.start = 0;
                }
                actor->head.count = actor->head.count - 1;
            }
        }
    }
    return 1;
}

/* Jet update callback: a point near any other active object (of the first 25) retires the older points. */
s32 flamethrower_hitObjects(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    JetPoint *points;
    Actor *obj;
    s32 i;
    s32 j;
    Vec3f pos;
    f32 r;
    f32 h;
    f32 d;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 p1; // matching: unused (with p3), keeps the original frame size
    f32 p3;

    jet = actor->state;
    points = actor->points;
    for (j = 0; j < 25; j++) {
        obj = &movement_obj[j];
        if (obj->type == 0 || obj->attach_char != 0 || jet->owner->unique_id == obj->unique_id) {
            continue;
        }
        {
            i = actor->head.end;
            while (i != actor->head.start) {
                i--;
                if (i < 0) {
                    i = actor->head.tmpl.itemCount - 1;
                }
                pos = points[i].pos;
                r = obj->bodyRadius;
                h = obj->bodyHeight;
                dx = pos.x - obj->xpos;
                dz = pos.z - obj->zpos;
                dy = pos.y - (obj->ypos + obj->bodyOffsetY);
                dy *= (1.0f + r / h) * 0.5f;
                if (dx * dx + dy * dy + dz * dz < r * r + 2500.0f) {
                    while (i != actor->head.start) {
                        actor->head.start = actor->head.start + 1;
                        if (actor->head.start == actor->head.tmpl.itemCount) {
                            actor->head.start = 0;
                        }
                        actor->head.count = actor->head.count - 1;
                    }
                }
            }
        }
    }
    return 1;
}

/*
 * Jet update callback, active while the owner's mv->unk58 is 1: the swarm actors of types 0x15-0x17 that
 * a point touches are hit (effect_setObjectType message 0x18), get model 5 and go into state 4.
 */
s32 flamethrower_burnSwarm(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet = actor->state;
    JetPoint *points = actor->points;
    Actor *owner;
    ActorCtl *sub;
    SwarmActor *head;
    SwarmActor *node;
    s32 i;
    s32 j;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 r;
    u8 lists[3] = { 0x15, 0x16, 0x17 };
    Vec3f pos;

    owner = jet->owner;
    sub = owner->mv;
    if (sub == NULL) {
        return 0;
    }
    if (sub->idle.active != 1) {
        owner->unk2F8 &= ~0x100;
        return 0;
    }
    if (&points) { // matching: address-taken points stays in its stack slot
    }
    for (i = 0; i < 3; i++) {
        head = (SwarmActor *)gObjectLists[0][lists[i]]; // flat index over both rows
        if (head != NULL) {
            node = head;
            j = actor->head.end;
            gObjectWalkDepth++;
            while (node != NULL) {
                gObjectWalkNext[gObjectWalkDepth] = node->header.next;
                nx = node->x;
                ny = node->y;
                nz = node->z;
                while (j != actor->head.start) {
                    j--;
                    if (j < 0) {
                        j = actor->head.tmpl.itemCount - 1;
                    }
                    pos = points[j].pos;
                    r = points[j].size;
                    dx = pos.x - nx;
                    dy = pos.y - ny;
                    dz = pos.z - nz;
                    if (dx * dx + dy * dy + dz * dz < r * r + 900.0f) {
                        swarm_loadModel(5);
                        effect_setObjectType(&node->header, 0x18);
                        node->modelIndex = 5;
                        node->vertices = gEffectModelSlots[5].unk8;
                        node->flags = 9;
                        node->yawStep = 40.0f;
                        node->pitch = 40.0f;
                        node->pitchStep = 0.0f;
                        node->groundHeight = collision_findMeshFloorNear(node->x, node->y, node->z, 0);
                        node->state = 4;
                    }
                }
                node = gObjectWalkNext[gObjectWalkDepth];
            }
            gObjectWalkDepth--;
        }
    }
    return 1;
}

/* Jet sound-start callback (gJetSoundStartProcs): starts the looping flame sound; its handle is left in v0. */
void flamethrower_startJetSound(void) {
    sound_play(0x1AA, 0x7FFF, 0x40, 0, 0);
}

/* Jet callback (gJetSoundUpdateProcs): moves the jet's sound to its oldest point. */
void flamethrower_moveJetSound(CharacterFlamethrowerActor *actor) {
    JetPoint *points;
    CharacterFlamethrowerJet *jet;
    u16 sound;

    jet = actor->state;
    points = actor->points;
    if (actor->head.count != 0) {
        sound = jet->sound;
        if (sound != 0) {
            sound_setVoiceSpatial(sound, 0x7FFF, 0, 0, 0, points[actor->head.start].pos.x, points[actor->head.start].pos.y,
                          points[actor->head.start].pos.z, 500, 5000);
        }
    }
}

/* Jet destroy callback (gJetSoundStopProcs): stops the jet's sound. */
void flamethrower_stopJetSound(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    u16 sound;

    jet = actor->state;
    if (actor->head.count != 0) {
        sound = jet->sound;
        if (sound != 0) {
            sound_stop(sound);
        }
    }
}

/* Jet callback (gFlamethrowerJetProcs): does nothing. */
void flamethrower_emptyJetProc(CharacterFlamethrowerActor *actor) {
}

/* Spawns scatterAccum particles (trail_createMotionTrail) flying from the actor towards random points around `end`. */
void flamethrower_scatterParticles(CharacterFlamethrowerActor *actor, Vec3f *end) {
    TrailRender style;
    CharacterFlamethrowerJet *jet;
    const ScatterPreset *preset;
    EffectTemplate18 spawn;
    TrailMotion motion;
    f32 px;
    f32 py;
    f32 pz;
    f32 speed;
    u8 a;
    u8 b;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;
    f32 rad;

    jet = actor->state;
    preset = &gJetScatterPresets[jet->scatterPreset];
    spawn.flags = 1;
    motion.texture = 0xA;
    motion.shade = 0xFF;
    motion.flags = 0x28;
    motion.alpha = 0x82;
    spawn.pos = actor->head.tmpl.pos;
    style.unk0 = 0;
    style.setGeometryMode = 1;
    style.clearGeometryMode = 0x160600;
    style.renderMode = 3;
    style.unk10 = 0x10;
    style.unk14 = 0x80;
    style.unk18 = 0x20;
    style.primMode = 0;
    style.envMode = 9;
    do {
        speed = (math_randomFloat() * preset->speedRange) + preset->speed;
        a = math_random() & 0xFF;
        b = math_random() & 0xFF;
        sinA = math_cosBinAngleMasked(a - 0x40);
        cosA = math_cosBinAngleMasked(a);
        sinB = math_cosBinAngleMasked(b - 0x40);
        cosB = math_cosBinAngleMasked(b);
        rad = preset->radius;
        px = end->x + (rad * cosB * sinA);
        py = end->y - (rad * sinB);
        pz = end->z + (rad * cosB * cosA);
        spawn.itemCount = (math_random() % (preset->itemCountRange + 1)) + preset->itemCount;
        spawn.life = (math_random() % (preset->lifeRange + 1)) + preset->life;
        motion.halfWidth = (math_randomFloat() * preset->halfWidthRange) + preset->halfWidth;
        motion.gravity = (math_randomFloat() * preset->gravityRange) + preset->gravity;
        motion.velocity.x = (px - actor->head.tmpl.pos.x) * speed;
        motion.velocity.y = (py - actor->head.tmpl.pos.y) * speed;
        motion.velocity.z = (pz - actor->head.tmpl.pos.z) * speed;
        trail_createMotionTrail(&spawn, &motion, 0, 1, 0, 0, 0, 0, 0, 0, 0, &style, 0, actor->head.header.owner, actor->head.header.kind);
        jet->scatterAccum -= 1.0f;
    } while (jet->scatterAccum > 1.0f);
}


/* Emits spriteAccum sprites (effect_spawnBillboard) flying from the actor towards random points around `end`. */
void flamethrower_emitSprites(CharacterFlamethrowerActor *actor, Vec3f *end) {
    f32 speed;
    BillboardTemplate packet;
    f32 cosine1;
    f32 sine1;
    f32 cosine2;
    f32 sine2;
    f32 x;
    f32 y;
    f32 z;
    u8 angle1;
    u8 angle2;
    u32 rnd;
    const JetSpritePreset *preset;
    CharacterFlamethrowerJet *jet;

    jet = actor->state;
    preset = &gJetSpritePresets[jet->spritePreset];
    packet.x = actor->head.tmpl.pos.x;
    packet.y = actor->head.tmpl.pos.y;
    packet.z = actor->head.tmpl.pos.z;
    packet.subX = 0;
    packet.subY = 0;
    packet.subZ = 0;
    packet.primR = 0xFF;
    packet.primG = 0xFF;
    packet.primB = 0xFF;
    packet.envR = 0;
    packet.envG = 0;
    packet.envB = 0;
    packet.primA = 0xFF;
    packet.callback = -1;
    packet.flags = 0x12;
    packet.frame = 0;
    packet.frameStep = 0;
    packet.anim = &D_80090514;
    packet.life = 300;
    do {
        speed = math_randomFloat() * preset->speed_range + preset->speed;
        rnd = math_random();
        angle1 = rnd & 255;
        rnd = math_random();
        angle2 = rnd & 255;
        cosine1 = math_cosBinAngleMasked(angle1 - 64);
        sine1 = math_cosBinAngleMasked(angle1);
        cosine2 = math_cosBinAngleMasked(angle2 - 64);
        sine2 = math_cosBinAngleMasked(angle2);
        x = end->x + preset->radius * sine2 * cosine1;
        y = end->y - preset->radius * cosine2;
        z = end->z + preset->radius * sine2 * sine1;
        packet.velX = (x - actor->head.tmpl.pos.x) * speed;
        packet.velY = (y - actor->head.tmpl.pos.y) * speed;
        packet.velZ = (z - actor->head.tmpl.pos.z) * speed;
        packet.accelY = math_random() % (preset->acceleration_range + 1) + preset->acceleration;
        packet.fadeStep = math_random() % (preset->fade_range + 1) + preset->fade;
        packet.width = packet.height = math_random() % (preset->size_range + 1) + preset->size;
        effect_spawnBillboard(&packet, 0, 0, -1, actor->head.header.owner, actor->head.header.kind);
        jet->spriteAccum -= 1.0f;
    } while (jet->spriteAccum > 1.0f);
}

/* Stops a jet: no new points, and it dies once fewer than 2 remain while the newest fade out. */
void flamethrower_stopJet(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;

    jet = actor->state;
    actor->head.updateKind2 = 0;
    actor->head.tmpl.flags &= ~2;
    jet->flags |= 1;
    jet->flags |= 4;
}

/* unique_ids of the 6 characters that have a flamethrower zone (gFlamethrowerZones). */
const u8 gFlamethrowerCharacterIds[] = { 6, 7, 8, 9, 10, 11 };

/* Jet init callback (gJetInitProcs): clears the owner's flamethrower zone's `inactive` flag. */
void flamethrower_activateJetZone(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    Actor *owner;
    s32 i;
    s32 found;

    jet = actor->state;
    owner = jet->owner;
    i = 0;
    found = FALSE;
    do {
        if (owner->unique_id == gFlamethrowerCharacterIds[i]) {
            found = TRUE;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        gFlamethrowerZones[i]->inactive = FALSE;
    }
}

/* Jet destroy callback (gJetDestroyProcs): sets the owner's flamethrower zone's `inactive` flag again. */
void flamethrower_deactivateJetZone(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    Actor *owner;
    s32 i;
    s32 found;

    jet = actor->state;
    owner = jet->owner;
    i = 0;
    found = FALSE;
    do {
        if (owner->unique_id == gFlamethrowerCharacterIds[i]) {
            found = TRUE;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        gFlamethrowerZones[i]->inactive = TRUE;
    }
}

/* One point of a flame's ring buffer (0x28 bytes). */
typedef struct FlamePoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 size;
    /* 0x1C */ f32 length; /* distance to the next newer point */
    /* 0x20 */ u8 alpha;
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ f32 texT;
} FlamePoint;

/* State block of a flame effect (flamethrower_spawnFlame): a ribbon of FlamePoints between two bone-relative points. */
typedef struct CharacterFlamethrowerFlame {
    /* 0x000 */ FlameSeed seed;
    /* 0x04C */ FlameLineParams line;
    /* 0x090 */ FlameScatterParams scatter;
    /* 0x0D4 */ FlameEmitParams emit;
    /* 0x110 */ Vec3f start;
    /* 0x11C */ Vec3f end;
    /* 0x128 */ f32 timeCarry;
    /* 0x12C */ f32 emitAccum;
    /* 0x130 */ u16 sound;
    /* 0x132 */ u8 pad132[2];
    /* 0x134 */ f32 lineAccum;
    /* 0x138 */ f32 scatterAccum;
    /* 0x13C */ f32 particleAccum;
    /* 0x140 */ f32 alpha;
    /* 0x144 */ f32 texOffset;
    /* 0x148 */ LightEffect *light; /* from flamethrower_createFlameLight */
    /* 0x14C */ f32 length;
    /* 0x150 */ u8 flags;
    /* 0x151 */ s8 timer151;
    /* 0x152 */ s16 sparkTimer;
} CharacterFlamethrowerFlame;

/* Flame callback tables, indexed by the FlameSeed *Fn fields (-1 = none). */
void (*gFlameInitProcs[])(void *) = { (void (*)(void *))flamethrower_activateFlameZone };
void (*gFlameAttackProcs[])(void *) = {
    (void (*)(void *))flamethrower_attackProc0,
    (void (*)(void *))flamethrower_attackProc1,
    (void (*)(void *))flamethrower_attackProc2,
};
void (*gFlameDestroyProcs[])(void *) = { (void (*)(void *))flamethrower_deactivateFlameZone };
s16 (*gFlameSoundStartProcs[])(void) = { (s16 (*)(void))flamethrower_startFlameSound };
void (*gFlameSoundUpdateProcs[])(void *) = { (void (*)(void *))flamethrower_moveFlameSound };
void (*gFlameSoundStopProcs[])(void *) = { (void (*)(void *))flamethrower_stopFlameSound };
s32 (*gFlameUpdateProcs[])(void *) = { (s32 (*)(void *))flamethrower_checkOwnerAlive };

/* Spawns a flame effect with `extraSize` extra state bytes; setup->flags & 1 also attaches a light. */
CharacterFlamethrowerActor *flamethrower_spawnFlame(s32 extraSize, CharacterFlamethrowerSetup *setup, u8 arg2, s32 arg3) {
    CharacterFlamethrowerActor *result;
    EffectTemplate spawn;
    CharacterFlamethrowerFlame initial;
    Actor *owner;
    const Vec3f *inputs[2];
    Vec3f *outputs[2];
    CharacterFlamethrowerFlame *state;

    owner = setup->seed.owner;
    if (owner->ob_matrix_ptr == NULL) {
        return NULL;
    }
    spawn.itemCount = setup->kind;
    spawn.life = 300;
    spawn.flags = 0x12;
    spawn.kind = 0xE;
    spawn.unk14 = 0;
    initial.seed = setup->seed;
    inputs[0] = &initial.seed.startOffset;
    inputs[1] = &initial.seed.endOffset;
    outputs[0] = &initial.start;
    outputs[1] = &initial.end;
    math_transformOffsets(inputs, outputs, &owner->ob_matrix_ptr[initial.seed.bone], 2);
    {
        s32 flag8;
        s32 flag4;
        s32 flag2;
        s32 flag32;
        s32 flags;

        spawn.pos = initial.start;
        initial.timeCarry = 0.0f;
        initial.emitAccum = 0.0f;
        initial.alpha = initial.seed.alpha;
        initial.texOffset = -16384.0f;
        initial.line = setup->line;
        initial.lineAccum = 0.0f;
        initial.scatter = setup->scatter;
        initial.scatterAccum = 0.0f;
        initial.emit = setup->emit;
        initial.particleAccum = 0.0f;
        initial.length = 0.0f;
        flags = setup->flags;
        flag8 = (flags & 8) ? 8 : 0;
        flag4 = (flags & 4) ? 4 : 0;
        flag2 = (flags & 2) ? 2 : 0;
        flag32 = (flags & 0x10) ? 0x20 : 0;
        initial.flags = flag32 | flag2 | flag4 | flag8;
        initial.sparkTimer = 0;
        initial.timer151 = 0;
        result = effect_newEffectObj(&spawn, extraSize + 0x158, sizeof(FlamePoint), 0xC,
                               0xC, 0xC, 0, 0, 0, arg2, arg3);
        if (result != NULL) {
            state = result->state;
            memcpy(state, &initial, sizeof(CharacterFlamethrowerFlame));
            if (initial.seed.soundFn != -1) {
                state->sound = gFlameSoundStartProcs[initial.seed.soundFn]();
            } else {
                state->sound = 0;
            }
            if (initial.seed.initFn != -1) {
                gFlameInitProcs[initial.seed.initFn](result);
            }
            if (setup->flags & 1) {
                state->light = flamethrower_createFlameLight(result, &spawn.pos, setup->red, setup->green, setup->blue,
                                         setup->alpha, arg2, arg3);
            } else {
                state->light = NULL;
            }
        }
    }
    return result;
}

/* Sends message 0x26 for `owner`, which stops its flames (see flamethrower_onFlameMessage). */
void flamethrower_stopFlames(Actor *owner) {
    ActorRef ref;

    if (owner != NULL) {
        ref.actor = owner;
        ref.uniqueId = owner->unique_id;
        effect_sendEventToEffectObjs((s32)&ref, 0x26);
    }
}


/* Message handler: dies when its owner is killed (0, 2, 0x26); follows an owner swap (0x2D). */
void flamethrower_onFlameMessage(CharacterFlamethrowerActor *actor, ActorMsg *msg, u8 kind) {
    ActorRef *ref = actor->state;

    if (kind == 0 || kind == 2 || kind == 0x26) {
        if (msg->ref.actor == ref->actor || msg->ref.uniqueId == ref->uniqueId) {
            effect_destroyObject(&actor->head.header);
        }
    } else if (kind == 0x2D) {
        if (ref->actor == msg->swap.actorA) {
            ref->actor = msg->swap.actorB;
            ref->uniqueId = msg->swap.idB;
        } else if (ref->actor == msg->swap.actorB) {
            ref->actor = msg->swap.actorA;
            ref->uniqueId = msg->swap.idA;
        }
    }
}


/* Flame destroy: callbacks, then effect_defaultDestroyEffectObj. */
void flamethrower_destroyFlame(CharacterFlamethrowerActor *actor) {
    flamethrower_releaseFlame(actor);
    effect_defaultDestroyEffectObj(actor);
}

/* Flame destroy: callbacks, then effect_defaultFreeEffectObj. */
void flamethrower_freeFlame(CharacterFlamethrowerActor *actor) {
    flamethrower_releaseFlame(actor);
    effect_defaultFreeEffectObj(actor);
}


/* Runs a flame's destroy callbacks and removes its light. */
void flamethrower_releaseFlame(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    LightEffect *light;

    flame = actor->state;
    if (flame->seed.soundStopFn != -1) {
        gFlameSoundStopProcs[flame->seed.soundStopFn](actor);
    }
    if (flame->seed.destroyFn != -1) {
        gFlameDestroyProcs[flame->seed.destroyFn](actor);
    }
    light = flame->light;
    if (light != NULL) {
        effect_destroyObject(light);
    }
}



/* Per-frame flame update: moves its points under gravity and keeps the light at the oldest point. */
s32 flamethrower_updateFlame(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    FlamePoint *points;
    s32 i;

    flame = actor->state;
    points = actor->points;
    flame->flags &= ~0x10;
    if (actor->head.count < 2) {
        if (actor->head.tmpl.flags & 8) {
            return 0;
        }
    }
    if (flame->seed.updateFn != -1) {
        if (gFlameUpdateProcs[flame->seed.updateFn](actor) == 0) {
            return 0;
        }
    }
    i = actor->head.end;
    while (i != actor->head.start) {
        FlamePoint *pt;

        i--;
        if (i < 0) {
            i = actor->head.tmpl.itemCount - 1;
        }
        pt = &points[i];
        flamethrower_integratePoint(&pt->pos, &pt->vel, flame->seed.gravity, gFrameDelta);
        pt->size += flame->seed.sizeRate * gFrameDelta;
    }
    {
        FlamePoint *oldest;

        if (flame->light != NULL && actor->head.count >= 2) {
            LightEffect *light = flame->light;
            PointLight *out = light->light;

            oldest = &points[actor->head.start];
            out->x = oldest->pos.x;
            out->y = oldest->pos.y;
            out->z = oldest->pos.z;
        }
    }
    if (flame->seed.soundUpdateFn != -1) {
        gFlameSoundUpdateProcs[flame->seed.soundUpdateFn](actor);
    }
    flame->flags &= ~1;
    if (actor->head.count > 0) {
        actor->head.modelPos = points[actor->head.start].pos;
    } else {
        actor->head.modelPos.x = 0.0f;
        actor->head.modelPos.y = 0.0f;
        actor->head.modelPos.z = 0.0f;
    }
    return 1;
}

/*
 * Flame emitter: tracks the owner's bone (or its position and facing when it has no bones), emits
 * emitAccum points between last and this frame's start/end, then the scatter and emit particles and sparks.
 */
s32 flamethrower_emitFlame(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    FlamePoint *points;
    Actor *obj;
    Vec3f pos0;
    Vec3f pos1;
    Vec3f pos2;
    Vec3f pos3;

    flame = actor->state;
    points = actor->points;
    obj = flame->seed.owner;
    if (obj->type == 0 || obj->unique_id != flame->seed.ownerId) {
        return 0;
    }
    if ((obj->cullMask & 0xF) == 0xF) {
        flame->flags |= 0x10;
    } else {
        flame->flags &= ~0x10;
    }
    if (obj->ob_matrix_ptr != NULL) {
        const Vec3f *inputs[4];
        Vec3f *outputs[4];

        inputs[0] = &flame->seed.startOffset;
        inputs[1] = &flame->seed.endOffset;
        inputs[2] = &flame->scatter.target;
        inputs[3] = &flame->emit.target;
        outputs[0] = &pos0;
        outputs[1] = &pos1;
        outputs[2] = &pos2;
        outputs[3] = &pos3;
        math_transformOffsets(inputs, outputs, &obj->ob_matrix_ptr[flame->seed.bone], 4);
        flame->flags &= ~1;
    } else {
        Vec3f dir;
        Vec3f up;

        math_getActorAim(obj, &dir, &up, 0, 0.0f);
        pos0.x = obj->xpos;
        pos0.y = obj->ypos + 33.0f;
        pos0.z = obj->zpos;
        pos1.x = dir.x * 0.03200000152f + pos0.x;
        pos1.y = dir.y * 0.03200000152f + pos0.y;
        pos1.z = dir.z * 0.03200000152f + pos0.z;
        flame->flags |= 1;
    }
    actor->head.tmpl.pos = pos0;
    flame->emitAccum += flame->seed.emitRate * gFrameDelta;
    if (flame->emitAccum > 1.0f) {
        f32 inv;
        f32 t;
        f32 step;
        Vec3f d0;
        Vec3f d1;
        Vec3f cur0;
        Vec3f cur1;
        f32 ix0;
        f32 iy0;
        f32 iz0;
        f32 ix1;
        f32 iy1;
        f32 iz1;

        inv = 1.0f / flame->emitAccum;
        t = flame->timeCarry + gFrameDelta;
        step = t * inv;
        cur0 = flame->start;
        cur1 = flame->end;
        d0.x = pos0.x - flame->start.x;
        d0.y = pos0.y - flame->start.y;
        d0.z = pos0.z - flame->start.z;
        d1.x = pos1.x - flame->end.x;
        d1.y = pos1.y - flame->end.y;
        d1.z = pos1.z - flame->end.z;
        ix0 = d0.x * inv;
        iy0 = d0.y * inv;
        iz0 = d0.z * inv;
        ix1 = d1.x * inv;
        iy1 = d1.y * inv;
        iz1 = d1.z * inv;
        do {
            FlamePoint *p;

            p = &points[actor->head.end];
            p->pos = cur0;
            p->vel.x = cur1.x - cur0.x;
            p->vel.y = cur1.y - cur0.y;
            p->vel.z = cur1.z - cur0.z;
            p->size = flame->seed.size;
            p->length = 0.0f;
            p->alpha = flame->seed.alpha;
            p->texT = flame->texOffset;
            flamethrower_integratePoint(&p->pos, &p->vel, flame->seed.gravity, t);
            p->size += flame->seed.sizeRate * t;
            actor->head.end++;
            if (actor->head.end == actor->head.tmpl.itemCount) {
                actor->head.end = 0;
            }
            actor->head.count++;
            if (actor->head.end == actor->head.start) {
                actor->head.start++;
                if (actor->head.start == actor->head.tmpl.itemCount) {
                    actor->head.start = 0;
                }
                actor->head.count--;
            }
            cur0.x += ix0;
            cur0.y += iy0;
            cur0.z += iz0;
            cur1.x += ix1;
            cur1.y += iy1;
            cur1.z += iz1;
            t -= step;
            flame->emitAccum -= 1.0f;
        } while (flame->emitAccum > 1.0f);
        flame->start = cur0;
        flame->end = cur1;
        flame->timeCarry = t;
    }
    if (obj->ob_matrix_ptr != NULL && (obj->cullMask & 0xF) != 0xF) {
        flame->scatterAccum += (flame->scatter.rate + math_randomFloat() * flame->scatter.rateRange) * gFrameDelta;
        if (flame->scatterAccum > 1.0f) {
            EffectTemplate spawn;
            TrailMotion motion;
            TrailRender style;

            style.setGeometryMode = 0x200005;
            style.clearGeometryMode = 0x1F0600;
            style.renderMode = 3;
            style.unk10 = 0x22;
            style.unk0 = 0;
            style.unk14 = 0x80;
            style.unk18 = 0x20;
            style.primMode = 0;
            style.envMode = 7;
            spawn.flags = 0x15;
            spawn.kind = 1;
            spawn.unk14 = -1;
            motion.flags = 0x68;
            motion.texture = 0xA;
            motion.shade = 0xFF;
            motion.unk14 = -10000.0f;
            motion.unk1C = flame->scatter.unk40;
            motion.unk1E = flame->scatter.unk42;
            do {
                spawn.itemCount = (math_random() % (flame->scatter.unk18 + 1)) + flame->scatter.unk14;
                spawn.life = (math_random() % (flame->scatter.unk1E + 1)) + flame->scatter.unk1C;
                motion.halfWidth = math_randomFloat() * flame->scatter.unk24 + flame->scatter.unk20;
                {
                    Vec3f delta;
                    Vec3f axisA;
                    Vec3f axisB;
                    f32 c;
                    f32 s;
                    f32 speed;

                    spawn.pos = pos0;
                    delta = pos2;
                    delta.x -= spawn.pos.x;
                    delta.y -= spawn.pos.y;
                    delta.z -= spawn.pos.z;
                    if (math_buildPerpendicularBasis(&delta, &axisA, &axisB) != 0) {
                        speed = math_randomFloat() * flame->scatter.speedRange + flame->scatter.speedMin;
                        math_dirFromBinAngleXZ(math_random() & 0xFF, math_randomFloat() * flame->scatter.spread, &s, &c);
                        motion.velocity.x = (axisA.x * s + axisB.x * c + delta.x) * speed;
                        motion.velocity.y = (axisA.y * s + axisB.y * c + delta.y) * speed;
                        motion.velocity.z = (axisA.z * s + axisB.z * c + delta.z) * speed;
                        motion.gravity = math_randomFloat() * flame->scatter.unk38 + flame->scatter.unk34;
                        motion.alpha = math_randomFloat() * flame->scatter.unk3E + flame->scatter.unk3C;
                        trail_createMotionTrail(&spawn, &motion, 0, 1, 0, 0, 0, 0, 0, 0, 0, &style, 0, actor->head.header.owner, actor->head.header.kind);
                    }
                }
                flame->scatterAccum -= 1.0f;
            } while (flame->scatterAccum > 1.0f);
        }
    }
    if (obj->ob_matrix_ptr != NULL && (obj->cullMask & 0xF) != 0xF) {
        flame->particleAccum += (flame->emit.rate + math_randomFloat() * flame->emit.rateRange) * gFrameDelta;
        if (flame->particleAccum > 1.0f) {
            ParticleDesc part;

            part.texture = 0x28;
            part.renderMode = 0x2203;
            part.unk0 = 0x200005;
            part.unk4 = 0x1F0600;
            part.frame = 0;
            part.frameRate = 0;
            part.r = 0xFF;
            part.g = 0xFF;
            part.b = 0xFF;
            part.a = 0xFF;
            part.r2 = 0xFF;
            part.g2 = 0xFF;
            part.b2 = 0xFF;
            part.pos = pos0;
            part.offset.x = 0.0f;
            part.offset.y = 0.0f;
            part.offset.z = 0.0f;
            part.fadeStart = flame->emit.unk38;
            part.fadeRate = flame->emit.unk3A;
            part.scaleStart = 1;
            part.scaleRate = 1.0f;
            part.flags = 0xC207;
            part.primColorMode = 6;
            part.envColorMode = 8;
            part.updateFunc = -1;
            part.deathFunc = -1;
            part.drawFunc = -1;
            part.handlerSet = 0;
            do {
                part.timer = (math_random() % (flame->emit.unk16 + 1)) + flame->emit.unk14;
                part.alpha = (math_random() % (flame->emit.unk1A + 1)) + flame->emit.unk18;
                part.width = part.height = math_randomFloat() * flame->emit.unk20 + flame->emit.unk1C;
                {
                    Vec3f delta;
                    Vec3f axisA;
                    Vec3f axisB;
                    f32 c;
                    f32 s;
                    f32 speed;

                    delta = pos3;
                    delta.x -= part.pos.x;
                    delta.y -= part.pos.y;
                    delta.z -= part.pos.z;
                    if (math_buildPerpendicularBasis(&delta, &axisA, &axisB) != 0) {
                        speed = math_randomFloat() * flame->emit.speedRange + flame->emit.speedMin;
                        math_dirFromBinAngleXZ(math_random() & 0xFF, math_randomFloat() * flame->emit.spread, &s, &c);
                        part.vel.x = (axisA.x * s + axisB.x * c + delta.x) * speed;
                        part.vel.y = (axisA.y * s + axisB.y * c + delta.y) * speed;
                        part.vel.z = (axisA.z * s + axisB.z * c + delta.z) * speed;
                        part.gravity = math_randomFloat() * flame->emit.unk34 + flame->emit.unk30;
                        particle_newSpriteParticleWithFloor(&part, 1, 0, 0, actor->head.header.owner, actor->head.header.kind);
                    }
                }
                flame->particleAccum -= 1.0f;
            } while (flame->particleAccum > 1.0f);
        }
    }
    flame->timer151 += gFrameTicks;
    if (flame->timer151 >= 0x3D) {
        if (flame->seed.owner != NULL && flame->seed.owner->mv != NULL) {
            flame->seed.owner->mv->shotsfired++;
        }
        flame->timer151 = 0;
    }
    if ((flame->flags & 8) && obj->CameraPtr != NULL) {
        flame->sparkTimer -= gFrameTicks;
        if (flame->sparkTimer < 0) {
            s32 pad2; // matching: unused stack slot
            RumbleParams spark;

            spark.flags = 1;
            spark.duration = math_random() % 21U + 0x14;
            spark.playerMask = 1 << obj->CameraPtr->CNumber;
            spark.strength = math_random() % 7U + 2;
            spark.updateKind = -1;
            cont_spawnRumble(&spark, 0, actor->head.header.owner, actor->head.header.kind);
            flame->sparkTimer = math_random() % 21U + 0x78;
        }
    }
    return 1;
}

/*
 * Flame shaping: clips the ribbon to maxLength (and to a wall hit when flags & 4), fades the far part,
 * scrolls the texture, sheds line particles near `line.position` of the length, and (flags & 2) calls
 * mp_addZone at the middle.
 */
s32 flamethrower_shapeFlame(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    FlamePoint *points;
    f32 maxLen;

    flame = actor->state;
    points = actor->points;
    maxLen = flame->seed.maxLength;
    if ((flame->flags & 4) && actor->head.count >= 2) {
        Actor *obj;
        Vec3f *end;
        s32 i;
        Vec3f hitPos;
        f32 x;
        f32 y;
        f32 z;
        f32 dx;
        f32 dy;
        f32 hit;
        f32 dz;

        obj = flame->seed.owner;
        if (actor->head.tmpl.flags & 2) {
            end = &actor->head.tmpl.pos;
        } else {
            i = actor->head.end;
            i--;
            if (i < 0) {
                i = actor->head.tmpl.itemCount - 1;
            }
            end = &points[i].pos;
        }
        x = obj->xpos;
        y = obj->ypos + 48.0f;
        z = obj->zpos;
        dx = end->x - x;
        dy = end->y - y;
        dz = end->z - z;
        if (collision_castRayWorld(x, y, z, dx, dy, dz, 0, 0, &hitPos.x, &hitPos.y,
                          &hitPos.z, &hit, 0, 0, 0.0f) != 0 &&
            hit < maxLen) {
            maxLen = hit;
        }
    }
    if (actor->head.count >= 2) {
        Vec3f *prev;
        s32 i;
        Vec3f diff;

        i = actor->head.end;
        flame->length = 0.0f;
        if (actor->head.tmpl.flags & 2) {
            prev = &actor->head.tmpl.pos;
        } else {
            i--;
            if (i < 0) {
                i = actor->head.tmpl.itemCount - 1;
            }
            prev = &points[i].pos;
        }
        do {
            FlamePoint *p;

            i--;
            if (i < 0) {
                i = actor->head.tmpl.itemCount - 1;
            }
            p = &points[i];
            diff.x = p->pos.x - prev->x;
            diff.y = p->pos.y - prev->y;
            diff.z = p->pos.z - prev->z;
            p->length = math_vec3Length(&diff);
            flame->length += p->length;
            if (maxLen < flame->length) {
                if (p->length) {
                    f32 t = (flame->length - maxLen) / p->length;

                    p->pos.x -= diff.x * t;
                    p->pos.y -= diff.y * t;
                    p->pos.z -= diff.z * t;
                    p->length = p->length * (1.0f - t);
                }
                while (i != actor->head.start) {
                    actor->head.start++;
                    if (actor->head.start == actor->head.tmpl.itemCount) {
                        actor->head.start = 0;
                    }
                    actor->head.count--;
                }
                flame->length = maxLen;
            }
            prev = &p->pos;
        } while (i != actor->head.start);
    }
    if (actor->head.count >= 2) {
        f32 sum;
        f32 fadeStart;
        f32 fadeLen;
        f32 inv;
        s32 i;

        sum = 0.0f;
        i = actor->head.end;
        fadeStart = flame->length * flame->seed.fadeStart;
        fadeLen = flame->length - fadeStart;
        inv = 1.0f / fadeLen;
        do {
            FlamePoint *p;

            i--;
            if (i < 0) {
                i = actor->head.tmpl.itemCount - 1;
            }
            p = &points[i];
            sum += p->length;
            if (fadeStart < sum) {
                s32 pad;

                p->alpha = flame->alpha * ((fadeLen - (sum - fadeStart)) * inv);
            } else {
                p->alpha = flame->seed.alpha;
            }
        } while (i != actor->head.start);
    }
    if (actor->head.count >= 2) {
        s32 i;
        f32 sum;

        flame->texOffset += flame->seed.texSpeed * gFrameDelta;
        flame->texOffset = math_wrapFloat(flame->texOffset, 16383.0f, -16384.0f);
        i = actor->head.end;
        sum = 0.0f;
        do {
            FlamePoint *p;

            i--;
            if (i < 0) {
                i = actor->head.tmpl.itemCount - 1;
            }
            p = &points[i];
            sum += p->length;
            p->texT = math_wrapFloat(flame->texOffset + sum * flame->seed.texScale, 16383.0f, -16384.0f);
        } while (i != actor->head.start);
    }
    if (actor->head.count >= 3 && (!(flame->flags & 0x20) || !(flame->flags & 0x10)) && maxLen <= flame->length) {
        flame->lineAccum += (flame->line.rate + math_randomFloat() * flame->line.rateRange) * points[actor->head.start].size * gFrameDelta;
        if (flame->lineAccum > 1.0f) {
            s32 head;
            s32 next;
            s32 i;
            ParticleDef4C spawn;
            FlameLineData lineData;
            s32 found;
            FlamePoint *last;
            FlamePoint *a;
            FlamePoint *b;
            Vec3f pos;
            f32 alpha;
            Vec3f dir;
            f32 sum;
            f32 target;
            SceneParticle *particle;
            s32 pad4; // matching: unused stack slots (also r, pad, pad9) keep the original frame

            head = actor->head.start;
            alpha = 0.0f;
            next = head + 1;
            if (next == actor->head.tmpl.itemCount) {
                next = 0;
            }
            sum = 0.0f;
            found = 0;
            last = NULL;
            target = flame->length * flame->line.position;
            i = actor->head.end;
            do {
                FlamePoint *p;

                i--;
                if (i < 0) {
                    i = actor->head.tmpl.itemCount - 1;
                }
                p = &points[i];
                sum += p->length;
                if (target <= sum) {
                    found = 1;
                    if (last != NULL && p->length) {
                        f32 t = (sum - target) / p->length;

                        pos.x = p->pos.x + (last->pos.x - p->pos.x) * t;
                        pos.y = p->pos.y + (last->pos.y - p->pos.y) * t;
                        pos.z = p->pos.z + (last->pos.z - p->pos.z) * t;
                        alpha = p->size + (last->size - p->size) * t;
                        dir.x = p->pos.x - last->pos.x;
                        dir.y = p->pos.y - last->pos.y;
                        dir.z = p->pos.z - last->pos.z;
                    } else {
                        pos = p->pos;
                        alpha = p->size;
                        dir.x = 0.0f;
                        dir.y = 0.0f;
                        dir.z = 0.0f;
                    }
                }
                last = p;
            } while (i != actor->head.start && found == 0);
            spawn.frame = 0;
            spawn.frameSpeed = 0;
            lineData.dragA = flame->line.dragA;
            lineData.dragB = flame->line.dragB;
            lineData.shrinkRate = flame->line.shrinkRate;
            lineData.fadeStart = flame->line.fadeStart;
            lineData.alphaPerLife = flame->line.alphaPerLife;
            spawn.type = 0;
            spawn.renderModes = 0x2203;
            spawn.rgba[0] = 0xFF;
            spawn.rgba[1] = 0xFF;
            spawn.rgba[2] = 0xFF;
            spawn.rgba[3] = 0xFF;
            spawn.scale.x = 1.0f;
            spawn.scale.y = 0.0f;
            spawn.scale.z = 1.0f;
            spawn.flags = 0x40CC05E1;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 7;
            spawn.unk48 = 0;
            a = &points[head];
            b = &points[next];
            do {
                f32 r1;
                f32 r2;
                f32 r3;
                s32 r;
                Vec3f vel;

                r1 = math_randomFloat() * flame->line.unk14 + flame->line.unkC;
                r2 = math_randomFloat() * flame->line.unk18 + flame->line.unk10;
                r3 = math_randomFloat() * flame->line.speedRange + flame->line.speedMin;
                spawn.width = 24.22288895f * r1;
                spawn.height = 3.440458536f * r2;
                lineData.velA.x = b->vel.x * r3;
                lineData.velA.y = b->vel.y * r3;
                lineData.velA.z = b->vel.z * r3;
                lineData.velB.x = a->vel.x * r3;
                lineData.velB.y = a->vel.y * r3;
                lineData.velB.z = a->vel.z * r3;
                lineData.gravity = math_randomFloat() * flame->line.gravityRange + flame->line.gravityMin;
                if (math_random() & 1) {
                    spawn.texture = 0x13;
                } else {
                    spawn.texture = 0x14;
                }
                spawn.lifetime = math_random() % (flame->line.lifeRange + 1) + flame->line.lifeMin;
                math_dirFromBinAngles(math_random() & 0xFF, (math_random() & 0x7F) - 0x3F, alpha, &vel);
                spawn.pos.x = pos.x + vel.x;
                spawn.pos.y = pos.y + vel.y;
                spawn.pos.z = pos.z + vel.z;
                spawn.rot.x = spawn.pos.x + dir.x;
                spawn.rot.y = spawn.pos.y + dir.y;
                spawn.rot.z = spawn.pos.z + dir.z;
                spawn.unk44 = math_random() % (flame->line.unk3D + 1) + flame->line.unk3C;
                particle = particle_newQuadParticle(&spawn, &gWhiteVtxColors, 0x1F, 0x22, 0, 0x1A, ((math_random() & 1) ? 1 : 0) | 2, 0, 0, 0x2C,
                                    actor->head.header.owner, actor->head.header.kind);
                if (particle != NULL) {
                    memcpy(&particle->data.line, &lineData, sizeof(lineData));
                }
                flame->lineAccum -= 1.0f;
            } while (flame->lineAccum > 1.0f);
        }
    }
    if (flame->seed.attackFn != -1) {
        gFlameAttackProcs[flame->seed.attackFn](actor);
    }
    if ((flame->flags & 2) && actor->head.count >= 2) {
        Vec3f *end;
        FlamePoint *h;
        s32 i;
        s32 pad9;

        if (actor->head.tmpl.flags & 2) {
            end = &actor->head.tmpl.pos;
        } else {
            i = actor->head.end - 1;
            if (i < 0) {
                i = actor->head.tmpl.itemCount - 1;
            }
            end = &points[i].pos;
        }
        h = &points[actor->head.start];
        mp_addZone((h->pos.x + end->x) * 0.5f, (h->pos.y + end->y) * 0.5f,
                      (h->pos.z + end->z) * 0.5f, flame->length * 0.5f);
    }
    return 1;
}


/* Draws a flame as a camera-facing ribbon through its points (culled when off screen). */
Gfx *flamethrower_drawFlame(CharacterFlamethrowerActor *actor, Gfx *gfx, s16 view) {
    CharacterFlamethrowerFlame *state;
    Vec3f next;
    Vec3f cur;
    s32 index;
    FlamePoint *points;
    Vec3f *camera;
    Vec3f toCamera;
    Vec3f delta;
    Vec3f normal;
    s32 previous;
    f32 lengthSquared;
    u8 flag;
    f32 nextWidth;
    f32 curWidth;
    u8 nextAlpha;
    f32 nextTexture;
    f32 curTexture;
    Vtx_t *vtx;
    u8 curAlpha;

    if (actor->head.count < 2) {
        return gfx;
    }
    state = actor->state;
    points = actor->points;
    if ((state->flags & 0x20) && (gIsMultiplayer == 0) && (state->flags & 0x10)) {
        return gfx;
    }
    if (state->flags & 1) {
        s32 tail;
        FlamePoint *first;
        FlamePoint *last;
        f32 dx;
        f32 dy;
        f32 dz;

        tail = actor->head.end;
        tail--;
        index = actor->head.start;
        if (tail < 0) {
            tail = actor->head.tmpl.itemCount - 1;
        }
        last = &points[index];
        first = &points[tail];
        dx = first->pos.x - last->pos.x;
        dy = first->pos.y - last->pos.y;
        dz = first->pos.z - last->pos.z;
        if (view_testCylinderInView(&gViews[view], gViewMatrices[view],
                          (last->pos.x + first->pos.x) * 0.5f,
                          (last->pos.y + first->pos.y) * 0.5f,
                          (last->pos.z + first->pos.z) * 0.5f,
                          sqrtf(dx * dx + dz * dz), fabsf(dy) + (state->seed.size + state->seed.size), 20000.0f) == 0) {
            return gfx;
        }
    }
    particle_getViewBuffer(actor->buffers, view, (actor->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx_t), &vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    camera = math_getCameraPos(view);
    flag = 1;
    gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[107], 0, 0, 0, 0, 0x6B, 0, 0, &flag, 3);
    gfx = math_gfxUpdateGeometryMode(gfx, 0x200005, 0x1F0600);
    gfx = particle_setCombineMode(gfx, 0x4C, &flag);
    gfx = math_gfxSetOtherMode(gfx, gSpriteTlutMode | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, gEffectRenderModes[3].cycle1 | gEffectRenderModes[3].cycle2, &flag);
    if (actor->head.tmpl.flags & 2) {
        index = actor->head.end;
        index--;
        if (index < 0) {
            index = actor->head.tmpl.itemCount - 1;
        }
        cur = actor->head.tmpl.pos;
        curWidth = state->seed.size;
        curAlpha = state->seed.alpha;
        curTexture = state->texOffset;
    } else {
        previous = actor->head.end;
        previous--;
        if (previous < 0) {
            previous = actor->head.tmpl.itemCount - 1;
        }
        index = previous;
        index--;
        if (index < 0) {
            index = actor->head.tmpl.itemCount - 1;
        }
        cur = points[previous].pos;
        curWidth = points[previous].size;
        curAlpha = points[previous].alpha;
        curTexture = points[previous].texT;
    }
    next = points[index].pos;
    nextWidth = points[index].size;
    nextAlpha = points[index].alpha;
    nextTexture = points[index].texT;
    delta.x = cur.x - next.x;
    delta.y = cur.y - next.y;
    delta.z = cur.z - next.z;
    toCamera.x = cur.x - camera->x;
    toCamera.y = cur.y - camera->y;
    toCamera.z = cur.z - camera->z;
    normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
    normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
    normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
    lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    if (lengthSquared != 0.0f) {
        f32 scale = curWidth / sqrtf(lengthSquared);

        normal.x *= scale;
        normal.y *= scale;
        normal.z *= scale;
    } else {
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
    }
    vtx->ob[0] = cur.x + normal.x;
    vtx->ob[1] = cur.y + normal.y;
    vtx->ob[2] = cur.z + normal.z;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0x47C0;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = curAlpha;
    vtx->flag = 0;
    vtx++;
    vtx->ob[0] = cur.x - normal.x;
    vtx->ob[1] = cur.y - normal.y;
    vtx->ob[2] = cur.z - normal.z;
    vtx->tc[0] = curTexture;
    vtx->tc[1] = 0x4000;
    vtx->cn[0] = 0xFF;
    vtx->cn[1] = 0xFF;
    vtx->cn[2] = 0xFF;
    vtx->cn[3] = curAlpha;
    vtx->flag = 0;
    vtx++;
    do {
        delta.x = cur.x - next.x;
        delta.y = cur.y - next.y;
        delta.z = cur.z - next.z;
        toCamera.x = next.x - camera->x;
        toCamera.y = next.y - camera->y;
        toCamera.z = next.z - camera->z;
        normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
        normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
        normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
        lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
        if (lengthSquared != 0.0f) {
            f32 scale = nextWidth / sqrtf(lengthSquared);

            normal.x *= scale;
            normal.y *= scale;
            normal.z *= scale;
        } else {
            normal.x = 0.0f;
            normal.y = 0.0f;
            normal.z = 0.0f;
        }
        vtx->ob[0] = next.x + normal.x;
        vtx->ob[1] = next.y + normal.y;
        vtx->ob[2] = next.z + normal.z;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0x47C0;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = nextAlpha;
        vtx->flag = 0;
        vtx++;
        vtx->ob[0] = next.x - normal.x;
        vtx->ob[1] = next.y - normal.y;
        vtx->ob[2] = next.z - normal.z;
        vtx->tc[0] = nextTexture;
        vtx->tc[1] = 0x4000;
        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0xFF;
        vtx->cn[2] = 0xFF;
        vtx->cn[3] = nextAlpha;
        vtx->flag = 0;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 1, 3, 2, 0);
        if (nextTexture < curTexture) {
            memcpy(vtx, vtx - 2, 0x20);
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
            vtx[-2].tc[0] -= 0x8000;
            vtx++;
        }
        previous = index;
        index--;
        if (index < 0) {
            index = actor->head.tmpl.itemCount - 1;
        }
        cur = points[previous].pos;
        next = points[index].pos;
        nextWidth = points[index].size;
        nextAlpha = points[index].alpha;
        curTexture = points[previous].texT;
        nextTexture = points[index].texT;
    } while (previous != actor->head.start);
    return gfx;
}

/* Integrates a point over `dt` under `gravity` (applied to vel.y). */
void flamethrower_integratePoint(Vec3f *pos, Vec3f *vel, f32 gravity, f32 dt) {
    Vec3f v;

    v = *vel;
    vel->y += gravity * dt;
    pos->x += v.x * dt;
    pos->y += (v.y * dt) + (0.5f * gravity * dt * dt);
    pos->z += v.z * dt;
}

/* Flame init callback (gFlameInitProcs): clears the owner's flamethrower zone's `inactive` flag. */
void flamethrower_activateFlameZone(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    Actor *owner;
    s32 i;
    s32 found;

    flame = actor->state;
    owner = flame->seed.owner;
    i = 0;
    found = FALSE;
    do {
        if (owner->unique_id == gFlamethrowerCharacterIds[i]) {
            found = TRUE;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        gFlamethrowerZones[i]->inactive = FALSE;
    }
}

/* Flame destroy callback (gFlameDestroyProcs): sets the owner's flamethrower zone's `inactive` flag again. */
void flamethrower_deactivateFlameZone(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    Actor *owner;
    s32 i;
    s32 found;

    flame = actor->state;
    owner = flame->seed.owner;
    i = 0;
    found = FALSE;
    do {
        if (owner->unique_id == gFlamethrowerCharacterIds[i]) {
            found = TRUE;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        gFlamethrowerZones[i]->inactive = TRUE;
    }
}

/* Flame sound-start callback (gFlameSoundStartProcs): starts the looping flame sound; its handle is left in v0. */
void flamethrower_startFlameSound(void) {
    sound_play(0x1AA, 0x7FFF, 0x40, 0, 0);
}

/* Flame callback (gFlameSoundUpdateProcs): moves the flame's sound to its oldest point. */
void flamethrower_moveFlameSound(CharacterFlamethrowerActor *actor) {
    FlamePoint *points;
    CharacterFlamethrowerFlame *flame;
    u16 sound;

    flame = actor->state;
    points = actor->points;
    if (actor->head.count != 0) {
        sound = flame->sound;
        if (sound != 0) {
            sound_setVoiceSpatial(sound, 0x7FFF, 0, 0, 0, points[actor->head.start].pos.x, points[actor->head.start].pos.y,
                          points[actor->head.start].pos.z, 500, 5000);
        }
    }
}

/* Flame sound-stop callback (gFlameSoundStopProcs). */
void flamethrower_stopFlameSound(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame = actor->state;
    u16 sound = flame->sound;
    if (sound != 0) {
        sound_stop(sound);
    }
}

/* Payload of a flame's light (jitter update): the jitter state, then the flame the light follows. */
typedef struct FlameLightData {
    /* 0x00 */ LightJitter jitter;
    /* 0x20 */ CharacterFlamethrowerActor *flame;
} FlameLightData;

/* Creates the light that follows flame `flame` at `pos`, colored (red, green, blue, alpha). */
LightEffect *flamethrower_createFlameLight(CharacterFlamethrowerActor *flame, Vec3f *pos, u8 red, u8 green, u8 blue, u8 alpha, u8 arg6,
                         s32 arg7) {
    LightEffect *light;
    LightEffectHeader header;
    Vec3i header2;
    CharacterFlamethrowerActor *owner;
    LightJitter params;

    params.mid = 35.0f;
    params.low = 5.0f;
    owner = flame;
    header.flags = 2;
    header.update = 2;
    header.lifetime = 300;
    header.lightType = 0x22;
    params.high = 110.0f;
    params.target = 1.0f;
    params.timer = 0.0f;
    params.interval = 7.0f;
    params.smoothing = 0.2488999963f;
    params.level = 127.0f;
    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;

    light = light_createWithEffect(&header, &header2, red, green, blue, alpha, 0xFF, 0, 0x24, arg6, arg7);
    if (light != NULL) {
        memcpy(&light->data.jitter, &params, sizeof(params));
        memcpy(&((FlameLightData *)&light->data)->flame, &owner, sizeof(owner));
    }
    return light;
}

/* Light destroy: detaches it from its flame, then light_defaultDestroy. */
void flamethrower_destroyFlameLight(LightEffect *light) {
    flamethrower_detachFlameLight(light);
    light_defaultDestroy(light);
}

/* Light destroy: detaches it from its flame, then light_defaultFree. */
void flamethrower_freeFlameLight(LightEffect *light) {
    flamethrower_detachFlameLight(light);
    light_defaultFree(light);
}

/* Clears the flame's pointer to this light. */
void flamethrower_detachFlameLight(LightEffect *light) {
    CharacterFlamethrowerFlame *flame = ((FlameLightData *)&light->data)->flame->state;

    flame->light = NULL;
}

/* Flame line particle callback: gravity, per-frame drag, then moves both beam ends (constant acceleration). */
s32 flamethrower_moveFlameLine(SceneParticle *beam) {
    f32 dtSq;
    Vec3f velA;
    Vec3f velB;
    Vec3f accA;
    Vec3f accB;
    FlameLineData *p = &beam->data.line;

    dtSq = gFrameDelta * gFrameDelta;
    velA = p->velA;
    velB = p->velB;
    p->velA.y += p->gravity * gFrameDelta;
    p->velB.y += p->gravity * gFrameDelta;
    {
        s32 i;

        for (i = gFrameTicks; i > 0; i--) {
            p->velA.x *= p->dragA;
            p->velA.y *= p->dragA;
            p->velA.z *= p->dragA;
            p->velB.x *= p->dragB;
            p->velB.y *= p->dragB;
            p->velB.z *= p->dragB;
        }
    }
    accA.x = (p->velA.x - velA.x) * gFrameDeltaInv;
    accA.y = (p->velA.y - velA.y) * gFrameDeltaInv;
    accA.z = (p->velA.z - velA.z) * gFrameDeltaInv;
    accB.x = (p->velB.x - velB.x) * gFrameDeltaInv;
    accB.y = (p->velB.y - velB.y) * gFrameDeltaInv;
    accB.z = (p->velB.z - velB.z) * gFrameDeltaInv;
    beam->head.def.pos.x += velA.x * gFrameDelta + 0.5f * accA.x * dtSq;
    beam->head.def.pos.y += velA.y * gFrameDelta + 0.5f * accA.y * dtSq;
    beam->head.def.pos.z += velA.z * gFrameDelta + 0.5f * accA.z * dtSq;
    beam->head.def.rot.x += velB.x * gFrameDelta + 0.5f * accB.x * dtSq;
    beam->head.def.rot.y += velB.y * gFrameDelta + 0.5f * accB.y * dtSq;
    beam->head.def.rot.z += velB.z * gFrameDelta + 0.5f * accB.z * dtSq;
    return 1;
}

/* Flame line particle callback: per-frame shrink of width and progress, alpha fades over the last frames. */
s32 flamethrower_fadeFlameLine(SceneParticle *beam) {
    s32 i;
    FlameLineData *p = &beam->data.line;

    for (i = gFrameTicks; i > 0; i--) {
        beam->head.def.width = beam->head.def.width - beam->head.def.width * p->shrinkRate;
        beam->head.def.height = beam->head.def.height - beam->head.def.height * p->shrinkRate;
    }
    if (beam->head.def.lifetime < p->fadeStart) {
        s32 alpha = beam->head.def.lifetime * p->alphaPerLife;
        if (alpha < beam->head.def.unk44) {
            beam->head.def.unk44 = alpha;
        }
    }
    return 1;
}

/* Builds a flame line beam's quad (4 vertices) from start towards end, `progress` of the way, facing the camera. */
void *flamethrower_buildFlameLineQuad(SceneParticle *beam, s16 view) {
    Vtx_t *vertices;
    void *result;
    Vec3f *pos;
    Vec3f dir;
    Vec3f mid;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    Vec3f closest;
    f32 t;
    f32 lengthSquared;
    u8 fresh;
    f32 scale;

    particle_getViewBuffer(beam->head.vtxBuffers, view, 0x40, &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(beam->head.vtxBuffers[view], beam->head.vtxTemplate, 0x40);
            memcpy(beam->head.vtxBuffers[view] + 4, beam->head.vtxTemplate, 0x40);
        }
    } else {
        return NULL;
    }
    pos = math_getCameraPos(view);
    dir.x = beam->head.def.rot.x - beam->head.def.pos.x;
    dir.y = beam->head.def.rot.y - beam->head.def.pos.y;
    dir.z = beam->head.def.rot.z - beam->head.def.pos.z;
    mid.x = beam->head.def.pos.x + dir.x * beam->head.def.height;
    mid.y = beam->head.def.pos.y + dir.y * beam->head.def.height;
    mid.z = beam->head.def.pos.z + dir.z * beam->head.def.height;
    dir.x = mid.x - beam->head.def.pos.x;
    dir.y = mid.y - beam->head.def.pos.y;
    dir.z = mid.z - beam->head.def.pos.z;
    math_closestPointOnSegment(&beam->head.def.pos, &dir, pos, &closest, &t);
    dx = closest.x - pos->x;
    dy = closest.y - pos->y;
    dz = closest.z - pos->z;
    nx = dir.y * dz - dy * dir.z;
    ny = dir.z * dx - dz * dir.x;
    nz = dir.x * dy - dx * dir.y;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared != 0.0f) {
        scale = beam->head.def.width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    lbl: ; // matching: an unused label ends the block where IDO needs it
    } else {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    }
    vertices->ob[0] = beam->head.def.pos.x + nx;
    vertices->ob[1] = beam->head.def.pos.y + ny;
    vertices->ob[2] = beam->head.def.pos.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->ob[0] = beam->head.def.pos.x - nx;
    vertices->ob[1] = beam->head.def.pos.y - ny;
    vertices->ob[2] = beam->head.def.pos.z - nz;
    vertices->flag = 0;
    vertices++;
    vertices->ob[0] = mid.x - nx;
    vertices->ob[1] = mid.y - ny;
    vertices->ob[2] = mid.z - nz;
    vertices->flag = 0;
    vertices++;
    vertices->ob[0] = mid.x + nx;
    vertices->ob[1] = mid.y + ny;
    vertices->ob[2] = mid.z + nz;
    vertices->flag = 0;
    return result;
}

/* State of the flames flamethrower_spawnControllerFlame spawns: a flame plus the two owner states it stays alive in. */
typedef struct CharacterFlamethrowerFlameEx {
    /* 0x000 */ CharacterFlamethrowerFlame flame;
    /* 0x154 */ u8 pad154[4];
    /* 0x158 */ s32 aliveState; /* owner->unk84.uh values the flame stays alive in */
    /* 0x15C */ s32 aliveState2;
} CharacterFlamethrowerFlameEx;

/* Flame update callback (gFlameUpdateProcs): keeps the flame only while its owner's unk84 is one of aliveState. */
s32 flamethrower_checkOwnerAlive(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlameEx *state = actor->state;
    Actor *owner = state->flame.seed.owner;
    u16 v = owner->animseq.uh;

    if (v != state->aliveState) {
        if (v != state->aliveState2) {
            return 0;
        }
    }
    return 1;
}

/* Flame callbacks (gFlameAttackProcs): flamethrower_attackAlongFlame with different spacings and kinds. */
s32 flamethrower_attackProc0(CharacterFlamethrowerActor *actor) {
    flamethrower_attackAlongFlame(actor, 0.25f, 0x1C);
    return 0;
}

s32 flamethrower_attackProc1(CharacterFlamethrowerActor *actor) {
    flamethrower_attackAlongFlame(actor, 0.5f, 0x1D);
    return 0;
}

s32 flamethrower_attackProc2(CharacterFlamethrowerActor *actor) {
    flamethrower_attackAlongFlame(actor, 0.2f, 0x1D);
    return 0;
}

/* An object that carries a character's flamethrower (callbacks flamethrower_spawnControllerFlame/flamethrower_updateController). */
struct FlamethrowerController {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Actor *owner;
    /* 0x2C */ u8 ownerId;
    /* 0x2D */ u8 pad2D[3];
    /* 0x30 */ s32 aliveState[2];
};

/* Spawns the controller owner's flame from a built-in setup. */
void flamethrower_spawnControllerFlame(FlamethrowerController *ctrl) {
    CharacterFlamethrowerActor *flame;
    CharacterFlamethrowerSetup setup;
    Actor *obj;
    s32 pad; // matching: unused stack slot
    CharacterFlamethrowerFlameEx *state;

    setup.seed.owner = obj = ctrl->owner;
    setup.seed.ownerId = obj->unique_id;
    setup.seed.bone = 9;
    setup.seed.startOffset.x = 7.0f;
    setup.seed.startOffset.y = -147.0f;
    setup.seed.startOffset.z = 54.0f;
    setup.seed.endOffset.x = 7.0f;
    setup.seed.endOffset.y = -228.0f;
    setup.seed.endOffset.z = 80.0f;
    setup.seed.gravity = 0.8700000048f;
    setup.seed.emitRate = 0.6970000267f;
    setup.seed.maxLength = 703.0f;
    setup.seed.fadeStart = 0.5600000024f;
    setup.seed.size = 8.400000572f;
    setup.seed.sizeRate = 6.599999905f;
    setup.seed.alpha = 200;
    setup.seed.initFn = -1;
    setup.seed.attackFn = 0;
    setup.seed.destroyFn = -1;
    setup.seed.soundFn = 0;
    setup.seed.soundUpdateFn = 0;
    setup.seed.soundStopFn = 0;
    setup.seed.texSpeed = -234.0f;
    setup.seed.texScale = 9.579999924f;
    setup.seed.updateFn = 0;
    setup.kind = 15;
    setup.flags = 15;
    setup.red = 60;
    setup.green = 255;
    setup.blue = 235;
    setup.alpha = 82;
    setup.line.rate = 0.001949999947f;
    setup.line.rateRange = 0.00699999975f;
    setup.line.lifeMin = 20;
    setup.line.lifeRange = 13;
    setup.line.unkC = 0.503000021f;
    setup.line.unk14 = 0.5120000243f;
    setup.line.unk10 = 0.2990000248f;
    setup.line.unk18 = 0.3920000196f;
    setup.line.speedMin = 0.5550000072f;
    setup.line.speedRange = 0.7080000043f;
    setup.line.gravityMin = 0.04200000316f;
    setup.line.gravityRange = 0.3410000205f;
    setup.line.dragA = 0.973477006f;
    setup.line.dragB = 0.9759860039f;
    setup.line.fadeStart = 12;
    setup.line.alphaPerLife = 21;
    setup.line.position = 0.4360000193f;
    setup.line.unk3C = 100;
    setup.line.unk3D = 80;
    setup.line.shrinkRate = 0.006000000052f;
    setup.scatter.target.x = 7.0f;
    setup.scatter.target.y = -218.0f;
    setup.scatter.target.z = 150.0f;
    setup.scatter.rate = 0.2040000111f;
    setup.scatter.rateRange = 0.4030000269f;
    setup.scatter.unk14 = 4;
    setup.scatter.unk18 = 3;
    setup.scatter.unk1C = 50;
    setup.scatter.unk1E = 21;
    setup.scatter.unk20 = 4.0f;
    setup.scatter.unk24 = 6.0f;
    setup.scatter.speedMin = 0.1000000015f;
    setup.scatter.speedRange = 0.1000000015f;
    setup.scatter.spread = 50.0f;
    setup.scatter.unk34 = 0.3110000193f;
    setup.scatter.unk38 = 0.3080000281f;
    setup.scatter.unk3C = 80;
    setup.scatter.unk3E = 175;
    setup.scatter.unk40 = 22;
    setup.scatter.unk42 = 11;
    setup.emit.target.x = 7.0f;
    setup.emit.target.y = -218.0f;
    setup.emit.target.z = 150.0f;
    setup.emit.rate = 0.05000000075f;
    setup.emit.rateRange = 0.351000011f;
    setup.emit.unk14 = 60;
    setup.emit.unk16 = 20;
    setup.emit.unk18 = 155;
    setup.emit.unk1A = 100;
    setup.emit.unk1C = 134.0f;
    setup.emit.unk20 = 120.0f;
    setup.emit.speedMin = 0.05000000075f;
    setup.emit.speedRange = 0.06000000238f;
    setup.emit.spread = 43.0f;
    setup.emit.unk30 = -0.25f;
    setup.emit.unk34 = 0.1780000031f;
    setup.emit.unk38 = 23;
    setup.emit.unk3A = 11;
    flame = flamethrower_spawnFlame(sizeof(ctrl->aliveState), &setup, ctrl->base.header.owner, ctrl->base.header.kind);
    if (flame != NULL) {
        state = flame->state;
        memcpy(&state->aliveState, ctrl->aliveState, sizeof(ctrl->aliveState));
    }
}

/* Controller callback: kills the controller once its owner is gone or replaced. */
void flamethrower_updateController(FlamethrowerController *ctrl) {
    Actor *owner = ctrl->owner;

    if (owner->type != 0 && owner->character_type != 0xFF && owner->unique_id == ctrl->ownerId) {
        return;
    }
    ctrl->base.header.state = -1;
    ctrl->base.header.flags |= 1;
}

/* Calls actor_runAttackAt(kind) at points spaced flame->length * fraction along the flame, oldest first. */
void flamethrower_attackAlongFlame(CharacterFlamethrowerActor *actor, f32 fraction, s32 kind) {
    f32 spacing;
    f32 factor;
    f32 total;
    f32 next;
    FlamePoint *points;
    s32 index;
    CharacterFlamethrowerFlame *state;
    FlamePoint *point;
    Actor *owner;
    Vec3f *previous;

    state = actor->state;
    points = actor->points;
    previous = &actor->head.tmpl.pos;
    if (actor->head.count >= 2) {
        f32 interpolated_z; // matching: z goes through this local (other functions' code moves without it)

        total = 0.0f;
        spacing = state->length * fraction;
        index = actor->head.end;
        next = 0.0f;
        do {
            f32 distance;

            index--;
            if (index < 0) {
                index = actor->head.tmpl.itemCount - 1;
            }
            point = &points[index];
            distance = point->length;
            total += distance;
            if (next <= total) {
                owner = state->seed.owner;
                factor = (total - next) / distance;
                actor_runAttackAt(point->pos.x + ((previous->x - point->pos.x) * factor),
                    point->pos.y + ((previous->y - point->pos.y) * factor),
                    (interpolated_z = point->pos.z + ((previous->z - point->pos.z) * factor)), kind,
                    owner - movement_obj,
                    owner->angle, 0, 0);
                next += spacing;
            }
            previous = &point->pos;
        } while (index != actor->head.start);
    }
}
