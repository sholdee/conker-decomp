#include <ultra64.h>
#define func_151422C0 func_151422C0_header
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_1513D2F0 func_1513D2F0_hdr
#define func_1513D524 func_1513D524_hdr
#define func_15195AA8 func_15195AA8_hdr
#include "functions.h"
#undef func_151422C0
#undef func_150ADA20
#undef func_1513D2F0
#undef func_1513D524
#undef func_15195AA8
u32 func_150ADA20(void);
void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, u8 argA, s32 argB);
#define D_800DBFF0 D_800DBFF0_hdr
#define D_800BE628 D_800BE628_hdr
#define D_800DD198 D_800DD198_hdr
#define D_800E0900 D_800E0900_hdr
#include "variables.h"
#undef D_800DBFF0
#undef D_800BE628
#undef D_800DD198
#undef D_800E0900

void *memcpy(void *, const void *, u32);

void func_1519CDB0(s32, f32, s32);
extern void *allocate_memory(s32, s32, s32, s32);

s32 func_151422C0(s32, const char *, s32, s32, const char *, s32);
struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);

extern f32 D_800A8AF8;

/* Generic doubly linked list node; every list allocated by func_151957B0 starts with one. */
typedef struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

/* Head/tail of the texture-scroll reference list (TexScrollRef) and the texture-scroll list (TexScroll). */
extern ListNode *D_800E08E0;
extern ListNode *D_800E08E4;
extern ListNode *D_800E08E8;
extern ListNode *D_800E08EC;
/* Next automatic (negative) texture-scroll id. */
extern s32 D_800E08F0;

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
typedef struct TexScroll {
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
    /* 0x1C */ s16 cmdIndex[2][5]; /* per main display list (D_800B0E00[0] / [1]), -1 = unused */
    /* 0x30 */ u8 cmdCount[2][5];
} TexScroll;

/* Allocates a `size`-byte list node and appends it to the list (*head, *tail). */
void *func_151957B0(s32 size, ListNode **head, ListNode **tail) {
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
void func_1519582C(void) {
    D_800E08E0 = D_800E08E4 = NULL;
    D_800E08E8 = D_800E08EC = NULL;
    D_800E08F0 = -2;
}

/*
 * Finds the `occurrence`-th G_SETTIMG in `dl` that loads texture D_800B0E58[texture] (any texture when
 * `texture` is 0). Returns the index of the G_SETTILESIZE that follows it, or -1, and stores in *levels how
 * many (G_SETTILESIZE, G_SETTILE) pairs follow.
 */
s32 func_15195868(Gfx *dl, s32 texture, s32 occurrence, s32 *levels) {
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
        if ((dl[i].words.w1 == D_800B0E58[texture] || texture == 0) && occurrence-- <= 0) {
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
TexScroll *func_15195984(s32 id, s32 size, s32 pos, s32 doubled) {
    TexScroll *scroll;
    s32 i;
    s32 width;
    s32 height;

    scroll = func_151957B0(sizeof(TexScroll), &D_800E08E8, &D_800E08EC);
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

s16 func_15195A84(s16 cmdIndex, s32 arg1, s32 arg2, s32 arg3) {
    return cmdIndex;
}

/*
 * Registers every (or the `occurrence`-th, when not -1) use of texture `texture` in main display list `which`
 * of `dl` with a TexScroll scrolling by (speedS, speedT).
 */
TexScroll *func_15195AA8(Gfx *dl, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 which, s32 speedS,
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
        idx = func_15195868(dl, texture, i, &levels);
        if (idx == -1) {
            occurrence = 0;
        } else {
            if (texture == 0) {
                texture = D_800E08F0;
            }
            size = dl[idx].words.w1;
            pos = dl[idx].words.w0;
            scroll = (TexScroll *)D_800E08E8;
            // matching: dead store left over from func_15195DD4's lookup slot; removing it moves branches
            if (isNew) {
                slot = -1;
            } else {
                slot = texture;
            }
            while (scroll != NULL && scroll->id != 0) {
                scroll = scroll->next;
            }
            if (scroll == NULL) {
                scroll = func_15195984(texture, size, pos, doubled);
                if (scroll != NULL) {
                    isNew = FALSE;
                    scroll->speedS = speedS;
                    scroll->speedT = speedT;
                } else {
                    return NULL;
                }
            } else {
                idx = func_15195A84(idx, (s32)scroll, size, doubled);
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
    if (texture == D_800E08F0) {
        D_800E08F0--;
    }
    return scroll;
}

/* Adds a reference to the `count` tile-size commands at dl[cmdIndex] to `scroll`, unless already present. */
void func_15195D00(TexScroll *scroll, Gfx *dl, s32 cmdIndex, s32 count) {
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
        next = func_151957B0(sizeof(TexScrollRef), &D_800E08E0, &D_800E08E4);
        ref->next = next;
    } else {
        next = func_151957B0(sizeof(TexScrollRef), &D_800E08E0, &D_800E08E4);
        scroll->refs = next;
    }
    next->dl = dl;
    next->cmdIndex = cmdIndex;
    next->count = count;
    next->next = NULL;
}

/* Like func_15195AA8, but records the uses in an arbitrary display list `dl` (as TexScrollRefs). */
TexScroll *func_15195DD4(Gfx *dl, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS,
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
    if (D_800BEAC0 != 0) {
        return NULL;
    }
    if (occurrence == -1) {
        i = 0;
    }
    do {
        idx = func_15195868(dl, texture, i, &levels);
        if (idx == -1) {
            occurrence = 0;
        } else {
            if (texture == 0) {
                texture = D_800E08F0;
            }
            if (isNew) {
                key = -1;
            } else {
                key = texture;
            }
            scroll = (TexScroll *)D_800E08E8;
            while (scroll != NULL && key != scroll->id) {
                scroll = scroll->next;
            }
            size = dl[idx].words.w1;
            pos = dl[idx].words.w0;
            if (scroll == NULL) {
                scroll = func_15195984(texture, size, pos, doubled);
                if (scroll != NULL) {
                    isNew = FALSE;
                    scroll->speedS = speedS;
                    scroll->speedT = speedT;
                } else {
                    return NULL;
                }
            } else {
                idx = func_15195A84(idx, (s32)scroll, size, doubled);
                isNew = FALSE;
            }
            if (scroll->levelCount < levels) {
                scroll->levelCount = levels;
            }
            func_15195D00(scroll, dl, idx, levels);
            i++;
        }
    } while (occurrence == -1);
    if (texture == D_800E08F0) {
        D_800E08F0--;
    }
    return scroll;
}

typedef struct {
    char pad_0x0[0x1C];
    Gfx *dl;
} ResourceEntry;

TexScroll *func_15195FB0(ResourceEntry *res, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS,
                         s32 speedT) {
    return func_15195DD4(res->dl, texture, doubled, occurrence, isNew, speedS, speedT);
}

/* Advances every TexScroll and rewrites the G_SETTILESIZE commands it is registered in. */
void func_15195FF0(Gfx *dl0, Gfx *dl1) {
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

    scroll = (TexScroll *)D_800E08E8;
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
        if (D_800BEAC0 == 0) {
            x += scroll->speedS * D_800BE9E4;
            y += scroll->speedT * D_800BE9E4;
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
void func_15196318(TexScroll *scroll, s32 speedS, s32 speedT) {
    if (scroll != NULL) {
        scroll->speedS = speedS;
        scroll->speedT = speedT;
    }
}

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* One point of an effect's ring buffer (0x24 bytes). */
typedef struct Particle {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 size;
    /* 0x1C */ s16 life;
    /* 0x1E */ s8 holdTimer;
    /* 0x1F */ u8 alpha;
    /* 0x20 */ u8 texS; /* texture s offset */
    /* 0x21 */ u8 spawnedChild;
    /* 0x22 */ u8 pad22[0x2];
} Particle;

/* Effect object created by func_15147A80: a ring buffer of points plus a kind-specific state block. */
typedef struct CharacterFlamethrowerActor {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ char pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ char padD[0x3];
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ char pad1C[0x2];
    /* 0x1E */ u16 flags1E;
    /* 0x20 */ char pad20[0x5];
    /* 0x25 */ u8 capacity;
    /* 0x26 */ char pad26[0x6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail; /* oldest point */
    /* 0x2E */ s8 head; /* next point to write */
    /* 0x2F */ char pad2F[0x1];
    /* 0x30 */ s8 unk30;
    /* 0x31 */ char pad31[0xA];
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ char pad3C[0x18];
    /* 0x54 */ Vec3f tailPos;
    /* 0x60 */ char pad60[0x24];
    /* 0x84 */ u8 buffers[0x10];
    /* 0x94 */ void *points;
    /* 0x98 */ void *state;
} CharacterFlamethrowerActor;

/* Spawn header passed to func_15147A80. */
typedef struct EffectSpawn {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 lifetime;
    /* 0x0E */ u16 kind;
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 mode;
    /* 0x15 */ u8 id;
    /* 0x16 */ u8 pad16[6];
} EffectSpawn;

/* Preset in D_800A87A0 that func_15196438 copies into a CharacterFlamethrowerJet. */
typedef struct CharacterFlamethrowerJetTemplate {
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
} CharacterFlamethrowerJetTemplate;

/* State block (0x90 bytes) of a jet effect: emits particles between two bone-relative points. */
typedef struct CharacterFlamethrowerJet {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id */
    /* 0x05 */ u8 bone;
    /* 0x06 */ u8 flags;
    /* 0x07 */ s8 speedFn; /* D_8008F870 index, -1 = none */
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
    /* 0x60 */ s8 initFn;    /* D_8008F87C index */
    /* 0x61 */ s8 updateFn;  /* D_8008F880 index */
    /* 0x62 */ s8 destroyFn; /* D_8008F88C index */
    /* 0x63 */ s8 soundFn;   /* D_8008F890 index: starts the sound */
    /* 0x64 */ s8 soundUpdateFn; /* D_8008F894 index */
    /* 0x65 */ s8 soundStopFn;   /* D_8008F898 index */
    /* 0x66 */ u16 sound;
    /* 0x68 */ s8 unk68;     /* D_8008F89C index */
    /* 0x69 */ u8 childPreset; /* D_800A8710 index */
    /* 0x6A */ u8 childLife; /* points whose life drops below this spawn a child (func_15198110) */
    /* 0x6C */ f32 childRate;
    /* 0x70 */ f32 childAccum;
    /* 0x74 */ u8 scatterPreset; /* D_800A8728 index (func_15198D88) */
    /* 0x78 */ f32 scatterRate;
    /* 0x7C */ f32 scatterAccum;
    /* 0x80 */ u8 spritePreset; /* D_800A8770 index (func_151990AC) */
    /* 0x84 */ f32 spriteRate;
    /* 0x88 */ f32 spriteAccum;
    /* 0x8C */ u8 headFadeCount;
    /* 0x8D */ u8 headFadeStep;
    /* 0x8E */ u8 tailFadeCount;
    /* 0x8F */ u8 tailFadeStep;
} CharacterFlamethrowerJet;

extern void (*D_8008F898[])(void *);
extern void (*D_8008F88C[])(void *);
void func_151478F4(void *);
void func_15147928(void *);

/* Runs a jet's destroy callbacks, then func_151478F4. */
void func_15196330(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;

    jet = actor->state;
    if (jet->soundStopFn != -1) {
        D_8008F898[jet->soundStopFn](actor);
    }
    if (jet->destroyFn != -1) {
        D_8008F88C[jet->destroyFn](actor);
    }
    func_151478F4(actor);
}

/* Runs a jet's destroy callbacks, then func_15147928. */
void func_151963B4(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;

    jet = actor->state;
    if (jet->soundStopFn != -1) {
        D_8008F898[jet->soundStopFn](actor);
    }
    if (jet->destroyFn != -1) {
        D_8008F88C[jet->destroyFn](actor);
    }
    func_15147928(actor);
}

extern CharacterFlamethrowerJetTemplate D_800A87A0[];
extern s16 (*D_8008F890[])(void);
extern f32 (*D_8008F870[])(void *, s32);
extern void (*D_8008F87C[])(void *);
void func_15143134(Vec3f *in, Vec3f *out, s32 matrix);
void *func_15147A80(void *spawn, s32 stateSize, s32 pointSize, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns a jet effect from preset D_800A87A0[preset], attached to a bone of `owner`. */
CharacterFlamethrowerActor *func_15196438(struct127 *owner, s32 preset, u8 arg2, s32 arg3) {
    EffectSpawn spawn;
    CharacterFlamethrowerJet jet;
    CharacterFlamethrowerActor *actor;
    s32 matrix;
    CharacterFlamethrowerJetTemplate *entry;
    CharacterFlamethrowerJet *state;

    if (owner == NULL) {
        return NULL;
    }
    if (owner->unk1D4 == NULL) {
        return NULL;
    }
    entry = &D_800A87A0[preset];
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
    spawn.lifetime = entry->lifetime;
    jet.gravity = entry->gravity;
    jet.emitRate = entry->emitRate;
    jet.shade = entry->shade;
    spawn.id = entry->id;
    jet.bone = entry->bone;
    jet.startOffset = entry->startOffset;
    jet.endOffset = entry->endOffset;
    jet.speedFn = entry->speedFn;
    jet.fadeRate = entry->fadeRate;
    jet.life = entry->life;
    jet.holdTime = entry->holdTime;
    spawn.kind = entry->kind;
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
        jet.sound = D_8008F890[jet.soundFn]();
    } else {
        jet.sound = 0;
    }
    matrix = (s32)jet.owner->unk1D4 + (jet.bone << 6);
    func_15143134(&jet.startOffset, &jet.start, matrix);
    func_15143134(&jet.endOffset, &jet.end, matrix);
    spawn.flags = 3;
    spawn.pos.x = jet.start.x;
    spawn.pos.y = jet.start.y;
    spawn.pos.z = jet.start.z;
    jet.timeCarry = 0.0f;
    actor = func_15147A80(&spawn, sizeof(CharacterFlamethrowerJet), sizeof(Particle), 2, 2, 2, 0, 0, 0, arg2, arg3);
    if (actor != NULL) {
        state = actor->state;
        memcpy(actor->state, &jet, sizeof(CharacterFlamethrowerJet));
        if (state->speedFn != -1) {
            state->speed = D_8008F870[state->speedFn](actor, state->speedFn);
        } else {
            state->speed = 0.0f;
        }
        if (jet.initFn != -1) {
            D_8008F87C[jet.initFn](actor);
        }
    }
    return actor;
}

extern void (*D_8008F894[])(void *);
extern void (*D_8008F89C[])(void *);
extern s32 (*D_8008F880[])(void *);
void func_15198110();

/* Per-frame jet update: moves, fades and retires its particles, then runs its update callbacks. */
s32 func_15196748(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    Particle *points;
    s32 i;
    s16 n;
    u8 scale;
    s8 hold;
    u8 jetFlags;
    u8 flags; /* doubles as the fade-in alpha */

    jet = actor->state;
    points = actor->points;
    if (actor->count < 2 && (jet->flags & 1)) {
        return 0;
    }
    i = actor->head;
    while (i != actor->tail) {
        i--;
        if (i < 0) {
            i = actor->capacity - 1;
        }
        points[i].vel.y += jet->gravity * D_800BE9A4;
        points[i].pos.x += points[i].vel.x * D_800BE9A4;
        points[i].pos.y += points[i].vel.y * D_800BE9A4;
        points[i].pos.z += points[i].vel.z * D_800BE9A4;
        points[i].alpha = 255;
        hold = points[i].holdTimer;
        if (hold > 0) {
            points[i].holdTimer = hold - D_800BE9E4;
        } else {
            points[i].life -= jet->fadeRate * D_800BE9E4;
        }
        if (points[i].life < jet->childLife) {
            if (points[i].spawnedChild == 0) {
                func_15198110(actor, i);
            }
        }
        points[i].size += jet->sizeRate * D_800BE9A4;
        if (points[i].life < 0) {
            /* retire this point and every older one */
            jet->flags &= ~2;
            while (i != actor->tail) {
                actor->tail = actor->tail + 1;
                if (actor->tail == actor->capacity) {
                    actor->tail = 0;
                }
                actor->count = actor->count - 1;
            }
            points[actor->tail].life = 0;
        }
    }
    i = actor->tail;
    // matching: the extra copy through jetFlags is needed
    jetFlags = jet->flags;
    flags = jetFlags;
    if (flags & 2) {
        /* fade in the oldest points */
        flags = 0;
        n = jet->headFadeCount;
        do { // matching: one-line body (-g3 line numbers steer as1's scheduling)
            points[i].alpha = flags; i++; flags += jet->headFadeStep; if (i == actor->capacity) { i = 0; } n--;
        } while (n != 0 && i != actor->head);
        flags = jet->flags;
    }
    if (flags & 4) {
        /* fade out the newest points */
        scale = 0;
        n = jet->tailFadeCount;
        i = actor->head - 1;
        if (i < 0) {
            i = actor->capacity - 1;
        }
        do { // matching: one-line body (-g3 line numbers steer as1's scheduling)
            points[i].alpha = (points[i].alpha * scale) >> 8; i--; scale += jet->tailFadeStep; if (i < 0) { i = actor->capacity - 1; } n--;
        } while (n != 0 && i != actor->head);
    }
    if (jet->soundUpdateFn != -1) {
        D_8008F894[jet->soundUpdateFn](actor);
    }
    if (jet->unk68 != -1) {
        D_8008F89C[jet->unk68](actor);
    }
    if (jet->updateFn != -1) {
        if (D_8008F880[jet->updateFn](actor) == 0) {
            return 0;
        }
    }
    if (actor->count > 0) {
        actor->tailPos = points[actor->tail].pos;
    } else {
        actor->tailPos.x = 0.0f;
        actor->tailPos.y = 0.0f;
        actor->tailPos.z = 0.0f;
    }
    return 1;
}


void func_15198D88(CharacterFlamethrowerActor *actor, Vec3f *end);
void func_151990AC(CharacterFlamethrowerActor *actor, Vec3f *end);

/*
 * Emits a jet's particles for this frame: tracks its bone-relative start/end points, then spawns
 * emitAccum particles interpolated between last frame's and this frame's points (and sparks/smoke).
 */
s32 func_15196B4C(CharacterFlamethrowerActor *actor, s32 arg1) {
    CharacterFlamethrowerJet *jet;
    Particle *points;
    Vec3f end;
    s32 unused; // matching: unused locals (also pad4-pad7) keep the original frame size
    s32 matrix;
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
    f32 fq;
    s32 head;
    s32 tail;
    f32 pad4;
    f32 pad5;
    f32 pad6;
    f32 pad7;

    jet = actor->state;
    points = actor->points;
    if (jet->owner->unk1D4 == NULL) {
        return 0;
    }
    if (jet->speedFn != -1) {
        jet->targetSpeed = D_8008F870[jet->speedFn](actor, arg1);
    }
    matrix = (s32)jet->owner->unk1D4 + (jet->bone << 6);
    func_15143134(&jet->startOffset, &actor->pos, matrix);
    func_15143134(&jet->endOffset, &end, matrix);
    jet->emitAccum += jet->emitRate * D_800BE9A4;
    if (jet->emitAccum > 1.0f) {
        do { // matching: a plain block here reorders the float rodata
            inv = 1.0f / jet->emitAccum;
            t = jet->timeCarry + D_800BE9A4;
            fq = t;
            cur = jet->start;
            cur2 = jet->end;
            tstep = t * inv;
            size = jet->size + (jet->sizeRate * t);
            speed = jet->speed;
            dx = actor->pos.x - jet->start.x;
            dy = actor->pos.y - jet->start.y;
            dz = actor->pos.z - jet->start.z;
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
                func_15198D88(actor, &end);
            }
            jet->spriteAccum += jet->spriteRate;
            if (jet->spriteAccum > 1.0f) {
                func_151990AC(actor, &end);
            }
            points[actor->head].spawnedChild = 0;
            points[actor->head].pos.x = cur.x;
            points[actor->head].pos.y = cur.y;
            points[actor->head].pos.z = cur.z;
            points[actor->head].vel.x = (cur2.x - cur.x) * speed;
            points[actor->head].vel.y = (cur2.y - cur.y) * speed;
            points[actor->head].vel.z = (cur2.z - cur.z) * speed;
            points[actor->head].size = size;
            points[actor->head].life = jet->life;
            points[actor->head].holdTimer = jet->holdTime;
            points[actor->head].alpha = 255;
            points[actor->head].texS = jet->texS;
            jet->texS += (func_150ADA20() % 6) + 3;
            points[actor->head].vel.y += jet->gravity * t;
            points[actor->head].pos.x += points[actor->head].vel.x * t;
            points[actor->head].pos.y += points[actor->head].vel.y * t;
            points[actor->head].pos.z += points[actor->head].vel.z * t;
            t -= tstep;
            head = ++actor->head;
            if ((head & 0xFFFFFFFF) == actor->capacity) { // matching: no-op mask steers the compare
                head = (actor->head = 0);
            }
            actor->count++;
            tail = actor->tail;
            if (tail == head) {
                actor->tail = tail + 1;
                if (actor->tail == actor->capacity) {
                    actor->tail = 0;
                }
                actor->count--;
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

typedef struct CharacterFlamethrowerBeamCamera {
    u8 unknown0[0x2F8];
    Vec3f position;
    u8 unknown304[0x69C];
} CharacterFlamethrowerBeamCamera;

void func_151D5D60(void *, s16, s32, void **, u8 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern CharacterFlamethrowerBeamCamera *D_800DBFF0;

typedef struct {
    s32 id;
    u16 width;
    u16 height;
    u8 unk8;
    u8 unk9;
    u8 fmt;
} TexSrc15197148;

typedef struct {
    s32 id;
    s32 *idp;
    u8 fmt;
    u16 width;
    u16 height;
    u8 unkE;
    u8 unkF;
} TexDesc15197148;

extern TexSrc15197148 D_800915B0;
extern s32 D_800D2C9C;
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);

/* Draws a jet as a camera-facing ribbon through its points, oldest last. */
Gfx *func_15197148(CharacterFlamethrowerActor *actor, Gfx *gfx, s16 view) {
    Vtx_t *vtx;
    Vec3f *eye;
    Vec3f next;
    Vec3f cur;
    Particle *points;
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

    if (actor->count >= 2) {
        func_151D5D60(actor->buffers, view, (actor->capacity * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
        if (vtx != NULL) {
            eye = &D_800DBFF0[view].position;
            jet = actor->state;
            points = actor->points;
            flag = 1;
            {
                TexDesc15197148 tex;

                tex.id = D_800915B0.id;
                tex.idp = &tex.id;
                tex.fmt = D_800915B0.fmt;
                tex.width = D_800915B0.width;
                tex.height = D_800915B0.height;
                tex.unkE = D_800915B0.unk8;
                tex.unkF = D_800915B0.unk9;
                gfx = (Gfx *)func_15142E24((s32)gfx, (s32)&tex.idp, 0, 0, 0x100, 0x100, 0, 4, 0, &flag, 3);
                gfx = func_15142C10(gfx, jet->shade, jet->shade, jet->shade, 0xFF, &flag);
                gfx = func_15142B7C(gfx, 1, 0x160600);
                gfx = func_1513F4E4(gfx, 0xB, &flag);
                gfx = func_15142FBC(gfx, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, 0x5049D8, &flag);
                if (actor->flags1E & 2) {
                    j = actor->head - 1;
                    if (j < 0) {
                        j = actor->capacity - 1;
                    }
                    cur = actor->pos;
                    next = points[j].pos;
                    nextTexS = points[j].texS;
                    curTexS = points[j].texS;
                    width = jet->size;
                } else {
                    i = actor->head - 1;
                    if (i < 0) {
                        i = actor->capacity - 1;
                    }
                    j = i - 1;
                    if (j < 0) {
                        j = actor->capacity - 1;
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
                        j = actor->capacity - 1;
                    }
                    cur = points[i].pos;
                    next = points[j].pos;
                    nextTexS = points[j].texS;
                } while (i != actor->tail);
            }
        }
    }
    return gfx;
}


f32 func_151979F8(s32 arg0) {
    return 1.32f;
}

f32 func_15197A0C(s32 arg0) {
    return func_151422C0(10, "power", 1, 500, "../Effects/CharacterFlameThrower/characterflamethrower.c", 2252) * 0.01f;
}

f32 func_15197A68(s32 arg0) {
    return 10.9f;
}

/* Message payload naming an effect owner (func_15147D64). */
typedef struct struct127Ref {
    struct127 *owner;
    u8 ownerId;
} struct127Ref;

extern void func_15147D64(struct127Ref *, s32);

/* Sends message 8 for `owner`, which stops its jets (see func_15198054). */
void func_15197A7C(struct127 *owner) {
    struct127Ref ref;

    if (owner != NULL) {
        ref.owner = owner;
        ref.ownerId = owner->unique_id;
        func_15147D64(&ref, 8);
    }
}

/* User data of the line particles a jet spawns (func_15198110), at SceneParticle 0x110. */
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

/* User data of the line particles a flame sheds (func_1519A9A4), at SceneParticle 0x110. */
typedef struct FlameLineData {
    /* 0x00 */ Vec3f velA;
    /* 0x0C */ Vec3f velB;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
} FlameLineData;

/* A particle of the shared particle system (func_1513D2F0/func_1513D524); only the fields used here. */
typedef struct SceneParticle {
    /* 0x000 */ char pad0[0x1C];
    /* 0x01C */ s16 life;
    /* 0x01E */ char pad1E[0xE];
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ f32 unk30;
    /* 0x034 */ Vec3f posA;
    /* 0x040 */ Vec3f posB;
    /* 0x04C */ char pad4C[0x10];
    /* 0x05C */ u8 alpha;
    /* 0x05D */ char pad5D[0x110 - 0x5D];
    /* 0x110 */ union {
        JetChildData jet;
        FlameLineData line;
    } data;
} SceneParticle;

/* Particle callback: applies drag and gravity to both end points' velocities, then moves them. */
s32 func_15197AB4(SceneParticle *p) {
    p->data.jet.velA.x *= p->data.jet.dragA;
    p->data.jet.velA.y += p->data.jet.gravity * D_800BE9A4;
    p->data.jet.velA.z *= p->data.jet.dragA;
    p->data.jet.velB.x *= p->data.jet.dragB;
    p->data.jet.velB.y += p->data.jet.gravity * D_800BE9A4;
    p->data.jet.velB.z *= p->data.jet.dragB;
    p->posA.x += p->data.jet.velA.x * D_800BE9A4;
    p->posA.y += p->data.jet.velA.y * D_800BE9A4;
    p->posA.z += p->data.jet.velA.z * D_800BE9A4;
    p->posB.x += p->data.jet.velB.x * D_800BE9A4;
    p->posB.y += p->data.jet.velB.y * D_800BE9A4;
    p->posB.z += p->data.jet.velB.z * D_800BE9A4;
    return 1;
}

/* Particle callback: shrinks unk2C/unk30 by shrinkRate and fades alpha out over the last fadeStart frames. */
s32 func_15197BBC(SceneParticle *p) {
    s16 life;

    life = p->life;
    p->unk30 = p->unk30 - (p->unk30 * p->data.jet.shrinkRate);
    p->unk2C = p->unk2C - (p->unk2C * p->data.jet.shrinkRate);
    if (life < p->data.jet.fadeStart) {
        p->alpha = life * p->data.jet.alphaPerLife;
    }
    return 1;
}

typedef struct CharacterFlamethrowerBeamVertex {
    s16 x, y, z, flag;
    u8 other[8];
} CharacterFlamethrowerBeamVertex;

typedef struct CharacterFlamethrowerBeam {
    u8 unknown0[0x2C];
    f32 width;
    f32 progress;
    Vec3f start;
    Vec3f end;
    u8 unknown4C[0x74];
    u8 vertexTemplate[0x40];
    u8 *buffers[1];
} CharacterFlamethrowerBeam;


/* Builds the beam quad (4 vertices) from start towards end, `progress` of the way, facing the camera. */
void *func_15197C10(CharacterFlamethrowerBeam *beam, s16 view) {
    CharacterFlamethrowerBeamVertex *vertices;
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

    func_151D5D60(beam->buffers, view, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(beam->buffers[view], beam->vertexTemplate, 0x40);
            memcpy(beam->buffers[view] + 0x40, beam->vertexTemplate, 0x40);
        }
    } else {
        return NULL;
    }
    progress = beam->progress;
    dx = beam->end.x - beam->start.x;
    dy = beam->end.y - beam->start.y;
    dz = beam->end.z - beam->start.z;
    tip.x = beam->start.x + dx * progress;
    tip.y = beam->start.y + dy * progress;
    tip.z = beam->start.z + dz * progress;
    eye = &D_800DBFF0[view].position;
    dx = tip.x - beam->start.x;
    dy = tip.y - beam->start.y;
    dz = tip.z - beam->start.z;
    vx = beam->start.x - eye->x;
    vy = beam->start.y - eye->y;
    vz = beam->start.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = beam->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = beam->start.x + nx;
    vertices->y = beam->start.y + ny;
    vertices->z = beam->start.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = beam->start.x - nx;
    vertices->y = beam->start.y - ny;
    vertices->z = beam->start.z - nz;
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
        scale = beam->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = tip.x + nx;
    vertices->y = tip.y + ny;
    vertices->z = tip.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = tip.x - nx;
    vertices->y = tip.y - ny;
    vertices->z = tip.z - nz;
    vertices->flag = 0;
    return result;
}

/* Payload of the messages sent to effects through func_15147D64. */
typedef struct EffectMessage {
    /* 0x0 */ struct127 *owner;
    /* 0x4 */ union {
        u8 id;              /* messages 0, 2, 8 and 0x26: owner's unique_id */
        struct127 *other; /* message 0x2D: owner and other swap */
    } u4;
    /* 0x8 */ u8 ownerId; /* message 0x2D */
    /* 0x9 */ u8 otherId; /* message 0x2D */
} EffectMessage;

void func_151993B4(CharacterFlamethrowerActor *actor);

/* Message handler: dies when its owner is killed (0, 2, 8); follows an owner swap (0x2D). */
void func_15198054(CharacterFlamethrowerActor *actor, EffectMessage *msg, u8 kind) {
    struct127Ref *ref = actor->state;

    if (kind == 0 || kind == 2 || kind == 8) {
        if (msg->owner == ref->owner || msg->u4.id == ref->ownerId) {
            func_151993B4(actor);
        }
    } else if (kind == 0x2D) {
        if (ref->owner == msg->owner) {
            ref->owner = msg->u4.other;
            ref->ownerId = msg->otherId;
        } else if (ref->owner == msg->u4.other) {
            ref->owner = msg->owner;
            ref->ownerId = msg->ownerId;
        }
    }
}

/* Preset in D_800A8710 for the child particles a jet point spawns (func_15198110). */
typedef struct JetChildPreset {
    /* 0x0 */ f32 sizeScale;
    /* 0x4 */ f32 unk4;
    /* 0x8 */ s16 lifeMin;
    /* 0xA */ s16 lifeRange;
} JetChildPreset;

/* Spawn description for func_1513D524. */
typedef struct SceneParticleSpawn {
    /* 0x00 */ u8 texture;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 life;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ Vec3f posA;
    /* 0x28 */ Vec3f posB;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
} SceneParticleSpawn;

extern JetChildPreset D_800A8710[];
extern SceneParticle *func_1513D524(SceneParticleSpawn *, u8, u8, u8, u8, u8, s32, u8, s32);

/*
 * Called when jet point `index` fades below childLife: spawns line particles (moved and faded by
 * func_15197AB4/func_15197BBC) around the segment between it and the next newer point.
 */
void func_15198110(CharacterFlamethrowerActor *actor, s32 index) {
    CharacterFlamethrowerJet *jet;
    Particle *points;
    Particle *cur;
    Particle *next;
    JetChildPreset *preset;
    s32 i;
    SceneParticle *ret;
    u8 a;
    u8 b;
    SceneParticleSpawn spawn;
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
    preset = &D_800A8710[jet->childPreset];
    cur = (Particle *)((u8 *)points + index * sizeof(Particle)); // matching: byte-offset form keeps the addu operand order
    jet->childAccum += jet->childRate;
    cur->spawnedChild = TRUE;
    if ((i = index + 1) == actor->capacity) {
        i = 0;
    }
    if (jet->childAccum > 1.0f && i != actor->head) {
        s32 pad[2]; // matching: unused stack slots

        spawn.unk8 = 0;
        data.unk20 = 25137.0f;
        data.unk24 = 6733.0f;
        data.unk2C = 0;
        spawn.unkC = (data.unk20 + data.unk24) * 0.5f;
        data.gravity = jet->gravity;
        data.fadeStart = 8;
        data.alphaPerLife = 32;
        data.dragA = 0.8280000091f;
        data.dragB = 0.753000021f;
        data.shrinkRate = -0.0120000001f;
        data.unk0 = preset->sizeScale * 30.09054375f;
        data.unk4 = preset->sizeScale * 18.35523224f;
        data.unk1C = 0;
        spawn.unk14 = (data.unk0 + data.unk4) * 0.5f;
        data.unk8 = spawn.unk14;
        data.unkC = preset->unk4 * 3.961740017f;
        data.unk10 = preset->unk4 * 2.919177055f;
        data.unk14 = preset->unk4 * 8.116548538f;
        data.unk1E = 0;
        spawn.unk34 = 1.0f;
        spawn.unk3C = 1.0f;
        spawn.unk40 = 0x40000001;
        spawn.unk44 = 255;
        spawn.unk45 = 255;
        spawn.unk18 = (data.unkC + data.unk10) * 0.5f;
        data.unk18 = spawn.unk18;
        spawn.unk38 = 0.0f;
        if (func_150ADA20() & 1) {
            spawn.texture = 0x13;
        } else {
            spawn.texture = 0x14;
        }
        spawn.unk10 = 0;
        spawn.unk11 = 0;
        spawn.unk12 = 0;
        spawn.unk13 = 0x68;
        spawn.unk2 = 0x301;
        next = (Particle *)((u8 *)points + i * sizeof(Particle));
        data.velA = next->vel;
        data.velB = cur->vel;
        do {
            a = func_150ADA20() & 0xFF;
            b = func_150ADA20() & 0xFF;
            sinA = func_151423D8((u8)(a - 0x40));
            cosA = func_151423D8(a);
            sinB = func_151423D8((u8)(b - 0x40));
            cosB = func_151423D8(b);
            rad = next->size * 0.8000000119f;
            spawn.life = (func_150ADA20() % (u32)(preset->lifeRange + 1)) + preset->lifeMin;
            rc = rad * cosB;
            dx = rc * sinA;
            dy = -rad * sinB;
            dz = rc * cosA;
            spawn.posA.x = next->pos.x + dx;
            spawn.posA.y = next->pos.y + dy;
            spawn.posA.z = next->pos.z + dz;
            spawn.posB.x = cur->pos.x + dx;
            spawn.posB.y = cur->pos.y + dy;
            spawn.posB.z = cur->pos.z + dz;
            if (func_150ADA20() & 1) {
                flag = 1;
            } else {
                flag = 0;
            }
            ret = func_1513D524(&spawn, 0xA, 0xB, 0, 9, (u8)(flag | 2), 0x5C, 0xFF, 0);
            if (ret != NULL) {
                memcpy(&ret->data, &data, sizeof(data));
            }
            jet->childAccum -= 1.0f;
        } while (jet->childAccum > 1.0f);
    }
}

extern void func_1505D024(struct127 *, s32, u16, s32);
extern s32 func_1518D1C0(struct127 *, s32, s32, s32, s32, s32, const char *);

/* Jet update callback: a point inside object 0's (the player's) cylinder hits it and retires the older points. */
s32 func_15198570(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    Particle *points;
    struct127 *obj;
    s32 i;
    Vec3f pos;
    f32 r;
    f32 h;
    f32 d;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 p1;

    jet = actor->state;
    points = actor->points;
    obj = &D_800CC2D0[0];
    i = actor->head;
    while (i != actor->tail) {
        i--;
        if (i < 0) {
            i = actor->capacity - 1;
        }
        pos = points[i].pos;
        r = obj->unkE4;
        h = obj->unkE6;
        dx = pos.x - obj->x_position;
        dz = pos.z - obj->z_position;
        dy = pos.y - (obj->y_position + (s16)obj->unkE8);
        dy *= (1.0f + r / h) * 0.5f;
        d = dx * dx + dy * dy + dz * dz;
        if (d < r * r) {
            func_1505D024(obj, 0x60006, obj->unk7A, jet->owner - D_800CC2D0);
            func_1518D1C0(obj, 0xB, 0, 1, actor->unkC, actor->unk1, "Characterflamethrower");
            while (i != actor->tail) {
                actor->tail = actor->tail + 1;
                if (actor->tail == actor->capacity) {
                    actor->tail = 0;
                }
                actor->count = actor->count - 1;
            }
        }
    }
    return 1;
}

/* Jet update callback: a point near any other active object (of the first 25) retires the older points. */
s32 func_151987CC(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    Particle *points;
    struct127 *obj;
    s32 i;
    s32 j;
    Vec3f pos;
    f32 r;
    f32 h;
    f32 d;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 p1;
    f32 p3;

    jet = actor->state;
    points = actor->points;
    for (j = 0; j < 25; j++) {
        obj = &D_800CC2D0[j];
        if (obj->interaction_state == 0 || obj->unk65 != 0 || jet->owner->unique_id == obj->unique_id) {
            continue;
        }
        {
            i = actor->head;
            while (i != actor->tail) {
                i--;
                if (i < 0) {
                    i = actor->capacity - 1;
                }
                pos = points[i].pos;
                r = obj->unkE4;
                h = obj->unkE6;
                dx = pos.x - obj->x_position;
                dz = pos.z - obj->z_position;
                dy = pos.y - (obj->y_position + (s16)obj->unkE8);
                dy *= (1.0f + r / h) * 0.5f;
                if (dx * dx + dy * dy + dz * dz < r * r + 2500.0f) {
                    while (i != actor->tail) {
                        actor->tail = actor->tail + 1;
                        if (actor->tail == actor->capacity) {
                            actor->tail = 0;
                        }
                        actor->count = actor->count - 1;
                    }
                }
            }
        }
    }
    return 1;
}

/* An object in the D_800DCE50 lists (only the fields used here). */
typedef struct ListedObject {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ struct ListedObject *next;
    /* 0x0C */ u8 padC[0x91 - 0xC];
    /* 0x91 */ u8 unk91;
    /* 0x92 */ s16 unk92;
    /* 0x94 */ s32 unk94;
    /* 0x98 */ Vec3f pos;
    /* 0xA4 */ u8 padA4[0x4];
    /* 0xA8 */ f32 unkA8;
    /* 0xAC */ u8 padAC[0x8];
    /* 0xB4 */ f32 unkB4;
    /* 0xB8 */ f32 unkB8;
    /* 0xBC */ u8 padBC[0xC];
    /* 0xC8 */ f32 unkC8;
    /* 0xCC */ u8 padCC[0xED - 0xCC];
    /* 0xED */ u8 unkED;
} ListedObject;

/* Object lists, and the stack of saved `next` pointers (depth D_800DD190) that makes walking them reentrant. */
extern ListedObject *D_800DCE50[];
extern ListedObject *D_800DD198[];
void func_15183ACC(s32);
s32 func_1510F8D8(s32, s32, s32, s32);

/*
 * Jet update callback, active while the owner's unk31C->unk58 is 1: objects in lists 0x15-0x17 that a
 * point touches are hit (func_15168B10 message 0x18) and put into state 5.
 */
s32 func_1519897C(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet = actor->state;
    Particle *points = actor->points;
    struct127 *owner;
    struct126 *sub;
    ListedObject *head;
    ListedObject *node;
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
    sub = owner->unk31C;
    if (sub == NULL) {
        return 0;
    }
    if (sub->unk58 != 1) {
        owner->unk2F8 &= ~0x100;
        return 0;
    }
    if (&points) { // matching: address-taken points stays in its stack slot
    }
    for (i = 0; i < 3; i++) {
        head = D_800DCE50[lists[i]];
        if (head != NULL) {
            node = head;
            j = actor->head;
            D_800DD190++;
            while (node != NULL) {
                D_800DD198[D_800DD190] = node->next;
                nx = node->pos.x;
                ny = node->pos.y;
                nz = node->pos.z;
                while (j != actor->tail) {
                    j--;
                    if (j < 0) {
                        j = actor->capacity - 1;
                    }
                    pos = points[j].pos;
                    r = points[j].size;
                    dx = pos.x - nx;
                    dy = pos.y - ny;
                    dz = pos.z - nz;
                    if (dx * dx + dy * dy + dz * dz < r * r + 900.0f) {
                        func_15183ACC(5);
                        func_15168B10((s32)node, 0x18);
                        node->unkED = 5;
                        node->unk94 = *(s32 *)((u8 *)D_800DDE80 + 0x6C);
                        node->unk92 = 9;
                        node->unkB8 = 40.0f;
                        node->unkA8 = 40.0f;
                        node->unkB4 = 0.0f;
                        node->unkC8 = func_1510F8D8(node->pos.x, node->pos.y, node->pos.z, 0);
                        node->unk91 = 4;
                    }
                }
                node = D_800DD198[D_800DD190];
            }
            D_800DD190--;
        }
    }
    return 1;
}

/* Jet sound-start callback (D_8008F890): starts the looping flame sound; its handle is left in v0. */
void func_15198C60(void) {
    func_10010F30(0x1AA, 0x7FFF, 0x40, 0, 0);
}

/* Jet callback (D_8008F894): moves the jet's sound to its oldest point. */
void func_15198C90(CharacterFlamethrowerActor *actor) {
    Particle *points;
    CharacterFlamethrowerJet *jet;
    u16 sound;

    jet = actor->state;
    points = actor->points;
    if (actor->count != 0) {
        sound = jet->sound;
        if (sound != 0) {
            func_1000F91C(sound, 0x7FFF, 0, 0, 0, points[actor->tail].pos.x, points[actor->tail].pos.y,
                          points[actor->tail].pos.z, 500, 5000);
        }
    }
}

/* Jet destroy callback (D_8008F898): stops the jet's sound. */
void func_15198D40(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    u16 sound;

    jet = actor->state;
    if (actor->count != 0) {
        sound = jet->sound;
        if (sound != 0) {
            func_100111C8(sound);
        }
    }
}

void func_15198D7C(s32 arg0) {
}

/* Preset in D_800A8728 for the particles func_15198D88 scatters around a jet's end point. */
typedef struct JetScatterPreset {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} JetScatterPreset;

/* Spawn description, motion and style for func_15147DA0. */
typedef struct {
    Vec3f pos;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    s8 unk14;
    u8 unk15;
    u8 pad16[2];
} ScatterSpawn;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    s16 unk1E;
} ScatterMotion;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
} ScatterStyle;

extern JetScatterPreset D_800A8728[];
extern s32 func_15147DA0(ScatterSpawn *, ScatterMotion *, s32, s32, s32, s32, s32, s32, s32, s32, s32, ScatterStyle *, s32, s32, s32);

/* Spawns scatterAccum particles (func_15147DA0) flying from the actor towards random points around `end`. */
void func_15198D88(CharacterFlamethrowerActor *actor, Vec3f *end) {
    ScatterStyle style;
    CharacterFlamethrowerJet *jet;
    JetScatterPreset *preset;
    ScatterSpawn spawn;
    ScatterMotion motion;
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
    preset = &D_800A8728[jet->scatterPreset];
    spawn.unkE = 1;
    motion.unk19 = 0xA;
    motion.unk1A = 0xFF;
    motion.unk18 = 0x28;
    motion.unk1B = 0x82;
    spawn.pos = actor->pos;
    style.unk0 = 0;
    style.unk4 = 1;
    style.unk8 = 0x160600;
    style.unkC = 3;
    style.unk10 = 0x10;
    style.unk14 = 0x80;
    style.unk18 = 0x20;
    style.unk1C = 0;
    style.unk1D = 9;
    do {
        speed = (func_150ADA68() * preset->unk8) + preset->unk4;
        a = func_150ADA20() & 0xFF;
        b = func_150ADA20() & 0xFF;
        sinA = func_151423D8((u8)(a - 0x40));
        cosA = func_151423D8(a);
        sinB = func_151423D8((u8)(b - 0x40));
        cosB = func_151423D8(b);
        rad = preset->unk0;
        px = end->x + (rad * cosB * sinA);
        py = end->y - (rad * sinB);
        pz = end->z + (rad * cosB * cosA);
        spawn.unk15 = (func_150ADA20() % (u32)(preset->unk22 + 1)) + preset->unk20;
        spawn.unkC = (func_150ADA20() % (u32)(preset->unk1E + 1)) + preset->unk1C;
        motion.unk0 = (func_150ADA68() * preset->unk10) + preset->unkC;
        motion.unk10 = (func_150ADA68() * preset->unk18) + preset->unk14;
        motion.unk4 = (px - actor->pos.x) * speed;
        motion.unk8 = (py - actor->pos.y) * speed;
        motion.unkC = (pz - actor->pos.z) * speed;
        func_15147DA0(&spawn, &motion, 0, 1, 0, 0, 0, 0, 0, 0, 0, &style, 0, actor->unkC, actor->unk1);
        jet->scatterAccum -= 1.0f;
    } while (jet->scatterAccum > 1.0f);
}

typedef struct {
    f32 radius;
    f32 speed;
    f32 speed_range;
    s16 acceleration;
    s16 acceleration_range;
    s16 fade;
    s16 fade_range;
    s16 size;
    s16 size_range;
} EmissionPreset151990AC;

typedef struct {
    void *texture;
    char pad4[8];
    s16 frame;
    s16 frame_rate;
    s16 position[3];
    s16 velocity_x;
    s16 velocity_z;
    char pad1A[2];
    u8 fraction_x;
    u8 fraction_y;
    u8 fraction_z;
    s8 callback;
    s16 velocity_y;
    s16 acceleration;
    s16 width;
    s16 height;
    s16 lifetime;
    s8 fade;
    char pad2B;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 environment[3];
    char pad33;
    u16 flags;
    char pad36[2];
} Emission151990AC;

void *func_15167D84(void *, s32, s32, s8, u8, s32);
extern u8 D_80090514[];
extern EmissionPreset151990AC D_800A8770[];

/* Emits spriteAccum sprites (func_15167D84) flying from the actor towards random points around `end`. */
void func_151990AC(CharacterFlamethrowerActor *actor, Vec3f *end) {
    f32 speed;
    Emission151990AC packet;
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
    EmissionPreset151990AC *preset;
    CharacterFlamethrowerJet *jet;

    jet = actor->state;
    preset = &D_800A8770[jet->spritePreset];
    packet.position[0] = actor->pos.x;
    packet.position[1] = actor->pos.y;
    packet.position[2] = actor->pos.z;
    packet.fraction_x = 0;
    packet.fraction_z = 0;
    packet.fraction_y = 0;
    packet.red = 0xFF;
    packet.green = 0xFF;
    packet.blue = 0xFF;
    packet.environment[0] = 0;
    packet.environment[1] = 0;
    packet.environment[2] = 0;
    packet.alpha = 0xFF;
    packet.callback = -1;
    packet.flags = 0x12;
    packet.frame = 0;
    packet.frame_rate = 0;
    packet.texture = D_80090514;
    packet.lifetime = 300;
    do {
        speed = func_150ADA68() * preset->speed_range + preset->speed;
        rnd = func_150ADA20();
        angle1 = rnd & 255;
        rnd = func_150ADA20();
        angle2 = rnd & 255;
        cosine1 = func_151423D8(angle1 - 64);
        sine1 = func_151423D8(angle1);
        cosine2 = func_151423D8(angle2 - 64);
        sine2 = func_151423D8(angle2);
        x = end->x + preset->radius * sine2 * cosine1;
        y = end->y - preset->radius * cosine2;
        z = end->z + preset->radius * sine2 * sine1;
        packet.velocity_x = (x - actor->pos.x) * speed;
        packet.velocity_y = (y - actor->pos.y) * speed;
        packet.velocity_z = (z - actor->pos.z) * speed;
        packet.acceleration = func_150ADA20() % (u32)(preset->acceleration_range + 1) + preset->acceleration;
        packet.fade = func_150ADA20() % (u32)(preset->fade_range + 1) + preset->fade;
        packet.width = packet.height = func_150ADA20() % (u32)(preset->size_range + 1) + preset->size;
        func_15167D84(&packet, 0, 0, -1, actor->unkC, actor->unk1);
        jet->spriteAccum -= 1.0f;
    } while (jet->spriteAccum > 1.0f);
}

/* Stops a jet: no new points, and it dies once fewer than 2 remain while the newest fade out. */
void func_151993B4(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;

    jet = actor->state;
    actor->unk30 = 0;
    actor->flags1E &= ~2;
    jet->flags |= 1;
    jet->flags |= 4;
}

/* Per-character flamethrower slots, indexed like D_800A8A9C. */
typedef struct FlamethrowerSlot {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 active;
} FlamethrowerSlot;

extern FlamethrowerSlot *D_800E0900[];
/* unique_ids of the 6 characters that have a FlamethrowerSlot. */
extern u8 D_800A8A9C[];

/* Jet init callback (D_8008F87C): clears the owner's FlamethrowerSlot `active` flag. */
void func_151993E4(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    struct127 *owner;
    s32 i;
    s32 found;

    jet = actor->state;
    owner = jet->owner;
    i = 0;
    found = FALSE;
    do {
        if (owner->unique_id == D_800A8A9C[i]) {
            found = TRUE;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        D_800E0900[i]->active = FALSE;
    }
}

/* Jet destroy callback (D_8008F88C): sets the owner's FlamethrowerSlot `active` flag again. */
void func_1519944C(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerJet *jet;
    struct127 *owner;
    s32 i;
    s32 found;

    jet = actor->state;
    owner = jet->owner;
    i = 0;
    found = FALSE;
    do {
        if (owner->unique_id == D_800A8A9C[i]) {
            found = TRUE;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        D_800E0900[i]->active = TRUE;
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

/* First 0x4C bytes of a flame's state, also given in its CharacterFlamethrowerSetup. */
typedef struct FlameSeed {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id */
    /* 0x05 */ u8 bone;
    /* 0x06 */ u8 flags;
    /* 0x07 */ u8 pad07;
    /* 0x08 */ Vec3f startOffset;
    /* 0x14 */ Vec3f endOffset;
    /* 0x20 */ f32 gravity;
    /* 0x24 */ f32 emitRate;
    /* 0x28 */ f32 maxLength;
    /* 0x2C */ f32 size;
    /* 0x30 */ f32 sizeRate;
    /* 0x34 */ s8 initFn;      /* D_8008F8A4 index */
    /* 0x35 */ s8 unk35;       /* D_8008F8A8 index */
    /* 0x36 */ s8 destroyFn;   /* D_8008F8B4 index */
    /* 0x37 */ s8 soundFn;     /* D_8008F8B8 index */
    /* 0x38 */ s8 unk38;       /* D_8008F8BC index */
    /* 0x39 */ s8 soundStopFn; /* D_8008F8C0 index */
    /* 0x3A */ u8 alpha;
    /* 0x3B */ u8 pad3B;
    /* 0x3C */ f32 fadeStart; /* fraction of the length after which points fade out */
    /* 0x40 */ f32 texSpeed;
    /* 0x44 */ f32 texScale;
    /* 0x48 */ s8 updateFn; /* D_8008F8C4 index */
    /* 0x49 */ u8 pad49[3];
} FlameSeed;

/* Parameters of the line particles (func_1513D2F0) a flame sheds near its far end; random values are min + range. */
typedef struct FlameLineParams {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ s16 lifeMin;
    /* 0x0A */ s16 lifeRange;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 speedMin;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ f32 position; /* fraction of the length where they spawn */
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ u8 unk3D;
    /* 0x3E */ u8 pad3E[2];
    /* 0x40 */ f32 unk40;
} FlameLineParams;

/* Parameters of the particles (func_15147DA0) a flame scatters towards `target`; random values are min + range. */
typedef struct FlameScatterParams {
    /* 0x00 */ Vec3f target; /* bone-relative */
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 speedMin;
    /* 0x2C */ f32 speedRange;
    /* 0x30 */ f32 spread;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ s16 unk42;
} FlameScatterParams;

/* Parameters of the particles (func_15130280) a flame emits towards `target`; random values are min + range. */
typedef struct FlameEmitParams {
    /* 0x00 */ Vec3f target; /* bone-relative */
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 speedMin;
    /* 0x28 */ f32 speedRange;
    /* 0x2C */ f32 spread;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
} FlameEmitParams;

/* State block of a flame effect (func_151994B8): a ribbon of FlamePoints between two bone-relative points. */
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
    /* 0x148 */ struct225 *light; /* from func_1519C09C */
    /* 0x14C */ f32 length;
    /* 0x150 */ u8 flags;
    /* 0x151 */ s8 timer151;
    /* 0x152 */ s16 sparkTimer;
} CharacterFlamethrowerFlame;

/* Spawn parameters for func_151994B8. */
typedef struct CharacterFlamethrowerSetup {
    /* 0x00 */ s32 kind;
    /* 0x04 */ FlameSeed seed;
    /* 0x50 */ u8 red;
    /* 0x51 */ u8 green;
    /* 0x52 */ u8 blue;
    /* 0x53 */ u8 alpha;
    /* 0x54 */ u8 flags;
    /* 0x55 */ u8 pad55[3];
    /* 0x58 */ FlameLineParams line;
    /* 0x9C */ FlameScatterParams scatter;
    /* 0xE0 */ FlameEmitParams emit;
} CharacterFlamethrowerSetup;

void func_15145EA4(Vec3f **in, Vec3f **out, s32 matrix, s32 count);
struct225 *func_1519C09C(s32 arg0, f32 *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7);
extern void (*D_8008F8A4[])(void *);
extern s16 (*D_8008F8B8[])(void);

/* Spawns a flame effect with `extraSize` extra state bytes; setup->flags & 1 also attaches a light. */
CharacterFlamethrowerActor *func_151994B8(s32 extraSize, CharacterFlamethrowerSetup *setup, u8 arg2, s32 arg3) {
    CharacterFlamethrowerActor *result;
    EffectSpawn spawn;
    CharacterFlamethrowerFlame initial;
    struct127 *owner;
    Vec3f *inputs[2];
    Vec3f *outputs[2];
    CharacterFlamethrowerFlame *state;

    owner = setup->seed.owner;
    if (owner->unk1D4 == NULL) {
        return NULL;
    }
    spawn.id = setup->kind;
    spawn.lifetime = 300;
    spawn.kind = 0x12;
    spawn.flags = 0xE;
    spawn.mode = 0;
    initial.seed = setup->seed;
    inputs[0] = &initial.seed.startOffset;
    inputs[1] = &initial.seed.endOffset;
    outputs[0] = &initial.start;
    outputs[1] = &initial.end;
    func_15145EA4(inputs, outputs, (s32)owner->unk1D4 + (initial.seed.bone << 6), 2);
    {
        s32 flag8;
        s32 flag4;
        s32 flag2;
        s32 flag32;
        s32 flags;

        spawn.pos = initial.start;
        initial.timeCarry = 0.0f;
        initial.emitAccum = 0.0f;
        initial.alpha = (f32)(u32)initial.seed.alpha;
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
        result = func_15147A80(&spawn, extraSize + 0x158, sizeof(FlamePoint), 0xC,
                               0xC, 0xC, 0, 0, 0, arg2, arg3);
        if (result != NULL) {
            state = result->state;
            memcpy(state, &initial, sizeof(CharacterFlamethrowerFlame));
            if (initial.seed.soundFn != -1) {
                state->sound = D_8008F8B8[initial.seed.soundFn]();
            } else {
                state->sound = 0;
            }
            if (initial.seed.initFn != -1) {
                D_8008F8A4[initial.seed.initFn](result);
            }
            if (setup->flags & 1) {
                state->light = func_1519C09C((s32)result, &spawn.pos.x, setup->red, setup->green, setup->blue,
                                         setup->alpha, arg2, arg3);
            } else {
                state->light = NULL;
            }
        }
    }
    return result;
}

/* Sends message 0x26 for `owner`, which stops its flames (see func_1519986C). */
void func_15199834(struct127 *owner) {
    struct127Ref ref;

    if (owner != NULL) {
        ref.owner = owner;
        ref.ownerId = owner->unique_id;
        func_15147D64(&ref, 0x26);
    }
}

void func_1516972C();

/* Message handler: dies when its owner is killed (0, 2, 0x26); follows an owner swap (0x2D). */
void func_1519986C(CharacterFlamethrowerActor *actor, EffectMessage *msg, u8 kind) {
    struct127Ref *ref = actor->state;

    if (kind == 0 || kind == 2 || kind == 0x26) {
        if (msg->owner == ref->owner || msg->u4.id == ref->ownerId) {
            func_1516972C(actor);
        }
    } else if (kind == 0x2D) {
        if (ref->owner == msg->owner) {
            ref->owner = msg->u4.other;
            ref->ownerId = msg->otherId;
        } else if (ref->owner == msg->u4.other) {
            ref->owner = msg->owner;
            ref->ownerId = msg->ownerId;
        }
    }
}

void func_15199980(CharacterFlamethrowerActor *actor);

/* Flame destroy: callbacks, then func_151478F4. */
void func_15199928(CharacterFlamethrowerActor *actor) {
    func_15199980(actor);
    func_151478F4(actor);
}

/* Flame destroy: callbacks, then func_15147928. */
void func_15199954(CharacterFlamethrowerActor *actor) {
    func_15199980(actor);
    func_15147928(actor);
}

extern void (*D_8008F8C0[])(void *);
extern void (*D_8008F8B4[])(void *);

/* Runs a flame's destroy callbacks and removes its light. */
void func_15199980(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    struct225 *light;

    flame = actor->state;
    if (flame->seed.soundStopFn != -1) {
        D_8008F8C0[flame->seed.soundStopFn](actor);
    }
    if (flame->seed.destroyFn != -1) {
        D_8008F8B4[flame->seed.destroyFn](actor);
    }
    light = flame->light;
    if (light != NULL) {
        func_1516972C((struct102 *)light);
    }
}

extern s32 (*D_8008F8C4[])(void *);
extern void (*D_8008F8BC[])(void *);

void func_1519BE1C(Vec3f *pos, Vec3f *vel, f32 gravity, f32 dt);

/* Per-frame flame update: moves its points under gravity and keeps the light at the oldest point. */
s32 func_15199A10(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    FlamePoint *points;
    s32 i;

    flame = actor->state;
    points = actor->points;
    flame->flags &= ~0x10;
    if (actor->count < 2) {
        if (actor->flags1E & 8) {
            return 0;
        }
    }
    if (flame->seed.updateFn != -1) {
        if (D_8008F8C4[flame->seed.updateFn](actor) == 0) {
            return 0;
        }
    }
    i = actor->head;
    while (i != actor->tail) {
        FlamePoint *pt;

        i--;
        if (i < 0) {
            i = actor->capacity - 1;
        }
        pt = &points[i];
        func_1519BE1C(&pt->pos, &pt->vel, flame->seed.gravity, D_800BE9A4);
        pt->size += flame->seed.sizeRate * D_800BE9A4;
    }
    {
        FlamePoint *oldest;

        if (flame->light != NULL && actor->count >= 2) {
            struct225 *light = flame->light;
            struct226 *out = light->unk14;

            oldest = &points[actor->tail];
            out->unkE = oldest->pos.x;
            out->unk10 = oldest->pos.y;
            out->unk12 = oldest->pos.z;
        }
    }
    if (flame->seed.unk38 != -1) {
        D_8008F8BC[flame->seed.unk38](actor);
    }
    flame->flags &= ~1;
    if (actor->count > 0) {
        actor->tailPos = points[actor->tail].pos;
    } else {
        actor->tailPos.x = 0.0f;
        actor->tailPos.y = 0.0f;
        actor->tailPos.z = 0.0f;
    }
    return 1;
}

/* Spawn description for func_15130280. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    Vec3f pos;
    Vec3f unk3C;
    Vec3f vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} FlameEmitParticle;

/* Spawn description for func_151D8868. */
typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 pad7;
} FlameSpark;

void func_15145740(void *, void *, void *, void *, f32);
s32 func_15146078(Vec3f *, Vec3f *, Vec3f *);
void func_15143874(s16, f32, f32 *, f32 *);
void *func_15130280(void *, u8, s32, s32, u8, s32);
void func_151D8868(void *, s32, s32, s32);

/*
 * Flame emitter: tracks the owner's bone (or its position and facing when it has no bones), emits
 * emitAccum points between last and this frame's start/end, then the scatter and emit particles and sparks.
 */
s32 func_15199C34(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    FlamePoint *points;
    struct127 *obj;
    Vec3f pos0;
    Vec3f pos1;
    Vec3f pos2;
    Vec3f pos3;

    flame = actor->state;
    points = actor->points;
    obj = flame->seed.owner;
    if (obj->interaction_state == 0 || obj->unique_id != flame->seed.ownerId) {
        return 0;
    }
    if ((obj->unk74 & 0xF) == 0xF) {
        flame->flags |= 0x10;
    } else {
        flame->flags &= ~0x10;
    }
    if (obj->unk1D4 != NULL) {
        Vec3f *inputs[4];
        Vec3f *outputs[4];

        inputs[0] = &flame->seed.startOffset;
        inputs[1] = &flame->seed.endOffset;
        inputs[2] = &flame->scatter.target;
        inputs[3] = &flame->emit.target;
        outputs[0] = &pos0;
        outputs[1] = &pos1;
        outputs[2] = &pos2;
        outputs[3] = &pos3;
        func_15145EA4(inputs, outputs, (s32)obj->unk1D4 + (flame->seed.bone << 6), 4);
        flame->flags &= ~1;
    } else {
        Vec3f dir;
        Vec3f up;

        func_15145740(obj, &dir, &up, 0, 0.0f);
        pos0.x = obj->x_position;
        pos0.y = obj->y_position + 33.0f;
        pos0.z = obj->z_position;
        pos1.x = dir.x * 0.03200000152f + pos0.x;
        pos1.y = dir.y * 0.03200000152f + pos0.y;
        pos1.z = dir.z * 0.03200000152f + pos0.z;
        flame->flags |= 1;
    }
    actor->pos = pos0;
    flame->emitAccum += flame->seed.emitRate * D_800BE9A4;
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
        t = flame->timeCarry + D_800BE9A4;
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

            p = &points[actor->head];
            p->pos = cur0;
            p->vel.x = cur1.x - cur0.x;
            p->vel.y = cur1.y - cur0.y;
            p->vel.z = cur1.z - cur0.z;
            p->size = flame->seed.size;
            p->length = 0.0f;
            p->alpha = flame->seed.alpha;
            p->texT = flame->texOffset;
            func_1519BE1C(&p->pos, &p->vel, flame->seed.gravity, t);
            p->size += flame->seed.sizeRate * t;
            actor->head++;
            if (actor->head == actor->capacity) {
                actor->head = 0;
            }
            actor->count++;
            if (actor->head == actor->tail) {
                actor->tail++;
                if (actor->tail == actor->capacity) {
                    actor->tail = 0;
                }
                actor->count--;
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
    if (obj->unk1D4 != NULL && (obj->unk74 & 0xF) != 0xF) {
        flame->scatterAccum += (flame->scatter.rate + func_150ADA68() * flame->scatter.rateRange) * D_800BE9A4;
        if (flame->scatterAccum > 1.0f) {
            s32 pad; // matching: unused stack slot (also pad2 below)
            ScatterSpawn spawn;
            ScatterMotion motion;
            ScatterStyle style;
            Vec3f delta;
            Vec3f axisA;
            Vec3f axisB;
            f32 c;
            f32 s;
            f32 speed;

            style.unk4 = 0x200005;
            style.unk8 = 0x1F0600;
            style.unkC = 3;
            style.unk10 = 0x22;
            style.unk0 = 0;
            style.unk14 = 0x80;
            style.unk18 = 0x20;
            style.unk1C = 0;
            style.unk1D = 7;
            spawn.unkE = 0x15;
            spawn.unk10 = 1;
            spawn.unk14 = -1;
            motion.unk18 = 0x68;
            motion.unk19 = 0xA;
            motion.unk1A = 0xFF;
            motion.unk14 = -10000.0f;
            motion.unk1C = flame->scatter.unk40;
            motion.unk1E = flame->scatter.unk42;
            do {
                { // matching: a block with a declaration in it steers IDO's scheduling here
                    typedef s32 T15199C34a;

                    spawn.unk15 = (func_150ADA20() % (u32)(flame->scatter.unk18 + 1)) + flame->scatter.unk14;
                    spawn.unkC = (func_150ADA20() % (u32)(flame->scatter.unk1E + 1)) + flame->scatter.unk1C;
                    motion.unk0 = func_150ADA68() * flame->scatter.unk24 + flame->scatter.unk20;
                }
                spawn.pos = pos0;
                delta = pos2;
                delta.x -= spawn.pos.x;
                delta.y -= spawn.pos.y;
                delta.z -= spawn.pos.z;
                if (func_15146078(&delta, &axisA, &axisB) != 0) {
                    speed = func_150ADA68() * flame->scatter.speedRange + flame->scatter.speedMin;
                    func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * flame->scatter.spread, &s, &c);
                    motion.unk4 = (axisA.x * s + axisB.x * c + delta.x) * speed;
                    motion.unk8 = (axisA.y * s + axisB.y * c + delta.y) * speed;
                    motion.unkC = (axisA.z * s + axisB.z * c + delta.z) * speed;
                    motion.unk10 = func_150ADA68() * flame->scatter.unk38 + flame->scatter.unk34;
                    motion.unk1B = func_150ADA68() * flame->scatter.unk3E + flame->scatter.unk3C;
                    func_15147DA0(&spawn, &motion, 0, 1, 0, 0, 0, 0, 0, 0, 0, &style, 0, actor->unkC, actor->unk1);
                }
                flame->scatterAccum -= 1.0f;
            } while (flame->scatterAccum > 1.0f);
        }
    }
    if (obj->unk1D4 != NULL && (obj->unk74 & 0xF) != 0xF) {
        flame->particleAccum += (flame->emit.rate + func_150ADA68() * flame->emit.rateRange) * D_800BE9A4;
        if (flame->particleAccum > 1.0f) {
            FlameEmitParticle part;
            Vec3f delta;
            Vec3f axisA;
            Vec3f axisB;
            f32 c;
            f32 s;
            f32 speed;

            part.unk1D = 0x28;
            part.unk8 = 0x2203;
            part.unk0 = 0x200005;
            part.unk4 = 0x1F0600;
            part.unkC = 0;
            part.unk10 = 0;
            part.unk14 = 0xFF;
            part.unk15 = 0xFF;
            part.unk16 = 0xFF;
            part.unk17 = 0xFF;
            part.unk18 = 0xFF;
            part.unk19 = 0xFF;
            part.unk1A = 0xFF;
            part.pos = pos0;
            part.unk3C.x = 0.0f;
            part.unk3C.y = 0.0f;
            part.unk3C.z = 0.0f;
            part.unk1E = flame->emit.unk38;
            part.unk20 = flame->emit.unk3A;
            part.unk22 = 1;
            part.unk24 = 1.0f;
            part.unk58 = 0xC207;
            part.unk60 = 6;
            part.unk61 = 8;
            part.unk62 = -1;
            part.unk63 = -1;
            part.unk64 = -1;
            part.unk65 = 0;
            do {
                { // matching: a block with a declaration in it steers IDO's scheduling here
                    typedef s32 T15199C34b;

                    part.unkA = (func_150ADA20() % (u32)(flame->emit.unk16 + 1)) + flame->emit.unk14;
                    part.unk1B = (func_150ADA20() % (u32)(flame->emit.unk1A + 1)) + flame->emit.unk18;
                    part.unk28 = part.unk2C = func_150ADA68() * flame->emit.unk20 + flame->emit.unk1C;
                }
                delta = pos3;
                delta.x -= part.pos.x;
                delta.y -= part.pos.y;
                delta.z -= part.pos.z;
                if (func_15146078(&delta, &axisA, &axisB) != 0) {
                    speed = func_150ADA68() * flame->emit.speedRange + flame->emit.speedMin;
                    func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * flame->emit.spread, &s, &c);
                    part.vel.x = (axisA.x * s + axisB.x * c + delta.x) * speed;
                    part.vel.y = (axisA.y * s + axisB.y * c + delta.y) * speed;
                    part.vel.z = (axisA.z * s + axisB.z * c + delta.z) * speed;
                    part.unk54 = func_150ADA68() * flame->emit.unk34 + flame->emit.unk30;
                    func_15130280(&part, 1, 0, 0, actor->unkC, actor->unk1);
                }
                flame->particleAccum -= 1.0f;
            } while (flame->particleAccum > 1.0f);
        }
    }
    flame->timer151 += D_800BE9E4;
    if (flame->timer151 >= 0x3D) {
        if (flame->seed.owner != NULL && flame->seed.owner->unk31C != NULL) {
            flame->seed.owner->unk31C->unk1AA++;
        }
        flame->timer151 = 0;
    }
    if ((flame->flags & 8) && obj->camera != NULL) {
        flame->sparkTimer -= D_800BE9E4;
        if (flame->sparkTimer < 0) {
            s32 pad2;
            FlameSpark spark;

            spark.unk0 = 1;
            spark.unk2 = func_150ADA20() % 21U + 0x14;
            spark.unk5 = 1 << obj->camera->unk23D;
            spark.unk4 = func_150ADA20() % 7U + 2;
            spark.unk6 = -1;
            func_151D8868(&spark, 0, actor->unkC, actor->unk1);
            flame->sparkTimer = func_150ADA20() % 21U + 0x78;
        }
    }
    return 1;
}

/* Spawn description for func_1513D2F0. */
typedef struct LineParticleSpawn {
    u8 texture;
    u8 unk1;
    s16 unk2;
    s16 life;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    Vec3f posA;
    Vec3f posB;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
} LineParticleSpawn;

extern void (*D_8008F8A8[])(void *);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, s32, f32 *, f32 *, f32 *, f32 *, s32, s32, f32);
f32 func_15143E64(Vec3f *);
f32 func_15144528(f32, f32, f32);
void func_15143794(s16, s16, f32, f32 *);

extern s32 D_800A4AA0;

/*
 * Flame shaping: clips the ribbon to maxLength (and to a wall hit when flags & 4), fades the far part,
 * scrolls the texture, sheds line particles near `line.position` of the length, and (flags & 2) calls
 * func_1508B20C at the middle.
 */
s32 func_1519A9A4(CharacterFlamethrowerActor *actor) {
    CharacterFlamethrowerFlame *flame;
    FlamePoint *points;
    f32 maxLen;

    flame = actor->state;
    points = actor->points;
    maxLen = flame->seed.maxLength;
    if ((flame->flags & 4) && actor->count >= 2) {
        struct127 *obj;
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
        if (actor->flags1E & 2) {
            end = &actor->pos;
        } else {
            i = actor->head;
            i--;
            if (i < 0) {
                i = actor->capacity - 1;
            }
            end = &points[i].pos;
        }
        x = obj->x_position;
        y = obj->y_position + 48.0f;
        z = obj->z_position;
        dx = end->x - x;
        dy = end->y - y;
        dz = end->z - z;
        if (func_150AC9C0(x, y, z, dx, dy, dz, 0, 0, &hitPos.x, &hitPos.y,
                          &hitPos.z, &hit, 0, 0, 0.0f) != 0 &&
            hit < maxLen) {
            maxLen = hit;
        }
    }
    if (actor->count >= 2) {
        Vec3f *prev;
        s32 i;
        Vec3f diff;

        i = actor->head;
        flame->length = 0.0f;
        if (actor->flags1E & 2) {
            prev = &actor->pos;
        } else {
            i--;
            if (i < 0) {
                i = actor->capacity - 1;
            }
            prev = &points[i].pos;
        }
        do {
            FlamePoint *p;

            i--;
            if (i < 0) {
                i = actor->capacity - 1;
            }
            p = &points[i];
            diff.x = p->pos.x - prev->x;
            diff.y = p->pos.y - prev->y;
            diff.z = p->pos.z - prev->z;
            p->length = func_15143E64(&diff);
            flame->length += p->length;
            if (maxLen < flame->length) {
                if (p->length) {
                    f32 t = (flame->length - maxLen) / p->length;

                    p->pos.x -= diff.x * t;
                    p->pos.y -= diff.y * t;
                    p->pos.z -= diff.z * t;
                    p->length = p->length * (1.0f - t);
                }
                while (i != actor->tail) {
                    actor->tail++;
                    if (actor->tail == actor->capacity) {
                        actor->tail = 0;
                    }
                    actor->count--;
                }
                flame->length = maxLen;
            }
            prev = &p->pos;
        } while (i != actor->tail);
    }
    if (actor->count >= 2) {
        f32 sum;
        f32 fadeStart;
        f32 fadeLen;
        f32 inv;
        s32 i;

        sum = 0.0f;
        i = actor->head;
        fadeStart = flame->length * flame->seed.fadeStart;
        fadeLen = flame->length - fadeStart;
        inv = 1.0f / fadeLen;
        do {
            FlamePoint *p;

            i--;
            if (i < 0) {
                i = actor->capacity - 1;
            }
            p = &points[i];
            sum += p->length;
            if (fadeStart < sum) {
                s32 pad;

                p->alpha = flame->alpha * ((fadeLen - (sum - fadeStart)) * inv);
            } else {
                p->alpha = flame->seed.alpha;
            }
        } while (i != actor->tail);
    }
    if (actor->count >= 2) {
        s32 i;
        f32 sum;

        flame->texOffset += flame->seed.texSpeed * D_800BE9A4;
        flame->texOffset = func_15144528(flame->texOffset, 16383.0f, -16384.0f);
        i = actor->head;
        sum = 0.0f;
        do {
            FlamePoint *p;

            i--;
            if (i < 0) {
                i = actor->capacity - 1;
            }
            p = &points[i];
            sum += p->length;
            p->texT = func_15144528(flame->texOffset + sum * flame->seed.texScale, 16383.0f, -16384.0f);
        } while (i != actor->tail);
    }
    if (actor->count >= 3 && (!(flame->flags & 0x20) || !(flame->flags & 0x10)) && maxLen <= flame->length) {
        flame->lineAccum += (flame->line.rate + func_150ADA68() * flame->line.rateRange) * points[actor->tail].size * D_800BE9A4;
        if (flame->lineAccum > 1.0f) {
            s32 head;
            s32 next;
            s32 i;
            LineParticleSpawn spawn;
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

            head = actor->tail;
            alpha = 0.0f;
            next = head + 1;
            if (next == actor->capacity) {
                next = 0;
            }
            sum = 0.0f;
            found = 0;
            last = NULL;
            target = flame->length * flame->line.position;
            i = actor->head;
            do {
                FlamePoint *p;

                i--;
                if (i < 0) {
                    i = actor->capacity - 1;
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
            } while (i != actor->tail && found == 0);
            spawn.unk8 = 0;
            spawn.unkC = 0;
            lineData.unk1C = flame->line.unk2C;
            lineData.unk20 = flame->line.unk30;
            lineData.unk24 = flame->line.unk40;
            lineData.unk28 = flame->line.unk34;
            lineData.unk2A = flame->line.unk36;
            spawn.unk1 = 0;
            spawn.unk2 = 0x2203;
            spawn.unk10 = 0xFF;
            spawn.unk11 = 0xFF;
            spawn.unk12 = 0xFF;
            spawn.unk13 = 0xFF;
            spawn.unk34 = 1.0f;
            spawn.unk38 = 0.0f;
            spawn.unk3C = 1.0f;
            spawn.unk40 = 0x40CC05E1;
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

                r1 = func_150ADA68() * flame->line.unk14 + flame->line.unkC;
                r2 = func_150ADA68() * flame->line.unk18 + flame->line.unk10;
                r3 = func_150ADA68() * flame->line.speedRange + flame->line.speedMin;
                spawn.unk14 = 24.22288895f * r1;
                spawn.unk18 = 3.440458536f * r2;
                lineData.velA.x = b->vel.x * r3;
                lineData.velA.y = b->vel.y * r3;
                lineData.velA.z = b->vel.z * r3;
                lineData.velB.x = a->vel.x * r3;
                lineData.velB.y = a->vel.y * r3;
                lineData.velB.z = a->vel.z * r3;
                lineData.unk18 = func_150ADA68() * flame->line.unk28 + flame->line.unk24;
                if (func_150ADA20() & 1) {
                    spawn.texture = 0x13;
                } else {
                    spawn.texture = 0x14;
                }
                spawn.life = func_150ADA20() % (flame->line.lifeRange + 1) + flame->line.lifeMin;
                func_15143794((s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() & 0x7F) - 0x3F), alpha, &vel.x);
                spawn.posA.x = pos.x + vel.x;
                spawn.posA.y = pos.y + vel.y;
                spawn.posA.z = pos.z + vel.z;
                spawn.posB.x = spawn.posA.x + dir.x;
                spawn.posB.y = spawn.posA.y + dir.y;
                spawn.posB.z = spawn.posA.z + dir.z;
                spawn.unk44 = func_150ADA20() % (flame->line.unk3D + 1) + flame->line.unk3C;
                particle = func_1513D2F0(&spawn, &D_800A4AA0, 0x1F, 0x22, 0, 0x1A, ((func_150ADA20() & 1) ? 1 : 0) | 2, 0, 0, 0x2C,
                                    actor->unkC, actor->unk1);
                if (particle != NULL) {
                    memcpy(&particle->data.line, &lineData, sizeof(lineData));
                }
                flame->lineAccum -= 1.0f;
            } while (flame->lineAccum > 1.0f);
        }
    }
    if (flame->seed.unk35 != -1) {
        D_8008F8A8[flame->seed.unk35](actor);
    }
    if ((flame->flags & 2) && actor->count >= 2) {
        Vec3f *end;
        FlamePoint *h;
        s32 i;
        s32 pad9;

        if (actor->flags1E & 2) {
            end = &actor->pos;
        } else {
            i = actor->head - 1;
            if (i < 0) {
                i = actor->capacity - 1;
            }
            end = &points[i].pos;
        }
        h = &points[actor->tail];
        func_1508B20C((h->pos.x + end->x) * 0.5f, (h->pos.y + end->y) * 0.5f,
                      (h->pos.z + end->z) * 0.5f, flame->length * 0.5f);
    }
    return 1;
}

typedef struct RenderView_1519B4B8 {
    u8 pad[0x180];
} RenderView_1519B4B8;

s32 func_15144B34(s32);
s32 func_150A6360(void *, void *, f32, f32, f32, f32, f32, f32);
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_800D9C10[];
extern u8 D_80091064[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;
extern RenderView_1519B4B8 *D_800BE628;

/* Draws a flame as a camera-facing ribbon through its points (culled when off screen). */
Gfx *func_1519B4B8(CharacterFlamethrowerActor *actor, Gfx *gfx, s16 view) {
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

    if (actor->count < 2) {
        return gfx;
    }
    state = actor->state;
    points = actor->points;
    if ((state->flags & 0x20) && (D_800BE616 == 0) && (state->flags & 0x10)) {
        return gfx;
    }
    if (state->flags & 1) {
        s32 tail;
        FlamePoint *first;
        FlamePoint *last;
        f32 dx;
        f32 dy;
        f32 dz;

        tail = actor->head;
        tail--;
        index = actor->tail;
        if (tail < 0) {
            tail = actor->capacity - 1;
        }
        last = &points[index];
        first = &points[tail];
        dx = first->pos.x - last->pos.x;
        dy = first->pos.y - last->pos.y;
        dz = first->pos.z - last->pos.z;
        if (func_150A6360(&D_800BE628[view], D_800D9C10 + view * 0x40,
                          (last->pos.x + first->pos.x) * 0.5f,
                          (last->pos.y + first->pos.y) * 0.5f,
                          (last->pos.z + first->pos.z) * 0.5f,
                          sqrtf(dx * dx + dz * dz), fabsf(dy) + (state->seed.size + state->seed.size), 20000.0f) == 0) {
            return gfx;
        }
    }
    func_151D5D60(actor->buffers, view, (actor->capacity * 2 + 10) * sizeof(Vtx_t), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    camera = (Vec3f *)func_15144B34(view);
    flag = 1;
    gfx = (Gfx *)func_15142E24((s32)gfx, (s32)D_80091064, 0, 0, 0, 0, 0x6B, 0, 0, &flag, 3);
    gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
    gfx = func_1513F4E4(gfx, 0x4C, &flag);
    gfx = func_15142FBC(gfx, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[3][0] | D_800A4AC8[3][1], &flag);
    if (actor->flags1E & 2) {
        index = actor->head;
        index--;
        if (index < 0) {
            index = actor->capacity - 1;
        }
        cur = actor->pos;
        curWidth = state->seed.size;
        curAlpha = state->seed.alpha;
        curTexture = state->texOffset;
    } else {
        previous = actor->head;
        previous--;
        if (previous < 0) {
            previous = actor->capacity - 1;
        }
        index = previous;
        index--;
        if (index < 0) {
            index = actor->capacity - 1;
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
            index = actor->capacity - 1;
        }
        cur = points[previous].pos;
        next = points[index].pos;
        nextWidth = points[index].size;
        nextAlpha = points[index].alpha;
        curTexture = points[previous].texT;
        nextTexture = points[index].texT;
    } while (previous != actor->tail);
    return gfx;
}

/* Integrates a point over `dt` under `gravity` (applied to vel.y). */
void func_1519BE1C(Vec3f *pos, Vec3f *vel, f32 gravity, f32 dt) {
    Vec3f v;

    v = *vel;
    vel->y += gravity * dt;
    pos->x += v.x * dt;
    pos->y += (v.y * dt) + (0.5f * gravity * dt * dt);
    pos->z += v.z * dt;
}

void func_1519BEB8(u8 *arg0) {
    u8 *q;
    u8 *p;
    s32 i;
    s32 found;

    q = *(u8 **)(arg0 + 0x98);
    p = *(u8 **)q;
    i = 0;
    found = 0;
    do {
        if (p[0x3B] == D_800A8A9C[i]) {
            found = 1;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        ((u8 *)D_800E0900[i])[0x14] = 0;
    }
}

void func_1519BF20(u8 *arg0) {
    u8 *q;
    u8 *p;
    s32 i;
    s32 found;

    q = *(u8 **)(arg0 + 0x98);
    p = *(u8 **)q;
    i = 0;
    found = 0;
    do {
        if (p[0x3B] == D_800A8A9C[i]) {
            found = 1;
        } else {
            i++;
        }
    } while (!found && i < 6);

    if (found) {
        ((u8 *)D_800E0900[i])[0x14] = 1;
    }
}

void func_1519BF8C(void) {
    func_10010F30(0x1AA, 0x7FFF, 0x40, 0, 0);
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    char padC[0x28 - 0xC];
} Struct1519BFBCVec;

typedef struct {
    char pad_0x0[0x130];
    u16 field_0x130;
} Struct1519BFBCSound;

typedef struct {
    char pad0[0x2C];
    s8 unk2C;
    s8 unk2D;
    char pad2E[0x94 - 0x2E];
    Struct1519BFBCVec *unk94;
    Struct1519BFBCSound *field_0x98;
} Struct1519BFBC;

void func_1519BFBC(Struct1519BFBC *arg0) {
    Struct1519BFBCVec *temp_v1;
    Struct1519BFBCSound *temp_v2;
    u16 temp_a0;
    s32 temp_t6;
    s32 temp_t7;

    temp_v2 = arg0->field_0x98;
    temp_v1 = arg0->unk94;

    if (arg0->unk2C != 0) {
        temp_a0 = temp_v2->field_0x130;
        if (temp_a0 != 0) {
            temp_t6 = 0x1388;
            temp_t7 = 0x1F4;
            func_1000F91C(temp_a0, 0x7FFF, 0, 0, 0,
                          ((Struct1519BFBCVec *)((u8 *)temp_v1 + (arg0->unk2D * 0x28)))->unk0,
                          ((Struct1519BFBCVec *)((u8 *)temp_v1 + (arg0->unk2D * 0x28)))->unk4,
                          ((Struct1519BFBCVec *)((u8 *)temp_v1 + (arg0->unk2D * 0x28)))->unk8,
                          temp_t7, temp_t6);
        }
    }
}

void func_1519C06C(Struct1519BFBC *arg0) {
    Struct1519BFBCSound *temp = arg0->field_0x98;
    u16 val = temp->field_0x130;
    if (val != 0) {
        func_100111C8(val);
    }
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Struct1519C09CData;

struct225 *func_1519C09C(s32 arg0, f32 *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7) {
    struct225 *temp_v0;
    Header header;
    Header2 header2;
    s32 sp5C;
    Struct1519C09CData data;

    data.unk0 = 35.0f;
    data.unk4 = 5.0f;
    sp5C = arg0;
    header.unk0 = 2;
    header.unk1 = 2;
    header.unk2 = 0x12C;
    header.unk4 = 0x22;
    data.unk8 = 110.0f;
    data.unkC = 1.0f;
    data.unk10 = 0.0f;
    data.unk14 = 7.0f;
    data.unk18 = D_800A8AF8;
    data.unk1C = 127.0f;
    header2.unk0 = arg1[0];
    header2.unk4 = arg1[1];
    header2.unk8 = arg1[2];

    temp_v0 = func_151602C0(&header, &header2, arg2, arg3, arg4, arg5, 0xFF, 0, 0x24, arg6, arg7);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x18, &data, sizeof(data));
        memcpy((u8 *)temp_v0 + 0x38, &sp5C, sizeof(sp5C));
    }
    return temp_v0;
}

void func_1519C258(s32);
void func_151617C4(s32);

void func_1519C200(s32 arg0) {
    func_1519C258(arg0);
    func_151617C4(arg0);
}

void func_151617E4(s32);

void func_1519C22C(s32 arg0) {
    func_1519C258(arg0);
    func_151617E4(arg0);
}

void func_1519C258(s32 arg0) {
    s32 *t = *(s32 **)(*(u32 *)(arg0 + 0x38) + 0x98);

    t[0x52] = 0;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f26C;

typedef struct {
    /* 0x00 */ Vec3f26C vel;
    /* 0x0C */ Vec3f26C avel;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
} Struct1519C26CSub;

typedef struct {
    char pad0[0x34];
    /* 0x34 */ Vec3f26C pos;
    /* 0x40 */ Vec3f26C rot;
} Struct1519C26C;

s32 func_1519C26C(Struct1519C26C *arg0) {
    f32 dtSq;
    Vec3f26C vel;
    Vec3f26C avel;
    Vec3f26C acc;
    Vec3f26C aacc;
    Struct1519C26CSub *p = (Struct1519C26CSub *)((char *)arg0 + 0x110);

    dtSq = D_800BE9A4 * D_800BE9A4;
    vel = p->vel;
    avel = p->avel;
    p->vel.y += p->unk18 * D_800BE9A4;
    p->avel.y += p->unk18 * D_800BE9A4;
    {
        s32 i;

        for (i = D_800BE9E4; i > 0; i--) {
            p->vel.x *= p->unk1C;
            p->vel.y *= p->unk1C;
            p->vel.z *= p->unk1C;
            p->avel.x *= p->unk20;
            p->avel.y *= p->unk20;
            p->avel.z *= p->unk20;
        }
    }
    acc.x = (p->vel.x - vel.x) * D_800BE9A8;
    acc.y = (p->vel.y - vel.y) * D_800BE9A8;
    acc.z = (p->vel.z - vel.z) * D_800BE9A8;
    aacc.x = (p->avel.x - avel.x) * D_800BE9A8;
    aacc.y = (p->avel.y - avel.y) * D_800BE9A8;
    aacc.z = (p->avel.z - avel.z) * D_800BE9A8;
    arg0->pos.x += vel.x * D_800BE9A4 + 0.5f * acc.x * dtSq;
    arg0->pos.y += vel.y * D_800BE9A4 + 0.5f * acc.y * dtSq;
    arg0->pos.z += vel.z * D_800BE9A4 + 0.5f * acc.z * dtSq;
    arg0->rot.x += avel.x * D_800BE9A4 + 0.5f * aacc.x * dtSq;
    arg0->rot.y += avel.y * D_800BE9A4 + 0.5f * aacc.y * dtSq;
    arg0->rot.z += avel.z * D_800BE9A4 + 0.5f * aacc.z * dtSq;
    return 1;
}

typedef struct {
    char pad0[0x24];
    f32 unk24;
    s16 unk28;
    s16 unk2A;
} C4E4Sub;

typedef struct {
    char pad0[0x1C];
    s16 unk1C;
    char pad1E[0x2C - 0x1E];
    f32 unk2C;
    f32 unk30;
    char pad34[0x5C - 0x34];
    u8 unk5C;
} C4E4Struct;

s32 func_1519C4E4(C4E4Struct *arg0) {
    s32 i;
    C4E4Sub *p = (C4E4Sub *)((char *)arg0 + 0x110);

    for (i = D_800BE9E4; i > 0; i--) {
        arg0->unk2C = arg0->unk2C - arg0->unk2C * p->unk24;
        arg0->unk30 = arg0->unk30 - arg0->unk30 * p->unk24;
    }
    if (arg0->unk1C < p->unk28) {
        s32 a1 = arg0->unk1C * p->unk2A;
        if (a1 < arg0->unk5C) {
            arg0->unk5C = a1;
        }
    }
    return 1;
}

typedef struct {
    u8 pad0[0x2C];
    f32 width;
    f32 t;
    Vec3f start;
    Vec3f end;
    u8 pad4C[0x74];
    u8 tmpl[0x40];
    u8 *buffers[4];
} Obj_1519C56C;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 flag;
    u8 pad8[8];
} Vtx_1519C56C;

void func_15145548(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2, Vec3f *arg3, f32 *arg4);

void *func_1519C56C(Obj_1519C56C *arg0, s16 arg1) {
    Vtx_1519C56C *vertices;
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

    func_151D5D60(arg0->buffers, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(arg0->buffers[arg1], arg0->tmpl, 0x40);
            memcpy(arg0->buffers[arg1] + 0x40, arg0->tmpl, 0x40);
        }
    } else {
        return NULL;
    }
    pos = (Vec3f *)func_15144B34(arg1);
    dir.x = arg0->end.x - arg0->start.x;
    dir.y = arg0->end.y - arg0->start.y;
    dir.z = arg0->end.z - arg0->start.z;
    mid.x = arg0->start.x + dir.x * arg0->t;
    mid.y = arg0->start.y + dir.y * arg0->t;
    mid.z = arg0->start.z + dir.z * arg0->t;
    dir.x = mid.x - arg0->start.x;
    dir.y = mid.y - arg0->start.y;
    dir.z = mid.z - arg0->start.z;
    func_15145548(&arg0->start, &dir, pos, &closest, &t);
    dx = closest.x - pos->x;
    dy = closest.y - pos->y;
    dz = closest.z - pos->z;
    nx = dir.y * dz - dy * dir.z;
    ny = dir.z * dx - dz * dir.x;
    nz = dir.x * dy - dx * dir.y;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared != 0.0f) {
        scale = arg0->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    lbl: ;
    } else {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    }
    vertices->x = arg0->start.x + nx;
    vertices->y = arg0->start.y + ny;
    vertices->z = arg0->start.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = arg0->start.x - nx;
    vertices->y = arg0->start.y - ny;
    vertices->z = arg0->start.z - nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = mid.x - nx;
    vertices->y = mid.y - ny;
    vertices->z = mid.z - nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = mid.x + nx;
    vertices->y = mid.y + ny;
    vertices->z = mid.z + nz;
    vertices->flag = 0;
    return result;
}

struct ConkerStruct1519C910_inner {
    char pad0[0x84];
    u16 unk84;
};

struct ConkerStruct1519C910 {
    struct ConkerStruct1519C910_inner *unk0;
    char pad4[0x154];
    s32 unk158;
    s32 unk15C;
};

struct ConkerStruct1519C910Outer {
    char pad0[0x98];
    struct ConkerStruct1519C910 *unk98;
};

s32 func_1519C910(struct ConkerStruct1519C910Outer *arg0) {
    struct ConkerStruct1519C910 *p = arg0->unk98;
    struct ConkerStruct1519C910_inner *q = p->unk0;
    u16 v = q->unk84;

    if (v != p->unk158) {
        if (v != p->unk15C) {
            return 0;
        }
    }
    return 1;
}

s32 func_1519C948(s32 arg0) {
    func_1519CDB0(arg0, 0.25f, 0x1C);
    return 0;
}

s32 func_1519C970(s32 arg0) {
    func_1519CDB0(arg0, 0.5f, 0x1D);
    return 0;
}

s32 func_1519C998(s32 arg0) {
    func_1519CDB0(arg0, 0.2f, 0x1D);
    return 0;
}

struct Desc1519C9C4 {
    s32 unk00;
    void *unk04;
    u8 unk08;
    s8 unk09;
    u8 pad0A[2];
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s8 unk38;
    s8 unk39;
    s8 unk3A;
    s8 unk3B;
    s8 unk3C;
    s8 unk3D;
    u8 unk3E;
    u8 pad3F[1];
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s8 unk4C;
    u8 pad4D[3];
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    s8 unk54;
    u8 pad55[3];
    f32 unk58;
    f32 unk5C;
    s16 unk60;
    s16 unk62;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    s16 unk8C;
    s16 unk8E;
    f32 unk90;
    u8 unk94;
    u8 unk95;
    u8 pad96[2];
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    f32 unkAC;
    s32 unkB0;
    s32 unkB4;
    s16 unkB8;
    s16 unkBA;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    s16 unkD8;
    s16 unkDA;
    s16 unkDC;
    s16 unkDE;
    f32 unkE0;
    f32 unkE4;
    f32 unkE8;
    f32 unkEC;
    f32 unkF0;
    s16 unkF4;
    s16 unkF6;
    s16 unkF8;
    s16 unkFA;
    f32 unkFC;
    f32 unk100;
    f32 unk104;
    f32 unk108;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    s16 unk118;
    s16 unk11A;
};


void func_1519C9C4(u8 *arg0) {
    u8 *ret;
    struct Desc1519C9C4 d;
    u8 *obj;
    s32 pad;
    u8 *sub;

    d.unk04 = obj = *(u8 **)(arg0 + 0x28);
    d.unk08 = obj[0x3B];
    d.unk09 = 9;
    d.unk0C = 7.0f;
    d.unk10 = -147.0f;
    d.unk14 = 54.0f;
    d.unk18 = 7.0f;
    d.unk1C = -228.0f;
    d.unk20 = 80.0f;
    d.unk24 = 0.8700000048f;
    d.unk28 = 0.6970000267f;
    d.unk2C = 703.0f;
    d.unk40 = 0.5600000024f;
    d.unk30 = 8.400000572f;
    d.unk34 = 6.599999905f;
    d.unk3E = 200;
    d.unk38 = -1;
    d.unk39 = 0;
    d.unk3A = -1;
    d.unk3B = 0;
    d.unk3C = 0;
    d.unk3D = 0;
    d.unk44 = -234.0f;
    d.unk48 = 9.579999924f;
    d.unk4C = 0;
    d.unk00 = 15;
    d.unk54 = 15;
    d.unk50 = 60;
    d.unk51 = 255;
    d.unk52 = 235;
    d.unk53 = 82;
    d.unk58 = 0.001949999947f;
    d.unk5C = 0.00699999975f;
    d.unk60 = 20;
    d.unk62 = 13;
    d.unk64 = 0.503000021f;
    d.unk6C = 0.5120000243f;
    d.unk68 = 0.2990000248f;
    d.unk70 = 0.3920000196f;
    d.unk74 = 0.5550000072f;
    d.unk78 = 0.7080000043f;
    d.unk7C = 0.04200000316f;
    d.unk80 = 0.3410000205f;
    d.unk84 = 0.973477006f;
    d.unk88 = 0.9759860039f;
    d.unk8C = 12;
    d.unk8E = 21;
    d.unk90 = 0.4360000193f;
    d.unk94 = 100;
    d.unk95 = 80;
    d.unk98 = 0.006000000052f;
    d.unk9C = 7.0f;
    d.unkA0 = -218.0f;
    d.unkA4 = 150.0f;
    d.unkA8 = 0.2040000111f;
    d.unkAC = 0.4030000269f;
    d.unkB0 = 4;
    d.unkB4 = 3;
    d.unkB8 = 50;
    d.unkBA = 21;
    d.unkBC = 4.0f;
    d.unkC0 = 6.0f;
    d.unkC4 = 0.1000000015f;
    d.unkC8 = 0.1000000015f;
    d.unkCC = 50.0f;
    d.unkD0 = 0.3110000193f;
    d.unkD4 = 0.3080000281f;
    d.unkD8 = 80;
    d.unkDA = 175;
    d.unkDC = 22;
    d.unkDE = 11;
    d.unkE0 = 7.0f;
    d.unkE4 = -218.0f;
    d.unkE8 = 150.0f;
    d.unkEC = 0.05000000075f;
    d.unkF0 = 0.351000011f;
    d.unkF4 = 60;
    d.unkF6 = 20;
    d.unkF8 = 155;
    d.unkFA = 100;
    d.unkFC = 134.0f;
    d.unk100 = 120.0f;
    d.unk104 = 0.05000000075f;
    d.unk108 = 0.06000000238f;
    d.unk10C = 43.0f;
    d.unk110 = -0.25f;
    d.unk114 = 0.1780000031f;
    d.unk118 = 23;
    d.unk11A = 11;
    ret = func_151994B8(8, (CharacterFlamethrowerSetup *)&d, arg0[0xC], arg0[1]);
    if (ret != NULL) {
        sub = *(u8 **)(ret + 0x98);
        memcpy(sub + 0x158, arg0 + 0x30, 8);
    }
}

struct Sub1519CD64 {
    s32 unk0;
    u8 unk4;
    u8 pad5[0x36];
    u8 unk3B;
};

struct Obj1519CD64 {
    u8 padD[0xD];
    u8 unkD;
    s16 unkE;
    u8 pad10[0x18];
    struct Sub1519CD64 *unk28;
    u8 unk2C;
};

void func_1519CD64(struct Obj1519CD64 *a0) {
    struct Sub1519CD64 *v0 = a0->unk28;

    if (v0->unk0 != 0 && v0->unk4 != 0xFF && v0->unk3B == a0->unk2C) {
        return;
    }
    a0->unkE = -1;
    a0->unkD |= 1;
}

typedef struct CFIdentity1519CDB0 {
    char pad0[0x76];
    u16 unk76;
} CFIdentity1519CDB0;

typedef struct CFState1519CDB0 {
    CFIdentity1519CDB0 *identity;
    char pad4[0x148];
    f32 spacing;
} CFState1519CDB0;

typedef struct CFActor1519CDB0 {
    char pad0[0x10];
    f32 position[3];
    char pad1C[0x9];
    u8 point_capacity;
    char pad26[0x6];
    s8 unk2C;
    s8 point_index;
    s8 point_end;
    char pad2F[0x65];
    void *points;
    CFState1519CDB0 *state;
} CFActor1519CDB0;

typedef struct CFTrailPoint1519CDB0 {
    f32 position[3];
    u8 padC[0x10];
    f32 distance;
    u8 pad20[0x8];
} CFTrailPoint1519CDB0;

s32 func_1505D1C4(f32, f32, f32, s32, s32, s32, s32, s32);

void func_1519CDB0(s32 arg0_, f32 arg1, s32 arg2) {
#define arg0 ((CFActor1519CDB0 *)arg0_)
    f32 spacing;
    f32 factor;
    f32 total;
    f32 next;
    u8 *points;
    s32 index;
    CFState1519CDB0 *state;
    CFTrailPoint1519CDB0 *point;
    CFIdentity1519CDB0 *identity;
    f32 *previous;

    state = arg0->state;
    points = arg0->points;
    previous = arg0->position;
    if (arg0->unk2C >= 2) {
        f32 interpolated_z;
        total = 0.0f;
        spacing = state->spacing * arg1;
        index = arg0->point_end;
        next = 0.0f;
        do {
            f32 distance;
            index--;
            if (index < 0) {
                index = arg0->point_capacity - 1;
            }
            point = (CFTrailPoint1519CDB0 *)(index * 0x28 + points);
            distance = point->distance;
            total += distance;
            if (next <= total) {
                identity = state->identity;
                factor = (total - next) / distance;
                func_1505D1C4(((previous[0] - point->position[0]) * factor) + point->position[0],
                    ((previous[1] - point->position[1]) * factor) + point->position[1],
                    (interpolated_z = ((previous[2] - point->position[2]) * factor) + point->position[2]), arg2,
                    ((s32)identity - (s32)D_800CC2D0) / 0x32C,
                    identity->unk76, 0, 0);
                next += spacing;
            }
            previous = point->position;
        } while (index != arg0->point_index);
    }
#undef arg0
}
